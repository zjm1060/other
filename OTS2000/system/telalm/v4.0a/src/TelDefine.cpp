// TelDefine.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/TelDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString	TelDef[MAX_TELDEF_NUM][MAX_TELDEF_STY];

/////////////////////////////////////////////////////////////////////////////
// CTelDefine dialog


CTelDefine::CTelDefine(CWnd* pParent /*=NULL*/)
	: CDialog(CTelDefine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelDefine)
	m_edit_page = _T("");
	m_teldef_edit11 = _T("");
	m_teldef_edit110 = _T("");
	m_teldef_edit12 = _T("");
	m_teldef_edit13 = _T("");
	m_teldef_edit14 = _T("");
	m_teldef_edit15 = _T("");
	m_teldef_edit16 = _T("");
	m_teldef_edit17 = _T("");
	m_teldef_edit18 = _T("");
	m_teldef_edit19 = _T("");
/*	m_teldef_edit21 = _T("");
	m_teldef_edit210 = _T("");
	m_teldef_edit22 = _T("");
	m_teldef_edit23 = _T("");
	m_teldef_edit24 = _T("");
	m_teldef_edit25 = _T("");
	m_teldef_edit26 = _T("");
	m_teldef_edit27 = _T("");
	m_teldef_edit28 = _T("");
	m_teldef_edit29 = _T("");
*/
	m_teldef_edit31 = _T("");
	m_teldef_edit310 = _T("");
	m_teldef_edit32 = _T("");
	m_teldef_edit33 = _T("");
	m_teldef_edit34 = _T("");
	m_teldef_edit35 = _T("");
	m_teldef_edit36 = _T("");
	m_teldef_edit37 = _T("");
	m_teldef_edit38 = _T("");
	m_teldef_edit39 = _T("");
/*	m_teldef_edit41 = _T("");
	m_teldef_edit410 = _T("");
	m_teldef_edit42 = _T("");
	m_teldef_edit43 = _T("");
	m_teldef_edit44 = _T("");
	m_teldef_edit45 = _T("");
	m_teldef_edit46 = _T("");
	m_teldef_edit47 = _T("");
	m_teldef_edit48 = _T("");
	m_teldef_edit49 = _T("");
*/
	//}}AFX_DATA_INIT
}


void CTelDefine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelDefine)
	DDX_Control(pDX, ID_TELDEF_SAVE, m_teldef_save);
	DDX_Control(pDX, ID_TELDEF_GET, m_teldef_get);
	DDX_Control(pDX, IDC_TELDEF_SCROLLBAR, m_teldef_scrollbar);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_edit_page);
	DDX_Text(pDX, IDC_TELDEF_EDIT11, m_teldef_edit11);
	DDX_Text(pDX, IDC_TELDEF_EDIT110, m_teldef_edit110);
	DDX_Text(pDX, IDC_TELDEF_EDIT12, m_teldef_edit12);
	DDX_Text(pDX, IDC_TELDEF_EDIT13, m_teldef_edit13);
	DDX_Text(pDX, IDC_TELDEF_EDIT14, m_teldef_edit14);
	DDX_Text(pDX, IDC_TELDEF_EDIT15, m_teldef_edit15);
	DDX_Text(pDX, IDC_TELDEF_EDIT16, m_teldef_edit16);
	DDX_Text(pDX, IDC_TELDEF_EDIT17, m_teldef_edit17);
	DDX_Text(pDX, IDC_TELDEF_EDIT18, m_teldef_edit18);
	DDX_Text(pDX, IDC_TELDEF_EDIT19, m_teldef_edit19);

/*	DDX_Text(pDX, IDC_TELDEF_EDIT21, m_teldef_edit21);
	DDX_Text(pDX, IDC_TELDEF_EDIT210, m_teldef_edit210);
	DDX_Text(pDX, IDC_TELDEF_EDIT22, m_teldef_edit22);
	DDX_Text(pDX, IDC_TELDEF_EDIT23, m_teldef_edit23);
	DDX_Text(pDX, IDC_TELDEF_EDIT24, m_teldef_edit24);
	DDX_Text(pDX, IDC_TELDEF_EDIT25, m_teldef_edit25);
	DDX_Text(pDX, IDC_TELDEF_EDIT26, m_teldef_edit26);
	DDX_Text(pDX, IDC_TELDEF_EDIT27, m_teldef_edit27);
	DDX_Text(pDX, IDC_TELDEF_EDIT28, m_teldef_edit28);
	DDX_Text(pDX, IDC_TELDEF_EDIT29, m_teldef_edit29);
*/
	DDX_Text(pDX, IDC_TELDEF_EDIT31, m_teldef_edit31);
	DDX_Text(pDX, IDC_TELDEF_EDIT310, m_teldef_edit310);
	DDX_Text(pDX, IDC_TELDEF_EDIT32, m_teldef_edit32);
	DDX_Text(pDX, IDC_TELDEF_EDIT33, m_teldef_edit33);
	DDX_Text(pDX, IDC_TELDEF_EDIT34, m_teldef_edit34);
	DDX_Text(pDX, IDC_TELDEF_EDIT35, m_teldef_edit35);
	DDX_Text(pDX, IDC_TELDEF_EDIT36, m_teldef_edit36);
	DDX_Text(pDX, IDC_TELDEF_EDIT37, m_teldef_edit37);
	DDX_Text(pDX, IDC_TELDEF_EDIT38, m_teldef_edit38);
	DDX_Text(pDX, IDC_TELDEF_EDIT39, m_teldef_edit39);

/*	DDX_Text(pDX, IDC_TELDEF_EDIT41, m_teldef_edit41);
	DDX_Text(pDX, IDC_TELDEF_EDIT410, m_teldef_edit410);
	DDX_Text(pDX, IDC_TELDEF_EDIT42, m_teldef_edit42);
	DDX_Text(pDX, IDC_TELDEF_EDIT43, m_teldef_edit43);
	DDX_Text(pDX, IDC_TELDEF_EDIT44, m_teldef_edit44);
	DDX_Text(pDX, IDC_TELDEF_EDIT45, m_teldef_edit45);
	DDX_Text(pDX, IDC_TELDEF_EDIT46, m_teldef_edit46);
	DDX_Text(pDX, IDC_TELDEF_EDIT47, m_teldef_edit47);
	DDX_Text(pDX, IDC_TELDEF_EDIT48, m_teldef_edit48);
	DDX_Text(pDX, IDC_TELDEF_EDIT49, m_teldef_edit49);
*/
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelDefine, CDialog)
	//{{AFX_MSG_MAP(CTelDefine)
	ON_BN_CLICKED(ID_TELDEF_GET, OnTeldefGet)
	ON_BN_CLICKED(ID_TELDEF_SAVE, OnTeldefSave)
	ON_BN_CLICKED(ID_TELDEF_DOWNPAGE, OnTeldefDownpage)
	ON_BN_CLICKED(ID_TELDEF_UPPAGE, OnTeldefUppage)
	ON_EN_CHANGE(IDC_EDIT_PAGE, OnChangeEditPage)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelDefine message handlers
extern int 	telPageNum,	telCurrentPage;

void CTelDefine::GetDataForDsp()
{  
	char		strNum[200];
	int			i;

	sprintf(strNum,"No %d (Total %d)",telCurrentPage,telPageNum); m_edit_page = strNum;
    i=(telCurrentPage-1)*10+0;
	if (i< 0  ) return ; 
	if(TelDef[i][0]=="no") m_teldef_edit11 = ""; else m_teldef_edit11 = TelDef[i][0];
//	if(TelDef[i][1]=="no") m_teldef_edit21 = ""; else m_teldef_edit21 = TelDef[i][1];
	if(TelDef[i][2]=="no") m_teldef_edit31 = ""; else m_teldef_edit31 = TelDef[i][2];
//	if(TelDef[i][3]=="no") m_teldef_edit41 = ""; else m_teldef_edit41 = TelDef[i][3];
	i=(telCurrentPage-1)*10+1;
    if(TelDef[i][0]=="no") m_teldef_edit12 = ""; else m_teldef_edit12 = TelDef[i][0];
//    if(TelDef[i][1]=="no") m_teldef_edit22 = ""; else m_teldef_edit22 = TelDef[i][1];
    if(TelDef[i][2]=="no") m_teldef_edit32 = ""; else m_teldef_edit32 = TelDef[i][2];
//    if(TelDef[i][3]=="no") m_teldef_edit42 = ""; else m_teldef_edit42 = TelDef[i][3];
	i=(telCurrentPage-1)*10+2;
    if(TelDef[i][0]=="no") m_teldef_edit13 = ""; else m_teldef_edit13 = TelDef[i][0];
 //   if(TelDef[i][1]=="no") m_teldef_edit23 = ""; else m_teldef_edit23 = TelDef[i][1];
    if(TelDef[i][2]=="no") m_teldef_edit33 = ""; else m_teldef_edit33 = TelDef[i][2];
  //  if(TelDef[i][3]=="no") m_teldef_edit43 = ""; else m_teldef_edit43 = TelDef[i][3];
    i=(telCurrentPage-1)*10+3;
	if(TelDef[i][0]=="no") m_teldef_edit14 = ""; else m_teldef_edit14 = TelDef[i][0];
//	if(TelDef[i][1]=="no") m_teldef_edit24 = ""; else m_teldef_edit24 = TelDef[i][1];
	if(TelDef[i][2]=="no") m_teldef_edit34 = ""; else m_teldef_edit34 = TelDef[i][2];
//	if(TelDef[i][3]=="no") m_teldef_edit44 = ""; else m_teldef_edit44 = TelDef[i][3];
    i=(telCurrentPage-1)*10+4;
    if(TelDef[i][0]=="no") m_teldef_edit15 = ""; else m_teldef_edit15 = TelDef[i][0];
  //  if(TelDef[i][1]=="no") m_teldef_edit25 = ""; else m_teldef_edit25 = TelDef[i][1];
    if(TelDef[i][2]=="no") m_teldef_edit35 = ""; else m_teldef_edit35 = TelDef[i][2];
 //   if(TelDef[i][3]=="no") m_teldef_edit45 = ""; else m_teldef_edit45 = TelDef[i][3];
    i=(telCurrentPage-1)*10+5;
    if(TelDef[i][0]=="no") m_teldef_edit16 = ""; else m_teldef_edit16 = TelDef[i][0];
   // if(TelDef[i][1]=="no") m_teldef_edit26 = ""; else m_teldef_edit26 = TelDef[i][1];
    if(TelDef[i][2]=="no") m_teldef_edit36 = ""; else m_teldef_edit36 = TelDef[i][2];
 //   if(TelDef[i][3]=="no") m_teldef_edit46 = ""; else m_teldef_edit46 = TelDef[i][3];
    i=(telCurrentPage-1)*10+6;
	if(TelDef[i][0]=="no") m_teldef_edit17 = ""; else m_teldef_edit17 = TelDef[i][0];
//	if(TelDef[i][1]=="no") m_teldef_edit27 = ""; else m_teldef_edit27 = TelDef[i][1];
	if(TelDef[i][2]=="no") m_teldef_edit37 = ""; else m_teldef_edit37 = TelDef[i][2];
//	if(TelDef[i][3]=="no") m_teldef_edit47 = ""; else m_teldef_edit47 = TelDef[i][3];
    i=(telCurrentPage-1)*10+7;
    if(TelDef[i][0]=="no") m_teldef_edit18 = ""; else m_teldef_edit18 = TelDef[i][0];
  //  if(TelDef[i][1]=="no") m_teldef_edit28 = ""; else m_teldef_edit28 = TelDef[i][1];
    if(TelDef[i][2]=="no") m_teldef_edit38 = ""; else m_teldef_edit38 = TelDef[i][2];
 //   if(TelDef[i][3]=="no") m_teldef_edit48 = ""; else m_teldef_edit48 = TelDef[i][3];
    i=(telCurrentPage-1)*10+8;
    if(TelDef[i][0]=="no") m_teldef_edit19 = ""; else m_teldef_edit19 = TelDef[i][0];
//    if(TelDef[i][1]=="no") m_teldef_edit29 = ""; else m_teldef_edit29 = TelDef[i][1];
    if(TelDef[i][2]=="no") m_teldef_edit39 = ""; else m_teldef_edit39 = TelDef[i][2];
 //   if(TelDef[i][3]=="no") m_teldef_edit49 = ""; else m_teldef_edit49 = TelDef[i][3];
    i=(telCurrentPage-1)*10+9;
    if(TelDef[i][0]=="no") m_teldef_edit110 = ""; else m_teldef_edit110 = TelDef[i][0];
 //   if(TelDef[i][1]=="no") m_teldef_edit210 = ""; else m_teldef_edit210 = TelDef[i][1];
    if(TelDef[i][2]=="no") m_teldef_edit310 = ""; else m_teldef_edit310 = TelDef[i][2];
   // if(TelDef[i][3]=="no") m_teldef_edit410 = ""; else m_teldef_edit410 = TelDef[i][3];

}

void CTelDefine::GetDataForSave()
{
    int			i;
	i=(telCurrentPage-1)*10+0 ;
	if(m_teldef_edit11=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit11;
	if(m_teldef_edit21=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit21;
	if(m_teldef_edit31=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit31;
	if(m_teldef_edit41=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit41;
    i=(telCurrentPage-1)*10+1 ;
	if(m_teldef_edit12=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit12;
	if(m_teldef_edit22=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit22;
	if(m_teldef_edit32=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit32;
	if(m_teldef_edit42=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit42;
    i=(telCurrentPage-1)*10+2 ;
	if(m_teldef_edit13=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit13;
	if(m_teldef_edit23=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit23;
	if(m_teldef_edit33=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit33;
	if(m_teldef_edit43=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit43;
    i=(telCurrentPage-1)*10+3 ;
	if(m_teldef_edit14=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit14;
	if(m_teldef_edit24=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit24;
	if(m_teldef_edit34=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit34;
	if(m_teldef_edit44=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit44;
    i=(telCurrentPage-1)*10+4 ;
	if(m_teldef_edit15=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit15;
	if(m_teldef_edit25=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit25;
	if(m_teldef_edit35=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit35;
	if(m_teldef_edit45=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit45;
    i=(telCurrentPage-1)*10+5 ;
	if(m_teldef_edit16=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit16;
	if(m_teldef_edit26=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit26;
	if(m_teldef_edit36=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit36;
	if(m_teldef_edit46=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit46;
    i=(telCurrentPage-1)*10+6 ;
	if(m_teldef_edit17=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit17;
	if(m_teldef_edit27=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit27;
	if(m_teldef_edit37=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit37;
	if(m_teldef_edit47=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit47;
    i=(telCurrentPage-1)*10+7 ;
	if(m_teldef_edit18=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit18;
	if(m_teldef_edit28=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit28;
	if(m_teldef_edit38=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit38;
	if(m_teldef_edit48=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit48;
    i=(telCurrentPage-1)*10+8 ;
	if(m_teldef_edit19=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit19;
	if(m_teldef_edit29=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit29;
	if(m_teldef_edit39=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit39;
	if(m_teldef_edit49=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit49;
    i=(telCurrentPage-1)*10+9 ;
	if(m_teldef_edit110=="") TelDef[i][0]="no"; else TelDef[i][0]=m_teldef_edit110;
	if(m_teldef_edit210=="") TelDef[i][1]="no"; else TelDef[i][1]=m_teldef_edit210;
	if(m_teldef_edit310=="") TelDef[i][2]="no"; else TelDef[i][2]=m_teldef_edit310;
	if(m_teldef_edit410=="") TelDef[i][3]="no"; else TelDef[i][3]=m_teldef_edit410;
}

void CTelDefine::OnTeldefGet() 
{
	// TODO: Add your control notification handler code here
	GetTelDef();
	UpdateData(TRUE);  
	GetDataForDsp();
	UpdateData(FALSE); //刷新窗口中数据	
}

void CTelDefine::OnTeldefSave() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{	  
		UpdateData(TRUE); 
		GetDataForSave();
	    UpdateData(FALSE);  			  
		SaveTelDef();	
		//MessageBox("使用新定义文件,需要重新启动EMS系统 !!!","警告!!!",MB_ICONEXCLAMATION  );
	}		
}

void CTelDefine::OnTeldefDownpage() 
{
	// TODO: Add your control notification handler code here
	if( telCurrentPage<telPageNum )
	{ 
		telCurrentPage++;
		GetDataForDsp();
		m_teldef_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("下翻页结束!","警告!!!",MB_ICONSTOP );
	}		
}

void CTelDefine::OnTeldefUppage() 
{
	// TODO: Add your control notification handler code here
	if( telCurrentPage>1 )
	{ 
		telCurrentPage--;
		GetDataForDsp();
		m_teldef_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("上翻页结束!","警告!!!", MB_ICONSTOP );
	}	
}

void CTelDefine::OnChangeEditPage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CTelDefine::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if( pScrollBar==&m_teldef_scrollbar )//选择页面
	{
	  m_teldef_scrollbar.SetScrollRange(1,telPageNum);
	  m_teldef_scrollbar.SetScrollPos(telCurrentPage);
	  switch( nSBCode )
	  {
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	      telCurrentPage = m_teldef_scrollbar.GetScrollPos();
		  telCurrentPage = telCurrentPage - 1;
		  if( telCurrentPage<1 )telCurrentPage = 1;
		  UpdateData(FALSE);
 		  break;
	  case SB_LINERIGHT: //单击滚动条向右箭头  
 	      telCurrentPage = m_teldef_scrollbar.GetScrollPos();
		  telCurrentPage = telCurrentPage + 1;
		  if( telCurrentPage>telPageNum )telCurrentPage = telPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEUP: //单击向左按钮和滑块之间的区域 
 	      telCurrentPage = m_teldef_scrollbar.GetScrollPos();
		  telCurrentPage = telCurrentPage - 3;
		  if( telCurrentPage<1 )telCurrentPage = 1;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEDOWN: //单击向右按钮和滑块之间的区域 
 	      telCurrentPage = m_teldef_scrollbar.GetScrollPos();
		  telCurrentPage = telCurrentPage + 3;
		  if( telCurrentPage>telPageNum )telCurrentPage = telPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_THUMBTRACK: //拖动滑块
          m_teldef_scrollbar.SetScrollPos(nPos);
		  telCurrentPage =  nPos; 	
		  UpdateData(FALSE);
 		  break;	  

	  }
	  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	  GetDataForDsp();
	  UpdateData(FALSE);  
	}
}

BOOL CTelDefine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	OnTeldefGet()  ;
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
