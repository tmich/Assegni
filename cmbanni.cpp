#include "stdafx.h"
#include "cmbanni.h"
#include <vector>
#include "datetime.h"

CComboAnni::CComboAnni()
{
}


CComboAnni::~CComboAnni()
{
}

unsigned int CComboAnni::GetSelectedItem()
{
	if (GetCurSel() >= 0)
	{
		return (unsigned int)GetItemData(GetCurSel());
	}
	return 0;
}

void CComboAnni::OnInitialUpdate()
{
	dtm::date now;
	int y = now.year();
	while (y >= 2017)
	{
		int i = AddString(std::to_wstring(y).c_str());
		SetItemData(i, DWORD(y));
		y--;
	}
}
