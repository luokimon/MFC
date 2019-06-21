#pragma once


typedef struct _NOTIFY_DEVICE_ID
{
	TCHAR ID[MAX_PATH];
	TCHAR ParentID[MAX_PATH];
}NOTIFY_DEVICE_ID, *PNOTIFY_DEVICE_ID;

class CDeviceNotify
{
public:
	CDeviceNotify();
	virtual ~CDeviceNotify();

	//LPCSTR operator [](int i);
	//VOID RefreshDeviceArray(const PTCHAR id, const PTCHAR parentID);

private:
	HDEVNOTIFY m_hDev;
	CArray<HDEVNOTIFY, HDEVNOTIFY>* m_hNotifyList;
	CStringArray* m_devList;
	CArray< NOTIFY_DEVICE_ID, NOTIFY_DEVICE_ID&>* m_devID;
public:
	BOOL RegisterNotification(HWND hWnd, DWORD flags, GUID guid);
	BOOL RegisterAllNotification(HWND hWnd);
	BOOL RegisterSpecialNotification(GUID guid, HWND hWnd);
	BOOL EnumDevice(GUID guid, CString preStr, CString inexistStr);
	//INT GetSize();
	//LPCSTR GetAt(INT idx);
	//LPCSTR GetParentAt(INT idx);
	//VOID RemoveAt(INT idx);
};

