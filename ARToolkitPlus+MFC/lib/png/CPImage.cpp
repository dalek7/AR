#include "stdafx.h"
#include "CPImage.h"
//#include "../game/wGame.h"

#define PNG_DEBUG 3 

#ifndef WIN32
zip_file	*pZFP=NULL;
void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length) 
{
	zip_fread(pZFP, data, length);
}

BOOL CPImage::PNGZipRead( char * file_name ) 
{ 	
	int x,y;
	unsigned char header [ 8 ]; // 8 is the maximum size that can be checked 

	pZFP= zip_fopen(pGame->pAPK, file_name, 0);
	if ( !pZFP ) 
		return FALSE;	

	zip_fread(pZFP, header, 8);

	if ( png_sig_cmp ( header, 0 , 8 )) 
		return FALSE;
	
	png_ptr = png_create_read_struct ( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL ); 
	
	if ( !png_ptr ) 
		return FALSE;

	info_ptr = png_create_info_struct ( png_ptr ); 
	if ( !info_ptr ) 
	return FALSE;

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) 
    return FALSE;

	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	return FALSE;
	
	//png_init_io ( png_ptr, fp ); 
	png_set_read_fn(png_ptr, NULL, png_zip_read);

	png_set_sig_bytes ( png_ptr, 8 ); 

	png_read_info ( png_ptr, info_ptr ); 

	w = info_ptr->width ; 
	h = info_ptr->height ; 

	color_type = info_ptr->color_type ; 
	bits_depth = info_ptr->bit_depth ; 
	bits	= info_ptr->pixel_depth/8;
	number_of_passes = png_set_interlace_handling ( png_ptr ); 

	png_read_update_info ( png_ptr, info_ptr ); 
	
	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	return FALSE;
	
	// Buffering.
	if (pBuffer)	free(pBuffer);
	pBuffer	= (unsigned char*)malloc(w*h*bits);
	
	row_pointers = ( png_bytep* ) malloc ( sizeof ( png_bytep ) * h ); 
	for ( y= 0 ; y<h; y++ ) 
	row_pointers [ y ] = &pBuffer[bits*w*y];

	png_read_image ( png_ptr, row_pointers ); 
	free(row_pointers);

	png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);//(png_infopp)0);

	zip_fclose ( pZFP ); 	
	return TRUE;
}
#endif

BOOL CPImage::PNGRead( char * file_name ) 
{ 
	int x,y;
	unsigned char header [ 8 ]; // 8 is the maximum size that can be checked 

	/* open file and test for it being a png */ 
	FILE * fp = fopen ( file_name, "rb" ); 
	if ( !fp ) 
		//abort_ ( "[read_png_file] File %s could not be opened for reading" , file_name ); 
		return FALSE;

	fread ( header, 1 , 8 , fp ); 
	if ( png_sig_cmp ( header, 0 , 8 )) 
		//abort_ ( "[read_png_file] File %s is not recognized as a PNG file" , file_name ); 
		return FALSE;
	
	/* initialize stuff */ 
	png_ptr = png_create_read_struct ( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL ); 
	
	if ( !png_ptr ) 
		//abort_ ( "[read_png_file] png_create_read_struct failed" ); 
		return FALSE;

	info_ptr = png_create_info_struct ( png_ptr ); 
	if ( !info_ptr ) 
	//abort_ ( "[read_png_file] png_create_info_struct failed" ); 
	return FALSE;

	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	//abort_ ( "[read_png_file] Error during init_io" ); 
	return FALSE;

	png_init_io ( png_ptr, fp ); 
	png_set_sig_bytes ( png_ptr, 8 ); 

	png_read_info ( png_ptr, info_ptr ); 

	w = info_ptr->width ; 
	h = info_ptr->height ; 
	color_type = info_ptr->color_type ; 
	bits_depth = info_ptr->bit_depth ; 
	bits	= info_ptr->pixel_depth/8;
	number_of_passes = png_set_interlace_handling ( png_ptr ); 
	png_read_update_info ( png_ptr, info_ptr ); 
	
	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	//abort_ ( "[read_png_file] Error during read_image" ); 
	return FALSE;
	
	// Buffering.
	if (pBuffer)	free(pBuffer);
	pBuffer	= (unsigned char*)malloc(w*h*bits);
	
	row_pointers = ( png_bytep* ) malloc ( sizeof ( png_bytep ) * h ); 
	for ( y= 0 ; y<h; y++ ) 
	row_pointers [ y ] = &pBuffer[bits*w*y];

	png_read_image ( png_ptr, row_pointers ); 
	free(row_pointers);

	png_destroy_read_struct(&png_ptr,&info_ptr,(png_infopp)0);
	fclose ( fp ); 	
	return TRUE;
} 

BOOL CPImage::PNGWrite( char * file_name ) 
{ 
	int x,y;

	/* create file */ 
	FILE * fp = fopen ( file_name, "wb" ); 
	if ( !fp ) 
	//abort_ ( "[write_png_file] File %s could not be opened for writing" , file_name ); 
	return FALSE;


	/* initialize stuff */ 
	png_ptr = png_create_write_struct ( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL ); 

	if ( !png_ptr ) 
	//abort_ ( "[write_png_file] png_create_write_struct failed" ); 
	return FALSE;

	info_ptr = png_create_info_struct ( png_ptr ); 
	if ( !info_ptr ) 
	//abort_ ( "[write_png_file] png_create_info_struct failed" ); 
	return FALSE;

	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	//abort_ ( "[write_png_file] Error during init_io" ); 
	return FALSE;

	png_init_io ( png_ptr, fp ); 


	/* write header */ 
	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	//abort_ ( "[write_png_file] Error during writing header" ); 
	return FALSE;

	png_set_IHDR ( png_ptr, info_ptr, w, h, 
	   bits, color_type, PNG_INTERLACE_NONE, 
	   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE ); 

	png_write_info ( png_ptr, info_ptr ); 


	/* write bytes */ 
	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	//abort_ ( "[write_png_file] Error during writing bytes" ); 
	return FALSE;

	png_write_image ( png_ptr, row_pointers ); 


	/* end write */ 
	if ( setjmp ( png_jmpbuf ( png_ptr ))) 
	//abort_ ( "[write_png_file] Error during end of write" ); 
	return FALSE;

	png_write_end ( png_ptr, NULL ); 

	/* cleanup heap allocation */ 
	for ( y= 0 ; y<h; y++ ) 
	free ( row_pointers [ y ]); 
	free ( row_pointers ); 

	fclose ( fp ); 

	return 0;
} 

/*
	// BUffering..

 void process_file ( void ) 
 { 
	if ( info_ptr->color_type != PNG_COLOR_TYPE_RGBA ) 
	  abort_ ( "[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (is %d)" , 
					 info_ptr->color_type ); 

	for ( y= 0 ; y<height ; y++ ) { 
	  png_byte* row = row_pointers [ y ]; 
	  for ( x= 0 ; x<width ; x++ ) { 
		  png_byte* ptr = & ( row [ x* 4 ]); 
		  printf ( "Pixel at position [ %d - %d ] has the following RGBA values: %d - %d - %d - %d\n" , 
				 x, y, ptr [ 0 ] , ptr [ 1 ] , ptr [ 2 ] , ptr [ 3 ]); 

		  ptr [ 0 ] = 0 ; 
		  ptr [ 1 ] = ptr [ 2 ]; 
	  } 
	} 

} 
*/

CPImage::CPImage()
{
	w	= h	= 0;
	bits=4;
	pBuffer	= NULL;
}

CPImage::~CPImage()
{
	if (pBuffer)	free(pBuffer);
}

//#include "../game/wGame.h"
BOOL CPImage::Load(char *psz)
{
#ifdef WIN32
	return PNGRead(psz);
#else
	if (pGame->pAPK)
	{
		return PNGZipRead(psz);
	}
	else
	{
		return PNGRead(psz);
	}	
#endif
}

