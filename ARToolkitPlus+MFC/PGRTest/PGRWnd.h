#pragma once


// PGRWnd
#include "../lib/guicon.h"


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
		//KillTimer(0);
	}

	BOOL IsCapturing()
	{
		return bCap;
	}

	void Run();

	void GetBuf(unsigned  char* p);
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


