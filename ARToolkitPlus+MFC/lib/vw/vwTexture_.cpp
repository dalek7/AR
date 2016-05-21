#include "StdAfx.h"
#include "vwTexture.h"

vwTexture::vwTexture(void)
{
	pBuf = NULL;

	nID		= 0;
	nTexID	= 0;

}

vwTexture::~vwTexture(void)
{
	
	if (pBuf)	delete pBuf;
	pBuf	= NULL;

	if (nTexID)	glDeleteTextures(1,&nTexID);
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

	CString m_fname	= CString(psz);

	m_png.Load(psz);
	nBPP = m_png.bits_depth * m_png.bits;
}

void vwTexture::LoadTexture(char *psz)
{
	LoadImage(psz);
	
	BYTE *pS	= m_png.pBuffer;
	SetTexture(pS, m_png.w, m_png.h, m_png.bits);

//	GenTexture2();
}




void vwTexture::SetTexture(const BYTE* src, int width,int height, int nChannel)
{
	nBPP = nChannel * 8;

	BOOL isPowerOf2	= ddIsPowerOfTwo(width) & ddIsPowerOfTwo(height);

	w	= ddNextPowerOfTwo(width);
	h	= ddNextPowerOfTwo(height);


	int sz_img	= width		* height	* nChannel;
	int sz_tex	= w	* h	* nChannel;


	if(isPowerOf2)
	{
		pBuf	= new BYTE[w*h*(nBPP/8)];
		memmove(pBuf, src, sz_img);
		return;
	}

	if (!pBuf)
	{
		pBuf	= new BYTE[w*h*(nBPP/8)];
		memset(pBuf, 0, w * h * nChannel);
	}
	

	
	
	//flip pS
	BYTE *pBufTemp	= new BYTE[sz_img];
	FlipV(pBufTemp, src, width, height, nChannel);

	for (int j=0;j<h;j++)
	{

		if(j<height)
			memmove(pBuf + w * nChannel * j, src + width * nChannel * j, width * nChannel);
			//memmove(pBuf + w * nChannel * j, pBufTemp + width * nChannel * j, width * nChannel);
		
	}

	delete pBufTemp;
	pBufTemp = NULL;

}



void vwTexture::FlipV(BYTE* dst, const BYTE* src, int width, int height, int nChannel)
{

	int sz = width * height* nChannel;

	int dw		= width*nChannel;
	int ts		= width*height*nChannel;

	for (int i=0;i<height;i++)
	{
		
		memmove( &dst[dw*i],&src[ts-dw*(i+1)],dw);
	}
}


int vwTexture::GenTexture2()
{
	if (pBuf==NULL)	return -1;
	if (nTexID)	glDeleteTextures(1,&nTexID);

	unsigned int ntex=0;
	glGenTextures(1, &ntex);			//generate a texture name
	glBindTexture(GL_TEXTURE_2D,ntex);

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


	return nTexID;
}



void vwTexture::GenTexture()
{
	if (pBuf==NULL)	return;

	unsigned int ntex=0;
	glGenTextures(1, &ntex);			//generate one texture name 
	glBindTexture(GL_TEXTURE_2D,ntex);

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


