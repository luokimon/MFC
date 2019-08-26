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
	void AddBlackText(const CString& str);
	void AddRedText(const CString& str);
	void AddGreenText(const CString& str);
	void AddBlueText(const CString& str);
	void SaveTo(const CString path);
	void LoadFrom(const CString path);
};

