// October 1, 2013
// Written by Seung-Chan Kim @TCL, KAIST

#pragma once

#include "vwO.h"
#include "vwTexture.h"

class vwImageObj : public vwO
{


public:
	vwImageObj();
	~vwImageObj();

	void	CloseAllTextures();
public:
	vwTexture *pTex;


	vArray <FaceVertex, FaceVertex> face_textures;

public:
	void	Make(float w_,float h_,int nlayer=1, int m=10,int n=10);
	void	LoadTexture(int nlayer, char* fn);


	void	LoadTextureFromBuffer(int nlayer, unsigned char* pBuf, int w=512, int h=512, int nCh=3, BOOL bFlip=FALSE,  BOOL bBGR2RGB= FALSE);
	void	GetPngBuffer(int nlayer, BYTE* pOut);

	void	GenTextures();

	CPoint	GetCursorPtInImageSpace(int nfaceid, float *barycentric, BOOL bInverseY=TRUE);

	void	SetUV(float xMin, float xMax, float yMin, float yMax);
	void	SetUV(int image_w= 256, int image_h=256);

	void	GetRGBAt(int nlayer, int nfaceid, float *barycentric, int &R, int &G, int &B);

public:
	void	DisplayOutline(float thickness=2.0);
	
public:
	int		n_active;
	void	SetActiveLayer(int n)
	{
		n_active = n;
	}
	int		GetActiveLayer() 
	{
		return n_active;
	}
protected:
	int		g_n_div_x;
	int		g_n_div_y;
	int		g_nLayer;

	float	g_uv_max_x;
	float	g_uv_max_y;


public:
	virtual void	DrawObject(BOOL bWireFrame, BOOL bDrawVert=FALSE);


	void	ReloadTexture(int nlayer, char* fn);

};

