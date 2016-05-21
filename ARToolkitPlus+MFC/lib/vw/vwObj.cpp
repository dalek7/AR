/*
 *  vwObj.cpp
 *
 *  Created by Seung-Chan Kim on 1/16/12.
 *  Copyright 2012 Disney Research Pittsburgh, PA. All rights reserved.
 *
 */

#include "stdafx.h"
#include "vwObj.h"

vwObj::vwObj()
{
	pTex		= NULL;
	//nTexID		= NULL;

	for(int i=0; i<16; i++)
		nTexID[i] = 0;
}


void vwObj::InitObj()
{
	verts.RemoveAll();
	faces.RemoveAll();
	face_textures.RemoveAll();
	

	uvs.RemoveAll();

	matArr.RemoveAll();
	texImgArr.RemoveAll();	//20130928
	face_usemtl.RemoveAll();	//20130928



	nUV		= 0;
	bComTexFile		= FALSE;

	
	tcnt = 0;
	

	for(int i=0; i<nTex; i++)
	{
			
			pTex[i].Close();
	}


	if(pTex)
		delete []pTex;

	pTex =NULL;


	nTex = 0;

	
	//pTextureManager	= &m_tm;

}


void vwObj::LoadObj(char *fname)
{
	int i,j;
	InitObj();

	LoadCompanionTex(fname);

	int tot_mtl = matArr.GetSize();
	for(i=0; i<tot_mtl; i++)
	{

		;//OutputDebugString(DDFORMAT("=== Loaded newmtl : " + CString(matArr.GetAt(i).matname)));
	}
	
	vToken tok;
	tok.Load(fname);
	tok.SetSeparator("\n\r");
	
	int nNewMtl=0;
	int nRow = tok.GetSize();
	for(i=0; i<nRow; i++)
	{
		vToken tok_l(tok[i]);

		if(tok[i].Left(2) =="v ")
		{
			tok_l.SetSeparator(" ");
			dhVector v1= dhVector(tok_l.f(1), tok_l.f(2), tok_l.f(3));
			verts.Add(v1);

			//kv.Add(v1);

			//Dbg("v %.2f,%.2f, %.2f", v1.x, v1.y, v1.z);

		}
		else if(tok[i].Left(6) =="usemtl")
		{
			vToken tok2(tok[i]);
			
			tok2.SetSeparator(" ");

			CString s2 = CString(tok2[1]);
			for(int k=0; k<matArr.GetSize(); k++)
			{

				CString s1 = CString(matArr.GetAt(k).matname);
				if( s1==s2)
				{
					nNewMtl = k;
					//OutputDebugString("  +++ "+DDFORMAT("%d", nNewMtl)+ ")"+s2);
					//nNewMtl++;
					break;
				}
			}
		}
		else if(tok[i].Left(2) =="f ")
		{		
			
			tok_l.SetSeparator(" ");
			if(tok_l.GetSize()>4)
			{
				AfxMessageBox("Preparing the quad...");
				InitObj();
				return;
			}
			FaceVertex fVid; 
			FaceVertex fVTid;	//texid 

			//http://en.wikipedia.org/wiki/Wavefront_.obj_file
			//Vertex/texture-coordinate/normal
			// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
			// f 314/418/1963 327/433/1964 324/432/1965


			for(j=0; j<3; j++)
			{
				vToken tok_s(tok_l[j+1]);
				tok_s.SetSeparator("/");
				int nSlash	= tok_s.GetSize();
				fVid.v[j]	= tok_s.i(0)-1;
				fVTid.v[j]  = tok_s.i(1)-1;
	
			}
			faces.Add(fVid);
			face_textures.Add(fVTid);
			face_usemtl.Add(nNewMtl);
			
			//Dbg("f %d,%d,%d / usemtl%d", fVid.v[0], fVid.v[1], fVid.v[2], nNewMtl-1);

			//OutputDebugString(DDFORMAT("f %d,%d,%d / usemtl%d", fVid.v[0], fVid.v[1], fVid.v[2], nNewMtl-1));

		}
		else if(tok[i].Left(2) =="vt")
		{

			tok_l.SetSeparator(" ");
			UV uv1;

			uv1.u = tok_l.f(1);
			uv1.v = tok_l.f(2);

			//Dbg("  U%f\tV%f\n", uv1.u, uv1.v);
			uvs.Add(uv1);

		}

	}

	nVer	= verts.GetSize();
	nFace	= faces.GetSize();
	nUV		= uvs.GetSize();

	Dbg("\nLoaded an object file");
	Dbg("\n nVer : %d", nVer);
	Dbg("\n nFace : %d", nFace);
	Dbg("\n nUV: %d \n", nUV);


	/*
	for(i=0; i<uvs.GetSize(); i++)
	{

		Dbg("  U%f\tV%f\n", uvs.GetAt(i).u, uvs.GetAt(i).v);
	}
*/

	Init(nVer, nFace);



	if(pUV) 
		delete pUV;


	pUV = new float[2*nUV];
	/*
	for(i=0; i<nUV; i++)
	{
		pUV[2*i] = uvs.GetAt(i).u;
		pUV[2*i+1] = uvs.GetAt(i).v;
	}
	*/

	for ( i = 0; i < nVer; i++ )
	{


		dhVector p0 = dhVector(verts.GetAt(i).x, verts.GetAt(i).y, verts.GetAt(i).z);
		//Dbg("Vertices : %.3f %.3f %.3f",p0.x, p0.y, p0.z);
		

		pVer[i]		= p0;
		pVer0[i]	= p0;
	
	}



	for ( i = 0; i < nFace; i++ )
	{
		
		//Dbg("Faces : %d %d %d",faces.GetAt(i).v[0], faces.GetAt(i).v[1], faces.GetAt(i).v[2]);

		int vertexIndices[3] = {faces.GetAt(i).v[0], faces.GetAt(i).v[1], faces.GetAt(i).v[2] };
		
		pFace[i].Tri(pVer,vertexIndices[0], vertexIndices[1], vertexIndices[2]);


		// 20120128
		pVer2[vertexIndices[0]].Add(&pFace[i]);
		pVer2[vertexIndices[1]].Add(&pFace[i]);
		pVer2[vertexIndices[2]].Add(&pFace[i]);
		
	}

/*
	pTex = new vwTexture[texImgArr.GetSize()];
	for( i=0; i<texImgArr.GetSize();i++)
	{
		CString fn = texImgArr.GetAt(i).map_Kd;
		fn.TrimLeft();

		//pTex[0].LoadTexture(

	}
	
*/

	

	for( i=0; i<nUV; i++)
	{
		
		//Dbg("UV : %.3f %.3f",uvs.GetAt(i).u, uvs.GetAt(i).v);

		
		pUV[2*i+0] = uvs.GetAt(i).u;
		pUV[2*i+1] = uvs.GetAt(i).v;
		
	}


	
	GetCenter();		//20131007
	FindGourad();
	
}


int	vwObj::GenTexture()
{
	//matArr.Add(m1);
	//vString fn_map_Kd = loc+"/"+fn_map_Kd;
	int ret		= 0;
	nTex		= texImgArr.GetSize();
	int szMat	= matArr.GetSize();

	if(nTex>0)//fn_map_Kd 
	{
		int i;
		
		//m_tx.LoadTexture(fn_map_Kd, TRUE);
		
		if(!pTex[0].pBuf) return 0;
		for( i=0; i<nTex; i++)
		{
			
			int n				= texImgArr.GetAt(i).nIDofNewMtl;
			CString fn_map_Kd	= matArr.GetAt(n).map_Kd;				//vString to CString
			char *ps			= fn_map_Kd.GetBuffer(fn_map_Kd.GetLength());//fn_map_Kd.Buf();
			
			nTexID[i]			= pTex[i].GenTexture2();


			//texImgArr.Add(mkd);
			//texImgArr.InsertAt(i+1, mkd);
			//texImgArr.RemoveAt(i+1);
			
			//texImgArr.GetAt(i).nGeneratedTexID =  1;//
		
		}
		for( i=0; i<nTex; i++)
		{

			//int aa = pTextureManager->Search(i)->tex.nTex;
			
		
			map_Kd mkd		= texImgArr.GetAt(i);
			int mm			= mkd.nIDofNewMtl;
			int nn			= nTexID[i];

			
		
			int tt = 0;
			
		}
		
		ret = 1;
	}



	return nTex;

}


// earlier version
// map_Kd

int vwObj::LoadCompanionTex(char *filename)
{

	bComTexFile = FALSE;
	
	OutputDebugString("Loading the companion tex...");


	CFileName fn(filename);
	CString loc = fn.GetPath();

	CString str(filename);
	CString fn_mtl = str.Left(str.GetLength()-3);
	fn_mtl += "mtl";

	vToken tok;
	char *pfn = fn_mtl.GetBuffer();
	tok.Load(pfn);
	tok.SetSeparator("\n\r");

	int tsz = tok.GetSize();
	if(tok.GetSize()<1)
	{
		Dbg("ERROR in the mtl file");
		return -1;
	}

	int i;
	

	NewMtl m1;
	int newmtlCnt	= 0;

	//memset(m1.matname, 0, 255);

	memset(&m1, 0, sizeof(NewMtl));
	for(i=0; i<tok.GetSize(); i++)
	{
		//tok[i].Buf();
		vToken tok2(tok[i].Buf());
		tok2.SetSeparator(" ");

		

		if(tok2[0] =="newmtl")
		{
			
			if(m1.matname[0])
			{
				matArr.Add(m1);
				//OutputDebugString("Loaded : "+CString(m1.matname));
			
				
				//memset(m1.matname, 0, 255);
				memset(&m1, 0, sizeof(NewMtl));
				
			}
			
			//m1.matname  = tok2[1].;
			
			memmove(m1.matname, tok2[1].Buf(), tok2[1].GetLength());
			
			//OutputDebugString(m1.matname);

			newmtlCnt++;
			//continue;

		}

		else if(tok2[0] =="map_Kd")
		{
		
			vString fn_map_Kd = loc+"/"+tok2[1]; 
			//memmove(m1.map_Kd, tok2[1].Buf(), tok2[1].GetLength());

			memmove(m1.map_Kd, fn_map_Kd.Buf(), fn_map_Kd.GetLength());


			map_Kd mkd;
			mkd.nIDofNewMtl	= newmtlCnt -1;
			mkd.nGeneratedTexID	= 0; // will be assigned after gentex
			texImgArr.Add(mkd);
			
		}


		else if(tok2[0] =="Kd")
		{
			m1.Kd = xRGB(tok2.f(1), tok2.f(2), tok2.f(3));

		}
		else if(tok2[0] =="Ka")
		{
			m1.Ka = xRGB(tok2.f(1), tok2.f(2), tok2.f(3));

		}
		else if(tok2[0] =="Ks")
		{
			m1.Ks = xRGB(tok2.f(1), tok2.f(2), tok2.f(3));

		}
	}	


	if(m1.matname[0])
	{
		matArr.Add(m1);

	}
	nTex	= texImgArr.GetSize();
	int szMat	= matArr.GetSize();

	if(nTex>0)//fn_map_Kd 
	{
		int i;
		
	

		
		//m_tx.LoadTexture(fn_map_Kd, TRUE);

		//if(texImgArr.GetAt(0).nGeneratedTexID>0) return 0;

		//if(pTex) return -1;

		OutputDebugString(DDFORMAT("Detected #%d image files from the mtl file.", nTex));
	

		pTex	= new vwTexture[nTex];
		//nTexID	= new int[nTex];

		for( i=0; i<nTex; i++)
		{
			int n = texImgArr.GetAt(i).nIDofNewMtl; //

			CString fn_map_Kd = matArr.GetAt(n).map_Kd;

			

			OutputDebugString(DDFORMAT("#%d / %d", n, matArr.GetSize()-1));
			OutputDebugString(fn_map_Kd); 

			
//			pTextureManager->TXLoad(fn_map_Kd.Buf());
			
			
			//pTex[i].CloseBuffer();
			pTex[i].LoadTexture(fn_map_Kd.GetBuffer(fn_map_Kd.GetLength()), TRUE);


		}
	}

////////////////////

	//vString fn_map_Kd = loc+"/"+fn_map_Kd;
	if(texImgArr.GetSize()>0)//fn_map_Kd 
	{
		// 5 files for tinkerbell
		OutputDebugString(DDFORMAT("Detected #%d image files from the mtl file.", texImgArr.GetSize()));
		//m_tx.LoadTexture(fn_map_Kd, TRUE);
		bComTexFile = TRUE;
	}
	else
	{
		bComTexFile = FALSE;

		OutputDebugString("No companion image texfile");
	}

	
	
	
	return 1;
	
}

/*
  # Face Definitions 
  f 1 2 3
  f 3/1 4/2 5/3
  f 6/4/1 3/5/3 7/6/5

*/



void vwObj::DrawObject(BOOL bWireFrame, BOOL bDrawVert)
{
	tcnt++;
	glEnable(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_2D,0);
	//m_tx.BindTexture();
	//////glBindTexture(GL_TEXTURE_2D,m_tx.nTexID);
	//Dbg("%d", m_tx.nTexID);
	
	int prevMatID = -1;
	nTex = texImgArr.GetSize();


	int i;

	

	for ( i=0;i<nTex;i++)
	{
		;//OutputDebugString(DDFORMAT("vwObj::DrawObject() %d) MatID:%d: glBindTexture : #%d ()", 
	//							tcnt, texImgArr.GetAt(i).nIDofNewMtl, texImgArr.GetAt(i).nGeneratedTexID));
		
	}

	BOOL bBind = FALSE;
	for ( i=0;i<nFace;i++)
	{
		vwPolygon *p	= &pFace[i];

		int a,b,c;
		a	= p->v[0]-pVer;
		b	= p->v[1]-pVer;
		c	= p->v[2]-pVer;


		int ta, tb, tc;
		ta = face_textures.GetAt(i).v[0];
		tb = face_textures.GetAt(i).v[1];
		tc = face_textures.GetAt(i).v[2];
		
		//int n = texImgArr.GetAt(i).nGeneratedTexID
		//glBegin(GL_TRIANGLES);

		if (pNormal)
		{
			if(matArr.GetSize()>0)//texImgArr
			{
				int mtl_id = face_usemtl.GetAt(i);
				
				if(mtl_id != prevMatID)
				{


					if(nTexID[mtl_id])//if(nTexID
					{
						int tID =nTexID[mtl_id];//mtl_id+1;//texImgArr.GetAt(mtl_id).nGeneratedTexID;
						//
						if(bBind)
							glEnd();

						glBindTexture(GL_TEXTURE_2D,tID);//mtl_id+1
						
						
						//glBegin(GL_TRIANGLES);
						
						//OutputDebugString(DDFORMAT("GL_TRIANGLES: %d(genTex:%d), %dth face",mtl_id,tID, i));
					}
					else
					{

						if(bBind)
							glEnd();

						glBindTexture(GL_TEXTURE_2D,0);
					}

					bBind = TRUE;
					glBegin(GL_TRIANGLES);
					

					xRGB clr = matArr.GetAt(mtl_id).Kd;
				
					if(!bComTexFile)						// to be updated
					glColor4f(clr.r, clr.g, clr.b, 1);
				}
				prevMatID = mtl_id;
				

				
				glNormal3f(pNormal[a].x,pNormal[a].y,pNormal[a].z);
				glTexCoord2f( pUV[2*ta],pUV[2*ta+1]);


				//face_usemtl
				glVertex3d(pVer[a].x,pVer[a].y,pVer[a].z);
				//glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);

				glNormal3f(pNormal[b].x,pNormal[b].y,pNormal[b].z);
				glTexCoord2f( pUV[2*tb],pUV[2*tb+1]);
				glVertex3d(pVer[b].x,pVer[b].y,pVer[b].z);
				//glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
				
				glNormal3f(pNormal[c].x,pNormal[c].y,pNormal[c].z);
				glTexCoord2f( pUV[2*tc],pUV[2*tc+1]);
				glVertex3d(pVer[c].x,pVer[c].y,pVer[c].z);
				
			}
			
			else
			{

				glBegin(GL_TRIANGLES);
				glNormal3f(pNormal[a].x,pNormal[a].y,pNormal[a].z);

				glTexCoord2f( pUV[2*ta],pUV[2*ta+1]);
				glVertex3d(pVer[a].x,pVer[a].y,pVer[a].z);
				//glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);

				glNormal3f(pNormal[b].x,pNormal[b].y,pNormal[b].z);
				glTexCoord2f( pUV[2*tb],pUV[2*tb+1]);
				glVertex3d(pVer[b].x,pVer[b].y,pVer[b].z);
				//glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
				
				glNormal3f(pNormal[c].x,pNormal[c].y,pNormal[c].z);
				glTexCoord2f( pUV[2*tc],pUV[2*tc+1]);
				glVertex3d(pVer[c].x,pVer[c].y,pVer[c].z);
			}
		}
		else
		{
			glBegin(GL_TRIANGLES);
			
			glVertex3d(p->v[0]->x,p->v[0]->y,p->v[0]->z);
			glVertex3d(p->v[1]->x,p->v[1]->y,p->v[1]->z);
			glVertex3d(p->v[2]->x,p->v[2]->y,p->v[2]->z);	
		}

		
	}


	glEnd();
	//OutputDebugString(" ");
	glDisable(GL_TEXTURE_2D);


}

vwObj::~vwObj()
{
	//vwO::Close();

	if(pTex)
		delete []pTex;

	pTex =NULL;

	//if(nTexID) delete nTexID;
	//nTexID = NULL;

	matArr.RemoveAll();

	OutputDebugString("vwObj::~vwObj()");
}
