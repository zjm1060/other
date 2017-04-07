// teldefView.cpp : implementation of the CTeldefView class
//

#include "stdafx.h"
#include "../inc/teldef.h"

#include "../inc/teldefDoc.h"
#include "../inc/teldefView.h"

#include "../inc/AlmAnaDef.h"
#include "../inc/AlmClsDef.h"
#include "../inc/CreateText.h"
#include "../inc/SelectSms.h"
#include "../inc/SelectSmsTime.h"
#include "../inc/SmsDef.h"
#include "../inc/SmsNum.h"
#include "../inc/SmsSend.h"
#include "../inc/TelDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTeldefView

IMPLEMENT_DYNCREATE(CTeldefView, CView)

BEGIN_MESSAGE_MAP(CTeldefView, CView)
	//{{AFX_MSG_MAP(CTeldefView)
	ON_COMMAND(ID_TEL_INPUT, OnTelInput)
	ON_COMMAND(ID_CLS_DEF, OnClsDef)
	ON_WM_TIMER()
	ON_COMMAND(ID_CREAT_TEXT, OnCreatText)
	ON_COMMAND(ID_SELECT_SMS, OnSelectSms)
	ON_COMMAND(ID_SMS_INPUT, OnSmsInput)
	ON_COMMAND(ID_SMSNUM_INPUT, OnSmsnumInput)
	ON_COMMAND(ID_SMSSEND_INPUT, OnSmssendInput)
	ON_COMMAND(ID_SELECT_SMSTIME, OnSelectSmstime)
	ON_COMMAND(ID_TELDEF_INPUT, OnTeldefInput)
	ON_COMMAND(ID_ANA_DEF, OnAnaDef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeldefView construction/destruction

CTeldefView::CTeldefView()
{
	// TODO: add construction code here

}

CTeldefView::~CTeldefView()
{
}

BOOL CTeldefView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTeldefView drawing

char r=0,g=110,b=0;
void CTeldefView::OnDraw(CDC* pDC)
{
	CTeldefDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	RECT ClientRect;
	GetClientRect( &ClientRect );

	CFont  font;
	font.CreateFont(-70,0,0,0,999,FALSE,FALSE,0,    
		            ANSI_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					VARIABLE_PITCH | FF_ROMAN, "ºÚÌå" );
	pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(r,g,b)); 
	pDC->SetBkMode(TRANSPARENT);   
	

	pDC->DrawText( " H9000 ONCALL System",-1,&ClientRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	

 	KillTimer(1);
	if (!SetTimer(1, (int)(0.5*1000), NULL))
	{
		MessageBox(_T("Not enough timers available for this window."),
				_T("MDI:Bounce"), MB_ICONEXCLAMATION | MB_OK);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CTeldefView diagnostics

#ifdef _DEBUG
void CTeldefView::AssertValid() const
{
	CView::AssertValid();
}

void CTeldefView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTeldefDoc* CTeldefView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTeldefDoc)));
	return (CTeldefDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTeldefView message handlers

extern CString	TelAudioNum[MAX_ALM_CLASS][MAX_TEL_NUM];  //[cls][num]
//extern CString	TelCallNum[MAX_ALM_CLASS][MAX_BP_NUM];
extern CString	TelSmsNum[MAX_ALM_CLASS][MAX_MOBILE_NUM];
extern CString	TelDef[MAX_TELDEF_NUM][MAX_TELDEF_STY];

void CTeldefView::OnTelInput() 
{
	// TODO: Add your command handler code here
	CProSheet myProperty("Input the phone number",this,0);	

	GetTelephoneNum();
	GetMobileNum();
	
	myProperty.m_telaudio.GetDataForDsp();
//	myProperty.m_telcall.GetDataForDsp();
	myProperty.m_telsms.GetDataForDsp();

	int result = myProperty.DoModal();
}


extern			REAL_SW RealSW[MAX_SW_NUM];
extern	int 	PageNum,	CurrentPage;

void CTeldefView::OnClsDef() 
{
	// TODO: Add your command handler code here
	CAlmClsDef	almclsdef;
			
	ReadDefineFile(0);
	almclsdef.GetDataForDsp();
	if( almclsdef.DoModal()==IDOK )
	{
	}
}


void CTeldefView::OnCreatText() 
{
	// TODO: Add your command handler code here
	CCreateText createtext;
	if( createtext.DoModal()==IDOK )
	{
	}
}


void CTeldefView::OnSelectSms() 
{
	// TODO: Add your command handler code here
	CSelectSms selectsms;
	if( _access(SMS_CENTER_FILE,0)!=-1 )
	{	char strTemp[50];
		FILE *fp;
		fp=fopen(SMS_CENTER_FILE,"r"); 	fscanf(fp,"%s",strTemp);  fclose(fp);			
		selectsms.m_edit = strTemp;
	}
	else
	{
		selectsms.m_edit = DEFAULT_SMS_CENTER;
	}
	if( selectsms.DoModal()==IDOK )
	{
	}
}


void CTeldefView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CTeldefDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	r=r+3;  if(r>255)r= 0;
	g=g+7;  if(g>255)g= 0;
	b=b+11; if(b>255)b= 0;

	Invalidate(TRUE);
	CView::OnTimer(nIDEvent);
}

void CTeldefView::OnSmsInput() 
{
	// TODO: Add your command handler code here
    CSmsDef csmsdef;
	GetSmsDef();	
	if( csmsdef.DoModal()==IDOK )
	{
	}
}

void CTeldefView::OnSmsnumInput() 
{
	// TODO: Add your command handler code here
    CSmsNum csmsnum;
	GetSmsNum();	
	if( csmsnum.DoModal()==IDOK )
	{
	}	
}

void CTeldefView::OnSmssendInput() 
{
	// TODO: Add your command handler code here
    CSmsSend csmssend;
	GetSmsSend();	
	if( csmssend.DoModal()==IDOK )
	{
	}	
}

void CTeldefView::OnSelectSmstime() 
{
	// TODO: Add your command handler code here
	CSelectSmsTime selectsmstime;
	if( _access(SMS_CENTER_FILE,0)!=-1 )
	{	char strTemp[50];
		FILE *fp;
		fp=fopen(SMSTIME_FILE,"r"); 	fscanf(fp,"%s",strTemp);  fclose(fp);			
		selectsmstime.m_edit = strTemp;
	}
//	else
//	{
//		selectsmstime.m_edit = DEFAULT_SMS_TIME;
//	}
	if( selectsmstime.DoModal()==IDOK )
	{
	}	
}

void CTeldefView::OnTeldefInput() 
{
	// TODO: Add your command handler code here
    CTelDefine cteldefine;
	GetTelDef();	
	if( cteldefine.DoModal()==IDOK )
	{
	}
}

void CTeldefView::OnAnaDef() 
{
	// TODO: Add your command handler code here
	CAlmAnaDef	almanadef;
			
	ReadAnaDefineFile(0);
	almanadef.GetDataForDsp();
	if( almanadef.DoModal()==IDOK )
	{
	}	
}
