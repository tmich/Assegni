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
