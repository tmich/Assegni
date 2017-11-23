#include "stdafx.h"
#include "assegno.h"
#include <stdexcept>

Assegno::Assegno()
	: mEmesso(false), mIncassato(false), mAnnullato(false)
{
}

Assegno::Assegno(std::wstring numero)
	: Assegno()
{
	mNumero = numero;
}

Assegno::Assegno(std::wstring numero, std::wstring beneficiario, date dataEmissione, double importo, std::wstring note)
	: Assegno{ numero }
{
	mBeneficiario = beneficiario;
	mDataEmissione = dataEmissione;
	mImporto = importo;
	mNote = note;
}

Assegno::Assegno(std::wstring numero, std::wstring beneficiario, date dataEmissione, double importo, std::wstring note, date dataIncasso)
	:Assegno(numero,beneficiario,dataEmissione,importo,note)
{
	mDataIncasso = dataIncasso;
	mIncassato = true;
}

Assegno::~Assegno()
{
}

void Assegno::emetti(std::wstring beneficiario, date dataEmissione, date dataScadenza, double importo, std::wstring note)
{
	if (mNumero.empty())
	{
		throw std::invalid_argument("Impossibile emettere un assegno senza numero.");
	}

	if (mEmesso)
	{
		throw std::invalid_argument("Impossibile emettere un assegno già emesso.");
	}

	if (mIncassato)
	{
		throw std::invalid_argument("Impossibile emettere un assegno già incassato.");
	}

	mBeneficiario = beneficiario;
	mDataEmissione = dataEmissione;
	mDataScadenza = dataScadenza;
	mImporto = importo;
	mNote = note;

	mEmesso = true;
}

void Assegno::incassa(date dataIncasso)
{
	mIncassato = true;
	mDataIncasso = dataIncasso;
}

void Assegno::annulla()
{
	mAnnullato = true;
}

bool Assegno::annullato() const
{
	return mAnnullato;
}

bool Assegno::emesso() const
{
	return mEmesso;
}

bool Assegno::incassato() const
{
	return mIncassato;
}

void Assegno::setId(long id)
{
	mId = id;
}

void Assegno::setIdLibretto(long idLibretto)
{
	mIdLibretto = idLibretto;
}

void Assegno::annullaEmissione()
{
	mEmesso = false;

	mBeneficiario = _T("");
	mDataEmissione = dtm::date{ 1,1,1900 };
	mDataScadenza = dtm::date{ 1,1,1900 };
	mImporto = 0.00;
	mNote = _T("");
}

AssegnoBuilder::AssegnoBuilder(std::wstring numero)
{
	mp_assegno.reset(new Assegno{ numero });
}

void AssegnoBuilder::setId(long id)
{
	mp_assegno->setId(id);
}

void AssegnoBuilder::setIdLibretto(long idLibretto)
{
	mp_assegno->setIdLibretto(idLibretto);
}

Assegno AssegnoBuilder::getAssegno() const
{
	return *mp_assegno;
}
