// October 1, 2013
// Written by Seung-Chan Kim @TCL, KAIST

#include "StdAfx.h"
#include "vwDisplacement.h"

vwDisplacement::vwDisplacement(void)
{
	pBuf = NULL;

	

}

vwDisplacement::~vwDisplacement(void)
{
	
	Close();
}



void vwDisplacement::Close()
{

	CloseBuffer();

	//if (nTexID)	glDeleteTextures(1,&nTexID);

}



void vwDisplacement::CloseBuffer()
{

	if (pBuf)	delete[] pBuf;
	pBuf	= NULL;

}





void vwDisplacement::LoadImage(char *psz)
{

	CString m_fname	= CString(psz);

	m_png.Load(psz);
	nBPP = m_png.bits_depth * m_png.bits; 
}

void vwDisplacement::LoadDisplacement(char *psz, BOOL bFlip)
{
	LoadImage(psz);
	
	int width = m_png.w;
	int height= m_png.h;
	int nChannel=m_png.bits;

	
	BYTE *pS	= m_png.pBuffer;
	
	if(bFlip)
	{
		BYTE *pBufTemp	= new BYTE[width* height* nChannel];
		FlipV(pBufTemp, pS, width, height, nChannel);

		
		memmove(pS, pBufTemp,  width* height* nChannel);


		delete[] pBufTemp;
		pBufTemp = NULL;
	}


	//SetTexture(pS, m_png.w, m_png.h, m_png.bits);

//	GenTexture2();
}






void vwDisplacement::FlipV(BYTE* dst, const BYTE* src, int width, int height, int nChannel)
{

	int sz = width * height* nChannel;

	int dw		= width*nChannel;
	int ts		= width*height*nChannel;

	for (int i=0;i<height;i++)
	{
		
		memmove( &dst[dw*i],&src[ts-dw*(i+1)],dw);
	}
}


/*
void vwDisplacement::GenTexture()
{
	if (pBuf==NULL)	return;

	unsigned int ntex=0;
	glGenTextures(1, &ntex);			//generate one texture name 
	//glBindTexture(GL_TEXTURE_2D,ntex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	switch(nBPP){
	case 32:	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,w,h, 0, GL_RGBA, GL_UNSIGNED_BYTE,pBuf);	break;
	case 24:	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,w,h, 0, GL_RGB, GL_UNSIGNED_BYTE,pBuf);	break;
	}

	nTexID	= ntex;

	delete pBuf;
	pBuf	= NULL;
}
*/



xRGB vwDisplacement::GetTexColorAtUV(float u, float v)
{
	xRGB clr;

	int w	= m_png.w;
	int h	= m_png.h;
	int nCh	=m_png.bits;

	
	int w_ = w*u;
	int h_ = h*v;
	
	
	clr = GetRGBAt(0, w_, h_);
	//clr.r



	return clr;
}



BYTE vwDisplacement::GetColorAt(int x, int y, BOOL bYFlip) 
{

	BYTE *pS	= m_png.pBuffer;
	if(!pS) 
	{
		OutputDebugString("No image available");
		return 0;
	}

	int w = m_png.w;
	int h = m_png.h;
	int nCh=m_png.bits;


	if(bYFlip) y = h -1 -  y;
	//y = CLAMP(y,0,h-1);
	int pID = nCh * (y*w + x);

	BYTE p1 =  pS[pID];

	return p1;
}

xRGB vwDisplacement::GetRGBAt(int nID, int x, int y,  BOOL bYFlip)
{
	//nID is not used now.
	BYTE *pS	= m_png.pBuffer;
	
	if(!pS) 
	{
		OutputDebugString("No image available");
		return xRGB(0,0,0);
	}

	int w = m_png.w;
	int h = m_png.h;
	int nCh=m_png.bits;

	if(bYFlip) y = h -1 -  y;
	//y = CLAMP(y,0,h-1);
	int pID = nCh * (y*w + x);
	
	xRGB ret;

	if(nCh ==1)
	{
		BYTE p1 =  pS[pID];
		ret = xRGB(p1, p1, p1);
	}
	else if(nCh ==3)
	{
		BYTE p1 =  pS[pID];
		BYTE p2 =  pS[pID+1];
		BYTE p3 =  pS[pID+2];

		ret = xRGB(p1, p2, p3);

	}
	else if(nCh ==4)
	{

		BYTE p1 =  pS[pID];
		BYTE p2 =  pS[pID+1];
		BYTE p3 =  pS[pID+2];
		BYTE p4 =  pS[pID+4];

		ret = xRGB(p1, p2, p3, p4);

	}


	return ret;

}


BYTE* vwDisplacement::GetBuffer()
{
	BYTE *pS	= m_png.pBuffer;

	return pS;

}



void vwDisplacement::GetImageBuffer(char *psz, BYTE *dst, int w, int h, int Ch)
{
	

}
