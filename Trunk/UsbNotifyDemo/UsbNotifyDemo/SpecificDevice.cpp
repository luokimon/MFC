#include "stdafx.h"
#include "SpecificDevice.h"
#include "UsbNotifyDemo.h"
#include "UsbNotifyDemoDlg.h"

UINT InitialProc(PVOID lParam)
{
	CSpecificDevice* dev = (CSpecificDevice*)lParam;
	dev->Initialize();
	return TRUE;
}

CSpecificDevice::CSpecificDevice()
	:m_pDevInfo(NULL), m_pBridgeInfo(NULL), m_hThread(NULL)
{
}

CSpecificDevice::CSpecificDevice(CString name)
	: CSpecificDevice()
{
	m_Name = name;
	m_ParentName = _T("");
}

CSpecificDevice::~CSpecificDevice()
{
	WaitForSingleObject(m_hThread->m_hThread, INFINITE);

	if (NULL != m_pDevInfo)
	{
		delete m_pDevInfo;
		m_pDevInfo = NULL;
	}

	if (NULL != m_pBridgeInfo)
	{
		delete m_pBridgeInfo;
		m_pBridgeInfo = NULL;
	}
}

void CSpecificDevice::DelegateInitialize()
{
	m_hThread = AfxBeginThread(InitialProc, this);
}

void CSpecificDevice::Initialize()
{
	//TODO: 添加初始化代码
	Sleep(5000);
	m_pDevInfo = new CDeviceInformation();
	m_pBridgeInfo = new CBridgeInformation();

	/*
	
	BOOL bRet = TRUE;
	CHIDDevice dev(m_Name);
	//CBridgeInfo bridgeInfo;
	//CDeviceInfo* devInfo = new CDeviceInfo30();
	m_pBridgeInfo = new CBridgeInfo();
	m_pDevInfo = new CDeviceInfo30();

	do
	{
		bRet = dev.OpenDeviceHandle();
		if (!bRet)
			break;

		bRet = dev.SetUpdateMode(FALSE);
		if (!bRet)
			break;

		Sleep(200);

		bRet = dev.GetBridgeInfo(m_pBridgeInfo->GetBuffer(), m_pBridgeInfo->GetLength());
		if (!bRet)
			break;

		if (!m_pBridgeInfo->HeaderVerify())
			break;

		dev.ReadDeviceInfo(m_pDevInfo->GetBuffer(), m_pDevInfo->GetLength());
	} while (FALSE);
	*/
	UpdateSpecificDevice();
}

BOOL CSpecificDevice::UpdateSpecificDevice()
{
	CUsbNotifyDemoDlg* pDlg = (CUsbNotifyDemoDlg *)theApp.m_pMainWnd;
	return (0 != ::PostMessage(pDlg->m_hWnd, WMU_SPECIFIC_DEVICE_UPDATE, (WPARAM)this, NULL));
}

void CSpecificDevice::operator = (const CSpecificDevice& o)
{
	if (this != &o)
	{
		m_pDevInfo = o.m_pDevInfo;
		m_pBridgeInfo = o.m_pBridgeInfo;
		m_Name = o.m_Name;
		m_ParentName = o.m_ParentName;
	}
}