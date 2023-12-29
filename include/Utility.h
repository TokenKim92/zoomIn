#ifndef _UTILITY_H_ 
#define _UTILITY_H_

#include <string>

std::wstring FloatToHexWString(const float a_value);
bool PointInRectF(const D2D1_RECT_F &ap_rect, const POINT &ap_pos);
bool CopyTextToClipboard(HWND ah_wnd, const std::wstring &a_string);

#endif //_UTILITY_H_