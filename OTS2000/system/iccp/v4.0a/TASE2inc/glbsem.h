/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*                  1997-2002, All Rights Reserved                   	*/
/*									*/
/* MODULE NAME : glbsem.h						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : Multi-thread support defines and prototypes.	*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/29/02  EJV     29    Added gs_timer_get_resolution(),		*/
/*			   GS_TIMER: made more fields common to UNIX,Win*/
/*			     Added field delay_start for WIN32.		*/
/* 04/09/02  EJV     28    __alpha: moved pthread.h up (cleaner code)	*/
/* 02/06/02  EJV     27    _AIX: check for _THREAD_SAFE compiler define	*/
/* 10/31/01  EJV     26    _WIN32: added code to call _beginthreadex;	*/
/*			   Added gs_close_thread, gs_pulse_event_sem;	*/
/*			   Added _ERR, _NERR log macros			*/
/*			   UNIX: chg predicate to ST_INT from ST_BOOLEAN*/
/*                         UNIX: added manualReset to GS_EVENT_SEM.	*/
/* 09/20/01  EJV     25    _WIN32: added pending field to GS_TIMER.	*/
/* 08/01/01  JRB     24    Add comment explaining S_LOCK_RES..		*/
/* 07/30/01  EJV     23    Added gs_timer_set_resolution().Chg GS_TIMER.*/
/*			   Added ST_THREAD_RET, ST_THREAD_RET_VAL macros*/
/*			   and changed gs_start_thread arg.		*/
/* 07/25/01  DSF     22    Use <> when including windows.h		*/
/* 07/24/01  EJV     21    _WIN32: added gs_timer support.		*/
/* 04/04/01  DSF     20    Added gs_set_thread_name for WIN32		*/
/* 03/15/01  EJV     19    UNIX: corrected spelling of REENTRANT.	*/
/* 03/07/01  EJV     18    Added slog macros for !DEBUG_SISCO		*/
/* 02/19/01  EJV     17    _AIX: eliminated need to call gs_timer_init.	*/	
/* 02/16/01  EJV     16	   AIX: added timer functions gs_times_xxx.	*/
/* 01/16/01  EJV     15    Moved GET_THREAD_ID define here from glbsem.c*/
/* 12/20/00  EJV     14    Ported to AIX. Revised DEC UNIX.		*/
/* 12/13/00  EJV     13    Added gs_mutex_... funs for multiple mutex	*/
/*			   semaphores support. Added ST_MUTEX_SEM.	*/
/* 12/13/00  EJV     12    Removed USE_MANUAL_RESET_SEM.		*/
/* 12/06/00  EJV     11    Changed gs_get_event_sem to accept arg.	*/
/* 10/25/00  JRB     10    For _WIN32, define S_MT_SUP.. if _MT defined.*/
/* 10/06/00  EJV     09    Ported to DEC UNIX (__alpha) pthread funcs.	*/
/*			   Added few GLBSEM_LOG_FLOWn macros.		*/
/*			   Deleted gs_chk_thread_id(), check _REENTRANT	*/
/* 01/21/00  JRB     08    Del *sem_fun & *sem_funx function pointers.	*/
/*			   Chg macros to call functions directly.	*/
/*			   S_GS_INSTALL, gs_install, S_GS_INIT obsolete.*/
/*			   Chg macros if !S_MT_SUPPORT to call gs_chk...*/
/* 09/13/99  MDE     07    Added SD_CONST modifiers			*/
/* 08/02/99  JRB     06    Chged "gs_get_event_sem" to "auto-reset" &	*/
/*			   #ifdef'd out "gs_reset_event_sem".		*/
/* 10/08/98  MDE     05    Migrated to updated SLOG interface		*/
/* 05/05/98  DSF     04    Added SD_TIMEOUT				*/
/* 12/11/97  KCR     03    Added gs_wait_mult_event_sem for _WIN32	*/
/* 10/06/97  DSF     02    Modified the macros used by SLOG such that	*/
/*			   they could be used by SLOG in DEBUG_SISCO 	*/
/*			   mode						*/
/* 08/15/97  MDE     01    Changed default ST_xxx to ST_VOID *		*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifndef GBLSEM_INCLUDED
#define GBLSEM_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "slog.h"
#include "stime.h"

/************************************************************************/
#define SD_TIMEOUT		2

/************************************************************************/
/* Logging masks and variable						*/

#define GS_LOG_FLOW 		0x0001 
#define GS_LOG_ERR 		0x0002
#define GS_LOG_NERR 		0x0004 
extern ST_UINT gs_debug_sel;

extern SD_CONST ST_CHAR *SD_CONST _glbem_always_logstr;
extern SD_CONST ST_CHAR *SD_CONST _glbem_flow_logstr;

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* WINDOWS 95/98/NT/2000						*/
/* These typedefs, defines, and function prototypes are for use with	*/
/* Windows only.							*/
/************************************************************************/

#if defined(_WIN32)				/* VC++, 32-Bit		*/

#include <windows.h>

/* The VC++ compiler automatically defines (_MT) if Multithread libs	*/
/* are selected. The user clearly wants multithread support, so		*/
/* S_MT_SUPPORT "MUST" be enabled.					*/
/* DEBUG: something similar should be done for any system that supports	*/
/*        multithreading (e.g. OS2).					*/

#if defined(_MT) && !defined(S_MT_SUPPORT)
#define S_MT_SUPPORT
#endif

#define GET_THREAD_ID()		GetCurrentThreadId()
#define ST_MUTEX_SEM 		CRITICAL_SECTION
#define ST_EVENT_SEM 		HANDLE
#define ST_THREAD_ID 		unsigned
#define ST_THREAD_HANDLE 	unsigned long
#define ST_THREAD_ARG 		void *
#define ST_THREAD_CALL_CONV	__stdcall
#define ST_THREAD_RET 		unsigned
#define ST_THREAD_RET_VAL	0

#endif  /* defined(_WIN32) */


/************************************************************************/
/************************************************************************/
/************************************************************************/
/* AIX, Digital UNIX							*/
/* These typedefs, defines, and function prototypes are for use with	*/
/* AIX, Digital UNIX only						*/
/* Implemented functions are complying with the "POSIX 1003.1c" standard.*/
/************************************************************************/


#if defined (_AIX) || (defined(__alpha) && !defined(__VMS))

#if defined(__alpha) && !defined(__VMS)
/* If the code is compiled with -pthread switch then the _REENTRANT	*/
/* symbol is defined. This means that there should be a multithread	*/
/* support for the code (the S_MT_SUPPORT should be defined).		*/

#if defined(_REENTRANT) && !defined(S_MT_SUPPORT)
#define		S_MT_SUPPORT
#endif

/* According to the POSIX standard this define should be included in 	*/
/* every application module. The <standards.h> included in <pthread.h>	*/
/* defines the _POSIX_C_SOURCE to be 199506L.				*/

#define _POSIX_C_SOURCE	199506L
#include <pthread.h>			/* keep the include here	*/
#endif  /* defined(__alpha) && !defined(__VMS) */

#if defined(_AIX)
/* If the compiler defines _THREAD_SAFE we should build the code	*/
/* with multithread support (the S_MT_SUPPORT should be defined).	*/

#if defined(_THREAD_SAFE) && !defined(S_MT_SUPPORT)
#define		S_MT_SUPPORT
#include <pthread.h>			/* keep the include here	*/
#endif
#endif  /* defined(_AIX) */

typedef struct gs_event_sem_tag
  {
  ST_INT		predicate;	/* predicate to change value	*/
  pthread_mutex_t  	mutex;		/* mutex sem protecting the cond*/
  pthread_cond_t	cond;		/* condition variable		*/
  ST_BOOLEAN		manualReset;	/* type os event semaphore	*/
  } GS_EVENT_SEM;

#define GET_THREAD_ID()		pthread_self()
#define ST_MUTEX_SEM 		pthread_mutex_t
#define ST_EVENT_SEM 		GS_EVENT_SEM *
#define ST_THREAD_ID 		pthread_t
#define ST_THREAD_HANDLE 	pthread_t
#define ST_THREAD_ARG 		void *
#define ST_THREAD_CALL_CONV
#define ST_THREAD_RET 		void *
#define ST_THREAD_RET_VAL	NULL

#endif /* defined (_AIX) || (defined(__alpha) && !defined(__VMS)) */


/************************************************************************/
/************************************************************************/
/* If no system has defined the required types, do so now. This allows	*/
/* the source to include variables of these types w/o errors.		*/

#if !defined (ST_EVENT_SEM)

#define GET_THREAD_ID()		(ST_THREAD_ID) 0xFFFFFFFF
#define ST_MUTEX_SEM 		ST_VOID *
#define ST_EVENT_SEM 		ST_VOID *
#define ST_THREAD_ID 		ST_VOID *
#define ST_THREAD_HANDLE 	ST_VOID *
#define ST_THREAD_ARG 		ST_VOID *
#define ST_THREAD_CALL_CONV
#define ST_THREAD_RET 		ST_VOID
#define ST_THREAD_RET_VAL

#endif  /* !defined (ST_EVENT_SEM) */

/************************************************************************/
/************************************************************************/
/* Global functions							*/

/* SISCO Single MUTEX functions */
ST_RET gs_init (ST_VOID);
ST_VOID gs_log_sem_state (ST_VOID);
ST_VOID gs_get_sem (ST_VOID);
ST_VOID gs_free_sem (ST_VOID);
ST_VOID gs_get_semx (SD_CONST ST_CHAR *srcFile, ST_INT srcLineNum);
ST_VOID gs_free_semx (SD_CONST ST_CHAR *srcFile, ST_INT srcLineNum);

/* User's MUTEX Semaphore functions */
ST_RET  gs_mutex_create (ST_MUTEX_SEM *ms);
ST_VOID gs_mutex_get (ST_MUTEX_SEM *ms);
ST_VOID gs_mutex_free (ST_MUTEX_SEM *ms);
ST_RET  gs_mutex_destroy (ST_MUTEX_SEM *ms);

/* Event Semaphores */
ST_EVENT_SEM gs_get_event_sem (ST_BOOLEAN manualReset);
ST_RET gs_wait_event_sem (ST_EVENT_SEM es, ST_LONG timeout);
ST_RET gs_wait_mult_event_sem (ST_INT numEvents, ST_EVENT_SEM *esTable, 
		ST_BOOLEAN *activity, ST_LONG timeout);
ST_VOID gs_signal_event_sem (ST_EVENT_SEM es);
ST_VOID gs_pulse_event_sem (ST_EVENT_SEM es);
ST_VOID gs_reset_event_sem (ST_EVENT_SEM es);
ST_VOID gs_free_event_sem (ST_EVENT_SEM es);

/* Thread */
ST_RET gs_start_thread (ST_THREAD_RET (ST_THREAD_CALL_CONV *threadFunc) (ST_THREAD_ARG),
			ST_THREAD_ARG threadArg, 
			ST_THREAD_HANDLE *threadHandleOut,
			ST_THREAD_ID *threadIdOut);
ST_RET gs_wait_thread (ST_THREAD_HANDLE threadHandle, 
		       ST_THREAD_ID threadId, ST_LONG timeout);
ST_RET gs_close_thread (ST_THREAD_HANDLE threadHandle);

#if defined (_WIN32)
ST_VOID gs_set_thread_name (ST_THREAD_ID threadId, ST_CHAR *pThreadName);
#endif

/************************************************************************/
/* These are the macro's used to call the glbsem get/free functions.	*/
/* Note that the behavior depends on the defines used at compile time.	*/

/* NOTE: S_LOCK_RESOURCES & S_UNLOCK_RESOURCES are called from SLOG, so	*/
/*       these macros disable GLBSEM logging by temporarily clearing	*/
/*       gs_debug_sel. This avoids infinite loop (i.e. lock funct	*/
/*       calling log funct which calls lock funct again).	 	*/

#if defined(S_MT_SUPPORT)	/* If code is to have MT support	*/

#if defined(DEBUG_SISCO) 	/* MT support, debug */

#define S_LOCK_COMMON_RESOURCES()\
 {gs_get_semx(thisFileName, __LINE__);} 

#define S_UNLOCK_COMMON_RESOURCES()\
 {gs_free_semx(thisFileName, __LINE__);} 

#define S_LOCK_RESOURCES() {ST_UINT temp; \
			    temp = gs_debug_sel; \
			    gs_debug_sel = 0;\
			    gs_get_semx (thisFileName, __LINE__);\
			    gs_debug_sel = temp;}
#define S_UNLOCK_RESOURCES() {ST_UINT temp; \
			      temp = gs_debug_sel; \
			      gs_debug_sel = 0;\
			      gs_free_semx (thisFileName, __LINE__); \
			      gs_debug_sel = temp;}

#else	/* MT support, no debug */

#define S_LOCK_COMMON_RESOURCES()\
 {gs_get_sem();} 

#define S_UNLOCK_COMMON_RESOURCES()\
 {gs_free_sem();} 

#define S_LOCK_RESOURCES() gs_get_sem ()
#define S_UNLOCK_RESOURCES() gs_free_sem ()

#endif
#else				/* No MT support is compiled in		*/
#define S_LOCK_COMMON_RESOURCES()
#define S_UNLOCK_COMMON_RESOURCES()
#define S_LOCK_RESOURCES()
#define S_UNLOCK_RESOURCES()
#endif

/************************************************************************/

#define MAX_SEM_NEST_TRACK  20 
extern ST_UINT  gs_track;	/* This variable when set to value >0	*/
				/* during an application startup allows	*/
				/* tracking of MUTEX or EVENT sem.	*/
extern ST_INT   gs_currSemOwnerIndex;
extern ST_CHAR  gs_currSemOwnerFile[MAX_SEM_NEST_TRACK][SLOG_MAX_FNAME+1];
extern ST_INT   gs_currSemOwnerLine[MAX_SEM_NEST_TRACK];

extern ST_DOUBLE gs_hwMutexTime;
extern ST_DOUBLE gs_hwEventTime;

	/*---*---*---*---*---*---*---*---*---*---*---*---*---*--*/
	/*							*/
	/*		Timer functions				*/
	/*							*/
	/*---*---*---*---*---*---*---*---*---*---*---*---*---*--*/

#if defined (_WIN32) || defined (_AIX) || (defined(__alpha) && !defined(__VMS))
	/* Note: On AIX the real-time support is not implemented.	*/
	/*	 Functions such as timer_create, sleep, or nanosleep are*/
	/*	 not available (in AIX version 4.3.2 or 4.3.3).		*/
	/*	 This implementation of timers is using the GLBSEM	*/
	/*	 gs_wait_event_sem function implemented with phtreads	*/
	/*	 to support the timer functionality.			*/

/* The timer support will be initialized automatically when the function*/
/* gs_timer_start is called for the first time.				*/

/* If the timer is of type SD_TIMER_ONESHOT then the timer resources	*/
/* will be automatically deleted after the first time the timer	is	*/
/* serviced. For the SD_TIMER_PERIODIC timer the user needs to call	*/
/* the gs_timer_end function to stop the timer and release resources.	*/

/* Before application exits the function gs_timer_cleanup should be	*/
/* called to cleanup all resources associates with timers.		*/

#define SD_TIMER_ONESHOT	1
#define SD_TIMER_PERIODIC	2

struct gs_timer_tag;		/* forward definition */

typedef struct gs_timer_tag
  {
#if defined (_WIN32)
  UINT		timerID;	/* timer id returned from timeSetEvent	*/
  ST_BOOLEAN	pending;	/* shows if timer processing is already pending */
  ST_DOUBLE     delay_start;	/* time when delete timer delay started	*/
#endif
  struct gs_timer_tag *next;	/* pointer to next element in list	*/
  struct gs_timer_tag *prev;	/* pointer to prev element in list	*/
  ST_ULONG      interval;	/* milliseconds, time between servicing	*/
  ST_ULONG      elapsed;	/* milliseconds, time elapsed in sleep	*/
  ST_UINT       type;		/* timer type: oneshot or periodic	*/
  ST_VOID       (*callback_fun)(ST_VOID *);
  ST_VOID      *arg;    	/* argument for the callback_fun.	*/
  } GS_TIMER;

/* The timer resolution could be set to the lowest value required by an	*/
/* application before calling the gs_timer_start function.		*/
/* On Windows system if the timer resolution is changed while the 	*/
/* application is running, only timers started afterwards will be	*/
/* affected. On UNIX system all timers will be afected.			*/

ST_RET   gs_timer_init (ST_VOID);
ST_RET   gs_timer_set_resolution (ST_ULONG timer_resolution);
ST_ULONG gs_timer_get_resolution (ST_VOID);

GS_TIMER *gs_timer_start (ST_UINT  type,
                          ST_ULONG interval,
                          ST_VOID (*callback_fun)(ST_VOID *),
                          ST_VOID  *arg);
ST_RET  gs_timer_end (GS_TIMER *timer_id);
ST_RET  gs_timer_cleanup (ST_VOID);

#endif /* defined (_WIN32) || defined (_AIX) || (defined(__alpha) && !defined(__VMS)) */


/************************************************************************/
/* Logging macro's used internally by glbsem.c				*/

#if defined(DEBUG_SISCO) 	

#define GLBSEM_LOG_ERR0(a) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slog (sLogCtrl, _glbem_err_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define GLBSEM_LOG_ERR1(a,b) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slog (sLogCtrl, _glbem_err_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define GLBSEM_LOG_ERR2(a,b,c) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slog (sLogCtrl, _glbem_err_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define GLBSEM_LOG_ERR3(a,b,c,d) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slog (sLogCtrl, _glbem_err_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define GLBSEM_LOG_CERR0(a) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define GLBSEM_LOG_CERR1(a,b) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define GLBSEM_LOG_CERR2(a,b,c) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define GLBSEM_LOG_CERR3(a,b,c,d) {\
                     if (gs_debug_sel & GS_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define GLBSEM_LOG_NERR0(a) {\
                     if (gs_debug_sel & GS_LOG_NERR)\
                       _slog (sLogCtrl, _glbem_nerr_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define GLBSEM_LOG_NERR1(a,b) {\
                     if (gs_debug_sel & GS_LOG_NERR)\
                       _slog (sLogCtrl, _glbem_nerr_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define GLBSEM_LOG_NERR2(a,b,c) {\
                     if (gs_debug_sel & GS_LOG_NERR)\
                       _slog (sLogCtrl, _glbem_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define GLBSEM_LOG_NERR3(a,b,c,d) {\
                     if (gs_debug_sel & GS_LOG_NERR)\
                       _slog (sLogCtrl, _glbem_nerr_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define GLBSEM_LOG_ALWAYS0(a) {\
                       _slog (sLogCtrl, _glbem_always_logstr,\
		             thisFileName,__LINE__,a);\
                    }

#define GLBSEM_LOG_ALWAYS1(a,b) {\
                       _slog (sLogCtrl, _glbem_always_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }

#define GLBSEM_LOG_ALWAYS2(a,b,c) {\
                       _slog (sLogCtrl, _glbem_always_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define GLBSEM_LOG_ALWAYS3(a,b,c,d) {\
                       _slog (sLogCtrl, _glbem_always_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define GLBSEM_LOG_CALWAYS0(a) {\
                       _slogc (sLogCtrl,a);\
                    }
#define GLBSEM_LOG_CALWAYS1(a,b) {\
                       _slogc (sLogCtrl,a,b);\
                    }
#define GLBSEM_LOG_CALWAYS2(a,b,c) {\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define GLBSEM_LOG_CALWAYS3(a,b,c,d) {\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
#define GLBSEM_LOG_FLOW0(a) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slog (sLogCtrl, _glbem_flow_logstr,\
		             thisFileName,__LINE__,a);\
                    }
#define GLBSEM_LOG_FLOW1(a,b) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slog (sLogCtrl, _glbem_flow_logstr,\
		             thisFileName,__LINE__,a,b);\
                    }
#define GLBSEM_LOG_FLOW2(a,b,c) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slog (sLogCtrl, _glbem_flow_logstr,\
		             thisFileName,__LINE__,a,b,c);\
                    }
#define GLBSEM_LOG_FLOW3(a,b,c,d) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slog (sLogCtrl, _glbem_flow_logstr,\
		             thisFileName,__LINE__,a,b,c,d);\
                    }
#define GLBSEM_LOG_CFLOW0(a) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define GLBSEM_LOG_CFLOW1(a,b) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define GLBSEM_LOG_CFLOW2(a,b,c) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define GLBSEM_LOG_CFLOW3(a,b,c,d) {\
                     if (gs_debug_sel & GS_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

#else	/* No DEBUG_SISCO */

#define GLBSEM_LOG_ERR0(a)
#define GLBSEM_LOG_ERR1(a,b)
#define GLBSEM_LOG_ERR2(a,b,c)
#define GLBSEM_LOG_ERR3(a,b,c,d)
#define GLBSEM_LOG_CERR0(a)
#define GLBSEM_LOG_CERR1(a,b)
#define GLBSEM_LOG_CERR2(a,b,c)
#define GLBSEM_LOG_CERR3(a,b,c,d)
#define GLBSEM_LOG_NERR0(a)
#define GLBSEM_LOG_NERR1(a,b)
#define GLBSEM_LOG_NERR2(a,b,c)
#define GLBSEM_LOG_NERR3(a,b,c,d)
#define GLBSEM_LOG_ALWAYS0(a)
#define GLBSEM_LOG_ALWAYS1(a,b)
#define GLBSEM_LOG_ALWAYS2(a,b,c)
#define GLBSEM_LOG_ALWAYS3(a,b,c,d)
#define GLBSEM_LOG_CALWAYS0(a)
#define GLBSEM_LOG_CALWAYS1(a,b)
#define GLBSEM_LOG_CALWAYS2(a,b,c)
#define GLBSEM_LOG_CALWAYS3(a,b,c,d)
#define GLBSEM_LOG_FLOW0(a)
#define GLBSEM_LOG_FLOW1(a,b)
#define GLBSEM_LOG_FLOW2(a,b,c)
#define GLBSEM_LOG_FLOW3(a,b,c,d)
#define GLBSEM_LOG_CFLOW0(a)
#define GLBSEM_LOG_CFLOW1(a,b)
#define GLBSEM_LOG_CFLOW2(a,b,c)
#define GLBSEM_LOG_CFLOW3(a,b,c,d)

#endif

/************************************************************************/
#define gs_install()		/* OBSOLETE: for backward compat. only	*/
#define S_GS_INSTALL()		/* OBSOLETE: for backward compat. only.	*/
#define S_GS_INIT()		/* OBSOLETE: for backward compat. only.	*/


/************************************************************************/
/************************************************************************/
/************************************************************************/
/* Digital UNIX								*/
/* We have implemented POSIX 1003.1b functions to use additional	*/
/* realtime user signals available on Digital UNIX (but not on AIX).	*/
/* These functions may be used if user application like to use signals	*/
/* other then the SIGUSR1 or SIGUSR2.					*/
/************************************************************************/

#if defined(__alpha) && !defined(__VMS)

#include <signal.h>

typedef struct gs_signal_tag
  {
  sigset_t   set;
  int        sig_num;
  } GS_SIGNAL;

#define ST_USR_SIG 		GS_SIGNAL
#define ST_PID	 		pid_t

ST_USR_SIG *gs_get_sig   (ST_INT sig_num);
ST_RET      gs_wait_sig  (ST_USR_SIG *us, ST_LONG timeout);
ST_RET      gs_raise_sig (ST_USR_SIG *us, ST_PID pid);
ST_RET      gs_free_sig  (ST_USR_SIG *us);

#endif /* defined(__alpha) && !defined(__VMS) */


#ifdef __cplusplus
}
#endif

#endif /* #ifndef GBLSEM_INCLUDED */

