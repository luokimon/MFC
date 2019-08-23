#pragma once
class CDeviceGroup
{
public:
	CDeviceGroup(CString node, CString hub);
	virtual ~CDeviceGroup();

private:
	CString m_nodeName;
	CString m_hubName;

private:
	CDeviceGroup();

public:
	virtual void operator = (const CDeviceGroup& o);
	CString GetNodeName() { return m_nodeName; }
	CString GetHubName() { return m_hubName; }
};

class CDeviceGroupList
{
public:
	CDeviceGroupList();
	virtual ~CDeviceGroupList();

private:
	CArray<CDeviceGroup*, CDeviceGroup*>* m_pDevGroupList;

public:
	void Add(LPCTSTR name, LPCTSTR parent);
	void RemoveAll();
	UINT GetCount();
	CDeviceGroup* GetAt(UINT idx);
	CString GetHubName(CString& name);
	virtual CDeviceGroup* operator [](int idx);
};
