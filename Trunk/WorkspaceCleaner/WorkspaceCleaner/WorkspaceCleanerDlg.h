
// WorkspaceCleanerDlg.h : header file
//

#pragma once


// CWorkspaceCleanerDlg dialog
class CWorkspaceCleanerDlg : public CDialogEx
{
// Construction
public:
	CWorkspaceCleanerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORKSPACECLEANER_DIALOG };
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPath();
	CButton m_ckbReleaseFolder;
	CButton m_ckbDebugFolder;
	CButton m_ckbKeepEmptyFolder;
	afx_msg void OnBnClickedButtonStart();
private:
	CString m_strSelectedFolder;
};
