// ProSheet.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/ProSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProSheet

IMPLEMENT_DYNAMIC(CProSheet, CPropertySheet)

CProSheet::CProSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CProSheet::CProSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage( &m_telaudio );
//	AddPage( &m_telcall );
	AddPage( &m_telsms );
}

CProSheet::~CProSheet()
{

}


BEGIN_MESSAGE_MAP(CProSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CProSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProSheet message handlers

BOOL CProSheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(MessageBox("Make Sure To End ?","Warning!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{
		return CPropertySheet::OnCommand(wParam, lParam);
	}
	return(TRUE);
}
