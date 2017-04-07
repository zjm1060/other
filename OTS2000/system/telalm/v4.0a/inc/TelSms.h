#if !defined(AFX_TELSMS_H__480FB5CB_CDFA_4B8E_B078_9E395581C78A__INCLUDED_)
#define AFX_TELSMS_H__480FB5CB_CDFA_4B8E_B078_9E395581C78A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TelSms.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTelSms dialog

class CTelSms : public CPropertyPage
{
	DECLARE_DYNCREATE(CTelSms)

// Construction
public:
	CTelSms();
	~CTelSms();

// Dialog Data
	//{{AFX_DATA(CTelSms)
	enum { IDD = IDD_TELSMS };
	CButton	m_telsms_save;
	CButton	m_telsms_get;
	CString	m_telsms_edit11;
	CString	m_telsms_edit12;
	CString	m_telsms_edit13;
	CString	m_telsms_edit14;
	CString	m_telsms_edit21;
	CString	m_telsms_edit22;
	CString	m_telsms_edit23;
	CString	m_telsms_edit24;
	CString	m_telsms_edit31;
	CString	m_telsms_edit32;
	CString	m_telsms_edit33;
	CString	m_telsms_edit34;
	CString	m_telsms_edit41;
	CString	m_telsms_edit42;
	CString	m_telsms_edit43;
	CString	m_telsms_edit44;
	CString	m_telsms_edit110;
	CString	m_telsms_edit111;
	CString	m_telsms_edit112;
	CString	m_telsms_edit113;
	CString	m_telsms_edit114;
	CString	m_telsms_edit115;
	CString	m_telsms_edit116;
	CString	m_telsms_edit15;
	CString	m_telsms_edit16;
	CString	m_telsms_edit17;
	CString	m_telsms_edit18;
	CString	m_telsms_edit19;
	CString	m_telsms_edit210;
	CString	m_telsms_edit211;
	CString	m_telsms_edit212;
	CString	m_telsms_edit213;
	CString	m_telsms_edit214;
	CString	m_telsms_edit216;
	CString	m_telsms_edit215;
	CString	m_telsms_edit25;
	CString	m_telsms_edit26;
	CString	m_telsms_edit27;
	CString	m_telsms_edit28;
	CString	m_telsms_edit29;
	CString	m_telsms_edit310;
	CString	m_telsms_edit311;
	CString	m_telsms_edit312;
	CString	m_telsms_edit313;
	CString	m_telsms_edit314;
	CString	m_telsms_edit315;
	CString	m_telsms_edit316;
	CString	m_telsms_edit35;
	CString	m_telsms_edit36;
	CString	m_telsms_edit37;
	CString	m_telsms_edit38;
	CString	m_telsms_edit39;
	CString	m_telsms_edit410;
	CString	m_telsms_edit411;
	CString	m_telsms_edit412;
	CString	m_telsms_edit413;
	CString	m_telsms_edit414;
	CString	m_telsms_edit415;
	CString	m_telsms_edit416;
	CString	m_telsms_edit45;
	CString	m_telsms_edit46;
	CString	m_telsms_edit47;
	CString	m_telsms_edit48;
	CString	m_telsms_edit49;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTelSms)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTelSms)
	afx_msg void OnTelsmsGet();
	afx_msg void OnTelsmsSave();
	afx_msg void OnSmssendInput();
	afx_msg void OnSmsnumInput();
	afx_msg void OnSmsInput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void GetDataForDsp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELSMS_H__480FB5CB_CDFA_4B8E_B078_9E395581C78A__INCLUDED_)
