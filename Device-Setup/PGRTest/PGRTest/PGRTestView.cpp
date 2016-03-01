
// PGRTestView.cpp : implementation of the CPGRTestView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PGRTest.h"
#endif

#include "PGRTestDoc.h"
#include "PGRTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPGRTestView

IMPLEMENT_DYNCREATE(CPGRTestView, CFormView)

BEGIN_MESSAGE_MAP(CPGRTestView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &CPGRTestView::OnInitCam)
	
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CPGRTestView construction/destruction

CPGRTestView::CPGRTestView()
	: CFormView(CPGRTestView::IDD)
{
	// TODO: add construction code here

}

CPGRTestView::~CPGRTestView()
{
}

void CPGRTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CPGRTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CPGRTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_wnd1.Create(CRect(0,0,1024,768),this);
	RedirectIOToConsole();




}

void CPGRTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPGRTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPGRTestView diagnostics

#ifdef _DEBUG
void CPGRTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPGRTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPGRTestDoc* CPGRTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPGRTestDoc)));
	return (CPGRTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CPGRTestView message handlers


void CPGRTestView::OnInitCam()
{
	if(!m_wnd1.IsCapturing())
	{
		m_wnd1.InitCam();
		
	}
	else
	{
		m_wnd1.StopCam();
		
	}


}



void CPGRTestView::OnDestroy()
{

	m_wnd1.StopCam();


	CFormView::OnDestroy();

	
}
