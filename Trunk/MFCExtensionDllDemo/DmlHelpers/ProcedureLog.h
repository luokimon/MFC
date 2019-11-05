#pragma once

class AFX_EXT_CLASS CProcedureLog
{
public:
	CProcedureLog();
	virtual ~CProcedureLog();

private:
	//CString m_logPath;
	TCHAR	m_logPath[MAX_PATH];

private:
	void LogV(const TCHAR *pszFmt, va_list argp);

public:
	void SetLogPath(const CString& str);
	void Log(const TCHAR* pszFmt, ...);
	void Clear();
};

