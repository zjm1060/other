#if !defined(AFX_SMSDEF_H__3A82DF34_62B6_4E78_84C9_BBAFCB9F3EF1__INCLUDED_)
#define AFX_SMSDEF_H__3A82DF34_62B6_4E78_84C9_BBAFCB9F3EF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SmsDef.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSmsDef dialog

class CSmsDef : public CDialog
//class CSmsDef : public CPropertyPage
{
// Construction
//	DECLARE_DYNCREATE(CSmsDef)
public:
	void GetDataForDsp(), GetDataForSave();

public:
	CSmsDef(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSmsDef)
	enum { IDD = IDD_SMSDEF };
	CScrollBar	m_smsdef_scrollbar;
	CButton	m_smsdef_save;
	CButton	m_smsdef_get;
	CString	m_smsdef_edit11;
	CString	m_smsdef_edit12;
	CString	m_smsdef_edit13;
	CString	m_smsdef_edit14;
	CString	m_smsdef_edit15;
	CString	m_smsdef_edit16;
	CString	m_smsdef_edit17;
	CString	m_smsdef_edit18;
	CString	m_smsdef_edit19;
	CString	m_smsdef_edit110;
	CString	m_smsdef_edit21;
	CString	m_smsdef_edit210;
	CString	m_smsdef_edit22;
	CString	m_smsdef_edit23;
	CString	m_smsdef_edit24;
	CString	m_smsdef_edit25;
	CString	m_smsdef_edit26;
	CString	m_smsdef_edit27;
	CString	m_smsdef_edit28;
	CString	m_smsdef_edit29;
	CString	m_smsdef_edit31;
	CString	m_smsdef_edit310;
	CString	m_smsdef_edit32;
	CString	m_smsdef_edit33;
	CString	m_smsdef_edit34;
	CString	m_smsdef_edit35;
	CString	m_smsdef_edit36;
	CString	m_smsdef_edit37;
	CString	m_smsdef_edit38;
	CString	m_smsdef_edit39;
	CString	m_smsdef_edit41;
	CString	m_smsdef_edit410;
	CString	m_smsdef_edit42;
	CString	m_smsdef_edit43;
	CString	m_smsdef_edit44;
	CString	m_smsdef_edit45;
	CString	m_smsdef_edit46;
	CString	m_smsdef_edit47;
	CString	m_smsdef_edit48;
	CString	m_smsdef_edit49;
	CString	m_edit_page;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmsDef)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSmsDef)
	afx_msg void OnSmsdefSave();
	afx_msg void OnSmsdefGet();
	afx_msg void OnSmsdefDownpage();
	afx_msg void OnSmsdefUppage();
	afx_msg void OnChangeEditPage();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnSmsInput();
	afx_msg void OnChangeSmsdefEdit21();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMSDEF_H__3A82DF34_62B6_4E78_84C9_BBAFCB9F3EF1__INCLUDED_)
