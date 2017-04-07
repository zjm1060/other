/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*                  2000-2002, All Rights Reserved                      */
/*									*/
/* MODULE NAME : MiSem.h						*/
/* PRODUCT(S)  : Multi-thread support mi.lib				*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR    02     Copyright year update                        */
/* 07/03/00  NAV	   Created with _WIN32 support only		*/
/************************************************************************/

#ifndef MISEM_INCLUDED
#define MISEM_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "slog.h"
#include "stime.h"


/************************************************************************/
/* Logging masks and variable						*/

#define MISEM_LOG_FLOW 		0x0001 
extern ST_UINT misem_debug_sel;

extern SD_CONST ST_CHAR *SD_CONST _misem_always_logstr;
extern SD_CONST ST_CHAR *SD_CONST _misem_flow_logstr;

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* WINDOWS 95/NT							*/
/* These typedefs, defines, and function prototypes are for use with	*/
/* Windows 95/NT only							*/
/************************************************************************/

#if defined(_WIN32)				/* VC++, 32-Bit		*/

#include "windows.h"

#if !defined (ST_THREAD_ID)
#define ST_THREAD_ID 		DWORD
#endif

#endif

/************************************************************************/
/************************************************************************/
/* If no system has defined the required types, do so now. This allows	*/
/* the source to include variables of these types w/o errors.		*/

#if !defined (ST_THREAD_ID)

#define ST_THREAD_ID 		ST_VOID *

#endif

/************************************************************************/
/************************************************************************/
/* Global functions							*/

ST_RET misem_init (ST_VOID);
ST_VOID misem_log_sem_state (ST_VOID);
ST_VOID misem_get_sem (ST_VOID);
ST_VOID misem_free_sem (ST_VOID);
ST_VOID misem_get_semx (SD_CONST ST_CHAR *srcFile, ST_INT srcLineNum);
ST_VOID misem_free_semx (SD_CONST ST_CHAR *srcFile, ST_INT srcLineNum);


/************************************************************************/
/* These are the macro's used to call the misem get/free functions.	*/
/* Note that the behavior depends on the defines used at compile time.	*/

#if defined(S_MI_MT_SUPPORT)	/* If code is to have MT support	*/

#if defined(DEBUG_SISCO) 	/* MT support, debug */

#define S_MI_LOCK_COMMON()\
 {misem_get_semx(thisFileName, __LINE__);} 

#define S_MI_UNLOCK_COMMON()\
 {misem_free_semx(thisFileName, __LINE__);} 

#else	/* MT support, no debug */

#define S_MI_LOCK_COMMON()\
 {misem_get_sem();} 

#define S_MI_UNLOCK_COMMON()\
 {misem_free_sem();} 

#endif
#else				/* No MT support is compiled in		*/
#define S_MI_LOCK_COMMON()	
#define S_MI_UNLOCK_COMMON()
#endif

/************************************************************************/

#define MAX_MI_SEM_NEST_TRACK  20 
extern ST_INT   misem_currSemOwnerIndex;
extern ST_CHAR  misem_currSemOwnerFile[MAX_MI_SEM_NEST_TRACK][SLOG_MAX_FNAME+1];
extern ST_INT   misem_currSemOwnerLine[MAX_MI_SEM_NEST_TRACK];

extern ST_DOUBLE misem_hwMutexTime;
extern ST_DOUBLE misem_hwEventTime;

/************************************************************************/
/* Logging macro's used internally by misem.c				*/

#if defined(DEBUG_SISCO) 	

#define MISEM_LOG_ALWAYS0(a) {\
                       _slog (sLogCtrl, _misem_always_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define MISEM_LOG_ALWAYS1(a,b) {\
                       _slog (sLogCtrl, _misem_always_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define MISEM_LOG_ALWAYS2(a,b,c) {\
                       _slog (sLogCtrl, _misem_always_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MISEM_LOG_ALWAYS3(a,b,c,d) {\
                       _slog (sLogCtrl, _misem_always_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define MISEM_LOG_CALWAYS0(a) {\
                       _slogc (sLogCtrl,a);\
                    }
#define MISEM_LOG_CALWAYS1(a,b) {\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MISEM_LOG_CALWAYS2(a,b,c) {\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MISEM_LOG_CALWAYS3(a,b,c,d) {\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define MISEM_LOG_FLOW2(a,b,c) {\
                     if (misem_debug_sel & MISEM_LOG_FLOW)\
                       _slog (sLogCtrl, _misem_flow_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define MISEM_LOG_CFLOW0(a) {\
                     if (misem_debug_sel & MISEM_LOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define MISEM_LOG_CFLOW1(a,b) {\
                     if (misem_debug_sel & MISEM_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define MISEM_LOG_CFLOW2(a,b,c) {\
                     if (misem_debug_sel & MISEM_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define MISEM_LOG_CFLOW3(a,b,c,d) {\
                     if (misem_debug_sel & MISEM_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

#else	/* No DEBUG_SISCO */

#define MISEM_LOG_ALWAYS0(a)
#define MISEM_LOG_ALWAYS1(a,b)
#define MISEM_LOG_ALWAYS2(a,b,c)
#define MISEM_LOG_CALWAYS0(a)
#define MISEM_LOG_CALWAYS3(a,b,c,d)
#define MISEM_LOG_FLOW2(a,b,c)
#define MISEM_LOG_CFLOW1(a,b)
#define MISEM_LOG_CFLOW2(a,b,c)
#define MISEM_LOG_CFLOW0(a)

#endif

#ifdef __cplusplus
}
#endif

#endif /* #ifndef GBLSEM_INCLUDED */

