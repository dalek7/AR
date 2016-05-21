#include "stdafx.h"
#include "dhQuat.h"
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


dhQuat::dhQuat()
{
	_v.Vector(0, 0, 0, 1);
}

dhQuat::dhQuat(float x, float y, float z, float w) 
{
    _v.Vector(x, y, z, w);
}





dhQuat::dhQuat(float angle, const dhVector& axis)
{
	makeRotate(angle, axis);

}
void dhQuat::Set(float x, float y, float z, float w) 
{
    _v.Vector(x, y, z, w);
}


//----------------------------------------
void dhQuat::Set(const dhVector4& v) 
{
    _v = v;
}


const dhQuat dhQuat::operator *(float rhs) const 
{
    return dhQuat(_v.x*rhs, _v.y*rhs, _v.z*rhs, _v.w*rhs);
}



dhQuat dhQuat::operator*(const dhQuat& rhs) const 
{
    return dhQuat(		rhs._v.w*_v.x + rhs._v.x*_v.w + rhs._v.y*_v.z - rhs._v.z*_v.y,
                        rhs._v.w*_v.y - rhs._v.x*_v.z + rhs._v.y*_v.w + rhs._v.z*_v.x,
                        rhs._v.w*_v.z + rhs._v.x*_v.y - rhs._v.y*_v.x + rhs._v.z*_v.w,
                        rhs._v.w*_v.w - rhs._v.x*_v.x - rhs._v.y*_v.y - rhs._v.z*_v.z);
}

//----------------------------------------
dhQuat& dhQuat::operator *=(float rhs) {
    _v.x *= rhs;
    _v.y *= rhs;
    _v.z *= rhs;
    _v.w *= rhs;
    return *this; // enable nesting
}

//----------------------------------------
const dhQuat dhQuat::operator +(const dhQuat& rhs) const {
    return dhQuat(_v.x + rhs._v.x, _v.y + rhs._v.y,
                        _v.z + rhs._v.z, _v.w + rhs._v.w);
}


//----------------------------------------
dhQuat& dhQuat::operator +=(const dhQuat& rhs) {
    _v.x += rhs._v.x;
    _v.y += rhs._v.y;
    _v.z += rhs._v.z;
    _v.w += rhs._v.w;
    return *this; // enable nesting
}


//----------------------------------------
dhQuat& dhQuat::operator -=(const dhQuat& rhs) 
{
    _v.x -= rhs._v.x;
    _v.y -= rhs._v.y;
    _v.z -= rhs._v.z;
    _v.w -= rhs._v.w;
    return *this; // enable nesting
}

//----------------------------------------
const dhQuat dhQuat::operator -() const 
{
    return dhQuat(-_v.x, -_v.y, -_v.z, -_v.w);
}

float& dhQuat::x() 
{
    return _v.x;
}



float& dhQuat::y() 
{
    return _v.y;
}



float& dhQuat::z() 
{
    return _v.z;
}



float& dhQuat::w() {
    return _v.w;
}



float dhQuat::x() const 
{
    return _v.x;
}



float dhQuat::y() const 
{
    return _v.y;
}



float dhQuat::z() const 
{
    return _v.z;
}



float dhQuat::w() const 
{
    return _v.w;
}

dhQuat dhQuat::conj() const {
    return dhQuat(-_v.x, -_v.y, -_v.z, _v.w);
}

//----------------------------------------

void dhQuat::getRotate( float& angle, dhVector& vec ) const 
{
	float x, y, z;
	getRotate(angle, x, y, z);
	vec.x = x;
	vec.y = y;
	vec.z = z;
}

// Get the angle of rotation and axis of this Quat object.
// Won't give very meaningful results if the Quat is not associated
// with a rotation!
void dhQuat::getRotate( float& angle, float& x, float& y, float& z ) const 
{
	float sinhalfangle = sqrt( _v.x * _v.x + _v.y * _v.y + _v.z * _v.z );

	angle = 2.0 * atan2( sinhalfangle, _v.w );
	if (sinhalfangle) {
		x = _v.x / sinhalfangle;
		y = _v.y / sinhalfangle;
		z = _v.z / sinhalfangle;
	} else {
		x = 0.0;
		y = 0.0;
		z = 1.0;
	}
	
	angle = DEG(angle);
}



// ref at http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
dhVector dhQuat::getEuler() const 
{
	float test = x()*y() + z()*w();
	float heading;
	float attitude;
	float bank;
	if (test > 0.499) { // singularity at north pole
		heading = 2 * atan2(x(), w());
		attitude = PI/2;
		bank = 0;
	} else if (test < -0.499) { // singularity at south pole
		heading = -2 * atan2(x(), w());
		attitude = - PI/2;
		bank = 0;
	} else {
		float sqx = x() * x();
		float sqy = y() * y();
		float sqz = z() * z();
		heading = atan2(2.0f * y() * w() - 2.0f * x() * z(), 1.0f - 2.0f*sqy - 2.0f*sqz);
		attitude = asin(2*test);
		bank = atan2(2.0f*x() * w() - 2.0f * y() * z(), 1.0f - 2.0f*sqx - 2.0f*sqz);
	}
	
	return dhVector(DEG(attitude), DEG(heading), DEG(bank));
}



// Set the elements of the Quat to represent a rotation of angle
// (degrees) around the axis (x,y,z)
// http://www.cprogramming.com/tutorial/3d/quaternions.html
void dhQuat::makeRotate( float angle, float x, float y, float z ) 
{
	angle = RAD(angle);//ofDegToRad
	
	const float epsilon = 0.0000001f;

	float length = sqrtf( x * x + y * y + z * z );
	if (length < epsilon) {
		// ~zero length axis, so reset rotation to zero.
		*this = dhQuat();
		return;
	}

	float inversenorm  = 1.0f / length;
	float coshalfangle = cosf( 0.5f * angle );
	float sinhalfangle = sinf( 0.5f * angle );

	_v.x = x * sinhalfangle * inversenorm;
	_v.y = y * sinhalfangle * inversenorm;
	_v.z = z * sinhalfangle * inversenorm;
	_v.w = coshalfangle;
}

//axis is a unit vector
//_v.w  = cosf( fAngle/2)
//_v.x = axis.x * sinf( fAngle/2 )
//_v.y = axis.y * sinf( fAngle/2 )
//_v.z = axis.z * sinf( fAngle/2 )

/*
GLKQuaternion.h
    
static __inline__ GLKQuaternion GLKQuaternionMakeWithAngleAndAxis(float radians, float x, float y, float z)
{
    float halfAngle = radians * 0.5f;
    float scale = sinf(halfAngle);
    GLKQuaternion q = { scale * x, scale * y, scale * z, cosf(halfAngle) };
    return q;
}


*/


void dhQuat::normalize()
{
	float len = _v.w*_v.w + _v.x*_v.x + _v.y*_v.y + _v.z*_v.z;
	float factor = 1.0f / sqrt(len);
	_v.x *= factor;
	_v.y *= factor;
	_v.z *= factor;
	_v.w *= factor;
}


void dhQuat::makeRotate(float angle, const dhVector& vec)
{
	makeRotate( angle, vec.x, vec.y, vec.z );
}


/** Make a rotation Quat which will rotate vec1 to vec2

 This routine uses only fast geometric transforms, without costly acos/sin computations.
 It's exact, fast, and with less degenerate cases than the acos/sin method.

 For an explanation of the math used, you may see for example:
 http://logiciels.cnes.fr/MARMOTTES/marmottes-mathematique.pdf

 @note This is the rotation with shortest angle, which is the one equivalent to the
 acos/sin transform method. Other rotations exists, for example to additionally keep
 a local horizontal attitude.

 @author Nicolas Brodu
 */
void dhQuat::makeRotate( const dhVector& from, const dhVector& to ) 
{

	// This routine takes any vector as argument but normalized
	// vectors are necessary, if only for computing the dot product.
	// Too bad the API is that generic, it leads to performance loss.
	// Even in the case the 2 vectors are not normalized but same length,
	// the sqrt could be shared, but we have no way to know beforehand
	// at this point, while the caller may know.
	// So, we have to test... in the hope of saving at least a sqrt
	dhVector sourceVector = from;
	dhVector targetVector = to;

	float fromLen2 = from.LengthSquared();
	float fromLen;
	// normalize only when necessary, epsilon test
	if ((fromLen2 < 1.0 - 1e-7) || (fromLen2 > 1.0 + 1e-7)) {
		fromLen = sqrt(fromLen2);
		sourceVector /= fromLen;
	} else fromLen = 1.0;

	float toLen2 = to.LengthSquared();
	// normalize only when necessary, epsilon test
	if ((toLen2 < 1.0 - 1e-7) || (toLen2 > 1.0 + 1e-7)) {
		float toLen;
		// re-use fromLen for case of mapping 2 vectors of the same length
		if ((toLen2 > fromLen2 - 1e-7) && (toLen2 < fromLen2 + 1e-7)) {
			toLen = fromLen;
		} else toLen = sqrt(toLen2);
		targetVector /= toLen;
	}


	// Now let's get into the real stuff
	// Use "dot product plus one" as test as it can be re-used later on
	double dotProdPlus1 = 1.0 + sourceVector.Dot(targetVector);

	// Check for degenerate case of full u-turn. Use epsilon for detection
	if (dotProdPlus1 < 1e-7) {

		// Get an orthogonal vector of the given vector
		// in a plane with maximum vector coordinates.
		// Then use it as quaternion axis with pi angle
		// Trick is to realize one value at least is >0.6 for a normalized vector.
		if (fabs(sourceVector.x) < 0.6) {
			const double norm = sqrt(1.0 - sourceVector.x * sourceVector.x);
			_v.x = 0.0;
			_v.y = sourceVector.z / norm;
			_v.z = -sourceVector.y / norm;
			_v.w = 0.0;
		} else if (fabs(sourceVector.y) < 0.6) {
			const double norm = sqrt(1.0 - sourceVector.y * sourceVector.y);
			_v.x = -sourceVector.z / norm;
			_v.y = 0.0;
			_v.z = sourceVector.x / norm;
			_v.w = 0.0;
		} else {
			const double norm = sqrt(1.0 - sourceVector.z * sourceVector.z);
			_v.x = sourceVector.y / norm;
			_v.y = -sourceVector.x / norm;
			_v.z = 0.0;
			_v.w = 0.0;
		}
	}

	else {
		// Find the shortest angle quaternion that transforms normalized vectors
		// into one other. Formula is still valid when vectors are colinear
		const double s = sqrt(0.5 * dotProdPlus1);
		//const dhVector tmp = sourceVector.getCrossed(targetVector) / (2.0 * s);

		const dhVector tmppp = sourceVector * targetVector;
		const dhVector tmp = sourceVector * targetVector / (2.0 * s);
		_v.x = tmp.x;
		_v.y = tmp.y;
		_v.z = tmp.z;
		_v.w = s;
	}
}


/// Spherical Linear Interpolation
/// As t goes from 0 to 1, the Quat object goes from "from" to "to"
/// Reference: Shoemake at SIGGRAPH 89
/// See also
/// http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
void dhQuat::slerp( float t, const dhQuat& from, const dhQuat& to ) {
	const double epsilon = 0.00001;
	double omega, cosomega, sinomega, scale_from, scale_to ;

	dhQuat quatTo(to);
	// this is a dot product


	cosomega = from.asVec4().Dot(to.asVec4());

	if ( cosomega < 0.0 ) {
		cosomega	= -cosomega;
		quatTo		= -to;
	}

	if ( (1.0 - cosomega) > epsilon ) {
		omega = acos(cosomega) ; // 0 <= omega <= Pi (see man acos)
		sinomega = sin(omega) ;  // this sinomega should always be +ve so
		// could try sinomega=sqrt(1-cosomega*cosomega) to avoid a sin()?
		scale_from = sin((1.0 - t) * omega) / sinomega ;
		scale_to = sin(t * omega) / sinomega ;
	} else {
		/* --------------------------------------------------
		The ends of the vectors are very close
		we can use simple linear interpolation - no need
		to worry about the "spherical" interpolation
		-------------------------------------------------- */
		scale_from = 1.0 - t ;
		scale_to = t ;
	}

	*this = (from * scale_from) + (quatTo * scale_to);

	// so that we get a Vec4
}




dhMat dhQuat::Matrix4MakeWithQuaternion(dhQuat quaternion)
{
	quaternion.normalize();
    
    float x = quaternion.x();//q[0];
    float y = quaternion.y();//.q[1];
    float z = quaternion.z();//.q[2];
    float w = quaternion.w();//.q[3];
    
    float _2x = x + x;
    float _2y = y + y;
    float _2z = z + z;
    float _2w = w + w;
    

	// check error C2552: 'm' : non-aggregates cannot be initialized with initializer list
    //    'dhMat' : Types with user defined constructors are not aggregate
	//GLKMatrix4 m
    dhMat m;
	double vm[16] = { 1.0f - _2y * y - _2z * z,
                     _2x * y + _2w * z,
                     _2x * z - _2w * y,
                     0.0f,
                     _2x * y - _2w * z,
                     1.0f - _2x * x - _2z * z,
                     _2y * z + _2w * x,
                     0.0f,
                     _2x * z + _2w * y,
                     _2y * z - _2w * x,
                     1.0f - _2x * x - _2y * y,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     1.0f };
    for(int i=0; i<16; i++)
	{
		m.v[i] = vm[i];
	}
    return m;

}


/*

	dhQuat qr (roll, Znormal);	// quat roll.
	dhQuat qp (pitch, Xnormal);	// quat pitch.
	dhQuat qh (heading, Ynormal);	// quat heading or yaw.
	dhQuat qt;					// quat total.

	qt = qr * qp * qh;

	dhVector qaxis; 
	float qangle;

    qt.getRotate(qangle, qaxis);
	
	glPushMatrix();

		glRotatef(qangle, qaxis.x, qaxis.y, qaxis.z);
	

		dhVector p7 = dhVector(0,0,0)+ObjPos;
		m_obj7.Trans(p7);
		m_obj7.Draw(FALSE, FALSE, bShowVert);

	glPopMatrix();

*/