#include "stdafx.h"
#include "DeviceManager.h"


CDeviceManager::CDeviceManager()
{
	m_pDevGroupList = NULL;
}


CDeviceManager::~CDeviceManager()
{
	RemoveAll();

	if (NULL != m_pDevGroupList)
		delete m_pDevGroupList;
}

void CDeviceManager::Arrival(CString name)
{
	Remove(name);

	CSpecificDevice* pDev = new CSpecificDevice(name);
	pDev->DelegateInitialize();
	//m_devList.AddTail(pDev);
}

void CDeviceManager::Add(CSpecificDevice* dev)
{
	m_devList.AddTail(dev);
}

void CDeviceManager::Remove(CString name)
{
	POSITION pos, nextPos;

	pos = m_devList.GetHeadPosition();
	while (pos)
	{
		CSpecificDevice* dev = m_devList.GetAt(pos);
		nextPos = pos;
		m_devList.GetNext(nextPos);
		if (0 == dev->GetName().CompareNoCase(name))
		{
			delete dev;
			m_devList.RemoveAt(pos);
		}
		pos = nextPos;
	}
}

void CDeviceManager::RemoveAll()
{
	POSITION pos, nextPos;

	pos = m_devList.GetHeadPosition();
	while (pos)
	{
		CSpecificDevice* dev = m_devList.GetAt(pos);
		nextPos = pos;
		m_devList.GetNext(nextPos);

		delete dev;
		m_devList.RemoveAt(pos);

		pos = nextPos;
	}
}

void CDeviceManager::SetDeviceGroupList(CDeviceGroupList* list)
{
	if (NULL != m_pDevGroupList)
		delete m_pDevGroupList;

	m_pDevGroupList = list;
}