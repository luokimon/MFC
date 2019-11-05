#pragma once
class AFX_EXT_CLASS CAppHelper
{
public:
	CAppHelper();
	virtual ~CAppHelper();

public:
	static CString GetCurrentAppDirectory();
	static CString GetCurrentAppPath();
	static CString GetFileVersion();
};

