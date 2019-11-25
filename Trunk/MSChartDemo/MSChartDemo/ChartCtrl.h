#pragma once
#include <afxwin.h>
class CChartCtrl : public CWnd
{
public:
	CChartCtrl();
	virtual ~CChartCtrl();

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

private:
	void RefreshCtrl();
	void DrawChart(CDC* pDC, CRect ChartRect);
	void DrawBackground(CDC* pDC, CRect ChartRect);
	void DrawGrid(CDC* pDC, CRect ChartRect);
	void DrawData(CDC* pDC, CRect ChartRect);
	BOOL RegisterWndClass();

public:
	void SetUnit(UINT row, UINT col);
};
