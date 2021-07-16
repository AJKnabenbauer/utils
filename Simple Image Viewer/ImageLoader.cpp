#include "ImageLoader.h"

#include "stb/stb_image.h"

#include <commdlg.h>



STB_IMAGE::STB_IMAGE()
{
}

STB_IMAGE::STB_IMAGE(const STB_IMAGE& srcImg)
{
	if (srcImg.m_data) {
		int bufferSize = srcImg.sizeBytes();
		unsigned char* ptr = nullptr;


		//bufferSize = srcImg
	}
}

STB_IMAGE::~STB_IMAGE()
{
	free_data();
}

bool STB_IMAGE::loadFromFile(const std::string& fileName)
{
	int width, height, nrChannels;

	unsigned char* data = stbi_load(
		fileName.c_str(), 
		&width, 
		&height, 
		&nrChannels, 
		4);				// Force 4 channels to get RGBA (for d2d)

	// If the image is not able to be loaded we return without 
	// changing the current image
	if (data == NULL) {
		return false;
	}

	// Release old data if there is any
	free_data();

	m_width = width;
	m_height = height;
	m_sizeBytes = width * height * 4;
	m_data = data;
	m_fileName = fileName;

	return true;
}

bool STB_IMAGE::loadFromFile(const std::wstring& fileName)
{
	char buffer[MAX_PATH] ={ 0 };

	int bytesConverted = WideCharToMultiByte(
		CP_UTF8,		  // Code page to use
		0,				  // Conversion Flags
		fileName.c_str(), // wchar buffer
		-1,				  // wchar string is null terminated
		buffer,			  // char buffer
		sizeof(buffer),	  // char buffer size in bytes
		NULL,			  // must be NULL when using UTF-8
		NULL);			  // must be NULL when using UTF-8

	if (bytesConverted == 0) {	
		// Update the STB_Image last error string so we can retrieve
		// the error message using stbi_failure_reason()
		//stbi__err("WideCharToMultiByte failed", 
		//	"Unable to convert file name from wchar_t to char string.");
		return false;
	}

	return loadFromFile(buffer);
}

bool STB_IMAGE::loadFromFileDialog()
{
	WCHAR pszFileName[MAX_PATH] ={ 0 };

	pszFileName[0] = L'\0';

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize     = sizeof(ofn);
	ofn.hwndOwner       = NULL;
	ofn.lpstrFilter     = 
		L"All Image Files\0"              L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico\0"
		L"Windows Bitmap\0"               L"*.bmp;*.dib\0"
		L"High Definition Photo\0"        L"*.wdp;*.mdp;*.hdp\0"
		L"Graphics Interchange Format\0"  L"*.gif\0"
		L"Portable Network Graphics\0"    L"*.png\0"
		L"JPEG File Interchange Format\0" L"*.jpg;*.jpeg\0"
		L"Tiff File\0"                    L"*.tif\0"
		L"Icon\0"                         L"*.ico\0"
		L"All Files\0"                    L"*.*\0"
		L"\0";
	ofn.lpstrFile       = pszFileName;
	ofn.nMaxFile        = ARRAYSIZE(pszFileName);
	ofn.lpstrTitle      = L"Open Image";
	ofn.Flags           = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&ofn) != TRUE)
	{
		return false;
	}

	return loadFromFile(pszFileName);
}


const char* STB_IMAGE::getErrorString()
{
	return stbi_failure_reason();
}

void STB_IMAGE::free_data()
{
	if (m_data) {
		free(m_data);
	}

	m_width = 0;
	m_height = 0;
	m_sizeBytes = 0;
	m_fileName = "";
}



