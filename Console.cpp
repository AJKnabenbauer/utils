#include "console.h"
#include <windows.h>

namespace cnsl 
{
    HANDLE g_hStdout = INVALID_HANDLE_VALUE;
    HANDLE g_hStdin = INVALID_HANDLE_VALUE;
    HANDLE g_hStderr = INVALID_HANDLE_VALUE;
    static char g_VTSeqBuf[128] ={ 0 }; 


    unsigned long Create()
    {
        if (!AllocConsole()) 
            return GetLastError();

        g_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        g_hStdin = GetStdHandle(STD_INPUT_HANDLE);
        g_hStderr = GetStdHandle(STD_ERROR_HANDLE);

        if (g_hStdout == INVALID_HANDLE_VALUE || g_hStdin == INVALID_HANDLE_VALUE || g_hStderr == INVALID_HANDLE_VALUE)
            return GetLastError();

        return 0;
    }


    int Free()
    {
        g_hStdout = INVALID_HANDLE_VALUE;
        g_hStdin = INVALID_HANDLE_VALUE;
        g_hStdout = INVALID_HANDLE_VALUE;
        return FreeConsole();
    }


    int ReopenFileAsOutput(FILE* pFile) 
    {
        FILE* pDF = NULL;
        return freopen_s(&pDF, CONSOLE_FNAME_OUTPUT, "w", pFile);
    }


    int ReopenFileAsInput(FILE* pFile) 
    {
        FILE* pDF = NULL;
        return freopen_s(&pDF, CONSOLE_FNAME_INPUT, "r", pFile);
    }


    unsigned long SetVTProcessingEn(bool enabled)
    {
        DWORD dwMode = 0;

        if (!GetConsoleMode(g_hStdout, &dwMode))
            return GetLastError();

        BOOL good = enabled ? SetConsoleMode(g_hStdout, (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) :
            SetConsoleMode(g_hStdout, (dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING));

        return !good ? GetLastError() : 0;
    }


    unsigned long OutputVTSequence(const char* sPrefix, int iVal, const char* sSuffix)
    {
        DWORD written = 0;
        int numChar = snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B%s%d%s", sPrefix, iVal, sSuffix);
        if (!WriteConsoleA(g_hStdout, g_VTSeqBuf, numChar, &written, NULL))
            return GetLastError();
        return 0;
    }


    unsigned long OutputVTSequence(const char* sPrefix, const char* sVal, const char* sSuffix)
    {
        DWORD written = 0;
        int numChar = snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B%s%s%s", sPrefix, sVal, sSuffix);
        if (!WriteConsoleA(g_hStdout, g_VTSeqBuf, numChar, &written, NULL))
            return GetLastError();
        return 0;
    }


    unsigned long OutputVTSequence(const char* sSeq)
    {
        DWORD written = 0;
        if (sSeq[0] == '\x1B') {
            if (!WriteConsoleA(g_hStdout, sSeq, strlen(sSeq), &written, NULL))
                return GetLastError();
        }
        else {
            // add terminal escape character if it is missing
            int numChar = snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B%s", sSeq);
            if (!WriteConsoleA(g_hStdout, g_VTSeqBuf, numChar, &written, NULL))
                return GetLastError();
        }
        return 0;
    }


    char* MakeVTSeq(const char* sA, int iB, const char* sC) 
    {
        snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B%s%d%s", sA, iB, sC);
        return g_VTSeqBuf;
    }


    char* MakeVTSeq(const char* sA, const char* sB, const char* sC) 
    {
        snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B%s%s%s", sA, sB, sC);
        return g_VTSeqBuf;
    }


    char* MakeVTSeq(const char* sA, const char* sB) 
    {
        snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B%s%s", sA, sB);
        return g_VTSeqBuf;
    }


    char* MakeVTSeq(const char* sA) 
    {
        sA[0] == '\x1B' ?
            snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "%s", sA) :
            snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B%s", sA);
        return g_VTSeqBuf;
    }


    unsigned long SetTxtColors(Color colorFG, Color colorBG) 
    {
        snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B[%dm\x1B[%dm", (int)colorFG, (int)colorBG + 10);
        return OutputVTSequence(g_VTSeqBuf);
    }


    char* GetColorVTSeq(Color colorFG, Color colorBG) 
    {
        snprintf(g_VTSeqBuf, sizeof(g_VTSeqBuf), "\x1B[%dm\x1B[%dm", (int)colorFG, (int)colorBG + 10);
        return g_VTSeqBuf;
    }


}