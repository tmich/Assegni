#include "stdafx.h"
#include "services.h"
#include "datetime.h"
#include "assdao.h"

vector<Assegno> services::Service::GetAssegniEmessi(const Azienda & azienda, unsigned int anno, unsigned short mese)
{
	AssegnoDao asdao;
	return asdao.getEmessi(azienda, anno, mese);
}

vector<Assegno> services::Service::GetAssegniEmessi(const Azienda & azienda, unsigned int annoDal, unsigned short meseDal, unsigned int annoAl, unsigned short meseAl)
{
	AssegnoDao asdao;
	return asdao.getEmessi(azienda, annoDal, annoAl, meseDal, meseAl);
}

vector<Assegno> services::Service::GetAssegniEmessi(unsigned int annoDal, unsigned short meseDal, unsigned int annoAl, unsigned short meseAl)
{
	AssegnoDao asdao;
	return asdao.getEmessi(annoDal, annoAl, meseDal, meseAl);
}
