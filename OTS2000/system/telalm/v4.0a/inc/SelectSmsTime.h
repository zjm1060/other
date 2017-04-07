#if !defined(AFX_SELECTSMSTIME_H__F873C0C8_5934_4B64_A68D_49438C6987E1__INCLUDED_)
#define AFX_SELECTSMSTIME_H__F873C0C8_5934_4B64_A68D_49438C6987E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectSmsTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectSmsTime dialog

class CSelectSmsTime : public CDialog
{
// Construction
public:
	CSelectSmsTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectSmsTime)
	enum { IDD = IDD_SELECE_TIME };
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectSmsTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectSmsTime)
	afx_msg void OnSaveSmsTime();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSMSTIME_H__F873C0C8_5934_4B64_A68D_49438C6987E1__INCLUDED_)
