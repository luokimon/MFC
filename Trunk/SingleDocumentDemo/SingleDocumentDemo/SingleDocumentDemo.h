
// SingleDocumentDemo.h : main header file for the SingleDocumentDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CSingleDocumentDemoApp:
// See SingleDocumentDemo.cpp for the implementation of this class
//

class CSingleDocumentDemoApp : public CWinAppEx
{
public:
	CSingleDocumentDemoApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSingleDocumentDemoApp theApp;
