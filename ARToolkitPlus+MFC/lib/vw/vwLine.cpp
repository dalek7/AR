/*
 *  vwLine.cpp
 *
 *  Created by Seung-Chan Kim on 2/16/13.
 *  Copyright 2013 KAIST, Korea. All rights reserved.
 *  Written @ Cafe CHAUD 1186, Daejeon
 */


#include "stdafx.h"
#include "vwLine.h"

#include "../3d/dhVector.h"
#include "../3d/dhMat.h"
#include "../ddmath.h"

vwLine::vwLine()
{
	
	
}

vwLine::vwLine(dhVector &p1, dhVector &p2)
{
	m_p1 = p1;
	m_p2 = p2;

	

}

vwLine::~vwLine()
{
	Close();
}


void vwLine::Close()
{


	OutputDebugString("vwLine::Close()");

}



dhVector vwLine::direction()
{
	 return m_p2 - m_p1;
}

/* Evaluates parametric form of line at t and returns point at f(t). */
dhVector vwLine::eval(float t)
{	
	return m_p1 + direction().Scaling(t);//m_p1 + t * direction();
}


// Project pt on line and return point of projection.
dhVector vwLine::project(dhVector &pt)
{
	dhVector p0 = getPointOnLine();
    dhVector v = pt - p0;
    dhVector u = unitDirection();

	float udotv = u.Dot(v);

	dhVector ret = p0 + u.Scaling(udotv);
	return ret;
}

BOOL vwLine::isParallel(vwLine &line, const float epsilon)
{
	dhVector lineDir1 = direction();
    dhVector lineDir2 = line.direction();

    // These lines are parallel if the absolute value of the dot product of the
    // two vectors are close to 1.0 within some tolerance
    float numer = fabs(lineDir1.Dot(lineDir2));
    float denom = lineDir1.Abs() * lineDir2.Abs();

    if (denom <= epsilon * numer)  // <=, not <, in case both are 0
        return TRUE;  // if degenerate, the app probably wants the special case

    float val = numer / denom;

    return ddIsEqual<float>(1.0, val, epsilon); 

}


float vwLine::distPointLine(dhVector &pt)
{
	float dist;
	dhVector ptl = project(pt);
	dist = sqrt(	(pt.x - ptl.x)*(pt.x - ptl.x) +
					(pt.y - ptl.y)*(pt.y - ptl.y) +
					(pt.z - ptl.z)*(pt.z - ptl.z));

	return dist;

}


//-----------------------------------------------------------------------------
//
// Function:    distFromLine
//
// Description: Determines the minimum distance between two infinite lines
//              Line1 = p1 + t(p2 - p1)
//              Line2 = p3 + s(p4 - p3)
//
// Parameters:  p1 and p2 describe a line in the direction of p1 -> p2
//              p3 and p4 describe a line in the direction of p3 -> p4
//
// Returns:     The return value of the function is the minimum distance
//              between these two lines.
//
//              The t and s parameters will indicate the parameters of
//              minimum distance for line1 and line2 respectively.
//-----------------------------------------------------------------------------


float vwLine::distFromLine(vwLine& line, float &t, float &s, const float epsilon)
{
	float ret = 0;


	dhVector v = direction();
    dhVector d = line.direction();
    dhVector w = getPointOnLine() - line.getPointOnLine();

	 // Case 1 applies if v is not parallel to d. Test this by comparing
    // the dot product of the two vectors to 1.0 within some tolerance
    if (!isParallel(line, epsilon))
    {
        double d_dot_v = d.Dot(v);
        double d_dot_d = d.Dot(d);
        double v_dot_v = v.Dot(v);
        double d_dot_w = d.Dot(w);
        double v_dot_w = v.Dot(w);

        double den = v_dot_v * d_dot_d - d_dot_v * d_dot_v;
        
        if (den)
        {
            t = (d_dot_v * d_dot_w - d_dot_d * v_dot_w) / den;
            s = (v_dot_v * d_dot_w - d_dot_v * v_dot_w) / den;
        }
        else
        {
            // Not sure how to handle a divide by zero case. Seems like
            // returning DBL_MAX is sufficient 
            return 9999999;
        }
    }
	// Case 2 applies if v is parallel to d
	/*
    else 
    {
        // No unique solution exists, so just set t = 0 and let s be the
        // projection of line1(t) onto line2.

        t = 0;
        hduLineSegment<T> line1(m_p1, m_p2);
        hduLineSegment<T> line2(line.m_p1, line.m_p2);
        s = line2.projectToParametric(line1.eval(t));

        // Make sure we don't wind up with a wild result
        s = ddClamp<float>(s, 0.0, 1.0);
    }
	*/

	dhVector c1 = eval(t);
    dhVector c2 = line.eval(s);

    return c1.Distance(c2);

}
////////////////////////////////////////////////////////////
