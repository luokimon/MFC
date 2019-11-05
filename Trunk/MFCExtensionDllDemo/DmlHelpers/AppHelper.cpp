#include "stdafx.h"
#include "AppHelper.h"


CAppHelper::CAppHelper()
{
}


CAppHelper::~CAppHelper()
{
}

CString CAppHelper::GetCurrentAppDirectory()
{
	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];

	_tsplitpath_s(GetCurrentAppPath(), drive, MAX_PATH, dir, MAX_PATH, NULL, 0, NULL, 0);

	CString str;
	str.Format(_T("%s%s"), drive, dir);
	return str;
}

CString CAppHelper::GetCurrentAppPath()
{
	TCHAR szFile[MAX_PATH];
	::GetModuleFileName(NULL, szFile, MAX_PATH);
	return szFile;
}

struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
} *lpTranslate;

CString CAppHelper::GetFileVersion()
{
	TCHAR  SubBlock[200];
	CString strVersion = _T("");
	UINT  cbTranslate = 0;
	PTCHAR lpBuffer = NULL;
	unsigned int dwBytes = 0;

	DWORD InfoSize = ::GetFileVersionInfoSize(GetCurrentAppPath(), NULL);
	if (0 != InfoSize)
	{
		std::unique_ptr<TCHAR[]> InfoBuf(new TCHAR[InfoSize]);
		::GetFileVersionInfo(GetCurrentAppPath(), 0, InfoSize, InfoBuf.get());
		::VerQueryValue(InfoBuf.get(), TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate);

		for (UINT i = 0; i < (cbTranslate / sizeof(struct LANGANDCODEPAGE)); i++)
		{
			wsprintf(SubBlock, _T("\\StringFileInfo\\%04x%04x\\FileVersion"), lpTranslate[i].wLanguage, lpTranslate[i].wCodePage);
			::VerQueryValue(InfoBuf.get(), SubBlock, (LPVOID *)&lpBuffer, &dwBytes);
			strVersion += lpBuffer;
		}
	}

	return strVersion;
}