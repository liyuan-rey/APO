#include "StdAfx.h"
#include "SimpleSkin.h"

CSimpleSkin::CSimpleSkin(void) : m_size(0, 0)
{
	HRESULT hr = m_spImgCtx.CoCreateInstance(CLSID_IImgCtx);

	if (FAILED(hr) || !m_spImgCtx)
		ATLASSERT( FALSE && _T("Err: could not create IImgCtx instance. Try call ::CoInitialize() first"));
}

CSimpleSkin::~CSimpleSkin(void)
{
	ClearBuffer();
}

BOOL CSimpleSkin::Decode(LPCTSTR szImgfile)
{
	if (!m_spImgCtx)
		return FALSE;

	CComBSTR bstrImgFile = szImgfile;
	HRESULT hr = m_spImgCtx->Load(bstrImgFile, 0);

	if (FAILED(hr))
		return FALSE;

	ULONG ulState;

	do 
	{
		hr = m_spImgCtx->GetStateInfo(&ulState, &m_size, FALSE);
		if (FAILED(hr))
			return FALSE;

		::Sleep(50);
	} while (ulState & IMGLOAD_LOADING);

	ATLASSERT( ulState & IMGLOAD_COMPLETE);

	return TRUE;
}

BOOL CSimpleSkin::Load(LPCTSTR szRootDir)
{
	// decode image
	if (!Decode(GetImagePath(szRootDir)))
		return FALSE;

	m_strRoot = szRootDir;

	// prepare skin buffer
	ClearBuffer();

	CDC dcScreen;
	dcScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	if (!m_dcBuffer.CreateCompatibleDC(dcScreen))
		return FALSE;

	HRESULT hr = m_spImgCtx->GetPalette(&(m_palBuffer.m_hPalette)); // no problem since CPaletteHandle

	if (!m_bmpBuffer.CreateCompatibleBitmap(dcScreen, m_size.cx, m_size.cy))
	{
		ATLASSERT( FALSE && _T("Err: create skin bmp failed."));
		return FALSE;
	}

	m_bmpOldBuffer = m_dcBuffer.SelectBitmap(m_bmpBuffer);

	if (m_palBuffer.m_hPalette)
	{
		m_palOldBuffer = m_dcBuffer.SelectPalette(m_palBuffer, TRUE);
		m_dcBuffer.RealizePalette();
	}

	hr = m_spImgCtx->Draw(m_dcBuffer, (LPRECT)CRect(0, 0, m_size.cx, m_size.cy));

	// test code
/*
	BITMAP bm;
	m_bmpBuffer.GetBitmap(&bm);

	m_dcBuffer.SelectBitmap(m_bmpOldBuffer);
	m_bmpBuffer.SetBitmapDimension(m_size.cx, m_size.cy);

	m_bmpBuffer.GetBitmap(&bm);
*/

	return !(FAILED(hr));
}

BOOL CSimpleSkin::Draw(HDC hdcDest, int x, int y, int cx, int cy, UINT nId)
{
	if (!m_dcBuffer.m_hDC)
		return FALSE;

	CDCHandle dcDest(hdcDest);

	// don't know if this is needed...
/*
	CPaletteHandle palOld;
	if (m_palBuffer.m_hPalette)
	{
		palOld = dcDest.SelectPalette(m_palBuffer, TRUE);
		dcDest.RealizePalette();
	}
*/

	CRect rcSkin = SkinGetRect(nId);

	BOOL bRet = ::StretchBlt(dcDest, x, y, cx, cy, m_dcBuffer, rcSkin.left,
		rcSkin.top, rcSkin.Width(), rcSkin.Height(), SRCCOPY);

/*
	if (palOld.m_hPalette)
	{
		dcDest.SelectPalette(palOld, TRUE);
		dcDest.RealizePalette();
	}
*/

	return bRet;
}

void CSimpleSkin::ClearBuffer()
{
	if (m_dcBuffer.m_hDC)
	{
		if (m_bmpOldBuffer.m_hBitmap)
		{
			m_dcBuffer.SelectBitmap(m_bmpOldBuffer);
			m_bmpOldBuffer = NULL;
		}

		if (m_palOldBuffer.m_hPalette)
		{
			m_dcBuffer.SelectPalette(m_palOldBuffer, TRUE);
			m_dcBuffer.RealizePalette();

			m_palOldBuffer = NULL;
		}

		m_bmpBuffer.DeleteObject();
		m_palBuffer = NULL;
		m_dcBuffer.DeleteDC();
	}
}

RECT CSimpleSkin::SkinGetRect(UINT nId)
{
	CRect rcRet(0, 0, 0, 0);

	switch (nId)
	{
	case IDD_MAIN:
		rcRet.SetRect(0,0,318,382);
		break;
	case IDD_NOTIFY:
		rcRet.SetRect(318,0,678,195);
		break;
	case IDC_BTN_HIDE:
		rcRet.SetRect(318,211,444,226);
		break;
	case IDC_BTN_HELP:
		rcRet.SetRect(318,226,393,241);
		break;
	case IDC_BTN_ABOUT:
		rcRet.SetRect(318,241,393,256);
		break;
	case IDC_BTN_LANGUAGE:
		rcRet.SetRect(318,256,393,271);
		break;
	default:
		ATLASSERT( FALSE && _T("Err: unexcept value in CSimpleSkin::SkinGetRect.") );
		break;
	}

	return rcRet;
}

HBITMAP CSimpleSkin::GetWidgetBitmap(UINT nId)
{
	if (!m_dcBuffer.m_hDC)
		return NULL;

	// calc return bitmap
	CDC dcScreen;
	CDC dcMem;

	dcScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	if (!dcMem.CreateCompatibleDC(dcScreen))
	{
		ATLASSERT( FALSE && _T("Err: canot create mem dc."));
		return NULL;
	}

	CBitmapHandle bmpRet;
	CRect rcBmp = SkinGetRect(nId);
	if (rcBmp.IsRectEmpty())
		return NULL;

	if (!bmpRet.CreateCompatibleBitmap(dcScreen, rcBmp.Width(), rcBmp.Height()))
	{
		ATLASSERT( FALSE && _T("Err: create bmp failed."));
		return NULL;
	}

	CBitmapHandle bmpOld = dcMem.SelectBitmap(bmpRet);

	CPaletteHandle palOld;
	if (m_palBuffer.m_hPalette)
	{
		palOld = dcMem.SelectPalette(m_palBuffer, TRUE);
		dcMem.RealizePalette();
	}

	dcMem.BitBlt(0, 0, rcBmp.Width(), rcBmp.Height(), m_dcBuffer, rcBmp.left, rcBmp.top, SRCCOPY);

	dcMem.SelectBitmap(bmpOld);
	if (palOld.m_hPalette)
	{
		dcMem.SelectPalette(palOld, TRUE);
		dcMem.RealizePalette();
	}

	::SetBitmapDimensionEx(bmpRet, rcBmp.Width(), rcBmp.Height(), NULL);

	return bmpRet;
}

POINT CSimpleSkin::GetWidgetPosition(UINT nId)
{
	//TODO add implement code here
	POINT ptRet= {-1, -1};

	switch (nId)
	{
	case IDC_BTN_HIDE:
		ptRet = CPoint(263, 1);
		break;
	case IDC_BTN_HELP:
		ptRet = CPoint(238, 1);
		break;
	case IDC_BTN_ABOUT:
		ptRet = CPoint(213, 1);
		break;
	case IDC_BTN_LANGUAGE:
		ptRet = CPoint(188, 1);
		break;
	default:
		ATLASSERT( FALSE && _T("Err: unexcept value in CSimpleSkin::GetWidgetPosition.") );
		break;
	}

	return ptRet;
}

SIZE CSimpleSkin::GetWidgetSize(UINT /*nId*/)
{
	//TODO add implement code here
	SIZE sizeRet = {-1,-1};
	ATLASSERT( FALSE && _T("Err: not implement yet."));

	return sizeRet;
}
