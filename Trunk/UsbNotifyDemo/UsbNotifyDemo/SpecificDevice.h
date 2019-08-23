#pragma once
class CSpecificDevice
{
public:
	CSpecificDevice(CString name);
	virtual ~CSpecificDevice();

private:
	CDeviceInformation* m_pDevInfo;
	CBridgeInformation* m_pBridgeInfo;
	//CBridgeInfo* m_pBridgeInfo;
	//CDeviceInfo* m_pDevInfo;
	CString m_Name;
	CString m_ParentName;
	CWinThread* m_hThread;

private:
	CSpecificDevice();
	BOOL UpdateSpecificDevice();

public:
	virtual void operator = (const CSpecificDevice& o);
	CString GetName() { return m_Name; }
	void SetName(CString name) { m_Name = name; }
	void SetHubName(CString name) { m_ParentName = name; }
	void DelegateInitialize();
	void Initialize();
};

