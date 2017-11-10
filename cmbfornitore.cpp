#include "stdafx.h"
#include "cmbfornitore.h"


CComboFornitore::CComboFornitore()
{
}


CComboFornitore::~CComboFornitore()
{
}

Fornitore CComboFornitore::GetSelectedItem() const
{
	return Fornitore();
}

void CComboFornitore::OnInitialUpdate()
{
	FornitoreDao forndao;
	m_fornitori = forndao.getAll();

	int cursel = -1;

	for (size_t i = 0; i < m_fornitori.size(); i++)
	{
		int x = AddString(m_fornitori[i].getDenominazione().c_str());
		if (m_fornitori[i].getId() == m_preSelectedId)
		{
			cursel = i;
		}
		SetItemData(x, i);
	}

	SetCurSel(cursel);
	EnableWindow(m_enabled);
}
