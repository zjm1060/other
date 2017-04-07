/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      		2001-2002, All Rights Reserved                          */
/*									*/
/* MODULE NAME : miu_ir.c    						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     05    Copyright year update                        */
/* 06/20/02  MDE     04	   Use cleaned up mics_icfg names		*/
/* 05/03/02  MDE     03	   Added mics_cfg.h				*/
/* 03/21/02  MDE     02	   Added Association State request		*/
/* 12/27/01  MDE     01    New module					*/
/************************************************************************/
/************************************************************************/

/* Ax-S4 ICCP TODO:
1. Use IccpCfg to set registry parameters
2. Read redundancy parameters from registry
3. Implement the IR user functions (templates below)
4. Call irStart at startup (see miuStartIr for example)
5. Call the irService function periodically (200ms?)
6. Pass changed data values to IR using 'irLocalDvValue'
  (may want to do this periodically or on change?? Will think about this)
7. Keep the following RPL parameters up to date:
  ST_BOOLEAN		ready;
  ST_INT32		totalLinks;
  ST_INT32		activeLinks;
  For instance, 
    irGetStatus (&irCtrl);
    irCtrl->rplState->ready = SD_TRUE;
9. During startup, (in redundant mode), wait for uIrSetMode request 
   before enabling associations.
10. During shutdown, issue irModeSelect to pass control to partner.
    See miuEndIr for example.
11. Add OPC Items, get values from irCtrl->rplState

Notes:
1. Data sync is not completed
2. The TCP/IP RPL still needs some work
3. Association Connect Timeout is not implemented
4. CRC on configuration is not complete irCtrl->rplState->rplCfg.cfgCrc

*/				        

/************************************************************************/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "glbsem.h"
#include "stime.h"

#include "mi.h"
#include "mi_usr.h"
#include "mics_cfg.h"
#include "rpl.h"
#include "gensock.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

#ifdef MOSI
ST_VOID wait_any_event (ST_LONG max_ms_delay);
#endif

/************************************************************************/
/* Used for testing only */

ST_BOOLEAN pauseIrService = SD_FALSE;

/************************************************************************/
/************************************************************************/
/* USER REDUNDANCY SUPPORT CODE 					*/
/************************************************************************/
/************************************************************************/
/*			miuStartIr						*/
/************************************************************************/

ST_VOID miuStartIr ()
  {
static RPL_CFG rplCfg;	      /* Static to avoid race before RPL copies */
ICCP_REDUNDANCY_CTRL *irCtrl;
MIU_REMOTE *micuRemote;
MICU_DV *micuDv;
MI_REMOTE *mi_remote;
ST_RET 	 rc;

  memset (&rplCfg, 0, sizeof (RPL_CFG));

  rpl_debug_sel |= RPL_LOG_ERR;
  rpl_debug_sel |= RPL_LOG_NERR;
//  rpl_debug_sel |= RPL_LOG_FLOW;

  ir_debug_sel |= IR_LOG_ERR;
  ir_debug_sel |= IR_LOG_NERR;
  ir_debug_sel |= IR_LOG_FLOW;
//  ir_debug_sel |= IR_LOG_DATA;

  gsock_debug_sel |= GSOCK_LOG_ERR;
//  gsock_debug_sel |= GSOCK_LOG_NERR;
//  gsock_debug_sel |= GSOCK_LOG_FLOW;

  rplCfg.port = 5555;
  strcpy (rplCfg.partnerIPAddr, "192.168.1.60");
  rplCfg.connectRetryTime = 5000;
  rplCfg.heartbeatTxPeriod = 30000;
  rplCfg.rplStartupTime = 5000;
  rplCfg.rplSleepTime = 200;
  rplCfg.rplRequestTimeout1 = 5000;
  rplCfg.rplRequestTimeout2 = 5000;
  rplCfg.switchToActiveDelay = 1000;
  rplCfg.maxDataValueReqPend = 2;
  rplCfg.linkConnectTimeoutEnable = SD_TRUE;
  rplCfg.linkConnectTimeout = 60000;

  rplCfg.dataSyncMode = RplDsModeLocalAndRemote;

  if (miuRole == MI_ROLE_PRIMARY)
    rplCfg.role = RplRolePrimary;
  else if (miuRole == MI_ROLE_SECONDARY)
    rplCfg.role = RplRoleSecondary;
  else
    return;

  _irDataValueBufferTime = 1000;
  _irDataValueBufferCount = 100;

  rc = irStart (&rplCfg);
  if (rc == SD_SUCCESS)
    {
    irGetStatus (&irCtrl);
    irCtrl->rplState->ready = SD_TRUE;
    irCtrl->rplState->totalLinks = 1;
    irCtrl->rplState->activeLinks = 0;

  /* Set remote DV index, for ease of use later .. */
    micuRemote = miu_remote_list;
    while (micuRemote != NULL)
      {
      mi_remote = micuRemote->mi_remote;
      micuDv = micuRemote->dv_list;
      while (micuDv != NULL)
        {
        micuDv->ir_idx = irFindMicDvIdx (mi_remote->mic_vcc, 
        				      micuDv->mic_dv);

        micuDv = list_get_next (micuRemote->dv_list, micuDv);
        }
      micuRemote = list_get_next (miu_remote_list, micuRemote);
      }
    }
  }

/************************************************************************/
/************************************************************************/

ST_VOID miuEndIr ()
  {
ICCP_REDUNDANCY_CTRL *irCtrl;


  irGetStatus (&irCtrl);
  if (irCtrl->rplState->rplMode != RplModeActive)
    return;

  if (irCtrl->rplState->rplState != RplStateOkConnected)
    return;

/* Terminate the redundancy connection, switch to standby */
  irEnd ();
  miuDoIt = SD_TRUE;
  while (miuDoIt)
    {
    if (irCtrl->rplState->rplMode != RplModeActive ||
        irCtrl->rplState->rplState != RplStateOkConnected)
      {
      break;
      }
#if defined (MOSI)		/* If stack is MOSI, use events		*/
    wait_any_event (100);	/* Wait 1000 milliseconds		*/
    while (mvl_comm_serve ())	/* Perform communications service 	*/
      { 			/* Loop until "mvl_comm_serve" returns 0*/
      }
#else
    mvl_comm_serve ();
#endif
    irService ();
    mi_service ();
    miUsrKbService ();
    }
  rplEnd ();
  }

/************************************************************************/
/*			uIrSetMode					*/
/************************************************************************/

extern ST_BOOLEAN micuAssocEnable;

ST_RET uIrSetMode (enum RPL_MODE mode)
  {
  printf ("\nSeting mode to %s", mode == RplModeActive ? "Active" : "Standby");
  if (mode == RplModeActive)
    {
    micuAssocEnable = SD_TRUE;
    mi_enable_remote_assoc (NULL);
    }
  else
    {
    micuAssocEnable = SD_FALSE;
    mi_disable_remote_assoc (NULL);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			uIrModeRequest					*/
/************************************************************************/

ST_RET uIrModeRequest (enum RPL_MODE mode)
  {
ST_RET rc;

  switch (mode)
    {
    case RplModeActive:
      printf ("\nMode request received: Active");
      rc = SD_SUCCESS;
    break;

    case RplModeStandby:
      printf ("\nMode request received: Standby");
      rc = SD_SUCCESS;
    break;

    case RplModeStartup:
      printf ("\nMode request received: Startup");
      rc = SD_FAILURE;
    break;
    }

  return (rc);
  }


/************************************************************************/
/*			uIrSetModeResp					*/
/************************************************************************/

ST_VOID uIrSetModeResp (RPL_SET_MODE_RESP *setModeResp)
  {
  /* This is called when the response to a switch request is received */
  if (setModeResp->result == SD_SUCCESS)
    printf ("\nSet Mode Succeded");
  else
    printf ("\nSet Mode Failed");
  }

/************************************************************************/
/*			uIrLinkConnectTimeout				*/
/************************************************************************/

ST_VOID uIrLinkConnectTimeout ()
  {
  printf ("\nLink connect timeout detected");
  printf ("\nAttempting switch to Standby");

  irModeSelect (RplModeActive);
  }

/************************************************************************/
/*			uIrRplMsgRcvd					*/
/************************************************************************/

ST_VOID uIrRplMsgRcvd ()
  {
  /* This is called from a RX thread. We can use it to wake the main	*/
  /* IR service thread.							*/
  }


/************************************************************************/
/*			uIrAssocState					*/
/************************************************************************/

ST_VOID uIrAssocState (MI_ASSOC_CTRL *mi_assoc, ST_INT remoteState)
  {
  printf ("\nPartner Remote '%s' assoc '%s' state: %s", 
	      mi_assoc->mi_remote->name, mi_assoc->name,
		remoteState == MI_ASSOC_STATE_ACTIVE ? "Active" : "Idle");
  }

/************************************************************************/
/************************************************************************/
/*			uIrStartDataValue				*/
/************************************************************************/

ST_RET uIrStartDataValue (RPL_DATA_VALUE_REQ *duReq, 
			   RPL_DATA_VALUE_RESP *duResp, ST_VOID **usrHandleOut)
  {
MI_REMOTE *mi_remote;
ST_RET rc;

  rc = SD_SUCCESS;
  if (duReq->ccType == RplCcTypeRemoteCc)
    {
    mi_remote = mi_find_remote (duReq->remoteName);
    *usrHandleOut = mi_remote;
    if (mi_remote == NULL)
      {
      printf ("\nStart Data Values: Could not find remote:%s", duReq->remoteName);
      rc = SD_FAILURE;
      }
    }
  return (rc);
  }

/************************************************************************/
/*			uIrLocalDvValue				*/
/************************************************************************/

ST_RET uIrLocalDvValue (RPL_DV_VALUE  *dvValue, ST_VOID *usrHandle)
  {
MIS_DV_REF dvRef;
MIS_DV *misDv;
MISU_DV *miuDv;
ST_RET rc;

  dvRef = (MIS_DV_REF) dvValue->ref;
  misDv = MIS_DV_REF_TO_DV(dvRef);
  miuDv = misDv->access_info;
#if 0
  printf ("\nReceived data update for DV: %s", misDv->dv_name);
#endif

  rc = SD_SUCCESS;
  switch (dvValue->type_id)
    {
    case MI_TYPEID_REAL:
      miuDv->data.re.Value = dvValue->value.r;
#if 0
      printf (" %.f", dvValue->value.r);
#endif
    break;
  
    case MI_TYPEID_STATE:
      miuDv->data.se.Flags = dvValue->value.s;
    break;
  
    case MI_TYPEID_DISCRETE:
#if 0
      printf (" %.d", dvValue->value.d);
#endif
      miuDv->data.de.Value = dvValue->value.d;
    break;
  
    case MI_TYPEID_REAL_Q:
#if 0
      printf (" %.f", dvValue->value.rq.Value);
#endif
      miuDv->data.re.Value = dvValue->value.rq.Value;
      miuDv->data.re.Flags = dvValue->value.rq.Flags;
    break;
  
    case MI_TYPEID_STATE_Q:
      miuDv->data.se.Flags = dvValue->value.sq;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
#if 0
      printf (" %.d", dvValue->value.dq.Value);
#endif
      miuDv->data.de.Value = dvValue->value.dq.Value;
      miuDv->data.de.Flags = dvValue->value.dq.Flags;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
#if 0
      printf (" %.f", dvValue->value.rqt.Value);
#endif
      miuDv->data.re.Value = dvValue->value.rqt.Value;
      miuDv->data.re.Flags = dvValue->value.rqt.Flags;
      miuDv->data.re.TimeStamp = dvValue->value.rqt.TimeStamp;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      miuDv->data.se.Flags = dvValue->value.sqt.Flags;
      miuDv->data.se.TimeStamp = dvValue->value.sqt.TimeStamp;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
#if 0
      printf (" %.d", dvValue->value.dqt.Value);
#endif
      miuDv->data.de.Value = dvValue->value.dqt.Value;
      miuDv->data.de.Flags = dvValue->value.dqt.Flags;
      miuDv->data.de.TimeStamp = dvValue->value.dqt.TimeStamp;
    break;

    case MI_TYPEID_REAL_EXTENDED:
#if 0
      printf (" %.f", dvValue->value.re.Value);
#endif
      miuDv->data.re = dvValue->value.re;
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      miuDv->data.se = dvValue->value.se;
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
#if 0
      printf (" %.d", dvValue->value.de.Value);
#endif
      miuDv->data.de = dvValue->value.de;
    break;
  
    default:
      printf ("\nError. invalid type_id: %d", dvValue->type_id);
      rc = SD_FAILURE;
    break;
    }
  return (rc);
  }

/************************************************************************/
/*			uIrLocalDevValue				*/
/************************************************************************/

ST_RET uIrLocalDevValue (RPL_DEV_VALUE  *devValue, ST_VOID *usrHandle)
  {
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			uIrRemoteDvValue				*/
/************************************************************************/


ST_RET uIrRemoteDvValue (RPL_DV_VALUE  *dvValue, ST_VOID *usrHandle)
  {
MI_REMOTE *mi_remote;
ST_UINT32 idx;
MIC_DV  *micDv;
MICU_DV *micuDv;
ST_RET rc;

  mi_remote = usrHandle;
  if (mi_remote == NULL)
    return (SD_FAILURE);

  idx = dvValue->ref;
  if (idx & IR_REMOTE_HANDLE_ICC_FLAG)
    {
    idx &= (~IR_REMOTE_HANDLE_ICC_FLAG);
    micDv = mi_remote->mic_vcc->icc_dv_tbl[idx];
    }
  else
    micDv = mi_remote->mic_vcc->vcc_dv_tbl[idx];

  micuDv = (MICU_DV  *) micDv->handle_info;
#if 0
  printf ("\nReceived data update for Remote:%s\tDV: %s",mi_remote->name, micDv->dv_name);
#endif
  rc = SD_SUCCESS;
  switch (dvValue->type_id)
    {
    case MI_TYPEID_REAL:
#if 0
      printf (" %.f", dvValue->value.r);
#endif
      micuDv->data.re.Value = dvValue->value.r;
    break;
  
    case MI_TYPEID_STATE:
      micuDv->data.se.Flags = dvValue->value.s;
    break;
  
    case MI_TYPEID_DISCRETE:
#if 0
      printf (" %.d", dvValue->value.d);
#endif
      micuDv->data.de.Value = dvValue->value.d;
    break;
  
    case MI_TYPEID_REAL_Q:
#if 0
      printf (" %.f", dvValue->value.rq.Value);
#endif
      micuDv->data.re.Value = dvValue->value.rq.Value;
      micuDv->data.re.Flags = dvValue->value.rq.Flags;
    break;
  
    case MI_TYPEID_STATE_Q:
      micuDv->data.se.Flags = dvValue->value.sq;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
#if 0
      printf (" %.d", dvValue->value.dq.Value);
#endif
      micuDv->data.de.Value = dvValue->value.dq.Value;
      micuDv->data.de.Flags = dvValue->value.dq.Flags;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
#if 0
      printf (" %.f", dvValue->value.rqt.Value);
#endif
      micuDv->data.re.Value = dvValue->value.rqt.Value;
      micuDv->data.re.Flags = dvValue->value.rqt.Flags;
      micuDv->data.re.TimeStamp = dvValue->value.rqt.TimeStamp;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      micuDv->data.se.Flags = dvValue->value.sqt.Flags;
      micuDv->data.se.TimeStamp = dvValue->value.sqt.TimeStamp;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
#if 0
      printf (" %.d", dvValue->value.dqt.Value);
#endif
      micuDv->data.de.Value = dvValue->value.dqt.Value;
      micuDv->data.de.Flags = dvValue->value.dqt.Flags;
      micuDv->data.de.TimeStamp = dvValue->value.dqt.TimeStamp;
    break;

    case MI_TYPEID_REAL_EXTENDED:
#if 0
      printf (" %.f", dvValue->value.re.Value);
#endif
      micuDv->data.re = dvValue->value.re;
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      micuDv->data.se = dvValue->value.se;
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
#if 0
      printf (" %.d", dvValue->value.de.Value);
#endif
      micuDv->data.de = dvValue->value.de;
    break;
  
    default:
      printf ("\nError. invalid type_id: %d", dvValue->type_id);
      rc = SD_FAILURE;
    break;
    }

  return (rc);
  }

/************************************************************************/
/*			uIrRemoteDevValue				*/
/************************************************************************/

ST_RET uIrRemoteDevValue (RPL_DEV_VALUE  *devValue, ST_VOID *usrHandle)
  {
MI_REMOTE *mi_remote;

  mi_remote = usrHandle;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			uIrEndDataValue				*/
/************************************************************************/

ST_RET uIrEndDataValue (RPL_DATA_VALUE_REQ *duReq, 
		         RPL_DATA_VALUE_RESP *duResp, ST_VOID **usrHandleOut)
  {
  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			uIrDataSyncReq					*/
/************************************************************************/

ST_RET uIrDataSyncReq  (RPL_DATA_SYNC_REQ *dsReq)
  {
MI_REMOTE *mi_remote;
MISU_DV *miuDv;
MIU_REMOTE *micuRemote;
MICU_DV	 *micuDv;	/* List of remote data values 		*/

/* Our partner is asking for a data update, let's send it to her	*/
/* First let's do local */
  printf ("\nPartner is asking for data sync");
  if (dsReq->syncType == RplDsReqTypeAll || 
      dsReq->syncType == RplDsReqTypeLocal)
    {

  /* Data Values */
    miuDv = misu_dv_list;
    while (miuDv != NULL)
      {
      irLocalDvValue (miuDv->dvRef, miuDv->mi_type, &miuDv->data);
      miuDv = (MISU_DV *) list_get_next (misu_dv_list, miuDv);
      }
    }

/* Doing any remotes? */
  if (dsReq->syncType == RplDsReqTypeAll || 
      dsReq->syncType == RplDsReqTypeRemote)
    {
    micuRemote = miu_remote_list;
    while (micuRemote != NULL)
      {
      mi_remote = micuRemote->mi_remote;

      if (dsReq->syncType == RplDsReqTypeAll ||
          !strcmp (mi_remote->name, dsReq->remoteName))
        {
        micuDv = micuRemote->dv_list;
        while (micuDv != NULL)
          {
          irRemoteDvValue (mi_remote, micuDv->ir_idx, 
          		 micuDv->mi_type, &micuDv->data);
  
          micuDv = list_get_next (micuRemote->dv_list, micuDv);
          }
        }
      micuRemote = list_get_next (miu_remote_list, micuRemote);
      }
    }

  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/************************************************************************/
static ST_VOID restartIr (ST_VOID);

ST_BOOLEAN miuIrKeyService (ST_CHAR c)
  {
ST_BOOLEAN handled = SD_FALSE;
MIC_DV 	  *mic_dv;
#if 0
RPL_STATE_INFO *rplState;

  rplState = _irCtrl->rplState;
  handled = SD_FALSE;
  if (c == 'C')
    {
    if (rplState->enable)
      rplDisable ();
    else
      rplEnable ();

    printf ("\n RPL %s", rplState->enable ? "Enabled" : "Disabled");
    handled = SD_TRUE;
    }
#endif

  if (c == 'A')
    {
    printf ("\nAttempting manual switch to Active");
    irModeSelect (RplModeStandby);
    handled = SD_TRUE;
    }
  if (c == 'S')
    {
    printf ("\nAttempting manual switch to Standby");
    irModeSelect (RplModeActive);
    handled = SD_TRUE;
    }

  if (c == 'L')
    {
    printf ("\nConfiguration change for Local");
    mis_resize_dv_tables (mis_num_dv_ctrl+1);
    mis_add_dv (chk_strdup ("Mike!"), NULL);
    irCfgChange ();
    }

  if (c == 'R')
    {
    printf ("\nConfiguration change for Remote");

    mic_dv = mi_remote_list->mic_vcc->icc_dv_tbl[mi_remote_list->mic_vcc->num_icc_dv-1];
    mic_dv->dv_name = chk_strdup ("Mike!");
    irCfgChange ();
    }
   
  if (c == 'T')
    {
    printf ("\nRestart RPL");
    restartIr ();
    }
   
  if (c == 'P')
    {
    if (pauseIrService == SD_FALSE)
      {
      printf ("\nPause IR service");
      pauseIrService = SD_TRUE;
      }
    else
      {
      printf ("\nUnpause IR service");
      pauseIrService = SD_FALSE;
      }
    }

  if (c == '?')
    {
    printf ("\n\n RPL Test Options:");
    printf ("\n   A = Manual switch to Active");
    printf ("\n   S = Manual switch to Standby");
    printf ("\n   L = Reconfigure local");
    printf ("\n   R = Reconfigure remote");
    printf ("\n   T = Restart IR");
    printf ("\n   P = Toggle 'Pause IR service'");
    }

  return (handled);
  }



/************************************************************************/
/*			restartIr					*/
/************************************************************************/

static ST_VOID restartIr ()
  {
ICCP_REDUNDANCY_CTRL *irCtrl;
ST_RET rc;

  irGetStatus (&irCtrl);
  rc = irModeSelect (RplModeActive);
  if (rc == SD_SUCCESS)
    {
    while (miuDoIt)
      {
      if (irCtrl->rplState->rplMode != RplModeActive ||
          irCtrl->rplState->rplState != RplStateOkConnected)
        {
        break;
        }
  #if defined (MOSI)		/* If stack is MOSI, use events		*/
      wait_any_event (100);	/* Wait 1000 milliseconds		*/
      while (mvl_comm_serve ())	/* Perform communications service 	*/
        { 			/* Loop until "mvl_comm_serve" returns 0*/
        }
  #else
      mvl_comm_serve ();
  #endif
      irService ();
      mi_service ();
      miUsrKbService ();
      }
    }
  rplEnd ();
  miuStartIr ();
  }
