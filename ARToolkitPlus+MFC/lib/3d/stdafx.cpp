#include "stdafx.h"


byte GetRValue(COLORREF col) { return (byte)col; }
byte GetGValue(COLORREF col) { return (byte)(col>>8);}
byte GetBValue(COLORREF col) { return (byte)(col>>16);} 

CPoint::CPoint() { x =0; y=0;}

CPoint::CPoint(int x,int y)
{
    this->x = x;
    this->y = y;
}  

CPoint::CPoint( double x,double y)
{
    this->x = (int)x;
    this->y = (int)y;
}

CPoint CPoint::operator+( CPoint pt )
{
    CPoint ret;
    ret.x = x+pt.x;
    ret.y = y+pt.y;
    return ret;
}

CPoint CPoint::operator-( CPoint pt )
{
    CPoint ret;
    ret.x = x-pt.x;
    ret.y = y-pt.y;
    return ret;
}

BOOL CPoint::operator==(CPoint pt)
{
	if ( pt.x==x && pt.y==y)	return TRUE;
	return FALSE;
}

BOOL CPoint::operator!=(CPoint pt)
{
	if ( pt.x==x && pt.y==y)	return FALSE;
	return TRUE;
}

CRect::CRect() { left = top = right = bottom = 0;}
CRect::CRect(int l,int t,int r,int b)
{
    left   = l;
    top    = t;
    right  = r;
    bottom = b;
}

CRect::CRect(CPoint ptLT,CPoint ptRB)
{
	left	= ptLT.x;
	top		= ptLT.y;
	right	= ptRB.x;
	bottom	= ptRB.y;
}

CRect::CRect(CPoint pt,CSize sz)
{
	left	= pt.x;
	top		= pt.y;
	right	= left+sz.cx;
	bottom	= top+sz.cy;
}

CRect CRect::operator+( CRect r)
{
    CRect ret;
    ret.left    = left +r.left;
    ret.right   = right+r.right;
    ret.top     = top  +r.top;
    ret.bottom  = bottom+r.bottom;
    return ret;
}

BOOL CRect::PtInRect(CPoint pt)
{
    if (right<left) return FALSE;
    if (bottom<top) return FALSE;
    if (pt.x<=right && pt.x>=left &&
	pt.y<=bottom&& pt.y>=top )
      return TRUE;
    return FALSE;
}

BOOL CRect::PtInRect( int x,int y)
{
  CPoint pt(x,y);
  return CRect::PtInRect(pt);
}

CSize::CSize()
{
	cx	= cy = 0;
}

CSize::CSize(int x,int y)
{
	cx	= x;
	cy	= y;
}

CFPoint::CFPoint() { x =0; y=0;}

CFPoint::CFPoint( float x,float y)
{
    this->x = (int)x;
    this->y = (int)y;
}

CFPoint CFPoint::operator+( CFPoint pt )
{
    CFPoint ret;
    ret.x = x+pt.x;
    ret.y = y+pt.y;
    return ret;
}

CFPoint CFPoint::operator-( CFPoint pt )
{
    CFPoint ret;
    ret.x = x-pt.x;
    ret.y = y-pt.y;
    return ret;
}


CFRect::CFRect() { left = top = right = bottom = 0;}
CFRect::CFRect(float l,float t,float r,float b)
{
    left   = l;
    top    = t;
    right  = r;
    bottom = b;
}

CFRect CFRect::operator+( CFRect r)
{
    CFRect ret;
    ret.left    = left +r.left;
    ret.right   = right+r.right;
    ret.top     = top  +r.top;
    ret.bottom  = bottom+r.bottom;
    return ret;
}

BOOL CFRect::PtInRect(CFPoint pt)
{
    if (right<left) return FALSE;
    if (bottom<top) return FALSE;
    if (pt.x<=right && pt.x>=left &&
	pt.y<=bottom&& pt.y>=top )
      return TRUE;
    return FALSE;
}

BOOL CFRect::PtInRect( float x,float y)
{
  CFPoint pt(x,y);
  return CFRect::PtInRect(pt);
}



#include <string.h>
DWORD i2p4(DWORD dw)
{
  BYTE s[4],t[4];
  memmove( s,&dw,4);
#ifdef IPHONE
  t[0] = s[0];
  t[1] = s[1]; 
  t[2] = s[2];
  t[3] = s[3];
#else
  t[0]= s[3];
  t[1]= s[2];
  t[2]= s[1];
  t[3]= s[0];
#endif  
  DWORD ret;
  memmove( &ret,t,4 );
 // printf("%d,%d\n",dw,ret);
  return ret;
}

float i2pf(float f)
{
  BYTE s[4],t[4];
  memmove( s,&f,4);
#ifdef IPHONE
  t[0] = s[0];
  t[1] = s[1]; 
  t[2] = s[2];
  t[3] = s[3];
#else
  t[0]= s[3];
  t[1]= s[2];
  t[2]= s[1];
  t[3]= s[0];
#endif  

  float ret;
  memmove( &ret,t,4 );
  return ret;
}


UINT i2p2(UINT dw)
{
  BYTE s[2],t[2];
  memmove( s,&dw,2);
#ifdef IPHONE
  t[0] = s[0];
  t[1] = s[1];
#else  
  t[0]= s[1];
  t[1]= s[0];
#endif  
  UINT ret;
  memmove( &ret,t,2 );
  return ret;
}


