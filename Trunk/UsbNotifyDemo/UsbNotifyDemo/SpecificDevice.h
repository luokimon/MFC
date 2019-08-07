#pragma once
class CSpecificDevice
{
public:
	CSpecificDevice(CString name);
	virtual ~CSpecificDevice();

private:
	CDeviceInformation* m_pDevInfo;
	CBridgeInformation* m_pBridgeInfo;
	CString m_Name;
	CString m_ParentName;

private:
	CSpecificDevice();

public:
	virtual void operator = (const CSpecificDevice& o);
	CString GetName() { return m_Name; }
};

