#pragma once

#include <list>
#include "ChartCtrl.h"

class CChartAxis;

class CChartGrid
{
	friend CChartAxis;

public:
	void SetVisible(bool bVisible);
	bool IsVisible() const { return m_bIsVisible; }
	void SetColor(COLORREF NewColor);
	COLORREF GetColor() const { return m_GridColor; }
private:
	CChartCtrl* m_pParentCtrl;
	bool m_bIsVisible;
	COLORREF m_GridColor;
	bool m_bIsHorizontal;
	std::list<int> m_lstTickPos;
private:
	CChartGrid();
	virtual ~CChartGrid();
	void Draw(CDC* pDC);
	void AddTick(int Pos);
	void ClearTicks();
};

