#include "stdafx.h"
#include "azienda.h"

Azienda::Azienda(const std::wstring& ragioneSociale, const std::wstring& indirizzo)
	:mRagioneSociale(ragioneSociale), mIndirizzo(indirizzo)
{
}

Azienda::Azienda(int id, const std::wstring & ragioneSociale, const std::wstring & indirizzo)
	:Azienda(ragioneSociale, indirizzo)
{
	mId = id;
}

Azienda::~Azienda()
{
}