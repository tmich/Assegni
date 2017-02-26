#pragma once
class GeneratoreNumeriAssegni
{
public:
	GeneratoreNumeriAssegni(int qta);
	virtual ~GeneratoreNumeriAssegni();
	
	std::vector<std::wstring> Genera(std::wstring primoNumero);
protected:
	int m_qta;
};

class Libretto
{
public:
	friend class LibrettoDao;

	Libretto();
	~Libretto();
	std::wstring getNumero() const;
	std::wstring getCodice() const;
	int getQta() const;
	long getId() const;
	long getIdConto() const;

	std::wstring toString() const;
private:
	void setId(long id);
	void setIdConto(long idConto);
	void setNumero(std::wstring numero);
	void setCodice(std::wstring codice);
	void setQta(int qta);

	long mId, mIdConto;
	std::wstring mCodice, mNumero;
	int mQta;
};