#include "stdafx.h"
#include "azienda.h"

Azienda::Azienda()
	: mId{ 0 }
{
}

Azienda::Azienda(const std::wstring& ragioneSociale, const std::wstring& indirizzo)
	: mRagioneSociale(ragioneSociale), mIndirizzo(indirizzo)
{
	mId = 0;
}

Azienda::Azienda(int id, const std::wstring & ragioneSociale, const std::wstring & indirizzo)
	:Azienda(ragioneSociale, indirizzo)
{
	mId = id;
}

Azienda::~Azienda()
{
}

void Azienda::setPartitaIva(const std::wstring partitaIva)
{
	mPiva = partitaIva;
}

void Azienda::setRagioneSociale(const std::wstring ragioneSociale)
{
	mRagioneSociale = ragioneSociale;
}

void Azienda::setIndirizzo(const std::wstring indirizzo)
{
	mIndirizzo = indirizzo;
}
