//-----------------------------------------------------------------
// Bitmap Object
// C++ Header - Bitmap.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>



    //-----------------------------------------------------------------
    // Custom Data Types
    //-----------------------------------------------------------------
    struct BITMAPINFO_256
    {
        BITMAPINFOHEADER  bmiHeader;
        RGBQUAD           bmiColors[256];
    };

    //-----------------------------------------------------------------
    // Bitmap Class
    //-----------------------------------------------------------------
    class CustomBitmap
    {
    protected:
        // Member Variables
        HBITMAP m_hBitmap;
        int     m_iWidth, m_iHeight;

        // Helper Methods
        void Free();

    public:
        // Constructor(s)/Destructor
        CustomBitmap();
        CustomBitmap(HDC hDC, LPTSTR szFileName);
        CustomBitmap(HDC hDC, UINT uiResID, HINSTANCE hInstance);
        CustomBitmap(HDC hDC, int iWidth, int iHeight, COLORREF crColor = RGB(0, 0, 0));
        virtual ~CustomBitmap();

        // General Methods
        BOOL Create(HDC hDC, LPTSTR szFileName);
        BOOL Create(HDC hDC, UINT uiResID, HINSTANCE hInstance);
        BOOL Create(HDC hDC, int iWidth, int iHeight, COLORREF crColor);
        void Draw(HDC hDC, int x, int y, BOOL bTrans = FALSE,
            COLORREF crTransColor = RGB(255, 0, 255));
        void DrawPart(HDC hDC, int x, int y, int xPart, int yPart,
            int wPart, int hPart, BOOL bTrans = FALSE,
            COLORREF crTransColor = RGB(255, 0, 255));
        int  GetWidth() { return m_iWidth; };
        int  GetHeight() { return m_iHeight; };
    };
