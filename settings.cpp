#include "stdafx.h"
#include "settings.h"
#include "wc_to_mb.h"

Settings::Settings()
	:m_ipaddress{ nullptr }
{
	// open the registry subkey
	CRegKey Key;
	OpenOrCreateKey(Key);

	// leggo...
	try
	{
		//CRegKey Key = OpenKey();

		// ip address
		TCHAR szIpAddress[50] = { 0 };
		ULONG size = 49;

		long res = Key.QueryStringValue(_T("IpAddress"), szIpAddress, &size);
		if (ERROR_SUCCESS != res)
			throw CWinException(_T("QueryStringValue Failed"));

		m_ipaddress.reset(new IpAddress{ szIpAddress });

		//// username
		//TCHAR szUsername[200] = { 0 };
		//ULONG size2 = 199;

		//if (ERROR_SUCCESS != Key.QueryStringValue(_T("Username"), szUsername, &size2))
		//	throw CWinException(_T("QueryStringValue Failed"));
		////m_user.append(szUsername);

		//// password
		//TCHAR szPassword[200] = { 0 };
		//ULONG size3 = 199;

		//if (ERROR_SUCCESS != Key.QueryStringValue(_T("Password"), szPassword, &size3))
		//	throw CWinException(_T("QueryStringValue Failed"));
		////m_pass.append(szPassword);
	}
	catch (const CWinException& e)
	{
		TRACE("*** Failed to load values from registry. ***\n");
		TRACE(e.GetText()); TRACE("\n");

		// Delete the bad key from the registry
		CRegKey ParentKey;
		CString KeyName = _T("Software\\") + CString(_T("GestionaleM2\\Assegni"));
		if (ERROR_SUCCESS == ParentKey.Open(HKEY_CURRENT_USER, KeyName, KEY_READ))
			ParentKey.DeleteSubKey(_T("DB Settings"));
	}
}

Settings::~Settings()
{
}

void Settings::SetDbAddress(IpAddress ip)
{
	// open the registry subkey
	CRegKey Key;
	OpenOrCreateKey(Key);

	// store a text string in the registry
	if (ERROR_SUCCESS != Key.SetStringValue(_T("IpAddress"), ip.str().c_str()))
		throw CWinException(_T("SetStringValue failed"));
}

std::string Settings::GetDbUser() const
{
	// open the registry subkey
	CRegKey Key;
	OpenOrCreateKey(Key);

	// username
	TCHAR szUsername[200] = { 0 };
	ULONG size2 = 199;

	if (ERROR_SUCCESS != Key.QueryStringValue(_T("Username"), szUsername, &size2))
		throw CWinException(_T("QueryStringValue Failed"));
	
	wc_to_mb mb1{ szUsername };
	return mb1.asStdString();
}

void Settings::SetDbUser(const std::string username)
{
	// open the registry subkey
	CRegKey Key;
	OpenOrCreateKey(Key);

	mb_to_wc mw{ username };

	// store a text string in the registry
	if (ERROR_SUCCESS != Key.SetStringValue(_T("Username"), mw.asStdWstring().c_str()))
		throw CWinException(_T("SetStringValue failed"));
}

std::string Settings::GetDbPassword() const
{
	// open the registry subkey
	CRegKey Key;
	OpenOrCreateKey(Key);

	// username
	TCHAR szPassword[200] = { 0 };
	ULONG size2 = 199;

	if (ERROR_SUCCESS != Key.QueryStringValue(_T("Password"), szPassword, &size2))
		throw CWinException(_T("QueryStringValue Failed"));

	wc_to_mb mb1{ szPassword };
	return mb1.asStdString();
}

void Settings::SetDbPassword(const std::string password)
{
	// open the registry subkey
	CRegKey Key;
	OpenOrCreateKey(Key);

	mb_to_wc mw{ password };
	
	// store a text string in the registry
	if (ERROR_SUCCESS != Key.SetStringValue(_T("Password"), mw.asStdWstring().c_str()))
		throw CWinException(_T("SetStringValue failed"));
}

void Settings::OpenOrCreateKey(CRegKey& Key) const
{
	CString KeyName = _T("Software\\") + CString(_T("GestionaleM2\\Assegni"));
	CString SubKeyName = KeyName + _T("\\DB Settings");
	//CRegKey Key;

	// open the registry subkey
	if (ERROR_SUCCESS != Key.Open(HKEY_CURRENT_USER, SubKeyName))
	{
		// Create and open the registry subkey
		if (ERROR_SUCCESS != Key.Create(HKEY_CURRENT_USER, SubKeyName))
			throw CWinException(_T("Create Key failed"));
		if (ERROR_SUCCESS != Key.Open(HKEY_CURRENT_USER, SubKeyName))
			throw CWinException(_T("Open Key failed"));

		// store a text string in the registry
		if (ERROR_SUCCESS != Key.SetStringValue(_T("IpAddress"), _T("192.168.1.112")))
			throw CWinException(_T("SetStringValue failed"));

		if (ERROR_SUCCESS != Key.SetStringValue(_T("Username"), _T("scott")))
			throw CWinException(_T("SetStringValue failed"));

		if (ERROR_SUCCESS != Key.SetStringValue(_T("Password"), _T("tiger")))
			throw CWinException(_T("SetStringValue failed"));
	}
}

//CRegKey Settings::OpenKey() const
//{
//	CString KeyName = _T("Software\\") + CString(_T("GestionaleM2\\Assegni"));
//	CString SubKeyName = KeyName + _T("\\DB Settings");
//	CRegKey Key;
//	
//	// open the registry subkey
//	if (ERROR_SUCCESS != Key.Open(HKEY_CURRENT_USER, SubKeyName))
//	{
//		Key = CreateKey();
//	}
//		//throw CWinException(_T("Open Key failed"));
//
//	return Key;
//}
//
//CRegKey Settings::CreateKey() const
//{
//	CString KeyName = _T("Software\\") + CString(_T("GestionaleM2\\Assegni"));
//	CString SubKeyName = KeyName + _T("\\DB Settings");
//	CRegKey Key;
//
//	// Create and open the registry subkey
//	if (ERROR_SUCCESS != Key.Create(HKEY_CURRENT_USER, SubKeyName))
//		throw CWinException(_T("Create Key failed"));
//	if (ERROR_SUCCESS != Key.Open(HKEY_CURRENT_USER, SubKeyName))
//		throw CWinException(_T("Open Key failed"));
//
//	// store a text string in the registry
//	if (ERROR_SUCCESS != Key.SetStringValue(_T("IpAddress"), _T("192.168.1.112")))
//		throw CWinException(_T("SetStringValue failed"));
//
//	if (ERROR_SUCCESS != Key.SetStringValue(_T("Username"), _T("scott")))
//		throw CWinException(_T("SetStringValue failed"));
//
//	if (ERROR_SUCCESS != Key.SetStringValue(_T("Password"), _T("tiger")))
//		throw CWinException(_T("SetStringValue failed"));
//
//	return Key;
//}
