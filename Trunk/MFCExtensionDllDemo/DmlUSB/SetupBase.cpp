#include "stdafx.h"
#include "SetupBase.h"


CSetupBase::CSetupBase()
{
	m_pBuff = NULL;
	ZeroMemory(&pack, sizeof(pack));
	SetRequestType(0x40);
}


CSetupBase::~CSetupBase()
{
	if (NULL != m_pBuff)
	{
		delete[] m_pBuff;
		m_pBuff = NULL;
	}
}

void CSetupBase::SetLength(UINT len)
{
	pack.Length = len;
	if (NULL != m_pBuff)
	{
		delete m_pBuff;
		m_pBuff = NULL;
	}

	if (len > 0)
	{
		m_pBuff = new BYTE[len];
		ZeroMemory(m_pBuff, len);
	}
}
