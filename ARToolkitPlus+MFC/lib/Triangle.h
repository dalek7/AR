#if !defined(AFX_TRIANGLE_H__8F5299D2_1BBD_4E3B_AE88_E79D417A0FB2__INCLUDED_)
#define AFX_TRIANGLE_H__8F5299D2_1BBD_4E3B_AE88_E79D417A0FB2__INCLUDED_



/////////////////////////////////////////////////////////////////////////////
// CTriangle window

#include "3d/dhVector.h"
class CTriangle
{
// Construction
public:
	CTriangle();

// Attributes
public:

// Operations
public:

	static float ChkBarycentric(dhVector *ptVertex, dhVector clickedPos, float* area_rtn);
	static float ChkBarycentric(dhVector *ptVertex, CPoint clickedPos, float* area_rtn);
	static float ChkBarycentricf(dhVector *ptVertex, dhVector clickedPosGL, float* area_rtn);
	static float ChkBarycentricf2(dhVector *ptVertex, dhVector clickedPosGL, float* area_rtn);
	static float ChkBarycentricf2_pc(dhVector *ptVertex, dhVector clickedPosGL, float* area_rtn,  float w=1);

	static BOOL IsIn(dhVector *ptVertex, dhVector clickedPos);
	static BOOL IsInSameSide(dhVector p1, dhVector p2, dhVector ref1, dhVector ref2);

	static int ChkBarycentricf3(dhVector *ptVertex, dhVector clickedPosGL, float* area_rtn);
	//float ChkBarycentric(float *area);

	static  float CalcArea(float a, float b,float  c);

// Implementation
public:
	virtual ~CTriangle();


};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_TRIANGLE_H__8F5299D2_1BBD_4E3B_AE88_E79D417A0FB2__INCLUDED_)
