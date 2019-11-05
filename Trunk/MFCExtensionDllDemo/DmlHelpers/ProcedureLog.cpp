#include "stdafx.h"
#include "ProcedureLog.h"


CProcedureLog::CProcedureLog()
{
	ZeroMemory(m_logPath, sizeof(m_logPath));
}


CProcedureLog::~CProcedureLog()
{
}

void CProcedureLog::LogV(const TCHAR *pszFmt, va_list argp)
{
	CString str;
#ifdef UNICODE
	_vsnwprintf_s(str.GetBufferSetLength(MAX_PATH), MAX_PATH, MAX_PATH-1,pszFmt, argp);
#else
	vsnprintf_s(str.GetBufferSetLength(MAX_PATH), MAX_PATH, MAX_PATH - 1, pszFmt, argp);
#endif
	str.ReleaseBuffer();
	
	CString path(m_logPath);
	if (path.IsEmpty())
		return;

	CFile file;
	if (CFileHelper::IsFileExist(m_logPath))
	{
		file.Open(m_logPath, CFile::modeReadWrite);
	}
	else
	{
		file.Open(m_logPath, CFile::modeCreate | CFile::modeReadWrite);
	}

	file.SeekToEnd();
	file.Write(str.GetBuffer(), str.GetLength());
	file.Flush();
	file.Close();
}

void CProcedureLog::Log(const TCHAR* pszFmt, ...)
{
	va_list argp;
	va_start(argp, pszFmt);
	LogV(pszFmt, argp);
	va_end(argp);
}

void CProcedureLog::SetLogPath(const CString& str)
{
	_tcscpy_s(m_logPath, MAX_PATH, str);
	//m_logPath = str;
}

void CProcedureLog::Clear()
{
	CFile file;
	if (CFileHelper::IsFileExist(m_logPath))
	{
		file.Open(m_logPath, CFile::modeCreate | CFile::modeReadWrite);
		file.Close();
	}
}