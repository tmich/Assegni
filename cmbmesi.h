#pragma once
#include <wxx_controls.h>
#include <map>

class CComboMesi : public CComboBox
{
public:
	CComboMesi() {};
	virtual ~CComboMesi() {};

	unsigned short GetSelectedItem();
protected:
	void OnInitialUpdate() override;
};