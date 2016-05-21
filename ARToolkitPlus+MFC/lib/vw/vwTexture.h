/*
 *  vwTexture.h
 *  WorldTest
 *
 *  Created by Seung-Chan Kim on 6/28/11.
 *  Copyright 2011 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */


#pragma once

#include "gl/gl.h"
#include "gl/glu.h"
#include "../png/CPImage.h"
#include "../xRGB.h"
#include "../ddmath.h"

class vwTexture
{
public:
	vwTexture(void);
	~vwTexture(void);


public:
	int		w_ori, h_ori;	//2013/10/1
	int		w,h;

	int		nBPP;
	int		nChannels; // = nBPP/8	

	BYTE	*pBuf;


public:
	CString m_fname;
	CString GetTexFileName(){return m_fname;}
public:
	void LoadImage(char *psz);
	void LoadTexture(char *psz, BOOL bFlip = TRUE);
	void GenTexture();

	int  GenTexture3();	//keeping image buffer @2012/10/17
	int  GenTexture2();

	void BindTexture();

	
	void Close();
	void CloseBuffer();

	

public:
	void SetTexture(const BYTE* src, int width,int height, int nChannel);
	void SetTexture2(const BYTE* src, int width,int height, int nChannel, BOOL bFlip = FALSE, BOOL bBGR2RGB=FALSE);
	void FlipV(BYTE* dst, const BYTE* src, int width,int height, int nChannel, BOOL bBGR2RGB=FALSE);

public:
	CPImage	m_png;
	unsigned int	nTexID;
	int				nID;

public:
	xRGB GetTexColorAtUV(float u, float v);
	BYTE GetColorAt( int x, int y,  BOOL bYFlip= FALSE); // for getting Red or gray
	
	xRGB GetRGBAt(int nID, int x, int y,  BOOL bYFlip= FALSE);

	
public:
	

//Util function

public:
	BYTE* GetBuffer();
	static void GetImageBuffer(char *psz, BYTE *dst, int w, int h, int Ch);

};
