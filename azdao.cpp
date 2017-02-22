#include "stdafx.h"
#include "dbconn.h"
#include "azdao.h"
#include <mariadb++\connection.hpp>
#include <mariadb++\statement.hpp>

AziendaDao::AziendaDao()
{
}

AziendaDao::~AziendaDao()
{
}

void AziendaDao::getById(int id, Azienda& azienda)
{
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement("select id, ragione_sociale, indirizzo from palmi.azienda where id = ?");
	stmt->set_unsigned16(0, id);
	auto res = stmt->query();
	
	if (res->next())
	{
		azienda = fromResultset(res);
	}

	con->disconnect();
}

void AziendaDao::save(Azienda azienda) 
{
	if (azienda.getId() == 0)
	{
		//insert
		//azienda.setId(nuovoid);
	}
	else
	{
		//update
	}
}

Azienda AziendaDao::fromResultset(const mariadb::result_set_ref res)
{
	Azienda azienda;
	int theId = res->get_unsigned16("id");
	std::wstring rsoc = res->get_wstring("ragione_sociale");
	std::wstring indz = res->get_wstring("indirizzo");

	azienda.setId(theId);
	azienda.mRagioneSociale = rsoc;
	azienda.mIndirizzo = indz;

	return azienda;
}

std::vector<Azienda> AziendaDao::all() 
{
	std::vector<Azienda> aziende;

	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement("select id, ragione_sociale, indirizzo from palmi.azienda");
	auto res = stmt->query();

	while (res->next())
	{
		aziende.push_back(fromResultset(res));
	}

	con->disconnect();
	return aziende;
}
