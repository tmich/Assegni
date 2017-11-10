#pragma once
#include <wxx_dialog.h>
#include <wxx_controls.h>
#include <wxx_stdcontrols.h>
#include "assdao.h"
#include "cmbazienda.h"
#include "cmbconti.h"
#include "cmblibretti.h"
#include "cmbassegni.h"
#include "cmbfornitore.h"
#include "forndao.h"

class DlgDettaglioAssegno : public CDialog
{
public:
	DlgDettaglioAssegno();
	DlgDettaglioAssegno(long idAssegno);
	~DlgDettaglioAssegno();
private:
	virtual BOOL OnInitDialog() override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	virtual void OnDraw(CDC & dc) override;
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	LRESULT OnColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnSalva();
	LRESULT PulisciCampi();
	void OnAzienda();
	void OnConto();
	void OnLibretto();
	void OnIncassato();
	void DatiAssegno();
	void OnBloccaSblocca();

	CComboAziende m_cmbAzienda;
	CComboConti m_cmbConti;
	CComboLibretti m_cmbLibretti;
	CComboAssegni m_cmbAssegni;
	CEdit m_txtImpAss, m_txtImpDecAss, m_txtCausAss;
	// CEdit m_txtIntAss;
	CComboFornitore m_txtIntAss;
	CDateTime m_dtAss, m_dtIncass, m_dtScadenza;
	CButton m_btnSalvAss, m_chkIncasso, m_btnSblocca;
	bool m_incassato, m_sbloccato;
	long m_idAssegno;
	std::vector<Assegno> vAssegno;
};

