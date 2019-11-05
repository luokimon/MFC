
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars




/*
#include "../MFCExtension/SafeBuff.h"
#include "../MFCExtension/FileHelper.h"
#include "../MFCExtension/ProcedureLog.h"

#ifdef DEBUG
#pragma comment(lib, "../Debug/MFCExtension.lib")
#else
#pragma comment(lib, "../Release/MFCExtension.lib")
#endif 
*/

#include <Dbt.h>
#include <memory>
#include "setupapi.h"
#include "cfgmgr32.h"
#pragma comment(lib, "Setupapi.lib")

#include "Global.h"
#include "Notification/GuidHelper.h"
#include "Notification/DeviceNotify.h"
#include "Notification/DeviceGroupArray.h"
#include "Notification/DeviceEnumeration.h"


#include "../DmlHelpers/Global.h"
#ifdef DEBUG
#pragma comment(lib, "../Debug/DmlHelpers.lib")
#else
#pragma comment(lib, "../Release/DmlHelpers.lib")
#endif 


#include "../DmlUSB/Import.h"
#ifdef DEBUG
#pragma comment(lib, "../Debug/DmlUSB.lib")
#else
#pragma comment(lib, "../Release/DmlUSB.lib")
#endif 


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


