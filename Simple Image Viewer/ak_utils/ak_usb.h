#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define gtest(x) _Generic((x), char: "character", unsigned char: "ucharacter")
#define type_string(v) _Generic((v), int: "int", float: "float")

// 
// Typedefs 
// 

typedef DWORD usb_ulong;
typedef HANDLE usb_file;
typedef HANDLE usb_event;
typedef OVERLAPPED usb_ols;
typedef COMMTIMEOUTS usb_timeouts;

typedef struct
{
	usb_ulong baud_rate;
	usb_ulong byte_size;
	usb_ulong stop_bits;
	usb_ulong parity;
}
usb_line_coding;

const usb_ulong usb_wait_infinite = INFINITE;

const usb_event usb_invalid_event = NULL;
const usb_file usb_invalid_file = INVALID_HANDLE_VALUE;

const usb_ulong usb_ev_break = EV_BREAK;
const usb_ulong usb_ev_cts = EV_CTS;
const usb_ulong usb_ev_dsr = EV_DSR;
const usb_ulong usb_ev_err = EV_ERR;
const usb_ulong usb_ev_ring = EV_RING;
const usb_ulong usb_ev_rlsd = EV_RLSD;
const usb_ulong usb_ev_rxchar = EV_RXCHAR;
const usb_ulong usb_ev_rxflag = EV_RXFLAG;
const usb_ulong usb_ev_txempty = EV_TXEMPTY;
const usb_ulong usb_ev_all = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;


typedef enum
{
	// One or more wait objects has been signaled/completed
	usb_wait_result_good = WAIT_OBJECT_0,

	// Timeout has been reached
	usb_wait_result_timeout = WAIT_TIMEOUT,

	// The wait was ended by one or more user-mode asynchronous procedure calls (APC) queued to the thread.
	usb_wait_result_APC = WAIT_IO_COMPLETION,

	// NOT USED FOR USB EVENTS BUT STILL A POSSIBLE RETURN STATUS OF WIN32 WAIT FUNCTIONS
	// 
	// The specified object is a mutex object that was not released by the thread that owned the mutex object 
	// before the owning thread terminated. Ownership of the mutex object is granted to the calling thread and 
	// the mutex is set to nonsignaled. If the mutex was protecting persistent state information, you should 
	// check it for consistency.
	usb_wait_result_abandoned = WAIT_ABANDONED_0,

	// Wait failed
	usb_wait_result_failed = WAIT_FAILED

} usb_wait_result;


/**
 * Creates a usb event
 *
 * @param  event_ptr Pointer to usb event. Only modified on success. Must not be a valid event.
 *
 * @returns True if it succeeds, false if it fails.
 */
BOOL usb_event_create( usb_event* event_ptr );


/**
 * Checks that a usb event has a valid value
 *
 * @param  e usb_event to check for a valid value.
 *
 * @returns True if the event value is valid, false if the event value is not valid. Note that just
 * 			because an even has a valid event does not mean that it is a handle to win32 event object.
 */
BOOL usb_event_valid( usb_event e );


/**
 * Closes a usb event and sets *event_ptr = usb_event_invalid_value
 *
 * @param  event_ptr If non-null, the event pointer.
 *
 * @returns True if the event is valid, false if it is not.
 */
BOOL usb_event_close( usb_event* event_ptr );
 

/**
 * Sets a usb event to a signaled state
 *
 * @param  e usb_event that should be set to a signaled state.
 *
 * @returns True if it succeeds, false if it fails.
 */
BOOL usb_event_set( usb_event e );


/**
 * Sets a usb_event to a non-signaled state
 *
 * @param  e usb_event that should be set to a non-signaled state.
 *
 * @returns True if it succeeds, false if it fails.
 */
BOOL usb_event_reset( usb_event e );


/**
 * Wait for a single usb event to be signaled. Note that while waiting, the
 * thread is placed into an alertable state so APC function or I/O completion
 * routines are able to bring the thread out of a wait state.
 *
 * @param  e	   usb_event to wait for.
 * @param  timeout time in milliseconds to wait before timeout occurs.
 *
 * @returns usb_wait_result_good - if the event was signaled.
 *			usb_wait_result_timeout - if the timeout elapsed.
 *			usb_wait_result_APC - user-mode APC queued to the thread.
 *			usb_wait_result_failed - if failed.
 */
usb_wait_result usb_wait_event_single( usb_event e, usb_ulong timeout );


/**
 * Wait for multiple usb events to be signaled. Note that while waiting, the
 * thread is placed into an alertable state so APC function or I/O completion
 * routines are able to bring the thread out of a wait state.
 *
 * @param  index_ptr Set to the 0 based index of the event that ended the wait. Ignored if NULL.
 * @param  ptr		 Array of usb_events. Note this function DOES NOT check if all events are valid.
 * @param  count	 Count of usb_events. Must be between 1 and usb_wait_count_max.
 * @param  timeout   The timeout value in milliseconds we should wait for .
 * @param  wait_all  True to wait until all events are signaled. False to wait until any are.
 *
 * @returns usb_wait_result_good - if the events were signaled.
 *			usb_wait_result_timeout - if the timeout elapsed.
 *			usb_wait_result_APC -  user-mode APC queued to the thread.
 *			usb_wait_result_failed -  if failed.
 */
usb_wait_result usb_wait_event_multi( usb_ulong* index_ptr, usb_event* ptr, usb_ulong count, usb_ulong timeout, BOOL wait_all );


/**
 * Checks if a usb_event is set to a signaled state
 *
 * @param  e usb_event to check.
 *
 * @returns True if event is in a signaled state, false if it is non-signaled.
 */
BOOL usb_event_is_set( usb_event e );


/**
 * Checks if one or more usb events, in an array, are in a signaled state
 *
 * @param  index_ptr Set to the 0 based index of the event that ended the wait. Ignored if NULL.
 * @param  ptr		 Array of usb_events. Note this function DOES NOT check if all events are valid.
 * @param  count	 Count of usb_events. Must be between 1 and usb_wait_count_max.
 *
 * @returns True if one or more event is signaled, false if no events are signaled.
 */
BOOL usb_event_is_set_any( usb_ulong* index_ptr, usb_event* ptr, usb_ulong count );


/**
 * Checks if all usb events, in an array, are in a signaled state
 *
 * @param  ptr   Array of usb_events. Note this function DOES NOT check if all events are valid.
 * @param  count Count of usb_events. Must be between 1 and usb_wait_count_max.
 *
 * @returns True if it succeeds, false if it fails.
 */
BOOL usb_event_is_set_all( usb_event* ptr, usb_ulong count );





//
// Win32 file object related
// 


BOOL usb_file_create_from_name( usb_file* port_ptr, const WCHAR* file_name );


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief USB file create from  number
/// 	   sdfgsaf
///
/// @param [in,out] port_ptr    If non-null, the port pointer.
/// @param 		    comm_number The communications number.
///
/// @returns True if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL usb_file_create_from_num( usb_file* port_ptr, usb_ulong comm_number );

BOOL usb_file_valid( usb_file port_handle );

BOOL usb_file_close( usb_file* port_ptr );

//
// Line coding functions
// 

BOOL usb_line_coding_set( usb_file port_handle, usb_line_coding lc );


BOOL usb_line_coding_get( usb_file port_handle, usb_line_coding* lc );

//
// Communication event functions
// 

BOOL usb_comm_event_mask_set( usb_file port_handle, usb_ulong event_mask );

BOOL usb_comm_event_mask_get( usb_file port_handle, usb_ulong* ptr_mask );

BOOL usb_comm_event_wait( usb_file h_file, usb_event h_event, usb_ols* ptr_ol, usb_ulong* ptr_mask );

const char* usb_comm_event_get_string( usb_ulong mask );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <brief> USB communications event get string w </brief>
///
/// <param name="mask"> The mask. </param>
///
/// <returns> Null if it fails, else a pointer to a const wchar_t. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t* usb_comm_event_get_wstring( usb_ulong mask );

//
// Read/Write timeout functions
// 

BOOL usb_timeouts_set( usb_file port_handle, usb_timeouts ct );

BOOL usb_timeouts_get( usb_file port_handle, usb_timeouts* ptr_ct );

// 
// Reading and writing functions
// 

BOOL usb_begin_read( usb_file h_file, usb_event h_event, usb_ols* ptr_ol, char* data_ptr, usb_ulong num_bytes );

BOOL usb_begin_write( usb_file h_file, usb_event h_event, usb_ols* ptr_ol, char* data_ptr, usb_ulong num_bytes );

BOOL usb_bytes_from_overlapped( HANDLE file_handle, usb_ols* overlapped, usb_ulong* num_bytes_read );


#ifdef __cplusplus
}
#endif




//
//
//class DeviceClassList;
//class usbCom;
//class IO_Request;
//
//
//
//class DeviceClassList
//{
//private:
//
//	BYTE* m_dataBuffer = NULL;
//	usb_uint m_dataBufferSize = 0;
//
//	WCHAR* m_nameBuffer = NULL;
//	usb_uint m_nameBufferSize = 0;
//
//public:
//
//	HDEVINFO m_devInfoSet = INVALID_HANDLE_VALUE;
//	std::vector<SP_DEVINFO_DATA> m_devInfoDataArray;
//
//
//	DeviceClassList ( PCWSTR enumerator, usb_uint flags );
//
//	~DeviceClassList ();
//
//	const BYTE* getRegProperty (
//		unsigned int devIndex,
//		usb_uint property,
//		DEVPROPTYPE* propTypePtr = NULL,
//		usb_uint* dataSizePtr = NULL );
//
//
//	const BYTE* getQueryRegKey (
//		unsigned int devIndex,
//		WCHAR* regValueString,
//		usb_uint* propTypePtr = NULL,
//		usb_uint* dataSizePtr = NULL );
//
//
//	HKEY EnumerateAndPrintRegKeys ( unsigned int devIndex );
//
//	static std::wstring combine_MULTI_SZ ( const WCHAR* ptr );
//
//	const WCHAR* getDevStr_ClassName ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_CLASS );
//	}
//
//	const WCHAR* getDevStr_ClassGUI ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_CLASSGUID );
//	}
//
//	const WCHAR* getDevStr_CompatIDs ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_COMPATIBLEIDS );
//	}
//
//	const WCHAR* getDevStr_Description ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_DEVICEDESC );
//	}
//
//	const WCHAR* getDevStr_DriverKey ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_DRIVER );
//	}
//
//	const WCHAR* getDevStr_EnumName ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_ENUMERATOR_NAME );
//	}
//
//	const WCHAR* getDevStr_FriendlyName ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_FRIENDLYNAME );
//	}
//
//	const WCHAR* getDevStr_HardwareID ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_HARDWAREID );
//	}
//
//	const WCHAR* getDevStr_LocInfo ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_LOCATION_INFORMATION );
//	}
//
//	const WCHAR* getDevStr_LocPath ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_LOCATION_PATHS );
//	}
//
//	const WCHAR* getDevStr_MFGName ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_MFG );
//	}
//
//	const WCHAR* getDevStr_PDOName ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME );
//	}
//
//	const WCHAR* getDevStr_SDS ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_SECURITY_SDS );
//	}
//
//	const WCHAR* getDevStr_ServiceName ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_SERVICE );
//	}
//
//	const WCHAR* getDevStr_UINumber ( usb_uint devIndex )
//	{
//		return ( WCHAR* )getRegProperty ( devIndex, SPDRP_UI_NUMBER_DESC_FORMAT );
//	}
//
//};
//
//

