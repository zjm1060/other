// SelectDev.cpp : implementation file
//

#include "stdafx.h"
#include "teldef.h"
#include "SelectDev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SelectDev dialog


SelectDev::SelectDev(CWnd* pParent /*=NULL*/)
	: CDialog(SelectDev::IDD, pParent)
{
	//{{AFX_DATA_INIT(SelectDev)
	m_check011 = FALSE;
	m_check0110 = FALSE;
	m_check0111 = FALSE;
	m_check0112 = FALSE;
	m_check0113 = FALSE;
	m_check0114 = FALSE;
	m_check0115 = FALSE;
	m_check0116 = FALSE;
	m_check0117 = FALSE;
	m_check0118 = FALSE;
	m_check012 = FALSE;
	m_check013 = FALSE;
	m_check014 = FALSE;
	m_check015 = FALSE;
	m_check016 = FALSE;
	m_check017 = FALSE;
	m_check019 = FALSE;
	m_edit_11 = _T("");
	m_edit_110 = _T("");
	m_edit_111 = _T("");
	m_edit_112 = _T("");
	m_edit_113 = _T("");
	m_edit_114 = _T("");
	m_edit_115 = _T("");
	m_edit_116 = _T("");
	m_edit_117 = _T("");
	m_edit_118 = _T("");
	m_edit_119 = _T("");
	m_edit_12 = _T("");
	m_edit_13 = _T("");
	m_edit_14 = _T("");
	m_edit_15 = _T("");
	m_edit_16 = _T("");
	m_edit_17 = _T("");
	m_edit_18 = _T("");
	m_edit_19 = _T("");
	m_check018 = FALSE;
	m_check0119 = FALSE;
	//}}AFX_DATA_INIT
}


void SelectDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SelectDev)
	DDX_Control(pDX, ID_SMSNUM_SAVE, m_seldev_save);
	DDX_Control(pDX, ID_SMSNUM_GET, m_seldev_get);
	DDX_Check(pDX, IDC_CHECK011, m_check011);
	DDX_Check(pDX, IDC_CHECK0110, m_check0110);
	DDX_Check(pDX, IDC_CHECK0111, m_check0111);
	DDX_Check(pDX, IDC_CHECK0112, m_check0112);
	DDX_Check(pDX, IDC_CHECK0113, m_check0113);
	DDX_Check(pDX, IDC_CHECK0114, m_check0114);
	DDX_Check(pDX, IDC_CHECK0115, m_check0115);
	DDX_Check(pDX, IDC_CHECK0116, m_check0116);
	DDX_Check(pDX, IDC_CHECK0117, m_check0117);
	DDX_Check(pDX, IDC_CHECK0118, m_check0118);
	DDX_Check(pDX, IDC_CHECK012, m_check012);
	DDX_Check(pDX, IDC_CHECK013, m_check013);
	DDX_Check(pDX, IDC_CHECK014, m_check014);
	DDX_Check(pDX, IDC_CHECK015, m_check015);
	DDX_Check(pDX, IDC_CHECK016, m_check016);
	DDX_Check(pDX, IDC_CHECK017, m_check017);
	DDX_Check(pDX, IDC_CHECK019, m_check019);
	DDX_Text(pDX, IDC_SMSDEV_EDIT11, m_edit_11);
	DDX_Text(pDX, IDC_SMSDEV_EDIT110, m_edit_110);
	DDX_Text(pDX, IDC_SMSDEV_EDIT111, m_edit_111);
	DDX_Text(pDX, IDC_SMSDEV_EDIT112, m_edit_112);
	DDX_Text(pDX, IDC_SMSDEV_EDIT113, m_edit_113);
	DDX_Text(pDX, IDC_SMSDEV_EDIT114, m_edit_114);
	DDX_Text(pDX, IDC_SMSDEV_EDIT115, m_edit_115);
	DDX_Text(pDX, IDC_SMSDEV_EDIT116, m_edit_116);
	DDX_Text(pDX, IDC_SMSDEV_EDIT117, m_edit_117);
	DDX_Text(pDX, IDC_SMSDEV_EDIT118, m_edit_118);
	DDX_Text(pDX, IDC_SMSDEV_EDIT119, m_edit_119);
	DDX_Text(pDX, IDC_SMSDEV_EDIT12, m_edit_12);
	DDX_Text(pDX, IDC_SMSDEV_EDIT13, m_edit_13);
	DDX_Text(pDX, IDC_SMSDEV_EDIT14, m_edit_14);
	DDX_Text(pDX, IDC_SMSDEV_EDIT15, m_edit_15);
	DDX_Text(pDX, IDC_SMSDEV_EDIT16, m_edit_16);
	DDX_Text(pDX, IDC_SMSDEV_EDIT17, m_edit_17);
	DDX_Text(pDX, IDC_SMSDEV_EDIT18, m_edit_18);
	DDX_Text(pDX, IDC_SMSDEV_EDIT19, m_edit_19);
	DDX_Check(pDX, IDC_CHECK018, m_check018);
	DDX_Check(pDX, IDC_CHECK0119, m_check0119);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SelectDev, CDialog)
	//{{AFX_MSG_MAP(SelectDev)
	ON_BN_CLICKED(ID_SMSNUM_SAVE, OnSmsdevSave)
	ON_BN_CLICKED(ID_SMSNUM_GET, OnSmsdevGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SelectDev message handlers
extern CString	SelectDevice[19][1];
extern int     SelectPre[19][1];

void SelectDev::OnSmsdevSave() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗 ?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{	  
		UpdateData(TRUE); 
		GetDataForSave();
	    UpdateData(FALSE);  			  
		SaveSelectDev();	
		//MessageBox("使用新定义文件,需要重新启动EMS系统 !!!","警告!!!",MB_ICONEXCLAMATION  );
	}	
}

void SelectDev::OnSmsdevGet() 
{
	// TODO: Add your control notification handler code here
	GetSelectDev();
	UpdateData(TRUE);  
	GetDataForDsp();
	UpdateData(FALSE); //刷新窗口中数据	
	
}

void SelectDev::GetDataForDsp()
{  
	if(SelectDevice[0][0]=="no") m_edit_11 = ""; else m_edit_11 = SelectDevice[0][0];
	m_check011 = SelectPre[0][0];
	if(SelectDevice[1][0]=="no") m_edit_12 = ""; else m_edit_12 = SelectDevice[1][0];
	m_check012 = SelectPre[1][0];
	if(SelectDevice[2][0]=="no") m_edit_13 = ""; else m_edit_13 = SelectDevice[2][0];
	m_check013 = SelectPre[2][0];
	if(SelectDevice[3][0]=="no") m_edit_14 = ""; else m_edit_14 = SelectDevice[3][0];
	m_check014 = SelectPre[3][0];
	if(SelectDevice[4][0]=="no") m_edit_15 = ""; else m_edit_15 = SelectDevice[4][0];
	m_check015 = SelectPre[4][0];
	if(SelectDevice[5][0]=="no") m_edit_16 = ""; else m_edit_16 = SelectDevice[5][0];
	m_check016 = SelectPre[5][0];
	if(SelectDevice[6][0]=="no") m_edit_17 = ""; else m_edit_17 = SelectDevice[6][0];
	m_check017 = SelectPre[6][0];
	if(SelectDevice[7][0]=="no") m_edit_18 = ""; else m_edit_18 = SelectDevice[7][0];
	m_check018 = SelectPre[7][0];
	if(SelectDevice[8][0]=="no") m_edit_19 = ""; else m_edit_19 = SelectDevice[8][0];
	m_check019 = SelectPre[8][0];
	if(SelectDevice[9][0]=="no") m_edit_110 = ""; else m_edit_110 = SelectDevice[9][0];
	m_check0110 = SelectPre[9][0];
	if(SelectDevice[10][0]=="no") m_edit_111 = ""; else m_edit_111 = SelectDevice[10][0];
	m_check0111 = SelectPre[10][0];
	if(SelectDevice[11][0]=="no") m_edit_112 = ""; else m_edit_112 = SelectDevice[11][0];
	m_check0112 = SelectPre[11][0];
	if(SelectDevice[12][0]=="no") m_edit_113 = ""; else m_edit_113 = SelectDevice[12][0];
	m_check0113 = SelectPre[12][0];
	if(SelectDevice[13][0]=="no") m_edit_114 = ""; else m_edit_114 = SelectDevice[13][0];
	m_check0114 = SelectPre[13][0];
	if(SelectDevice[14][0]=="no") m_edit_115 = ""; else m_edit_115 = SelectDevice[14][0];
	m_check0115 = SelectPre[14][0];
	if(SelectDevice[15][0]=="no") m_edit_116 = ""; else m_edit_116 = SelectDevice[15][0];
	m_check0116 = SelectPre[15][0];
	if(SelectDevice[16][0]=="no") m_edit_117 = ""; else m_edit_117 = SelectDevice[16][0];
	m_check0117 = SelectPre[16][0];
	if(SelectDevice[17][0]=="no") m_edit_118 = ""; else m_edit_118 = SelectDevice[17][0];
	m_check0118 = SelectPre[17][0];
	if(SelectDevice[18][0]=="no") m_edit_119 = ""; else m_edit_119 = SelectDevice[18][0];
	m_check0119 = SelectPre[18][0];
}
void SelectDev::GetDataForSave()
{
	if(m_edit_11=="") SelectDevice[0][0]="no"; else SelectDevice[0][0] = m_edit_11;
    SelectPre[0][0] = m_check011;
	if(m_edit_12=="") SelectDevice[1][0]="no"; else SelectDevice[1][0] = m_edit_12;
    SelectPre[1][0] = m_check012;
	if(m_edit_13=="") SelectDevice[2][0]="no"; else SelectDevice[2][0] = m_edit_13;
    SelectPre[2][0] = m_check013;
	if(m_edit_14=="") SelectDevice[3][0]="no"; else SelectDevice[3][0] = m_edit_14;
    SelectPre[3][0] = m_check014;
	if(m_edit_15=="") SelectDevice[4][0]="no"; else SelectDevice[4][0] = m_edit_15;
    SelectPre[4][0] = m_check015;
	if(m_edit_16=="") SelectDevice[5][0]="no"; else SelectDevice[5][0] = m_edit_16;
    SelectPre[5][0] = m_check016;
	if(m_edit_17=="") SelectDevice[6][0]="no"; else SelectDevice[6][0] = m_edit_17;
    SelectPre[6][0] = m_check017;
	if(m_edit_18=="") SelectDevice[7][0]="no"; else SelectDevice[7][0] = m_edit_18;
    SelectPre[7][0] = m_check018;
	if(m_edit_19=="") SelectDevice[8][0]="no"; else SelectDevice[8][0] = m_edit_19;
    SelectPre[8][0] = m_check019;
	if(m_edit_110=="") SelectDevice[9][0]="no"; else SelectDevice[9][0] = m_edit_110;
    SelectPre[9][0] = m_check0110;
	if(m_edit_111=="") SelectDevice[10][0]="no"; else SelectDevice[10][0] = m_edit_111;
    SelectPre[10][0] = m_check0111;
	if(m_edit_112=="") SelectDevice[11][0]="no"; else SelectDevice[11][0] = m_edit_112;
    SelectPre[11][0] = m_check0112;
	if(m_edit_113=="") SelectDevice[12][0]="no"; else SelectDevice[12][0] = m_edit_113;
    SelectPre[12][0] = m_check0113;
	if(m_edit_114=="") SelectDevice[13][0]="no"; else SelectDevice[13][0] = m_edit_114;
    SelectPre[13][0] = m_check0114;
	if(m_edit_115=="") SelectDevice[14][0]="no"; else SelectDevice[14][0] = m_edit_115;
    SelectPre[14][0] = m_check0115;
	if(m_edit_116=="") SelectDevice[15][0]="no"; else SelectDevice[15][0] = m_edit_116;
    SelectPre[15][0] = m_check0116;
	if(m_edit_117=="") SelectDevice[16][0]="no"; else SelectDevice[16][0] = m_edit_117;
    SelectPre[16][0] = m_check0117;
	if(m_edit_118=="") SelectDevice[17][0]="no"; else SelectDevice[17][0] = m_edit_118;
    SelectPre[17][0] = m_check0118;
	if(m_edit_119=="") SelectDevice[18][0]="no"; else SelectDevice[18][0] = m_edit_119;
    SelectPre[18][0] = m_check0119;
}

BOOL SelectDev::OnInitDialog() 
{
	CDialog::OnInitDialog();
	OnSmsdevGet() ;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
