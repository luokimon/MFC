#include "stdafx.h"
#include "DeviceNotify.h"

CDeviceNotify::CDeviceNotify()
{
	m_hNotifyList = new CArray<HDEVNOTIFY, HDEVNOTIFY>();
	m_hNotifyList->RemoveAll();
}


CDeviceNotify::~CDeviceNotify()
{
	for (int i = 0; i < m_hNotifyList->GetSize(); i++)
	{

		if (m_hNotifyList->GetAt(i) != NULL)
		{
			::UnregisterDeviceNotification(m_hNotifyList->GetAt(i));
		}
	}
	m_hNotifyList->RemoveAll();
	delete m_hNotifyList;
}

BOOL CDeviceNotify::RegisterNotification(GUID guid, HWND hWnd)
{
	DEV_BROADCAST_DEVICEINTERFACE filter;

	ZeroMemory(&filter, sizeof(filter));
	filter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = guid;

	HDEVNOTIFY hDev = ::RegisterDeviceNotification(hWnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);

	if (hDev != NULL)
	{
		m_hNotifyList->Add(hDev);
		return TRUE;
	}
	return FALSE;
}