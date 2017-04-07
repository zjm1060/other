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

  nfds = select (nfds, (int *)&readfds, (int *)&writefds, (int *)&exceptfds, &stTimeVal);
  }

#endif		/* MOSI				*/
