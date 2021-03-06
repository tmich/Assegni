#pragma once
#include <vector>
#include <mariadb++\result_set.hpp>
#include "azienda.h"

class AziendaDao
{
public:
	AziendaDao();
	~AziendaDao();

	void getById(int id, Azienda& azienda);
	std::vector<Azienda> all();
	void save(Azienda& azienda);
	void Delete(long idAzienda);
private:
	Azienda fromResultset(const mariadb::result_set_ref res);
	std::string qry;
};