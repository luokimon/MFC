
// CMFCPropertyGridCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CMFCPropertyGridCtrlDemo.h"
#include "CMFCPropertyGridCtrlDemoDlg.h"
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


// CCMFCPropertyGridCtrlDemoDlg dialog



CCMFCPropertyGridCtrlDemoDlg::CCMFCPropertyGridCtrlDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMFCPROPERTYGRIDCTRLDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCMFCPropertyGridCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_propertyGrid);
}

BEGIN_MESSAGE_MAP(CCMFCPropertyGridCtrlDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CCMFCPropertyGridCtrlDemoDlg message handlers

BOOL CCMFCPropertyGridCtrlDemoDlg::OnInitDialog()
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
	HDITEM item;
	item.cxy = 120;
	item.mask = HDI_WIDTH;
	//m_propertyGrid.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	m_propertyGrid.GetHeaderCtrl().SetItem(0, &item);

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(
		_T("我是不是帅哥?"),
		_T("看选项"),
		_T(""));

	//pProp->AddOption(_T("是"));
	//pProp->AddOption(_T("肯定是"));
	//pProp->AddOption(_T("绝对是"));
	pProp->AddOption(_T("是"));
	pProp->AddOption(_T("否"));
	pProp->AllowEdit(FALSE);


	CMFCPropertyGridColorProperty* pProp3 = new CMFCPropertyGridColorProperty(
		_T("颜色"), RGB(0, 111, 200));

	CMFCPropertyGridFileProperty* pProp4 = new CMFCPropertyGridFileProperty(
		_T("打开文件"), TRUE, _T("D://test.txt"));


	LOGFONT font = { NULL };
	CMFCPropertyGridFontProperty* pProp5 = new CMFCPropertyGridFontProperty(
		_T("选择字体"), font);

	CMFCPropertyGridByteProperty* pProp6 = new CMFCPropertyGridByteProperty(
		_T("Value"), _T("0x23")
	);


	CMFCPropertyGridProperty* grp1 = new CMFCPropertyGridProperty(_T("分组1"));
	CMFCPropertyGridProperty* grp2 = new CMFCPropertyGridProperty(_T("分组1"));

	grp1->AddSubItem(pProp);
	grp1->AddSubItem(pProp3);
	grp2->AddSubItem(pProp4);
	grp2->AddSubItem(pProp5);
	grp2->AddSubItem(pProp6);

	m_propertyGrid.AddProperty(grp1);
	m_propertyGrid.AddProperty(grp2);

	m_propertyGrid.SetVSDotNetLook();
	m_propertyGrid.MarkModifiedProperties();
	//m_propertyGrid.AddProperty(pProp);
	//m_propertyGrid.AddProperty(pProp3);
	//m_propertyGrid.AddProperty(pProp4);
	//m_propertyGrid.AddProperty(pProp5);

	//m_maskedEdit.EnableMask(_T("  AAAAA"), _T("0x_____"));
	//m_maskedEdit.SetValidChars(_T("0123456789ABCDEFabcdef"));

	//m_maskedEdit.SetWindowText(_T("0x1234"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCMFCPropertyGridCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCMFCPropertyGridCtrlDemoDlg::OnPaint()
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
HCURSOR CCMFCPropertyGridCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

