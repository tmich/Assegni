#include "stdafx.h"
#include "wc_to_mb.h"


wc_to_mb::wc_to_mb(std::wstring wide_string)
	: m_wcs( wide_string )
{
}

wc_to_mb::~wc_to_mb()
{
	if (m_pmb)
		delete[] m_pmb;
}

std::string wc_to_mb::asStdString()
{
	size_t i;
	size_t sz = m_wcs.size() * sizeof(wchar_t);
	m_pmb = new char[sz];
	wcstombs_s(&i, m_pmb, (size_t)sz, m_wcs.c_str(), (size_t)sz);

	std::string s{ m_pmb };

	return s;
}


/******** mb_to_wc *************/

mb_to_wc::mb_to_wc(std::string mbstring)
	: m_str( mbstring )
{
}

mb_to_wc::~mb_to_wc()
{
	if (m_pws)
		delete[] m_pws;
}

std::wstring mb_to_wc::asStdWstring()
{
	size_t sz = m_str.size() + 1;
	m_pws = new wchar_t[sz];
	size_t i = 0;
	mbstowcs_s(&i, m_pws, sz, m_str.c_str(), sz - 1);

	return std::wstring(m_pws);
}
