#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <stdarg.h>
#include <string.h>
#include <tchar.h>

#include <string>

#include <iostream>
#include <vector>

#include "resource.h"

#include "WIC_ImageViewer.h"

#include "ak_utils/ak_files.h"

#include "ak_utils/ak_usb.h"
#include "ak_utils/ak_console.h"
#include "ak_utils/ak_bitwise.h"

#include "logger.h"

# include <assert.h>

#include <ole2.h> // OLE2 Definitions

// AutoWrap() - Automation helper function...
HRESULT AutoWrap( int autoType, VARIANT* pvResult, IDispatch* pDisp, LPOLESTR ptName, int cArgs... )
{
	// Begin variable-argument list...
	va_list marker;
	va_start( marker, cArgs );

	if ( !pDisp ) {
		MessageBox( NULL, L"NULL IDispatch passed to AutoWrap()", L"Error", 0x10010 );
		_exit( 0 );
	}

	// Variables used...
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID dispidNamed = DISPID_PROPERTYPUT;
	DISPID dispID;
	HRESULT hr;
	char buf[200];
	char szName[200];


	// Convert down to ANSI
	WideCharToMultiByte( CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL );

	// Get DISPID for name passed...
	hr = pDisp->GetIDsOfNames( IID_NULL, &ptName, 1, LOCALE_USER_DEFAULT, &dispID );
	if ( FAILED( hr ) ) {
		//sprintf(buf, "IDispatch::GetIDsOfNames(\"%s\") failed w/err 0x%08lx", szName, hr);
		//MessageBox(NULL, buf, "AutoWrap()", 0x10010);
		_exit( 0 );
		return hr;
	}

	// Allocate memory for arguments...
	VARIANT* pArgs = new VARIANT[cArgs + 1];
	// Extract arguments...
	for ( int i = 0; i < cArgs; i++ ) {
		pArgs[i] = va_arg( marker, VARIANT );
	}

	// Build DISPPARAMS
	dp.cArgs = cArgs;
	dp.rgvarg = pArgs;

	// Handle special-case for property-puts!
	if ( autoType & DISPATCH_PROPERTYPUT ) {
		dp.cNamedArgs = 1;
		dp.rgdispidNamedArgs = &dispidNamed;
	}

	// Make the call!
	hr = pDisp->Invoke( dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, autoType, &dp, pvResult, NULL, NULL );
	if ( FAILED( hr ) ) {
		//sprintf(buf, "IDispatch::Invoke(\"%s\"=%08lx) failed w/err 0x%08lx", szName, dispID, hr);
		//MessageBox(NULL, buf, "AutoWrap()", 0x10010);
		_exit( 0 );
		return hr;
	}
	// End variable-argument section...
	va_end( marker );

	delete[] pArgs;

	return hr;
}


#define BUFFERSIZE 128

#define COMM_PORT_NUMBER 8 
#define COMM_PORT_NAME L"\\\\.\\COM8"

#define ID_EDIT_READ 100
#define ID_EDIT_STATUS 101

#define WM_USB_EVENT_STATUS (WM_APP + 0) // WPARAM = NULL, LPARAM = (usb_struct*) usb data pointer
#define WM_USB_EVENT_RX (WM_APP + 1) // WPARAM = NULL, LPARAM = (usb_struct*) usb data pointer
#define WM_USB_EVENT_TX (WM_APP + 2) // WPARAM = NULL, LPARAM = (usb_struct*) usb data pointer

#define MAIN_WIN_CLASS_NAME L"GenericClassName"
#define MAIN_WIN_TITLE L"Generic Window Title"


typedef struct usb_struct
{
	usb_file file;

	usb_event status_event;
	usb_ols status_ols;
	bool status_in_process;
	usb_ulong status_flags;

	usb_event read_event;
	usb_ols read_ols;
	bool read_in_process;
	char read_buffer[BUFFERSIZE + 1];

	usb_event write_event;
	usb_ols write_ols;
	bool write_in_process;
	char write_buffer[BUFFERSIZE + 1];
} usb_struct;

typedef struct windowStruct
{
	usb_struct* usb_ptr;
	HWND hwnd_parent;
	HWND hwnd_edit_status;
	HWND hwnd_edit_read;
} windowStruct;

int g = 0;

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void* set_window_data_pointer( HWND hWnd, LPARAM lParam );
void* get_window_data_pointer( HWND hWnd );

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
	akConsole_allocate( true, true );

	// Initialize COM for this thread...
	CoInitialize( NULL );

	// Get CLSID for our server...
	CLSID clsid;
	HRESULT hr = CLSIDFromProgID( L"Excel.Application", &clsid );

	if ( FAILED( hr ) ) {

		::MessageBoxA( NULL, "CLSIDFromProgID() failed", "Error", 0x10010 );
		return -1;
	}

	// Start server and get IDispatch...
	IDispatch* pXlApp;
	hr = CoCreateInstance( clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, ( void** )&pXlApp );
	if ( FAILED( hr ) ) {
		::MessageBoxA( NULL, "Excel not registered properly", "Error", 0x10010 );
		return -2;
	}

	WCHAR txt1[] = L"Visible";

	// Make it visible (i.e. app.visible = 1)
	{

		VARIANT x;
		x.vt = VT_I4;
		x.lVal = 1;
		AutoWrap( DISPATCH_PROPERTYPUT, NULL, pXlApp, txt1, 1, x );
	}

	// Get Workbooks collection
	IDispatch* pXlBooks;
	{
		VARIANT result;
		VariantInit( &result );
		AutoWrap( DISPATCH_PROPERTYGET, &result, pXlApp, const_cast < wchar_t* >( L"Workbooks" ), 0 );
		pXlBooks = result.pdispVal;
	}

	// Call Workbooks.Add() to get a new workbook...
	IDispatch* pXlBook;
	{
		VARIANT result;
		VariantInit( &result );
		AutoWrap( DISPATCH_PROPERTYGET, &result, pXlBooks, const_cast < wchar_t* >( L"Add" ), 0 );
		pXlBook = result.pdispVal;
	}

	// Create a 15x15 safearray of variants...
	VARIANT arr;
	arr.vt = VT_ARRAY | VT_VARIANT;
	{
		SAFEARRAYBOUND sab[2];
		sab[0].lLbound = 1; sab[0].cElements = 15;
		sab[1].lLbound = 1; sab[1].cElements = 15;
		arr.parray = SafeArrayCreate( VT_VARIANT, 2, sab );
	}

	// Fill safearray with some values...
	for ( int i = 1; i <= 15; i++ ) {
		for ( int j = 1; j <= 15; j++ ) {
			// Create entry value for (i,j)
			VARIANT tmp;
			tmp.vt = VT_I4;
			tmp.lVal = i * j;
			// Add to safearray...
			long indices[] = { i,j };
			SafeArrayPutElement( arr.parray, indices, ( void* )&tmp );
		}
	}

	// Get ActiveSheet object
	IDispatch* pXlSheet;
	{
		VARIANT result;
		VariantInit( &result );
		AutoWrap( DISPATCH_PROPERTYGET, &result, pXlApp, const_cast < wchar_t* >( L"ActiveSheet" ), 0 );
		pXlSheet = result.pdispVal;
	}

	// Get Range object for the Range A1:O15...
	IDispatch* pXlRange;
	{
		VARIANT parm;
		parm.vt = VT_BSTR;
		parm.bstrVal = ::SysAllocString( L"A1:O15" );

		VARIANT result;
		VariantInit( &result );
		AutoWrap( DISPATCH_PROPERTYGET, &result, pXlSheet, const_cast < wchar_t* >( L"Range" ), 1, parm );
		VariantClear( &parm );

		pXlRange = result.pdispVal;
	}

	// Set range with our safearray...
	AutoWrap( DISPATCH_PROPERTYPUT, NULL, pXlRange, const_cast < wchar_t* >( L"Value" ), 1, arr );

	// Wait for user...
	::MessageBoxA( NULL, "All done.", "Notice", 0x10000 );

	// Set .Saved property of workbook to TRUE so we aren't prompted
	// to save when we tell Excel to quit...
	{
		VARIANT x;
		x.vt = VT_I4;
		x.lVal = 1;
		AutoWrap( DISPATCH_PROPERTYPUT, NULL, pXlBook, const_cast < wchar_t* >( L"Saved" ), 1, x );
	}

	// Tell Excel to quit (i.e. App.Quit)
	AutoWrap( DISPATCH_METHOD, NULL, pXlApp, const_cast < wchar_t* >( L"Quit"), 0 );

	// Release references...
	pXlRange->Release();
	pXlSheet->Release();
	pXlBook->Release();
	pXlBooks->Release();
	pXlApp->Release();
	VariantClear( &arr );




	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = ( HBRUSH )( COLOR_3DFACE + 1 );
	wcex.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );
	wcex.lpszClassName = MAIN_WIN_CLASS_NAME;
	wcex.hIconSm = LoadIcon( NULL, IDI_WINLOGO );

	assert( RegisterClassEx( &wcex ) != 0 );

	windowStruct ws = { 0 };
	HWND hWnd = CreateWindow( MAIN_WIN_CLASS_NAME, MAIN_WIN_TITLE,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480, NULL, NULL, hInstance, &ws );

	assert( hWnd != NULL );

	int i = 0;

	//usb_struct usb = { 0 };
	//usb.file = usb_invalid_file;
	//
	//log_assert( usb_file_create_from_num( &usb.file, 10 ) );
	//log_assert( usb_file_valid( usb.file ) );
	//log_assert( usb_comm_event_mask_set( usb.file, usb_ev_all ) );
	//log_assert( usb_timeouts_set( usb.file, { 1, 0, 0, 1, 10 } ) );
	//log_assert( usb_event_create( &usb.status_event ) );
	//log_assert( usb_event_create( &usb.read_event ) );
	//log_assert( usb_event_create( &usb.write_event ) );
	//
	//ws.usb_ptr = &usb;
	//
	//usb_comm_event_wait( usb.file, usb.status_event, &usb.status_ols, &usb.status_flags );
	//usb.status_in_process = true;

	MSG msg = { 0 };
	while ( msg.message != WM_QUIT ) {
		//usb_ulong index = 0;
		//usb_event event_array[3] = { usb.read_event, usb.write_event, usb.status_event };

		//if ( usb_event_is_set_any( &index, event_array, 3 ) ) {
		//	switch ( index ) {
		//		case 0:
		//			// read event
		//			//usb_event_reset( usb.read_event );
		//			//usb.read_in_process = false;
		//			SendMessage( hWnd, WM_USB_EVENT_RX, NULL, ( LPARAM )&usb );
		//			break;

		//		case 1:
		//			// write event
		//			//usb_event_reset( usb.write_event );
		//			//usb.write_in_process = false;
		//			SendMessage( hWnd, WM_USB_EVENT_TX, NULL, ( LPARAM )&usb );
		//			break;

		//		case 2:
		//			// comm status event
		//			//usb_event_reset( usb.status_event );
		//			//usb.status_in_process = false;
		//			SendMessage( hWnd, WM_USB_EVENT_STATUS, NULL, ( LPARAM )&usb );
		//			break;
		//	}
		//}

		if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	//usb_file_close( &usb.file );
	//usb_event_close( &usb.status_event );
	//usb_event_close( &usb.read_event );
	//usb_event_close( &usb.write_event );
	// 

	   // Uninitialize COM for this thread...
	CoUninitialize();

	akConsole_free();

	return 0;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	windowStruct* pThis = ( uMsg == WM_NCCREATE ) ?
		( windowStruct* )set_window_data_pointer( hWnd, lParam ) :
		( windowStruct* )get_window_data_pointer( hWnd );

	if ( !pThis )
		return DefWindowProc( hWnd, uMsg, wParam, lParam );

	usb_struct* pUSB = pThis->usb_ptr;

	switch ( uMsg ) {
		case WM_CREATE:
		{
			HINSTANCE inst = ( HINSTANCE )GetWindowLongPtr( hWnd, GWLP_HINSTANCE );
			DWORD style_edit = ( WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL );

			pThis->hwnd_parent = hWnd;

			pThis->hwnd_edit_read = CreateWindow( L"EDIT", NULL, style_edit, 0, 0, 0, 0, hWnd, ( HMENU )ID_EDIT_READ, inst, NULL );
			pThis->hwnd_edit_status = CreateWindow( L"EDIT", NULL, style_edit, 0, 0, 0, 0, hWnd, ( HMENU )ID_EDIT_STATUS, inst, NULL );

			assert( pThis->hwnd_edit_read != NULL );
			assert( pThis->hwnd_edit_status != NULL );
		}

		break;


		case WM_SIZE:
		{
			int edit_height = 90;

			WORD width = LOWORD( lParam );
			WORD height = HIWORD( lParam );

			MoveWindow( pThis->hwnd_edit_read, 10, 10, width - 10, edit_height, TRUE );
			MoveWindow( pThis->hwnd_edit_status, 10, 110, width - 10, edit_height, TRUE );
		}

		break;

		case WM_COMMAND:

			switch ( LOWORD( wParam ) ) {
				// Parse the menu selections:
				case ID_FILE_OPEN:
					break;

				case ID_FILE_EXIT:
					PostMessage( hWnd, WM_CLOSE, 0, 0 );
					break;
			}

			break;


		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint( hWnd, &ps );
			// TODO: Add any drawing code that uses hdc here...

			FillRect( hdc, &ps.rcPaint, ( HBRUSH )( COLOR_3DFACE + 1 ) );

			EndPaint( hWnd, &ps );
		}

		return 0;


		case WM_CHAR:
		{
			char key = wParam;


			if ( !pUSB->write_in_process ) {
				pUSB->write_buffer[0] = ( char )wParam;
				usb_begin_write( pUSB->file, pUSB->write_event, &pUSB->write_ols, &pUSB->write_buffer[0], 1 );
				pUSB->write_in_process = true;
			}
		}

		break;

		case WM_USB_EVENT_STATUS:
		{
			usb_struct* pUSB = pThis->usb_ptr;

			if ( pUSB->status_flags & usb_ev_rxchar ) {
				if ( !pUSB->read_in_process ) {
					usb_begin_read( pUSB->file, pUSB->read_event, &pUSB->read_ols, pUSB->read_buffer, BUFFERSIZE );
					pUSB->read_in_process = true;
				}
			}

			usb_comm_event_wait( pUSB->file, pUSB->status_event, &pUSB->status_ols, &pUSB->status_flags );
			pUSB->status_in_process = true;
		}

		return 0;


		case WM_USB_EVENT_RX:
		{
			usb_ulong num_bytes = 0;
			usb_struct* pUSB = pThis->usb_ptr;
			wchar_t edit_bfr[BUFFERSIZE] = { 0 };

			usb_bytes_from_overlapped( pUSB->file, &pUSB->read_ols, &num_bytes );

			MultiByteToWideChar( CP_UTF8, 0, pUSB->read_buffer, num_bytes, edit_bfr, BUFFERSIZE );

			Edit_SetText( pThis->hwnd_edit_read, edit_bfr );
		}

		return 0;


		case WM_USB_EVENT_TX:
		{
			usb_struct* pUSB = pThis->usb_ptr;
		}

		return 0;


		case WM_DESTROY:

			PostQuitMessage( 0 );

			return 0;


		default:

			return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}

void* set_window_data_pointer( HWND hWnd, LPARAM lParam )
{
	LPCREATESTRUCT cs = ( LPCREATESTRUCT )lParam;
	void* pData = ( windowStruct* )cs->lpCreateParams;
	SetWindowLongPtr( hWnd, GWLP_USERDATA, ( LONG_PTR )pData );
	return pData;
}

void* get_window_data_pointer( HWND hWnd )
{
	void* pData = ( void* )GetWindowLongPtr( hWnd, GWLP_USERDATA );
	return pData;
}




//
//void WinError ()
//{
//	TCHAR* errorBuf = NULL;
//	DWORD error = GetLastError ();
//
//	DWORD len = FormatMessage (
//		FORMAT_MESSAGE_ALLOCATE_BUFFER |			//Allocate enough memory for the entire message
//		FORMAT_MESSAGE_FROM_SYSTEM |				//Search system message table resources
//		FORMAT_MESSAGE_IGNORE_INSERTS,				//Ignore insert sequences
//		NULL,										//Location of the message definition (ignored if NULL)
//		error,										//Error Message ID
//		MAKELANGID ( LANG_NEUTRAL, SUBLANG_DEFAULT ),	//Message language ID
//		( LPTSTR )&errorBuf,							//Buffer to recieve message string
//		0,											//Min number of TCHARs to allocate buffer for
//		NULL										//va_list* of insert arguments
//	);
//
//	DWORD lenBuffer = len + ( sizeof ( TCHAR ) * 128 );
//	TCHAR* strBuffer = ( TCHAR* )malloc ( lenBuffer );
//
//	if ( strBuffer != NULL )
//	{
//		swprintf_s ( strBuffer, lenBuffer / sizeof ( TCHAR ), L"Windows Error [%d]: %s \n", error, errorBuf );
//
//		MessageBox (
//			NULL,			    //Handle to partent window
//			strBuffer,          //MessageBox Text
//			NULL,			    //MessageBox Title (NULL = "Error")
//			MB_OK |			    //OK button
//			MB_ICONERROR |	    //Stop sign icon
//			MB_SETFOREGROUND    //Force forground
//		);
//	}
//
//	LocalFree ( errorBuf ); //FormatMessage uses LOCALALLOC so we have to use LOCALFREE
//	free ( strBuffer );
//	//exit(error);
//}
//
//
//

/*

void EnumerateComPort()
{

	HDEVINFO DeviceInfoSet = INVALID_HANDLE_VALUE;
	DWORD DeviceIndex =0;

	SP_DEVINFO_DATA DeviceInfoData ={ 0 };
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);



	//DeviceInfoSet = SetupDiGetClassDevs(
	//    NULL,
	//    L"USB",
	//    NULL,
	//    DIGCF_PRESENT | DIGCF_ALLCLASSES
	//);
	//
	//if (DeviceInfoSet == INVALID_HANDLE_VALUE)
	//{
	//    WinError();
	//    return;
	//}

	// create a list of all connected USB devices
	DeviceClassList devList(L"USB", DIGCF_PRESENT | DIGCF_ALLCLASSES);
	devList.getRegPropType(0);
	//std::wcout << L"USB devices present: " << devList.devInfoDataArray.size() << std::endl;
	//std::wcout << L"############################" << std::endl;

	//DEVPROPTYPE ulPropertyType;
	//BYTE szBuffer[1024] ={ 0 };
	//DWORD dwSize = 0;
	//
	//for (int i = 0; i < devList.devInfoDataArray.size(); i++)
	//{
	//    std::wcout << L"Device Index: " << i << std::endl;
	//
	//    // Hardware ID
	//    BOOL good = SetupDiGetDeviceRegistryProperty(
	//        devList.devInfoSet,
	//        &devList.devInfoDataArray[i],
	//        SPDRP_HARDWAREID,
	//        &ulPropertyType,
	//        (BYTE*)szBuffer,
	//        sizeof(szBuffer),   // The size, in bytes
	//        &dwSize);
	//
	//    std::wcout << L"Hardware ID: ";
	//
	//    if (good)
	//    {
	//        // Walk through segments of the REG_MULTI_SZ
	//        PTSTR ptr = (WCHAR*)szBuffer;
	//        while (*ptr)
	//        {
	//            std::wcout << ptr;
	//            ptr = ptr + _tcslen(ptr) + 1;
	//        }
	//    }
	//
	//    std::wcout << std::endl;
	//}



	//Receive information about an enumerated device
	//while (TRUE == SetupDiEnumDeviceInfo(
	//    DeviceInfoSet, DeviceIndex, &DeviceInfoData))
	//{
	//    DeviceIndex++;
	//
	//
	//
	//    //Retrieves a specified Plug and Play device property
	//    if (SetupDiGetDeviceRegistryProperty(
	//        DeviceInfoSet, &DeviceInfoData,
	//        SPDRP_HARDWAREID,
	//        &ulPropertyType,
	//        (BYTE*)szBuffer, sizeof(szBuffer),
	//        &dwSize))
	//    {
	//        //Get the key
	//        HKEY hDeviceRegistryKey = SetupDiOpenDevRegKey(
	//            DeviceInfoSet,
	//            &DeviceInfoData,
	//            DICS_FLAG_GLOBAL,
	//            0,
	//            DIREG_DEV,
	//            KEY_READ);
	//
	//        if (hDeviceRegistryKey == INVALID_HANDLE_VALUE)
	//        {
	//            Error = GetLastError();
	//            break; //Not able to open registry
	//        }
	//
	//        else
	//        {
	//            // Read in the name of the port
	//            wchar_t pszPortName[BUFF_LEN];
	//            DWORD dwSize = sizeof(pszPortName);
	//            DWORD dwType = 0;
	//            DWORD dwRet = 0;
	//
	//            dwRet = RegQueryValueEx(
	//                hDeviceRegistryKey,     // Handle to open registry key
	//                TEXT("PortName"),       // Name of registry Value
	//                NULL,                   // Reserved, must be NULL
	//                &dwType,                // Pointer to DWORD recieving registry type code
	//                (LPBYTE)pszPortName,    // Buffer recieving data
	//                &dwSize);               // DWORD containing size of data buffer
	//
	//            if (dwRet == ERROR_SUCCESS && dwType == REG_SZ)
	//            {
	//                std::wcout << L"PortName: " << pszPortName << std::endl;
	//            }
	//
	//            else
	//            {
	//                std::wcout << L"PortName: FAILED TO READ" << std::endl;
	//            }
	//
	//            //if ((RegQueryValueEx(hDeviceRegistryKey, L"PortName", NULL, &dwType, (LPBYTE)pszPortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
	//            //{
	//            //    // Check if it really is a com port
	//            //    if (_tcsnicmp(pszPortName, _T("COM"), 3) == 0)
	//            //    {
	//            //        int nPortNr = _ttoi(pszPortName + 3);
	//            //        if (nPortNr != 0)
	//            //        {
	//            //            _tcscpy_s(pszComePort, BUFF_LEN, pszPortName);
	//            //        }
	//            //    }
	//            //}
	//            // Close the key now that we are finished with it
	//            RegCloseKey(hDeviceRegistryKey);
	//        }
	//    }
	//}


	//
	//if (DeviceInfoSet)
	//{
	//    SetupDiDestroyDeviceInfoList(DeviceInfoSet);
	//}
}


*/