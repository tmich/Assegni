#pragma once
class AziendaDao;

class Azienda
{
public:
	friend class AziendaDao;

	Azienda();
	Azienda(const std::wstring& ragioneSociale, const std::wstring& indirizzo);
	Azienda(int id, const std::wstring& ragioneSociale, const std::wstring& indirizzo);
	~Azienda();

	int getId() const { return mId; }
	std::wstring getRagioneSociale() const { return mRagioneSociale; }
	std::wstring getIndirizzo() const { return mIndirizzo; }
	std::wstring getPartitaIva() const { return mPiva; }
	
	void setPartitaIva(const std::wstring);
	void setRagioneSociale(const std::wstring);
	void setIndirizzo(const std::wstring);
protected:
	void setId(int id) { mId = id; }
private:
	int mId;
	std::wstring mRagioneSociale, mIndirizzo, mPiva;
};

