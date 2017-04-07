// SmsSend.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/SmsSend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmsSend dialog


CSmsSend::CSmsSend(CWnd* pParent /*=NULL*/)
	: CDialog(CSmsSend::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmsSend)
	m_edit_page = _T("");

/*	m_smssend_edit11 = _T("");
	m_smssend_edit110 = _T("");
	m_smssend_edit12 = _T("");
	m_smssend_edit13 = _T("");
	m_smssend_edit14 = _T("");
	m_smssend_edit15 = _T("");
	m_smssend_edit16 = _T("");
	m_smssend_edit17 = _T("");
	m_smssend_edit18 = _T("");
	m_smssend_edit19 = _T("");
*/
	m_smssend_edit21 = _T("");
	m_smssend_edit210 = _T("");
	m_smssend_edit22 = _T("");
	m_smssend_edit23 = _T("");
	m_smssend_edit24 = _T("");
	m_smssend_edit25 = _T("");
	m_smssend_edit26 = _T("");
	m_smssend_edit27 = _T("");
	m_smssend_edit28 = _T("");
	m_smssend_edit29 = _T("");

/*	m_smssend_edit31 = _T("");
	m_smssend_edit310 = _T("");
	m_smssend_edit32 = _T("");
	m_smssend_edit33 = _T("");
	m_smssend_edit34 = _T("");
	m_smssend_edit35 = _T("");
	m_smssend_edit36 = _T("");
	m_smssend_edit37 = _T("");
	m_smssend_edit38 = _T("");
	m_smssend_edit39 = _T("");
*/
	//}}AFX_DATA_INIT
}


void CSmsSend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmsSend)
	DDX_Control(pDX, IDC_SMSSEND_SCROLLBAR, m_smssend_scrollbar);
	DDX_Control(pDX, ID_SMSSEND_SAVE, m_smssend_save);
	DDX_Control(pDX, ID_SMSSEND_GET, m_smssend_get);
	DDX_Text(pDX, IDC_EDIT_PAGE, m_edit_page);
/*	DDX_Text(pDX, IDC_SMSSEND_EDIT11, m_smssend_edit11);
	DDX_Text(pDX, IDC_SMSSEND_EDIT110, m_smssend_edit110);
	DDX_Text(pDX, IDC_SMSSEND_EDIT12, m_smssend_edit12);
	DDX_Text(pDX, IDC_SMSSEND_EDIT13, m_smssend_edit13);
	DDX_Text(pDX, IDC_SMSSEND_EDIT14, m_smssend_edit14);
	DDX_Text(pDX, IDC_SMSSEND_EDIT15, m_smssend_edit15);
	DDX_Text(pDX, IDC_SMSSEND_EDIT16, m_smssend_edit16);
	DDX_Text(pDX, IDC_SMSSEND_EDIT17, m_smssend_edit17);
	DDX_Text(pDX, IDC_SMSSEND_EDIT18, m_smssend_edit18);
	DDX_Text(pDX, IDC_SMSSEND_EDIT19, m_smssend_edit19);
*/
	DDX_Text(pDX, IDC_SMSSEND_EDIT21, m_smssend_edit21);
	DDX_Text(pDX, IDC_SMSSEND_EDIT210, m_smssend_edit210);
	DDX_Text(pDX, IDC_SMSSEND_EDIT22, m_smssend_edit22);
	DDX_Text(pDX, IDC_SMSSEND_EDIT23, m_smssend_edit23);
	DDX_Text(pDX, IDC_SMSSEND_EDIT24, m_smssend_edit24);
	DDX_Text(pDX, IDC_SMSSEND_EDIT25, m_smssend_edit25);
	DDX_Text(pDX, IDC_SMSSEND_EDIT26, m_smssend_edit26);
	DDX_Text(pDX, IDC_SMSSEND_EDIT27, m_smssend_edit27);
	DDX_Text(pDX, IDC_SMSSEND_EDIT28, m_smssend_edit28);
	DDX_Text(pDX, IDC_SMSSEND_EDIT29, m_smssend_edit29);

/*	DDX_Text(pDX, IDC_SMSSEND_EDIT31, m_smssend_edit31);
	DDX_Text(pDX, IDC_SMSSEND_EDIT310, m_smssend_edit310);
	DDX_Text(pDX, IDC_SMSSEND_EDIT32, m_smssend_edit32);
	DDX_Text(pDX, IDC_SMSSEND_EDIT33, m_smssend_edit33);
	DDX_Text(pDX, IDC_SMSSEND_EDIT34, m_smssend_edit34);
	DDX_Text(pDX, IDC_SMSSEND_EDIT35, m_smssend_edit35);
	DDX_Text(pDX, IDC_SMSSEND_EDIT36, m_smssend_edit36);
	DDX_Text(pDX, IDC_SMSSEND_EDIT37, m_smssend_edit37);
	DDX_Text(pDX, IDC_SMSSEND_EDIT38, m_smssend_edit38);
	DDX_Text(pDX, IDC_SMSSEND_EDIT39, m_smssend_edit39);
*/
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmsSend, CDialog)
	//{{AFX_MSG_MAP(CSmsSend)
	ON_BN_CLICKED(ID_SMSSEND_SAVE, OnSmssendSave)
	ON_BN_CLICKED(ID_SMSSEND_GET, OnSmssendGet)
	ON_BN_CLICKED(ID_SMSSEND_UPPAGE, OnSmssendUppage)
	ON_BN_CLICKED(ID_SMSSEND_DOWNPAGE, OnSmssendDownpage)
	ON_EN_CHANGE(IDC_EDIT_PAGE, OnChangeEditPage)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_SMSSEND_EDIT18, OnChangeSmssendEdit18)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmsSend message handlers
extern int 	smssendCurPag,	smssendPageNum;
extern CString	TelSmsSend[MAX_SMSSEND_NUM][3];

void CSmsSend::GetDataForDsp()
{  
	char		strNum[200];
	int			i;

	sprintf(strNum,"No %d (Total %d)",smssendCurPag,smssendPageNum); m_edit_page = strNum;
    i=(smssendCurPag-1)*10+0;
	if (i< 0  ) return ; 
//	if(TelSmsSend[i][0]=="no") m_smssend_edit11 = ""; else m_smssend_edit11 = TelSmsSend[i][0];
	if(TelSmsSend[i][1]=="no") m_smssend_edit21 = ""; else m_smssend_edit21 = TelSmsSend[i][1];
//	if(TelSmsSend[i][2]=="no") m_smssend_edit31 = ""; else m_smssend_edit31 = TelSmsSend[i][2];
	i=(smssendCurPag-1)*10+1;
 //   if(TelSmsSend[i][0]=="no") m_smssend_edit12 = ""; else m_smssend_edit12 = TelSmsSend[i][0];
    if(TelSmsSend[i][1]=="no") m_smssend_edit22 = ""; else m_smssend_edit22 = TelSmsSend[i][1];
  //  if(TelSmsSend[i][2]=="no") m_smssend_edit32 = ""; else m_smssend_edit32 = TelSmsSend[i][2];
	i=(smssendCurPag-1)*10+2;
 //   if(TelSmsSend[i][0]=="no") m_smssend_edit13 = ""; else m_smssend_edit13 = TelSmsSend[i][0];
    if(TelSmsSend[i][1]=="no") m_smssend_edit23 = ""; else m_smssend_edit23 = TelSmsSend[i][1];
 //   if(TelSmsSend[i][2]=="no") m_smssend_edit33 = ""; else m_smssend_edit33 = TelSmsSend[i][2];
    i=(smssendCurPag-1)*10+3;
//	if(TelSmsSend[i][0]=="no") m_smssend_edit14 = ""; else m_smssend_edit14 = TelSmsSend[i][0];
	if(TelSmsSend[i][1]=="no") m_smssend_edit24 = ""; else m_smssend_edit24 = TelSmsSend[i][1];
//	if(TelSmsSend[i][2]=="no") m_smssend_edit34 = ""; else m_smssend_edit34 = TelSmsSend[i][2];
    i=(smssendCurPag-1)*10+4;
 //   if(TelSmsSend[i][0]=="no") m_smssend_edit15 = ""; else m_smssend_edit15 = TelSmsSend[i][0];
    if(TelSmsSend[i][1]=="no") m_smssend_edit25 = ""; else m_smssend_edit25 = TelSmsSend[i][1];
//    if(TelSmsSend[i][2]=="no") m_smssend_edit35 = ""; else m_smssend_edit35 = TelSmsSend[i][2];
    i=(smssendCurPag-1)*10+5;
 //   if(TelSmsSend[i][0]=="no") m_smssend_edit16 = ""; else m_smssend_edit16 = TelSmsSend[i][0];
  if(TelSmsSend[i][1]=="no") m_smssend_edit26 = ""; else m_smssend_edit26 = TelSmsSend[i][1];
 //   if(TelSmsSend[i][2]=="no") m_smssend_edit36 = ""; else m_smssend_edit36 = TelSmsSend[i][2];
    i=(smssendCurPag-1)*10+6;
//	if(TelSmsSend[i][0]=="no") m_smssend_edit17 = ""; else m_smssend_edit17 = TelSmsSend[i][0];
	if(TelSmsSend[i][1]=="no") m_smssend_edit27 = ""; else m_smssend_edit27 = TelSmsSend[i][1];
//	if(TelSmsSend[i][2]=="no") m_smssend_edit37 = ""; else m_smssend_edit37 = TelSmsSend[i][2];
    i=(smssendCurPag-1)*10+7;
 //   if(TelSmsSend[i][0]=="no") m_smssend_edit18 = ""; else m_smssend_edit18 = TelSmsSend[i][0];
    if(TelSmsSend[i][1]=="no") m_smssend_edit28 = ""; else m_smssend_edit28 = TelSmsSend[i][1];
//    if(TelSmsSend[i][2]=="no") m_smssend_edit38 = ""; else m_smssend_edit38 = TelSmsSend[i][2];
    i=(smssendCurPag-1)*10+8;
//    if(TelSmsSend[i][0]=="no") m_smssend_edit19 = ""; else m_smssend_edit19 = TelSmsSend[i][0];
    if(TelSmsSend[i][1]=="no") m_smssend_edit29 = ""; else m_smssend_edit29 = TelSmsSend[i][1];
//    if(TelSmsSend[i][2]=="no") m_smssend_edit39 = ""; else m_smssend_edit39 = TelSmsSend[i][2];
    i=(smssendCurPag-1)*10+9;
 //   if(TelSmsSend[i][0]=="no") m_smssend_edit110 = ""; else m_smssend_edit110 = TelSmsSend[i][0];
    if(TelSmsSend[i][1]=="no") m_smssend_edit210 = ""; else m_smssend_edit210 = TelSmsSend[i][1];
//    if(TelSmsSend[i][2]=="no") m_smssend_edit310 = ""; else m_smssend_edit310 = TelSmsSend[i][2];
}

void CSmsSend::GetDataForSave()
{
    int			i;
	i=(smssendCurPag-1)*10+0 ;
//	if(m_smssend_edit11=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit11;
	if(m_smssend_edit21=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit21;
//	if(m_smssend_edit31=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit31;
    i=(smssendCurPag-1)*10+1 ;
//	if(m_smssend_edit12=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit12;
	if(m_smssend_edit22=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit22;
//	if(m_smssend_edit32=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit32;
    i=(smssendCurPag-1)*10+2 ;
//	if(m_smssend_edit13=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit13;
	if(m_smssend_edit23=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit23;
//	if(m_smssend_edit33=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit33;
    i=(smssendCurPag-1)*10+3 ;
//	if(m_smssend_edit14=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit14;
	if(m_smssend_edit24=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit24;
//	if(m_smssend_edit34=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit34;
    i=(smssendCurPag-1)*10+4 ;
//	if(m_smssend_edit15=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit15;
	if(m_smssend_edit25=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit25;
//	if(m_smssend_edit35=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit35;
    i=(smssendCurPag-1)*10+5 ;
//	if(m_smssend_edit16=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit16;
	if(m_smssend_edit26=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit26;
//	if(m_smssend_edit36=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit36;
    i=(smssendCurPag-1)*10+6 ;
//	if(m_smssend_edit17=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit17;
	if(m_smssend_edit27=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit27;
//	if(m_smssend_edit37=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit37;
    i=(smssendCurPag-1)*10+7 ;
//	if(m_smssend_edit18=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit18;
	if(m_smssend_edit28=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit28;
//	if(m_smssend_edit38=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit38;
    i=(smssendCurPag-1)*10+8 ;
//	if(m_smssend_edit19=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit19;
	if(m_smssend_edit29=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit29;
//	if(m_smssend_edit39=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit39;
    i=(smssendCurPag-1)*10+9 ;
//	if(m_smssend_edit110=="") TelSmsSend[i][0]="no"; else TelSmsSend[i][0]=m_smssend_edit110;
	if(m_smssend_edit210=="") TelSmsSend[i][1]="no"; else TelSmsSend[i][1]=m_smssend_edit210;
//	if(m_smssend_edit310=="") TelSmsSend[i][2]="no"; else TelSmsSend[i][2]=m_smssend_edit310;
}

void CSmsSend::OnSmssendSave() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{	  
		UpdateData(TRUE); 
		GetDataForSave();
	    UpdateData(FALSE);  			  
		SaveSmsSend();	
		//MessageBox("使用新定义文件,需要重新启动EMS系统 !!!","警告!!!",MB_ICONEXCLAMATION  );
	}	
	
}

void CSmsSend::OnSmssendGet() 
{
	// TODO: Add your control notification handler code here
	GetSmsSend();
	UpdateData(TRUE);  
	GetDataForDsp();
	UpdateData(FALSE); //刷新窗口中数据	
	
}

void CSmsSend::OnSmssendUppage() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if( smssendCurPag>1 )
	{ 
		smssendCurPag--;
		GetDataForDsp();
		m_smssend_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("上翻页结束!","警告!!!", MB_ICONSTOP );
	}	
}

void CSmsSend::OnSmssendDownpage() 
{
	// TODO: Add your control notification handler code here
	if( smssendCurPag<smssendPageNum )
	{ 
		smssendCurPag++;
		GetDataForDsp();
		m_smssend_scrollbar.SetScrollPos(1);
		UpdateData(FALSE);  
	}
	else
	{
		MessageBox("下翻页结束!","警告!!!",MB_ICONSTOP );
	}	
}

void CSmsSend::OnChangeEditPage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CSmsSend::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if( pScrollBar==&m_smssend_scrollbar )//选择页面
	{
	  m_smssend_scrollbar.SetScrollRange(1,smssendPageNum);
	  m_smssend_scrollbar.SetScrollPos(smssendCurPag);
	  switch( nSBCode )
	  {
	  case SB_LINELEFT:  //单击滚动条向左箭头 
	      smssendCurPag = m_smssend_scrollbar.GetScrollPos();
		  smssendCurPag = smssendCurPag - 1;
		  if( smssendCurPag<1 )smssendCurPag = 1;
		  UpdateData(FALSE);
 		  break;
	  case SB_LINERIGHT: //单击滚动条向右箭头  
 	      smssendCurPag = m_smssend_scrollbar.GetScrollPos();
		  smssendCurPag = smssendCurPag + 1;
		  if( smssendCurPag>smssendPageNum )smssendCurPag = smssendPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEUP: //单击向左按钮和滑块之间的区域 
 	      smssendCurPag = m_smssend_scrollbar.GetScrollPos();
		  smssendCurPag = smssendCurPag - 3;
		  if( smssendCurPag<1 )smssendCurPag = 1;
		  UpdateData(FALSE); 
          break;
	  case SB_PAGEDOWN: //单击向右按钮和滑块之间的区域 
 	      smssendCurPag = m_smssend_scrollbar.GetScrollPos();
		  smssendCurPag = smssendCurPag + 3;
		  if( smssendCurPag>smssendPageNum )smssendCurPag = smssendPageNum;
		  UpdateData(FALSE); 
          break;
	  case SB_THUMBTRACK: //拖动滑块
          m_smssend_scrollbar.SetScrollPos(nPos);
		  smssendCurPag =  nPos; 	
		  UpdateData(FALSE);
 		  break;	  

	  }		
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  	  GetDataForDsp();
	  UpdateData(FALSE);  
	}
}

BOOL CSmsSend::OnInitDialog() 
{
	CDialog::OnInitDialog();
	OnSmssendGet()  ;
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSmsSend::OnChangeSmssendEdit18() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
