#pragma once
#include <wxx_dialog.h>
#include "fornitore.h"

class DlgFornitore
	: public CDialog
{
public:
	DlgFornitore();
	DlgFornitore(int id);
	virtual ~DlgFornitore();
protected:
	BOOL OnInitDialog() override;
	BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	void OnSalva();
	void OnElimina();
	CEdit m_txtId, m_txtDenominazione, m_txtIndirizzo, m_txtTel, m_txtPiva;
	CButton m_btnElimina;
	int m_IdForn;
};