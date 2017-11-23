#include "stdafx.h"
#include "dlgScegliLibretto.h"
#include "dlglibretto.h"
#include "resource.h"

DlgScegliLibretto::DlgScegliLibretto()
	: CDialog(IDD_SCEGLILIB)
{
}

DlgScegliLibretto::~DlgScegliLibretto()
{
}

BOOL DlgScegliLibretto::OnInitDialog()
{
	AttachItem(IDC_CMBAZSCLIB, m_cmbAzienda);
	AttachItem(IDC_CMBSCCCLIB, m_cmbConti);
	AttachItem(IDC_CMBSCLIBR, m_cmbLibretti);

	return 0;
}

BOOL DlgScegliLibretto::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CMBAZSCLIB:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			OnAzienda();
		break;
	case IDC_CMBSCCCLIB:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			OnConto();
		break;
	case IDC_BTNVAILIB:
		if (m_cmbLibretti.GetCurSel() >= 0)
		{
			auto libretto = m_cmbLibretti.GetSelectedItem();
			DlgLibretto dlgLib{ libretto.getId() };
			dlgLib.DoModal(*this);
			EndDialog(IDOK);
		}
	}
	return 0;
}

void DlgScegliLibretto::OnAzienda()
{
	Azienda az = m_cmbAzienda.GetSelectedItem();
	m_cmbConti.Aggiorna(az);
}

void DlgScegliLibretto::OnConto()
{
	ContoCorrente cc = m_cmbConti.GetSelectedItem();
	m_cmbLibretti.Aggiorna(cc);
}
