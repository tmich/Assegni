#pragma once
#include "cmbazienda.h"
#include "contocorrente.h"
#include <memory>


/***************** DlgContoState *****************/
class DlgConto;
class DlgContoState
{
public:
	DlgContoState(DlgConto * context) : m_context{ context } {};
	virtual void Salva() = 0;
	virtual void Annulla() = 0;
	virtual void Elimina() = 0;
protected:
	DlgConto * m_context;
};

class DlgContoStateNull;
class DlgContoStateInserimento;
class DlgContoStateModifica;

/***************** DlgConto *****************/
class DlgConto : public CDialog
{
public:
	DlgConto();
	virtual ~DlgConto();

	friend class DlgContoStateNull;
	friend class DlgContoStateInserimento;
	friend class DlgContoStateModifica;
protected:
	virtual BOOL OnInitDialog() override;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	virtual void OnDraw(CDC& dc) override;
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT OnColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void Notifica(LPCTSTR messaggio, LPCTSTR titolo, UINT tipo);
	bool Conferma(LPCTSTR messaggio, LPCTSTR titolo);
	void OnConto();
	void OnAzienda();
	void AggiornaPaginatore();
	bool OnPrevBanca();
	bool OnNextBanca();
	bool OnNuovo();
	bool OnSalva();
	bool OnAnnulla();
	bool OnElimina();
	void Pulisci();

	CComboAziende m_cmbAzienda;
	std::vector<ContoCorrente> m_conti;
	std::vector<ContoCorrente>::iterator m_iter;
	size_t idx;
	
	CEdit m_pagerText, m_txtDenominazione, m_txtNumConto, m_txtSede, m_txtAgenzia, m_txtNote;
	CButton m_btnNuovo, m_btnSalva, m_btnAnnulla, m_btnElimina;

	std::unique_ptr<DlgContoState> m_state;
};

/***************** DlgContoStateNull *****************/
class DlgContoStateNull : public DlgContoState
{
public:
	DlgContoStateNull(DlgConto *context);
	virtual void Salva() override {};
	virtual void Annulla() override {};
	virtual void Elimina() override {};
};

/***************** DlgContoStateInserimento *****************/
class DlgContoStateInserimento : public DlgContoState
{
public:
	DlgContoStateInserimento(DlgConto *context);
	~DlgContoStateInserimento();
	virtual void Salva() override;
	virtual void Annulla() override;
	virtual void Elimina() override {};
};

/***************** DlgContoStateModifica *****************/
class DlgContoStateModifica : public DlgContoState
{
public:
	DlgContoStateModifica(DlgConto *context);
	virtual void Salva() override;
	virtual void Annulla() override;
	virtual void Elimina() override;
};