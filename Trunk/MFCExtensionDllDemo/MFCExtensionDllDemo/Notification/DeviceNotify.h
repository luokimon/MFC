#pragma once

class CDeviceNotify
{
public:
	CDeviceNotify();
	virtual ~CDeviceNotify();

private:
	CArray<HDEVNOTIFY, HDEVNOTIFY>* m_hNotifyList;

public:
	BOOL CDeviceNotify::RegisterNotification(GUID guid, HWND hWnd);
};

