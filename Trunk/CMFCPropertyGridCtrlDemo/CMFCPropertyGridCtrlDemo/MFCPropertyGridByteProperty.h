#pragma once
#include <afxpropertygridctrl.h>
class CMFCPropertyGridByteProperty :
	public CMFCPropertyGridProperty
{
public:
	//CMFCPropertyGridByteProperty();
	virtual ~CMFCPropertyGridByteProperty();

	CMFCPropertyGridByteProperty(const CString& strName, const CString& strValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	//virtual CString FormatProperty();
};

