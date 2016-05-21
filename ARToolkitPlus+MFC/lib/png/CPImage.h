#ifndef __CIMAGE__
#define __CIMAGE__

#include "png/png.h"

class CPImage
{
public:
		CPImage();
		~CPImage();
public:
		BOOL	Load(char*);

		int		w,h;
		int		bits;
		unsigned char *pBuffer;
public:		
		png_byte color_type ; 
		png_byte bits_depth; 
		png_structp png_ptr ; 
		png_infop info_ptr ; 
		int number_of_passes ; 
		png_bytep * row_pointers ; 

		
		BOOL	PNGWrite(char*);

private:
		BOOL	PNGRead(char *);
		

#ifndef WIN32
		BOOL	PNGZipRead(char*);		
#endif
};

#endif