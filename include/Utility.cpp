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