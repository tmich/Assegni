#include "stdafx.h"
#include "CShell32.h"

CShell32::CShell32()
	: hDll{ ::LoadLibrary(_T("SHELL32.dll")) }
{
	
}

CShell32::~CShell32()
{
	::FreeLibrary(hDll);
}

HICON CShell32::GetIcon(UINT resourceId)
{
	return ::LoadIcon(hDll, MAKEINTRESOURCE(resourceId));
}

HANDLE CShell32::GetImage(UINT resourceId, UINT cx, UINT cy)
{
	return ::LoadImage(hDll, MAKEINTRESOURCE(resourceId), IMAGE_ICON, cx, cy, NULL);
}
