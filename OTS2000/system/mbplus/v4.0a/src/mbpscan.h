// mbpscan.h : main header file for the MBPSCAN application
//

#if !defined(AFX_MBPSCAN_H__04AC4BE3_8033_4FC3_86A7_A95132E64607__INCLUDED_)
#define AFX_MBPSCAN_H__04AC4BE3_8033_4FC3_86A7_A95132E64607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMbpscanApp:
// See mbpscan.cpp for the implementation of this class
//

class CMbpscanApp : public CWinApp
{
public:
	CMbpscanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMbpscanApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMbpscanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBPSCAN_H__04AC4BE3_8033_4FC3_86A7_A95132E64607__INCLUDED_)
