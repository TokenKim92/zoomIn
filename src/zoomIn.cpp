// AppTemplate.cpp : Defines the entry point for the application.

#include "framework.h"
#include "ApplicationCore.h"
#include "zoomInDialog.h"

#ifdef _DEBUG
    #pragma comment (lib, "AppTemplateDebug.lib")
#else
    #pragma comment (lib, "AppTemplate.lib")     
#endif

int APIENTRY wWinMain(_In_ HINSTANCE ah_instance, _In_opt_ HINSTANCE ah_notUseInstance, _In_ wchar_t *ap_cmdLine, _In_ int a_showType)
{
    ApplicationCore appCore(ah_instance);
    if (S_OK == appCore.Create()) {
        const int centerPosX = ::GetSystemMetrics(SM_CXSCREEN) / 2;
        const int centerPosY = ::GetSystemMetrics(SM_CYSCREEN) / 2;
        const int size = 256;

        zoomInDialog dialog;
        dialog.SetExtendStyle(WS_EX_TOPMOST);
        return dialog.Create(size, size, centerPosX - size / 2, centerPosY - size / 2);
    }

    return 0;
}
