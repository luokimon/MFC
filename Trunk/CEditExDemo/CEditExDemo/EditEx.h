#pragma once
#include <afxwin.h>

enum EDIT_TYPE
{
	etHex,
	etDec,
	etOct,
	etBin,
};

class CEditEx :
	public CEdit
{
public:
	CEditEx();
	virtual ~CEditEx();
	DECLARE_MESSAGE_MAP()

private:
	EDIT_TYPE m_editTyp;

public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void SetEditType(EDIT_TYPE typ) { m_editTyp = typ; }
};

