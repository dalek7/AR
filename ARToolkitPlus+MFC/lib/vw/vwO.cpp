
/*
 *  vwO.h
 *
 *  Created by Seung-Chan Kim on 2/20/11.
 *  Copyright 2011 KAIST. All rights reserved.
 *  Special thanks to JY Yang, KAIST
 *
 */

#include "stdafx.h"
#include "vwO.h"


//#include "../../TouchObj/GL.h"
//extern CGL *pGL;
#include "../GL/GLTouch.h"
#include "../ddArray.h"
extern CGLTouch *pGL;


vwPolygon::vwPolygon()
{
	for (int i=0;i<3;i++)
		v[i]	= NULL;

	
}

void vwPolygon::Tri(dhVector *p,int a,int b,int c)
{
	v[0] = &p[a];
    v[1] = &p[b];
    v[2] = &p[c];
}

vwVertex::vwVertex()
{

}

vwVertex::~vwVertex()
{
/*
	for (int i=0;i<GetCount();i++)
	{
		vwPolygon *pObj = (vwPolygon*)Search(i);
		//delete pObj;
	}
*/
	RemoveAll();
}

void vwVertex::Add(vwPolygon *p)
{
	AddTail(p);
}



void vwVertex::Remove(vwPolygon *p)
{
	for (int i=0;i<GetCount();i++)
	{
		vwPolygon *pObj = (vwPolygon*)Search(i);
		if (pObj==p)	
		{
			//delete pObj;
			RemoveAt(i);
			return;
		}
	}
}

void vwVertex::Reset()
{
	RemoveAll();
}



vwO::vwO()
{
	pVer	= NULL;
	pVer0 = NULL;
	pFace	= NULL;
	//nVer	= NULL;
	nFace	= NULL;
	pNormal	= NULL;
	pUV		= NULL;

	pVer2	= NULL;

	model_x = model_y = model_z = 0;

	bUseSingleColor = FALSE;


	pCamera = NULL;

	darr = NULL;

	nChkFace = 0;
	kNearestVer = 20;
	for(int i=0; i<100; i++)
	{
		chkFace[i] = 0;
		chkFaceUnique[i] = 0;
	}

	objCenter.Vector(0,0,0);

	geoCenter.Vector(0,0,0);


}

vwO::~vwO()
{
	Close();
}

void vwO::Init(int nv,int nf)
{
	Close();

	pVer	= new dhVector[nv];
	pVer0	= new dhVector[nv];
	pFace	= new vwPolygon[nf];
	nVer	= nv;
	nFace	= nf;

	pUV		= new float[2*nv];	// for plane -_-

	pVer2	= new vwVertex[nv];

	quadratic	=	gluNewQuadric();

	pCamera = pGL->pCamera;


}

void vwO::Close()
{
	if (pVer0)	delete []pVer0;
	pVer0	= NULL;

	if (pVer)	delete []pVer;
	pVer	= NULL;


	if (pVer2)	delete []pVer2;
	pVer2	= NULL;


	if (pFace)	delete []pFace;
	pFace	= NULL;
	if (pNormal)delete []pNormal;
	pNormal	= NULL;


	if (pUV)	delete pUV;
	pUV		= NULL;


	nVer	= 0;
	nFace	= 0;

	for (int i=0;i<m_child.GetSize();i++)
	{
		vwO *pChild	= m_child[i];
		pChild->Close();
		delete pChild;
	}
	m_child.RemoveAll();

	gluDeleteQuadric(quadratic);


	//	2013/10/5	-_-
	if(darr) delete []darr;
	darr = NULL;
	//OutputDebugString("vwO::Close()");

}


void vwO::Add(vwO *p)
{
	m_child.Add(p);
}



void vwO::RotY(float y)
{
	H	= H.RotY(RAD(y))*H;

	
}


void vwO::RotX(float x)
{
	H	= H.RotX(RAD(x))*H;
}

void vwO::RotZ(float x)
{
	H	= H.RotZ(RAD(x))*H;
}


void vwO::Rot(float a,float b,float r)
{
	dhVector v = H.O();
	H.I();
	H	= H.RotZ(RAD(r))*H.RotY(RAD(b))*H.RotX(RAD(a));
	
	
	objCenter = H* objCenter;	//20131007

	H	= H	+ v;

		
	
}

void vwO::Trans(float x,float y,float z)
{
	dhVector o_prev = o;
	H.v[12]	= x;
	H.v[13]	= y;
	H.v[14]	= z;

	o = dhVector(x, y, z);

	//objCenter =  objCenter + (o-o_prev);			//20131007
	objCenter =  o;			//20131007
	//GetCenter();
}


void vwO::Trans(dhVector v)
{
	vwO::Trans(v.x,v.y,v.z);
}

void vwO::Move(float x,float y,float z)
{
	dhVector p2  = o+ dhVector(x, y, z);

	vwO::Trans(p2);
}




void vwO::Scale(float s)
{
	int i;
	for(i=0; i<nVer; i++)
	{
		
		pVer[i] = pVer[i].Scaling(s);
		pVer0[i] = pVer0[i].Scaling(s);
		

		model_x = model_x *s;
		model_y = model_y *s;
		model_z = model_z *s;



	}
}


void vwO::Scale(dhVector s)
{
	int i;
	for(i=0; i<nVer; i++)
	{
		
		pVer[i] = pVer[i].Scaling(s);
		pVer0[i] = pVer0[i].Scaling(s);


		model_x = model_x *s.x;
		model_y = model_y *s.y;
		model_z = model_z *s.z;


	}
}



int	vwO::GenTexture()
{
	
	return m_tx.GenTexture2();

}

void vwO::MakePlane(float a,float b,int m,int n)
{
	Init( (m+1)*(n+1),2*m*n);

	model_x = a; 
	model_y = b;
	model_z = 0; 
	
	float dx,dy;
	dx	= a/((float)m);
	dy	= b/((float)n);

	int i,j;
	for (j=0;j<=n;j++)
	for (i=0;i<=m;i++)
		pVer[(m+1)*j+i]	= dhVector(dx*i,dy*j,0);

	int np = 0;
	for (j=0;j<n;j++)
	for (i=0;i<m;i++)
	{
		int idx	= (m+1)*j+i;
		pFace[np++].Tri(pVer,idx,idx+1,idx+m+1);
		pFace[np++].Tri(pVer,idx+m+1,idx+1,idx+m+2);
	}

	FindGourad();
}

void vwO::DrawNormal()
{
	for (int i=0;i<nFace;i++)
	{
		vwPolygon *p	= &pFace[i];
		glBegin(GL_TRIANGLES);
		
		dhVector n = (*p->v[2]-*p->v[1])*(*p->v[0]-*p->v[1]);
		n	= n.Unit();
		n	= n*-1;
		glNormal3f(n.x,n.y,n.z);
		{
			glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);
			glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
			glVertex3d(p->v[2]->x,p->v[2]->y,p->v[2]->z);	
		}
		glEnd();
	}
}


void vwO::DrawPlaneNormal()
{
	// Rectangle
	dhVector ptCenter = 	dhVector(model_x/2.0,model_x/2.0,0);
	
	glColor4f(0,1,1,1);
	DrawLineWithArrow3D(ptCenter, ptCenter + dhVector(0,0,-3), TRUE, 0,0.3);
	glColor4f(1,1,1,1);


	/*
	int fID[4];
		
	fID[0] = 0;
	fID[1] = 10-1;
	fID[2] = 10 * (10-1);
	fID[3] = 10*10 -1;
*/
	
}
dhVector vwO::GetTouchedPointOnSurface(int faceID, float *pArea)
{

	if(faceID<0 || faceID >=nFace) return dhVector(-1,-1,-1);

	vwPolygon *p	= &pFace[faceID];
	
	dhVector avg_vec;
	avg_vec = p->v[0]->Scaling(pArea[0]) + p->v[1]->Scaling(pArea[1]) + p->v[2]->Scaling(pArea[2]) ;


	return avg_vec;


}

//GetSurfaceNormal
dhVector vwO::GetSurfaceNormalInterpolatedAbs(int faceID, float *pArea)
{
	if(faceID<0 || faceID >=nFace) return dhVector(-1,-1,-1);

	vwPolygon *p	= &pFace[faceID];
	int a,b,c;
	a	= p->v[0]-pVer;
	b	= p->v[1]-pVer;
	c	= p->v[2]-pVer;


	// Get barycenteric coordinate
	dhVector avg_vec;


	dhVector tr[3];
	GetTriangleVertices(faceID, tr);
	//float *pArea = ((CGL*) pWorld)->g_area;
	

	dhVector pn_a =  pNormal[a];
	dhVector pn_b =  pNormal[b];
	dhVector pn_c =  pNormal[c];


	avg_vec = pn_a.Scaling(pArea[0]) + pn_b.Scaling(pArea[1]) + pn_c.Scaling(pArea[2]) ;



	return avg_vec;

}



dhVector vwO::GetSurfaceNormalInterpolatedMV(int faceID, float *pArea)
{
	if(faceID<0 || faceID >=nFace) return dhVector(-1,-1,-1);

	dhVector avg_vec = GetSurfaceNormalInterpolatedAbs(faceID, pArea);

		
	dhVector vw_o = pCamera->vw.o;
	dhVector vw_r = pCamera->vw.r;


	dhMat T;
	T	= vw_o*-1;
	dhMat R,tmp;
	R	= tmp.RotX(RAD(vw_r.x))*tmp.RotY(RAD(vw_r.y));
	
	dhVector ret = R*H*avg_vec;//T*R*avg_vec;


	return ret.Unit();//.Scaling(-1.0);

}

void vwO::Draw3VertexNormals(int faceID, float *pArea, float sc)
{

	if(faceID<0 || faceID >=nFace) return ;
	glDisable(GL_LIGHTING);

	float arrow_scale = 0.3;


	vwPolygon *p	= &pFace[faceID];

	int a,b,c;
	a	= p->v[0]-pVer;
	b	= p->v[1]-pVer;
	c	= p->v[2]-pVer;


	sc = sc*-1.0;

	dhVector pnA = pNormal[a].Unit();
	dhVector pnB = pNormal[b].Unit();
	dhVector pnC = pNormal[c].Unit();

	dhVector v0 = *p->v[0] + pnA.Scaling(sc);
	dhVector v1 = *p->v[1] + pnB.Scaling(sc);
	dhVector v2 = *p->v[2] + pnC.Scaling(sc);


	
	
	DrawLineWithArrow3D(*p->v[0], v0, TRUE, 0,arrow_scale);
	DrawLineWithArrow3D(*p->v[1], v1, TRUE, 0,arrow_scale);
	DrawLineWithArrow3D(*p->v[2], v2, TRUE, 0,arrow_scale);

	glEnable(GL_LIGHTING);

	dhVector vw_o = pGL->pCamera->vw.o;
	dhVector vw_r = pGL->pCamera->vw.r;


	
}


void vwO::DrawAllSurfaceNormals(int faceID, float scale, int nJump)
{
	for (int i=0;i<nFace;i+= nJump)
	{
		dhVector sn_tr = GetSurfaceNormalMV(i);


		//	if(IsUsingWireMode())
		if(0)
			if(sn_tr.z<-0.3) continue;
		

		float alpha = sigmoid(20.0, sn_tr.z, 0);


		if(i==faceID)
			glColor4f(1,0,1,1);
		else

			glColor4f(1,1,0,alpha);


		
		DrawASurfaceNormal(i, scale);
	

	}

}

dhVector vwO::DrawASurfaceNormalInterpolated(int faceID,float *pArea, float sc)
{

	if(faceID<0 || faceID >=nFace) return dhVector(-1,-1,-1);
	glDisable(GL_LIGHTING);

	float arrow_scale = 0.3;

	dhVector avg_vec = GetSurfaceNormalInterpolatedAbs(faceID, pArea);	

	dhVector tr[3];
	GetTriangleVertices(faceID, tr);
	

	dhVector p_cursor_in_triangle = tr[0].Scaling(pArea[0]) + tr[1].Scaling(pArea[1]) + tr[2].Scaling(pArea[2]);
	dhVector p2 = p_cursor_in_triangle +avg_vec.Scaling(-sc);

	DrawLineWithArrow3D(p_cursor_in_triangle,  p2 ,  TRUE, 0,arrow_scale);

	//int rm = ((CGL*) pWorld)->render_mode;
	//if( rm == kSurfaceNormalVectorInterpolation)
	//	DrawLineWithArrow3D(p_cursor_in_triangle,  p2 ,  TRUE, 0,arrow_scale);


	return avg_vec;

}




void vwO::DrawLineWithArrow3D(dhVector stPt_gl, dhVector endPt_gl, BOOL arrowAtTheEnd, float z_offset,  float scale_arrow)
{
/*
	dhVector vw_o = pGL->pCamera->vw.o;
	dhVector vw_r = pGL->pCamera->vw.r;

	dhMat T;
	T	= vw_o*-1;
	dhMat R,tmp;
	R	= tmp.RotX(RAD(vw_r.x))*tmp.RotY(RAD(vw_r.y));
*/

	stPt_gl			= H* stPt_gl;
	endPt_gl		= H* endPt_gl;


	//stPt_gl		= T*stPt_gl;
	//endPt_gl	= T*endPt_gl;


	glDisable(GL_LIGHTING);
	
	
	glBegin(GL_LINES);
		glVertex3f(stPt_gl.x, stPt_gl.y, stPt_gl.z + z_offset);	//p
		glVertex3f(endPt_gl.x, endPt_gl.y, endPt_gl.z + z_offset);								//q
	glEnd();


	dhVector stPt_gdi, endPt_gdi;

	stPt_gdi	= pGL->pCamera->Projection(stPt_gl);
	endPt_gdi	= pGL->pCamera->Projection(endPt_gl);

	double angle; 
	angle		= atan2( (double) stPt_gdi.y - endPt_gdi.y, (double) stPt_gdi.x - endPt_gdi.x );
	double hypotenuse; 
	hypotenuse = sqrt( square(stPt_gdi.y - endPt_gdi.y) + square(stPt_gdi.x - endPt_gdi.x) );

	dhVector pt_arrow_gdi[2], pt_arrow_gl[2];

	if(arrowAtTheEnd)
	{
		pt_arrow_gdi[0].x = (endPt_gdi.x + scale_arrow *hypotenuse* cos(angle + PI / 4.0));
		pt_arrow_gdi[0].y = (endPt_gdi.y + scale_arrow *hypotenuse* sin(angle + PI / 4.0));

		pt_arrow_gdi[1].x = (endPt_gdi.x + scale_arrow *hypotenuse* cos(angle - PI / 4.0));
		pt_arrow_gdi[1].y = (endPt_gdi.y + scale_arrow *hypotenuse* sin(angle - PI / 4.0));

	}
	else
	{
		pt_arrow_gdi[0].x = (endPt_gdi.x - scale_arrow *hypotenuse* cos(angle + PI / 4.0));
		pt_arrow_gdi[0].y = (endPt_gdi.y - scale_arrow *hypotenuse* sin(angle + PI / 4.0));

		pt_arrow_gdi[1].x = (endPt_gdi.x - scale_arrow *hypotenuse* cos(angle - PI / 4.0));
		pt_arrow_gdi[1].y = (endPt_gdi.y - scale_arrow *hypotenuse* sin(angle - PI / 4.0));

	}
	pt_arrow_gl[0] = pGL->pCamera->GetGLPos(pt_arrow_gdi[0].x, pt_arrow_gdi[0].y);
	pt_arrow_gl[1] = pGL->pCamera->GetGLPos(pt_arrow_gdi[1].x, pt_arrow_gdi[1].y);

	glBegin(GL_LINES);
		glVertex3f(endPt_gl.x, endPt_gl.y, endPt_gl.z + z_offset);	//p
		glVertex3f(pt_arrow_gl[0].x, pt_arrow_gl[0].y, pt_arrow_gl[0].z + z_offset);								//q
	glEnd();

	
	glBegin(GL_LINES);
		glVertex3f(endPt_gl.x, endPt_gl.y, endPt_gl.z +z_offset);	//p
		glVertex3f(pt_arrow_gl[1].x, pt_arrow_gl[1].y, pt_arrow_gl[1].z +z_offset);								//q
	glEnd();


	glEnable(GL_LIGHTING);

}


void vwO::Draw(BOOL bDispAxis, BOOL bWireFrame, BOOL bDrawVert)
{	

	if(nVer<=0) return;

	glPushMatrix();
	glMultMatrixf(H.v);

	//m_tx.GenTexture();
	int i;
	if(!bUseSingleColor)
		DrawObject(bWireFrame,bDrawVert);
	else
	{	
		glColor3f(67./255, 255./255, 163/255.);
		DrawObject(bWireFrame,bDrawVert);
	}
	for ( i=0;i<m_child.GetSize();i++)
		m_child[i]->Draw();



	if(bDrawVert)
	{
		glDisable(GL_LIGHTING);
		glColor3f(195./255.,0,208./255.);
		//glColor3f(110./255, 219./255, 255/255.);
		for(i=0; i<nVer; i++)
		{
			glPushMatrix();
			glTranslatef(pVer[i].x, pVer[i].y, pVer[i].z);
			gluSphere(quadratic,0.05f,4,4); 
			glPopMatrix();

		}

		glColor3f(1,1,1);
		glEnable(GL_LIGHTING);
	}


	glPopMatrix();
	

	if(bDispAxis)
		DisplayOrigin();
}
/*
void vwO::DrawObject()
{
	for (int i=0;i<nFace;i++)
	{
		vwPolygon *p	= &pFace[i];

		int a,b,c;
		a	= p->v[0]-pVer;
		b	= p->v[1]-pVer;
		c	= p->v[2]-pVer;

		glBegin(GL_TRIANGLES);
		{
		
				glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);
				glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
				glVertex3d(p->v[2]->x,p->v[2]->y,p->v[2]->z);	
	
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	
}
*/
void vwO::DrawObject(BOOL bWireFrame, BOOL bDrawVert)
{
	
	GLint polygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, polygonMode);

	glPolygonMode(GL_FRONT_AND_BACK, bWireFrame);

	

	glEnable(GL_TEXTURE_2D);

	//m_tx.BindTexture();
	glBindTexture(GL_TEXTURE_2D,m_tx.nTexID);
	//Dbg("%d", m_tx.nTexID);
	

	for (int i=0;i<nFace;i++)
	{
		vwPolygon *p	= &pFace[i];

		int a,b,c;
		a	= p->v[0]-pVer;
		b	= p->v[1]-pVer;
		c	= p->v[2]-pVer;

		glBegin(GL_TRIANGLES);
		{
			
			if (pNormal)
			{
				glNormal3f(pNormal[a].x,pNormal[a].y,pNormal[a].z);

				glTexCoord2f( pUV[2*a],pUV[2*a+1]);
				glVertex3d(pVer[a].x,pVer[a].y,pVer[a].z);
				//glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);

				glNormal3f(pNormal[b].x,pNormal[b].y,pNormal[b].z);
				glTexCoord2f( pUV[2*b],pUV[2*b+1]);
				glVertex3d(pVer[b].x,pVer[b].y,pVer[b].z);
				//glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
				
				glNormal3f(pNormal[c].x,pNormal[c].y,pNormal[c].z);
				glTexCoord2f( pUV[2*c],pUV[2*c+1]);
				glVertex3d(pVer[c].x,pVer[c].y,pVer[c].z);
				//glVertex3d(p->v[2]->x,p->v[2]->y,p->v[2]->z);
			}
			else
			{
				glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);
				glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
				glVertex3d(p->v[2]->x,p->v[2]->y,p->v[2]->z);	
			}
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode[1]);

}

void vwO::FindGourad()
{
	int *pIndex	= new int[nVer];
	memset(pIndex,0,sizeof(int)*nVer);
	if (pNormal)	delete[]pNormal;
	pNormal	= new dhVector[nVer];
	memset(pNormal,0,sizeof(dhVector)*nVer);

	int i;
	for (i=0;i<nFace;i++)
	{
		vwPolygon *p = &pFace[i];

		dhVector n = (*p->v[2]-*p->v[1])*(*p->v[0]-*p->v[1]);
		n	= n.Unit();
		n	= n*-1;

		int a,b,c;
		a	= p->v[0]-pVer;
		b	= p->v[1]-pVer;
		c	= p->v[2]-pVer;

		pNormal[a]	= pNormal[a] + n;
		pIndex[a]	= pIndex[a]+1;
		pNormal[b]	= pNormal[b] + n;
		pIndex[b]	= pIndex[b]+1;
		pNormal[c]	= pNormal[c] + n;
		pIndex[c]	= pIndex[c]+1;
	}

	for (i=0;i<nVer;i++)
	{
		if ( pIndex[i]==0)	continue;
		float f	= 1./pIndex[i];
		pNormal[i]	= pNormal[i]*f;
		pNormal[i]	= pNormal[i].Unit();
	}

	delete pIndex;
}

/*

  	float xMin	= 0.0;
	float xMax	= width_x;

	float yMin	= 0.0;
	float yMax	= 1.0;

  */

// FOR PLANE
void vwO::SetUV(float xMin, float xMax, float yMin, float yMax,  int n_div_x, int n_div_y)
{
	float dX	= (xMax - xMin) /(float) n_div_x;
	float dY	= (yMax - yMin) /(float) n_div_y;

	int nPtX = 2 * (1 + n_div_x);
	for(int j=0; j<n_div_y+1; j++)
	{
		for(int i=0; i<n_div_x+1; i++)
		{
			pUV[j*nPtX + 2*i+ 0]	= xMin + dX *i;
			pUV[j*nPtX + 2*i+ 1]	= yMin+j*dY;
	
		}
	}

}
void vwO::SetUV(int n_div_x, int n_div_y)
{

	SetUV(0, 1, 0, 1,   n_div_x,  n_div_y);

}

void vwO::DisplayOrigin()
{

	glPushMatrix();
	//glMultMatrixf(H.v);

	glDisable(GL_LIGHTING);
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(1.0f, 0.0f, 0.0f); // ending point of the line
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(0.0f, 1.0f, 0.0f); // ending point of the line
	glEnd( );

	glColor3f(0,0,1);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(0.0f, 0.0f, 1.0f); // ending point of the line
	glEnd( );
	glColor3f(1,1,1);

	glEnable(GL_LIGHTING);


	glPopMatrix();
}

void vwO::GetVertexNormals(int faceID, dhVector* v_vectors, BOOL bCompensateMV)
{
	if(faceID<0 || faceID >=nFace)
	{
		v_vectors[0] = dhVector(-1,-1,-1);
		v_vectors[1] = dhVector(-1,-1,-1);
		v_vectors[2] = dhVector(-1,-1,-1);
		return;
	}

	vwPolygon *p	= &pFace[faceID];
	int a,b,c;
	a	= p->v[0]-pVer;
	b	= p->v[1]-pVer;
	c	= p->v[2]-pVer;
	

	dhVector pn_a =  pNormal[a];
	dhVector pn_b =  pNormal[b];
	dhVector pn_c =  pNormal[c];


	if(bCompensateMV)
	{
		dhVector vw_o = pGL->pCamera->vw.o;
		dhVector vw_r = pGL->pCamera->vw.r;

		dhMat T;
		T	= vw_o;//*-1;
		dhMat R,tmp;
		R	= tmp.RotX(RAD(vw_r.x))*tmp.RotY(RAD(vw_r.y));

		pn_a 	= T*R*H *pn_a;
		pn_b 	= T*R*H *pn_b;
		pn_c 	= T*R*H *pn_c;
	}


	v_vectors[0] = pn_a.Scaling(-1);
	v_vectors[1] = pn_b.Scaling(-1);
	v_vectors[2] = pn_c.Scaling(-1);


}
void vwO::dispSurf(int nFaceID, dhVector offset_o, BOOL bInterp)
{

	if(nFaceID<0 || nFaceID >=nFace)
	{
		
		return;
		
	}
	
	vwPolygon *p	= &pFace[nFaceID];

	dhVector vdbg[3];

	vdbg[0] = *p->v[0] + offset_o;
	vdbg[1] = *p->v[1] + offset_o;
	vdbg[2] = *p->v[2] + offset_o;


	glBegin(GL_TRIANGLES);
	{
		glVertex3d(vdbg[0].x,vdbg[0].y,vdbg[0].z);
		glVertex3d(vdbg[1].x,vdbg[1].y,vdbg[1].z);
		glVertex3d(vdbg[2].x,vdbg[2].y,vdbg[2].z);
		
	}
	glEnd();




}

void vwO::dispTBN(int nFaceID, BOOL bInterp)
{
	

	dhVector triangle_o;
	dhVector tr[3];
	GetTriangleVertices(nFaceID, tr); 
	if(!bInterp)
	{
		


		triangle_o	= tr[0]+tr[1]+tr[2];
		triangle_o = triangle_o/3.0;

	}
	else
	{
		float *pArea = pGL->gArea;
		triangle_o = tr[0].Scaling(pArea[0]) + tr[1].Scaling(pArea[1]) + tr[2].Scaling(pArea[2]);

	}


	dhVector T, B, N;
	constructTBN(nFaceID, T, B, N, FALSE, bInterp);

	//if(

	T= T.Scaling(1);
	B= B.Scaling(1);

	

	//GetSurfaceNormalInterpolatedAbs(nFaceID, pArea);	
	

	T = triangle_o + T;
	B = triangle_o + B;

	// return;
	glColor3f(1,0,0);
	DrawLineWithArrow3D(triangle_o, T, TRUE, 0,0.3);

	glColor3f(0,1,0);
	DrawLineWithArrow3D(triangle_o, B, TRUE, 0,0.3);

	glColor3f(0,0,1);
	DrawLineWithArrow3D(triangle_o, N, TRUE, 0,0.3);
	//dhVector p0 = m_plane.pointOnPlane();


}

void vwO::constructTBN(int nFaceID, dhMat &mat1, BOOL bUseModelView, BOOL bInterp)
{
	if(nFaceID<0 || nFaceID >=nFace)
	{
		mat1.I();
		return;
		
	}

	float *pArea = pGL->gArea;

	dhVector n;
	
	if(bUseModelView)
	{
		if(!bInterp)
		{
			n = GetSurfaceNormalMV(nFaceID);
		}
		else
		{
			n = GetSurfaceNormalInterpolatedMV(nFaceID, pArea);	///
		}
	}
	else
	{
		if(!bInterp)
		{

			n = GetSurfaceNormal(nFaceID);
		}
		else
		{
			//GetSurfaceNormalInterpolatedAbs
			n = GetSurfaceNormalInterpolatedAbs(nFaceID, pArea);
		}
	}


	dhVector tmp = n - dhVector(0,0,1);

	dhVector u,v;
	if (ddIsEqual<float>(tmp.Abs(), 0.0, 0.001))
    {        
        u.Vector(1,0,0);
		v.Vector(0,1,0);
    }
	else
	{
		if(1)
		{
			v = n * dhVector(0,0,1);
			u = n * v;
		}
		else
		{
			u = n * dhVector(0,0,1);
			v = n * u;
		}
		u= u.Unit();
		v= v.Unit();

	}

	
	mat1.v[0] = u.x;
	mat1.v[1] = u.y;
	mat1.v[2] = u.z;

	mat1.v[4] = v.x;
	mat1.v[5] = v.y;
	mat1.v[6] = v.z;

	mat1.v[8] = n.x;
	mat1.v[9] = n.y;
	mat1.v[10] = n.z;
}

void vwO::constructTBN(int nFaceID, dhVector &mT, dhVector &mB, dhVector &mN, BOOL bUseModelView, BOOL bInterp)
{
	if(nFaceID<0 || nFaceID >=nFace)
	{
		
		return;
		
	}

	float *pArea = pGL->gArea;

	dhVector n;
	
	if(bUseModelView)
	{
		if(!bInterp)
		{
			n = GetSurfaceNormalMV(nFaceID);
		}
		else
		{
			n = GetSurfaceNormalInterpolatedMV(nFaceID, pArea);	///	///
		}
	}
	else
	{
		if(!bInterp)
		{

			n = GetSurfaceNormal(nFaceID);
		}
		else
		{
			//GetSurfaceNormalInterpolatedAbs
			n = GetSurfaceNormalInterpolatedAbs(nFaceID, pArea);
		}
	}


	dhVector tmp = n - dhVector(0,0,1);

	dhVector u,v;
	if (ddIsEqual<float>(tmp.Abs(), 0.0, 0.001))
    {        
        u.Vector(1,0,0);
		v.Vector(0,1,0);
    }
	else
	{
		
		if(1)
		{
			v = n * dhVector(0,0,1);
			//u = n * v;
			u.Vector(n.x*n.z, n.y*n.z, -(n.x*n.x + n.y*n.y));
		}
		else
		{
			u = n * dhVector(0,0,1);
			v = n * u;
		}


		u= u.Unit();
		v= v.Unit();

	}

	mT = u;
	mB = v;
	mN = n;
	

}

dhVector vwO::GetSurfaceNormal(int nFaceID)
{
	
	if(nFaceID<0 || nFaceID >=nFace) return dhVector(0,0,0);

	vwPolygon *pF = &pFace[nFaceID];

	
 	int a,b,c;
	a	= pF->v[0]- pVer;
	b	= pF->v[1]- pVer;
	c	= pF->v[2]- pVer;

	dhVector v[3];

	v[0]	= pVer[a];//H *
	v[1]	= pVer[b];
	v[2]	= pVer[c];
	
	dhVector n1 = (v[2]-v[1])*(v[0]-v[1]);

	return n1.Unit();

}


dhVector vwO::GetSurfaceNormalMV(int nFaceID)
{
	if(nFaceID<0 || nFaceID >=nFace) return dhVector(0,0,0);

	vwPolygon *pF = &pFace[nFaceID];

	dhVector vw_o = pGL->pCamera->vw.o;
	dhVector vw_r = pGL->pCamera->vw.r;

 	int a,b,c;
	a	= pF->v[0]- pVer;
	b	= pF->v[1]- pVer;
	c	= pF->v[2]- pVer;

	dhVector v[3];

	dhMat T;
	T	= vw_o*-1;
	dhMat R,tmp;
	R	= tmp.RotX(RAD(vw_r.x))*tmp.RotY(RAD(vw_r.y));


	//sn_tr	= H * sn_tr;

	v[0]	= T*R*H *pVer[a];
	v[1]	= T*R*H *pVer[b];
	v[2]	= T*R*H *pVer[c];

	/*
	v[0]	= T*R*pVer[a];
	v[1]	= T*R*pVer[b];
	v[2]	= T*R*pVer[c];

	*/			
	dhVector n_transformed = (v[2]-v[1])*(v[0]-v[1]);

	return n_transformed.Unit();
}


dhVector vwO::GetSurfaceNormalMV(int nFaceID, dhVector vw_o, dhVector vw_r)
{
	if(nFaceID<0 || nFaceID >=nFace) return dhVector(0,0,0);
	vwPolygon *pF = &pFace[nFaceID];

	int a,b,c;
	a	= pF->v[0]- pVer;
	b	= pF->v[1]- pVer;
	c	= pF->v[2]- pVer;

	dhVector v[3];

	dhMat T;
	T	= vw_o*-1;
	dhMat R,tmp;
	R	= tmp.RotX(RAD(vw_r.x))*tmp.RotY(RAD(vw_r.y));

	v[0]	= T*R*H *pVer[a];
	v[1]	= T*R*H *pVer[b];
	v[2]	= T*R*H *pVer[c];
				
	dhVector n_transformed = (v[2]-v[1])*(v[0]-v[1]);
	return n_transformed.Unit();
}

dhVector vwO::GetTriangleCenter(int nFaceID, BOOL bApplyModelView)
{
	
	dhVector tr[3];
	GetTriangleVertices(nFaceID, tr); 


	dhVector c = tr[0]+tr[1]+tr[2];
	c = c.Scaling(1.0/3.0);
	return c;

}
void vwO::GetTriangleVertices(int faceID, dhVector* pV, BOOL bApplyModelView )
{

	if(faceID<0 || faceID >=nFace) return;


	vwPolygon *p	= &pFace[faceID];

	//pV =  *p->v;
	dhVector p_rtn[3];

	p_rtn[0] = *p->v[0];
	p_rtn[1] = *p->v[1];
	p_rtn[2] = *p->v[2];

	if(bApplyModelView)
	{

		dhVector vw_o = pGL->pCamera->vw.o;
		dhVector vw_r = pGL->pCamera->vw.r;

		dhMat T;
		T	= vw_o*-1;
		dhMat R,tmp;
		R	= tmp.RotX(RAD(vw_r.x))*tmp.RotY(RAD(vw_r.y));

		p_rtn[0]	= T*R*p_rtn[0];
		p_rtn[1]	= T*R*p_rtn[1];
		p_rtn[2]	= T*R*p_rtn[2];

	}

	pV[0] = p_rtn[0];
	pV[1] = p_rtn[1];
	pV[2] = p_rtn[2];

}

dhVector vwO::GetANormalAbs(int faceID)
{
	if(faceID<0 || faceID >=nFace) return dhVector(0,0,0);


	vwPolygon *p	= &pFace[faceID];

	int a,b,c;
	a	= p->v[0]-pVer;
	b	= p->v[1]-pVer;
	c	= p->v[2]-pVer;
	
	dhVector n = (*p->v[2]-*p->v[1])*(*p->v[0]-*p->v[1]);

	//n	= H*n;
	n	= n.Unit();
	

	return n;

}


void vwO::DrawASurfaceNormal(int faceID, float scale, BOOL stickToMouse)
{
	
	if(faceID<0 || faceID >=nFace) return;
	glDisable(GL_LIGHTING);

	dhVector n = GetANormalAbs(faceID);


	n	= n*-scale;

	dhVector tr[3];
	GetTriangleVertices(faceID, tr); 


	if(stickToMouse)
	{
		// Barycentric coordnate !!!
		float *pArea = pGL->gArea;
		

		p_weighted = tr[0].Scaling(pArea[0]) + tr[1].Scaling(pArea[1]) + tr[2].Scaling(pArea[2]);

		//p_weighted = pGL->mPt_GL;
		n = p_weighted + n;

		DrawLineWithArrow3D(p_weighted, n, TRUE, 0,0.3);

	}

	else
	{
		dhVector triangle_o	= tr[0]+tr[1]+tr[2];
		triangle_o = triangle_o/3.0;

		n = triangle_o + n;

		DrawLineWithArrow3D(triangle_o, n, TRUE, 0,0.3);

	}

	glEnable(GL_LIGHTING);
}

dhVector vwO::GetCenter()
{
	dhVector ret(0,0,0);

	for(int i=0; i<nVer; i++)
	{
		
		ret = ret + pVer[i];
		
	}
	
	ret = ret.Scaling(1.0/nVer);

	objCenter = ret;
	geoCenter	= ret;
	return ret;
}

dhVector vwO::GetCenterInScreen(vwCamera* pCamera1)
{
	dhVector objCenterp	= pCamera1->Projection(objCenter);


	return objCenterp;
	//pCamera1->Projection


}

void vwO::SetCenterWithXY(vwCamera* pCamera1, int x_screen, int y_screen)
{
	dhVector p1p;// = pCamera1->UnProjection2(dhVector(x_screen, y_screen, 0));

	double glpt[3];
	pCamera1->UnProjection3(dhVector(x_screen, y_screen, 0),glpt);

	H.v[12] = glpt[0]; 
	H.v[13] = glpt[1];

	objCenter.x = geoCenter.x + glpt[0];
	objCenter.y = geoCenter.x + glpt[1];

	//H.v[14] = glpt[2];

	//H.v[14] = -p1p.z;
	// CORRECT PROPERLY !!
/*
	H.v[12] = 1+x_screen/20.0;
	H.v[13] = 29-y_screen/20.0;
	*/
}


void vwO::MakeGaussianDeform(unsigned int vID_,  int noVert, float spread_, float height_, BOOL bUseModifed, BOOL bUpdate)// float radius_,
{
	//find closest points
	const int n			= nVer;
	const int nf		= nFace;

	float		nearestDistance		= 0;
	dhVector	nearestVertex;
	int			nearestIndex		= 0;

	dhVector *pCur;
	if(bUseModifed)
		pCur = pVer;
	else
		pCur = pVer0;

	int i,j;
	
	dhVector mPt = pCur[vID_];
	
	if(!darr)
		darr = new Dist [n];


	for (i=0;i<n;i++)
	{
		
		dhVector p1		= pCur[i];//H*pVer[i];
		dhVector p1p	= p1;//pCamera1->Projection(p1);

		float distance = (p1p.x - mPt.x)*(p1p.x - mPt.x) + (p1p.y - mPt.y)* (p1p.y - mPt.y) +  (p1p.z - mPt.z)* (p1p.z - mPt.z);

		darr[i].dist	= distance;
		darr[i].index	= i;

	}

	qsort(darr, n, sizeof(darr[0]), compare_structs);

	height_ *= 1;
	
	// Make deform
	//pVer[4515] = pVer0[4515] + dhVector(0,0,2);

	
	for(i=0; i< noVert; i++)
	{

		int vid1		= darr[nVer - 1 - i].index;
		float dist1		= sqrt(darr[nVer - 1 - i].dist);

		//pVer[vid1].z = pVer[vid1].z + 1;
		//pVer[vid1] = pVer0[vid1] ;// dhVector(0,0,0.00001);
		vwPolygon* pp	= (vwPolygon*) pVer2[vid1].GetAt(0);	//at least one face
			
		
		int f = f = pp - pFace;

		dhVector vns[3];

		GetVertexNormals(f, vns);

		
		
		for(j=0; j<3; j++)
		{
			dhVector *pp1 =  pp->v[j];
			dhVector *pp2 =  &pVer[vid1] ;

			if(pp1 == pp2)

			//if(pVer0[vid1].x == pp->v[j]->x &&pVer0[vid1].y == pp->v[j]->y && pVer0[vid1].z == pp->v[j]->z)
			{
				
				float h2 =height_ *exp(-dist1*dist1 /spread_);


				pVer[vid1].x = pCur[vid1].x+ vns[j].x*h2;
				pVer[vid1].y = pCur[vid1].y+ vns[j].y*h2;
				pVer[vid1].z = pCur[vid1].z+ vns[j].z*h2;

				
				if(1)
				{
					glDisable(GL_LIGHTING);
					glColor3f(195./255.,0,208./255.);
					
				
					{
						glPushMatrix();
						glTranslatef(pVer[vid1].x, pVer[vid1].y, pVer[vid1].z);
						gluSphere(quadratic,0.05f,4,4); 
						glPopMatrix();

					}

					glColor3f(1,1,1);
					glEnable(GL_LIGHTING);

				}






				break;
			}

			

		}
	
	}


	//pVer[n];
	
	if(bUpdate)
	FindGourad();
}


dhVector vwO::GetTouchedFace(CPoint mpt, vwCamera *pCamera1, int &nface, float *_area, BOOL bInterpolateNormal)
{
	const int n		= nVer; //1995
	const int nf	= nFace;


	dhVector sn(0,0,0);
	

	float		nearestDistance = 0;
	dhVector	nearestVertex;
	int			nearestIndex	= 0;

	dhVector mPt = dhVector(mpt.x, mpt.y, 0);
	int i,j;
	
	
	if(!darr)
		darr = new Dist [n];


	

	for (i=0;i<n;i++)
	{
		
		dhVector p1		= H*pVer[i];
		dhVector p1p	= pCamera1->Projection(p1);

		float distance = (p1p.x - mPt.x)*(p1p.x - mPt.x) + (p1p.y - mPt.y)* (p1p.y - mPt.y);

		darr[i].dist	= distance;
		darr[i].index	= i;

		if(i == 0 || distance < nearestDistance) 
		{
			// check whether this vertex is in the visible face ?
			// if not, pass updating values

			
			{
				nearestDistance		= distance;
				nearestVertex		= p1p;
				nearestIndex		= i;
			}
		}
	}

	qsort(darr, n, sizeof(darr[0]), compare_structs);
     
	dhVector chk = dhVector(mpt.x, mpt.y, 0);
	
	dhVector m1;

	/// Get the face list of interest 
	nChkFace = 0;
	for(i=0; i<100; i++)
	{
		chkFace[i]			= 0;
		chkFaceUnique[i]	= -1;
	}

	for(i=0; i<kNearestVer; i++)
	{
		int vid1 = darr[nVer - 1 - i].index;
		//vwPolygon* pp = (vwPolygon*) pVer2[vid1].;//->GetAt(

		//printf("%d ", vid1);

		int nsz = pVer2[vid1].GetCount();	// !!!

		
		for(j=0; j<nsz; j++)
		{
			vwPolygon* pp = (vwPolygon*) pVer2[vid1].GetAt(j);
			
			int f;	//face ID
			f = pp - pFace;

			chkFace[nChkFace++] = f;
			
			

		}
	}

	//printf("\n");

	//Array *a1 = new Array(chkFace, sizeof(chkFace) / sizeof(chkFace[0]));
	Array *a1			= new Array(chkFace, nChkFace);
	Array *u1			= Find_Unique_Elements(a1);
	
	int *sndir			= new int[u1->len];
	memset(sndir, 0, u1->len* sizeof(int));

	float max_depth_m	=-999999;
	int nFinalist		= -1;
	dhVector vp[3];
	for(i=0; i<u1->len; i++)
	{
		chkFaceUnique[i] = u1->elements[i];
		
		vwPolygon* pFace1 = &pFace[chkFaceUnique[i]];

		// get projected...

		vp[0]		= H**pFace1->v[0];
		vp[1]		= H**pFace1->v[1];
		vp[2]		= H**pFace1->v[2];

		vp[0]		= pCamera1->Projection(vp[0]);
		vp[1]		= pCamera1->Projection(vp[1]);
		vp[2]		= pCamera1->Projection(vp[2]);

/*
		dhVector p_rot[3];
		p_rot[0]	= *pFace->v[0];
		p_rot[1]	= *pFace->v[1];
		p_rot[2]	= *pFace->v[2];

		vp[0]		= pCamera1->Projection(p_rot[0]);
		vp[1]		= pCamera1->Projection(p_rot[1]);
		vp[2]		= pCamera1->Projection(p_rot[2]);
*/
		dhVector vp_chk[3];

		vp_chk[0] = dhVector(vp[0].x, vp[0].y, 0);//T*R*H**pFace->v[0];//
		vp_chk[1] = dhVector(vp[1].x, vp[1].y, 0);//T*R*H**pFace->v[1];//
		vp_chk[2] = dhVector(vp[2].x, vp[2].y, 0);//T*R*H**pFace->v[2];//
	
		//mPt
		int n = CTriangle::IsIn(vp_chk, mPt);
		if(n>0)
		{

			sn	= (vp_chk[2]-vp_chk[1])*(vp_chk[0]-vp_chk[1]);
			sn	= sn.Unit();

			// this is describing only surface direction... 
			// need to be enhanced with distance..
			if(sn.z<0)	// 1 or -1
			{
				
				sndir[i] = -1;

				dhVector tr[3];
				GetTriangleVertices(chkFaceUnique[i], tr, TRUE);
				vwPolygon* pFace1 = &pFace[chkFaceUnique[i]];
				

				float zmean = (tr[0].z + tr[1].z + tr[2].z)/3;

				if(zmean>max_depth_m) 
				{
					max_depth_m = zmean;
					nFinalist = chkFaceUnique[i];

				}

			}
			else if(sn.z>0)
			{
				sndir[i] = 1;
			}

		}
		
	}
	

	nface = nFinalist;


	if(nFinalist>-1)
	{
	
		vwPolygon* pFace1 = &pFace[nFinalist];
		vp[0]		= H**pFace1->v[0];
		vp[1]		= H**pFace1->v[1];
		vp[2]		= H**pFace1->v[2];

		vp[0]		= pCamera1->Projection(vp[0]);
		vp[1]		= pCamera1->Projection(vp[1]);
		vp[2]		= pCamera1->Projection(vp[2]);

		CTriangle::ChkBarycentric(vp, mPt, _area);

		////////////////////////////////////////////////
		//ADDED FOR W
		float b0 = _area[0];
		float b1 = _area[1];
		float b2 = _area[2];


		dhVector p_rot[3];
		dhVector p_rot_face[3];

		p_rot[0]	= H**pFace1->v[0];
		p_rot[1]	= H**pFace1->v[1];
		p_rot[2]	= H**pFace1->v[2];


		dhMat mat_modelview = pCamera1->mat_mm;

		p_rot_face[0] = mat_modelview *p_rot[0];	//20120104
		p_rot_face[1] = mat_modelview *p_rot[1];
		p_rot_face[2] = mat_modelview *p_rot[2];


		float w0 = p_rot_face[0].z;//v0_proj_unproj.z;
		float w1 = p_rot_face[1].z;//v1_proj_unproj.z;
		float w2 = p_rot_face[2].z;//v2_proj_unproj.z;


		float w_= b0/w0 + b1/w1 + b2/w2;
		w_ = 1.0/w_;
					

		_area[0] = _area[0] / w0 * w_;
		_area[1] = _area[1] / w1 * w_;
		_area[2] = _area[2] / w2 * w_;


		////////////////////////////////////////////////


		// Calc angle between mat_modelview* H**pFace1 and z=0
		

		/////mtl_id = 0;//face_usemtl.GetAt(nFinalist);

	}
	else
	{
		_area[0] = 0;
		_area[1] = 0;
		_area[2] = 0;
	}

	dhVector sn_tr(0,0,0);
	
		
	if(nFinalist>-1)
	{

		if(bInterpolateNormal)
		{
			
			sn_tr = GetSurfaceNormalInterpolatedAbs(nFinalist, _area);	
			sn_tr = sn_tr.Scaling(-1);
		}
		else
		{
			sn_tr = GetSurfaceNormalMV(nFinalist);
		}
		
/*
		
		if(1)
			sn = GetSurfaceNormalInterpolatedAbs(nFinalist, _area);	// CHECK
		else
			sn = GetSurfaceNormal(nFinalist);
*/


	}





	return sn_tr;	//sn?

}

///////////////
// BOX
//

void vwBox::MakeBox(float a,float b,float c)
{
	Init(8,12);

	if(pUV) delete pUV;
	pUV		= new float[2*4];

	model_x = a; 
	model_y = b;
	model_z = c;

	pVer[0].Vector( 0,0,0 );
    pVer[1].Vector( a,0,0 );
    pVer[2].Vector( a,b,0 );
    pVer[3].Vector( 0,b,0 );

    pVer[4].Vector( 0,0,c );
    pVer[5].Vector( a,0,c );
    pVer[6].Vector( a,b,c );
    pVer[7].Vector( 0,b,c );

	// cw
	pFace[0].Tri(pVer,1,3,0);
	pFace[1].Tri(pVer,1,2,3);

	pFace[2].Tri(pVer,0,4,5);
	pFace[3].Tri(pVer,0,5,1);

	pFace[4].Tri(pVer,1,5,6);
	pFace[5].Tri(pVer,1,6,2);

	pFace[6].Tri(pVer,2,6,3);
	pFace[7].Tri(pVer,3,6,7);

	pFace[8].Tri(pVer,3,7,0);
	pFace[9].Tri(pVer,0,7,4);

	pFace[10].Tri(pVer,4,7,5);
	pFace[11].Tri(pVer,5,7,6);

	FindGourad();
}



void vwBox::DrawObject()
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,m_tx.nTexID);
	glBegin(GL_QUADS);
		// Front Face
		int a, b, c, d;
		//1265
		a = 1; b = 5; c = 6; d = 2;//cw
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pVer[a].x, pVer[a].y, pVer[a].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pVer[b].x, pVer[b].y, pVer[b].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pVer[c].x, pVer[c].y, pVer[c].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pVer[d].x, pVer[d].y, pVer[d].z);
		
		// Back Face
		//0374
		a = 3; b = 0; c = 4; d = 7;//
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pVer[a].x, pVer[a].y, pVer[a].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pVer[b].x, pVer[b].y, pVer[b].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pVer[c].x, pVer[c].y, pVer[c].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pVer[d].x, pVer[d].y, pVer[d].z);

		// Top Face
		a = 5; b = 4; c = 7; d = 6;
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pVer[a].x, pVer[a].y, pVer[a].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pVer[b].x, pVer[b].y, pVer[b].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pVer[c].x, pVer[c].y, pVer[c].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pVer[d].x, pVer[d].y, pVer[d].z);

		
		// Bottom Face
		a = 0; b = 1; c = 2; d = 3;
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pVer[a].x, pVer[a].y, pVer[a].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pVer[b].x, pVer[b].y, pVer[b].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pVer[c].x, pVer[c].y, pVer[c].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pVer[d].x, pVer[d].y, pVer[d].z);

		// Right face
		a = 3; b = 2; c = 6; d = 7;
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pVer[a].x, pVer[a].y, pVer[a].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pVer[b].x, pVer[b].y, pVer[b].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pVer[c].x, pVer[c].y, pVer[c].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pVer[d].x, pVer[d].y, pVer[d].z);

		
		// Left Face
		a = 1; b = 0; c = 4; d = 5;
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pVer[a].x, pVer[a].y, pVer[a].z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pVer[b].x, pVer[b].y, pVer[b].z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pVer[c].x, pVer[c].y, pVer[c].z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pVer[d].x, pVer[d].y, pVer[d].z);

		
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


/// Dont need this. Use H instead
/*
dhMat T;
T	= vw_o;//*-1;
dhMat R,tmp;
R	= tmp.RotX(RAD(vw_r.x))*tmp.RotY(RAD(vw_r.y));
*/