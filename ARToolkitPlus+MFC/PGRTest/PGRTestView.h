
// PGRTestView.h : interface of the CPGRTestView class

// 5/19/2014
// Seung-Chan Kim
// Telerobotics and Control Lab., KAIST

#pragma once

#include "resource.h"

#include "../lib/GL/GLTouch.h"
#include "CamWnd.h"
#include "afxwin.h"
#include "../lib/cmdedit.h"
#include "../lib/ddStatic.h"
#include "../lib/guicon.h"
#include "afxcmn.h"
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
	CGLTouch	m_gl;

	CamWnd m_wnd2;
	
	void DispMatrix1(float*);
	void DispMatrix2(float*);
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
	afx_msg void OnInitCamLogitech();
	afx_msg void OnDestroy();
	CCMDEdit m_edit_ar1;
	CCMDEdit m_edit_ARDBG;
	CButton m_chk_dbg1;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_chk_disp_binary;
	CSliderCtrl m_sld_threshold;
	ddStatic m_info_slder1;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#ifndef _DEBUG  // debug version in PGRTestView.cpp
inline CPGRTestDoc* CPGRTestView::GetDocument() const
   { return reinterpret_cast<CPGRTestDoc*>(m_pDocument); }
#endif

