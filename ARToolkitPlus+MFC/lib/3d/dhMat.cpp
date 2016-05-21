#include "stdafx.h"
#include "dhMat.h"

#ifndef PI
#define PI              3.14159265358979323   /* 180 deg */
#endif

#ifndef DEG
#define DEG(a)          (180.*a/PI )
#endif 

#ifndef RAD
#define RAD(a)          (PI*a/180.)
#endif 

dhMat::dhMat()
{
	I();	
}

dhMat::dhMat(float x,float y,float z)
{
	I();	
	v[12] = x;
	v[13] = y;
	v[14] = z;
}

dhMat::dhMat(dhVector p)
{
	I();
	v[12] = p.x;
	v[13] = p.y;
	v[14] = p.z;
}

void dhMat::I()
{
	memset( v,0,sizeof(float)*16);
	v[0]	= 1;
	v[5]	= 1;
	v[10]	= 1;
	v[15]	= 1;
}

dhMat dhMat::RotX( float f)
{
	dhMat ret;
	ret.v[5]	= cos(f);
	ret.v[9]	= -sin(f);
	ret.v[6]	= sin(f);
	ret.v[10]	= cos(f);
	return ret;
}

dhMat dhMat::RotY( float f)
{
	dhMat ret;
	ret.v[0]	= cos(f);
	ret.v[8]	= sin(f);
	ret.v[2]	= -sin(f);
	ret.v[10]	= cos(f);
	return ret;
}

dhMat dhMat::RotZ( float f)
{
	dhMat ret;
	ret.v[0]	= cos(f);
	ret.v[4]	= -sin(f);
	ret.v[1]	= sin(f);
	ret.v[5]	= cos(f);
	return ret;
}


dhMat dhMat::Trans(float x,float y,float z)
{
	dhMat ret;
	ret.v[12]	= x;
	ret.v[13]	= y;
	ret.v[14]	= z;
	return ret;
}

dhMat dhMat::Trans( dhVector v)
{
	dhMat ret;
	ret.v[12]	= v.x;
	ret.v[13]	= v.y;
	ret.v[14]	= v.z;
	return ret;
}

dhMat dhMat::operator+(dhVector& v)
{
	dhMat ret	= *this;
	ret.v[12]	=	this->v[12]+v.x;
	ret.v[13]	=	this->v[13]+v.y;
	ret.v[14]	=	this->v[14]+v.z;
	return ret;
}

dhMat dhMat::operator+(dhMat& m)
{
	dhMat ret;
	for (int i=0;i<16;i++)
	ret.v[i] = v[i]+m.v[i];
	return ret;
}

dhMat dhMat::operator-(dhMat& m)
{
	dhMat ret;
	for (int i=0;i<16;i++)
	ret.v[i] = v[i]-m.v[i];
	return ret;
}

dhMat dhMat::operator *(dhMat& m)
{
	dhMat ret;
	for ( int i=0;i<4;i++)
	for ( int j=0;j<4;j++)
	{
		float sum = 0;

		for( int k=0;k<4;k++)
		sum+=( v[k*4+i]*m.v[j*4+k] );
			
		ret.v[j*4+i]	= sum;
	}

	return ret;
}

dhMat dhMat::operator *(float f)
{
	dhMat ret;
	for ( int i=0;i<4;i++)
	for ( int j=0;j<4;j++)
		ret.v[j*4+i]	= v[j*4+i]*f;

	ret.v[15] = 1;

	return ret;
}

dhVector dhMat::O()
{
	dhVector ret( v[12],v[13],v[14]);
	return ret;
}

dhMat dhMat::R()
{
	dhMat ret;
	memmove( ret.v,v,sizeof(float)*12);
	return ret;
}

dhMat dhMat::T()
{
	dhMat ret;
	ret.v[0]	= v[0];
	ret.v[1]	= v[4];
	ret.v[2]	= v[8];
	ret.v[3]	= v[12];

	ret.v[4]	= v[1];
	ret.v[5]	= v[5];
	ret.v[6]	= v[9];
	ret.v[7]	= v[13];

	ret.v[8]	= v[2];
	ret.v[9]	= v[6];
	ret.v[10]	= v[10];
	ret.v[11]	= v[14];

	ret.v[12]	= v[3];
	ret.v[13]	= v[7];
	ret.v[14]	= v[11];
	ret.v[15]	= v[15];

	return ret;
}

dhMat dhMat::H( float rx,float ry,float rz,dhVector p)
{
	dhMat ret;
	ret = ret.Trans(p)*ret.RotZ(RAD(rz))*ret.RotY(RAD(ry))*ret.RotX(RAD(rx));
	return ret;
}

dhVector dhMat::operator *(dhVector &h)
{
	dhVector ret;
	ret.x	= v[0]*h.x+ v[4]*h.y+v[8]*h.z+ v[12];
	ret.y	= v[1]*h.x+ v[5]*h.y+v[9]*h.z+ v[13];
	ret.z	= v[2]*h.x+ v[6]*h.y+v[10]*h.z+v[14];
	return ret;
}

void dhMat::operator =(dhVector &p)
{
	v[12]	= p.x;
	v[13]	= p.y;
	v[14]	= p.z;
}

dhMat dhMat::RotAxis( dhVector& o,dhVector& d,float f)
{
	dhMat unit;
	return RotAxis(o,d,f,unit);
}

dhMat dhMat::RotAxis( dhVector& o,dhVector& d,float f,dhMat prev)
{
	float a,b,c,s;
	
	// alpha angle between projected vector on xy plane and x axis
	dhVector pv	= d;
	pv.z	= 0;
	pv		= pv.Unit();
	
	c		= pv.x;
	c		= acos(c);
	s		= asin(pv.y);

	if ( c>0)	
	{
		dhVector v = dhVector(1,0,0)*pv;
		if ( v.z<0)	c*=-1;
	}
	else
		if ( s<0)	c	= 2*PI-c;

	a	= c;

	// beta angle between z axis and d-o;
	c	= d.Unit().z;
	b	= acos(c);
	if ( b==0)	a	= 0;
	
	dhMat rot;
	rot	= rot.RotZ(a)*rot.RotY(b)*rot.RotZ(f)*rot.RotY(-b)*rot.RotZ(-a);
	rot	= rot.Trans(prev.O())*rot.Trans(o*-1)*rot*rot.Trans(o)*rot.Trans(prev.O()*-1);
	//rot	= prev*rot.Trans(o*-1)*rot*rot.Trans(o)*prev.Inv();
	
	return rot;
}


void dhMat::setTranslation(const dhVector& v1)
{
	v[12] = v1.x;
	v[13] = v1.y;
	v[14] = v1.z;

}

#define QX  q._v[0]
#define QY  q._v[1]
#define QZ  q._v[2]
#define QW  q._v[3]

#ifndef EPSILON
#define EPSILON    (1.0E-8)
#endif

void dhMat::setRotate(const dhQuat& q)
{
    double length2 = q.length2();
    if (fabs(length2) <= EPSILON)
    {
		I();
		/*
        _mat[0][0] = 1.0; _mat[1][0] = 0.0; _mat[2][0] = 0.0;
        _mat[0][1] = 0.0; _mat[1][1] = 1.0; _mat[2][1] = 0.0;
        _mat[0][2] = 0.0; _mat[1][2] = 0.0; _mat[2][2] = 1.0;
		*/
    }
    else
    {
        double rlength2;
        // normalize quat if required.
        // We can avoid the expensive sqrt in this case since all 'coefficients' below are products of two q components.
        // That is a square of a square root, so it is possible to avoid that
        if (length2 != 1.0)
        {
            rlength2 = 2.0/length2;
        }
        else
        {
            rlength2 = 2.0;
        }

        // Source: Gamasutra, Rotating Objects Using Quaternions
        //
        //http://www.gamasutra.com/features/19980703/quaternions_01.htm

        double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

        // calculate coefficients
        x2 = rlength2*QX;
        y2 = rlength2*QY;
        z2 = rlength2*QZ;

        xx = QX * x2;
        xy = QX * y2;
        xz = QX * z2;

        yy = QY * y2;
        yz = QY * z2;
        zz = QZ * z2;

        wx = QW * x2;
        wy = QW * y2;
        wz = QW * z2;

        // Note.  Gamasutra gets the matrix assignments inverted, resulting
        // in left-handed rotations, which is contrary to OpenGL and OSG's
        // methodology.  The matrix assignment has been altered in the next
        // few lines of code to do the right thing.
        // Don Burns - Oct 13, 2001

		I();

        v[0] = 1.0 - (yy + zz);	//_mat[0][0]
        v[1] = xy - wz;	//_mat[1][0]
        v[2] = xz + wy;	//_mat[2][0]


        v[4] = xy + wz;	//_mat[0][1]
        v[5] = 1.0 - (xx + zz);	//_mat[1][1]
        v[6] = yz - wx;	//_mat[2][1]

        v[8] = xz - wy;	//_mat[0][2] 
        v[9] =  yz + wx;	//_mat[1][2]
        v[10] = 1.0 - (xx + yy);	//_mat[2][2] 
    }


}
//

//Check this again !!
dhQuat dhMat::getRotate() const
{
    dhQuat q;

    dhMat mat = *this;
    //ofVec3f vs = mat.getScale();
    //mat.scale(1./vs.x,1./vs.y,1./vs.z);

    //ofVec4f* m = mat._mat;

	float *mv = mat.v;

	dhVector m[4];
	m[0].Vector(mv[0], mv[1], mv[2]);
	m[1].Vector(mv[4], mv[5], mv[6]);
	m[2].Vector(mv[8], mv[9], mv[10]);
	m[3].Vector(mv[12], mv[13], mv[14]);


    // Source: Gamasutra, Rotating Objects Using Quaternions
    //
    //http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm

    float  tr, s;
    float tq[4];
    int    i, j, k;

    int nxt[3] = {1, 2, 0};

    tr = m[0][0] + m[1][1] + m[2][2]+1.0;

    // check the diagonal
    if (tr > 0.0)
    {
        s = (float)sqrt (tr);
        QW = s / 2.0;
        s = 0.5 / s;
        QX = (m[1][2] - m[2][1]) * s;
        QY = (m[2][0] - m[0][2]) * s;
        QZ = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        // diagonal is negative
        i = 0;
        if (m[1][1] > m[0][0])
            i = 1;
        if (m[2][2] > m[i][i])
            i = 2;
        j = nxt[i];
        k = nxt[j];

        s = (float)sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0);

        tq[i] = s * 0.5;

        if (s != 0.0)
            s = 0.5 / s;

        tq[3] = (m[j][k] - m[k][j]) * s;
        tq[j] = (m[i][j] + m[j][i]) * s;
        tq[k] = (m[i][k] + m[k][i]) * s;

        QX = tq[0];
        QY = tq[1];
        QZ = tq[2];
        QW = tq[3];
    }

    return q;
}
dhMat dhMat::RotAxis( dhVector& d)
{
	float a,b,c,s;
	
	// alpha angle between projected vector on xy plane and x axis
	dhVector pv	= d;
	pv.z	= 0;
	pv		= pv.Unit();
	
	c		= pv.x;
	c		= acos(c);
	s		= asin(pv.y);

	if ( c>0)	
	{
		dhVector v = dhVector(1,0,0)*pv;
		if ( v.z<0)	c*=-1;
	}
	else
		if ( s<0)	c	= 2*PI-c;

	a	= c;

	// beta angle between z axis and d-o;
	c	= d.Unit().z;
	b	= acos(c);
	if ( b==0)	a	= 0;
	
	dhMat rot;
	rot	= rot.RotZ(a)*rot.RotY(b);
	return rot;
}

dhMat dhMat::Inv()
{
	dhMat ret;
	ret.v[0]	= v[0];
	ret.v[1]	= v[4];
	ret.v[2]	= v[8];
	ret.v[4]	= v[1];
	ret.v[5]	= v[5];
	ret.v[6]	= v[9];
	ret.v[8]	= v[2];
	ret.v[9]	= v[6];
	ret.v[10]	= v[10];

	dhVector pos = O();
	pos	= ret*(pos*-1);

	ret.v[12]	= pos.x;
	ret.v[13]	= pos.y;
	ret.v[14]	= pos.z;
	return ret;
}

// X-Y-Z fixed angles
// R_XYZ*(r, b, a)
// actual : r about X_A --> b about Y_A --> a about Z_A
dhVector dhMat::RPY()
{
	//RPY( a,b,c )	= Rot(z,a)Rot(y,b)Rot(x,c)
	/*
		[Cos(a)*Cos(b), -(Cos(c)*Sin(a)) + Cos(a)*Sin(b)*Sin(c), Cos(a)*Cos(c)*Sin(b) + Sin(a)*Sin(c)), 
		[Cos(b)*Sin(a), Cos(a)*Cos(c) + Sin(a)*Sin(b)*Sin(c)   , Cos(c)*Sin(a)*Sin(b) - Cos(a)*Sin(c)), 
		[-Sin(b)      , Cos(b)*Sin(c)                          , Cos(b)*Cos(c)
	*/

	float a,b,r,cb;	// from P.47 of J.Craig.
	b	= atan2( -v[2],sqrt( v[0]*v[0] + v[1]*v[1]) );
	cb	= cos(b);

	
	a	= atan2( v[1]/cb,v[0]/cb);
	r	= atan2( v[6]/cb,v[10]/cb);

	// need to consider the degenerate cases, cb = 0


	return dhVector(a,b,r);
}



dhMat dhMat::Scale(float x,float y,float z)
{
	dhMat ret;
	/*
	ret.v[3]	= x;
	ret.v[7]	= y;
	ret.v[11]	= z;
	*/

	return ret;
}

dhMat dhMat::Scale(float r)
{
	return Scale(r,r,r);
}

dhMat dhMat::DeScale()
{
	dhVector s;
	s.x	= sqrt(v[0]*v[0]+ v[4]*v[4]+ v[8]*v[8]);
	s.y	= sqrt(v[1]*v[1]+ v[5]*v[5]+ v[9]*v[9]);
	s.z	= sqrt(v[2]*v[2]+ v[6]*v[6]+ v[10]*v[10]);

	dhMat ret;
	memmove( ret.v,this->v,sizeof(float)*16);
	ret.v[0]/=s.x;
	ret.v[4]/=s.x;
	ret.v[8]/=s.x;
	ret.v[1]/=s.y;
	ret.v[5]/=s.y;
	ret.v[9]/=s.y;
	ret.v[2]/=s.z;
	ret.v[6]/=s.z;
	ret.v[10]/=s.z;
	
	return ret;
}


 /**
    * [ a b c ]
    * [ d e f ]
    * [ g h i ]
    */

float dhMat::determinant() const 
{
	
	float a = this->v[0];
    float b = this->v[4];
    float c = this->v[8];
    float d = this->v[1];
    float e = this->v[5];
    float f = this->v[9];
    float g = this->v[2];
    float h = this->v[6];
    float i = this->v[10];

	float det = a * e * i
			   + b * f * g
			   + d * h * c
			   - g * e * c
			   - d * b * i
			   - h * f * a;
	return det;
}





// Static

