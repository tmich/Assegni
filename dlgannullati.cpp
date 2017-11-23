#include "stdafx.h"
#include "dlgannullati.h"
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

DlgAssegniAnnullati::DlgAssegniAnnullati()
	: CDialog{ IDD_ANNULLATI }
{
	m_Totale = 0.0;
}

DlgAssegniAnnullati::~DlgAssegniAnnullati()
{
}

BOOL DlgAssegniAnnullati::OnInitDialog()
{
	AttachItem(IDC_TXTTOTANNULL, m_txtTotale);
	AttachItem(IDC_LISTASSEGNIANNULLATI, m_listAssegni);
	
	Aggiorna();
	
	return 0;
}

BOOL DlgAssegniAnnullati::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT DlgAssegniAnnullati::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void DlgAssegniAnnullati::Aggiorna()
{
	services::Service service;
	m_Totale = 0;
	m_listAssegni.DeleteAllItems();
	auto assegniAnnullati = service.GetAssegniAnnullati();

	// ordino per data scadenza
	std::sort(assegniAnnullati.begin(), assegniAnnullati.end(), [](const Assegno& a1, const Assegno& a2) {
		return a1.getDataScadenza() < a2.getDataScadenza(); });

	m_listAssegni.SetItems(assegniAnnullati);

	// calcolo il totale
	for each (const Assegno& ass in assegniAnnullati)
	{
		m_Totale += ass.getImporto();
	}
	m_txtTotale.SetWindowTextW(utils::format(m_Totale).c_str());
	
	//m_btnStampa.EnableWindow(assegniAnnullati.size() > 0);
}
