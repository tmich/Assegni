#include "stdafx.h"
#include "ccdao.h"
#include "dbconn.h"
#include "azienda.h"

ContoCorrenteDao::ContoCorrenteDao()
{
}

ContoCorrenteDao::~ContoCorrenteDao()
{
}

ContoCorrente ContoCorrenteDao::GetById(long id)
{
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, id_azienda, numero, banca, sede, agenzia,
		note FROM palmi.conto_corrente where id = ?)");
		stmt->set_unsigned32(0, id);
	auto res = stmt->query();

	if (res->next())
	{
		ContoCorrente cc = fromResultset(res);
		con->disconnect();
		return cc;
	}

	throw NotFoundException();
}

std::vector<ContoCorrente> ContoCorrenteDao::GetAll()
{
	std::vector<ContoCorrente> conti;
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, id_azienda, numero, banca, sede, agenzia,
		note FROM palmi.conto_corrente)");
	auto res = stmt->query();

	while (res->next())
	{
		conti.push_back(fromResultset(res));
	}

	con->disconnect();
	return conti;
}

std::vector<ContoCorrente> ContoCorrenteDao::GetByAzienda(const Azienda & az)
{
	std::vector<ContoCorrente> conti;
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, id_azienda, numero, banca, sede, agenzia,
		note FROM palmi.conto_corrente where id_azienda = ?)");
	stmt->set_unsigned16(0, az.getId());
	auto res = stmt->query();

	while (res->next())
	{
		conti.push_back(fromResultset(res));
	}

	con->disconnect();
	return conti;
}

bool ContoCorrenteDao::Insert(ContoCorrente & cc, long idAzienda)
{
	mydb::Connection conn;

	try
	{
		auto con = conn.connect();
		auto stmt = con->create_statement(R"(INSERT INTO palmi.conto_corrente(id_azienda,numero,banca,sede,agenzia,note) VALUES (?,?,?,?,?,?); )");
		stmt->set_unsigned32(0, idAzienda);
		stmt->set_wstring(1, cc.getNumero());
		stmt->set_wstring(2, cc.getBanca());
		stmt->set_wstring(3, cc.getSede());
		stmt->set_wstring(4, cc.getAgenzia());
		stmt->set_wstring(5, cc.getNote());
		long id = stmt->insert();
		con->disconnect();
		cc.setId(id);
		cc.setIdAzienda(idAzienda);
		return true;
	}
	catch (const std::exception& exc)
	{
		log.Write(exc.what());
		return false;
	}
}

bool ContoCorrenteDao::Update(ContoCorrente & cc)
{
	mydb::Connection conn;

	try
	{
		auto con = conn.connect();
		auto stmt = con->create_statement(R"(UPDATE palmi.conto_corrente SET numero=?,banca=?,sede=?,agenzia=?,note=? WHERE id=?)");
		stmt->set_wstring(0, cc.getNumero());
		stmt->set_wstring(1, cc.getBanca());
		stmt->set_wstring(2, cc.getSede());
		stmt->set_wstring(3, cc.getAgenzia());
		stmt->set_wstring(4, cc.getNote());
		stmt->set_unsigned32(5, cc.getId());
		long id = stmt->insert();
		con->disconnect();
		return true;
	}
	catch (const std::exception& exc)
	{
		log.Write(exc.what());
		return false;
	}
}

bool ContoCorrenteDao::Delete(long idContoCorrente)
{
	mydb::Connection conn;

	try
	{
		auto con = conn.connect();
		auto stmt = con->create_statement(R"(DELETE FROM palmi.conto_corrente WHERE id=?)");
		stmt->set_unsigned32(0, idContoCorrente);
		long recordsAffected = stmt->execute();
		con->disconnect();
		return recordsAffected == 1;
	}
	catch (const std::exception& exc)
	{
		std::string error{ "DELETE FROM palmi.conto_corrente WHERE id=" };
		error.append(std::to_string(idContoCorrente));
		error.append(" ");
		error.append(exc.what());
		log.Write(error.c_str());
		return false;
	}
}

ContoCorrente ContoCorrenteDao::fromResultset(const mariadb::result_set_ref res)
{
	long id = res->get_unsigned32("id");
	long id_azienda = res->get_unsigned32("id_azienda");
	std::wstring numero = res->get_wstring("numero");
	std::wstring banca = res->is_null("banca") ? _T("") : res->get_wstring("banca");
	std::wstring sede = res->is_null("sede") ? _T("") : res->get_wstring("sede");
	std::wstring agenzia = res->is_null("agenzia") ? _T("") : res->get_wstring("agenzia");
	std::wstring note = res->is_null("note") ? _T("") : res->get_wstring("note");
	ContoCorrente cc{ numero, banca, sede, agenzia };
	cc.setNote(note);
	cc.setId(id);
	cc.setIdAzienda(id_azienda);
	return cc;
}
