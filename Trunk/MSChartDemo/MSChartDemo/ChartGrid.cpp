#include "stdafx.h"
#include "ChartGrid.h"
#include "ChartAxis.h"


CChartGrid::CChartGrid()
{
	m_GridColor = RGB(128, 128, 128);
	m_pParentCtrl = NULL;
	m_bIsVisible = true;
	m_bIsHorizontal = true;
}


CChartGrid::~CChartGrid()
{
}

void CChartGrid::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;

	if (!pDC->GetSafeHdc())
		return;

	CRect plottingRect = m_pParentCtrl->GetPlottingRect();
	pDC->IntersectClipRect(plottingRect);

	CPen* pOldPen;
	CPen NewPen(PS_SOLID, 0, m_GridColor);
	pOldPen = pDC->SelectObject(&NewPen);

	std::list<int>::iterator iter = m_lstTickPos.begin();
	int ActuPosition = 0;

	for (iter; iter != m_lstTickPos.end(); iter++)
	{
		ActuPosition = *iter;
		if (m_bIsHorizontal)
		{
			int ActuY = plottingRect.bottom;
			while (ActuY > plottingRect.top)
			{
				pDC->MoveTo(ActuPosition, ActuY);
				ActuY -= 3;
				pDC->LineTo(ActuPosition, ActuY);
				ActuY -= 3;
			}
		}
		else
		{
			int ActuX = plottingRect.left;
			while (ActuX < plottingRect.right)
			{
				pDC->MoveTo(ActuX, ActuPosition);
				ActuX += 3;
				pDC->LineTo(ActuX, ActuPosition);
				ActuX += 3;
			}
		}
	}


	pDC->SelectClipRgn(NULL);
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

void CChartGrid::AddTick(int Pos)
{
	m_lstTickPos.push_back(Pos);
}

void CChartGrid::ClearTicks()
{
	m_lstTickPos.clear();
}

void CChartGrid::SetVisible(bool bVisible)
{
	m_bIsVisible = bVisible;
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

void CChartGrid::SetColor(COLORREF NewColor)
{
	m_GridColor = NewColor;
	if (m_pParentCtrl)
		m_pParentCtrl->RefreshCtrl();
}

