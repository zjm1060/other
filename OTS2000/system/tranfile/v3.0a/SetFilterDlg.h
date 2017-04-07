#if !defined(AFX_SETFILTERDLG_H__A833A937_6663_4532_BD89_295D2C135266__INCLUDED_)
#define AFX_SETFILTERDLG_H__A833A937_6663_4532_BD89_295D2C135266__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetFilterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetFilterDlg dialog

class CSetFilterDlg : public CDialog
{
// Construction
public:
	CSetFilterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetFilterDlg)
	enum { IDD = IDD_SETFILTERDLG };
	CString	m_editF1;
	CString	m_editF2;
	CString	m_editF3;
	CString	m_editF4;
	CString	m_editF5;
	CString	m_editF6;
	CString	m_editF7;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetFilterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetFilterDlg)

	afx_msg void OnSetdefault();
	afx_msg void OnConfirmset();
	afx_msg void OnCancelset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETFILTERDLG_H__A833A937_6663_4532_BD89_295D2C135266__INCLUDED_)
