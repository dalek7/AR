#pragma once


#include "ofVectorMath.h"

#include "gl/gl.h"
#include "gl/glu.h"
#pragma comment(lib, TEXT("glu32.lib"))
#pragma comment(lib, TEXT("opengl32.lib"))



inline void ofRect(float x,float y,float w,float h)
{
	//ofRect(x, y, 0.0f, w, h);

	glBegin(GL_LINE_LOOP);//start drawing a line loop
      glVertex3f(x,y,0.0f);//left of window
      glVertex3f(x+w, y,0.0f);//bottom of window
      glVertex3f(x+w, y+h,0.0f);//right of window
      glVertex3f(x, y+h,0.0f);//top of window
    glEnd();//end drawing of line loop

}	

/*
if (ofGetStyle().rectMode == OF_RECTMODE_CORNER){
		cairo_move_to(cr,x,y);
		cairo_line_to(cr,x+w, y);
		cairo_line_to(cr,x+w, y+h);
		cairo_line_to(cr,x, y+h);
	}else{
		cairo_move_to(cr,x-w/2.0f, y-h/2.0f);
		cairo_line_to(cr,x+w/2.0f, y-h/2.0f);
		cairo_line_to(cr,x+w/2.0f, y+h/2.0f);
		cairo_line_to(cr,x-w/2.0f, y+h/2.0f);
	}


*/