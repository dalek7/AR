/*
 *  vwTexture.cpp
 *  WorldTest
 *
 *  Created by Seung-Chan Kim on 6/28/11.
 *  Copyright 2011 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */

#include "StdAfx.h"
#include "vwTexture.h"


vwTexture::vwTexture(void)
{
	pBuf = NULL;

	nID		= 0;
	nTexID	= 0;

	w_ori	= 0;
	h_ori	= 0;

	nChannels	= 0;
	nBPP		= 0;



	m_fname="";
}

vwTexture::~vwTexture(void)
{
	
	Close();
}



void vwTexture::Close()
{

	CloseBuffer();

	if (nTexID>0)
	{
		
		printf("vwTexture- glDeleteTextures - of nTexID #%d : %4dx%4dx%d\n", nTexID, w_ori, h_ori, nBPP/8);
		printf("vwTexture- glDeleteTextures - of nTexID #%d\n", nTexID);
		
		glDeleteTextures(1,&nTexID);

		nID		= 0;
		nTexID	= 0;

		w_ori	= 0;
		h_ori	= 0;

		w		= 0;
		h		= 0;

		nChannels	= 0;
		nBPP		= 0;

		m_fname="";

	}

	

}



void vwTexture::CloseBuffer()
{

	if (pBuf)	delete[] pBuf;
	pBuf	= NULL;

}






void vwTexture::BindTexture()
{
	//if (nTexID)
	{
		glBindTexture(GL_TEXTURE_2D,nTexID);
	}

}

void vwTexture::LoadImage(char *psz)
{
	m_fname	= CString(psz);

	m_png.Load(psz);
	nBPP		= m_png.bits_depth * m_png.bits; 

	nChannels	= nBPP/8;
	w_ori		= m_png.w;
	h_ori		= m_png.h;
}

void vwTexture::LoadTexture(char *psz, BOOL bFlip)
{
	LoadImage(psz);
	
	int width		= m_png.w;
	int height		= m_png.h;
	int nChannel	= m_png.bits;

	
	BYTE *pS	= m_png.pBuffer;
	
	if(bFlip)
	{
		BYTE *pBufTemp	= new BYTE[width* height* nChannel];
		FlipV(pBufTemp, pS, width, height, nChannel, FALSE);

		
		memmove(pS, pBufTemp,  width* height* nChannel);


		delete[] pBufTemp;
		pBufTemp = NULL;
	}


	SetTexture(pS, m_png.w, m_png.h, m_png.bits);

}


// for generated textures
void vwTexture::SetTexture2(const BYTE* src, int width,int height, int nCh,  BOOL bFlip,  BOOL bBGR2RGB)
{

	m_png.w		= width;
	m_png.h		= height;
	m_png.bits	= nCh;

	// This loop requires time to process
	//if(m_png.pBuffer) delete [] m_png.pBuffer;
	//m_png.pBuffer = new BYTE[width*height*nCh];

	if(bFlip)
	{
		BYTE *pBufTemp	= new BYTE[width* height* nCh];
		FlipV(pBufTemp, src, width, height, nCh, bBGR2RGB);

		
		
		// TODO : FIX this line for a BGR image !!
		if(0)
		{
			memmove(m_png.pBuffer, pBufTemp,  width* height* nCh);
		}
		/*
		else
		{
			for(int i=0; i<width* height; i++)
			{
				
				pBufTemp[nCh*i + 0] = m_png.pBuffer[nCh*i + 2];
				pBufTemp[nCh*i + 1] = m_png.pBuffer[nCh*i + 1];
				pBufTemp[nCh*i + 2] = m_png.pBuffer[nCh*i + 0];

			}

		}
		*/
		SetTexture(pBufTemp,  width, height,  nCh);



		delete pBufTemp;
		pBufTemp = NULL;

	}
	else
	{

		//memmove(m_png.pBuffer, src,  width* height* nCh);
		SetTexture(src,  width, height,  nCh);


	}

	
}

void vwTexture::SetTexture(const BYTE* src, int width,int height, int nCh)
{

	w_ori		= width;
	h_ori		= height;
	nChannels	= nCh;


	nBPP = nChannels * 8;

	BOOL isPowerOf2	= ddIsPowerOfTwo(width) & ddIsPowerOfTwo(height);

	w	= ddNextPowerOfTwo(width);
	h	= ddNextPowerOfTwo(height);


	int sz_img	= width		* height	* nChannels;
	int sz_tex	= w	* h	* nChannels;

	if(pBuf) delete[] pBuf;

	if(isPowerOf2)
	{
		pBuf	= new BYTE[w*h*(nBPP/8)];
		memmove(pBuf, src, sz_img);
		return;
	}

	if (!pBuf)
	{
		pBuf	= new BYTE[w*h*(nBPP/8)];
		memset(pBuf, 0, w * h * nChannels);
	}
	
	for (int j=0;j<h;j++)
	{
		int jj = h-j-1;
		if(j<height)
			memmove(pBuf + w * nChannels * j, src + width * nChannels * j, width * nChannels);
		
	}

}



void vwTexture::FlipV(BYTE* dst, const BYTE* src, int width, int height, int nCh, BOOL bBGR2RGB)
{

	int sz = width * height* nCh;

	int dw		= width*nCh;
	int ts		= width*height*nCh;


	for(int i=0; i<width* height; i++)
	{
		if(bBGR2RGB)
		{
			dst[nCh*i + 0] = src[nCh*(width* height-i-1) + 2];
			dst[nCh*i + 1] = src[nCh*(width* height-i-1) + 1];
			dst[nCh*i + 2] = src[nCh*(width* height-i-1) + 0];
		}
		else
		{
			dst[nCh*i + 0] = src[nCh*(width* height-i-1) + 0];
			dst[nCh*i + 1] = src[nCh*(width* height-i-1) + 1];
			dst[nCh*i + 2] = src[nCh*(width* height-i-1) + 2];
		}

	}

	if(0)	// Old-way..
	{
		for (int i=0;i<height;i++)
		{
			memmove( &dst[dw*i],&src[ts-dw*(i+1)],dw);
		}
	}
}


int vwTexture::GenTexture2()
{
	if (pBuf==NULL)	return -1;
	if (nTexID>0)					//////////// >-1??
		glDeleteTextures(1,&nTexID);

	unsigned int ntex=0;
	glGenTextures(1, &ntex);			//generate a texture name
	glBindTexture(GL_TEXTURE_2D,ntex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	switch(nBPP)
	{
		case 32:	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,w,h, 0, GL_RGBA, GL_UNSIGNED_BYTE,pBuf);	break;
			//gluBuild2DMipmaps(GL_TEXTURE_2D, 0, w,h, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);	break;
	 
		case 24:	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,w,h, 0, GL_RGB, GL_UNSIGNED_BYTE,pBuf);	break;
	}


	GLenum x = glGetError();

	nTexID	= ntex;

	if(pBuf)
		delete [] pBuf;

	pBuf	= NULL;
	//starting from 1

	if(x==0)
	{
		//printf("vwTexture- glGenTextures(1, &ntex); #%d : %4dx%4dx%d\n", nTexID, w_ori, h_ori, nBPP/8));
		//printf("vwTexture- glGenTextures(1, &ntex); #%d : %4dx%4dx%d\n", nTexID, w_ori, h_ori, nBPP/8);
	}
	else
	{
		//OutputDebugString(DDFORMAT("vwTexture- glGenTextures(1, &ntex); #%d :ERR:%d", nTexID,x));
		printf("vwTexture- glGenTextures(1, &ntex); #%d :ERR:%d\n", nTexID,x);
	}

	return nTexID;
}



/*
void vwTexture::GenTexture()
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



xRGB vwTexture::GetTexColorAtUV(float u, float v)
{
	xRGB clr;
/*
	int w	=	m_png.w;
	int h	=	m_png.h;
	int nCh	=	m_png.bits;
*/

	int w	=	w_ori;
	int h	=	h_ori;
	int nCh	=	nChannels;
	
	int w_ = w*u;
	int h_ = h*v;
	
	
	clr = GetRGBAt(0, w_, h_);
	//clr.r



	return clr;
}



BYTE vwTexture::GetColorAt(int x, int y, BOOL bYFlip) 
{

	BYTE *pS	= m_png.pBuffer;
	if(!pS) 
	{
		OutputDebugString("No image available");
		return 0;
	}

	int w	=	w_ori;
	int h	=	h_ori;
	int nCh	=	nChannels;

/*
	int w = m_png.w;
	int h = m_png.h;
	int nCh=m_png.bits;
*/

	if(bYFlip) y = h -1 -  y;
	//y = CLAMP(y,0,h-1);
	int pID = nCh * (y*w + x);

	BYTE p1 =  pS[pID];

	return p1;
}

xRGB vwTexture::GetRGBAt(int nID, int x, int y,  BOOL bYFlip)
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


BYTE* vwTexture::GetBuffer()
{
	BYTE *pS	= m_png.pBuffer;

	return pS;

}



void vwTexture::GetImageBuffer(char *psz, BYTE *dst, int w, int h, int Ch)
{
	

}
