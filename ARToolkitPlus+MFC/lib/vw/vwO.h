
/*
 *  vwO.h
 *
 *  Created by Seung-Chan Kim on 2/20/11.
 *  Copyright 2011 KAIST. All rights reserved.
 *  Special thanks to JY Yang, KAIST
 *
 */


#ifndef __VWO__
#define __VWO__

#include "gl/gl.h"
#include "gl/glu.h"
#include "../3d/dhVector.h"
#include "../3d/dhMat.h"

#include "../ddMacros.h"
#include "../vArray.h"
#include "../Triangle.h"
#include "../ddMath.h"
#include "../DBG.h"

#include "vwTexture.h"
#include "vwCamera.h"

#include "vwMathGeometry.h"
#include "vwPrimitives.h"


#include "../PTRList.h"

typedef struct __FaceVertex
{
	int v[3];

} FaceVertex;


typedef struct __UV
{
	float u, v;

} UV;




typedef struct __Dist{ // Struct to store and order the values of the amplitudes preserving the index in the original array
    float dist;
    int index;
} Dist;
 

inline int compare_structs(const void *a, const void *b)
{
     
    Dist *struct_a = (Dist *) a;
    Dist *struct_b = (Dist *) b;
     
    if (struct_a->dist < struct_b->dist) return 1;
    else if (struct_a->dist == struct_b->dist) return 0;
    else return -1;
     
}
     
 




class vwPolygon
{
public:
		vwPolygon();
public:
		dhVector *v[3];
		void	Tri(dhVector*,int,int,int);
};

class vwVertex : public CPTRList
{
public:
		vwVertex();
		~vwVertex();
public:
		void	Add(vwPolygon*);
		void	Remove(vwPolygon*);
		void	Reset();

		
};


class vwO
{
public:
		vwO();
		~vwO();
public:
// startup
		void	Init(int,int);
		void	Close();
		
		GLUquadric* quadratic;
		
		void	MakePlane(float,float,int m=10,int n=10);
		void	MakeCylinder(float r,float R,float h,int n=36);
		void	DrawNormal();
		void	DrawPlaneNormal();
		virtual void	Draw(BOOL bDispAxis=FALSE, BOOL bWireFrame=FALSE, BOOL bDrawVert=FALSE);
		virtual void	DrawObject(BOOL bWireFrame, BOOL bDrawVert=FALSE);
		//void	DrawObject();


		void	FindFlat();
		void	FindGourad();

// coordinate
		void	Trans(dhVector);
		void	Trans(float,float,float);

		void	Move(float,float,float);

		void	Rot(float a,float b,float r);	// general coord.
		void	RotX(float);
		void	RotY(float);
		void	RotZ(float);

		void	Scale(float);
		void	Scale(dhVector);

// hierarchy
		void	Add(vwO*);


// texture
		vwTexture m_tx;
		void	SetUV(float xMin, float xMax, float yMin, float yMax,  int n_div_x, int n_div_y);
		void	SetUV(int n_div_x, int n_div_y);
		virtual int		GenTexture();


// Get Informatino
		dhVector GetSurfaceNormalMV(int nFaceID, dhVector vw_o, dhVector vw_r);
		dhVector GetSurfaceNormalMV(int nFaceID);

		dhVector GetSurfaceNormalInterpolatedAbs(int faceID, float *pArea);	//Interpolated one
		dhVector GetSurfaceNormalInterpolatedMV(int faceID, float *pArea);	//Interpolated one
		
		
		dhVector GetTouchedPointOnSurface(int faceID, float *pArea);

// Drawing		
		void		Draw3VertexNormals(int faceID, float *pArea, float scale=1.0);
		dhVector	DrawASurfaceNormalInterpolated(int faceID, float *pArea, float scale=1.0);
		void		DrawASurfaceNormal(int faceID, float scale, BOOL stickToMouse = FALSE);
		
		void		DrawAllSurfaceNormals(int face_selected=-1, float scale=1.0, int nJump=1);


		

		void	GetVertexNormals(int nFaceID, dhVector* v_vectors, BOOL bCompensateMV=FALSE);
		
// 2013/9/26

		virtual dhVector GetTouchedFace(CPoint, vwCamera *pCamera1, int &nface, float *bc, BOOL bInterpolateNormal=FALSE);	//override this for vwOBJ
		vwCamera *pCamera;

//2013/10/06
		void	MakeGaussianDeform(unsigned int vertexID_,  int noVert, float spread_, float height_, BOOL bUseModifed=FALSE, BOOL bUpdate=FALSE);	// gaussian
		dhVector	GetCenterInScreen(vwCamera*);//2D 
		dhVector	GetCenter();//2D 
		dhVector	objCenter;
		dhVector	geoCenter;


		BOOL bMovedOrRotated;		// TO ACCELERATE COMPUTATION.. to be implemented.
		void		SetCenterWithXY(vwCamera*, int x_screen, int y_screen);
		
public:
	Dist* darr;
	int kNearestVer;
	int nChkFace;
	int chkFace[100];
	int chkFaceUnique[100];


// 2013/07/18
		dhVector GetSurfaceNormal(int nFaceID);

		void	constructTBN(int nFaceID, dhMat &mat1, BOOL bUseModelView=FALSE, BOOL bInterp=FALSE);
		void	constructTBN(int nFaceID, dhVector &mT, dhVector &mB, dhVector &mN, BOOL bUseModelView=FALSE, BOOL bInterp=FALSE);
		void	dispTBN(int nFaceID, BOOL bInterp = FALSE);

		void	dispSurf(int nFaceID, dhVector offset_o=dhVector(0,0,0),BOOL bInterp = FALSE);

// Experimental
		int		model_x;
		int		model_y;
		int		model_z;

// Util
		dhVector p_weighted;	
		dhVector	GetANormalAbs(int faceID);
		void	GetTriangleVertices(int faceID, dhVector* pV, BOOL bApplyModelView = FALSE);
		


		dhVector	GetTriangleCenter(int nFaceID,BOOL bApplyModelView = FALSE);
public:
		void	DisplayOrigin();
		void	DrawLineWithArrow3D(dhVector stPt, dhVector endPt,BOOL arrowAtTheEnd = TRUE, float z_offset= 0, float scale_arrow= 1.0);
	

		dhMat		H;
		dhVector	o;
public:
		dhVector	*pVer;
		dhVector	*pVer0;

		int			nVer;
		dhVector	*pNormal;

		vwPolygon	*pFace;
		int			nFace;

		float		*pUV;	// 2*nVer.
		vArray<vwO*,vwO*>	m_child;



		vwVertex	*pVer2;	//20130128


		BOOL bUseSingleColor;
		void SetSingleColor(BOOL b1)
		{
			bUseSingleColor = b1;
		}

public:
		//KDT kv;




};


class vwBox : public vwO
{
public:
	void	MakeBox(float,float,float);
	void	DrawObject();

};


#endif