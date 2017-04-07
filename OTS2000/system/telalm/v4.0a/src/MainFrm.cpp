// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "../inc/teldef.h"

#include "../inc/MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// Size the window to 4/5 screen size and center it  //by zm
    cs.cy = ::GetSystemMetrics(SM_CYSCREEN) * 5 / 6; 
    cs.cx = ::GetSystemMetrics(SM_CXSCREEN) * 5 / 6; 
    cs.y = ((cs.cy * 1) - cs.cy*4/5) / 2; 
    cs.x = ((cs.cx * 1) - cs.cx*4/5) / 2;
    cs.style    = WS_OVERLAPPEDWINDOW;
	cs.lpszName = "H9000 ONCALL Define Interface";

	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnAppExit() 
{
	// TODO: Add your command handler code here
	if( MessageBox("ÒªÍË³öÂð ?","¾¯¸æ!!!",
		        	MB_OKCANCEL | MB_ICONQUESTION )==IDOK       )
		exit(0);
}



void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnAppExit();
}

