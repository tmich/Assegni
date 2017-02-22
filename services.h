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
	};
}