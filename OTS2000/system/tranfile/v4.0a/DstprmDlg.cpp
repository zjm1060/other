// DstprmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "transferfile.h"
#include "DstprmDlg.h"
#include "TransferConfig.h"
#include "transferfileDlg.h"
#include "DstSelDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDstprmDlg dialog

CTransferConfig m_pconfig;

CDstprmDlg::CDstprmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDstprmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDstprmDlg)
	m_status = TRUE;
	m_username = _T("ems");
	m_radiodriver = 2;
	m_radioType = 1;
	m_ip = _T("");
	m_passwd="H9000ems";
	m_pwdConf = _T("H9000ems");
	//}}AFX_DATA_INIT
}


void CDstprmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDstprmDlg)
	DDX_Check(pDX, IDC_CHECKSTATUS, m_status);
	DDX_Text(pDX, IDC_EDITPWD, m_passwd);
	DDX_Text(pDX, IDC_EDITUSERNAME, m_username);
	DDX_Radio(pDX, IDC_RADIODRIVERA, m_radiodriver);
	DDX_Radio(pDX, IDC_RADIOTYPE0, m_radioType);
	DDX_Text(pDX, IDC_EDITIP, m_ip);
	DDX_Text(pDX, IDC_EDITPWDCONF, m_pwdConf);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDstprmDlg, CDialog)
	//{{AFX_MSG_MAP(CDstprmDlg)
	ON_BN_CLICKED(IDCONFIRM, OnConfirm)
	ON_BN_CLICKED(IDC_GETFROMFILE, OnGetfromfile)
	ON_BN_CLICKED(IDC_RADIOTYPE0, OnRadiotype0)
	ON_BN_CLICKED(IDC_RADIOTYPE1, OnRadiotype1)
	ON_BN_CLICKED(IDMODIFY, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDstprmDlg message handlers

void CDstprmDlg::OnConfirm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_ip.TrimLeft();
	m_ip.TrimRight();
	//如果没有输入IP，则提示
	if(m_ip.GetLength()<1)
	{
		AfxMessageBox("IP/机器名不能为空！");
		return;
	}
	if(m_passwd != m_pwdConf)
	{
		AfxMessageBox("密码和确认密码不一致！");
		return;
	}
	CDialog::OnOK();
}

//从系统的host文件读取计算机列表信息
void CDstprmDlg::OnGetfromfile() 
{
	// TODO: Add your control notification handler code here
	CDstSelDlg dlg;							//声明对话框对象
//	CString strName;
	int nResponse = dlg.DoModal();				//弹出对话框
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
		m_ip=dlg.m_curName;
		UpdateData(false);
		
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}		
}

void CDstprmDlg::OnRadiotype0() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("只有在可以映射对方网络驱动器前提下,往Windows传输方能成功");
	m_username="Administrator";
	m_passwd="H9000ems";
	m_pwdConf="H9000ems";
	m_radioType=0;
	UpdateData(false);
}

void CDstprmDlg::OnRadiotype1() 
{
	// TODO: Add your control notification handler code here
	m_username="ems";
	m_passwd="H9000ems";
	m_pwdConf="H9000ems";
	m_radioType=1;
	UpdateData(false);	
}

//修改目标计算机属性
void CDstprmDlg::OnModify() 
{
	// TODO: Add your control notification handler code here
	
}
