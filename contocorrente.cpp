#include "stdafx.h"
#include "contocorrente.h"

ContoCorrente::ContoCorrente(const std::wstring & numero, const std::wstring & banca, const std::wstring & sede, const std::wstring & agenzia)
	:mNumero(numero), mBanca(banca), mSede(sede), mAgenzia(agenzia)
{
}

ContoCorrente::~ContoCorrente()
{
}

std::wstring ContoCorrente::toString() const
{
	return mNumero + _T(" ") + mBanca;
}

void ContoCorrente::setId(long id)
{
	mId = id;
}

void ContoCorrente::setIdAzienda(long idConto)
{
	mIdAzienda = idConto;
}
