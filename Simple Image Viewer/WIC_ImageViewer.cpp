#include "WIC_ImageViewer.h"

#define STB_IMAGE_IMPLEMENTATION	// Required 
#define STBI_FAILURE_USERMSG
#define STBI_WINDOWS_UTF8	

#include "stb/stb_image.h"


template <typename T>
inline void SafeRelease(T*& p)
{
    if (nullptr != p)
    {
        p->Release();
        p = nullptr;
    }
}





ImageDisplay::ImageDisplay()
{
}



ImageDisplay::~ImageDisplay()
{
    //if (image_Data != nullptr) {
    //    stbi_image_free(image_Data);
    //}

    SafeRelease(m_pD2DBitmap);
    SafeRelease(m_pD2DFactory);
    SafeRelease(m_pRT);
}



HRESULT ImageDisplay::Initialize(HWND hParent, int xPos, int yPos, int width, int height)
{
    HRESULT hr = S_OK;

    // Register window class if we haven't yet
    // 
    if (m_windowClassRegistered == false) {
        
        WNDCLASSEX wcex ={ 0 };
        wcex.cbSize        = sizeof(WNDCLASSEX);
        wcex.style         = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc   = ImageDisplay::s_WndProc;
        wcex.cbClsExtra    = 0;
        wcex.cbWndExtra    = sizeof(LONG_PTR);
        wcex.hInstance     = GetModuleHandle(NULL);     // handle of the calling .exe process
        wcex.hbrBackground = NULL;                      // since this is null we have to draw our own background 
        wcex.lpszClassName = className();

        hr = RegisterClassEx(&wcex) ? S_OK : E_FAIL;

        if (SUCCEEDED(hr)) m_windowClassRegistered = true;
    }

    // Create window
    // 
    if (SUCCEEDED(hr)) {
        
        m_hWnd = CreateWindow(
            className(),
            L"",                    // No name
            WS_CHILD | WS_VISIBLE,
            xPos, yPos,
            width, height,
            hParent,
            nullptr,                // No menu handle
            GetModuleHandle(NULL),  // Default instance handle
            this);

        hr = m_hWnd ? S_OK : E_FAIL;
    }


    // Create D2D resources
    // 
    if (SUCCEEDED(hr)) {
        hr = setupResources();
    }


    return hr;
}



LRESULT ImageDisplay::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

        case WM_SIZE:
        {
            auto size = D2D1::SizeU(LOWORD(lParam), HIWORD(lParam));

            if (m_pRT)
            {
                // If we couldn't resize, release the device and we'll recreate it
                // during the next render pass.
                if (FAILED(m_pRT->Resize(size)))
                {
                    SafeRelease(m_pRT);
                    SafeRelease(m_pD2DBitmap);
                }
            }

            InvalidateRect(m_hWnd, nullptr, TRUE);

            break;
        }


        case WM_PAINT:
        {
            return OnPaint();
        }


        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }


        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);


    }

    return 0;
}

bool ImageDisplay::LoadImageFromDialog()
{
    WCHAR pszFileName[MAX_PATH] ={ 0 };

    pszFileName[0] = L'\0';

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = NULL;
    ofn.lpstrFilter     = L"All Image Files\0"              L"*.bmp;*.dib;*.wdp;*.mdp;*.hdp;*.gif;*.png;*.jpg;*.jpeg;*.tif;*.ico\0"
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

    // Display the Open dialog box. 
    if (GetOpenFileName(&ofn) == TRUE) 
    {
        char fileName[MAX_PATH] ={ 0 };
        stbi_convert_wchar_to_utf8(fileName, sizeof(fileName), pszFileName);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 4);

        if (data)
        {
            // free old image data and release bitmap
            if (image_Data != nullptr) {
                stbi_image_free(image_Data);
            }

            SafeRelease(m_pD2DBitmap);

            image_Width = width;
            image_Height = height;
            image_BPP = 4;
            image_Data = data;
            
            // redraw
            InvalidateRect(m_hWnd, nullptr, TRUE);

            return true;
        }
    }

    return false;
}

bool ImageDisplay::loadImage(const STB_IMAGE& img)
{
    if (img.m_data == nullptr) {
        return false;
    }

    image_Width = img.m_width;
    image_Height = img.m_height;
    image_BPP = 4;
    image_Data = img.m_data;

    SafeRelease(m_pD2DBitmap);
    InvalidateRect(m_hWnd, nullptr, TRUE);

    return true;
}



bool ImageDisplay::loadImage(const WIC_IMAGE& img)
{
    // release old bitmap
    SafeRelease(m_pD2DBitmap);

    if (m_pRT && img.wicConverter && !m_pD2DBitmap) {
        HRESULT hr = m_pRT->CreateBitmapFromWicBitmap(img.wicConverter, &m_pD2DBitmap);
        
        return SUCCEEDED(hr) ? true : false;
    }
    
    return false;
}



HRESULT ImageDisplay::setupResources()
{
    HRESULT hr = S_OK;

    if (SUCCEEDED(hr) && !m_pD2DFactory)
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    }

    if (SUCCEEDED(hr) && !m_pRT) 
    {
        RECT rc;
        hr = GetClientRect(m_hWnd, &rc) ? S_OK : E_FAIL;

        if (SUCCEEDED(hr)) {
            auto size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

            hr = m_pD2DFactory->CreateHwndRenderTarget(
                m_rtProps, 
                D2D1::HwndRenderTargetProperties(m_hWnd, size), 
                &m_pRT);
        }
    }

    // D2DBitmap may have been released due to device loss. 
    // If so, re-create it from the source data if possible.
    if (SUCCEEDED(hr) && (!m_pD2DBitmap && image_Data)) 
    {
        hr = m_pRT->CreateBitmap(
            D2D1::SizeU(image_Width, image_Height),
            D2D1::BitmapProperties(m_rtProps.pixelFormat),
            &m_pD2DBitmap);

        if (SUCCEEDED(hr)) 
        {
            D2D1_RECT_U recDest = D2D1::Rect<UINT>(0, 0, image_Width, image_Height);
        
            hr = m_pD2DBitmap->CopyFromMemory(
                &recDest, 
                image_Data, 
                image_Width * image_BPP);
        }
    }

    return hr;
}



LRESULT ImageDisplay::OnPaint()
{
    HRESULT hr = S_OK;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hWnd, &ps);

    if (hdc != NULL)
    {
        hr = setupResources();


        if (SUCCEEDED(hr) && !(m_pRT->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
        {
            m_pRT->BeginDraw();
            m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());
            m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));



            auto rtSize = m_pRT->GetSize();

            if (m_pD2DBitmap) {
                m_pRT->DrawBitmap(m_pD2DBitmap, 
                    D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height));
            }



            hr = m_pRT->EndDraw();

            if (hr == D2DERR_RECREATE_TARGET) {
                SafeRelease(m_pD2DBitmap);
                SafeRelease(m_pRT);

                // Force a re-render
                hr = InvalidateRect(m_hWnd, nullptr, TRUE) ? S_OK : E_FAIL; 
            }
        }

        EndPaint(m_hWnd, &ps);
    }

    return SUCCEEDED(hr) ? 0 : 1;
}



LRESULT ImageDisplay::s_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImageDisplay* pThis;
    LRESULT lRet = 0;

    if (uMsg == WM_NCCREATE) {
        auto pcs = reinterpret_cast<LPCREATESTRUCT> (lParam);
        pThis = reinterpret_cast<ImageDisplay*> (pcs->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR> (pThis));
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    else {
        pThis = reinterpret_cast<ImageDisplay*> (GetWindowLongPtr(hWnd, GWLP_USERDATA));

        lRet = (pThis) ?
            pThis->WndProc(hWnd, uMsg, wParam, lParam) :
            DefWindowProc(hWnd, uMsg, wParam, lParam);

    }
    return lRet;
}