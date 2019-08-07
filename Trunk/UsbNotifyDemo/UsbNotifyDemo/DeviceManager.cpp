#include "stdafx.h"
#include "DeviceManager.h"


CDeviceManager::CDeviceManager()
{
}


CDeviceManager::~CDeviceManager()
{
	RemoveAll();
}


void CDeviceManager::Add(CString name)
{
	Remove(name);

	CSpecificDevice* pDev = new CSpecificDevice(name);
	m_devList.AddTail(pDev);
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