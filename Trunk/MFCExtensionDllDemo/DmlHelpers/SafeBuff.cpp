#include "stdafx.h"
#include "SafeBuff.h"


CSafeBuff::CSafeBuff()
{
	m_buff = NULL;
	m_len = 0;
}

CSafeBuff::CSafeBuff(UINT len)
{
	m_buff = new BYTE[len];
	ZeroMemory(m_buff, len);
	m_len = len;
}

CSafeBuff::~CSafeBuff()
{
	ReleaseBuffer();
}

VOID CSafeBuff::ReleaseBuffer()
{
	if (NULL != m_buff)
	{
		delete[] m_buff;
		m_buff = NULL;
	}

	m_len = 0;
}

PBYTE CSafeBuff::GetBufferSetLength(UINT len)
{
	SetLength(len);
	return m_buff;
}

VOID CSafeBuff::SetLength(UINT len)
{
	ReleaseBuffer();

	m_buff = new BYTE[len];
	m_len = len;

	ZeroMemory(m_buff, len);
}
