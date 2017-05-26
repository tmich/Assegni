#include "stdafx.h"
#include "dlgFornitori.h"
#include "dlgForn.h"
#include "resource.h"
#include "forndao.h"

DlgFornitori::DlgFornitori()
	: CDialog{ IDD_FORNITORI }
{
}

DlgFornitori::~DlgFornitori()
{

}

BOOL DlgFornitori::OnInitDialog()
{
	AttachItem(IDC_LISTFORN, m_lstFornitori);
	AttachItem(IDC_BTNNFORN, m_btnNuovo);
	
	m_lstFornitori.InsertColumn(0, _T("Denominazione"), 0, 200);
	m_lstFornitori.InsertColumn(1, _T("Indirizzo"), 0, 400);
	m_lstFornitori.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	Update();
	
	return true;
}

BOOL DlgFornitori::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BTNNFORN:
		OnNuovo();
		break;
	}
	return 0;
}

LRESULT DlgFornitori::OnNotify(WPARAM wParam, LPARAM lParam)
{
	long id = 0;
	switch (LOWORD(wParam))
	{
	case IDC_LISTFORN:
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_DBLCLK:
			OnDblClick(lParam);
			break;
		}
	}

	return 0;
}

void DlgFornitori::OnDblClick(LPARAM lParam)
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
	Fornitore *forn = (Fornitore*)(m_lstFornitori.GetItemData(lpnmitem->iItem));

	int id = forn->getId();
	if (id > 0)
	{
		DlgFornitore dlgForn{ id };
		INT_PTR res = dlgForn.DoModal(*this);
		if (res == IDOK)
		{
			Update();
		}
	}
}

void DlgFornitori::Update()
{
	m_lstFornitori.DeleteAllItems();

	FornitoreDao fdao;
	m_fornitori = fdao.getAll();

	int i = 0;
	for each (const auto& f in m_fornitori)
	{
		LVITEM lv{ 0 };

		//denominazione
		std::wstring denom = f.getDenominazione();
		lv.iItem = i;
		lv.mask = LVFIF_TEXT;
		lv.pszText = (LPWSTR)(denom.c_str());
		m_lstFornitori.InsertItem(lv);

		//indirizzo
		std::wstring indir = f.getIndirizzo();
		lv.iSubItem = 1;
		lv.pszText = (LPWSTR)(indir.c_str());
		m_lstFornitori.SetItem(lv);

		m_lstFornitori.SetItemData(i, (DWORD_PTR)&f);
		i++;
	}
}

void DlgFornitori::OnNuovo()
{
	DlgFornitore dlg;
	if (dlg.DoModal(*this) == IDOK)
	{
		Update();
	}
}
