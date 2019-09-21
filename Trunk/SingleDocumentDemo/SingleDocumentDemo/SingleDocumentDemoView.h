
// SingleDocumentDemoView.h : interface of the CSingleDocumentDemoView class
//

#pragma once


class CSingleDocumentDemoView : public CView
{
protected: // create from serialization only
	CSingleDocumentDemoView() noexcept;
	DECLARE_DYNCREATE(CSingleDocumentDemoView)

// Attributes
public:
	CSingleDocumentDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSingleDocumentDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SingleDocumentDemoView.cpp
inline CSingleDocumentDemoDoc* CSingleDocumentDemoView::GetDocument() const
   { return reinterpret_cast<CSingleDocumentDemoDoc*>(m_pDocument); }
#endif

