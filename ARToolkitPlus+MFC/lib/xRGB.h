#ifndef __xRGB__
#define __xRGB__

#ifndef byte
#define byte unsigned char
#endif

/////////////////////////////////////////////////////////////////////////////////////
//
//	xRGB is automatically converted as float,integer,and bytes.
//  And xRGB can be done with COLORREF for MS windows and Array for OpenGL.
//
//	Written by J.Y.Yang
//
/////////////////////////////////////////////////////////////////////////////////////

class _xRGB
{
public:
		_xRGB(){}
public:
		
		void operator=(BYTE a)
		{	
			if ( a==255 )	f	= 1.0f;
			else
			{
				f	= (float)a;	
				f	= f/256.f;
			}
			b	= a;
			n	= a;
		}

		void operator=(int a)
		{	
			if ( a==255 )	f	= 1.0f;
			else
			{
				f	= (float)a;	
				f	= f/256.f;
			}
			b	= (BYTE)a;
			n	= a;
		}

		void operator=(float a)
		{	
			f	= (float)a;	
			if ( f==1.0f )	
			{
				b	= 255;
				n	= 255;
			}
			else
			{
				b	= (BYTE)( a*256.);
				n	= b;
			}
		}

		void operator=(double a)
		{	
			f	= (float)a;	
			if ( f==1.0f )
			{
				b	= 255;
				n	= 255;
			}
			else
			{
				b	= (BYTE)( a*256.);
				n	= b;
			}
		}
		
		operator float() 
		{	return f;	}
		operator double() 
		{	return f;	}
		operator int() 
		{	return n;	}
		operator BYTE() 
		{	return b;	}

		_xRGB	operator+(_xRGB v)
		{
			_xRGB ret; 	ret.f	= f+v.f; ret.b	= b+v.b; ret.n	= n+v.n;
			return ret;
		}
		_xRGB	operator-(_xRGB v)
		{
			_xRGB ret;	ret.f	= f-v.f; ret.b	= b-v.b; ret.n	= n-v.n;
			return ret;
		}

		_xRGB	operator*(float d)
		{
			_xRGB ret;	
			ret.f	= f*d; 
			if ( ret.f>=1.0f )	ret.f	= 1.0;
			ret.b = (byte)ret.f*255; ret.n	= (int)ret.f*255;
			return ret;
		}
		_xRGB	operator*(double dd)
		{
			float d = (float)dd;
			_xRGB ret;	
			ret.f	= f*d; 
			if ( ret.f>=1.0f )	ret.f	= 1.0;
			ret.b = (byte)ret.f*255; ret.n	= (int)ret.f*255;
			return ret;
		}

		_xRGB	operator*(int n)
		{
			_xRGB ret;
			ret.n	= ret.n*n;
			if ( ret.n>255)	while(ret.n>255) ret.n-=256;
			if ( ret.n<0)	while(ret.n<0 )	ret.n+=256;
			ret.b	= (BYTE)ret.n;
			ret.f	= (float)ret.n;
			ret.f	/=256.;
			return ret;
		}

		_xRGB	operator*(BYTE b)
		{
			int n = (int)b;
			return operator*(n);
		}

		// f and union of b & n can not be used at same union structure
		// because f = b/256 and this can not be found automatically.
		float	f;
		union
		{
			byte	b;
			int		n;
		};
};

class xRGB
{
public:
		xRGB()											
		{ 
			r	= 0;
			g	= 0;
			b	= 0;
			x	= 255;
		}
		
		xRGB( int ra,int ga,int ba,int xa=255 )			
		{ r=ra;g=ga;b=ba;x = xa;  }
		
		xRGB( COLORREF rgb )							
		{	
			r= GetRValue(rgb); 
			g= GetGValue(rgb);  
			b= GetBValue(rgb); 
			x= 255;
		}

		xRGB( float ra,float ga,float ba,float xa=1. )	
		{ 
			r = ra,g=ga,b=ba,x= xa; 
		}
		
		xRGB( double ra,double ga,double ba,double xa=1.)	
		{ 
			r	= (float)ra;
			g	= (float)ga;
			b	= (float)ba;
			x	= (float)xa; 
		}

		xRGB( float *p)
		{
			r	= p[0]; g	= p[1];	b	= p[2]; x	= 1;
		}

		xRGB( double *p)
		{
			r	= p[0]; g	= p[1];	b	= p[2]; x	= 1;
		}

public:
		
		xRGB	operator+( xRGB a)
		{
			xRGB temp;	temp.r	= r+a.r; temp.g	= g+a.g; temp.b	= b+a.b; temp.x	= x;		
			return temp;
		}
		xRGB	operator-( xRGB a)
		{
			xRGB temp;	temp.r	= r-a.r; temp.g	= g-a.g; temp.b	= b-a.b; temp.x	= x;		
			return temp;
		}
		xRGB	operator*(float f)	
		{
			xRGB temp;	temp.r = r*f; temp.g = g*f; temp.b = b*f;
			return temp;
		}
		xRGB	operator*(double f)	
		{
			xRGB temp;	temp.r = r*f; temp.g = g*f; temp.b = b*f;
			return temp;
		}
		xRGB Mul(xRGB v)
		{
			xRGB temp; temp.r = r.f*v.r.f; temp.g = g.f*v.g.f; temp.b = b.f*v.b.f;
			return temp;
		}
		float Dot(xRGB v)
		{
			return r.f*v.r.f+g.f*v.g.f+b.f*v.b.f;
		}

		operator COLORREF()								
		{ return RGB( r.b,g.b,b.b );}

		void	Clear()									
		{ r= 0;
		  g= 0;
		  b= 0; 
		  x= 255; 
		}

		operator float*()								
		{
			rgb[0]	= r.f;
			rgb[1]	= g.f;
			rgb[2]	= b.f;
			rgb[3]	= x.f;
			return rgb; 
		}
	
		void operator =(long c)
		{
			r	= (BYTE)c;
			g	= ((BYTE) (((WORD) (c)) >> 8)); 
			b	= ((BYTE) (((DWORD) (c)) >> 16)); 			
			x	= ((BYTE) (((DWORD) (c)) >> 24));
		}
#ifdef __AFXWIN_H__
		void Serialize( CArchive &ar)
		{
			if (ar.IsStoring())
			{
				ar.Write(&r,sizeof(_xRGB));
				ar.Write(&g,sizeof(_xRGB));
				ar.Write(&b,sizeof(_xRGB));
				ar.Write(&x,sizeof(_xRGB));
				ar.Write(rgb,4*sizeof(float));
			}
			else
			{
				ar.Read(&r,sizeof(_xRGB));
				ar.Read(&g,sizeof(_xRGB));
				ar.Read(&b,sizeof(_xRGB));
				ar.Read(&x,sizeof(_xRGB));
				ar.Read(rgb,4*sizeof(float));
			}
		}
#endif


#ifdef DH3D_DX		
		/*
		operator D3DCOLORVALUE()						
		{
			D3DCOLORVALUE ret;
			ret.r	= rf;
			ret.g	= gf;
			ret.b	= bf;
			ret.a	= xf;
			return ret;
		}
		*/
#endif

protected:

public:

		_xRGB	r,g,b,x;

		float   rgb[4];
};



#endif



