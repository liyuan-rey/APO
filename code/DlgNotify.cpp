// DlgNotify.cpp : Implementation of CDlgNotify
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "DlgNotify.h"
#include "DlgMain.h"

/////////////////////////////////////////////////////////////////////////////
HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);

/////////////////////////////////////////////////////////////////////////////
CDlgNotify::CDlgNotify() : m_pWndMsgTarget(NULL)
{
}

CDlgNotify::~CDlgNotify()
{
}

LRESULT CDlgNotify::OnInitDialog(HWND /*hwndFocus*/, LPARAM /*lParam*/)
{
	if (!::IsWindow(m_pWndMsgTarget->m_hWnd))
	{
		ATLASSERT( FALSE && _T("Err: need set m_pWndMsgTarget before creating dialog.") );
		EndDialog(0);
	}

	// Reset data at every time we re-create the dialog
	m_nChoice = 0;
	m_nDelayTime = 1;

	DoDataExchange(FALSE);

	//
	m_spnDelayTime.SetRange(EDIT_NUMBER_MIN, EDIT_NUMBER_MAX);
	m_edtDelayTime.SetLimitText(EDIT_LIMITTEXT_NUM);

	//
	CBitmap bmpTmp = m_pWndMsgTarget->_env.GetSkin().GetWidgetBitmap(IDD);
	if (!bmpTmp.m_hBitmap)
	{
		MessageBox(_T("Can't initialize the skin of UI, application will quit."), _T(""), MB_ICONWARNING);
		PostMessage(WM_QUIT);
		return TRUE;
	}

	CRgn rgnWnd = CreateRgnFromBitmap(bmpTmp, m_pWndMsgTarget->_env.GetSkin().GetColorKey());
	if (!rgnWnd.m_hRgn)
	{
		MessageBox(_T("Can't initialize UI region, application will quit."), _T(""), MB_ICONWARNING);
		PostMessage(WM_QUIT);
		return TRUE;
	}

	SetWindowRgn(rgnWnd, FALSE);

	// adjust control position
	CSize szOffset(12, 26);

	CWindow wnd = GetWindow(GW_CHILD);
	CRect rcChild;
	while (wnd.m_hWnd)
	{
		wnd.GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(szOffset);
		wnd.MoveWindow(rcChild, FALSE);

		wnd = wnd.GetWindow(GW_HWNDNEXT);
	}

	return TRUE;
}

LRESULT CDlgNotify::OnPaint(HDC /*hdc*/)
{
	CPaintDC dc(*this);

	CRect rcClient;
	GetClientRect(rcClient);

	m_pWndMsgTarget->_env.GetSkin().Draw(dc, rcClient.left, rcClient.top,
		rcClient.Width(), rcClient.Height(), IDD);

	return 0;
}

LRESULT CDlgNotify::OnEraseBkgnd(HDC /*hdc*/)
{
	return TRUE;
}

LRESULT CDlgNotify::OnCtlColor(HDC /*hdc*/, HWND /*hwndChild*/)
{
	return (LRESULT)::GetStockObject(WHITE_BRUSH);
}

LRESULT CDlgNotify::OnNcHitTest(CPoint pt)
{
	CRect rcCaption(0, 0, 377, 21);

	ClientToScreen(rcCaption);
	if (rcCaption.PtInRect(pt))
		return HTCAPTION;

	SetMsgHandled(FALSE);
	return 0;
}

LRESULT CDlgNotify::OnBnClickedConfirm(UINT /*nNotifyCode*/, int /*nId*/, HWND /*hWndCtl*/)
{
	DestroyWindow();
	m_hWnd = NULL;

	ATLASSERT( ::IsWindow(m_pWndMsgTarget->m_hWnd) );
	m_pWndMsgTarget->SendMessage(WM_APP_NOTIFYCHOOSED, (WPARAM)m_nChoice, (LPARAM)m_nDelayTime);

	return 0;
}

LRESULT CDlgNotify::OnUpdateTimeLeft(UINT /*nMsg*/, WPARAM wp, LPARAM lp)
{
	LPCTSTR szAction = (LPCTSTR)wp;
	int time_left = (int)lp;

	CString strMsg;
	strMsg.Format(m_pWndMsgTarget->_env.GetLanguage().NotifyMessage, szAction, time_left);

	m_lblMessage.SetWindowText(strMsg);

	return 0;
}
