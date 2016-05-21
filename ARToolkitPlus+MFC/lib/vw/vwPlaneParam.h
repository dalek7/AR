#pragma once

#include "gl/gl.h"
#include "gl/glu.h"



#include "../png/CPImage.h"
#include "../xRGB.h"


#include "../ddmath.h"
#include "./vwO.h"

#include "vwDisplacement.h"

#include "vwMathGeometry.h"

class vwPlaneParam : public vwO
{
public:
	vwPlaneParam();

	void	SetUV(float xMin, float xMax, float yMin, float yMax,  int n_div_x, int n_div_y);
	void	SetUV(int n_div_x, int n_div_y);
	
	int g_n_div_x;
	int g_n_div_y;
	
	void	SetDisplacementMap(char* fn, float height_gain_);

	void	MakePlane(float,float,int m=10,int n=10);




public:
	// displacement
	void MakePlaneWithDispMap(char* fn, float g_=0.1);
	vwDisplacement m_height;


	void MakePlaneWithGaussian2D(float sig, float m = 0, float h = 1);


public:
	virtual void	Draw(BOOL bDispAxis=FALSE, BOOL bWireFrame=FALSE, BOOL bDrawVert=FALSE)
	{

		glPushMatrix();
		glTranslatef(-model_x/2.0, -model_y/2.0, 0);
		vwO::Draw( bDispAxis,  bWireFrame,  bDrawVert);
		glPopMatrix();

	}


	
	

// Virtual
	
};

