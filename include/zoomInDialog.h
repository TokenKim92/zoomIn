#ifndef _ZOOM_IN_DIALOG_H_
#define _ZOOM_IN_DIALOG_H_

#include "WindowDialog.h"

class zoomInDialog : public WindowDialog
{
protected:
	DRect m_indicateRect;
	DRect m_colorRect;
	DRect m_valueRect;
	DColor m_indicateBackgroundColor;
	DColor m_indicateBorderColor;
	DColor m_selectedColor;

public:
	zoomInDialog();
	virtual ~zoomInDialog();

protected:
	void DrawIndicate();

public:
	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;
};

#endif //_ZOOM_IN_DIALOG_H_