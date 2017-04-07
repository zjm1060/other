// TelAudio.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/TelAudio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTelAudio property page

IMPLEMENT_DYNCREATE(CTelAudio, CPropertyPage)

CTelAudio::CTelAudio() : CPropertyPage(CTelAudio::IDD)
{
	//{{AFX_DATA_INIT(CTelAudio)
	m_telaudio_edit11 = _T("");
	m_telaudio_edit12 = _T("");
	m_telaudio_edit21 = _T("");
	m_telaudio_edit22 = _T("");
	m_telaudio_edit31 = _T("");
	m_telaudio_edit32 = _T("");
	m_telaudio_edit41 = _T("");
	m_telaudio_edit42 = _T("");
	m_telaudio_edit13 = _T("");
	m_telaudio_edit14 = _T("");
	m_telaudio_edit23 = _T("");
	m_telaudio_edit24 = _T("");
	m_telaudio_edit33 = _T("");
	m_telaudio_edit34 = _T("");
	m_telaudio_edit43 = _T("");
	m_telaudio_edit44 = _T("");
	m_telaudio_edit110 = _T("");
	m_telaudio_edit111 = _T("");
	m_telaudio_edit112 = _T("");
	m_telaudio_edit113 = _T("");
	m_telaudio_edit114 = _T("");
	m_telaudio_edit115 = _T("");
	m_telaudio_edit116 = _T("");
	m_telaudio_edit15 = _T("");
	m_telaudio_edit16 = _T("");
	m_telaudio_edit17 = _T("");
	m_telaudio_edit18 = _T("");
	m_telaudio_edit19 = _T("");
	m_telaudio_edit210 = _T("");
	m_telaudio_edit211 = _T("");
	m_telaudio_edit212 = _T("");
	m_telaudio_edit213 = _T("");
	m_telaudio_edit214 = _T("");
	m_telaudio_edit215 = _T("");
	m_telaudio_edit216 = _T("");
	m_telaudio_edit25 = _T("");
	m_telaudio_edit26 = _T("");
	m_telaudio_edit27 = _T("");
	m_telaudio_edit28 = _T("");
	m_telaudio_edit29 = _T("");
	m_telaudio_edit310 = _T("");
	m_telaudio_edit311 = _T("");
	m_telaudio_edit312 = _T("");
	m_telaudio_edit313 = _T("");
	m_telaudio_edit314 = _T("");
	m_telaudio_edit315 = _T("");
	m_telaudio_edit316 = _T("");
	m_telaudio_edit35 = _T("");
	m_telaudio_edit36 = _T("");
	m_telaudio_edit37 = _T("");
	m_telaudio_edit38 = _T("");
	m_telaudio_edit39 = _T("");
	m_telaudio_edit410 = _T("");
	m_telaudio_edit411 = _T("");
	m_telaudio_edit412 = _T("");
	m_telaudio_edit413 = _T("");
	m_telaudio_edit414 = _T("");
	m_telaudio_edit415 = _T("");
	m_telaudio_edit416 = _T("");
	m_telaudio_edit45 = _T("");
	m_telaudio_edit46 = _T("");
	m_telaudio_edit47 = _T("");
	m_telaudio_edit48 = _T("");
	m_telaudio_edit49 = _T("");
	//}}AFX_DATA_INIT
}

CTelAudio::~CTelAudio()
{
}

void CTelAudio::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelAudio)
	DDX_Control(pDX, ID_TELAUDIO_SAVE, m_telaudio_save);
	DDX_Control(pDX, ID_TELAUDIO_GET, m_telaudio_get);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT11, m_telaudio_edit11);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT12, m_telaudio_edit12);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT21, m_telaudio_edit21);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT22, m_telaudio_edit22);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT31, m_telaudio_edit31);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT32, m_telaudio_edit32);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT41, m_telaudio_edit41);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT42, m_telaudio_edit42);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT13, m_telaudio_edit13);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT14, m_telaudio_edit14);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT23, m_telaudio_edit23);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT24, m_telaudio_edit24);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT33, m_telaudio_edit33);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT34, m_telaudio_edit34);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT43, m_telaudio_edit43);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT44, m_telaudio_edit44);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT110, m_telaudio_edit110);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT111, m_telaudio_edit111);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT112, m_telaudio_edit112);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT113, m_telaudio_edit113);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT114, m_telaudio_edit114);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT115, m_telaudio_edit115);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT116, m_telaudio_edit116);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT15, m_telaudio_edit15);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT16, m_telaudio_edit16);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT17, m_telaudio_edit17);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT18, m_telaudio_edit18);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT19, m_telaudio_edit19);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT210, m_telaudio_edit210);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT211, m_telaudio_edit211);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT212, m_telaudio_edit212);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT213, m_telaudio_edit213);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT214, m_telaudio_edit214);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT215, m_telaudio_edit215);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT216, m_telaudio_edit216);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT25, m_telaudio_edit25);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT26, m_telaudio_edit26);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT27, m_telaudio_edit27);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT28, m_telaudio_edit28);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT29, m_telaudio_edit29);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT310, m_telaudio_edit310);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT311, m_telaudio_edit311);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT312, m_telaudio_edit312);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT313, m_telaudio_edit313);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT314, m_telaudio_edit314);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT315, m_telaudio_edit315);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT316, m_telaudio_edit316);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT35, m_telaudio_edit35);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT36, m_telaudio_edit36);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT37, m_telaudio_edit37);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT38, m_telaudio_edit38);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT39, m_telaudio_edit39);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT410, m_telaudio_edit410);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT411, m_telaudio_edit411);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT412, m_telaudio_edit412);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT413, m_telaudio_edit413);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT414, m_telaudio_edit414);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT415, m_telaudio_edit415);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT416, m_telaudio_edit416);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT45, m_telaudio_edit45);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT46, m_telaudio_edit46);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT47, m_telaudio_edit47);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT48, m_telaudio_edit48);
	DDX_Text(pDX, IDC_TELAUDIO_EDIT49, m_telaudio_edit49);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelAudio, CPropertyPage)
	//{{AFX_MSG_MAP(CTelAudio)
	ON_BN_CLICKED(ID_TELAUDIO_GET, OnTelaudioGet)
	ON_BN_CLICKED(ID_TELAUDIO_SAVE, OnTelaudioSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelAudio message handlers


extern CString	TelAudioNum[MAX_ALM_CLASS][MAX_TEL_NUM];  //[cls][num]
extern CString	TelCallNum[MAX_ALM_CLASS][MAX_BP_NUM];
extern CString	TelSmsNum[MAX_ALM_CLASS][MAX_MOBILE_NUM];


void CTelAudio::OnTelaudioGet() 
{
	// TODO: Add your control notification handler code here
	GetTelephoneNum();
	UpdateData(TRUE);  
	GetDataForDsp();
	UpdateData(FALSE); //刷新窗口中数据
}

void CTelAudio::OnTelaudioSave() 
{
	// TODO: Add your control notification handler code here
	if( MessageBox("确定要保存吗?","警告!!!",
		        	MB_OKCANCEL | MB_ICONQUESTION )==IDOK       )
	{
	UpdateData(TRUE); 
	if(m_telaudio_edit11=="") TelAudioNum[0][0]="no"; else TelAudioNum[0][0]=m_telaudio_edit11;
	if(m_telaudio_edit12=="") TelAudioNum[0][1]="no"; else TelAudioNum[0][1]=m_telaudio_edit12;
	if(m_telaudio_edit13=="") TelAudioNum[0][2]="no"; else TelAudioNum[0][2]=m_telaudio_edit13;
	if(m_telaudio_edit14=="") TelAudioNum[0][3]="no"; else TelAudioNum[0][3]=m_telaudio_edit14;
	if(m_telaudio_edit15=="") TelAudioNum[0][4]="no"; else TelAudioNum[0][4]=m_telaudio_edit15;
	if(m_telaudio_edit16=="") TelAudioNum[0][5]="no"; else TelAudioNum[0][5]=m_telaudio_edit16;
	if(m_telaudio_edit17=="") TelAudioNum[0][6]="no"; else TelAudioNum[0][6]=m_telaudio_edit17;
	if(m_telaudio_edit18=="") TelAudioNum[0][7]="no"; else TelAudioNum[0][7]=m_telaudio_edit18;
	if(m_telaudio_edit19=="") TelAudioNum[0][8]="no"; else TelAudioNum[0][8]=m_telaudio_edit19;
	if(m_telaudio_edit110=="") TelAudioNum[0][9]="no"; else TelAudioNum[0][9]=m_telaudio_edit110;
	if(m_telaudio_edit111=="") TelAudioNum[0][10]="no"; else TelAudioNum[0][10]=m_telaudio_edit111;
	if(m_telaudio_edit112=="") TelAudioNum[0][11]="no"; else TelAudioNum[0][11]=m_telaudio_edit112;
	if(m_telaudio_edit113=="") TelAudioNum[0][12]="no"; else TelAudioNum[0][12]=m_telaudio_edit113;
	if(m_telaudio_edit114=="") TelAudioNum[0][13]="no"; else TelAudioNum[0][13]=m_telaudio_edit114;
	if(m_telaudio_edit115=="") TelAudioNum[0][14]="no"; else TelAudioNum[0][14]=m_telaudio_edit115;
	if(m_telaudio_edit116=="") TelAudioNum[0][15]="no"; else TelAudioNum[0][15]=m_telaudio_edit116;

	if(m_telaudio_edit21=="") TelAudioNum[1][0]="no"; else TelAudioNum[1][0]=m_telaudio_edit21;
	if(m_telaudio_edit22=="") TelAudioNum[1][1]="no"; else TelAudioNum[1][1]=m_telaudio_edit22;
	if(m_telaudio_edit23=="") TelAudioNum[1][2]="no"; else TelAudioNum[1][2]=m_telaudio_edit23;
	if(m_telaudio_edit24=="") TelAudioNum[1][3]="no"; else TelAudioNum[1][3]=m_telaudio_edit24;
	if(m_telaudio_edit25=="") TelAudioNum[1][4]="no"; else TelAudioNum[1][4]=m_telaudio_edit25;
	if(m_telaudio_edit26=="") TelAudioNum[1][5]="no"; else TelAudioNum[1][5]=m_telaudio_edit26;
	if(m_telaudio_edit27=="") TelAudioNum[1][6]="no"; else TelAudioNum[1][6]=m_telaudio_edit27;
	if(m_telaudio_edit28=="") TelAudioNum[1][7]="no"; else TelAudioNum[1][7]=m_telaudio_edit28;
	if(m_telaudio_edit29=="") TelAudioNum[1][8]="no"; else TelAudioNum[1][8]=m_telaudio_edit29;
	if(m_telaudio_edit210=="") TelAudioNum[1][9]="no"; else TelAudioNum[1][9]=m_telaudio_edit210;
	if(m_telaudio_edit211=="") TelAudioNum[1][10]="no"; else TelAudioNum[1][10]=m_telaudio_edit211;
	if(m_telaudio_edit212=="") TelAudioNum[1][11]="no"; else TelAudioNum[1][11]=m_telaudio_edit212;
	if(m_telaudio_edit213=="") TelAudioNum[1][12]="no"; else TelAudioNum[1][12]=m_telaudio_edit213;
	if(m_telaudio_edit214=="") TelAudioNum[1][13]="no"; else TelAudioNum[1][13]=m_telaudio_edit214;
	if(m_telaudio_edit215=="") TelAudioNum[1][14]="no"; else TelAudioNum[1][14]=m_telaudio_edit215;
	if(m_telaudio_edit216=="") TelAudioNum[1][15]="no"; else TelAudioNum[1][15]=m_telaudio_edit216;

	if(m_telaudio_edit31=="") TelAudioNum[2][0]="no"; else TelAudioNum[2][0]=m_telaudio_edit31;
	if(m_telaudio_edit32=="") TelAudioNum[2][1]="no"; else TelAudioNum[2][1]=m_telaudio_edit32;
	if(m_telaudio_edit33=="") TelAudioNum[2][2]="no"; else TelAudioNum[2][2]=m_telaudio_edit33;
	if(m_telaudio_edit34=="") TelAudioNum[2][3]="no"; else TelAudioNum[2][3]=m_telaudio_edit34;
	if(m_telaudio_edit35=="") TelAudioNum[2][4]="no"; else TelAudioNum[2][4]=m_telaudio_edit35;
	if(m_telaudio_edit36=="") TelAudioNum[2][5]="no"; else TelAudioNum[2][5]=m_telaudio_edit36;
	if(m_telaudio_edit37=="") TelAudioNum[2][6]="no"; else TelAudioNum[2][6]=m_telaudio_edit37;
	if(m_telaudio_edit38=="") TelAudioNum[2][7]="no"; else TelAudioNum[2][7]=m_telaudio_edit38;
	if(m_telaudio_edit39=="") TelAudioNum[2][8]="no"; else TelAudioNum[2][8]=m_telaudio_edit39;
	if(m_telaudio_edit310=="") TelAudioNum[2][9]="no"; else TelAudioNum[2][9]=m_telaudio_edit310;
	if(m_telaudio_edit311=="") TelAudioNum[2][10]="no"; else TelAudioNum[2][10]=m_telaudio_edit311;
	if(m_telaudio_edit312=="") TelAudioNum[2][11]="no"; else TelAudioNum[2][11]=m_telaudio_edit312;
	if(m_telaudio_edit313=="") TelAudioNum[2][12]="no"; else TelAudioNum[2][12]=m_telaudio_edit313;
	if(m_telaudio_edit314=="") TelAudioNum[2][13]="no"; else TelAudioNum[2][13]=m_telaudio_edit314;
	if(m_telaudio_edit315=="") TelAudioNum[2][14]="no"; else TelAudioNum[2][14]=m_telaudio_edit315;
	if(m_telaudio_edit316=="") TelAudioNum[2][15]="no"; else TelAudioNum[2][15]=m_telaudio_edit316;

	if(m_telaudio_edit41=="") TelAudioNum[3][0]="no"; else TelAudioNum[3][0]=m_telaudio_edit41;
	if(m_telaudio_edit42=="") TelAudioNum[3][1]="no"; else TelAudioNum[3][1]=m_telaudio_edit42;
	if(m_telaudio_edit43=="") TelAudioNum[3][2]="no"; else TelAudioNum[3][2]=m_telaudio_edit43;
	if(m_telaudio_edit44=="") TelAudioNum[3][3]="no"; else TelAudioNum[3][3]=m_telaudio_edit44;
	if(m_telaudio_edit45=="") TelAudioNum[3][4]="no"; else TelAudioNum[3][4]=m_telaudio_edit45;
	if(m_telaudio_edit46=="") TelAudioNum[3][5]="no"; else TelAudioNum[3][5]=m_telaudio_edit46;
	if(m_telaudio_edit47=="") TelAudioNum[3][6]="no"; else TelAudioNum[3][6]=m_telaudio_edit47;
	if(m_telaudio_edit48=="") TelAudioNum[3][7]="no"; else TelAudioNum[3][7]=m_telaudio_edit48;
	if(m_telaudio_edit49=="") TelAudioNum[3][8]="no"; else TelAudioNum[3][8]=m_telaudio_edit49;
	if(m_telaudio_edit410=="") TelAudioNum[3][9]="no"; else TelAudioNum[3][9]=m_telaudio_edit410;
	if(m_telaudio_edit411=="") TelAudioNum[3][10]="no"; else TelAudioNum[3][10]=m_telaudio_edit411;
	if(m_telaudio_edit412=="") TelAudioNum[3][11]="no"; else TelAudioNum[3][11]=m_telaudio_edit412;
	if(m_telaudio_edit413=="") TelAudioNum[3][12]="no"; else TelAudioNum[3][12]=m_telaudio_edit413;
	if(m_telaudio_edit414=="") TelAudioNum[3][13]="no"; else TelAudioNum[3][13]=m_telaudio_edit414;
	if(m_telaudio_edit415=="") TelAudioNum[3][14]="no"; else TelAudioNum[3][14]=m_telaudio_edit415;
	if(m_telaudio_edit416=="") TelAudioNum[3][15]="no"; else TelAudioNum[3][15]=m_telaudio_edit416;
	UpdateData(FALSE);
	SaveTelephoneNum();
	}
}


void CTelAudio::GetDataForDsp()
{
	if(TelAudioNum[0][0]=="no") m_telaudio_edit11 = ""; else m_telaudio_edit11 = TelAudioNum[0][0];
	if(TelAudioNum[0][1]=="no") m_telaudio_edit12 = ""; else m_telaudio_edit12 = TelAudioNum[0][1];
	if(TelAudioNum[0][2]=="no") m_telaudio_edit13 = ""; else m_telaudio_edit13 = TelAudioNum[0][2];
	if(TelAudioNum[0][3]=="no") m_telaudio_edit14 = ""; else m_telaudio_edit14 = TelAudioNum[0][3];
	if(TelAudioNum[0][4]=="no") m_telaudio_edit15 = ""; else m_telaudio_edit15 = TelAudioNum[0][4];
	if(TelAudioNum[0][5]=="no") m_telaudio_edit16 = ""; else m_telaudio_edit16 = TelAudioNum[0][5];
	if(TelAudioNum[0][6]=="no") m_telaudio_edit17 = ""; else m_telaudio_edit17 = TelAudioNum[0][6];
	if(TelAudioNum[0][7]=="no") m_telaudio_edit18 = ""; else m_telaudio_edit18 = TelAudioNum[0][7];
	if(TelAudioNum[0][8]=="no") m_telaudio_edit19 = ""; else m_telaudio_edit19 = TelAudioNum[0][8];
	if(TelAudioNum[0][9]=="no") m_telaudio_edit110 = ""; else m_telaudio_edit110 = TelAudioNum[0][9];
	if(TelAudioNum[0][10]=="no") m_telaudio_edit111 = ""; else m_telaudio_edit111 = TelAudioNum[0][10];
	if(TelAudioNum[0][11]=="no") m_telaudio_edit112 = ""; else m_telaudio_edit112 = TelAudioNum[0][11];
	if(TelAudioNum[0][12]=="no") m_telaudio_edit113 = ""; else m_telaudio_edit113 = TelAudioNum[0][12];
	if(TelAudioNum[0][13]=="no") m_telaudio_edit114 = ""; else m_telaudio_edit114 = TelAudioNum[0][13];
	if(TelAudioNum[0][14]=="no") m_telaudio_edit115 = ""; else m_telaudio_edit115 = TelAudioNum[0][14];
	if(TelAudioNum[0][15]=="no") m_telaudio_edit116 = ""; else m_telaudio_edit116 = TelAudioNum[0][15];

	if(TelAudioNum[1][0]=="no") m_telaudio_edit21 = ""; else m_telaudio_edit21 = TelAudioNum[1][0]; 
	if(TelAudioNum[1][1]=="no") m_telaudio_edit22 = ""; else m_telaudio_edit22 = TelAudioNum[1][1]; 
	if(TelAudioNum[1][2]=="no") m_telaudio_edit23 = ""; else m_telaudio_edit23 = TelAudioNum[1][2]; 
	if(TelAudioNum[1][3]=="no") m_telaudio_edit24 = ""; else m_telaudio_edit24 = TelAudioNum[1][3]; 
	if(TelAudioNum[1][4]=="no") m_telaudio_edit25 = ""; else m_telaudio_edit25 = TelAudioNum[1][4];
	if(TelAudioNum[1][5]=="no") m_telaudio_edit26 = ""; else m_telaudio_edit26 = TelAudioNum[1][5];
	if(TelAudioNum[1][6]=="no") m_telaudio_edit27 = ""; else m_telaudio_edit27 = TelAudioNum[1][6];
	if(TelAudioNum[1][7]=="no") m_telaudio_edit28 = ""; else m_telaudio_edit28 = TelAudioNum[1][7];
	if(TelAudioNum[1][8]=="no") m_telaudio_edit29 = ""; else m_telaudio_edit29 = TelAudioNum[1][8];
	if(TelAudioNum[1][9]=="no") m_telaudio_edit210 = ""; else m_telaudio_edit210 = TelAudioNum[1][9];
	if(TelAudioNum[1][10]=="no") m_telaudio_edit211 = ""; else m_telaudio_edit211 = TelAudioNum[1][10];
	if(TelAudioNum[1][11]=="no") m_telaudio_edit212 = ""; else m_telaudio_edit212 = TelAudioNum[1][11];
	if(TelAudioNum[1][12]=="no") m_telaudio_edit213 = ""; else m_telaudio_edit213 = TelAudioNum[1][12];
	if(TelAudioNum[1][13]=="no") m_telaudio_edit214 = ""; else m_telaudio_edit214 = TelAudioNum[1][13];
	if(TelAudioNum[1][14]=="no") m_telaudio_edit215 = ""; else m_telaudio_edit215 = TelAudioNum[1][14];
	if(TelAudioNum[1][15]=="no") m_telaudio_edit216 = ""; else m_telaudio_edit216 = TelAudioNum[1][15];

	if(TelAudioNum[2][0]=="no") m_telaudio_edit31 = ""; else m_telaudio_edit31 = TelAudioNum[2][0];  
	if(TelAudioNum[2][1]=="no") m_telaudio_edit32 = ""; else m_telaudio_edit32 = TelAudioNum[2][1]; 
	if(TelAudioNum[2][2]=="no") m_telaudio_edit33 = ""; else m_telaudio_edit33 = TelAudioNum[2][2];  
	if(TelAudioNum[2][3]=="no") m_telaudio_edit34 = ""; else m_telaudio_edit34 = TelAudioNum[2][3]; 	
	if(TelAudioNum[2][4]=="no") m_telaudio_edit35 = ""; else m_telaudio_edit35 = TelAudioNum[2][4];
	if(TelAudioNum[2][5]=="no") m_telaudio_edit36 = ""; else m_telaudio_edit36 = TelAudioNum[2][5];
	if(TelAudioNum[2][6]=="no") m_telaudio_edit37 = ""; else m_telaudio_edit37 = TelAudioNum[2][6];
	if(TelAudioNum[2][7]=="no") m_telaudio_edit38 = ""; else m_telaudio_edit38 = TelAudioNum[2][7];
	if(TelAudioNum[2][8]=="no") m_telaudio_edit39 = ""; else m_telaudio_edit39 = TelAudioNum[2][8];
	if(TelAudioNum[2][9]=="no") m_telaudio_edit310 = ""; else m_telaudio_edit310 = TelAudioNum[2][9];
	if(TelAudioNum[2][10]=="no") m_telaudio_edit311 = ""; else m_telaudio_edit311 = TelAudioNum[2][10];
	if(TelAudioNum[2][11]=="no") m_telaudio_edit312 = ""; else m_telaudio_edit312 = TelAudioNum[2][11];
	if(TelAudioNum[2][12]=="no") m_telaudio_edit313 = ""; else m_telaudio_edit313 = TelAudioNum[2][12];
	if(TelAudioNum[2][13]=="no") m_telaudio_edit314 = ""; else m_telaudio_edit314 = TelAudioNum[2][13];
	if(TelAudioNum[2][14]=="no") m_telaudio_edit315 = ""; else m_telaudio_edit315 = TelAudioNum[2][14];
	if(TelAudioNum[2][15]=="no") m_telaudio_edit316 = ""; else m_telaudio_edit316 = TelAudioNum[2][15];

	if(TelAudioNum[3][0]=="no") m_telaudio_edit41 = ""; else m_telaudio_edit41 = TelAudioNum[3][0];
	if(TelAudioNum[3][1]=="no") m_telaudio_edit42 = ""; else m_telaudio_edit42 = TelAudioNum[3][1];
	if(TelAudioNum[3][2]=="no") m_telaudio_edit43 = ""; else m_telaudio_edit43 = TelAudioNum[3][2];
	if(TelAudioNum[3][3]=="no") m_telaudio_edit44 = ""; else m_telaudio_edit44 = TelAudioNum[3][3];
	if(TelAudioNum[3][4]=="no") m_telaudio_edit45 = ""; else m_telaudio_edit45 = TelAudioNum[3][4];
	if(TelAudioNum[3][5]=="no") m_telaudio_edit46 = ""; else m_telaudio_edit46 = TelAudioNum[3][5];
	if(TelAudioNum[3][6]=="no") m_telaudio_edit47 = ""; else m_telaudio_edit47 = TelAudioNum[3][6];
	if(TelAudioNum[3][7]=="no") m_telaudio_edit48 = ""; else m_telaudio_edit48 = TelAudioNum[3][7];
	if(TelAudioNum[3][8]=="no") m_telaudio_edit49 = ""; else m_telaudio_edit49 = TelAudioNum[3][8];
	if(TelAudioNum[3][9]=="no") m_telaudio_edit410 = ""; else m_telaudio_edit410 = TelAudioNum[3][9];
	if(TelAudioNum[3][10]=="no") m_telaudio_edit411 = ""; else m_telaudio_edit411 = TelAudioNum[3][10];
	if(TelAudioNum[3][11]=="no") m_telaudio_edit412 = ""; else m_telaudio_edit412 = TelAudioNum[3][11];
	if(TelAudioNum[3][12]=="no") m_telaudio_edit413 = ""; else m_telaudio_edit413 = TelAudioNum[3][12];
	if(TelAudioNum[3][13]=="no") m_telaudio_edit414 = ""; else m_telaudio_edit414 = TelAudioNum[3][13];
	if(TelAudioNum[3][14]=="no") m_telaudio_edit415 = ""; else m_telaudio_edit415 = TelAudioNum[3][14];
	if(TelAudioNum[3][15]=="no") m_telaudio_edit416 = ""; else m_telaudio_edit416 = TelAudioNum[3][15];
}
