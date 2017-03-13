#include "stdafx.h"
#include "dlglibretto.h"
#include "resource.h"
#include <sstream>
#include "libretto.h"
#include "libdao.h"
#include "assdao.h"
#include "azdao.h"
#include "ccdao.h"
#include <boost\algorithm\string.hpp>

DlgLibretto::DlgLibretto()
	:CDialog{ IDD_LIBRETTO }
{
	m_idLibretto = 0;
	m_readOnly = false;
}

DlgLibretto::DlgLibretto(long idLibretto)
	: DlgLibretto()
{
	m_idLibretto = idLibretto;
	m_readOnly = true;
}

DlgLibretto::~DlgLibretto()
{
}

BOOL DlgLibretto::OnInitDialog()
{
	AttachItem(IDC_CMBAZLIB, m_cmbAziende);
	AttachItem(IDC_CMBCCLIB, m_cmbConti);
	AttachItem(IDC_BTNCALCNUMASS, m_btnCalcolaNumeri);
	AttachItem(IDC_TXTNLIB, m_txtLibretto);
	AttachItem(IDC_TXTPRIMASS, m_txtAssegno);
	AttachItem(IDC_LISTASSEGNI, m_lstAssegni);
	AttachItem(IDC_TXTCODLIB, m_txtCodice);
	AttachItem(IDC_RB10, m_rbQta10);
	AttachItem(IDC_RB20, m_rbQta20);

	::SendDlgItemMessage(*this, IDC_RB10, BM_SETCHECK, BST_CHECKED, 0);
	m_qta = 10;

	if (m_idLibretto > 0)
	{
		DatiLibretto();
	}

	return 0;
}

BOOL DlgLibretto::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CMBAZLIB:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			OnAzienda();
		break;
	case IDC_BTNCALCNUMASS:
		OnCalcolaNumeri();
		break;
	case IDC_RB10:
		m_qta = 10;
		break;
	case IDC_RB20:
		m_qta = 20;
		break;
	case IDC_BTNSALVALIB:
		OnOK();
		break;
	}
	return 0;
}

void DlgLibretto::OnOK()
{
	// controlli
	std::wstring codLib = m_txtCodice.GetWindowTextW();
	std::wstring nLib = m_txtLibretto.GetWindowTextW();

	boost::trim(codLib);
	boost::trim(nLib);

	if (m_cmbAziende.GetCurSel() < 0)
	{
		Errore(_T("Manca l'azienda"));
		return;
	}
	else if (m_cmbConti.GetCurSel() < 0)
	{
		Errore(_T("Manca il conto corrente d'appoggio"));
		return;
	}
	else if (nLib.size() == 0)
	{
		Errore(_T("Manca il numero del primo assegno"));
		return;
	}
	else if (codLib.size() == 0)
	{
		Errore(_T("Manca il codice del libretto"));
		return;
	}
	else if(m_lstAssegni.GetCount() == 0)
	{
		Errore(_T("Mancano i numeri degli assegni"));
		return;
	}
	else
	{
		Salva();
		CDialog::OnOK();
	}
}

void DlgLibretto::DatiLibretto()
{
	LibrettoDao libdao;
	Libretto lib = libdao.getById(m_idLibretto);

	std::wstring title{ _T("Libretto n° ") };
	title.append(lib.getNumero());
	SetWindowText(title.c_str());

	m_txtLibretto.SetWindowTextW(lib.getNumero().c_str());
	m_txtCodice.SetWindowTextW(lib.getCodice().c_str());

	ContoCorrenteDao ccdao;
	ContoCorrente cc = ccdao.GetById(lib.getIdConto());
	m_cmbConti.AddString(cc.toString().c_str());
	m_cmbConti.SetCurSel(0);

	AziendaDao azdao;
	Azienda az;
	azdao.getById(cc.getIdAzienda(), az);
	m_cmbAziende.SetSelectedItem(az, true);

	AssegnoDao asdao;
	std::vector<Assegno> assegni = asdao.getByLibretto(lib.getId());
	m_lstAssegni.ResetContent();
	for (auto const& a : assegni)
	{
		m_lstAssegni.AddString(a.getNumero().c_str());
	}

	// disabilito tutto
	m_txtLibretto.EnableWindow(false);
	m_txtCodice.EnableWindow(false);
	::SendDlgItemMessage(*this, IDC_RB20, BM_SETCHECK, BST_UNCHECKED, 0);
	::SendDlgItemMessage(*this, (lib.getQta() == 20 ? IDC_RB20 : IDC_RB10), BM_SETCHECK, BST_CHECKED, 0);
	m_rbQta20.EnableWindow(false);
	m_rbQta10.EnableWindow(false);
	m_btnCalcolaNumeri.EnableWindow(false);
	::EnableWindow(::GetDlgItem(*this, IDC_BTNSALVALIB), FALSE);
}

void DlgLibretto::OnAzienda()
{
	Azienda az = m_cmbAziende.GetSelectedItem();
	m_cmbConti.Aggiorna(az);
}

void DlgLibretto::OnCalcolaNumeri()
{
	m_lstAssegni.ResetContent();
	std::wstring nLib = m_txtLibretto.GetWindowTextW();

	GeneratoreNumeriAssegni gen{ m_qta };
	auto numeri = gen.Genera(nLib);

	for (const auto& n : numeri)
	{
		m_lstAssegni.AddString(n.c_str());
	}
}

void DlgLibretto::Errore(const std::wstring & msg)
{
	::MessageBox(*this, msg.c_str(), _T("Errore"), MB_ICONEXCLAMATION);
}

void DlgLibretto::Salva()
{
	ContoCorrente cc = m_cmbConti.GetSelectedItem();
	long id_conto = cc.getId();
	std::vector<std::wstring> assegni;
	for (int i = 0; i < m_lstAssegni.GetCount(); i++)
	{
		int sz = m_lstAssegni.GetTextLen(i) + 1;
		wchar_t * lp = new wchar_t[sz];
		m_lstAssegni.GetText(i, lp);
		assegni.push_back(lp);
		delete [] lp;
		lp = nullptr;
	}

	std::wstring codLib = m_txtCodice.GetWindowTextW();
	std::wstring nLib = m_txtLibretto.GetWindowTextW();

	boost::trim(codLib);
	boost::trim(nLib);

	LibrettoDao libdao;
	Libretto l = libdao.insert(id_conto, nLib, codLib, m_qta, assegni);
	::MessageBox(*this, _T("Salvato"), _T("Ok"), MB_ICONINFORMATION);
}
