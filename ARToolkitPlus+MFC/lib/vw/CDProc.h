#pragma once
#include "../vArray.h"

//Dbg("%d\t%f\t%f\t(\t%f\t%f\t)\n",i,  
//pParticle[i].life, pParticle[i].vDir.y, pParticle[i].o.x, pParticle[i].o.y);


#define KEEP_MAX_NO 60*5

typedef struct __CDInfo
{
	float	t;
	int		hit_ID;
	float	life;
	dhVector	vel_normal;
	dhVector	pos_col;
	

} CDInfo;


class CDProc
{
public:
	CDProc(void);
	~CDProc(void);

public:

	void Run();
	void Add(CDInfo);
	void Add(int count);
	int eventHist[KEEP_MAX_NO];
	vArray <CDInfo, CDInfo> traj;

		
};
