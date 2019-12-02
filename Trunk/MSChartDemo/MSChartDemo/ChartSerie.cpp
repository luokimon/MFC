#include "stdafx.h"
#include "ChartSerie.h"


CChartSerie::CChartSerie(CChartCtrl* pParent)
{
	m_pParentCtrl = pParent;

	m_bIsVisible = true;
	m_SerieColor = RGB(0,0,0);

	m_bMouseClickNotifications = true;
	m_bMouseMoveNotifications = false;
}


CChartSerie::~CChartSerie()
{
}

void CChartSerie::EnableMouseNotifications(bool bClickEnabled, bool bMoveEnabled)
{
	m_bMouseClickNotifications = bClickEnabled;
	m_bMouseMoveNotifications = bMoveEnabled;
}