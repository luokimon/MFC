
// TreeCtrlDemoDlg.h : header file
//

#pragma once


// CTreeCtrlDemoDlg dialog
class CTreeCtrlDemoDlg : public CDialogEx
{
// Construction
public:
	CTreeCtrlDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TREECTRLDEMO_DIALOG };
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
	//CTreeCtrl m_trParameter;
	CTreeCtrlEx m_trParameter;
	afx_msg void OnBnClickedButtonAddChild();
	afx_msg void OnBnClickedButtonAddRoot();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnTvnEndlabeleditTreeParameter(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleteItem();
//	afx_msg void OnTvnBeginlabeleditTreeParameter(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMDblclkTreeParameter(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnTvnEndlabeleditTreeParameter(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
