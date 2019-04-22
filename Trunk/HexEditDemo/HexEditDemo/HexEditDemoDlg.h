
// HexEditDemoDlg.h : header file
//

#pragma once

#include "HexEdit.h"


// CHexEditDemoDlg dialog
class CHexEditDemoDlg : public CDialogEx
{
// Construction
public:
	CHexEditDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEXEDITDEMO_DIALOG };
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

private:
	BYTE m_hexData[64];
public:
	CHexEdit m_hexEdit;
};
