#include "stdafx.h"
#include "resource.h"
#include "dlgconto.h"
#include "ccdao.h"

DlgConto::DlgConto()
	:CDialog{ IDD_CONTOCORRENTE }
{
}

DlgConto::~DlgConto()
{
}

BOOL DlgConto::OnInitDialog()
{
	AttachItem(IDC_CMBAZ2, m_cmbAzienda);
	AttachItem(IDC_TXTPAGER, m_pagerText);
	AttachItem(IDC_TXTBANCA, m_txtDenominazione);
	AttachItem(IDC_TXTNUMCONTO, m_txtNumConto);
	AttachItem(IDC_TXTSEDE, m_txtSede);
	AttachItem(IDC_TXTAGENZIA, m_txtAgenzia);
	AttachItem(IDC_TXTNOTEBANCA, m_txtNote);
	AttachItem(IDC_BTNNUOVABANCA, m_btnNuovo);
	AttachItem(IDC_BTNSALVABANCA, m_btnSalva);
	AttachItem(IDC_BTNANNULLABANCA, m_btnAnnulla);
	AttachItem(IDC_BTNELIMINABANCA, m_btnElimina);
	
	// icone
	m_btnElimina.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_TRASH), IMAGE_ICON, 24, 24, NULL));
	m_btnSalva.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_DISK), IMAGE_ICON, 24, 24, NULL));
	m_btnAnnulla.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_CANCEL), IMAGE_ICON, 24, 24, NULL));
	m_btnNuovo.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_NEW), IMAGE_ICON, 24, 24, NULL));
	

	// stato iniziale
	m_state.reset(new DlgContoStateNull(this));
	m_btnNuovo.EnableWindow(false);
	AggiornaPaginatore();
	return 0;
}

BOOL DlgConto::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CMBAZ2:
		if (HIWORD(wParam) == CBN_SELCHANGE)
			OnAzienda();
		break;
	case IDC_BTNPREVBANCA:
		return OnPrevBanca();
	case IDC_BTNNEXTBANCA: 
		return OnNextBanca();
	case IDC_BTNNUOVABANCA:
		return OnNuovo();
	case IDC_BTNSALVABANCA:
		return OnSalva();
	case IDC_BTNANNULLABANCA:
		return OnAnnulla();
	case IDC_BTNELIMINABANCA:
		return OnElimina();
	}
	
	return false;
}

void DlgConto::OnDraw(CDC & dc)
{
	// Logo
	//HICON hIcon = (HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_ERROR), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	HICON hIcon = (HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_BANKGREEN), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	::DrawIconEx(dc, 10, 10, hIcon, 256, 256, 0, NULL, DI_NORMAL);
	DestroyIcon(hIcon);
}

LRESULT DlgConto::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return CDialog::OnPaint(uMsg, wParam, lParam);
}

BOOL DlgConto::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CTLCOLORSTATIC:	return OnColorStatic(uMsg, wParam, lParam);
	}

	return DialogProcDefault(uMsg, wParam, lParam);
}

LRESULT DlgConto::OnColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void DlgConto::Notifica(LPCTSTR messaggio, LPCTSTR titolo, UINT tipo)
{
	this->MessageBoxW(messaggio, titolo, tipo);
}

bool DlgConto::Conferma(LPCTSTR messaggio, LPCTSTR titolo)
{
	return this->MessageBoxW(messaggio, titolo, MB_ICONASTERISK + MB_YESNO) == IDYES;
}

void DlgConto::OnConto()
{
	// stato: modifica conto esistente
	m_state.reset(new DlgContoStateModifica(this));
	m_btnElimina.EnableWindow(m_conti.size() > 0);
	m_btnSalva.EnableWindow(m_conti.size() > 0);
	m_btnAnnulla.EnableWindow(m_conti.size() > 0);
	
	AggiornaPaginatore();
}

void DlgConto::OnAzienda()
{
	Azienda az = m_cmbAzienda.GetSelectedItem();
	//TRACE(az->getRagioneSociale().c_str()); TRACE(L"\n");
	ContoCorrenteDao ccdao;
	m_conti = ccdao.GetByAzienda(az);
	Pulisci();
	CString text;
	size_t sz = m_conti.size();
	idx = 1;
	if (sz > 0)
	{
		m_iter = m_conti.begin();
		OnConto();
		AggiornaPaginatore();
	}
	else
	{
		m_txtDenominazione.EnableWindow(false);
		m_txtNumConto.EnableWindow(false);
		m_txtSede.EnableWindow(false);
		m_txtAgenzia.EnableWindow(false);
		m_txtNote.EnableWindow(false);
		m_pagerText.EnableWindow(false);
		m_state.reset(new DlgContoStateNull(this));
	}
}

void DlgConto::AggiornaPaginatore()
{
	CString text;
	size_t sz = m_conti.size();
	if (sz == 0)
	{
		::EnableWindow(GetDlgItem(IDC_BTNNEXTBANCA), 0);
		::EnableWindow(GetDlgItem(IDC_BTNPREVBANCA), 0);
	}
	text.AppendFormat(_T("%d di %d"), idx, sz);
	m_pagerText.SetWindowTextW(text);

	::EnableWindow(GetDlgItem(IDC_BTNNEXTBANCA), (idx < sz));
	::EnableWindow(GetDlgItem(IDC_BTNPREVBANCA), (idx > 1));
}

bool DlgConto::OnPrevBanca()
{
	if (m_iter != m_conti.begin())
	{
		m_iter--;
		OnConto();
		idx--;
		AggiornaPaginatore();
	}
	return true;
}

bool DlgConto::OnNextBanca()
{
	if (++m_iter != m_conti.end())
	{
		OnConto();
		idx++;
		AggiornaPaginatore();
	}
	else
	{
		--m_iter;
	}
	return false;
}

bool DlgConto::OnNuovo()
{
	// stato: inserimento nuovo conto
	//m_state.release();
	m_state.reset(new DlgContoStateInserimento(this));
	
	return true;
}

bool DlgConto::OnSalva()
{
	m_state->Salva();

	// ritorno allo stato iniziale
	//m_state.reset(new DlgContoStateNull(this));

	return true;
}

bool DlgConto::OnAnnulla()
{
	m_state->Annulla();
	
	// ritorno allo stato iniziale
	//m_state.reset(new DlgContoStateNull(this));
	
	return true;
}

bool DlgConto::OnElimina()
{
	m_state->Elimina();
	return true;
}

void DlgConto::Pulisci()
{
	m_txtDenominazione.SetWindowTextW(_T(""));
	m_txtNumConto.SetWindowTextW(_T(""));
	m_txtSede.SetWindowTextW(_T(""));
	m_txtAgenzia.SetWindowTextW(_T(""));
	m_txtNote.SetWindowTextW(_T(""));
	m_pagerText.SetWindowTextW(_T(""));
}

void DlgConto::AbilitaPaginatore(bool abilita)
{
	::EnableWindow(::GetDlgItem(*this, IDC_BTNPREVBANCA), abilita);
	::EnableWindow(::GetDlgItem(*this, IDC_BTNNEXTBANCA), abilita);
	m_pagerText.EnableWindow(abilita);
}

DlgContoStateInserimento::DlgContoStateInserimento(DlgConto *context)
	: DlgContoState{ context }
{
	m_context->m_btnNuovo.EnableWindow(false);
	m_context->Pulisci();
	m_context->m_txtDenominazione.SetFocus();

	m_context->m_txtDenominazione.EnableWindow(true);
	m_context->m_txtNumConto.EnableWindow(true);
	m_context->m_txtSede.EnableWindow(true);
	m_context->m_txtAgenzia.EnableWindow(true);
	m_context->m_txtNote.EnableWindow(true);
	//m_pagerText.EnableWindow(false);

	m_context->m_btnNuovo.EnableWindow(false);
	m_context->m_btnSalva.EnableWindow(true);
	m_context->m_btnAnnulla.EnableWindow(true);
	m_context->m_btnElimina.EnableWindow(false);

	m_context->AbilitaPaginatore(false);
}

DlgContoStateInserimento::~DlgContoStateInserimento()
{
}

void DlgContoStateInserimento::Salva()
{
	std::wstring numero, banca, sede, agenzia, note;
	numero = m_context->m_txtNumConto.GetWindowTextW().c_str();
	banca = m_context->m_txtDenominazione.GetWindowTextW().c_str();
	sede = m_context->m_txtSede.GetWindowTextW().c_str();
	agenzia = m_context->m_txtAgenzia.GetWindowTextW().c_str();
	note = m_context->m_txtNote.GetWindowTextW().c_str();

	ContoCorrente conto{ numero,banca,sede,agenzia };
	conto.setNote(note);
	Azienda az = m_context->m_cmbAzienda.GetSelectedItem();
	ContoCorrenteDao ccdao;
	if (ccdao.Insert(conto, az.getId()))
	{
		m_context->m_conti.push_back(conto);
		m_context->Notifica(_T("Inserito correttamente"), _T("Inserimento"), MB_ICONINFORMATION);
		m_context->OnAzienda();
	}
	else
	{
		m_context->Notifica(_T("Si è verificato un errore"), _T("Inserimento"), MB_ICONWARNING);
	}
}

void DlgContoStateInserimento::Annulla()
{
	size_t sz = m_context->m_conti.size();
	m_context->m_btnElimina.EnableWindow(sz > 0);
	m_context->OnConto();
}

DlgContoStateModifica::DlgContoStateModifica(DlgConto *context)
	: DlgContoState{ context }
{
	if (m_context->m_conti.size() > 0)
	{
		try
		{
			auto cc = *m_context->m_iter;
			m_context->m_txtDenominazione.SetWindowTextW(cc.getBanca().c_str());
			m_context->m_txtNumConto.SetWindowTextW(cc.getNumero().c_str());
			m_context->m_txtSede.SetWindowTextW(cc.getSede().c_str());
			m_context->m_txtAgenzia.SetWindowTextW(cc.getAgenzia().c_str());
			m_context->m_txtNote.SetWindowTextW(cc.getNote().c_str());
		}
		catch (const std::exception&)
		{

		}
	}
	else
	{
		m_context->m_txtDenominazione.SetWindowTextW(_T(""));
		m_context->m_txtNumConto.SetWindowTextW(_T(""));
		m_context->m_txtSede.SetWindowTextW(_T(""));
		m_context->m_txtAgenzia.SetWindowTextW(_T(""));
		m_context->m_txtNote.SetWindowTextW(_T(""));
	}

	m_context->m_txtDenominazione.EnableWindow(m_context->m_conti.size() > 0);
	m_context->m_txtNumConto.EnableWindow(m_context->m_conti.size() > 0);
	m_context->m_txtSede.EnableWindow(m_context->m_conti.size() > 0);
	m_context->m_txtAgenzia.EnableWindow(m_context->m_conti.size() > 0);
	m_context->m_txtNote.EnableWindow(m_context->m_conti.size() > 0);

	m_context->m_btnNuovo.EnableWindow(true);
	m_context->m_btnSalva.EnableWindow(true);
	m_context->m_btnAnnulla.EnableWindow(true);
	m_context->m_btnElimina.EnableWindow(true);
	m_context->AbilitaPaginatore(true);
}

void DlgContoStateModifica::Salva()
{
	std::wstring numero, banca, sede, agenzia, note;
	numero = m_context->m_txtNumConto.GetWindowTextW().c_str();
	banca = m_context->m_txtDenominazione.GetWindowTextW().c_str();
	sede = m_context->m_txtSede.GetWindowTextW().c_str();
	agenzia = m_context->m_txtAgenzia.GetWindowTextW().c_str();
	note = m_context->m_txtNote.GetWindowTextW().c_str();
	
	m_context->m_iter->setAgenzia(agenzia);
	m_context->m_iter->setBanca(banca);
	m_context->m_iter->setNote(note);
	m_context->m_iter->setNumero(numero);
	m_context->m_iter->setSede(sede);

	ContoCorrenteDao ccdao;
	if (ccdao.Update(*(m_context->m_iter)))
	{
		m_context->Notifica(_T("Modificato correttamente"), _T("Modifica"), MB_ICONINFORMATION);
	}
	else
	{
		m_context->Notifica(_T("Si è verificato un errore"), _T("Modifica"), MB_ICONWARNING);
	}
}

void DlgContoStateModifica::Annulla()
{
	m_context->OnConto();
}

void DlgContoStateModifica::Elimina()
{
	/*if (m_context->m_iter == 0)
		return;*/

	if (m_context->m_iter == m_context->m_conti.end())
		return;

	CString text;
	text.AppendFormat(_T("Vuoi davvero eliminare il conto %s?"), m_context->m_iter->getBanca().c_str());
	if (m_context->Conferma(text, _T("Elimina")))
	{
		ContoCorrenteDao ccdao;
		if (ccdao.Delete(m_context->m_iter->getId()))
		{
			m_context->Notifica(_T("Eliminato"), _T("Elimina"), MB_ICONINFORMATION);
			m_context->m_conti.erase(m_context->m_iter);
			m_context->OnAzienda();
		}
		else
		{
			m_context->Notifica(_T("Impossibile eliminare. Probabilmente ci sono dei libretti già caricati."), _T("Elimina"), MB_ICONWARNING);
		}
	}
}

DlgContoStateNull::DlgContoStateNull(DlgConto *context)
	: DlgContoState{ context }
{
	
	m_context->m_btnNuovo.EnableWindow(true);
	m_context->m_btnSalva.EnableWindow(false);
	m_context->m_btnAnnulla.EnableWindow(false);
	m_context->m_btnElimina.EnableWindow(false);
}
