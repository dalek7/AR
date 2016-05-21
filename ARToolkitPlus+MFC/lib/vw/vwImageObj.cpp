// October 1, 2013
// Written by Seung-Chan Kim @TCL, KAIST

#include "stdafx.h"
#include "vwImageObj.h"


vwImageObj::vwImageObj()
{

	pTex = NULL;
	g_nLayer = 0;

	n_active	= 0;


	g_uv_max_x = 1;
	g_uv_max_y = 1;
}

vwImageObj::~vwImageObj()
{
	for(int i=0; i<g_nLayer; i++)
	{
		if(pTex[i].nTexID>0)
		pTex[i].Close();

	}

	if(pTex)
		delete []pTex;

	pTex =NULL;
}


void vwImageObj::CloseAllTextures()
{
	for(int i=0; i<g_nLayer; i++)
	{
		if(pTex[i].nTexID>0)
		pTex[i].Close();

	}
/*
	if(pTex)
		delete []pTex;

	pTex =NULL;

*/


}


void vwImageObj::GetPngBuffer(int n, BYTE* pOut)
{
	int w_pic = pTex[n].m_png.w;
	int h_pic = pTex[n].m_png.h;

	int nCh	  = pTex[n].m_png.bits;

	if(pOut) delete[] pOut;
	pOut = new BYTE[w_pic * h_pic * nCh];


	
	memmove(pOut, pTex[n].m_png.pBuffer,w_pic * h_pic * nCh);
	

}

//LoadTextureFromBuffer(0,pBuf, 1280, 800, 3);
void vwImageObj::LoadTextureFromBuffer(int n, unsigned char* pS, int w_pic, int h_pic, int nCh, BOOL bFlip,  BOOL bBGR2RGB)
{
	pTex[n].SetTexture2(pS, w_pic, h_pic, nCh, bFlip, bBGR2RGB);


}

void vwImageObj::LoadTexture(int n, char* fn)
{
	pTex[n].LoadTexture(fn);
	printf("Loaded texture image : %d\n", n);
	
}

void vwImageObj::ReloadTexture(int n, char* fn)
{

	if(pTex[n].nTexID>0)
		pTex[n].Close();

	pTex[n].LoadTexture(fn);
	printf("Loaded texture image : %d\n", n);
}


// Consider RELEASE
void vwImageObj::GenTextures()
{
	int i;
	for(i=0; i<g_nLayer; i++)
	{
		int nn = pTex[i].GenTexture2();
		//if(nn>0)
		//	printf("vwImageObj::GenTextures() -- glGenTextures : %d\n ", nn);
	}
	
}
	

void vwImageObj::Make(float w_,float h_,int nlayer, int m,int n)
{

	Init( (m+1)*(n+1),2*m*n);

	// Texture
	if(pTex)
		delete []pTex;

	pTex = new vwTexture[nlayer];


	// Geometry
	float dx,dy;
	dx	= w_/((float)m);
	dy	= h_/((float)n);

	int i,j;
	for (j=0;j<=n;j++)
	for (i=0;i<=m;i++)
		pVer[(m+1)*j+i]	= dhVector(dx*i,dy*j,0);

	int np = 0;
	for (j=0;j<n;j++)
	for (i=0;i<m;i++)
	{
		int idx	= (m+1)*j+i;

		//int vertexIndices[3] = {faces.GetAt(i).v[0], faces.GetAt(i).v[1], faces.GetAt(i).v[2] };
		//pFace[np]

		
		pFace[np].Tri(pVer,idx,idx+1,idx+m+1);
		pVer2[idx].Add(&pFace[np]);
		pVer2[idx+1].Add(&pFace[np]);
		pVer2[idx+m+1].Add(&pFace[np]);
		np++;
		

		pFace[np].Tri(pVer,idx+1,idx+m+2, idx+m+1);
		pVer2[idx+1].Add(&pFace[np]);
		pVer2[idx+m+2].Add(&pFace[np]);
		pVer2[idx+m+1].Add(&pFace[np]);
		np++;
		

		//printf("IDX: %d F(%d,%d,%d) F(%d,%d,%d)\n",idx,  idx,idx+1,idx+m+1, idx+1,idx+m+2, idx+m+1);
	}

	FindGourad();


	g_n_div_x = m;
	g_n_div_y = n;
	g_nLayer = nlayer;

}



// FOR PLANE
void vwImageObj::SetUV(float xMin, float xMax, float yMin, float yMax)
{

	float n_div_x = g_n_div_x;
	float n_div_y = g_n_div_y;

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

	g_uv_max_x = xMax;
	g_uv_max_y = yMax;


}

void vwImageObj::SetUV(int image_w, int image_h)
{
	//1280 by 800

	int xp2 = ddNextPowerOfTwo(image_w);
	int yp2 = ddNextPowerOfTwo(image_h);

	float xmax = image_w/ (float) xp2;
	float ymax = image_h/ (float) yp2;


	printf("SetUV : x:%.3f, y:%.3f\n", xmax, ymax);//pTex[0].w

	SetUV(0, xmax, 0, ymax);

	

}

// Display outline
void vwImageObj::DisplayOutline(float thickness)
{
	
	glLineWidth(thickness); 

	int nx = g_n_div_x*2-1;
	int ny = (g_n_div_x*2) * (g_n_div_y -1);

	glBegin(GL_LINE_STRIP);
		glVertex3f(pFace[0].v[0]->x ,			pFace[0].v[0]->y ,		pFace[0].v[0]->z);
		glVertex3f(pFace[nx].v[0]->x,			pFace[nx].v[0]->y ,		pFace[nx].v[0]->z);
		glVertex3f(pFace[nFace-1].v[1]->x ,		pFace[nFace-1].v[1]->y,	pFace[nFace-1].v[1]->z);
		glVertex3f(pFace[ny].v[2]->x,			pFace[ny].v[2]->y ,		pFace[ny].v[2]->z);
		glVertex3f(pFace[0].v[0]->x ,			pFace[0].v[0]->y ,		pFace[0].v[0]->z);
	glEnd();



	glLineWidth(1); 



}

CPoint	vwImageObj::GetCursorPtInImageSpace(int nfaceid, float *bc,  BOOL bInverseY)
{

	int ta, tb, tc;
	ta = pFace[nfaceid].v[0] - pVer;
	tb = pFace[nfaceid].v[1] - pVer;
	tc = pFace[nfaceid].v[2] - pVer;


	float _u1 = pUV[2*ta];
	float _v1 = pUV[2*ta+1];

	float _u2 = pUV[2*tb];
	float _v2 = pUV[2*tb+1];

	float _u3 = pUV[2*tc];
	float _v3 = pUV[2*tc+1];

	float u = _u1 * bc[0] + _u2 * bc[1]+_u3 * bc[2];
	float v = _v1 * bc[0] + _v2 * bc[1]+_v3 * bc[2]; 


	u = u/g_uv_max_x;
	v = v/g_uv_max_y;
	


	CPoint ret;
	ret.x = u * pTex[0].w_ori;
	ret.y = v * pTex[0].h_ori;

	// Flip the y-axis because pixel coords increase toward the bottom.
	if(bInverseY)
		ret.y = pTex[0].h_ori - ret.y;

	

	return ret;
}
void vwImageObj::GetRGBAt(int nlayer, int nfaceid, float *bc, int &R, int &G, int &B)
{

	int ta, tb, tc;
	ta = pFace[nfaceid].v[0] - pVer;
	tb = pFace[nfaceid].v[1] - pVer;
	tc = pFace[nfaceid].v[2] - pVer;


	float _u1 = pUV[2*ta];
	float _v1 = pUV[2*ta+1];

	float _u2 = pUV[2*tb];
	float _v2 = pUV[2*tb+1];

	float _u3 = pUV[2*tc];
	float _v3 = pUV[2*tc+1];

	float u = _u1 * bc[0] + _u2 * bc[1]+_u3 * bc[2];
	float v = _v1 * bc[0] + _v2 * bc[1]+_v3 * bc[2]; 


	u = u/g_uv_max_x;
	v = v/g_uv_max_y;
	
	xRGB clr = pTex[nlayer].GetTexColorAtUV(u, v);

	R= GetRValue(clr); 
	G= GetGValue(clr);  
	B= GetBValue(clr); 



	//printf("%1.3f,%1.3f, R%3d, G%3d, B%3d, \r", u, v,R, G, B);


}

void vwImageObj::DrawObject(BOOL bWireFrame, BOOL bDrawVert)
{


	//glScalef(1,-1,-1);
	glEnable(GL_TEXTURE_2D);

	
	glBindTexture(GL_TEXTURE_2D,pTex[n_active].nTexID);

	

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
	
	//glScalef(1,-1,-1);
	//glScalef(1,-1,1);
}