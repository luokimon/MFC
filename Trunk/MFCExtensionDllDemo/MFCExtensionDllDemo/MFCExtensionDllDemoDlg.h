
// MFCExtensionDllDemoDlg.h : header file
//

#pragma once


// CMFCExtensionDllDemoDlg dialog
class CMFCExtensionDllDemoDlg : public CDialogEx
{
// Construction
public:
	CMFCExtensionDllDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCEXTENSIONDLLDEMO_DIALOG };
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
	CDeviceNotify m_devNotify;
	CDeviceEnumeration m_devEnumeration;

private:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void DeviceArrival(const CString& name);
	void DeviceRemove(const CString& name);
	void TestDmlHelper();
public:
	afx_msg LRESULT OnDeviceGroupUpdate(WPARAM wParam, LPARAM lParam);
};
