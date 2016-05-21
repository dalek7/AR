

// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST
// PGRTestView.cpp : implementation of the CPGRTestView class

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PGRTest.h"
#endif

#include "PGRTestDoc.h"
#include "PGRTestView.h"

CPGRTestView *pv;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPGRTestView

IMPLEMENT_DYNCREATE(CPGRTestView, CFormView)

BEGIN_MESSAGE_MAP(CPGRTestView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON2, &CPGRTestView::OnInitCamLogitech)
	

	
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
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
	DDX_Control(pDX, IDC_EDIT1, m_edit_ar1);
	DDX_Control(pDX, IDC_EDIT2, m_edit_ARDBG);
	DDX_Control(pDX, IDC_CHECK1, m_chk_dbg1);
	DDX_Control(pDX, IDC_CHECK2, m_chk_disp_binary);
	DDX_Control(pDX, IDC_SLIDER1, m_sld_threshold);
	DDX_Control(pDX, IDC_STATIC1, m_info_slder1);
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


	int w = 640;//800; 
	int h = 480;//640;

	CRect viewSize1 = CRect(0,0,w,h)+CPoint(10,10);

	m_gl.Create(viewSize1,this);
	m_wnd2.Create(CRect(0,0,1,1),this);

	RedirectIOToConsole();
	
	m_sld_threshold.SetRange(0,255);
	m_sld_threshold.SetPos(54);
	
	pv = this;



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
void CPGRTestView::DispMatrix1(float* p1)
{
	m_edit_ar1.Reset();
	m_edit_ar1.PutLine("AR #0");

	CString buf;

	for(int i=0; i<4; i++)
	{

		buf.Format("%.2f, %.2f, %.2f, %.2f", p1[0+i], p1[4+i], p1[8+i], p1[12+i]); 
		m_edit_ar1.PutLine(buf);

	}
	


}
void CPGRTestView::DispMatrix2(float* p1)
{
	m_edit_ARDBG.Reset();
	m_edit_ARDBG.PutLine("Proj.");

	CString buf;

	for(int i=0; i<4; i++)
	{

		buf.Format("%.2f, %.2f, %.2f, %.2f", p1[0+i], p1[4+i], p1[8+i], p1[12+i]); 
		m_edit_ARDBG.PutLine(buf);

	}
	


}



void CPGRTestView::OnInitCamLogitech()
{

	m_wnd2.Init();
	/*
	if(!m_wnd1.IsCapturing())
	{
		m_wnd1.InitCam();
		
	}
	else
	{
		m_wnd1.StopCam();
		
	}

	*/
}




void CPGRTestView::OnDestroy()
{

	


	CFormView::OnDestroy();

	
}


void CPGRTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnTimer(nIDEvent);
}


void CPGRTestView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	
	// Get the current position of scroll box.
	//int curpos = GetScrollPos(SB_HORZ);
	//if(m_sld_threshold.GetPos() == nPos)
	int n = m_sld_threshold.GetPos();
	m_info_slder1.SetValued(n);



	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
