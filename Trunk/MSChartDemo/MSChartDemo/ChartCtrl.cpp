#include "stdafx.h"
#include "ChartCtrl.h"

#define CHARTCTRL_CLASSNAME    _T("ChartCtrl")

CChartCtrl::CChartCtrl()
{
	RegisterWndClass();
	m_bMemDCCreated = FALSE;
	m_BackColor = GetSysColor(COLOR_BTNFACE);
	m_BorderColor = RGB(0, 0, 0);

	m_nColumnNum = 20;
	m_nRowNum = 9;
}


CChartCtrl::~CChartCtrl()
{
}
BEGIN_MESSAGE_MAP(CChartCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CChartCtrl::RegisterWndClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if (!(::GetClassInfo(hInst, CHARTCTRL_CLASSNAME, &wndcls)))
	{
		memset(&wndcls, 0, sizeof(WNDCLASS));

		wndcls.hInstance = hInst;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.hCursor = NULL;
		wndcls.hIcon = 0;
		wndcls.lpszMenuName = NULL;
		wndcls.hbrBackground = (HBRUSH) ::GetStockObject(WHITE_BRUSH);
		wndcls.style = CS_DBLCLKS;
		wndcls.cbClsExtra = 0;
		wndcls.cbWndExtra = 0;
		wndcls.lpszClassName = CHARTCTRL_CLASSNAME;

		if (!RegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

void CChartCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CWnd::OnPaint() for painting messages
	if (!m_bMemDCCreated)
	{
		RefreshCtrl();
		m_bMemDCCreated = TRUE;
	}

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(),
		&m_BackgroundDC, 0, 0, SRCCOPY);

}

void CChartCtrl::RefreshCtrl()
{
	if (!GetSafeHwnd())
		return;


	CClientDC dc(this);
	CRect ClientRect;
	GetClientRect(&ClientRect);

	if (!m_BackgroundDC.GetSafeHdc())
	{
		CBitmap memBitmap;
		m_BackgroundDC.CreateCompatibleDC(&dc);
		memBitmap.CreateCompatibleBitmap(&dc, ClientRect.Width(), ClientRect.Height());
		m_BackgroundDC.SelectObject(&memBitmap);
	}

	DrawBackground(&m_BackgroundDC, ClientRect);
	ClientRect.DeflateRect(3, 3);
	DrawChart(&m_BackgroundDC, ClientRect);
	DrawGrid(&m_BackgroundDC, ClientRect);
	DrawData(&m_BackgroundDC, ClientRect);
	Invalidate();
}

void CChartCtrl::DrawChart(CDC* pDC, CRect ChartRect)
{
	m_PlottingRect = ChartRect;
	CPen SolidPen(PS_SOLID, 0, m_BorderColor);
	CPen* pOldPen = pDC->SelectObject(&SolidPen);

	pDC->MoveTo(m_PlottingRect.left, m_PlottingRect.top);
	pDC->LineTo(m_PlottingRect.right, m_PlottingRect.top);
	pDC->LineTo(m_PlottingRect.right, m_PlottingRect.bottom);
	pDC->LineTo(m_PlottingRect.left, m_PlottingRect.bottom);
	pDC->LineTo(m_PlottingRect.left, m_PlottingRect.top);



	pDC->SelectObject(pOldPen);
	DeleteObject(SolidPen);
}

void CChartCtrl::DrawBackground(CDC* pDC, CRect ChartRect)
{
	CBrush BrushBack;
	BrushBack.CreateSolidBrush(m_BackColor);
	pDC->SetBkColor(m_BackColor);
	pDC->FillRect(ChartRect, &BrushBack);
	//pDC->DrawEdge(ChartRect, EDGE_RAISED, BF_RECT);
}

void CChartCtrl::DrawGrid(CDC* pDC, CRect ChartRect)
{
	m_PlottingRect = ChartRect;
	CPen SolidPen(PS_DOT, 0, m_BorderColor);
	CPen* pOldPen = pDC->SelectObject(&SolidPen);


	UINT startX = 0;
	UINT startY = 0;
	UINT endX = 0;
	UINT endY = 0;
	if (m_nColumnNum > 1)
	{
		//double unitX = (double)m_PlottingRect.Width() / (m_nColumnNum - 1);
		m_dColumnUnit = (double)m_PlottingRect.Width() / (m_nColumnNum - 1);
		
		for (int i = 1; i < m_nColumnNum; i++)
		{
			startX = m_PlottingRect.top + (UINT)(i * m_dColumnUnit);
			startY = 0;
			endX = startX;
			endY = m_PlottingRect.Height();

			pDC->MoveTo(startX, startY);
			pDC->LineTo(endX, endY);
		}
	}

	if (m_nRowNum > 1)
	{
		m_dRowUnit = (double)m_PlottingRect.Height() / (m_nRowNum - 1);
		for (int i = 0; i < m_nRowNum - 1; i++)
		{
			startX = 0;
			startY = m_PlottingRect.top + (UINT)(i * m_dRowUnit);
			endX = m_PlottingRect.Width();
			endY = startY;

			pDC->MoveTo(startX, startY);
			pDC->LineTo(endX, endY);
		}
	}

	pDC->SelectObject(pOldPen);
	DeleteObject(SolidPen);
}

void CChartCtrl::DrawData(CDC* pDC, CRect ChartRect)
{
	CFont font;
	CFont *pOldFont;

	font.CreateFont(
		(m_dRowUnit*4)/5, 
		((m_dColumnUnit*4)/5)/5, 
		0,                         
		0,                         
		FW_NORMAL,
		FALSE,                     
		FALSE,                     
		0,                         
		ANSI_CHARSET,              
		OUT_DEFAULT_PRECIS,        
		CLIP_DEFAULT_PRECIS,       
		DEFAULT_QUALITY,           
		DEFAULT_PITCH | FF_SWISS,  
		_T("ËÎÌו"));                 
	pOldFont = pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);

	CPen pen(PS_SOLID, 1, RGB(0, 128, 255));
	CPen *pOldPen = pDC->SelectObject(&pen);

	CSize sz = pDC->GetTextExtent(_T("12345"));

	pDC->TextOut(ChartRect.left+(m_dColumnUnit-sz.cx)/2, ChartRect.top + (m_dRowUnit - sz.cy) / 2, _T("12345"));

	pDC->SelectObject(pOldPen);
	DeleteObject(font);
	pDC->SelectObject(pOldFont);
	DeleteObject(pen);
}

void CChartCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_BackgroundDC.GetSafeHdc())
		m_BackgroundDC.DeleteDC();

	RefreshCtrl();
}

BOOL CChartCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//return CWnd::OnEraseBkgnd(pDC);
	return FALSE;
}


void CChartCtrl::SetUnit(UINT row, UINT col)
{
	UINT m_nColumnNum = col;
	UINT m_nRowNum = row;
}