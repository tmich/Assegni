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
	
	for (size_t i = 0; i < m_assegni.size(); i++)
	{
		int x = AddString(m_assegni[i].getNumero().c_str());
		SetItemData(x, i);
	}
	
	EnableWindow(GetCount() > 0);

	if (GetCount() > 0)
	{
		SetCurSel(0);
	}
}

Assegno CComboAssegni::GetSelectedItem() const
{
	int cursel = GetCurSel();
	int index = GetItemData(cursel);
	return m_assegni[index];
}
