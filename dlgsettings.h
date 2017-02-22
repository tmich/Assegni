#pragma once
#include "wxx_dialog.h"
#include "wxx_controls.h"
#include "settings.h"

class DlgSettings : public CDialog
{
public:
	DlgSettings();
	virtual ~DlgSettings();

	//virtual IpAddress GetIpAddress();
	
private:
	virtual void OnOK();

	BOOL OnInitDialog() override;
	CIPAddress m_Ip;
	CEdit m_tUser, m_tPass;
	Settings m_Settings;
};