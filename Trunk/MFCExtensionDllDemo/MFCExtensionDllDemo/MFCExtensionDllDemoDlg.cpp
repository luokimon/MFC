
// MFCExtensionDllDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCExtensionDllDemo.h"
#include "MFCExtensionDllDemoDlg.h"
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


// CMFCExtensionDllDemoDlg dialog



CMFCExtensionDllDemoDlg::CMFCExtensionDllDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCEXTENSIONDLLDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCExtensionDllDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCExtensionDllDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WMU_DEVICE_GROUP_UPDATE, OnDeviceGroupUpdate)
END_MESSAGE_MAP()


// CMFCExtensionDllDemoDlg message handlers

BOOL CMFCExtensionDllDemoDlg::OnInitDialog()
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
	//CSafeBuff buff(10);
	if (!m_devNotify.RegisterNotification(CGuidHelper::GetHostBoard(), GetSafeHwnd()))
	{
		AfxMessageBox(_T("RegisterNotification Fail!"));
	}

	m_devEnumeration.AddFilter(new CEnumerationFilter(CGuidHelper::GetHostBoard(), CGuidHelper::GetHostBoardPrefix(), _T("")));
	m_devEnumeration.SetNewArrival(TRUE);
	m_devEnumeration.DelegateTask();

	TestDmlHelper();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCExtensionDllDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCExtensionDllDemoDlg::OnPaint()
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
HCURSOR CMFCExtensionDllDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CMFCExtensionDllDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DEVICECHANGE:
	{
		if ((wParam == DBT_DEVICEARRIVAL) || (wParam == DBT_DEVICEREMOVECOMPLETE))
		{
			PDEV_BROADCAST_HDR hDev = (PDEV_BROADCAST_HDR)lParam;
			if (DBT_DEVTYP_DEVICEINTERFACE == hDev->dbch_devicetype)
			{
				DEV_BROADCAST_DEVICEINTERFACE *dbcc = (DEV_BROADCAST_DEVICEINTERFACE *)hDev;
				CString name(dbcc->dbcc_name);
				if(CGuidHelper::IsHostBoard(dbcc->dbcc_classguid))
				{
					if (wParam == DBT_DEVICEARRIVAL)
						DeviceArrival(name);
					//TRACE(_T("USB-DISK Arrival:%s \n"), name);
					else
						DeviceRemove(name);
						//TRACE(_T("USB-DISK Remove:%s \n"), name);
				}
			}
		}
	}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CMFCExtensionDllDemoDlg::DeviceArrival(const CString& name)
{
	TRACE(_T("DeviceArrival: %s\r\n"), name);
	m_devEnumeration.SetNewArrival(TRUE);

	CUsbWrapper usb;
	usb.CreateDevice(CUsbWrapper::HOST_BOARD, name);

	usb.OpenDevice(name);

	BYTE buff[128];

	buff[0] = 0x01;
	buff[1] = 0x1C;
	buff[2] = 0x12;
	buff[3] = 0x1e;
	usb.SetPowerStatus(buff, 4);
	usb.SetDeviceGroup(0x040b);
	usb.GetDeviceID(buff, 2);
	usb.GetDeviceInformation(buff, 128);
	buff[0] = 0x00;
	buff[1] = 0x1C;
	buff[2] = 0x12;
	buff[3] = 0x1e;
	usb.SetPowerStatus(buff, 4);

	//TODO: Get Device Info Base on name
}

void CMFCExtensionDllDemoDlg::DeviceRemove(const CString& name)
{
	TRACE(_T("DeviceRemove: %s\r\n"), name);

	//TODO: Remove Device Base on name
}

LRESULT CMFCExtensionDllDemoDlg::OnDeviceGroupUpdate(WPARAM wParam, LPARAM lParam)
{
	CDeviceGroupArray* pGroup = (CDeviceGroupArray *)wParam;
	delete pGroup;
	//m_portManagerList.SetDeviceGroupList((CDeviceGroupList*)wParam);

	//if (m_bInitialEnumeration)
	//{
	//	m_bInitialEnumeration = FALSE;
	//	for (int i = 0; i < m_portManagerList.GetDeviceGroupList()->GetCount(); i++)
	//		m_portManagerList.Arrival((*m_portManagerList.GetDeviceGroupList())[i]->GetNodeName());
	//}

	return 0;
}


void CMFCExtensionDllDemoDlg::TestDmlHelper()
{
	CString str = CAppHelper::GetCurrentAppDirectory();
	TRACE(_T("CAppHelper::GetCurrentAppDirectory: %s\r\n"), str);
	str = CAppHelper::GetCurrentAppPath();
	TRACE(_T("CAppHelper::GetCurrentAppPath: %s\r\n"), str);
	str = CAppHelper::GetFileVersion();
	TRACE(_T("CAppHelper::GetFileVersion: %s\r\n"), str);

	//CFileHelper::CreateMultipleDirectory(_T("C:\\Users\\kimon\\Desktop\\11\\22\\33\\44\\555"));
}