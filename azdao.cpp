#include "stdafx.h"
#include "dbconn.h"
#include "azdao.h"
#include <mariadb++\connection.hpp>
#include <mariadb++\statement.hpp>

AziendaDao::AziendaDao()
	: qry{ "select id, ragione_sociale, indirizzo, partita_iva from palmi.azienda " }
{
}

AziendaDao::~AziendaDao()
{
}

void AziendaDao::getById(int id, Azienda& azienda)
{
	mydb::Connection conn;

	auto con = conn.connect();
	std::string _qry = qry + " where id = ?";
	auto stmt = con->create_statement(_qry.c_str());
	stmt->set_unsigned16(0, id);
	auto res = stmt->query();
	
	if (res->next())
	{
		azienda = fromResultset(res);
	}

	con->disconnect();
}

void AziendaDao::save(Azienda& azienda) 
{
	try
	{
		mydb::Connection conn;
		auto con = conn.connect();

		if (azienda.getId() == 0)
		{
			//insert

			auto stmt = con->create_statement("insert into palmi.azienda (ragione_sociale, indirizzo, partita_iva) values (?, ?, ?);");
			stmt->set_wstring(0, azienda.getRagioneSociale());
			stmt->set_wstring(1, azienda.getIndirizzo());
			stmt->set_wstring(2, azienda.getPartitaIva());

			int nuovoId = stmt->insert();
			azienda.setId(nuovoId);
		}
		else
		{
			//update
			mydb::Connection conn;
			auto con = conn.connect();
			auto stmt = con->create_statement("update palmi.azienda set ragione_sociale = ?, indirizzo = ?, partita_iva = ? where id = ?;");
			stmt->set_wstring(0, azienda.getRagioneSociale());
			stmt->set_wstring(1, azienda.getIndirizzo());
			stmt->set_wstring(2, azienda.getPartitaIva());
			stmt->set_unsigned32(3, azienda.getId());

			int rows = stmt->execute();
		}
	}
	catch (const std::exception&)
	{
		throw;
	}
}

void AziendaDao::Delete(long idAzienda)
{
	mydb::Connection conn;
	try
	{
		auto con = conn.connect();
		auto stmt = con->create_statement("delete from palmi.azienda where id = ?;");
		stmt->set_unsigned32(0, idAzienda);
		stmt->execute();
	}
	catch (const std::exception&)
	{
		throw;
	}
}

Azienda AziendaDao::fromResultset(const mariadb::result_set_ref res)
{
	Azienda azienda;
	int theId = res->get_unsigned16("id");
	std::wstring rsoc = res->get_wstring("ragione_sociale");
	std::wstring indz = res->get_wstring("indirizzo");
	std::wstring piva = res->is_null("partita_iva") ? _T("") : res->get_wstring("partita_iva");

	azienda.setId(theId);
	azienda.mRagioneSociale = rsoc;
	azienda.mIndirizzo = indz;
	azienda.mPiva = piva;

	return azienda;
}

std::vector<Azienda> AziendaDao::all() 
{
	std::vector<Azienda> aziende;

	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(qry.c_str());
	auto res = stmt->query();

	while (res->next())
	{
		aziende.push_back(fromResultset(res));
	}

	con->disconnect();
	return aziende;
}
