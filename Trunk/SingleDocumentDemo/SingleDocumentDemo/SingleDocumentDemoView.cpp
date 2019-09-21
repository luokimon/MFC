
// SingleDocumentDemoView.cpp : implementation of the CSingleDocumentDemoView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SingleDocumentDemo.h"
#endif

#include "SingleDocumentDemoDoc.h"
#include "SingleDocumentDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSingleDocumentDemoView

IMPLEMENT_DYNCREATE(CSingleDocumentDemoView, CView)

BEGIN_MESSAGE_MAP(CSingleDocumentDemoView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSingleDocumentDemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSingleDocumentDemoView construction/destruction

CSingleDocumentDemoView::CSingleDocumentDemoView() noexcept
{
	// TODO: add construction code here

}

CSingleDocumentDemoView::~CSingleDocumentDemoView()
{
}

BOOL CSingleDocumentDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSingleDocumentDemoView drawing

void CSingleDocumentDemoView::OnDraw(CDC* /*pDC*/)
{
	CSingleDocumentDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSingleDocumentDemoView printing


void CSingleDocumentDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSingleDocumentDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSingleDocumentDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSingleDocumentDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSingleDocumentDemoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSingleDocumentDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSingleDocumentDemoView diagnostics

#ifdef _DEBUG
void CSingleDocumentDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CSingleDocumentDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSingleDocumentDemoDoc* CSingleDocumentDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSingleDocumentDemoDoc)));
	return (CSingleDocumentDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CSingleDocumentDemoView message handlers
