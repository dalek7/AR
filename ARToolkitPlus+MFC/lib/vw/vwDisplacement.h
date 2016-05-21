// October 1, 2013
// Written by Seung-Chan Kim @TCL, KAIST

#pragma once

#include "gl/gl.h"
#include "gl/glu.h"



#include "../png/CPImage.h"
#include "../xRGB.h"


#include "../ddmath.h"

class vwDisplacement
{
public:
	vwDisplacement(void);
	~vwDisplacement(void);


public:
	int		w,h;
	int		nBPP;
	BYTE	*pBuf;

public:
	void LoadImage(char *psz);
	void LoadDisplacement(char *psz, BOOL bFlip = TRUE);

	
	void Close();
	void CloseBuffer();

	

public:
	void FlipV(BYTE* dst, const BYTE* src, int width,int height, int nChannel);

public:
	CPImage	m_png;

public:
	xRGB GetTexColorAtUV(float u, float v);
	BYTE GetColorAt( int x, int y,  BOOL bYFlip= FALSE); // for getting Red or gray
	
	xRGB GetRGBAt(int nID, int x, int y,  BOOL bYFlip= FALSE);

	
	

//Util function

public:
	BYTE* GetBuffer();
	static void GetImageBuffer(char *psz, BYTE *dst, int w, int h, int Ch);

};
