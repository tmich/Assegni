#pragma once
class ContoCorrente
{
public:
	friend class ContoCorrenteDao;

	ContoCorrente(const std::wstring& numero, const std::wstring& banca, const std::wstring& sede, const std::wstring& agenzia);
	~ContoCorrente();

	std::wstring getNumero() const { return mNumero; }
	void setNumero(std::wstring numero) { mNumero = numero; };
	std::wstring getBanca() const { return mBanca; }
	void setBanca(std::wstring banca) { mBanca = banca; }
	std::wstring getSede() const { return mSede; }
	void setSede(std::wstring sede) { mSede = sede; }
	std::wstring getAgenzia() const { return mAgenzia; }
	void setAgenzia(std::wstring agenzia) { mAgenzia = agenzia; }
	std::wstring getNote() const { return mNote; }
	void setNote(std::wstring note) { mNote = note; }
	long getId() const { return mId; }
	std::wstring toString() const;
	long getIdAzienda() const { return mIdAzienda; }
private:
	ContoCorrente() {}
	void setId(long id);
	void setIdAzienda(long idConto);
	long mId, mIdAzienda;
	std::wstring mNumero, mBanca, mSede, mAgenzia, mNote;
};