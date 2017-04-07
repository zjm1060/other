// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1090B494_4EDE_4AFE_8C44_E6A34E42F3B1__INCLUDED_)
#define AFX_STDAFX_H__1090B494_4EDE_4AFE_8C44_E6A34E42F3B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//zm================================================================

#include "/home/ems/system/telalm/v4.0a/inc/tele.h"


void		GetTelephoneNum(),		SaveTelephoneNum(), 
			GetMobileNum(),			SaveMobileNum(),
			GetSmsDef(),            SaveSmsDef(), 
			GetTelDef(),            SaveTelDef(), 
			GetSmsNum(),            SaveSmsNum(),
            GetSmsSend(),            SaveSmsSend(),
			ReadDefineFile(int),	SaveDefineFile(),
            ReadAnaDefineFile(int),	SaveAnaDefineFile();


//zm=============================================================

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1090B494_4EDE_4AFE_8C44_E6A34E42F3B1__INCLUDED_)
