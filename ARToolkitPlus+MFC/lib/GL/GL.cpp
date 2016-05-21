// GL.cpp : implementation file
//

#include "stdafx.h"
#include "GL.h"


// CGL

IMPLEMENT_DYNAMIC(CGL, CWnd)

CGL::CGL()
{

	tcnt	= 0;
	view_mode		= GL_FILL;//;

	bShowVert		= FALSE;

	pCamera	= &m_camera;

	bShowDebugs = 2;


	mpt = CPoint(0,0);

}

CGL::~CGL()
{
}



void CGL::Create(CRect rc,CWnd *pWnd)//|WS_BORDER
{
	CWnd::Create( NULL,"",WS_CHILD|WS_VISIBLE,rc,pWnd,0);
}


BEGIN_MESSAGE_MAP(CGL, CWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CGL message handlers



void CGL::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		ProcAtEveryFrame();
		Invalidate();
	

		tcnt++;

	}
	CWnd::OnTimer(nIDEvent);
}

int CGL::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hDC	= ::GetDC( m_hWnd );

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		32,								// Want 24bit color 
		8,16,8,8,8,0,					// Not used to select mode
		0,0,							// Not used to select mode
		64,16,16,16,0,						// Not used to select mode
		32,								// Size of depth buffer
		8,								// Not used to select mode
		0,								// Not used to select mode
		PFD_MAIN_PLANE,					// Draw in main plane
		0,								// Not used to select mode
		0,0,0 };						// Not used to select mode

	int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	VERIFY(SetPixelFormat(m_hDC, nPixelFormat, &pfd));
	m_hRC = wglCreateContext(m_hDC);

	InitValues();
	CreateObjects();

	
	SetTimer(1,10,NULL);

	t0 = m_tm.End();
	return 0;
}



void CGL::SwitchViewMode()
{
	//view_mode ^=1; 

	if(view_mode ==GL_FILL)
		view_mode = GL_LINE;

	else view_mode = GL_FILL;


}

void CGL::ToggleDispVertex()
{
	bShowVert ^=1;
}


void CGL::ToggleShowDebugs()
{
	//bShowDebugs ^= 1;

	bShowDebugs = ++bShowDebugs %4;

}

void CGL::DrawLayer0()
{
	GetCursorPos(&mpt);
	ScreenToClient(&mpt);

}

void CGL::DrawLayer1()
{
	

}
void CGL::Draw()
{

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	


	// camera
	{
		pCamera->ApplyProjectionTransform();
		pCamera->ApplyModelViewTransform2();
	
	}

	// object
	{
		glNormal3f(1,0,0);
		glColor4f(1,1,1,1);
		glPolygonMode(GL_FRONT_AND_BACK, view_mode);
		GenTex();

		{
			glPushMatrix();
			DrawLayer0();
			glPopMatrix();
		}
		{
			
			glPushMatrix();
			DrawLayer1();
			glPopMatrix();
		}
	}

	glFinish();
}


void CGL::OnMouseMove(UINT nFlags, CPoint point)
{
	
	if (pCamera->MouseMove(nFlags,point))
	{
		Invalidate();
		
	}

	CWnd::OnMouseMove(nFlags, point);
}

/*
void CGL::TouchesBegan(CPoint pt)
{
	pCamera->TouchesBegan(dhVector(pt.x, pt.y, 0));

}

void CGL::TouchesEnded(CPoint pt)	
{
	pCamera->TouchesEnded(dhVector(pt.x, pt.y, 0));

}
	*/



void CGL::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	VERIFY(wglMakeCurrent(m_hDC,m_hRC));
	
	{
		pCamera->SetCamera(cx, cy);
		
	}

	{
		// Lights
		m_light.SetDefaultMaterial();
		//m_light.MakeFog();
	}	
}

void CGL::OnDestroy()
{
	CWnd::OnDestroy();

	//KillTimer(1);
	Dbg("CGL::OnDestroy()");
}

void CGL::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	wglMakeCurrent( m_hDC,m_hRC);
	Draw();				
	SwapBuffers( m_hDC);
	wglMakeCurrent( m_hDC,NULL);

}

void CGL::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	//ProcKey(nChar);
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CGL::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	
	if(pMsg->message==WM_MOUSEMOVE)
    {

		//if( (pMsg->wParam& MK_LBUTTON) == MK_LBUTTON)
		if( (pMsg->wParam& MK_LBUTTON))
		{

			;
		}
		

	}
	else if(pMsg->message==WM_KEYDOWN)
	{
		ProcKey(pMsg->wParam);
		return 0;
		
	}
	

	/*
	else if(pMsg->message==WM_LBUTTONDOWN)
	{
		
		TouchesBegan(pMsg->pt);
	}
	else if(pMsg->message==WM_LBUTTONUP)
	{
		
		TouchesEnded(pMsg->pt);
	}
	*/
	return CWnd::PreTranslateMessage(pMsg);
}
