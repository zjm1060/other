// SetFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "transferfile.h"
#include "SetFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetFilterDlg dialog


CSetFilterDlg::CSetFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetFilterDlg)
	m_editF1 = _T("");
	m_editF2 = _T("");
	m_editF3 = _T("");
	m_editF4 = _T("");
	m_editF5 = _T("");
	m_editF6 = _T("");
	m_editF7 = _T("");
	//}}AFX_DATA_INIT
}


void CSetFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetFilterDlg)
	DDX_Text(pDX, IDC_EDITF1, m_editF1);
	DDX_Text(pDX, IDC_EDITF2, m_editF2);
	DDX_Text(pDX, IDC_EDITF3, m_editF3);
	DDX_Text(pDX, IDC_EDITF4, m_editF4);
	DDX_Text(pDX, IDC_EDITF5, m_editF5);
	DDX_Text(pDX, IDC_EDITF6, m_editF6);
	DDX_Text(pDX, IDC_EDITF7, m_editF7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CSetFilterDlg)
	ON_BN_CLICKED(IDC_SETDEFAULT, OnSetdefault)
	ON_BN_CLICKED(IDCONFIRMSET, OnConfirmset)
	ON_BN_CLICKED(IDCANCELSET, OnCancelset)
	ON_COMMAND(ID_ENABLEALLMENU, OnEnableallmenu)
	ON_COMMAND(ID_DISENABLEALLMENU, OnDisenableallmenu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetFilterDlg message handlers
void CSetFilterDlg::OnSetdefault() 
{
	// TODO: Add your control notification handler code here
	//初始化源文件过滤参数

	m_editF1="*.*";
	m_editF2="DATABASE.OUT";
	m_editF3="*.txt";
	m_editF4="*.dat";
	m_editF5="*.DBIN | *.dbin | *.drbin | *.mrbin";
	m_editF6="*.wav";
	m_editF7="*.FAIL";
	UpdateData(false);
}

void CSetFilterDlg::OnConfirmset() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_editF1.TrimLeft();
	m_editF1.TrimRight();
	m_editF2.TrimLeft();
	m_editF2.TrimRight();
	m_editF3.TrimLeft();
	m_editF3.TrimRight();
	m_editF4.TrimLeft();
	m_editF4.TrimRight();
	m_editF5.TrimLeft();
	m_editF5.TrimRight();
	m_editF6.TrimLeft();
	m_editF6.TrimRight();
	m_editF7.TrimLeft();
	m_editF7.TrimRight();
	//如果没有输入过滤条件，则提示
	if(m_editF1.GetLength()<1)
	{
		AfxMessageBox("过滤条件不能为空！");
		return;
	}
	if(m_editF2.GetLength()<1)
	{
		AfxMessageBox("过滤条件不能为空！");
		return;
	}	
	if(m_editF3.GetLength()<1)
	{
		AfxMessageBox("过滤条件不能为空！");
		return;
	}	
	if(m_editF4.GetLength()<1)
	{
		AfxMessageBox("过滤条件不能为空！");
		return;
	}	
	if(m_editF5.GetLength()<1)
	{
		AfxMessageBox("过滤条件不能为空！");
		return;
	}	
	if(m_editF6.GetLength()<1)
	{
		AfxMessageBox("过滤条件不能为空！");
		return;
	}	
	if(m_editF7.GetLength()<1)
	{
		AfxMessageBox("过滤条件不能为空！");
		return;
	}		
	CDialog::OnOK();		
}

void CSetFilterDlg::OnCancelset() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();		
}


void CSetFilterDlg::OnEnableallmenu() 
{
	// TODO: Add your command handler code here
	
}

void CSetFilterDlg::OnDisenableallmenu() 
{
	// TODO: Add your command handler code here
	
}
