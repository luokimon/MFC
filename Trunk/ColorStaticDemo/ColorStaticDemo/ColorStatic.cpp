#include "stdafx.h"
#include "ColorStatic.h"


CColorStatic::CColorStatic()
{
	m_bFillBackColor = FALSE;
	m_Font = NULL;
	m_DisBackColor = RGB(200, 200, 200);
	SetBackColor(GetSysColor(COLOR_3DFACE));
	SetTextColor(0);
}


CColorStatic::~CColorStatic()
{
	if (m_Font)
	{
		m_Font->DeleteObject();
		delete m_Font;
		m_Font = NULL;
	}

	m_BackBrush.DeleteObject();
	m_DisBackBrush.DeleteObject();
}

void CColorStatic::SetFont(LOGFONT* logFont, BOOL bRedraw)
{
	if (m_Font)
		m_Font->DeleteObject();

	if (NULL == m_Font)
		m_Font = new CFont();

	if (m_Font)
	{
		if (!m_Font->CreatePointFontIndirect(logFont))
		{
			delete m_Font;
			m_Font = NULL;
		}
	}

	if (bRedraw&&GetSafeHwnd())
		Invalidate();
}

void CColorStatic::SetFont(CFont* pFont, BOOL bRedraw)
{
	LOGFONT logFont;
	pFont->GetLogFont(&logFont);
	SetFont(&logFont, bRedraw);
}

void CColorStatic::SetFont(UINT nHeight, LPCTSTR fontName, BOOL bRedraw)
{
	if (m_Font)
		m_Font->DeleteObject();

	if (NULL == m_Font)
		m_Font = new CFont();

	if (m_Font)
	{
		if (!m_Font->CreatePointFont(nHeight, fontName))
		{
			delete m_Font;
			m_Font = NULL;
		}
	}

	if (bRedraw&&GetSafeHwnd())
		Invalidate();
}

void CColorStatic::SetBackColor(COLORREF cr)
{
	m_BackColor = cr;
	m_bFillBackColor = TRUE;

	m_BackBrush.DeleteObject();
	m_BackBrush.CreateSolidBrush(m_BackColor);

	m_DisBackBrush.DeleteObject();
	m_DisBackBrush.CreateSolidBrush(m_DisBackColor);

	if (GetSafeHwnd())
		Invalidate();
}

void CColorStatic::SetTextColor(COLORREF cr)
{
	m_TextColor = cr;
	if (GetSafeHwnd())
		Invalidate();
}

BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_TextColor);

	if (m_Font)
		pDC->SelectObject(m_Font);

	HBRUSH hBrush = (HBRUSH)m_BackBrush;
	if (!IsWindowEnabled())
		hBrush = (HBRUSH)m_DisBackBrush;

	return (hBrush);

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	//return NULL;
}


BOOL CColorStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CStatic::OnEraseBkgnd(pDC);
}
