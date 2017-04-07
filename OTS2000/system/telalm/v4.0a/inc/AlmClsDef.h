#if !defined(AFX_ALMCLSDEF_H__A59E43FC_28B0_4317_9642_953987031C1E__INCLUDED_)
#define AFX_ALMCLSDEF_H__A59E43FC_28B0_4317_9642_953987031C1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlmClsDef.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlmClsDef dialog

class CAlmClsDef : public CDialog
{
// Construction
public:
	void GetDataForDsp(),	GetDataForSave();
	    
public:
	CAlmClsDef(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlmClsDef)
	enum { IDD = IDD_ALMCLSDEF };
	CScrollBar	m_stnselect_scrollbar;
	CScrollBar	m_almclsdef_scrollbar;
	CButton	m_cls4_on;
	CButton	m_cls4_off;
	CButton	m_cls3_on;
	CButton	m_cls3_off;
	CButton	m_cls2_on;
	CButton	m_cls2_off;
	CButton	m_cls1_on;
	CButton	m_cls1_off;
	CString	m_edit01;
	CString	m_edit03;
	CString	m_edit02;
	CString	m_edit04;
	CString	m_edit05;
	CString	m_edit06;
	CString	m_edit07;
	CString	m_edit08;
	CString	m_edit09;
	CString	m_edit10;
	CString	m_edit11;
	CString	m_edit12;
	BOOL	m_check011;
	BOOL	m_check012;
	BOOL	m_check013;
	BOOL	m_check014;
	BOOL	m_check021;
	BOOL	m_check022;
	BOOL	m_check023;
	BOOL	m_check024;
	BOOL	m_check031;
	BOOL	m_check032;
	BOOL	m_check033;
	BOOL	m_check034;
	BOOL	m_check041;
	BOOL	m_check042;
	BOOL	m_check043;
	BOOL	m_check044;
	BOOL	m_check051;
	BOOL	m_check052;
	BOOL	m_check053;
	BOOL	m_check054;
	BOOL	m_check061;
	BOOL	m_check062;
	BOOL	m_check063;
	BOOL	m_check064;
	BOOL	m_check072;
	BOOL	m_check071;
	BOOL	m_check073;
	BOOL	m_check074;
	BOOL	m_check081;
	BOOL	m_check082;
	BOOL	m_check083;
	BOOL	m_check084;
	BOOL	m_check091;
	BOOL	m_check092;
	BOOL	m_check093;
	BOOL	m_check094;
	BOOL	m_check101;
	BOOL	m_check102;
	BOOL	m_check103;
	BOOL	m_check104;
	BOOL	m_check111;
	BOOL	m_check112;
	BOOL	m_check113;
	BOOL	m_check114;
	BOOL	m_check121;
	BOOL	m_check122;
	BOOL	m_check123;
	BOOL	m_check124;
	CString	m_edit_page;
	CString	m_edit_station;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlmClsDef)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlmClsDef)
	afx_msg void OnAlmclsdefGet();
	afx_msg void OnAlmclsdefUppage();
	afx_msg void OnAlmclsdefDownpage();
	afx_msg void OnButton1Off();
	afx_msg void OnButton1On();
	afx_msg void OnButton2Off();
	afx_msg void OnButton2On();
	afx_msg void OnButton3Off();
	afx_msg void OnButton3On();
	afx_msg void OnButton4Off();
	afx_msg void OnButton4On();
	afx_msg void OnAlmclsdefSave();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditPage();
	afx_msg void OnCheck011();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALMCLSDEF_H__A59E43FC_28B0_4317_9642_953987031C1E__INCLUDED_)
