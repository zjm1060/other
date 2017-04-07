#if !defined(AFX_PROSHEET_H__4CAD0EC5_71A3_4EE0_880A_1C1A19B3695E__INCLUDED_)
#define AFX_PROSHEET_H__4CAD0EC5_71A3_4EE0_880A_1C1A19B3695E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProSheet

#include	"TelAudio.h"
//#include	"TelCall.h"
#include	"TelSms.h"
#include    "SmsDef.h"
#include "TelDefine.h"

class CProSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CProSheet)

// Construction
public:
	CProSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CProSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

	CTelAudio	m_telaudio;
//	CTelCall	m_telcall;
	CTelSms		m_telsms;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProSheet)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROSHEET_H__4CAD0EC5_71A3_4EE0_880A_1C1A19B3695E__INCLUDED_)
