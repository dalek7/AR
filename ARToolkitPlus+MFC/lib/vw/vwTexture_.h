#pragma once

#include "gl/gl.h"
#include "gl/glu.h"



#include "../lib/png/CPImage.h"

#include "ddmath.h"

class vwTexture
{
public:
	vwTexture(void);
	~vwTexture(void);


public:
	int		w,h;
	int		nBPP;
	BYTE	*pBuf;

public:
	void LoadImage(char *psz);
	void LoadTexture(char *psz);
	void GenTexture();
	int  GenTexture2();
	void BindTexture();

	

public:
	void SetTexture(const BYTE* src, int width,int height, int nChannel);
	void SetTexture2(const BYTE* src, int width,int height, int nChannel);
	void FlipV(BYTE* dst, const BYTE* src, int width,int height, int nChannel);

public:
	CPImage	m_png;
	unsigned int	nTexID;
	int				nID;



};
