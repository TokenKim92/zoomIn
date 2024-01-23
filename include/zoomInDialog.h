#ifndef _ZOOM_IN_DIALOG_H_
#define _ZOOM_IN_DIALOG_H_

#include "WindowDialog.h"

class zoomInDialog : public WindowDialog
{
protected:
	RECT m_viewRect;
	DRect m_indicateRect;
	DRect m_colorRect;
	DRect m_valueRect;
	DColor m_indicateBackgroundColor;
	DColor m_indicateBorderColor;
	DColor m_selectedColor;
	DColor m_textColor;

	bool m_hoverOnIndicate;
	bool m_clickedOnIndicate;
	POINT m_mousePos;
	unsigned int m_imageSize;

public:
	zoomInDialog();
	virtual ~zoomInDialog();

	void StretchScreenImage(const POINT &a_pos);
	void SetMousePos(const POINT &a_pos);

protected:
	void DrawIndicate();

	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;
	virtual void OnSetColorMode() override;

	// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
	int MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONDOWN  message that occurs when a window is destroyed
	int MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONUP  message that occurs when a window is destroyed
	int MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_MOUSEWHEEL  message that occurs when a window is destroyed
	int MouseWheelHandler(WPARAM a_wordParam, LPARAM a_longParam);
};

#endif //_ZOOM_IN_DIALOG_H_