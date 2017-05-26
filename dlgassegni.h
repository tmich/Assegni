#pragma once

class DlgAssegni : public CDialog
{
public:
	DlgAssegni();
	~DlgAssegni();
private:
	virtual BOOL OnInitDialog() override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam) override;
	virtual void OnSettings();
	virtual void OnConnectionException();
	
	CButton m_btnNuovoAssegno, m_btnNuovoLibretto, m_btnCercaAssegno, m_btnEmessi, m_btnBanche, m_btnAziende, m_btnFornitori;
	CStatic m_imgLogo;
};