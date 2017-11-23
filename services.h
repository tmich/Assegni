#pragma once
#include <vector>
#include <string>
#include "azienda.h"
#include "assegno.h"
#include "contocorrente.h"
#include <mariadb++\result_set.hpp>

using namespace std;

namespace services
{
	class Service
	{
	public:
		vector<Assegno> GetAssegniEmessi(const ContoCorrente&);
		vector<Assegno> GetAssegniEmessi();
		vector<Assegno> GetAssegniAnnullati();
	};
}