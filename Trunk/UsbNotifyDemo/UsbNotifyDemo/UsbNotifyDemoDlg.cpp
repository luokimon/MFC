
// UsbNotifyDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UsbNotifyDemo.h"
#include "UsbNotifyDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GUID GUID_USB_DISK = { 0x53F56307L, 0xB6BF, 0x11D0, {0x94, 0xF2, 0x00, 0xA0, 0xC9, 0x1E, 0xFB, 0x8B} };
GUID GUID_USB_HID = { 0x4D1E55B2L, 0xF16F, 0x11CF, {0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };

#define USB_DISK_STRING		(_T("\\\\?\\usbstor#disk&ven_usb&prod_flash_drive&rev_1.68"))
#define USB_HID_STRING		(_T("\\\\?\\hid#vid_26ae&pid_8507"))
#define USB_HID_STRING_1	(_T("\\\\?\\hid#vid_26ae&pid_8507"))
#define USB_HID_STRING_2	(_T("\\\\?\\hid#vid_222a&pid_0001"))


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


// CUsbNotifyDemoDlg dialog



CUsbNotifyDemoDlg::CUsbNotifyDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USBNOTIFYDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbNotifyDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUsbNotifyDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()

	ON_MESSAGE(WMU_DEVICE_GROUP_UPDATE, OnDeviceGroupUpdate)
END_MESSAGE_MAP()


// CUsbNotifyDemoDlg message handlers

BOOL CUsbNotifyDemoDlg::OnInitDialog()
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
	VariableInitialization();
	RegisterNotification();
	InitialEnumeration();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUsbNotifyDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUsbNotifyDemoDlg::OnPaint()
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
HCURSOR CUsbNotifyDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUsbNotifyDemoDlg::VariableInitialization()
{
	m_pDevGroupList = NULL;
}

BOOL CUsbNotifyDemoDlg::RegisterNotification()
{
	BOOL bRet = FALSE;
	bRet = m_devNotify.RegisterNotification(GUID_USB_DISK, GetSafeHwnd());
	if (!bRet)
	{
		AfxMessageBox(_T("RegisterNotification GUID_USB_DISK Failed"));
		return bRet;
	}
	bRet = m_devNotify.RegisterNotification(GUID_USB_HID, GetSafeHwnd());
	if (!bRet)
	{
		AfxMessageBox(_T("RegisterNotification GUID_USB_HID Failed"));
		return bRet;
	}
	return bRet;
}

void CUsbNotifyDemoDlg::InitialEnumeration()
{
	m_devEnumeration.AddFilter(new CEnumerationPackage(GUID_USB_DISK, USB_DISK_STRING, _T("")));
	m_devEnumeration.AddFilter(new CEnumerationPackage(GUID_USB_HID, USB_HID_STRING_1, _T("&col")));
	m_devEnumeration.AddFilter(new CEnumerationPackage(GUID_USB_HID, USB_HID_STRING_2, _T("&col")));

	m_devEnumeration.SetNewArrival(TRUE);
	m_devEnumeration.DelegateTask();
}

LRESULT CUsbNotifyDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
				CString str(dbcc->dbcc_name);
				if (dbcc->dbcc_classguid == GUID_USB_DISK)
				{
					TRACE(_T("USB-DISK CHANGE \n"));
					if (wParam == DBT_DEVICEARRIVAL)
					{
						m_devManager.Add(str);
						m_devEnumeration.SetNewArrival(TRUE);
					}
					else
						m_devManager.Remove(str);
				}
				else if (dbcc->dbcc_classguid == GUID_USB_HID)
				{
					if (-1 == str.Find(_T("&Col")))
					{
						TRACE(_T("USB-HID CHANGE \n"));
						if (wParam == DBT_DEVICEARRIVAL)
						{
							m_devManager.Add(str);
							m_devEnumeration.SetNewArrival(TRUE);
						}
						else
							m_devManager.Remove(str);
					}
				}
			}
		}
	}
	break;
	default:
		break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CUsbNotifyDemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}

LRESULT CUsbNotifyDemoDlg::OnDeviceGroupUpdate(WPARAM wParam, LPARAM lParam)
{
	if (NULL != m_pDevGroupList)
		delete m_pDevGroupList;
	m_pDevGroupList = (CDeviceGroupList*)wParam;

	for (int i = 0; i < m_pDevGroupList->GetCount(); i++)
	{
		//TRACE(_T("[%s]- %s \n"), grp->GetAt(i)->GetHubName(), grp->GetAt(i)->GetNodeName());
		TRACE(_T("[%s]- %s \n"), (*m_pDevGroupList)[i]->GetHubName(), (*m_pDevGroupList)[i]->GetNodeName());
	}

	return 0;
}
