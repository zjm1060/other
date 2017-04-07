// AlmAnaDef.cpp : implementation file
//

#include <stdafx.h>
#include "../inc/teldef.h"
#include "../inc/AlmAnaDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlmAnaDef dialog

CAlmAnaDef::CAlmAnaDef(CWnd* pParent /*=NULL*/)
	: CDialog(CAlmAnaDef::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlmAnaDef)
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
	m_check071 = FALSE;
	m_check072 = FALSE;
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
	m_edit01 = _T("");
	m_edit02 = _T("");
	m_edit03 = _T("");
	m_edit04 = _T("");
	m_edit05 = _T("");
	m_edit06 = _T("");
	m_edit07 = _T("");
	m_edit08 = _T("");
	m_edit09 = _T("");
	m_edit10 = _T("");
	m_edit11 = _T("");
	m_edit12 = _T("");
	//}}AFX_DATA_INIT
}


void CAlmAnaDef::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlmAnaDef)
	DDX_Control(pDX, IDC_STNSELECT_SCROLLBAR, m_stnselect_scrollbar);
	DDX_Control(pDX, IDC_BUTTON_4_ON, m_ana4_on);
	DDX_Control(pDX, IDC_BUTTON_4_OFF, m_ana4_off);
	DDX_Control(pDX, IDC_BUTTON_3_ON, m_ana3_on);
	DDX_Control(pDX, IDC_BUTTON_3_OFF, m_ana3_off);
	DDX_Control(pDX, IDC_BUTTON_2_ON, m_ana2_on);
	DDX_Control(pDX, IDC_BUTTON_2_OFF, m_ana2_off);
	DDX_Control(pDX, IDC_BUTTON_1_ON, m_ana1_on);
	DDX_Control(pDX, IDC_BUTTON_1_OFF, m_ana1_off);
	DDX_Control(pDX, IDC_ALMANADEF_SCROLLBAR, m_almanadef_scrollbar);
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
	DDX_Check(pDX, IDC_CHECK071, m_check071);
	DDX_Check(pDX, IDC_CHECK072, m_check072);
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
	DDX_Text(pDX, IDC_EDIT01, m_edit01);
	DDX_Text(pDX, IDC_EDIT02, m_edit02);
	DDX_Text(pDX, IDC_EDIT03, m_edit03);
	DDX_Text(pDX, IDC_EDIT04, m_edit04);
	DDX_Text(pDX, IDC_EDIT05, m_edit05);
	DDX_Text(pDX, IDC_EDIT06, m_edit06);
	DDX_Text(pDX, IDC_EDIT07, m_edit07);
	DDX_Text(pDX, IDC_EDIT08, m_edit08);
	DDX_Text(pDX, IDC_EDIT09, m_edit09);
	DDX_Text(pDX, IDC_EDIT10, m_edit10);
	DDX_Text(pDX, IDC_EDIT11, m_edit11);
	DDX_Text(pDX, IDC_EDIT12, m_edit12);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlmAnaDef, CDialog)
	//{{AFX_MSG_MAP(CAlmAnaDef)
	ON_BN_CLICKED(ID_ALMANADEF_UPPAGE, OnAlmanadefUppage)
	ON_BN_CLICKED(ID_ALMANADEF_GET, OnAlmanadefGet)
	ON_BN_CLICKED(ID_ALMANADEF_SAVE, OnAlmanadefSave)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_PAGE, OnChangeEditPage)
	ON_BN_CLICKED(ID_ALMANADEF_DOWNPAGE, OnAlmanadefDownpage)
	ON_BN_CLICKED(IDC_BUTTON_1_OFF, OnButton1Off)
	ON_BN_CLICKED(IDC_BUTTON_1_ON, OnButton1On)
	ON_BN_CLICKED(IDC_BUTTON_2_OFF, OnButton2Off)
	ON_BN_CLICKED(IDC_BUTTON_2_ON, OnButton2On)
	ON_BN_CLICKED(IDC_BUTTON_3_OFF, OnButton3Off)
	ON_BN_CLICKED(IDC_BUTTON_3_ON, OnButton3On)
	ON_BN_CLICKED(IDC_BUTTON_4_OFF, OnButton4Off)
	ON_BN_CLICKED(IDC_BUTTON_4_ON, OnButton4On)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlmAnaDef message handlers

extern			REAL_ANA RealANA[MAX_ANA_NUM];
extern			STN		AnaStn[STATION_ENTRY_NUMBER+1];	
extern	int 	AnaNum,	anadefCurPag,	anadefPageNum;
extern	int     AnaStnNum,AnaCurrentStnId;

void CAlmAnaDef::OnAlmanadefSave() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{	  
		UpdateData(TRUE); 
		GetDataForSave();
	    UpdateData(FALSE);  			  
		SaveAnaDefineFile();
		FILE *fp=NULL;
		if(fp=fopen("/home/ems/h9000/tmp/flagfile2.txt","w"))
			fclose(fp);
		//MessageBox("使用新定义文件,需要重新启动EMS系统 !!!","警告!!!",MB_ICONEXCLAMATION  );
	}	
}

void CAlmAnaDef::OnAlmanadefGet() 
{
	// TODO: Add your control notification handler code here
	ReadAnaDefineFile(1);	
	GetDataForDsp();
    UpdateData(FALSE);  	
}

void CAlmAnaDef::OnAlmanadefUppage() 
{
	// TODO: Add your control notification handler code here
	if( anadefCurPag>1 )
	{ 
		anadefCurPag--;
		GetDataForDsp();
		m_almanadef_scrollbar.SetScrollPos(anadefCurPag);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("上翻页结束!","警告!!!", MB_ICONSTOP );
	}	
}

void CAlmAnaDef::GetDataForDsp()
{
	char		strNum[200],	strStn[50];
	int			i;
		sprintf(strNum,"No %d (Total %d)",anadefCurPag,AnaStn[AnaCurrentStnId].PageNum); m_edit_page = strNum;
		sprintf(strStn,"%s (%d--%d)",AnaStn[AnaCurrentStnId].StnCName,AnaStn[AnaCurrentStnId].StartPnt+1,AnaStn[AnaCurrentStnId].EndPnt+1); m_edit_station = strStn;
//        anadefCurPag = 1;
		i=(anadefCurPag-1)*12+0 + AnaStn[AnaCurrentStnId].StartPnt; 
    	if (i< 0  ) return ; 
//		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].c_name); m_edit01=strNum;
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit01=strNum;
		m_check011 = RealANA[i].class1;
		m_check012 = RealANA[i].class2;
		m_check013 = RealANA[i].class3;
		m_check014 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+1 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit02=strNum;
		m_check021 = RealANA[i].class1;
		m_check022 = RealANA[i].class2;
		m_check023 = RealANA[i].class3;
		m_check024 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+2 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit03=strNum;
		m_check031 = RealANA[i].class1;
		m_check032 = RealANA[i].class2;
		m_check033 = RealANA[i].class3;
		m_check034 = RealANA[i].class4;	

		i=(anadefCurPag-1)*12+3 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit04=strNum;
		m_check041 = RealANA[i].class1;
		m_check042 = RealANA[i].class2;
		m_check043 = RealANA[i].class3;
		m_check044 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+4 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit05=strNum;
		m_check051 = RealANA[i].class1;
		m_check052 = RealANA[i].class2;
		m_check053 = RealANA[i].class3;
		m_check054 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+5 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit06=strNum;
		m_check061 = RealANA[i].class1;
		m_check062 = RealANA[i].class2;
		m_check063 = RealANA[i].class3;
		m_check064 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+6 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit07=strNum;
		m_check071 = RealANA[i].class1;
		m_check072 = RealANA[i].class2;
		m_check073 = RealANA[i].class3;
		m_check074 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+7 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit08=strNum;
		m_check081 = RealANA[i].class1;
		m_check082 = RealANA[i].class2;
		m_check083 = RealANA[i].class3;
		m_check084 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+8 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit09=strNum;
		m_check091 = RealANA[i].class1;
		m_check092 = RealANA[i].class2;
		m_check093 = RealANA[i].class3;
		m_check094 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+9 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit10=strNum;
		m_check101 = RealANA[i].class1;
		m_check102 = RealANA[i].class2;
		m_check103 = RealANA[i].class3;
		m_check104 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+10 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit11=strNum;
		m_check111 = RealANA[i].class1;
		m_check112 = RealANA[i].class2;
		m_check113 = RealANA[i].class3;
		m_check114 = RealANA[i].class4;

		i=(anadefCurPag-1)*12+11 + AnaStn[AnaCurrentStnId].StartPnt; 
		sprintf(strNum,"%04d:  %s",i+1,RealANA[i].l_name); m_edit12=strNum;
		m_check121 = RealANA[i].class1;
		m_check122 = RealANA[i].class2;
		m_check123 = RealANA[i].class3;
		m_check124 = RealANA[i].class4;
}

void CAlmAnaDef::GetDataForSave()
{
	int			i;
		i=(anadefCurPag-1)*12+0 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check011;
		RealANA[i].class2 = m_check012;
		RealANA[i].class3 = m_check013;
		RealANA[i].class4 = m_check014;

		i=(anadefCurPag-1)*12+1 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check021;
		RealANA[i].class2 = m_check022;
		RealANA[i].class3 = m_check023;
		RealANA[i].class4 = m_check024;

		i=(anadefCurPag-1)*12+2 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check031;
		RealANA[i].class2 = m_check032;
		RealANA[i].class3 = m_check033;
		RealANA[i].class4 = m_check034;

		i=(anadefCurPag-1)*12+3 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check041;
		RealANA[i].class2 = m_check042;
		RealANA[i].class3 = m_check043;
		RealANA[i].class4 = m_check044;

		i=(anadefCurPag-1)*12+4 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check051;
		RealANA[i].class2 = m_check052;
		RealANA[i].class3 = m_check053;
		RealANA[i].class4 = m_check054;

		i=(anadefCurPag-1)*12+5 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check061;
		RealANA[i].class2 = m_check062;
		RealANA[i].class3 = m_check063;
		RealANA[i].class4 = m_check064;

		i=(anadefCurPag-1)*12+6 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check071;
		RealANA[i].class2 = m_check072;
		RealANA[i].class3 = m_check073;
		RealANA[i].class4 = m_check074;

		i=(anadefCurPag-1)*12+7 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check081;
		RealANA[i].class2 = m_check082;
		RealANA[i].class3 = m_check083;
		RealANA[i].class4 = m_check084;

		i=(anadefCurPag-1)*12+8 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check091;
		RealANA[i].class2 = m_check092;
		RealANA[i].class3 = m_check093;
		RealANA[i].class4 = m_check094;

		i=(anadefCurPag-1)*12+9 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check101;
		RealANA[i].class2 = m_check102;
		RealANA[i].class3 = m_check103;
		RealANA[i].class4 = m_check104;

		i=(anadefCurPag-1)*12+10 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check111;
		RealANA[i].class2 = m_check112;
		RealANA[i].class3 = m_check113;
		RealANA[i].class4 = m_check114;

		i=(anadefCurPag-1)*12+11 + AnaStn[AnaCurrentStnId].StartPnt; 
		RealANA[i].class1 = m_check121;
		RealANA[i].class2 = m_check122;
		RealANA[i].class3 = m_check123;
		RealANA[i].class4 = m_check124;	
}

extern			REAL_ANA RealANA[MAX_ANA_NUM];
extern			STN		AnaStn[STATION_ENTRY_NUMBER+1];	
extern	int 	AnaNum,	anadefCurPag,	anadefPageNum;
extern	int     AnaStnNum,AnaCurrentStnId;

void CAlmAnaDef::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if( pScrollBar==&m_almanadef_scrollbar )//选择页面
	{
	  m_almanadef_scrollbar.SetScrollRange(1,anadefPageNum);
	  m_almanadef_scrollbar.SetScrollPos(anadefCurPag);
	  switch( nSBCode )
	  {
	  case SB_THUMBTRACK: //拖动滑块
          m_almanadef_scrollbar.SetScrollPos(nPos);
		  anadefCurPag =  nPos; 	
		  UpdateData(FALSE);
 		  break;	  
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	      anadefCurPag = m_almanadef_scrollbar.GetScrollPos();
		  anadefCurPag = anadefCurPag - 1;
		  if( anadefCurPag<1 )anadefCurPag = 1;
		  UpdateData(FALSE);
 		  break;
	  case SB_LINERIGHT: //单击滚动条向右箭头  
 	      anadefCurPag = m_almanadef_scrollbar.GetScrollPos();
		  anadefCurPag = anadefCurPag + 1;
		  if( anadefCurPag>anadefPageNum )anadefCurPag = anadefPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEUP: //单击向左按钮和滑块之间的区域 
 	      anadefCurPag = m_almanadef_scrollbar.GetScrollPos();
		  anadefCurPag = anadefCurPag - 3;
		  if( anadefCurPag<1 )anadefCurPag = 1;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEDOWN: //单击向右按钮和滑块之间的区域 
 	      anadefCurPag = m_almanadef_scrollbar.GetScrollPos();
		  anadefCurPag = anadefCurPag + 3;
		  if( anadefCurPag>anadefPageNum )anadefCurPag = anadefPageNum;
		  UpdateData(FALSE); 
          break;
	  }
	  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	  GetDataForDsp();
	  UpdateData(FALSE);  
	}	


	if( pScrollBar==&m_stnselect_scrollbar )//选择厂站
	{
	  m_stnselect_scrollbar.SetScrollRange(0,AnaStnNum);
	  m_stnselect_scrollbar.SetScrollPos(AnaCurrentStnId);
	  switch( nSBCode )
	  {
	  case SB_THUMBTRACK: //拖动滑块
          m_stnselect_scrollbar.SetScrollPos(nPos);
		  AnaCurrentStnId =  nPos; 
		  
		  anadefPageNum	   = AnaStn[AnaCurrentStnId].PntNum/12;  
		  if( (AnaStn[AnaCurrentStnId].PntNum%12)!=0 )anadefPageNum++;
	      anadefCurPag= 1;
		  
		  UpdateData(FALSE);
 		  break;
		  
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	  case SB_PAGEUP:    //单击向左按钮和滑块之间的区域
	      AnaCurrentStnId = m_stnselect_scrollbar.GetScrollPos();
		  AnaCurrentStnId = AnaCurrentStnId - 1;
		  if( AnaCurrentStnId<0 )AnaCurrentStnId = 0;

          anadefPageNum	   = AnaStn[AnaCurrentStnId].PntNum/12;  
		  if( (AnaStn[AnaCurrentStnId].PntNum%12)!=0 )anadefPageNum++;
	      anadefCurPag	= 1;
		  
		  UpdateData(FALSE);
 		  break;

	  case SB_LINERIGHT: //单击滚动条向右箭头  
      case SB_PAGEDOWN:  //单击向右按钮和滑块之间的区域 	      
		  AnaCurrentStnId = m_stnselect_scrollbar.GetScrollPos();
		  AnaCurrentStnId = AnaCurrentStnId + 1;
		  if( AnaCurrentStnId>=AnaStnNum )AnaCurrentStnId = AnaStnNum;	
		  
		  anadefPageNum	   = AnaStn[AnaCurrentStnId].PntNum/12;  
		  if( (AnaStn[AnaCurrentStnId].PntNum%12)!=0 )anadefPageNum++;
	      anadefCurPag	= 1;
		  
		  UpdateData(FALSE); 
          break;
	  }
	  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	  GetDataForDsp();
	  m_almanadef_scrollbar.SetScrollPos(anadefCurPag);
	  UpdateData(FALSE);  	
	}	

}

BOOL CAlmAnaDef::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlmAnaDef::OnChangeEditPage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CAlmAnaDef::OnAlmanadefDownpage() 
{
	// TODO: Add your control notification handler code here
	if( anadefCurPag<anadefPageNum )
	{ 
		anadefCurPag++;
		GetDataForDsp();
		m_almanadef_scrollbar.SetScrollPos(anadefCurPag);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("下翻页结束!","警告!!!",MB_ICONSTOP );
	}		
}

void CAlmAnaDef::OnButton1Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class1 = 0;
	GetDataForDsp();
	UpdateData(FALSE); 	
}

void CAlmAnaDef::OnButton1On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class1 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  		
}

void CAlmAnaDef::OnButton2Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class2 = 0;
	GetDataForDsp();
	UpdateData(FALSE);  		
}

void CAlmAnaDef::OnButton2On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class2 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  			
}

void CAlmAnaDef::OnButton3Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class3 = 0;
	GetDataForDsp();
	UpdateData(FALSE);  		
	
}

void CAlmAnaDef::OnButton3On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class3 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  		
	
}

void CAlmAnaDef::OnButton4Off() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class4 = 0;
	GetDataForDsp();
	UpdateData(FALSE);  		
	
}

void CAlmAnaDef::OnButton4On() 
{
	// TODO: Add your control notification handler code here
	int	i;
	for(i=AnaStn[AnaCurrentStnId].StartPnt;i<=AnaStn[AnaCurrentStnId].EndPnt;i++) RealANA[i].class4 = 1;
	GetDataForDsp();
	UpdateData(FALSE);  		
	
}
