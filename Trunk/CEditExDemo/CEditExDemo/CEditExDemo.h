
// CEditExDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CCEditExDemoApp:
// See CEditExDemo.cpp for the implementation of this class
//

class CCEditExDemoApp : public CWinApp
{
public:
	CCEditExDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCEditExDemoApp theApp;
