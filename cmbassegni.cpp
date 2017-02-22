#include "stdafx.h"
#include "cmbassegni.h"
#include <algorithm>

CComboAssegni::CComboAssegni()
{
}

CComboAssegni::~CComboAssegni()
{
}

void CComboAssegni::Aggiorna(const std::vector<Assegno> assegni)
{
	m_assegni.resize(assegni.size());
	std::copy(assegni.begin(), assegni.end(), m_assegni.begin());
	ResetContent();
	int i = 0;

	for (const auto& a : m_assegni)
	{
		AddString(a.getNumero().c_str());
		SetItemData(i++, (DWORD)&a);
	}
	
	if (GetCount() > 0)
	{
		SetCurSel(0);
		EnableWindow();
	}
}

Assegno * CComboAssegni::GetSelectedItem() const
{
	if (GetCurSel() >= 0)
	{
		return (Assegno*)GetItemData(GetCurSel());
	}
	else
	{
		return nullptr;
	}
}
