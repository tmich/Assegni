#include "stdafx.h"
#include "resource.h"
#include "CDBRecord.h"

CDBRecord::CDBRecord()
	: m_current{ 0 }, m_max{ 0 }
{
	InitControl();
}

CDBRecord::~CDBRecord()
{
}

void CDBRecord::SetCurrent(size_t current)
{
	m_current = current;
	Update();
}

void CDBRecord::SetParent(HWND parent)
{
	m_parent = parent;
}

void CDBRecord::SetMax(size_t max)
{
	if (m_current == 0)
		m_current = 1;

	m_max = max;
}

void CDBRecord::Update()
{
	UpdateText();
	UpdateButtons();
}

BOOL CDBRecord::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case 10001:
		OnPrev();
		GetParent().SendMessageW(WM_COMMAND, DBRECORD, -1);
		break;
	case 10002:
		OnNext();
		GetParent().SendMessageW(WM_COMMAND, DBRECORD, 1);
		break;
	default:
		break;
	}
	return 0;
}

int CDBRecord::OnCreate(CREATESTRUCT & cs)
{
	//return ::CreateDialog(GetApp().GetInstanceHandle(), MAKEINTRESOURCE(m_id), m_parent, 
	cs.cx = 200;
	cs.cy = 200;
	return CWnd::OnCreate(cs);
}

LRESULT CDBRecord::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return CWnd::OnPaint(uMsg, wParam, lParam);
}

void CDBRecord::OnInitialUpdate()
{
	m_txtVal.Create(*this);
	btnPrev.Create(*this);
	btnNext.Create(*this);
	btnPrev.SetWindowLongW(GWL_ID, 10001);
	btnNext.SetWindowLongW(GWL_ID, 10002);
	btnPrev.SetWindowTextW(_T("<"));
	btnNext.SetWindowTextW(_T(">"));
	btnPrev.SetWindowPos(NULL, 10, 10, 50, 25, 0);
	btnNext.SetWindowPos(NULL, 150, 10, 50, 25, 0);
	m_txtVal.SetWindowPos(NULL, 60, 10, 90, 25, 0);
	
	Update();
}

void CDBRecord::UpdateText()
{
	CString text;
	text.AppendFormat(_T("%d di %d"), m_current, m_max);
	m_txtVal.SetWindowTextW(text);
}

void CDBRecord::UpdateButtons()
{
	btnNext.EnableWindow(m_current < m_max);
	btnPrev.EnableWindow(m_current > 1);
}

void CDBRecord::InitControl()
{
	// The entry for the dialog's control in resource.rc must match this name.
	CString ClassName = _T("DbRecord");

	// Register the window class
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));

	if (!::GetClassInfo(GetApp().GetInstanceHandle(), ClassName, &wc))
	{
		wc.lpszClassName = ClassName;
		wc.lpfnWndProc = ::DefWindowProc;
		wc.hInstance = GetApp().GetInstanceHandle();
		::RegisterClass(&wc);
	}

	assert(::GetClassInfo(GetApp().GetInstanceHandle(), ClassName, &wc));
}

void CDBRecord::OnPrev()
{
	if (m_current > 1)
		m_current--;
		
	Update();
}

void CDBRecord::OnNext()
{
	if (m_current < m_max)
		m_current++;

	Update();
}
