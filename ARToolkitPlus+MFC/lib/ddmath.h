/*
 *  ddmath.h
 *  WorldTest
 *
 *  Created by Seung-Chan Kim on 6/28/11.
 *  Copyright 2011 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */

#ifndef DD_MATH_H
#define DD_MATH_H
#include <math.h>

#ifndef VERYSMALL
#define VERYSMALL  (1.0E-150)
#endif

#ifndef EPSILON
#define EPSILON    (1.0E-8)
#endif

typedef unsigned int uint32;
typedef signed char	int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;

typedef float float32;

/// "Next Largest Power of 2
/// Given a binary integer value x, the next largest power of 2 can be computed by a SWAR algorithm
/// that recursively "folds" the upper bits into the lower bits. This process yields a bit vector with
/// the same most significant 1 as x, but all 1's below it. Adding 1 to that value yields the next
/// largest power of 2. For a 32-bit value:"

 
inline double round(double number)
{
    return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}



inline double square(double x)
{
	return x*x;
}

inline bool ddIsPowerOfTwo(uint32 x)
{
	bool result = x > 0 && (x & (x - 1)) == 0;
	return result;
}


inline uint32 ddNextPowerOfTwo(uint32 x)
{
	if(!ddIsPowerOfTwo(x))
	{
	   x |= (x >> 1);
	   x |= (x >> 2);
	   x |= (x >> 4);
	   x |= (x >> 8);
	   x |= (x >> 16);
		return x + 1;
	}
	else {
		return x;
	}	   
}

//C:\Users\kimsc\Documents\MATLAB\ETNA_128_2_185_149\imagework\stest.m
//float alpha = sigmoid(20.0, sn_tr.z, 0);
inline double sigmoid(float slope, float x, float center)
{


	return 1.0 / (1.0 + exp(slope*(-x + center)));
	
}

inline void ShiftWithNewf(float* arr, float n_val, int sz)
{
	int i;
	for(i=0; i< sz-1; i++)
	{
		arr[sz-1-i] = arr[sz-2-i];
		
	}

	arr[0] = n_val;

}


inline void minmax2(float v1, float v2, float v3, float *min, float *max, int *minID, int *maxID)
{
	if (v1 > v2)
	{
		if (v1 > v3)
			*max = v1;
		else
			*max = v3;

		if (v2 < v3)
			*min = v2;
		else
			*min = v3;
	}
	else
	{
		if (v2 > v3)
			*max = v2;
		else
			*max = v3;

		if (v1 < v3)
			*min = v1;
			else
			*min = v3;
	}


	if(*max == v1)
		*maxID = 0;
	else if (*max == v2)
		*maxID =  1;
	else if (*max == v3)
		*maxID = 2;
	else 
		*maxID = -1;




	if(*min == v1)
		*minID = 0;
	else if (*min == v2)
		*minID =  1;
	else if (*min == v3)
		*minID = 2;
	else 
		*minID = -1;



}


inline int minmax(double v1, double v2, double v3, double *min, double *max)
{
	if (v1 > v2)
	{
		if (v1 > v3)
			*max = v1;
		else
			*max = v3;

		if (v2 < v3)
			*min = v2;
		else
			*min = v3;
	}
	else
	{
		if (v2 > v3)
			*max = v2;
		else
			*max = v3;

		if (v1 < v3)
			*min = v1;
			else
			*min = v3;
	}


	if(*max == v1)
		return 0;
	else if (*max == v2)
		return 1;
	else if (*max == v3)
		return 2;
	else return -1;
	
	
	
	//not all control paths return a value
}





//check for ttmath.h
template <class T>
inline void ddShiftWithNew(T* arr, const T &n_val, int sz)
{

	int i;
	for(i=0; i< sz-1; i++)
	{
		arr[sz-1-i] = arr[sz-2-i];
		
	}
	arr[0] = n_val;

    //if (val < 0) return -val;
    
}

// added 2013/2/16 @CHAUD 1186
// from hduMath.h

template <class T>
inline T ddAbsValue(const T &val)
{
    if (val < 0) return -val;
    return val;
}

template <class T>
bool ddIsEqual(const T &v0, const T &v1, const T &epsilon)
{
    return (ddAbsValue(v0 - v1) <= epsilon);
}


template <class T>
T ddEvalLine(const T &p0, const T &p1, typename T::EltType t)
{
    return p0 + (p1 - p0) * t;
}

template <class T>
T ddLerp(const T &v0, const T &v1, double t)
{
    return v0 * (1.0 - t) + v1 * t;
}

template <class T>
T ddClamp(const T &val, const T &MIN, const T &MAX)
{
    if (val < MIN) return MIN;
    if (val > MAX) return MAX;
    return val;
}

template <class T>
T ddRound(const T &val)
{
    return floor(val + 0.5);
}

#endif

