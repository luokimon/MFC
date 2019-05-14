#pragma once

class CFileHelper
{
public:
	CFileHelper();
	virtual ~CFileHelper();

public:
	static BOOL IsFileExist(const CString& csFile);
	static VOID CreateDataLog(PBYTE buff, DWORD len, const CString& name);
};

