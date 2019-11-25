#pragma once
#include <afxwin.h>
class CDataGridEx :
	public CWnd
{
public:
	CDataGridEx();
	virtual ~CDataGridEx();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

