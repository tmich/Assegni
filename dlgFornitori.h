#pragma once
#include <wxx_dialog.h>
#include <wxx_listview.h>
#include "fornitore.h"

class DlgFornitori
	: public CDialog
{
public:
	DlgFornitori();
	virtual ~DlgFornitori();
protected:
	BOOL OnInitDialog() override;
	BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	void OnDblClick(LPARAM lParam);
	void Update();
	void OnNuovo();
	std::vector<Fornitore> m_fornitori;
	CListView m_lstFornitori;
	CButton m_btnNuovo;
};