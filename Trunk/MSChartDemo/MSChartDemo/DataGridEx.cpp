#include "stdafx.h"
#include "DataGridEx.h"


CDataGridEx::CDataGridEx()
{
}


CDataGridEx::~CDataGridEx()
{
}
BEGIN_MESSAGE_MAP(CDataGridEx, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CDataGridEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages

	TRACE(_T("CDataGridEx::OnPaint\r\n"));
}


void CDataGridEx::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	TRACE(_T("CDataGridEx::OnSize\r\n"));
}
