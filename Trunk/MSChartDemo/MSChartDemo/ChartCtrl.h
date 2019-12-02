#pragma once
#include <afxwin.h>

#include "ChartAxis.h"

class CChartCtrl : public CWnd
{
public:
	CChartCtrl();
	virtual ~CChartCtrl();

public:
	void SetUnit(UINT row, UINT col);
	void RefreshCtrl();

	
	enum EAxisPos
	{
		AxisLeft = 0,
		AxisBottom,
		AxisMax
	};
	
	CChartAxis* CreateAxis(EAxisPos pos);
	CChartAxis* GetLeftAxis() const;
	CChartAxis* GetBottomAxis() const;
	void EnableRefresh(bool bEnable);
	CRect GetPlottingRect() const { return m_PlottingRect; }

protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMemDCCreated;
	CDC m_BackgroundDC;
	CRect m_PlottingRect;
	COLORREF m_BackColor;
	COLORREF m_BorderColor;

	UINT m_nColumnNum;
	UINT m_nRowNum;
	double m_dColumnUnit;
	double m_dRowUnit;

	COLORREF** m_backColorMatrix;
	COLORREF** m_fontColorMatrix;

	CChartAxis* m_pAxes[2];

	int m_iEnableRefresh;
	bool m_bPendingRefresh;

private:
	void DrawChart(CDC* pDC, CRect ChartRect);
	void DrawBackground(CDC* pDC, CRect ChartRect);
	void DrawGrid(CDC* pDC, CRect ChartRect);
	void DrawData(CDC* pDC, CRect ChartRect);
	BOOL RegisterWndClass();


};
