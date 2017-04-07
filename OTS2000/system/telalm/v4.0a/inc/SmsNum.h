#if !defined(AFX_SMSNUM_H__0CE01BEC_61CA_4EB0_8321_AC4829410278__INCLUDED_)
#define AFX_SMSNUM_H__0CE01BEC_61CA_4EB0_8321_AC4829410278__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsNum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmsNum dialog

class CSmsNum : public CDialog
{
// Construction
public:
	void GetDataForDsp(),	GetDataForSave();

public:
	CSmsNum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmsNum)
	enum { IDD = IDD_SMSNUM };
	CButton	m_num_on2;
	CButton	m_num_off2;
	CButton	m_smsnum_save;
	CButton	m_smsnum_get;
	CButton	m_num_on;
	CButton	m_num_off;
	CScrollBar	m_smsnum_scrollbar;
	BOOL	m_check011;
	BOOL	m_check021;
	BOOL	m_check031;
	BOOL	m_check041;
	BOOL	m_check051;
	BOOL	m_check061;
	BOOL	m_check071;
	BOOL	m_check081;
	BOOL	m_check091;
	BOOL	m_check101;
	CString	m_edit_page;
	CString	m_edit_11;
	CString	m_edit_110;
	CString	m_edit_12;
	CString	m_edit_13;
	CString	m_edit_14;
	CString	m_edit_15;
	CString	m_edit_16;
	CString	m_edit_17;
	CString	m_edit_18;
	CString	m_edit_19;
	CString	m_edit_21;
	CString	m_edit_210;
	CString	m_edit_22;
	CString	m_edit_23;
	CString	m_edit_24;
	CString	m_edit_25;
	CString	m_edit_26;
	CString	m_edit_27;
	CString	m_edit_28;
	CString	m_edit_29;
	BOOL	m_check012;
	BOOL	m_check022;
	BOOL	m_check032;
	BOOL	m_check042;
	BOOL	m_check052;
	BOOL	m_check062;
	BOOL	m_check072;
	BOOL	m_check082;
	BOOL	m_check092;
	BOOL	m_check102;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmsNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmsNum)
	afx_msg void OnSmsnumSave();
	afx_msg void OnSmsnumGet();
	afx_msg void OnSmsnumDownpage();
	afx_msg void OnSmsnumUppage();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1On();
	afx_msg void OnButton1Off();
	afx_msg void OnCheck011();
	afx_msg void OnButton1On2();
	afx_msg void OnButton1Off2();
	afx_msg void OnSmsnumInput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSNUM_H__0CE01BEC_61CA_4EB0_8321_AC4829410278__INCLUDED_)
