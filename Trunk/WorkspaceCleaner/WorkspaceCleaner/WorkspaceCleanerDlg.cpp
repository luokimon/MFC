
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
	DDX_Control(pDX, IDC_CHECK_RELEASE, m_ckbReleaseFolder);
	DDX_Control(pDX, IDC_CHECK_DEBUG, m_ckbDebugFolder);
	DDX_Control(pDX, IDC_CHECK_KEEP_EMPTY, m_ckbKeepEmptyFolder);
	DDX_Control(pDX, IDC_CHECK_INTERMEDIATE_FILE, m_ckbIntermediateFiles);
	DDX_Control(pDX, IDC_CHECK_OUTPUT_FILE, m_ckbOutputFiles);
	DDX_Control(pDX, IDC_CHECK_OTHER_FILE, m_ckbOtherFiles);
	DDX_Control(pDX, IDC_CHECK_OTHER_FOLDER, m_ckbOtherFolders);
}

BEGIN_MESSAGE_MAP(CWorkspaceCleanerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CWorkspaceCleanerDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_START, &CWorkspaceCleanerDlg::OnBnClickedButtonStart)
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
	if (strDirectoryPath.Right(1) != "\\")
		strDirectoryPath += "\\";

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

	Clean(m_strSelectedFolder);
}


void CWorkspaceCleanerDlg::Clean(const CString& strPath)
{
	CFileFind find;

	CString srcDir = strPath;
	if (srcDir.Right(1) != "\\")
		srcDir += "\\";

	srcDir += "*.*";
	BOOL bResult = find.FindFile(srcDir);
	if (!bResult)
	{
		CString s;
		s.Format("Error! Probably the directory\n\n%s\n\n does not exist.", strPath);
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

			if (path.Right(8) == "\\RELEASE")
			{
				if (BST_CHECKED == m_ckbReleaseFolder.GetCheck())
				{
					RecursiveDelete(find.GetFilePath());

					if (BST_CHECKED != m_ckbKeepEmptyFolder.GetCheck())
						RemoveDirectory(find.GetFilePath());
				}
			}
			else if (path.Right(6) == "\\DEBUG")
			{
				if (BST_CHECKED == m_ckbDebugFolder.GetCheck())
				{
					RecursiveDelete(find.GetFilePath());

					if (BST_CHECKED != m_ckbKeepEmptyFolder.GetCheck())
						RemoveDirectory(find.GetFilePath());
				}
			}
			else if (path.Right(4) == "\\.VS")
			{
				if (BST_CHECKED == m_ckbOtherFolders.GetCheck())
				{
					RecursiveDelete(find.GetFilePath());

					if (BST_CHECKED != m_ckbKeepEmptyFolder.GetCheck())
						RemoveDirectory(find.GetFilePath());
				}
			}
			else
			{
				Clean(find.GetFilePath());
			}
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

	// delete intermediate files
	if (str == ".aps")
	{
		if(BST_CHECKED == m_ckbIntermediateFiles.GetCheck())
			DeleteFile(path);
	}


	// delete output files
	if (str == ".exe" || str == ".dll" || str == ".lib") 
	{
		if(BST_CHECKED == m_ckbOutputFiles.GetCheck())
			DeleteFile(path);
	}

	// delete other files


}

void CWorkspaceCleanerDlg::RecursiveDelete(const CString& szPath)
{
	CFileFind find;
	CString path = szPath;

	if (path.Right(1) != "\\")
		path += "\\";
	path += "*.*";

	BOOL bResult = find.FindFile(path);
	while (bResult)
	{
		bResult = find.FindNextFile();
		if ((!find.IsDots()) && (!find.IsDirectory()))
		{
			CString str;
			str.Format("Deleting file %s", find.GetFilePath());
			DeleteFile(find.GetFilePath());
		}
		else if (find.IsDots())
			continue;
		else if (find.IsDirectory())
		{
			path = find.GetFilePath();
			RecursiveDelete(path);
			RemoveDirectory(path);
		}
	}
	find.Close();
}