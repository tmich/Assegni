#pragma once
#include "wxx_controls.h"

class CComboAnni :
	public CComboBox
{
public:
	CComboAnni();
	virtual ~CComboAnni();

	unsigned int GetSelectedItem();
protected:
	void OnInitialUpdate() override;
};

