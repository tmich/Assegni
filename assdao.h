#pragma once
#include <vector>
#include <mariadb++\result_set.hpp>
#include "assegno.h"
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
	std::vector<Assegno> getEmessi(const Azienda&, unsigned int anno, unsigned short mese=0);
	void salva(Assegno& assegno);
	void annulla(Assegno& assegno);
private:
	Assegno fromResultset(const mariadb::result_set_ref);
};