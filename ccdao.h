#pragma once
#include <vector>
#include <mariadb++\result_set.hpp>
#include "contocorrente.h"
#include "notfoundex.h"
#include "log.h"

class Azienda;

class ContoCorrenteDao
{
public:
	ContoCorrenteDao();
	~ContoCorrenteDao();

	ContoCorrente GetById(long id);
	std::vector<ContoCorrente> GetByAzienda(const Azienda&);
	bool Insert(ContoCorrente& cc, long idAzienda);
	bool Update(ContoCorrente& cc);
	bool Delete(long idContoCorrente);
private:
	ContoCorrente fromResultset(const mariadb::result_set_ref res);
	Log log;
};