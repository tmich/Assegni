#pragma once
#include <wxx_controls.h>
#include "assegno.h"

class CComboAssegni : public CComboBox
{
public:
	CComboAssegni();
	~CComboAssegni();

	void Aggiorna(const std::vector<Assegno>);
	Assegno * GetSelectedItem() const;
private:
	//virtual void OnInitialUpdate() override;
	std::vector<Assegno> m_assegni;
};