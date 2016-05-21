/*
 *  vwLineSegment.cpp
 *
 *  Created by Seung-Chan Kim on 2/16/13.
 *  Copyright 2013 KAIST, Korea. All rights reserved.
 *  Written @ Cafe CHAUD 1186, Daejeon
 */


#include "stdafx.h"
#include "vwLineSegment.h"


vwLineSegment::vwLineSegment()
{
	x_= y_ = 0.5;
}


void vwLineSegment::Update()
{
	dhVector vec = m_p2 - m_p1;
	m_length = vec.Abs();

	m_unitDirection = vec.Unit();

	
}





//-----------------------------------------------------------------------------
//
// Function:    distFromSegment
//
// Description: Determines the minimum distance between two finite line segs
//              segment1 = p1 + t(p2 - p1)
//              segment2 = p3 + s(p4 - p3)
//
// Parameters:  p1 and p2 describe a segment in the direction of p1 -> p2
//              p3 and p4 describe a segment in the direction of p3 -> p4
//
// Returns:     The return value of the function is the minimum distance
//              between these two segments.
//
//              The t and s parameters will indicate the parameters of
//              minimum distance for segment1 and segment2 respectively.
//
//-----------------------------------------------------------------------------
float vwLineSegment::projectToParametric(dhVector &pt)
{

	if (length() == 0)
    {
        return 0;
    }
    else
    {
		dhVector tmpv = pt - m_p1;

        float t = tmpv.Dot(unitDirection()) / length();
        return ddClamp<float>(t, 0.0, 1.0);
    }


	return 0;
}

float vwLineSegment::distFromSegment(vwLineSegment &segment,
                                  float &t, float &s, 
                                  const float epsilon)
{
    // Determine closest t and s parameters between the two lines
    float dist = distFromLine(segment, t, s, epsilon);   

    if ((t >= 0.0) && (t <= 1.0) && (s >= 0.0) && (s <= 1.0))
    {
        // min distance occurred on the segments
        return dist;
    }
    else
    {
        // Now determine the closest points on the two line segments
        dhVector cp1, cp2;

        // min distance not on both segments, must choose minimum
        // distance from each of the segment endpoint to the other
        // segment
        float minDist = 99999999.0;//DBL_MAX
        float dist;
		dhVector tempPt;
        float tempParam;

		// Point 1 to line seg from points 3 to 4.
        tempParam = segment.projectToParametric(getStartPoint());
        tempPt = segment.evalSegment(tempParam);
        dist = getStartPoint().Distance(tempPt);
        if (dist <= minDist)
        {
            minDist = dist;

            t = 0.0;
            cp1 = getStartPoint();

            s = tempParam;            
            cp2 = tempPt;
        }

		// Point 2 to line seg from points 3 to 4.
        tempParam = segment.projectToParametric(getEndPoint());  
        tempPt = segment.evalSegment(tempParam);
        dist = getEndPoint().Distance(tempPt);
        if (dist <= minDist)
        {
            minDist = dist;

            t = 1.0;
            cp1 = getEndPoint();

            s = tempParam;            
            cp2 = tempPt;
        }

        // Point 3 to line seg from points 1 to 2.
        tempParam = projectToParametric(segment.getStartPoint());
        tempPt = evalSegment(tempParam);
        dist = segment.getStartPoint().Distance(tempPt);
        if (dist <= minDist)
        {
            minDist = dist;

            t = tempParam;
            cp1 = tempPt;

            s = 0.0;            
            cp2 = segment.getStartPoint();
        }

        // Point 3 to line seg from points 1 to 2.
        tempParam = projectToParametric(segment.getEndPoint());
        tempPt = evalSegment(tempParam);
        dist = segment.getEndPoint().Distance(tempPt);
        if (dist <= minDist)
        {
            minDist = dist;

            t = tempParam;
            cp1 = tempPt;

            s = 1.0;            
            cp2 = segment.getEndPoint();
        }


		return cp1.Distance(cp2);

	}
}


// Visual 
void vwLineSegment::SetRCSize(float x, float y)
{
	x_ = x;
	y_ = y;

}
void vwLineSegment::MakeObj()
{
	Update();
	Init(8,12);

	float a;
	float b;
	float c;

	c = length();
	a = x_;
	b = y_;

	MakeBox(a, b, c);

}

//http://lifeofaprogrammergeek.blogspot.kr/2008/07/rendering-cylinder-between-two-points.html
void vwLineSegment::DrawObject()
{
	glPushMatrix();
		

		dhVector st = getStartPoint();

		glTranslatef(st.x, st.y, st.z);

		dhVector udir = unitDirection();
		if(udir.z == 0)
		udir.z = EPSILON;

		float ax = 57.2957795*acos( udir.z );
		if ( udir.z < 0.0 )
			ax = -ax;
		float rx = -udir.y*udir.z;
		float ry = udir.x*udir.z;
		glRotatef(ax, rx, ry, 0.0);

		//DisplayOrigin();
		//vwBox::DrawObject();
		Draw(TRUE);

	glPopMatrix();
	
	
}