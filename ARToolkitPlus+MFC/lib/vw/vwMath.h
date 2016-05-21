/*
 *  vwMath.h
 *  WorldTest
 *
 *  Created by Seung-Chan Kim on 6/28/11.
 *  Copyright 2011 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */

#pragma once

class vwMath
{
public:
	vwMath(void);
	~vwMath(void);


public:


	void CvToGL(float *v_cv, float *v_gl);
	void GLToCV(float *v_gl, float *v_cv);
};
