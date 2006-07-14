#pragma once

#include "include\settingsstorage\msettingsstorage.h"
using Mortimer::CSettings;

static const DWORD APO_LANGUAGE_VER = 1;

class CAPOLanguage : public CSettings
{
public:
	static const TCHAR defaultSectionName[];

	DWORD Version;
	CString Name;
	CString Author;
	CString Remark;

	CString ActionText;
	CString NotifyMessage;

	BEGIN_SETTING_MAP(CAPOLanguage)
		SETTING_ITEM_DEFAULT(Version, APO_LANGUAGE_VER)
		SETTING_ITEM_DEFAULT(Name, _T("English"))
		SETTING_ITEM_DEFAULT(Author, _T("BitSoul Studio"))
		SETTING_ITEM_DEFAULT(Remark, _T("This is default language port by APO."))

		SETTING_ITEM_DEFAULT(ActionText, _T("Shutdown;Restart;Log off;Lock;Hang up;Standby;Habernate;"))

		SETTING_ITEM_DEFAULT(NotifyMessage, _T("System will %s in %d seconds, what would you like to do?"))
	END_SETTING_MAP()
};
