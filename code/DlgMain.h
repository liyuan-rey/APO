// DlgMain.h : interface of the CDlgMain class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlcomtime.h>

#include "include/TrayIcon/trayiconimpl.h"
#include "include/WTL_patch/atlddx_patch.h"
#include "include/WTL_patch/ComboBoxEx2.h"

#include "Env.h"

class CDlgNotify;

// CDlgMain

class CDlgMain : public CDialogImpl<CDlgMain>,
				 public CWinDataExchange_Patch<CDlgMain>,
				 public CTrayIconImpl<CDlgMain>
{
public:
	CDlgMain();
	virtual ~CDlgMain();

	enum { IDD = IDD_MAIN };

	BEGIN_MSG_MAP_EX(CDlgMain) //TODO change macro to BEGIN_MSG_MAP_EX
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_CTLCOLORSTATIC(OnCtlColor)
		MSG_WM_CTLCOLOREDIT(OnCtlColor)
		MSG_WM_HELP(OnHelp)
		MSG_WM_CLOSE(OnClose)
		COMMAND_ID_HANDLER_EX(IDC_BTN_HIDE, OnHide)
		COMMAND_ID_HANDLER_EX(IDC_BTN_ABOUT, OnAbout)
		COMMAND_ID_HANDLER_EX(IDC_BTN_LANGUAGE, OnLanguage)
		COMMAND_ID_HANDLER_EX(IDC_RDO_IN, AdjustCtrlState)
		COMMAND_ID_HANDLER_EX(IDC_RDO_AT, AdjustCtrlState)
		COMMAND_ID_HANDLER_EX(IDC_CHK_SHOWMSG, AdjustCtrlState)
		COMMAND_ID_HANDLER_EX(IDC_CHK_PLAYSOUND, AdjustCtrlState)
		COMMAND_ID_HANDLER_EX(IDC_BTN_START, OnBnClickedStart)
		COMMAND_ID_HANDLER_EX(IDC_BTN_NOW, OnBnClickedNow)
		COMMAND_ID_HANDLER_EX(IDC_BTN_SELFILE, OnBnClickedSelFile)
		MSG_WM_TIMER(OnTimer)
		MESSAGE_HANDLER_EX(WM_APP_NOTIFYCHOOSED, OnNotifyChoosed)

		CHAIN_MSG_MAP(CTrayIconImpl<CDlgMain>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgMain)
		DDX_CONTROL(IDC_BTN_HIDE, m_btnHide)
		DDX_CONTROL(IDC_BTN_HELP, m_btnHelp)
		DDX_CONTROL(IDC_BTN_ABOUT, m_btnAbout)
		DDX_CONTROL(IDC_BTN_LANGUAGE, m_btnLanguage)

		DDX_CONTROL(IDC_CBX_ACTION, m_cbxAction);
		DDX_CONTROL_HANDLE(IDC_CHK_FORCE, m_chkForce);
		DDX_CONTROL_HANDLE(IDC_EDT_INHOUR, m_edtInHour);
		DDX_CONTROL_HANDLE(IDC_SPN_INHOUR, m_spnInHour);
		DDX_CONTROL_HANDLE(IDC_EDT_INMINUTE, m_edtInMinute);
		DDX_CONTROL_HANDLE(IDC_SPN_INMINUTE, m_spnInMinute);
		DDX_CONTROL_HANDLE(IDC_DTP_ATTIME, m_dtcAtTime);
		DDX_CONTROL_HANDLE(IDC_CHK_USEDATE, m_chkAtDate);
		DDX_CONTROL_HANDLE(IDC_DTP_ATDATE, m_dtcAtDate);
		DDX_CONTROL_HANDLE(IDC_EDT_SHOWMSGTIME, m_edtShowMsgTime);
		DDX_CONTROL_HANDLE(IDC_SPN_SHOWMSGTIME, m_spnShowMsgTime);
		DDX_CONTROL_HANDLE(IDC_CHK_PLAYSOUND, m_chkPlaySound);
		DDX_CONTROL_HANDLE(IDC_EDT_SOUNDFILE, m_edtSoundFile);
		DDX_CONTROL_HANDLE(IDC_BTN_SELFILE, m_btnSelFile);
		DDX_CONTROL_HANDLE(IDC_LBL_MESSAGE, m_lblMessage);
		DDX_CONTROL_HANDLE(IDC_BTN_START, m_btnStart);

		CAPOSettings& settings = _env.GetSettings();
		DDX_COMBO_INDEX(IDC_CBX_ACTION, settings.Action);
		DDX_CHECK(IDC_CHK_FORCE, settings.ForceMode);
		DDX_RADIO(IDC_RDO_IN, settings.PeriodMode);
		DDX_INT(IDC_EDT_INHOUR, settings.InHours);
		DDX_INT(IDC_EDT_INMINUTE, settings.InMinutes);
		DDX_CHECK(IDC_CHK_USEDATE, settings.UseDate);
		DDX_CHECK(IDC_CHK_AUTORUN, settings.AutoRun);
		DDX_CHECK(IDC_CHK_STARTENABLED, settings.StartEnabled);
		DDX_CHECK(IDC_CHK_SHOWMSG, settings.ShowMessage);
		DDX_INT(IDC_EDT_SHOWMSGTIME, settings.ShowMsgTime);
		DDX_CHECK(IDC_CHK_PLAYSOUND, settings.PlaySound);
		DDX_TEXT(IDC_EDT_SOUNDFILE, settings.SoundFile);
		// Do not use COleDateTime for DDX_Text because it is localization relatively
		if (bSaveAndValidate)
		{
			//TODO: enchance this by add DTS_SHOWNONE style of CDateTimePickerCtrl
			m_dtcAtTime.GetSystemTime(&settings.AtTime);
			m_dtcAtDate.GetSystemTime(&settings.AtDate);
		}
		else
		{
			m_dtcAtTime.SetSystemTime(GDT_VALID, &settings.AtTime);
			m_dtcAtDate.SetSystemTime(GDT_VALID, &settings.AtDate);
		}
	END_DDX_MAP()

	virtual void PrepareMenu(HMENU hMenu);

protected:
	LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnDestroy(void);
	LRESULT OnPaint(HDC hdc);
	LRESULT OnEraseBkgnd(HDC hdc);
	LRESULT OnNcHitTest(CPoint pt);
	LRESULT OnCtlColor(HDC hdc, HWND hwndChild);
	LRESULT OnHelp(LPHELPINFO lpHelpInfo);
	LRESULT OnClose(void);
	LRESULT OnHide(UINT nNotifyCode, int nId, HWND hWndCtl);
	LRESULT OnAbout(UINT nNotifyCode, int nId, HWND hWndCtl);
	LRESULT OnLanguage(UINT nNotifyCode, int nId, HWND hWndCtl);

	LRESULT AdjustCtrlState(UINT nNotifyCode = 0, int nId = 0, HWND hWndCtl = NULL);
	LRESULT OnBnClickedStart(UINT nNotifyCode = 0, int nId = 0, HWND hWndCtl = NULL);
	LRESULT OnBnClickedNow(UINT nNotifyCode = 0, int nId = 0, HWND hWndCtl = NULL);
	LRESULT OnBnClickedSelFile(UINT nNotifyCode, int nId, HWND hWndCtl);
	LRESULT OnTimer(UINT nId, TIMERPROC lpTimerProc);
	LRESULT OnNotifyChoosed(UINT nMsg, WPARAM wp, LPARAM lp);

private:
	void SetupBmpButton();
	COleDateTime GetScheduleTime();

	//////////////////////////////////////////////////////////////////////////
public:
	CEnv _env;

protected:
	CBitmapButton m_btnHide;
	CBitmapButton m_btnHelp;
	CBitmapButton m_btnAbout;
	CBitmapButton m_btnLanguage;

	CComboBoxEx2 m_cbxAction;
	CButton m_chkForce;
	CEdit m_edtInHour;
	CUpDownCtrl m_spnInHour;
	CEdit m_edtInMinute;
	CUpDownCtrl m_spnInMinute;
	CDateTimePickerCtrl m_dtcAtTime;
	CButton m_chkAtDate;
	CDateTimePickerCtrl m_dtcAtDate;
	CEdit m_edtShowMsgTime;
	CUpDownCtrl m_spnShowMsgTime;
	CButton m_chkPlaySound;
	CEdit m_edtSoundFile;
	CButton m_btnSelFile;
	CStatic m_lblMessage;
	CButton m_btnStart;

private:
	CImageList m_ilAction;
	UINT_PTR m_tidPerformSchedule; // timer id and flag for if timer is set, 0 for no timer
	CDlgNotify* m_pdlgNotify;
	BOOL m_bShowNotify;
	COleDateTime m_odtSchedule;
};
