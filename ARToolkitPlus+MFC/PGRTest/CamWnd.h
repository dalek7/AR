
// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST

#pragma once


// CamWnd
#include "../lib/BaseCV.h"
#include "../lib/ddCVMacro.h"
#include "../lib/ttMath.h"

#include "../lib/ARToolKitPlus/ofxARToolkitPlus.h"

class CamWnd : public CWnd
{
	DECLARE_DYNAMIC(CamWnd)

public:
	CamWnd();
	virtual ~CamWnd();
	void Create(CRect rc, CWnd *pWnd);

	
	BaseCV m_cv;

	ofxARToolkitPlus artk;	

	int width, height;
public:
	void Proc();
	virtual void Run();
	virtual void Init();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};


