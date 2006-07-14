// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _RICHEDIT_VER	0x0100

#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES

#include <atlstr.h>

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atldlgs.h>
#include <atlcrack.h>
#include <atlddx.h>
#include <atlgdi.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atltypes.h>
#include <atlpath.h>

#include <Mmsystem.h>		// For PlaySound API
#pragma comment(lib, "winmm.lib")


// User defined
//
#define WM_APP_UPDATETIMELEFT           (WM_APP + 1)
#define WM_APP_NOTIFYCHOOSED            (WM_APP + 2)

#define TIMERID_PERFORMSCHEDULE			1
#define TIMERID_PERFORMSCHEDULE_ELAPSE	1000

#define EDIT_NUMBER_MIN					0
#define EDIT_NUMBER_MAX					999
#define EDIT_LIMITTEXT_NUM				3


#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
