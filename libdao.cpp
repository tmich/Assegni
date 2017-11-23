#include "stdafx.h"
#include "libdao.h"
#include "contocorrente.h"
#include "dbconn.h"
#include "notfoundex.h"
#include <mariadb++\exceptions.hpp>

LibrettoDao::LibrettoDao()
{
}

LibrettoDao::~LibrettoDao()
{
}

Libretto LibrettoDao::getById(long id)
{
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement("SELECT id, id_conto_corrente, numero, codice, qta FROM palmi.libretto_assegni where id = ?");
	stmt->set_unsigned32(0, id);
	auto res = stmt->query();

	if (res->next())
	{
		Libretto lib = fromResultset(res);
		con->disconnect();
		return lib;
	}

	throw NotFoundException();
}

std::vector<Libretto> LibrettoDao::getByConto(const ContoCorrente& conto)
{
	std::vector<Libretto> libretti;
	mydb::Connection conn;
	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, id_conto_corrente, numero, codice, qta
		FROM palmi.libretto_assegni where id_conto_corrente = ?)");
	stmt->set_unsigned32(0, conto.getId());
	auto res = stmt->query();

	while (res->next())
	{
		libretti.push_back(fromResultset(res));
	}

	con->disconnect();
	return libretti;
}

Libretto LibrettoDao::insert(long id_conto, std::wstring numero, std::wstring codice, int qta, std::vector<std::wstring> assegni)
{
	mariadb::u32 libId = 0;
	mydb::Connection conn;
	auto con = conn.connect();
	auto tran = con->create_transaction();
	try
	{
		auto stmt = con->create_statement("INSERT INTO palmi.libretto_assegni (id_conto_corrente, numero, codice, qta) VALUES(?, ?, ?, ?);");
		stmt->set_unsigned64(0, id_conto);
		stmt->set_wstring(1, numero);
		stmt->set_wstring(2, codice);
		stmt->set_unsigned32(3, qta);
		libId = stmt->insert();

		// inserisco gli assegni
		auto stmt2 = con->create_statement("INSERT INTO palmi.assegno (id_libretto, numero) VALUES (?,?);");

		for (auto const& nAss : assegni)
		{
			stmt2->set_unsigned64(0, libId);
			stmt2->set_wstring(1, nAss);
			stmt2->insert();
		}

		tran->commit();
	}
	catch (std::exception&)
	{
		
	}

	con->disconnect();

	Libretto lib;
	lib.setId(libId);
	lib.setNumero(numero);
	lib.setCodice(codice);
	lib.setQta(qta);
	return lib;
}

int LibrettoDao::elimina(long idLibretto)
{
	auto res = eliminaAssegni(idLibretto);

	mydb::Connection conn;
	auto con = conn.connect();
	auto stmt = con->create_statement(R"(DELETE FROM palmi.libretto_assegni WHERE id = ?)");
	stmt->set_unsigned32(0, idLibretto);
	res = stmt->execute();
	con->disconnect();

	return res;
}

int LibrettoDao::eliminaAssegni(long idLibretto)
{
	mydb::Connection conn;
	auto con = conn.connect();
	auto stmt = con->create_statement(R"(DELETE FROM palmi.assegno WHERE id_libretto = ?)");
	stmt->set_unsigned32(0, idLibretto);
	auto res = stmt->execute();
	con->disconnect();
	return res;
}

Libretto LibrettoDao::fromResultset(const mariadb::result_set_ref res)
{
	// id, numero, codice, qta, note
	long id = res->get_unsigned32("id");
	long idConto = res->get_unsigned32("id_conto_corrente");
	std::wstring numero = res->get_wstring("numero");
	std::wstring codice = res->get_wstring("codice");
	int qta = res->get_unsigned32("qta");

	Libretto l;
	l.setId(id);
	l.setIdConto(idConto);
	l.setNumero(numero);
	l.setCodice(codice);
	l.setQta(qta);
	return l;
}
