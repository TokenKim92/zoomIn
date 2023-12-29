#include "framework.h"
#include "ScreenD2D.h"

extern ApplicationCore *gp_appCore;

ScreenD2D::ScreenD2D(HWND ah_wnd, const RECT *const ap_scaledRect, const RECT *const ap_orgRect)
	:Direct2DEx(ah_wnd, ap_scaledRect)
{
	mh_memDC = nullptr;
	mh_memBitmap = nullptr;
	mp_screenBitmap = nullptr;

	m_srcPoint = {
		ap_scaledRect->left,
		ap_scaledRect->top
	};
	m_destRect = {
		0,
		0,
		static_cast<float>(ap_scaledRect->right - ap_scaledRect->left),
		static_cast<float>(ap_scaledRect->bottom - ap_scaledRect->top)
	};
	m_bitmapSize = {
		ap_orgRect->right - ap_orgRect->left,
		 ap_orgRect->bottom - ap_orgRect->top
	};

	mp_dibBits = nullptr;
	m_dibBitsSize = 0;
	m_screenBrightness = 0;
}

ScreenD2D::~ScreenD2D()
{
	if (nullptr != mh_memDC) {
		::DeleteDC(mh_memDC);
	}
	if (nullptr != mh_memBitmap) {
		::DeleteObject(mh_memBitmap);
	}
	InterfaceRelease(&mp_screenBitmap);
}

int ScreenD2D::Create()
{
	HRESULT hResult = Direct2DEx::Create();
	if (S_OK != hResult) {
		return hResult;
	}

	HDC h_screenDC = ::GetWindowDC(NULL);
	mh_memDC = ::CreateCompatibleDC(h_screenDC);
	if (!mh_memDC) return D2DERR_WIN32_ERROR;
	HBITMAP h_tempBitmap = ::CreateCompatibleBitmap(h_screenDC, m_bitmapSize.cx, m_bitmapSize.cy);
	if (!h_tempBitmap) return D2DERR_WIN32_ERROR;

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
	mh_memBitmap = CreateDIBSection(mh_memDC, &bitmapInfo, DIB_RGB_COLORS, (void **)&mp_dibBits, NULL, 0);
	if (!mh_memBitmap) return D2DERR_WIN32_ERROR;

	m_dibBitsSize = bitmap.bmHeight * bitmap.bmWidthBytes;
	::SelectObject(mh_memDC, mh_memBitmap);

	::DeleteObject(h_tempBitmap);
	::ReleaseDC(NULL, h_screenDC);

	return S_OK;
}

bool ScreenD2D::InitScreenBitmap()
{
	HDC h_screenDC = ::GetWindowDC(NULL);
	::BitBlt(
		mh_memDC,
		0, 0, m_bitmapSize.cx, m_bitmapSize.cy,
		h_screenDC,
		m_srcPoint.x, m_srcPoint.y,
		SRCCOPY
	);
	::ReleaseDC(NULL, h_screenDC);

	IWICBitmap *p_wicBitmap = nullptr;
	IWICImagingFactory *p_wicFactory = gp_appCore->GetWICFactory();

	if (S_OK == p_wicFactory->CreateBitmapFromHBITMAP(mh_memBitmap, NULL, WICBitmapUseAlpha, &p_wicBitmap)) {
		IWICFormatConverter *p_converter;
		if (S_OK == p_wicFactory->CreateFormatConverter(&p_converter)) {
			if (S_OK == p_converter->Initialize(p_wicBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)) {
				mp_renderTarget->CreateBitmapFromWicBitmap(p_converter, NULL, &mp_screenBitmap);
			}
			p_converter->Release();
		}
		p_wicBitmap->Release();
	}

	UINT8 *p_pattern = mp_dibBits;
	UINT8 *p_lastPattern = mp_dibBits + m_dibBitsSize;
	UINT32 value = 0;
	//TODO:: Error!!!
	////////////////////////
	//DWORD stride = ((((bitmapInfo.bmiHeader.biWidth * bitmapInfo.bmiHeader.biBitCount) + 31) & ~31) >> 3);
	//m_fragmentSize = stride * bitmapInfo.bmiHeader.biHeight;
	////////////////////////

	while (p_pattern < p_lastPattern) {
		value += (*p_pattern + *(p_pattern + 1) + *(p_pattern + 2)) / 3;
		p_pattern += 4;
	}
	m_screenBrightness = (UINT8)(value / (m_bitmapSize.cx * m_bitmapSize.cy));

	return mp_screenBitmap != nullptr;
}

void ScreenD2D::ReleaseScreenBitmap()
{
	InterfaceRelease(&mp_screenBitmap);
}

void ScreenD2D::DrawScreenBitmap(const D2D1_RECT_F *const ap_rect)
{
	if (nullptr == ap_rect) {
		mp_renderTarget->DrawBitmap(mp_screenBitmap, &m_destRect);
		return;
	}

	mp_renderTarget->DrawBitmap(mp_screenBitmap, ap_rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, ap_rect);
}

UINT8 ScreenD2D::GetScreenBrightness()
{
	return m_screenBrightness;
}