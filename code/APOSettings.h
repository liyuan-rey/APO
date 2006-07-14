#pragma once

#include "SettingUtil.h"

#include "include\settingsstorage\msettingsstorage.h"
using Mortimer::CSettings;

static const DWORD APO_CONFIG_VER = 1;

class CAPOSettings : public CSettings
{
public:
	static const TCHAR defaultSectionName[];

	DWORD Version;

	int Action;
	int ForceMode;

	int PeriodMode;
	int InHours;
	int InMinutes;
	SYSTEMTIME AtTime;
	int UseDate;
	SYSTEMTIME AtDate;

	int AutoRun;
	int StartEnabled;
	int ShowMessage;
	int ShowMsgTime;
	int PlaySound;
	CString SoundFile;

	CString Language;
	CString Skin;

	BEGIN_SETTING_MAP(CAPOSettings)
		SETTING_ITEM_DEFAULT(Version, APO_CONFIG_VER)

		SETTING_ITEM_DEFAULT(Action, 0)
		SETTING_ITEM_DEFAULT(ForceMode, 1)

		SETTING_ITEM_DEFAULT(PeriodMode, 0)
		SETTING_ITEM_DEFAULT(InHours, 0)
		SETTING_ITEM_DEFAULT(InMinutes, 0)
		SETTING_ITEM_BINARY_DEFAULT(AtTime, sizeof(SYSTEMTIME), &DEFAULT_ZEROTIME)
		SETTING_ITEM_DEFAULT(UseDate, 0)
		SETTING_ITEM_BINARY_DEFAULT(AtDate, sizeof(SYSTEMTIME), &DEFAULT_ZEROTIME)

		SETTING_ITEM_DEFAULT(AutoRun, 0)
		SETTING_ITEM_DEFAULT(StartEnabled, 0)
		SETTING_ITEM_DEFAULT(ShowMessage, 1)
		SETTING_ITEM_DEFAULT(ShowMsgTime, 20)
		SETTING_ITEM_DEFAULT(PlaySound, 0)
		SETTING_ITEM_DEFAULT(SoundFile, DEFAULT_EMPTYSTR)

		SETTING_ITEM_DEFAULT(Language, _T("eng"))
		SETTING_ITEM_DEFAULT(Skin, _T("default"))
	END_SETTING_MAP()
};
