#ifndef LEAN_A_H
#define LEAN_A_H
/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	1996 All Rights Reserved					*/
/*									*/
/* MODULE NAME : lean_a.h						*/
/* PRODUCT(S)  : Lean-T Stack SUIC					*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 02/08/02  JRB     09    Move osicfgx prototype to mvl_acse.h.	*/
/* 11/27/00  JRB     08    Add osicfgx proto.				*/
/* 12/10/97  JRB     07    Add MMSEventName and hMMSEvent for OS2.	*/
/* 08/14/97  JRB     06    #include "acse2usr.h" and use PRES_ADDR in	*/
/*			   DIB_ENTRY.					*/
/* 05/27/97  JRB   7.00    MMSEASE 7.0 release.				*/
/* 05/02/97  JRB     05    Del MAX_?_SEL defines.  Use suicacse.h defs.	*/
/* 05/01/97  JRB     04    Move MAX_SUIC_CHAN define to here.		*/
/* 01/02/96  JRB     03    Add MMSEventName and hMMSEvent.		*/
/* 10/04/96  JRB     02    #ifdef MCGW, add adlcAddr to DIB_ENTRY.	*/
/*			   Add *_clt7 function prototypes.		*/
/* 05/29/96  MDE     01    Changed DIB_ENTRY - name is now a pointer	*/
/* 03/27/96  MDE           Created	                              	*/
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined (_WIN32)
#include <windows.h>
#endif	/* _WIN32	*/

#if defined (__OS2__)
#define INCL_BASE
#include <os2.h>
#endif	/* __OS2__	*/

#include "asn1defs.h"		/* for MMS_OBJ_ID definition		*/
#include "acse2usr.h"		/* for PRES_ADDR definition		*/
									  
#define MAX_SUIC_CHAN 256	/* number of channels			*/

/************************************************************************/
/* The structure below is used to save Directory and other information	*/
/* about local and remote AE's read in from SUIC.DIB			*/
/************************************************************************/

#define DIB_ENTRY_DEFINED

struct dib_entry
{
  ST_LONG reserved;		/* reserved field			*/
  ST_CHAR *name;			/* user-defined ASCII character string	*/
  ST_CHAR local;			/* local or remote name			*/
  ST_UCHAR AP_title_pres;		/* present flag				*/
  MMS_OBJ_ID AP_title; 		/* AP title    				*/
  ST_UCHAR AP_inv_id_pres;		/* present flag				*/
  ST_INT32 AP_invoke_id;		/* AP invocation ID    			*/
  ST_UCHAR AE_qual_pres;		/* present flag				*/
  ST_INT32 AE_qual;			/* AE qualifier    			*/
  ST_UCHAR AE_inv_id_pres;		/* present flag				*/
  ST_INT32 AE_invoke_id;		/* AE invocation ID    			*/
  PRES_ADDR pres_addr;		/* Presentation address.		*/
} ;
typedef struct dib_entry DIB_ENTRY;

/************************************************************************/
/* Channel-oriented control structure used to hold connection-specific	*/
/* information								*/
/************************************************************************/

#define MAX_TX_OUT	10	/* max. outstanding TX requests allowed	*/

struct chanctrl
  {
  DIB_ENTRY *loc_de;		/* Pointer to Local DIB Entry */
  DIB_ENTRY *rem_de;		/* Pointer to Remote DIB Entry */
  } ;
extern struct chanctrl *s_chan_ctrl;

/************************************************************************/
/* Structure to store incoming associate indication in case no listen	*/
/* channel is available							*/
/************************************************************************/

extern ST_INT num_loc_dib_entries;
extern ST_INT num_rem_dib_entries;
extern DIB_ENTRY *loc_dib_table;
extern DIB_ENTRY *rem_dib_table;

/************************************************************************/
/* Event handling variables for WIN32 & OS2.				*/
/************************************************************************/
#ifdef _WIN32
extern ST_CHAR *MMSEventName;	/* Pointer to the globally unique name of Event Object */
extern HANDLE hMMSEvent;	/* Handle to Event Object to be signalled when */
				/* s_serve () needs to be called */
#endif	/* _WIN32	*/
#ifdef __OS2__
extern ST_CHAR *MMSEventName;	/* Pointer to the globally unique name of Event Object */
extern HEV      hMMSEvent;	/* Handle to Event Object to be signalled when */
				/* s_serve () needs to be called */
#endif	/* __OS2__	*/

/************************************************************************/
/* Prototypes								*/
/************************************************************************/
ST_VOID s_reset_ctrl (ST_INT);
ST_RET init_dirser (ST_VOID);
DIB_ENTRY *find_dib_entry (ST_CHAR *);
DIB_ENTRY *find_loc_dib_entry (ST_CHAR *ar_name);
DIB_ENTRY *find_rem_dib_entry (ST_CHAR *ar_name);

ST_RET init_dirser_clt7 (ST_VOID);
DIB_ENTRY *find_dib_entry_clt7 (ST_CHAR *);
DIB_ENTRY *find_loc_dib_entry_clt7 (ST_CHAR *ar_name);

#ifdef __cplusplus
}
#endif

#endif	/* !LEAN_A_H	*/


