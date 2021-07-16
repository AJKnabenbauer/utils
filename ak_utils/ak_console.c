#include "ak_console.h"

#include <windows.h>

bool akConsole_allocate( bool redirectIO, bool enableVTP )
{
	if ( !AllocConsole() )
		return false;

	if ( redirectIO )
		if ( akConsole_redirect_stdIO(true, true, true) != 0 ) return false;
		
	if ( !akConsole_set_enable_VTP( enableVTP ) )
		return false;

	return true;
}

bool akConsole_free( void )
{
	if ( FreeConsole() ) 
		return true;

	// Return true if no console was attached to the calling thread	
	return GetLastError() == ERROR_INVALID_PARAMETER ? true : false;

}

errno_t akConsole_freopen_as_output( FILE* pFile )
{
	FILE* pDF = NULL;
	return freopen_s( &pDF, "CONOUT$", "w", pFile );
}

errno_t akConsole_freopen_as_input( FILE* pFile )
{
	FILE* pDF = NULL;
	return freopen_s( &pDF, "CONIN$", "r", pFile );
}

errno_t akConsole_redirect_stdIO( bool stdIn, bool stdOut, bool stdErr )
{
	errno_t err = 0;
	if ( !err && stdIn )
		err = akConsole_freopen_as_input( stdin );
	if ( !err && stdOut )
		err = akConsole_freopen_as_output( stdout );
	if ( !err && stdErr )
		err = akConsole_freopen_as_output( stderr );
	return err;
}

bool akConsole_set_enable_VTP( bool enabled )
{
	DWORD dwMode = 0;
	HANDLE hStdout = GetStdHandle( STD_OUTPUT_HANDLE );

	if ( hStdout == INVALID_HANDLE_VALUE )
		return false;

	if ( !GetConsoleMode( hStdout, &dwMode ) )
		return false;

	return enabled ?
		SetConsoleMode( hStdout, ( dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING ) ) :
		SetConsoleMode( hStdout, ( dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING ) );
}

//
//HANDLE Console::m_hStdout = INVALID_HANDLE_VALUE;
//HANDLE Console::m_hStdin = INVALID_HANDLE_VALUE;
//HANDLE Console::m_hStderr = INVALID_HANDLE_VALUE;
//char Console::m_VTSeqBuf[128] = { 0 };
//
//unsigned long Console::Allocate()
//{
//	if ( !AllocConsole() )
//		return GetLastError();
//
//	m_hStdout = GetStdHandle( STD_OUTPUT_HANDLE );
//	m_hStdin = GetStdHandle( STD_INPUT_HANDLE );
//	m_hStderr = GetStdHandle( STD_ERROR_HANDLE );
//
//	if ( m_hStdout == INVALID_HANDLE_VALUE || m_hStdin == INVALID_HANDLE_VALUE || m_hStderr == INVALID_HANDLE_VALUE )
//		return GetLastError();
//
//	return 0;
//}
//
//
//int Console::Free()
//{
//	m_hStdout = INVALID_HANDLE_VALUE;
//	m_hStdin = INVALID_HANDLE_VALUE;
//	m_hStdout = INVALID_HANDLE_VALUE;
//	return FreeConsole();
//}
//
//
//int Console::ReopenFileAsOutput( FILE* pFile )
//{
//	FILE* pDF = NULL;
//	return freopen_s( &pDF, CNSL_FNAME_OUTPUT, "w", pFile );
//}
//
//
//int Console::ReopenFileAsInput( FILE* pFile )
//{
//	FILE* pDF = NULL;
//	return freopen_s( &pDF, CNSL_FNAME_INPUT, "r", pFile );
//}
//
//
//int Console::RedirectStdIO( bool stdIn, bool stdOut, bool stdErr )
//{
//	errno_t err = 0;
//
//	if ( !err && stdIn )
//		err = ReopenFileAsInput( stdin );
//
//	if ( !err && stdOut )
//		err = ReopenFileAsOutput( stdout );
//
//	if ( !err && stdErr )
//		err = ReopenFileAsOutput( stderr );
//
//	return err;
//}
//
//
//unsigned long Console::SetVTProcessingEn( bool enabled )
//{
//	DWORD dwMode = 0;
//
//	if ( !GetConsoleMode( m_hStdout, &dwMode ) )
//		return GetLastError();
//
//	BOOL good = enabled ? SetConsoleMode( m_hStdout, ( dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING ) ) :
//		SetConsoleMode( m_hStdout, ( dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING ) );
//
//	return !good ? GetLastError() : 0;
//}
//
//
//unsigned long Console::OutputVTSequence( const char* sPrefix, int iVal, const char* sSuffix )
//{
//	DWORD written = 0;
//	int numChar = snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B%s%d%s", sPrefix, iVal, sSuffix );
//	if ( !WriteConsoleA( m_hStdout, m_VTSeqBuf, numChar, &written, NULL ) )
//		return GetLastError();
//	return 0;
//}
//
//
//unsigned long Console::OutputVTSequence( const char* sPrefix, const char* sVal, const char* sSuffix )
//{
//	DWORD written = 0;
//	int numChar = snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B%s%s%s", sPrefix, sVal, sSuffix );
//	if ( !WriteConsoleA( m_hStdout, m_VTSeqBuf, numChar, &written, NULL ) )
//		return GetLastError();
//	return 0;
//}
//
//
//unsigned long Console::OutputVTSequence( const char* sSeq )
//{
//	DWORD written = 0;
//	if ( sSeq[0] == '\x1B' )
//	{
//		if ( !WriteConsoleA( m_hStdout, sSeq, strlen( sSeq ), &written, NULL ) )
//			return GetLastError();
//	}
//	else
//	{
//		// add terminal escape character if it is missing
//		int numChar = snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B%s", sSeq );
//		if ( !WriteConsoleA( m_hStdout, m_VTSeqBuf, numChar, &written, NULL ) )
//			return GetLastError();
//	}
//	return 0;
//}
//
//
//char* Console::MakeVTSeq( const char* sA, int iB, const char* sC )
//{
//	snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B%s%d%s", sA, iB, sC );
//	return m_VTSeqBuf;
//}
//
//
//char* Console::MakeVTSeq( const char* sA, const char* sB, const char* sC )
//{
//	snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B%s%s%s", sA, sB, sC );
//	return m_VTSeqBuf;
//}
//
//
//char* Console::MakeVTSeq( const char* sA, const char* sB )
//{
//	snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B%s%s", sA, sB );
//	return m_VTSeqBuf;
//}
//
//
//char* Console::MakeVTSeq( const char* sA )
//{
//	sA[0] == '\x1B' ?
//		snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "%s", sA ) :
//		snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B%s", sA );
//	return m_VTSeqBuf;
//}
//
//
//unsigned long Console::SetTxtColors( Color colorFG, Color colorBG )
//{
//	snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B[%dm\x1B[%dm", ( int )colorFG, ( int )colorBG + 10 );
//	return OutputVTSequence( m_VTSeqBuf );
//}
//
//
//char* Console::GetColorVTSeq( Color colorFG, Color colorBG )
//{
//	snprintf( m_VTSeqBuf, sizeof( m_VTSeqBuf ), "\x1B[%dm\x1B[%dm", ( int )colorFG, ( int )colorBG + 10 );
//	return m_VTSeqBuf;
//}



