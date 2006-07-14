// APO.cpp : main source file for APO.exe
//

#include "stdafx.h"
#include "resource.h"

#include "Env.h"
#include "DlgMain.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	// add flags to support other controls
	AtlInitCommonControls(ICC_TREEVIEW_CLASSES /*| ICC_UPDOWN_CLASS
		| ICC_PROGRESS_CLASS */| ICC_DATE_CLASSES | ICC_USEREX_CLASSES);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	// BLOCK: Run application
	{
		CDlgMain dlgMain;
		dlgMain.DoModal();
	}

	_Module.Term();
	::CoUninitialize();

	return 0;
}
