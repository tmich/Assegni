#include "stdafx.h"
#include "dlgemessi.h"
#include "dlgdettassegno.h"
#include "resource.h"
#include "services.h"
#include <sstream>
#include "utils.h"
#include <algorithm>
#include "notfoundex.h"
#include "libdao.h"
#include "ccdao.h"
#include "azdao.h"

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
	AttachItem(IDC_CMBMESIEM, m_cmbMeseDal);
	AttachItem(IDC_CMBANNIEM, m_cmbAnnoDal);
	AttachItem(IDC_CMBMESIEM2, m_cmbMeseAl);
	AttachItem(IDC_CMBANNIEM2, m_cmbAnnoAl);
	AttachItem(IDC_LISTASSEGNIEMESSI, m_listAssegni);
	AttachItem(IDC_CMBBANCHEM, m_cmbConti);

	m_listAssegni.InsertColumn(0, _T("Scadenza"), 0, 120);
	m_listAssegni.InsertColumn(1, _T("Numero"), 0, 160);
	m_listAssegni.InsertColumn(2, _T("Intestato a"), 0, 300);
	m_listAssegni.InsertColumn(3, _T("Importo"), 0, 100);
	m_listAssegni.InsertColumn(4, _T("Libretto"), 0, 120);
	m_listAssegni.InsertColumn(5, _T("Azienda"), 0, 120);
	m_listAssegni.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_cmbConti.Aggiorna();

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
	services::Service service;
	unsigned int annoDal = 0, meseDal = 0, annoAl = 0, meseAl = 0;

	try
	{
		//Azienda az = m_cmbAziende.GetSelectedItem();
		//assegniEmessi = service.GetAssegniEmessi(az, annoDal, meseDal, annoAl, meseAl);

		m_listAssegni.DeleteAllItems();
		annoDal = m_cmbAnnoDal.GetSelectedItem();
		meseDal = m_cmbMeseDal.GetSelectedItem();
		annoAl = m_cmbAnnoAl.GetSelectedItem();
		meseAl = m_cmbMeseAl.GetSelectedItem();

		ContoCorrente cc = m_cmbConti.GetSelectedItem();
		assegniEmessi = service.GetAssegniEmessi(cc, annoDal, meseDal, annoAl, meseAl);
	}
	catch (const NotFoundException&)
	{
		// conto corrente non selezionato, cerco tutti gli assegni per date
		assegniEmessi = service.GetAssegniEmessi(annoDal, meseDal, annoAl, meseAl);
	}
	catch (const std::exception&)
	{
		MessageBox(_T("Errore"), _T("Eccezione"), MB_ICONEXCLAMATION);
	}
	

	// ordino per data scadenza
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

	// data scadenza
	lv.iItem = i;
	lv.mask = LVFIF_TEXT;
	lv.pszText = (LPWSTR)(dataScadenza.c_str());
	m_listAssegni.InsertItem(lv);

	// numero
	lv.iSubItem = 1;
	lv.pszText = (LPWSTR)numero.c_str();
	m_listAssegni.SetItem(lv);

	// intestatario
	lv.iSubItem = 2;
	lv.pszText = (LPWSTR)intest.c_str();
	m_listAssegni.SetItem(lv);

	// importo
	lv.iSubItem = 3;
	lv.pszText = (LPWSTR)importo.c_str();
	m_listAssegni.SetItem(lv);

	// libretto
	LibrettoDao libdao;
	Libretto l = libdao.getById(a.getIdLibretto());
	std::wstring codiceLibretto{ l.getCodice() };
	lv.iSubItem = 4;
	lv.pszText = (LPWSTR)(codiceLibretto.c_str());
	m_listAssegni.SetItem(lv);

	// assegno
	AziendaDao azdao;
	ContoCorrenteDao ccdao;
	ContoCorrente cc = ccdao.GetById(l.getIdConto());
	Azienda az;
	azdao.getById(cc.getIdAzienda(), az);
	lv.iSubItem = 5;
	std::wstring azDenom{ az.getRagioneSociale() };
	lv.pszText = (LPWSTR)(azDenom.c_str());
	m_listAssegni.SetItem(lv);

	m_listAssegni.SetItemData(i, (DWORD_PTR)&a);
}
