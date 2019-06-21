
// SerialCommunicationDemoDlg.h : header file
//

#pragma once


// CSerialCommunicationDemoDlg dialog
class CSerialCommunicationDemoDlg : public CDialogEx
{
// Construction
public:
	CSerialCommunicationDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALCOMMUNICATIONDEMO_DIALOG };
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
	COCX m_ctrlComm;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
};
