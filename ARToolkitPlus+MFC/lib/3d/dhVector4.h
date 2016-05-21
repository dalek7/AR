#pragma once

// need to be cleaned up
// 20130828 @TCL

#include "dhVector.h"

class dhVector4 :public dhVector 
{
public:
	
	REAL			x,y,z,w;
	
	dhVector4( REAL _x=0.f, REAL _y=0.f, REAL _z=0.f, REAL _w=0.f );
	dhVector4( const dhVector& vec);
	
    // Getters and Setters.
	void Vector( REAL,REAL,REAL,REAL);
	void Set( const dhVector4& vec );
	
	float * getPtr() {
		return (float*)&x;
	}
	const float * getPtr() const {
		return (const float *)&x;
	}
	
	float& operator[]( int n ){
		return getPtr()[n];
	}
	
	float operator[]( int n ) const {
		return getPtr()[n];
	}
	
	
    // Check similarity/equality.
    //
    bool operator==( const dhVector4& vec ) const;
    bool operator!=( const dhVector4& vec ) const;
   
	
    // Additions and Subtractions.
    dhVector4  operator+( const dhVector4& vec ) const;
    dhVector4& operator+=( const dhVector4& vec );
    dhVector4  operator-( const float f ) const;
    dhVector4& operator-=( const float f );
    dhVector4  operator-( const dhVector4& vec ) const;
    dhVector4& operator-=( const dhVector4& vec );
    dhVector4  operator+( const float f ) const;
    dhVector4& operator+=( const float f );
    dhVector4  operator-() const;
	
	
    // Scalings
    //
    dhVector4  operator*( const dhVector4& vec ) const;
    dhVector4& operator*=( const dhVector4& vec );
    dhVector4  operator*( const float f ) const;
    dhVector4& operator*=( const float f );
    dhVector4  operator/( const dhVector4& vec ) const;
    dhVector4& operator/=( const dhVector4& vec );
    dhVector4  operator/( const float f ) const;
    dhVector4& operator/=( const float f );

	//dhVector4	Scaling( REAL,REAL,REAL);
	//dhVector4	Scaling(dhVector4);

	dhVector4& Scaling( const float length );
	

	//
    REAL Distance( const dhVector4& v1);


    // Normalization
    //
    
	dhVector4&	Normalize();
	
    // Length
    //
	REAL		Length() const;
    REAL		Abs();
	REAL 		Norm();

    /**
	 * Dot Product.
	 */
	REAL		Dot( const dhVector4& vec);

    
    // return all zero vector
    static dhVector4 zero() { return dhVector4(0, 0, 0, 0); }
    
    // return all one vector
    static dhVector4 one() { return dhVector4(1, 1, 1, 1); }

};




// Non-Member operators
//
//
dhVector4 operator+( float f, const dhVector4& vec );
dhVector4 operator-( float f, const dhVector4& vec );
dhVector4 operator*( float f, const dhVector4& vec );
dhVector4 operator/( float f, const dhVector4& vec );






