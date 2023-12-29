#ifndef _SCREEN_D2D_H_
#define _SCREEN_D2D_H_

#include "Direct2DEx.h"
#include <wincodec.h>

class ScreenD2D : public Direct2DEx
{
protected:
	HDC mh_memDC;
	HBITMAP mh_memBitmap;
	ID2D1Bitmap *mp_screenBitmap;

	POINT m_srcPoint;
	D2D1_RECT_F m_destRect;
	SIZE m_bitmapSize;
	unsigned char *mp_dibBits;
	unsigned int m_dibBitsSize;
	unsigned char m_screenBrightness;

public:
	ScreenD2D(HWND ah_wnd, const RECT *const ap_scaledRect, const RECT *const ap_orgRect);
	virtual ~ScreenD2D();

	virtual int Create();

	bool InitScreenBitmap();
	void ReleaseScreenBitmap();
	void DrawScreenBitmap(const D2D1_RECT_F *const ap_rect = nullptr);
	unsigned char GetScreenBrightness();
};

#endif // !_SCREEN_D2D_H_
