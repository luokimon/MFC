
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


#define WMU_DEVICE_GROUP_UPDATE			(WM_USER + 100)
#define WMU_SPECIFIC_DEVICE_UPDATE		(WM_USER + 101)


#include <Dbt.h>
#include <memory>

/*
#include "../UsbBase/DeviceBase.h"
#include "../UsbBase/HIDDevice.h"
#include "../UsbBase/MSDevice.h"
#include "../UsbBase/BridgeInfo.h"
#include "../UsbBase/DeviceInfo.h"
#include "../UsbBase/DeviceInfo30.h"
*/
#include "DeviceGroup.h"
#include "EnumerationPackage.h"
#include "DeviceInformation.h"
#include "BridgeInformation.h"
#include "SpecificDevice.h"
#include "DeviceNotify.h"
#include "DeviceManager.h"
#include "DeviceEnumeration.h"

//#ifdef DEBUG
//#pragma comment(lib, "../Debug/UsbBase.lib")
//#else
//#pragma comment(lib, "../Release/UsbBase.lib")
//#endif

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


