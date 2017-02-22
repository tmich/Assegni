#include "stdafx.h"
#include "services.h"
#include "datetime.h"
#include "assdao.h"

vector<Assegno> services::Service::GetAssegniEmessi(const Azienda & azienda, unsigned int anno, unsigned short mese)
{
	AssegnoDao asdao;
	return asdao.getEmessi(azienda, anno, mese);
}
