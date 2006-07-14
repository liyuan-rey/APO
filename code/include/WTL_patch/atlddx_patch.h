// patch of atlddx.h of WTL 7.5
//
// This patch acroding to http://www.codeproject.com/wtl/propsheetddx.asp

#ifndef __ATLDDX_PATCH_H__
#define __ATLDDX_PATCH_H__

#pragma once

#include <atlddx.h>


///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CWinDataExchange_Patch<T>


namespace WTL
{

// DDX map macros
#define DDX_COMBO_INDEX(nID, var) \
	if(nCtlID == (UINT)-1 || nCtlID == nID) \
	{ \
		if(!DDX_Combo_Index(nID, var, TRUE, bSaveAndValidate)) \
			return FALSE; \
	}

#define DDX_BOOL_RADIO(nID, var, nID2 ) \
	if(nCtlID == (UINT)-1 || nCtlID == nID) \
		DDX_Bool_Radio(nID, var, bSaveAndValidate, nID2);


///////////////////////////////////////////////////////////////////////////////
// CWinDataExchange_Patch - patch of CWinDataExchange

template <class T>
class CWinDataExchange_Patch : public CWinDataExchange< T >
{
public:
// Numeric exchange
	template <class Type>
	BOOL DDX_Combo_Index(UINT nID, Type& nVal, BOOL /*bSigned*/, BOOL bSave, BOOL bValidate = FALSE, Type nMin = 0, Type nMax = 0)
	{
		T* pT = static_cast<T*>(this);
		BOOL bSuccess = TRUE;
		
		if(bSave)
		{
			nVal = ::SendMessage(
									(HWND) pT->GetDlgItem(nID),              // handle to destination window 
									CB_GETCURSEL,	// message to send
									(WPARAM) 0,		// item index
									(LPARAM) 0		// not used; must be zero
								);
			bSuccess = (nVal == CB_ERR ? false : true);	
		}
		else
		{
			ATLASSERT(!bValidate || nVal >= nMin && nVal <= nMax);
			int iRet = 	::SendMessage( 
										(HWND) pT->GetDlgItem(nID),              // handle to destination window 
										CB_SETCURSEL,	// message to send
										(WPARAM) nVal,	// item index
										(LPARAM) 0		// not used; must be zero
									);
			
			bSuccess = ((iRet == CB_ERR && nVal != -1) ? false : true);
		}
		
		if(!bSuccess)
		{
			pT->OnDataExchangeError(nID, bSave);
		}
		else if(bSave && bValidate)	// validation
		{
			ATLASSERT(nMin != nMax);
			if(nVal < nMin || nVal > nMax)
			{
				_XData data;
				data.nDataType = ddxDataInt;
				data.intData.nVal = (long)nVal;
				data.intData.nMin = (long)nMin;
				data.intData.nMax = (long)nMax;
				pT->OnDataValidateError(nID, bSave, data);
				bSuccess = FALSE;
			}
		}
		return bSuccess;
	}

// Control state
	void DDX_Bool_Radio(UINT nID, BOOL& fValue, BOOL bSave, UINT nID2)
	{
		T* pT = static_cast<T*>(this);
		HWND hWndCtrl = pT->GetDlgItem(nID);
		HWND hWndCounterpart = pT->GetDlgItem(nID2);
		ATLASSERT(hWndCtrl || hWndCounterpart); //are your controls really there?


		if(bSave)
		{
			fValue = (int)::SendMessage(hWndCtrl, BM_GETCHECK, 0, 0L);
		}
		else
		{
			::SendMessage(hWndCtrl, BM_SETCHECK, fValue, 0L);
			::SendMessage(hWndCounterpart, BM_SETCHECK, !fValue, 0L);
		}
	}
};

}; //namespace WTL

#endif //__ATLDDX_PATCH_H__
