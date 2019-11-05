#pragma once

class CEnumerationFilter
{
public:
	CEnumerationFilter(GUID guid, CString pre, CString non) 
		:m_guid(guid), m_preStr(pre), m_nonexistentStr(non) {};
	virtual ~CEnumerationFilter() {};

private:
	GUID m_guid;
	CString m_preStr;
	CString m_nonexistentStr;

private:
	CEnumerationFilter();

public:
	virtual void operator = (const CEnumerationFilter& o);
	GUID GetGuid() { return m_guid; }
	CString GetPrefixString() { return m_preStr; }
	CString GetNonexistentString() { return m_nonexistentStr; }

};

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
	CWinThread* m_hThread;
	CArray<CEnumerationFilter*, CEnumerationFilter*> m_ClassGuidArray;
	CDeviceGroupArray* m_devGroupArray;

private:
	BOOL GetRunningState();
	BOOL GetNewArrival();
	BOOL UpdateDeviceGroups();
	void EnumDevice(GUID guid, CString preStr, CString inexistStr);
	CString GetParentName(GUID guid, const CString& name);
	CString GetParentName_HostBoard(const CString& name);
public:
	void Enumerate();
	void SetRunningState(BOOL state);
	void SetNewArrival(BOOL arrival);
	void DelegateTask();
	void AddFilter(CEnumerationFilter* pack);

};

