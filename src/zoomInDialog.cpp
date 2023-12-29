#include "zoomInDialog.h"

#ifdef _DEBUG
	#pragma comment (lib, "AppTemplateDebug.lib")
#else
	#pragma comment (lib, "AppTemplate.lib")     
#endif

zoomInDialog::zoomInDialog() :
	WindowDialog(L"ZOOMIN", L"zoomIn")
{
}

zoomInDialog::~zoomInDialog()
{
}

void zoomInDialog::OnInitDialog()
{
	DisableMaximize();
	DisableMinimize();
	DisableSize();
}


void zoomInDialog::OnDestroy()
{

}

void zoomInDialog::OnPaint()
{

}