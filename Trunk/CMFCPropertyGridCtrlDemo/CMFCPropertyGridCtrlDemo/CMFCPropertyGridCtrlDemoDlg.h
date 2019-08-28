
// CMFCPropertyGridCtrlDemoDlg.h : header file
//

#pragma once


// CCMFCPropertyGridCtrlDemoDlg dialog
class CCMFCPropertyGridCtrlDemoDlg : public CDialogEx
{
// Construction
public:
	CCMFCPropertyGridCtrlDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMFCPROPERTYGRIDCTRLDEMO_DIALOG };
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
	CMFCPropertyGridCtrl m_propertyGrid;
	//CMFCMaskedEdit m_maskedEdit;
};
