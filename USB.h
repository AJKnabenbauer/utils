#pragma once
#include <windows.h>

// 
// Typedefs 
// 

typedef unsigned long usb_uint;

typedef HANDLE usb_file;

typedef HANDLE usb_event;

typedef COMMTIMEOUTS usb_timeouts;

typedef struct
{
	usb_uint baud_rate;
	usb_uint byte_size;
	usb_uint stop_bits;
	usb_uint parity;
}
usb_line_coding;

// 
// Global constants
// 

const usb_event usb_event_invalid_value = NULL;

const usb_file usb_file_invalid_value = INVALID_HANDLE_VALUE;

const usb_uint usb_wait_good = WAIT_OBJECT_0;

const usb_uint usb_wait_timeout = WAIT_TIMEOUT;

const usb_uint usb_wait_abandoned = WAIT_ABANDONED;

const usb_uint usb_wait_failed = WAIT_FAILED;

const usb_uint usb_comm_event_all = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;

const usb_uint usb_comm_event_break = EV_BREAK;

const usb_uint usb_comm_event_cts = EV_CTS;

const usb_uint usb_comm_event_dsr = EV_DSR;

const usb_uint usb_comm_event_err = EV_ERR;

const usb_uint usb_comm_event_ring = EV_RING;

const usb_uint usb_comm_event_rlsd = EV_RLSD;

const usb_uint usb_comm_event_rxchar = EV_RXCHAR;

const usb_uint usb_comm_event_rxflag = EV_RXFLAG;

const usb_uint usb_comm_event_txempty = EV_TXEMPTY;

//
// Win32 event object functions
//

bool usb_event_is_valid ( usb_event event_handle );

bool usb_event_create ( usb_event* event_ptr );

bool usb_event_close ( usb_event* event_ptr );

bool usb_event_set ( usb_event event_handle );

bool usb_event_reset ( usb_event event_handle );

usb_uint usb_event_wait ( usb_event event_handle, usb_uint milliseconds );

bool usb_event_signaled ( usb_event event_handle );

//
// Win32 file object related
// 

bool usb_file_is_valid ( usb_file port_handle );

bool usb_file_create_from_name ( usb_file* port_ptr, const WCHAR* file_name );

bool usb_file_create_from_num ( usb_file* port_ptr, usb_uint comm_number );

bool usb_file_close ( usb_file* port_ptr );

//
// Line coding functions
// 

bool usb_line_coding_set ( usb_file port_handle, usb_line_coding lc );

bool usb_line_coding_get ( usb_file port_handle, usb_line_coding* lc );

//
// Communication event functions
// 

bool usb_comm_event_mask_set ( usb_file port_handle, usb_uint event_mask );

bool usb_comm_event_mask_get ( usb_file port_handle, usb_uint* ptr_mask );

bool usb_comm_event_wait ( usb_file h_file, usb_event h_event, OVERLAPPED* ptr_ol, usb_uint* ptr_mask );

const char* usb_comm_event_get_string ( usb_uint mask );

const wchar_t* usb_comm_event_get_stringW ( usb_uint mask );

//
// Read/Write timeout functions
// 

bool usb_timeouts_set ( usb_file port_handle, usb_timeouts ct );

bool usb_timeouts_get ( usb_file port_handle, usb_timeouts* ptr_ct );

// 
// Reading and writing functions
// 

bool usb_begin_read ( usb_file h_file, usb_event h_event, OVERLAPPED* ptr_ol, char* data_ptr, usb_uint num_bytes );

bool usb_begin_write ( usb_file h_file, usb_event h_event, OVERLAPPED* ptr_ol, char* data_ptr, usb_uint num_bytes );

bool usb_bytes_from_overlapped ( HANDLE file_handle, OVERLAPPED* overlapped, usb_uint* num_bytes_read );
