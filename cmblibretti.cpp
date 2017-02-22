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
	m_libretti.clear();
	LibrettoDao ldao;
	this->m_libretti = ldao.getByConto(cc);
	int i = 0;
	ResetContent();
	for (const auto& lib : m_libretti)
	{
		AddString(lib.toString().c_str());
		this->SetItemData(i++, (DWORD)&lib);
	}

	if (GetCount() > 0)
	{
		EnableWindow();
	}
}

Libretto * CComboLibretti::GetSelectedItem() const
{
	int cursel = GetCurSel();
	if (cursel >= 0)
	{
		return (Libretto *)GetItemData(cursel);
	}
	return nullptr;
}
