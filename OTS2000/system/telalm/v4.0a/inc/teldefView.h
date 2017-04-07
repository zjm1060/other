// teldefView.h : interface of the CTeldefView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TELDEFVIEW_H__B2C3D18F_6DC7_4846_94E9_75F6CE4492A5__INCLUDED_)
#define AFX_TELDEFVIEW_H__B2C3D18F_6DC7_4846_94E9_75F6CE4492A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProSheet.h"

class CTeldefView : public CView
{
protected: // create from serialization only
	CTeldefView();
	DECLARE_DYNCREATE(CTeldefView)

// Attributes
public:
	CTeldefDoc* GetDocument();

// Operations
public:



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeldefView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTeldefView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTeldefView)
	afx_msg void OnTelInput();
	afx_msg void OnClsDef();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCreatText();
	afx_msg void OnSelectSms();
	afx_msg void OnSmsInput();
	afx_msg void OnSmsnumInput();
	afx_msg void OnSmssendInput();
	afx_msg void OnSelectSmstime();
	afx_msg void OnTeldefInput();
	afx_msg void OnAnaDef();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in teldefView.cpp
inline CTeldefDoc* CTeldefView::GetDocument()
   { return (CTeldefDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELDEFVIEW_H__B2C3D18F_6DC7_4846_94E9_75F6CE4492A5__INCLUDED_)
