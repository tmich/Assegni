#pragma once
#include <wxx_dialog.h>
#include <wxx_listview.h>
#include "cmbazienda.h"
#include "cmbconti.h"
#include "cmbmesi.h"
#include "cmbanni.h"
#include "assegno.h"
#include "lvassegni.h"

class DlgAssegniEmessi :
	public CDialog
{
public:
	DlgAssegniEmessi();
	virtual ~DlgAssegniEmessi();
protected:
	BOOL OnInitDialog() override;
	BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	void OnCerca();
	void OnStampa();
	void OnDblClick(LPARAM lParam);
	void Aggiungi(const Assegno& a);

	CComboAziendeTutte m_cmbAziende;
	CComboConti m_cmbConti;
	CComboMesi m_cmbMeseDal, m_cmbMeseAl;
	CComboAnni m_cmbAnnoDal, m_cmbAnnoAl;
	CButton m_btnStampa;
	ListViewAssegni m_listAssegni;
	std::vector<Assegno> assegniEmessi, assegniFiltrati;
	CEdit m_txtTotale;
	double m_TotaleDaIncassare;
};

