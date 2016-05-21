/*
 *  vwLine.h
 *
 *  Created by Seung-Chan Kim on 2/16/13.
 *  Copyright 2013 KAIST, Korea. All rights reserved.
 *  Written @ Cafe CHAUD 1186, Daejeon
 */


#ifndef __VWLINE__
#define __VWLINE__



#include "../3d/dhVector.h"
#include "../3d/dhMat.h"

#include "../ddmath.h"

#include "vwO.h"

class vwLine : public vwBox
{
public:
		vwLine();
		// Constructor defines line as passing through points p1 and p2.
		// The parametric representation is then f(t) = (1-t)P1+tP2.
		vwLine(dhVector &p1, dhVector &p2);
        

		~vwLine();
public:

		//void	Init(int,int);
		void	Close();

public:
		dhVector m_p1, m_p2;

		

public:
		dhVector direction();
		dhVector eval(float);

		dhVector project(dhVector &pt);

		BOOL isParallel(vwLine &line, const float epsilon=1.0E-8);

		
		float	distPointLine(dhVector &pt);

		/* Determines the minimum distance between two infinite lines. */
		float distFromLine(vwLine &line,
                                  float &t, float &s, 
                                  const float epsilon=1.0E-8);


public:
		void setStartPoint(const dhVector &startPoint)
		{
			m_p1 = startPoint;
		}

		void setEndPoint(const dhVector &endPoint)
		{
			m_p2 = endPoint;  
		}


		dhVector &getEndPoint() { return m_p2; }
        dhVector &getStartPoint() { return m_p1; }


		dhVector unitDirection() 
		{
			dhVector u = direction();
			
			return u.Unit();
		}


		dhVector getPointOnLine() const { return m_p1; }
		

		

};
/*
m_line1.setStartPoint(dhVector(0,0,0));
	m_line1.setEndPoint(dhVector(10,10,0));

	//m_line2.setStartPoint(dhVector(0,10,0));
	//m_line2.setEndPoint(dhVector(10,0,0));

	m_line2.setStartPoint(dhVector(0,0,0));
	m_line2.setEndPoint(dhVector(10,10,0));

	BOOL b1 = m_line1.isParallel(m_line2, 0.0001);

	dhVector p1 = dhVector(5,0,0);
	float d1 = m_line1.distPointLine(p1);



*/

#endif