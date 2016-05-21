/*
 *  vwPlane.cpp
 *
 *  Created by Seung-Chan Kim on 6/19/13.
 *  Copyright 2013 KAIST, Korea. All rights reserved.
 *
 */

#include "stdafx.h"
#include "vwPlane.h"


vwPlane::vwPlane()
{
	
}

void vwPlane::InitPlane(dhVector sn, dhVector p)
{
	int m=1;
	int n=1;

	Init( (m+1)*(n+1),2*m*n);

	m_plane.setPlane(sn, p);

}

void vwPlane::MarkVert0(int nvert_, float r_)
{
	glPushMatrix();
	glMultMatrixf(H.v);

		//glLoadIdentity();
		//glMultMatrixf(H.v);
		glTranslatef(pVer[nvert_].x, pVer[nvert_].y, pVer[nvert_].z);
		drawSphere(1, r_, r_);

	glPopMatrix();
	//pVer[0]
	//m_plane.GetFourPoints(p1, p2, p3, p4);



}

dhVector vwPlane::GetNormal()
{
	dhVector ret =  H*	m_plane.normal();

	return ret;
}

// http://www.gamedev.net/topic/340803-drawing-a-plane-as-a-quad/
void vwPlane::MakeVisiblePlane(float width,float height,int m,int n)
{
	/*
	//basic plane
	m=1;
	n=1;

	Init( (m+1)*(n+1),2*m*n);
	*/

	// plane is in the implicit form 
	// n * P  + d = 0  

	dhVector p1, p2, p3, p4;
	m_plane.GetFourPoints(p1, p2, p3, p4, width, height);

	pVer[0] = p1;
	pVer[1] = p2;
	pVer[2] = p3;
	pVer[3] = p4;

	pFace[0].Tri(pVer, 0, 1, 3);
	pFace[1].Tri(pVer, 1, 2, 3);

	//FindGourad();

}

void vwPlane::DisplayTBN(dhVector offset)
{
	dhVector T, B, N;
	m_plane.constructTBN(T, B, N);

	T= T.Scaling(5);
	B= B.Scaling(5);
	dhVector p0 = offset;//m_plane.pointOnPlane();

	T = p0+T;
	B = p0+B;



	glDisable(GL_LIGHTING);
	
	glLineWidth(3.0f);

		glColor3f(0,1,1);
		glBegin(GL_LINES);
		glVertex3f(p0.x, p0.y, p0.z); // origin of the line
		glVertex3f(T.x, T.y, T.z); // ending point of the line
		glEnd();

		glColor3f(1,0,1);
		glBegin(GL_LINES);
		glVertex3f(p0.x, p0.y, p0.z); // origin of the line
		glVertex3f(B.x, B.y, B.z); // ending point of the line
		glEnd();

	glPopMatrix();

	glLineWidth(1.f);//reset
	

}
/*

void vwPlane::DrawAxisOfRotation()
{
	dhVector axis1 =	m_plane.GetRotationAxis();
	dhVector p0 = m_plane.pointOnPlane();
	axis1 = axis1.Scaling(5.0);


	glDisable(GL_LIGHTING);
	glColor3f(0,1,1);
	glLineWidth(3.0f);
		glBegin(GL_LINES);
		
		glVertex3f(p0.x, p0.y, p0.z); // origin of the line
		glVertex3f(axis1.x, axis1.y, axis1.z); // ending point of the line
		glEnd();

	glPopMatrix();


	glLineWidth(1.f);//reset


}
*/
void vwPlane::DrawSurfaceNormal()
{

	glPushMatrix();
	
	dhVector normal1 = m_plane.normal();
	normal1 = normal1.Scaling(5.0);

	dhVector p0 = m_plane.pointOnPlane();

	glDisable(GL_LIGHTING);
	glColor3f(1,0,0);
	glLineWidth(3.0f);
		glBegin(GL_LINES);
		
		glVertex3f(p0.x, p0.y, p0.z); // origin of the line
		glVertex3f(normal1.x, normal1.y, normal1.z); // ending point of the line
		glEnd();

	glPopMatrix();


	glLineWidth(1.f);//reset
}

/*
void vwPlane::GetAngle(float* angle_rtn)
{
	
}
*/

// Constructor

//mPlane(const dhVector &normal, const dhVector &pt);
mPlane::mPlane( dhVector &pt1,  dhVector &pt2,  dhVector &pt3)
{

	dhVector v1 = pt2 - pt1;
	dhVector v2 = pt3 - pt1;
    
    m_normal = v1*v2;
    m_normal = m_normal.Unit();
    
    m_d = -(m_normal.Dot(pt1));
}

mPlane::mPlane(float a, float b, float c, float d) : 
				m_normal(a, b, c),
				m_d(d)
{


	float length = m_normal.Abs();

	if (length > 0.0)
    {
        m_d /= length;
        //m_normal /= length;
		m_normal = m_normal.Unit();

    }

	
}

mPlane::mPlane(dhVector &normal,
                   dhVector &pt)
{
    setPlane(normal, pt);
}



void mPlane::setPlane( dhVector &normal,  dhVector &pt)
{
	m_normal = normal;
    m_normal = m_normal.Unit();
    
    m_d = -(m_normal.Dot(pt));

}


void mPlane::Update()
{
	

}




//http://stackoverflow.com/questions/13199126/find-opengl-rotation-matrix-for-a-plane-given-the-normal-vector-after-the-rotat
//RotationAxis = cross(N, N')
dhVector mPlane::GetRotationAxis(dhVector originalVector)
{

	dhVector ret;
	ret = originalVector * normal();

	return ret;
}


// Returns 1 if pP is infront of pIn. 
// Returns -1 
// Returns 0 otherwise

int mPlane::PlaneClassifyPoint(dhVector& pt, mPlane* pIn)
{
	if(pIn == NULL) pIn = this;

   // This function will determine if a point is on, in front of, or behind
   // the plane.  First we store the dot product of the plane and the point.

   float distance = pIn->a() * pt.x + pIn->b() * pt.y + pIn->c() * pt.z + pIn->d();

   // Simply put if the dot product is greater than 0 then it is infront of it.
   // If it is less than 0 then it is behind it.  And if it is 0 then it is on it.
   if(distance > 0.001) return 1;
   if(distance < -0.001) return -1;

   return 0;
}



float mPlane::GetRotationAngle(dhVector originalVector)
{
	float n= originalVector.Dot(normal()) / originalVector.Abs() / normal().Abs();
	float ret = acos(n);
	return ret;
}


void mPlane::constructTBN(dhMat &mat1)
{
	dhVector u,v;
	dhVector tmp = normal() - dhVector(0,0,1);
	
	if (ddIsEqual<float>(tmp.Abs(), 0.0, 0.001))
    {        
        u.Vector(1,0,0);
		v.Vector(0,1,0);
    }
	else
	{
		dhVector p1n = normal();
		u = p1n * dhVector(0,0,1);

		u= u.Unit();
/*
		if(u.y<-0.5)
		{
			dhMat rot1;
			rot1.I();
			rot1 = rot1.RotZ(-90);
			u = rot1 * u;
		}
*/

		v = p1n * u;

		u= u.Unit();
		v= v.Unit();

		//Case 1
		if(u.y<-0.5)
		{
			dhVector tt = u;
			u = v;
			v.Vector(tt.x, -tt.y, tt.z);

		}

	}
	dhVector n = normal().Unit();
	mat1.v[0] = u.x;
	mat1.v[1] = u.y;
	mat1.v[2] = u.z;

	mat1.v[4] = v.x;
	mat1.v[5] = v.y;
	mat1.v[6] = v.z;

	mat1.v[8] = n.x;
	mat1.v[9] = n.y;
	mat1.v[10] = n.z;

}
/*

u = p1n * dhVector(0,0,1);
		u= u.Unit();

		if(u.y<-0.5)
		{
			dhMat rot1;
			rot1.I();
			rot1.RotZ(90);
			u = rot1 * u;
			



*/

/*
void vwPlane::dispTBN()
{
	

	dhVector T, B, N;
	constructTBN(T, B, N);

	//if(

	T= T.Scaling(1);
	B= B.Scaling(1);

	

	//GetSurfaceNormalInterpolatedAbs(nFaceID, pArea);	
	
	dhVector triangle_o;
	triangle_o.Vector(0,0,0);
	
	//T = triangle_o + T;
	//B = triangle_o + B;

	// return;
	glColor3f(0,0,1);
	DrawLineWithArrow3D(triangle_o, T, TRUE, 0,0.3);

	glColor3f(1,1,0);
	DrawLineWithArrow3D(triangle_o, B, TRUE, 0,0.3);
	//dhVector p0 = m_plane.pointOnPlane();


}
*/
// not actual TBN. 
void mPlane::constructTBN(dhVector &mT, dhVector &mB, dhVector &mN)
{
	dhVector u,v;
	dhVector tmp = normal() - dhVector(0,0,1);
	
	if (ddIsEqual<float>(tmp.Abs(), 0.0, 0.001))
    {        
        u.Vector(1,0,0);
		v.Vector(0,1,0);
    }
	else
	{
		dhVector p1n = normal();
		//u = p1n * dhVector(0,0,1);	// ok..but discontinuos
		u = p1n * dhVector(0,-1,0);	//ok..but
		//u = p1n * dhVector(1,0,0);		////same as above
		v = p1n * u;

		u= u.Unit();
		v= v.Unit();

		
	}


	mT = u;
	mB = v;
	mN = normal().Unit();
	

}

void mPlane::GetFourPoints(dhVector &p1,dhVector &p2,dhVector &p3,dhVector &p4, float w, float h, BOOL bApplyH)
{
	
	dhVector u,v;
	dhVector tmp = normal() - dhVector(0,0,1);
	

	if (ddIsEqual<float>(tmp.Abs(), 0.0, 0.001))
    {        
        u.Vector(1,0,0);
		v.Vector(0,1,0);
    }
	else
	{
		dhVector p1n = normal();
		u = p1n * dhVector(0,0,1);
		v = p1n * u;

		u= u.Unit();
		v= v.Unit();

	}

	//// "arbitrary" point
	dhVector p0 = pointOnPlane();

	dhVector fu =  u * w;
	dhVector fv =  v * h;

	p1 = p0 - fu - fv;
	p2 = p0 + fu - fv;
	p3 = p0 + fu + fv;
	p4 = p0 - fu + fv;

	if(bApplyH)
	{
		;

	}
}


//http://mathworld.wolfram.com/Point-PlaneDistance.html
//Assumes that the normal is unit length.
 /* Returns ax + by + cz + d. */
	
float mPlane::perpDistance(const dhVector &pt) 
{

	return m_normal.Dot(pt) + m_d;
}


dhVector mPlane::perpVector(const dhVector &pt)
{

	return m_normal*perpDistance(pt);

}


dhVector mPlane::projectPoint(const dhVector &pt) 
{
    return (dhVector) pt - perpVector(pt);
}


dhVector mPlane::projectPoint(const dhVector  &pt, const float epsilon)
{
	// add offset
	return projectPoint(pt) + m_normal * epsilon;
}


int mPlane::intersectSegmentFront(vwLineSegment &segment, float &t, const float epsilon)
{
	// No intersection if the start point is above the plane
    // Epsilon tolerance is used to allow for near-miss cases 

	float distToStartPt = perpDistance(segment.getStartPoint());    
    if (distToStartPt < 0.0 - epsilon)
    {
        return kResultNone;
    }

    // No intersection if the end point is above the plane
    // Epsilon tolerance is used to allow for near-miss cases 
	float distToEndPt = perpDistance(segment.getEndPoint());
    if (distToEndPt > 0.0 + epsilon)
    {        
        return kResultNone;
    }
    
    float ndotu = m_normal.Dot(segment.unitDirection());

    // If line is perp. with normal then line does not intersect
    if (ddIsEqual<float>(ndotu, 0.0, epsilon))
    {        
        return kResultNone;
    }

    float ndotv = ndotu * segment.length();
    t = (-distToStartPt) / ndotv;
    
	return kResultFront;

}




int mPlane::intersectSegmentBack(vwLineSegment &segment, float &t, const float epsilon)
{
	
	// No intersection if the start point is above the plane
    // Epsilon tolerance is used to allow for near-miss cases 
    float distToStartPt = perpDistance(segment.getStartPoint());
    if (distToStartPt > 0.0 + epsilon)
    {
        return kResultNone;
    }

    // No intersection if the end point is above the plane
    // Epsilon tolerance is used to allow for near-miss cases 
    float distToEndPt = perpDistance(segment.getEndPoint());
    if (distToEndPt < 0.0 - epsilon)
    {        
        return kResultNone;
    }

    float ndotu = m_normal.Dot(segment.unitDirection());

    // If line is perp. with normal then line does not intersect
    if (ddIsEqual<float>(ndotu, 0.0, epsilon))
    {
        return kResultNone;
    }

    float ndotv = ndotu * segment.length();        
    t = (-distToStartPt) / ndotv;
    return kResultBack;
}

int mPlane::intersectSegmentFrontAndBack(vwLineSegment &segment, float &t, const float epsilon)
{

	float distToStartPt		= perpDistance(segment.getStartPoint());
    float distToEndPt		= perpDistance(segment.getEndPoint());

	int result = kResultNone;


	if ((distToStartPt > 0.0 - epsilon) && (distToEndPt < 0.0 + epsilon))
    {        
        result = kResultFront;
    }
    // Test if the start is below the plane and end is above the plane.
    // Epsilon tolerance is used to allow for near-miss cases 
    else if ((distToStartPt < 0.0 + epsilon) && (distToEndPt > 0.0 - epsilon))
    {
        result = kResultBack;
    }
    else
    {
        return kResultNone;
    }


	// Now perform the standard intersection code
    float ndotu = m_normal.Dot(segment.unitDirection());
	
	// If line is perp. with normal then line does not intersect
    if (ddIsEqual<float>(ndotu, 0.0, epsilon))
    {
        return kResultNone;
    }


	float ndotv = ndotu * segment.length();
    
    t = (-distToStartPt) / ndotv;
		

	return result;
	
}

int mPlane::intersectLineFront(vwLine &line, float &t, const float epsilon)
{
	const dhVector v	= line.direction();
    float ndotv			= m_normal.Dot(v);
    
    // If line is perp. with normal then line does not intersect
    // Also make sure that the line is intersecting from the front
    if (ddIsEqual<float>(ndotv, 0.0, epsilon) || ndotv > 0.0)
    {
        t = 0;
        return kResultNone;
    }
    
    t = -perpDistance(line.getPointOnLine()) / ndotv;

	return kResultFront;
}


int mPlane::intersectLineBack(vwLine &line, float &t, const float epsilon)
{
	const dhVector v = line.direction();
    float ndotv = m_normal.Dot(v);
    
    // If line is perp. with normal then line does not intersect
    // Also make sure that the line is intersecting from the front
    if (ddIsEqual<float>(ndotv, 0.0, epsilon) || ndotv < 0.0)
    {
        t = 0;
        return kResultNone;
    }
        
    t = -perpDistance(line.getPointOnLine()) / ndotv;
    return kResultBack;

}
int mPlane::intersectLineFrontAndBack(vwLine &line, float &t, const float epsilon)
{
	const dhVector v = line.direction();
    float ndotv = m_normal.Dot(v);
    
    int result = kResultNone;

    // If line is perp. with normal then line does not intersect
    if (ddIsEqual<float>(ndotv, 0.0, epsilon))
    {
        t = 0;
        return kResultNone;
    }
    
    // Check the directionality of the line relative to the plane
    if (ndotv < 0.0)
    {
        result = kResultFront;        
    }
    else
    {
        result = kResultBack;
    }

    t = -perpDistance(line.getPointOnLine()) / ndotv;
    return result;
}




///////////////////////////////////////////////////////////////////////////////
// convert Euler angles(x,y,z) to axes(left, up, forward)
// Each column of the rotation matrix represents left, up and forward axis.
// The order of rotation is Roll->Yaw->Pitch (Rx*Ry*Rz)
// Rx: rotation about X-axis, pitch
// Ry: rotation about Y-axis, yaw(heading)
// Rz: rotation about Z-axis, roll
//    Rx           Ry          Rz
// |1  0   0| | Cy  0 Sy| |Cz -Sz 0|   | CyCz        -CySz         Sy  |
// |0 Cx -Sx|*|  0  1  0|*|Sz  Cz 0| = | SxSyCz+CxSz -SxSySz+CxCz -SxCy|
// |0 Sx  Cx| |-Sy  0 Cy| | 0   0 1|   |-CxSyCz+SxSz  CxSySz+SxCz  CxCy|
///////////////////////////////////////////////////////////////////////////////
//http://www.songho.ca/opengl/gl_anglestoaxes.html

void mPlane::anglesToAxes(const dhVector angles, dhVector& left, dhVector& up, dhVector& forward)
{
    const float DEG2RAD = 3.141593f / 180;
    float sx, sy, sz, cx, cy, cz, theta;

    // rotation angle about X-axis (pitch)
    theta = angles.x * DEG2RAD;
    sx = sinf(theta);
    cx = cosf(theta);

    // rotation angle about Y-axis (yaw)
    theta = angles.y * DEG2RAD;
    sy = sinf(theta);
    cy = cosf(theta);

    // rotation angle about Z-axis (roll)
    theta = angles.z * DEG2RAD;
    sz = sinf(theta);
    cz = cosf(theta);

    // determine left axis
    left.x = cy*cz;
    left.y = sx*sy*cz + cx*sz;
    left.z = -cx*sy*cz + sx*sz;

    // determine up axis
    up.x = -cy*sz;
    up.y = -sx*sy*sz + cx*cz;
    up.z = cx*sy*sz + sx*cz;

    // determine forward axis
    forward.x = sy;
    forward.y = -sx*cy;
    forward.z = cx*cy;
}
