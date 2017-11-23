#include "stdafx.h"
#include "dlgdettassegno.h"
#include "resource.h"
#include "assdao.h"
#include "azdao.h"
#include "ccdao.h"
#include "libdao.h"
#include <algorithm>
#include "utils.h"

DlgDettaglioAssegno::DlgDettaglioAssegno()
	: CDialog{ IDD_DETTASSEGNO }
{
	m_incassato = false;
	m_idAssegno = 0;
	m_sbloccato = false;
}

DlgDettaglioAssegno::DlgDettaglioAssegno(long idAssegno)
	: DlgDettaglioAssegno()
{
	m_idAssegno = idAssegno;
}

DlgDettaglioAssegno::~DlgDettaglioAssegno()
{
}

BOOL DlgDettaglioAssegno::OnInitDialog()
{
	AttachItem(IDC_CMBAZ, m_cmbAzienda);
	AttachItem(IDC_CMBCC, m_cmbConti);
	AttachItem(IDC_CMBLIBR, m_cmbLibretti);
	AttachItem(IDC_CMBNUMASS, m_cmbAssegni);
	//AttachItem(IDC_TXTINTASS, m_txtIntAss);
	AttachItem(IDC_CMBFORN, m_txtIntAss);
	AttachItem(IDC_TXTIMPASS, m_txtImpAss);
	AttachItem(IDC_TXTDECIMPASS, m_txtImpDecAss);
	AttachItem(IDC_TXTCAUSASS, m_txtCausAss);
	AttachItem(IDC_DTASS, m_dtAss);
	AttachItem(IDC_BTNSALVASS, m_btnSalvAss);
	AttachItem(IDC_DTINCASS, m_dtIncass);
	AttachItem(IDC_CHKINCASS, m_chkIncasso);
	AttachItem(IDC_DTSCAD, m_dtScadenza);
	AttachItem(IDC_BTNSBLOCCA, m_btnSblocca);
	AttachItem(IDC_BTNANNULLASS, m_btnAnnullaAss);

	m_txtImpAss.SetLimitText(7);
	m_txtImpDecAss.SetLimitText(2);
	m_dtIncass.SetWindowTextW(_T(""));

	m_btnAnnullaAss.EnableWindow(m_idAssegno > 0);
	
	::SendDlgItemMessage(*this, IDC_CHKINCASS, BM_SETCHECK, (m_incassato ? BST_CHECKED : BST_UNCHECKED), 0);
	m_dtIncass.EnableWindow(m_incassato);

	// Pulsante di sblocco
	m_btnSblocca.EnableWindow(false);
	m_btnSblocca.SetIcon(::LoadIcon(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_LOCK)));

	// data scadenza a null di default
	m_dtScadenza.SetTimeNone();

	m_cmbAssegni.ResetContent();
	
	if (m_idAssegno != 0)
	{
		//carico i dati dell'assegno
		DatiAssegno();
	}

	return 0;
}

BOOL DlgDettaglioAssegno::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CMBAZ:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			OnAzienda();
		break;
	case IDC_CMBCC:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			OnConto();
		break;
	case IDC_CMBLIBR:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			OnLibretto();
		break;
	case IDC_BTNSALVASS:
		OnSalva();
		break;
	case IDC_CHKINCASS:
		OnIncassato();
		break;
	case IDC_BTNSBLOCCA:
		OnBloccaSblocca();
		break;
	case IDC_BTNANNULLASS:
		OnAnnulla();
		break;
	}
	return 0;
}

void DlgDettaglioAssegno::OnSalva()
{
	AssegnoDao asdao;
	if (m_cmbAssegni.GetCurSel() < 0)
	{
		MessageBox(_T("N° assegno non valido"), _T("Attenzione"), MB_ICONEXCLAMATION);
		return;
	}

	Assegno apt = m_cmbAssegni.GetSelectedItem();
	std::wstring note = m_txtCausAss.GetWindowTextW().c_str();

	// è stato sbloccato?
	if (m_sbloccato)
	{
		try
		{
			asdao.annullaEmissione(apt);
		}
		catch (const std::exception&)
		{
			MessageBox(_T("Si è verificato un errore. Non sono riuscito ad annullare l'emissione"),
				_T("Gestione assegni"), MB_ICONERROR);
			EndDialog(IDCANCEL);
		}
	}

	if (!apt.emesso())
	{
		// beneficiario
		std::wstring beneficiario = m_txtIntAss.GetWindowTextW().c_str();

		// data emissione
		SYSTEMTIME dtEmis = m_dtAss.GetTime();
		if (dtEmis.wDay == 0 || dtEmis.wMonth == 0 || dtEmis.wYear == 0)
		{
			MessageBox(_T("Inserire la data di emissione"), _T("Attenzione"), MB_ICONEXCLAMATION);
			return;
		}
		unsigned int dayEmis, monthEmis, yearEmis;
		dayEmis = dtEmis.wDay;
		monthEmis = dtEmis.wMonth;
		yearEmis = dtEmis.wYear;

		// controllo data scadenza
		SYSTEMTIME dtScad = m_dtScadenza.GetTime();
		if (dtScad.wDay == 0 || dtScad.wMonth == 0 || dtScad.wYear == 0)
		{
			MessageBox(_T("Inserire la data di scadenza"), _T("Attenzione"), MB_ICONEXCLAMATION);
			return;
		}

		unsigned int dayScad, monthScad, yearScad;
		dayScad = dtScad.wDay;
		monthScad = dtScad.wMonth;
		yearScad = dtScad.wYear;

		std::wstring sImp = m_txtImpAss.GetWindowTextW() + _T(".") + m_txtImpDecAss.GetWindowTextW();
		wchar_t* end;
		double importo = std::wcstod(sImp.c_str(), &end);

		apt.emetti(beneficiario, dtm::date{ dayEmis,monthEmis,yearEmis }, 
			dtm::date{ dayScad,monthScad,yearScad }, importo, note);

		/*try
		{
			asdao.salva(*apt);
		}
		catch (const std::exception&)
		{
			MessageBox(_T("Si è verificato un errore. Non sono riuscito a salvare l'assegno."), 
				_T("Gestione assegni"), MB_ICONERROR);
			EndDialog(IDCANCEL);
		}*/
	}
	
	if (m_incassato)
	{
		SYSTEMTIME st2 = m_dtIncass.GetTime();
		dtm::date dtIncass{ st2.wDay, st2.wMonth, st2.wYear };
		apt.incassa(dtIncass);
	}
	
	apt.setNote(note);

	try
	{
		asdao.salva(apt);
		MessageBox(_T("Salvato"), _T("Gestione assegni"), MB_ICONINFORMATION);
	}
	catch (const std::exception&)
	{
		MessageBox(_T("Si è verificato un errore"), _T("Gestione assegni"), MB_ICONERROR);
		EndDialog(IDCANCEL);
	}

	PulisciCampi();

	// ricarico l'assegno successivo
	OnLibretto();

	//EndDialog(IDOK);
}

LRESULT DlgDettaglioAssegno::PulisciCampi()
{
	m_txtCausAss.SetWindowTextW(L"");
	m_txtIntAss.SetWindowTextW(L"");
	m_txtImpAss.SetWindowTextW(L"");
	m_txtImpDecAss.SetWindowTextW(L"");

	return 0L;
}

LRESULT DlgDettaglioAssegno::OnColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdcStatic = (HDC)wParam;
	HWND hwnd = (HWND)lParam;
	LONG id = ::GetWindowLong(hwnd, GWL_ID);
	if (id == IDC_STATIC)
	{
		CDC dc{ hdcStatic };
		dc.SetTextColor(RGB(0, 0, 0));
		dc.SetBkMode(TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}
	return 0;
}

void DlgDettaglioAssegno::OnDraw(CDC & dc)
{
	// Logo
	HICON hIcon = (HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_ASSEGNOROSA), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	::DrawIconEx(dc, 0, 0, hIcon, 210, 160, 0, NULL, DI_NORMAL);
	DestroyIcon(hIcon);
}

BOOL DlgDettaglioAssegno::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CTLCOLORSTATIC:	return OnColorStatic(uMsg, wParam, lParam);
	}

	return DialogProcDefault(uMsg, wParam, lParam);
}

void DlgDettaglioAssegno::OnAzienda()
{
	Azienda az = m_cmbAzienda.GetSelectedItem();
	m_cmbConti.Aggiorna(az);
}

void DlgDettaglioAssegno::OnConto()
{
	ContoCorrente cc = m_cmbConti.GetSelectedItem();
	m_cmbLibretti.Aggiorna(cc);
}

void DlgDettaglioAssegno::OnLibretto()
{
	Libretto lib = m_cmbLibretti.GetSelectedItem();
	
	AssegnoDao asdao;
	std::vector<Assegno> assegni = asdao.getByLibretto(lib.getId());
	std::vector<Assegno> assegniNonEmessi{ assegni.size() };
	auto it = std::copy_if(assegni.begin(), assegni.end(),
		assegniNonEmessi.begin(), [=](Assegno a) { return !a.emesso(); });
	assegniNonEmessi.resize(std::distance(assegniNonEmessi.begin(), it));	// shrink container to new size
	m_cmbAssegni.Aggiorna(assegniNonEmessi);
	m_txtIntAss.EnableWindow(true);
}

void DlgDettaglioAssegno::OnIncassato()
{
	LRESULT lres = ::SendDlgItemMessage(*this, IDC_CHKINCASS, BM_GETCHECK, 0, 0);
	m_incassato = (lres == BST_CHECKED);
	m_dtIncass.EnableWindow(m_incassato);
}

void DlgDettaglioAssegno::DatiAssegno()
{
	AssegnoDao asdao;
	Assegno assegno = asdao.getById(m_idAssegno);
	vAssegno.push_back(assegno);
	m_cmbAssegni.Aggiorna(vAssegno);
	m_cmbAssegni.SetCurSel(0);
	m_cmbAssegni.EnableWindow(false);
	
	std::wstring title{ _T("Assegno n° ") };
	title.append(assegno.getNumero());
	SetWindowText(title.c_str());

	LibrettoDao libdao;
	Libretto lib = libdao.getById(assegno.getIdLibretto());
	m_cmbLibretti.AddString(lib.toString().c_str());
	m_cmbLibretti.SetCurSel(0);

	ContoCorrenteDao ccdao;
	ContoCorrente cc = ccdao.GetById(lib.getIdConto());
	m_cmbConti.AddString(cc.toString().c_str());
	m_cmbConti.SetCurSel(0);

	AziendaDao azdao;
	Azienda az;
	azdao.getById(cc.getIdAzienda(), az);
	m_cmbAzienda.SetSelectedItem(az, true);

	m_txtIntAss.SetWindowTextW(assegno.getBeneficiario().c_str());
	m_txtIntAss.EnableWindow(false);

	// data emissione, data incasso
	SYSTEMTIME st1, st2, st3;
	dtm::date dtAss = assegno.getDataEmissione();
	::GetSystemTime(&st1);

	st1.wDay = dtAss.day();
	st1.wMonth = dtAss.month();
	st1.wYear = dtAss.year();
	m_dtAss.SetTime(st1);

	// data scadenza
	dtm::date dtScad = assegno.getDataScadenza();
	::GetSystemTime(&st3);
	st3.wDay = dtScad.day();
	st3.wMonth = dtScad.month();
	st3.wYear = dtScad.year();
	m_dtScadenza.SetTime(st3);

	auto importo = assegno.getImporto();
	std::wstring sImporto = std::to_wstring(assegno.getImporto());
	std::vector<std::wstring> tokens = utils::split(sImporto, _T("."));
	assert(tokens.size() >= 1);
	m_txtImpAss.SetWindowTextW(tokens[0].c_str());

	if (tokens.size() == 2)
	{
		m_txtImpDecAss.SetWindowTextW(tokens[1].substr(0, 2).c_str());
	}
	else
	{
		m_txtImpDecAss.SetWindowTextW(_T("00"));
	}

	m_txtCausAss.SetWindowTextW(assegno.getNote().c_str());

	// blocco la modifica
	m_dtAss.EnableWindow(false);
	m_dtScadenza.EnableWindow(false);
	m_txtImpAss.EnableWindow(false);
	m_txtImpDecAss.EnableWindow(false);

	// pulsante per sbloccare la modifica di un assegno emesso
	m_btnSblocca.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_LOCK), IMAGE_ICON, 48, 48, LR_SHARED)));
	m_btnSblocca.EnableWindow(true);

	// incassato?
	m_incassato = assegno.incassato();
	::SendDlgItemMessage(*this, IDC_CHKINCASS, BM_SETCHECK, (m_incassato ? BST_CHECKED : BST_UNCHECKED), 0);

	if (m_incassato)
	{
		// assegno incassato
		dtm::date dtIncass = assegno.getDataIncasso();
		::GetSystemTime(&st2);
		st2.wDay = dtIncass.day();
		st2.wMonth = dtIncass.month();
		st2.wYear = dtIncass.year();
		m_dtIncass.SetTime(st2);

		// non si modifica più
		m_dtIncass.EnableWindow(false);
		m_txtCausAss.EnableWindow(false);
		m_chkIncasso.EnableWindow(false);
		m_btnSalvAss.EnableWindow(false);
		m_btnSblocca.EnableWindow(false);
	}
}

void DlgDettaglioAssegno::OnBloccaSblocca()
{
	m_sbloccato = !m_sbloccato;

	if (m_sbloccato)
	{
		m_btnSblocca.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_UNLOCK), IMAGE_ICON, 48, 48, LR_SHARED)));
	}
	else
	{
		m_btnSblocca.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_LOCK), IMAGE_ICON, 48, 48, LR_SHARED)));
	}

	// sblocco/blocco i campi
	m_txtIntAss.EnableWindow(m_sbloccato);
	m_dtAss.EnableWindow(m_sbloccato);
	m_dtScadenza.EnableWindow(m_sbloccato);
	m_txtImpAss.EnableWindow(m_sbloccato);
	m_txtImpDecAss.EnableWindow(m_sbloccato);
	m_btnSalvAss.EnableWindow(m_sbloccato);
}

void DlgDettaglioAssegno::OnAnnulla()
{
	if (MessageBox(_T("Annullare l'assegno?"), _T("Attenzione"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
	{
		AssegnoDao asdao;
		Assegno apt = m_cmbAssegni.GetSelectedItem();
		apt.annulla();
		asdao.salva(apt);
		MessageBox(_T("Assegno annullato"), _T("Conferma"), MB_ICONINFORMATION);
		//::SendMessage(*this, WM_CLOSE, 0, 0);
		EndDialog(IDOK);
	}
}
