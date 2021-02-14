#include <stdio.h>

#pragma once

#define CONSOLE_FNAME_OUTPUT "CONOUT$"
#define CONSOLE_FNAME_INPUT "CONIN$"

#define CNSL_COLOR_FRG_BLACK	  "\x1b[30m"
#define CNSL_COLOR_FRG_RED		  "\x1b[31m"
#define CNSL_COLOR_FRG_GREEN	  "\x1b[32m"
#define CNSL_COLOR_FRG_YELLOW	  "\x1b[33m"
#define CNSL_COLOR_FRG_BLUE		  "\x1b[34m"
#define CNSL_COLOR_FRG_MAGENTA	  "\x1b[35m"
#define CNSL_COLOR_FRG_CYAN		  "\x1b[36m"
#define CNSL_COLOR_FRG_WHITE	  "\x1b[37m"
#define CNSL_COLOR_FRG_EXTENDED	  "\x1b[38m"
#define CNSL_COLOR_FRG_DEFAULT	  "\x1b[39m"
#define CNSL_COLOR_FRG_BLACK_B	  "\x1b[90m"
#define CNSL_COLOR_FRG_RED_B	  "\x1b[91m"
#define CNSL_COLOR_FRG_GREEN_B	  "\x1b[92m"
#define CNSL_COLOR_FRG_YELLOW_B	  "\x1b[93m"
#define CNSL_COLOR_FRG_BLUE_B	  "\x1b[94m"
#define CNSL_COLOR_FRG_MAGENTA_B  "\x1b[95m"
#define CNSL_COLOR_FRG_CYAN_B	  "\x1b[96m"
#define CNSL_COLOR_FRG_WHITE_B	  "\x1b[97m"
#define CNSL_COLOR_BKG_BLACK	  "\x1b[40m"
#define CNSL_COLOR_BKG_RED		  "\x1b[41m"
#define CNSL_COLOR_BKG_GREEN	  "\x1b[42m"
#define CNSL_COLOR_BKG_YELLOW	  "\x1b[43m"
#define CNSL_COLOR_BKG_BLUE		  "\x1b[44m"
#define CNSL_COLOR_BKG_MAGENTA	  "\x1b[45m"
#define CNSL_COLOR_BKG_CYAN		  "\x1b[46m"
#define CNSL_COLOR_BKG_WHITE	  "\x1b[47m"
#define CNSL_COLOR_BKG_EXTENDED	  "\x1b[48m"
#define CNSL_COLOR_BKG_DEFAULT	  "\x1b[49m"
#define CNSL_COLOR_BKG_BLACK_B	  "\x1b[100m"
#define CNSL_COLOR_BKG_RED_B	  "\x1b[101m"
#define CNSL_COLOR_BKG_GREEN_B	  "\x1b[102m"
#define CNSL_COLOR_BKG_YELLOW_B	  "\x1b[103m"
#define CNSL_COLOR_BKG_BLUE_B	  "\x1b[104m"
#define CNSL_COLOR_BKG_MAGENTA_B  "\x1b[105m"
#define CNSL_COLOR_BKG_CYAN_B	  "\x1b[106m"
#define CNSL_COLOR_BKG_WHITE_B	  "\x1b[107m"
#define CNSL_TFORMAT_DEFAULT	  "\x1b[0m"
#define CNSL_TFORMAT_BOLD_EN	  "\x1b[1m"
#define CNSL_TFORMAT_BOLD_DIS	  "\x1b[22m"
#define CNSL_TFORMAT_ULINE_EN	  "\x1b[4m"
#define CNSL_TFORMAT_ULINE_DIS	  "\x1b[24m"
#define CNSL_TFORMAT_NEGATIVE_EN  "\x1b[7m"
#define CNSL_TFORMAT_NEGATIVE_DIS "\x1b[27m"
#define CNSL_CLEAR_DISPLAY		  "\x1b[2J"
#define CNSL_CLEAR_SCROLLB		  "\x1b[3J"
#define CNSL_CLEAR_ALL			  "\x1B[2J\x1B[3J"

namespace cnsl {

	enum class Color {
		BLACK		   = 30,
		RED			   = 31,
		GREEN		   = 32,
		YELLOW		   = 33,
		BLUE		   = 34,
		MAGENTA		   = 35,
		CYAN		   = 36,
		WHITE		   = 37,
		EXTENDED	   = 38,
		DEFAULT		   = 39,
		BRIGHT_BLACK   = 90,
		BRIGHT_RED	   = 91,
		BRIGHT_GREEN   = 92,
		BRIGHT_YELLOW  = 93,
		BRIGHT_BLUE	   = 94,
		BRIGHT_MAGENTA = 95,
		BRIGHT_CYAN	   = 96,
		BRIGHT_WHITE   = 97
	};

	unsigned long Create();

	int Free();

	int ReopenFileAsOutput(FILE* pFile);
	int ReopenFileAsInput(FILE* pFile);
	
	unsigned long SetVTProcessingEn(bool enabled);
	
	unsigned long OutputVTSequence(const char* sPrefix, int iVal, const char* sSuffix);
	unsigned long OutputVTSequence(const char* sPrefix, const char* sVal, const char* sSuffix);	
	unsigned long OutputVTSequence(const char* sSeq);

	char* MakeVTSeq(const char* sA, int iB, const char* sC);
	char* MakeVTSeq(const char* sA, const char* sB, const char* sC);
	char* MakeVTSeq(const char* sA, const char* sB);
	char* MakeVTSeq(const char* sA);

	inline unsigned long SetTxtColorBkg(Color color) { return OutputVTSequence("[", (int)color + 10, "m"); }
	inline unsigned long SetTxtColorFrg(Color color) { return OutputVTSequence("[", (int)color, "m"); }
	unsigned long SetTxtColors(Color colorFG, Color colorBG);

	inline char* GetColorVTSeqBkg(Color color) { return MakeVTSeq("[", (int)color + 10, "m"); }
	inline char* GetColorVTSeqFrg(Color color) { return MakeVTSeq("[", (int)color, "m"); }
	char* GetColorVTSeq(Color colorFG, Color colorBG);	

	inline unsigned long SetTxtBoldEn(bool enabled) { return OutputVTSequence(enabled ? CNSL_TFORMAT_BOLD_EN : CNSL_TFORMAT_BOLD_DIS); }
	inline unsigned long SetTxtUnderlineEn(bool enabled) { return OutputVTSequence(enabled ? CNSL_TFORMAT_ULINE_EN : CNSL_TFORMAT_ULINE_DIS); }
	inline unsigned long SetTxtNegativeEn(bool enabled) { return OutputVTSequence(enabled ? CNSL_TFORMAT_NEGATIVE_EN : CNSL_TFORMAT_NEGATIVE_DIS); }

	inline unsigned long ClearTxtFormat() { return OutputVTSequence(CNSL_TFORMAT_DEFAULT); }

	inline unsigned long DeleteTxtVisible() { return OutputVTSequence(CNSL_CLEAR_DISPLAY); }				
	inline unsigned long DeleteTxtScrollback() { return OutputVTSequence(CNSL_CLEAR_SCROLLB); }						
	inline unsigned long DeleteTxtAll() { return OutputVTSequence(CNSL_CLEAR_ALL); }
}

