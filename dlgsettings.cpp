#include "stdafx.h"
#include "dlgsettings.h"
#include "resource.h"
#include "wc_to_mb.h"
#include "dbconn.h"
#include <mariadb++\exceptions.hpp>

DlgSettings::DlgSettings()
	:CDialog{ IDD_SETTINGS }
{
}

DlgSettings::~DlgSettings()
{
}

//IpAddress DlgSettings::GetIpAddress()
//{
//	BYTE m_Field0 = 0, m_Field1 = 0, m_Field2 = 0, m_Field3 = 0;
//	m_Ip.GetAddress(m_Field0, m_Field1, m_Field2, m_Field3);
//	return IpAddress{ m_Field0 ,m_Field1,m_Field2,m_Field3 };
//}

//void DlgSettings::SetIpAddress(BYTE nField0, BYTE nField1, BYTE nField2, BYTE nField3)
//{
//	m_Field0 = nField0;
//	m_Field1 = nField1;
//	m_Field2 = nField2;
//	m_Field3 = nField3;
//}

void DlgSettings::OnOK()
{
	BYTE m_Field0 = 0, m_Field1 = 0, m_Field2 = 0, m_Field3 = 0;
	m_Ip.GetAddress(m_Field0, m_Field1, m_Field2, m_Field3);
	IpAddress ip{ m_Field0 ,m_Field1,m_Field2,m_Field3 };
	std::wstring wuser = m_tUser.GetWindowTextW();
	std::wstring wpass = m_tPass.GetWindowTextW();

	wc_to_mb wc1{ wuser };
	wc_to_mb wc2{ wpass };

	m_Settings.SetDbAddress(ip);
	m_Settings.SetDbUser(wc1.asStdString().c_str());
	m_Settings.SetDbPassword(wc2.asStdString().c_str());

	// prova di connessione
	try
	{
		mydb::Connection cnn;
		auto cref = cnn.connect();
		cref->disconnect();

		::MessageBox(*this,
			_T("Connessione riuscita."),
			_T("Ok"),
			MB_ICONINFORMATION);

		CDialog::OnOK();
	}
	catch (const mariadb::exception::connection&)
	{
		::MessageBox(*this,
			_T("Errore nella connessione al database. Controllare i parametri e riprovare."),
			_T("Attenzione"),
			MB_ICONWARNING);
	}
}

BOOL DlgSettings::OnInitDialog()
{
	AttachItem(IDC_IPADDRESS, m_Ip);
	AttachItem(IDC_TXTUSER, m_tUser);
	AttachItem(IDC_TXTPASS, m_tPass);

	IpAddress ip{ m_Settings.GetDbAddress() };
	std::wstring wUser, wPass;
	mb_to_wc mb1{ m_Settings.GetDbUser() };
	mb_to_wc mb2{ m_Settings.GetDbPassword() };

	m_Ip.SetAddress(ip.Field0(), ip.Field1(), ip.Field2(), ip.Field3());
	m_tUser.SetWindowTextW(mb1.asStdWstring().c_str());
	m_tPass.SetWindowTextW(mb2.asStdWstring().c_str());
	return 0;
}
