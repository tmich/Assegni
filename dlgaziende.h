#pragma once
#include "CDBRecord.h"
#include "azdao.h"

/***************** DlgAziendeoState *****************/
class DlgAziende;
class DlgAziendeState
{
public:
	DlgAziendeState(DlgAziende * context) : m_context{ context } {};
	virtual void Salva() = 0;
	virtual void Annulla() = 0;
	virtual void Elimina() = 0;
protected:
	DlgAziende * m_context;
};

class DlgAziendeStateNull;
class DlgAziendeStateInserimento;
class DlgAziendeStateModifica;

/***************** DlgAziende *****************/
class DlgAziende : public CDialog
{
public:
	DlgAziende();
	~DlgAziende();

	friend class DlgAziendeStateNull;
	friend class DlgAziendeStateInserimento;
	friend class DlgAziendeStateModifica;
protected:
	virtual BOOL OnInitDialog() override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	virtual void OnDraw(CDC & dc) override;
	virtual void OnAzienda();
private:
	CDBRecord m_dbrec;
	std::vector<Azienda> m_aziende;
	std::vector<Azienda>::iterator m_iter;
	CButton m_btnNuovaAz, m_btnEliminaAz, m_btnSalvaAz, m_btnAnnullaAz;
	CEdit m_txtRagSoc, m_txtIndirizzo, m_txtPiva;
	std::unique_ptr<DlgAziendeState> m_state;
};

/***************** DlgAziendeStateNull *****************/
class DlgAziendeStateNull : public DlgAziendeState
{
public:
	DlgAziendeStateNull(DlgAziende *context);
	virtual void Salva() override {};
	virtual void Annulla() override {};
	virtual void Elimina() override {};
};

/***************** DlgAziendeStateInserimento *****************/
class DlgAziendeStateInserimento : public DlgAziendeState
{
public:
	DlgAziendeStateInserimento(DlgAziende *context);
	~DlgAziendeStateInserimento();
	virtual void Salva() override;
	virtual void Annulla() override;
	virtual void Elimina() override {};
};

/***************** DlgAziendeStateModifica *****************/
class DlgAziendeStateModifica : public DlgAziendeState
{
public:
	DlgAziendeStateModifica(DlgAziende *context);
	virtual void Salva() override;
	virtual void Annulla() override;
	virtual void Elimina() override;
};