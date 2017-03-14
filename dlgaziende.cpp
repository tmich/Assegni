#include "stdafx.h"
#include "resource.h"
#include "dlgaziende.h"

DlgAziende::DlgAziende()
	: CDialog{ IDD_AZIENDE }
{
}

DlgAziende::~DlgAziende()
{
}

BOOL DlgAziende::OnInitDialog()
{
	m_dbrec.AttachDlgItem(IDC_CUSTOM3, *this);
	AttachItem(IDC_TXTDENOMAZ, m_txtRagSoc);
	AttachItem(IDC_TXTINDIRIZZAZ, m_txtIndirizzo);
	AttachItem(IDC_TXTPIVAZ, m_txtPiva);
	AttachItem(IDC_BTNNUOVAZ, m_btnNuovaAz);
	AttachItem(IDC_BTNELIMINAZ, m_btnEliminaAz);
	AttachItem(IDC_BTNANNULLAZ, m_btnAnnullaAz);
	AttachItem(IDC_BTNSALVAZ, m_btnSalvaAz);

	// icone
	m_btnEliminaAz.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_TRASH), IMAGE_ICON, 24, 24, NULL));
	m_btnSalvaAz.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_DISK), IMAGE_ICON, 24, 24, NULL));
	m_btnAnnullaAz.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_CANCEL), IMAGE_ICON, 24, 24, NULL));
	m_btnNuovaAz.SetIcon((HICON)::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_NEW), IMAGE_ICON, 24, 24, NULL));

	size_t n = 0;
	AziendaDao azdao;
	m_aziende = azdao.all();
	m_iter = m_aziende.begin();
	n = m_aziende.size();
	
	// ce n'è almeno una
	if (n > 0)
	{
		m_state.reset(new DlgAziendeStateModifica(this));
	}
	else
	{
		m_state.reset(new DlgAziendeStateNull(this));
	}
	return 0;
}

void DlgAziende::OnDraw(CDC & dc)
{
	// Logo
	/*HICON hIcon = (HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_AZI), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	::DrawIconEx(dc, 10, 10, hIcon, 256, 256, 0, NULL, DI_NORMAL);
	DestroyIcon(hIcon);*/
}

BOOL DlgAziende::OnCommand(WPARAM wParam, LPARAM lParam)
{
	size_t current = 0;
	switch (LOWORD(wParam))
	{
	case IDC_BTNSALVAZ:
		m_state->Salva();
		break;
	case IDC_BTNELIMINAZ:
		m_state->Elimina();
		break;
	case IDC_BTNANNULLAZ:
		m_state->Annulla();
		break;
	case IDC_BTNNUOVAZ:
		m_state.reset(new DlgAziendeStateInserimento(this));
		break;
	case DBRECORD:
		current = m_dbrec.GetCurrent();
		switch (lParam)
		{
		case -1:
			m_iter--;
			break;
		default:
			m_iter++;
		}
		OnAzienda();
		break;
	default:
		break;
	}
	return 0;
}

void DlgAziende::OnAzienda()
{
	if (m_aziende.size() > 0)
	{
		m_state.reset(new DlgAziendeStateModifica(this));
	}
	else
	{
		m_state.reset(new DlgAziendeStateNull(this));
	}
}


/***************** DlgAziendeStateNull *****************/
DlgAziendeStateNull::DlgAziendeStateNull(DlgAziende * context)
	: DlgAziendeState{ context }
{
	m_context->m_btnNuovaAz.EnableWindow(true);
	m_context->m_btnSalvaAz.EnableWindow(false);
	m_context->m_btnAnnullaAz.EnableWindow(false);
	m_context->m_btnEliminaAz.EnableWindow(false);

	m_context->m_txtRagSoc.EnableWindow(false);
	m_context->m_txtIndirizzo.EnableWindow(false);
	m_context->m_txtPiva.EnableWindow(false);
}

/***************** DlgAziendeStateModifica *****************/
DlgAziendeStateModifica::DlgAziendeStateModifica(DlgAziende * context)
	: DlgAziendeState{ context }
{
	size_t n = m_context->m_aziende.size();
	m_context->m_dbrec.SetMax(n);

	m_context->m_txtRagSoc.EnableWindow(true);
	m_context->m_txtIndirizzo.EnableWindow(true);
	m_context->m_txtPiva.EnableWindow(true);

	if (m_context->m_aziende.size() > 0)
	{
		m_context->m_txtRagSoc.SetWindowTextW(m_context->m_iter->getRagioneSociale().c_str());
		m_context->m_txtIndirizzo.SetWindowTextW(m_context->m_iter->getIndirizzo().c_str());
		m_context->m_txtPiva.SetWindowTextW(m_context->m_iter->getPartitaIva().c_str());
	}
	m_context->m_btnNuovaAz.EnableWindow(true);
	m_context->m_btnEliminaAz.EnableWindow(true);
}

void DlgAziendeStateModifica::Salva()
{
	std::wstring ragsoc, indir, piva;
	ragsoc = m_context->m_txtRagSoc.GetWindowTextW();
	indir = m_context->m_txtIndirizzo.GetWindowTextW();
	piva= m_context->m_txtPiva.GetWindowTextW();

	m_context->m_iter->setRagioneSociale(ragsoc);
	m_context->m_iter->setIndirizzo(indir);
	m_context->m_iter->setPartitaIva(piva);

	try
	{
		AziendaDao azdao;
		azdao.save(*m_context->m_iter);
		m_context->m_dbrec.SetMax(m_context->m_aziende.size());
		::MessageBox(*m_context, _T("Modificato"), _T("Modifica") , MB_ICONINFORMATION);
	}
	catch (const std::exception&)
	{
		::MessageBox(*m_context, _T("Si è verificato un errore"), _T("Modifica"), MB_ICONWARNING);
	}
	
}

void DlgAziendeStateModifica::Annulla()
{
	m_context->OnAzienda();
}

void DlgAziendeStateModifica::Elimina()
{
	CString question;
	question.AppendFormat(_T("Vuoi davvero eliminare l'azienda %s?"), m_context->m_iter->getRagioneSociale().c_str());

	if (::MessageBox(*m_context, question, _T("Eliminazione"), MB_ICONQUESTION | MB_YESNO) == IDNO)
	{
		return;
	}

	long id = m_context->m_iter->getId();
	AziendaDao azdao;
	try
	{
		azdao.Delete(id);
		::MessageBox(*m_context, _T("Eliminato"), _T("Eliminazione"), MB_ICONINFORMATION);
		m_context->m_aziende.erase(m_context->m_iter);
		m_context->m_iter = m_context->m_aziende.begin();
		m_context->m_dbrec.SetMax(m_context->m_aziende.size());
		m_context->m_dbrec.SetCurrent(1);
		m_context->OnAzienda();
	}
	catch (const std::exception&)
	{
		::MessageBox(*m_context, _T("Si è verificato un errore"), _T("Eliminazione"), MB_ICONINFORMATION);
	}
}

DlgAziendeStateInserimento::DlgAziendeStateInserimento(DlgAziende * context)
	: DlgAziendeState{ context }
{
	m_context->m_btnNuovaAz.EnableWindow(false);
	m_context->m_btnEliminaAz.EnableWindow(false);
	m_context->m_btnAnnullaAz.EnableWindow(true);
	m_context->m_btnSalvaAz.EnableWindow(true);

	m_context->m_txtRagSoc.EnableWindow(true);
	m_context->m_txtIndirizzo.EnableWindow(true);
	m_context->m_txtPiva.EnableWindow(true);

	m_context->m_txtRagSoc.SetWindowTextW(_T(""));
	m_context->m_txtIndirizzo.SetWindowTextW(_T(""));
	m_context->m_txtPiva.SetWindowTextW(_T(""));

	m_context->m_txtRagSoc.SetFocus();
}

DlgAziendeStateInserimento::~DlgAziendeStateInserimento()
{
}

void DlgAziendeStateInserimento::Salva()
{
	std::wstring ragsoc, indir, piva;
	ragsoc = m_context->m_txtRagSoc.GetWindowTextW();
	indir = m_context->m_txtIndirizzo.GetWindowTextW();
	piva = m_context->m_txtPiva.GetWindowTextW();

	if (ragsoc.size() == 0)
	{
		m_context->MessageBoxW(_T("Inserire la denominazione dell'azienda!"), _T("Attenzione"), MB_ICONWARNING);
		return;
	}

	Azienda az{ ragsoc,indir };
	az.setPartitaIva(piva);

	try
	{
		AziendaDao azdao;
		azdao.save(az);
		m_context->m_aziende.push_back(az);
		m_context->m_iter = m_context->m_aziende.end() - 1;
		::MessageBox(*m_context, _T("Inserito"), _T("Inserimento"), MB_ICONINFORMATION);
		m_context->EndDialog(IDOK);
	}
	catch (const std::exception&)
	{
		::MessageBox(*m_context, _T("Si è verificato un errore"), _T("Inserimento"), MB_ICONWARNING);
	}
	m_context->OnAzienda();
}

void DlgAziendeStateInserimento::Annulla()
{
	m_context->OnAzienda();
}
