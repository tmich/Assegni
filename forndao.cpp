#include "stdafx.h"
#include "forndao.h"
#include "dbconn.h"
#include "notfoundex.h"
#include "mariadb++\exceptions.hpp"

FornitoreDao::FornitoreDao()
{

}

FornitoreDao::~FornitoreDao()
{

}

Fornitore FornitoreDao::getById(long id)
{
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, denominazione, indirizzo, telefono, partita_iva 
					FROM palmi.fornitori where id = ?)");
	stmt->set_unsigned32(0, id);
	auto res = stmt->query();

	if (res->next())
	{
		Fornitore forn = fromResultset(res);
		con->disconnect();
		return forn;
	}

	throw NotFoundException();
}

std::vector<Fornitore> FornitoreDao::getAll()
{
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, denominazione, indirizzo, telefono, partita_iva FROM palmi.fornitori where cancellato=0)");
	
	auto res = stmt->query();
	std::vector<Fornitore> fornitori;
	while (res->next())
	{
		Fornitore forn = fromResultset(res);
		fornitori.push_back(forn);
	}
	con->disconnect();
	return fornitori;
}

void FornitoreDao::salva(Fornitore& fornitore)
{
	mydb::Connection conn;
	auto con = conn.connect();
	mariadb::statement_ref stmt;

	if (fornitore.getId() > 0)
	{
		stmt = con->create_statement(R"(UPDATE palmi.fornitori set denominazione=?,indirizzo=?,telefono=?,
					partita_iva=? WHERE id=?)");
		stmt->set_wstring(0, fornitore.getDenominazione());
		stmt->set_wstring(1, fornitore.getIndirizzo());
		stmt->set_wstring(2, fornitore.getTelefono());
		stmt->set_wstring(3, fornitore.getPartitaIva());
		stmt->set_unsigned32(4, fornitore.getId());

		auto ret = stmt->execute();
	}
	else
	{
		//SELECT last_insert_id();
		stmt = con->create_statement("INSERT INTO palmi.fornitori (denominazione,indirizzo,telefono,partita_iva) VALUES (?,?,?,?);");
		stmt->set_wstring(0, fornitore.getDenominazione());
		stmt->set_wstring(1, fornitore.getIndirizzo());
		stmt->set_wstring(2, fornitore.getTelefono());
		stmt->set_wstring(3, fornitore.getPartitaIva());

		auto ret = stmt->execute();
		if (ret == 1)
		{
			auto stmt = con->create_statement("SELECT last_insert_id()");
			auto res = stmt->query();
			if (res->next())
			{
				long id = res->get_unsigned32(0U);
				fornitore.setId(id);
			}
		}
	}
}

Fornitore FornitoreDao::fromResultset(const mariadb::result_set_ref res)
{
	long id = res->get_unsigned32("id");
	std::wstring denominazione = res->get_wstring("denominazione");
	std::wstring indirizzo = res->get_wstring("indirizzo");
	std::wstring telefono = res->get_wstring("telefono");
	std::wstring partita_iva = res->get_wstring("partita_iva");

	Fornitore f{ id,denominazione,indirizzo,telefono,partita_iva };
	return f;
}