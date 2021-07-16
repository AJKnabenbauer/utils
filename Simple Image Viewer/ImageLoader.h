#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wincodec.h>
#pragma comment(lib,"windowscodecs.lib")
#include <string>

template <typename T>
inline void SafeRelease_WIC(T*& p)
{
    if (nullptr != p)
    {
        p->Release();
        p = nullptr;
    }
}


class WIC_IMAGE
{
public:

    // Format converter pixel format GUIDs
    // See https://docs.microsoft.com/en-us/windows/win32/direct2d/supported-pixel-formats-and-alpha-modes
    // 
    inline static const REFWICPixelFormatGUID FORMAT_RGBA = GUID_WICPixelFormat32bppPRGBA;
    inline static const REFWICPixelFormatGUID FORMAT_RGB = GUID_WICPixelFormat32bppPRGBA;
    inline static const REFWICPixelFormatGUID FORMAT_BGRA = GUID_WICPixelFormat32bppPBGRA;
    inline static const REFWICPixelFormatGUID FORMAT_BGR = GUID_WICPixelFormat32bppBGR;
    inline static const REFWICPixelFormatGUID FORMAT_A8 = GUID_WICPixelFormat8bppAlpha;

    IWICImagingFactory* wicFactory = NULL;
    IWICBitmapDecoder* wicDecoder = NULL;
    IWICBitmapFrameDecode* wicFrame = NULL;
    IWICFormatConverter* wicConverter = NULL;
    REFWICPixelFormatGUID pixelFormat = WIC_IMAGE::FORMAT_RGBA;
    std::wstring fileName;

    WIC_IMAGE() {};
    ~WIC_IMAGE()
    {
        SafeRelease_WIC(wicConverter);
        SafeRelease_WIC(wicFrame);
        SafeRelease_WIC(wicDecoder);
        SafeRelease_WIC(wicFactory);
    }

    HRESULT loadImageFromFile(const std::wstring& fName)
    {
        HRESULT hr = S_OK;

        // create factory 
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,        // CLS ID of the object we are making
            NULL,                           // Not part of an agregate
            CLSCTX_INPROC_SERVER,           // DLL runs in the same process
            IID_IWICImagingFactory,         // Ref to interface that communicates with the object
            (LPVOID*)&wicFactory);          // Pointer that'll contain our factory
        
        if (SUCCEEDED(hr))
        {
            // create decoder
            hr = wicFactory->CreateDecoderFromFilename(
                fName.c_str(),                  // The file name
                NULL,                           // No preffered vendor
                GENERIC_READ,                   // Reading the file only, not writing
                WICDecodeMetadataCacheOnLoad,   // Cache on load
                &wicDecoder);                   // Decoder we are making
        }

        if (SUCCEEDED(hr))
        {
            // get frame 0
            hr = wicDecoder->GetFrame(0, &wicFrame);
        }

        if (SUCCEEDED(hr))
        {
            // create a format converter so we can convert pixel format to one
            // that direct2d is compatible with
            hr = wicFactory->CreateFormatConverter(&wicConverter);
        }

        if (SUCCEEDED(hr))
        {
            // initialize our format converter
            hr = wicConverter->Initialize(
                wicFrame,					 // Frame
                pixelFormat,				 // Pixel Format
                WICBitmapDitherTypeNone,	 // Irrelevant
                NULL,						 // No pallet needed
                0.0,						 // Alpha transparency %
                WICBitmapPaletteTypeCustom); // Irrelevant
        }

        return hr;
    }
};


class STB_IMAGE
{
public:

    std::string	m_fileName;
    int			m_width = 0;
    int			m_height = 0;
    int			m_sizeBytes = 0;
    unsigned char* m_data = nullptr;

	STB_IMAGE();
	STB_IMAGE(const STB_IMAGE& srcImg);
	~STB_IMAGE();

	bool loadFromFile(const std::string& fileName);
	bool loadFromFile(const std::wstring& fileName);

	bool loadFromFileDialog();

	const char* getErrorString();


	unsigned int sizeBytes() const { return m_sizeBytes; }

private:

	void free_data();



};