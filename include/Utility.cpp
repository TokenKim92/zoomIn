#include <d2d1.h>
#include "Utility.h"
#include <sstream>
#include <iomanip>

std::wstring FloatToHexWString(const float a_value)
{
	int integerValue = static_cast<unsigned char>(a_value * 255);

	std::stringstream stream;
	stream << std::setfill('0') << std::setw(2) << std::hex << integerValue;
	std::string result(stream.str());

	return std::wstring(result.begin(), result.end());
}

bool PointInRectF(const D2D1_RECT_F &ap_rect, const POINT &ap_pos)
{
	return ap_rect.left <= ap_pos.x && 
		ap_rect.top <= ap_pos.y &&
		ap_rect.right >= ap_pos.x && 
		ap_rect.bottom >= ap_pos.y;
}

bool CopyTextToClipboard(HWND ah_wnd, const std::wstring &a_string)
{
	const size_t textSize = (a_string.length() + 1) * 2;
	HGLOBAL h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, textSize);
	if (nullptr == h_data) {
		return false;
	}

	wchar_t *p_data = static_cast<wchar_t *>(::GlobalLock(h_data));
	if (nullptr == p_data) {
		::GlobalFree(h_data);
		return false;
	}

	memcpy(p_data, a_string.c_str(), textSize);
	if (::OpenClipboard(ah_wnd)) {
		::EmptyClipboard();
		::SetClipboardData(CF_UNICODETEXT, h_data);
		::CloseClipboard();
	}
	::GlobalUnlock(h_data);
	return true;
}