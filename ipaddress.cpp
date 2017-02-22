#include "stdafx.h"
#include "ipaddress.h"
#include <sstream>

IpAddress::IpAddress(std::wstring ip)
	: mIp{ ip }
{
	std::wstringstream s(ip);
	wchar_t ch;			//to temporarily store the '.'
	s >> m0 >> ch >> m1 >> ch >> m2 >> ch >> m3;
}

IpAddress::IpAddress(int field0, int field1, int field2, int field3)
	: m0{ field0 }, m1{ field1 }, m2{ field2 }, m3{ field3 }
{
	std::wstringstream s;
	wchar_t ch{ '.' };
	s << m0 << ch << m1 << ch << m2 << ch << m3;
	mIp = s.str();
}

IpAddress::~IpAddress()
{
}

std::string IpAddress::mb_str()
{
	wc_to_mb wc2mb{ mIp };
	return wc2mb.asStdString();
}

BYTE IpAddress::Field0() const
{
	return m0;
}

BYTE IpAddress::Field1() const
{
	return m1;
}

BYTE IpAddress::Field2() const
{
	return m2;
}

BYTE IpAddress::Field3() const
{
	return m3;
}
