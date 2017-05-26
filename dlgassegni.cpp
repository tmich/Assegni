#include "stdafx.h"
#include "dlgassegni.h"
#include "resource.h"
#include "assdao.h"
#include "dlgdettassegno.h"
#include "dlglibretto.h"
#include "dlgemessi.h"
#include "dlgsettings.h"
#include "dlgconto.h"
#include "dlgFornitori.h"
#include <wxx_gdi.h>
#include <mariadb++\exceptions.hpp>
#include <stdexcept>
#include "dbconn.h"
#include "cmbazienda.h"
#include "dlgaziende.h"

DlgAssegni::DlgAssegni()
	: CDialog{ IDD_ASSEGNI }
{
}

DlgAssegni::~DlgAssegni()
{
}

BOOL DlgAssegni::OnInitDialog()
{
	// Icona
	SetIcon(::LoadIcon(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_BANK)), false);
	AttachItem(IDC_BTNNASS, m_btnNuovoAssegno);
	AttachItem(IDC_BTNCARICALIB, m_btnNuovoLibretto);
	AttachItem(IDC_BTNCERCASS, m_btnCercaAssegno);
	AttachItem(IDC_BTNEMESSI, m_btnEmessi);
	AttachItem(IDC_IMGLOGO, m_imgLogo);
	AttachItem(IDC_BTNBANCHE, m_btnBanche);
	AttachItem(IDC_BTNAZIENDE, m_btnAziende);
	AttachItem(IDC_BTNFORNITORI, m_btnFornitori);

	//Icona dei pulsanti
	m_btnNuovoAssegno.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_PEN), IMAGE_ICON, 48, 48, LR_SHARED)));
	//m_btnCercaAssegno.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_FIND), IMAGE_ICON, 48, 48, LR_SHARED)));
	m_btnEmessi.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_BANK), IMAGE_ICON, 48, 48, LR_SHARED)));
	m_btnNuovoLibretto.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_BOOKLET), IMAGE_ICON, 48, 48, LR_SHARED)));
	m_btnBanche.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_BANK), IMAGE_ICON, 48, 48, LR_SHARED)));
	m_btnAziende.SetIcon((HICON)(::LoadImage(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_AZI), IMAGE_ICON, 48, 48, LR_SHARED)));
	// Logo
	//m_imgLogo.SetBitmap(::LoadBitmap(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDB_LOGO)));
	return 0;
}

BOOL DlgAssegni::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//test di connessione, da migliorare!
	mydb::Connection cnn;
	try
	{
		auto cref = cnn.connect();
		cref->disconnect();
	}
	catch (const mariadb::exception::connection&)
	{
		OnConnectionException();
		return false;
	}

	switch (LOWORD(wParam))
	{
	case IDC_BTNNASS:
	{
		DlgDettaglioAssegno dlg;
		dlg.DoModal(*this);
		break;

	}
	case IDC_BTNCARICALIB:
	{
		DlgLibretto dlg;
		dlg.DoModal(*this);
		break;
	}
	case IDC_BTNCERCASS:
	{
		DlgDettaglioAssegno dlgAss{ 5 };
		dlgAss.DoModal(*this);
		break;
	}
	case IDC_BTNCERCALIB:
	{
		// PROVA
		DlgLibretto dlgLib{ 8 };
		dlgLib.DoModal(*this);
		break;
	}
	case IDC_BTNEMESSI:
	{
		DlgAssegniEmessi dlgEmessi;
		dlgEmessi.DoModal(*this);
		break;
	}
	case IDC_BTNBANCHE:
	{
		DlgConto dlgConto;
		dlgConto.DoModal(*this);
		break;
	}
	case IDC_BTNAZIENDE:
	{
		DlgAziende dlgAz;
		dlgAz.DoModal(*this);
		break;
	}
	case IDC_BTNFORNITORI:
	{
		DlgFornitori dlgForn;
		dlgForn.DoModal(*this);
		break;
	}
	}
	return 0;
}

LRESULT DlgAssegni::OnNotify(WPARAM wParam, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case NM_CLICK:
		if (((LPNMHDR)lParam)->idFrom == IDC_LNKSETT)
		{
			OnSettings();
		}
		break;
	}
	return LRESULT();
}

void DlgAssegni::OnSettings()
{
	DlgSettings dlgsett;
	
	if (dlgsett.DoModal(*this) == IDOK)
	{
		;
	}
}

void DlgAssegni::OnConnectionException()
{
	::MessageBox(*this,
		_T("Errore nella connessione al database. Controllare i parametri e riprovare."),
		_T("Attenzione"),
		MB_ICONWARNING);
	OnSettings();
}
