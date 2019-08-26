#pragma once
#include <afxcmn.h>
class CRichEditCtrlEx :
	public CRichEditCtrl
{
public:
	CRichEditCtrlEx();
	virtual ~CRichEditCtrlEx();

private:
	void AddColorText(const CString& str, COLORREF col);

public:
	void AddNormalLog(const CString& str);
	void AddErrorLog(const CString& str);
	void AddPassLog(const CString& str);
	void SaveTo(const CString path);
	void LoadFrom(const CString path);
};

