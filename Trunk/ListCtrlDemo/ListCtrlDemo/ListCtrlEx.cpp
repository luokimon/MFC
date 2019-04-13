#include "stdafx.h"
#include "ListCtrlEx.h"

CListCtrlEx::CListCtrlEx()
{
}


CListCtrlEx::~CListCtrlEx()
{
}
BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CListCtrlEx::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	int Top = GetTopIndex();
	int Total = GetItemCount();
	int PerPage = GetCountPerPage();
	int LastItem = ((Top + PerPage) > Total) ? Total : Top + PerPage;

	// if the count in the list os nut zero delete all the progress controls and them procede
	{
		int Count = (int)m_ProgressList.GetCount();
		for (int i = 0; i < Count; i++)
		{
			CProgressCtrl* pControl = m_ProgressList.GetAt(0);
			pControl->DestroyWindow();
			m_ProgressList.RemoveAt(0);
		}
	}

	CHeaderCtrl* pHeader = GetHeaderCtrl();
	for (int i = Top; i < LastItem; i++)
	{
		CRect ColRt;
		pHeader->GetItemRect(m_ProgressColumn, &ColRt);
		// get the rect
		CRect rt;
		GetItemRect(i, &rt, LVIR_LABEL);
		rt.top += 1;
		rt.bottom -= 1;
		rt.left += ColRt.left;
		int Width = ColRt.Width();
		rt.right = rt.left + Width - 4;
		/*
		  rt.left=ColRt.left+1;
		  rt.right=ColRt.right-1;
		*/

		// create the progress control and set their position
		CProgressCtrl* pControl = new CProgressCtrl();
		pControl->Create(NULL, rt, this, IDC_PROGRESS_LIST + i);

		CString Data = GetItemText(i, 4);
		int Percent = _ttoi(Data);

		// set the position on the control
		pControl->SetPos(Percent);
		pControl->ShowWindow(SW_SHOWNORMAL);
		// add them to the list
		m_ProgressList.Add(pControl);
	}
	CListCtrl::OnPaint();

	// Do not call CListCtrl::OnPaint() for painting messages
}

void CListCtrlEx::InitProgressColumn(int ColNum)
{
	m_ProgressColumn = ColNum;
}

void CListCtrlEx::Initial()
{
	DWORD dwStyle = GetExtendedStyle();
	SetExtendedStyle(dwStyle | LVS_EX_GRIDLINES);

	InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	InsertColumn(1, _T("Type"), LVCFMT_CENTER, 100);
	InsertColumn(2, _T("Version"), LVCFMT_CENTER, 100);
	InsertColumn(3, _T("Status"), LVCFMT_CENTER, 100);
	InsertColumn(4, _T("Progress"), LVCFMT_CENTER, 100);
	InitProgressColumn(4);
}

BOOL CListCtrlEx::SetTypeText(int nItem, const CString& str)
{
	return SetItemText(nItem, 1, str);
}

BOOL CListCtrlEx::SetVersionText(int nItem, const CString& str)
{
	return SetItemText(nItem, 2, str);
}

BOOL CListCtrlEx::SetStatusText(int nItem, const CString& str)
{
	return SetItemText(nItem, 3, str);
}

BOOL CListCtrlEx::SetProgressText(int nItem, const CString& str)
{
	return SetItemText(nItem, 4, str);
}

BOOL CListCtrlEx::Append()
{
	return InsertItem(GetItemCount(), _T(""));
}
