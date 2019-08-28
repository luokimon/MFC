#include "stdafx.h"
#include "MFCPropertyGridByteProperty.h"


//CMFCPropertyGridByteProperty::CMFCPropertyGridByteProperty()
//{
//}


CMFCPropertyGridByteProperty::~CMFCPropertyGridByteProperty()
{
}


CMFCPropertyGridByteProperty::CMFCPropertyGridByteProperty(const CString& strName, const CString& strValue, LPCTSTR lpszDescr, DWORD dwData)
	:CMFCPropertyGridProperty(strName, (LPCTSTR)strValue, lpszDescr, dwData)
{

}


CWnd* CMFCPropertyGridByteProperty::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
	CMFCMaskedEdit *pEdit = new CMFCMaskedEdit();

	pEdit->EnableMask(_T("  AA"), _T("0x__"));
	//pEdit->EnableMask(_T("  AAAA"), _T("0x____"));
	//pEdit->EnableMask(_T("  AAAAAAAA"), _T("0x________"));
	pEdit->SetValidChars(_T("1234567890ABCDEFabcdef"));
	DWORD dwStyle = WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL;
	pEdit->Create(dwStyle, rectEdit, m_pWndList, AFX_PROPLIST_ID_INPLACE);

	return pEdit;
}