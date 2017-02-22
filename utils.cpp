#include "stdafx.h"
#include "utils.h"

std::vector<std::wstring> utils::split(const std::wstring & str, std::wstring delimiter)
{
	std::wstring myString{ str };
	std::vector<std::wstring> tokens;
	size_t pos = 0;
	std::wstring token;

	while ((pos = myString.find(delimiter)) != std::wstring::npos) {
		token = myString.substr(0, pos);
		tokens.push_back(token);
		myString.erase(0, pos + delimiter.length());
	}
	tokens.push_back(myString);
	return tokens;
}

std::wstring utils::format(double val)
{
	std::wstring ws = std::to_wstring(val);
	std::vector<std::wstring> tokens = utils::split(ws, _T("."));
	return std::wstring{ tokens[0] + _T(",") + tokens[1].substr(0,2) };
}
