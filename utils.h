#pragma once
#include <string>
#include <vector>

namespace utils
{
	std::vector<std::wstring> split(const std::wstring& str, std::wstring delimiter);
	std::wstring format(double val);
}