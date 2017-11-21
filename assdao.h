#pragma once
#include <vector>
#include <mariadb++\result_set.hpp>
#include "assegno.h"
#include "contocorrente.h"
#include "azienda.h"

class Libretto;

class AssegnoDao
{
public:
	AssegnoDao();
	~AssegnoDao();

	Assegno getById(long id);
	std::vector<Assegno> getByLibretto(long idLibretto);
	//std::vector<Assegno> getByLibretto(const Libretto&);
	std::vector<Assegno> getEmessi(const Azienda&);
	std::vector<Assegno> getEmessi();
	std::vector<Assegno> getEmessi(const ContoCorrente& cc);
	std::vector<Assegno> getAnnullati();
	// and annullato = 0
	void salva(Assegno& assegno);
	void annullaEmissione(Assegno& assegno);
	void annulla(Assegno& assegno);
private:
	/*std::string createQuery(unsigned int annoDal, unsigned int annoAl, unsigned short meseDal = 0, unsigned short meseAl = 0);
	std::string createQuery(const Azienda& az, unsigned int annoDal, unsigned int annoAl, unsigned short meseDal = 0, unsigned short meseAl = 0);
	std::string createQuery(const ContoCorrente& cc, unsigned int annoDal, unsigned int annoAl, unsigned short meseDal = 0, unsigned short meseAl = 0);*/
	std::string createQuery();
	std::string createQuery(const Azienda& az);
	std::string createQuery(const ContoCorrente& cc);

	std::vector<Assegno> execQuery(const std::string& query, const unsigned int id = 0);
	Assegno fromResultset(const mariadb::result_set_ref);
	std::string m_sql;
};