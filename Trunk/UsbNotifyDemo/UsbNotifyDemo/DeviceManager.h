#pragma once
class CDeviceManager
{
public:
	CDeviceManager();
	virtual ~CDeviceManager();
private:
	CList<CSpecificDevice*, CSpecificDevice*> m_devList;
	CCriticalSection m_csDevice;

public:
	CDeviceGroupList* m_pDevGroupList;

public:
	void Arrival(CString name);
	void Add(CSpecificDevice* dev);
	void Remove(CString name);
	void RemoveAll();

	void SetDeviceGroupList(CDeviceGroupList* list);
};

