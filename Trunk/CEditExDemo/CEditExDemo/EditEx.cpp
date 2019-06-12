#include "stdafx.h"
#include "EditEx.h"


CEditEx::CEditEx()
{
}


CEditEx::~CEditEx()
{
}
BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (m_editTyp)
	{
	case EDIT_TYPE::etHex:
		if ((nChar >= '0' && nChar <= '9') ||
			(nChar >= 'a' && nChar <= 'f') ||
			(nChar >= 'A' && nChar <= 'F') ||
			nChar == VK_BACK ||
			nChar == VK_DELETE)
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		break;
	case EDIT_TYPE::etDec:
		if ((nChar >= '0' && nChar <= '9') ||
			nChar == VK_BACK ||
			nChar == VK_DELETE)
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		break;
	case EDIT_TYPE::etOct:
	case EDIT_TYPE::etBin:
	default:
		AfxMessageBox(_T("CEditEx don't support this type!"));
		break;
	}

}
