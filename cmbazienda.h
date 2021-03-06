#pragma once
#include <wxx_controls.h>
#include "azdao.h"

class CComboAziende : public CComboBox
{
public:
	CComboAziende();
	~CComboAziende();

	Azienda GetSelectedItem() const;
	void SetSelectedItem(const Azienda&, bool disabled = false);
protected:
	virtual void OnInitialUpdate() override;
	std::vector<Azienda> m_aziende;
	int m_preSelectedId;
	bool m_enabled;
};

class CComboAziendeTutte : public CComboAziende
{
public:
	Azienda GetSelectedItem() const;
private:
	virtual void OnInitialUpdate() override;
};