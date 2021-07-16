#pragma once

#pragma once

#include <Windows.h>

//#include <wincodec.h>

#include <commdlg.h>

#include <d2d1.h>
#pragma comment(lib,"d2d1.lib")

#include "ImageLoader.h"

#include "ak_utils/ak_files.h"

static const wchar_t WIC_IMAGE_FILETYPE_FILTER[] = fileTypeFilterString(
    fileTypeFilterValue("All Image Files", "*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico")
    fileTypeFilterValue("Windows Bitmap", "*.bmp;*.dib")
    fileTypeFilterValue("High Definition Photo", "*.wdp;*.mdp;*.hdp")
    fileTypeFilterValue("Graphics Interchange Format", "*.gif")
    fileTypeFilterValue("Portable Network Graphics", "*.png")
    fileTypeFilterValue("JPEG File Interchange Format", "*.jpg;*.jpeg")
    fileTypeFilterValue("Tiff File", "*.tif")
    fileTypeFilterValue("Icon", "*.ico")
    fileTypeFilterValue("All Files", "*.*")
);

const float DEFAULT_DPI = 96.f;   // Default DPI that maps image resolution directly to screen resoltuion

class ImageDisplay
{
private:
    ID2D1Factory* m_pD2DFactory = nullptr;
    ID2D1HwndRenderTarget* m_pRT = nullptr;
    ID2D1Bitmap* m_pD2DBitmap = nullptr;
    HWND m_hWnd = NULL;

    // Note that depending on if we are using software or hardware rendering, then 
    // only certain combinations of pixel format and alpha mode are aloud. See 
    // https://docs.microsoft.com/en-us/windows/win32/Direct2D/supported-pixel-formats-and-alpha-modes
    // for more info on the allowed combinations
    
    D2D1_RENDER_TARGET_PROPERTIES m_rtProps = D2D1::RenderTargetProperties(
        // Force hardware rendering so we can use RGBA format
        D2D1_RENDER_TARGET_TYPE_HARDWARE, 
        // Default format to RGBA8 and ignore alpha channel
        D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
        DEFAULT_DPI, DEFAULT_DPI
    );

    // static bool to keep track of if we have to register our window class or not
    inline static bool m_windowClassRegistered = false;

public:
    
    int image_Width = 0;
    int image_Height = 0; 
    int image_BPP = 0;
    unsigned char* image_Data = nullptr;

    ImageDisplay();
    ~ImageDisplay();

    HWND handle() const { return m_hWnd; }
    static const WCHAR* className() { return L"ImageDisplayClass"; }

    HRESULT Initialize(HWND hParent, int xPos, int yPos, int width, int height);
    LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool LoadImageFromDialog();

    bool loadImage(const STB_IMAGE& img);

    bool loadImage(const WIC_IMAGE& img);

    bool redraw() { return InvalidateRect(m_hWnd, nullptr, TRUE); }
private:

    HRESULT setupResources();
    LRESULT OnPaint();

    static LRESULT CALLBACK s_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



};





//class WIC_ImageViewer_Child
//{
//private:
//    HWND m_hWnd = NULL;
//
//    IWICImagingFactory* m_pIWICFactory = nullptr;
//
//    ID2D1Factory* m_pD2DFactory = nullptr;
//    ID2D1HwndRenderTarget* m_pRT = nullptr;
//    ID2D1Bitmap* m_pD2DBitmap = nullptr;
//    IWICFormatConverter* m_pConvertedSourceBitmap = nullptr;
//
//    static HRESULT s_RegisterWindowsClass();
//    static LRESULT CALLBACK s_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//
//public:
//    WIC_ImageViewer_Child();
//    ~WIC_ImageViewer_Child();
//
//    HRESULT Initialize(int xPos, int yPos, int width, int height);
//
//private:
//    HRESULT CreateD2DBitmapFromFile(HWND hWnd);
//    bool LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName);
//    HRESULT CreateDeviceResources(HWND hWnd);
//
//    LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//    LRESULT OnPaint(HWND hWnd);
//
//    
//
//
//private:
//
//
//};