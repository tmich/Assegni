#include "stdafx.h"
#include "fornitore.h"

Fornitore::Fornitore()
{

}

Fornitore::~Fornitore()
{

}

Fornitore::Fornitore(std::wstring denominazione, std::wstring indirizzo, std::wstring telefono, std::wstring partita_iva)
	: m_denominazione{ denominazione }, m_indirizzo{ indirizzo }, m_telefono{ telefono }, m_partitaIva{ partita_iva }
{

}

Fornitore::Fornitore(int id, std::wstring denominazione, std::wstring indirizzo, std::wstring telefono, std::wstring partita_iva)
	: Fornitore(denominazione, indirizzo, telefono, partita_iva)
{
	m_id = id;
}