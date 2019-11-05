#pragma once

class CDeviceGroup
{
public:
	CDeviceGroup(CString node, CString hub) :m_nodeName(node), m_hubName(hub) {};
	virtual ~CDeviceGroup() {};

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

class CDeviceGroupArray
{
public:
	CDeviceGroupArray();
	virtual ~CDeviceGroupArray();

private:
	CArray<CDeviceGroup*, CDeviceGroup*>* m_pDevGroupArray;

public:
	void Add(LPCTSTR name, LPCTSTR parent);
	//void RemoveAll();
	//UINT GetCount();
	//CDeviceGroup* GetAt(UINT idx);
	CString GetHubName(CString& name);
	//virtual CDeviceGroup* operator [](int idx);
};

