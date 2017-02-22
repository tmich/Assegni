#pragma once
#include <mariadb++\connection.hpp>

namespace mydb
{
	class Connection
	{
	public:
		Connection();
		virtual ~Connection();

		mariadb::connection_ref connect();
	protected:
		mariadb::connection_ref conn;
	};
}