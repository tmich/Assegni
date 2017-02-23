#pragma once

class CShell32 
{
public:
	CShell32();
	~CShell32();

	HICON GetIcon(UINT resourceId);
	HANDLE GetImage(UINT resourceId, UINT cx, UINT cy);
private:
	HINSTANCE hDll;
};