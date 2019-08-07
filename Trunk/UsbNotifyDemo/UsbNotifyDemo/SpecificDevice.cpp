#include "stdafx.h"
#include "SpecificDevice.h"


CSpecificDevice::CSpecificDevice()
	:m_pDevInfo(NULL), m_pBridgeInfo(NULL)
{
}

CSpecificDevice::CSpecificDevice(CString name)
	: CSpecificDevice()
{
	m_Name = name;
	m_ParentName = _T("");
}

CSpecificDevice::~CSpecificDevice()
{
	if (NULL != m_pDevInfo)
	{
		delete m_pDevInfo;
		m_pDevInfo = NULL;
	}

	if (NULL != m_pBridgeInfo)
	{
		delete m_pBridgeInfo;
		m_pBridgeInfo = NULL;
	}
}


void CSpecificDevice::operator = (const CSpecificDevice& o)
{
	if (this != &o)
	{
		m_pDevInfo = o.m_pDevInfo;
		m_pBridgeInfo = o.m_pBridgeInfo;
		m_Name = o.m_Name;
		m_ParentName = o.m_ParentName;
	}
}