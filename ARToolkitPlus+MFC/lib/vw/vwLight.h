#pragma once


#include "gl/gl.h"
#include "gl/glu.h"

class vwLight
{
public:
	vwLight(void);
	~vwLight(void);

public:
	void MakeFog();



public:
	void SetDefaultMaterial();
	void SetDefaultMaterial2();
	void SetDefaultMaterial3();

};
