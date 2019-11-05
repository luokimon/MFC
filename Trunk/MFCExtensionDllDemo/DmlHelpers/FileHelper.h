#pragma once
class AFX_EXT_CLASS CFileHelper
{
public:
	CFileHelper();
	virtual ~CFileHelper();

public:
	static BOOL IsFileExist(const CString& csFile);
	static BOOL CompareExtentionNoCase(const CString& csFile, const CString& csExt);
	static VOID CreateDataLog(PBYTE buff, DWORD len, const CString& name);
	static BOOL CreateMultipleDirectory(const CString& path);
};

