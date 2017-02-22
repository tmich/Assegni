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
	m_conti.clear();
	ResetContent();
	ContoCorrenteDao ccdao;
	m_conti = ccdao.GetByAzienda(az);
	int i = 0;
	for (const auto& cc : m_conti)
	{
		AddString(cc.toString().c_str());
		SetItemData(i++, (DWORD)&cc);
	}

	if (GetCount() > 0)
	{
		EnableWindow();
	}

	SetCurSel(-1);
}

ContoCorrente * CComboConti::GetSelectedItem() const
{
	int cursel = GetCurSel();
	if (cursel >= 0)
	{
		return (ContoCorrente *)GetItemData(cursel);
	}
	return nullptr;
}
