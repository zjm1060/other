// SelectSms.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/SelectSms.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectSms dialog


CSelectSms::CSelectSms(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSms::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSms)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CSelectSms::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectSms)
	DDX_Text(pDX, IDC_SMS_CENTER, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectSms, CDialog)
	//{{AFX_MSG_MAP(CSelectSms)
	ON_BN_CLICKED(ID_SAVE_SMS_CENTER, OnSaveSmsCenter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectSms message handlers

void CSelectSms::OnSaveSmsCenter() 
{	
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{		
		FILE  *fp=NULL;
		UpdateData(TRUE); 
		fp=fopen(SMS_CENTER_FILE,"w"); fprintf(fp,"%s",m_edit);	fclose(fp);
	}		
}

void CSelectSms::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
