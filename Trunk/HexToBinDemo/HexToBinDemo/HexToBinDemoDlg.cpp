
// HexToBinDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HexToBinDemo.h"
#include "HexToBinDemoDlg.h"
#include "afxdialogex.h"
#include "HexFileConverter.h"

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


// CHexToBinDemoDlg dialog



CHexToBinDemoDlg::CHexToBinDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEXTOBINDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHexToBinDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHexToBinDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_FILE_SELECT, &CHexToBinDemoDlg::OnBnClickedButtonFileSelect)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CHexToBinDemoDlg::OnBnClickedButtonConvert)
END_MESSAGE_MAP()


// CHexToBinDemoDlg message handlers

BOOL CHexToBinDemoDlg::OnInitDialog()
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

void CHexToBinDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHexToBinDemoDlg::OnPaint()
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
HCURSOR CHexToBinDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHexToBinDemoDlg::OnBnClickedButtonFileSelect()
{
	// TODO: Add your control notification handler code here
	//CString szFilter = _T("Binary File(*.bin)|*.bin|Intel HEX File(*.hex)|*.hex||");
	CString szFilter = _T("Intel HEX File(*.hex)|*.hex||");
	CFileDialog dlg(TRUE, _T("hex"), NULL, 0, szFilter, this);
	if (IDOK == dlg.DoModal())
	{
		m_strFilePath = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_PATH, m_strFilePath);
	}
}


void CHexToBinDemoDlg::OnBnClickedButtonConvert()
{
	// TODO: Add your control notification handler code here
	CHexFileConverter hex(m_strFilePath);

	std::unique_ptr<BYTE> buff(new BYTE[64 * 1024]);
	ZeroMemory(buff.get(), 64 * 1024);
	hex.ToBin(buff.get(), 64*1024);

	CString strOutput = m_strFilePath.MakeLower();
	if (0 != strOutput.Replace(_T(".hex"), _T(".bin")))
	{
		CFile file;
		file.Open(strOutput, CFile::modeCreate | CFile::modeWrite);
		file.Write(buff.get(), 64 * 1024);
		file.Flush();
		file.Close();
	}
}
