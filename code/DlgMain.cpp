// DlgMain.cpp : implementation of the CDlgMain class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "DlgMain.h"
#include "DlgNotify.h"

class CAPOSettings;

/////////////////////////////////////////////////////////////////////////////
HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);

/////////////////////////////////////////////////////////////////////////////
CDlgMain::CDlgMain() : m_tidPerformSchedule(0), m_pdlgNotify(NULL), m_bShowNotify(TRUE)
{
	_env.Init();
}

CDlgMain::~CDlgMain()
{
	_env.Term();
}

LRESULT CDlgMain::OnInitDialog(HWND /*hwndFocus*/, LPARAM /*lParam*/)
{
	{//HACK: for 
		int nAction = _env.GetSettings().Action;
		_env.GetSettings().Action = CB_ERR;

		DoDataExchange(FALSE);
		
		_env.GetSettings().Action = nAction;
	}

	//////////////////////////////////////////////////////////////////////////
	// Init combo box
	CString strItems = _T("Shutdown;Restart;Log off;Lock;Hang up;Standby;Habernate;");
	CString resToken;
	int curPos = 0;
	const TCHAR* token = _T(";");

	COMBOBOXEXITEM cbxi;
	ZeroMemory(&cbxi, sizeof(COMBOBOXEXITEM));
	cbxi.mask = CBEIF_IMAGE | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE
		| CBEIF_TEXT;
	cbxi.iItem = 0;

	resToken = strItems.Tokenize(token, curPos);
	while (resToken != "")
	{
		cbxi.cchTextMax = resToken.GetLength();
		cbxi.pszText = resToken.GetBuffer();
		cbxi.iImage = cbxi.iItem;
		cbxi.iSelectedImage = cbxi.iItem;
		cbxi.iOverlay = cbxi.iItem;
		m_cbxAction.InsertItem(&cbxi);
		resToken.ReleaseBuffer();

		cbxi.iItem++;
		resToken= strItems.Tokenize(token, curPos);
	}

	// Init il of combo box
	CBitmap bmpAction = _env.GetSkin().GetWidgetBitmap(IDC_BTN_HIDE/*IDC_CBX_ACTION*/);
	if (bmpAction.m_hBitmap)
	{
		CImageList il;
		CSize size;

		bmpAction.GetSize(size);
		il.Create(size.cx / 7, size.cy, ILC_COLORDDB, 0, 7);
		il.Add(bmpAction, _env.GetSkin().GetColorKey());
		ATLASSERT( il.GetImageCount() == 7 );

		m_cbxAction.SetImageList(il); //TODO: review MSDN remarks section of CBEM_SETIMAGELIST
	}
	else
	{
		MessageBox(_T("Can't initialize icon of combo box, application will quit."), _T(""), MB_ICONWARNING);
		PostMessage(WM_QUIT);
		return TRUE;
	}

	m_cbxAction.SetCurSel(_env.GetSettings().Action);

	// Init control data
	m_spnInHour.SetRange(EDIT_NUMBER_MIN, EDIT_NUMBER_MAX);
	m_spnInMinute.SetRange(EDIT_NUMBER_MIN, EDIT_NUMBER_MAX);
	m_spnShowMsgTime.SetRange(EDIT_NUMBER_MIN, EDIT_NUMBER_MAX);

	m_edtInHour.SetLimitText(EDIT_LIMITTEXT_NUM);
	m_edtInMinute.SetLimitText(EDIT_LIMITTEXT_NUM);
	m_edtShowMsgTime.SetLimitText(EDIT_LIMITTEXT_NUM);

	//////////////////////////////////////////////////////////////////////////

	// set window region
	CBitmap bmpTmp = _env.GetSkin().GetWidgetBitmap(IDD);
	if (!bmpTmp.m_hBitmap)
	{
		MessageBox(_T("Can't initialize the skin of UI, application will quit."), _T(""), MB_ICONWARNING);
		PostMessage(WM_QUIT);
		return TRUE;
	}

	CRgn rgnWnd = CreateRgnFromBitmap(bmpTmp, _env.GetSkin().GetColorKey());
	if (!rgnWnd.m_hRgn)
	{
		MessageBox(_T("Can't initialize UI region, application will quit."), _T(""), MB_ICONWARNING);
		PostMessage(WM_QUIT);
		return TRUE;
	}

	SetWindowRgn(rgnWnd, FALSE);

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// set title
	CString strCaption;
	strCaption.LoadString(IDS_CAPTION);
	SetWindowText(strCaption);

	// set tray icon
	InstallIcon(strCaption, hIconSmall, IDR_POPUP);
	SetDefaultItem(ID_SHOWHIDE);

	// setup bitmap button
	SetupBmpButton();

	//
	AdjustCtrlState();

	//
	m_pdlgNotify = new CDlgNotify();
	m_pdlgNotify->m_pWndMsgTarget = this;

	//
	if (_env.GetSettings().StartEnabled)
		OnBnClickedStart(); // to start

	//////////////////////////////////////////////////////////////////////////

	// center the dialog on the screen
	CenterWindow();

	return TRUE; // set focus to default control
}

void CDlgMain::SetupBmpButton()
{
	typedef struct _BMP_BTN_INFO 
	{
		UINT nId;
		CBitmapButton* pBtn;
	} BMP_BTN_INFO;

	const UINT bbis_size = 4;
	BMP_BTN_INFO bbis[bbis_size] = {
		{IDC_BTN_HIDE, &m_btnHide},
		{IDC_BTN_HELP, &m_btnHelp},
		{IDC_BTN_ABOUT, &m_btnAbout},
		{IDC_BTN_LANGUAGE, &m_btnLanguage}
	};

	CString strTip;
	CPoint pt;
	CBitmap bmpBotton;
	CSize size;
	CImageList il;

	for (int i = 0; i < bbis_size; ++i)
	{
		BMP_BTN_INFO& bbi = bbis[i];

		ATLASSERT( ::IsWindow(bbi.pBtn->m_hWnd) );

		bbi.pBtn->SetBitmapButtonExtendedStyle(BMPBTN_HOVER | BMPBTN_AUTOSIZE);

		strTip.LoadString(bbi.nId);
		bbi.pBtn->SetToolTipText(strTip);

		bmpBotton = _env.GetSkin().GetWidgetBitmap(bbi.nId);
		if (bmpBotton.m_hBitmap)
		{
			il = NULL; // Detach ImageList handler

			bmpBotton.GetSize(size);
			size.cx /= 3; // fix imglist width to one icon width
			il.Create(size.cx, size.cy, ILC_COLORDDB, 0, 3);
			il.Add(bmpBotton);
			ATLASSERT( il.GetImageCount() == 3 );

			CImageList ilOld = bbi.pBtn->SetImageList(il);
			ilOld.Destroy();

			bbi.pBtn->SetImages(0, 1, 2, 0);
		}

		pt = _env.GetSkin().GetWidgetPosition(bbi.nId);
		bbi.pBtn->MoveWindow(CRect(pt, size), FALSE);
	}
}

LRESULT CDlgMain::OnDestroy(void)
{
	if (m_tidPerformSchedule)
		OnBnClickedStart(); // to stop

	DoDataExchange(TRUE);

	if (::IsWindow(m_pdlgNotify->m_hWnd))
		m_pdlgNotify->DestroyWindow();

	delete m_pdlgNotify;
	m_pdlgNotify = NULL;

	SetMsgHandled(FALSE); // To exit application
	return 0;
}

LRESULT CDlgMain::OnPaint(HDC /*hdc*/)
{
	CPaintDC dc(*this);

	CRect rcClient;
	GetClientRect(rcClient);

	_env.GetSkin().Draw(dc, rcClient.left, rcClient.top, rcClient.Width(),
		rcClient.Height(), IDD);

	return 0;
}

LRESULT CDlgMain::OnEraseBkgnd(HDC /*hdc*/)
{
	return TRUE;
}

LRESULT CDlgMain::OnNcHitTest(CPoint pt)
{
	CRect rcCaption(1, 1, 317, 21);

	ClientToScreen(rcCaption);
	if (rcCaption.PtInRect(pt))
		return HTCAPTION;

	SetMsgHandled(FALSE);
	return 0;
//	return DefWindowProc();
}

LRESULT CDlgMain::OnCtlColor(HDC /*hdc*/, HWND /*hwndChild*/)
{
	return (LRESULT)::GetStockObject(WHITE_BRUSH);
}

void CDlgMain::PrepareMenu(HMENU hMenu)
{
	hMenu;
}

#include <Htmlhelp.h>
#pragma comment( lib, "htmlhelp.lib" )

LRESULT CDlgMain::OnHelp(LPHELPINFO /*lpHelpInfo*/)
{
	extern CAppModule _Module;

	CWaitCursor wait;

	TCHAR szPath[MAX_PATH] = {_T('\0')};
	::GetModuleFileName(_Module.m_hInst, szPath, MAX_PATH);

	CPath pthHelp = szPath;
	pthHelp.RemoveExtension();
	pthHelp.AddExtension(_T(".chm::/intro.htm>mainwin"));

	::HtmlHelp(*this, pthHelp, HH_DISPLAY_TOPIC, NULL);

	return TRUE;
}

LRESULT CDlgMain::OnClose(void)
{
	//TODO minimize and hide window (to tray)
	EndDialog(IDCLOSE);

	return 0;
}

LRESULT CDlgMain::OnHide(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
	return OnClose();
}

LRESULT CDlgMain::OnAbout(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
// 	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
// 	dlg.DoModal();
	return 0;
}

LRESULT CDlgMain::OnLanguage(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
	//TODO show language selection menu
	return 0;
}

LRESULT CDlgMain::AdjustCtrlState(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
	DoDataExchange(TRUE);

	CAPOSettings& settings = _env.GetSettings();

	// Disable all controls
	m_cbxAction.EnableWindow(FALSE);
	m_chkForce.EnableWindow(FALSE);
	m_edtInHour.EnableWindow(FALSE);
	m_spnInHour.EnableWindow(FALSE);
	m_edtInMinute.EnableWindow(FALSE);
	m_spnInMinute.EnableWindow(FALSE);
	m_dtcAtTime.EnableWindow(FALSE);
	m_chkAtDate.EnableWindow(FALSE);
	m_dtcAtDate.EnableWindow(FALSE);
	m_edtShowMsgTime.EnableWindow(FALSE);
	m_spnShowMsgTime.EnableWindow(FALSE);
	m_chkPlaySound.EnableWindow(FALSE);
	m_btnSelFile.EnableWindow(FALSE);

	// if we have a schedule timer, that means we start schedule action
	m_cbxAction.EnableWindow(!m_tidPerformSchedule);
	m_chkForce.EnableWindow(!m_tidPerformSchedule);

	CWindow wnd = GetDlgItem(IDC_RDO_AT);
	wnd.EnableWindow(!m_tidPerformSchedule);

	wnd = GetDlgItem(IDC_RDO_IN);
	wnd.EnableWindow(!m_tidPerformSchedule);

	if (0 == settings.PeriodMode)
	{
		if (!m_tidPerformSchedule)
		{
			m_edtInHour.EnableWindow(TRUE);
			m_spnInHour.EnableWindow(TRUE);
			m_edtInMinute.EnableWindow(TRUE);
			m_spnInMinute.EnableWindow(TRUE);
		}
	}
	else
	{
		if (!m_tidPerformSchedule)
		{
			m_dtcAtTime.EnableWindow(TRUE);
			m_chkAtDate.EnableWindow(TRUE);
			m_dtcAtDate.EnableWindow(TRUE);
		}
	}

	//
	if (1 == settings.ShowMessage)
	{
		m_edtShowMsgTime.EnableWindow(TRUE);
		m_spnShowMsgTime.EnableWindow(TRUE);
		m_chkPlaySound.EnableWindow(TRUE);

		if (1 == settings.PlaySound)
			m_btnSelFile.EnableWindow(TRUE);
	}

	return 0;
}

LRESULT CDlgMain::OnBnClickedStart(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
	if (!m_tidPerformSchedule)
	{// start
		// check schedule config
		m_odtSchedule = GetScheduleTime();
		COleDateTimeSpan tspan = m_odtSchedule - COleDateTime::GetCurrentTime();
		double time_left = tspan.GetTotalSeconds();

		if (time_left < 0)
		{
			MessageBox(_T("Schedule time is early than today, please correct it."), _T("Information"), MB_ICONINFORMATION);
			return 0;
		}

		m_tidPerformSchedule = SetTimer(TIMERID_PERFORMSCHEDULE,
			TIMERID_PERFORMSCHEDULE_ELAPSE, NULL);
		m_btnStart.SetWindowText(_T("Stop"));

		m_bShowNotify = TRUE;
	}
	else
	{// stop
		KillTimer(m_tidPerformSchedule);
		m_tidPerformSchedule = 0;
		m_btnStart.SetWindowText(_T("Start"));

		if (::IsWindow(m_pdlgNotify->m_hWnd))
		{
			m_pdlgNotify->DestroyWindow();
			m_pdlgNotify->m_hWnd = NULL;
		}
	}

	AdjustCtrlState();
	return 0;
}

LRESULT CDlgMain::OnBnClickedNow(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
	MessageBox(_T("OnBnClickedNow"));

	DoDataExchange(TRUE);

	CAPOSettings settings = _env.GetSettings();

	switch(settings.Action)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		break;
	default:
		ATLASSERT( FALSE && _T("Unexcepted value at OnBnClickedNow.") );
	}

	return 0;
}

LRESULT CDlgMain::OnBnClickedSelFile(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
	CString strFile;
	m_edtSoundFile.GetWindowText(strFile);

	CFileDialog dlgSelFile(TRUE, NULL, (strFile.IsEmpty() ? NULL : (LPCTSTR)strFile),
		OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON,
		_T("Wave Files (*.wav)\0*.wav\0\0"), *this);

	if (IDOK == dlgSelFile.DoModal())
		m_edtSoundFile.SetWindowText(dlgSelFile.m_szFileName);

	return 0;
}

LRESULT CDlgMain::OnTimer(UINT nId, TIMERPROC /*lpTimerProc*/)
{
	if (TIMERID_PERFORMSCHEDULE == nId)
	{
		DoDataExchange(TRUE);

		CAPOSettings settings = _env.GetSettings();

		// get left time
		COleDateTimeSpan tspan = m_odtSchedule - COleDateTime::GetCurrentTime();
		double time_left = tspan.GetTotalSeconds();
		ATLTRACE("%.0f seconds left.\n", time_left);

		// check if we need show notify msg
		if (time_left <= settings.ShowMsgTime && m_bShowNotify &&
			!::IsWindow(m_pdlgNotify->m_hWnd))
		{
			m_pdlgNotify->Create(*this, NULL);
			ATLASSERT( ::IsWindow(m_pdlgNotify->m_hWnd) );

			m_pdlgNotify->CenterWindow();
			m_pdlgNotify->ShowWindow(SW_NORMAL);

			if (settings.PlaySound)
			{
				CPath sndPath(settings.SoundFile);
				if (sndPath.FileExists())
					::PlaySound(sndPath, NULL, SND_FILENAME | SND_ASYNC | SND_NOWAIT | SND_NODEFAULT);
			}

			m_bShowNotify = FALSE;
		}

		if (::IsWindow(m_pdlgNotify->m_hWnd) && m_pdlgNotify->IsWindowVisible())
		{
			TCHAR szItemText[MAX_PATH] = _T("");

			COMBOBOXEXITEM cbxi;
			ZeroMemory(&cbxi, sizeof(COMBOBOXEXITEM));
			cbxi.mask = CBEIF_TEXT;
			cbxi.pszText = szItemText;
			cbxi.cchTextMax = MAX_PATH;

			m_cbxAction.GetItem(&cbxi);

			m_pdlgNotify->SendMessage(WM_APP_UPDATETIMELEFT,
				(WPARAM)szItemText, (LPARAM)(int)time_left);
		}

		// if time's up, stop timer and perform the action
		if (time_left <= 0)
		{
			OnBnClickedStart(); // to stop
			OnBnClickedNow();
		}
	}

	SetMsgHandled(FALSE);
	return 0;
//	CDialog::OnTimer(nIDEvent);
}

LRESULT CDlgMain::OnNotifyChoosed(UINT /*nMsg*/, WPARAM wp, LPARAM lp)
{
	int nChoice = wp;
	int nDelayTime = lp;

#ifdef _DEBUG
	CString strMsg;
	strMsg.Format("OnNotifyChoosed[c=%d][t=%d]", nChoice, nDelayTime);
	MessageBox(strMsg);
#endif

	switch(nChoice)
	{
	case 0: // continue
		break;
	case 1: // delay
		OnBnClickedStart(); // to stop
		CheckRadioButton(IDC_RDO_IN, IDC_RDO_AT, IDC_RDO_IN);
		SetDlgItemInt(IDC_EDT_INHOUR, 0, 0);
		SetDlgItemInt(IDC_EDT_INMINUTE, nDelayTime, 0);
		OnBnClickedStart(); // to restart
		break;
	case 2: // cancel
		OnBnClickedStart(); // to stop
		break;
	default:
		ATLASSERT( FALSE && _T("Unexcepted value!") );
	}

	return 0;
}

COleDateTime CDlgMain::GetScheduleTime()
{
	DoDataExchange(TRUE);

	CAPOSettings& settings = _env.GetSettings();
	COleDateTime odtSystem = COleDateTime::GetCurrentTime();
	COleDateTime odtSchedule;
	ATLTRACE("system time: %s\n", odtSystem.Format());

	if (0 == settings.PeriodMode)
	{// count down time
		odtSchedule = odtSystem;
		odtSchedule += COleDateTimeSpan(0, settings.InHours, settings.InMinutes, 0);
	}
	else
	{// specified time
		odtSchedule.SetDateTime(odtSystem.GetYear(), odtSystem.GetMonth(),
			odtSystem.GetDay(), settings.AtTime.wHour, settings.AtTime.wMinute,
			settings.AtTime.wSecond);
		if (settings.UseDate)
		{
			odtSchedule.SetDateTime(settings.AtTime.wYear, settings.AtTime.wMonth,
				settings.AtTime.wDay, odtSchedule.GetHour(), odtSchedule.GetMinute(),
				odtSchedule.GetSecond());
		}
	}

	ATLTRACE("schedule time: %s\n", odtSchedule.Format());
	return odtSchedule;
}
