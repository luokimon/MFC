
// TreeCtrlDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTreeCtrlDemoApp:
// See TreeCtrlDemo.cpp for the implementation of this class
//

class CTreeCtrlDemoApp : public CWinApp
{
public:
	CTreeCtrlDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTreeCtrlDemoApp theApp;
