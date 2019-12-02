#pragma once

class CChartCtrl;
class CChartGrid;

class CChartAxis
{
	friend CChartCtrl;

public:
	CChartAxis();
	virtual ~CChartAxis();

private:
	INT m_nMaxValue;
	INT m_nUnzoomMax;
	INT m_nMinValue;
	INT m_nUnzoomMin;
	int  m_nFontSize;
	int m_iMarginSize;
	CString m_strFontName;
	double m_dTickIncrement;
	double m_dFirstTickValue;
	CChartGrid* m_pAxisGrid;
private:
	void SetParent(CChartCtrl* pParent);
	CSize GetLargestTick(CDC* pDC);
	void Draw(CDC* pDC);
	void SetHorizontal(bool bHorizontal);
	void SetAxisSize(const CRect& ControlRect, const CRect& MarginRect);
	void RefreshTickIncrement();
	void Recalculate();
	int ClipMargin(CRect ControlRect, CRect& MarginRect, CDC* pDC);
	void RefreshFirstTick();
	double GetFirstTickValue() const;
	bool GetNextTickValue(double nCurrentTick, double& nNextTick) const;
	CString GetTickLabel(double TickValue) const;
	void DrawTick(CDC* pDC, double dTickVal);
	int GetTickPos(double TickVal) const;
protected:
	bool m_bIsAutoMode;
	bool m_bIsVisible;
	bool m_bIsHorizontal;
	CChartCtrl* m_pParentCtrl;
	bool m_bAutoTicks;
	int m_StartPos;
	int m_EndPos;
	CRect m_AxisRect;
public:
	void SetAutomatic(bool bAutomatic);
	bool IsAutomatic() { return m_bIsAutoMode; }
	void SetVisible(bool bVisible);
	bool IsVisible()  const { return m_bIsVisible; }
	void SetMinMax(INT min, INT max);
	INT GetMin() { return m_nMinValue; }
	INT GetMax() { return m_nMaxValue; }
};

