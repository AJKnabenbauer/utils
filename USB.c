#include "USB.h"

#pragma comment( lib, "WindowsApp.lib" ) // for OpenCommPort()


BOOL usb_event_create( usb_event* event_ptr )
{
	if ( event_ptr ) {
		if ( usb_event_valid( *event_ptr ) ) {
			// If the event pointed to is already valid then we 
			// return false without overriding. 
			return FALSE;
		}

		usb_event temp = CreateEvent( NULL, TRUE, FALSE, NULL );

		if ( usb_event_valid( temp ) ) {
			*event_ptr = temp;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL usb_event_valid( usb_event e )
{
	return ( e == usb_invalid_event ) ? FALSE : TRUE;
}


BOOL usb_event_close( usb_event* event_ptr )
{
	if ( event_ptr ) {
		if ( usb_event_valid( *event_ptr ) ) {
			CloseHandle( *event_ptr );
			*event_ptr = usb_invalid_event;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL usb_event_set( usb_event e )
{
	return usb_event_valid( e ) ?
		SetEvent( e ) : FALSE;
}


BOOL usb_event_reset( usb_event e )
{
	return usb_event_valid( e ) ?
		ResetEvent( e ) : FALSE;
}


usb_wait_result usb_wait_event_single( usb_event e, usb_ulong timeout )
{
	usb_wait_result result = usb_wait_result_failed;

	if ( usb_event_valid( e ) ) {
		// we use the extended version of the function so that we can put the 
		// thread into an alertable state. That way if an APC function or I/O 
		// completion routine is queued by the system, the thread is brought out
		// of a waiting state.

		result = WaitForSingleObjectEx( e, timeout, TRUE );
	}

	return result;
}


usb_wait_result usb_wait_event_multi( usb_ulong* index_ptr, usb_event* ptr, usb_ulong count, usb_ulong timeout, BOOL wait_all )
{
	usb_wait_result result = usb_wait_result_failed;

	if ( ptr != NULL && ( count > 0 && count <= MAXIMUM_WAIT_OBJECTS ) ) {
		// we use the extended version of the function so that we can put the 
		// thread into an alertable state. That way if an APC function or I/O 
		// completion routine is queued by the system, the thread is brought out
		// of a waiting state.

		result = WaitForMultipleObjectsEx( count, ptr, wait_all, timeout, TRUE );

		if ( result >= WAIT_OBJECT_0 && result <= ( WAIT_OBJECT_0 + count - 1 ) ) {
			if ( index_ptr != NULL ) {
				*index_ptr = result - WAIT_OBJECT_0;
			}

			result = usb_wait_result_good;
		}

		else if ( result >= WAIT_ABANDONED_0 && result <= ( WAIT_ABANDONED_0 + count - 1 ) ) {
			// Normally we should never get an abandoned object return code since they are only
			// returned when using mutex objects. However we still check if any objects in the array 
			// were abandoned and assign our return result to usb_wait_result_abandoned. That way aren't
			// returning an enum with a non defined value. 

			result = usb_wait_result_abandoned;
		}
	}

	return result;
}


BOOL usb_event_is_set( usb_event e )
{
	return ( usb_wait_event_single( e, 0 ) == usb_wait_result_good ) ?
		TRUE : FALSE;
}


BOOL usb_event_is_set_any( usb_ulong* index_ptr, usb_event* ptr, usb_ulong count )
{
	return ( usb_wait_event_multi( index_ptr, ptr, count, 0, FALSE ) == usb_wait_result_good ) ?
		TRUE : FALSE;
}


BOOL usb_event_is_set_all( usb_event* ptr, usb_ulong count )
{
	return ( usb_wait_event_multi( NULL, ptr, count, 0, TRUE ) == usb_wait_result_good ) ?
		TRUE : FALSE;
}


BOOL usb_file_create_from_name( usb_file* port_ptr, const WCHAR* file_name )
{
	if ( port_ptr ) {
		if ( usb_file_valid( *port_ptr ) ) {
			// Do not override any valid file pointers
			// 
			// !!IMPORTANT NOTE!!
			// 
			// Since a '0' is considered a valid file handle value, the usb_file object has 
			// to be initialized to 'usb_invalid_file' prior to calling this function.
			//
			// ---------------
			// WRONG:
			// ---------------
			//		usb_file usb;	<- Value defaults to 0 which is a valid file handle
			//		
			//		usb_file_create_from_name ( &usb, L"\\\\.\\COM8" );
			//		
			//		...stuff...
			//
			//		usb_file_close ( &usb );
			//		
			// ---------------
			// RIGHT:
			// ---------------
			//		usb_file usb = usb_invalid_file;	<- Value is properly invalid
			//		
			//		usb_file_create_from_name ( &usb, L"\\\\.\\COM8" );
			//		
			//		...stuff...
			//
			//		usb_file_close ( &usb );
			//		

			return FALSE;
		}

		*port_ptr = CreateFile( file_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );

		if ( usb_file_valid( *port_ptr ) ) {
			return TRUE;
		}
	}

	return FALSE;
}


BOOL usb_file_create_from_num( usb_file* port_ptr, usb_ulong comm_number )
{
	if ( port_ptr ) {
		if ( usb_file_valid( *port_ptr ) ) {
			// Do not override any valid file pointers
			// 
			// !!IMPORTANT NOTE!!
			// 
			// Since a '0' is considered a valid file handle value, the usb_file object has 
			// to be initialized to 'usb_invalid_file' prior to calling this function.
			//
			// ---------------
			// WRONG:
			// ---------------
			//		usb_file usb;	<- Value defaults to 0 which is a valid file handle
			//		
			//		usb_file_create_from_num ( &usb, 8 );
			//		
			//		...stuff...
			//
			//		usb_file_close ( &usb );
			//		
			// ---------------
			// RIGHT:
			// ---------------
			//		usb_file usb = usb_invalid_file;	<- Value is properly invalid
			//		
			//		usb_file_create_from_num ( &usb, 8 );
			//		
			//		...stuff...
			//
			//		usb_file_close ( &usb );
			//		

			return FALSE;
		}

		*port_ptr = OpenCommPort( comm_number, GENERIC_READ | GENERIC_WRITE, FILE_FLAG_OVERLAPPED );

		if ( usb_file_valid( *port_ptr ) ) {
			return TRUE;
		}
	}

	return FALSE;
}


BOOL usb_file_valid( usb_file port_handle )
{
	return ( port_handle == usb_invalid_file ) ? FALSE : TRUE;
}


BOOL usb_file_close( usb_file* port_ptr )
{
	if ( port_ptr ) {
		if ( usb_file_valid( *port_ptr ) ) {
			CloseHandle( *port_ptr );
			*port_ptr = usb_invalid_file;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL usb_line_coding_set( usb_file port_handle, usb_line_coding lc )
{
	if ( usb_file_valid( port_handle ) ) {
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof( dcbSerialParams );

		if ( !GetCommState( port_handle, &dcbSerialParams ) )
			return FALSE;

		dcbSerialParams.BaudRate = lc.baud_rate;
		dcbSerialParams.ByteSize = lc.byte_size;
		dcbSerialParams.StopBits = lc.stop_bits;
		dcbSerialParams.Parity = lc.parity;

		return SetCommState( port_handle, &dcbSerialParams );
	}

	return FALSE;
}

BOOL usb_line_coding_get( usb_file port_handle, usb_line_coding* ptr_lc )
{
	if ( usb_file_valid( port_handle ) && ptr_lc ) {
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof( dcbSerialParams );

		if ( GetCommState( port_handle, &dcbSerialParams ) ) {
			ptr_lc->baud_rate = dcbSerialParams.BaudRate;
			ptr_lc->byte_size = dcbSerialParams.ByteSize;
			ptr_lc->stop_bits = dcbSerialParams.StopBits;
			ptr_lc->parity = dcbSerialParams.Parity;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL usb_comm_event_mask_set( usb_file port_handle, usb_ulong event_mask )
{
	if ( usb_file_valid( port_handle ) ) {
		return SetCommMask( port_handle, event_mask );
	}

	return FALSE;
}

BOOL usb_comm_event_mask_get( usb_file port_handle, usb_ulong* ptr_mask )
{
	if ( usb_file_valid( port_handle ) && ptr_mask ) {
		return GetCommMask( port_handle, ptr_mask );
	}

	return FALSE;
}

BOOL usb_comm_event_wait( usb_file h_file, usb_event h_event, usb_ols* ptr_ol, usb_ulong* ptr_mask )
{
	ptr_ol->hEvent = h_event;

	BOOL status = WaitCommEvent( h_file, ptr_mask, ptr_ol );

	if ( status )
		return TRUE;

	if ( GetLastError() == ERROR_IO_PENDING )
		return TRUE;

	return FALSE;
}

const char* usb_comm_event_get_string( usb_ulong mask )
{
	switch ( mask )
	{
		case EV_BREAK: 
			return "EV_BREAK";
		case EV_CTS: 
			return "EV_CTS";
		case EV_DSR: 
			return "EV_DSR";
		case EV_ERR: 
			return "EV_ERR";
		case EV_RING: 
			return "EV_RING";
		case EV_RLSD: 
			return "EV_RLSD";
		case EV_RXCHAR: 
			return "EV_RXCHAR";
		case EV_RXFLAG: 
			return "EV_RXFLAG";
		case EV_TXEMPTY: 
			return "EV_TXEMPTY";
		default: 
			return "EV_UNKNOWN";
	}
}

const wchar_t* usb_comm_event_get_wstring( usb_ulong mask )
{
	switch ( mask )
	{
		case EV_BREAK:
			return L"EV_BREAK";
		case EV_CTS:
			return L"EV_CTS";
		case EV_DSR:
			return L"EV_DSR";
		case EV_ERR:
			return L"EV_ERR";
		case EV_RING:
			return L"EV_RING";
		case EV_RLSD:
			return L"EV_RLSD";
		case EV_RXCHAR:
			return L"EV_RXCHAR";
		case EV_RXFLAG:
			return L"EV_RXFLAG";
		case EV_TXEMPTY:
			return L"EV_TXEMPTY";
		default:
			return L"EV_UNKNOWN";
	}
}

BOOL usb_timeouts_set( usb_file port_handle, usb_timeouts ct )
{
	if ( usb_file_valid( port_handle ) ) {
		return SetCommTimeouts( port_handle, &ct );
	}

	return FALSE;
}

BOOL usb_timeouts_get( usb_file port_handle, usb_timeouts* ptr_ct )
{
	if ( usb_file_valid( port_handle ) && ptr_ct ) {
		return GetCommTimeouts( port_handle, ptr_ct );
	}

	return FALSE;
}

BOOL usb_begin_read( usb_file h_file, usb_event h_event, usb_ols* ptr_ol, char* data_ptr, usb_ulong num_bytes )
{
	if ( usb_file_valid( h_file ) &&
		usb_event_valid( h_event ) &&
		ptr_ol != NULL &&
		data_ptr != NULL ) {
		ptr_ol->hEvent = h_event;

		BOOL status = ReadFile( h_file, data_ptr, num_bytes, NULL, ptr_ol );

		if ( status )
			return TRUE;
		else if ( GetLastError() == ERROR_IO_PENDING )
			return TRUE;
	}

	return FALSE;
}

BOOL usb_begin_write( usb_file h_file, usb_event h_event, usb_ols* ptr_ol, char* data_ptr, usb_ulong num_bytes )
{
	if ( usb_file_valid( h_file ) &&
		usb_event_valid( h_event ) &&
		ptr_ol != NULL &&
		data_ptr != NULL ) {
		ptr_ol->hEvent = h_event;

		BOOL status = WriteFile( h_file, data_ptr, num_bytes, NULL, ptr_ol );

		if ( status )
			return TRUE;
		else if ( GetLastError() == ERROR_IO_PENDING )
			return TRUE;
	}

	return FALSE;
}

BOOL usb_bytes_from_overlapped( HANDLE file_handle, usb_ols* overlapped, usb_ulong* num_bytes_read )
{
	if ( usb_file_valid( file_handle ) &&
		overlapped != NULL &&
		num_bytes_read != NULL ) {
		BOOL status = GetOverlappedResult( file_handle, overlapped, num_bytes_read, FALSE );

		if ( status )
			return TRUE;
		else if ( GetLastError() == ERROR_IO_PENDING )
			return TRUE;
	}

	return FALSE;
}






//
//
//
//
//void _USB_WinError()
//{
//    TCHAR* errorBuf = NULL;
//    usb_uint error = GetLastError();
//
//    usb_uint len = FormatMessage(
//        FORMAT_MESSAGE_ALLOCATE_BUFFER |			//Allocate enough memory for the entire message
//        FORMAT_MESSAGE_FROM_SYSTEM |				//Search system message table resources
//        FORMAT_MESSAGE_IGNORE_INSERTS,				//Ignore insert sequences
//        NULL,										//Location of the message definition (ignored if NULL)
//        error,										//Error Message ID
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	//Message language ID
//        (LPTSTR)&errorBuf,							//Buffer to recieve message string
//        0,											//Min number of TCHARs to allocate buffer for
//        NULL										//va_list* of insert arguments
//    );
//
//    usb_uint lenBuffer = len + (sizeof(TCHAR) * 128);
//    TCHAR* strBuffer = (TCHAR*)malloc(lenBuffer);
//
//    if (strBuffer != NULL)
//    {
//        swprintf_s(strBuffer, lenBuffer / sizeof(TCHAR), L"Windows Error [%d]: %s \n", error, errorBuf);
//
//        MessageBox(
//            NULL,			    //Handle to partent window
//            strBuffer,          //MessageBox Text
//            NULL,			    //MessageBox Title (NULL = "Error")
//            MB_OK |			    //OK button
//            MB_ICONERROR |	    //Stop sign icon
//            MB_SETFOREGROUND    //Force forground
//        );
//    }
//
//    LocalFree(errorBuf); //FormatMessage uses LOCALALLOC so we have to use LOCALFREE
//    free(strBuffer);
//    //exit(error);
//}
//
//DeviceClassList::DeviceClassList(PCWSTR enumerator, usb_uint flags)
//{
//    usb_uint devIndex =0;
//
//    SP_DEVINFO_DATA devInfoData ={ 0 };
//    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
//
//    m_devInfoSet = SetupDiGetClassDevs(NULL, enumerator, NULL, flags); // Get device info set
//
//    if (m_devInfoSet != INVALID_HANDLE_VALUE) 
//    {
//        // Loop through all devices and get their info
//        while (TRUE == SetupDiEnumDeviceInfo(m_devInfoSet, devIndex, &devInfoData))
//        {
//            devIndex++;
//            m_devInfoDataArray.push_back(devInfoData);
//        }
//    }
//}
//
//DeviceClassList::~DeviceClassList()
//{
//    if (m_dataBuffer) {
//        free(m_dataBuffer);
//        m_dataBuffer = NULL;
//        m_dataBufferSize = 0;
//    }
//
//    if (m_nameBuffer) {
//        free(m_nameBuffer);
//        m_nameBuffer = NULL;
//        m_dataBufferSize = 0;
//    }
//
//    if (m_devInfoSet && m_devInfoSet != INVALID_HANDLE_VALUE)
//        SetupDiDestroyDeviceInfoList(m_devInfoSet);
//}
//
//const BYTE* DeviceClassList::getRegProperty(unsigned int devIndex, usb_uint property, DEVPROPTYPE* propTypePtr, usb_uint* dataSizePtr)
//{
//    usb_uint dwSize = 0;
//    DEVPROPTYPE propType;
//
//    if (devIndex >= m_devInfoDataArray.size()) return NULL;
//
//    BOOL good = SetupDiGetDeviceRegistryProperty(
//        m_devInfoSet,
//        &m_devInfoDataArray[devIndex],
//        property,
//        &propType,
//        m_dataBuffer,
//        m_dataBufferSize,
//        &dwSize);
//
//    while (!good)
//    {
//        usb_uint err = GetLastError();
//
//        if (err == ERROR_INVALID_DATA)
//            return NULL;
//
//        else if (err == ERROR_INSUFFICIENT_BUFFER) {
//            BYTE* temp_ptr = (BYTE*)realloc(m_dataBuffer, dwSize);
//
//            if (temp_ptr == NULL)
//                _USB_WinError();
//
//            m_dataBuffer = temp_ptr;
//            m_dataBufferSize = dwSize;
//
//            good = SetupDiGetDeviceRegistryProperty(
//                m_devInfoSet,
//                &m_devInfoDataArray[devIndex],
//                property,
//                &propType,
//                m_dataBuffer,
//                m_dataBufferSize,
//                &dwSize);
//        }
//
//        else {
//            _USB_WinError();
//            return NULL;
//        }
//    }
//
//    if (propTypePtr) *propTypePtr = propType;
//    if (dataSizePtr) *dataSizePtr = dwSize;
//    return m_dataBuffer;
//}
//
//const BYTE* DeviceClassList::getQueryRegKey(unsigned int devIndex, WCHAR* regValueString, usb_uint* propTypePtr, usb_uint* dataSizePtr)
//{
//    if (devIndex >= m_devInfoDataArray.size()) NULL;
//
//    HKEY hDeviceRegistryKey = SetupDiOpenDevRegKey(
//        m_devInfoSet,
//        &m_devInfoDataArray[devIndex],
//        DICS_FLAG_GLOBAL,
//        0,
//        DIREG_DEV,
//        KEY_READ);
//
//    if (hDeviceRegistryKey == INVALID_HANDLE_VALUE) return NULL;
//
//    usb_uint dwSize = m_dataBufferSize;
//    usb_uint propType = 0;
//
//    usb_uint err = RegQueryValueEx(
//        hDeviceRegistryKey,	// Handle to open registry key
//        regValueString,		// Name of registry Value
//        NULL,				// Reserved, must be NULL
//        &propType,			// Pointer to usb_uint recieving registry type code
//        m_dataBuffer,		// Buffer recieving data
//        &dwSize);			// usb_uint containing size of data buffer
//
//    while (err != ERROR_SUCCESS)
//    {
//        if (err == ERROR_FILE_NOT_FOUND)
//            return NULL;
//
//        else if (err == ERROR_MORE_DATA) {
//            BYTE* temp_ptr = (BYTE*)realloc(m_dataBuffer, dwSize);
//
//            if (temp_ptr == NULL)
//                _USB_WinError();
//
//            m_dataBuffer = temp_ptr;
//            m_dataBufferSize = dwSize;
//
//            err = RegQueryValueEx(
//                hDeviceRegistryKey,
//                regValueString,
//                NULL,
//                &propType,
//                m_dataBuffer,
//                &dwSize);
//        }
//
//        else {
//            _USB_WinError();
//            return NULL;
//        }
//    }
//
//    if (propTypePtr) *propTypePtr = propType;
//    if (dataSizePtr) *dataSizePtr = dwSize;
//    RegCloseKey(hDeviceRegistryKey);
//    return m_dataBuffer;
//}
//
//HKEY DeviceClassList::EnumerateAndPrintRegKeys(unsigned int devIndex)
//{
//    HKEY hDeviceRegistryKey = (HKEY)INVALID_HANDLE_VALUE;
//    usb_uint index = 0;
//    usb_uint type = 0;
//    WCHAR* nameBuffer = NULL;
//    BYTE* dataBuffer = NULL;
//    usb_uint nameBufferSize = 0;
//    usb_uint dataBufferSize = 0;
//    LSTATUS error = ERROR_SUCCESS;
//
//    if (devIndex >= m_devInfoDataArray.size())
//    {
//        return (HKEY)INVALID_HANDLE_VALUE;
//    }
//
//    hDeviceRegistryKey = SetupDiOpenDevRegKey(
//        m_devInfoSet,
//        &m_devInfoDataArray[devIndex],
//        DICS_FLAG_GLOBAL,
//        0,
//        DIREG_DEV,
//        KEY_READ);
//
//    if (hDeviceRegistryKey == INVALID_HANDLE_VALUE)
//    {
//        _USB_WinError();
//        return hDeviceRegistryKey;
//    }
//
//    nameBuffer = (WCHAR*)LocalAlloc(LPTR, 64);
//    dataBuffer = (BYTE*)LocalAlloc(LPTR, 64);
//
//    if (!nameBuffer || !dataBuffer)
//    {
//        _USB_WinError();
//        return hDeviceRegistryKey;
//    }
//
//    while (TRUE)
//    {
//        nameBufferSize = LocalSize(nameBuffer) / sizeof(WCHAR);
//        dataBufferSize = LocalSize(dataBuffer);
//
//        error = RegEnumValue(
//            hDeviceRegistryKey,
//            index,
//            nameBuffer,
//            &nameBufferSize,
//            NULL,
//            &type,
//            dataBuffer,
//            &dataBufferSize);
//
//        if (error == ERROR_NO_MORE_ITEMS)
//            break;
//
//        else if (error == ERROR_MORE_DATA)
//        {
//            if (LocalSize(nameBuffer) < nameBufferSize) {
//                WCHAR* temp_ptr = (WCHAR*)LocalReAlloc(nameBuffer, nameBufferSize, LMEM_MOVEABLE | LMEM_ZEROINIT);
//
//                if (temp_ptr == NULL) {
//                    _USB_WinError();
//                    break;
//                }
//
//                nameBuffer = temp_ptr;
//            }
//
//            if (LocalSize(dataBuffer) < dataBufferSize) {
//                BYTE* temp_ptr = (BYTE*)LocalReAlloc(dataBuffer, dataBufferSize, LMEM_MOVEABLE | LMEM_ZEROINIT);
//
//                if (temp_ptr == NULL) {
//                    _USB_WinError();
//                    break;
//                }
//
//                dataBuffer = temp_ptr;
//            }
//        }
//
//        else if (error == ERROR_SUCCESS)
//        {
//            index++;
//
//            switch (type)
//            {
//                case REG_usb_uint:
//                    wprintf(L"%s: %lu\n", nameBuffer, *(usb_uint*)dataBuffer);
//                    break;
//                case REG_LINK:
//                    wprintf(L"%s: %s\n", nameBuffer, (WCHAR*)dataBuffer);
//                    break;
//                case REG_SZ:
//                    wprintf(L"%s: %s\n", nameBuffer, (WCHAR*)dataBuffer);
//                    break;
//                case REG_EXPAND_SZ:
//                    wprintf(L"%s: %s\n", nameBuffer, (WCHAR*)dataBuffer);
//                    break;
//                case REG_MULTI_SZ:
//                    wprintf(L"%s: %s\n", nameBuffer, combine_MULTI_SZ((WCHAR*)dataBuffer).c_str());
//                    break;
//                case REG_QWORD:
//                    wprintf(L"%s: %llu\n", nameBuffer, *(usb_uint64*)dataBuffer);
//                    break;
//                default:
//                    wprintf(L"%s: UNKNOWN TYPE\n", nameBuffer);
//            }
//        }
//    }
//
//    LocalFree(dataBuffer);
//    LocalFree(nameBuffer);
//    RegCloseKey(hDeviceRegistryKey);
//    return hDeviceRegistryKey;
//
//}
//
//std::wstring DeviceClassList::combine_MULTI_SZ(const WCHAR* ptr)
//{
//    std::wstring str;
//    if (ptr)
//    {
//        while (*ptr)
//        {
//            str.append(ptr);
//            ptr = ptr + _tcslen(ptr) + 1;
//        }
//    }
//    return str;
//}
