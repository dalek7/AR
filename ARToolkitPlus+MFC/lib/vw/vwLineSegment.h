/*
 *  vwLineSegment.h
 *
 *  Created by Seung-Chan Kim on 2/16/13.
 *  Copyright 2013 KAIST, Korea. All rights reserved.
 *  Written @ Cafe CHAUD 1186, Daejeon
 */


#ifndef __VWLINESEGMENT__
#define __VWLINESEGMENT__


#include "vwLine.h"

class vwLineSegment :public vwLine
{
public:
	vwLineSegment();
public:
	float distFromSegment(vwLineSegment &line,
                                  float &t, float &s, 
                                  const float epsilon=1.0E-8);
	float projectToParametric(dhVector &pt);


public:
	float length() const { return m_length; }
	dhVector unitDirection() const 
    { 
        return m_unitDirection; 
    }

	dhVector evalSegment(const float t)
    {
        return eval(ddClamp<float>(t, 0.0, 1.0));
    }


	void Update();
	void SetRCSize(float x, float y);

private:
    
    float m_length;    
    dhVector m_unitDirection;

	float x_, y_;


public:
	void MakeObj();
	virtual void	DrawObject();

	


};

#endif