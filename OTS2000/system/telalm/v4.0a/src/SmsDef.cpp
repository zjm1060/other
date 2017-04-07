// SmsDef.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/SmsDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString	TelSmsDef[MAX_SMSDEF_NUM][MAX_SMSDEF_STY];

/////////////////////////////////////////////////////////////////////////////
// CSmsDef dialog


CSmsDef::CSmsDef(CWnd* pParent /*=NULL*/)
	: CDialog(CSmsDef::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmsDef)
	m_smsdef_edit11 = _T("");
	m_smsdef_edit12 = _T("");
	m_smsdef_edit13 = _T("");
	m_smsdef_edit14 = _T("");
	m_smsdef_edit15 = _T("");
	m_smsdef_edit16 = _T("");
	m_smsdef_edit17 = _T("");
	m_smsdef_edit18 = _T("");
	m_smsdef_edit19 = _T("");
	m_smsdef_edit110 = _T("");

/*	m_smsdef_edit21 = _T("");
	m_smsdef_edit210 = _T("");
	m_smsdef_edit22 = _T("");
	m_smsdef_edit23 = _T("");
	m_smsdef_edit24 = _T("");
	m_smsdef_edit25 = _T("");
	m_smsdef_edit26 = _T("");
	m_smsdef_edit27 = _T("");
	m_smsdef_edit28 = _T("");
	m_smsdef_edit29 = _T("");
*/
	m_smsdef_edit31 = _T("");
	m_smsdef_edit310 = _T("");
	m_smsdef_edit32 = _T("");
	m_smsdef_edit33 = _T("");
	m_smsdef_edit34 = _T("");
	m_smsdef_edit35 = _T("");
	m_smsdef_edit36 = _T("");
	m_smsdef_edit37 = _T("");
	m_smsdef_edit38 = _T("");
	m_smsdef_edit39 = _T("");

/*	m_smsdef_edit41 = _T("");
	m_smsdef_edit410 = _T("");
	m_smsdef_edit42 = _T("");
	m_smsdef_edit43 = _T("");
	m_smsdef_edit44 = _T("");
	m_smsdef_edit45 = _T("");
	m_smsdef_edit46 = _T("");
	m_smsdef_edit47 = _T("");
	m_smsdef_edit48 = _T("");
	m_smsdef_edit49 = _T("");
*/
	m_edit_page = _T("");
	//}}AFX_DATA_INIT
}


void CSmsDef::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmsDef)
	DDX_Control(pDX, IDC_SMSDEF_SCROLLBAR, m_smsdef_scrollbar);
	DDX_Control(pDX, ID_SMSDEF_SAVE, m_smsdef_save);
	DDX_Control(pDX, ID_SMSDEF_GET, m_smsdef_get);
	DDX_Text(pDX, IDC_SMSDEF_EDIT11, m_smsdef_edit11);
	DDX_Text(pDX, IDC_SMSDEF_EDIT12, m_smsdef_edit12);
	DDX_Text(pDX, IDC_SMSDEF_EDIT13, m_smsdef_edit13);
	DDX_Text(pDX, IDC_SMSDEF_EDIT14, m_smsdef_edit14);
	DDX_Text(pDX, IDC_SMSDEF_EDIT15, m_smsdef_edit15);
	DDX_Text(pDX, IDC_SMSDEF_EDIT16, m_smsdef_edit16);
	DDX_Text(pDX, IDC_SMSDEF_EDIT17, m_smsdef_edit17);
	DDX_Text(pDX, IDC_SMSDEF_EDIT18, m_smsdef_edit18);
	DDX_Text(pDX, IDC_SMSDEF_EDIT19, m_smsdef_edit19);
	DDX_Text(pDX, IDC_SMSDEF_EDIT110, m_smsdef_edit110);

/*	DDX_Text(pDX, IDC_SMSDEF_EDIT21, m_smsdef_edit21);
	DDX_Text(pDX, IDC_SMSDEF_EDIT210, m_smsdef_edit210);
	DDX_Text(pDX, IDC_SMSDEF_EDIT22, m_smsdef_edit22);
	DDX_Text(pDX, IDC_SMSDEF_EDIT23, m_smsdef_edit23);
	DDX_Text(pDX, IDC_SMSDEF_EDIT24, m_smsdef_edit24);
	DDX_Text(pDX, IDC_SMSDEF_EDIT25, m_smsdef_edit25);
	DDX_Text(pDX, IDC_SMSDEF_EDIT26, m_smsdef_edit26);
	DDX_Text(pDX, IDC_SMSDEF_EDIT27, m_smsdef_edit27);
	DDX_Text(pDX, IDC_SMSDEF_EDIT28, m_smsdef_edit28);
	DDX_Text(pDX, IDC_SMSDEF_EDIT29, m_smsdef_edit29);
*/
	DDX_Text(pDX, IDC_SMSDEF_EDIT31, m_smsdef_edit31);
	DDX_Text(pDX, IDC_SMSDEF_EDIT310, m_smsdef_edit310);
	DDX_Text(pDX, IDC_SMSDEF_EDIT32, m_smsdef_edit32);
	DDX_Text(pDX, IDC_SMSDEF_EDIT33, m_smsdef_edit33);
	DDX_Text(pDX, IDC_SMSDEF_EDIT34, m_smsdef_edit34);
	DDX_Text(pDX, IDC_SMSDEF_EDIT35, m_smsdef_edit35);
	DDX_Text(pDX, IDC_SMSDEF_EDIT36, m_smsdef_edit36);
	DDX_Text(pDX, IDC_SMSDEF_EDIT37, m_smsdef_edit37);
	DDX_Text(pDX, IDC_SMSDEF_EDIT38, m_smsdef_edit38);
	DDX_Text(pDX, IDC_SMSDEF_EDIT39, m_smsdef_edit39);

/*	DDX_Text(pDX, IDC_SMSDEF_EDIT41, m_smsdef_edit41);
	DDX_Text(pDX, IDC_SMSDEF_EDIT410, m_smsdef_edit410);
	DDX_Text(pDX, IDC_SMSDEF_EDIT42, m_smsdef_edit42);
	DDX_Text(pDX, IDC_SMSDEF_EDIT43, m_smsdef_edit43);
	DDX_Text(pDX, IDC_SMSDEF_EDIT44, m_smsdef_edit44);
	DDX_Text(pDX, IDC_SMSDEF_EDIT45, m_smsdef_edit45);
	DDX_Text(pDX, IDC_SMSDEF_EDIT46, m_smsdef_edit46);
	DDX_Text(pDX, IDC_SMSDEF_EDIT47, m_smsdef_edit47);
	DDX_Text(pDX, IDC_SMSDEF_EDIT48, m_smsdef_edit48);
	DDX_Text(pDX, IDC_SMSDEF_EDIT49, m_smsdef_edit49);
*/
	DDX_Text(pDX, IDC_EDIT_PAGE, m_edit_page);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsDef, CDialog)
	//{{AFX_MSG_MAP(CSmsDef)
	ON_BN_CLICKED(ID_SMSDEF_SAVE, OnSmsdefSave)
	ON_BN_CLICKED(ID_SMSDEF_GET, OnSmsdefGet)
	ON_BN_CLICKED(ID_SMSDEF_DOWNPAGE, OnSmsdefDownpage)
	ON_BN_CLICKED(ID_SMSDEF_UPPAGE, OnSmsdefUppage)
	ON_EN_CHANGE(IDC_EDIT_PAGE, OnChangeEditPage)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_SMSDEF_EDIT21, OnChangeSmsdefEdit21)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsDef message handlers

extern int 	smsPageNum,	smsCurrentPage;

void CSmsDef::GetDataForDsp()
{  
	char		strNum[200];
	int			i;

	sprintf(strNum,"No %d (Total %d)",smsCurrentPage,smsPageNum); m_edit_page = strNum;
    i=(smsCurrentPage-1)*10+0;
	if (i< 0  ) return ; 
	if(TelSmsDef[i][0]=="no") m_smsdef_edit11 = ""; else m_smsdef_edit11 = TelSmsDef[i][0];
//	if(TelSmsDef[i][1]=="no") m_smsdef_edit21 = ""; else m_smsdef_edit21 = TelSmsDef[i][1];
	if(TelSmsDef[i][2]=="no") m_smsdef_edit31 = ""; else m_smsdef_edit31 = TelSmsDef[i][2];
//	if(TelSmsDef[i][3]=="no") m_smsdef_edit41 = ""; else m_smsdef_edit41 = TelSmsDef[i][3];
	i=(smsCurrentPage-1)*10+1;
    if(TelSmsDef[i][0]=="no") m_smsdef_edit12 = ""; else m_smsdef_edit12 = TelSmsDef[i][0];
//    if(TelSmsDef[i][1]=="no") m_smsdef_edit22 = ""; else m_smsdef_edit22 = TelSmsDef[i][1];
    if(TelSmsDef[i][2]=="no") m_smsdef_edit32 = ""; else m_smsdef_edit32 = TelSmsDef[i][2];
 //   if(TelSmsDef[i][3]=="no") m_smsdef_edit42 = ""; else m_smsdef_edit42 = TelSmsDef[i][3];
	i=(smsCurrentPage-1)*10+2;
    if(TelSmsDef[i][0]=="no") m_smsdef_edit13 = ""; else m_smsdef_edit13 = TelSmsDef[i][0];
 //   if(TelSmsDef[i][1]=="no") m_smsdef_edit23 = ""; else m_smsdef_edit23 = TelSmsDef[i][1];
    if(TelSmsDef[i][2]=="no") m_smsdef_edit33 = ""; else m_smsdef_edit33 = TelSmsDef[i][2];
 //   if(TelSmsDef[i][3]=="no") m_smsdef_edit43 = ""; else m_smsdef_edit43 = TelSmsDef[i][3];
    i=(smsCurrentPage-1)*10+3;
	if(TelSmsDef[i][0]=="no") m_smsdef_edit14 = ""; else m_smsdef_edit14 = TelSmsDef[i][0];
//	if(TelSmsDef[i][1]=="no") m_smsdef_edit24 = ""; else m_smsdef_edit24 = TelSmsDef[i][1];
	if(TelSmsDef[i][2]=="no") m_smsdef_edit34 = ""; else m_smsdef_edit34 = TelSmsDef[i][2];
//	if(TelSmsDef[i][3]=="no") m_smsdef_edit44 = ""; else m_smsdef_edit44 = TelSmsDef[i][3];
    i=(smsCurrentPage-1)*10+4;
    if(TelSmsDef[i][0]=="no") m_smsdef_edit15 = ""; else m_smsdef_edit15 = TelSmsDef[i][0];
  //  if(TelSmsDef[i][1]=="no") m_smsdef_edit25 = ""; else m_smsdef_edit25 = TelSmsDef[i][1];
    if(TelSmsDef[i][2]=="no") m_smsdef_edit35 = ""; else m_smsdef_edit35 = TelSmsDef[i][2];
 //   if(TelSmsDef[i][3]=="no") m_smsdef_edit45 = ""; else m_smsdef_edit45 = TelSmsDef[i][3];
    i=(smsCurrentPage-1)*10+5;
    if(TelSmsDef[i][0]=="no") m_smsdef_edit16 = ""; else m_smsdef_edit16 = TelSmsDef[i][0];
 //   if(TelSmsDef[i][1]=="no") m_smsdef_edit26 = ""; else m_smsdef_edit26 = TelSmsDef[i][1];
    if(TelSmsDef[i][2]=="no") m_smsdef_edit36 = ""; else m_smsdef_edit36 = TelSmsDef[i][2];
 //   if(TelSmsDef[i][3]=="no") m_smsdef_edit46 = ""; else m_smsdef_edit46 = TelSmsDef[i][3];
    i=(smsCurrentPage-1)*10+6;
	if(TelSmsDef[i][0]=="no") m_smsdef_edit17 = ""; else m_smsdef_edit17 = TelSmsDef[i][0];
//	if(TelSmsDef[i][1]=="no") m_smsdef_edit27 = ""; else m_smsdef_edit27 = TelSmsDef[i][1];
	if(TelSmsDef[i][2]=="no") m_smsdef_edit37 = ""; else m_smsdef_edit37 = TelSmsDef[i][2];
//	if(TelSmsDef[i][3]=="no") m_smsdef_edit47 = ""; else m_smsdef_edit47 = TelSmsDef[i][3];
    i=(smsCurrentPage-1)*10+7;
    if(TelSmsDef[i][0]=="no") m_smsdef_edit18 = ""; else m_smsdef_edit18 = TelSmsDef[i][0];
 //   if(TelSmsDef[i][1]=="no") m_smsdef_edit28 = ""; else m_smsdef_edit28 = TelSmsDef[i][1];
    if(TelSmsDef[i][2]=="no") m_smsdef_edit38 = ""; else m_smsdef_edit38 = TelSmsDef[i][2];
  //  if(TelSmsDef[i][3]=="no") m_smsdef_edit48 = ""; else m_smsdef_edit48 = TelSmsDef[i][3];
    i=(smsCurrentPage-1)*10+8;
    if(TelSmsDef[i][0]=="no") m_smsdef_edit19 = ""; else m_smsdef_edit19 = TelSmsDef[i][0];
//    if(TelSmsDef[i][1]=="no") m_smsdef_edit29 = ""; else m_smsdef_edit29 = TelSmsDef[i][1];
    if(TelSmsDef[i][2]=="no") m_smsdef_edit39 = ""; else m_smsdef_edit39 = TelSmsDef[i][2];
  //  if(TelSmsDef[i][3]=="no") m_smsdef_edit49 = ""; else m_smsdef_edit49 = TelSmsDef[i][3];
    i=(smsCurrentPage-1)*10+9;
    if(TelSmsDef[i][0]=="no") m_smsdef_edit110 = ""; else m_smsdef_edit110 = TelSmsDef[i][0];
  //  if(TelSmsDef[i][1]=="no") m_smsdef_edit210 = ""; else m_smsdef_edit210 = TelSmsDef[i][1];
    if(TelSmsDef[i][2]=="no") m_smsdef_edit310 = ""; else m_smsdef_edit310 = TelSmsDef[i][2];
 //   if(TelSmsDef[i][3]=="no") m_smsdef_edit410 = ""; else m_smsdef_edit410 = TelSmsDef[i][3];

}

void CSmsDef::OnSmsdefSave() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{	  
		UpdateData(TRUE); 
		GetDataForSave();
	    UpdateData(FALSE);  			  
		SaveSmsDef();	
		//MessageBox("使用新定义文件,需要重新启动EMS系统 !!!","警告!!!",MB_ICONEXCLAMATION  );
	}	
}

void CSmsDef::GetDataForSave()
{
    int			i;
	i=(smsCurrentPage-1)*10+0 ;
	if(m_smsdef_edit11=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit11;
//	if(m_smsdef_edit21=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit21;
	if(m_smsdef_edit31=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit31;
//	if(m_smsdef_edit41=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit41;
    i=(smsCurrentPage-1)*10+1 ;
	if(m_smsdef_edit12=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit12;
//	if(m_smsdef_edit22=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit22;
	if(m_smsdef_edit32=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit32;
//	if(m_smsdef_edit42=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit42;
    i=(smsCurrentPage-1)*10+2 ;
	if(m_smsdef_edit13=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit13;
//	if(m_smsdef_edit23=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit23;
	if(m_smsdef_edit33=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit33;
//	if(m_smsdef_edit43=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit43;
    i=(smsCurrentPage-1)*10+3 ;
	if(m_smsdef_edit14=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit14;
//	if(m_smsdef_edit24=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit24;
	if(m_smsdef_edit34=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit34;
//	if(m_smsdef_edit44=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit44;
    i=(smsCurrentPage-1)*10+4 ;
	if(m_smsdef_edit15=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit15;
//	if(m_smsdef_edit25=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit25;
	if(m_smsdef_edit35=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit35;
//	if(m_smsdef_edit45=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit45;
    i=(smsCurrentPage-1)*10+5 ;
	if(m_smsdef_edit16=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit16;
//	if(m_smsdef_edit26=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit26;
	if(m_smsdef_edit36=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit36;
//	if(m_smsdef_edit46=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit46;
    i=(smsCurrentPage-1)*10+6 ;
	if(m_smsdef_edit17=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit17;
//	if(m_smsdef_edit27=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit27;
	if(m_smsdef_edit37=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit37;
//	if(m_smsdef_edit47=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit47;
    i=(smsCurrentPage-1)*10+7 ;
	if(m_smsdef_edit18=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit18;
//	if(m_smsdef_edit28=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit28;
	if(m_smsdef_edit38=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit38;
//	if(m_smsdef_edit48=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit48;
    i=(smsCurrentPage-1)*10+8 ;
	if(m_smsdef_edit19=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit19;
//	if(m_smsdef_edit29=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit29;
	if(m_smsdef_edit39=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit39;
//	if(m_smsdef_edit49=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit49;
    i=(smsCurrentPage-1)*10+9 ;
	if(m_smsdef_edit110=="") TelSmsDef[i][0]="no"; else TelSmsDef[i][0]=m_smsdef_edit110;
//	if(m_smsdef_edit210=="") TelSmsDef[i][1]="no"; else TelSmsDef[i][1]=m_smsdef_edit210;
	if(m_smsdef_edit310=="") TelSmsDef[i][2]="no"; else TelSmsDef[i][2]=m_smsdef_edit310;
//	if(m_smsdef_edit410=="") TelSmsDef[i][3]="no"; else TelSmsDef[i][3]=m_smsdef_edit410;
}

void CSmsDef::OnSmsdefGet() 
{
	// TODO: Add your control notification handler code here
	GetSmsDef();
	UpdateData(TRUE);  
	GetDataForDsp();
	UpdateData(FALSE); //刷新窗口中数据	
}

void CSmsDef::OnSmsdefDownpage() 
{
	// TODO: Add your control notification handler code here
	if( smsCurrentPage<smsPageNum )
	{ 
		smsCurrentPage++;
		GetDataForDsp();
		m_smsdef_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("下翻页结束!","警告!!!",MB_ICONSTOP );
	}	
}

void CSmsDef::OnSmsdefUppage() 
{
	// TODO: Add your control notification handler code here
	if( smsCurrentPage>1 )
	{ 
		smsCurrentPage--;
		GetDataForDsp();
		m_smsdef_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("上翻页结束!","警告!!!", MB_ICONSTOP );
	}
}

void CSmsDef::OnChangeEditPage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CSmsDef::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if( pScrollBar==&m_smsdef_scrollbar )//选择页面
	{
	  m_smsdef_scrollbar.SetScrollRange(1,smsPageNum);
	  m_smsdef_scrollbar.SetScrollPos(smsCurrentPage);
	  switch( nSBCode )
	  {
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	      smsCurrentPage = m_smsdef_scrollbar.GetScrollPos();
		  smsCurrentPage = smsCurrentPage - 1;
		  if( smsCurrentPage<1 )smsCurrentPage = 1;
		  UpdateData(FALSE);
 		  break;
	  case SB_LINERIGHT: //单击滚动条向右箭头  
 	      smsCurrentPage = m_smsdef_scrollbar.GetScrollPos();
		  smsCurrentPage = smsCurrentPage + 1;
		  if( smsCurrentPage>smsPageNum )smsCurrentPage = smsPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEUP: //单击向左按钮和滑块之间的区域 
 	      smsCurrentPage = m_smsdef_scrollbar.GetScrollPos();
		  smsCurrentPage = smsCurrentPage - 3;
		  if( smsCurrentPage<1 )smsCurrentPage = 1;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEDOWN: //单击向右按钮和滑块之间的区域 
 	      smsCurrentPage = m_smsdef_scrollbar.GetScrollPos();
		  smsCurrentPage = smsCurrentPage + 3;
		  if( smsCurrentPage>smsPageNum )smsCurrentPage = smsPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_THUMBTRACK: //拖动滑块
          m_smsdef_scrollbar.SetScrollPos(nPos);
		  smsCurrentPage =  nPos; 	
		  UpdateData(FALSE);
 		  break;	  

	  }
	  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	  GetDataForDsp();
	  UpdateData(FALSE);  
	}
}

BOOL CSmsDef::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	OnSmsdefGet()  ;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsDef::OnChangeSmsdefEdit21() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
