// netpocDlg.h : header file
//

#if !defined(AFX_NETPOCDLG_H__E786F080_AF0B_4876_9718_F61FB9CD8308__INCLUDED_)
  #define AFX_NETPOCDLG_H__E786F080_AF0B_4876_9718_F61FB9CD8308__INCLUDED_

  #if _MSC_VER > 1000
    #pragma once
  #endif // _MSC_VER > 1000

  #define COM_OUTUDP_PORT      21207
  #define COM_INUDP_PORT      21208
  #define UDP_BUFFER_SIZE    64000
  #define MAX_AIDI_SIZE     7000

  /////////////////////////////////////////////////////////////////////////////
  // CNetpocDlg dialog
  typedef struct
  {
    char szHost[20];
    BYTE nModNum;
    int nSize;
  } COM_HEAD;

  typedef union
  {
    float a;
    long d;
  } sctvalue;
  typedef struct
  {
    int index;
    sctvalue value;
  } COM_BODY;

  typedef union
  {
    float f[2];
    int l[2];
    char ssbuf[8];
  } sbuf;

  typedef struct
  {
    BOOL bOut;
    COM_HEAD head;
    COM_BODY *pBody;

  } REC_PACK;

  class CSctVar
  {
    public:
      CSctVar()
      {
          m_idxDB =  - 1;
      } int m_index;

      char m_szName[50];
      int m_type; //    SCT AI¡¢DI Type

      sctvalue m_value;

      int m_idxDB;

      int m_idxCyberDB;
  };

  typedef CTypedPtrMap < CMapStringToPtr, CString, CSctVar * > CSctVarMap;

  typedef struct
  {
    float fvalue;
    int index;

  } in_struct;


  /////////////////////////////////////////////////////////////////////////////
  // CNetpocDlg dialog

  class CNetpocDlg: public CDialog
  {
      // Construction
    public:
      CNetpocDlg(CWnd *pParent = NULL); // standard constructor
      static UINT m_nInputMsg;
      // Dialog Data
      //{{AFX_DATA(CNetpocDlg)
      enum
      {
          IDD = IDD_NETPOC_DIALOG
      };
      // NOTE: the ClassWizard will add data members here
      //}}AFX_DATA

      // ClassWizard generated virtual function overrides
      //{{AFX_VIRTUAL(CNetpocDlg)
    protected:
      virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
      //}}AFX_VIRTUAL

    public:
      /*void SetList();
      BOOL  RunHMI();
      void SetStatus();
      void GetSCTValue();

      BOOL InitCyberControl();*/
      void Init(BOOL bFirstInit);
      BOOL SendTo();
      static UINT ReceiveProc(LPVOID lpv);
      //static UINT ReceiveProc2( LPVOID lpv );

      void DealWithReceive(char *szBuffer, DWORD dwBytes);
      //void SendAIDIChange();

      //BOOL m_bCyberControl;




      int m_nRecPack;

      char m_szUserName[50];

      int m_iMod;
      char m_szServer[48];
      CString m_strHMI;
      BOOL m_bSetServer;

      CString m_strHost;
      SOCKET m_hSendSocket;
      BOOL m_bInit;
      UINT m_nTimer;

      COM_HEAD m_comhead;
      COM_BODY m_combody[MAX_AIDI_SIZE];


      CWinThread *m_pUPDRecvThread;
      CWinThread *m_pUPDRecvThread2;

      static BOOL m_bExit;

      ///
      CSctVar *m_pAIs;
      CSctVar *m_pDIs;
      CSctVar *m_pAOs;
      CSctVar *m_pDOs;
      int m_nAI;
      int m_nDI;
      int m_nAO;
      int m_nDO;

      CSctVarMap m_SctAIMap;
      CSctVarMap m_SctDIMap;
      CSctVarMap m_SctAOMap;
      CSctVarMap m_SctDOMap;

      float *m_pPVAValue;
      bool *m_pPVDValue;
      in_struct *m_pInValue;
      CDWordArray m_InVarArray;
	  CDWordArray m_AoVarArray;
	  CDWordArray m_DoVarArray;

      HANDLE m_hMutex;

      BOOL SetDigitPacket(int index, int id32, int idv, int num);
      BOOL SetAnalogPacket(int index, int id32, float fao, int num);

      static DWORD WINAPI ThreadUnpacket(LPDWORD lpdwParam);

      // Implementation
    protected:
      HICON m_hIcon;

      // Generated message map functions
      //{{AFX_MSG(CNetpocDlg)
      virtual BOOL OnInitDialog();
      afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
      afx_msg void OnPaint();
      afx_msg HCURSOR OnQueryDragIcon();
      afx_msg void HandleMessage(UINT, LONG lParam);
      //}}AFX_MSG
      DECLARE_MESSAGE_MAP()
  };

  //{{AFX_INSERT_LOCATION}}
  // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

  
#endif
// !defined(AFX_NETPOCDLG_H__E786F080_AF0B_4876_9718_F61FB9CD8308__INCLUDED_)
