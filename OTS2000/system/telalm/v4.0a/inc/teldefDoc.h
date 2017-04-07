// teldefDoc.h : interface of the CTeldefDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TELDEFDOC_H__20138F92_D540_4A94_BDD0_FD7E17C8004D__INCLUDED_)
#define AFX_TELDEFDOC_H__20138F92_D540_4A94_BDD0_FD7E17C8004D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTeldefDoc : public CDocument
{
protected: // create from serialization only
	CTeldefDoc();
	DECLARE_DYNCREATE(CTeldefDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeldefDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTeldefDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTeldefDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELDEFDOC_H__20138F92_D540_4A94_BDD0_FD7E17C8004D__INCLUDED_)
