/*
 *  vwMathGeometry.h
 *
 *  Created by Seung-Chan Kim on 1/16/12.
 *  Copyright 2012 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */


#pragma once

#if (_MSC_VER)      
// make microsoft visual studio complain less about double / float conversion.
#pragma warning(disable : 4244)
#endif




inline float gaussian(float x, float m, float sig)
{
	//p = -1* (x(i) - m)^2 / (2* sig^2);
  	float p = -1* square((x - m)) / (2* square(sig));
    float y = exp(p);
	return y;
	
}





//the first-derivative of the Gaussian
inline float dgaussian(float x, float m, float sig)
{
	
	float p = -1* square((x - m)) / (2* square(sig));
    float c = -2 *(x-m) / square(sig);
    float y = c*exp(p);
	return y;
	
}


inline float GetMaxDgaussian(float r_min, float r_max,float m, float sig)
{
	float vMax = -100000;

	for(int i=r_min; i<r_max; i++)
	{
		
		float dg = dgaussian(i, m, sig);
		if(dg>vMax) vMax = dg;

	}
	return vMax;

}



inline float gaussianMixture(float x, int nModel, int *m, float *sig)
{
	float pt,yt, y=0;

	for(int i=0; i<nModel; i++)
	{
		pt = -1* square((x - m[i])) / (2* square(sig[i]));
		yt = exp(pt);
		y = y+yt;
	}
	y =CLAMP(y, 0, 1);
	return y;
}



//the first-derivative of the Gaussian
inline float dgaussianMixture(float x, int nModel, int *m, float *sig)
{
	int i;
	float pt, ct, yt, y =0;
	for(i=0; i<nModel; i++)
	{
		pt = -1* square((x - m[i])) / (2* square(sig[i]));
		ct = -2 *(x-m[i]) / square(sig[i]);
		yt = ct*exp(pt);
		y = y+yt;
	}

	//y =CLAMP(y, 0, 1);
	return y;

}




// http://en.wikipedia.org/wiki/Gaussian_function
inline float gaussian2D(float x,float y, float m_x,float m_y,float sig_x, float sig_y) 
{
	float t1 = square(x - m_x) / (2* square(sig_x));
	float t2 = square(y - m_y) / (2* square(sig_y));

	float p = t1 + t2;
    float z = exp(-p);
	return z;
}





inline float gaussianMixture2D(float x, float y, int nModel, int *m_x, int *m_y, float *sig)
{
	float z=0;

	for(int i=0; i<nModel; i++)
	{
		
		float zt = gaussian2D( x, y, m_x[i],m_y[i], sig[i], sig[i]) ;

		
		z = z+zt;


	}
	z =CLAMP(z, 0, 1);
	return z;
}
