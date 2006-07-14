// stdafx.cpp : source file that includes just the standard includes
//	APO.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color)
{
	if (!hBmp) return NULL;

	BITMAP bm;
	GetObject( hBmp, sizeof(BITMAP), &bm ); // get bitmap attributes

	CDC dcScreen;
	CDC dcBmp;

	dcScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	dcBmp.CreateCompatibleDC(dcScreen); //Creates a memory device context for the bitmap
	dcBmp.SelectBitmap(hBmp); //selects the bitmap in the device context

	CSimpleArray<CRect> Regions;

	int nState=-1;
	int nStart=0;
	bool bIsMask;

	// scan the image to find areas of solid regions to draw
	for ( int y = 0; y < bm.bmHeight; ++y )
	{
		nState = -1;
		nStart = 0;
		for ( int x = 0; x < bm.bmWidth; ++x )
		{
			// get color
			bIsMask=(dcBmp.GetPixel(x,bm.bmHeight-y-1)==color);

			switch( nState )
			{
			case -1: // first pixel
				if( bIsMask )
					nState = 0;
				else 
					nState = 1;

				nStart = 0;
				break;

			case 1: // solid
				if( bIsMask )
				{
					// create region
					Regions.Add( CRect( nStart, bm.bmHeight - y - 1, x, bm.bmHeight - y) );
					nState = 0;
				} 
				break;

			case 0: // transaparent

				if( !bIsMask )
				{
					// start region
					nStart = x;
					nState = 1;
				}
				break;
			}
		}

		// solid last pixel?
		if( nStart != y-1 && nState==1)
		{
			// add region
			Regions.Add( CRect( nStart, bm.bmHeight - y - 1, bm.bmWidth, bm.bmHeight - y) );
		}
	}

	dcBmp.DeleteDC(); //release the bitmap


	// create region
	// Under WinNT the ExtCreateRegion returns NULL (by Fable@aramszu.net) 
	// HRGN hRgn = ExtCreateRegion( NULL, RDHDR + pRgnData->nCount * sizeof(RECT), (LPRGNDATA)pRgnData );
	// ExtCreateRegion replacement { 
	HRGN hRgn=CreateRectRgn(0, 0, 0, 0);
	ATLASSERT( hRgn!=NULL );

	for( int i=0;i<(int)Regions.GetSize();++i )
	{
		const CRect& rt = Regions[i];
		HRGN hr=CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);
		ATLVERIFY(CombineRgn(hRgn, hRgn, hr, RGN_OR)!=ERROR);
		if (hr) DeleteObject(hr);
	}

	ATLASSERT( hRgn!=NULL );

	return hRgn;
}

/*
// This function converts the given bitmap to a DFB.
// Returns true if the conversion took place,
// false if the conversion either unneeded or unavailable
bool ConvertToDFB(HBITMAP& hBitmap)
{
	bool bConverted = false;
	BITMAP stBitmap;
	if (GetObject(hBitmap, sizeof(stBitmap), &stBitmap) && stBitmap.bmBits)
	{
		// that is a DIB. Now we attempt to create
		// a DFB with the same sizes, and with the pixel
		// format of the display (to omit conversions
		// every time we draw it).
		HDC hScreen = GetDC(NULL);
		if (hScreen)
		{
			HBITMAP hDfb = 
				CreateCompatibleBitmap(hScreen, 
				stBitmap.bmWidth, stBitmap.bmHeight);
			if (hDfb)
			{
				// now let's ensure what we've created is a DIB.
				if (GetObject(hDfb, sizeof(stBitmap), 
					&stBitmap) && !stBitmap.bmBits)
				{
					// ok, we're lucky. Now we have
					// to transfer the image to the DFB.
					HDC hMemSrc = CreateCompatibleDC(NULL);
					if (hMemSrc)
					{
						HGDIOBJ hOldSrc = SelectObject(hMemSrc, hBitmap);
						if (hOldSrc)
						{
							HDC hMemDst = CreateCompatibleDC(NULL);
							if (hMemDst)
							{
								HGDIOBJ hOldDst = SelectObject(hMemDst, hDfb);
								if (hOldDst)
								{
									// transfer the image using BitBlt
									// function. It will probably end in the
									// call to driver's DrvCopyBits function.
									if (BitBlt(hMemDst, 0, 0, 
										stBitmap.bmWidth, stBitmap.bmHeight, 
										hMemSrc, 0, 0, SRCCOPY))
										bConverted = true; // success

									VERIFY(SelectObject(hMemDst, hOldDst));
								}
								VERIFY(DeleteDC(hMemDst));
							}
							VERIFY(SelectObject(hMemSrc, hOldSrc));
						}
						VERIFY(DeleteDC(hMemSrc));
					}
				}

				if (bConverted)
				{
					VERIFY(DeleteObject(hBitmap)); // it's no longer needed
					hBitmap = hDfb;
				}
				else
					VERIFY(DeleteObject(hDfb));
			}
			ReleaseDC(NULL, hScreen);
		}
	}
	return bConverted;
}

// This function converts the given bitmap to a DIB.
// Returns true if the conversion took place,
// false if the conversion either unneeded or unavailable
bool ConvertToDIB(HBITMAP& hBitmap)
{
	bool bConverted = false;
	BITMAP stBitmap;
	if (GetObject(hBitmap, sizeof(stBitmap), 
		&stBitmap) && !stBitmap.bmBits)
	{
		// that is a DFB. Now we attempt to create
		// a DIB with the same sizes and pixel format.
		HDC hScreen = GetDC(NULL);
		if (hScreen)
		{
			union {
				BITMAPINFO stBitmapInfo;
				BYTE pReserveSpace[sizeof(BITMAPINFO) 
					+ 0xFF * sizeof(RGBQUAD)];
			};
			ZeroMemory(pReserveSpace, sizeof(pReserveSpace));
			stBitmapInfo.bmiHeader.biSize = sizeof(stBitmapInfo.bmiHeader);
			stBitmapInfo.bmiHeader.biWidth = stBitmap.bmWidth;
			stBitmapInfo.bmiHeader.biHeight = stBitmap.bmHeight;
			stBitmapInfo.bmiHeader.biPlanes = 1;
			stBitmapInfo.bmiHeader.biBitCount = stBitmap.bmBitsPixel;
			stBitmapInfo.bmiHeader.biCompression = BI_RGB;

			if (stBitmap.bmBitsPixel <= 8)
			{
				stBitmapInfo.bmiHeader.biClrUsed = 
					1 << stBitmap.bmBitsPixel;
				// This image is paletted-managed.
				// Hence we have to synthesize its palette.
			}
			stBitmapInfo.bmiHeader.biClrImportant = 
				stBitmapInfo.bmiHeader.biClrUsed;

			PVOID pBits;
			HBITMAP hDib = CreateDIBSection(hScreen, 
				&stBitmapInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

			if (hDib)
			{
				// ok, we're lucky. Now we have
				// to transfer the image to the DFB.
				HDC hMemSrc = CreateCompatibleDC(NULL);
				if (hMemSrc)
				{
					HGDIOBJ hOldSrc = SelectObject(hMemSrc, hBitmap);
					if (hOldSrc)
					{
						HDC hMemDst = CreateCompatibleDC(NULL);
						if (hMemDst)
						{
							HGDIOBJ hOldDst = SelectObject(hMemDst, hDib);
							if (hOldDst)
							{
								if (stBitmap.bmBitsPixel <= 8)
								{
									// take the DFB's palette and set it to our DIB
									HPALETTE hPalette = 
										(HPALETTE) GetCurrentObject(hMemSrc, OBJ_PAL);
									if (hPalette)
									{
										PALETTEENTRY pPaletteEntries[0x100];
										UINT nEntries = GetPaletteEntries(hPalette, 
											0, stBitmapInfo.bmiHeader.biClrUsed, 
											pPaletteEntries);
										if (nEntries)
										{
											ASSERT(nEntries <= 0x100);
											for (UINT nIndex = 0; nIndex < nEntries; nIndex++)
												pPaletteEntries[nEntries].peFlags = 0;
											VERIFY(SetDIBColorTable(hMemDst, 0, 
												nEntries, (RGBQUAD*) pPaletteEntries) == nEntries);

										}
									}
								}

								// transfer the image using BitBlt function.
								// It will probably end in the
								// call to driver's DrvCopyBits function.
								if (BitBlt(hMemDst, 0, 0, stBitmap.bmWidth, 
									stBitmap.bmHeight, hMemSrc, 0, 0, SRCCOPY))
									bConverted = true; // success

								VERIFY(SelectObject(hMemDst, hOldDst));
							}
							VERIFY(DeleteDC(hMemDst));
						}
						VERIFY(SelectObject(hMemSrc, hOldSrc));
					}
					VERIFY(DeleteDC(hMemSrc));
				}

				if (bConverted)
				{
					VERIFY(DeleteObject(hBitmap)); // it's no longer needed
					hBitmap = hDib;
				}
				else
					VERIFY(DeleteObject(hDib));
			}
			ReleaseDC(NULL, hScreen);
		}
	}
	return bConverted;
}
*/