/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1986 - 2002, All Rights Reserved.		        */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : sysincs.h						*/
/* PRODUCT(S)  : MMSEASE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	The purpose of this include file is to bring in include files	*/
/*	that come with one of the various C compilers.			*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 02/27/02  EJV     17    Added limits.h to most of systems.		*/
/*			   Check if LONG_MAX and INT_MAX defined.	*/
/* 07/27/01  DSF     16    crtdbg.h should only be included if _DEBUG	*/
/*			   is defined					*/
/* 03/16/01  DSF     15    Make sure VERIFY is not defined before	*/
/*			   defining it					*/
/* 02/02/01  DSF     14    Added asserts				*/
/* 01/19/01  EJV     13    Added <ctype.h>  for      SYSTEM_SEL_SYS_5.	*/
/* 01/03/01  EJV     12    Added <fcntl.h>  for      SYSTEM_SEL_SYS_5.	*/
/* 10/13/00  EJV     11    Added <unistd.h> for some SYSTEM_SEL_SYS_5.	*/
/* 05/05/00  JRB     10    Added "select.h" for some systems.		*/
/* 04/20/00  EJV     09    Added stdarg.h for SYSTEM_SEL_SYS_5.		*/
/* 02/04/99  JRB     08    Added types.h & stat.h for most systems.	*/
/* 07/01/98  JRB     07    Added stdarg.h & ctype.h for some systems.	*/
/* 12/29/97  JRB     06    Cleaned up OS2 (include os2.h). More PSOS.	*/
/* 12/11/97  JRB     05    Cleaned up PSOS includes. Use SYS_PSOS.	*/
/* 12/11/97  JRB     04    Deleted EMBEDDED_6800 includes.		*/
/* 10/29/97  EJV     03    Corrected max maro definition		*/
/* 10/28/97  EJV     02    Added *.h files specific for SYS_5		*/
/* 08/15/97  EJV     01    Added *.h files specific for SYS_QNX4	*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifndef SYSINCS_INCLUDED
#define SYSINCS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#if (SYSTEM_SEL & (SYSTEM_SEL_OS2))
#define INCL_BASE	/* include all OS2 definitions.			*/
#include <os2.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#ifdef kbhit		/* The OS/2 2.0 version is not what we want	*/
#undef kbhit
#endif
#endif	/* OS2	*/

#if (SYSTEM_SEL & (SYSTEM_SEL_MSOFT | SYSTEM_SEL_QNX_C86 | SYSTEM_SEL_NEW_SYSTEM))
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#endif

#if (SYSTEM_SEL & (SYSTEM_SEL_SYSVXWORKS))
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <selectLib.h>
#include <limits.h>
#endif

#if	(SYSTEM_SEL & (SYSTEM_SEL_MSOFT))
#ifndef CDECL
#define CDECL _cdecl
#endif
#else
#define CDECL 
#endif

#if	(SYSTEM_SEL & (SYSTEM_SEL_QNX_C86))
#define	max(a,b)	((a > b) ? a : b)
#define	min(a,b)	((a < b) ? a : b)
#endif

#if	(SYSTEM_SEL & (SYSTEM_SEL_SYS_QNX4))
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>		/* S_IFIFO, S_IRUSR, S_IWUSR, etc.	*/
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>		/* mknod, etc.				*/
#include <fcntl.h>		/* open, O_RDONLY, O_WRONLY, etc.	*/
#include <process.h>		/* execlp, etc.				*/
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>
#endif

#if	(SYSTEM_SEL & (SYSTEM_SEL_SYS_5 | SYSTEM_SEL_SYS_BSD))
#include <unistd.h>	/* SHOULD be before any other include files 	*/
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define	max(a,b)	((a > b) ? a : b)
#define	min(a,b)	((a < b) ? a : b)
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>		/* open, O_RDONLY, O_WRONLY, etc.	*/
#include <ctype.h>
#include <limits.h>
#endif

#if	(SYSTEM_SEL & (SYSTEM_SEL_OPEN_VMS))
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <stat.h>
#define	max(a,b)	((a > b) ? a : b)
#define	min(a,b)	((a < b) ? a : b)
#endif

#if (SYSTEM_SEL & SYSTEM_SEL_SYS_PSOS)
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include <psos.h>
#include <probe.h>
#include <prepc.h>
#define	max(a,b)	((a > b) ? a : b)
#define	min(a,b)	((a < b) ? a : b)
#endif

#if (!defined(INT_MAX)) || (!defined(LONG_MAX))
#error INT_MAX and LONG_MAX must be defined. Usually defined in limits.h
#endif


/************************************************************************/
/************************************************************************/
/*		Assert stuff						*/
/************************************************************************/
/************************************************************************/

/* We'll have to figure out which compilers do not provide assert.h */
#if defined (DEBUG_SISCO)
#undef NDEBUG
#else
#if !defined (NDEBUG)
#define NDEBUG
#endif
#endif

#include <assert.h>

#if defined (_WIN32) && defined (_DEBUG)
#include <crtdbg.h>
#endif

#if !defined (_ASSERT)
#define _ASSERT	assert
#endif

#if !defined (_ASSERTE)
#define _ASSERTE(e)	assert(e)
#endif

#if !defined (ASSERT)
#define ASSERT	_ASSERT
#endif

#if !defined (ASSERTE)
#define ASSERTE	_ASSERTE
#endif

#if !defined (VERIFY)
#if defined (DEBUG_SISCO)
#define VERIFY(e)	_ASSERTE(e)
#else
#define VERIFY(e)	((void) (e))
#endif
#endif

#if !defined (C_ASSERT)
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#endif

#ifdef __cplusplus
}
#endif

#endif


