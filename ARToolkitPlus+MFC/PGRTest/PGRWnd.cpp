// PGRWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PGRTest.h"
#include "PGRWnd.h"


// PGRWnd

IMPLEMENT_DYNAMIC(PGRWnd, CWnd)

PGRWnd::PGRWnd()
{
	bCap = FALSE;

}

PGRWnd::~PGRWnd()
{
}



void PGRWnd::Create(CRect rc, CWnd *pWnd)
{
	CWnd::Create( NULL,"",WS_CHILD|WS_VISIBLE|WS_BORDER,rc,pWnd,0);//|WS_VISIBLE|WS_BORDER

}

void PGRWnd::InitCam()
{

	mPgrCam.setup(1280,1024);
	bCap = TRUE;
	SetTimer(0,10,NULL);
}

void PGRWnd::GetBuf(unsigned  char* p)
{
	if(IsCapturing())
	{
		
		p = (unsigned  char*) mPgrCam.mCamImage->imageData;
		printf("%d", p[0]);
	}

}

void PGRWnd::Run()
{
	if(IsCapturing())
	{


		mPgrCam.update();
		//Invalidate();

	}

}


BEGIN_MESSAGE_MAP(PGRWnd, CWnd)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// PGRWnd message handlers




void PGRWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==0)
	{
		
		Run();
	}
	CWnd::OnTimer(nIDEvent);
}


void PGRWnd::OnDestroy()
{
	CWnd::OnDestroy();

	//KillTimer(0);
}


void PGRWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	//printf("ab");

	if(IsCapturing())
	{

		//mPgrCam.draw(dc);
		// REMOVED 
	}
	
}
