#if !defined(AFX_DSTSELDLG_H__0A2A1543_A021_4868_8DCF_F7FB1BA160A7__INCLUDED_)
#define AFX_DSTSELDLG_H__0A2A1543_A021_4868_8DCF_F7FB1BA160A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DstSelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDstSelDlg dialog

class CDstSelDlg : public CDialog
{
// Construction
public:
	CDstSelDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL GetListFromHost();
// Dialog Data
	//{{AFX_DATA(CDstSelDlg)
	enum { IDD = IDD_DSTSELDLG };
	CListCtrl	m_listHost;
	CString m_curName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDstSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDstSelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnConfirm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSTSELDLG_H__0A2A1543_A021_4868_8DCF_F7FB1BA160A7__INCLUDED_)
