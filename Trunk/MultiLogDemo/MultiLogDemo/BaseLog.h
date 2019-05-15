#pragma once
class CBaseLog
{
public:
	CBaseLog();
	virtual ~CBaseLog();

protected:
	CString m_content;

public:
	virtual CString GetExtension() = 0;

	virtual void WriteLine(LPCTSTR str, COLORREF color = 0) = 0;
	virtual void WriteData(LPCTSTR str, COLORREF color = 0) = 0;

	virtual void AddTableStart() {}
	virtual void AddTableEnd() {}

	virtual void AddTableRowStart() {}
	virtual void AddTableRowEnd() {}

	virtual void AddTableUnitData(CString& dat, COLORREF color = 0) = 0;

	virtual PBYTE GetContent() { return (PBYTE)m_content.GetBuffer(); }
	virtual UINT GetLength() { return m_content.GetLength(); }
};

