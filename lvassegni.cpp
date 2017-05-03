#include "stdafx.h"
#include "lvassegni.h"
#include "utils.h"
#include "assegno.h"
#include "libdao.h"
#include "ccdao.h"
#include "azdao.h"

ListViewAssegni::ListViewAssegni()
{
}

ListViewAssegni::~ListViewAssegni()
{
}

void ListViewAssegni::Aggiungi(const Assegno & a)
{
	m_assegni.push_back(a);
}

void ListViewAssegni::Update() const
{
	for each (const auto& a in m_assegni)
	{
		int i = GetItemCount();
		std::wstring numero = a.getNumero();
		std::wstring intest = a.getBeneficiario();
		dtm::date dtEmess = a.getDataEmissione();
		std::wstring importo = utils::format(a.getImporto());
		std::wostringstream ss;
		ss << std::setw(2) << std::setfill(_T('0')) << dtEmess.day() << _T('/')
			<< std::setw(2) << std::setfill(_T('0')) << dtEmess.month() << _T('/')
			<< dtEmess.year();
		std::wstring dataEmissione = ss.str();

		dtm::date dtScadenza = a.getDataScadenza();
		std::wostringstream s2;
		s2 << std::setw(2) << std::setfill(_T('0')) << dtScadenza.day() << _T('/')
			<< std::setw(2) << std::setfill(_T('0')) << dtScadenza.month() << _T('/')
			<< dtScadenza.year();
		std::wstring dataScadenza = s2.str();

		LVITEM lv{ 0 };

		// data scadenza
		lv.iItem = i;
		lv.mask = LVFIF_TEXT;
		lv.pszText = (LPWSTR)(dataScadenza.c_str());
		InsertItem(lv);

		// numero
		lv.iSubItem = 1;
		lv.pszText = (LPWSTR)numero.c_str();
		SetItem(lv);

		// intestatario
		lv.iSubItem = 2;
		lv.pszText = (LPWSTR)intest.c_str();
		SetItem(lv);

		// importo
		lv.iSubItem = 3;
		lv.pszText = (LPWSTR)importo.c_str();
		SetItem(lv);

		// libretto
		LibrettoDao libdao;
		Libretto l = libdao.getById(a.getIdLibretto());
		std::wstring codiceLibretto{ l.getCodice() };
		lv.iSubItem = 4;
		lv.pszText = (LPWSTR)(codiceLibretto.c_str());
		SetItem(lv);

		// assegno
		AziendaDao azdao;
		ContoCorrenteDao ccdao;
		ContoCorrente cc = ccdao.GetById(l.getIdConto());
		Azienda az;
		azdao.getById(cc.getIdAzienda(), az);
		lv.iSubItem = 5;
		std::wstring azDenom{ az.getRagioneSociale() };
		lv.pszText = (LPWSTR)(azDenom.c_str());
		SetItem(lv);

		SetItemData(i, (DWORD_PTR)&a);
	}
}

void ListViewAssegni::ClearItems()
{
	m_assegni.clear();
}

void ListViewAssegni::SetItems(const std::vector<Assegno>& items)
{
	ClearItems();
	m_assegni.resize(items.size());
	std::copy(items.cbegin(), items.cend(), m_assegni.begin());
	Update();
}

void ListViewAssegni::OnInitialUpdate()
{
	InsertColumn(0, _T("Scadenza"), 0, 120);
	InsertColumn(1, _T("Numero"), 0, 160);
	InsertColumn(2, _T("Intestato a"), 0, 300);
	InsertColumn(3, _T("Importo"), 0, 100);
	InsertColumn(4, _T("Libretto"), 0, 120);
	InsertColumn(5, _T("Azienda"), 0, 120);
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}
