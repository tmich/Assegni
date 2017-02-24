#include "stdafx.h"
#include "dlgemessi.h"
#include "dlgdettassegno.h"
#include "resource.h"
#include "services.h"
#include <sstream>
#include "utils.h"
#include <algorithm>

DlgAssegniEmessi::DlgAssegniEmessi()
	: CDialog{ IDD_ASSEGNIEMESSI }
{
}


DlgAssegniEmessi::~DlgAssegniEmessi()
{
}

BOOL DlgAssegniEmessi::OnInitDialog()
{
	AttachItem(IDC_CMBAZEM, m_cmbAziende);
	AttachItem(IDC_CMBMESIEM, m_cmbMese);
	AttachItem(IDC_CMBANNIEM, m_cmbAnno);
	AttachItem(IDC_LISTASSEGNIEMESSI, m_listAssegni);

	m_listAssegni.InsertColumn(0, _T("Scadenza"), 0, 100);
	m_listAssegni.InsertColumn(1, _T("Numero"), 0, 150);
	m_listAssegni.InsertColumn(2, _T("Intestato a"), 0, 280);
	m_listAssegni.InsertColumn(3, _T("Importo"), 0, 100);
	m_listAssegni.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return 0;
}

BOOL DlgAssegniEmessi::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BTNCERCAEMESSI:
		OnCerca();
		break;
	}
	return 0;
}

LRESULT DlgAssegniEmessi::OnNotify(WPARAM wParam, LPARAM lParam)
{
	long id = 0;
	switch (LOWORD(wParam))
	{
	case IDC_LISTASSEGNIEMESSI:
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_DBLCLK:
			OnDblClick(lParam);
			break;
		}
	}

	return 0;
}

void DlgAssegniEmessi::OnCerca()
{
	m_listAssegni.DeleteAllItems();
	Azienda az = m_cmbAziende.GetSelectedItem();
	unsigned short mese = m_cmbMese.GetSelectedItem();
	unsigned int anno = m_cmbAnno.GetSelectedItem();
	
	services::Service service;
	assegniEmessi = service.GetAssegniEmessi(az, anno, mese);
	std::sort(assegniEmessi.begin(), assegniEmessi.end(), [](const Assegno& a1, const Assegno& a2) { return a1.getDataScadenza() < a2.getDataScadenza(); });
	
	for (const auto& a : assegniEmessi)
	{
		Aggiungi(a);
	}
}

void DlgAssegniEmessi::OnDblClick(LPARAM lParam)
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
	Assegno *as = (Assegno*)(m_listAssegni.GetItemData(lpnmitem->iItem));
	int id = as->getId();
	if (id > 0)
	{
		DlgDettaglioAssegno dlgDett{ id };
		INT_PTR res = dlgDett.DoModal(*this);
		if (res == IDOK)
		{
			OnCerca();
		}
	}
}

void DlgAssegniEmessi::Aggiungi(const Assegno & a)
{
	int i = m_listAssegni.GetItemCount();
	std::wstring numero = a.getNumero();
	std::wstring intest = a.getBeneficiario();
	dtm::date dtEmess = a.getDataEmissione();
	std::wstring importo = utils::format(a.getImporto());
	std::wostringstream ss;
	ss << std::setw(2) << std::setfill(_T('0')) << dtEmess.day() << _T('/')
		<< std::setw(2) << std::setfill(_T('0')) << dtEmess.month() << _T('/')
		<< dtEmess.year();
	std::wstring dataEmissione = ss.str();

	dtm::date dtScadenza = a.getDataScadenza();
	std::wostringstream s2;
	s2 << std::setw(2) << std::setfill(_T('0')) << dtScadenza.day() << _T('/')
		<< std::setw(2) << std::setfill(_T('0')) << dtScadenza.month() << _T('/')
		<< dtScadenza.year();
	std::wstring dataScadenza = s2.str();

	LVITEM lv{ 0 };
	lv.iItem = i;
	lv.mask = LVFIF_TEXT;
	lv.pszText = (LPWSTR)(dataScadenza.c_str());
	m_listAssegni.InsertItem(lv);

	lv.iSubItem = 1;
	lv.pszText = (LPWSTR)numero.c_str();
	m_listAssegni.SetItem(lv);

	lv.iSubItem = 2;
	lv.pszText = (LPWSTR)intest.c_str();
	m_listAssegni.SetItem(lv);

	lv.iSubItem = 3;
	lv.pszText = (LPWSTR)importo.c_str();
	m_listAssegni.SetItem(lv);

	m_listAssegni.SetItemData(i, (DWORD_PTR)&a);
}
