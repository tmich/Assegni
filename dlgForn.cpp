#include "stdafx.h"
#include "dlgForn.h"
#include "resource.h"
#include "fornitore.h"
#include "forndao.h"

DlgFornitore::DlgFornitore()
	: CDialog{ IDD_FORNITORE }
{
	m_IdForn = 0;
}

DlgFornitore::DlgFornitore(int id)
	: DlgFornitore()
{
	m_IdForn = id;
}



DlgFornitore::~DlgFornitore()
{
}

BOOL DlgFornitore::OnInitDialog()
{
	AttachItem(IDC_TXTIDFORN, m_txtId);
	AttachItem(IDC_TXTDENOMFORN, m_txtDenominazione);
	AttachItem(IDC_TXTTELFORN, m_txtTel);
	AttachItem(IDC_TXTINDIRFORN, m_txtIndirizzo);
	AttachItem(IDC_TXTPIVAFORN, m_txtPiva);
	AttachItem(IDC_BTNELIMINAFORN, m_btnElimina);
	m_txtId.SetWindowTextW(_T(""));

	m_btnElimina.EnableWindow(m_IdForn > 0);

	if (m_IdForn > 0)
	{
		FornitoreDao fornDao;
		Fornitore f = fornDao.getById(m_IdForn);
		std::wstring denom = f.getDenominazione();
		std::wstring indir = f.getIndirizzo();
		std::wstring tel = f.getTelefono();
		std::wstring piva = f.getPartitaIva();

		m_txtId.SetWindowTextW(std::to_wstring(m_IdForn).c_str());
		m_txtDenominazione.SetWindowTextW(denom.c_str());
		m_txtIndirizzo.SetWindowTextW(indir.c_str());
		m_txtTel.SetWindowTextW(tel.c_str());
		m_txtPiva.SetWindowTextW(piva.c_str());
	}

	return 0;
}

BOOL DlgFornitore::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BTNSALVAFORN:
		OnSalva();
		break;
	case IDC_BTNELIMINAFORN:
		OnElimina();
		break;
	}
	return 0;
}

LRESULT DlgFornitore::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void DlgFornitore::OnSalva()
{
	std::wstring id{ m_txtId.GetWindowTextW() };
	std::wstring denom{ m_txtDenominazione.GetWindowTextW() };
	std::wstring indir{ m_txtIndirizzo.GetWindowTextW() };
	std::wstring tel{ m_txtTel.GetWindowTextW() };
	std::wstring piva{ m_txtPiva.GetWindowTextW() };

	FornitoreDao fornDao;

	if (id.size() == 0)
	{
		// inserimento
		Fornitore f{ denom,indir,tel,piva };
		fornDao.salva(f);
		m_txtId.SetWindowTextW(std::to_wstring(f.getId()).c_str());
		MessageBox(_T("Salvataggio effettuato"), _T("Ok"), MB_ICONINFORMATION);
		EndDialog(IDOK);
	}
	else
	{
		int d = std::stoi(id);
		Fornitore f{ d, denom,indir,tel,piva };
		fornDao.salva(f);
		MessageBox(_T("Salvataggio effettuato"), _T("Ok"), MB_ICONINFORMATION);
		EndDialog(IDOK);
	}
}

void DlgFornitore::OnElimina()
{
	if (MessageBox(_T("Eliminare questo fornitore? "), _T("Conferma"), MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
	{
		FornitoreDao fornDao;
		//Fornitore f = fornDao.getById(m_IdForn);
		if (fornDao.elimina(m_IdForn) == 1)
		{
			MessageBox(_T("Operazione completata"), _T("Ok"), MB_ICONINFORMATION);
			EndDialog(IDOK);
		}
	}
}
