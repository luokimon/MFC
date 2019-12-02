#include "stdafx.h"
#include "ChartAxis.h"
#include "ChartGrid.h"


CChartAxis::CChartAxis()
{
	m_bIsVisible = true;
	m_nMaxValue = 0;
	m_nUnzoomMax = 0;
	m_nMinValue = 0;
	m_nUnzoomMin = 0;
	m_nFontSize = 80;
	m_strFontName = _T("Microsoft Sans Serif");
	m_bAutoTicks = true;
	m_iMarginSize = 0;
	m_StartPos = 0;
	m_EndPos = 0;
	m_dTickIncrement = 1.0;
	m_dFirstTickValue = 0;

	m_pAxisGrid = new CChartGrid();
}


CChartAxis::~CChartAxis()
{
	if (m_pAxisGrid)
	{
		delete m_pAxisGrid;
		m_pAxisGrid = NULL;
	}
}

void CChartAxis::SetAutomatic(bool bAutomatic)
{

}

void CChartAxis::SetParent(CChartCtrl* pParent)
{
	m_pParentCtrl = pParent;
	m_pAxisGrid->m_pParentCtrl = pParent;
	//m_pAxisLabel->m_pParentCtrl = pParent;
}

void CChartAxis::SetVisible(bool bVisible)
{
	m_bIsVisible = bVisible;
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetMinMax(INT min, INT max)
{
	ASSERT(max >= min);
	m_nMaxValue = m_nUnzoomMax = max;
	m_nMinValue = m_nUnzoomMin = min;
	m_pParentCtrl->RefreshCtrl();
}

void CChartAxis::SetHorizontal(bool bHorizontal)
{
	m_bIsHorizontal = bHorizontal;
	m_pAxisGrid->m_bIsHorizontal = bHorizontal;
}

void CChartAxis::Draw(CDC* pDC)
{
	if (!m_bIsVisible)
		return;

	if (NULL == pDC->GetSafeHdc())
		return;

	CPen SolidPen(PS_SOLID, 0, RGB(0, 0, 0));
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize, m_strFontName, pDC);
	CPen* pOldPen = pDC->SelectObject(&SolidPen);
	CFont* pOldFont = pDC->SelectObject(&NewFont);
	COLORREF OldTextColor = pDC->SetTextColor(0);
	int iPrevMode = pDC->SetBkMode(TRANSPARENT);

	int Pos = 0;
	if (m_bIsHorizontal)
	{
		Pos = m_AxisRect.top + 1;
		pDC->MoveTo(m_StartPos, Pos);
		pDC->LineTo(m_EndPos, Pos);
	}
	else
	{
		Pos = m_AxisRect.right - 1;
		pDC->MoveTo(Pos, m_StartPos);
		pDC->LineTo(Pos, m_EndPos);
	}

	m_pAxisGrid->ClearTicks();

	double TickValue = GetFirstTickValue();
	do
	{
		DrawTick(pDC, TickValue);
	} while (GetNextTickValue(TickValue, TickValue));

	m_pAxisGrid->Draw(pDC);

	pDC->SelectObject(pOldPen);
	SolidPen.DeleteObject();
	pDC->SelectObject(pOldFont);
	NewFont.DeleteObject();
	pDC->SetTextColor(OldTextColor);
	pDC->SetBkMode(iPrevMode);
}

int CChartAxis::GetTickPos(double TickVal) const
{
	int Offset = 0;
	int retVal = 0;

	Offset = (TickVal - m_nMinValue)*(m_EndPos - m_StartPos) / (m_nMaxValue - m_nMinValue);
	if(m_bIsHorizontal)
		retVal = m_StartPos + Offset;
	else
		retVal = (m_EndPos - Offset);
	return retVal;
}

void CChartAxis::DrawTick(CDC* pDC, double dTickVal)
{
	int TickPos = GetTickPos(dTickVal);
	CString strBuffer = GetTickLabel(dTickVal);
	CSize TextSize = pDC->GetTextExtent(strBuffer);

	if (m_bIsHorizontal)
	{
		pDC->MoveTo(TickPos, m_AxisRect.top + 1);
		pDC->LineTo(TickPos, m_AxisRect.top + 4);
		pDC->ExtTextOut(TickPos - TextSize.cx / 2, m_AxisRect.top + 5,
			ETO_CLIPPED | ETO_OPAQUE, NULL, strBuffer, NULL);
	}
	else
	{
		pDC->MoveTo(m_AxisRect.right - 1, TickPos);
		pDC->LineTo(m_AxisRect.right - 4, TickPos);
		pDC->ExtTextOut(m_AxisRect.left + 4, TickPos - TextSize.cy / 2,
			ETO_CLIPPED | ETO_OPAQUE, NULL, strBuffer, NULL);
	}

	m_pAxisGrid->AddTick(TickPos);
}

void CChartAxis::SetAxisSize(const CRect& ControlRect,
							const CRect& MarginRect)
{
	if (m_bIsHorizontal)
	{
		m_StartPos = MarginRect.left;
		m_EndPos = MarginRect.right;
		m_AxisRect = ControlRect;
		m_AxisRect.top = MarginRect.bottom;
	}
	else
	{
		m_StartPos = MarginRect.top; 
		m_EndPos = MarginRect.bottom;
		m_AxisRect = ControlRect;
		m_AxisRect.right = MarginRect.left;
	}
}

void CChartAxis::Recalculate()
{
	if (m_bAutoTicks)
		RefreshTickIncrement();

	RefreshFirstTick();
}

int CChartAxis::ClipMargin(CRect ControlRect, CRect& MarginRect, CDC* pDC)
{
	if (!m_bIsVisible)
		return 0;

	int Size = 0;
	CSize TickSize = GetLargestTick(pDC);

	if (m_bIsHorizontal)
	{
		Size += 4 + 2;
		Size += TickSize.cy;
		m_iMarginSize = Size;
		ControlRect.bottom -= Size;
		ControlRect.right -= TickSize.cx / 2 + 3;
		if (ControlRect.bottom < MarginRect.bottom)
			MarginRect.bottom = ControlRect.bottom;
		if (ControlRect.right < MarginRect.right)
			MarginRect.right = ControlRect.right;
	}
	else
	{
		Size += 7 + 1;
		Size += TickSize.cx;
		m_iMarginSize = Size;
		ControlRect.left += Size;
		ControlRect.top += TickSize.cy / 2 + 3;

		if (ControlRect.top > MarginRect.top)
			MarginRect.top = ControlRect.top;
		if (ControlRect.left > MarginRect.left)
			MarginRect.left = ControlRect.left;
	}

	return Size;
}

CSize CChartAxis::GetLargestTick(CDC* pDC)
{
	CFont NewFont;
	NewFont.CreatePointFont(m_nFontSize, m_strFontName, pDC);
	CFont* pOldFont = pDC->SelectObject(&NewFont);

	CSize MaxSize(0, 0);

	double TickValue = GetFirstTickValue();
	do
	{
		CString strLabel = GetTickLabel(TickValue);
		CSize TextSize = pDC->GetTextExtent(strLabel, strLabel.GetLength());
		if (TextSize.cy > MaxSize.cy)
			MaxSize.cy = TextSize.cy;
		if (TextSize.cx > MaxSize.cx)
			MaxSize.cx = TextSize.cx;

	} while (GetNextTickValue(TickValue, TickValue));

	pDC->SelectObject(pOldFont);
	NewFont.DeleteObject();
	return MaxSize;
}

CString CChartAxis::GetTickLabel(double TickValue) const
{
	int val = TickValue;
	CString ssLabel;
	ssLabel.Format(_T("%d"), val);
	return ssLabel;
}

double CChartAxis::GetFirstTickValue() const
{
	return m_dFirstTickValue;
}

bool CChartAxis::GetNextTickValue(double dCurrentTick, double& dNextTick) const
{
	if (0 == m_dTickIncrement)
		return false;

	dNextTick = dCurrentTick + m_dTickIncrement;

	return (dNextTick <= m_nMaxValue);
}

void CChartAxis::RefreshTickIncrement()
{
	if (!m_bAutoTicks)
		return;

	if (m_nMaxValue == m_nMinValue)
	{
		m_dTickIncrement = 0;
		return;
	}

	int PixelSpace = m_bIsHorizontal ? 30 : 20;
	int MaxTickNumber = (int)fabs((m_EndPos - m_StartPos) / PixelSpace * 1.0);
	//double TempTickIncrement = (m_nMaxValue - m_nMinValue) / (double)MaxTickNumber;
	m_dTickIncrement = (m_nMaxValue - m_nMinValue) / (double)MaxTickNumber;
}

void CChartAxis::RefreshFirstTick()
{
	if (m_dTickIncrement != 0)
	{
		if (0 == m_nMinValue)
			m_dFirstTickValue = 0;
		else
		{
			m_dFirstTickValue = (int)(m_nMinValue / m_dTickIncrement)*m_dTickIncrement;
			while (m_dFirstTickValue < m_nMinValue)
				m_dFirstTickValue += m_dTickIncrement;
		}
	}
	else
	{
		m_dFirstTickValue = m_nMinValue;
	}
}