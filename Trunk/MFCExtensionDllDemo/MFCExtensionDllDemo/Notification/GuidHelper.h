#pragma once
class CGuidHelper
{
public:
	CGuidHelper();
	virtual ~CGuidHelper();

public:
	static BOOL IsHostBoard(GUID guid);
	static GUID GetHostBoard();
	static CString GetHostBoardPrefix();
};

