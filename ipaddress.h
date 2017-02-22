#pragma once
#include <string>
#include "wc_to_mb.h"

class IpAddress
{
public:
	IpAddress(std::wstring);
	IpAddress(int field0, int field1, int field2, int field3);
	virtual ~IpAddress();

	std::wstring str() { return mIp; }
	std::string mb_str();

	BYTE Field0() const;
	BYTE Field1() const;
	BYTE Field2() const;
	BYTE Field3() const;
protected:
	std::wstring mIp;
	int m0 = 0, m1 = 0, m2 = 0, m3 = 0;
};