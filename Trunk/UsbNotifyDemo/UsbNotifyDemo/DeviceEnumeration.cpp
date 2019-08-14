#include "stdafx.h"
#include "DeviceEnumeration.h"
#include "UsbNotifyDemo.h"
#include "UsbNotifyDemoDlg.h"

#include "setupapi.h"
#include "cfgmgr32.h"

#pragma comment(lib, "Setupapi.lib")

UINT EnumerateProc(PVOID lParam)
{
	CDeviceEnumeration* dev = (CDeviceEnumeration*)lParam;
	dev->Enumerate();
	return TRUE;
}

CDeviceEnumeration::CDeviceEnumeration()
{
	m_devGroupList = NULL;
	m_hThread = NULL;
}


CDeviceEnumeration::~CDeviceEnumeration()
{
	SetRunningState(FALSE);
	WaitForSingleObject(m_hThread->m_hThread, INFINITE);

	for (int i = 0; i < m_ClassGuidArray.GetCount(); i++)
	{
		delete m_ClassGuidArray[i];
	}
	m_ClassGuidArray.RemoveAll();

	delete m_devGroupList;
}

void CDeviceEnumeration::DelegateTask()
{
	SetRunningState(TRUE);
	m_hThread = AfxBeginThread(EnumerateProc, this);
}

BOOL CDeviceEnumeration::GetRunningState()
{
	BOOL bRet;
	m_criticalRunning.Lock();
	bRet = m_bRunningState;
	m_criticalRunning.Unlock();
	return bRet;
}

void CDeviceEnumeration::SetRunningState(BOOL state)
{
	m_criticalRunning.Lock();
	m_bRunningState = state;
	m_criticalRunning.Unlock();
}

BOOL CDeviceEnumeration::GetNewArrival()
{
	BOOL bRet;
	m_criticalArrival.Lock();
	bRet = m_bNewArrival;
	m_criticalArrival.Unlock();
	return bRet;
}
void CDeviceEnumeration::SetNewArrival(BOOL arrival)
{
	m_criticalArrival.Lock();
	m_bNewArrival = arrival;
	m_criticalArrival.Unlock();
}

void CDeviceEnumeration::AddFilter(CEnumerationPackage* pack)
{
	m_ClassGuidArray.Add(pack);
}

void CDeviceEnumeration::Enumerate()
{
	while (GetRunningState())
	{
		if (GetNewArrival())
		{
			SetNewArrival(FALSE);
			//m_devGroupList->RemoveAll();
			m_devGroupList = new CDeviceGroupList();
			for (int i = 0; i < m_ClassGuidArray.GetCount(); i++)
			{
				EnumDevice(m_ClassGuidArray[i]->GetGuid(),
					m_ClassGuidArray[i]->GetPrefixString(),
					m_ClassGuidArray[i]->GetNonexistentString());
			}

			UpdateDeviceGroups();
		}
		else
		{
			Sleep(10);
		}
	}
}

BOOL CDeviceEnumeration::UpdateDeviceGroups()
{
	CUsbNotifyDemoDlg* pDlg = (CUsbNotifyDemoDlg *)theApp.m_pMainWnd;
	return (0 != ::PostMessage(pDlg->m_hWnd, WMU_DEVICE_GROUP_UPDATE, (WPARAM)m_devGroupList, NULL));
}

CString CDeviceEnumeration::GetParentName(DWORD dnDevInst)
{
	TCHAR szParentID[MAX_PATH];
	DEVINST DevInstParent = 0;

	CM_Get_Parent(&DevInstParent, dnDevInst, 0);
	CM_Get_Device_ID(DevInstParent, szParentID, MAX_PATH, 0);
	CString strParent(szParentID);
	if (-1 != strParent.Find(_T("&MI_")))
	{
		ZeroMemory(szParentID, sizeof(szParentID));
		CM_Get_Parent(&DevInstParent, DevInstParent, 0);
		CM_Get_Device_ID(DevInstParent, szParentID, MAX_PATH, 0);
	}

	CString str(szParentID);
	CString tmp;
	tmp = str.Mid(str.ReverseFind('\\'));
	str = tmp.Mid(tmp.Find('&') + 1);

	return str;
}

void CDeviceEnumeration::EnumDevice(GUID guid, CString preStr, CString inexistStr)
{
	HDEVINFO handle;
	handle = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT); //Get only HID devices

	if (INVALID_HANDLE_VALUE == handle) return;

	BOOL resultEnum = FALSE;
	BOOL resultDetail = FALSE;
	DWORD count = 0;
	DWORD size = 0;
	DWORD requiredSize = 0;

	SP_DEVINFO_DATA DeviceInfoData;
	SP_DEVICE_INTERFACE_DATA  DeviceInterfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;
	do
	{
		DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		resultEnum = SetupDiEnumDeviceInterfaces(
			handle,
			NULL,
			&guid,
			count,
			&DeviceInterfaceData
		);

		if (!resultEnum) break;

		SetupDiGetDeviceInterfaceDetail(handle,
			&DeviceInterfaceData,
			NULL,
			0,
			&size,
			NULL);

		requiredSize = size;
		std::unique_ptr<BYTE> detailData(new BYTE[requiredSize]);
		DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)detailData.get();
		//DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
		DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		resultDetail = SetupDiGetDeviceInterfaceDetail(handle,
			&DeviceInterfaceData,
			DeviceInterfaceDetailData,
			size,
			&requiredSize,
			&DeviceInfoData);

		if (!resultDetail)
		{
			break;
		}

		CString str(DeviceInterfaceDetailData->DevicePath);

		if (0 == str.Find(preStr))
		{
			if (!inexistStr.IsEmpty())
			{
				if (-1 == str.Find(inexistStr))
				{
					CString strName(DeviceInterfaceDetailData->DevicePath);
					m_devGroupList->Add(DeviceInterfaceDetailData->DevicePath, GetParentName(DeviceInfoData.DevInst));
					//CEnumerationNode node(strName, GetParentName(DeviceInfoData.DevInst));
					//AddNode(node);
					//TRACE("[%s] %s\n", node.GetHubName(), node.GetName());
				}
			}
			else
			{
				CString strName(DeviceInterfaceDetailData->DevicePath);
				m_devGroupList->Add(DeviceInterfaceDetailData->DevicePath, GetParentName(DeviceInfoData.DevInst));
				//CEnumerationNode node(strName, GetParentName(DeviceInfoData.DevInst));
				//AddNode(node);
				//TRACE("[%s] %s\n", node.GetHubName(), node.GetName());
			}
		}

		count++;
	} while (resultEnum);

	SetupDiDestroyDeviceInfoList(handle);
}