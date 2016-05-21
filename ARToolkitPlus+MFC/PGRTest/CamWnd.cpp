// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST
// CamWnd.cpp : implementation file

#include "stdafx.h"
#include "PGRTest.h"
#include "CamWnd.h"


#include "PGRTestDoc.h"
#include "PGRTestView.h"

extern CPGRTestView *pv;

// CamWnd

IMPLEMENT_DYNAMIC(CamWnd, CWnd)

CamWnd::CamWnd()
{

}

CamWnd::~CamWnd()
{
}


void CamWnd::Create(CRect rc, CWnd *pWnd)
{
	width = rc.Width();
	height = rc.Height();
	CWnd::Create( NULL,"",WS_CHILD|WS_VISIBLE|WS_BORDER,rc,pWnd,0);//|WS_VISIBLE|WS_BORDER

}


void CamWnd::Init()
{
	artk.setup(640, 480);//width, height

	m_cv.InitCount();
	
	
	SetTimer(1,10,NULL);
}



void CamWnd::Run()
{
	
	#ifdef USE_FIREFLY
		RunFireFly();
		// REMOVED FOR A WHILE
	#else
		
		m_cv.capture.read(m_cv.frame); 
	#endif
		m_cv.InitTime();
	

	Proc();

	m_cv.IncreaseCount();

}


void CamWnd::Proc()
{

	cvtColor(m_cv.frame, m_cv.frameg, CV_BGR2GRAY);
	
	Mat im_bw;
	
	// then adjust the threshold to actually make it binary
	int nthres = pv->m_sld_threshold.GetPos();
	threshold(m_cv.frameg, im_bw, nthres, 255, CV_THRESH_BINARY);
	
	int w = im_bw.size().width;
	int h = im_bw.size().height;

	printf("%d %d\r", w, h);
	BYTE* pBuf = (BYTE*) im_bw.data;

	artk.update(pBuf);

	
	{
		float dt = m_cv.UpdateTime();
		
	
		if(pv->m_chk_dbg1.GetCheck())
		{
			char txt1[256];
			sprintf(txt1, "%d, %.1f fps", m_cv.tcnt0, 1.0/dt );

			putText(m_cv.frameg, txt1, cv::Point2d(10,30),1,1,Scalar(255,0,255));

			imshow("Cam-Grayscale", m_cv.frameg);
		}

		if(pv->m_chk_disp_binary.GetCheck())
		{

			char txt1[256];
			sprintf(txt1, "%d, %.1f fps", m_cv.tcnt0, 1.0/dt );

			putText(im_bw, txt1, cv::Point2d(10,30),1,1,Scalar(255,0,255));

		
			imshow("Cam-BW", im_bw);

		}
	}
}



BEGIN_MESSAGE_MAP(CamWnd, CWnd)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CamWnd message handlers




void CamWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==1)
	{

		Run();

	}

	CWnd::OnTimer(nIDEvent);
}


void CamWnd::OnDestroy()
{
	CWnd::OnDestroy();

	KillTimer(1);
	// TODO: Add your message handler code here
}
