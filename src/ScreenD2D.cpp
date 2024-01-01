#include "framework.h"
#include "ScreenD2D.h"

extern ApplicationCore *gp_appCore;

ScreenD2D::ScreenD2D(HWND ah_wnd, const RECT *const ap_viewRect)
	:Direct2DEx(ah_wnd, ap_viewRect)
{
	mh_memDC = nullptr;
	mh_memBitmap = nullptr;

	m_destRect = {
		0,
		0,
		static_cast<float>(ap_viewRect->right - ap_viewRect->left),
		static_cast<float>(ap_viewRect->bottom - ap_viewRect->top)
	};
	m_imageSize = 0;
	m_imageHalfSize = 0;
	mp_dibBits = nullptr;
	m_dibBitsSize = 0;
}

ScreenD2D::~ScreenD2D()
{
	DestroyImage();
}

bool ScreenD2D::CreateImage(const unsigned short a_imageSize)
{
	m_imageSize = a_imageSize;
	m_imageHalfSize = a_imageSize / 2;

	HDC h_screenDC = ::GetWindowDC(nullptr);
	HDC h_memDC = ::CreateCompatibleDC(h_screenDC);
	if (nullptr == h_memDC) {
			return false;
	}
	mh_memDC = h_memDC;

	HBITMAP h_tempBitmap = ::CreateCompatibleBitmap(h_screenDC, a_imageSize, a_imageSize);
	if (nullptr == h_tempBitmap) {
		return false;
	}

	BITMAP bitmap;
	GetObject(h_tempBitmap, sizeof(BITMAP), &bitmap);

	BITMAPINFO bitmapInfo;
	ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = bitmap.bmWidth;
	bitmapInfo.bmiHeader.biHeight = bitmap.bmHeight;
	bitmapInfo.bmiHeader.biPlanes = bitmap.bmPlanes;
	bitmapInfo.bmiHeader.biBitCount = bitmap.bmBitsPixel;
	bitmapInfo.bmiHeader.biCompression = bitmap.bmType;
	bitmapInfo.bmiHeader.biSizeImage = bitmap.bmHeight * bitmap.bmWidth * bitmap.bmBitsPixel;

	// create a bitmap and set the first memory address of the bitmap to `mp_dibBitx`
	HBITMAP h_memBitmap = CreateDIBSection(mh_memDC, &bitmapInfo, DIB_RGB_COLORS, (void **)&mp_dibBits, nullptr, 0);
	if (nullptr == h_memBitmap) {
		return false;
	}

	mh_memBitmap = h_memBitmap;
	m_dibBitsSize = bitmap.bmHeight * bitmap.bmWidthBytes;
	::SelectObject(mh_memDC, mh_memBitmap);

	::DeleteObject(h_tempBitmap);
	::ReleaseDC(nullptr, h_screenDC);

	return true;
}

void ScreenD2D::DestroyImage()
{
	if (nullptr != mh_memDC) {
		::DeleteDC(mh_memDC);
	}
	if (nullptr != mh_memBitmap) {
		::DeleteObject(mh_memBitmap);
	}
	if (nullptr != mp_dibBits) {
		mp_dibBits = nullptr;
	}
	m_dibBitsSize = 0;
}

void ScreenD2D::DrawImage(const POINT &a_pos)
{
	HDC h_screenDC = ::GetWindowDC(nullptr);
	::BitBlt(
		mh_memDC,
		0, 0, m_imageSize, m_imageSize,
		h_screenDC,
		a_pos.x - m_imageHalfSize, a_pos.y - m_imageHalfSize,
		SRCCOPY
	);
	::ReleaseDC(NULL, h_screenDC);

	IWICBitmap *p_wicBitmap = nullptr;
	IWICImagingFactory *p_wicFactory = gp_appCore->GetWICFactory();

	if (S_OK != p_wicFactory->CreateBitmapFromHBITMAP(mh_memBitmap, NULL, WICBitmapUseAlpha, &p_wicBitmap)) {
		return;
	}

	IWICFormatConverter *p_converter;
	if (S_OK != p_wicFactory->CreateFormatConverter(&p_converter)) {
		p_wicBitmap->Release();
		return;
	}

	if (S_OK != p_converter->Initialize(p_wicBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)) {
		p_converter->Release();
		p_wicBitmap->Release();
		return;
	}

	ID2D1Bitmap *p_screenBitmap;
	if (S_OK != mp_renderTarget->CreateBitmapFromWicBitmap(p_converter, NULL, &p_screenBitmap)) {
		p_converter->Release();
		p_wicBitmap->Release();
		return;
	}

	mp_renderTarget->DrawBitmap(p_screenBitmap, &m_destRect);

	p_screenBitmap->Release();
	p_converter->Release();
	p_wicBitmap->Release();
}