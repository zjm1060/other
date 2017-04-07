// DstSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "transferfile.h"
#include "transferfileDlg.h"
#include "DstSelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


										//hosts文件的完整路径及文件名	
CString HostFilePath="C:\\winnt\\system32\\drivers\\etc\\hosts";

/////////////////////////////////////////////////////////////////////////////
// CDstSelDlg dialog


CDstSelDlg::CDstSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDstSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDstSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDstSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDstSelDlg)
	DDX_Control(pDX, IDC_LISTHOST, m_listHost);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDstSelDlg, CDialog)
	//{{AFX_MSG_MAP(CDstSelDlg)
	ON_BN_CLICKED(IDCONFIRM, OnConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDstSelDlg message handlers

void CDstSelDlg::OnConfirm() 
{
	// TODO: Add your control notification handler code here
		int i,iState,nItemSelected,nItemCount;
		nItemSelected=m_listHost.GetSelectedCount();	//所选表项数
		nItemCount=m_listHost.GetItemCount();			//表项总数#
		if(nItemSelected<1) return;
		for(i=0;i<nItemCount;i++)
		{
			iState=m_listHost.GetItemState(i,LVIS_SELECTED);
			if(iState!=0)							//如果为所选项，则获取其计算机名
			{
				m_curName=m_listHost.GetItemText(i,0);
			}
		}
	CDialog::OnOK();
}


BOOL CDstSelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//初始化HOST计算机列表
	m_listHost.InsertColumn(0, "计算机名");		
	m_listHost.InsertColumn(1, "IP地址");
	m_listHost.SetColumnWidth(0, 150);
	m_listHost.SetColumnWidth(1, 150);	
	//读取hosts文件
	int i;
	int nIndex;
	CStdioFile myfile;
	CString strFilePath;
	CString strline;
	CString strIP;
	CString strName;
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在API中定义了吧，好象是128 
	GetWindowsDirectory(exeFullPath,MAX_PATH);
	HostFilePath=exeFullPath;
	HostFilePath=HostFilePath+"\\system32\\drivers\\etc\\hosts";
	strFilePath=HostFilePath;
	if ( !myfile.Open(strFilePath,CFile::modeRead,NULL) )
	{
		MessageBox(strFilePath+"没有找到。","Error",MB_OK);
		return true;
	}

	while(myfile.ReadString(strline))
	{
		i=strline.Find("#");
		if(strline.Find("#")<0)			//查找是否有#号
		{
			
			if(strline.Find(".")>=0)
			{
				strline.TrimLeft();
				strline.TrimRight();
				strline.Replace("	"," ");
				i=strline.Find(" ");
				strIP=strline.Left(i);
				strName=strline.Right(strline.GetLength()-i-1);
				strName.TrimLeft();
				nIndex = m_listHost.InsertItem(0,strName);
				m_listHost.SetItemText(nIndex, 1, strIP);
			
			}
		}
		//FileContainer.Add(strline);
	}

	myfile.Close();

	UpdateData(false);
	return true;
}

//从HOST文件中读取计算机列表
BOOL CDstSelDlg::GetListFromHost()
{
	CString strtmp;
	strtmp=HostFilePath;
	return true;
}
