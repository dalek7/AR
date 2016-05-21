#pragma once

// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST

#include "gl.h"
#include "../FileName.h"

//#include "../KDT.h"
//#include "../ddSort.h"


//#include "../vw/vwFX.h"
#include "../vw/vwPlane.h"
#include "../vw/vwImageObj.h"

#include "../3d/dhQuat.h"
#include "../ARToolKitPlus/ofxARToolkitPlus.h"

// Accessing openCV
#include "../BaseCV.h"

class CGLTouch :
	public CGL
{
public:
	CGLTouch(void);
	~CGLTouch(void);

	
public:
	vwImageObj m_img1;


	ofxARToolkitPlus *pArtk;
	vector<ofPoint> displayImageCorners;

	float pr_gl[16];
	float pr_gl2[16];

public:
	void	InitValues();
	void	CreateObjects();
	void	ProcAtEveryFrame();
	void	DrawLayer0();
	void	DrawLayer1();
	void	DrawARStuff();

	void	GenTex();


	void	LoadObject(int nid);

public:
	


//Debug
public:

	void DispMatrices(double *mm,double *pm, int* vp);


	int nStepRotation;

	int gFaceSelected;
	float gArea[3];


// Interaction
public:
	dhVector ObjPos;
	dhVector ObjRot;
	void	ProcKey(UINT);
	
	

	void Close();

	

	


	int bPaintSelectedFace;
	void	TogglePaintSelectedFace();

	BOOL bInterpolateNormal;
	void ToggleInterpolateNormal();


	BOOL bShowTexColorInArrow;
	void ToggleShowTexColorInArrow();
	
	BOOL bUseSingleColor;

	void ToggleUseSingleColor();

	BOOL bShowGrid;
	void ToggleShowGrid();

	


	

	};
