#pragma once
class CDeviceEnumeration
{
public:
	CDeviceEnumeration();
	virtual ~CDeviceEnumeration();

private:
	BOOL m_bRunningState;
	CCriticalSection m_criticalRunning;
	BOOL m_bNewArrival;
	CCriticalSection m_criticalArrival;
	CArray<CEnumerationPackage*, CEnumerationPackage*> m_ClassGuidArray;
	CDeviceGroupList* m_devGroupList;
	CWinThread* m_hThread;
private:
	BOOL GetRunningState();
	BOOL GetNewArrival();
	void EnumDevice(GUID guid, CString preStr, CString inexistStr);
	CString GetParentName(DWORD dnDevInst);
	BOOL UpdateDeviceGroups();
public:
	void Enumerate();
	void DelegateTask();
	void SetRunningState(BOOL state);
	void SetNewArrival(BOOL arrival);
	void AddFilter(CEnumerationPackage* pack);
};

