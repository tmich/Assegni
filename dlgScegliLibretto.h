#pragma once
#include <wxx_dialog.h>
#include <wxx_listview.h>
#include "cmbazienda.h"
#include "cmbconti.h"
#include "cmblibretti.h"

class DlgScegliLibretto :
	public CDialog
{
public:
	DlgScegliLibretto();
	virtual ~DlgScegliLibretto();
protected:
	BOOL OnInitDialog() override;
	BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	//LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	//void Aggiorna();
	void OnAzienda();
	void OnConto();

	CComboAziende m_cmbAzienda;
	CComboConti m_cmbConti;
	CComboLibretti m_cmbLibretti;

};

