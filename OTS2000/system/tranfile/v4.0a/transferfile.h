// transferfile.h : main header file for the TRANSFERFILE application
//

#if !defined(AFX_TRANSFERFILE_H__0D95699C_49F2_4F59_A0E7_70C294889BBA__INCLUDED_)
#define AFX_TRANSFERFILE_H__0D95699C_49F2_4F59_A0E7_70C294889BBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CTransferfileApp:
// See transferfile.cpp for the implementation of this class
//

class CTransferfileApp : public CWinApp
{
public:
	CTransferfileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferfileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTransferfileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERFILE_H__0D95699C_49F2_4F59_A0E7_70C294889BBA__INCLUDED_)
