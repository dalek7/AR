/*
 *  vwPlane.h
 *
 *  Created by Seung-Chan Kim on 6/19/13.
 *  Copyright 2013 KAIST, Korea. All rights reserved.
 *
 */


#ifndef __mPlane__
#define __mPlane__



//#include "vwLine.h"
#include "vwLineSegment.h"
#include "vwO.h"


/*
// consider chaging code like this.
  typedef struct kmPlane {
	kmScalar 	a, b, c, d;
} kmPlane;


  */

class mPlane
{
public:
	//mPlane();
	mPlane( dhVector &normal,  dhVector &pt);
	mPlane( dhVector &pt1,  dhVector &pt2,  dhVector &pt3);
	mPlane(float a = 0.0,
                      float b = 1.0, 
                      float c = 0.0, 
                      float d = 0.0);

	mPlane(const mPlane &rhs) :
        m_normal(rhs.m_normal),
        m_d(rhs.m_d)
	{
	}

	virtual ~mPlane()
    {
    }
	
public:
	

	enum IntersectResult
    {
        kResultNone = 0,
        kResultFront,
        kResultBack
    };


public:

	//orthogonal basis
	void constructTBN(dhVector &mT, dhVector &mB, dhVector &mN);
	void constructTBN(dhMat &mat1);// TODO


	

	dhVector GetRotationAxis(dhVector originalVector=dhVector(0,0,1));
	float GetRotationAngle(dhVector originalVector=dhVector(0,0,1));
    bool operator==(mPlane const &rhs) 
    {
        return m_normal == rhs.m_normal && m_d == rhs.m_d;
    }
public:
	float a() {return m_normal.x;}
	float b() {return m_normal.y;}
	float c() {return m_normal.z;}
	float d() {return m_d;}
	 
    dhVector &normal() { return m_normal; }


public:

	
	//https://github.com/Kazade/kazmath/blob/master/kazmath/plane.h
	int PlaneClassifyPoint(dhVector& pt, mPlane* pIn=NULL);


//	void setOffset(EltType d) { m_d = d; }

    /* Set plane to be located at position indicated by normal and point. */
    void setPlane( dhVector &normal,  dhVector &pt);



    float perpDistance(const dhVector &pt);
	dhVector perpVector(const dhVector &pt);
   
	/* Returns projection of point p onto plane. */
    dhVector projectPoint(const dhVector &pt);

	 /* Returns projection of point p onto plane. Applies epsilon offset
       of point along normal. */
    dhVector projectPoint(const dhVector  &pt, const float epsilon);


	int intersectSegmentFrontAndBack(vwLineSegment &segment, float &t, const float epsilon);
	int intersectSegmentFront(vwLineSegment &segment, float &t, const float epsilon);
	int intersectSegmentBack(vwLineSegment &segment, float &t, const float epsilon);
	
	
/* The following line intersection routines test for plane intersection with
       an infinite line directed from pt1 through pt2.  Intersection tests can
       be performed against the front, back or front and back of the plane. */
    int intersectLineFront(vwLine &line, float &t, const float epsilon);
    int intersectLineBack(vwLine &line, float &t, const float epsilon);
    int intersectLineFrontAndBack(vwLine &line, float &t, const float epsilon);


	dhVector pointOnPlane() { return m_normal * -m_d; }  //"arbitrary" point
	void	GetFourPoints(dhVector &p1,dhVector &p2,dhVector &p3,dhVector &p4, float w=1.0, float h=1.0, BOOL bApplyH=FALSE);

	void GetAngle(float* angle_rtn);


	
	void anglesToAxes(const dhVector angles, dhVector& left, dhVector& up, dhVector& forward);
	
public:

    dhVector m_normal; // <a, b, c>
    float m_d;
	
public:
	void Update();
	void MakeObj();
	//virtual void	DrawObject();


};



class vwPlane : public vwO
{
public: 
	vwPlane();
	
public:
	mPlane	m_plane;
	
	void	InitPlane(dhVector sn, dhVector p);
	void	MakeVisiblePlane(float width=5.0,float height=5.0,int m=10,int n=10);
	void	MakeVisiblePlane(mPlane &plane1, float width=5.0,float height=5.0,int m=10,int n=10);


	void DrawSurfaceNormal();
	void DrawAxisOfRotation();
	void DisplayTBN(dhVector offset=dhVector(0,0,0));


	
	void MarkVert0(int nvert_=0, float r_=10);

	dhVector GetNormal();		//2013/11/12
};


#endif