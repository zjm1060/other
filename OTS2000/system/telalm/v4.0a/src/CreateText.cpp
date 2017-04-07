// CreateText.cpp : implementation file
//

#include "stdafx.h"
#include "../inc/teldef.h"
#include "../inc/CreateText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateText dialog


CCreateText::CCreateText(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateText)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCreateText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateText)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateText, CDialog)
	//{{AFX_MSG_MAP(CCreateText)
	ON_BN_CLICKED(ID_CREATE_TEXT_OK, OnCreateTextOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateText message handlers

void CCreateText::OnCreateTextOk() 
{
	// TODO: Add your control notification handler code here
	system("\\home\\ems\\h9000\\bin\\AlmClassGen.exe  1 ");	
	FILE *fp=NULL;
	if(fp=fopen("/home/ems/h9000/tmp/flagfile.txt","w"))
		fclose(fp);
	MessageBox("定义文件生成完毕。","警告!!!",MB_ICONEXCLAMATION  );
}




void CCreateText::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
