#include "USB.h"

#pragma comment( lib, "WindowsApp.lib" ) // for OpenCommPort()


bool usb_event_is_valid ( usb_event event_handle )
{
	return ( event_handle == usb_event_invalid_value ) ? false : true;
}

bool usb_event_create ( usb_event* event_ptr )
{
	if ( event_ptr )
	{
		usb_event temp = CreateEvent ( NULL, TRUE, FALSE, NULL );

		if ( usb_event_is_valid ( temp ) )
		{
			*event_ptr = temp;
			return true;
		}
	}

	return false;
}

bool usb_event_close ( usb_event* event_ptr )
{
	if ( event_ptr )
	{
		if ( usb_event_is_valid ( *event_ptr ) )
		{
			CloseHandle ( *event_ptr );
			*event_ptr = usb_event_invalid_value;
			return true;
		}
	}

	return false;
}

bool usb_event_set ( usb_event event_handle )
{
	return usb_event_is_valid ( event_handle ) ? SetEvent ( event_handle ) : false;
}

bool usb_event_reset ( usb_event event_handle )
{
	return usb_event_is_valid ( event_handle ) ? ResetEvent ( event_handle ) : false;
}

usb_uint usb_event_wait ( usb_event event_handle, usb_uint milliseconds )
{
	return usb_event_is_valid ( event_handle ) ? WaitForSingleObject ( event_handle, milliseconds ) : usb_wait_failed;
}

bool usb_event_signaled ( usb_event event_handle )
{
	return usb_event_wait ( event_handle, 0 ) == usb_wait_good ? true : false;
}

bool usb_file_is_valid ( usb_file port_handle )
{
	return ( port_handle == usb_file_invalid_value ) ? false : true;
}

bool usb_file_create_from_name ( usb_file* port_ptr, const WCHAR* file_name )
{
	if ( port_ptr )
	{
		usb_file temp = CreateFile (file_name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL );

		if ( usb_file_is_valid ( temp ) )
		{
			*port_ptr = temp;
			return true;
		}
	}

	return false;
}

bool usb_file_create_from_num ( usb_file* port_ptr, usb_uint comm_number )
{
	if ( port_ptr )
	{
		usb_file temp = OpenCommPort ( comm_number, GENERIC_READ | GENERIC_WRITE, FILE_FLAG_OVERLAPPED );

		if ( usb_file_is_valid ( temp ) )
		{
			*port_ptr = temp;
			return true;
		}
	}

	return false;
}

bool usb_file_close ( usb_file* port_ptr )
{
	if ( port_ptr )
	{
		if ( usb_file_is_valid ( *port_ptr ) )
		{
			CloseHandle ( *port_ptr );
			*port_ptr = usb_file_invalid_value;
			return true;
		}
	}

	return false;
}

bool usb_line_coding_set ( usb_file port_handle, usb_line_coding lc )
{
	if ( usb_file_is_valid ( port_handle ) )
	{
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof ( dcbSerialParams );

		if ( !GetCommState ( port_handle, &dcbSerialParams ) )
			return false;

		dcbSerialParams.BaudRate = lc.baud_rate;
		dcbSerialParams.ByteSize = lc.byte_size;
		dcbSerialParams.StopBits = lc.stop_bits;
		dcbSerialParams.Parity = lc.parity;

		return SetCommState ( port_handle, &dcbSerialParams );
	}

	return false;
}

bool usb_line_coding_get ( usb_file port_handle, usb_line_coding* ptr_lc )
{
	if ( usb_file_is_valid ( port_handle ) && ptr_lc )
	{
		DCB dcbSerialParams = { 0 };
		dcbSerialParams.DCBlength = sizeof ( dcbSerialParams );

		if ( GetCommState ( port_handle, &dcbSerialParams ) )
		{
			ptr_lc->baud_rate = dcbSerialParams.BaudRate;
			ptr_lc->byte_size = dcbSerialParams.ByteSize;
			ptr_lc->stop_bits = dcbSerialParams.StopBits;
			ptr_lc->parity = dcbSerialParams.Parity;
			return true;
		}
	}

	return false;
}

bool usb_comm_event_mask_set ( usb_file port_handle, usb_uint event_mask )
{
	if ( usb_file_is_valid ( port_handle ) )
	{
		return SetCommMask ( port_handle, event_mask );
	}

	return false;
}

bool usb_comm_event_mask_get ( usb_file port_handle, usb_uint* ptr_mask )
{
	if ( usb_file_is_valid ( port_handle ) && ptr_mask )
	{
		return GetCommMask ( port_handle, ptr_mask );
	}

	return false;
}

bool usb_comm_event_wait ( usb_file h_file, usb_event h_event, OVERLAPPED* ptr_ol, usb_uint* ptr_mask )
{
	ptr_ol->hEvent = h_event;

	BOOL status = WaitCommEvent ( h_file, ptr_mask, ptr_ol );

	if ( status )
		return true;

	if ( GetLastError () == ERROR_IO_PENDING )
		return true;

	return false;
}

const char* usb_comm_event_get_string ( usb_uint mask )
{
	if ( mask & EV_BREAK ) return "EV_BREAK ";
	if ( mask & EV_CTS ) return "EV_CTS ";
	if ( mask & EV_DSR ) return "EV_DSR ";
	if ( mask & EV_ERR ) return "EV_ERR ";
	if ( mask & EV_RING ) return "EV_RING ";
	if ( mask & EV_RLSD ) return "EV_RLSD ";
	if ( mask & EV_RXCHAR ) return "EV_RXCHAR ";
	if ( mask & EV_RXFLAG ) return "EV_RXFLAG ";
	if ( mask & EV_TXEMPTY ) return "EV_TXEMPTY ";
	return "Unknown";
}

const wchar_t* usb_comm_event_get_stringW ( usb_uint mask )
{
	if ( mask & EV_BREAK ) return L"EV_BREAK ";
	if ( mask & EV_CTS ) return L"EV_CTS ";
	if ( mask & EV_DSR ) return L"EV_DSR ";
	if ( mask & EV_ERR ) return L"EV_ERR ";
	if ( mask & EV_RING ) return L"EV_RING ";
	if ( mask & EV_RLSD ) return L"EV_RLSD ";
	if ( mask & EV_RXCHAR ) return L"EV_RXCHAR ";
	if ( mask & EV_RXFLAG ) return L"EV_RXFLAG ";
	if ( mask & EV_TXEMPTY ) return L"EV_TXEMPTY ";
	return L"Unknown";
}

bool usb_timeouts_set ( usb_file port_handle, usb_timeouts ct )
{
	if ( usb_file_is_valid ( port_handle ) )
	{
		return SetCommTimeouts ( port_handle, &ct );
	}

	return false;
}

bool usb_timeouts_get ( usb_file port_handle, usb_timeouts* ptr_ct )
{
	if ( usb_file_is_valid ( port_handle ) && ptr_ct )
	{
		return GetCommTimeouts ( port_handle, ptr_ct );
	}

	return false;
}

bool usb_begin_read ( usb_file h_file, usb_event h_event, OVERLAPPED* ptr_ol, char* data_ptr, usb_uint num_bytes )
{
	if ( usb_file_is_valid ( h_file ) &&
		usb_event_is_valid ( h_event ) &&
		ptr_ol != NULL &&
		data_ptr != NULL )
	{
		ptr_ol->hEvent = h_event;

		BOOL status = ReadFile ( h_file, data_ptr, num_bytes, NULL, ptr_ol );

		if ( status )
			return true;
		else if ( GetLastError () == ERROR_IO_PENDING )
			return true;
	}

	return false;
}

bool usb_begin_write ( usb_file h_file, usb_event h_event, OVERLAPPED* ptr_ol, char* data_ptr, usb_uint num_bytes )
{
	if ( usb_file_is_valid ( h_file ) && 
		usb_event_is_valid ( h_event ) && 
		ptr_ol != NULL && 
		data_ptr != NULL )
	{
		ptr_ol->hEvent = h_event;
		
		BOOL status = WriteFile ( h_file, data_ptr, num_bytes, NULL, ptr_ol );

		if ( status )
			return true;
		else if ( GetLastError () == ERROR_IO_PENDING )
			return true;
	}

	return false;
}

bool usb_bytes_from_overlapped ( HANDLE file_handle, OVERLAPPED* overlapped, usb_uint* num_bytes_read )
{
	if ( usb_file_is_valid ( file_handle ) && 
		overlapped != NULL && 
		num_bytes_read != NULL )
	{
		BOOL status = GetOverlappedResult ( file_handle, overlapped, num_bytes_read, FALSE );

		if ( status ) 
			return true;
		else if ( GetLastError () == ERROR_IO_PENDING ) 
			return true;
	}

	return false;
}

