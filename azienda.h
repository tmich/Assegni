#pragma once
class AziendaDao;

class Azienda
{
public:
	friend class AziendaDao;

	Azienda(){};
	Azienda(const std::wstring& ragioneSociale, const std::wstring& indirizzo);
	Azienda(int id, const std::wstring& ragioneSociale, const std::wstring& indirizzo);
	~Azienda();

	int getId() const { return mId; }
	std::wstring getRagioneSociale() const { return mRagioneSociale; }
	std::wstring getIndirizzo() const { return mIndirizzo; }
protected:
	void setId(int id) { mId = id; }
private:
	int mId;
	std::wstring mRagioneSociale, mIndirizzo;
};

