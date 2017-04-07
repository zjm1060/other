#if !defined(AFX_DSTPRMDLG_H__C08668B7_679C_406D_867D_B4FB3EA3FABB__INCLUDED_)
#define AFX_DSTPRMDLG_H__C08668B7_679C_406D_867D_B4FB3EA3FABB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DstprmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDstprmDlg dialog

class CDstprmDlg : public CDialog
{
// Construction
public:
	CDstprmDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDstprmDlg)
	enum { IDD = IDD_DSTPRMDIALOG };
	BOOL	m_status;
	CString	m_passwd;
	CString	m_username;
	int		m_radiodriver;
	int		m_radioType;
	CString	m_ip;
	CString	m_pwdConf;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDstprmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDstprmDlg)
	afx_msg void OnConfirm();
	afx_msg void OnGetfromfile();
	afx_msg void OnRadiotype0();
	afx_msg void OnRadiotype1();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSTPRMDLG_H__C08668B7_679C_406D_867D_B4FB3EA3FABB__INCLUDED_)
