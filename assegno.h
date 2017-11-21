#pragma once
#include <memory>
#include "datetime.h"

using namespace dtm;

class Assegno
{
public:
	friend class AssegnoDao;
	friend class AssegnoBuilder;

	Assegno() {}
	Assegno(std::wstring numero);
	Assegno(std::wstring numero, std::wstring beneficiario, date dataEmissione, double importo, std::wstring note);
	Assegno(std::wstring numero, std::wstring beneficiario, date dataEmissione, double importo, std::wstring note, date dataIncasso);
	virtual ~Assegno();

	void emetti(std::wstring beneficiario, date dataEmissione, date dataScadenza, double importo, std::wstring note);
	void incassa(date dataIncasso);
	void annulla();

	bool annullato() const;
	bool emesso() const;
	bool incassato() const;
	date getDataEmissione() const { return mDataEmissione; }
	//void setBeneficiario(std::string beneficiario) { mBeneficiario = beneficiario; }
	std::wstring getBeneficiario() const { return mBeneficiario; }
	//void setImporto(float importo) { mImporto = importo; }
	double getImporto() const { return mImporto; }
	//void setDataIncasso(date dataIncasso) { mDataIncasso = dataIncasso; }
	date getDataIncasso() const { return mDataIncasso; }
	//void setDataScadenza(const date& dataScadenza);
	date getDataScadenza() const { return mDataScadenza; }
	void setNote(std::wstring note) { mNote = note; }
	std::wstring getNote() const { return mNote; }
	long getId() const { return mId; }
	std::wstring getNumero() const { return mNumero; }
	long getIdLibretto() const { return mIdLibretto; }
protected:
	void setId(long id);
	void setIdLibretto(long idLibretto);
	void annullaEmissione();
	long mId, mIdLibretto;
	std::wstring mNumero;
	date mDataEmissione;
	date mDataScadenza;
	std::wstring mBeneficiario;
	double mImporto;
	date mDataIncasso;
	std::wstring mNote;
	bool mEmesso, mIncassato, mAnnullato;
};

class AssegnoBuilder
{
public:
	AssegnoBuilder(std::wstring numero);
	virtual ~AssegnoBuilder() {}

	void setId(long id);
	void setIdLibretto(long idLibretto);

	Assegno getAssegno() const;
protected:
	std::unique_ptr<Assegno> mp_assegno;
};