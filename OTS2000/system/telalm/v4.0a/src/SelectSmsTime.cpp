// SelectSmsTime.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/SelectSmsTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectSmsTime dialog


CSelectSmsTime::CSelectSmsTime(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSmsTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSmsTime)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CSelectSmsTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectSmsTime)
	DDX_Text(pDX, IDC_SMS_TIME, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectSmsTime, CDialog)
	//{{AFX_MSG_MAP(CSelectSmsTime)
	ON_BN_CLICKED(ID_SAVE_SMS_TIME, OnSaveSmsTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectSmsTime message handlers

void CSelectSmsTime::OnSaveSmsTime() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("确定要保存吗?","警告!!!",MB_OKCANCEL | MB_ICONQUESTION )==IDOK)		        	
	{		
		FILE  *fp=NULL;
		UpdateData(TRUE); 
		fp=fopen(SMSTIME_FILE,"w"); fprintf(fp,"%s",m_edit);	fclose(fp);
	}		
	
}

void CSelectSmsTime::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
