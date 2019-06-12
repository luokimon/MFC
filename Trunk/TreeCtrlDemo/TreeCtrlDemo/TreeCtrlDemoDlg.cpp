
// TreeCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TreeCtrlDemo.h"
#include "TreeCtrlDemoDlg.h"
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


// CTreeCtrlDemoDlg dialog



CTreeCtrlDemoDlg::CTreeCtrlDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TREECTRLDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTreeCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PARAMETER, m_trParameter);
}

BEGIN_MESSAGE_MAP(CTreeCtrlDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD_CHILD, &CTreeCtrlDemoDlg::OnBnClickedButtonAddChild)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ROOT, &CTreeCtrlDemoDlg::OnBnClickedButtonAddRoot)
//	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE_PARAMETER, &CTreeCtrlDemoDlg::OnTvnEndlabeleditTreeParameter)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ITEM, &CTreeCtrlDemoDlg::OnBnClickedButtonDeleteItem)
//	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE_PARAMETER, &CTreeCtrlDemoDlg::OnTvnBeginlabeleditTreeParameter)
	//ON_NOTIFY(NM_DBLCLK, IDC_TREE_PARAMETER, &CTreeCtrlDemoDlg::OnNMDblclkTreeParameter)
	//ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE_PARAMETER, &CTreeCtrlDemoDlg::OnTvnEndlabeleditTreeParameter)
END_MESSAGE_MAP()


// CTreeCtrlDemoDlg message handlers

BOOL CTreeCtrlDemoDlg::OnInitDialog()
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

void CTreeCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTreeCtrlDemoDlg::OnPaint()
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
HCURSOR CTreeCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTreeCtrlDemoDlg::OnBnClickedButtonAddChild()
{
	// TODO: Add your control notification handler code here
	HTREEITEM item = m_trParameter.GetSelectedItem();

	if (NULL == item)
	{
		AfxMessageBox(_T("Please Selected An Item!"));
		return;
	}

	HTREEITEM subItem = m_trParameter.InsertItem(_T("New Child"), item);
	m_trParameter.EnsureVisible(subItem);
	m_trParameter.EditLabel(subItem);
	m_trParameter.Select(subItem, TVGN_CARET);
}


void CTreeCtrlDemoDlg::OnBnClickedButtonAddRoot()
{
	// TODO: Add your control notification handler code here
	m_trParameter.InsertItem(_T("New Root"), TVI_ROOT);
}


//BOOL CTreeCtrlDemoDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		if (pMsg->wParam == VK_RETURN)
//		{
//			CEdit * pLableEdit = m_trParameter.GetEditControl();
//			if (pLableEdit && pLableEdit->IsWindowVisible())
//			{
//				this->SetFocus();
//				return TRUE;
//			}
//		}
//	}
//
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


//void CTreeCtrlDemoDlg::OnTvnEndlabeleditTreeParameter(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	pTVDispInfo->item.mask = TVIF_TEXT;
//	m_trParameter.SetItem(&pTVDispInfo->item);
//	*pResult = 0;
//}


void CTreeCtrlDemoDlg::OnBnClickedButtonDeleteItem()
{
	// TODO: Add your control notification handler code here
	HTREEITEM item = m_trParameter.GetSelectedItem();

	if (NULL == item)
	{
		AfxMessageBox(_T("Please Selected An Item!"));
		return;
	}

	m_trParameter.DeleteItem(item);
}


//void CTreeCtrlDemoDlg::OnTvnBeginlabeleditTreeParameter(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


//void CTreeCtrlDemoDlg::OnNMDblclkTreeParameter(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//
//	HTREEITEM item = m_trParameter.GetSelectedItem();
//	if (NULL != item)
//		m_trParameter.EditLabel(item);
//
//	*pResult = 0;
//}


//void CTreeCtrlDemoDlg::OnTvnEndlabeleditTreeParameter(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	pTVDispInfo->item.mask = TVIF_TEXT;
//	if(NULL != pTVDispInfo->item.pszText)
//		m_trParameter.SetItem(&pTVDispInfo->item);
//	*pResult = 0;
//}


BOOL CTreeCtrlDemoDlg::PreTranslateMessage(MSG* pMsg)
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
