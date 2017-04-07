// cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/AlmClsDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlmClsDef dialog


CAlmClsDef::CAlmClsDef(CWnd* pParent /*=NULL*/)
	: CDialog(CAlmClsDef::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlmClsDef)
	m_edit01 = _T("");
	m_edit03 = _T("");
	m_edit02 = _T("");
	m_edit04 = _T("");
	m_edit05 = _T("");
	m_edit06 = _T("");
	m_edit07 = _T("");
	m_edit08 = _T("");
	m_edit09 = _T("");
	m_edit10 = _T("");
	m_edit11 = _T("");
	m_edit12 = _T("");
	m_check011 = FALSE;
	m_check012 = FALSE;
	m_check013 = FALSE;
	m_check014 = FALSE;
	m_check021 = FALSE;
	m_check022 = FALSE;
	m_check023 = FALSE;
	m_check024 = FALSE;
	m_check031 = FALSE;
	m_check032 = FALSE;
	m_check033 = FALSE;
	m_check034 = FALSE;
	m_check041 = FALSE;
	m_check042 = FALSE;
	m_check043 = FALSE;
	m_check044 = FALSE;
	m_check051 = FALSE;
	m_check052 = FALSE;
	m_check053 = FALSE;
	m_check054 = FALSE;
	m_check061 = FALSE;
	m_check062 = FALSE;
	m_check063 = FALSE;
	m_check064 = FALSE;
	m_check072 = FALSE;
	m_check071 = FALSE;
	m_check073 = FALSE;
	m_check074 = FALSE;
	m_check081 = FALSE;
	m_check082 = FALSE;
	m_check083 = FALSE;
	m_check084 = FALSE;
	m_check091 = FALSE;
	m_check092 = FALSE;
	m_check093 = FALSE;
	m_check094 = FALSE;
	m_check101 = FALSE;
	m_check102 = FALSE;
	m_check103 = FALSE;
	m_check104 = FALSE;
	m_check111 = FALSE;
	m_check112 = FALSE;
	m_check113 = FALSE;
	m_check114 = FALSE;
	m_check121 = FALSE;
	m_check122 = FALSE;
	m_check123 = FALSE;
	m_check124 = FALSE;
	m_edit_page = _T("");
	m_edit_station = _T("");
	//}}AFX_DATA_INIT
}


void CAlmClsDef::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlmClsDef)
	DDX_Control(pDX, IDC_STNSELECT_SCROLLBAR, m_stnselect_scrollbar);
	DDX_Control(pDX, IDC_ALMCLSDEF_SCROLLBAR, m_almclsdef_scrollbar);
	DDX_Control(pDX, IDC_BUTTON_4_ON, m_cls4_on);
	DDX_Control(pDX, IDC_BUTTON_4_OFF, m_cls4_off);
	DDX_Control(pDX, IDC_BUTTON_3_ON, m_cls3_on);
	DDX_Control(pDX, IDC_BUTTON_3_OFF, m_cls3_off);
	DDX_Control(pDX, IDC_BUTTON_2_ON, m_cls2_on);
	DDX_Control(pDX, IDC_BUTTON_2_OFF, m_cls2_off);
	DDX_Control(pDX, IDC_BUTTON_1_ON, m_cls1_on);
	DDX_Control(pDX, IDC_BUTTON_1_OFF, m_cls1_off);
	DDX_Text(pDX, IDC_EDIT01, m_edit01);
	DDX_Text(pDX, IDC_EDIT03, m_edit03);
	DDX_Text(pDX, IDC_EDIT02, m_edit02);
	DDX_Text(pDX, IDC_EDIT04, m_edit04);
	DDX_Text(pDX, IDC_EDIT05, m_edit05);
	DDX_Text(pDX, IDC_EDIT06, m_edit06);
	DDX_Text(pDX, IDC_EDIT07, m_edit07);
	DDX_Text(pDX, IDC_EDIT08, m_edit08);
	DDX_Text(pDX, IDC_EDIT09, m_edit09);
	DDX_Text(pDX, IDC_EDIT10, m_edit10);
	DDX_Text(pDX, IDC_EDIT11, m_edit11);
	DDX_Text(pDX, IDC_EDIT12, m_edit12);
	DDX_Check(pDX, IDC_CHECK011, m_check011);
	DDX_Check(pDX, IDC_CHECK012, m_check012);
	DDX_Check(pDX, IDC_CHECK013, m_check013);
	DDX_Check(pDX, IDC_CHECK014, m_check014);
	DDX_Check(pDX, IDC_CHECK021, m_check021);
	DDX_Check(pDX, IDC_CHECK022, m_check022);
	DDX_Check(pDX, IDC_CHECK023, m_check023);
	DDX_Check(pDX, IDC_CHECK024, m_check024);
	DDX_Check(pDX, IDC_CHECK031, m_check031);
	DDX_Check(pDX, IDC_CHECK032, m_check032);
	DDX_Check(pDX, IDC_CHECK033, m_check033);
	DDX_Check(pDX, IDC_CHECK034, m_check034);
	DDX_Check(pDX, IDC_CHECK041, m_check041);
	DDX_Check(pDX, IDC_CHECK042, m_check042);
	DDX_Check(pDX, IDC_CHECK043, m_check043);
	DDX_Check(pDX, IDC_CHECK044, m_check044);
	DDX_Check(pDX, IDC_CHECK051, m_check051);
	DDX_Check(pDX, IDC_CHECK052, m_check052);
	DDX_Check(pDX, IDC_CHECK053, m_check053);
	DDX_Check(pDX, IDC_CHECK054, m_check054);
	DDX_Check(pDX, IDC_CHECK061, m_check061);
	DDX_Check(pDX, IDC_CHECK062, m_check062);
	DDX_Check(pDX, IDC_CHECK063, m_check063);
	DDX_Check(pDX, IDC_CHECK064, m_check064);
	DDX_Check(pDX, IDC_CHECK072, m_check072);
	DDX_Check(pDX, IDC_CHECK071, m_check071);
	DDX_Check(pDX, IDC_CHECK073, m_check073);
	DDX_Check(pDX, IDC_CHECK074, m_check074);
	DDX_Check(pDX, IDC_CHECK081, m_check081);
	DDX_Check(pDX, IDC_CHECK082, m_check082);
	DDX_Check(pDX, IDC_CHECK083, m_check083);
	DDX_Check(pDX, IDC_CHECK084, m_check084);
	DDX_Check(pDX, IDC_CHECK091, m_check091);
	DDX_Check(pDX, IDC_CHECK092, m_check092);
	DDX_Check(pDX, IDC_CHECK093, m_check093);
	DDX_Check(pDX, IDC_CHECK094, m_check094);
	DDX_Check(pDX, IDC_CHECK101, m_check101);
	DDX_Check(pDX, IDC_CHECK102, m_check102);
	DDX_Check(pDX, IDC_CHECK103, m_check103);
	DDX_Check(pDX, IDC_CHECK104, m_check104);
	DDX_Check(pDX, IDC_CHECK111, m_check111);
	DDX_Check(pDX, IDC_CHECK112, m_check112);
	DDX_Check(pDX, IDC_CHECK113, m_check113);
	DDX_Check(pDX, IDC_CHECK114, m_check114);
	DDX_Check(pDX, IDC_CHECK121, m_check121);
	DDX_Check(pDX, IDC_CHECK122, m_check122);
	DDX_Check(pDX, IDC_CHECK123, m_check123);
	DDX_Check(pDX, IDC_CHECK124, m_check124);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_edit_page);
	DDX_Text(pDX, IDC_EDIT_STN, m_edit_station);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlmClsDef, CDialog)
	//{{AFX_MSG_MAP(CAlmClsDef)
	ON_BN_CLICKED(ID_ALMCLSDEF_GET, OnAlmclsdefGet)
	ON_BN_CLICKED(ID_ALMCLSDEF_UPPAGE, OnAlmclsdefUppage)
	ON_BN_CLICKED(ID_ALMCLSDEF_DOWNPAGE, OnAlmclsdefDownpage)
	ON_BN_CLICKED(IDC_BUTTON_1_OFF, OnButton1Off)
	ON_BN_CLICKED(IDC_BUTTON_1_ON, OnButton1On)
	ON_BN_CLICKED(IDC_BUTTON_2_OFF, OnButton2Off)
	ON_BN_CLICKED(IDC_BUTTON_2_ON, OnButton2On)
	ON_BN_CLICKED(IDC_BUTTON_3_OFF, OnButton3Off)
	ON_BN_CLICKED(IDC_BUTTON_3_ON, OnButton3On)
	ON_BN_CLICKED(IDC_BUTTON_4_OFF, OnButton4Off)
	ON_BN_CLICKED(IDC_BUTTON_4_ON, OnButton4On)
	ON_BN_CLICKED(ID_ALMCLSDEF_SAVE, OnAlmclsdefSave)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_PAGE, OnChangeEditPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlmClsDef message handlers


extern			REAL_SW RealSW[MAX_ANA_NUM];
extern			STN		Stn[STATION_ENTRY_NUMBER+1];	
extern	int 	Num,	StnNum, PageNum,	CurrentPage,	CurrentStnId;

void CAlmClsDef::OnAlmclsdefGet() 
{
	// TODO: Add your control notification handler code here
		ReadDefineFile(1);	
		GetDataForDsp();
		UpdateData(FALSE);  
}

void CAlmClsDef::OnAlmclsdefSave() 
{
	// TODO: Add your control notification handler code here			  
	if(MessageBox("确定要保存吗 ?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{	  
		UpdateData(TRUE); 
		GetDataForSave();
	    UpdateData(FALSE);  			  
		SaveDefineFile();
		FILE *fp=NULL;
		if(fp=fopen("/home/ems/h9000/tmp/flagfile.txt","w"))
			fclose(fp);
		//MessageBox("使用新定义文件,需要重新启动EMS系统 !!!","警告!!!",MB_ICONEXCLAMATION  );
	}	
		
}

void CAlmClsDef::OnAlmclsdefUppage() 
{
	// TODO: Add your control notification handler code here
	if( CurrentPage>1 )
	{ 
		CurrentPage--;
		GetDataForDsp();
		m_almclsdef_scrollbar.SetScrollPos(CurrentPage);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("上翻页结束!","警告!!!", MB_ICONSTOP );
	}
}

void CAlmClsDef::OnAlmclsdefDownpage() 
{
	// TODO: Add your control notification handler code here
	if( CurrentPage<PageNum )
	{ 
		CurrentPage++;
		GetDataForDsp();
		m_almclsdef_scrollbar.SetScrollPos(CurrentPage);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("下翻页结束!","警告!!!",MB_ICONSTOP );
	}
}

void CAlmClsDef::OnButton1Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class1 = 0;
	GetDataForDsp();
	UpdateData(FALSE);  
}

void CAlmClsDef::OnButton1On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class1 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  	
}

void CAlmClsDef::OnButton2Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class2 = 0;
	GetDataForDsp();
	UpdateData(FALSE);  	
}

void CAlmClsDef::OnButton2On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class2 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  		
}

void CAlmClsDef::OnButton3Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class3 = 0;
	GetDataForDsp();
	UpdateData(FALSE);  		
}

void CAlmClsDef::OnButton3On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class3 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  			
}

void CAlmClsDef::OnButton4Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class4 = 0;
	GetDataForDsp();
	UpdateData(FALSE);  			
}

void CAlmClsDef::OnButton4On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=Stn[CurrentStnId].StartPnt;i<=Stn[CurrentStnId].EndPnt;i++) RealSW[i].class4 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  			
}

void CAlmClsDef::GetDataForDsp()
{
	char		strNum[200],	strStn[50];
	int			i;
		sprintf(strNum,"No %d (Total %d)",CurrentPage,Stn[CurrentStnId].PageNum); m_edit_page = strNum;
		sprintf(strStn,"%s (%d--%d)",Stn[CurrentStnId].StnCName,Stn[CurrentStnId].StartPnt+1,Stn[CurrentStnId].EndPnt+1); m_edit_station = strStn;

		i=(CurrentPage-1)*12+0 + Stn[CurrentStnId].StartPnt; 
    	if (i< 0  ) return ; 
//		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].c_name); m_edit01=strNum;
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit01=strNum;
		m_check011 = RealSW[i].class1;
		m_check012 = RealSW[i].class2;
		m_check013 = RealSW[i].class3;
		m_check014 = RealSW[i].class4;
		i=(CurrentPage-1)*12+1 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit02=strNum;
		m_check021 = RealSW[i].class1;
		m_check022 = RealSW[i].class2;
		m_check023 = RealSW[i].class3;
		m_check024 = RealSW[i].class4;
		i=(CurrentPage-1)*12+2 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit03=strNum;
		m_check031 = RealSW[i].class1;
		m_check032 = RealSW[i].class2;
		m_check033 = RealSW[i].class3;
		m_check034 = RealSW[i].class4;	
		i=(CurrentPage-1)*12+3 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit04=strNum;
		m_check041 = RealSW[i].class1;
		m_check042 = RealSW[i].class2;
		m_check043 = RealSW[i].class3;
		m_check044 = RealSW[i].class4;
		i=(CurrentPage-1)*12+4 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit05=strNum;
		m_check051 = RealSW[i].class1;
		m_check052 = RealSW[i].class2;
		m_check053 = RealSW[i].class3;
		m_check054 = RealSW[i].class4;
		i=(CurrentPage-1)*12+5 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit06=strNum;
		m_check061 = RealSW[i].class1;
		m_check062 = RealSW[i].class2;
		m_check063 = RealSW[i].class3;
		m_check064 = RealSW[i].class4;
		i=(CurrentPage-1)*12+6 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit07=strNum;
		m_check071 = RealSW[i].class1;
		m_check072 = RealSW[i].class2;
		m_check073 = RealSW[i].class3;
		m_check074 = RealSW[i].class4;
		i=(CurrentPage-1)*12+7 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit08=strNum;
		m_check081 = RealSW[i].class1;
		m_check082 = RealSW[i].class2;
		m_check083 = RealSW[i].class3;
		m_check084 = RealSW[i].class4;
		i=(CurrentPage-1)*12+8 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit09=strNum;
		m_check091 = RealSW[i].class1;
		m_check092 = RealSW[i].class2;
		m_check093 = RealSW[i].class3;
		m_check094 = RealSW[i].class4;
		i=(CurrentPage-1)*12+9 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit10=strNum;
		m_check101 = RealSW[i].class1;
		m_check102 = RealSW[i].class2;
		m_check103 = RealSW[i].class3;
		m_check104 = RealSW[i].class4;
		i=(CurrentPage-1)*12+10 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit11=strNum;
		m_check111 = RealSW[i].class1;
		m_check112 = RealSW[i].class2;
		m_check113 = RealSW[i].class3;
		m_check114 = RealSW[i].class4;
		i=(CurrentPage-1)*12+11 + Stn[CurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealSW[i].l_name); m_edit12=strNum;
		m_check121 = RealSW[i].class1;
		m_check122 = RealSW[i].class2;
		m_check123 = RealSW[i].class3;
		m_check124 = RealSW[i].class4;
}


void CAlmClsDef::GetDataForSave()
{
	int			i;
		i=(CurrentPage-1)*12+0 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check011;
		RealSW[i].class2 = m_check012;
		RealSW[i].class3 = m_check013;
		RealSW[i].class4 = m_check014;

		i=(CurrentPage-1)*12+1 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check021;
		RealSW[i].class2 = m_check022;
		RealSW[i].class3 = m_check023;
		RealSW[i].class4 = m_check024;

		i=(CurrentPage-1)*12+2 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check031;
		RealSW[i].class2 = m_check032;
		RealSW[i].class3 = m_check033;
		RealSW[i].class4 = m_check034;

		i=(CurrentPage-1)*12+3 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check041;
		RealSW[i].class2 = m_check042;
		RealSW[i].class3 = m_check043;
		RealSW[i].class4 = m_check044;

		i=(CurrentPage-1)*12+4 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check051;
		RealSW[i].class2 = m_check052;
		RealSW[i].class3 = m_check053;
		RealSW[i].class4 = m_check054;

		i=(CurrentPage-1)*12+5 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check061;
		RealSW[i].class2 = m_check062;
		RealSW[i].class3 = m_check063;
		RealSW[i].class4 = m_check064;

		i=(CurrentPage-1)*12+6 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check071;
		RealSW[i].class2 = m_check072;
		RealSW[i].class3 = m_check073;
		RealSW[i].class4 = m_check074;

		i=(CurrentPage-1)*12+7 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check081;
		RealSW[i].class2 = m_check082;
		RealSW[i].class3 = m_check083;
		RealSW[i].class4 = m_check084;

		i=(CurrentPage-1)*12+8 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check091;
		RealSW[i].class2 = m_check092;
		RealSW[i].class3 = m_check093;
		RealSW[i].class4 = m_check094;

		i=(CurrentPage-1)*12+9 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check101;
		RealSW[i].class2 = m_check102;
		RealSW[i].class3 = m_check103;
		RealSW[i].class4 = m_check104;

		i=(CurrentPage-1)*12+10 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check111;
		RealSW[i].class2 = m_check112;
		RealSW[i].class3 = m_check113;
		RealSW[i].class4 = m_check114;

		i=(CurrentPage-1)*12+11 + Stn[CurrentStnId].StartPnt; 
		RealSW[i].class1 = m_check121;
		RealSW[i].class2 = m_check122;
		RealSW[i].class3 = m_check123;
		RealSW[i].class4 = m_check124;	
}



void CAlmClsDef::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	
	if( pScrollBar==&m_almclsdef_scrollbar )//选择页面
	{
	  m_almclsdef_scrollbar.SetScrollRange(1,PageNum);
	  m_almclsdef_scrollbar.SetScrollPos(CurrentPage);
	  switch( nSBCode )
	  {
	  case SB_THUMBTRACK: //拖动滑块
          m_almclsdef_scrollbar.SetScrollPos(nPos);
		  CurrentPage =  nPos; 	
		  UpdateData(FALSE);
 		  break;	  
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	      CurrentPage = m_almclsdef_scrollbar.GetScrollPos();
		  CurrentPage = CurrentPage - 1;
		  if( CurrentPage<1 )CurrentPage = 1;
		  UpdateData(FALSE);
 		  break;
	  case SB_LINERIGHT: //单击滚动条向右箭头  
 	      CurrentPage = m_almclsdef_scrollbar.GetScrollPos();
		  CurrentPage = CurrentPage + 1;
		  if( CurrentPage>PageNum )CurrentPage = PageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEUP: //单击向左按钮和滑块之间的区域 
 	      CurrentPage = m_almclsdef_scrollbar.GetScrollPos();
		  CurrentPage = CurrentPage - 3;
		  if( CurrentPage<1 )CurrentPage = 1;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEDOWN: //单击向右按钮和滑块之间的区域 
 	      CurrentPage = m_almclsdef_scrollbar.GetScrollPos();
		  CurrentPage = CurrentPage + 3;
		  if( CurrentPage>PageNum )CurrentPage = PageNum;
		  UpdateData(FALSE); 
          break;
	  }
	  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	  GetDataForDsp();
	  UpdateData(FALSE);  
	}	


	if( pScrollBar==&m_stnselect_scrollbar )//选择厂站
	{
	  m_stnselect_scrollbar.SetScrollRange(0,StnNum);
	  m_stnselect_scrollbar.SetScrollPos(CurrentStnId);
	  switch( nSBCode )
	  {
	  case SB_THUMBTRACK: //拖动滑块
          m_stnselect_scrollbar.SetScrollPos(nPos);
		  CurrentStnId =  nPos; 
		  
		  PageNum	   = Stn[CurrentStnId].PntNum/12;  
		  if( (Stn[CurrentStnId].PntNum%12)!=0 )PageNum++;
	      CurrentPage	= 1;
		  
		  UpdateData(FALSE);
 		  break;
		  
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	  case SB_PAGEUP:    //单击向左按钮和滑块之间的区域
	      CurrentStnId = m_stnselect_scrollbar.GetScrollPos();
		  CurrentStnId = CurrentStnId - 1;
		  if( CurrentStnId<0 )CurrentStnId = 0;

          PageNum	   = Stn[CurrentStnId].PntNum/12;  
		  if( (Stn[CurrentStnId].PntNum%12)!=0 )PageNum++;
	      CurrentPage	= 1;
		  
		  UpdateData(FALSE);
 		  break;

	  case SB_LINERIGHT: //单击滚动条向右箭头  
      case SB_PAGEDOWN:  //单击向右按钮和滑块之间的区域 	      
		  CurrentStnId = m_stnselect_scrollbar.GetScrollPos();
		  CurrentStnId = CurrentStnId + 1;
		  if( CurrentStnId>=StnNum )CurrentStnId = StnNum;	
		  
		  PageNum	   = Stn[CurrentStnId].PntNum/12;  
		  if( (Stn[CurrentStnId].PntNum%12)!=0 )PageNum++;
	      CurrentPage	= 1;
		  
		  UpdateData(FALSE); 
          break;
	  }
	  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	  GetDataForDsp();
	  m_almclsdef_scrollbar.SetScrollPos(CurrentPage);
	  UpdateData(FALSE);  	
	}	

}

BOOL CAlmClsDef::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlmClsDef::OnChangeEditPage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
