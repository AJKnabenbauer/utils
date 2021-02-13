#pragma once

/* newline varies from platform to platform so needs to be defined accordingly */
#if defined _WIN32
#define GS_NEWLINE_T TEXT("\r\n")
#define GS_NEWLINE_A "\r\n"
#define GS_NEWLINE_W L"\r\n"
#elif defined macintosh	/* Mac OS 9 */
#define GS_NEWLINE_T TEXT("\r")
#define GS_NEWLINE_A "\r"
#define GS_NEWLINE_W L"\r"
#else /* Mac OS X and standard unix like systems use \n */
#define GS_NEWLINE_T TEXT("\n")
#define GS_NEWLINE_A "\n"
#define GS_NEWLINE_W L"\n"
#endif 

#define _newl GS_NEWLINE_T /* Platform independent newline character using system character encoding. */
#define _newlA GS_NEWLINE_A /* Platform independent newline character using ASCII character encoding. */
#define _newlW GS_NEWLINE_W /* Platform independent newline character using WIDE character encoding. */

#define _strBool(b)	 ((b) ? TEXT("TRUE") : TEXT("FALSE")) /* Converts a bool to a text string. */
#define _strBoolA(b) ((b) ? "TRUE" : "FALSE") /* Converts a bool to an ASCII string. */
#define _strBoolW(b) ((b) ? L"TRUE" : L"FALSE") /* Converts a bool to a WIDE character string. */

/* If C++ is used then we define the _str macros as quick shorthands for to_string functions */
#ifdef __cplusplus
#include <string>

#ifdef UNICODE
#define _str(x) std::to_wstring(x)
#else
#define _str(x) std::to_string(x)
#endif

#define _strA(x) std::to_string(x)
#define _strW(x) std::to_wstring(x)

#endif







