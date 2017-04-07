#if !defined(AFX_SELECTDEV_H__C1B31A10_2E63_4FF6_A758_2016076BBC29__INCLUDED_)
#define AFX_SELECTDEV_H__C1B31A10_2E63_4FF6_A758_2016076BBC29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectDev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SelectDev dialog

class SelectDev : public CDialog
{
// Construction
	public:
	void GetDataForDsp(),	GetDataForSave();

public:
	SelectDev(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SelectDev)
	enum { IDD = IDD_SELECT_DEV };
	CButton	m_seldev_save;
	CButton	m_seldev_get;
	BOOL	m_check011;
	BOOL	m_check0110;
	BOOL	m_check0111;
	BOOL	m_check0112;
	BOOL	m_check0113;
	BOOL	m_check0114;
	BOOL	m_check0115;
	BOOL	m_check0116;
	BOOL	m_check0117;
	BOOL	m_check0118;
	BOOL	m_check012;
	BOOL	m_check013;
	BOOL	m_check014;
	BOOL	m_check015;
	BOOL	m_check016;
	BOOL	m_check017;
	BOOL	m_check019;
	CString	m_edit_11;
	CString	m_edit_110;
	CString	m_edit_111;
	CString	m_edit_112;
	CString	m_edit_113;
	CString	m_edit_114;
	CString	m_edit_115;
	CString	m_edit_116;
	CString	m_edit_117;
	CString	m_edit_118;
	CString	m_edit_119;
	CString	m_edit_12;
	CString	m_edit_13;
	CString	m_edit_14;
	CString	m_edit_15;
	CString	m_edit_16;
	CString	m_edit_17;
	CString	m_edit_18;
	CString	m_edit_19;
	BOOL	m_check018;
	BOOL	m_check0119;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SelectDev)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SelectDev)
	afx_msg void OnSmsdevSave();
	afx_msg void OnSmsdevGet();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDEV_H__C1B31A10_2E63_4FF6_A758_2016076BBC29__INCLUDED_)
