// TelSms.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/TelSms.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTelSms property page

IMPLEMENT_DYNCREATE(CTelSms, CPropertyPage)

CTelSms::CTelSms() : CPropertyPage(CTelSms::IDD)
{
	//{{AFX_DATA_INIT(CTelSms)
	m_telsms_edit11 = _T("");
	m_telsms_edit12 = _T("");
	m_telsms_edit13 = _T("");
	m_telsms_edit14 = _T("");
	m_telsms_edit21 = _T("");
	m_telsms_edit22 = _T("");
	m_telsms_edit23 = _T("");
	m_telsms_edit24 = _T("");
	m_telsms_edit31 = _T("");
	m_telsms_edit32 = _T("");
	m_telsms_edit33 = _T("");
	m_telsms_edit34 = _T("");
	m_telsms_edit41 = _T("");
	m_telsms_edit42 = _T("");
	m_telsms_edit43 = _T("");
	m_telsms_edit44 = _T("");
	m_telsms_edit110 = _T("");
	m_telsms_edit111 = _T("");
	m_telsms_edit112 = _T("");
	m_telsms_edit113 = _T("");
	m_telsms_edit114 = _T("");
	m_telsms_edit115 = _T("");
	m_telsms_edit116 = _T("");
	m_telsms_edit15 = _T("");
	m_telsms_edit16 = _T("");
	m_telsms_edit17 = _T("");
	m_telsms_edit18 = _T("");
	m_telsms_edit19 = _T("");
	m_telsms_edit210 = _T("");
	m_telsms_edit211 = _T("");
	m_telsms_edit212 = _T("");
	m_telsms_edit213 = _T("");
	m_telsms_edit214 = _T("");
	m_telsms_edit216 = _T("");
	m_telsms_edit215 = _T("");
	m_telsms_edit25 = _T("");
	m_telsms_edit26 = _T("");
	m_telsms_edit27 = _T("");
	m_telsms_edit28 = _T("");
	m_telsms_edit29 = _T("");
	m_telsms_edit310 = _T("");
	m_telsms_edit311 = _T("");
	m_telsms_edit312 = _T("");
	m_telsms_edit313 = _T("");
	m_telsms_edit314 = _T("");
	m_telsms_edit315 = _T("");
	m_telsms_edit316 = _T("");
	m_telsms_edit35 = _T("");
	m_telsms_edit36 = _T("");
	m_telsms_edit37 = _T("");
	m_telsms_edit38 = _T("");
	m_telsms_edit39 = _T("");
	m_telsms_edit410 = _T("");
	m_telsms_edit411 = _T("");
	m_telsms_edit412 = _T("");
	m_telsms_edit413 = _T("");
	m_telsms_edit414 = _T("");
	m_telsms_edit415 = _T("");
	m_telsms_edit416 = _T("");
	m_telsms_edit45 = _T("");
	m_telsms_edit46 = _T("");
	m_telsms_edit47 = _T("");
	m_telsms_edit48 = _T("");
	m_telsms_edit49 = _T("");
	//}}AFX_DATA_INIT
}

CTelSms::~CTelSms()
{
}

void CTelSms::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelSms)
	DDX_Control(pDX, ID_TELSMS_SAVE, m_telsms_save);
	DDX_Control(pDX, ID_TELSMS_GET, m_telsms_get);
	DDX_Text(pDX, IDC_TELSMS_EDIT11, m_telsms_edit11);
	DDX_Text(pDX, IDC_TELSMS_EDIT12, m_telsms_edit12);
	DDX_Text(pDX, IDC_TELSMS_EDIT13, m_telsms_edit13);
	DDX_Text(pDX, IDC_TELSMS_EDIT14, m_telsms_edit14);
	DDX_Text(pDX, IDC_TELSMS_EDIT21, m_telsms_edit21);
	DDX_Text(pDX, IDC_TELSMS_EDIT22, m_telsms_edit22);
	DDX_Text(pDX, IDC_TELSMS_EDIT23, m_telsms_edit23);
	DDX_Text(pDX, IDC_TELSMS_EDIT24, m_telsms_edit24);
	DDX_Text(pDX, IDC_TELSMS_EDIT31, m_telsms_edit31);
	DDX_Text(pDX, IDC_TELSMS_EDIT32, m_telsms_edit32);
	DDX_Text(pDX, IDC_TELSMS_EDIT33, m_telsms_edit33);
	DDX_Text(pDX, IDC_TELSMS_EDIT34, m_telsms_edit34);
	DDX_Text(pDX, IDC_TELSMS_EDIT41, m_telsms_edit41);
	DDX_Text(pDX, IDC_TELSMS_EDIT42, m_telsms_edit42);
	DDX_Text(pDX, IDC_TELSMS_EDIT43, m_telsms_edit43);
	DDX_Text(pDX, IDC_TELSMS_EDIT44, m_telsms_edit44);
	DDX_Text(pDX, IDC_TELSMS_EDIT110, m_telsms_edit110);
	DDX_Text(pDX, IDC_TELSMS_EDIT111, m_telsms_edit111);
	DDX_Text(pDX, IDC_TELSMS_EDIT112, m_telsms_edit112);
	DDX_Text(pDX, IDC_TELSMS_EDIT113, m_telsms_edit113);
	DDX_Text(pDX, IDC_TELSMS_EDIT114, m_telsms_edit114);
	DDX_Text(pDX, IDC_TELSMS_EDIT115, m_telsms_edit115);
	DDX_Text(pDX, IDC_TELSMS_EDIT116, m_telsms_edit116);
	DDX_Text(pDX, IDC_TELSMS_EDIT15, m_telsms_edit15);
	DDX_Text(pDX, IDC_TELSMS_EDIT16, m_telsms_edit16);
	DDX_Text(pDX, IDC_TELSMS_EDIT17, m_telsms_edit17);
	DDX_Text(pDX, IDC_TELSMS_EDIT18, m_telsms_edit18);
	DDX_Text(pDX, IDC_TELSMS_EDIT19, m_telsms_edit19);
	DDX_Text(pDX, IDC_TELSMS_EDIT210, m_telsms_edit210);
	DDX_Text(pDX, IDC_TELSMS_EDIT211, m_telsms_edit211);
	DDX_Text(pDX, IDC_TELSMS_EDIT212, m_telsms_edit212);
	DDX_Text(pDX, IDC_TELSMS_EDIT213, m_telsms_edit213);
	DDX_Text(pDX, IDC_TELSMS_EDIT214, m_telsms_edit214);
	DDX_Text(pDX, IDC_TELSMS_EDIT216, m_telsms_edit216);
	DDX_Text(pDX, IDC_TELSMS_EDIT215, m_telsms_edit215);
	DDX_Text(pDX, IDC_TELSMS_EDIT25, m_telsms_edit25);
	DDX_Text(pDX, IDC_TELSMS_EDIT26, m_telsms_edit26);
	DDX_Text(pDX, IDC_TELSMS_EDIT27, m_telsms_edit27);
	DDX_Text(pDX, IDC_TELSMS_EDIT28, m_telsms_edit28);
	DDX_Text(pDX, IDC_TELSMS_EDIT29, m_telsms_edit29);
	DDX_Text(pDX, IDC_TELSMS_EDIT310, m_telsms_edit310);
	DDX_Text(pDX, IDC_TELSMS_EDIT311, m_telsms_edit311);
	DDX_Text(pDX, IDC_TELSMS_EDIT312, m_telsms_edit312);
	DDX_Text(pDX, IDC_TELSMS_EDIT313, m_telsms_edit313);
	DDX_Text(pDX, IDC_TELSMS_EDIT314, m_telsms_edit314);
	DDX_Text(pDX, IDC_TELSMS_EDIT315, m_telsms_edit315);
	DDX_Text(pDX, IDC_TELSMS_EDIT316, m_telsms_edit316);
	DDX_Text(pDX, IDC_TELSMS_EDIT35, m_telsms_edit35);
	DDX_Text(pDX, IDC_TELSMS_EDIT36, m_telsms_edit36);
	DDX_Text(pDX, IDC_TELSMS_EDIT37, m_telsms_edit37);
	DDX_Text(pDX, IDC_TELSMS_EDIT38, m_telsms_edit38);
	DDX_Text(pDX, IDC_TELSMS_EDIT39, m_telsms_edit39);
	DDX_Text(pDX, IDC_TELSMS_EDIT410, m_telsms_edit410);
	DDX_Text(pDX, IDC_TELSMS_EDIT411, m_telsms_edit411);
	DDX_Text(pDX, IDC_TELSMS_EDIT412, m_telsms_edit412);
	DDX_Text(pDX, IDC_TELSMS_EDIT413, m_telsms_edit413);
	DDX_Text(pDX, IDC_TELSMS_EDIT414, m_telsms_edit414);
	DDX_Text(pDX, IDC_TELSMS_EDIT415, m_telsms_edit415);
	DDX_Text(pDX, IDC_TELSMS_EDIT416, m_telsms_edit416);
	DDX_Text(pDX, IDC_TELSMS_EDIT45, m_telsms_edit45);
	DDX_Text(pDX, IDC_TELSMS_EDIT46, m_telsms_edit46);
	DDX_Text(pDX, IDC_TELSMS_EDIT47, m_telsms_edit47);
	DDX_Text(pDX, IDC_TELSMS_EDIT48, m_telsms_edit48);
	DDX_Text(pDX, IDC_TELSMS_EDIT49, m_telsms_edit49);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelSms, CPropertyPage)
	//{{AFX_MSG_MAP(CTelSms)
	ON_BN_CLICKED(ID_TELSMS_GET, OnTelsmsGet)
	ON_BN_CLICKED(ID_TELSMS_SAVE, OnTelsmsSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelSms message handlers

extern CString	TelSmsNum[MAX_ALM_CLASS][MAX_TEL_NUM];  //[cls][num]
extern CString	TelCallNum[MAX_ALM_CLASS][MAX_BP_NUM];
extern CString	TelSmsNum[MAX_ALM_CLASS][MAX_MOBILE_NUM];


void CTelSms::OnTelsmsGet() 
{
	// TODO: Add your control notification handler code here
	GetMobileNum();
	UpdateData(TRUE);  
	GetDataForDsp();
	UpdateData(FALSE); //刷新窗口中数据	
}

void CTelSms::OnTelsmsSave() 
{
	// TODO: Add your control notification handler code here
	if( MessageBox("确定要保存吗?","警告!!!",
		        	MB_OKCANCEL | MB_ICONQUESTION )==IDOK       )
	{
	UpdateData(TRUE); 
	if(m_telsms_edit11=="") TelSmsNum[0][0]="no"; else TelSmsNum[0][0]=m_telsms_edit11;
	if(m_telsms_edit12=="") TelSmsNum[0][1]="no"; else TelSmsNum[0][1]=m_telsms_edit12;
	if(m_telsms_edit13=="") TelSmsNum[0][2]="no"; else TelSmsNum[0][2]=m_telsms_edit13;
	if(m_telsms_edit14=="") TelSmsNum[0][3]="no"; else TelSmsNum[0][3]=m_telsms_edit14;
	if(m_telsms_edit15=="") TelSmsNum[0][4]="no"; else TelSmsNum[0][4]=m_telsms_edit15;
	if(m_telsms_edit16=="") TelSmsNum[0][5]="no"; else TelSmsNum[0][5]=m_telsms_edit16;
	if(m_telsms_edit17=="") TelSmsNum[0][6]="no"; else TelSmsNum[0][6]=m_telsms_edit17;
	if(m_telsms_edit18=="") TelSmsNum[0][7]="no"; else TelSmsNum[0][7]=m_telsms_edit18;
	if(m_telsms_edit19=="") TelSmsNum[0][8]="no"; else TelSmsNum[0][8]=m_telsms_edit19;
	if(m_telsms_edit110=="") TelSmsNum[0][9]="no"; else TelSmsNum[0][9]=m_telsms_edit110;
	if(m_telsms_edit111=="") TelSmsNum[0][10]="no"; else TelSmsNum[0][10]=m_telsms_edit111;
	if(m_telsms_edit112=="") TelSmsNum[0][11]="no"; else TelSmsNum[0][11]=m_telsms_edit112;
	if(m_telsms_edit113=="") TelSmsNum[0][12]="no"; else TelSmsNum[0][12]=m_telsms_edit113;
	if(m_telsms_edit114=="") TelSmsNum[0][13]="no"; else TelSmsNum[0][13]=m_telsms_edit114;
	if(m_telsms_edit115=="") TelSmsNum[0][14]="no"; else TelSmsNum[0][14]=m_telsms_edit115;
	if(m_telsms_edit116=="") TelSmsNum[0][15]="no"; else TelSmsNum[0][15]=m_telsms_edit116;

	if(m_telsms_edit21=="") TelSmsNum[1][0]="no"; else TelSmsNum[1][0]=m_telsms_edit21;
	if(m_telsms_edit22=="") TelSmsNum[1][1]="no"; else TelSmsNum[1][1]=m_telsms_edit22;
	if(m_telsms_edit23=="") TelSmsNum[1][2]="no"; else TelSmsNum[1][2]=m_telsms_edit23;
	if(m_telsms_edit24=="") TelSmsNum[1][3]="no"; else TelSmsNum[1][3]=m_telsms_edit24;
	if(m_telsms_edit25=="") TelSmsNum[1][4]="no"; else TelSmsNum[1][4]=m_telsms_edit25;
	if(m_telsms_edit26=="") TelSmsNum[1][5]="no"; else TelSmsNum[1][5]=m_telsms_edit26;
	if(m_telsms_edit27=="") TelSmsNum[1][6]="no"; else TelSmsNum[1][6]=m_telsms_edit27;
	if(m_telsms_edit28=="") TelSmsNum[1][7]="no"; else TelSmsNum[1][7]=m_telsms_edit28;
	if(m_telsms_edit29=="") TelSmsNum[1][8]="no"; else TelSmsNum[1][8]=m_telsms_edit29;
	if(m_telsms_edit210=="") TelSmsNum[1][9]="no"; else TelSmsNum[1][9]=m_telsms_edit210;
	if(m_telsms_edit211=="") TelSmsNum[1][10]="no"; else TelSmsNum[1][10]=m_telsms_edit211;
	if(m_telsms_edit212=="") TelSmsNum[1][11]="no"; else TelSmsNum[1][11]=m_telsms_edit212;
	if(m_telsms_edit213=="") TelSmsNum[1][12]="no"; else TelSmsNum[1][12]=m_telsms_edit213;
	if(m_telsms_edit214=="") TelSmsNum[1][13]="no"; else TelSmsNum[1][13]=m_telsms_edit214;
	if(m_telsms_edit215=="") TelSmsNum[1][14]="no"; else TelSmsNum[1][14]=m_telsms_edit215;
	if(m_telsms_edit216=="") TelSmsNum[1][15]="no"; else TelSmsNum[1][15]=m_telsms_edit216;

	if(m_telsms_edit31=="") TelSmsNum[2][0]="no"; else TelSmsNum[2][0]=m_telsms_edit31;
	if(m_telsms_edit32=="") TelSmsNum[2][1]="no"; else TelSmsNum[2][1]=m_telsms_edit32;
	if(m_telsms_edit33=="") TelSmsNum[2][2]="no"; else TelSmsNum[2][2]=m_telsms_edit33;
	if(m_telsms_edit34=="") TelSmsNum[2][3]="no"; else TelSmsNum[2][3]=m_telsms_edit34;
	if(m_telsms_edit35=="") TelSmsNum[2][4]="no"; else TelSmsNum[2][4]=m_telsms_edit35;
	if(m_telsms_edit36=="") TelSmsNum[2][5]="no"; else TelSmsNum[2][5]=m_telsms_edit36;
	if(m_telsms_edit37=="") TelSmsNum[2][6]="no"; else TelSmsNum[2][6]=m_telsms_edit37;
	if(m_telsms_edit38=="") TelSmsNum[2][7]="no"; else TelSmsNum[2][7]=m_telsms_edit38;
	if(m_telsms_edit39=="") TelSmsNum[2][8]="no"; else TelSmsNum[2][8]=m_telsms_edit39;
	if(m_telsms_edit310=="") TelSmsNum[2][9]="no"; else TelSmsNum[2][9]=m_telsms_edit310;
	if(m_telsms_edit311=="") TelSmsNum[2][10]="no"; else TelSmsNum[2][10]=m_telsms_edit311;
	if(m_telsms_edit312=="") TelSmsNum[2][11]="no"; else TelSmsNum[2][11]=m_telsms_edit312;
	if(m_telsms_edit313=="") TelSmsNum[2][12]="no"; else TelSmsNum[2][12]=m_telsms_edit313;
	if(m_telsms_edit314=="") TelSmsNum[2][13]="no"; else TelSmsNum[2][13]=m_telsms_edit314;
	if(m_telsms_edit315=="") TelSmsNum[2][14]="no"; else TelSmsNum[2][14]=m_telsms_edit315;
	if(m_telsms_edit316=="") TelSmsNum[2][15]="no"; else TelSmsNum[2][15]=m_telsms_edit316;

	if(m_telsms_edit41=="") TelSmsNum[3][0]="no"; else TelSmsNum[3][0]=m_telsms_edit41;
	if(m_telsms_edit42=="") TelSmsNum[3][1]="no"; else TelSmsNum[3][1]=m_telsms_edit42;
	if(m_telsms_edit43=="") TelSmsNum[3][2]="no"; else TelSmsNum[3][2]=m_telsms_edit43;
	if(m_telsms_edit44=="") TelSmsNum[3][3]="no"; else TelSmsNum[3][3]=m_telsms_edit44;
	if(m_telsms_edit45=="") TelSmsNum[3][4]="no"; else TelSmsNum[3][4]=m_telsms_edit45;
	if(m_telsms_edit46=="") TelSmsNum[3][5]="no"; else TelSmsNum[3][5]=m_telsms_edit46;
	if(m_telsms_edit47=="") TelSmsNum[3][6]="no"; else TelSmsNum[3][6]=m_telsms_edit47;
	if(m_telsms_edit48=="") TelSmsNum[3][7]="no"; else TelSmsNum[3][7]=m_telsms_edit48;
	if(m_telsms_edit49=="") TelSmsNum[3][8]="no"; else TelSmsNum[3][8]=m_telsms_edit49;
	if(m_telsms_edit410=="") TelSmsNum[3][9]="no"; else TelSmsNum[3][9]=m_telsms_edit410;
	if(m_telsms_edit411=="") TelSmsNum[3][10]="no"; else TelSmsNum[3][10]=m_telsms_edit411;
	if(m_telsms_edit412=="") TelSmsNum[3][11]="no"; else TelSmsNum[3][11]=m_telsms_edit412;
	if(m_telsms_edit413=="") TelSmsNum[3][12]="no"; else TelSmsNum[3][12]=m_telsms_edit413;
	if(m_telsms_edit414=="") TelSmsNum[3][13]="no"; else TelSmsNum[3][13]=m_telsms_edit414;
	if(m_telsms_edit415=="") TelSmsNum[3][14]="no"; else TelSmsNum[3][14]=m_telsms_edit415;
	if(m_telsms_edit416=="") TelSmsNum[3][15]="no"; else TelSmsNum[3][15]=m_telsms_edit416;
	UpdateData(FALSE);
	SaveMobileNum();	
	}
}


void CTelSms::GetDataForDsp()
{
	if(TelSmsNum[0][0]=="no") m_telsms_edit11 = ""; else m_telsms_edit11 = TelSmsNum[0][0];
	if(TelSmsNum[0][1]=="no") m_telsms_edit12 = ""; else m_telsms_edit12 = TelSmsNum[0][1];
	if(TelSmsNum[0][2]=="no") m_telsms_edit13 = ""; else m_telsms_edit13 = TelSmsNum[0][2];
	if(TelSmsNum[0][3]=="no") m_telsms_edit14 = ""; else m_telsms_edit14 = TelSmsNum[0][3];
	if(TelSmsNum[0][4]=="no") m_telsms_edit15 = ""; else m_telsms_edit15 = TelSmsNum[0][4];
	if(TelSmsNum[0][5]=="no") m_telsms_edit16 = ""; else m_telsms_edit16 = TelSmsNum[0][5];
	if(TelSmsNum[0][6]=="no") m_telsms_edit17 = ""; else m_telsms_edit17 = TelSmsNum[0][6];
	if(TelSmsNum[0][7]=="no") m_telsms_edit18 = ""; else m_telsms_edit18 = TelSmsNum[0][7];
	if(TelSmsNum[0][8]=="no") m_telsms_edit19 = ""; else m_telsms_edit19 = TelSmsNum[0][8];
	if(TelSmsNum[0][9]=="no") m_telsms_edit110 = ""; else m_telsms_edit110 = TelSmsNum[0][9];
	if(TelSmsNum[0][10]=="no") m_telsms_edit111 = ""; else m_telsms_edit111 = TelSmsNum[0][10];
	if(TelSmsNum[0][11]=="no") m_telsms_edit112 = ""; else m_telsms_edit112 = TelSmsNum[0][11];
	if(TelSmsNum[0][12]=="no") m_telsms_edit113 = ""; else m_telsms_edit113 = TelSmsNum[0][12];
	if(TelSmsNum[0][13]=="no") m_telsms_edit114 = ""; else m_telsms_edit114 = TelSmsNum[0][13];
	if(TelSmsNum[0][14]=="no") m_telsms_edit115 = ""; else m_telsms_edit115 = TelSmsNum[0][14];
	if(TelSmsNum[0][15]=="no") m_telsms_edit116 = ""; else m_telsms_edit116 = TelSmsNum[0][15];

	if(TelSmsNum[1][0]=="no") m_telsms_edit21 = ""; else m_telsms_edit21 = TelSmsNum[1][0]; 
	if(TelSmsNum[1][1]=="no") m_telsms_edit22 = ""; else m_telsms_edit22 = TelSmsNum[1][1]; 
	if(TelSmsNum[1][2]=="no") m_telsms_edit23 = ""; else m_telsms_edit23 = TelSmsNum[1][2]; 
	if(TelSmsNum[1][3]=="no") m_telsms_edit24 = ""; else m_telsms_edit24 = TelSmsNum[1][3]; 
	if(TelSmsNum[1][4]=="no") m_telsms_edit25 = ""; else m_telsms_edit25 = TelSmsNum[1][4];
	if(TelSmsNum[1][5]=="no") m_telsms_edit26 = ""; else m_telsms_edit26 = TelSmsNum[1][5];
	if(TelSmsNum[1][6]=="no") m_telsms_edit27 = ""; else m_telsms_edit27 = TelSmsNum[1][6];
	if(TelSmsNum[1][7]=="no") m_telsms_edit28 = ""; else m_telsms_edit28 = TelSmsNum[1][7];
	if(TelSmsNum[1][8]=="no") m_telsms_edit29 = ""; else m_telsms_edit29 = TelSmsNum[1][8];
	if(TelSmsNum[1][9]=="no") m_telsms_edit210 = ""; else m_telsms_edit210 = TelSmsNum[1][9];
	if(TelSmsNum[1][10]=="no") m_telsms_edit211 = ""; else m_telsms_edit211 = TelSmsNum[1][10];
	if(TelSmsNum[1][11]=="no") m_telsms_edit212 = ""; else m_telsms_edit212 = TelSmsNum[1][11];
	if(TelSmsNum[1][12]=="no") m_telsms_edit213 = ""; else m_telsms_edit213 = TelSmsNum[1][12];
	if(TelSmsNum[1][13]=="no") m_telsms_edit214 = ""; else m_telsms_edit214 = TelSmsNum[1][13];
	if(TelSmsNum[1][14]=="no") m_telsms_edit215 = ""; else m_telsms_edit215 = TelSmsNum[1][14];
	if(TelSmsNum[1][15]=="no") m_telsms_edit216 = ""; else m_telsms_edit216 = TelSmsNum[1][15];

	if(TelSmsNum[2][0]=="no") m_telsms_edit31 = ""; else m_telsms_edit31 = TelSmsNum[2][0];  
	if(TelSmsNum[2][1]=="no") m_telsms_edit32 = ""; else m_telsms_edit32 = TelSmsNum[2][1]; 
	if(TelSmsNum[2][2]=="no") m_telsms_edit33 = ""; else m_telsms_edit33 = TelSmsNum[2][2];  
	if(TelSmsNum[2][3]=="no") m_telsms_edit34 = ""; else m_telsms_edit34 = TelSmsNum[2][3]; 	
	if(TelSmsNum[2][4]=="no") m_telsms_edit35 = ""; else m_telsms_edit35 = TelSmsNum[2][4];
	if(TelSmsNum[2][5]=="no") m_telsms_edit36 = ""; else m_telsms_edit36 = TelSmsNum[2][5];
	if(TelSmsNum[2][6]=="no") m_telsms_edit37 = ""; else m_telsms_edit37 = TelSmsNum[2][6];
	if(TelSmsNum[2][7]=="no") m_telsms_edit38 = ""; else m_telsms_edit38 = TelSmsNum[2][7];
	if(TelSmsNum[2][8]=="no") m_telsms_edit39 = ""; else m_telsms_edit39 = TelSmsNum[2][8];
	if(TelSmsNum[2][9]=="no") m_telsms_edit310 = ""; else m_telsms_edit310 = TelSmsNum[2][9];
	if(TelSmsNum[2][10]=="no") m_telsms_edit311 = ""; else m_telsms_edit311 = TelSmsNum[2][10];
	if(TelSmsNum[2][11]=="no") m_telsms_edit312 = ""; else m_telsms_edit312 = TelSmsNum[2][11];
	if(TelSmsNum[2][12]=="no") m_telsms_edit313 = ""; else m_telsms_edit313 = TelSmsNum[2][12];
	if(TelSmsNum[2][13]=="no") m_telsms_edit314 = ""; else m_telsms_edit314 = TelSmsNum[2][13];
	if(TelSmsNum[2][14]=="no") m_telsms_edit315 = ""; else m_telsms_edit315 = TelSmsNum[2][14];
	if(TelSmsNum[2][15]=="no") m_telsms_edit316 = ""; else m_telsms_edit316 = TelSmsNum[2][15];

	if(TelSmsNum[3][0]=="no") m_telsms_edit41 = ""; else m_telsms_edit41 = TelSmsNum[3][0];
	if(TelSmsNum[3][1]=="no") m_telsms_edit42 = ""; else m_telsms_edit42 = TelSmsNum[3][1];
	if(TelSmsNum[3][2]=="no") m_telsms_edit43 = ""; else m_telsms_edit43 = TelSmsNum[3][2];
	if(TelSmsNum[3][3]=="no") m_telsms_edit44 = ""; else m_telsms_edit44 = TelSmsNum[3][3];
	if(TelSmsNum[3][4]=="no") m_telsms_edit45 = ""; else m_telsms_edit45 = TelSmsNum[3][4];
	if(TelSmsNum[3][5]=="no") m_telsms_edit46 = ""; else m_telsms_edit46 = TelSmsNum[3][5];
	if(TelSmsNum[3][6]=="no") m_telsms_edit47 = ""; else m_telsms_edit47 = TelSmsNum[3][6];
	if(TelSmsNum[3][7]=="no") m_telsms_edit48 = ""; else m_telsms_edit48 = TelSmsNum[3][7];
	if(TelSmsNum[3][8]=="no") m_telsms_edit49 = ""; else m_telsms_edit49 = TelSmsNum[3][8];
	if(TelSmsNum[3][9]=="no") m_telsms_edit410 = ""; else m_telsms_edit410 = TelSmsNum[3][9];
	if(TelSmsNum[3][10]=="no") m_telsms_edit411 = ""; else m_telsms_edit411 = TelSmsNum[3][10];
	if(TelSmsNum[3][11]=="no") m_telsms_edit412 = ""; else m_telsms_edit412 = TelSmsNum[3][11];
	if(TelSmsNum[3][12]=="no") m_telsms_edit413 = ""; else m_telsms_edit413 = TelSmsNum[3][12];
	if(TelSmsNum[3][13]=="no") m_telsms_edit414 = ""; else m_telsms_edit414 = TelSmsNum[3][13];
	if(TelSmsNum[3][14]=="no") m_telsms_edit415 = ""; else m_telsms_edit415 = TelSmsNum[3][14];
	if(TelSmsNum[3][15]=="no") m_telsms_edit416 = ""; else m_telsms_edit416 = TelSmsNum[3][15];
}
