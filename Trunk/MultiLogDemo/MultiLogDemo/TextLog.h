#pragma once
#include "BaseLog.h"
class CTextLog :
	public CBaseLog
{
public:
	CTextLog();
	virtual ~CTextLog();

public:
	CString GetExtension() { return _T("html"); }
	void WriteLine(LPCTSTR str, COLORREF color = 0);
	void WriteData(LPCTSTR str, COLORREF color = 0);

	void AddTableRowEnd() { m_content += _T("\r\n"); }

	void AddTableUnitData(CString& dat, COLORREF color = 0);
};

