#pragma once
#include "stdafx.h"
#include "assegno.h"
#include <wxx_listview.h>

class ListViewAssegni : public CListView
{
public:
	ListViewAssegni();
	ListViewAssegni(std::vector<Assegno> items);
	virtual ~ListViewAssegni();
	void Aggiungi(const Assegno& a);
	void Update();
	void ClearItems();
	void SetItems(const std::vector<Assegno>& items);
protected:
	virtual void OnInitialUpdate() override;
	std::vector<Assegno> m_assegni;
};