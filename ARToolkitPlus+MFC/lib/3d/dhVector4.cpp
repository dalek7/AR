#include "stdafx.h"
#include "dhVector4.h"

dhVector4::dhVector4(const dhVector& vec) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = 0;
}



/////////////////
// Implementation
/////////////////

dhVector4::dhVector4( float _x,
						float _y,
						float _z,
						float _w ):x(_x), y(_y), z(_z), w(_w) {}

// Getters and Setters.
//
//

void dhVector4::Vector( REAL x1,REAL y1,REAL z1, REAL w1)
{
    x = x1;
    y = y1;
    z = z1;
	w = w1;
}

void dhVector4::Set( const dhVector4& vec ) {
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}


// Check similarity/equality.
//
//
bool dhVector4::operator==( const dhVector4& vec ) const 
{
	return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
}

bool dhVector4::operator!=( const dhVector4& vec ) const 
{
	return (x != vec.x) || (y != vec.y) || (z != vec.z) || (w != vec.w);
}




// Additions and Subtractions.
//
//
dhVector4 dhVector4::operator+( const dhVector4& vec ) const {
	return dhVector4( x+vec.x, y+vec.y, z+vec.z, w+vec.w);
}

dhVector4& dhVector4::operator+=( const dhVector4& vec ) {
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

dhVector4 dhVector4::operator-( const float f ) const {
	return dhVector4( x-f, y-f, z-f, w-f );
}

dhVector4& dhVector4::operator-=( const float f ) {
	x -= f;
	y -= f;
	z -= f;
	w -= f;
	return *this;
}

dhVector4 dhVector4::operator-( const dhVector4& vec ) const {
	return dhVector4( x-vec.x, y-vec.y, z-vec.z, w-vec.w );
}

dhVector4& dhVector4::operator-=( const dhVector4& vec ) {
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

dhVector4 dhVector4::operator+( const float f ) const {
	return dhVector4( x+f, y+f, z+f, w+f );
}

dhVector4& dhVector4::operator+=( const float f ) {
	x += f;
	y += f;
	z += f;
	w += f;
	return *this;
}

dhVector4 dhVector4::operator-() const {
	return dhVector4( -x, -y, -z, -w );
}


// Scalings
//
//
dhVector4 dhVector4::operator*( const dhVector4& vec ) const {
	return dhVector4( x*vec.x, y*vec.y, z*vec.z, w*vec.w );
}

dhVector4& dhVector4::operator*=( const dhVector4& vec ) {
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;
	return *this;
}

dhVector4 dhVector4::operator*( const float f ) const {
	return dhVector4( x*f, y*f, z*f, w*f );
}

dhVector4& dhVector4::operator*=( const float f ) {
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

dhVector4 dhVector4::operator/( const dhVector4& vec ) const {
	return dhVector4( vec.x!=0 ? x/vec.x : x , vec.y!=0 ? y/vec.y : y, vec.z!=0 ? z/vec.z : z, vec.w!=0 ? w/vec.w : w  );
}

dhVector4& dhVector4::operator/=( const dhVector4& vec ) 
{
	vec.x!=0 ? x/=vec.x : x;
	vec.y!=0 ? y/=vec.y : y;
	vec.z!=0 ? z/=vec.z : z;
	vec.w!=0 ? w/=vec.w : w;
	return *this;
}

dhVector4 dhVector4::operator/( const float f ) const 
{
	if(f == 0) return dhVector4(x, y, z, w);
	
	return dhVector4( x/f, y/f, z/f, w/f );
}

dhVector4& dhVector4::operator/=( const float f ) 
{
	if(f == 0)return *this;
	
	x /= f;
	y /= f;
	z /= f;
	w /= f;
	return *this;
}


dhVector4& dhVector4::Scaling( const float length ) 
{
	float l = (float)sqrt(x*x + y*y + z*z + w*w);
	if (l > 0) 
	{
		x = (x/l)*length;
		y = (y/l)*length;
		z = (z/l)*length;
		w = (w/l)*length;
	}
	return *this;
}





// Distance between two points.
//

REAL dhVector4::Distance( const dhVector4& pnt) 
{
	float vx = x-pnt.x;
	float vy = y-pnt.y;
	float vz = z-pnt.z;
	float vw = w-pnt.w;
	return (float)sqrt( vx*vx + vy*vy + vz*vz + vw*vw );
}



// Normalization
//
//

dhVector4& dhVector4::Normalize() {
	float lenght = (float)sqrt(x*x + y*y + z*z + w*w);
	if( lenght > 0 ) {
		x /= lenght;
		y /= lenght;
		z /= lenght;
		w /= lenght;
	}
	return *this;
}



// Length
//
//

float dhVector4::Length() const 
{
	return (float)sqrt( x*x + y*y + z*z + w*w );
}


REAL dhVector4::Abs()
{
    return Length();
}

REAL dhVector4::Norm()
{
	return Length();
}

/**
 * Dot Product.
 */
REAL dhVector4::Dot( const dhVector4& vec ) 
{
	return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
}


// Non-Member operators
//
//
dhVector4 operator+( float f, const dhVector4& vec ) {
    return dhVector4( f+vec.x, f+vec.y, f+vec.z, f+vec.w );
}

dhVector4 operator-( float f, const dhVector4& vec ) {
    return dhVector4( f-vec.x, f-vec.y, f-vec.z, f-vec.w );
}

dhVector4 operator*( float f, const dhVector4& vec ) {
    return dhVector4( f*vec.x, f*vec.y, f*vec.z, f*vec.w );
}

dhVector4 operator/( float f, const dhVector4& vec ) {
    return dhVector4( f/vec.x, f/vec.y, f/vec.z, f/vec.w);
}




