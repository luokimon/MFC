#include "stdafx.h"
#include "FileHelper.h"


CFileHelper::CFileHelper()
{
}


CFileHelper::~CFileHelper()
{
}

BOOL CFileHelper::IsFileExist(const CString& csFile)
{
	DWORD dwAttrib = GetFileAttributes(csFile);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

VOID CFileHelper::CreateDataLog(PBYTE buff, DWORD len, const CString& name)
{
	CString  strPath;
	SHGetSpecialFolderPath(0, strPath.GetBufferSetLength(MAX_PATH), CSIDL_DESKTOPDIRECTORY, 0);
	strPath.ReleaseBuffer();
	strPath.Format(_T("%s\\%s.bin"), strPath, name);
	CFile file;
	file.Open(strPath, CFile::modeCreate | CFile::modeWrite);
	file.Write(buff, len);
	file.Flush();
	file.Close();
}

BOOL CFileHelper::CompareExtentionNoCase(const CString& csFile, const CString& csExt)
{
	TCHAR ext[MAX_PATH];
	_tsplitpath_s(csFile, NULL, 0, NULL, 0, NULL, 0, ext, MAX_PATH);
	return (0 == csExt.CompareNoCase(ext));
}

BOOL CFileHelper::CreateMultipleDirectory(const CString& szPath)
{
	CString strDir(szPath);
	if (strDir.GetAt(strDir.GetLength() - 1) != _T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}
	std::vector<CString> vPath;
	CString strTemp;
	bool bSuccess = false;

	for (int i = 0; i < strDir.GetLength(); ++i)
	{
		if (strDir.GetAt(i) != _T('\\'))
		{                               
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else
		{                               
			vPath.push_back(strTemp);    
			strTemp.AppendChar(_T('\\'));
		}
	}

	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++)
	{
		if (!PathIsDirectory(*vIter))
		{
			bSuccess = CreateDirectory(*vIter, NULL) ? true : false;
		}
	}

	return bSuccess;
}