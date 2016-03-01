
// PGRTestView.h : interface of the CPGRTestView class
//

#pragma once

#include "resource.h"
#include "PGRWnd.h"


class CPGRTestView : public CFormView
{
protected: // create from serialization only
	CPGRTestView();
	DECLARE_DYNCREATE(CPGRTestView)

public:
	enum{ IDD = IDD_PGRTEST_FORM };

// Attributes
public:
	CPGRTestDoc* GetDocument() const;

// Operations
public:
	
	PGRWnd m_wnd1;
	

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CPGRTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnInitCam();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // debug version in PGRTestView.cpp
inline CPGRTestDoc* CPGRTestView::GetDocument() const
   { return reinterpret_cast<CPGRTestDoc*>(m_pDocument); }
#endif

