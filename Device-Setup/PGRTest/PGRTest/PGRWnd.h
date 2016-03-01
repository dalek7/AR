#pragma once

// Telerobotics and Control Lab., KAIST
// Seung-Chan Kim
// 2014/05/19
// PGRWnd


// PGRWnd
#include "../lib/FlyCapture2/PGRCamera.h"
#include "../lib/guicon.h"
//#include "../lib/CvvImage.h"


class PGRWnd : public CWnd
{
	DECLARE_DYNAMIC(PGRWnd)

public:
	PGRWnd();
	virtual ~PGRWnd();


public:
	void Create(CRect rc, CWnd *pWnd);

	void InitCam();
	void StopCam()
	{
		bCap = FALSE;
		KillTimer(0);
	}

	BOOL IsCapturing()
	{
		return bCap;
	}

	void Run();
protected:
	BOOL bCap;
	//

public:
	PGRCamera mPgrCam;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
};


