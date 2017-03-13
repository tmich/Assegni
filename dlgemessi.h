#pragma once
#include <wxx_dialog.h>
#include <wxx_listview.h>
#include "cmbazienda.h"
#include "cmbmesi.h"
#include "cmbanni.h"
#include "assegno.h"

class DlgAssegniEmessi :
	public CDialog
{
public:
	DlgAssegniEmessi();
	virtual ~DlgAssegniEmessi();
private:
	BOOL OnInitDialog() override;
	BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	void OnCerca();
	void OnDblClick(LPARAM lParam);
	void Aggiungi(const Assegno& a);

	CComboAziendeTutte m_cmbAziende;
	CComboMesi m_cmbMeseDal, m_cmbMeseAl;
	CComboAnni m_cmbAnnoDal, m_cmbAnnoAl;
	CListView m_listAssegni;
	std::vector<Assegno> assegniEmessi;
};

