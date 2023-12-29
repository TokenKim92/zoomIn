#ifndef _ZOOM_IN_DIALOG_H_
#define _ZOOM_IN_DIALOG_H_

#include "WindowDialog.h"

class zoomInDialog : public WindowDialog
{
public:
	zoomInDialog();
	virtual ~zoomInDialog();

public:
	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;
};

#endif //_ZOOM_IN_DIALOG_H_