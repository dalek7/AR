#include "StdAfx.h"
#include "vwLight.h"
/*
// TODO


#include "ofMaterial.h"

void ofMaterial::begin() {
    // save previous values, opengl es cannot use push/pop attrib
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,&prev_diffuse.r);
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,&prev_specular.r);
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,&prev_ambient.r);
	glGetMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,&prev_emissive.r);
	glGetMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

    // Material colors and properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &ambient.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &emissive.r);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void ofMaterial::end() {
    // Set previous material colors and properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &prev_diffuse.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &prev_specular.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &prev_ambient.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &prev_emissive.r);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, prev_shininess);
}


*/
vwLight::vwLight(void)
{
}

vwLight::~vwLight(void)
{
}


void vwLight::SetDefaultMaterial()
{

	//glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	//glClearColor(0.5f,0.5f,0.5f,1.0f);					// We'll Clear To The Color Of The Fog
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	{
		// Lights
		GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};//{0.1f, 0.1f, 0.1f, 1.0f};
		GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat	specularProperties[] = {0.1f, 0.1f, 0.1f, 1.0f};
		
		glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
		glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
		glPolygonMode(GL_FRONT,GL_FILL);
		//glPolygonMode(GL_FRONT,GL_LINE);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseProperties);
		glEnable( GL_COLOR_MATERIAL );
	}	

	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

}


void vwLight::MakeFog()
{	
	
	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };

	GLuint	fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// Storage For Three Types Of Fog
	GLuint	fogfilter =2;								// Which Fog Mode To Use 
	GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,1.0f};		// Fog Color

	//GLfloat	fogColor[4] = {1.0f,1.0f,1.0f,0.2f};
	glFogi(GL_FOG_MODE, fogMode[fogfilter]);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.012f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, 0.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 150.0f);							// Fog End Depth
	glEnable(GL_FOG);			
}

/*
void vwLight::SetDefaultMaterial2()
{

	{
		// Lights
		GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};//{0.1f, 0.1f, 0.1f, 1.0f};
		GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat	specularProperties[] = {0.1f, 0.1f, 0.1f, 1.0f};
		
		glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
		glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
		glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
		glPolygonMode(GL_FRONT,GL_FILL);
		//glPolygonMode(GL_FRONT,GL_LINE);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseProperties);
		glEnable( GL_COLOR_MATERIAL );
	}	

	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	
}

void vwLight::SetDefaultMaterial3()
{

	float LightPos[] = { 0.0f, 5.0f,-40.0f, 1.0f};			// Light Position
	float LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};			// Ambient Light Values
	float LightDif[] = { 0.6f, 0.6f, 0.6f, 1.0f};			// Diffuse Light Values
	float LightSpc[] = {-0.2f, -0.2f, -0.2f, 1.0f};			// Specular Light Values

	float MatAmb[] = {0.4f, 0.4f, 0.4f, 1.0f};				// Material - Ambient Values
	float MatDif[] = {0.2f, 0.6f, 0.9f, 1.0f};				// Material - Diffuse Values
	float MatSpc[] = {0.0f, 0.0f, 0.0f, 1.0f};				// Material - Specular Values
	float MatShn[] = {0.0f};								// Mad


	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// Set Light1 Position
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);			// Set Light1 Ambience
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);			// Set Light1 Diffuse
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);		// Set Light1 Specular
	glEnable(GL_LIGHT1);								// Enable Light1
	glEnable(GL_LIGHTING);								// Enable Lighting



	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);			// Set Material Ambience
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDif);			// Set Material Diffuse
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpc);		// Set Material Specular
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShn);		// Set Material Shininess

	
	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling


}
*/