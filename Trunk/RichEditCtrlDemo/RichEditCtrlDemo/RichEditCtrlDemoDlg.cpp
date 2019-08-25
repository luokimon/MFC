
// RichEditCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RichEditCtrlDemo.h"
#include "RichEditCtrlDemoDlg.h"
#include "afxdialogex.h"

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


// CRichEditCtrlDemoDlg dialog



CRichEditCtrlDemoDlg::CRichEditCtrlDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RICHEDITCTRLDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRichEditCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_LOG, m_reLog);
}

BEGIN_MESSAGE_MAP(CRichEditCtrlDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CRichEditCtrlDemoDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CRichEditCtrlDemoDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CRichEditCtrlDemoDlg::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CRichEditCtrlDemoDlg message handlers

BOOL CRichEditCtrlDemoDlg::OnInitDialog()
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

void CRichEditCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRichEditCtrlDemoDlg::OnPaint()
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
HCURSOR CRichEditCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRichEditCtrlDemoDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("\r\n"));
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("123"));
	m_reLog.AddErrorLog(_T("456"));
	m_reLog.AddNormalLog(_T("\r\n"));
}

static DWORD CALLBACK
MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CFile* pFile = (CFile*)dwCookie;

	pFile->Write(pbBuff, cb);
	*pcb = cb;

	return 0;
}

static DWORD CALLBACK
MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CFile* pFile = (CFile*)dwCookie;

	*pcb = pFile->Read(pbBuff, cb);

	return 0;
}


void CRichEditCtrlDemoDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	//CFile cFile(_T("Test.rtf"), CFile::modeCreate | CFile::modeWrite);
	//EDITSTREAM es;
	//es.dwCookie = (DWORD)&cFile;
	//es.pfnCallback = MyStreamOutCallback;
	//m_reLog.StreamOut(SF_RTF, es);
	CFileDialog pCFileDialog(TRUE, NULL, NULL, 0, _T("Rich Text(*.rtf)|*.rtf||"));
	pCFileDialog.m_ofn.lpstrTitle = _T("另存为");
	if (IDOK == pCFileDialog.DoModal())
	{
		m_reLog.SaveTo(pCFileDialog.GetPathName());
	}
}


void CRichEditCtrlDemoDlg::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	CFileDialog pCFileDialog(TRUE, NULL, NULL, 0, _T("Rich Text(*.rtf)|*.rtf||"));
	pCFileDialog.m_ofn.lpstrTitle = _T("打开文件");
	if (IDOK == pCFileDialog.DoModal())
	{
		m_reLog.LoadFrom(pCFileDialog.GetPathName());
	}

}
