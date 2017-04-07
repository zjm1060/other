#if !defined(AFX_CheckUser_H__571E0C94_7765_11D4_B8F0_00A00C141200__INCLUDED_)
#define AFX_CheckUser_H__571E0C94_7765_11D4_B8F0_00A00C141200__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "NTService.h"

class CSettingServ : public CNTService {
	public:	// construction
		CSettingServ();

	public:	// overridables
		virtual void	Run(DWORD, LPTSTR *);
		virtual void	Stop();
};

#endif // !defined(AFX_CheckUser_H__571E0C94_7765_11D4_B8F0_00A00C141200__INCLUDED_)
