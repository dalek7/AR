#include "StdAfx.h"
#include "vwCamera.h"

vwCamera::vwCamera(void)
{
	vw.f		= 1000;
	vw.n		= 0.001;
	vw.angle	= 30;
//	vw.o		= dhVector(0,-5,-60);//dhVector(-3,-3,-30);
	vw.o		= dhVector(0,0,-30);//dhVector(-3,-3,-30);
	vw.r		= dhVector(0,0,0);
	//vw.h
	ptMouseOld	= CPoint(0,0);


	vw_prev		= vw;
	bViewChanged	= FALSE;
	//vw.eye ??
	SaveView();
	Update();

	_rotMatrix.I();

	_quat		= dhQuat(0, 0, 0, 1);
    _quatStart	= dhQuat(0, 0, 0, 1);



	fov = 60;
	nearClip = 0;
	farClip = 0;
	lensOffset = dhVector(0.0f, 0.0f);

}

vwCamera::~vwCamera(void)
{
}

void vwCamera::SaveView()
{
	memmove(&vw_ori, &vw, sizeof(VIEW));

}

void vwCamera::RestoreView()
{
	memmove(&vw, &vw_ori, sizeof(VIEW));
}
void vwCamera::Update()
{

	vw.vDir = vw.o - vw.eye;
	vw.vDir	= vw.vDir.Unit();

}

BOOL vwCamera::IsViewChanged()
{

	if(	vw.o.x != vw_prev.o.x || vw.o.y != vw_prev.o.y || vw.o.z != vw_prev.o.z || 
		vw.r.x != vw_prev.r.x || vw.r.y != vw_prev.r.y || vw.r.z != vw_prev.r.z)
	{
		bViewChanged = TRUE;
	}
	else
		bViewChanged = FALSE;
	

	vw_prev = vw;

	return bViewChanged;
}



void vwCamera::ApplyProjectionTransform()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(vw.angle,vw.fAR,vw.n,vw.f );
	//glScalef(1, -1, 1);
}

void vwCamera::ApplyModelViewTransform()
{
    
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    glTranslatef(vw.o.x, vw.o.y, vw.o.z);
	glRotatef(vw.r.x,1,0,0);
	glRotatef(vw.r.y,0,1,0);


	

	CopyMatrices();
}


void vwCamera::ApplyModelViewTransform2(BOOL bForceIdentity)
{
	glMatrixMode( GL_MODELVIEW );
	if(bForceIdentity)
		glLoadIdentity();

    glTranslatef(vw.o.x, vw.o.y, vw.o.z);
	glRotatef(vw.r.x,1,0,0);
	glRotatef(vw.r.y,0,1,0);

	//additional 
	//////glMultMatrixf(_rotMatrix.v);
	//_rotMatrix.I();

	CopyMatrices();
}


void vwCamera::SetCamTrans(float x, float y, float z)
{

	vw.o.x += x;
	vw.o.y += y;
	vw.o.z += z;


}

BOOL vwCamera::MouseMove(int nFlags,CPoint pt)
{

	CPoint dpt	= pt-ptMouseOld;
	ptMouseOld	= pt;

	int dx,dy;
	dx	= fabs((float)dpt.x);
	dy	= fabs((float)dpt.y);

	switch(nFlags)
	{
		case MK_LBUTTON://|MK_RBUTTON
		if (dx>dy)
		{
			if (dpt.x>2)		{	vw.r.y+=5;	return TRUE; }
			else if (dpt.x<-2)	{	vw.r.y-=5;	return TRUE; }
		}
		else
		{
			if (dpt.y>2)		{	vw.r.x+=5;	return TRUE; }
			else if (dpt.y<-2)	{	vw.r.x-=5;	return TRUE; }
		}


		break;
	

		case MK_RBUTTON:
		{
			vw.o.z -= (float)0.1f * dpt.y;
			
			break;
		}

		

		case MK_MBUTTON:
		{

			vw.o.x += (float)0.05f * dpt.x;
			vw.o.y -= (float)0.05f * dpt.y;
			
			break;
		}

		

	}
	/*
	CString buf;
	buf.Format("%d, %d", pt.x, pt.y);
	pv->dbg(buf,1);
	buf.Format("%d, %d", dpt.x, dpt.y);
	pv->dbg(buf);
*/

	return FALSE;



}
void vwCamera::TouchesBegan(dhVector ptscreen)
{
	_anchor_position	= projectOntoSurface(ptscreen); //[self projectOntoSurface:_anchor_position];

	_current_position	= _anchor_position;
	_quatStart			= _quat;
}

void vwCamera::TouchesEnded(dhVector ptscreen)
{
	
}

dhVector vwCamera::projectOntoSurface(dhVector touchPoint)
{
	int w = vp[2];
	int h = vp[3];

	float radius	= w/3.0; 
	dhVector center = dhVector(w/2,h/2, 0);
    dhVector P		= touchPoint-center;
	P.y *=-1.0;

	float radius2 = radius * radius;
    float length2 = P.x*P.x + P.y*P.y;

	if (length2 <= radius2)
        P.z = sqrt(radius2 - length2);
    else
    {
        /*
        P.x *= radius / sqrt(length2);
        P.y *= radius / sqrt(length2);
        P.z = 0;
        */
        P.z = radius2 / (2.0 * sqrt(length2));
        float length = sqrt(length2 + P.z * P.z);
        //P = GLKVector3DivideScalar(P, length);
		P= P.Scaling(1/length);

		//P = P.Unit();
    }

	return P.Unit();

}

void vwCamera::computeIncremental()
{

	dhVector axis	= _anchor_position * _current_position;
	float dot		= _anchor_position.Dot(_current_position);   
	float angle		= acosf(dot);
	dhQuat Q_rot	= dhQuat(angle * 2, axis);
	Q_rot.normalize();


	_quat.makeRotate(_anchor_position, _current_position);	// FIX this !!! almost done

	//_quat = Q_rot * _quatStart;//GLKQuaternionMultiply(Q_rot, _quatStart);

	dhQuat qt;
	dhMat rotation = qt.Matrix4MakeWithQuaternion(_quat);
	_rotMatrix = rotation;


	// = 

	/*
	 GLKVector3 axis = GLKVector3CrossProduct(_anchor_position, _current_position);
    float dot = GLKVector3DotProduct(_anchor_position, _current_position);    
    float angle = acosf(dot);
    
    GLKQuaternion Q_rot = GLKQuaternionMakeWithAngleAndVector3Axis(angle * 2, axis);
    Q_rot = GLKQuaternionNormalize(Q_rot);

    // TODO: Do something with Q_rot...
    _quat = GLKQuaternionMultiply(Q_rot, _quatStart);
*/



}
void vwCamera::TouchesMoved(dhVector ptscreen)
{

	diff	= ptscreen-dhVector(ptMouseOld.x, ptMouseOld.y,0);

	if(1)
	{
		float rotX = -1 * RAD(diff.y / 2.0);
		float rotY = -1 * RAD(diff.x / 2.0);

		_rotMatrix = mat_mm.R();

		dhMat _rotMatrixInv = _rotMatrix.Inv();
		dhVector xAxis		= _rotMatrixInv * dhVector(1, 0, 0);
		_rotMatrix			= Matrix4Rotate(_rotMatrix, rotX, xAxis.x, xAxis.y, xAxis.z);


		_rotMatrixInv		= _rotMatrix.Inv();
		dhVector yAxis		= _rotMatrixInv * dhVector(0, 1, 0);

		_rotMatrix			= Matrix4Rotate(_rotMatrix, rotY, yAxis.x, yAxis.y, yAxis.z);

		

	}
	else
	{

		_current_position	= projectOntoSurface(ptscreen);
	
		computeIncremental();

	}
	//dhVector yAxis	= _rotMatrixInv * dhVector(0, 1, 0);


	//dhVector xAxis = _rotMatrix.I();
	
	//* dhVector(1, 0, 0);




	//dhVector xAxis = //GLKMatrix4MultiplyVector3(GLKMatrix4Invert(_rotMatrix, &isInvertible), GLKVector3Make(1, 0, 0));





}



// ARC BALL 
BOOL vwCamera::MouseMove2(int nFlags,CPoint pt)
{
	CPoint dpt	= pt-ptMouseOld;
	

	int dx,dy;
	dx	= fabs((float)dpt.x);
	dy	= fabs((float)dpt.y);

	switch(nFlags)
	{
		case MK_LBUTTON://|MK_RBUTTON

		

			TouchesMoved(dhVector(pt.x, pt.y,0));
			/*
		if (dx>dy)
		{
			if (dpt.x>2)		{	vw.r.y+=5;	return TRUE; }
			else if (dpt.x<-2)	{	vw.r.y-=5;	return TRUE; }
		}
		else
		{
			if (dpt.y>2)		{	vw.r.x+=5;	return TRUE; }
			else if (dpt.y<-2)	{	vw.r.x-=5;	return TRUE; }
		}
*/


		break;


		case MK_RBUTTON:
		{
			vw.o.z -= (float)0.1f * dpt.y;
			
			break;
		}

	

		case MK_MBUTTON:
		{

			vw.o.x += (float)0.05f * dpt.x;
			vw.o.y -= (float)0.05f * dpt.y;
			
			break;
		}
		
		

	}
	/*
	CString buf;
	buf.Format("%d, %d", pt.x, pt.y);
	pv->dbg(buf,1);
	buf.Format("%d, %d", dpt.x, dpt.y);
	pv->dbg(buf);
*/

	ptMouseOld	= pt;
	return FALSE;
}



GLdouble* vwCamera::GetMM()
{
	return mm;
}
GLdouble* vwCamera::GetPM()
{
	return pm;
}

int* vwCamera::GetVP()
{
	return vp;
}

void vwCamera::CopyMatrices()
{
	glGetDoublev(GL_MODELVIEW_MATRIX,	mm);
	glGetDoublev(GL_PROJECTION_MATRIX,	pm);
	glGetIntegerv(GL_VIEWPORT,			vp);


	for(int i=0; i<16; i++)
	{
		mat_mm.v[i]		= mm[i];
		mat_pm.v[i]		= pm[i];
	}

}



void vwCamera::CopyMatricesd(double *mm_,double *pm_, int* vp_ )
{

	glGetDoublev(GL_MODELVIEW_MATRIX,	mm_);
	glGetDoublev(GL_PROJECTION_MATRIX,	pm_);
	glGetIntegerv(GL_VIEWPORT,			vp_);


}



dhVector vwCamera::Projection(dhVector v)
{
	dhVector ret;
	
	double x, y, z;

	gluProject(v.x, v.y, v.z, mm, pm, vp, &x, &y, &z);

	ret.x = x;
	ret.y = vp[3] - y;
	ret.z = 0;

	return ret;

}


dhVector vwCamera::Projection_(double *mm1, double *pm1, int *vp1, dhVector p1)
{

	//get projected
	dhVector p1p;
	double x, y, z;

	gluProject(p1.x, p1.y, p1.z, mm1, pm1, vp1, &x, &y, &z);

	p1p.x = x;
	p1p.y = vp1[3] - y;
	p1p.z = 0;


	return p1p;

}

dhVector vwCamera::UnProjection2(dhVector pt_screen)
{

	double x, y, z;
	
	//pt_screen.z == 0??

	GLdouble winX, winY, winZ;

	winX = (float) pt_screen.x;
    winY = (float) vw.h- (float) pt_screen.y;


	gluUnProject(winX, winY, 0,  mm, pm, vp, &x, &y,&z );


	dhVector ret = dhVector(x, y, z);
	
	return ret;

}

void vwCamera::UnProjection3(dhVector pt_screen, double* out_glspace)
{
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 

	winX = (double)pt_screen.x;
    winY = (double)vw.h - (double)pt_screen.y;
    winZ = 0;
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	out_glspace[0] = posX;//*400;
	out_glspace[1] = posY;
	out_glspace[2] = posZ;


	//OutputDebugString("UnProjection");
	//gluUnProject((GLdouble)pt_screen.x,(GLdouble)pt_screen.y, 0, mm, pm, vp, &out_glspace[0],&out_glspace[1],&out_glspace[2] );


}
void vwCamera::UnProjection(dhVector pt_screen, dhVector* output)
{


	double out[3];

	gluUnProject((double)pt_screen.x, (double)vw.h-(double)pt_screen.y, (double)0.0, mm, pm, vp, &out[0],&out[1],&out[2] );
	

	output->x = out[0];
	output->y = out[1];
	output->z = out[2];

	/*
	double out[3];

	GLdouble winX, winY, winZ;

	winX = (float) pt_screen.x;
    winY = (float) vw.h- (float) pt_screen.y;
    glReadPixels( winX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	gluUnProject(winX, winY, winZ, mm, pm, vp, &out[0],&out[1],&out[2] );
	

	output->x = out[0];
	output->y = out[1];
	output->z = out[2];

	*/

}



dhVector vwCamera::UnProjection(CPoint pt_screen)
{
	double x, y, z;
	gluUnProject(pt_screen.x, vw.h-pt_screen.y, 0, mm, pm, vp, &x, &y,&z );

	dhVector ret = dhVector(x, y, z);
	return ret;

}


void vwCamera::UnProjection(dhVector pt_screen, double* out_glspace)
{

	//OutputDebugString("UnProjection");
	gluUnProject(pt_screen.x, vw.h-pt_screen.y, 0, mm, pm, vp, &out_glspace[0],&out_glspace[1],&out_glspace[2] );


}

dhVector vwCamera::GetGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
    return dhVector(posX, posY, posZ);
}

dhVector vwCamera::GetGLPos(int x, int y, float z_ref)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;

    //glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	winZ = z_ref;
	
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
    return dhVector(posX, posY, posZ);
}

dhVector vwCamera::GetGLPosWithZ(int x, int y, float &rz)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
	rz = winZ;
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
    return dhVector(posX, posY, posZ);
}

/*

if(0)
	{
		dhVector v1 = m_camera.GetGLPos(300,400);
		glColor4f(1,0,1,1);
		glPushMatrix();
		
		glTranslatef(v1.x, v1.y, v1.z+0.1);
		DrawCRect(0.5);
		glPopMatrix();
		glColor4f(1,1,1,1);
	}
*/



void vwCamera::Pan(double h, double v)
{
	glTranslatef(h, v, 0);
	//vw.o = dhVector(h, v, 0);
}

void vwCamera::Zoom(double depth)
{
	glTranslatef(0,0,depth);
}

void vwCamera::SaveDepthMap(float *p)
{
	int i,j;
	float *pBuf = new float[vw.w*vw.h];

	int c = 0;
	for(i=0; i<vw.h; i++)
	{
		for(j=0; j<vw.w; j++)
		{
			c = i*vw.w + j;
			
			float winZ;
			float winX = (float)j;
			float winY = (float)vw.h - (float)i -1;
			glReadPixels( winX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

						

			pBuf[c] = winZ;
		}
		float percent = (float)i / vw.h;
		CString buf; 
		buf.Format("%.1f", percent);
		OutputDebugString(buf);

	}

	//pBuf[0] = 1;
	FILE *fp;
	fp=fopen("depth.txt","w+");
	for(i=0; i<vw.h; i++)
	{
		for(j=0; j<vw.w; j++)
		{
			c = i*vw.w + j;
			if( pBuf[c]<1 &&  pBuf[c]>0)
				fprintf(fp,"%.5f ", pBuf[c]);
			else
				fprintf(fp,"%.f ", pBuf[c]);
			
		}

		fprintf(fp,"\n");
	}

	fclose(fp);

	memmove(p, pBuf, sizeof(float)*vw.w*vw.h);

	delete pBuf; 
	pBuf = NULL;

}



void vwCamera::SetCamera(int cx, int cy)
{

	vw.w	= cx;
	vw.h	= cy;
	vw.fAR= ((float)cx)/((float)cy);	
	
	if(1)
		ApplyProjectionTransform();
	else
		setupPerspective();	// TODO 

	glMatrixMode(GL_MODELVIEW);
	glDrawBuffer(GL_BACK);
	glViewport( 0,0,cx,cy );
	glEnable(GL_DEPTH_TEST);
	glFrontFace( GL_CW );


}

void vwCamera::lookAt(const dhVector& lookAtPosition, dhVector upVector) 
{
	//f(parent) upVector = upVector * ofMatrix4x4::getInverseOf(parent->getGlobalTransformMatrix());	
	dhVector zaxis = (vw.o - lookAtPosition).Unit();	
	if (zaxis.Abs() > 0) 
	{
		dhVector xaxis = (upVector*zaxis).Unit();	//upVector.getCrossed(zaxis).normalized();	
		dhVector yaxis = zaxis*xaxis;//zaxis.getCrossed(xaxis);
		
		dhMat m;
		m.v[0] = xaxis.x;
		m.v[1] = xaxis.y;
		m.v[2] = xaxis.z;

		m.v[4] = yaxis.x;
		m.v[5] = yaxis.y;
		m.v[6] = yaxis.z;

		m.v[8] = zaxis.x;
		m.v[9] = zaxis.y;
		m.v[10] = zaxis.z;

		//m._mat[0].set(xaxis.x, xaxis.y, xaxis.z, 0);
		//m._mat[1].set(yaxis.x, yaxis.y, yaxis.z, 0);
		//m._mat[2].set(zaxis.x, zaxis.y, zaxis.z, 0);
		
		dhQuat q1 = m.getRotate();

		setGlobalOrientation(q1);
	}
}

void vwCamera::setGlobalOrientation(const dhQuat& q)
{
	dhMat localTransformMatrix;

	localTransformMatrix.setRotate(q);
	localTransformMatrix.setTranslation(vw.o);


	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMultMatrixf(localTransformMatrix.v);

	//postMultRotate



}
//----------------------------------------
void vwCamera::setupPerspective(bool vFlip, float fov, float nearDist, float farDist, const dhVector & lensOffset){
	float viewW = vw.w;//ofGetViewportWidth();
	float viewH = vw.h;//ofGetViewportHeight();

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	setFov(fov);
	setNearClip(nearDist);
	setFarClip(farDist);
	setLensOffset(lensOffset);
	//setForceAspectRatio(false);

	//setPosition(eyeX,eyeY,dist);
	lookAt(dhVector(eyeX,eyeY,0),dhVector(0,1,0));

	/*
	if(vFlip){
		setScale(1,-1,1);
	}
	*/
}



//----------------------------------------
void vwCamera::setFov(float f) {
	fov = f;
}

//----------------------------------------
void vwCamera::setNearClip(float f) {
	nearClip = f;
}

//----------------------------------------
void vwCamera::setFarClip(float f) {
	farClip = f;
}

//----------------------------------------
void vwCamera::setLensOffset(const dhVector & lensOffset){
	this->lensOffset = lensOffset;
}

//----------------------------------------
void vwCamera::setAspectRatio(float aspectRatio){
	this->aspectRatio = aspectRatio;
	//setForceAspectRatio(true);
}
