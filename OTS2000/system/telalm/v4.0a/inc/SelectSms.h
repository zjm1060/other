#if !defined(AFX_SELECTSMS_H__1FDDEEFB_EC2C_4786_882C_106A807B5B52__INCLUDED_)
#define AFX_SELECTSMS_H__1FDDEEFB_EC2C_4786_882C_106A807B5B52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectSms.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectSms dialog

class CSelectSms : public CDialog
{
// Construction
public:
	CSelectSms(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectSms)
	enum { IDD = IDD_SELECE_SMS };
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectSms)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectSms)
	afx_msg void OnSaveSmsCenter();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSMS_H__1FDDEEFB_EC2C_4786_882C_106A807B5B52__INCLUDED_)
