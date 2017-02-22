#include "stdafx.h"
#include "dbconn.h"
#include <stdexcept>
#include "settings.h"
#include <mariadb++\exceptions.hpp>

mydb::Connection::Connection()
{
	Settings settings;
	auto acc_login = mariadb::account::create(settings.GetDbAddress().mb_str().c_str(), 
		settings.GetDbUser().c_str(), settings.GetDbPassword().c_str());
	conn = mariadb::connection::create(acc_login);
}

mydb::Connection::~Connection()
{
	if (conn->connected())
	{
		conn->disconnect();
		TRACE("Disconnesso");
	}
}

mariadb::connection_ref mydb::Connection::connect()
{
	try
	{
		conn->connect();

		if (conn->connected())
		{
			TRACE("Connesso");
			return conn;
		}
		else
		{
			throw std::runtime_error("Connessione non riuscita");
		}
	}
	catch (const mariadb::exception::connection& cnex)
	{
		throw cnex;
	}
	catch (const std::exception& ee)
	{
		throw ee;
	}
}