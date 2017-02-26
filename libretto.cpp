#include "stdafx.h"
#include "libretto.h"

Libretto::Libretto()
{
}

Libretto::~Libretto()
{
}

std::wstring Libretto::getNumero() const
{
	return mNumero;
}

std::wstring Libretto::getCodice() const
{
	return mCodice;
}

int Libretto::getQta() const
{
	return mQta;
}

long Libretto::getId() const
{
	return mId;
}

long Libretto::getIdConto() const
{
	return mIdConto;
}

std::wstring Libretto::toString() const
{
	return mCodice;
}

void Libretto::setId(long id)
{
	mId = id;
}

void Libretto::setIdConto(long idConto)
{
	mIdConto = idConto;
}

void Libretto::setNumero(std::wstring numero)
{
	mNumero = numero;
}

void Libretto::setCodice(std::wstring codice)
{
	mCodice = codice;
}

void Libretto::setQta(int qta)
{
	mQta = qta;
}

GeneratoreNumeriAssegni::GeneratoreNumeriAssegni(int qta)
	: m_qta{ qta }
{
}

GeneratoreNumeriAssegni::~GeneratoreNumeriAssegni()
{
}

std::vector<std::wstring> GeneratoreNumeriAssegni::Genera(std::wstring primoNumero)
{
	std::vector<std::wstring> numeri;

	// aggiungo il primo assegno
	numeri.push_back(primoNumero);

	//std::wstring base = primoNumero.substr(0, primoNumero.size() - 3);
	//std::wstring s_num = primoNumero.substr(primoNumero.size() - 3);
	//unsigned int num = std::stoi(s_num.c_str());

	std::wstring tmp = _T("1");
	tmp += primoNumero;
	unsigned long long int num = std::stoull(tmp.c_str());
	
	for (int i = 1; i < m_qta; i++)
	{
		std::wostringstream ss;
		//ss << base << std::setw(3) << std::setfill(_T('0')) << ++num;
		ss << ++num;
		numeri.push_back(ss.str().substr(1).c_str());
	}

	return numeri;
}
