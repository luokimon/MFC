#pragma once
class CAppHelper
{
public:
	CAppHelper();
	virtual ~CAppHelper();
public:
	static CString GetCurrentAppDirectory();
	static CString GetCurrentAppPath();
	static CString GetFileVersion();
};

