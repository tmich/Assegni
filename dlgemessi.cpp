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
#include "utils.h"
#include <wxx_printdialogs.h>
#include <wxx_rect.h>

DlgAssegniEmessi::DlgAssegniEmessi()
	: CDialog{ IDD_ASSEGNIEMESSI }
{
	m_TotaleDaIncassare = 0.0;
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
	AttachItem(IDC_TXTTOTALE, m_txtTotale);
	AttachItem(IDC_BTNSTAMPA, m_btnStampa);

	/*m_listAssegni.InsertColumn(0, _T("Scadenza"), 0, 120);
	m_listAssegni.InsertColumn(1, _T("Numero"), 0, 160);
	m_listAssegni.InsertColumn(2, _T("Intestato a"), 0, 300);
	m_listAssegni.InsertColumn(3, _T("Importo"), 0, 100);
	m_listAssegni.InsertColumn(4, _T("Libretto"), 0, 120);
	m_listAssegni.InsertColumn(5, _T("Azienda"), 0, 120);
	m_listAssegni.SetExtendedStyle(LVS_EX_FULLROWSELECT);*/

	m_btnStampa.EnableWindow(FALSE);

	m_cmbConti.Aggiorna();

	//OnCerca();
	//this->SendDlgItemMessage(IDC_BTNCERCAEMESSI, BM_CLICK, 0, 0);

	return 0;
}

BOOL DlgAssegniEmessi::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BTNCERCAEMESSI:
		OnCerca();
		break;
	case IDC_BTNSTAMPA:
		OnStampa();
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
		break;
	}

	return 0;
}

void DlgAssegniEmessi::OnCerca()
{
	services::Service service;
	unsigned int annoDal = 0, meseDal = 0, annoAl = 0, meseAl = 0;

	m_listAssegni.DeleteAllItems();
	annoDal = m_cmbAnnoDal.GetSelectedItem();
	meseDal = m_cmbMeseDal.GetSelectedItem();
	annoAl = m_cmbAnnoAl.GetSelectedItem();
	meseAl = m_cmbMeseAl.GetSelectedItem();
	
	try
	{
		ContoCorrente cc = m_cmbConti.GetSelectedItem();
		assegniEmessi = service.GetAssegniEmessi(cc);
	}
	catch (const NotFoundException&)
	{
		// conto corrente non selezionato, cerco tutti gli assegni emessi
		try
		{
			assegniEmessi = service.GetAssegniEmessi();
		}
		catch (const std::exception&)
		{
			MessageBox(_T("Errore"), _T("Eccezione 1"), MB_ICONEXCLAMATION);
		}
		
	}
	catch (const std::exception&)
	{
		MessageBox(_T("Errore"), _T("Eccezione 2"), MB_ICONEXCLAMATION);
	}
	
	// filtro per data scadenza
	if (meseDal == 0)
		meseDal = 1;
		
	if (annoDal == 0)
		annoDal = 2017;
		
	if (annoAl == 0)
		annoAl = 3000;
		
	if (meseAl == 0)
		meseAl = 12;
		
	unsigned int day = dtm::daysInMonth(meseAl, annoAl);
	dtm::date dataDal{ 1, meseDal, annoDal };
	dtm::date dataAl{ day, meseAl, annoAl };

	assegniFiltrati.resize(assegniEmessi.size());
	//vector<Assegno> filtered{ assegniEmessi.size() };
	auto it = std::copy_if(assegniEmessi.begin(), assegniEmessi.end(), assegniFiltrati.begin(), [&](const Assegno& ass) {
		return (ass.getDataScadenza() >= dataDal && ass.getDataScadenza() <= dataAl);
	});
	assegniFiltrati.erase(it, assegniFiltrati.end());

	// ordino per data scadenza
	std::sort(assegniFiltrati.begin(), assegniFiltrati.end(), [](const Assegno& a1, const Assegno& a2) { 
		return a1.getDataScadenza() < a2.getDataScadenza(); });
	
	m_listAssegni.SetItems(assegniFiltrati);

	// calcolo il totale
	for each (const Assegno& ass in assegniFiltrati)
	{
		m_TotaleDaIncassare += ass.getImporto();
	}
	m_txtTotale.SetWindowTextW(utils::format(m_TotaleDaIncassare).c_str());

	m_btnStampa.EnableWindow(assegniFiltrati.size() > 0);
}

void DlgAssegniEmessi::OnStampa()
{
	// Copy the bitmap from the View window
	/*CClientDC dcView(GetView());
	CMemDC MemDC(dcView);
	CBitmap bmView;
	bmView.CreateCompatibleBitmap(dcView, Width, Height);
	MemDC.SelectObject(bmView);
	MemDC.BitBlt(0, 0, Width, Height, dcView, 0, 0, SRCCOPY);*/

	const int CHARS_PER_LINE = 90;

	CPrintDialog printDlg;
	if (printDlg.DoModal(*this) == IDOK)
	{
		// Zero and then initialize the members of a DOCINFO structure.
		DOCINFOW di = { 0 };
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = _T("Assegni da incassare");
		di.lpszOutput = (LPTSTR)NULL;
		di.lpszDatatype = (LPTSTR)NULL;
		di.fwType = 0;
		CDC printerDC = printDlg.GetPrinterDC();
		// Begin a print job by calling the StartDoc function.
		printerDC.StartDocW(&di);
		// Inform the driver that the application is about to begin sending data.
		printerDC.StartPage();
		//printerDC.Rectangle(100, 100, 200, 200);
		int startY = 150;
		dtm::date oggi = dtm::date::date();
		std::wostringstream titolo;
		titolo << L"Assegni da incassare al ";
		titolo << oggi.day() << L"/" << oggi.month() << L"/" << oggi.year();
		printerDC.TextOutW(100, startY, titolo.str().c_str());
		
		// linea
		std::wostringstream linea;
		for (int x = 0; x < 95; x++)
		{
			linea << L"_";
		}
		startY += 100;
		/*CPoint ptBegin{ 100, startY };
		CPoint ptEnd{ 4500,startY };*/
		//printerDC.TextOutW(100, startY, linea.str().c_str());
		// fine linea
		
		startY += 100;
		for each (const Assegno& ass in assegniFiltrati)
		{
			startY += 100;
			std::wostringstream sb;
			sb << "N° " << ass.getNumero() << " " << ass.getBeneficiario() << L" (" << ass.getNote() << L")";
			printerDC.TextOutW(100, startY, sb.str().c_str());

			int theX = 4500;
			if (ass.getImporto() > 999.99)
				theX -= 52;
			printerDC.TextOutW(theX, startY, utils::format(ass.getImporto()).c_str());
			
			startY += 85;
			printerDC.TextOutW(100, startY, linea.str().c_str());
		}
		
		// totale
		startY += 100;
		std::wstringstream tot;
		tot << L"TOTALE: ";
		printerDC.TextOutW(100, startY, tot.str().c_str());
		int theX = 4500;
		if (m_TotaleDaIncassare > 999.99)
			theX -= 52;
		printerDC.TextOutW(theX, startY, utils::format(m_TotaleDaIncassare).c_str());
		
		printerDC.EndPage();
		printerDC.EndDoc();
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
	//int i = m_listAssegni.GetItemCount();
	//std::wstring numero = a.getNumero();
	//std::wstring intest = a.getBeneficiario();
	//dtm::date dtEmess = a.getDataEmissione();
	//std::wstring importo = utils::format(a.getImporto());
	//std::wostringstream ss;
	//ss << std::setw(2) << std::setfill(_T('0')) << dtEmess.day() << _T('/')
	//	<< std::setw(2) << std::setfill(_T('0')) << dtEmess.month() << _T('/')
	//	<< dtEmess.year();
	//std::wstring dataEmissione = ss.str();

	//dtm::date dtScadenza = a.getDataScadenza();
	//std::wostringstream s2;
	//s2 << std::setw(2) << std::setfill(_T('0')) << dtScadenza.day() << _T('/')
	//	<< std::setw(2) << std::setfill(_T('0')) << dtScadenza.month() << _T('/')
	//	<< dtScadenza.year();
	//std::wstring dataScadenza = s2.str();
	//
	//LVITEM lv{ 0 };

	//// data scadenza
	//lv.iItem = i;
	//lv.mask = LVFIF_TEXT;
	//lv.pszText = (LPWSTR)(dataScadenza.c_str());
	//m_listAssegni.InsertItem(lv);

	//// numero
	//lv.iSubItem = 1;
	//lv.pszText = (LPWSTR)numero.c_str();
	//m_listAssegni.SetItem(lv);

	//// intestatario
	//lv.iSubItem = 2;
	//lv.pszText = (LPWSTR)intest.c_str();
	//m_listAssegni.SetItem(lv);

	//// importo
	//lv.iSubItem = 3;
	//lv.pszText = (LPWSTR)importo.c_str();
	//m_listAssegni.SetItem(lv);

	//// libretto
	//LibrettoDao libdao;
	//Libretto l = libdao.getById(a.getIdLibretto());
	//std::wstring codiceLibretto{ l.getCodice() };
	//lv.iSubItem = 4;
	//lv.pszText = (LPWSTR)(codiceLibretto.c_str());
	//m_listAssegni.SetItem(lv);

	//// assegno
	//AziendaDao azdao;
	//ContoCorrenteDao ccdao;
	//ContoCorrente cc = ccdao.GetById(l.getIdConto());
	//Azienda az;
	//azdao.getById(cc.getIdAzienda(), az);
	//lv.iSubItem = 5;
	//std::wstring azDenom{ az.getRagioneSociale() };
	//lv.pszText = (LPWSTR)(azDenom.c_str());
	//m_listAssegni.SetItem(lv);

	//m_listAssegni.SetItemData(i, (DWORD_PTR)&a);
	m_listAssegni.Aggiungi(a);
}
