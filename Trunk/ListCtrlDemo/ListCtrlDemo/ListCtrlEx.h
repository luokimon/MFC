#pragma once
#include <afxcmn.h>
class CListCtrlEx :
	public CListCtrl
{
public:
	CListCtrlEx();
	virtual ~CListCtrlEx();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	CArray<CProgressCtrl*, CProgressCtrl*> m_ProgressList;
	int m_ProgressColumn;
	int m_Data;

private:
	void InitProgressColumn(int ColNum);

public:
	void Initial();
	BOOL SetTypeText(int nItem, const CString& str);
	BOOL SetVersionText(int nItem, const CString& str);
	BOOL SetStatusText(int nItem, const CString& str);
	BOOL SetProgressText(int nItem, const CString& str);
	BOOL Append();
};

