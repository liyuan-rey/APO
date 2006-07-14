#include "StdAfx.h"
#include "Env.h"

#include "include\SettingsStorage\MSettingsStorageIniFile.h"
using Mortimer::CSettingsStorageIniFile;

const TCHAR CEnv::defaultSettingsFolder[] = _T("");
const TCHAR CEnv::defaultSettingsFile[] = _T("config.ini");
const TCHAR CEnv::defaultLanguageFolder[] = _T("language");
const TCHAR CEnv::defaultLanguageFile[] = _T("language");

/////////////////////////////////////////////////////////////////////////////
void CEnv::Init()
{
	// Get app root information for later use
	TCHAR szPath[MAX_PATH] = {_T('\0')};
	::GetModuleFileName(_Module.m_hInst, szPath, MAX_PATH);

	m_pthAppRoot = szPath;
	m_pthAppRoot.RemoveFileSpec();
	m_pthAppRoot.AddBackslash();

	//
	LoadSettings();
	LoadLanguage(m_settings.Language);
	m_skin.Load(_T("res://APO.exe/"));
}

void CEnv::Term()
{
	//TODO add code for save language setting
#if 0
	SaveLanguage(m_settings.Language);
#endif
	SaveSettings();
}

/////////////////////////////////////////////////////////////////////////////

int CEnv::LoadSettings()
{
	CPath pthAppSettings = GetSettingsPath();
	if (pthAppSettings.FileExists())
	{
		CSettingsStorageIniFile stg;
		stg.SetIniFileName(pthAppSettings, CAPOSettings::defaultSectionName);

		return m_settings.Load(stg) ? 0 : 1;
	}

	return -1;
}

BOOL CEnv::SaveSettings()
{
	CPath pthAppSettings = GetSettingsPath();

	CSettingsStorageIniFile stg;
	stg.SetIniFileName(pthAppSettings, CAPOSettings::defaultSectionName);

	return m_settings.Save(stg);
}

CPath CEnv::GetSettingsPath(LPCTSTR szSettingsFile/* = defaultSettingsFile*/)
{
	CPath pthAppSettings = m_pthAppRoot + defaultSettingsFolder;
	CreateDirectory(pthAppSettings, NULL); //TODO: Ensure folder has been created.

	pthAppSettings.AddBackslash();
	pthAppSettings += szSettingsFile;

	return pthAppSettings;
}

int CEnv::LoadLanguage(LPCTSTR idLanguage)
{
	CPath pthLanguage = GetLanguagePath(idLanguage);
	if (pthLanguage.FileExists())
	{
		CSettingsStorageIniFile stg;
		stg.SetIniFileName(pthLanguage, CAPOLanguage::defaultSectionName);

		bool bRet = m_language.Load(stg);
		if (bRet)
			m_idLanguage = idLanguage;

		return bRet ? 0 : 1;
	}

	return -1;
}

BOOL CEnv::SaveLanguage(LPCTSTR idLanguage)
{
	CPath pthLanguage = GetLanguagePath(idLanguage);

	CSettingsStorageIniFile stg;
	stg.SetIniFileName(pthLanguage, CAPOLanguage::defaultSectionName);

	return m_language.Save(stg);
}

CPath CEnv::GetLanguagePath(LPCTSTR idLanguage)
{
	CPath pthLanguage = m_pthAppRoot + defaultLanguageFolder;
	CreateDirectory(pthLanguage, NULL); //TODO: Ensure folder has been created.

	pthLanguage.AddBackslash();
	pthLanguage += defaultLanguageFile;
	CString strTmp = _T(".");
	pthLanguage.AddExtension(strTmp + idLanguage);

	return pthLanguage;
}

