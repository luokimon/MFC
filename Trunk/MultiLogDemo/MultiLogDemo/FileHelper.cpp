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

