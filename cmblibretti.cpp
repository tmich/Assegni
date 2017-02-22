#include "stdafx.h"
#include "cmblibretti.h"
#include "contocorrente.h"
#include "libdao.h"

CComboLibretti::CComboLibretti()
{
}

CComboLibretti::~CComboLibretti()
{
}

void CComboLibretti::Aggiorna(const ContoCorrente & cc)
{
	ResetContent();
	LibrettoDao ldao;
	m_libretti = ldao.getByConto(cc);
	for (size_t i = 0; i < m_libretti.size(); i++)
	{
		int x = AddString(m_libretti[i].toString().c_str());
		SetItemData(x, i);
	}

	EnableWindow(GetCount() > 0);
}

Libretto CComboLibretti::GetSelectedItem() const
{
	int cursel = GetCurSel();
	int index = GetItemData(cursel);
	return m_libretti[index];
}