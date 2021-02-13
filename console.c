#include "console.h"
#include <windows.h>


unsigned long consoleCreate()
{
    if (!AllocConsole())
    {
        return GetLastError();
    }

    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return GetLastError();
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return GetLastError();
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return GetLastError();
    }

    return 0;
}

int consoleOutputFile(FILE* pFile)
{
    FILE* fp = NULL;
    int err = ERROR_SUCCESS;

    if (!pFile) {
        return ERROR_INVALID_PARAMETER;
    }

    err = freopen_s(&fp, CONSOLE_FILEPATH_OUTPUT, "w", pFile);

    return err;
}

int consoleInputFile(FILE* pFile)
{
    FILE* fp = NULL;
    int err = ERROR_SUCCESS;

    if (!pFile) {
        return ERROR_INVALID_PARAMETER;
    }

    err = freopen_s(&fp, CONSOLE_FILEPATH_INPUT, "r", pFile);

    return err;
}

int consoleFree()
{
	return FreeConsole();
}

