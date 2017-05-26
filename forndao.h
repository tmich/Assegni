#pragma once
#include <vector>
#include <mariadb++\result_set.hpp>
#include "fornitore.h"

class Libretto;

class FornitoreDao
{
public:
	FornitoreDao();
	~FornitoreDao();

	Fornitore getById(long id);
	std::vector<Fornitore> getAll();
	
	void salva(Fornitore& fornitore);
	/*void annulla(Fornitore& fornitore);*/
private:
	/*std::string createQuery();
	std::string createQuery(long id);*/

	std::vector<Fornitore> execQuery(const std::string& query, const unsigned int id = 0);
	Fornitore fromResultset(const mariadb::result_set_ref);
	/*std::string m_sql;*/
};
