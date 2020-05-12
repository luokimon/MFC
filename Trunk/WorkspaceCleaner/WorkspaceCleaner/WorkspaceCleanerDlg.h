
// WorkspaceCleanerDlg.h : header file
//

#pragma once

#define COLOR_RED RGB(255,0,0) 
#define COLOR_BLUE RGB(0,0,255) 
#define COLOR_GREEN RGB(0,255,0) 
#define COLOR_YELLOW RGB(255,128,0) 
#define COLOR_DialogBK RGB(240,240,240) 
#define COLOR_BLACK RGB(0,0,0) 


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
	CStringArray m_CleanFolderArray;
	CStringArray m_CleanFileArray;
private:
	void ParsingFilter();
	void LoadSetting();
	void SaveSetting();
	void Clean(const CString& strPath);
	void CleanFile(const CString& path);
	void RecursiveDelete(const CString& szPath);
	void AdjustLayout();
	//void AddLog(const CString& str, COLORREF color = COLOR_BLACK);
public:
	CButton m_ckbIntermediateFiles;
	CButton m_ckbOutputFiles;
	CButton m_ckbOtherFiles;
	CButton m_ckbOtherFolders;
	CEdit m_edtCleanFilter;
	CEdit m_edtPath;
	afx_msg void OnClose();
	CRichEditCtrlEx m_reLog;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
