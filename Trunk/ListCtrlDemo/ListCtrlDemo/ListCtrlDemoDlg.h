
// ListCtrlDemoDlg.h : header file
//

#pragma once
#include "ListCtrlEx.h"

// CListCtrlDemoDlg dialog
class CListCtrlDemoDlg : public CDialogEx
{
// Construction
public:
	CListCtrlDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTCTRLDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_ProgressCtrl;
	//CListCtrlEx m_ProgressCtrl;
};
