// mbpscanDlg.h : header file
//

#if !defined(AFX_MBPSCANDLG_H__873C4C82_AF90_4141_BAD3_5A001479DABE__INCLUDED_)
#define AFX_MBPSCANDLG_H__873C4C82_AF90_4141_BAD3_5A001479DABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//自定义消息
const WM_THREADPROC=WM_USER+100;		//传输进程消息

/////////////////////////////////////////////////////////////////////////////
// CMbpscanDlg dialog

class CMbpscanDlg : public CDialog
{
// Construction
public:
	CMbpscanDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMbpscanDlg)
	enum { IDD = IDD_MBPSCAN_DIALOG };
	CEdit	m_editLogCtrl;
	CButton	m_buttonStop;
	CButton	m_buttonStart;
	CString	m_editLog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMbpscanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMbpscanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnStart();
	afx_msg void OnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg long UpdateLog(WPARAM wParam, LPCTSTR lParam);	//更新日志显示框
	long UpdateLogLine(CString MyStrLine);				//更新日志显示框，简单方式
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBPSCANDLG_H__873C4C82_AF90_4141_BAD3_5A001479DABE__INCLUDED_)

	void MyMsgBox(unsigned char* MyMsg);