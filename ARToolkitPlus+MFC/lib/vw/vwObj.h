/*
 *  vwObj.h
 *
 *  Created by Seung-Chan Kim on 1/16/12.
 *  Copyright 2012 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */


#ifndef __VWOBJ__
#define __VWOBJ__

#include "vwO.h"
#include "../vToken.h"

#include "../FileName.h"
#include "../xRGB.h"



// ongoing....
// http://en.wikipedia.org/wiki/Wavefront_.obj_file
// 
// TODO
// - Material template library(MTL) for texture coordinate
//   : loading texture image
//   : usemtl


// Now only handling the form of
// Vertex/texture-coordinate/normal
//  f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...

/* // Moved to vwO as of September 26, 2013
typedef struct __FaceVertex
{
	int v[3];

} FaceVertex;


typedef struct __UV
{
	float u, v;

} UV;
*/

typedef struct __NewMtl
{
	char matname[255];
	char map_Kd[255];
	xRGB Kd;		// diffuse color
	xRGB Ka;		// ambient color of the material
	xRGB Ks;		// specular color

} NewMtl;


typedef struct __map_Kd
{

	int nIDofNewMtl;
	int nGeneratedTexID;


} map_Kd;

class vwObj : public vwO 
{

public:
	vwObj();
	~vwObj();

public:
	void LoadObj(char *filename);
	int LoadCompanionTex(char *filename);

	void InitObj();
	
	//void	Close();
	BOOL	bComTexFile;
	//void	DrawObject();
	virtual void	DrawObject(BOOL bWireFrame=FALSE, BOOL bDrawVert=FALSE);


	vArray <dhVector, dhVector> verts;
	vArray <FaceVertex, FaceVertex> faces;
	vArray <FaceVertex, FaceVertex> face_textures;

	vArray	<int, int> face_usemtl;


	vArray <UV, UV> uvs;
	
	int		GenTexture();
	
	int GetTexNo(){return nTex;}
	
private:
	int		nTex;
	int		nUV;

	int tcnt;

public:

	vArray <NewMtl,NewMtl> matArr;
	vArray <map_Kd, map_Kd> texImgArr;
	

	vwTexture *pTex;
	
	int nTexID[16];



		// Texture
	//amTM	m_tm;
	//amTM	*pTextureManager;

};
#endif


/*
	//vString fn_map_Kd = loc+"/"+fn_map_Kd;
	if(texImgArr.GetSize()>0)//fn_map_Kd 
	{
		
		OutputDebugString(DDFORMAT("Detected #%d image files from the mtl file.", texImgArr.GetSize()));
		//m_tx.LoadTexture(fn_map_Kd, TRUE);
		bComTexFile = TRUE;
	}

*/