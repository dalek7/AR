#ifndef __dhQuat__
#define __dhQuat__

#include "dhVector.h"
#include "dhVector4.h"

//Quaternion

class dhMat;

class dhQuat
{
public:
		dhVector4 _v;

		dhQuat();
		dhQuat(float x,float y,float z);
		dhQuat(float x, float y, float z, float w);
		dhQuat(float angle, const dhVector& axis);

		dhQuat(const dhVector4& v);

		void Set(float x, float y, float z, float w);
		void Set(const dhVector4& v);

		float& x();
		float& y();
		float& z();
		float& w();
		
		float x() const;
		float y() const;
		float z() const;
		float w() const;
			
		
		//dhQuat(float angle1, const dhVector& axis1, float angle2, const dhVector& axis2, float angle3, const dhVector& axis3);
	    

		 // Return the angle and vector components represented by the quaternion.
		void getRotate(float&angle, float& x, float& y, float& z) const;
		void getRotate(float& angle, dhVector& vec) const;
	    
		// calculate and return the rotation as euler angles
		dhVector getEuler() const;


		// Conjugate
		dhQuat conj() const;

		

		// METHODS RELATED TO ROTATIONS
		// Set a quaternion which will perform a rotation of an
		// angle around the axis given by the vector(x,y,z).
		// Should be written to also accept an angle and a Vec3?

		// Define Spherical Linear interpolation method also
		void makeRotate(float angle, float x, float y, float z);
		void makeRotate(float angle, const dhVector& vec);		
		// same as static __inline__ GLKQuaternion GLKQuaternionMakeWithAngleAndVector3Axis(float radians, GLKVector3 axisVector);
		
		void makeRotate(float angle1, const dhVector& axis1, float angle2, const dhVector& axis2, float angle3, const dhVector& axis3);


		dhMat Matrix4MakeWithQuaternion(dhQuat quaternion);

		// Make a rotation Quat which will rotate vec1 to vec2.
		// Generally take a dot product to get the angle between these
		// and then use a cross product to get the rotation axis
		// Watch out for the two special cases when the vectors
		// are co-incident or opposite in direction.
		void makeRotate(const dhVector& vec1, const dhVector& vec2);



		dhQuat operator*(const dhQuat& rhs) const;     // Binary multiply

		dhQuat& operator *=(float rhs);                // Unary multiply by scalar
		const dhQuat operator *(float rhs) const;                  // Multiply by scalar


		const dhQuat operator +(const dhQuat& rhs) const;    // Binary addition
		dhQuat& operator +=(const dhQuat& rhs);              // Unary addition


		dhQuat& operator -=(const dhQuat& rhs);        // Unary subtraction
  
		const dhQuat operator -() const;               // returns the negative of the quaternion. calls operator -() on the Vec4 */



		void normalize();


		// ofQuaternion 
		// 2014/05/21
		//----------------------------------------
		float length() const {
			return sqrt(_v.x*_v.x + _v.y*_v.y + _v.z*_v.z + _v.w*_v.w);
		}


		//----------------------------------------
		float length2() const {
			return _v.x*_v.x + _v.y*_v.y + _v.z*_v.z + _v.w*_v.w;
		}



public:
	//2013/10/27
    // Spherical Linear Interpolation.
    // As t goes from 0 to 1, the Quat object goes from "from" to "to".
    void slerp(float t, const dhQuat& from, const dhQuat& to);


	dhVector4 asVec4() const 
	{
		return _v;
	}



public:
		//float	v[16];
};

#endif