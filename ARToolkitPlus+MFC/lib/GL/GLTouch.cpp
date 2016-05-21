
// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST


#include "StdAfx.h"
#include "GLTouch.h"


CGLTouch *pGL; 

#include "../../PGRTest/PGRTestDoc.h"
#include "../../PGRTest/PGRTestView.h"



extern CPGRTestView *pv;


CGLTouch::CGLTouch(void)
{
	
}

CGLTouch::~CGLTouch(void)
{
	Close();
}


void CGLTouch::InitValues()
{
	//nTouchFaceID = -1;
	pGL = this;
	

	bPaintSelectedFace	= 0;
	bInterpolateNormal	= FALSE;

	bShowTexColorInArrow	= FALSE;

	bUseSingleColor = FALSE;

//	pFX = NULL;

	bShowGrid = TRUE;

	memset(pr_gl, 0, 16*sizeof(float));
	memset(pr_gl2, 0, 16*sizeof(float));

	displayImageCorners.push_back(ofPoint(0, 0));
	displayImageCorners.push_back(ofPoint(640, 0));
	displayImageCorners.push_back(ofPoint(640, 480));
	displayImageCorners.push_back(ofPoint(0, 480));	


}



void CGLTouch::Close()
{


	
}
void CGLTouch::LoadObject(int modelID)
{
	
	/*
	if(modelID==0)
	{
		m_obj7.LoadObj("../../../_Resource/obj/barrel_triangle.obj"); //("../../_Resource/assem1-test2.obj"); 
		//assem1-test
		//Body_assem_1
		//Body_assem_2p
		float g= 1;
		m_obj7.Scale(dhVector(g,g,g));
	}
	

	//dbg2

	vwObj *pObj = &m_obj7;
	CString buf;

	int nv = pObj->verts.GetSize();
	int nf = pObj->faces.GetSize();
	*/
	/*
	buf.Format("Vert : %d", nv);
	pv->dbg2(buf);

	buf.Format("Faces : %d", nf);
	pv->dbg2(buf);
	*/
}


void CGLTouch::CreateObjects()
{
	
	if(0)
	{
		m_img1.Make(8*3,6.4*3, 1);
		m_img1.SetUV(800,640);
		m_img1.ReloadTexture(0,"../_image/bg_frog_800x640.png");
	}
	else
	{
		m_img1.Make(6.4*3,4.8*3, 1);
		m_img1.SetUV(640,480);
		m_img1.ReloadTexture(0,"../_image/bg_frog_640x480.png");

	}
	
	SetCameraPos(dhVector(-9.6,-7.2, -26.9));
	//SetCameraPos(dhVector(-10,-7,0));
	
}

dhVector prev_vNorm(-1,-1,-1);
void CGLTouch::ProcAtEveryFrame()
{
	
	unsigned char *p1 = NULL;
	BaseCV *pCV= &pv->m_wnd2.m_cv;

	if(pCV->GetCount()>0)
	{
		Mat img1 = pCV->frame;
		p1 = (BYTE*) img1.data;
	}

	if(p1)
	{
		//printf("%d", p1[0]);
		m_img1.LoadTextureFromBuffer(0, p1, 640,480,3, TRUE, TRUE);//800,640, 3);
	}
	else
		return;

	/*
	if(pv->m_wnd2.m_cv.tcnt0 >0)
		p1 = (BYTE*) pv->m_wnd2.m_cv.frameg->imageData;

	if(p1)
	{
		//printf("%d", p1[0]);
		m_img1.LoadTextureFromBuffer(0, p1, 640,480,3, TRUE);//800,640, 3);
	}
	else
		return;

	pArtk = &pv->m_wnd2.artk;


	int nDetected = pArtk->getNumDetectedMarkers();
	CString buf;
	buf.Format("# of markers :", nDetected);


	*/

	//GetBuf
	/*
	if(tcnt%100==0)
		this->SetFocus();

		*/
}


// Camera stuff
void CGLTouch::DrawLayer0()
{
	CGL::DrawLayer0();

	//m_obj7.Draw(FALSE, FALSE, bShowVert);
	
	if(bShowGrid)	DrawGrid(30,1);
	
	dhMat mm= pCamera->mat_mm;
	pv->DispMatrix2(mm.v);

	//AR DBG
	BaseCV *pCV= &pv->m_wnd2.m_cv;

	if(pCV->GetCount()<1)
	{
		return;
		
		//printf("artk.draw");
	}
	

	if(0)
	{

		pv->m_edit_ARDBG.Reset();

		CString buf;
		buf.Format("%.2f, %.2f", pArtk->markerpos[0], pArtk->markerpos[1]);
		pv->m_edit_ARDBG.PutLine(buf);

		buf.Format("%.2f, %.2f", pArtk->markerpos[0]*3 / 100.0, pArtk->markerpos[1]*3 / 100.0);
		pv->m_edit_ARDBG.PutLine(buf);
	}

	if(tcnt %50 ==0)
	{
		
		if(pCamera->IsViewChanged())
		{
			Dbg("ViewChanged");
			
			//pv->DispMatrix2((float*) pCamera->GetMM());
			//DispMatrices(pCamera->GetMM(), pCamera->GetPM(), pCamera->GetVP());

		}

	}

	


	//glTranslatef(8,10,0);
	//if(pFX)
	//pFX->Draw();

}


// AR Stuff
void CGLTouch::DrawLayer1()
{
//	vwObj *p	= &m_obj7;

	vwImageObj *p	= &m_img1;
	p->Draw(TRUE);


	if(pv->m_wnd2.m_cv.tcnt0<1)
	{
		return;
		
		//printf("artk.draw");
	}

	// ARTK 3D stuff
	pArtk = &pv->m_wnd2.artk;
	pArtk->draw(0,0);
	float sf = pArtk->sf;
	int i,j;
	int myIndex1 = pArtk->getMarkerIndex(0);

	if(myIndex1>=0) 
	{
		ofMatrix4x4 m1 = pArtk->getMatrix(myIndex1);

		for(j=0; j<4; j++)
		{
					 ;
			// ofMatrix4x4				openGL
			// [0]  [1]  [2]  [3]		[0] [4] [8]  [12]
			// [4]  [5]  [6]  [7]		[1] [5] [9]  [13]
			// [8]  [9]  [10] [11]		[2] [6] [10] [14]
			// [12] [13] [14] [15]		[3] [7] [11] [15]

			pr_gl[4*0+ j] = m1(j,0);
			pr_gl[4*1+ j] = m1(j,1);
			pr_gl[4*2+ j] = m1(j,2);
			pr_gl[4*3+ j] = m1(j,3);

			//string str2;
			//str2 = ofToString(m1(j,0), 2)+","+ofToString(m1(j,1),2)+","+ofToString(m1(j,2), 2)+","+ofToString(m1(j,3), 2);
			//ofDrawBitmapString(str2, 100, offset_info+1*50+ 20*j);

		}
		
		pv->DispMatrix1(pr_gl);
		//printf("%.2f, %.2f, %.2f\n", pr_gl[12], pr_gl[13], pr_gl[14]);
		//pr_gl[12] = sf;
		//pr_gl[13] = sf;
		//pr_gl[14] = sf;

		//glMatrixMode( GL_MODELVIEW );
		//glLoadMatrixf( pr_gl );

		//glScalef(sf,sf,sf);

		//float x1 = sf* (640/2 +pr_gl[12]);
		//float y1 = sf* (480/2 +pr_gl[13]);

		//glTranslatef(x1, y1, 0);
		//glBegin(GL_LINES);
        //glVertex3f(0, 0, 0);
        //glVertex3f(0, 0, 10);
        //glEnd();



	}
	else
		return;
	
	//glPushMatrix();
	pArtk->applyProjectionMatrix();

	//dhMat m1;
	//pArtk->GetProjectionMatrix(m1.v);
	//pv->DispMatrix2(m1.v);

	BOOL bShowDbg = TRUE;
	int numDetected = pArtk->getNumDetectedMarkers(	);
	
	// Draw for each marker discovered
	//sf = 0.531;
	//glScalef(sf, sf, sf);

	for(int i=0; i<numDetected; i++) {
		// Set the matrix to the perspective of this marker
		// The origin is in the middle of the marker	
		pArtk->applyModelMatrix(i);		


		
		// Draw a stack of rectangles by offseting on the z axis
        glLineWidth(1);

		//ofNoFill();
		//ofEnableSmoothing();
		//ofSetColor(255, 255, 0, 50);
       // ofSetColor(255, 255, 0, 50);
        //BOX
        glColor3f(0, 255, 255 );//ofSetColor(255, 0, 255 );
        //ofRect(-26, -26, 52, 52);
      
		//glTranslatef(10,7, 50.5);
       
        // Draw a line from the center out
        
        if(bShowDbg)
        {
            glPushMatrix();
			DrawARStuff();
			glPopMatrix();

           
        }
	}
   

	


	//glPopMatrix();
	
	
}


void CGLTouch::DrawARStuff()
{
	float gain_axis = 50;
	glLineWidth(5);


	glColor4f(255, 255, 0, 128);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, gain_axis);
	glEnd();



	glLineWidth(1);
	return;
	
  
	glColor3f(0, 0, 255);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, gain_axis);
	glEnd();

	glColor3f(0, 255, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, gain_axis, 0);
	glEnd();

	glColor3f(255, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(gain_axis, 0, 0);
	glEnd();

	glLineWidth(1);
}


void CGLTouch::GenTex()
{	

	m_img1.GenTextures();

	/*
	vwObj *pObj = &m_obj7;
	int ntex = pObj->GenTexture();

	CString buf;
	if(ntex>0)
	{

		buf.Format("Texture files:#%d", ntex);
		//pv->dbg3(buf);
		//pv->dbg3("=========");
		int i;
		for(i=0; i<ntex; i++)
		{
			CString fname = pObj->matArr.GetAt(i).map_Kd;
			//fname = CFileName::GetFileNameS(fname);
			//pv->dbg3(fname);
		}
		
	}

	*/

	/*
	int texp = pFX->GenTexture();
	if(texp>0) 
	{
		
		OutputDebugString(DDFORMAT("Particle TEx  :#%d", texp));

		pv->dbg3(DDFORMAT("Particle TEx  :#%d", texp));
	}
	*/

}


void CGLTouch::ToggleShowGrid()
{
	bShowGrid ^=1;

}



void CGLTouch::TogglePaintSelectedFace()
{

//	bPaintSelectedFace ^=1;

		bPaintSelectedFace = 	++bPaintSelectedFace%3;

}

void CGLTouch::ToggleInterpolateNormal()
{

	bInterpolateNormal ^=1;
}


	

void CGLTouch::ToggleUseSingleColor()
{
	bUseSingleColor ^= 1;
	//m_obj7.bUseSingleColor = bUseSingleColor;
}


void CGLTouch::ToggleShowTexColorInArrow()
{
	bShowTexColorInArrow ^=1;
}


void CGLTouch::DispMatrices(double *mm,double *pm, int* vp)
{
	int i;
	CString buf;


	buf.Format("GL_MODELVIEW_MATRIX");
	//pTargetWnd->m_edit.Reset();
	//pv->dbg4(buf, TRUE);

	for(i=0; i<4; i++)
	{

		buf.Format("%.2f %.2f %.2f %.2f", mm[4*0+i], mm[4*1+i], mm[4*2+i], mm[4*3+i]);
		//pv->dbg4(buf);
	}

	
}



void CGLTouch::ProcKey(UINT nChar)
{
//	vwObj* p = &m_obj7;
//	dhVector o = p->o;

	//219 for [
	//221 for ]

	int key = nChar;

	if(key==VK_F1)
	{
		
;
	}

}


