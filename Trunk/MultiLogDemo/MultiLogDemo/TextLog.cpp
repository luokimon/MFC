#include "stdafx.h"
#include "TextLog.h"


CTextLog::CTextLog()
{
}


CTextLog::~CTextLog()
{
}

void CTextLog::WriteLine(LPCTSTR dat, COLORREF color)
{
	CString str;
	str.Format(_T("%s\r\n"), dat);

	m_content += str;
}

void CTextLog::WriteData(LPCTSTR dat, COLORREF color)
{
	m_content += dat;
}

void CTextLog::AddTableUnitData(CString& dat, COLORREF color)
{
	CString str;
	str.Format(_T("%s"), dat);
	m_content += str;
}