/*
 *  vwMath.cpp
 *  WorldTest
 *
 *  Created by Seung-Chan Kim on 6/28/11.
 *  Copyright 2011 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */

#include "StdAfx.h"
#include "vwMath.h"

vwMath::vwMath(void)
{
}

vwMath::~vwMath(void)
{
}

void vwMath::GLToCV(float *v_gl, float *v_cv)
{
	memset(v_cv, 0, sizeof(float)*9);
	
	v_cv[0] = v_gl[0];
	v_cv[1] = v_gl[4];
	v_cv[2] = v_gl[12];
	v_cv[3] = v_gl[1];
	v_cv[4] = v_gl[5];
	v_cv[5] = v_gl[13];
	v_cv[6] = v_gl[3];
	v_cv[7] = v_gl[7];
	v_cv[8] = v_gl[15];

}

void vwMath::CvToGL(float *v_cv, float *v_gl)
{
	//memset(v_gl, 0, sizeof(float)*16);


	v_gl[0] = v_cv[0];
	v_gl[1] = v_cv[3];
	v_gl[4] = v_cv[1];
	v_gl[5] = v_cv[4];
	v_gl[3] = v_cv[6];
	v_gl[7] = v_cv[7];
	v_gl[12] = v_cv[2];
	v_gl[13] = v_cv[5];

}
