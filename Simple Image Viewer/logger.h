#pragma once

#ifndef LOGGER
#define LOGGER

#include<iostream>
#include<string>

#define log_errorString(userMsg) logError ( __TIMESTAMP__, __FILE__, __FUNCSIG__, __LINE__, userMsg ); __debugbreak ();
#define log_error() log_errorString("UNOWN ERROR")
#define log_assert(expression) \
	if (!(expression)) { \
		logError ( __TIMESTAMP__, __FILE__, __FUNCSIG__, __LINE__, "Assert Failed: " #expression); \
		__debugbreak (); \
	} 

std::string utf8_encode( const std::wstring& wstr );
std::wstring utf8_decode( const std::string& str );
void showErrorMessageBox( const std::string& msg );
void showErrorMessageBox( const std::wstring& msg );
void logError( const std::string& time, const std::string& file, const std::string& function, const int line, const std::string& errorString );

#endif