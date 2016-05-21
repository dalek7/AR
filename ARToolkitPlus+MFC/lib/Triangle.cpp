
#include "stdafx.h"
#include "Triangle.h"

/////////////////////////////////////////////////////////////////////////////
// CTriangle

CTriangle::CTriangle()
{
}

CTriangle::~CTriangle()
{
}

 
// In the GL space
float CTriangle::ChkBarycentricf(dhVector *ptVertex, dhVector clickedPosGL, float* area_rtn)
{
	dhVector mouse_pos(clickedPosGL.x, clickedPosGL.y,0);
	dhVector p0(ptVertex[0].x, ptVertex[0].y, 0);
	dhVector p1(ptVertex[1].x, ptVertex[1].y, 0);
	dhVector p2(ptVertex[2].x, ptVertex[2].y, 0);

	dhVector cV = p1 - p0;
	dhVector cW = p2 - p0;
	dhVector cU = cV *cW;

	float l0 = (p2- p1).Abs();
	float l1 = (p0- p2).Abs();
	float l2 = (p1- p0).Abs();


	float lc0 = (p0 - mouse_pos).Abs();
	float lc1 = (p1 - mouse_pos).Abs();
	float lc2 = (p2 - mouse_pos).Abs();

	float a0 = CalcArea(l0, lc1, lc2);
	float a1 = CalcArea(l1, lc0, lc2);
	float a2 = CalcArea(l2, lc0, lc1);


	dhVector cU0 = (p1 - mouse_pos)*(p2 - mouse_pos);
	dhVector cU1 = (p2 - mouse_pos)*(p0 - mouse_pos);
	dhVector cU2 = (p0 - mouse_pos)*(p1 - mouse_pos);

	float s0 = cU.Dot(cU0);
	float s1 = cU.Dot(cU1);
	float s2 = cU.Dot(cU2);

	if(s0>=0) s0 = 1;			else s0 = -1;
	if(s1>=0) s1 = 1;			else s1 = -1;
	if(s2>=0) s2 = 1;			else s2 = -1;


	float A = a0+a1+a2;//s0*a0+s1*a1+s2*a2;

	//float area[3];
	area_rtn[0] = s0*a0/A;
	area_rtn[1] = s1*a1/A;
	area_rtn[2] = s2*a2/A;

	int i;
	for(i=0; i<3; i++)
	{
		if(area_rtn[i]<0)
			return -1*i;
	}

	return 1;
}


BOOL CTriangle::IsInSameSide(dhVector p1, dhVector p2, dhVector ref1, dhVector ref2)
{
	dhVector v1 = (ref2-ref1) * (p1 - ref1);
	dhVector v2 = (ref2-ref1) * (p2 - ref1);

	if(v1.Dot(v2) >=0) 
		return TRUE;
	else 
		return FALSE;

}

BOOL CTriangle::IsIn(dhVector *ptVertex, dhVector p)
{

	BOOL c1 = IsInSameSide(p, ptVertex[0], ptVertex[1], ptVertex[2]);
	BOOL c2 = IsInSameSide(p, ptVertex[1], ptVertex[0], ptVertex[2]);
	BOOL c3 = IsInSameSide(p, ptVertex[2], ptVertex[0], ptVertex[1]);

	if(c1 && c2 && c3)
		return TRUE;
	else
		return FALSE;

}

float CTriangle::ChkBarycentric(dhVector *ptVertex, dhVector clickedPos, float* area_rtn)
{
	 CPoint clickedPos1;
	 clickedPos1.x = clickedPos.x;
	 clickedPos1.y = clickedPos.y;


	return ChkBarycentric(ptVertex, clickedPos1, area_rtn);
}

// In the GDI space
float CTriangle::ChkBarycentric(dhVector *ptVertex, CPoint clickedPos, float* area_rtn)
{
	dhVector mouse_pos(clickedPos.x, clickedPos.y,0);
	dhVector p0(ptVertex[0].x, ptVertex[0].y, 0);
	dhVector p1(ptVertex[1].x, ptVertex[1].y, 0);
	dhVector p2(ptVertex[2].x, ptVertex[2].y, 0);

	dhVector cV = p1 - p0;
	dhVector cW = p2 - p0;
	dhVector cU = cV *cW;

	float l0 = (p2- p1).Abs();
	float l1 = (p0- p2).Abs();
	float l2 = (p1- p0).Abs();


	float lc0 = (p0 - mouse_pos).Abs();
	float lc1 = (p1 - mouse_pos).Abs();
	float lc2 = (p2 - mouse_pos).Abs();

	float a0 = CalcArea(l0, lc1, lc2);
	float a1 = CalcArea(l1, lc0, lc2);
	float a2 = CalcArea(l2, lc0, lc1);


	dhVector cU0 = (p1 - mouse_pos)*(p2 - mouse_pos);
	dhVector cU1 = (p2 - mouse_pos)*(p0 - mouse_pos);
	dhVector cU2 = (p0 - mouse_pos)*(p1 - mouse_pos);

	float s0 = cU.Dot(cU0);
	float s1 = cU.Dot(cU1);
	float s2 = cU.Dot(cU2);

	if(s0>=0) s0 = 1;			else s0 = -1;
	if(s1>=0) s1 = 1;			else s1 = -1;
	if(s2>=0) s2 = 1;			else s2 = -1;


	float A = a0+a1+a2;//s0*a0+s1*a1+s2*a2;

	//float area[3];
	area_rtn[0] = s0*a0/A;
	area_rtn[1] = s1*a1/A;
	area_rtn[2] = s2*a2/A;

	int i;
	for(i=0; i<3; i++)
	{
		if(area_rtn[i]<0)
			return -1*i;
	}

	return 1;
}


float CTriangle::CalcArea(float a, float b,float  c)
{
	float s = 0.5*(a+b+c);
	return 0.25*sqrt( s*(s-a)*(s-b)*(s-c));

}	


// To be fixed
int CTriangle::ChkBarycentricf3(dhVector *ptVertex, dhVector ptCur, float* area_rtn)
{

	int ret;
	// Compute vectors        
	dhVector u = ptVertex[1]	- ptVertex[0];
	dhVector v = ptVertex[2]	- ptVertex[0];
	dhVector w = ptCur			- ptVertex[0];

	

	if( (v*w).Dot(v*u) < 0)
        ret = -1;

	else 
		ret = 1;


	float r = (v*w).Abs() / (v*u).Abs();
	float t = (u*w).Abs() / (u*v).Abs();


	// Compute barycentric coordinates

	area_rtn[0] = 1-r-t;
	area_rtn[1] = r;
	area_rtn[2] = t;

	// Check if point is in triangle
	return ret;///(u >= 0) && (v >= 0) && (u + v < 1);

}
//http://blogs.msdn.com/b/rezanour/archive/2011/08/07/barycentric-coordinates-and-point-in-triangle-tests.aspx




float CTriangle::ChkBarycentricf2(dhVector *ptVertex, dhVector ptCur, float* area_rtn)
{

	// Compute vectors        
	dhVector v0 = ptVertex[2] - ptVertex[0];
	dhVector v1 = ptVertex[1] - ptVertex[0];
	dhVector v2 = ptCur - ptVertex[0];

	// Compute dot products
	float dot00 = v0.Dot(v0);
	float dot01 = v0.Dot(v1);
	float dot02 = v0.Dot(v2);
	float dot11 = v1.Dot(v1);
	float dot12 = v1.Dot(v2);

	// Compute barycentric coordinates
	float invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;


	area_rtn[0] = 1-u-v;;
	area_rtn[1] = v;
	area_rtn[2] = u;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);

	

}
//http://www.blackpawn.com/texts/pointinpoly/default.html



float CTriangle::ChkBarycentricf2_pc(dhVector *ptVertex, dhVector ptCur,  float* area_rtn, float w)
{

	// Compute vectors        
	dhVector v0 = ptVertex[2] - ptVertex[0];
	dhVector v1 = ptVertex[1] - ptVertex[0];
	dhVector v2 = ptCur - ptVertex[0];

	// Compute dot products
	float dot00 = v0.Dot(v0);
	float dot01 = v0.Dot(v1);
	float dot02 = v0.Dot(v2);
	float dot11 = v1.Dot(v1);
	float dot12 = v1.Dot(v2);

	// Compute barycentric coordinates
	float invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;


	//float w = 1.0 / v0.z;

	area_rtn[0] = 1-u-v;
	area_rtn[1] = v;
	area_rtn[2] = u;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);

	

}

/*
float CTriangle::ChkBarycentric(float *area)
{
	dhVector mouse_pos(clickedPos.x, clickedPos.y,0);
	dhVector p0(ptVertex[0].x, ptVertex[0].y, 0);
	dhVector p1(ptVertex[1].x, ptVertex[1].y, 0);
	dhVector p2(ptVertex[2].x, ptVertex[2].y, 0);

	dhVector cV = p1 - p0;
	dhVector cW = p2 - p0;
	dhVector cU = cV *cW;

	float l0 = (p2- p1).Abs();
	float l1 = (p0- p2).Abs();
	float l2 = (p1- p0).Abs();


	float lc0 = (p0 - mouse_pos).Abs();
	float lc1 = (p1 - mouse_pos).Abs();
	float lc2 = (p2 - mouse_pos).Abs();

	float a0 = CalcArea(l0, lc1, lc2);
	float a1 = CalcArea(l1, lc0, lc2);
	float a2 = CalcArea(l2, lc0, lc1);


	dhVector cU0 = (p1 - mouse_pos)*(p2 - mouse_pos);
	dhVector cU1 = (p2 - mouse_pos)*(p0 - mouse_pos);
	dhVector cU2 = (p0 - mouse_pos)*(p1 - mouse_pos);

	float s0 = cU.Dot(cU0);
	float s1 = cU.Dot(cU1);
	float s2 = cU.Dot(cU2);

	if(s0>=0) s0 = 1;			else s0 = -1;
	if(s1>=0) s1 = 1;			else s1 = -1;
	if(s2>=0) s2 = 1;			else s2 = -1;


	float A = a0+a1+a2;//s0*a0+s1*a1+s2*a2;

	area[0] = s0*a0/A;
	area[1] = s1*a1/A;
	area[2] = s2*a2/A;

	int i;
	for(i=0; i<3; i++)
	{
		if(area[i]<0)
			return -1*i;
	}

	return 1;
}
*/
