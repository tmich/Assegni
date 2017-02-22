#pragma once
#include "ipaddress.h"
#include <memory>
#include <wxx_regkey.h>

class Settings
{
public:
	Settings();
	virtual ~Settings();

	IpAddress GetDbAddress() const { return *m_ipaddress; }
	void SetDbAddress(IpAddress ip);

	std::string GetDbUser() const;
	void SetDbUser(const std::string username);

	std::string GetDbPassword() const;
	void SetDbPassword(const std::string password);
protected:
	void OpenOrCreateKey(CRegKey&) const;
	/*CRegKey OpenKey() const;
	CRegKey CreateKey() const;*/
	std::unique_ptr<IpAddress> m_ipaddress;
	//std::wstring m_user, m_pass;
};