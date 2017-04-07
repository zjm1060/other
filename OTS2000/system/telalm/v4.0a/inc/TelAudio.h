#if !defined(AFX_TELAUDIO_H__9176954A_4463_40DA_A402_05E66D53238A__INCLUDED_)
#define AFX_TELAUDIO_H__9176954A_4463_40DA_A402_05E66D53238A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TelAudio.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CTelAudio dialog

class CTelAudio : public CPropertyPage
{
	DECLARE_DYNCREATE(CTelAudio)

// Construction
public:
	CTelAudio();
	~CTelAudio();

// Dialog Data
	//{{AFX_DATA(CTelAudio)
	enum { IDD = IDD_TELAUDIO };
	CButton	m_telaudio_save;
	CButton	m_telaudio_get;
	CString	m_telaudio_edit11;
	CString	m_telaudio_edit12;
	CString	m_telaudio_edit21;
	CString	m_telaudio_edit22;
	CString	m_telaudio_edit31;
	CString	m_telaudio_edit32;
	CString	m_telaudio_edit41;
	CString	m_telaudio_edit42;
	CString	m_telaudio_edit13;
	CString	m_telaudio_edit14;
	CString	m_telaudio_edit23;
	CString	m_telaudio_edit24;
	CString	m_telaudio_edit33;
	CString	m_telaudio_edit34;
	CString	m_telaudio_edit43;
	CString	m_telaudio_edit44;
	CString	m_telaudio_edit110;
	CString	m_telaudio_edit111;
	CString	m_telaudio_edit112;
	CString	m_telaudio_edit113;
	CString	m_telaudio_edit114;
	CString	m_telaudio_edit115;
	CString	m_telaudio_edit116;
	CString	m_telaudio_edit15;
	CString	m_telaudio_edit16;
	CString	m_telaudio_edit17;
	CString	m_telaudio_edit18;
	CString	m_telaudio_edit19;
	CString	m_telaudio_edit210;
	CString	m_telaudio_edit211;
	CString	m_telaudio_edit212;
	CString	m_telaudio_edit213;
	CString	m_telaudio_edit214;
	CString	m_telaudio_edit215;
	CString	m_telaudio_edit216;
	CString	m_telaudio_edit25;
	CString	m_telaudio_edit26;
	CString	m_telaudio_edit27;
	CString	m_telaudio_edit28;
	CString	m_telaudio_edit29;
	CString	m_telaudio_edit310;
	CString	m_telaudio_edit311;
	CString	m_telaudio_edit312;
	CString	m_telaudio_edit313;
	CString	m_telaudio_edit314;
	CString	m_telaudio_edit315;
	CString	m_telaudio_edit316;
	CString	m_telaudio_edit35;
	CString	m_telaudio_edit36;
	CString	m_telaudio_edit37;
	CString	m_telaudio_edit38;
	CString	m_telaudio_edit39;
	CString	m_telaudio_edit410;
	CString	m_telaudio_edit411;
	CString	m_telaudio_edit412;
	CString	m_telaudio_edit413;
	CString	m_telaudio_edit414;
	CString	m_telaudio_edit415;
	CString	m_telaudio_edit416;
	CString	m_telaudio_edit45;
	CString	m_telaudio_edit46;
	CString	m_telaudio_edit47;
	CString	m_telaudio_edit48;
	CString	m_telaudio_edit49;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTelAudio)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTelAudio)
	afx_msg void OnTelaudioGet();
	afx_msg void OnTelaudioSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void GetDataForDsp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELAUDIO_H__9176954A_4463_40DA_A402_05E66D53238A__INCLUDED_)
