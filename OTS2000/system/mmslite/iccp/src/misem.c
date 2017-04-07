/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*                  2000-2002, All Rights Reserved                      */
/*									*/
/* MODULE NAME : MiSem.c						*/
/* PRODUCT(S)  : Multi-thread support for mi.lib			*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     04    Copyright year update                        */
/* 06/20/02  ASK     03	   Changed to lower case #includes for Unices	*/
/* 03/25/02  MDE     02    Fixed use of uninitialized fname		*/
/* 07/03/00  NAV           created with _WIN32 support only		*/
/************************************************************************/

#if defined (_WIN32)
#include <windows.h>
#endif
#include "glbtypes.h"
#include "sysincs.h"
#include "slog.h"
#include "misem.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/

SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;

/************************************************************************/
/************************************************************************/
/* GLOBAL VARIABLES							*/

ST_UINT misem_debug_sel = 0;

#ifdef DEBUG_SISCO
SD_CONST ST_CHAR *SD_CONST _misem_always_logstr = "MISEM_LOG_ALWAYS";
SD_CONST ST_CHAR *SD_CONST _misem_flow_logstr = "MISEM_LOG_FLOW";
#endif

ST_DOUBLE misem_hwMutexTime;
ST_DOUBLE misem_hwEventTime;

/************************************************************************/

static ST_THREAD_ID myThreadId;

/************************************************************************/
/************************************************************************/
/* WINDOWS 95/NT							*/
/* These defines, and function prototypes are for Windows 95/NT only	*/
/* and are used within the common glbsem code.				*/
/************************************************************************/

#if defined(_WIN32)				/* VC++, 32-Bit		*/


#define GET_THREAD_ID()	GetCurrentThreadId();
static ST_RET  initMutexSem (ST_VOID);

#if defined(DEBUG_SISCO) 	
static ST_VOID requestMutexSem (ST_VOID);
static ST_VOID releaseMutexSem (ST_VOID);
#else
#define requestMutexSem() EnterCriticalSection (&miSemCritSection)
#define releaseMutexSem() LeaveCriticalSection (&miSemCritSection)
#endif

CRITICAL_SECTION miSemCritSection;

static ST_VOID logLastError (DWORD errCode);

#endif

#if !defined (_WIN32)

#define GET_THREAD_ID()		(ST_THREAD_ID) 0xFFFFFFFF

#define initMutexSem() 		SD_FAILURE
#define requestMutexSem() 	
#define releaseMutexSem() 	

#endif

/************************************************************************/
/* STATIC VARIABLES							*/

static ST_BOOLEAN misem_already_inited = SD_FALSE;

/* These variables are only accessed by a thread that owns the sem	*/
static ST_THREAD_ID currSemOwner;
static ST_LONG      semCount;

#if defined(DEBUG_SISCO) 	
/* The debug version keeps a stack of sem owners call locations		*/
/* which can be logged to aid in debugging.				*/
/* These variables are only changed by a thread that owns the sem, but	*/
/* can be read (for logging) by 'gs_log_sem_state', possibly resulting	*/
/* in a 'torn' log if the called is not careful.			*/

#define MAX_SEM_NEST_TRACK  20 
ST_INT   misem_currSemOwnerIndex;
ST_CHAR  misem_currSemOwnerFile[MAX_MI_SEM_NEST_TRACK][SLOG_MAX_FNAME+1];
ST_INT   misem_currSemOwnerLine[MAX_MI_SEM_NEST_TRACK];
#endif	/* DEBUG_SISCO */

/************************************************************************/
/************************************************************************/
/*                       misem_init					*/
/************************************************************************/

ST_RET misem_init (ST_VOID)
  {
ST_RET rc;

  if (misem_already_inited)
    return (SD_SUCCESS);

  /* Initialize the MUTEX semaphore					*/
  rc = initMutexSem ();

  /* Set up to track the semaphore owners by nesting level		*/
#if defined(DEBUG_SISCO) 	
  misem_currSemOwnerIndex = -1;
#endif

  if (rc == SD_SUCCESS)
    misem_already_inited = SD_TRUE;

  return (rc);
  }

/************************************************************************/
/************************************************************************/
/*                       misem_log_sem_state				*/
/************************************************************************/

ST_VOID misem_log_sem_state (ST_VOID)
  {
#if defined(DEBUG_SISCO) 	
ST_INT i;

  MISEM_LOG_ALWAYS0 ("MISEM: Semaphore owner stack (oldest to latest) :"); 
  for (i = 0; i <= misem_currSemOwnerIndex && i < MAX_MI_SEM_NEST_TRACK; ++i)
    {
    MISEM_LOG_CALWAYS3 ("  %d) File %s, Line %d", 
  			i+1, misem_currSemOwnerFile[i], misem_currSemOwnerLine[i]);
    }
  if (misem_currSemOwnerIndex >= MAX_MI_SEM_NEST_TRACK)
    MISEM_LOG_CALWAYS0 ("  Sem's nested too deep to track further");
#endif
  }


/************************************************************************/
/************************************************************************/
/*                       misem_get_semx					*/
/* This debug version function keeps track of the sem owner stack. If	*/
/* compiled without DEBUG_SISCO is just falls through to misem_get_sem	*/
/************************************************************************/


ST_VOID misem_get_semx (SD_CONST ST_CHAR *srcFile, ST_INT srcLineNum)
  {
#if defined(DEBUG_SISCO) 	
ST_CHAR fname[SLOG_MAX_FNAME+1];
ST_DOUBLE startTime;
ST_DOUBLE endTime;
ST_DOUBLE elapsedTime;

  if (misem_debug_sel & MISEM_LOG_FLOW)
    startTime = sGetMsTime ();
  misem_get_sem ();
  if (misem_debug_sel & MISEM_LOG_FLOW)
    endTime = sGetMsTime ();

  if (misem_debug_sel & MISEM_LOG_FLOW)
    {
    if (srcFile != NULL)
      slogTrimFileName (fname, srcFile);
    else
      strcpy (fname, "Unknown");

    elapsedTime = endTime - startTime;
    if (elapsedTime > misem_hwMutexTime)
      misem_hwMutexTime = elapsedTime;

    MISEM_LOG_FLOW2 ("MISEM: File %s, Line %d has the mutex", 
  		     fname, srcLineNum);
    MISEM_LOG_CFLOW3 ("        semCount = %d, took %.3f sec (hw = %.3f)", 
		      semCount, elapsedTime/1000, misem_hwMutexTime/1000); 
    }
  else
    fname[0] = 0;

  ++misem_currSemOwnerIndex;
  if (misem_currSemOwnerIndex < MAX_SEM_NEST_TRACK)
    {
    strcpy (misem_currSemOwnerFile[misem_currSemOwnerIndex], fname);
    misem_currSemOwnerLine[misem_currSemOwnerIndex] = srcLineNum;
    }
  else 
    {
    MISEM_LOG_CFLOW0  ("        Nested too deep to track"); 
    }

#else  /* Not DEBUG_SISCO */

  misem_get_sem ();

#endif /* DEBUG_SISCO */
  }		        
/************************************************************************/
/*                       misem_free_semx				*/
/* This debug version function keeps track of the sem owner stack. If	*/
/* compiled without DEBUG_SISCO is just falls through to misem_free_sem	*/
/************************************************************************/

ST_VOID misem_free_semx (SD_CONST ST_CHAR *srcFile, ST_INT srcLineNum)
  {
#if defined(DEBUG_SISCO) 	
ST_CHAR fname[SLOG_MAX_FNAME+1];

  if (misem_debug_sel & MISEM_LOG_FLOW)
    {
    if (srcFile != NULL)
      slogTrimFileName (fname, srcFile);
    else
      strcpy (fname, "Unknown");

    MISEM_LOG_FLOW2 ("MISEM: File %s, Line %d freeing the semaphore", 
  		      fname, srcLineNum);
    }

  --misem_currSemOwnerIndex;
  if (misem_currSemOwnerIndex == -1)
    {
    MISEM_LOG_CFLOW0 ("        The semaphore should now be free"); 
    }
  else if (misem_currSemOwnerIndex >= 0 && misem_currSemOwnerIndex < MAX_MI_SEM_NEST_TRACK)
    {
    MISEM_LOG_CFLOW2 ("        File %s, Line %d now has  the semaphore", 
		      misem_currSemOwnerFile[misem_currSemOwnerIndex],
		      misem_currSemOwnerLine[misem_currSemOwnerIndex]);
    }
  else if (misem_currSemOwnerIndex >= 0 && misem_currSemOwnerIndex >= MAX_MI_SEM_NEST_TRACK)
    {
    MISEM_LOG_CFLOW0  ("        Nested too deep to track"); 
    }
  else if (misem_currSemOwnerIndex < -1)
    {
    MISEM_LOG_ALWAYS0 ("MISEM: Sem track index negative");
    misem_currSemOwnerIndex = -1;
    }

  MISEM_LOG_CFLOW1 ("        semCount = %d", semCount-1); 
#endif /* DEBUG_SISCO */

  misem_free_sem ();       
  }

/************************************************************************/
/************************************************************************/
/*                       misem_get_sem					*/
/************************************************************************/

ST_VOID misem_get_sem (ST_VOID)
  {
ST_THREAD_ID thisThreadId;

  if (!misem_already_inited)	/* Make sure gs is initialized.		*/
    misem_init ();

  /* Let's see just who we are						*/
  thisThreadId = GET_THREAD_ID();

  /* If we don't already own the semaphore, time to get it		*/
  if (currSemOwner != thisThreadId)
    {
    /* We need to get the semaphore, and will wait forever to do so	*/
    /* This function is not allowed to fail.				*/
    requestMutexSem ();

    /* We now have the semaphore					*/
    currSemOwner = thisThreadId;
    }

  /* This is a counting semaphore, eh?					*/
  ++semCount;
  }

/************************************************************************/
/************************************************************************/
/*                       misem_free_sem					*/
/************************************************************************/

ST_VOID misem_free_sem (ST_VOID)
  {
ST_THREAD_ID thisThreadId;

  /* Let's see just who we are						*/
  thisThreadId = GET_THREAD_ID();

  if (currSemOwner != thisThreadId || !semCount)
    {
    /* Log a major error here, call an exception, and scream!		*/
    MISEM_LOG_ALWAYS2 ("Error: Thread 0x%08lx freeing semaphore owned by Thread 0x%08lx", 
				thisThreadId, currSemOwner);
    return;
    }

  /* This is a counting semaphore, eh?					*/
  --semCount;
  if (semCount == 0)
    {
    /* Time to let go of the semaphore					*/
    /* This function is not allowed to fail.				*/
    currSemOwner = (ST_THREAD_ID) NULL;
    releaseMutexSem ();
    }
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* WINDOWS 95/NT							*/
/************************************************************************/
#if defined(_WIN32)				/* VC++, 32-Bit		*/
/************************************************************************/
/*                         initMutexSem					*/
/************************************************************************/

static ST_RET initMutexSem ()
  {
  
  InitializeCriticalSection (&miSemCritSection);

  return (SD_SUCCESS);
  }

/************************************************************************/
/*			requestMutexSem	        			*/
/************************************************************************/
/* This function is provided as macro's in the no-debug versions	*/
#if defined(DEBUG_SISCO) 	

static ST_VOID requestMutexSem ()
  {

  EnterCriticalSection (&miSemCritSection);
  }
#endif /* (DEBUG_SISCO) */

/************************************************************************/
/*			releaseMutexSem		        		*/
/************************************************************************/
/* This function is provided as macro's in the no-debug versions	*/
#if defined(DEBUG_SISCO) 	

static ST_VOID releaseMutexSem ()
  {

  LeaveCriticalSection (&miSemCritSection);
  }

#endif /* (DEBUG_SISCO) */

#endif /* _WIN32  */



