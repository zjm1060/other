#if !defined(AFX_SMSSEND_H__6163D58D_B069_4814_8FA0_B46A443F8246__INCLUDED_)
#define AFX_SMSSEND_H__6163D58D_B069_4814_8FA0_B46A443F8246__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsSend.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmsSend dialog

class CSmsSend : public CDialog
{
// Construction

public:
	void GetDataForDsp(), GetDataForSave();

public:
	CSmsSend(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmsSend)
	enum { IDD = IDD_SMS_SEND };
	CScrollBar	m_smssend_scrollbar;
	CButton	m_smssend_save;
	CButton	m_smssend_get;
	CString	m_edit_page;
	CString	m_smssend_edit11;
	CString	m_smssend_edit110;
	CString	m_smssend_edit12;
	CString	m_smssend_edit13;
	CString	m_smssend_edit14;
	CString	m_smssend_edit15;
	CString	m_smssend_edit16;
	CString	m_smssend_edit17;
	CString	m_smssend_edit18;
	CString	m_smssend_edit19;
	CString	m_smssend_edit21;
	CString	m_smssend_edit210;
	CString	m_smssend_edit22;
	CString	m_smssend_edit23;
	CString	m_smssend_edit24;
	CString	m_smssend_edit25;
	CString	m_smssend_edit26;
	CString	m_smssend_edit27;
	CString	m_smssend_edit28;
	CString	m_smssend_edit29;
	CString	m_smssend_edit31;
	CString	m_smssend_edit310;
	CString	m_smssend_edit32;
	CString	m_smssend_edit33;
	CString	m_smssend_edit34;
	CString	m_smssend_edit35;
	CString	m_smssend_edit36;
	CString	m_smssend_edit37;
	CString	m_smssend_edit38;
	CString	m_smssend_edit39;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmsSend)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmsSend)
	afx_msg void OnSmssendSave();
	afx_msg void OnSmssendGet();
	afx_msg void OnSmssendUppage();
	afx_msg void OnSmssendDownpage();
	afx_msg void OnChangeEditPage();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSmssendEdit18();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSSEND_H__6163D58D_B069_4814_8FA0_B46A443F8246__INCLUDED_)
