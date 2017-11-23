#include "stdafx.h"
#include "services.h"
#include "assdao.h"

vector<Assegno> services::Service::GetAssegniEmessi(const ContoCorrente & cc)
{
	AssegnoDao asdao;
	auto assegni = asdao.getEmessi(cc);
	return assegni;
}

vector<Assegno> services::Service::GetAssegniEmessi()
{
	AssegnoDao asdao;
	auto assegni = asdao.getEmessi();
	return assegni;
}

vector<Assegno> services::Service::GetAssegniAnnullati()
{
	AssegnoDao asdao;
	auto assegni = asdao.getAnnullati();
	return assegni;
}
