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

Azienda CComboAziende::GetSelectedItem() const
{
	int cursel = GetCurSel();
	int index = GetItemData(cursel);
	return m_aziende[index];
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
	
	for (size_t i = 0; i < m_aziende.size(); i++)
	{
		int x = AddString(m_aziende[i].getRagioneSociale().c_str());
		if (m_aziende[i].getId() == m_preSelectedId)
		{
			cursel = i;
		}
		SetItemData(x, i);
	}
	
	SetCurSel(cursel);
	EnableWindow(m_enabled);
}
