#include "stdafx.h"
#include <vld.h>
#include "resource.h"
#include "azdao.h"
#include "dlgassegni.h"

class MyWinApp : public CWinApp
{
public:
	BOOL InitInstance()
	{
		DlgAssegni dlg;
		dlg.DoModal();
		
		PostQuitMessage(0);
		return true;
	}
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	MyWinApp MyApp;
	MyApp.InitInstance();

    return 0;
}


