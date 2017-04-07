// mbpscanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mbpscan.h"
#include "mbpscanDlg.h"
#include "../inc/mbx.h" 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int IsStop;
CString strLine;
CMbpscanDlg *pMbpscanDlg;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMbpscanDlg dialog

CMbpscanDlg::CMbpscanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMbpscanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMbpscanDlg)
	m_editLog = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pMbpscanDlg=this;
}

void CMbpscanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMbpscanDlg)
	DDX_Control(pDX, IDC_EDITLOG, m_editLogCtrl);
	DDX_Control(pDX, IDSTOP, m_buttonStop);
	DDX_Control(pDX, IDSTART, m_buttonStart);
	DDX_Text(pDX, IDC_EDITLOG, m_editLog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMbpscanDlg, CDialog)
	//{{AFX_MSG_MAP(CMbpscanDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSTART, OnStart)
	ON_BN_CLICKED(IDSTOP, OnStop)
	ON_MESSAGE(WM_THREADPROC,UpdateLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMbpscanDlg message handlers

BOOL CMbpscanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMbpscanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMbpscanDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	/* pMbpscanDlg->ShowWindow(SW_HIDE);  */
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMbpscanDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMbpscanDlg::OnOK() 
{
	// TODO: Add extra validation here

	void *pt = NULL;
/*	MBXScan(pt);	 */
	CDialog::OnOK();
}


/********************************************************************************************************
  code below is added by yyp  06.04.01
*********************************************************************************************************/



void CMbpscanDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	IsStop = FALSE;
printf("hello !\n");
	//第二步：创建传输进程
	HWND hWnd=GetSafeHwnd();
	AfxBeginThread((AFX_THREADPROC) MBXScan,hWnd,0);
/*	UpdateLogLine("采集线程已建立！");*/
	//第三步：对话框界面处理
}

void CMbpscanDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	IsStop = TRUE;	
}


/////////////////////////////////////////////////////////////////////
//更新日志框的内容
/////////////////////////////////////////////////////////////////////
long CMbpscanDlg::UpdateLog(WPARAM wParam, LPCTSTR lParam) 
{
	// TODO: Add your command handler code here
	UpdateData(true);
	Sleep(100);
	if(m_editLog.GetLength()>10000)
	{
		//m_editLog=m_editLog.Left(500);
		m_editLog="";
	}
	CString strtmp="\r\n";
	m_editLog=m_editLog + (CString) lParam;
	UpdateData(false);
	m_editLogCtrl.SetSel(m_editLog.GetLength()-1,m_editLog.GetLength());
	return 0;
}

/////////////////////////////////////////////////////////////////////
//更新日志框的内容，简单方式
/////////////////////////////////////////////////////////////////////
long CMbpscanDlg::UpdateLogLine(CString MyStrLine) 
{
	// TODO: Add your command handler code here
	UpdateData(true);
	Sleep(10);
	if(m_editLog.GetLength()>10000)
	{
		//m_editLog=m_editLog.Left(500);
		m_editLog="";
	}
	CString strtmp="\r\n";
	m_editLog=m_editLog +strtmp+MyStrLine;
	UpdateData(false);
	m_editLogCtrl.SetSel(m_editLog.GetLength()-1,m_editLog.GetLength());
	return 0;
}

void MyMsgBox(unsigned char * MyMsg)
{
	/*MyMsg = "HELLO";*/
	pMbpscanDlg->PostMessage(WM_THREADPROC,0,(LPARAM)(LPCTSTR)MyMsg);
}

