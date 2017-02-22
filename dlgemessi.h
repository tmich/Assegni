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

	CComboAziende m_cmbAziende;
	CComboMesi m_cmbMese;
	CComboAnni m_cmbAnno;
	CListView m_listAssegni;
	std::vector<Assegno> assegniEmessi;
};

