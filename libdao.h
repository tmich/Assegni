#pragma once
#include <vector>
#include <mariadb++\result_set.hpp>
#include "libretto.h"

class ContoCorrente;

class LibrettoDao
{
public:
	LibrettoDao();
	~LibrettoDao();

	Libretto getById(long id);
	std::vector<Libretto> getByConto(const ContoCorrente&);
	Libretto insert(long id_conto, std::wstring numero, std::wstring codice, int qta, std::vector<std::wstring> assegni);
	int elimina(long idLibretto);
	int eliminaAssegni(long idLibretto);
private:
	Libretto fromResultset(const mariadb::result_set_ref res);
};