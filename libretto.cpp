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
	return mNumero;
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
