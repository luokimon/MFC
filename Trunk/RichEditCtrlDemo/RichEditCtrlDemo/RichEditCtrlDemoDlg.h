
// RichEditCtrlDemoDlg.h : header file
//

#pragma once


// CRichEditCtrlDemoDlg dialog
class CRichEditCtrlDemoDlg : public CDialogEx
{
// Construction
public:
	CRichEditCtrlDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RICHEDITCTRLDEMO_DIALOG };
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
	CRichEditCtrlEx m_reLog;
	afx_msg void OnBnClickedButtonAdd();

private:
	UINT idx = 0;
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
};
