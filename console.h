#include <stdio.h>

#pragma once

#define CONSOLE_FILEPATH_OUTPUT "CONOUT$"
#define CONSOLE_FILEPATH_INPUT "CONIN$"

#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define CONSOLE_DEFAULT		"\x1b[0m"
#define CONSOLE_BOLD		"\x1b[1m"
#define CONSOLE_UNDERLINE	"\x1b[4m"
#define CONSOLE_NOUNDERLINE	"\x1b[24m"
#define CONSOLE_NEGATIVE	"\x1b[7m"
#define CONSOLE_POSITIVE	"\x1b[27m"

#ifdef __cplusplus			
extern "C" {
#endif

	unsigned long consoleCreate();
	int consoleOutputFile(FILE* pFile);
	int consoleInputFile(FILE* pFile);
	int consoleFree();

#ifdef __cplusplus
}
#endif