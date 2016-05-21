#pragma once

#if (_MSC_VER)      
// make microsoft visual studio complain less about double / float conversion.
#pragma warning(disable : 4244)
#endif


#include "gl/gl.h"
#include "gl/glu.h"
#pragma comment(lib, TEXT("glu32.lib"))
#pragma comment(lib, TEXT("opengl32.lib"))


#include "../3D/dhVector.h"


#include "../vw/vwLight.h"
#include "../vw/vwO.h"
#include "../vw/vwCamera.h"
#include "../vw/vwPrimitives.h"


#include "../vw/vwObj.h"

#include "../Elapsed.h"
// CGL

class CGL : public CWnd
{
	DECLARE_DYNAMIC(CGL)

public:
	CGL();
	virtual ~CGL();

public:
	// World
public:
	vwLight		m_light;
	//vwO			m_obj1;
	//vwObj		m_obj7;
	
	int view_mode;

	//vArray<mv3DObj*,mv3DObj*>	m_objs;
	vwCamera	*pCamera;
	
private:
	vwCamera	m_camera;


	
public:
	int			tcnt;

virtual void Create( CRect, CWnd *);

	void	Draw();

	void	SwitchViewMode();

	BOOL bShowVert;
	void	ToggleDispVertex();


	int bShowDebugs;
	void ToggleShowDebugs();
	
public:
	virtual void	InitValues(){};
	virtual void	CreateObjects(){};
	virtual void	SetCamera(dhVector o_, dhVector r_)
	{
		pCamera->vw.o		= o_; // -55 is too small
		pCamera->vw.r		= r_;
	};

	virtual void	SetCameraPos(dhVector o_)
	{
		pCamera->vw.o		= o_; // -55 is too small
	};

	virtual void	ProcAtEveryFrame(){}
	virtual void	DrawLayer0();
	virtual void	DrawLayer1();


	CPoint mpt;

	
	CElapsed m_tm;
	float t0;


public:
	virtual void	GenTex(){}
	

// User Interaction
public:
	virtual void	ProcKey(UINT){};
	//virtual void	TouchesBegan(CPoint);
	//virtual void	TouchesEnded(CPoint);


protected:
	//CPoint	ptMouseOld;
	HDC		m_hDC;			
	HGLRC	m_hRC;





protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


