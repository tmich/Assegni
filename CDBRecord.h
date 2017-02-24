#pragma once

const UINT DBRECORD = WM_USER + 1;

class CDBRecord : public CWnd
{
public:
	CDBRecord();
	~CDBRecord();

	void SetCurrent(size_t current);
	size_t GetCurrent() const { return m_current; }
	void SetParent(HWND parent);
	void SetMax(size_t max);
	void Update();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) override;
	virtual int OnCreate(CREATESTRUCT& cs) override;
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnInitialUpdate() override;

	void UpdateText();
	void UpdateButtons();
	void InitControl();
	void OnPrev();
	void OnNext();

	CButton btnPrev, btnNext;
	CEdit m_txtVal;
	size_t m_max, m_current;
	UINT m_id;
	HWND m_parent;
};