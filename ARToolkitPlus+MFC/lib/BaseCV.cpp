// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST

#include "BaseCV.h"



BaseCV::BaseCV(void) : capture(0)
{

	tcnt0 = dt_smooth = 0;
	for(int i=0; i<30; i++)
	{
		tf[i] = 0;
	}

}

BaseCV::~BaseCV(void)
{
	

}


float BaseCV::UpdateTime()
{
	
	float t1 = m_tm.End();
	float dt = t1 - t0;

	ShiftWithNew<float>(tf,dt,30);
	int i;

	// To aviod abrupt fluctuation
	float sum1=0;
	for(i=0; i<30; i++)
	{
		sum1 += tf[i];
	}
			

	sum1 /= 30.0;

	dt_smooth = sum1;

	t0 = t1;

	return dt_smooth;
}