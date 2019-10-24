
// MultiDocumentDemoView.cpp : implementation of the CMultiDocumentDemoView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MultiDocumentDemo.h"
#endif

#include "MultiDocumentDemoDoc.h"
#include "MultiDocumentDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiDocumentDemoView

IMPLEMENT_DYNCREATE(CMultiDocumentDemoView, CView)

BEGIN_MESSAGE_MAP(CMultiDocumentDemoView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMultiDocumentDemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMultiDocumentDemoView construction/destruction

CMultiDocumentDemoView::CMultiDocumentDemoView() noexcept
{
	// TODO: add construction code here

}

CMultiDocumentDemoView::~CMultiDocumentDemoView()
{
}

BOOL CMultiDocumentDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMultiDocumentDemoView drawing

void CMultiDocumentDemoView::OnDraw(CDC* /*pDC*/)
{
	CMultiDocumentDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMultiDocumentDemoView printing


void CMultiDocumentDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMultiDocumentDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMultiDocumentDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMultiDocumentDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMultiDocumentDemoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMultiDocumentDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMultiDocumentDemoView diagnostics

#ifdef _DEBUG
void CMultiDocumentDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CMultiDocumentDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMultiDocumentDemoDoc* CMultiDocumentDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMultiDocumentDemoDoc)));
	return (CMultiDocumentDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CMultiDocumentDemoView message handlers
