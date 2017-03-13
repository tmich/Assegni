#pragma once
#include <vector>
#include <string>
#include "azienda.h"
#include "assegno.h"
#include <mariadb++\result_set.hpp>

using namespace std;

namespace services
{
	class Service
	{
	public:
		vector<Assegno> GetAssegniEmessi(const Azienda&, unsigned int anno, unsigned short mese);
		vector<Assegno> GetAssegniEmessi(const Azienda&, unsigned int annoDal, unsigned short meseDal, unsigned int annoAl, unsigned short meseAl);
		vector<Assegno> GetAssegniEmessi(unsigned int annoDal, unsigned short meseDal, unsigned int annoAl, unsigned short meseAl);
	};
}