#pragma once
#include <vector>
#include <mariadb++\result_set.hpp>
#include "contocorrente.h"
#include "notfoundex.h"

class Azienda;

class ContoCorrenteDao
{
public:
	ContoCorrenteDao();
	~ContoCorrenteDao();

	ContoCorrente GetById(long id);
	std::vector<ContoCorrente> GetByAzienda(const Azienda&);
private:
	ContoCorrente fromResultset(const mariadb::result_set_ref res);
};