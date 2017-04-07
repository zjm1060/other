// teldefDoc.cpp : implementation of the CTeldefDoc class
//

#include "stdafx.h"
#include "../inc/teldef.h"

#include "../inc/teldefDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeldefDoc

IMPLEMENT_DYNCREATE(CTeldefDoc, CDocument)

BEGIN_MESSAGE_MAP(CTeldefDoc, CDocument)
	//{{AFX_MSG_MAP(CTeldefDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeldefDoc construction/destruction

CTeldefDoc::CTeldefDoc()
{
	// TODO: add one-time construction code here

}

CTeldefDoc::~CTeldefDoc()
{
}



BOOL CTeldefDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTeldefDoc serialization

void CTeldefDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTeldefDoc diagnostics

#ifdef _DEBUG
void CTeldefDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTeldefDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTeldefDoc commands
