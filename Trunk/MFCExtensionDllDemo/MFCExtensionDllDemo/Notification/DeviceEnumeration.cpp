#include "stdafx.h"
#include "../stdafx.h"
#include "DeviceEnumeration.h"
#include "../MFCExtensionDllDemo.h"
#include "../MFCExtensionDllDemoDlg.h"

UINT EnumerateProc(PVOID lParam)
{
	CDeviceEnumeration* dev = (CDeviceEnumeration*)lParam;
	dev->Enumerate();
	return TRUE;
}

void CEnumerationFilter::operator = (const CEnumerationFilter& o)
{
	if (this != &o)
	{
		m_guid = o.m_guid;
		m_preStr = o.m_preStr;
		m_nonexistentStr = o.m_nonexistentStr;
	}
}

CDeviceEnumeration::CDeviceEnumeration()
{
	m_devGroupArray = NULL;
	m_hThread = NULL;
}


CDeviceEnumeration::~CDeviceEnumeration()
{
	SetRunningState(FALSE);
	if (m_hThread)
		WaitForSingleObject(m_hThread->m_hThread, INFINITE);

	for (int i = 0; i < m_ClassGuidArray.GetCount(); i++)
	{
		delete m_ClassGuidArray[i];
	}
	m_ClassGuidArray.RemoveAll();
}

void CDeviceEnumeration::DelegateTask()
{
	SetRunningState(TRUE);
	m_hThread = AfxBeginThread(EnumerateProc, this);
}

void CDeviceEnumeration::Enumerate()
{
	while (GetRunningState())
	{
		if (GetNewArrival())
		{
			SetNewArrival(FALSE);
			m_devGroupArray = new CDeviceGroupArray();
			for (int i = 0; i < m_ClassGuidArray.GetCount(); i++)
			{
				EnumDevice(m_ClassGuidArray[i]->GetGuid(),
					m_ClassGuidArray[i]->GetPrefixString(),
					m_ClassGuidArray[i]->GetNonexistentString());
			}
			UpdateDeviceGroups();
		}
		else
			Sleep(10);
	}
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

void CDeviceEnumeration::AddFilter(CEnumerationFilter* pack)
{
	m_ClassGuidArray.Add(pack);
}


BOOL CDeviceEnumeration::UpdateDeviceGroups()
{
	//TODO: Add Code to Notify Main Dialog
	CMFCExtensionDllDemoDlg* pDlg = (CMFCExtensionDllDemoDlg *)theApp.m_pMainWnd;
	return (0 != ::PostMessage(pDlg->m_hWnd, WMU_DEVICE_GROUP_UPDATE, (WPARAM)m_devGroupArray, NULL));
	 
}

void CDeviceEnumeration::EnumDevice(GUID guid, CString preStr, CString inexistStr)
{
	// Identify the specified devices
	HDEVINFO handle;
	handle = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

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
		str.MakeLower();

		if (0 == str.Find(preStr))
		{
			if (!inexistStr.IsEmpty())
			{
				if (-1 == str.Find(inexistStr))
				{
					CString strName(DeviceInterfaceDetailData->DevicePath);
					//m_devGroupList->Add(DeviceInterfaceDetailData->DevicePath, GetParentName(DeviceInfoData.DevInst));
					m_devGroupArray->Add(DeviceInterfaceDetailData->DevicePath, GetParentName(guid, str));
				}
			}
			else
			{
				CString strName(DeviceInterfaceDetailData->DevicePath);
				//m_devGroupList->Add(DeviceInterfaceDetailData->DevicePath, GetParentName(DeviceInfoData.DevInst));
				m_devGroupArray->Add(DeviceInterfaceDetailData->DevicePath, GetParentName(guid, str));
			}
		}

		count++;
	} while (resultEnum);

	SetupDiDestroyDeviceInfoList(handle);

}

CString CDeviceEnumeration::GetParentName(GUID guid, const CString& name)
{
	if(CGuidHelper::IsHostBoard(guid))
	{
		return GetParentName_HostBoard(name);
	}

	AfxMessageBox(_T("This Type of Device not implemented yet!"));
	return _T("");
}


CString CDeviceEnumeration::GetParentName_HostBoard(const CString& name)
{
	CString str(name);
	CString tmp;
	tmp = str.Left(str.ReverseFind('#'));
	str = tmp.Mid(tmp.ReverseFind('#') + 1);

	return str;
}