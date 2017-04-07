/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	      1998-2002, All Rights Reserved                        */
/*									*/
/* MODULE NAME : mi_usr.h    						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*    This file contains declarations needed by rt_types.c to resolve	*/
/*    variable association initiailzation statements.			*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     08    Copyright year update                        */
/* 04/16/02  MDE     07	   Moved mics_icfg related to (new) mics_icfg.h	*/
/* 03/25/02  MDE     06	   Added tolerance of mis-configured DS		*/
/* 01/25/02  MDE     05	   Added redundancy support code		*/
/* 10/29/01  MDE     04	   Added USE_ICFG code				*/
/* 10/19/01  MDE     03	   Minor cleanup				*/
/* 10/12/99  NAV     02    Add Block 4 and 5 support			*/
/* 12/31/97  MDE     01    New file					*/
/************************************************************************/

#ifndef MI_USR_INCLUDED
#define MI_USR_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/

#include "mi.h"
#include "mi_icfg.h"

/************************************************************************/

extern ST_BOOLEAN miuDoIt;
extern ST_BOOLEAN miuAuto;
extern ST_BOOLEAN miuFullSpeed;

/* miuMode defines */
#define MI_MODE_CLIENT	0
#define MI_MODE_SERVER	1
#define MI_MODE_BOTH	2
extern ST_INT miuMode;

ST_VOID miuInitIccp (ST_VOID);
ST_RET  miUsrKbService (ST_VOID);
ST_VOID miuTerminate (ST_VOID);
ST_VOID wait_any_event (ST_LONG max_ms_delay);

extern ST_BOOLEAN micuLoopEnable;

extern MI_CONN       *miuMiConn;
extern MI_REMOTE     *miuClientRemote;
extern MI_ASSOC_CTRL *miuClientRemoteAssoc;

ST_VOID miuEnableIccpClient (ST_VOID);
ST_VOID miuEnableIccpServer (ST_VOID);

ST_VOID misuChangeDv (ST_VOID);
ST_VOID misuSendMessage (ST_VOID);

ST_VOID micuStartAssocActivity (MI_CONN *mi_conn);
ST_VOID micuStopAssocActivity (MI_CONN *mi_conn);
static ST_VOID micuStopAllDsTs (MI_CONN *mi_conn);
static ST_VOID micuStartIMTransfers (MI_CONN *mi_conn);
static ST_VOID micuStopIMTransfers  (MI_CONN *mi_conn);
ST_VOID micuTestAllDevices (MI_CONN *mi_conn);

ST_VOID micuReadAllDv (MI_CONN *mi_conn);
ST_VOID micuWriteDv (MI_CONN *mi_conn);

/************************************************************************/
/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* MI_USR_INCLUDED */
/************************************************************************/

