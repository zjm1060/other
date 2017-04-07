#if !defined(AFX_TELDEFINE_H__AB45D3D3_3831_417E_9A78_A215204AC652__INCLUDED_)
#define AFX_TELDEFINE_H__AB45D3D3_3831_417E_9A78_A215204AC652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TelDefine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTelDefine dialog

class CTelDefine : public CDialog
{
// Construction

public:
	void GetDataForDsp(), GetDataForSave();

public:
	CTelDefine(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTelDefine)
	enum { IDD = IDD_TELDEF };
	CButton	m_teldef_save;
	CButton	m_teldef_get;
	CScrollBar	m_teldef_scrollbar;
	CString	m_edit_page;
	CString	m_teldef_edit11;
	CString	m_teldef_edit110;
	CString	m_teldef_edit12;
	CString	m_teldef_edit13;
	CString	m_teldef_edit14;
	CString	m_teldef_edit15;
	CString	m_teldef_edit16;
	CString	m_teldef_edit17;
	CString	m_teldef_edit18;
	CString	m_teldef_edit19;
	CString	m_teldef_edit21;
	CString	m_teldef_edit210;
	CString	m_teldef_edit22;
	CString	m_teldef_edit23;
	CString	m_teldef_edit24;
	CString	m_teldef_edit25;
	CString	m_teldef_edit26;
	CString	m_teldef_edit27;
	CString	m_teldef_edit28;
	CString	m_teldef_edit29;
	CString	m_teldef_edit31;
	CString	m_teldef_edit310;
	CString	m_teldef_edit32;
	CString	m_teldef_edit33;
	CString	m_teldef_edit34;
	CString	m_teldef_edit35;
	CString	m_teldef_edit36;
	CString	m_teldef_edit37;
	CString	m_teldef_edit38;
	CString	m_teldef_edit39;
	CString	m_teldef_edit41;
	CString	m_teldef_edit410;
	CString	m_teldef_edit42;
	CString	m_teldef_edit43;
	CString	m_teldef_edit44;
	CString	m_teldef_edit45;
	CString	m_teldef_edit46;
	CString	m_teldef_edit47;
	CString	m_teldef_edit48;
	CString	m_teldef_edit49;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelDefine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTelDefine)
	afx_msg void OnTeldefGet();
	afx_msg void OnTeldefSave();
	afx_msg void OnTeldefDownpage();
	afx_msg void OnTeldefUppage();
	afx_msg void OnChangeEditPage();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELDEFINE_H__AB45D3D3_3831_417E_9A78_A215204AC652__INCLUDED_)
