#include <afxwin.h>
#include <afxsock.h>		// MFC socket extensions
#include <afxdb.h>			// MFC ODBC database classes
#include <afxmt.h>
#include <iostream>

#define SUNNY			1
#define CLOUDY			2
#define RAINNY			3
#define	SNOW			4
#define	FOG				5
#define NIGHT			6

#define	CONNECT_WITH_STUDENT	1
#define	CONNECT_WITH_TEACHER	2

#define INDEX           register int
#define WINDEX          register WORD
#define LINDEX          register long
#define DWINDEX         register DWORD
typedef unsigned char	byte;	

#define POWER_NODE_NUM	1500
#define FINIT		0.000001

#define		STUDENT_NUM	10

#include "SimComm_Teacher.h"