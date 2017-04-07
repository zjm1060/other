#if !defined(AFX_CREATETEXT_H__B2C303B3_C1C5_4F31_8C36_B803E52B53AF__INCLUDED_)
#define AFX_CREATETEXT_H__B2C303B3_C1C5_4F31_8C36_B803E52B53AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateText dialog

class CCreateText : public CDialog
{
// Construction

public:
	CCreateText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateText)
	enum { IDD = IDD_CREATE_TXT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateText)
	afx_msg void OnCreateTextOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATETEXT_H__B2C303B3_C1C5_4F31_8C36_B803E52B53AF__INCLUDED_)
