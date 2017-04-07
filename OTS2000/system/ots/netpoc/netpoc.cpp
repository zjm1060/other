// netpoc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "netpoc.h"
#include "netpocDlg.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetpocApp

BEGIN_MESSAGE_MAP(CNetpocApp, CWinApp)
//{{AFX_MSG_MAP(CNetpocApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetpocApp construction

CNetpocApp::CNetpocApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNetpocApp object

CNetpocApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNetpocApp initialization

BOOL CNetpocApp::InitInstance()
{
  AfxEnableControlContainer();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

  AfxSocketInit();

  if(MountDB(DB_NAME) == -1)
  {
	  printf("==== SimuService.c: call MountDB(DATABASE_NAME) error\n");		
	  exit(0);
  }
  
  if(lan_init_all() == FALSE)
  {
	  printf("==== SimuService: call Lan_init_all() error\n");		
	  exit(0);
	}

  #ifdef _AFXDLL
    Enable3dControls(); // Call this when using MFC in a shared DLL
  #else
    Enable3dControlsStatic(); // Call this when linking to MFC statically
  #endif

  CNetpocDlg dlg;
  m_pMainWnd = &dlg;
  int nResponse = dlg.DoModal();
  if (nResponse == IDOK)
  {
    // TODO: Place code here to handle when the dialog is
    //  dismissed with OK
  }
  else if (nResponse == IDCANCEL)
  {
    // TODO: Place code here to handle when the dialog is
    //  dismissed with Cancel
  }

  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  return FALSE;
}
