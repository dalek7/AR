
// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST

#pragma once

#include <windows.h>
#include "CvvImage.h"
#include "../lib/Elapsed.h"
#include "../lib/ttMath.h"
using namespace cv;

class BaseCV 
{
public:
	BaseCV(void);
	virtual ~BaseCV(void);

public:
	int tcnt0;
	float t0;
	float dt_smooth;
	CElapsed m_tm;
	float UpdateTime();
	void InitTime()
	{
		if(GetCount() ==0)
			t0 = m_tm.End();
	}

	void InitCount()
	{
		tcnt0 = 0;
	}

	float GetCount()
	{ 
		return tcnt0;
	}

	int IncreaseCount()
	{
		return ++tcnt0;
		
	}




public:

	VideoCapture capture;
	
	Mat frame;
	Mat frameg;

public:
	CvvImage	m_CvvImage;		// for drawing in the MFC dialog
	float tf[30];

	//IplImage *img1;
	//IplImage *img2;

};
