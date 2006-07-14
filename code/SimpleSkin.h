#pragma once
#include "resource.h"

#include "ImgCtx.h"

class CSimpleSkin
{
public:
	CSimpleSkin(void);
	virtual ~CSimpleSkin(void);

public:
	// szRootDir must end with '\' (or '/' for url)
	BOOL Load(LPCTSTR szRootDir);

	// nId: 0 for entire skin, other for specified widget id
	// return handle of bitmap must destroy by using ::DeleteObject(...), return NULL for error
	HBITMAP GetWidgetBitmap(UINT nId);

	// nId: 0 for entire skin, other for specified widget id
	// return position of top-left corner, (-1,-1) for error
	POINT GetWidgetPosition(UINT nId);

	// nId: 0 for entire skin, other for specified widget id
	// return (-1,-1) for error
	SIZE GetWidgetSize(UINT nId);

	BOOL Draw(HDC hdcDest, int x, int y, int cx, int cy, UINT nId);

private:
	BOOL Decode(LPCTSTR szImg);
	RECT SkinGetRect(UINT nId);
	void ClearBuffer();

private:
	CComPtr<IImgCtx> m_spImgCtx;
	CSize m_size;
	CString m_strRoot;

	CDC m_dcBuffer;
	CBitmap m_bmpBuffer;
	CBitmapHandle m_bmpOldBuffer;
	CPaletteHandle m_palBuffer;
	CPaletteHandle m_palOldBuffer;

	//////////////////////////////////////////////////////////////////////////
	//HACK hard code for skin configuration
public:
	inline CString GetConfigPath(LPCTSTR szRootDir) {
		return (CString(szRootDir) + _T("skincfg.ini"));
	}

	inline CString GetImagePath(LPCTSTR szRootDir) {
		return (CString(szRootDir) + _T("skin.png"));
	}
	
	inline COLORREF GetColorKey() {
		return RGB(255, 0, 255);
	}

};
