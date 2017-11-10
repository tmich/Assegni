#pragma once
#include "wxx_controls.h"
#include "fornitore.h"
#include "forndao.h"

class CComboFornitore :
	public CComboBox
{
public:
	CComboFornitore();
	~CComboFornitore();

	Fornitore GetSelectedItem() const;
protected:
	virtual void OnInitialUpdate() override;
	std::vector<Fornitore> m_fornitori;
	int m_preSelectedId;
	bool m_enabled;
};

