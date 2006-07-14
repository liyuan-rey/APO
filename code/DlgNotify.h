// DlgNotify.h : Declaration of the CDlgNotify
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CDlgMain;

// CDlgNotify

class CDlgNotify : public CDialogImpl<CDlgNotify>,
				   public CWinDataExchange<CDlgNotify>	
{
public:
	CDlgNotify();
	~CDlgNotify();

	enum { IDD = IDD_NOTIFY };

	BEGIN_MSG_MAP(CDlgNotify)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CTLCOLORSTATIC(OnCtlColor)
		MSG_WM_CTLCOLOREDIT(OnCtlColor)
		MSG_WM_NCHITTEST(OnNcHitTest)
		COMMAND_ID_HANDLER_EX(IDC_BTN_CONFIRM, OnBnClickedConfirm)
		MESSAGE_HANDLER_EX(WM_APP_UPDATETIMELEFT, OnUpdateTimeLeft)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgNotify)
		DDX_CONTROL_HANDLE(IDC_LBL_MESSAGE, m_lblMessage);
		DDX_CONTROL_HANDLE(IDC_EDT_DELAYTIME, m_edtDelayTime);
		DDX_CONTROL_HANDLE(IDC_SPN_DELAYTIME, m_spnDelayTime);
		DDX_CONTROL_HANDLE(IDC_BTN_CONFIRM, m_btnConfirm);
		DDX_RADIO(IDC_RDO_GOON, m_nChoice);
		DDX_INT(IDC_EDT_DELAYTIME, m_nDelayTime);
	END_DDX_MAP()

	LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);
	LRESULT OnPaint(HDC hdc);
	LRESULT OnEraseBkgnd(HDC hdc);
	LRESULT OnCtlColor(HDC hdc, HWND hwndChild);
	LRESULT OnNcHitTest(CPoint pt);
	LRESULT OnBnClickedConfirm(UINT nNotifyCode = 0, int nId = 0, HWND hWndCtl = NULL);
	LRESULT OnUpdateTimeLeft(UINT nMsg, WPARAM wp, LPARAM lp);

public:
	CDlgMain* m_pWndMsgTarget;

protected:
	CStatic m_lblMessage;
	int m_nChoice;
	CEdit m_edtDelayTime;
	int m_nDelayTime;
	CUpDownCtrl m_spnDelayTime;
	CButton m_btnConfirm;
};


