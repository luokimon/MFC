
// WorkspaceCleanerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WorkspaceCleaner.h"
#include "WorkspaceCleanerDlg.h"
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


// CWorkspaceCleanerDlg dialog



CWorkspaceCleanerDlg::CWorkspaceCleanerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORKSPACECLEANER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWorkspaceCleanerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_KEEP_EMPTY, m_ckbKeepEmptyFolder);
	DDX_Control(pDX, IDC_EDIT_CLEAN_FILTER, m_edtCleanFilter);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edtPath);
	DDX_Control(pDX, IDC_RICHEDIT2_LOG, m_reLog);
}

BEGIN_MESSAGE_MAP(CWorkspaceCleanerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CWorkspaceCleanerDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_START, &CWorkspaceCleanerDlg::OnBnClickedButtonStart)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CWorkspaceCleanerDlg message handlers

BOOL CWorkspaceCleanerDlg::OnInitDialog()
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

	CString title;
	title.Format(_T("WorkspaceCleaner%s"), CAppHelper::GetFileVersion());
	this->SetWindowText(title);
	LoadSetting();	
	AdjustLayout();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWorkspaceCleanerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWorkspaceCleanerDlg::OnPaint()
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
HCURSOR CWorkspaceCleanerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CWorkspaceCleanerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CWorkspaceCleanerDlg::OnBnClickedButtonPath()
{
	// TODO: Add your control notification handler code here
	BROWSEINFO bi;
	TCHAR name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
	bi.pszDisplayName = name;
	bi.lpszTitle = _T("Select Folder");
	bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return;

	CString strDirectoryPath;
	SHGetPathFromIDList(idl, strDirectoryPath.GetBuffer(MAX_PATH));
	strDirectoryPath.ReleaseBuffer();
	if (strDirectoryPath.IsEmpty())
		return;
	if (strDirectoryPath.Right(1) != _T("\\"))
		strDirectoryPath += _T("\\");

	m_strSelectedFolder = strDirectoryPath;
	SetDlgItemText(IDC_EDIT_PATH, m_strSelectedFolder);
}


void CWorkspaceCleanerDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	CWaitCursor wait;

	if (m_strSelectedFolder.IsEmpty())
	{
		AfxMessageBox(_T("Please enter or choose a directory."));
		return;
	}

	m_reLog.SetReadOnly(FALSE);
	m_reLog.SetFocus();
	m_reLog.SetSel(0, -1);
	m_reLog.Clear();
	m_reLog.SetReadOnly(TRUE);

	ParsingFilter();

	Clean(m_strSelectedFolder);
}


void CWorkspaceCleanerDlg::Clean(const CString& strPath)
{
	CFileFind find;
	BOOL bFilterFolder;

	CString srcDir = strPath;
	if (srcDir.Right(1) != _T("\\"))
		srcDir += _T("\\");

	srcDir += _T("*.*");
	BOOL bResult = find.FindFile(srcDir);
	if (!bResult)
	{
		CString s;
		s.Format(_T("Error! Probably the directory\n\n%s\n\n does not exist."), strPath);
		AfxMessageBox(s, MB_ICONSTOP);
		return;
	}

	while (bResult)
	{
		bResult = find.FindNextFile();

		if ((!find.IsDots()) && find.IsDirectory())
		{
			CString path = find.GetFilePath();
			path.MakeUpper();

			bFilterFolder = FALSE;
			for (int i = 0; i < m_CleanFolderArray.GetCount(); i++)
			{
				UINT idx = path.Find(m_CleanFolderArray[i]);
				idx = path.GetLength() - m_CleanFolderArray[i].GetLength();
				if (path.Find(m_CleanFolderArray[i]) == path.GetLength() - m_CleanFolderArray[i].GetLength())
				{
					bFilterFolder = TRUE;
					RecursiveDelete(find.GetFilePath());
					if (BST_CHECKED != m_ckbKeepEmptyFolder.GetCheck())
					{
						m_reLog.AddRedText(_T("RemoveDirectory "));
						m_reLog.AddBlackText(find.GetFilePath()+ _T("\r\n"));
						RemoveDirectory(find.GetFilePath());
					}
				}
			}

			if(!bFilterFolder)
				Clean(find.GetFilePath());
		}
		else if ((!find.IsDots()) && (!find.IsDirectory()))
		{
			CleanFile(find.GetFilePath());
		}
	}

	find.Close();
}

void CWorkspaceCleanerDlg::CleanFile(const CString& path)
{
	CString str = path.Right(4);
	str.MakeLower();

	// delete files
	for (int i = 0; i < m_CleanFileArray.GetCount(); i++)
	{
		if (str == m_CleanFileArray[i])
		{
			m_reLog.AddRedText(_T("DeleteFile "));
			m_reLog.AddBlackText(path + _T("\r\n"));
			DeleteFile(path);
		}
	}
}

void CWorkspaceCleanerDlg::RecursiveDelete(const CString& szPath)
{
	CFileFind find;
	CString path = szPath;

	if (path.Right(1) != _T("\\"))
		path += _T("\\");
	path += _T("*.*");

	BOOL bResult = find.FindFile(path);
	while (bResult)
	{
		bResult = find.FindNextFile();
		if ((!find.IsDots()) && (!find.IsDirectory()))
		{
			m_reLog.AddRedText(_T("DeleteFile "));
			m_reLog.AddBlackText(find.GetFilePath() + _T("\r\n"));
			DeleteFile(find.GetFilePath());
		}
		else if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			path = find.GetFilePath();
			RecursiveDelete(path);

			m_reLog.AddRedText(_T("RemoveDirectory "));
			m_reLog.AddBlackText(path + _T("\r\n"));
			RemoveDirectory(path);
		}
	}
	find.Close();
}

void CWorkspaceCleanerDlg::ParsingFilter()
{
	CString str;
	UINT idx = 0;
	BOOL bRet = TRUE;
	m_edtCleanFilter.GetWindowText(str);

	m_CleanFolderArray.RemoveAll();
	m_CleanFileArray.RemoveAll();

	while (bRet)
	{
		CString item;
		bRet = AfxExtractSubString(item, str, idx++, ',');
		item.Trim();

		if (!bRet)
			continue;

		if (-1 != item.Find(_T("*.")))
			m_CleanFileArray.Add(item.Right(item.GetLength() - 1).MakeLower());
		else
			m_CleanFolderArray.Add(_T("\\")+item.MakeUpper());
	}
}

void CWorkspaceCleanerDlg::LoadSetting()
{
	CString filter;
	CString str = CAppHelper::GetCurrentAppDirectory() + _T("setting.ini");
	GetPrivateProfileString(_T("Setting"), _T("Value"), _T("Debug, Release, *.aps, .vs"), filter.GetBufferSetLength(1024), 1024, str);
	filter.ReleaseBuffer();
	m_edtCleanFilter.SetWindowText(filter);

	GetPrivateProfileString(_T("Setting"), _T("Path"), _T(""), m_strSelectedFolder.GetBufferSetLength(1024), 1024, str);
	m_strSelectedFolder.ReleaseBuffer();
	m_edtPath.SetWindowText(m_strSelectedFolder);
}

void CWorkspaceCleanerDlg::SaveSetting()
{
	CString filter;
	CString str = CAppHelper::GetCurrentAppDirectory() + _T("setting.ini");
	m_edtCleanFilter.GetWindowText(filter);
	WritePrivateProfileString(_T("Setting"), _T("Value"), filter, str);

	m_edtPath.GetWindowText(filter);
	WritePrivateProfileString(_T("Setting"), _T("Path"), filter, str);
}

void CWorkspaceCleanerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	SaveSetting();

	CDialogEx::OnClose();
}

void CWorkspaceCleanerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	AdjustLayout();
}

void CWorkspaceCleanerDlg::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
}

