#include "stdafx.h"

CDeviceBase::CDeviceBase()
	:m_nItem(0),
	m_pObject(NULL),
	m_pCallback(NULL)
{
	m_strName = NULL;
	m_hDisk = INVALID_HANDLE_VALUE;
}

CDeviceBase::~CDeviceBase()
{
	CloseDeviceHandle();
	delete m_strName;
}

void CDeviceBase::Free(CDeviceBase* base)
{
	if (base != NULL)
	{
		base->CloseDeviceHandle();
		delete base;
		base = NULL;
	}
}

BOOL CDeviceBase::OpenDeviceHandle()
{
	CloseDeviceHandle();

	if (m_strName->IsEmpty())
		return FALSE;

	for (int i = 0; i < CREATE_FILE_RETRY; i++)
	{
		m_hDisk = CreateFile(m_strName->GetBuffer(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			0, OPEN_EXISTING,
			0, 0);

		if (m_hDisk != INVALID_HANDLE_VALUE)
			break;

		Sleep(200);
	}

	return (m_hDisk != INVALID_HANDLE_VALUE);
}

void CDeviceBase::CloseDeviceHandle()
{
	if (m_hDisk != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDisk);
		m_hDisk = INVALID_HANDLE_VALUE;
	}
}

VOID CDeviceBase::SetCallback(PVOID obj, TaskCallBack callback, UINT start, UINT stop)
{
	m_pObject = obj;
	m_pCallback = callback;
	m_nStartPos = start;
	m_nStopPos = stop;
}

VOID CDeviceBase::SetTaskPosition(UINT pos)
{
	if ((NULL == m_pCallback) || (NULL == m_pObject))
		return;

	if ((0 == m_nStartPos) && (0 == m_nStopPos))
		m_pCallback(m_pObject, m_nItem, 50);
	else
		m_pCallback(m_pObject, m_nItem, m_nStartPos + ((m_nStopPos - m_nStartPos) * pos) / 100);
}

