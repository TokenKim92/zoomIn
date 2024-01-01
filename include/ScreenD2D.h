#ifndef _SCREEN_D2D_H_
#define _SCREEN_D2D_H_

#include "Direct2DEx.h"
#include <wincodec.h>

class ScreenD2D : public Direct2DEx
{
protected:
	HDC mh_memDC;
	HBITMAP mh_memBitmap;

	SIZE m_viewSize;
	DRect m_viewRect;
	unsigned short m_imageSize;
	unsigned short m_imageHalfSize;

public:
	ScreenD2D(HWND ah_wnd, const RECT *const ap_viewRect);
	virtual ~ScreenD2D();

	bool CreateImage(const unsigned short a_imageSize);
	void DestroyImage();
	void DrawImage(const POINT &a_pos);
	DColor GetPixelOnMousePos(const POINT &a_pos);
};

#endif // !_SCREEN_D2D_H_
