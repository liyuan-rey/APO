#pragma once

#include "APOSettings.h"
#include "APOLanguage.h"
#include "SimpleSkin.h"

class CEnv
{
	//////////////////////////////////////////////////////////////////////////
public:
	void Init();
	void Term();

	//////////////////////////////////////////////////////////////////////////
public:
	static const TCHAR defaultSettingsFolder[];
	static const TCHAR defaultSettingsFile[];
	static const TCHAR defaultLanguageFolder[];
	static const TCHAR defaultLanguageFile[];

	CPath m_pthAppRoot;

	inline CAPOSettings& GetSettings(void) {
		return m_settings;
	}

	inline CAPOLanguage& GetLanguage(void) {
		return m_language;
	}

	inline CSimpleSkin& GetSkin(void) {
		return m_skin;
	}

protected:
	int LoadSettings();
	BOOL SaveSettings();
	CPath GetSettingsPath(LPCTSTR szSettingsFile = defaultSettingsFile);
	int LoadLanguage(LPCTSTR idLanguage);
	BOOL SaveLanguage(LPCTSTR idLanguage);
	CPath GetLanguagePath(LPCTSTR idLanguage);

	CAPOSettings m_settings;
	CAPOLanguage m_language;
	CString m_idLanguage;
	CSimpleSkin m_skin;
};
