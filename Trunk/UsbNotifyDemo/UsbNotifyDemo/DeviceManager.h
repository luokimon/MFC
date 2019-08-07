#pragma once
class CDeviceManager
{
public:
	CDeviceManager();
	virtual ~CDeviceManager();
private:
	CList<CDeviceGroup*, CDeviceGroup*> m_devGroupList;
	CList<CSpecificDevice*, CSpecificDevice*> m_devList;
	CCriticalSection m_csDevice;

public:
	void Add(CString name);
	void Remove(CString name);
	void RemoveAll();
};

