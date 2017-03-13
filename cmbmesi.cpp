#include "stdafx.h"
#include "cmbmesi.h"
#include <algorithm>

unsigned short CComboMesi::GetSelectedItem()
{
	if (GetCurSel() >= 0)
	{
		return (unsigned short)GetItemData(GetCurSel());
	}
	return 0;
}

void CComboMesi::OnInitialUpdate()
{
	std::map<unsigned short, std::wstring> m_mesi
	{
		std::pair<unsigned short, std::wstring>{0, _T("(tutti)")},
		std::pair<unsigned short, std::wstring>{1, _T("Gennaio")},
		std::pair<unsigned short, std::wstring>{2, _T("Febbraio")},
		std::pair<unsigned short, std::wstring>{3, _T("Marzo")},
		std::pair<unsigned short, std::wstring>{4, _T("Aprile")},
		std::pair<unsigned short, std::wstring>{5, _T("Maggio")},
		std::pair<unsigned short, std::wstring>{6, _T("Giugno")},
		std::pair<unsigned short, std::wstring>{7, _T("Luglio")},
		std::pair<unsigned short, std::wstring>{8, _T("Agosto")},
		std::pair<unsigned short, std::wstring>{9, _T("Settembre")},
		std::pair<unsigned short, std::wstring>{10, _T("Ottobre")},
		std::pair<unsigned short, std::wstring>{11, _T("Novembre")},
		std::pair<unsigned short, std::wstring>{12, _T("Dicembre")}
	};
	
	for (auto const& m : m_mesi)
	{
		int i = AddString(m.second.c_str());
		SetItemData(i, (DWORD)m.first);
	}
}
