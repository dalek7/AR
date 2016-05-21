#include "StdAfx.h"
#include "vwFX.h"

//#include "../../ddCVStatic/gl.h"
//extern CGL *pGL;

vwParticle::vwParticle()
{
	nSharedTexID = 0;
}



void vwParticle::DrawObject()
{
	
	glEnable(GL_TEXTURE_2D);
	
	
	glBindTexture(GL_TEXTURE_2D,nSharedTexID);// pPM->nTex;	//GLuint

	glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);

	for (int i=0;i<nFace;i++)
	{
		vwPolygon *p	= &pFace[i];

		int a,b,c;
		a	= p->v[0]-pVer;
		b	= p->v[1]-pVer;
		c	= p->v[2]-pVer;

		glBegin(GL_TRIANGLES);
		{
			if (pNormal)
			{
				glNormal3f(pNormal[a].x,pNormal[a].y,pNormal[a].z);
				glTexCoord2f( pUV[2*a],pUV[2*a+1]);
				glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);
				glNormal3f(pNormal[b].x,pNormal[b].y,pNormal[b].z);
				glTexCoord2f( pUV[2*b],pUV[2*b+1]);
				glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
				glNormal3f(pNormal[c].x,pNormal[c].y,pNormal[c].z);
				glTexCoord2f( pUV[2*c],pUV[2*c+1]);
				glVertex3d(p->v[2]->x,p->v[2]->y,p->v[2]->z);
			}
			else
			{
				glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);
				glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
				glVertex3d(p->v[2]->x,p->v[2]->y,p->v[2]->z);	
			}
		}
		glEnd();
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_TEXTURE_2D);
}


vwFX::vwFX(void)
{
	nParticle	= 0;
	nTex		= 0;
	pParticle		= NULL;
	pCamera			= NULL;

	pParent			= NULL;
}

vwFX::~vwFX(void)
{

}

void vwFX::SetParentObj(void *p)
{
	
	pParent = p;
}


void vwFX::Init(int nTot, vwCamera* pCam_)
{

	//ddOSC::SetFreq(200);	///
	//ddOSC::SetAmp(50);	///




	pCamera = pCam_;//pGL->pCamera;
	


	srand(time(NULL));
	Close();

	MaxSpread		= nTot/2;//INITIAL_PARTICLE_SPREAD;
	MaxParticles	= nTot;

	
	lasttime= m_tm.End();

	nParticle = nTot;
	pParticle = new vwParticle[nTot];

	int i;
	for(i=0; i<nParticle; i++)
	{
		pParticle[i].MakePlane(PARTICLE_SIZE,PARTICLE_SIZE,1,1);
		pParticle[i].SetUV(1,1);

		pParticle[i].Trans(0,-PARTICLE_SIZE,0);
		pParticle[i].pPM = this;
		
		pParticle[i].life	=0.0f;
		pParticle[i].r		=1.0f;
		pParticle[i].b		=0.0f;
	    

	}


}

void vwFX::Init(int nTot)
{

	//ddOSC::SetFreq(200);	///
	//ddOSC::SetAmp(50);	///




//	pCamera = pGL->pCamera;
	


	srand(time(NULL));
	Close();

	MaxSpread		= nTot/2;//INITIAL_PARTICLE_SPREAD;
	MaxParticles	= nTot;

	
	lasttime= m_tm.End();

	nParticle = nTot;
	pParticle = new vwParticle[nTot];

	int i;
	for(i=0; i<nParticle; i++)
	{
		pParticle[i].MakePlane(PARTICLE_SIZE,PARTICLE_SIZE,1,1);
		pParticle[i].SetUV(1,1);

		pParticle[i].Trans(0,-PARTICLE_SIZE,0);
		pParticle[i].pPM = this;
		
		pParticle[i].life	=0.0f;
		pParticle[i].r		=1.0f;
		pParticle[i].b		=0.0f;
	    

	}


}

void vwFX::Close()
{
	if(pParticle)
	{
		delete pParticle;
		pParticle = NULL;
	}

}

void vwFX::LoadParticleTex(char *psz)
{
	m_tx.LoadTexture(psz);
	
	//return n;
}


int vwFX::GenTexture()
{
	nTex = m_tx.GenTexture2();
	
	if(nTex>0)
	for(int i=0; i<nParticle; i++)
	{

		pParticle[i].nSharedTexID = nTex;
	}
	return nTex;
}


void vwFX::Draw()
{
	Run();

	int i;
	for(i=0; i<nParticle; i++)
	{
		
		glPushMatrix();
		/*
			if(pParent)
			{
				vwObj *pObj = (vwObj *) pParent;
				glTranslatef(pObj->o.x, pObj->o.y, pObj->o.z);

			}
			*/
			glRotatef(-pCamera->vw.r.y,0.0f,1.0f,0.0f);
			glRotatef(-pCamera->vw.r.x,1.0f,0.0f,0.0f);
			//glColor4f(pParticle[i].r,pParticle[i].g,pParticle[i].b,pParticle[i].life);
			glColor4f(1,1,1,pParticle[i].life);
			pParticle[i].Draw(TRUE);


		glPopMatrix();
	}

}


void vwFX::Run()
{
	int i;
	float t = m_tm.End();
	float dt = t - lasttime;
	dt *= 100;
	lasttime = t;

	//if(fabs(dt)>1) return;
	//m_cd.Run();
	//CDInfo cd;
	int col_cnt=0;
	for(i=0;i<nParticle;i++)
	{

		if(pParticle[i].life>0.0f)
		{

			pParticle[i].vDir.y -=(SPEED_DECAY*dt);  
			dhVector pt = pParticle[i].o + pParticle[i].vDir* dt;

			pParticle[i].Trans(pt);
			
			// 'Bounce' particle if on the floor square
            // if(pParticle[i].o.x>-10.0f && pParticle[i].o.x<10.0f &&
            //  pParticle[i].o.z>-10.0f && pParticle[i].o.z<10.0f)
        	
			
			if(pParticle[i].o.y>-PARTICLE_SIZE)
			{
				//pParticle[i].Trans(0,-PARTICLE_SIZE,0);
				pParticle[i].life	-=0.01f;
				pParticle[i].vDir.y *=-0.6f;//0.6

				float dt_hit = pParticle[i].life_latest_hit-  pParticle[i].life;
				if(dt_hit>0.2 && pParticle[i].life>0.3)
				{
					//if(pParent)
					//if(((vwObj*)pParent)->o.y < -3)
					col_cnt++;
					//Dbg("%d\t%f\t%f\t(\t%f\t%f\t)\n",i,  pParticle[i].life, pParticle[i].vDir.y, pParticle[i].o.x, pParticle[i].o.y);
					
					/*
					cd.hit_ID		= i;
					cd.life			= pParticle[i].life;
					cd.vel_normal	= pParticle[i].vDir.y;
					cd.hit_x		= pParticle[i].o.x;
					cd.hit_y		= pParticle[i].o.y;
					*/
					pParticle[i].life_latest_hit =  pParticle[i].life;

					

				}
			}
			
			pParticle[i].life-=(0.001f*dt);
		}
		else // Spawn a new particle
		{
			
			if(pParent)
			{
				dhVector offsetFX = dhVector(0,-6.5,0);
				pParticle[i].Trans(((vwObj*)pParent)->o.Scaling(1) - dhVector(0,PARTICLE_SIZE,0) );//+ offsetFX
			}
			else
				pParticle[i].Trans(0,-PARTICLE_SIZE-10,0);
			
			// Get a random spread and direction
			Spread	=(float)(rand()%MaxSpread)/10000.0f;
			Angle	= rand()%90; //(rand()%157)/100.0f/3.0f; // Quarter circle
			Angle	= RAD(Angle);

			//pParticle[i].vDir = dhVector(cos(Angle)*Spread, 0, sin(Angle)*Spread);
			float x_r = (rand()%1000)/1000.0;
			float z_r = (rand()%1000)/1000.0;
			pParticle[i].vDir = dhVector(x_r*Spread, 0, z_r*Spread);
	
			// Randomly reverse X and Z vector to complete the circle
			if(rand()%2)
				pParticle[i].vDir.x *= -1;

			if(rand()%2)
				pParticle[i].vDir.z *= -1;

			// Get a random initial speed
			pParticle[i].vDir.y = (float)(rand()%500)/10000.0f;

			// Get a random life and 'yellowness'
			pParticle[i].life	=(float)(rand()%100)/100.0f;
			pParticle[i].g		= 0.2f+((float)(rand()%50)/100.0f);


			pParticle[i].life_latest_hit = 10; //something big

		
		}
	}
	
	//if(pGL->tcnt %2==0)
	{
		//static int prevAmpFX = -1;

		int ampFX = 50+ col_cnt*5;
		
		ampFX = CLAMP(ampFX, 0, 100);
		
		//if(prevAmpFX != ampFX)
		//ddOSC::SetAmp(ampFX);/////
		//prevAmpFX = ampFX;
	}
}
