#include "zoomInDialog.h"
#include "ColorPalette.h"
#include "Utility.h"

#ifdef _DEBUG
#pragma comment (lib, "AppTemplateDebug.lib")
#else
#pragma comment (lib, "AppTemplate.lib")     
#endif

zoomInDialog::zoomInDialog() :
	WindowDialog(L"ZOOMIN", L"zoomIn")
{
	m_indicateRect = {
		10, 0, 140, 0
	};
	m_indicateBackgroundColor = RGB_TO_COLORF(NEUTRAL_400);
	m_indicateBorderColor = RGB_TO_COLORF(NEUTRAL_50);
	m_selectedColor = RGB_TO_COLORF(NEUTRAL_800);
}

zoomInDialog::~zoomInDialog()
{
}

void zoomInDialog::OnInitDialog()
{
	DisableMaximize();
	DisableMinimize();
	DisableSize();

	RECT viewRect;
	::GetClientRect(mh_window, &viewRect);

	m_indicateRect.top = viewRect.bottom - 50.0f;
	m_indicateRect.bottom = viewRect.bottom - 10.0f;

	const float offset = 5.0f;
	m_colorRect.left = m_indicateRect.left + offset;
	m_colorRect.top = m_indicateRect.top + offset;
	m_colorRect.bottom = m_indicateRect.bottom - offset;
	m_colorRect.right = m_colorRect.left + m_colorRect.bottom - m_colorRect.top;

	m_valueRect.left = m_colorRect.right + offset;
	m_valueRect.top = m_indicateRect.top + offset;
	m_valueRect.right = m_indicateRect.right - offset;
	m_valueRect.bottom = m_indicateRect.bottom - offset;
}

void zoomInDialog::OnDestroy()
{

}

void zoomInDialog::OnPaint()
{
	mp_direct2d->Clear();
	DrawIndicate();
}

void zoomInDialog::DrawIndicate()
{
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