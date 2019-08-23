#include "stdafx.h"
#include "DeviceGroup.h"


CDeviceGroup::CDeviceGroup()
{
}

CDeviceGroup::CDeviceGroup(CString node, CString hub)
	:m_nodeName(node), m_hubName(hub)
{

}


CDeviceGroup::~CDeviceGroup()
{
}

void CDeviceGroup::operator = (const CDeviceGroup& o)
{
	if (this != &o)
	{
		m_nodeName = o.m_nodeName;
		m_hubName = o.m_hubName;
	}
}



CDeviceGroupList::CDeviceGroupList()
{
	m_pDevGroupList = new CArray<CDeviceGroup*, CDeviceGroup*>();
}

CDeviceGroupList::~CDeviceGroupList()
{
	RemoveAll();
	delete m_pDevGroupList;
}


void CDeviceGroupList::Add(LPCTSTR name, LPCTSTR parent)
{
	CDeviceGroup* grp = new CDeviceGroup(name, parent);
	m_pDevGroupList->Add(grp);
}

void CDeviceGroupList::RemoveAll()
{
	for (int i = 0; i < m_pDevGroupList->GetCount(); i++)
	{
		delete m_pDevGroupList->GetAt(i);
	}
	m_pDevGroupList->RemoveAll();
}

UINT CDeviceGroupList::GetCount()
{
	if (NULL != m_pDevGroupList)
		return m_pDevGroupList->GetCount();
	return 0;
}
CDeviceGroup* CDeviceGroupList::GetAt(UINT idx)
{
	if ((NULL != m_pDevGroupList) && (idx < m_pDevGroupList->GetCount()))
		return m_pDevGroupList->GetAt(idx);
	return NULL;
}

CString CDeviceGroupList::GetHubName(CString& name)
{
	if (NULL != m_pDevGroupList)
	{
		for (int i = 0; i < m_pDevGroupList->GetCount(); i++)
		{
			if (0 == m_pDevGroupList->GetAt(i)->GetNodeName().CompareNoCase(name))
				return  m_pDevGroupList->GetAt(i)->GetHubName();
		}
	}
	return _T("");
}


CDeviceGroup* CDeviceGroupList::operator [](int idx)
{
	if (idx >= m_pDevGroupList->GetCount())
		return NULL;

	return m_pDevGroupList->GetAt(idx);
}
