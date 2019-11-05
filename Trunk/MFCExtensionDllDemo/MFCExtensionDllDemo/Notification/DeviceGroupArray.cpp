#include "stdafx.h"
#include "../stdafx.h"
#include "DeviceGroupArray.h"

void CDeviceGroup::operator = (const CDeviceGroup& o)
{
	if (this != &o)
	{
		m_nodeName = o.m_nodeName;
		m_hubName = o.m_hubName;
	}
}

CDeviceGroupArray::CDeviceGroupArray()
{
	m_pDevGroupArray = new CArray<CDeviceGroup*, CDeviceGroup*>();
}


CDeviceGroupArray::~CDeviceGroupArray()
{
	for (int i = 0; i < m_pDevGroupArray->GetCount(); i++)
	{
		delete m_pDevGroupArray->GetAt(i);
	}
	m_pDevGroupArray->RemoveAll();
	delete m_pDevGroupArray;
}

void CDeviceGroupArray::Add(LPCTSTR name, LPCTSTR parent)
{
	CDeviceGroup* grp = new CDeviceGroup(name, parent);
	m_pDevGroupArray->Add(grp);
}

CString CDeviceGroupArray::GetHubName(CString& name)
{
	if (NULL != m_pDevGroupArray)
	{
		for (int i = 0; i < m_pDevGroupArray->GetCount(); i++)
		{
			if (0 == m_pDevGroupArray->GetAt(i)->GetNodeName().CompareNoCase(name))
				return  m_pDevGroupArray->GetAt(i)->GetHubName();
		}
	}
	return _T("");
}