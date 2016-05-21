#pragma once

#include "vwobj.h"

#include "vwTexture.h"
#include "../elapsed.h"

#include "vwCamera.h"

//#include "../ddOSC.h"
#include "../vArray.h"
#include "../FileName.h"

const float PARTICLE_SIZE = 0.5f;
const int INITIAL_PARTICLE_SPREAD = 10;
const float SPEED_DECAY = -0.00005f; // (Gravity)


class vwFX;

class vwParticle : public vwO
{
public:
	vwParticle();

public:
	//dhVector o;
	dhVector vDir;
	float r,g,b,life;

	float life_latest_hit;


	int nSharedTexID;
	vwFX *pPM;			//to access the container

public:
	void	DrawObject();

};



class vwFX
{
public:
	vwFX(void);
	~vwFX(void);


public:
	void *pParent;
	void SetParentObj(void*);
	
public:
	
	vwTexture m_tx;
	void LoadParticleTex(char *psz);
	int GenTexture();

	void Init(int nTot);
	void Init(int nTot, vwCamera* pCam_);
	void Close();


	void Draw();

	vwParticle* pParticle;


	int		MaxSpread,MaxParticles,Index;
	float	Spread,Angle;  
	float	ViewYaw, ViewPitch;

	CElapsed m_tm;
	float lasttime;
	


	vwCamera	*pCamera;


	//20120206

//	vArray <CDInfo,CDInfo> traj;
//	BOOL bRecord;
//	int	SaveTraj();



public:
	int nParticle;
	int nTex;		//sharedID


private:
	void Run();
};
