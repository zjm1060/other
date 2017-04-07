// SmsNum.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/SmsNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsNum dialog


CSmsNum::CSmsNum(CWnd* pParent /*=NULL*/)
	: CDialog(CSmsNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmsNum)
	m_check011 = FALSE;
	m_check021 = FALSE;
	m_check031 = FALSE;
	m_check041 = FALSE;
	m_check051 = FALSE;
	m_check061 = FALSE;
	m_check071 = FALSE;
	m_check081 = FALSE;
	m_check091 = FALSE;
	m_check101 = FALSE;
	m_edit_page = _T("");
	m_edit_11 = _T("");
	m_edit_110 = _T("");
	m_edit_12 = _T("");
	m_edit_13 = _T("");
	m_edit_14 = _T("");
	m_edit_15 = _T("");
	m_edit_16 = _T("");
	m_edit_17 = _T("");
	m_edit_18 = _T("");
	m_edit_19 = _T("");
	m_edit_21 = _T("");
	m_edit_210 = _T("");
	m_edit_22 = _T("");
	m_edit_23 = _T("");
	m_edit_24 = _T("");
	m_edit_25 = _T("");
	m_edit_26 = _T("");
	m_edit_27 = _T("");
	m_edit_28 = _T("");
	m_edit_29 = _T("");
	m_check012 = FALSE;
	m_check022 = FALSE;
	m_check032 = FALSE;
	m_check042 = FALSE;
	m_check052 = FALSE;
	m_check062 = FALSE;
	m_check072 = FALSE;
	m_check082 = FALSE;
	m_check092 = FALSE;
	m_check102 = FALSE;
	//}}AFX_DATA_INIT
}


void CSmsNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmsNum)
	DDX_Control(pDX, IDC_BUTTON_1_ON2, m_num_on2);
	DDX_Control(pDX, IDC_BUTTON_1_OFF2, m_num_off2);
	DDX_Control(pDX, ID_SMSNUM_SAVE, m_smsnum_save);
	DDX_Control(pDX, ID_SMSNUM_GET, m_smsnum_get);
	DDX_Control(pDX, IDC_BUTTON_1_ON, m_num_on);
	DDX_Control(pDX, IDC_BUTTON_1_OFF, m_num_off);
	DDX_Control(pDX, IDC_SMSNUM_SCROLLBAR, m_smsnum_scrollbar);
	DDX_Check(pDX, IDC_CHECK011, m_check011);
	DDX_Check(pDX, IDC_CHECK021, m_check021);
	DDX_Check(pDX, IDC_CHECK031, m_check031);
	DDX_Check(pDX, IDC_CHECK041, m_check041);
	DDX_Check(pDX, IDC_CHECK051, m_check051);
	DDX_Check(pDX, IDC_CHECK061, m_check061);
	DDX_Check(pDX, IDC_CHECK071, m_check071);
	DDX_Check(pDX, IDC_CHECK081, m_check081);
	DDX_Check(pDX, IDC_CHECK091, m_check091);
	DDX_Check(pDX, IDC_CHECK101, m_check101);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_edit_page);
	DDX_Text(pDX, IDC_SMSNUM_EDIT11, m_edit_11);
	DDX_Text(pDX, IDC_SMSNUM_EDIT110, m_edit_110);
	DDX_Text(pDX, IDC_SMSNUM_EDIT12, m_edit_12);
	DDX_Text(pDX, IDC_SMSNUM_EDIT13, m_edit_13);
	DDX_Text(pDX, IDC_SMSNUM_EDIT14, m_edit_14);
	DDX_Text(pDX, IDC_SMSNUM_EDIT15, m_edit_15);
	DDX_Text(pDX, IDC_SMSNUM_EDIT16, m_edit_16);
	DDX_Text(pDX, IDC_SMSNUM_EDIT17, m_edit_17);
	DDX_Text(pDX, IDC_SMSNUM_EDIT18, m_edit_18);
	DDX_Text(pDX, IDC_SMSNUM_EDIT19, m_edit_19);
	DDX_Text(pDX, IDC_SMSNUM_EDIT21, m_edit_21);
	DDX_Text(pDX, IDC_SMSNUM_EDIT210, m_edit_210);
	DDX_Text(pDX, IDC_SMSNUM_EDIT22, m_edit_22);
	DDX_Text(pDX, IDC_SMSNUM_EDIT23, m_edit_23);
	DDX_Text(pDX, IDC_SMSNUM_EDIT24, m_edit_24);
	DDX_Text(pDX, IDC_SMSNUM_EDIT25, m_edit_25);
	DDX_Text(pDX, IDC_SMSNUM_EDIT26, m_edit_26);
	DDX_Text(pDX, IDC_SMSNUM_EDIT27, m_edit_27);
	DDX_Text(pDX, IDC_SMSNUM_EDIT28, m_edit_28);
	DDX_Text(pDX, IDC_SMSNUM_EDIT29, m_edit_29);
	DDX_Check(pDX, IDC_CHECK012, m_check012);
	DDX_Check(pDX, IDC_CHECK022, m_check022);
	DDX_Check(pDX, IDC_CHECK032, m_check032);
	DDX_Check(pDX, IDC_CHECK042, m_check042);
	DDX_Check(pDX, IDC_CHECK052, m_check052);
	DDX_Check(pDX, IDC_CHECK062, m_check062);
	DDX_Check(pDX, IDC_CHECK072, m_check072);
	DDX_Check(pDX, IDC_CHECK082, m_check082);
	DDX_Check(pDX, IDC_CHECK092, m_check092);
	DDX_Check(pDX, IDC_CHECK102, m_check102);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsNum, CDialog)
	//{{AFX_MSG_MAP(CSmsNum)
	ON_BN_CLICKED(ID_SMSNUM_SAVE, OnSmsnumSave)
	ON_BN_CLICKED(ID_SMSNUM_GET, OnSmsnumGet)
	ON_BN_CLICKED(ID_SMSNUM_DOWNPAGE, OnSmsnumDownpage)
	ON_BN_CLICKED(ID_SMSNUM_UPPAGE, OnSmsnumUppage)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_1_ON, OnButton1On)
	ON_BN_CLICKED(IDC_BUTTON_1_OFF, OnButton1Off)
	ON_BN_CLICKED(IDC_BUTTON_1_ON2, OnButton1On2)
	ON_BN_CLICKED(IDC_BUTTON_1_OFF2, OnButton1Off2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsNum message handlers
extern int 	smsnumPageNum,	smsnumCurrentPage;
extern CString	SmsName[MAX_SMSNUM_NUM][1];
extern CString	SmsNum[MAX_SMSNUM_NUM][1];
extern int     SmsPre[MAX_SMSNUM_NUM][1];
extern int     SmsBytimePre[MAX_SMSNUM_NUM][1];

void CSmsNum::OnSmsnumSave() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{	  
		UpdateData(TRUE); 
		GetDataForSave();
	    UpdateData(FALSE);  			  
		SaveSmsNum();	
		//MessageBox("使用新定义文件,需要重新启动EMS系统 !!!","警告!!!",MB_ICONEXCLAMATION  );
	}		
}

void CSmsNum::GetDataForSave()
{
    int			i;
	i=(smsnumCurrentPage-1)*10+0 ;
	if(m_edit_11=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_11;
	if(m_edit_21=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_21;
	SmsPre[i][0] = m_check011;
    SmsBytimePre[i][0] = m_check012;

    i=(smsnumCurrentPage-1)*10+1 ;
	if(m_edit_12=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_12;
	if(m_edit_22=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_22;
	SmsPre[i][0] = m_check021;
    SmsBytimePre[i][0] = m_check022;

    i=(smsnumCurrentPage-1)*10+2 ;
	if(m_edit_13=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_13;
	if(m_edit_23=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_23;
	SmsPre[i][0] = m_check031;
    SmsBytimePre[i][0] = m_check032;

    i=(smsnumCurrentPage-1)*10+3 ;
	if(m_edit_14=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_14;
	if(m_edit_24=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_24;
	SmsPre[i][0] = m_check041;
    SmsBytimePre[i][0] = m_check042;

    i=(smsnumCurrentPage-1)*10+4 ;
	if(m_edit_15=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_15;
	if(m_edit_25=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_25;
	SmsPre[i][0] = m_check051;
    SmsBytimePre[i][0] = m_check052;

    i=(smsnumCurrentPage-1)*10+5 ;
	if(m_edit_16=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_16;
	if(m_edit_26=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_26;
	SmsPre[i][0] = m_check061;
    SmsBytimePre[i][0] = m_check062;

    i=(smsnumCurrentPage-1)*10+6 ;
	if(m_edit_17=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_17;
	if(m_edit_27=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_27;
	SmsPre[i][0] = m_check071;
    SmsBytimePre[i][0] = m_check072;

    i=(smsnumCurrentPage-1)*10+7 ;
	if(m_edit_18=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_18;
	if(m_edit_28=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_28;
	SmsPre[i][0] = m_check081;
    SmsBytimePre[i][0] = m_check082;

    i=(smsnumCurrentPage-1)*10+8 ;
	if(m_edit_19=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_19;
	if(m_edit_29=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_29;
	SmsPre[i][0] = m_check091;
    SmsBytimePre[i][0] = m_check092;

    i=(smsnumCurrentPage-1)*10+9 ;
	if(m_edit_110=="") SmsName[i][0]="no"; else SmsName[i][0] = m_edit_110;
	if(m_edit_210=="") SmsNum[i][0] ="no";  else SmsNum[i][0] = m_edit_210;
	SmsPre[i][0] = m_check101;
    SmsBytimePre[i][0] = m_check102;
}

void CSmsNum::OnSmsnumGet() 
{
	// TODO: Add your control notification handler code here
	GetSmsNum();
	UpdateData(TRUE);  
	GetDataForDsp();
	UpdateData(FALSE); //刷新窗口中数据	
	
}

void CSmsNum::OnSmsnumDownpage() 
{
	// TODO: Add your control notification handler code here
	if( smsnumCurrentPage<smsnumPageNum )
	{ 
		smsnumCurrentPage++;
		GetDataForDsp();
		m_smsnum_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("下翻页结束!","警告!!!",MB_ICONSTOP );
	}		
}

void CSmsNum::OnSmsnumUppage() 
{
	// TODO: Add your control notification handler code here
	if( smsnumCurrentPage>1 )
	{ 
		smsnumCurrentPage--;
		GetDataForDsp();
		m_smsnum_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("上翻页结束!","警告!!!", MB_ICONSTOP );
	}	
}

void CSmsNum::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if( pScrollBar==&m_smsnum_scrollbar )//选择页面
	{
	  m_smsnum_scrollbar.SetScrollRange(1,smsnumPageNum);
	  m_smsnum_scrollbar.SetScrollPos(smsnumCurrentPage);
	  switch( nSBCode )
	  {
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	      smsnumCurrentPage = m_smsnum_scrollbar.GetScrollPos();
		  smsnumCurrentPage = smsnumCurrentPage - 1;
		  if( smsnumCurrentPage<1 )smsnumCurrentPage = 1;
		  UpdateData(FALSE);
 		  break;
	  case SB_LINERIGHT: //单击滚动条向右箭头  
 	      smsnumCurrentPage = m_smsnum_scrollbar.GetScrollPos();
		  smsnumCurrentPage = smsnumCurrentPage + 1;
		  if( smsnumCurrentPage>smsnumPageNum )smsnumCurrentPage = smsnumPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEUP: //单击向左按钮和滑块之间的区域 
 	      smsnumCurrentPage = m_smsnum_scrollbar.GetScrollPos();
		  smsnumCurrentPage = smsnumCurrentPage - 3;
		  if( smsnumCurrentPage<1 )smsnumCurrentPage = 1;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEDOWN: //单击向右按钮和滑块之间的区域 
 	      smsnumCurrentPage = m_smsnum_scrollbar.GetScrollPos();
		  smsnumCurrentPage = smsnumCurrentPage + 3;
		  if( smsnumCurrentPage>smsnumPageNum )smsnumCurrentPage = smsnumPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_THUMBTRACK: //拖动滑块
          m_smsnum_scrollbar.SetScrollPos(nPos);
		  smsnumCurrentPage =  nPos; 	
		  UpdateData(FALSE);
 		  break;	  

	  }	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	GetDataForDsp();
	UpdateData(FALSE);  
	}

}
void CSmsNum::GetDataForDsp()
{  
	char		strNum[200];
	int			i;

	sprintf(strNum,"No %d (Total %d)",smsnumCurrentPage,smsnumPageNum); m_edit_page = strNum;
    i=(smsnumCurrentPage-1)*10+0;
	if (i< 0  ) return ; 
	if(SmsName[i][0]=="no") m_edit_11 = ""; else m_edit_11 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_21 = ""; else m_edit_21 = SmsNum[i][0];
	m_check011 = SmsPre[i][0];
    m_check012 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+1 ;
	if(SmsName[i][0]=="no") m_edit_12 = ""; else m_edit_12 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_22 = ""; else m_edit_22 = SmsNum[i][0];
	m_check021 = SmsPre[i][0];
    m_check022 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+2 ;
	if(SmsName[i][0]=="no") m_edit_13 = ""; else m_edit_13 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_23 = ""; else m_edit_23 = SmsNum[i][0];
	m_check031 = SmsPre[i][0];
    m_check032 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+3 ;
	if(SmsName[i][0]=="no") m_edit_14 = ""; else m_edit_14 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_24 = ""; else m_edit_24 = SmsNum[i][0];
	m_check041 = SmsPre[i][0];
    m_check042 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+4 ;
	if(SmsName[i][0]=="no") m_edit_15 = ""; else m_edit_15 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_25 = ""; else m_edit_25 = SmsNum[i][0];
	m_check051 = SmsPre[i][0];
    m_check052 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+5 ;
	if(SmsName[i][0]=="no") m_edit_16 = ""; else m_edit_16 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_26 = ""; else m_edit_26 = SmsNum[i][0];
	m_check061 = SmsPre[i][0];
    m_check062 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+6 ;
	if(SmsName[i][0]=="no") m_edit_17 = ""; else m_edit_17 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_27 = ""; else m_edit_27 = SmsNum[i][0];
	m_check071 = SmsPre[i][0];
    m_check072 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+7 ;
	if(SmsName[i][0]=="no") m_edit_18 = ""; else m_edit_18 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_28 = ""; else m_edit_28 = SmsNum[i][0];
	m_check081 = SmsPre[i][0];
    m_check082 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+8 ;
	if(SmsName[i][0]=="no") m_edit_19 = ""; else m_edit_19 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_29 = ""; else m_edit_29 = SmsNum[i][0];
	m_check091 = SmsPre[i][0];
    m_check092 = SmsBytimePre[i][0];

    i=(smsnumCurrentPage-1)*10+9 ;
	if(SmsName[i][0]=="no") m_edit_110 = ""; else m_edit_110 = SmsName[i][0];
	if(SmsNum[i][0]=="no") m_edit_210 = ""; else m_edit_210 = SmsNum[i][0];
	m_check101 = SmsPre[i][0];
    m_check102 = SmsBytimePre[i][0];
}

BOOL CSmsNum::OnInitDialog() 
{
	CDialog::OnInitDialog();
	OnSmsnumGet() ;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsNum::OnButton1On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=0;i<=MAX_SMSNUM_NUM;i++) SmsPre[i][0] = 1;
	GetDataForDsp();
	UpdateData(FALSE);  	
	
}

void CSmsNum::OnButton1Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=0;i<=MAX_SMSNUM_NUM;i++) SmsPre[i][0] = 0;
	GetDataForDsp();
	UpdateData(FALSE);  
	
}

void CSmsNum::OnButton1On2() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=0;i<=MAX_SMSNUM_NUM;i++) SmsBytimePre[i][0] = 1;
	GetDataForDsp();
	UpdateData(FALSE);  	
	
}

void CSmsNum::OnButton1Off2() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=0;i<=MAX_SMSNUM_NUM;i++) SmsBytimePre[i][0] = 0;
	GetDataForDsp();
	UpdateData(FALSE);  
	
}
