#pragma once
#include <wxx_controls.h>
#include "contocorrente.h"

class Azienda;

class CComboConti : public CComboBox
{
public:
	CComboConti();
	~CComboConti();

	void Aggiorna();
	void Aggiorna(const Azienda&);
	ContoCorrente GetSelectedItem() const;
private:
	std::vector<ContoCorrente> m_conti;
};