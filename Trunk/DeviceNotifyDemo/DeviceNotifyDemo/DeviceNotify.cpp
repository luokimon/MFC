#include "stdafx.h"

#include <memory>
#include <Dbt.h>
#include "setupapi.h"
#include "cfgmgr32.h"

#include "DeviceNotify.h"

#pragma comment(lib, "Setupapi.lib")

GUID GUID_USB_DISK = { 0x53F56307L, 0xB6BF, 0x11D0, {0x94, 0xF2, 0x00, 0xA0, 0xC9, 0x1E, 0xFB, 0x8B} };

CDeviceNotify::CDeviceNotify()
{
	m_hDev = NULL;
	m_hNotifyList = new CArray<HDEVNOTIFY, HDEVNOTIFY>();
	m_devList = new CStringArray();
	m_devID = new CArray< NOTIFY_DEVICE_ID, NOTIFY_DEVICE_ID&>();
}

CDeviceNotify::~CDeviceNotify()
{
	if (m_hDev != NULL)
	{
		::UnregisterDeviceNotification(m_hDev);
		m_hDev = NULL;
	}

	for (int i = 0; i < m_hNotifyList->GetSize(); i++)
	{
		
		if (m_hNotifyList->GetAt(i) != NULL)
		{
			::UnregisterDeviceNotification(m_hNotifyList->GetAt(i));
		}
	}
	m_hNotifyList->RemoveAll();
	delete m_hNotifyList;

	m_devID->RemoveAll();
	delete m_devID;

	delete m_devList;
}

BOOL CDeviceNotify::RegisterNotification(HWND hWnd, DWORD flags, GUID guid)
{
	DEV_BROADCAST_DEVICEINTERFACE filter;

	ZeroMemory(&filter, sizeof(filter));
	filter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = guid;

	HDEVNOTIFY hDev = ::RegisterDeviceNotification(hWnd, &filter, flags);

	if (hDev != NULL)
	{
		m_hNotifyList->Add(hDev);
		return TRUE;
	}
	return FALSE;
}

BOOL CDeviceNotify::RegisterAllNotification(HWND hWnd)
{
	return RegisterNotification(hWnd, DEVICE_NOTIFY_ALL_INTERFACE_CLASSES, GUID_USB_DISK);
}

BOOL CDeviceNotify::RegisterSpecialNotification(GUID guid, HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return RegisterNotification(hWnd, DEVICE_NOTIFY_WINDOW_HANDLE, guid);
}

void CDeviceNotify::EnumDevice(GUID guid, CString preStr, CString inexistStr)
{
	//GUID deviceId;
	//HidD_GetHidGuid(&deviceId);

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
			//free(DeviceInterfaceDetailData);
			break;
		}

		// Get Root ID
		TCHAR szParentID[MAX_PATH];
		TCHAR szGrandFatherID[MAX_PATH];
		TCHAR szGreatGrandFatherID[MAX_PATH];
		DEVINST DevInstParent = 0;
		CM_Get_Parent(&DevInstParent, DeviceInfoData.DevInst, 0);
		CM_Get_Device_ID(DevInstParent, szParentID, MAX_PATH, 0);
		CM_Get_Parent(&DevInstParent, DevInstParent, 0);
		CM_Get_Device_ID(DevInstParent, szGrandFatherID, MAX_PATH, 0);
		CM_Get_Parent(&DevInstParent, DevInstParent, 0);
		CM_Get_Device_ID(DevInstParent, szGreatGrandFatherID, MAX_PATH, 0);
		
		CString str(DeviceInterfaceDetailData->DevicePath);

		if (0 == str.Find(preStr))
		{
			if (!inexistStr.IsEmpty())
			{
				if (-1 == str.Find(inexistStr))
				{
					CString str(szGrandFatherID);
					str.Format(_T("%s%s"), szGreatGrandFatherID, str.Mid(str.ReverseFind('\\')));
					//RefreshDeviceArray(DeviceInterfaceDetailData->DevicePath, str.GetBuffer());
				}
			}
			else
			{
				CString str(szParentID);
				str.Format(_T("%s%s"), szGrandFatherID, str.Mid(str.ReverseFind('\\')));
				//RefreshDeviceArray(DeviceInterfaceDetailData->DevicePath, str.GetBuffer());
			}
		}

		//free(DeviceInterfaceDetailData);
		count++;
	} while (resultEnum);

	SetupDiDestroyDeviceInfoList(handle);
}

/*
VOID CDeviceNotify::RefreshDeviceArray(const PTCHAR id, const PTCHAR parentID)
{
	m_devList->Add(id);
	NOTIFY_DEVICE_ID devID;
	ZeroMemory(&devID, sizeof(devID));
	strcpy_s(devID.ID, id);
	strcpy_s(devID.ParentID, parentID);

	BOOL match = FALSE;
	for (int i = 0; i < m_devID->GetSize(); i++)
	{
		if (0 == strcmp(parentID, m_devID->GetAt(i).ParentID))
		{
			match = TRUE;
			strcpy_s(m_devID->GetAt(i).ID, id);
			break;
		}
	}

	if (!match)
		m_devID->Add(devID);


	//TRACE(_T("%s\n"), parentID);
	//TRACE(_T("%s\n"), id);
}
*/

/*
INT CDeviceNotify::GetSize()
{
	//return m_devList->GetSize();
	return m_devID->GetSize();
}

LPCSTR CDeviceNotify::GetAt(INT idx)
{
	//return m_devList->GetAt(idx);
	return m_devID->GetAt(idx).ID;
}

LPCSTR CDeviceNotify::GetParentAt(INT idx)
{
	//return m_devList->GetAt(idx);
	return m_devID->GetAt(idx).ParentID;
}

VOID CDeviceNotify::RemoveAt(INT idx)
{
	//m_devList->RemoveAt(idx);
	m_devID->RemoveAt(idx);
}

LPCSTR CDeviceNotify::operator [](int i)
{
	//if (i >= m_devList->GetSize())
	//	return NULL;
	//return m_devList->GetAt(i);
	if (i >= m_devID->GetSize())
		return NULL;
	return m_devID->GetAt(i).ID;
}
*/