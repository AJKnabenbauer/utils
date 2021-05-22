#include "logger.h"
#include <windows.h>

// Convert a wide Unicode std::string to an UTF8 std::string
std::string utf8_encode ( const std::wstring& wstr )
{
	if ( wstr.empty () ) return std::string ();
	int size_needed = WideCharToMultiByte ( CP_UTF8, 0, &wstr[0], ( int )wstr.size (), NULL, 0, NULL, NULL );
	std::string strTo ( size_needed, 0 );
	WideCharToMultiByte ( CP_UTF8, 0, &wstr[0], ( int )wstr.size (), &strTo[0], size_needed, NULL, NULL );
	return strTo;
}

// Convert an UTF8 std::string to a wide Unicode String
std::wstring utf8_decode ( const std::string& str )
{
	if ( str.empty () ) return std::wstring ();
	int size_needed = MultiByteToWideChar ( CP_UTF8, 0, &str[0], ( int )str.size (), NULL, 0 );
	std::wstring wstrTo ( size_needed, 0 );
	MultiByteToWideChar ( CP_UTF8, 0, &str[0], ( int )str.size (), &wstrTo[0], size_needed );
	return wstrTo;
}

// Get windows last error message as char std::string
std::string getWindowsErrorString ( DWORD errorNum )
{
	WCHAR* errorBuf = NULL;
	FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorNum, 0, ( LPTSTR )&errorBuf, 0, NULL );
	std::string str = utf8_encode ( errorBuf );
	LocalFree ( errorBuf ); //FormatMessage uses LOCALALLOC so we have to use LOCALFREE
	return str;
}

// Get windows last error message as wide std::string
std::wstring getWindowsErrorStringW ( DWORD errorNum )
{
	WCHAR* errorBuf = NULL;
	FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorNum, 0, ( LPTSTR )&errorBuf, 0, NULL );
	std::wstring str = errorBuf;
	LocalFree ( errorBuf ); //FormatMessage uses LOCALALLOC so we have to use LOCALFREE
	return str;
}

// Display a windows error popup
void showErrorMessageBox ( const std::string& msg )
{
	MessageBox ( NULL, utf8_decode ( msg ).c_str (), NULL, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
}

void showErrorMessageBox ( const std::wstring& msg )
{
	MessageBox ( NULL, msg.c_str (), NULL, MB_OK | MB_ICONERROR | MB_SETFOREGROUND );
}

void logError ( const std::string& time, const std::string& file, const std::string& function, const int line, const std::string& errorString )
{

#ifdef _DEBUG

	DWORD winError = GetLastError ();
	std::cout << time << "\n";
	std::cout << "Thread ID: " << GetCurrentThreadId () << "\n";
	std::cout << "File: "  << file << " line " << line << "\n";
	std::cout << "Function: " << function << "\n";
	std::cout << "User Error String: " << errorString << "\n";
	std::cout << "Windows Error[" << winError << "]: " << getWindowsErrorString ( winError ) << std::endl;
	__debugbreak ();

#else

	showErrorMessageBox (
		time + "\n"
		file + " line " + std::to_string ( line ) + "\n"
		"Function: " + function + "\n"
		"User Error String: " + errorString + "\n"
		"Windows Error[" + to_string ( winError ) + "]: " + getWindowsErrorString ( winError ) + "\n" );

#endif 

}