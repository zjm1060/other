// netpocDlg.cpp : implementation file
//

#include "stdafx.h"
#include "netpoc.h"
#include "netpocDlg.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

CNetpocDlg *g_pMainDlg;
BOOL CNetpocDlg::m_bExit = FALSE;
UINT CNetpocDlg::m_nInputMsg = RegisterWindowMessage(_T("SendNetMsg"));

ANA_MSG	ml_buf_ANA[64];
SYS_MSG	ml_buf_IND[64];
int Number_IND=0;
int Number_ANA=0;
LAN_HEAD	lanhd_ANA,lanhd_IND;
int UpdateRTDB_ANA (char *,float);
int UpdateRTDB_IND (char *,short);


IN_ADDR GetBroadcastAddr()
{
  char szHost[128];
  gethostname(szHost, 128);
  LPHOSTENT lpHostEnt = gethostbyname(szHost);
  IN_ADDR in_addr;
  memcpy((void*) &in_addr, (void*)lpHostEnt->h_addr_list[0], sizeof(IN_ADDR));
  in_addr.S_un.S_un_b.s_b4 = 255;
  return in_addr;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg: public CDialog
{
  public:
    CAboutDlg();

    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum
    {
        IDD = IDD_ABOUTBOX
    };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
  protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
  protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(): CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetpocDlg dialog

CNetpocDlg::CNetpocDlg(CWnd *pParent /*=NULL*/): CDialog(CNetpocDlg::IDD,
  pParent)
{
  //{{AFX_DATA_INIT(CNetpocDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  m_bInit = FALSE;

  m_pAIs = NULL;
  m_pDIs = NULL;
  m_pAOs = NULL;
  m_pDOs = NULL;

  m_SctAIMap.InitHashTable(2999);
  m_SctDIMap.InitHashTable(2999);
  m_SctAOMap.InitHashTable(2999);
  m_SctDOMap.InitHashTable(2999);

  //g_Dlg = this;

  m_nRecPack = 0;
  m_bSetServer = TRUE;
  m_hSendSocket = INVALID_SOCKET;
}

void CNetpocDlg::DoDataExchange(CDataExchange *pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CNetpocDlg)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetpocDlg, CDialog)
//{{AFX_MSG_MAP(CNetpocDlg)
ON_WM_SYSCOMMAND()ON_WM_PAINT()ON_WM_QUERYDRAGICON()ON_REGISTERED_MESSAGE
  (m_nInputMsg, HandleMessage)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetpocDlg message handlers

BOOL CNetpocDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  this->MoveWindow( - 100,  - 100, 0, 0); //隐藏窗体本身------------------------
  ShowWindow(SW_HIDE);
  ::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
  //隐藏任务栏---------
  ShowWindow(SW_SHOWNORMAL);
  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX &0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu *pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL)
  {
    CString strAboutMenu;
    strAboutMenu.LoadString(IDS_ABOUTBOX);
    if (!strAboutMenu.IsEmpty())
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE); // Set big icon
  SetIcon(m_hIcon, FALSE); // Set small icon

  // TODO: Add extra initialization here
  g_pMainDlg = this;
  Init(TRUE); //关键初始化
  return TRUE; // return TRUE  unless you set the focus to a control
}

void CNetpocDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID &0xFFF0) == IDM_ABOUTBOX)
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNetpocDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialog::OnPaint();
  }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNetpocDlg::OnQueryDragIcon()
{
  return (HCURSOR)m_hIcon;
}

void CNetpocDlg::Init(BOOL bFirstInit)
{
  m_bInit = FALSE;
  //
  m_nAI = 0;
  m_nDI = 0;
  m_nAO = 0;
  m_nDO = 0;

  /*if(m_pAIs!=NULL)
  {
  delete[] m_pAIs;
  m_pAIs = NULL;
  }
  if(m_pAOs!=NULL)
  {
  delete[] m_pAOs;
  m_pAOs = NULL;
  }
  if(m_pDIs!=NULL)
  {
  delete[] m_pDIs;
  m_pDIs = NULL;
  }
  if(m_pDOs!=NULL)
  {
  delete[] m_pDOs;
  m_pDOs = NULL;
  }*/

  //获取服务器SCT库点

  char *pChr;
  float *pfvs;
  int *pivs;
  int num;


  ///创建共享
  CSctVar *pVar;

  int AONUM = 0;
  int DONUM = 0;
  m_InVarArray.RemoveAll();
  m_DoVarArray.RemoveAll();
  m_InVarArray.RemoveAll();

  char strFilePath[MAX_PATH];
  GetModuleFileName(NULL, strFilePath, MAX_PATH);
  *strrchr(strFilePath, '\\') = '\0'; //截取路径E:\Tools

  CString strFile;
  CStdioFile file;

  strFile = CString(strFilePath) + "\\dcs.pva";
  if (file.Open(strFile, CFile::modeRead))
  {
    CString strLine;
    while (file.ReadString(strLine))
    {
      strLine.TrimLeft();
      strLine.TrimRight();
      if (strLine != "")
      {
        int ipos = strLine.Find(",");
        if (ipos > 0)
        {
          int index = atoi(strLine.Left(ipos));
          strLine = strLine.Right(strLine.GetLength() - ipos - 1);

		  pVar = new CSctVar();
          strcpy(pVar->m_szName, strLine);
          pVar->m_idxDB = index;
          m_AoVarArray.Add((DWORD)pVar);
		  

        }
        AONUM += 1;
      }
    }
    m_nAO = AONUM; //模拟量总数
    file.Close();
  }

  strFile = CString(strFilePath) + ("\\dcs.pvd");
  if (file.Open(strFile, CFile::modeRead))
  {
    CString strLine;
    while (file.ReadString(strLine))
    {
      strLine.TrimLeft();
      strLine.TrimRight();
      if (strLine != "")
      {
        int ipos = strLine.Find(",");
        if (ipos > 0)
        {
          int index = atoi(strLine.Left(ipos));
          strLine = strLine.Right(strLine.GetLength() - ipos - 1);

		  pVar = new CSctVar();
          strcpy(pVar->m_szName, strLine);
          pVar->m_idxDB = index;
          m_DoVarArray.Add((DWORD)pVar);
		  
        }
        DONUM += 1;
      }
    }
    m_nDO = DONUM;
    file.Close();
  }

  strFile = CString(strFilePath) + ("\\dcs.pvi");
  if (file.Open(strFile, CFile::modeRead))
  {
    CString strLine;
    while (file.ReadString(strLine))
    {
      strLine.TrimLeft();
      strLine.TrimRight();
      if (strLine != "")
      {
        int ipos = strLine.Find(",");
        if (ipos > 0)
        {
          int index = atoi(strLine.Left(ipos));
          strLine = strLine.Right(strLine.GetLength() - ipos - 1);

          pVar = new CSctVar();
          strcpy(pVar->m_szName, strLine);
          pVar->m_idxDB = index;
          m_InVarArray.Add((DWORD)pVar);
        }
      }
    }

    file.Close();
  }
  ///

  HANDLE hMap;

  if (bFirstInit)
  {
    hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0,
      sizeof(float) *AONUM + DONUM, "RecvData");
    m_pPVAValue = (float*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    m_pPVDValue = (bool*)((char*)m_pPVAValue + AONUM * sizeof(float));
    hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0,
      sizeof(in_struct), "SendData");
    m_pInValue = (in_struct*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
  }

  //赋初值
  /*
  CString strKey;
  POSITION pos = m_SctAOMap.GetStartPosition();
  while(pos)
  {
  m_SctAOMap.GetNextAssoc(pos,strKey,pVar);
  if(pVar->m_idxDB>=0)
  {
   *(m_pPVAValue + pVar->m_idxDB) = pVar->m_value.a;
  }
  }

  pos = m_SctDOMap.GetStartPosition();
  while(pos)
  {
  m_SctDOMap.GetNextAssoc(pos,strKey,pVar);
  if(pVar->m_idxDB>=0)
  {
   *(m_pPVDValue + pVar->m_idxDB) = (bool)(pVar->m_value.d);
  }
  }*/

  if (bFirstInit)
  {
    if (m_hSendSocket == INVALID_SOCKET)
    {
      m_hSendSocket = socket(AF_INET, SOCK_DGRAM, 0);
      if (m_hSendSocket != INVALID_SOCKET)
      {
        BOOL bFlag = TRUE;
        int nSize = UDP_BUFFER_SIZE;
        setsockopt(m_hSendSocket, SOL_SOCKET, SO_BROADCAST, (char*) &bFlag,
          sizeof(BOOL));
        setsockopt(m_hSendSocket, SOL_SOCKET, SO_SNDBUF, (char*) &nSize, sizeof
          (nSize));
      }

      char szHost[20];
      gethostname(szHost, 20);

      strcpy(m_comhead.szHost, szHost);

      m_pUPDRecvThread = AfxBeginThread((AFX_THREADPROC)ReceiveProc, this);
    }
  }
  m_bInit = TRUE;
}

void CNetpocDlg::DealWithReceive(char *szBuffer, DWORD dwBytes)
{

// 	SW_MSG		sw_buf[70];
// //	ALL_SW_MSG	all_sw_buf[70];
// 	unsigned char	me_buf[1440];
// 	LAN_HEAD	lanhead;
// 	IND_DATA	sw_status;
// 	DMS_COMMON	dbentry;

  if (!m_bInit)
    return ;

  CString strmsg;
  COM_HEAD head;
  memcpy(&head, szBuffer, sizeof(COM_HEAD));
  if (strcmp(szBuffer,"192.169.11.123"))
  {
	  return;
  }
  //if(strcmp(head.szHost,m_comhead.szHost)!=0)return; //不处理
  if (head.nSize <= 0 /*||head.nModNum<1||head.nModNum>4*/)
    return ;
  COM_BODY *pBody = new COM_BODY[head.nSize];
  memcpy(pBody, szBuffer + sizeof(COM_HEAD), sizeof(COM_BODY) *head.nSize);

  int iNumber_A = 0;
  int iNumber_D = 0;
  CSctVar* pVar;


  int index;
  for (int i = 0; i < head.nSize; i++)
  {
    //test
    if (pBody[i].index >  - 1 && pBody[i].index <= m_nAO)
    {
    //  *(m_pPVAValue + pBody[i].index) = pBody[i].value.a;
	  	  pVar = (CSctVar*)(m_AoVarArray.GetAt(pBody[i].index));
		 //CString name = pVar->m_szName;

	 	UpdateRTDB_ANA (pVar->m_szName,pBody[i].value.a);
		if(Number_ANA>0)
        {
			lan_out ((char *)&ml_buf_ANA,lanhd_ANA,(unsigned char)0);
			Number_ANA=0;
        }

      TRACE("pva %d:%f\n", pBody[i].index,pBody[i].value.a);
    }
    else if (pBody[i].index > m_nAO && pBody[i].index <= m_nAO + m_nDO)
    {
    //  *(m_pPVDValue + pBody[i].index - m_nAO) = pBody[i].value.d;
      //TRACE("pvd %d:%d\n", pPack->pBody[i].index,d);			
	  pVar = (CSctVar*)(m_DoVarArray.GetAt(pBody[i].index-m_nAO));
	  //CString name = pVar->m_szName;

	  UpdateRTDB_IND (pVar->m_szName,pBody[i].value.d);
	  if(Number_IND>0)
	  {
		  lan_out ((char *)&ml_buf_IND,lanhd_IND,(unsigned char)0);
		  Number_IND=0;
       }

    }

  }
  delete []pBody;
  pBody = NULL;
}


void CNetpocDlg::HandleMessage(UINT, LONG lParam)
{
  if (!m_bInit)
    return ;

  CSctVar *pVar;
  CString strmsg;

  g_pMainDlg->m_comhead.nSize = 0;
  g_pMainDlg->m_comhead.nModNum = g_pMainDlg->m_iMod;

  pVar = (CSctVar*)(m_InVarArray.GetAt(m_pInValue->index));
  if (pVar == NULL)
    return ;

  /*if(pVar->m_index>m_nAI)//nu 此处无法区分AI/DI
  {
  pVar->m_value.d = (int)(m_pInValue->fvalue);
  }
  else
  pVar->m_value.a = m_pInValue->fvalue;*/

  pVar->m_value.a = m_pInValue->fvalue; //nu 此处无法区分AI/DI

  g_pMainDlg->m_combody[g_pMainDlg->m_comhead.nSize].index = pVar->m_idxDB;
  g_pMainDlg->m_combody[g_pMainDlg->m_comhead.nSize].value = pVar->m_value;
  g_pMainDlg->m_comhead.nSize += 1;

  //OperatorRecord(pVar,TRUE);			

  g_pMainDlg->SendTo();
}

BOOL CNetpocDlg::SendTo()
{

  char szBuffer[UDP_BUFFER_SIZE];
  DWORD dwSize;

  memcpy(szBuffer, &m_comhead, sizeof(m_comhead));
  dwSize = sizeof(m_comhead);

  memcpy(szBuffer + dwSize, m_combody, sizeof(COM_BODY) *m_comhead.nSize);
  dwSize += sizeof(COM_BODY) *m_comhead.nSize;


  WSABUF buff;
  buff.len = dwSize;
  buff.buf = szBuffer;

  SOCKADDR_IN Address;
  memset(&Address, 0, sizeof(Address));
  Address.sin_family = AF_INET;
  Address.sin_port = htons(COM_INUDP_PORT);
  IN_ADDR Addr = GetBroadcastAddr();
  Address.sin_addr.s_addr = Addr.s_addr;
  //	Address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  DWORD dwSend;
  if (m_hSendSocket != INVALID_SOCKET)
  {
    if (WSASendTo(m_hSendSocket, &buff, 1, &dwSend, 0, (SOCKADDR*) &Address,
      sizeof(Address), NULL, NULL) || dwSend != buff.len)
    {
      TRACE("WSASendTo Error:%d\n", WSAGetLastError());
      return FALSE;
    }
  }
  return TRUE;
}

UINT CNetpocDlg::ReceiveProc(LPVOID lpv)
{
  CNetpocDlg *pDlg = (CNetpocDlg*)lpv;

  SOCKET m_hSocket;
  SOCKADDR_IN Address;
  memset(&Address, 0, sizeof(Address));
  Address.sin_family = AF_INET;
  Address.sin_port = htons(COM_OUTUDP_PORT);
  Address.sin_addr.s_addr = htonl(INADDR_ANY);

  m_hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0,
    WSA_FLAG_OVERLAPPED);
  if (m_hSocket == INVALID_SOCKET)
    return 0;
  BOOL bEnable = TRUE;
  VERIFY(!setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &bEnable,
    sizeof(BOOL)));
  if (bind(m_hSocket, (SOCKADDR*) &Address, sizeof(Address)) != 0)
  {
    closesocket(m_hSocket);
    m_hSocket = INVALID_SOCKET;
    return 0;
  }


  SOCKADDR_IN SockAddr;


  DWORD dwBytes;
  int nRcvRet;
  memset(&SockAddr, 0, sizeof(SockAddr));
  int SockAddrLen = sizeof(SockAddr);
  char szBuffer[UDP_BUFFER_SIZE];

  while (!m_bExit)
  {
    WSABUF wsabuff;
    wsabuff.len = UDP_BUFFER_SIZE;
    wsabuff.buf = szBuffer;
    DWORD dwRecvFlags = 0;
    nRcvRet = WSARecvFrom(m_hSocket, &wsabuff, 1, &dwBytes, &dwRecvFlags,
      (sockaddr*) &SockAddr, &SockAddrLen, NULL, NULL);
    if (nRcvRet == 0)
    {
      if (dwBytes == 0)
      {
        TRACE("WSARecvFrom 0 byte\n");
        Sleep(30000);
        continue;
      }
      int curbytes = 0;
      while (curbytes < dwBytes)
      {
        REC_PACK *pPack = new REC_PACK;
        pPack->bOut = TRUE;
        memcpy(&(pPack->head), szBuffer + curbytes, sizeof(COM_HEAD));

        pPack->pBody = new COM_BODY[pPack->head.nSize];
        memcpy(pPack->pBody, szBuffer + sizeof(COM_HEAD) + curbytes, sizeof
          (COM_BODY) *pPack->head.nSize);

        curbytes += sizeof(COM_HEAD) + sizeof(COM_BODY) *pPack->head.nSize;

        if (pPack->pBody->index == 0)
        //命令处理
        {
          /*if(pPack->pBody->value.d>=CMD_LOAD)
          {
          pDlg->GetSCTValue();
          }*/
        }
        else
        {
          pDlg->DealWithReceive(szBuffer, dwBytes);
        }

        delete []pPack->pBody;
        delete pPack;
      }
    }
    else
    {
      TRACE("WSARecvFrom Error:%d\n", WSAGetLastError());
      Sleep(30000); // sleep 30 second the continue;
    }
  }
  TRACE("UDP Receive thread terminates\n");
  return 0;
}


int UpdateRTDB_ANA (char *PointName,float data)
{
     DMS_COMMON common_aa;
     char err_str[80]="";
    time_t  tim;
    struct tm   tmpt;
    int value;
    float OldValue=0;
    value=data*1000;
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
	  sprintf(err_str,"No this point %s\n",PointName);
	  Err_log(err_str);
          return(-1);
     }
    
    OldValue=common_aa.var.anlg.fValue; 	
    if(fabs(OldValue-data)<0.001)
        return 0;    
    
    ml_buf_ANA[Number_ANA].type_id =DPS_ANALOG;
    ml_buf_ANA[Number_ANA].stn_id =common_aa.point.stn_id;
    ml_buf_ANA[Number_ANA].dev_id =common_aa.point.dev_id;
    ml_buf_ANA[Number_ANA].data_type=common_aa.point.data_type;
    ml_buf_ANA[Number_ANA].number=1;
    ml_buf_ANA[Number_ANA].point_id = common_aa.point.pt_id;

    ml_buf_ANA[Number_ANA].status=0;
    memcpy(ml_buf_ANA[Number_ANA].data,&value,4);
    GetLocalHostId(&ml_buf_ANA[Number_ANA].host_id);

    tim=time (NULL);
    tmpt=*localtime (&tim);
    ml_buf_ANA[Number_ANA].sec=tmpt.tm_sec;
    ml_buf_ANA[Number_ANA].min=tmpt.tm_min;
    ml_buf_ANA[Number_ANA].hour=tmpt.tm_hour;
    ml_buf_ANA[Number_ANA].day=tmpt.tm_mday;
    ml_buf_ANA[Number_ANA].month=tmpt.tm_mon+1;
    ml_buf_ANA[Number_ANA].year=tmpt.tm_year+1900;
    
    Number_ANA+=1;
    lanhd_ANA.length=ANAMSG_SIZE*Number_ANA;
    
    lanhd_ANA.dp_type=BJ_DB_TIME;
    lanhd_ANA.dest_id =0;
    GetLocalHostId(&lanhd_ANA.src_id);
    lanhd_ANA.dest_stn =ml_buf_ANA[0].stn_id;
    lanhd_ANA.packet_num =0; 
    if(Number_ANA>=64)
    {
        lan_out ((char *)&ml_buf_ANA,lanhd_ANA,(unsigned char)0); 
        Number_ANA=0; 
    }
    return 0;
}



int UpdateRTDB_IND (char *PointName,short data)
{
   DMS_COMMON common_aa; 
   char err_str[80]="";
    time_t  tim;
    struct tm   tmpt;
    int OldValue;
    if (ReadEntryByNameStr(PointName,&common_aa)==-1)
    {
	sprintf(err_str,"No this point %s\n",PointName);
	Err_log(err_str);
	return(-1);
    }
	
    OldValue=common_aa.var.ind.status.state;  
    if(OldValue==data)
        return  0;

    ml_buf_IND[Number_IND].type_id =DPS_IND_ALM;
    ml_buf_IND[Number_IND].stn_id =common_aa.point.stn_id;
    ml_buf_IND[Number_IND].dev_id =common_aa.point.dev_id;
    ml_buf_IND[Number_IND].data_type=common_aa.point.data_type;
    ml_buf_IND[Number_IND].number=1;
    ml_buf_IND[Number_IND].point_id = common_aa.point.pt_id;

    ml_buf_IND[Number_IND].status=data+256;
 
    GetLocalHostId(&ml_buf_IND[Number_IND].host_id);
    tim=time (NULL);
    tmpt=*localtime (&tim);
    ml_buf_IND[Number_IND].sec=tmpt.tm_sec; 
    ml_buf_IND[Number_IND].min=tmpt.tm_min;
    ml_buf_IND[Number_IND].hour=tmpt.tm_hour;
    ml_buf_IND[Number_IND].day=tmpt.tm_mday;
    ml_buf_IND[Number_IND].month=tmpt.tm_mon+1;
    ml_buf_IND[Number_IND].year=tmpt.tm_year+1900;
    lanhd_IND.length=SYSMSG_SIZE*(Number_IND+1);


    lanhd_IND.dp_type=BJ_DB_TIME;
    lanhd_IND.dest_id =0;
    GetLocalHostId(&lanhd_IND.src_id);
    lanhd_IND.dest_stn =ml_buf_IND[0].stn_id;
    lanhd_IND.packet_num =0;
    
    Number_IND+=1;
 
    
    if(Number_IND>=64)
    {
         lan_out ((char *)&ml_buf_IND,lanhd_IND,(unsigned char)0);
         Number_IND=0;
    }
    return 0;
}