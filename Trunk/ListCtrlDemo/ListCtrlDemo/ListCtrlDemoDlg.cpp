
// ListCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "ListCtrlDemoDlg.h"
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


// CListCtrlDemoDlg dialog



CListCtrlDemoDlg::CListCtrlDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LISTCTRLDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Progress, m_ProgressCtrl);
}

BEGIN_MESSAGE_MAP(CListCtrlDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CListCtrlDemoDlg message handlers

BOOL CListCtrlDemoDlg::OnInitDialog()
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
	//DWORD dwStyle = m_ProgressCtrl.GetExtendedStyle();
	//m_ProgressCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//m_ProgressCtrl.SetExtendedStyle(dwStyle | LVS_EX_GRIDLINES);
	m_ProgressCtrl.Initial();
	/*m_ProgressCtrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_ProgressCtrl.InsertColumn(1, _T("Type"), LVCFMT_CENTER, 100);
	m_ProgressCtrl.InsertColumn(2, _T("Version"), LVCFMT_CENTER, 100);
	m_ProgressCtrl.InsertColumn(3, _T("Status"), LVCFMT_CENTER, 100);
	m_ProgressCtrl.InsertColumn(4, _T("Progress"), LVCFMT_CENTER, 100);
	m_ProgressCtrl.InitProgressColumn(4);*/

	m_ProgressCtrl.DeleteAllItems();

	//m_ProgressCtrl.InsertItem(0, _T(""));

	m_ProgressCtrl.Append();
	m_ProgressCtrl.SetTypeText(0, _T("USB"));
	m_ProgressCtrl.SetVersionText(0, _T("UNKNOWN"));
	m_ProgressCtrl.SetStatusText(0, _T("READY"));
	m_ProgressCtrl.SetProgressText(0, _T("20"));

	m_ProgressCtrl.Append();
	m_ProgressCtrl.SetTypeText(1, _T("USB"));
	m_ProgressCtrl.SetVersionText(1, _T("UNKNOWN"));
	m_ProgressCtrl.SetStatusText(1, _T("READY"));
	m_ProgressCtrl.SetProgressText(1, _T("30"));

	//m_ProgressCtrl.UpdateWindow();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CListCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CListCtrlDemoDlg::OnPaint()
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
HCURSOR CListCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

