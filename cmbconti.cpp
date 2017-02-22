#include "stdafx.h"
#include "cmbconti.h"
#include "ccdao.h"
#include "azienda.h"

CComboConti::CComboConti()
{
}

CComboConti::~CComboConti()
{
}

void CComboConti::Aggiorna(const Azienda & az)
{
	ResetContent();
	ContoCorrenteDao ccdao;
	m_conti = ccdao.GetByAzienda(az);
	
	for (size_t i = 0; i < m_conti.size(); i++)
	{
		int x = AddString(m_conti[i].toString().c_str());
		SetItemData(x, i);
	}

	EnableWindow(GetCount() > 0);

	SetCurSel(-1);
}

ContoCorrente CComboConti::GetSelectedItem() const
{
	int cursel = GetCurSel();
	int index = GetItemData(cursel);
	return m_conti[index];
}
