/*

		This stdafx.h is only used for the compatible platform codes
		written by J.Yang.

		EDITED by S.Kim. 20101205
*/
#ifndef _BABO_
#define _BABO_


#include <string.h>	//20130827
//http://forum.openframeworks.cc/index.php?topic=11096.0

#ifndef _WINDEF_

#ifndef BYTE
typedef unsigned char		byte;
typedef unsigned char       BYTE;
#endif

#ifndef __APPLE__
typedef int                 BOOL;
#else
typedef signed char			BOOL;
#endif

// It is added in 5/12/2010
#define GNU99           // GNU99 is important for vString

#include <sys/types.h>

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned short      WORD;
typedef float	FLOAT;
typedef unsigned long       DWORD;
typedef unsigned long long	ULONG64;
typedef unsigned long long  ULONGLONG;
typedef short SHORT;
typedef long LONG;
typedef char CHAR;
typedef const char* LPCTSTR;
typedef char* LPTSTR;
typedef DWORD   COLORREF;
typedef unsigned long ULONG;
typedef unsigned char* LPBYTE;

#ifndef NULL
#define NULL 0
#endif
#define TRUE 1
#define FALSE 0

#define MAX_PATH 256

#define RGB(r,g,b) ( (g<<16) | (b<<24) | (r<<8) )
#define MK_LBUTTON 1
#define MK_RBUTTON 3
#define MK_CONTROL 0
#define MK_SHIFT   2
#ifndef  _WINDEF_
#define WM_MOUSEWHEEL 4
#endif

byte GetRValue(COLORREF );
byte GetGValue(COLORREF );
byte GetBValue(COLORREF );

DWORD i2p4(DWORD);
UINT  i2p2(UINT);
float i2pf(float);

//#define CString vString
//#include "vString.h"

#ifndef CPoint
class CPoint
{
public:
  CPoint();
  CPoint(int x,int y);
  CPoint( double x,double y);

public:
  CPoint operator+( CPoint pt );
  CPoint operator-( CPoint pt );
  BOOL	 operator==(CPoint pt);
  BOOL	 operator!=(CPoint pt);

public:
  int x,y;
};
#endif

#ifndef CSize
class CSize
{
public:
  CSize();
  CSize(int x,int y);
  CSize( double x,double y);

public:
  //CSize operator+( CSize pt );
  //CSize operator-( CSize pt );
	int	cx,cy;

};
#endif

#ifndef CRect
class CRect 
{
public:
  CRect();
  CRect(int l,int t,int r,int b);
  CRect(CPoint,CPoint);
  CRect(CPoint,CSize);
  
public:
  CRect		operator+( CRect r);
  CRect		operator+(CPoint pt)	
  { 
	CRect ret;
	ret.left = left+pt.x;
	ret.top	 = top+pt.y;
	ret.right= right+pt.x;
	ret.bottom=bottom+pt.y;
	return ret;
  }
    CRect		operator-(CPoint pt)	
    { 
        CRect ret;
        ret.left = left-pt.x;
        ret.top	 = top-pt.y;
        ret.right= right-pt.x;
        ret.bottom=bottom-pt.y;
        return ret;
    }
  BOOL		PtInRect(CPoint pt);
  BOOL		PtInRect( int x,int y );
  int		Width()	{	return right-left; }
  int		Height(){	return bottom-top; }
  CPoint	CenterPoint()	{ return CPoint( (right+left)/2,(top+bottom)/2); }
  void		InflateRect(int l,int t,int r,int b)
  {
	left-=l;
	right+=r;
	top-=t;
	bottom+=b;
  }
  
  void		DeflateRect(int l,int t,int r,int b)
  {
	left+=l;
	right-=r;
	top+=t;
	bottom-=b;
  }
  
public:
  int left,top,right,bottom;

};
#endif

#ifndef CFPoint
class CFPoint
{
public:
  CFPoint();
  CFPoint( float x,float y);

public:
  CFPoint operator+( CFPoint pt );
  CFPoint operator-( CFPoint pt );

public:
  float x,y;
};
#endif

#ifndef CFRect
class CFRect 
{
public:
  CFRect();
  CFRect(int l,int t,int r,int b);
  CFRect(float l,float t,float r,float b);	

public:
  CFRect		operator+(CFRect r);
  CFRect		operator+(CFPoint pt)	
  { 
	CFRect ret;
	ret.left = left+pt.x;
	ret.top	 = top+pt.y;
	ret.right= right+pt.x;
	ret.bottom=bottom+pt.y;
	return ret;
  }
  BOOL		PtInRect(CFPoint pt);
  BOOL		PtInRect(float x,float y );
  float		Width()	{	return right-left; }
  float		Height(){	return bottom-top; }
  CFPoint	CenterPoint()	{ return CFPoint( (right+left)/2.,(top+bottom)/2.); }
  void		InflateRect(float l,float t,float r,float b)
  {
	left-=l;
	right+=r;
	top-=t;
	bottom+=b;
  }
  
  void		DeflateRect(float l,float t,float r,float b)
  {
	left+=l;
	right-=r;
	top+=t;
	bottom-=b;
  }
  
public:
  float left,top,right,bottom;

};
#endif

/*
#ifndef CDC
#define CDC byte
#endif
*/
/*
#ifndef CWnd 
#define CWnd byte
#endif
*/

#ifndef CGdiObject
#define CGdiObject int
#endif

#ifndef CPen
#define CPen int
#endif

#ifndef CFont
#define CFont int
#endif

#ifndef PPC
#define PPC // for only Mac based Linux!
#endif


#ifndef MACOS
#define MACOS
#endif

#endif

#endif





