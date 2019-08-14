
// UsbNotifyDemoDlg.h : header file
//

#pragma once


// CUsbNotifyDemoDlg dialog
class CUsbNotifyDemoDlg : public CDialogEx
{
// Construction
public:
	CUsbNotifyDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBNOTIFYDEMO_DIALOG };
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
	CDeviceManager m_devManager;
	CDeviceGroupList* m_pDevGroupList;
private:
	void VariableInitialization();
	BOOL RegisterNotification();
	void InitialEnumeration();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg LRESULT OnDeviceGroupUpdate(WPARAM wParam, LPARAM lParam);
};
