#ifndef __dhMat__
#define __dhMat__

#include "dhVector.h"

#include "dhQuat.h"
		 /*
     0 4  8  12
	 1 5  9  13
	 2 6 10  14
	 3 7 11  15 
*/

class dhMat
{
public:
		dhMat();
		dhMat(float x,float y,float z);
		dhMat(dhVector);
public:
		dhMat	H(float rx,float ry,float rz,dhVector);
		dhMat	RotX(float);
		dhMat	RotY(float);
		dhMat	RotZ(float);
		dhMat	Trans(float,float,float);
		dhMat	Trans(dhVector);
		dhMat	Scale(float,float,float);
		dhMat	Scale(float);
		dhMat	DeScale();	// remove scale
		void	I();
		dhMat	T();	// transpose
		dhMat	operator+(dhVector&);
		dhMat	operator+(dhMat&);
		dhMat	operator-(dhMat&);
		dhMat	operator*(dhMat&);
		dhMat	operator*(float);
		dhVector operator*(dhVector&);
		void	operator=(dhVector&);
		dhVector O();
		dhMat	Inv();
		dhMat	R();
		dhMat	RotAxis( dhVector& o,dhVector& d,float);	// rotation when prevous coordinate is world coordinate.
		dhMat	RotAxis( dhVector& o,dhVector& d,float,dhMat prev);	// rotation about an arbitrary axis on previous coordinate
		dhMat	RotAxis(dhVector& d);
		dhVector	RPY();

public:
	//20131003
	
	
	//	GLKMatrix4.h @Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk/System/Library/Frameworks/GLKit.framework/Headers/


	//20131101

	//of_v0.7.4_vs2010_release\libs\openFrameworks\math\ofMatrix3x3.cpp
	float determinant() const;
    float determinant(const dhMat& A);

	//20140521 - ofMatrix4x4
	void setRotate(const dhQuat & q);
	dhQuat getRotate() const;
	void setTranslation(const dhVector& v);

public:
		float	v[16];
};



inline dhMat Matrix4MakeRotation(dhMat matrix, float radians, float x, float y, float z)
{
	dhMat rm = Matrix4MakeRotation(matrix, radians, x, y, z);

	return matrix*rm;
}
inline dhMat Matrix4Rotate(dhMat matrix, float radians, float x, float y, float z)
{
	dhVector p  = dhVector(x, y, z);
	p = p.Unit();

	dhMat ret;
	
	float cosv = cosf(radians);
	float cosp = 1.0f - cosv;
	float sinv = sinf(radians);

	float v[16] = { cosv + cosp * p.x * p.x,
					 cosp * p.x * p.y + p.z * sinv,
					 cosp * p.x * p.z - p.y * sinv,
					 0.0f,
					 cosp * p.x * p.y - p.z * sinv,
					 cosv + cosp * p.y * p.y,
					 cosp * p.y * p.z + p.x * sinv,
					 0.0f,
					 cosp * p.x * p.z + p.y * sinv,
					 cosp * p.y * p.z - p.x * sinv,
					 cosv + cosp * p.z * p.z,
					 0.0f,
					 0.0f,
					 0.0f,
					 0.0f,
					 1.0f };


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



#endif