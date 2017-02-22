#pragma once
#include <wxx_controls.h>
#include "libretto.h"

class ContoCorrente;

class CComboLibretti : public CComboBox
{
public:
	CComboLibretti();
	~CComboLibretti();

	void Aggiorna(const ContoCorrente&);
	Libretto * GetSelectedItem() const;
private:
	std::vector<Libretto> m_libretti;
};