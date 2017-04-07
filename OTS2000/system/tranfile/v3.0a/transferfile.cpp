// transferfile.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "transferfile.h"
#include "transferfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferfileApp

BEGIN_MESSAGE_MAP(CTransferfileApp, CWinApp)
	//{{AFX_MSG_MAP(CTransferfileApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferfileApp construction

CTransferfileApp::CTransferfileApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTransferfileApp object

CTransferfileApp theApp;



/////////////////////////////////////////////////////////////////////////////
// CTransferfileApp initialization

BOOL CTransferfileApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	/******** 以下代码将实现只运行程序的一个实例 ********/
	HANDLE h;
	if (!(h=CreateMutex(NULL,TRUE,"transferfile")))
	{
		MessageBox(NULL,"创建Mutex失败!","transferfile1",MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}
	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,"已有transferfile的一个实例在运行, 只允许启动一个实例，当前实例将被终止!",
						"transferfile",MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}
	else
	{
		ReleaseMutex(h);
	}

	/******** 结束 **************************************/


	CTransferfileDlg dlg;
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
