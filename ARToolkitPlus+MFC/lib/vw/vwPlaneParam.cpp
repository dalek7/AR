#include "StdAfx.h"
#include "vwPlaneParam.h"

vwPlaneParam::vwPlaneParam()
{
	g_n_div_x = 0;
	g_n_div_y = 0;
}

// FOR PLANE
void vwPlaneParam::SetUV(float xMin, float xMax, float yMin, float yMax,  int n_div_x, int n_div_y)
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

	g_n_div_x = n_div_x;
	g_n_div_y = n_div_y;

}
void vwPlaneParam::SetUV(int n_div_x, int n_div_y)
{

	SetUV(0, 1, 0, 1,   n_div_x,  n_div_y);

}


void vwPlaneParam::MakePlaneWithGaussian2D(float sig, float m, float h)
{
	
	int i, j;
	//int dx = m_height.m_png.w / g_n_div_x;
	//int dy = m_height.m_png.h / g_n_div_y;
	int cntv = 0;
	int xc = (g_n_div_x)/2;
	int yc = (g_n_div_y)/2;

	for(i=0; i<g_n_div_y+1; i++)
	{
		for(j=0; j<g_n_div_x+1; j++)
		{

			int x = j - xc;
			int y = i - yc;

			//float g = x*x + y*y;
			//g = g*0.1;

			float g = h* gaussian2D(x, y, 0, 0, 2, 2);
			
				

			pVer[(g_n_div_y+1)*j + i].z =g;
			/*
			//GetTexColorAtUV
			float u1 = (float) j* 1.0 /  g_n_div_x;
			float v1 = (float) i* 1.0 /  g_n_div_y;

			
			
			float g1 = i;//
			pVer[cntv].z =g1;
			
			cntv++;
*/
			//Dbg("U%.2f, V%.2f : %.2f",u1, v1,  g1);
		}
	}

	FindGourad();

}

void vwPlaneParam::MakePlaneWithDispMap(char* fn, float g_)
{
	int ndiv = 30;
	//m_height.LoadDisplacement(fn);
	MakePlane(1, 1, ndiv, ndiv);
	SetUV(ndiv,ndiv);
	
	SetDisplacementMap(fn, g_);

}

void vwPlaneParam::MakePlane(float a,float b,int m,int n)
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
		//pFace[np++].Tri(pVer,idx,idx+1,idx+m+1);
		pFace[np++].Tri(pVer,idx+1,idx+m+2, idx+m+1);

		Dbg("IDX: %d F(%d,%d,%d) F(%d,%d,%d)",idx,  idx,idx+1,idx+m+1, idx+1,idx+m+2, idx+m+1);
	}

	FindGourad();
}

void vwPlaneParam::SetDisplacementMap(char* fname, float height_gain_)
{

	m_height.LoadDisplacement(fname);
	
	int i, j;
	//int dx = m_height.m_png.w / g_n_div_x;
	//int dy = m_height.m_png.h / g_n_div_y;
	int cntv = 0;
	for(i=0; i<g_n_div_y; i++)
	{
		for(j=0; j<g_n_div_x+1; j++)
		{
			//GetTexColorAtUV
			float u1 = (float) j* 1.0 /  g_n_div_x;
			float v1 = (float) i* 1.0 /  g_n_div_y;

			xRGB c1 = m_height.GetTexColorAtUV(u1,v1);
			
			int R = c1.r;
			//if(R>255) R = 0;
			
			float g1 = (float)R * height_gain_;
			pVer[cntv].z +=g1;
			cntv++;

			Dbg("U%.2f, V%.2f : %.2f",u1, v1,  g1);
		}
	}

	FindGourad();

}