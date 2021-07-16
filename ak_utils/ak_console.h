#pragma once

#include <stdio.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

#define CNSL_FCLR_BLACK				"\x1b[30m"
#define CNSL_FCLR_RED				"\x1b[31m"
#define CNSL_FCLR_GREEN				"\x1b[32m"
#define CNSL_FCLR_YELLOW			"\x1b[33m"
#define CNSL_FCLR_BLUE				"\x1b[34m"
#define CNSL_FCLR_MAGENTA			"\x1b[35m"
#define CNSL_FCLR_CYAN				"\x1b[36m"
#define CNSL_FCLR_WHITE				"\x1b[37m"
#define CNSL_FCLR_EXTENDED			"\x1b[38m"
#define CNSL_FCLR_DEFAULT			"\x1b[39m"
#define CNSL_FCLR_BRIGHT_BLACK		"\x1b[90m"
#define CNSL_FCLR_BRIGHT_RED		"\x1b[91m"
#define CNSL_FCLR_BRIGHT_GREEN		"\x1b[92m"
#define CNSL_FCLR_BRIGHT_YELLOW		"\x1b[93m"
#define CNSL_FCLR_BRIGHT_BLUE		"\x1b[94m"
#define CNSL_FCLR_BRIGHT_MAGENTA	"\x1b[95m"
#define CNSL_FCLR_BRIGHT_CYAN		"\x1b[96m"
#define CNSL_FCLR_BRIGHT_WHITE		"\x1b[97m"

#define CNSL_BCLR_BLACK				"\x1b[40m"
#define CNSL_BCLR_RED				"\x1b[41m"
#define CNSL_BCLR_GREEN				"\x1b[42m"
#define CNSL_BCLR_YELLOW			"\x1b[43m"
#define CNSL_BCLR_BLUE				"\x1b[44m"
#define CNSL_BCLR_MAGENTA			"\x1b[45m"
#define CNSL_BCLR_CYAN				"\x1b[46m"
#define CNSL_BCLR_WHITE				"\x1b[47m"
#define CNSL_BCLR_EXTENDED			"\x1b[48m"
#define CNSL_BCLR_DEFAULT			"\x1b[49m"
#define CNSL_BCLR_BRIGHT_BLACK		"\x1b[100m"
#define CNSL_BCLR_BRIGHT_RED		"\x1b[101m"
#define CNSL_BCLR_BRIGHT_GREEN		"\x1b[102m"
#define CNSL_BCLR_BRIGHT_YELLOW		"\x1b[103m"
#define CNSL_BCLR_BRIGHT_BLUE		"\x1b[104m"
#define CNSL_BCLR_BRIGHT_MAGENTA	"\x1b[105m"
#define CNSL_BCLR_BRIGHT_CYAN		"\x1b[106m"
#define CNSL_BCLR_BRIGHT_WHITE		"\x1b[107m"

#define CNSL_TFORMAT_DEFAULT		"\x1b[0m"
#define CNSL_TFORMAT_BOLD_EN		"\x1b[1m"
#define CNSL_TFORMAT_BOLD_DIS		"\x1b[22m"
#define CNSL_TFORMAT_ULINE_EN		"\x1b[4m"
#define CNSL_TFORMAT_ULINE_DIS		"\x1b[24m"
#define CNSL_TFORMAT_NEGATIVE_EN	"\x1b[7m"
#define CNSL_TFORMAT_NEGATIVE_DIS	"\x1b[27m"

#define CNSL_CLEAR_DISPLAY			"\x1b[2J"
#define CNSL_CLEAR_SCROLLB			"\x1b[3J"
#define CNSL_CLEAR_ALL				"\x1B[2J\x1B[3J"

#define CNSL_FCLR_BLACK_W			L"\x1b[30m"
#define CNSL_FCLR_RED_W				L"\x1b[31m"
#define CNSL_FCLR_GREEN_W			L"\x1b[32m"
#define CNSL_FCLR_YELLOW_W			L"\x1b[33m"
#define CNSL_FCLR_BLUE_W			L"\x1b[34m"
#define CNSL_FCLR_MAGENTA_W			L"\x1b[35m"
#define CNSL_FCLR_CYAN_W			L"\x1b[36m"
#define CNSL_FCLR_WHITE_W			L"\x1b[37m"
#define CNSL_FCLR_EXTENDED_W		L"\x1b[38m"
#define CNSL_FCLR_DEFAULT_W			L"\x1b[39m"
#define CNSL_FCLR_BRIGHT_BLACK_W	L"\x1b[90m"
#define CNSL_FCLR_BRIGHT_RED_W		L"\x1b[91m"
#define CNSL_FCLR_BRIGHT_GREEN_W	L"\x1b[92m"
#define CNSL_FCLR_BRIGHT_YELLOW_W	L"\x1b[93m"
#define CNSL_FCLR_BRIGHT_BLUE_W		L"\x1b[94m"
#define CNSL_FCLR_BRIGHT_MAGENTA_W	L"\x1b[95m"
#define CNSL_FCLR_BRIGHT_CYAN_W		L"\x1b[96m"
#define CNSL_FCLR_BRIGHT_WHITE_W	L"\x1b[97m"

#define CNSL_BCLR_BLACK_W			L"\x1b[40m"
#define CNSL_BCLR_RED_W				L"\x1b[41m"
#define CNSL_BCLR_GREEN_W			L"\x1b[42m"
#define CNSL_BCLR_YELLOW_W			L"\x1b[43m"
#define CNSL_BCLR_BLUE_W			L"\x1b[44m"
#define CNSL_BCLR_MAGENTA_W			L"\x1b[45m"
#define CNSL_BCLR_CYAN_W			L"\x1b[46m"
#define CNSL_BCLR_WHITE_W			L"\x1b[47m"
#define CNSL_BCLR_EXTENDED_W		L"\x1b[48m"
#define CNSL_BCLR_DEFAULT_W			L"\x1b[49m"
#define CNSL_BCLR_BRIGHT_BLACK_W	L"\x1b[100m"
#define CNSL_BCLR_BRIGHT_RED_W		L"\x1b[101m"
#define CNSL_BCLR_BRIGHT_GREEN_W	L"\x1b[102m"
#define CNSL_BCLR_BRIGHT_YELLOW_W	L"\x1b[103m"
#define CNSL_BCLR_BRIGHT_BLUE_W		L"\x1b[104m"
#define CNSL_BCLR_BRIGHT_MAGENTA_W	L"\x1b[105m"
#define CNSL_BCLR_BRIGHT_CYAN_W		L"\x1b[106m"
#define CNSL_BCLR_BRIGHT_WHITE_W	L"\x1b[107m"

#define CNSL_TFORMAT_DEFAULT_W		L"\x1b[0m"
#define CNSL_TFORMAT_BOLD_EN_W		L"\x1b[1m"
#define CNSL_TFORMAT_BOLD_DIS_W		L"\x1b[22m"
#define CNSL_TFORMAT_ULINE_EN_W		L"\x1b[4m"
#define CNSL_TFORMAT_ULINE_DIS_W	L"\x1b[24m"
#define CNSL_TFORMAT_NEGATIVE_EN_W	L"\x1b[7m"
#define CNSL_TFORMAT_NEGATIVE_DIS_W	L"\x1b[27m"

#define CNSL_CLEAR_DISPLAY_W		L"\x1b[2J"
#define CNSL_CLEAR_SCROLLB_W		L"\x1b[3J"
#define CNSL_CLEAR_ALL_W			L"\x1B[2J\x1B[3J"



	/**
	 * Allocate a new console for the current thread
	 *
	 * @param  redirectIO True to redirect STDIN, STDOUT, and STDERR to the allocated console
	 * @param  enableVTP  True to enable, false to disable virtual terminal processing of
	 * 					  terminal control sequences
	 *
	 * @returns True if it succeeds, false if it fails.
	 */
	bool akConsole_allocate( bool redirectIO, bool enableVTP );


	/**
	 * Frees the console attached to the calling thread
	 *
	 * @returns True if it succeeds, false if it fails. Note that True is returned if no console is attached
	 * 			to the calling thread.
	 */
	bool akConsole_free( void );


	/**
	 * Closes a file and reassign the stream to the output file of the console attached
	 * to the calling thread.
	 *
	 * @param  pFile  The file stream to reopen.
	 *
	 * @returns 0 if it succeeds, otherwise an error code.
	 */
	errno_t akConsole_freopen_as_output( FILE* pFile );


	/**
	 * Closes a file and reassign the stream to the input file of the console attached
	 * to the calling thread.
	 *
	 * @param  pFile  The file stream to reopen.
	 *
	 * @returns 0 if it succeeds, otherwise an error code.
	 */
	errno_t akConsole_freopen_as_input( FILE* pFile );


	/**
	 * Redirect standard IO file streams to the console attached to the current thread
	 *
	 * @param  stdIn  True to redirect STDIN.
	 * @param  stdOut True to redirect STDOUT.
	 * @param  stdErr True to redirect STDERR.
	 *
	 * @returns 0 if it succeeds, otherwise an error code.
	 */
	errno_t akConsole_redirect_stdIO( bool stdIn, bool stdOut, bool stdErr );


	/**
	 * Enable or disable Virtual Terminal Processing of terminal control sequences for the console
	 * attached to the calling thread
	 *
	 * @param  enabled True to enable, false to disable.
	 *
	 * @returns True if it succeeds, false if it fails.
	 */
	bool akConsole_set_enable_VTP( bool enabled );



	//class Console
	//{
	//	static void* m_hStdout;
	//	static void* m_hStdin;
	//	static void* m_hStderr;
	//	static char m_VTSeqBuf[128];

	//public:

	//	enum class Color
	//	{
	//		BLACK = 30,
	//		RED = 31,
	//		GREEN = 32,
	//		YELLOW = 33,
	//		BLUE = 34,
	//		MAGENTA = 35,
	//		CYAN = 36,
	//		WHITE = 37,
	//		EXTENDED = 38,
	//		DEFAULT = 39,
	//		BRIGHT_BLACK = 90,
	//		BRIGHT_RED = 91,
	//		BRIGHT_GREEN = 92,
	//		BRIGHT_YELLOW = 93,
	//		BRIGHT_BLUE = 94,
	//		BRIGHT_MAGENTA = 95,
	//		BRIGHT_CYAN = 96,
	//		BRIGHT_WHITE = 97
	//	};

	//	static unsigned long Allocate();
	//	static int Free();

	//	static int ReopenFileAsOutput( FILE* pFile );
	//	static int ReopenFileAsInput( FILE* pFile );
	//	static int RedirectStdIO( bool stdIn = true, bool stdOut = true, bool stdErr = true );

	//	static unsigned long SetVTProcessingEn( bool enabled );
	//	static unsigned long OutputVTSequence( const char* sPrefix, int iVal, const char* sSuffix );
	//	static unsigned long OutputVTSequence( const char* sPrefix, const char* sVal, const char* sSuffix );
	//	static unsigned long OutputVTSequence( const char* sSeq );

	//	static char* MakeVTSeq( const char* sA, int iB, const char* sC );
	//	static char* MakeVTSeq( const char* sA, const char* sB, const char* sC );
	//	static char* MakeVTSeq( const char* sA, const char* sB );
	//	static char* MakeVTSeq( const char* sA );

	//	static unsigned long SetTxtColorBkg( Color color )
	//	{
	//		return OutputVTSequence( "[", ( int )color + 10, "m" );
	//	}
	//	static unsigned long SetTxtColorFrg( Color color )
	//	{
	//		return OutputVTSequence( "[", ( int )color, "m" );
	//	}
	//	static unsigned long SetTxtColors( Color colorFG, Color colorBG );

	//	static char* GetColorVTSeqBkg( Color color )
	//	{
	//		return MakeVTSeq( "[", ( int )color + 10, "m" );
	//	}
	//	static char* GetColorVTSeqFrg( Color color )
	//	{
	//		return MakeVTSeq( "[", ( int )color, "m" );
	//	}
	//	static char* GetColorVTSeq( Color colorFG, Color colorBG );

	//	static unsigned long SetTxtBoldEn( bool enabled )
	//	{
	//		return OutputVTSequence( enabled ? CNSL_TFORMAT_BOLD_EN : CNSL_TFORMAT_BOLD_DIS );
	//	}

	//	static unsigned long SetTxtUnderlineEn( bool enabled )
	//	{
	//		return OutputVTSequence( enabled ? CNSL_TFORMAT_ULINE_EN : CNSL_TFORMAT_ULINE_DIS );
	//	}

	//	static unsigned long SetTxtNegativeEn( bool enabled )
	//	{
	//		return OutputVTSequence( enabled ? CNSL_TFORMAT_NEGATIVE_EN : CNSL_TFORMAT_NEGATIVE_DIS );
	//	}

	//	static unsigned long ClearTxtFormat()
	//	{
	//		return OutputVTSequence( CNSL_TFORMAT_DEFAULT );
	//	}

	//	static unsigned long DeleteTxtVisible()
	//	{
	//		return OutputVTSequence( CNSL_CLEAR_DISPLAY );
	//	}
	//	static unsigned long DeleteTxtScrollback()
	//	{
	//		return OutputVTSequence( CNSL_CLEAR_SCROLLB );
	//	}
	//	static unsigned long DeleteTxtAll()
	//	{
	//		return OutputVTSequence( CNSL_CLEAR_ALL );
	//	}
	//};


#ifdef __cplusplus
}
#endif

