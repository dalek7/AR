#pragma once

#include "gl/gl.h"
#include "gl/glu.h"
#include "../3d/dhVector.h"
#include "../3d/dhMat.h"
#include "../3d/dhQuat.h"

#ifndef PI
#define PI	3.1415926535897932f
#endif

#ifndef DEG
#define DEG(a)          (180.*a/PI )
#endif

#ifndef RAD
#define RAD(a)          (PI*a/180.)
#endif

typedef struct __view
{
	float	n,f;
	float	angle;
	float	fAR;	// aspect ratio.
	float	w,h;
	
	dhVector	o;	// camera position == eye
	dhVector	r;	// rotation

	dhVector    eye;	// current position	 eye?
	
	dhVector	vDir;	// viewing vector = o - eye


} VIEW;


class vwCamera
{
public:
	vwCamera(void);
	~vwCamera(void);


public:
	void SetCamera(int, int);
	

	dhVector GetGLPos(int x, int y);
	dhVector GetGLPos(int x, int y, float z_ref);
	dhVector GetGLPosWithZ(int x, int y, float &z);	

	
	BOOL MouseMove(int nFlags,CPoint pt);
	void Pan(double h, double v);
	void Zoom(double depth);


	//20131025
	BOOL MouseMove2(int nFlags,CPoint pt);

	dhVector diff;
	dhVector	_anchor_position;
    dhVector	_current_position;
	dhMat		_rotMatrix;

	dhQuat		_quatStart;
    dhQuat		_quat;



	void	TouchesBegan(dhVector ptscreen);
	void	TouchesEnded(dhVector ptscreen);

	void	TouchesMoved(dhVector ptscreen);
	
	dhVector	projectOntoSurface(dhVector touchPoint);
	void	computeIncremental();

	
	//20130129
	void SetCamTrans(float x, float y, float z);	


	void SetViewport( int x, int y, int width, int height );

	void CopyMatrices();
	void CopyMatricesd(double *mm_,double *pm_, int* vp_ );


	void ApplyModelViewTransform();
	void ApplyModelViewTransform2(BOOL bForceIdentity=TRUE);//experimental
	

	void ApplyProjectionTransform();

	void Update();



	// from Openframeworks

	void lookAt(const dhVector& lookAtPosition, dhVector upVector = dhVector(0, 1, 0));
	void setGlobalOrientation(const dhQuat& q);

	//void setupPerspective(bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0, const dhVector & lensOffset2D = dhVector(0.0f, 0.0f));
	void setupPerspective(bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0, const dhVector & lensOffset2D = dhVector(0.0f, 0.0f));
	float fov;
	float nearClip;
	float farClip;
	dhVector lensOffset;
	bool forceAspectRatio;
	float aspectRatio; // only used when forceAspect=true, = w / h



	// projection properties:
	void setFov(float f);
	void setNearClip(float f);
	void setFarClip(float f);
	void setLensOffset(const dhVector & lensOffset2D);
	void setAspectRatio(float aspectRatio);
	void setForceAspectRatio(bool forceAspectRatio);





	GLdouble*	GetMM();
	GLdouble*	GetPM();
	int*	GetVP();

	dhVector	Projection(dhVector);
	void		UnProjection(dhVector, dhVector* output);
	dhVector	UnProjection(CPoint);
	void		UnProjection(dhVector pt_screen, double* out_glspace);

	dhVector	UnProjection2(dhVector pt_screen);
	void		UnProjection3(dhVector pt_screen, double* out_glspace);

	void		SaveDepthMap(float *p);

	dhMat		mat_mm, mat_pm;

	//dhMat retMM() { return mat_mm;)
public:
	VIEW	vw, vw_prev;
	VIEW	vw_ori;	//for restoring the view
	void	SaveView();
	void	RestoreView();

	BOOL	IsViewChanged();
	BOOL	bViewChanged;	//20131007
	


public:
	static dhVector	Projection_(double *mm1, double *pm1, int *vp1, dhVector);

protected:
	GLdouble	mm[16];
	GLdouble	pm[16];
	int			vp[4];
	

protected:
	CPoint	ptMouseOld;

};
