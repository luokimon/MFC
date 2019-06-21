
// HexToBinDemoDlg.h : header file
//

#pragma once


// CHexToBinDemoDlg dialog
class CHexToBinDemoDlg : public CDialogEx
{
// Construction
public:
	CHexToBinDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HEXTOBINDEMO_DIALOG };
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
	CString m_strFilePath;
public:
	afx_msg void OnBnClickedButtonFileSelect();
	afx_msg void OnBnClickedButtonConvert();
};
