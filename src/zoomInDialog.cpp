#include "zoomInDialog.h"
#include "ColorPalette.h"
#include "Utility.h"
#include "ScreenD2D.h"

#ifdef _DEBUG
#pragma comment (lib, "AppTemplateDebug.lib")
#else
#pragma comment (lib, "AppTemplate.lib")     
#endif

zoomInDialog::zoomInDialog() :
	WindowDialog(L"ZOOMIN", L"zoomIn")
{
	memset(&m_viewRect, 0, sizeof(RECT));
	m_indicateRect = {
		10, 0, 140, 0
	};
	memset(&m_colorRect, 0, sizeof(DRect));
	memset(&m_valueRect, 0, sizeof(DRect));

	m_indicateBackgroundColor = RGB_TO_COLORF(NEUTRAL_400);
	m_indicateBorderColor = RGB_TO_COLORF(NEUTRAL_50);
	m_selectedColor = RGB_TO_COLORF(NEUTRAL_800);
	m_textColor = RGB_TO_COLORF(NEUTRAL_800);

	m_hoverOnIndicate = false;
	m_clickedOnIndicate = false;
}

zoomInDialog::~zoomInDialog()
{
}

void zoomInDialog::OnInitDialog()
{
	DisableMaximize();
	DisableMinimize();
	DisableSize();

	::GetClientRect(mh_window, &m_viewRect);

	m_indicateRect.top = m_viewRect.bottom - 50.0f;
	m_indicateRect.bottom = m_viewRect.bottom - 10.0f;

	const float offset = 5.0f;
	m_colorRect.left = m_indicateRect.left + offset;
	m_colorRect.top = m_indicateRect.top + offset;
	m_colorRect.bottom = m_indicateRect.bottom - offset;
	m_colorRect.right = m_colorRect.left + m_colorRect.bottom - m_colorRect.top;

	m_valueRect.left = m_colorRect.right + offset;
	m_valueRect.top = m_indicateRect.top + offset;
	m_valueRect.right = m_indicateRect.right - offset;
	m_valueRect.bottom = m_indicateRect.bottom - offset;

	auto p_direct2d = new ScreenD2D(mh_window, &m_viewRect, &m_viewRect);
	p_direct2d->Create();
	p_direct2d->InitScreenBitmap();
	p_direct2d->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	InheritDirect2D(p_direct2d);

	// add message handlers
	AddMessageHandler(WM_MOUSEMOVE, (MessageHandler)&zoomInDialog::MouseMoveHandler);
	AddMessageHandler(WM_LBUTTONDOWN, (MessageHandler)&zoomInDialog::MouseLeftButtonDownHandler);
	AddMessageHandler(WM_LBUTTONUP, (MessageHandler)&zoomInDialog::MouseLeftButtonUpHandler);
}

void zoomInDialog::OnDestroy()
{

}

void zoomInDialog::OnPaint()
{
	mp_direct2d->Clear();
	static_cast<ScreenD2D *>(mp_direct2d)->DrawScreenBitmap();

	DrawIndicate();
}

// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
int zoomInDialog::MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	if (PointInRectF(m_indicateRect, pos)) {
		if (!m_hoverOnIndicate) {
			m_hoverOnIndicate = true;
			InvalidateRect(mh_window, &m_viewRect, false);
		}
	}
	else {
		if (m_hoverOnIndicate) {
			m_hoverOnIndicate = false;
			InvalidateRect(mh_window, &m_viewRect, false);
		}
	}

	return S_OK;
}

// to handle the WM_LBUTTONDOWN  message that occurs when a window is destroyed
int zoomInDialog::MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	if (PointInRectF(m_indicateRect, pos)) {
		m_clickedOnIndicate = true;
		InvalidateRect(mh_window, &m_viewRect, false);

		std::wstring rgbText =
			FloatToHexWString(m_selectedColor.r) +
			FloatToHexWString(m_selectedColor.g) +
			FloatToHexWString(m_selectedColor.b);

		CopyTextToClipboard(mh_window, rgbText);
	}

	return S_OK;
}

// to handle the WM_LBUTTONUP  message that occurs when a window is destroyed
int zoomInDialog::MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	//const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };
	if (m_clickedOnIndicate) {
		m_clickedOnIndicate = false;
		InvalidateRect(mh_window, &m_viewRect, false);
	}

	return S_OK;
}

void zoomInDialog::DrawIndicate()
{
	float transparency = m_hoverOnIndicate ? 1.0f : 0.8f;
	if (m_clickedOnIndicate) {
		transparency = 0.8f;
	}
	
	m_indicateBackgroundColor.a = transparency;
	m_indicateBorderColor.a = transparency;
	m_selectedColor.a = transparency;
	m_textColor.a = transparency;

	// draw background
	mp_direct2d->SetBrushColor(m_indicateBackgroundColor);
	mp_direct2d->FillRoundedRectangle(m_indicateRect, 5.0f);
	mp_direct2d->SetBrushColor(m_indicateBorderColor);
	mp_direct2d->DrawRoundedRectangle(m_indicateRect, 5.0f);
	
	// draw color box
	mp_direct2d->SetBrushColor(m_selectedColor);
	mp_direct2d->FillRoundedRectangle(m_colorRect, 5.0f);

	// draw color value
	std::wstring rgbText = L"#" +
		FloatToHexWString(m_selectedColor.r) + 
		FloatToHexWString(m_selectedColor.g) + 
		FloatToHexWString(m_selectedColor.b);
	mp_direct2d->DrawUserText(rgbText.c_str(), m_valueRect);
}