#pragma once
#include "BaseLog.h"
class CHtmlLog :
	public CBaseLog
{
public:
	CHtmlLog();
	virtual ~CHtmlLog();

private:
	int m_cellWidth;
	int m_tblBorderThickness;
	CString m_htmlLog;

private:

	UINT ColorToHex(COLORREF color);

	void SetCellWidth(int width) { m_cellWidth = width; }
	void SetTableBorderThickness(int thickness) { m_tblBorderThickness = thickness; }

public:
	CString GetExtension() { return _T("html"); }
	void WriteLine(LPCTSTR str, COLORREF color = 0);
	void WriteData(LPCTSTR str, COLORREF color = 0);

	CString GetBodyStart();
	CString GetBodyEnd();

	void AddTableStart();
	void AddTableEnd();

	void AddTableRowStart();
	void AddTableRowEnd();

	void AddTableUnitData(CString& dat, COLORREF color = 0);

	PBYTE GetContent();
	UINT GetLength();
};

