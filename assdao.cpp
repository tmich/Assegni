#include "stdafx.h"
#include "assdao.h"
#include "libretto.h"
#include "dbconn.h"
#include "notfoundex.h"
#include "mariadb++\exceptions.hpp"

AssegnoDao::AssegnoDao()
{
	m_sql = "SELECT a.id,a.id_libretto,a.numero,a.data_emissione,a.data_scadenza,a.beneficiario,a.importo,a.data_incasso,a.note,a.annullato ";
	m_sql += "FROM palmi.assegno a INNER JOIN palmi.libretto_assegni lib ON lib.id = a.id_libretto  ";
	m_sql += "INNER JOIN palmi.conto_corrente cc on cc.id = lib.id_conto_corrente ";
	m_sql += "INNER JOIN palmi.azienda az on az.id = cc.id_azienda ";
	m_sql += "WHERE a.data_incasso IS NULL and a.data_emissione IS NOT NULL ";
}

AssegnoDao::~AssegnoDao()
{
}

Assegno AssegnoDao::getById(long id)
{
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, id_libretto, numero, data_emissione, data_scadenza,
		beneficiario, importo, data_incasso, note, annullato FROM palmi.assegno where id = ?)");
	stmt->set_unsigned32(0, id);
	auto res = stmt->query();

	if (res->next())
	{
		Assegno ass = fromResultset(res);
		con->disconnect();
		return ass;
	}

	throw NotFoundException();
}

std::vector<Assegno> AssegnoDao::getByLibretto(long idLibretto)
{
	std::vector<Assegno> assegni;
	mydb::Connection conn;

	auto con = conn.connect();
	auto stmt = con->create_statement(R"(SELECT id, id_libretto, numero, data_emissione, data_scadenza,
		beneficiario, importo, data_incasso, note FROM palmi.assegno where id_libretto = ? AND annullato = 0)");
	stmt->set_unsigned32(0, idLibretto);
	auto res = stmt->query();

	while (res->next())
	{
		assegni.push_back(fromResultset(res));
	}

	con->disconnect();
	return assegni;
}

std::vector<Assegno> AssegnoDao::getEmessi(const Azienda &az)
{
	std::string sql = createQuery(az);
	sql += " AND annullato = 0;";
	std::vector<Assegno> assegniEmessi = execQuery(sql, az.getId());
	return assegniEmessi;
}

std::vector<Assegno> AssegnoDao::getEmessi()
{
	std::string sql = createQuery();
	sql += " AND annullato = 0;";
	std::vector<Assegno> assegniEmessi = execQuery(sql);
	return assegniEmessi;
}

std::vector<Assegno> AssegnoDao::getEmessi(const ContoCorrente & cc)
{
	std::string sql = createQuery(cc);
	sql += " AND annullato = 0;";
	std::vector<Assegno> assegniEmessi = execQuery(sql, cc.getId());
	return assegniEmessi;
}

std::vector<Assegno> AssegnoDao::getAnnullati()
{
	std::string sql = createQuery();
	sql += " AND annullato = 1;";
	std::vector<Assegno> assegniAnnullati = execQuery(sql);
	return assegniAnnullati;
}

void AssegnoDao::salva(Assegno & assegno)
{
	mydb::Connection conn;
	auto con = conn.connect();
	
	if (assegno.getId() > 0)
	{
		mariadb::statement_ref stmt;
		
		if (assegno.incassato())
		{
			stmt = con->create_statement(R"(UPDATE palmi.assegno set data_incasso=?, note=? WHERE id=?)");
			dtm::date dtIncass = assegno.getDataIncasso();
			stmt->set_date(0, mariadb::date_time{ dtIncass.year(), dtIncass.month(), dtIncass.day() });
			stmt->set_wstring(1, assegno.getNote());
			stmt->set_unsigned32(2, assegno.getId());
		}
		else
		{
			stmt = con->create_statement(R"(UPDATE palmi.assegno set numero=?,beneficiario=?,data_emissione=?,
						importo=?,note=?,data_scadenza=?,annullato=? WHERE id=?)");
			stmt->set_wstring(0, assegno.getNumero());
			stmt->set_wstring(1, assegno.getBeneficiario());
			dtm::date dtEmis = assegno.getDataEmissione();
			stmt->set_date(2, mariadb::date_time{ dtEmis.year(), dtEmis.month(), dtEmis.day() });
			stmt->set_double(3, assegno.getImporto());
			stmt->set_wstring(4, assegno.getNote());
			dtm::date dtScad = assegno.getDataScadenza();
			stmt->set_date(5, mariadb::date_time{ dtScad.year(), dtScad.month(), dtScad.day() });
			stmt->set_unsigned32(6, (assegno.annullato() ? 1 : 0));
			stmt->set_unsigned32(7, assegno.getId());
		}
		
		auto ret = stmt->execute();
	}
}

void AssegnoDao::annullaEmissione(Assegno& assegno)
{
	mydb::Connection conn;
	auto con = conn.connect();

	if (assegno.getId() > 0)
	{
		mariadb::statement_ref stmt;
		stmt = con->create_statement(R"(UPDATE palmi.assegno set beneficiario=null,data_emissione=null,
						importo=null,note=null,data_scadenza=null,data_incasso=null WHERE id=?)");
		stmt->set_unsigned32(0, assegno.getId());
		auto ret = stmt->execute();
		
		if (ret == 1)
		{
			assegno.annullaEmissione();
		}
	}
}

void AssegnoDao::annulla(Assegno & assegno)
{
	mydb::Connection conn;
	auto con = conn.connect();

	if (assegno.getId() > 0)
	{
		mariadb::statement_ref stmt;
		stmt = con->create_statement(R"(UPDATE palmi.assegno set annullato=1 WHERE id=?)");
		stmt->set_unsigned32(0, assegno.getId());
		auto ret = stmt->execute();

		if (ret == 1)
		{
			assegno.annulla();
		}
	}
}

std::string AssegnoDao::createQuery(const Azienda& az)
{
	std::string sql = createQuery();
	sql += " and az.id = ? ";
	return sql;
}

std::string AssegnoDao::createQuery(const ContoCorrente & cc)
{
	std::string sql = createQuery();
	sql += "AND cc.id = ?";
	return sql;
}

std::vector<Assegno> AssegnoDao::execQuery(const std::string & query, const unsigned int id)
{
	mydb::Connection conn;
	auto con = conn.connect();

	std::vector<Assegno> assegniEmessi;
	try
	{
		auto stmt = con->create_statement(query.c_str());

		if (id > 0)
		{
			stmt->set_unsigned32(0, id);
		}

		auto res = stmt->query();

		while (res->next())
		{
			Assegno a{ fromResultset(res) };
			assegniEmessi.push_back(a);
		}

		auto errnum = stmt->error_no();
	}
	catch (const mariadb::exception::connection& exc)
	{
		TRACE(exc.what());
	}

	con->disconnect();
	return assegniEmessi;
}


std::string AssegnoDao::createQuery()
{
	return std::string{ m_sql };
}

Assegno AssegnoDao::fromResultset(const mariadb::result_set_ref res)
{
	long id = res->get_unsigned32("id");
	long idLibretto = res->get_unsigned32("id_libretto");
	std::wstring numero = res->get_wstring("numero");
	//short annullato = res->get_unsigned8("annullato");
	//bool annullato = res->get_boolean("annullato");

	Assegno ass{ numero };
	ass.mId = id;
	ass.setIdLibretto(idLibretto);
	//ass.mAnnullato = (annullato == 1);

	if (!res->is_null("data_emissione"))
	{
		mariadb::date_time data_emissione = res->get_date_time("data_emissione");
		mariadb::date_time data_scadenza = res->get_date_time("data_scadenza");
		dtm::date dt_em{ data_emissione.day(), data_emissione.month(), data_emissione.year() };
		dtm::date dt_sc{ data_scadenza.day(),data_scadenza.month(),data_scadenza.year() };
		std::wstring beneficiario = res->get_wstring("beneficiario");
		double importo = std::stod(res->get_string("importo"));
		std::wstring note = _T("");
		if (!res->is_null("note"))
		{
			note = res->get_wstring("note");
		}
		ass.emetti(beneficiario, dt_em, dt_sc, importo, note);
	}
	
	if (!res->is_null("data_incasso"))
	{
		mariadb::date_time data_incasso = res->get_date_time("data_incasso");
		dtm::date dt_inc{ data_incasso.day(), data_incasso.month(), data_incasso.year() };
		ass.incassa(dt_inc);
	}

	if (res->get_unsigned8("annullato") == 1)
	{
		ass.annulla();
	}

	return ass;
}
