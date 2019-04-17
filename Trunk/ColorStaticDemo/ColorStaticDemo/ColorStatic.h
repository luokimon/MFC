#pragma once
#include <afxwin.h>
class CColorStatic :
	public CStatic
{
public:
	CColorStatic();
	virtual ~CColorStatic();
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	BOOL m_bFillBackColor;
	COLORREF m_DisBackColor;
	COLORREF m_BackColor;
	COLORREF m_TextColor;
	CFont*	m_Font;
	CBrush m_BackBrush;
	CBrush m_DisBackBrush;

public:
	CFont* GetFont() { return m_Font; }
	COLORREF GetBackColor() { return m_BackColor; }
	COLORREF GetTextColor() { return m_TextColor; }

	virtual void SetFont(LOGFONT* logFont, BOOL bRedraw = TRUE);
	virtual void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	virtual void SetFont(UINT nHeight, LPCTSTR fontName, BOOL bRedraw = TRUE);

	void SetBackColor(COLORREF cr);
	void SetTextColor(COLORREF cr);
};

