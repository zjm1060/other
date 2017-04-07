// transferfileDlg.h : header file
//

#if !defined(AFX_TRANSFERFILEDLG_H__698D9093_182E_4764_B4EF_C96F07D9717D__INCLUDED_)
#define AFX_TRANSFERFILEDLG_H__698D9093_182E_4764_B4EF_C96F07D9717D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TransferConfig.h"				// 配置信息类的头文件

//自定义消息
const WM_THREADPROC=WM_USER+100;		//传输进程消息
//自定义常量

/////////////////////////////////////////////////////////////////////////////
// CTransferfileDlg dialog


class CTransferfileDlg : public CDialog
{
// Construction
public:
	CTransferfileDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTransferfileDlg)
	enum { IDD = IDD_TRANSFERFILE_DIALOG };
	CButton	m_buttonSet;
	CButton	m_buttonGet;
	CButton	m_buttonCancel;
	CButton	m_buttonSave;
	CButton	m_buttonAddDst;
	CButton	m_starttransfer;
	CEdit	m_editLogCtrl;
	CListCtrl	m_listDst;
	CString	m_editLog;
	int		m_radioVer;
	int		m_radioDriver;
	BOOL	m_check1;
	BOOL	m_check2;
	BOOL	m_check3;
	BOOL	m_check4;
	BOOL	m_check5;
	BOOL	m_check6;
	BOOL	m_check7;
	CTransferConfig m_tmpConfig;
	BOOL	m_check8;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferfileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTransferfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAddDst();
	afx_msg void OnDelDst();
	afx_msg void OnModifyDst();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnStarttransfer();
	afx_msg void OnAdddst();
	afx_msg void OnSaveconfig();
	afx_msg void OnStartget();
	afx_msg void OnGetmenu();
	virtual void OnCancel();
	afx_msg void OnSetFilter();
	afx_msg void OnRadiodriverf();
	afx_msg void OnRadiodrivere();
	afx_msg void OnRadiodriverd();
	afx_msg void OnClickListdst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfilter1();
	afx_msg void OnSetfilter2();
	afx_msg void OnSetfilter3();
	afx_msg void OnSetfilter4();
	afx_msg void OnSetfilter5();
	afx_msg void OnSetfilter6();
	afx_msg void OnSetfilter7();
	afx_msg void OnRadiodriverc();
	afx_msg void OnRadiover2();
	afx_msg void OnRadiover3();
	afx_msg void OnEnablemenu();
	afx_msg void OnSetfilter8();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnableallmenu();
	afx_msg void OnDisenableallmenu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg long UpdateLog(WPARAM wParam, LPCTSTR lParam);	//更新日志显示框
	long UpdateLogLine(CString MyStrLine);				//更新日志显示框，简单方式
	bool UpdateIPList();			//更新IP列表，通过m_config变量
	bool UpdatePubConfig();		//更新公用配置信息
	bool TransferControl();			//传输控制
	bool ShowFileDlg(int DlgID);	//显示文件过滤对话框，DlgID为文件类型编号	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERFILEDLG_H__698D9093_182E_4764_B4EF_C96F07D9717D__INCLUDED_)

	UINT ThreadProc(LPVOID param);
	void MyMsgBox(CString MyMsg);
	bool WriteToFtpTxt(CString mystrBat,CString mystrTxt);
