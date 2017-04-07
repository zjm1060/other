/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	1998 All Rights Reserved					*/
/*									*/
/* MODULE NAME : event.c						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	Event handling functions.					*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	wait_any_event ()						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev     Comments					*/
/* --------  ---  ------   -------------------------------------------  */
/* 05/17/02  JRB    05     Put back old code for PHARLAP_ETS, PharLap	*/
/* 			   doesn't have "Wait..Ex".			*/
/* 04/10/00  JRB    04     Del "select.h". See "sysincs.h".		*/
/* 04/06/00  JRB    03     Use WaitForMultipleObjectsEx for WIN32:	*/
/*			   CRITICAL for ethernet driver interface.	*/
/* 08/05/99  JRB    02     Use a_get_ev.., gs_wait.. for WIN32.		*/
/*			   Use hTcpEvent for OS2.			*/
/* 11/10/98  JRB    01     Created					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mmsdefs.h"

#include <assert.h>

#if defined (MOSI)		/* Events only supported for MOSI.	*/

ST_VOID wait_any_event (ST_LONG max_ms_delay);

#if defined (_WIN32)
/************************************************************************/
/*                          wait_any_event				*/
/************************************************************************/

ST_VOID wait_any_event (ST_LONG max_ms_delay)
  {
ST_EVENT_SEM *EventArray;
ST_INT NumEntries;
#if defined (PHARLAP_ETS)
ST_BOOLEAN activity [2];	/* We don't chk it, but gs_wait.. needs it*/
#endif

  EventArray = a_get_event_handles (&NumEntries);
  
  /* Make sure there is at least 1 event to wait on. If not, probably	*/
  /* because library not compiled with correct switches.		*/
  assert (NumEntries > 0);

  /* If this fails, size of "activity" array must be increased.		*/
  assert (NumEntries <= 2);

#if defined (PHARLAP_ETS)	/* PharLap doesn't have/need "Wait...Ex"*/
  gs_wait_mult_event_sem (NumEntries, EventArray, activity, max_ms_delay);
#else
  /* CRITICAL: Use Wait..Ex with last arg TRUE so that Ethernet driver	*/
  /*           completion routines may be called.			*/
  WaitForMultipleObjectsEx (NumEntries, EventArray,
                 FALSE,	/* return when ANY object signaled	*/
                 max_ms_delay,
                 TRUE);
#endif

  /* Something woke us up. Don't care if it was event or timeout.	*/
  /* Just return to main loop to attempt processing.			*/
  return;
  }

#elif defined (__OS2__)

/************************************************************************/
/*                          wait_any_event				*/
/* DEBUG: not tested but should work on OS/2.				*/
/************************************************************************/

ST_VOID wait_any_event (ST_LONG max_ms_delay)
  {
#define MMS_EVENT_SEM_ID	0
static SEMRECORD sems[2];
static HMUX hMux;
APIRET apiret;
ST_ULONG post_count;
ST_ULONG semId;

  /* This function only waits on the MMS event. To wait on other events	*/
  /* also, add to the "sems" array.					*/

  if (hMux == NULL)
    {
    sems[0].hsemCur = (void *) hTcpEvent;
    sems[0].ulUser = MMS_EVENT_SEM_ID;
    apiret = DosCreateMuxWaitSem (NULL, &hMux, 1, sems, DCMW_WAIT_ANY);
    if (apiret != NO_ERROR)
      {
      printf ("\n DosCreateMuxWaitSem failed, rc = %d",apiret);
      hMux = NULL;
      }
    return;	/* Just return. New event so no one has signaled it yet.*/
    }

  /* Wait on the MMS Event Semaphore.		*/

  apiret = DosWaitMuxWaitSem (hMux, max_ms_delay, &semId);

  if (apiret == NO_ERROR)
    {
    if (semId == MMS_EVENT_SEM_ID) /* MMS event			*/
      {
      gs_reset_event_sem (hTcpEvent);	/* OS2 requires reset	*/
      }
    }
  else if (apiret != ERROR_TIMEOUT)
    {
    printf ("\n DosWaitMuxWaitSem error, apiret = %d",apiret);
    }
  }

#elif defined(MSDOS) || defined(__MSDOS__)

/************************************************************************/
/*                          wait_any_event				*/
/************************************************************************/

ST_VOID wait_any_event (ST_LONG max_ms_delay)
  {
  return;
  }

#else	/* All other systems	*/

extern int pipe_to_main;	/* DEBUG: move to include	*/
/************************************************************************/
/*                          wait_any_event				*/
/* Waits for msg received in "pipe_to_main" pipe.			*/
/************************************************************************/
ST_VOID wait_any_event (ST_LONG max_ms_delay)
  {
  struct timeval stTimeVal;
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  int    nfds;   

  FD_ZERO (&readfds);
  FD_ZERO (&writefds);
  FD_ZERO (&exceptfds);
  FD_SET (0, &readfds);		/* select on "stdin"	*/
  FD_SET (pipe_to_main, &readfds );	/* select on MMS Event pipe */

  /* pipe_to_main is only fds to check, so set nfds one greater.	*/
  nfds = pipe_to_main+1;

  /* If you want to wait for other events too, add more FD_SET calls,	*/
  /* and recompute "nfds" (or just set it to FD_SETSIZE).		*/

  max_ms_delay *= 1000; /*** Convert to micro seconds ***/
  stTimeVal.tv_sec  = max_ms_delay / 1000000;
  stTimeVal.tv_usec = max_ms_delay % 1000000;

/*  nfds = select (nfds, (int *)&readfds, (int *)&writefds, (int *)&exceptfds, &stTimeVal);
Modified 2006.12.22 zy */
  nfds = select (nfds, &readfds, &writefds, &exceptfds, &stTimeVal);
  }

#endif		/* All other systems		*/

#endif		/* MOSI				*/

