
// ColorStaticDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CColorStaticDemoApp:
// See ColorStaticDemo.cpp for the implementation of this class
//

class CColorStaticDemoApp : public CWinApp
{
public:
	CColorStaticDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CColorStaticDemoApp theApp;
