#include "stdafx.h"
#include "cmbazienda.h"

CComboAziende::CComboAziende()
{
	m_preSelectedId = 0;
	m_enabled = true;
}

CComboAziende::~CComboAziende()
{
}

Azienda * CComboAziende::GetSelectedItem() const
{
	int cursel = GetCurSel();
	if (cursel >= 0)
	{
		return (Azienda *)GetItemData(cursel);
	}
	return nullptr;
}

void CComboAziende::SetSelectedItem(const Azienda & azienda, bool disabled)
{
	m_preSelectedId = azienda.getId();
	m_enabled = !disabled;
}

void CComboAziende::OnInitialUpdate()
{
	AziendaDao azdao;
	m_aziende = azdao.all();

	int cursel = -1;
	int i = 0;
	for (const auto& az : m_aziende)
	{
		AddString(az.getRagioneSociale().c_str());
		if (az.getId() == m_preSelectedId)
		{
			cursel = i;
		}
		SetItemData(i++, (DWORD)&az);
	}
	
	SetCurSel(cursel);
	EnableWindow(m_enabled);
}
