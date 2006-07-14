// ComboBoxEx2.h : Declaration of the CComboBoxEx2
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <atlctrls.h>

// CComboBoxEx2

#ifndef _WIN32_WCE

class CComboBoxEx2 : public CWindowImpl<CComboBoxEx2, CComboBoxEx>
{
public:
	CComboBoxEx2(HWND hWnd = NULL) : m_bNeedRecalc(FALSE)
	{ m_hWnd = hWnd; }

	CComboBoxEx2& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}
	
	DECLARE_WND_SUPERCLASS(NULL, CComboBoxEx::GetWndClassName())

    BEGIN_MSG_MAP_EX(CComboBoxEx2)
		MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)
    END_MSG_MAP()

protected:

	LRESULT OnCtlColorListBox(HDC /*hdc*/, HWND hwndChild)
	{
		if (m_bNeedRecalc && ((GetStyle() & 0x0F) != CBS_SIMPLE))
		{
			int nHeight = 0;
			int nCount = GetCount();

			if (0 == nCount)
			{
				CRect rcComboEx;
				GetWindowRect(rcComboEx);
				nHeight = rcComboEx.Height();
			}
			else
			{
				for (int i = 0; i < nCount; i++)
					nHeight += GetItemHeight(i);
			}

			nHeight += ::GetSystemMetrics(SM_CYBORDER) * 2 + 1;

			CWindow wndChild = hwndChild;
			CRect rcAction;
			wndChild.GetWindowRect(rcAction);
			wndChild.SetWindowPos(NULL, 0, 0, rcAction.Width(), nHeight,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

			m_bNeedRecalc = FALSE;

#if 0
			static int counter = 0;
			ATLTRACE(_T("counter: %d\n"), ++counter);
#endif // for test performance
		}

		SetMsgHandled(FALSE);
		return NULL;
	}

public:
	int InsertItem(const COMBOBOXEXITEM* pCBItem)
	{
		int nRet = CComboBoxEx::InsertItem(pCBItem);
		_SetNeedRecalc(nRet);
		return nRet;
	}

	int InsertItem(UINT nMask, int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage, 
		int iIndent, int iOverlay, DWORD lParam)
	{
		int nRet = CComboBoxEx::InsertItem(nMask, nIndex, lpszItem, nImage, nSelImage, iIndent, iOverlay, lParam);
		_SetNeedRecalc(nRet);
		return nRet;
	}

	int InsertItem(int nIndex, LPCTSTR lpszItem, int nImage, int nSelImage, int iIndent, DWORD lParam = 0)
	{
		int nRet = CComboBoxEx::InsertItem(nIndex, lpszItem, nImage, nSelImage, iIndent, lParam);
		_SetNeedRecalc(nRet);
		return nRet;
	}

	int DeleteItem(int iIndex)
	{
		int nRet = CComboBoxEx::DeleteItem(iIndex);
		_SetNeedRecalc(nRet);
		return nRet;
	}

	int DeleteString(UINT nIndex)
	{
		int nRet = CComboBoxEx::DeleteString(nIndex);
		_SetNeedRecalc(nRet);
		return nRet;
	}

	void ResetContent()
	{
		CComboBoxEx::ResetContent();

		if (!m_bNeedRecalc)
			m_bNeedRecalc = TRUE;
	}

private:
	inline void _SetNeedRecalc(int nRet) {
		if (!m_bNeedRecalc && (CB_ERR != nRet))
			m_bNeedRecalc = TRUE;
	}

protected:
	BOOL m_bNeedRecalc;
};

#endif // _WIN32_WCE
