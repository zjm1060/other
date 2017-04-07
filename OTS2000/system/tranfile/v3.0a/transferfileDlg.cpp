// transferfileDlg.cpp : implementation file
//
#include "stdafx.h"
#include "transferfile.h"
#include "transferfileDlg.h"
#include "DstprmDlg.h"
#include "SetFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

/////////////////////////////////////////////////////////////////////
//全局变量
/////////////////////////////////////////////////////////////////////

int FtpTransMode=1;						//ftp传输方式,0为.bat方式，1为采用CFtpConnection对象方式，目前只用方式1											
CString XcopyFilePath="";				//xcopy文件的完整路径及文件名
CTransferfileDlg *pTransferfileDlg;
int threadcontroller =0;				//进程控制标识变量，1为正在传输，0为未传输
int transferType=0;						//传输种类，0为发送,1为接收
bool IsExit=false;						//退出标识，若为true则退出
bool IsTransDB=false;					//是否正在传输Database.Out文件
int curDst=-1;							//当前所选目标计算机在m_config的编号
CString csMessage;						//一条日志消息内容
CString strDriverName[6];
CString strVerName[3];
CTransferConfig m_config;				//传输配置信息变量
CString CurExeFilePath="";				//可执行文件的完整路径及文件名
CString strWindowsPath="";				//系统Windows目录
CString FtpTxtName="autoftp.txt";		//autoftp.txt文件
CString FtpBatName="autoftp.bat";		//autoftp.bat文件
int nErrCount=0;						//统计传输过中的故障次数
int nTransferCount=0;					//传输文件总数
CString strErrLog="";					//错误日志
					
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
	virtual void OnOK();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferfileDlg dialog

CTransferfileDlg::CTransferfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransferfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferfileDlg)
	m_editLog = _T("");
	m_radioVer = 2;
	m_radioDriver = 2;
	m_check1 = TRUE;
	m_check2 = FALSE;
	m_check3 = TRUE;
	m_check4 = TRUE;
	m_check5 = TRUE;
	m_check6 = TRUE;
	m_check7 = TRUE;
	m_check8 = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON4);
	pTransferfileDlg=this;
}

void CTransferfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferfileDlg)
	DDX_Control(pDX, IDSETFILTER, m_buttonSet);
	DDX_Control(pDX, IDSTARTGET, m_buttonGet);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Control(pDX, IDSAVECONFIG, m_buttonSave);
	DDX_Control(pDX, IDADDDST, m_buttonAddDst);
	DDX_Control(pDX, IDSTARTTRANSFER, m_starttransfer);
	DDX_Control(pDX, IDC_EDITLOG, m_editLogCtrl);
	DDX_Control(pDX, IDC_LISTDST, m_listDst);
	DDX_Text(pDX, IDC_EDITLOG, m_editLog);
	DDX_Radio(pDX, IDC_RADIOVER1, m_radioVer);
	DDX_Radio(pDX, IDC_RADIODRIVERA, m_radioDriver);
	DDX_Check(pDX, IDC_CHECK1, m_check1);
	DDX_Check(pDX, IDC_CHECK2, m_check2);
	DDX_Check(pDX, IDC_CHECK3, m_check3);
	DDX_Check(pDX, IDC_CHECK4, m_check4);
	DDX_Check(pDX, IDC_CHECK5, m_check5);
	DDX_Check(pDX, IDC_CHECK6, m_check6);
	DDX_Check(pDX, IDC_CHECK7, m_check7);
	DDX_Check(pDX, IDC_CHECK8, m_check8);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransferfileDlg, CDialog)
	//{{AFX_MSG_MAP(CTransferfileDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ADDMENU, OnAddDst)
	ON_COMMAND(ID_DELMENU, OnDelDst)
	ON_COMMAND(ID_MODIFYMENU, OnModifyDst)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDSTARTTRANSFER, OnStarttransfer)
	ON_BN_CLICKED(IDADDDST, OnAdddst)
	ON_BN_CLICKED(IDSAVECONFIG, OnSaveconfig)
	ON_BN_CLICKED(IDSTARTGET, OnStartget)
	ON_COMMAND(ID_GETMENU, OnGetmenu)
	ON_BN_CLICKED(IDSETFILTER, OnSetFilter)
	ON_BN_CLICKED(IDC_RADIODRIVERF, OnRadiodriverf)
	ON_BN_CLICKED(IDC_RADIODRIVERE, OnRadiodrivere)
	ON_BN_CLICKED(IDC_RADIODRIVERD, OnRadiodriverd)
	ON_NOTIFY(NM_CLICK, IDC_LISTDST, OnClickListdst)
	ON_BN_CLICKED(IDSETFILTER1, OnSetfilter1)
	ON_BN_CLICKED(IDSETFILTER2, OnSetfilter2)
	ON_BN_CLICKED(IDSETFILTER3, OnSetfilter3)
	ON_BN_CLICKED(IDSETFILTER4, OnSetfilter4)
	ON_BN_CLICKED(IDSETFILTER5, OnSetfilter5)
	ON_BN_CLICKED(IDSETFILTER6, OnSetfilter6)
	ON_BN_CLICKED(IDSETFILTER7, OnSetfilter7)
	ON_BN_CLICKED(IDC_RADIODRIVERC, OnRadiodriverc)
	ON_BN_CLICKED(IDC_RADIOVER2, OnRadiover2)
	ON_BN_CLICKED(IDC_RADIOVER3, OnRadiover3)
	ON_COMMAND(ID_ENABLEMENU, OnEnablemenu)
	ON_BN_CLICKED(IDSETFILTER8, OnSetfilter8)
	ON_MESSAGE(WM_THREADPROC,UpdateLog)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferfileDlg message handlers

BOOL CTransferfileDlg::OnInitDialog()
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
	//初始化全局变量
	int j,curPos;
	CString strtmp="";
	TCHAR exeFullPath[MAX_PATH];	// MAX_PATH在API中定义了吧，好象是128 
	//获取当前运行路径及文件名。
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	strtmp=exeFullPath;		
	CurExeFilePath="";
	j=1;
	while(j>=0)
	{
		j=strtmp.Find("\\");
		CurExeFilePath=CurExeFilePath+strtmp.Left(j+1);
		strtmp=strtmp.Right(strtmp.GetLength()-j-1);
		curPos=j;
	};
	//获得系统目录
	GetWindowsDirectory(exeFullPath,MAX_PATH);
	strWindowsPath=exeFullPath;
					
	
	strDriverName[0]="A";
	strDriverName[1]="C";
	strDriverName[2]="D";
	strDriverName[3]="E";
	strDriverName[4]="F";
	strDriverName[5]="J";
	strVerName[0]="v2.0a";
	strVerName[1]="v3.0a";
	strVerName[2]="v4.0a";
	//初始化日志框
	UpdateData(true);
	//初始化目标计算机列表
	m_listDst.InsertColumn(0, "IP地址");		
	m_listDst.InsertColumn(1, "类型");
	m_listDst.InsertColumn(2, "启用");		
	m_listDst.SetColumnWidth(0, 150);
	m_listDst.SetColumnWidth(1, 60);
	m_listDst.SetColumnWidth(2, 60);
	UpdateData(false);
	
	//从配置文件获取配置信息并显示在对话框中
	m_config.GetConfigFromFile();
	m_config.SetConfigDir();  
	UpdateIPList();
	UpdatePubConfig();
	//获取Ftp传输方式，0为.bat方式，1为类方式
	FtpTransMode=m_config.m_ftpTransMode;	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTransferfileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTransferfileDlg::OnPaint() 
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
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTransferfileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////
//响应右键事件
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//如果正在传输，则禁用右键菜单
	if(threadcontroller==1)
	{
		return;
	}
	//得到选中的客户端
	int nIndex = this->m_listDst.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nIndex == -1)
        return;

	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_DSTIPMENU));
//	menu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this, NULL);
	
	int i,iState;
	int nDst=0;										//选中的目标计算机编号
	CString	str;
	int nItemSelected=m_listDst.GetSelectedCount();	//所选表项数
	int nItemCount=m_listDst.GetItemCount();		//表项总数
	if(nItemSelected<1) return;
	//获得所选计算机编号
	for(i=0;i<nItemCount;i++)
	{
		iState=m_listDst.GetItemState(i,LVIS_SELECTED);
		if(iState!=0)
		{
			nDst=m_config.m_dstCount-1-i;
			break;
		}
	}
	if(m_config.m_dstComputer[nDst].Status==0)
		str= "启用";
	else
		str= "禁用";
	menu.ModifyMenu(ID_ENABLEMENU, MF_BYCOMMAND|MF_STRING, ID_ENABLEMENU, str);
	menu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this, NULL);

}
/////////////////////////////////////////////////////////////////////
//响应弹出菜单项“添加计算机”
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnAddDst() 
{
	// TODO: Add your command handler code here
	CDstprmDlg dlg;								//声明对话框对象
	//m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();				//弹出对话框
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
	
}
/////////////////////////////////////////////////////////////////////
//删除目标计算机
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnDelDst() 
{
	// TODO: Add your command handler code here
	//确认是否发送
	if(MessageBox("是否确认删除?","请确认...",MB_YESNO|MB_ICONQUESTION)!=IDYES)
	{
		return;
	}
	int i,iState;
	int nDst=0;										//选中的目标计算机编号
	int nItemSelected=m_listDst.GetSelectedCount();	//所选表项数
	
	int nItemCount=m_listDst.GetItemCount();		//表项总数
	if(nItemSelected<1) return;
	for(i=0;i<nItemCount;i++)
	{
		iState=m_listDst.GetItemState(i,LVIS_SELECTED);
		if(iState!=0)
		{
			nDst=m_config.m_dstCount-1-i;
			//删除所选计算机
			m_config.DelDst(nDst);
			UpdateIPList();	
			break;
		}
	}
	return;
}
void CTransferfileDlg::OnModifyDst() 
{
	// TODO: Add your command handler code here
	CDstprmDlg dlg(this);							//声明对话框对象
	int i,iState;
	int nDst=0;										//选中的目标计算机编号
	int nItemSelected=m_listDst.GetSelectedCount();	//所选表项数
	int nItemCount=m_listDst.GetItemCount();		//表项总数
	if(nItemSelected<1) return;
	//获得所选计算机编号
	for(i=0;i<nItemCount;i++)
	{
		iState=m_listDst.GetItemState(i,LVIS_SELECTED);
		if(iState!=0)
		{
			nDst=m_config.m_dstCount-1-i;
			break;
		}
	}
	//给对话框填充内容
	dlg.m_ip=m_config.m_dstComputer[nDst].DstIP;
	dlg.m_passwd=m_config.m_dstComputer[nDst].Passwd;
	dlg.m_pwdConf=m_config.m_dstComputer[nDst].Passwd;
	dlg.m_username=m_config.m_dstComputer[nDst].Username;
	dlg.m_radioType=m_config.m_dstComputer[nDst].DstType;
	dlg.m_status=m_config.m_dstComputer[nDst].Status;
	for(i=0;i<6;i++)
	{
		if(strDriverName[i]==m_config.m_dstComputer[nDst].DstDriver)
		{
			dlg.m_radiodriver=i;
			break;
		}
	}
	int nResponse = dlg.DoModal();				//弹出对话框
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		//将修改后的信息写到m_config
		m_config.m_dstComputer[nDst].DstIP=dlg.m_ip;
		if(dlg.m_radiodriver<0) dlg.m_radiodriver=0;
		//从对化框读取得源盘和版本并赋予m_config	
		m_config.m_dstComputer[nDst].DstDriver=strDriverName[dlg.m_radiodriver];
		m_config.m_dstComputer[nDst].DstType=dlg.m_radioType;
		m_config.m_dstComputer[nDst].Status=dlg.m_status;
		m_config.m_dstComputer[nDst].Username=dlg.m_username;
		m_config.m_dstComputer[nDst].Passwd=dlg.m_passwd;
		m_config.m_dstComputer[nDst].PortNum=21;
		//刷新m_config的路径相关信息
		m_config.SetConfigDir();
		//通过m_config的目标计算机信息更新IP列表的信息
		UpdateIPList();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

}

void CTransferfileDlg::OnButton1() 
{
	//AfxMessageBox(CurExeFilePath);
	CString strDir="e:\\a\\b\\c\\d\\e\\f\\";
	if(CreateDirectory(strDir,NULL)==0)
	{
		AfxMessageBox("创建目录 出错了！" );
	}
}
/////////////////////////////////////////////////////////////////////
//更新日志框的内容
/////////////////////////////////////////////////////////////////////
long CTransferfileDlg::UpdateLog(WPARAM wParam, LPCTSTR lParam) 
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
	m_editLog=m_editLog +strtmp+lParam;
	UpdateData(false);
	m_editLogCtrl.SetSel(m_editLog.GetLength()-1,m_editLog.GetLength());
	return 0;
}

/////////////////////////////////////////////////////////////////////
//更新日志框的内容，简单方式
/////////////////////////////////////////////////////////////////////
long CTransferfileDlg::UpdateLogLine(CString MyStrLine) 
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

/////////////////////////////////////////////////////////////////////
//更新IP列表的内容
/////////////////////////////////////////////////////////////////////
bool CTransferfileDlg::UpdateIPList() 
{
	// TODO: Add your command handler code here
	UpdateData(true);
	int i=0;
	int nIndex=0;
	int tmp;
	CString strDstType="Unix";
	CString strDstStatus="启用";
	//清空原有内容
	m_listDst.DeleteAllItems();
	//循环显示每一个目标计算机
	for(i=0;i<m_config.m_dstCount;i++)
	{	
		tmp=m_config.m_dstComputer[i].DstType;
		if(m_config.m_dstComputer[i].DstType==0) 
			strDstType="PC";
		else
			strDstType="Unix";
		if(m_config.m_dstComputer[i].Status==0) 
			strDstStatus="禁用";
		else
			strDstStatus="启用";
		nIndex = m_listDst.InsertItem(0,m_config.m_dstComputer[i].DstIP);
		m_listDst.SetItemText(nIndex, 1, strDstType);
		m_listDst.SetItemText(nIndex, 2, strDstStatus);
	}
	UpdateData(false);
	return true;
}
/////////////////////////////////////////////////////////////////////
//更新共用信息的内容
/////////////////////////////////////////////////////////////////////
bool CTransferfileDlg::UpdatePubConfig() 
{
	// TODO: Add your command handler code here
	UpdateData(true);
	int i=0;
	//版本
	for(i=0;i<3;i++)
	{
		if(strVerName[i]==m_config.m_srcVersion)
		{
			m_radioVer=i;
		}
	}
	//源盘
	for(i=0;i<6;i++)
	{
		if(strDriverName[i]==m_config.m_srcDriver)
		{
			m_radioDriver=i;
		}
	}
	//所选文件
	if(m_config.m_srcFileSel[0]==1)
	{
		m_check1=1;
	}
	else
	{
		m_check1=0;
	}
	if(m_config.m_srcFileSel[1]==1)
	{
		m_check2=1;
	}
	else
	{
		m_check2=0;
	}
	if(m_config.m_srcFileSel[2]==1)
	{
		m_check3=1;
	}
	else
	{
		m_check3=0;
	}
	if(m_config.m_srcFileSel[3]==1)
	{
		m_check4=1;
	}
	else
	{
		m_check4=0;
	}
	if(m_config.m_srcFileSel[4]==1)
	{
		m_check5=1;
	}
	else
	{
		m_check5=0;
	}
	if(m_config.m_srcFileSel[5]==1)
	{
		m_check6=1;
	}
	else
	{
		m_check6=0;
	}
	if(m_config.m_srcFileSel[6]==1)
	{
		m_check7=1;
	}
	else
	{
		m_check7=0;
	}
	if(m_config.m_srcFileSel[7]==1)
	{
		m_check8=1;
	}
	else
	{
		m_check8=0;
	}
	UpdateData(false);
	return true;
}

void CTransferfileDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
}

void CTransferfileDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	if(m_config.GetConfigFromFile()==false)
		AfxMessageBox("读取配置文件出现错误！");
	else
		AfxMessageBox("配置信息初始化成功！");
}

/////////////////////////////////////////////////////////////////////
//PC机文件传输处理函数
//采用shellexecuteex调用xcopy.exe进行文件传输
/////////////////////////////////////////////////////////////////////
bool TransferFileToPC(int nDst)
{
	int i,j;
	CString strTmp;							
	XcopyFilePath=strWindowsPath+"\\system32\\xcopy.exe";
	//查找该文件是否存在
	CFileFind fileFind;								//查找文件对象
	i=fileFind.FindFile(XcopyFilePath);
	//如果没找到，则取当前目录下的xcopy.exe文件
	if(!i)
	{
		XcopyFilePath=CurExeFilePath+"xcopy.exe";
	}
	SHELLEXECUTEINFO ShExecInfo = {0};				//ShellExecuteEX参数
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = XcopyFilePath;	
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;	
	for(i=0;i<MaxFileType;i++)
	{
		if(m_config.m_srcFileSel[i]==1)
		{
			for(j=0;j<m_config.m_srcFileNum[j];j++)
			{
				strTmp= " " + m_config.m_srcFileDir[i]+"\\"+m_config.m_srcFileList[i][j];
				csMessage=" xcopy " + strTmp + "     结束";
				strTmp= strTmp+ "  \\\\"+ m_config.m_dstComputer[nDst].DstIP +"\\" ;
				strTmp= strTmp+ m_config.m_dstComputer[nDst].DstFileDir[i] +"\\ ";
				strTmp= strTmp + " /y";
				ShExecInfo.lpParameters=strTmp;
				ShellExecuteEx(&ShExecInfo);
				WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
				Sleep(30);
				MyMsgBox(csMessage);
				Sleep(100);
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////
//Unix机文件传输处理函数
/////////////////////////////////////////////////////////////////////
bool TransferFileToUnix(int nDst)
{
	//判断nDst合法性
	if((nDst<0)||(nDst>(m_config.m_dstCount-1)))
	{
		csMessage="TransferFileToUnix函数中目标计算机编号错误!";
		MyMsgBox(csMessage);		
		return false;
	}
	int i,j;
	CFileFind fileFind;								//查找文件对象
	CString strtmp,strLocFiles,strDstDir,tFile,
		strSrcFullName,strDstFullName;				//路径，文件名
	UINT bContinue;									//标识
	CInternetSession *pInetSession;					//Internetsession对象
	CFtpConnection *pFtpConnection=NULL;			//ftpconnection对象
	CString DstIP,Username,Passwd;					//用户名，密码
	int PortNum;									//端口号
	DWORD myFlags;									//传输类型，为FTP_TRANSFER_TYPE_BINARY或FTP_TRANSFER_TYPE_ASCII
	IsTransDB=false;
	DstIP=m_config.m_dstComputer[nDst].DstIP;
	Username=m_config.m_dstComputer[nDst].Username;
	Passwd=m_config.m_dstComputer[nDst].Passwd;
	PortNum=m_config.m_dstComputer[nDst].PortNum;
	//初始化Internetsession对象	
	pInetSession=new CInternetSession(AfxGetAppName(),1, PRE_CONFIG_INTERNET_ACCESS); 
	try 
	{	
		//根据IP或servername打开ftp连接
		Sleep(30);
		csMessage="正在打开ftp连接，请稍候...";
		MyMsgBox(csMessage);
		pFtpConnection=pInetSession->GetFtpConnection(DstIP,Username,Passwd,PortNum); 

	} 
	//异常处理
	catch (CInternetException* pEx)					
	{
		// catch errors from WinINet
		TCHAR szErr[1024];
		//如果建立连接失败则报错
		Sleep(30);
		if (pEx->GetErrorMessage(szErr, 1024))		
			{
			csMessage="连接" + DstIP +"失败，原因：" + szErr;
			MyMsgBox(csMessage);
			}
		else
			{		
			csMessage="Could not get the ftp connection";
			MyMsgBox(csMessage);
			}

		Sleep(30);
		csMessage="传输到Unix机 "+DstIP+" 时出现故障。\r\n请确认该主机是否在线或用户名及密码正确！";
		nErrCount=nErrCount+1;
		strErrLog=strErrLog+csMessage+"\r\n";
		MyMsgBox(csMessage);

		pEx->Delete();
		if (pFtpConnection != NULL)
		{
			pFtpConnection->Close();
		
		}
		delete pFtpConnection;
		if (pInetSession != NULL)
		{
			pInetSession->Close();
		}
		delete pInetSession;
		Sleep(1000);
		return false;
	}
	Sleep(30);
	csMessage="打开ftp连接成功!";
	MyMsgBox(csMessage);
	Sleep(30);
	CFtpFileFind CFtp(pFtpConnection);
	CString Tmp,Dir,strFilter;
	BOOL Ret=false;
	
	/////////////////////////////////////////////////////////////////////
	//如果为发送文件
	if(transferType==0)
	{
		//循环处理每一类文件
		for(i=0;i<MaxFileType;i++)						
		{	
			Sleep(100);
			if(IsExit==true)
			{
				break;
			}
			//得到传输类型，0为ASC文件，1为二进制文件
			if(m_config.m_fileType[i]==0)
			{	
				myFlags=FTP_TRANSFER_TYPE_ASCII;
			}	
			else
			{
				myFlags=FTP_TRANSFER_TYPE_BINARY;
			}
			//判断是否为生成文件
			if(i==1||i==3)
			{
				IsTransDB=true;
			}
			else
			{
				IsTransDB=false;
			}
			//continue;
			//处理Windows中*.DBIN与*.dbin相同的问题
	/*		if(i==7)
			{
				if((m_config.m_srcFileList[7]=="*.dbin") && 
					(m_config.m_srcFileList[4]=="*.DBIN"))
				{
					continue;
				}
			}
			if(m_config.m_srcFileList[i].GetLength()<1)
			{
				continue;
			}
	*/
			if(m_config.m_srcFileSel[i]==1)
			{

				for(j=0;j<m_config.m_srcFileNum[i];j++)
				{		
					m_config.m_srcFileList[i][j].TrimLeft();
					m_config.m_srcFileList[i][j].TrimRight();		
					//获取源文件目录及其过滤条件
					strLocFiles= m_config.m_srcFileDir[i]+"\\"+m_config.m_srcFileList[i][j];
					strDstDir=m_config.m_dstComputer[nDst].DstFileDir[i];
					//Sleep(30);
					//csMessage=strLocFiles+ "  正在进行传输...";
					//MyMsgBox(csMessage);
					//如果为发送文件
					//根据过滤条件查找文件
					bContinue=fileFind.FindFile(strLocFiles);		
					//若查找成功
					if(!bContinue)
					{
						Sleep(30);
						csMessage="查找" + strLocFiles+ "  失败。";
						MyMsgBox(csMessage);
						nErrCount=nErrCount+1;
						strErrLog=strErrLog+csMessage+"\t\r\n";
					}
					
					if(bContinue)								
					{
						//循环处理每一个文件
						do										
						{
							if(IsExit==true)
							{
								break;
							}
							bContinue=fileFind.FindNextFile();
							//获取文件名
							tFile=fileFind.GetFileName();
							if(tFile=="."||tFile==".."||fileFind.IsHidden())continue;
							//获取源文件完整路径
							strSrcFullName=m_config.m_srcFileDir[i]+"\\"+tFile;
							//获取目标文件完整路径
							strDstFullName=m_config.m_dstComputer[nDst].DstFileDir[i] +"/" +tFile;
							//传输一个相应文件
							Sleep(30);
							if(!pFtpConnection->PutFile(strSrcFullName,strDstFullName,myFlags)) 
							{
								nErrCount=nErrCount+1;
								csMessage="FtpPutFile " + strSrcFullName +" 时出现错误。";
								strErrLog=strErrLog+csMessage+"\t\r\n";
								MyMsgBox(csMessage);
							}
							else
							{
								nTransferCount=nTransferCount+1;
								csMessage="FtpPutFile " + strSrcFullName +" 完成。";
								MyMsgBox(csMessage);
							}//end if
						}while(bContinue);
						fileFind.Close();
					}//end for
				}//end if
				Sleep(30);
				//csMessage=strLocFiles+ "  结束";
				//MyMsgBox(csMessage);
				//Sleep(30);
			}//end if
		}//end for
	}//end if

	
	/////////////////////////////////////////////////////////////////////
	//如果为接收文件
	if(transferType==1)
	{
		//循环处理每一类文件
		for(i=0;i<MaxFileType;i++)						
		{	
			Sleep(100);
			if(IsExit==true)
			{
				break;
			}
			//得到传输类型
			if(m_config.m_fileType[i]==0)
			{	
				myFlags=FTP_TRANSFER_TYPE_ASCII;
			}	
			else
			{
				myFlags=FTP_TRANSFER_TYPE_BINARY;
			}

			if(m_config.m_srcFileSel[i]==1)
			{
				for(j=0;j<m_config.m_srcFileNum[j];j++)
				{				
					//获取源文件目录及其过滤条件
					strLocFiles= m_config.m_srcFileDir[i]+"\\"+m_config.m_srcFileList[i][j];
					strDstDir=m_config.m_dstComputer[nDst].DstFileDir[i];
					strFilter=m_config.m_srcFileList[i][j];
					Dir=strDstDir+"/"+m_config.m_srcFileList[i][j];
					//Sleep(30);
					//csMessage=Dir+ "  正在进行传输...";
					//MyMsgBox(csMessage);
					//根据过滤条件查找文件
					pFtpConnection->SetCurrentDirectory(strDstDir);
					bContinue=CFtp.FindFile(strFilter, 0);				
					if(!bContinue)
					{
						Sleep(30);
						csMessage="查找" + Dir+ "  失败。";
						MyMsgBox(csMessage);
						nErrCount=nErrCount+1;
						strErrLog=strErrLog+csMessage+"\t\r\n";
					}
					//若查找成功
					if(bContinue)								
					{
						//创建目录
			/*			if(CreateDirectory(m_config.m_srcFileDir[i],NULL)==0)
						{
							Sleep(30);
							nErrCount=nErrCount+1;
							csMessage="创建目录 " + m_config.m_srcFileDir[i] +" 时出现错误。";
							strErrLog=strErrLog+csMessage+"\t\r\n";
							MyMsgBox(csMessage);

						}
			*/
						//循环处理每一个文件
						do										
						{
							if(IsExit==true)
							{
								break;
							}
							bContinue=CFtp.FindNextFile();
							//获取文件名
							tFile=CFtp.GetFileName();
							strtmp=tFile;
							//对文件名进行处理，因为sun solaris所取得的文件名存在问题
							j=0;
							tFile.TrimRight();
							do
							{
								tFile.TrimLeft();
								j=tFile.Find(" ");
								tFile=tFile.Right(tFile.GetLength()-j-1);
							}while(j>=0);

							if(tFile=="."||tFile==".."||CFtp.IsHidden())continue;
							if(CFtp.IsDirectory()) continue;
							//获取源文件完整路径
							strSrcFullName=m_config.m_srcFileDir[i]+"\\"+tFile;
							//获取目标文件完整路径
							strDstFullName=strDstDir +"/" +tFile;
							//传输一个相应文件
							Sleep(30);
			/*				if(tFile==".dbin")
							{
								Sleep(30);
								continue;
							}
			*/
							if(!pFtpConnection->GetFile(strDstFullName,strSrcFullName,false,FILE_ATTRIBUTE_NORMAL,myFlags)) 
							{
								nErrCount=nErrCount+1;
								strDstFullName=strDstDir +"/" +strtmp;
								csMessage="FtpGetFile " + strDstFullName +" 时出现错误。";
								strErrLog=strErrLog+csMessage+"\t\r\n";
								MyMsgBox(csMessage);
							}
							else
							{
								nTransferCount=nTransferCount+1;
								csMessage="FtpGetFile " + strDstFullName +" 完成。";
								MyMsgBox(csMessage);
							}//end if
						}while(bContinue);
						fileFind.Close();
					}//end if
				}//end for
				Sleep(30);
				//csMessage=Dir+ "  传送结束";
				//MyMsgBox(csMessage);
				//Sleep(30);
			}//end if
		}//end for
	}//end if
	
	//对象删除
	CFtp.Close();
	if (pFtpConnection != NULL)
	{
		pFtpConnection->Close();
		Sleep(200);
	}
	delete pFtpConnection;
	if (pInetSession != NULL)
	{
		pInetSession->Close();
		Sleep(200);
	}
	delete pInetSession;
	Sleep(1000);
	return true;
}


/////////////////////////////////////////////////////////////////////
//Unix机文件传输处理函数（利用.bat文件，调用ftp.exe方式）
/////////////////////////////////////////////////////////////////////
bool TransferFileToUnixByBat(int nDst)
{
/*
	CString strTxt;					//写入autoftp.txt中的内容，每行以\r\n分隔  
	CString strBat;					//写入autoftp.bat中的内容
	CString FtpBatFullName;			//.bat文件绝对路径
	CString strtmp,strLocFiles,strDstDir,tFile,
		strSrcFullName,strDstFullName;				//路径，文件名	
	int i;
	CString DstIP,Username,Passwd;					//用户名，密码
	CString strFilter;
	int PortNum;									//端口号
	FtpBatFullName=CurExeFilePath+FtpBatName;		
	SHELLEXECUTEINFO ShExecInfo = {0};				//ShellExecuteEX参数
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = "open";
	ShExecInfo.lpFile = FtpBatFullName;	
	ShExecInfo.lpDirectory = NULL;
	if(m_config.m_ShowMode==0)
	{
		ShExecInfo.nShow = SW_HIDE;
	}
	else
	{
		ShExecInfo.nShow = SW_SHOW;
	}
	ShExecInfo.hInstApp = NULL;	
	ShExecInfo.lpParameters="";
	//判断nDst合法性
	if((nDst<0)||(nDst>(m_config.m_dstCount-1)))
	{
		csMessage="TransferFileToUnixByBat函数中目标计算机编号错误!";
		MyMsgBox(csMessage);		
		return false;
	}
	DstIP=m_config.m_dstComputer[nDst].DstIP;
	Username=m_config.m_dstComputer[nDst].Username;
	Passwd=m_config.m_dstComputer[nDst].Passwd;
	Username.TrimLeft();
	Username.TrimRight();
	if(Username.GetLength()<1)
	{
		Username="anonymous";
		Passwd="test@yang";
	}

	PortNum=m_config.m_dstComputer[nDst].PortNum;
	//autoftp.bat的串
	strBat="ftp -s:"+CurExeFilePath + FtpTxtName + "\t" +DstIP; 
	
	/////////////////////////////////////////////////////////////////////
	//如果为发送文件
	if(transferType==0)
	{
		//生成autoftp.txt文件的串
		strTxt="";
		strTxt=Username+"\r\n";
		strTxt=strTxt+Passwd+"\r\n";
		strTxt=strTxt+"prompt"+"\r\n";
		//生成asc的mput 串
		strTxt=strTxt+"asc"+"\r\n";
		strTxt=strTxt+"\r\n";
		for(i=0;i<MaxFileType;i++)						
		{
			//得到传输类型，0为ASC文件，1为二进制文件
			if((m_config.m_fileType[i]==0) && (m_config.m_srcFileSel[i]==1))
			{
				//获取源文件目录及其过滤条件
				strLocFiles= m_config.m_srcFileDir[i]+"\\"+m_config.m_srcFileList[i];
				strDstDir=m_config.m_dstComputer[nDst].DstFileDir[i];
				strTxt=strTxt+"cd"+"\t"+strDstDir+"\r\n";		
				strTxt=strTxt+"mput"+"\t"+strLocFiles+"\r\n";						
				strTxt=strTxt+"\r\n";
			}
		}
		//生成bin的mput 串
		strTxt=strTxt+"bin"+"\r\n";
		strTxt=strTxt+"\r\n";
		for(i=0;i<MaxFileType;i++)						
		{
			//得到传输类型，0为ASC文件，1为二进制文件
			if((m_config.m_fileType[i]==1) && (m_config.m_srcFileSel[i]==1))
			{
				//获取源文件目录及其过滤条件
				strLocFiles= m_config.m_srcFileDir[i]+"\\"+m_config.m_srcFileList[i];
				strDstDir=m_config.m_dstComputer[nDst].DstFileDir[i];
				strTxt=strTxt+"cd"+"\t"+strDstDir+"\r\n";		
				strTxt=strTxt+"mput"+"\t"+strLocFiles+"\r\n";						
				strTxt=strTxt+"\r\n";
			}
		}
		strTxt=strTxt+"quit"+"\r\n";
		//写入autoftp.bat和autoftp.txt文件
		if(!WriteToFtpTxt(strBat,strTxt))
		{
			csMessage=DstIP + " 传输时写入文本/bat文件时出错! ";
			MyMsgBox(csMessage);
			return false;
		}

		//运行autoftp.bat文件
//		ShellExecute(NULL, _T("open"), _T(FtpBatFullName), _T(""), NULL, SW_SHOW);
		csMessage=DstIP + " 正在传输文件，请稍候...... ";
		MyMsgBox(csMessage);
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
		Sleep(30);
		csMessage=DstIP + " 传输结束! ";
		MyMsgBox(csMessage);

	}//end if
	
	/////////////////////////////////////////////////////////////////////
	//如果为接收文件
	if(transferType==1)
	{
		//生成autoftp.txt文件的串
		strTxt="";
		strTxt=Username+"\r\n";
		strTxt=strTxt+Passwd+"\r\n";
		strTxt=strTxt+"prompt"+"\r\n";
		//生成asc的mget 串
		strTxt=strTxt+"asc"+"\r\n";
		strTxt=strTxt+"\r\n";
		for(i=0;i<MaxFileType;i++)						
		{
			//得到传输类型，0为ASC文件，1为二进制文件
			if((m_config.m_fileType[i]==0) && (m_config.m_srcFileSel[i]==1))
			{
				//获取源文件目录及其过滤条件
				strLocFiles= m_config.m_srcFileDir[i]+"\\"+m_config.m_srcFileList[i];
				strDstDir=m_config.m_dstComputer[nDst].DstFileDir[i];
				strTxt=strTxt+"cd"+"\t"+strDstDir+"\r\n";
				strTxt=strTxt+"lcd"+"\t"+m_config.m_srcFileDir[i]+"\r\n";
				strTxt=strTxt+"mget"+"\t"+m_config.m_srcFileList[i]+"\r\n";						
				strTxt=strTxt+"\r\n";
			}
		}

		//生成bin的mget 串
		strTxt=strTxt+"bin"+"\r\n";
		strTxt=strTxt+"\r\n";
		for(i=0;i<MaxFileType;i++)						
		{
			//得到传输类型，0为ASC文件，1为二进制文件
			if((m_config.m_fileType[i]==1) && (m_config.m_srcFileSel[i]==1))
			{
				//获取源文件目录及其过滤条件
				strLocFiles= m_config.m_srcFileDir[i]+"\\"+m_config.m_srcFileList[i];
				strDstDir=m_config.m_dstComputer[nDst].DstFileDir[i];
				strTxt=strTxt+"cd"+"\t"+strDstDir+"\r\n";
				strTxt=strTxt+"lcd"+"\t"+m_config.m_srcFileDir[i]+"\r\n";
				strTxt=strTxt+"mget"+"\t"+m_config.m_srcFileList[i]+"\r\n";						
				strTxt=strTxt+"\r\n";
			}
		}
		strTxt=strTxt+"quit"+"\r\n";
		//写入autoftp.bat和autoftp.txt文件
		if(!WriteToFtpTxt(strBat,strTxt))
		{
			csMessage=DstIP + " 传输时写入文本/bat文件时出错! ";
			MyMsgBox(csMessage);
			return false;
		}

		//运行autoftp.bat文件
		csMessage=DstIP + " 正在传输文件，请稍候...... ";
		MyMsgBox(csMessage);
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
		Sleep(30);
		csMessage=DstIP + " 传输结束! ";
		MyMsgBox(csMessage);

	}//end if
//	AfxMessageBox(strTxt);
	Sleep(1000);
*/
	return true;
}

/////////////////////////////////////////////////////////////////////
//按钮“开始传输”的响应函数，负责传输进程的创建
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnStarttransfer() 
{
	//确认是否发送
	if(MessageBox("传送文件将覆盖原有文件，请先关闭\r\n目标计算机的H9000系统，是否继续?",
		"请确认...",MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		transferType=0;
		TransferControl();	
	}

}
/////////////////////////////////////////////////////////////////////
//传输控制函数，根据传输类型不同，对发送和接收进行控制
/////////////////////////////////////////////////////////////////////
bool CTransferfileDlg::TransferControl()
{
	//变量定义
	int i;
	i=0;
	//第一步：将控件的值赋予m_config
	UpdateData(true);
	if(m_radioVer<0) m_radioVer=0;
	if(m_radioDriver<0) m_radioDriver=0;
	//从对化框读取得源盘和版本并赋予m_config	
	m_config.m_srcDriver=strDriverName[m_radioDriver];
	m_config.m_srcVersion=strVerName[m_radioVer];
	//UpdateLogLine(m_config.m_srcVersion);
	//UpdateLogLine(m_config.m_srcDriver);
	//从对化框读取得传输文件类型并赋予m_config
	for(i=0;i<MaxFileType;i++)
	{
		m_config.m_srcFileSel[i]=0;
	}
	if(m_check1==1)
	{
		m_config.m_srcFileSel[0]=1;
	}
	if(m_check2==1)
	{
		m_config.m_srcFileSel[1]=1;
	}
	if(m_check3==1)
	{
		m_config.m_srcFileSel[2]=1;
	}
	if(m_check4==1)
	{
		m_config.m_srcFileSel[3]=1;
	}
	if(m_check5==1)
	{
		m_config.m_srcFileSel[4]=1;
	}
	if(m_check6==1)
	{
		m_config.m_srcFileSel[5]=1;
	}
	if(m_check7==1)
	{
		m_config.m_srcFileSel[6]=1;
	}
	if(m_check8==1)
	{
		m_config.m_srcFileSel[7]=1;
	}
	//根据源盘和版本信息，得到m_config的相关路径信息
	if(!m_config.SetConfigDir())			
	{	
		AfxMessageBox("设置路径错误！");
		return false;
	}
	//判断DATABASE.OUT是否需要传输以及是否被占用
	if(m_config.m_srcFileSel[1]==1)
	{
		CString DBFilename;		
		DBFilename=m_config.m_srcFileDir[1]+"\\"+m_config.m_srcFileList[1][0];
		CFileFind fileFind;								//查找文件对象
		i=fileFind.FindFile(DBFilename);
		//若存在该文件
		if(i)
		{
			CFile myfile;
			
			if(!myfile.Open(DBFilename,CFile::modeReadWrite|CFile::shareExclusive))
			{
				AfxMessageBox("数据库生成文件"+DBFilename+"正在被占用。\r\n\r\n请先关闭本机的H9000、DETool、IPM软件，再进行传输!",MB_ICONSTOP);
				return false;//创建失败返回
			}
			myfile.Close();
		}
	}
	//第二步：创建传输进程
	HWND hWnd=GetSafeHwnd();
	AfxBeginThread(ThreadProc,hWnd,0);
	//第三步：对话框界面处理
	m_starttransfer.EnableWindow (FALSE);
	m_buttonAddDst.EnableWindow (FALSE);
	m_buttonSave.EnableWindow (FALSE);
	m_buttonGet.EnableWindow (FALSE);
	m_buttonSet.EnableWindow (FALSE);
	return true;
}

/////////////////////////////////////////////////////////////////////
//文件传输处理进程，文件传输的主函数
/////////////////////////////////////////////////////////////////////
UINT ThreadProc(LPVOID param)
{
	
	int i;									//临时变量
	int nDstCount;							//目标计算机的个数
	int nType;								//目标计算机类型
//	char buffer[10];
	threadcontroller=1;
	nTransferCount=0;
	nDstCount=m_config.m_dstCount;			//从配置信息中获取目标计算机的个数
	Sleep(30);
	strErrLog="";
	csMessage="\r\n开始进行传输...";
	MyMsgBox(csMessage);
	Sleep(1000);
	nErrCount=0;
	//如果为发送
	if(transferType==0)
	{
	for(i=0;i<nDstCount;i++)							//循环处理每个目标计算机的文件传输
	{
		nType=m_config.m_dstComputer[i].DstType;		//获取类型
		if(m_config.m_dstComputer[i].Status != 1) continue;

		csMessage = "开始进行  " + m_config.m_dstComputer[i].DstIP +"  的处理...";
		Sleep(30);
		MyMsgBox(csMessage);
		Sleep(1000);
		
		if(nType==0)									//如果为PC机，则调用PC处理函数
		{
			if(TransferFileToPC(i)==false)				//调用PC机文件传输函数
			{
				Sleep(30);
				nErrCount=nErrCount+1;
				csMessage="传输到PC机时出现故障。";
				strErrLog=strErrLog+csMessage+"\r\n";
				MyMsgBox(csMessage);
			}
		}
		else									//如果为Unix机，则调用Unix机处理函数
		{
			//如果采用CFtpConnection方式
			if(FtpTransMode==1)								
			{
				if(TransferFileToUnix(i)==false)		//调用Unix机文件传输函数
				{
					Sleep(300);
				}//end if
			}//end if
			//如果采用.bat方式进行传输
			else
			{
				if(TransferFileToUnixByBat(i)==false)		//调用Unix机文件（.bat方式）传输函数
				{
					Sleep(300);
				}//end if
			}//end else
		}//end if
	}//end for
	}//end if
	
	//如果为接收
	if(transferType==1)
	{
		nType=m_config.m_dstComputer[curDst].DstType;	//获取类型

		csMessage = "开始进行  " + m_config.m_dstComputer[curDst].DstIP +"  的处理...";
		Sleep(30);
		MyMsgBox(csMessage);
		Sleep(1000);
		
		if(nType==0)									//如果为PC机，则退出
		{
			//csMessage = m_config.m_dstComputer[i].DstIP +"为PC机，不能从PC机接收文件。";
			//Sleep(30);
			//MyMsgBox(csMessage);
			//Sleep(1000);
			threadcontroller=0;			
			return 0;
		}
		else											//如果为Unix机，则调用Unix机处理函数
		{
			//如果采用CFtpConnection方式
			if(FtpTransMode==1)								
			{
				if(TransferFileToUnix(curDst)==false)		//调用Unix机文件传输函数
				{
					Sleep(300);
				}//end if
			}//end if
			//如果采用.bat方式进行传输
			else
			{
				if(TransferFileToUnixByBat(curDst)==false)		//调用Unix机文件（.bat方式）传输函数
				{
					Sleep(300);
				}//end if
			}//end else
		}//end if
	}
	Sleep(1000);
	csMessage="\r\n所有传输操作已结束！";
	MyMsgBox(csMessage);
	Sleep(300);	
	csMessage="所有传输操作已结束！";

/*	
	if(nErrCount<1)
	{
		sprintf(buffer,"%d",nTransferCount);
		csMessage="传输文件成功。共传输 ";
		csMessage=csMessage+buffer;
		csMessage=csMessage+" 个文件。";
	}
	else
	{
		sprintf(buffer,"%d",nErrCount);
		csMessage=csMessage+"\r\n传输过程中出现共 " ;
		csMessage=csMessage+buffer;
		csMessage=csMessage+" 次故障信息。\r\n\r\n";
		csMessage=csMessage+strErrLog;
	}
*/

	csMessage="传输文件结束。";
	
	
	//恢复对话框控件
	pTransferfileDlg->m_starttransfer.EnableWindow (TRUE);
	pTransferfileDlg->m_buttonAddDst.EnableWindow (TRUE);
	pTransferfileDlg->m_buttonSave.EnableWindow (TRUE);
	pTransferfileDlg->m_buttonSet.EnableWindow (TRUE);
	AfxMessageBox(csMessage,MB_ICONINFORMATION);
	threadcontroller=0;
	Sleep(500);
	return 0;
}


/////////////////////////////////////////////////////////////////////
//添加目标计算机
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnAdddst() 
{
	// TODO: Add your command handler code here
	//m_tmpConfig=m_config;
	CDstprmDlg dlg(this);								//声明对话框对象
	int nDstCount;
	int nResponse = dlg.DoModal();						//弹出对话框
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		//增加一个目标计算机到m_config
		m_config.m_dstCount=m_config.m_dstCount+1;
		nDstCount=m_config.m_dstCount;
		m_config.m_dstComputer[nDstCount-1].DstIP=dlg.m_ip;
		if(dlg.m_radiodriver<0) dlg.m_radiodriver=0;
		//从对化框读取得源盘和版本并赋予m_config	
		m_config.m_dstComputer[nDstCount-1].DstDriver=strDriverName[dlg.m_radiodriver];
		m_config.m_dstComputer[nDstCount-1].DstType=dlg.m_radioType;
		m_config.m_dstComputer[nDstCount-1].Status=dlg.m_status;
		m_config.m_dstComputer[nDstCount-1].Username=dlg.m_username;
		m_config.m_dstComputer[nDstCount-1].Passwd=dlg.m_passwd;
		m_config.m_dstComputer[nDstCount-1].PortNum=21;
		//刷新m_config的路径相关信息
		m_config.SetConfigDir();
		//通过m_config的目标计算机信息更新IP列表的信息
		UpdateIPList();
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
}

void MyMsgBox(CString MyMsg)
{
	pTransferfileDlg->PostMessage(WM_THREADPROC,0,(LPARAM)(LPCTSTR)MyMsg);
	//pTransferfileDlg->m_editLogCtrl
}


void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////
//保存所有配置信息
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnSaveconfig() 
{
	// TODO: Add your control notification handler code here
	//将控件的值赋予m_config
	int i;
	UpdateData(true);
	if(m_radioVer<0) m_radioVer=0;
	if(m_radioDriver<0) m_radioDriver=0;
	//从对化框读取得源盘和版本并赋予m_config	
	m_config.m_srcDriver=strDriverName[m_radioDriver];
	m_config.m_srcVersion=strVerName[m_radioVer];
	//从对化框读取得传输文件类型并赋予m_config
	for(i=0;i<MaxFileType;i++)
	{
		m_config.m_srcFileSel[i]=0;
	}
	if(m_check1==1)
	{
		m_config.m_srcFileSel[0]=1;
	}
	if(m_check2==1)
	{
		m_config.m_srcFileSel[1]=1;
	}
	if(m_check3==1)
	{
		m_config.m_srcFileSel[2]=1;
	}
	if(m_check4==1)
	{
		m_config.m_srcFileSel[3]=1;
	}
	if(m_check5==1)
	{
		m_config.m_srcFileSel[4]=1;
	}
	if(m_check6==1)
	{
		m_config.m_srcFileSel[5]=1;
	}
	if(m_check7==1)
	{
		m_config.m_srcFileSel[6]=1;
	}
	if(m_check8==1)
	{
		m_config.m_srcFileSel[7]=1;
	}
	
	m_config.WriteConfigToFile();
	csMessage="保存配置信息成功!";
	MyMsgBox(csMessage);


}

/////////////////////////////////////////////////////////////////////
//响应获取文件按钮
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnStartget() 
{
	// TODO: Add your control notification handler code here
	int i,iState;
	int nItemSelected=m_listDst.GetSelectedCount();	//所选表项数
	int nItemCount=m_listDst.GetItemCount();		//表项总数
	curDst=-1;
	if(nItemSelected<1) 
	{
		AfxMessageBox("没有选择计算机，\r\n请先从计算机列表中选取。");
		return;
	}
	for(i=0;i<nItemCount;i++)
	{
		iState=m_listDst.GetItemState(i,LVIS_SELECTED);
		if(iState!=0)
		{
			curDst=m_config.m_dstCount-1-i;
			break;
		}
	}
	if(curDst<0)
	{
		AfxMessageBox("没有选择计算机，\r\n请先从计算机列表中选取。");
		return;
	}
	//如果为PC机，则不能GET
	if(m_config.m_dstComputer[curDst].DstType==0)
	{
		AfxMessageBox("不能从PC机接收数据，\r\n请从计算机列表中重新选取UNIX机。");
		return;
	}
	if(MessageBox("该操作将从选中的计算机接收文件，\r\n将覆盖原目录相应文件，是否继续?",
		"请确认...",MB_YESNO|MB_ICONWARNING)==IDYES)
	{
		transferType=1;				
		TransferControl();	
	}
}

void CTransferfileDlg::OnGetmenu() 
{
	// TODO: Add your command handler code here
	//调用OnstartGet
	OnStartget(); 
}

void CTransferfileDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//确认是否发送
	if(MessageBox("是否确认退出？","请确认...",MB_YESNO|MB_ICONQUESTION)!=IDYES)
	{
		return;
	}

	if(IsTransDB==true)
	{
		AfxMessageBox("正在传输数据库或综合计算与闭锁生成文件，请稍候再退出程序!");
		return;
	}
	//一直等待到进程退出
	
	IsExit=true;
//	while(threadcontroller==1)
//	{
//		Sleep(100);
//	}
	CDialog::OnCancel();
	DestroyWindow();
}

/////////////////////////////////////////////////////////////////////
//设置过滤条件按钮
/////////////////////////////////////////////////////////////////////
void CTransferfileDlg::OnSetFilter() 
{
	// TODO: Add your control notification handler code here
/*	CSetFilterDlg dlg(this);								//声明对话框对象
	int i;
	CString strline,strtmp;	
	//给对话框填充内容
	dlg.m_editF1=m_config.m_srcFileList[0];
	dlg.m_editF2=m_config.m_srcFileList[1];
	dlg.m_editF3=m_config.m_srcFileList[2];
	dlg.m_editF4=m_config.m_srcFileList[3];
	dlg.m_editF6=m_config.m_srcFileList[5];
	dlg.m_editF7=m_config.m_srcFileList[6];
	//语音文件特殊处理
	dlg.m_editF5=m_config.m_srcFileList[4]+" | ";
	dlg.m_editF5=dlg.m_editF5+m_config.m_srcFileList[7]+" | ";
	dlg.m_editF5=dlg.m_editF5+m_config.m_srcFileList[8]+" | ";
	dlg.m_editF5=dlg.m_editF5+m_config.m_srcFileList[9];
	int nResponse = dlg.DoModal();				//弹出对话框
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		//将修改后的信息写到m_config
		m_config.m_srcFileList[0]=dlg.m_editF1;
		m_config.m_srcFileList[1]=dlg.m_editF2;
		m_config.m_srcFileList[2]=dlg.m_editF3;
		m_config.m_srcFileList[3]=dlg.m_editF4;
		m_config.m_srcFileList[5]=dlg.m_editF6;
		m_config.m_srcFileList[6]=dlg.m_editF7;
		//语音文件特殊处理
		strline=dlg.m_editF5;
		strline.TrimLeft();
		strline.TrimRight();
		strline.Replace(" ","");
		i=strline.Find("|");
		//语音一
		if(i>=0)
		{	
			m_config.m_srcFileList[4]=strline.Left(i);
			strline=strline.Right(strline.GetLength()-i-1);
		}
		else
		{
			m_config.m_srcFileList[4]=strline;
			strline="";
		}
		//语音二
		i=strline.Find("|");		
		if(i>=0)
		{	
			m_config.m_srcFileList[7]=strline.Left(i);
			strline=strline.Right(strline.GetLength()-i-1);
		}
		else
		{
			m_config.m_srcFileList[7]=strline;;
			strline="";
		}
		//语音三
		i=strline.Find("|");		
		if(i>=0)
		{	
			m_config.m_srcFileList[8]=strline.Left(i);
			strline=strline.Right(strline.GetLength()-i-1);
		}
		else
		{
			m_config.m_srcFileList[8]=strline;;
			strline="";
		}
		//语音四
		i=strline.Find("|");		
		if(i>=0)
		{	
			m_config.m_srcFileList[9]=strline.Left(i);
		}
		else
		{
			m_config.m_srcFileList[9]=strline;
			strline="";
		}
		csMessage="设置过滤条件成功!";
		MyMsgBox(csMessage);
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
*/
}

/////////////////////////////////////////////////////////////////////
//生成autoftp.bat和autoftp.txt文件
/////////////////////////////////////////////////////////////////////
bool WriteToFtpTxt(CString mystrBat,CString mystrTxt)
{
/*
	CString strFilePath;
	CFile myfile;
	//写入autoftp.txt文件
	strFilePath=CurExeFilePath+FtpTxtName;		
	if (!myfile.Open(strFilePath,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		csMessage="打开/创建txt文件失败!";
		MyMsgBox(csMessage);
		return false;
	}
	myfile.Write(mystrTxt,mystrTxt.GetLength());
	myfile.Close();
	//写入autoftp.bat文件
	strFilePath=CurExeFilePath+FtpBatName;		
	if (!myfile.Open(strFilePath,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		csMessage="打开/创建txt文件失败!";
		MyMsgBox(csMessage);
		return false;
	}
	myfile.Write(mystrBat,mystrBat.GetLength());
	myfile.Close();
*/
	return true;
}

void CTransferfileDlg::OnRadiodriverf() 
{
	// TODO: Add your control notification handler code here
	csMessage="本地磁盘设置为F:\\ ,  本地路径为F:\\Users\\ems\\";
	MyMsgBox(csMessage);
	
}

void CTransferfileDlg::OnRadiodrivere() 
{
	// TODO: Add your control notification handler code here
/*
	if(MessageBox("您是否确定将本地磁盘设置为 E 盘 ? ","请确认...",MB_YESNO|MB_ICONQUESTION)!=IDYES)
	{
		m_radioDriver=2;
		UpdateData(false);
		return;
	}
*/
	csMessage="本地磁盘设置为E:\\ ,  本地路径为E:\\Users\\ems\\";
	MyMsgBox(csMessage);
	UpdateData(true);
	m_config.m_srcDriver=strDriverName[m_radioDriver];
	m_config.m_srcVersion=strVerName[m_radioVer];
	if(!m_config.SetConfigDir())			
	{	
		AfxMessageBox("设置路径错误！");
	}
	return;
}

void CTransferfileDlg::OnRadiodriverd() 
{
	// TODO: Add your control notification handler code here
/*	if(MessageBox("您是否确定将本地磁盘设置为 D 盘 ? ","请确认...",MB_YESNO|MB_ICONQUESTION)!=IDYES)
	{
		m_radioDriver=3;
		UpdateData(false);
		return;
	}
*/
	csMessage="本地磁盘设置为D:\\ ,  本地路径为D:\\Users\\ems\\";
	MyMsgBox(csMessage);
	UpdateData(true);
	m_config.m_srcDriver=strDriverName[m_radioDriver];
	m_config.m_srcVersion=strVerName[m_radioVer];	
	if(!m_config.SetConfigDir())			
	{	
		AfxMessageBox("设置路径错误！");
	}
	return;	
}

void CTransferfileDlg::OnClickListdst(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CTransferfileDlg::OnSetfilter1() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(0)==true)
	{

	}
}

bool CTransferfileDlg::ShowFileDlg(int DlgID)
{
	int i,j;
	CString strFile;						//所选文件列表
	char buff[10];
	CString strFilename,strPathname,strTmp;				
	CFileDialog dlgFile(TRUE);				//选择文件对话框 
	char* buf = new char[MAXFILE];
	dlgFile.m_ofn.lStructSize=76;			//88为Wind2000的风格，76为Win98的简单风格	
	dlgFile.m_ofn.lpstrFile = buf;
	dlgFile.m_ofn.lpstrFile[0] = NULL; 
	dlgFile.m_ofn.lpstrDefExt=".rtf";
	if(DlgID==0)
	{
		dlgFile.m_ofn.lpstrFilter= "数据库文本文件(*.*)\0*.*";
	}
	if(DlgID==1)
	{
		dlgFile.m_ofn.lpstrFilter= "数据库生成文件(*.OUT)\0*.OUT";
	}
	if(DlgID==2)
	{
		dlgFile.m_ofn.lpstrFilter= "闭锁条件及综合计算文本(*.txt)\0*.txt";
	}
	if(DlgID==3)
	{
		dlgFile.m_ofn.lpstrFilter= "闭锁条件及综合计算生成文件(*.dat)\0*.dat";
	}
	if(DlgID==4)
	{
		dlgFile.m_ofn.lpstrFilter= "画面文件(*.*)\0*.*";
	}
	if(DlgID==5)
	{
		dlgFile.m_ofn.lpstrFilter= "语音文件(*.wav)\0*.wav";
	}
	if(DlgID==6)
	{
		dlgFile.m_ofn.lpstrFilter= "事故追忆文件(*.FAIL)\0*.FAIL";
	}
	if(DlgID==7)
	{
		dlgFile.m_ofn.lpstrFilter= "项目配置文件(*.txt)\0*.txt";
	}
	dlgFile.m_ofn.Flags|=OFN_PATHMUSTEXIST|OFN_CREATEPROMPT|OFN_ENABLESIZING|OFN_EXTENSIONDIFFERENT|OFN_ALLOWMULTISELECT;
	dlgFile.m_ofn.nMaxFile = MAXFILE; 
	dlgFile.m_ofn.lpstrInitialDir=m_config.m_srcFileDir[DlgID];
	//strFile="注意：一次选择的文件个数不能超过1000个!\r\n";
	//UpdateLogLine(strFile);
	if(dlgFile.DoModal() == IDOK)        
	{		
		j=0;
		POSITION pos = dlgFile.GetStartPosition();
		while (pos != NULL)
		{
			strPathname=dlgFile.GetNextPathName(pos); 
			strPathname.TrimRight();
			i=strPathname.ReverseFind('\\');			
			m_config.m_srcFileList[DlgID][j]=strPathname.Mid(i+1); 
			j++;
			strFile+=strPathname.Mid(i+1);
			strFile+="\r\n";
		}
		sprintf(buff,"%d",j);
		strFile+="共选择文件个数：";
		strFile+=buff;
		strFile+="\r\n";
		m_config.m_srcFileNum[DlgID]= j;
	}

	UpdateLogLine(strFile);
	return true;
}

void CTransferfileDlg::OnSetfilter2() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(1)==true)
	{

	}	
}

void CTransferfileDlg::OnSetfilter3() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(2)==true)
	{

	}	
}

void CTransferfileDlg::OnSetfilter4() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(3)==true)
	{

	}	
}

void CTransferfileDlg::OnSetfilter5() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(4)==true)
	{

	}	
}

void CTransferfileDlg::OnSetfilter6() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(5)==true)
	{

	}	
}

void CTransferfileDlg::OnSetfilter7() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(6)==true)
	{

	}	
}

void CTransferfileDlg::OnRadiodriverc() 
{
	// TODO: Add your control notification handler code here
	csMessage="本地磁盘设置为c:\\ ,  本地路径为c:\\Users\\ems\\";
	MyMsgBox(csMessage);
	UpdateData(true);
	m_config.m_srcDriver=strDriverName[m_radioDriver];
	m_config.m_srcVersion=strVerName[m_radioVer];
	if(!m_config.SetConfigDir())			
	{	
		AfxMessageBox("设置路径错误！");
	}
}

void CTransferfileDlg::OnRadiover2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//根据源盘和版本信息，得到m_config的相关路径信息
	m_config.m_srcDriver=strDriverName[m_radioDriver];
	m_config.m_srcVersion=strVerName[m_radioVer];
	if(!m_config.SetConfigDir())			
	{	
		AfxMessageBox("设置路径错误！");
	}
}

void CTransferfileDlg::OnRadiover3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//根据源盘和版本信息，得到m_config的相关路径信息
	m_config.m_srcDriver=strDriverName[m_radioDriver];
	m_config.m_srcVersion=strVerName[m_radioVer];
	if(!m_config.SetConfigDir())			
	{	
		AfxMessageBox("设置路径错误！");
	}
}

void CTransferfileDlg::OnEnablemenu() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	int i,iState;
	int nDst=0;										//选中的目标计算机编号
	int nItemSelected=m_listDst.GetSelectedCount();	//所选表项数
	int nItemCount=m_listDst.GetItemCount();		//表项总数
	if(nItemSelected<1) return;
	//获得所选计算机编号
	for(i=0;i<nItemCount;i++)
	{
		iState=m_listDst.GetItemState(i,LVIS_SELECTED);
		if(iState!=0)
		{
			nDst=m_config.m_dstCount-1-i;
			break;
		}
	}
	if(m_config.m_dstComputer[nDst].Status==0)
		m_config.m_dstComputer[nDst].Status= 1;
	else
		m_config.m_dstComputer[nDst].Status= 0;
	UpdateIPList();
	return;	
}



void CTransferfileDlg::OnSetfilter8() 
{
	// TODO: Add your control notification handler code here
	if(ShowFileDlg(7)==true)
	{

	}	
}

void CTransferfileDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}
