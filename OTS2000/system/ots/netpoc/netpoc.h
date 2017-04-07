// netpoc.h : main header file for the NETPOC application
//

#if !defined(AFX_NETPOC_H__AC64E7A2_B7F5_4A34_8E84_D0727DEB153F__INCLUDED_)
  #define AFX_NETPOC_H__AC64E7A2_B7F5_4A34_8E84_D0727DEB153F__INCLUDED_

  #if _MSC_VER > 1000
    #pragma once
  #endif // _MSC_VER > 1000

  #ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
  #endif

  #include "resource.h"		// main symbols

  #include <winsock2.h>
  #include <afxsock.h>
  #include <afxtempl.h>


#include	"/home/ems/system/dms/v4.0a/inc/dms_com.h"
#include	"/home/ems/system/dps/v4.0a/inc/dps_type_def.h"
#include	"/home/ems/system/lan/v4.0a/inc/lan_common.h"
#include    "/home/ems/system/cmd/v4.0a/inc/cmd_def.h"
#include    "/home/ems/system/dms/v4.0a/inc/dms_std.h"
#include    "/home/ems/system/dms/v4.0a/inc/dms_structure_def.h"
#include    "/home/ems/system/dms/v4.0a/inc/os_type_def.h"
  /////////////////////////////////////////////////////////////////////////////
  // CNetpocApp:
  // See netpoc.cpp for the implementation of this class
  //

  class CNetpocApp: public CWinApp
  {
    public:
      CNetpocApp();

      // Overrides
      // ClassWizard generated virtual function overrides
      //{{AFX_VIRTUAL(CNetpocApp)
    public:
      virtual BOOL InitInstance();
      //}}AFX_VIRTUAL

      // Implementation

      //{{AFX_MSG(CNetpocApp)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
      //}}AFX_MSG
      DECLARE_MESSAGE_MAP()
  };


  /////////////////////////////////////////////////////////////////////////////

  //{{AFX_INSERT_LOCATION}}
  // Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
// !defined(AFX_NETPOC_H__AC64E7A2_B7F5_4A34_8E84_D0727DEB153F__INCLUDED_)
