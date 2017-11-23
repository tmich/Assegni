#pragma once
#include <wxx_dialog.h>
#include <wxx_controls.h>
#include "cmbazienda.h"
#include "cmbconti.h"

class DlgLibretto : public CDialog
{
public: 
	DlgLibretto();
	DlgLibretto(long idLibretto);
	~DlgLibretto();
private:
	virtual BOOL OnInitDialog() override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	virtual void OnOK() override;
	//virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	virtual void DatiLibretto();
	void OnAzienda();
	void OnCalcolaNumeri();
	void Errore(const std::wstring& msg);
	void Salva();

	CButton m_btnCalcolaNumeri, m_rbQta10, m_rbQta20;
	CComboAziende m_cmbAziende;
	CComboConti m_cmbConti;
	CEdit m_txtLibretto, m_txtAssegno, m_txtCodice;
	CListBox m_lstAssegni;
	int m_qta;
	long m_idLibretto;
	bool m_readOnly, m_ricalcolato;
};