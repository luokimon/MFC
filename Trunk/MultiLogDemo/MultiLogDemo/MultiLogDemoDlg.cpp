
// MultiLogDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiLogDemo.h"
#include "MultiLogDemoDlg.h"
#include "afxdialogex.h"
#include "TextLog.h"
#include "HtmlLog.h"
#include "FileHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMultiLogDemoDlg dialog



CMultiLogDemoDlg::CMultiLogDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MULTILOGDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiLogDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiLogDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_HTML, &CMultiLogDemoDlg::OnBnClickedButtonHtml)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, &CMultiLogDemoDlg::OnBnClickedButtonText)
END_MESSAGE_MAP()


// CMultiLogDemoDlg message handlers

BOOL CMultiLogDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMultiLogDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMultiLogDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMultiLogDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMultiLogDemoDlg::OnBnClickedButtonHtml()
{
	// TODO: Add your control notification handler code here
	CBaseLog* log;

	log = new CHtmlLog();

	log->AddBodyStart();

	log->WriteLine(_T("Test WriteLine"));

	log->WriteData(_T("Test WriteData"));
	log->WriteLine(_T(""));

	log->AddTableStart();
	log->AddTableRowStart();
	for (int i = 0; i < 10; i++)
	{
		CString str;
		str.Format(_T("%d,"), i);
		log->AddTableUnitData(str);
	}
	log->AddTableRowEnd();

	log->AddTableRowStart();
	for (int i = 0; i < 10; i++)
	{
		CString str;
		str.Format(_T("%d,"), i);
		log->AddTableUnitData(str, RGB(0, 255, 0));		
	}
	log->AddTableRowEnd();

	log->AddTableEnd();

	log->AddBodyEnd();

	CFileHelper::CreateDataLog((PBYTE)log->GetContent().GetBuffer(), log->GetContent().GetLength(), _T("Html"));

	delete log;
	log = NULL;
}


void CMultiLogDemoDlg::OnBnClickedButtonText()
{
	// TODO: Add your control notification handler code here
	CBaseLog* log;

	log = new CTextLog();

	log->AddBodyStart();

	log->WriteLine(_T("Test WriteLine"));

	log->WriteData(_T("Test WriteData"));
	log->WriteLine(_T(""));

	log->AddTableStart();
	log->AddTableRowStart();
	for (int i = 0; i < 10; i++)
	{
		CString str;
		str.Format(_T("%d, "), i);
		log->AddTableUnitData(str);
	}
	log->AddTableRowEnd();

	log->AddTableRowStart();
	for (int i = 0; i < 10; i++)
	{
		CString str;
		str.Format(_T("%d, "), i);
		log->AddTableUnitData(str, RGB(0, 255, 0));
	}
	log->AddTableRowEnd();

	log->AddTableEnd();

	log->AddBodyEnd();


	CFileHelper::CreateDataLog((PBYTE)log->GetContent().GetBuffer(), log->GetContent().GetLength(), _T("Text"));

	delete log;
	log = NULL;
}
