#pragma once

class Fornitore
{
public:
	Fornitore();
	virtual ~Fornitore();
	Fornitore(std::wstring denominazione, std::wstring indirizzo, std::wstring telefono, std::wstring partita_iva);
	Fornitore(int id, std::wstring denominazione, std::wstring indirizzo, std::wstring telefono, std::wstring partita_iva);

	int getId() const { return m_id; }
	void setId(int id) { m_id = id; }
	std::wstring getDenominazione() const { return m_denominazione; }
	std::wstring getIndirizzo() const { return m_indirizzo; }
	std::wstring getTelefono() const { return m_telefono; }
	std::wstring getPartitaIva() const { return m_partitaIva; }
protected:
	std::wstring m_denominazione, m_indirizzo, m_telefono, m_partitaIva;
	int m_id;
};