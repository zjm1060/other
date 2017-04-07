/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      		2001-2002, All Rights Reserved                          */
/*									*/
/* MODULE NAME : mi_ir.c    						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     08    Copyright year update                        */
/* 04/24/02  MDE     07    Flush list checks irDvListCtrl for NULL	*/
/* 04/16/02  MDE     06	   Now require uIrAssocState			*/
/* 03/21/02  MDE     05	   Added Association State request		*/
/* 02/26/02  MDE     04    Fixed heartbeat timeout for Active node	*/
/* 02/21/02  MDE     03    Fixed data sync for reconfigure 		*/
/* 02/18/02  MDE     02    Online reconfig, data sync, etc..		*/
/* 12/27/01  MDE     01    New module					*/
/************************************************************************/
/************************************************************************/

/************************************************************************/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "glbsem.h"
#include "stime.h"
#include "mi.h"
#include "rpl.h"
#include "gensock.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/

#define RPL_MODE_TEXT(mode) mode ==RplModeActive?"RplModeActive":"RplModeStandby"


/************************************************************************/

ST_UINT  ir_debug_sel;

SD_CONST ST_CHAR *SD_CONST _ir_flow_logstr = "IR_LOG_FLOW";
SD_CONST ST_CHAR *SD_CONST _ir_nerr_logstr = "IR_LOG_NERR";
SD_CONST ST_CHAR *SD_CONST _ir_err_logstr  = "IR_LOG_ERR";
SD_CONST ST_CHAR *SD_CONST _ir_data_logstr  = "IR_LOG_DATA";

ST_INT    _irDataValueBufferCount = IR_DATA_VALUE_BUFFER_COUNT_DEF;
ST_DOUBLE _irDataValueBufferTime  = IR_DATA_VALUE_BUFFER_TIME_DEF;
ST_INT    _irDataValueLocalListMax = IR_DATA_VALUE_LOCAL_LIST_MAX_DEF;
ST_INT    _irDataValueRemoteListMax = IR_DATA_VALUE_REMOTE_LIST_MAX_DEF;

static ICCP_REDUNDANCY_CTRL *_irCtrl;

typedef struct
  {
  DBL_LNK l;
  RPL_MSG_HDR *msg;
  } IR_RPL_MSG_RCV_CTRL;
IR_RPL_MSG_RCV_CTRL *_irRplRxMsgList;

static ST_VOID _irProcAllRxRplMsgs (ST_VOID);
static ST_VOID _irProcRplRxMsg (RPL_MSG_HDR *msg);

static ST_VOID _irAllocDataValueLists (ST_VOID);
static ST_VOID _irAllocLocalDataValueLists (ST_VOID);
static ST_VOID _irAllocRemoteDataValueLists (MI_REMOTE *mi_remote);
static ST_VOID _irAllocDataValueControl (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl);
static ST_VOID  _irFreeDataValueLists (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl);
static ST_VOID _irAddDvToDvList (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl, 
			         MIS_DV_REF dvRef, 
			         ST_INT type_id, ST_VOID *data);

static ST_VOID _irSendCrc (ST_BOOLEAN getpartnerCrc);
static ST_VOID _irSendHeartbeat (ST_VOID);
static ST_VOID _irSendAllAssocStates (ST_VOID);
static ST_VOID _irSendAssocState (MI_ASSOC_CTRL *mi_assoc, ST_INT state);
static ST_VOID _irHandleFirstHeartbeat (ST_VOID);
static ST_VOID _irHandleStartupTimeout (ST_VOID);
static ST_VOID _irGetDataLinks (ST_VOID);
static ST_VOID _irProcessHbTimeout (ST_VOID);

static ST_VOID _irServiceDataValues (ST_VOID);
static ST_BOOLEAN _irServiceDataValuesList (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl,
					 ST_LONG *dataSyncCountIo);
static ST_BOOLEAN _irOkToSendData (ST_VOID);

static ST_VOID _irProcHeartbeat (RPL_MSG_HDR *msg);
static ST_VOID _irProcCrc (RPL_MSG_HDR *msg);
static ST_VOID _irCompareCrc (ST_INT32 ourCrc, ST_INT32 partCrc, 
		              ST_INT32 *partStoreCrcIo, 
		              ST_BOOLEAN *matchedOut,
		              ST_BOOLEAN *matchedNewOut, ST_CHAR *logStr);
static ST_VOID _irProcAssocState (RPL_MSG_HDR *msg);
static ST_VOID _irProcSetModeReq (RPL_MSG_HDR *msg);
static ST_VOID _irProcSetModeResp (RPL_MSG_HDR *msg);
static ST_RET  _irCalluIrSetMode (enum RPL_MODE rplMode, 
			  	  enum IR_STATE irState);
static ST_VOID _irProcDataValuesResp (RPL_MSG_HDR *msg);
static ST_VOID _irProcDataValuesReq (RPL_MSG_HDR *msg);
static ST_VOID _irProcDataSyncReq (RPL_MSG_HDR *msg);
static ST_VOID _irProcDataSyncResp (RPL_MSG_HDR *msg);

static ST_VOID _irSetLocalLinkages (ST_BOOLEAN setMatching);
static ST_VOID _irSetAllRemoteLinkages (ST_BOOLEAN setMatching);
static ST_VOID _irSetRemoteLinkages (MI_REMOTE *mi_remote, ST_BOOLEAN setMatching);
static ST_VOID _irSendDataSyncReq (enum RPL_DATA_SYNC_REQ_TYPE syncType, 
				   MI_REMOTE *mi_remote);

static ST_VOID _irCalcCrcs (ST_VOID);
static ST_UINT32 _irLocalCrc (ST_VOID);
static ST_UINT32 _irLocalDvCrc (ST_UINT32 crc);
static ST_UINT32 _irLocalDevCrc (ST_UINT32 crc);

static ST_UINT32 _irRemoteCrc (MI_REMOTE *mi_remote, ST_UINT32 crc);
static ST_UINT32 _irMisDvCrc (MI_REMOTE *mi_remote, ST_UINT32 crc);
static ST_UINT32 _irMisDeviceCrc (MI_REMOTE *mi_remote, ST_UINT32 crc);
static ST_UINT32 _irMicDvCrc (MI_REMOTE *mi_remote, ST_UINT32 crc);
static ST_UINT32 _irMicDeviceCrc (MI_REMOTE *mi_remote, ST_UINT32 crc);

static ST_UINT   _irStringCrc (ST_UCHAR *address, ST_UINT size, ST_UINT crc);
static ST_UINT   _irIntCrc (ST_INT number, ST_UINT crc);

/************************************************************************/
/************************************************************************/
/*			irStart						*/
/************************************************************************/

ST_RET irStart (RPL_CFG *rplCfg)
  {
ST_RET 	 rc;

  stimeInit ();
  if (_irCtrl == NULL)
    _irCtrl = chk_calloc (1, sizeof (ICCP_REDUNDANCY_CTRL));

  _irCtrl->rplCfg = *rplCfg;

  if (rplCfg->role == RplRolePrimary)
    _irCtrl->irState = IrStatePrimaryStartup;
  else
    _irCtrl->irState = IrStateSecondaryStartup;

  _irCtrl->startupTimeOut = sGetMsTime () + rplCfg->rplStartupTime;

  rplGetState (&_irCtrl->rplState);
  IR_LOG_FLOW1 ("Starting as %s", 
		rplCfg->role == RplRolePrimary ? "Primary" : "Secondary");

  rc = rplStart (rplCfg);
  if (rc == SD_SUCCESS)
    rc = rplEnable ();

/* If we are doing data sync, alloc the buffers */
  _irAllocDataValueLists ();

/* Calculate all our configuration CRC's */
  _irCalcCrcs ();
  return (rc);
  }

/************************************************************************/
/*			irCfgChange					*/
/************************************************************************/

ST_VOID irCfgChange ()
  {
  IR_LOG_FLOW0 ("Configuration change, reset data sync system"); 

/* Stop data sync for now */
  _irCtrl->crcReceived = SD_FALSE;

/* Free and realloc the data value lists for the new config */
  _irAllocDataValueLists ();

/* Calculate all our configuration CRC's */
  _irCalcCrcs ();

/* Now sent the CRC's to the partner, get theirs back */
  _irSendCrc (SD_TRUE);
  }

/************************************************************************/
/*			_irCalcCrcs					*/
/************************************************************************/

static ST_VOID _irCalcCrcs ()
  {
MI_REMOTE  *mi_remote;

  _irCtrl->localCfgCrc = _irLocalCrc ();
  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    mi_remote->cfgCrc = _irRemoteCrc (mi_remote, 0);
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

/************************************************************************/
/*			_irAllocDataValueLists				*/
/************************************************************************/

static ST_VOID _irAllocDataValueLists ()
  {
MI_REMOTE *mi_remote;

  _irAllocLocalDataValueLists ();
  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    _irAllocRemoteDataValueLists (mi_remote);
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

/************************************************************************/

static ST_VOID _irAllocLocalDataValueLists ()
  {
  S_LOCK_COMMON_RESOURCES ();
  if (_irCtrl->_irDvListCtrl != NULL)
    _irFreeDataValueLists (_irCtrl->_irDvListCtrl);

  
  if (_irCtrl->rplCfg.dataSyncMode == RplDsModeLocalAndRemote ||
      _irCtrl->rplCfg.dataSyncMode == RplDsModeLocal)
    {
    _irCtrl->_irDvListCtrl = chk_calloc (1, sizeof (IR_DATA_VALUE_LIST_CTRL));
    _irCtrl->_irDvListCtrl->numDataValueCtrl = _irDataValueLocalListMax;
    _irCtrl->_irDvListCtrl->numDv = mis_num_dv_ctrl -_mi_num_special_vars;
    _irCtrl->_irDvListCtrl->ccType = RplCcTypeLocalCc;
    _irAllocDataValueControl (_irCtrl->_irDvListCtrl);
    }
  S_UNLOCK_COMMON_RESOURCES ();
  }


/************************************************************************/

static ST_VOID _irAllocRemoteDataValueLists (MI_REMOTE *mi_remote)
  {
IR_DATA_VALUE_LIST_CTRL *irDvListCtrl;
MIC_VCC *mic_vcc;


  S_LOCK_COMMON_RESOURCES ();
  if (mi_remote->irDvListCtrl != NULL)
    _irFreeDataValueLists (mi_remote->irDvListCtrl);

  if (_irCtrl->rplCfg.dataSyncMode == RplDsModeLocalAndRemote)
    {
    mic_vcc = mi_remote->mic_vcc;
    if (mic_vcc != NULL)
      {
      irDvListCtrl = chk_calloc (1, sizeof (IR_DATA_VALUE_LIST_CTRL));
      mi_remote->irDvListCtrl = irDvListCtrl;
      irDvListCtrl->mi_remote = mi_remote;
      irDvListCtrl->numDataValueCtrl = _irDataValueRemoteListMax;
  
    /* Minus 4 for DSTS special variables */
      irDvListCtrl->numDv = mic_vcc->num_vcc_dv + mic_vcc->num_icc_dv - 4;
      irDvListCtrl->ccType = RplCcTypeRemoteCc;
      _irAllocDataValueControl (irDvListCtrl);
      }
    }
  S_UNLOCK_COMMON_RESOURCES ();
  }

/************************************************************************/
/*			_irAllocDataValueControl			*/
/************************************************************************/

static ST_VOID _irAllocDataValueControl (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl)
  {
IR_DATA_VALUE_CTRL *irDataValue;
RPL_MSG_HDR   *msg;
ST_INT msgSize;
ST_INT i;

  msgSize = sizeof (RPL_MSG_HDR) + sizeof (RPL_DATA_VALUE_REQ) + 
	      (irDvListCtrl->numDv  * sizeof (RPL_DV_VALUE));
  irDvListCtrl->msgSize = msgSize;
  for (i = 0; i < irDvListCtrl->numDataValueCtrl; ++i)
    {
    irDataValue = chk_calloc (1, sizeof (IR_DATA_VALUE_CTRL)); 
    msg = chk_calloc (1, msgSize);
    irDataValue->msg = msg;
    irDataValue->rplValueReq = (RPL_DATA_VALUE_REQ *) (msg + 1);
    irDataValue->rplValueReq->ccType = irDvListCtrl->ccType;

    if (irDataValue->rplValueReq->ccType == RplCcTypeRemoteCc)
      strcpy (irDataValue->rplValueReq->remoteName, irDvListCtrl->mi_remote->name);

    list_add_last (&irDvListCtrl->listHead, irDataValue);
    }
  irDvListCtrl->active   = irDvListCtrl->listHead;
  irDvListCtrl->nextSend = irDvListCtrl->listHead;
  }
  
/************************************************************************/

static ST_VOID  _irFreeDataValueLists (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl)
  {
IR_DATA_VALUE_CTRL *irDataValue;

  irDataValue = irDvListCtrl->listHead;
  while (irDataValue)
    {
    list_unlink ((ST_VOID **) &irDvListCtrl->listHead, irDataValue);
    chk_free (irDataValue->msg); 
    chk_free (irDataValue); 
    irDataValue = irDvListCtrl->listHead;
    }
  chk_free (irDvListCtrl); 
  }

/************************************************************************/
/************************************************************************/
/*			irGetStatus					*/
/************************************************************************/

ST_VOID irGetStatus (ICCP_REDUNDANCY_CTRL **irCtrlOut)
  {
  *irCtrlOut = _irCtrl;
  }

/************************************************************************/
/*			irService					*/
/************************************************************************/

ST_RET irService ()
  {
ST_DOUBLE currTime;
RPL_STATE_INFO *rplState;
ST_DOUBLE hbTime;
ST_RET rc;

  rplState = _irCtrl->rplState;
  currTime = sGetMsTime ();
  _irProcAllRxRplMsgs ();

  switch (_irCtrl->irState)
    {
    case IrStatePrimaryStartup:
    case IrStateSecondaryStartup:

    /* We are waiting for the startup sequence, check for timeout */
      if (currTime > _irCtrl->startupTimeOut)
        {
        IR_LOG_NERR0 ("Heartbeat startup timeout"); 
        _irHandleStartupTimeout ();
        }
    break;

    case IrStateActiveRplNotOk:
    case IrStateActiveRplConnStarting:
    case IrStateActiveManToStandby:
    case IrStateStandbyRplNotOk:
    break;


    case IrStateStandbyRplConnStarting:
    break;

    case IrStateStandbyDetectedHbFailure:
      if (currTime >= _irCtrl->hbSwitchTimeOutTime)
        {	/* time to switch to active */
        IR_LOG_FLOW0 ("No response to SetMode due to HB failure, entering Active Mode");
        rc =  _irCalluIrSetMode (RplModeActive, IrStateActiveRplOk);
	}
    break;

    case IrStateStandbyManToActive:
      if (currTime >= _irCtrl->switchToActiveTime)
        {	/* time to switch to active */
        IR_LOG_FLOW0 ("Entering Active Mode");
        rc =  _irCalluIrSetMode (RplModeActive, IrStateActiveRplOk);
	}
    break;

    case IrStateActiveRplOk:
    /* OK, everything is in a nice steady state condition 		*/
    /* Let's see if it is time to send a heartbeat 			*/
      if (currTime >= _irCtrl->heartbeatTransmitTime)
        _irSendHeartbeat ();

    /* Now check for heartbeat timeout */
      if (currTime >= _irCtrl->heartbeatExpireTime)
        {
        IR_LOG_NERR0 ("Heartbeat timeout detected; in Active mode, no action required"); 
        hbTime = _irCtrl->partnerRplState.rplCfg.heartbeatTxPeriod * 2;
        if (hbTime <= 0.0)
          hbTime = 5000.0;	/* just prevent chatter */
        _irCtrl->heartbeatExpireTime = currTime + hbTime;
	}		     

    /* Now see if it is time to send data value data */
      if (_irCtrl->rplCfg.dataSyncMode != RplDsModeNone)
        _irServiceDataValues ();

    /* OK, time to check our connect timeout */
      if (rplState->rplCfg.linkConnectTimeoutEnable)
        {
        if (rplState->activeLinks == 0)
          {
  	  if (currTime > _irCtrl->linkConnectTimeoutTime)
  	    {
            IR_LOG_FLOW0 ("Link connect timeout, telling user");
            uIrLinkConnectTimeout ();

          /* reset the link connect timeout */
      	    _irCtrl->linkConnectTimeoutTime = 
  		currTime + rplState->rplCfg.linkConnectTimeout;
  	    }
  	  }
        else    /* Some links are active */
          {
    	  _irCtrl->linkConnectTimeoutTime = 
  		currTime + rplState->rplCfg.linkConnectTimeout;
          }
        }
    break;

    case IrStateStandbyRplOk:

    /* OK, everything is in a nice steady state condition 		*/
    /* Let's see if it is time to send a heartbeat 			*/
      if (currTime >= _irCtrl->heartbeatTransmitTime)
        _irSendHeartbeat ();

    /* Now check for heartbeat timeout */
      if (currTime >= _irCtrl->heartbeatExpireTime)
        _irProcessHbTimeout ();
    break;

    default:
    break;
    }
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			irEnd						*/
/************************************************************************/

ST_RET  irEnd ()
  {
ST_RET rc;

  rc = irModeSelect (RplModeActive);
  return (rc);
  }


ST_RET  irTerminate (ST_DOUBLE timeout)
  {
ST_RET rc;
ST_DOUBLE currTime;
ST_DOUBLE startTime;

/* First try to set our mode to Standby */
  rc = irModeSelect (RplModeActive);
  if (rc == SD_SUCCESS)
    {
    startTime = sGetMsTime ();
    while (SD_TRUE)
      {
      currTime = sGetMsTime ();
      if (currTime - startTime > timeout)
        {
	break;
	}
      if (_irCtrl->rplState->rplMode != RplModeActive ||
          _irCtrl->rplState->rplState != RplStateOkConnected)
        {
        break;
        }
      irService ();
      }
    }
  rplEnd ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			irModeSelect					*/
/************************************************************************/

ST_RET  irModeSelect (enum RPL_MODE mode)
  {
RPL_STATE_INFO *rplState;
RPL_MSG_HDR   *msg;
RPL_SET_MODE_REQ *smReq;
ST_RET rc;


  if (_irCtrl->rplState->modeLocked == SD_TRUE)
    {
    IR_LOG_NERR0 ("Mode locked, cannot switch mode"); 
    return (SD_FAILURE);
    }

  IR_LOG_FLOW1 ("Sending mode select: %s", RPL_MODE_TEXT(mode));

  rplState = _irCtrl->rplState;
  if (rplState->rplState != RplStateOkConnected)
    {
    IR_LOG_CFLOW0 ("  RPL connection not OK");
    return (SD_FAILURE);
    }

  msg = chk_calloc (1, sizeof (RPL_MSG_HDR) + sizeof (RPL_SET_MODE_REQ));
  msg->magicNumber = RPL_MAGIC_NUMBER;
  msg->op = RplOpSetModeRequest;
  msg->bufferSize = sizeof (RPL_SET_MODE_REQ);

  smReq = (RPL_SET_MODE_REQ *) (msg + 1);
  smReq->mode = mode;
  rc = rplSendMsg  (msg);
  if (rc != SD_SUCCESS)
    {
    IR_LOG_NERR1 ("Send mode select failed: %d", rc);
    chk_free (msg);
    }
  return (rc);
  }

/************************************************************************/
/*			irModeLock					*/
/************************************************************************/

ST_RET  irModeLock (ST_VOID)
  {
  IR_LOG_FLOW0 ("Locking mode");
  _irCtrl->rplState->modeLocked = SD_TRUE;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			irModeUnlock					*/
/************************************************************************/

ST_RET  irModeUnlock (ST_VOID)
  {
  IR_LOG_FLOW0 ("Unlocking mode");
  _irCtrl->rplState->modeLocked = SD_FALSE;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			irDataValue					*/
/************************************************************************/

ST_VOID irLocalDvValue (MIS_DV_REF dvRef, 
			ST_INT type_id, ST_VOID *data)
  {
MIS_DV *misDv;

  if (_irCtrl->rplCfg.dataSyncMode == RplDsModeNone || 
      _irCtrl->syncActive != SD_TRUE)
    {
    return;
    }

  misDv = MIS_DV_REF_TO_DV(dvRef);
  IR_LOG_DATA1 ("User is updating local DV: %s", misDv->dv_name);
  _irAddDvToDvList (_irCtrl->_irDvListCtrl, dvRef, type_id, data);
  }


/************************************************************************/
/************************************************************************/

ST_VOID irRemoteDvValue (MI_REMOTE *mi_remote, ST_INT idx,
			ST_INT type_id, ST_VOID *data)
  {
  if (_irCtrl->rplCfg.dataSyncMode != RplDsModeLocalAndRemote ||
      mi_remote->syncActive != SD_TRUE)
    {
    return;
    }

  IR_LOG_DATA1("User is updating remote (%s) DV", mi_remote->name);

  _irAddDvToDvList (mi_remote->irDvListCtrl, (ST_UINT32) idx, 
  		     type_id, data);
  }


/************************************************************************/
/*			_irAddDvToDvList				*/
/************************************************************************/


static ST_VOID _irAddDvToDvList (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl, 
			         ST_UINT32 ref, 
			         ST_INT type_id, ST_VOID *data)
  {
IR_DATA_VALUE_CTRL *irDataValue;
IR_DATA_VALUE_CTRL *nextIrDataValue;
RPL_DATA_VALUE_REQ *rplValueReq;
RPL_DV_VALUE *dvValue;
MI_REAL			*r;
MI_STATE		*s;
MI_DISCRETE		*d;
MI_REAL_Q		*rq;
MI_STATE_Q		*sq;
MI_DISCRETE_Q		*dq;
MI_REAL_Q_TIMETAG	*rqt;
MI_STATE_Q_TIMETAG	*sqt;
MI_DISCRETE_Q_TIMETAG	*dqt;
MI_REAL_EXTENDED	*re;
MI_STATE_EXTENDED	*se;
MI_DISCRETE_EXTENDED	*de;


/* We only do this when we are active and the CRC has been received 	*/
  if (_irCtrl->rplState->rplMode != RplModeActive ||
      _irCtrl->crcReceived != SD_TRUE)
    {
    IR_LOG_CDATA0 ("RPL not active or CRC not received, discarding");
    return;
    }

/* Make sure the partner knows what to do with this data */
  if (ref == IR_REF_INVALID)
    return;

/* We need to protect the data value lists */
  S_LOCK_COMMON_RESOURCES ();

/* get a pointer to our IR Data Value Control structure */
  irDataValue = irDvListCtrl->active;
  rplValueReq = irDataValue->rplValueReq;

  dvValue = (RPL_DV_VALUE *) (rplValueReq + 1);
  dvValue += rplValueReq->numDv;

  dvValue->ref = ref;
  dvValue->type_id = type_id;

  switch (type_id)
    {
    case MI_TYPEID_REAL:
      r = data;
      dvValue->value.r = *r;
    break;
  
    case MI_TYPEID_STATE:
      s = data;
      dvValue->value.s = *s;
    break;
  
    case MI_TYPEID_DISCRETE:
      d = data;
      dvValue->value.d = *d;
    break;
  
    case MI_TYPEID_REAL_Q:
      rq = data;
      dvValue->value.rq = *rq;
    break;
  
    case MI_TYPEID_STATE_Q:
      sq = data;
      dvValue->value.sq = *sq;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      dq = data;
      dvValue->value.dq = *dq;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      rqt = data;
      dvValue->value.rqt = *rqt;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      sqt = data;
      dvValue->value.sqt = *sqt;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      dqt = data;
      dvValue->value.dqt = *dqt;
    break;

    case MI_TYPEID_REAL_EXTENDED:
      re = data;
      dvValue->value.re = *re;
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      se = data;
      dvValue->value.se = *se;
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      de = data;
      dvValue->value.de = *de;
    break;
  
    default:
      IR_LOG_ERR1 ("Invalid data value type_id: %d", type_id);
    break;
    }

/* If this is the first, set the send time */
  if (rplValueReq->numDv == 0)
    irDataValue->nextSendTime = sGetMsTime () + _irDataValueBufferTime;

/* Check to see if this is full */
  if (++rplValueReq->numDv >= irDvListCtrl->numDv)
    {
    irDataValue->nextSendTime = 0;	/* Let this one go right away */

  /* This one is full, move to the next */
    IR_LOG_CDATA0 ("Data value buffer full, move to next");
    nextIrDataValue = list_get_next (irDvListCtrl->listHead, irDataValue);
    if (nextIrDataValue == NULL)
      nextIrDataValue = irDvListCtrl->listHead;

    if (nextIrDataValue == irDvListCtrl->nextSend)
      {
      IR_LOG_NERR0 ("Warning: Data value transmit saturated, discarding data");
      }
    irDvListCtrl->active = nextIrDataValue;
    rplValueReq = nextIrDataValue->rplValueReq;
    rplValueReq->numDv = 0;
    rplValueReq->numDev = 0;
    nextIrDataValue->nextSendTime = sGetMsTime () + _irDataValueBufferTime;
    }

  S_UNLOCK_COMMON_RESOURCES ();
  }

/************************************************************************/
/*			_irServiceDataValues				*/
/************************************************************************/

#define _IR_DV_SERVICE_LOCAL	1
#define _IR_DV_SERVICE_REMOTE	2

static ST_VOID _irServiceDataValues ()
  {
static ST_INT startService = _IR_DV_SERVICE_LOCAL;
static MI_REMOTE *startRemote;
MI_REMOTE *mi_remote;
MI_REMOTE *firstRemote;
ST_BOOLEAN couldNotSend;

  if (_irCtrl->rplCfg.dataSyncMode == RplDsModeNone)
    return;

  if (startService == _IR_DV_SERVICE_LOCAL)
    {
    couldNotSend = _irServiceDataValuesList (_irCtrl->_irDvListCtrl, 
                                             &_irCtrl->dataSyncCount);
    if (couldNotSend)
      return;		/* Can't send local data, stop here	*/

    if (_irCtrl->rplCfg.dataSyncMode != RplDsModeLocalAndRemote)
      return;		/* Not doing remotes */

  /* We are syncing remotes too, set them as next service */
    startService = _IR_DV_SERVICE_REMOTE;
    startRemote = mi_remote_list;
    }

  /* We need to service remotes */
  firstRemote = startRemote;	/* Remember where we started */
  while (SD_TRUE)
    {
    mi_remote = startRemote;
    if (mi_remote->mic_vcc != NULL)
      {
      couldNotSend = _irServiceDataValuesList (mi_remote->irDvListCtrl, 
        			  &mi_remote->dataSyncCount);
      if (couldNotSend)
        break;	/* Can't send this remotes data, stop here	*/
      }

    /* OK, we serviced the last OK, see which to do next 		*/
    startRemote = (MI_REMOTE *)mi_remote->l.next;
    if (startRemote == mi_remote) 
      {
      startService = _IR_DV_SERVICE_LOCAL;
      break;		/* Only one on the list, done		*/
      }
    if (startRemote == firstRemote)
      {
      startService = _IR_DV_SERVICE_LOCAL;
      break;		/* Already did this one, done 		*/
      }
    }
  }
  

/************************************************************************/
/*			_irServiceDataValuesList				*/
/************************************************************************/

static ST_BOOLEAN _irServiceDataValuesList (IR_DATA_VALUE_LIST_CTRL *irDvListCtrl,
					 ST_LONG *dataSyncCountIo)
  {
RPL_STATE_INFO *rplState;
IR_DATA_VALUE_CTRL *irDataValue;
ST_DOUBLE currTime;
RPL_MSG_HDR   *msg;
RPL_MSG_HDR   *newMsg;
RPL_DATA_VALUE_REQ *rplValueReq;
ST_INT totalValues;
ST_BOOLEAN couldNotSend;
ST_BOOLEAN okToSend;
ST_RET rc;

/* We need to protect the data value lists */
  S_LOCK_COMMON_RESOURCES ();

  currTime = sGetMsTime ();
  irDataValue = irDvListCtrl->nextSend;
  couldNotSend = SD_FALSE;

  rplState = _irCtrl->rplState;
  while (SD_TRUE)
    {
    rplValueReq = irDataValue->rplValueReq;
    totalValues = rplValueReq->numDv + rplValueReq->numDev;
    if (totalValues > 0)
      {
      if (currTime >= irDataValue->nextSendTime) 
        {
        okToSend = _irOkToSendData ();
        if (!okToSend)
	  {
          couldNotSend = SD_TRUE;
	  break;
	  }

        IR_LOG_FLOW1 ("Sending data value request, %d values, %d pending", totalValues);
        msg = irDataValue->msg;
        msg->magicNumber = RPL_MAGIC_NUMBER;
        msg->op = RplOpDataValuesRequest;
        msg->bufferSize = sizeof (RPL_DATA_VALUE_REQ) +
	      (totalValues * sizeof (RPL_DV_VALUE));
              
        rc = rplSendMsg  (msg);
        if (rc == SD_SUCCESS)
	  {
       /* OK, we have passed this msg to RPL, it will free it 		*/
          newMsg = chk_calloc (1, irDvListCtrl->msgSize);
          irDataValue->msg = newMsg;
          irDataValue->rplValueReq = (RPL_DATA_VALUE_REQ *) (newMsg + 1);
          irDataValue->rplValueReq->ccType = irDvListCtrl->ccType;
          if (irDvListCtrl->ccType == RplCcTypeRemoteCc)
            strcpy (irDataValue->rplValueReq->remoteName, 
		    irDvListCtrl->mi_remote->name);
          ++rplState->dataValueReqPend;
          _irCtrl->dataSyncRateCount += totalValues;
	  *dataSyncCountIo += totalValues;
	  }
	else
          {
          IR_LOG_NERR1 ("Data value request send failed: %d", rc);
          rplValueReq->numDv = 0;
          rplValueReq->numDev = 0;
          }

      /* If we just tried to send the active one, we are done */
        if (irDataValue == irDvListCtrl->active)
          break;

      /* OK, it was not the active, see if we should send another */
        irDataValue = list_get_next (irDvListCtrl->listHead, irDataValue);
	if (irDataValue == NULL)
          irDataValue = irDvListCtrl->listHead;

        irDvListCtrl->nextSend = irDataValue;
        } /* not time to send yet */
      else
        break;
      }
    else	/* No data values in this one */
      break;
    }

  S_UNLOCK_COMMON_RESOURCES ();
  return  (couldNotSend);
  }


/************************************************************************/
/*			_irOkToSendData					*/
/************************************************************************/

static ST_BOOLEAN _irOkToSendData ()
  {
ST_INT     maxDataValueReqPend;
RPL_STATE_INFO *rplState;

  rplState = _irCtrl->rplState;
  maxDataValueReqPend = rplState->rplCfg.maxDataValueReqPend;
  if (maxDataValueReqPend != 0 &&  
      rplState->dataValueReqPend >= maxDataValueReqPend)
    {
    return (SD_FALSE);
    }
  return (SD_TRUE);
  }


/************************************************************************/
/*			_irFlushAllDvLists				*/
/************************************************************************/

static ST_VOID _irFlushAllDvLists () 
  {
IR_DATA_VALUE_LIST_CTRL *irDvListCtrl;
MI_REMOTE *mi_remote;

  S_LOCK_COMMON_RESOURCES ();

/* Local DV lists first */
  irDvListCtrl = _irCtrl->_irDvListCtrl;
  if (irDvListCtrl == NULL)
    return;

  irDvListCtrl->active   = irDvListCtrl->listHead;
  irDvListCtrl->nextSend = irDvListCtrl->listHead;
  irDvListCtrl->listHead->rplValueReq->numDv = 0;
  irDvListCtrl->listHead->rplValueReq->numDev = 0;

/* Now remote ones */
  if (_irCtrl->rplCfg.dataSyncMode == RplDsModeLocalAndRemote)
    {
    mi_remote = mi_remote_list;
    while (mi_remote)
      {
      irDvListCtrl = mi_remote->irDvListCtrl;
      irDvListCtrl->active   = irDvListCtrl->listHead;
      irDvListCtrl->nextSend = irDvListCtrl->listHead;
      irDvListCtrl->listHead->rplValueReq->numDv = 0;
      irDvListCtrl->listHead->rplValueReq->numDev = 0;

      mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
      }
    }
  S_UNLOCK_COMMON_RESOURCES ();
  }

/************************************************************************/
/*			_irProcessHbTimeout				*/
/************************************************************************/

static ST_VOID _irProcessHbTimeout ()
  {
RPL_STATE_INFO *rplState;
ST_RET rc;

  IR_LOG_NERR0 ("Heartbeat timeout detected, initiate switchover to Active");
  if (_irCtrl->rplState->modeLocked == SD_TRUE)
    {
    IR_LOG_CNERR0 ("Mode locked, cannot switch mode"); 
    return;
    }

/* If the RPL link is not OK, then we just switch to active */
  rplState = _irCtrl->rplState;
  switch (rplState->rplState)
    {
    case RplStateOkConnected:
  /* OK, we need to send a SetMode message to partner, just in case ...*/
      IR_LOG_CNERR0 ("  RPL connected, attempting to contact partner");
      rc = irModeSelect (RplModeStandby);
      if (rc == SD_SUCCESS)
        {
        IR_LOG_CNERR0 ("  Sent mode switch to partner, waiting for response");
       _irCtrl->irState = IrStateStandbyDetectedHbFailure;
       _irCtrl->hbSwitchTimeOutTime = 
       		sGetMsTime () + _irCtrl->rplCfg.rplRequestTimeout2;
	}
      else	/* that's it, we are going to switch to active now! */
        {
        IR_LOG_CNERR0 ("  Mode switch message failed, switching to Active");
        rc =  _irCalluIrSetMode (RplModeActive, IrStateActiveRplOk);
	}
    break;

    case RplStateOkDisabled:
    case RplStateOkWaitingConnect:
    case RplStateOkConnectPending:
    case RplStateOkBackoff:
    case RplStateOkConnecting:
    case RplStateOkDisconnecting:
    case RplStateOkIdle:
    /* Not really sure here what IR state should go to... */

    case RplStateNotOk:
    default:
      IR_LOG_CNERR0 ("  RPL inactive, switching to Active");
      rc =  _irCalluIrSetMode (RplModeActive, IrStateActiveRplNotOk);
    break;
    }
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			uRplActive					*/
/************************************************************************/

ST_VOID uRplActive ()
  {
RPL_STATE_INFO *rplState;
ST_DOUBLE currTime;

  IR_LOG_FLOW0 ("RPL conection is active");

  rplState = _irCtrl->rplState;
  _irSendHeartbeat ();
  _irSendCrc (SD_FALSE);
  currTime = sGetMsTime ();
  _irCtrl->heartbeatExpireTime = currTime + rplState->rplCfg.rplStartupTime;

/* We do this to allow data sync to take place, otherwise the active 	*/
/* node may switch modes before the standby can receive data		*/
  _irCtrl->linkConnectTimeoutTime = currTime + rplState->rplCfg.linkConnectTimeout;

  _irCtrl->rplState->dataValueReqPend = 0;
  _irCtrl->firstHbReceived = SD_FALSE;

/* Stop data sync */
  _irCtrl->crcReceived = SD_FALSE;

/* Flush the data value lists */
  _irFlushAllDvLists ();

  switch (_irCtrl->irState)
    {
    case IrStatePrimaryStartup:
    case IrStateSecondaryStartup:
    break;

    case IrStateActiveRplNotOk:
      _irCtrl->irState = IrStateActiveRplOk;
    break;

    case IrStateActiveRplConnStarting:
    case IrStateActiveManToStandby:
    break;

    case IrStateStandbyRplNotOk:
      _irCtrl->irState = IrStateStandbyRplOk;
    break;

    case IrStateStandbyRplConnStarting:
    case IrStateStandbyDetectedHbFailure:
    break;

    case IrStateStandbyManToActive:
    break;

    case IrStateActiveRplOk:
    case IrStateStandbyRplOk:
    break;

    default:
    break;
    }
  }

/************************************************************************/
/*			uRplInactive					*/
/************************************************************************/

ST_VOID uRplInactive ()
  {
  IR_LOG_FLOW0 ("RPL conection is inactive");
  }


/************************************************************************/
/*			uRplMsgRcvd					*/
/************************************************************************/

ST_VOID uRplMsgRcvd  (RPL_MSG_HDR *msg)
  {
IR_RPL_MSG_RCV_CTRL *rxMsg;

  rxMsg = chk_calloc (1, sizeof (IR_RPL_MSG_RCV_CTRL));
  rxMsg->msg = msg;
  list_add_last((ST_VOID **) &_irRplRxMsgList, rxMsg);
  uIrRplMsgRcvd ();
  }


/************************************************************************/
/*			_irProcAllRxRplMsgs				*/
/************************************************************************/

static ST_VOID _irProcAllRxRplMsgs ()
  {
IR_RPL_MSG_RCV_CTRL *rxMsg;

  while (SD_TRUE)
    {
    rxMsg = _irRplRxMsgList;
    if (rxMsg == NULL)
      break;
    list_unlink (&_irRplRxMsgList, rxMsg);
    _irProcRplRxMsg (rxMsg->msg);
    chk_free (rxMsg);
    }
  }

/************************************************************************/
/*			_irProcRplRxMsg					*/
/************************************************************************/

static ST_VOID _irProcRplRxMsg (RPL_MSG_HDR *msg)
  {
  switch (msg->op)
    {
    case RplOpHeartBeat:
      _irProcHeartbeat (msg);
    break;

    case RplOpCrc:
      _irProcCrc (msg);
    break;

    case RplOpAssociationState:
      _irProcAssocState (msg);
    break;

    case RplOpSetModeRequest:
      _irProcSetModeReq (msg);
    break;

    case RplOpSetModeResponse:
      _irProcSetModeResp (msg);
    break;

    case RplOpDataSyncRequest:
      _irProcDataSyncReq (msg);
    break;

    case RplOpDataSyncResponse:
      _irProcDataSyncResp (msg);
    break;

    case RplOpDataValuesRequest:
      _irProcDataValuesReq (msg);
    break;

    case RplOpDataValuesResponse:
      _irProcDataValuesResp (msg);
    break;

    case RplOpDataLinkageRequest:   /* Not implemented */
    case RplOpDataLinkageResponse:	
    default:
      IR_LOG_NERR1 ("ERROR: Unknown opcode: %d", msg->op);
    break;
    }

  chk_free (msg);
  }


/************************************************************************/
/************************************************************************/
/*			_irProcHeartbeat				*/
/************************************************************************/

static ST_VOID _irProcHeartbeat (RPL_MSG_HDR *msg)
  {
RPL_HEARTBEAT *hb;
ST_DOUBLE currTime;
ST_DOUBLE timeSinceLastHb;
ST_RET rc;

  hb = (RPL_HEARTBEAT *) (msg + 1);
  IR_LOG_FLOW1 ("Received Heartheat, holdTime = '%.3fms", hb->holdTime);
  IR_LOG_CFLOW1 ("  Remote mode:      %s", 
  	hb->stateInfo.rplMode == RplModeActive ? "Active" : "Standby");
  IR_LOG_CFLOW1 ("  Remote ready:     %d", hb->stateInfo.ready);
  IR_LOG_CFLOW1 ("  Remote mode lock: %d", hb->stateInfo.modeLocked);

  currTime = sGetMsTime ();

/* Stats maintenance */
  timeSinceLastHb = currTime - _irCtrl->lastHbRxTime;
  if (timeSinceLastHb > _irCtrl->heartBeatHwRxTime)
    _irCtrl->heartBeatHwRxTime = timeSinceLastHb;
    
  _irCtrl->lastHbRxTime = currTime;

/* Ok, get to business */
  _irCtrl->heartbeatExpireTime = currTime + hb->holdTime;
  _irCtrl->partnerRplState = hb->stateInfo;

/* Ok, see if we need to handle the first heartbeat received */
  if (_irCtrl->firstHbReceived == SD_FALSE)
    {
    _irCtrl->firstHbReceived = SD_TRUE;
    _irHandleFirstHeartbeat ();
    }
  else /* Just another heartbeat; make sure we are not both active 	*/
    {
    if (_irCtrl->rplState->rplMode == RplModeActive &&
        _irCtrl->partnerRplState.rplMode == RplModeActive)
      {
      IR_LOG_NERR0 ("Both nodes active");
      if (_irCtrl->rplCfg.role == RplRolePrimary)
        {
        IR_LOG_CNERR0 ("  Switching to Standby");
        rc = _irCalluIrSetMode (RplModeStandby, IrStateStandbyRplOk);
        }
      }
    }
  }

/************************************************************************/
/*			_irHandleFirstHeartbeat				*/
/************************************************************************/

static ST_VOID _irHandleFirstHeartbeat ()
  {
RPL_STATE_INFO *rplState;
ST_BOOLEAN enterActive;
ST_BOOLEAN enterStandby;
ST_RET rc;

  rplState = _irCtrl->rplState;
  enterActive = SD_FALSE;
  enterStandby = SD_FALSE;

  switch (_irCtrl->irState)
    {
    case IrStatePrimaryStartup:
      if (_irCtrl->partnerRplState.rplMode != RplModeActive)
        enterActive = SD_TRUE;
      else
        enterStandby = SD_TRUE;
    break;

    case IrStateSecondaryStartup:
      if (_irCtrl->partnerRplState.rplMode != RplModeActive &&
          _irCtrl->partnerRplState.rplMode != RplModeStartup)
        {
        enterActive = SD_TRUE;
        }
      else
        enterStandby = SD_TRUE;
    break;

    case IrStateActiveRplOk:
      if (_irCtrl->partnerRplState.rplMode == RplModeActive)
        { /* We are both active, secondary to go to standby */
        IR_LOG_NERR0 ("Both nodes are Active");
        if (_irCtrl->rplCfg.role == RplRoleSecondary)
	  {
          IR_LOG_CNERR0 ("  Entering Standby");
          enterStandby = SD_TRUE;	
  	  }
	}
    break;

    case IrStateStandbyRplOk:
      if (_irCtrl->partnerRplState.rplMode == RplModeStandby)
        { /* We are both standby, primary to go to active */
        IR_LOG_NERR0 ("Both nodes are Standby");
        if (_irCtrl->rplCfg.role == RplRolePrimary)
	  {
          IR_LOG_CNERR0 ("  Entering Active");
          enterActive = SD_TRUE;	
  	  }
	}
    break;

    case IrStateStandbyManToActive:
    case IrStateActiveManToStandby:
    case IrStateStandbyDetectedHbFailure:
    case IrStateStandbyRplConnStarting:
    case IrStateActiveRplConnStarting:
    case IrStateActiveRplNotOk:
    case IrStateStandbyRplNotOk:
    default:
      IR_LOG_ERR1 ("First heartbeat bad irState:%d",_irCtrl->irState);
    break;
    }

  if (enterActive == SD_TRUE)  
    {
    IR_LOG_CFLOW0 ("Entering Active Mode");
    rc =  _irCalluIrSetMode (RplModeActive, IrStateActiveRplOk);
    }
  if (enterStandby == SD_TRUE)  
    {
    IR_LOG_CFLOW0 ("Entering Standby Mode");
    rc =  _irCalluIrSetMode (RplModeStandby, IrStateStandbyRplOk);
    }

/* Send the new mode to the partner */
  _irSendHeartbeat ();	

/* If we are now in active mode, send assoc states 			*/
  if (_irCtrl->rplState->rplMode == RplModeActive)
    _irSendAllAssocStates ();
  }


/************************************************************************/
/*			_irProcCrc					*/
/************************************************************************/

static ST_VOID _irProcCrc (RPL_MSG_HDR *msg)
  {
RPL_CRC_REQ  *crcReq;
RPL_REMOTE_CRC *remCrc;
enum RPL_MODE rplMode;
MI_REMOTE *mi_remote;
ST_BOOLEAN matched;
ST_BOOLEAN matchedNew;
ST_INT i;

  IR_LOG_FLOW0 ("Received CRC report");
  crcReq = (RPL_CRC_REQ *) (msg + 1);
  _irCtrl->crcReceived = SD_TRUE;

  if (crcReq->sendCrcBack == SD_TRUE)
    _irSendCrc (SD_FALSE);

/* See if we care  ... */
  if (_irCtrl->rplCfg.dataSyncMode == RplDsModeNone)
    {		
    IR_LOG_CFLOW0 ("  Not doing any data sync, ignore");
    return;
    }

  rplMode = _irCtrl->rplState->rplMode;

/* Look over the local object CRC */
  _irCompareCrc (_irCtrl->localCfgCrc,crcReq->localCfgCrc,
		 &_irCtrl->partnerLocalCfgCrc,
		 &matched, &matchedNew, "Local");
  if (matched)
    {
    _irCtrl->syncActive = SD_TRUE;
    if (matchedNew)
      {
      _irSetLocalLinkages (SD_TRUE);
      if (rplMode == RplModeStandby)
        {
        IR_LOG_CFLOW0 ("  Sending data sync request, Local");
        _irSendDataSyncReq (RplDsReqTypeLocal, NULL);
	}
      }
    }
  else
    {
    _irCtrl->syncActive = SD_FALSE;
    _irSetLocalLinkages (SD_FALSE);
    }

/* See if we are also doing remotes */
  if (_irCtrl->rplCfg.dataSyncMode == RplDsModeLocalAndRemote)
    {
  /* OK, let's look at the remote CRC's */
    remCrc = (RPL_REMOTE_CRC *) (crcReq + 1);
  
    mi_remote = mi_remote_list;
    while (mi_remote)
      {
    /* Find the matching remote in the CRC request */
      for (i = 0; i < crcReq->numRemotes; ++i)
        {
        if (!strcmp (mi_remote->name, remCrc[i].name))
          {
          _irCompareCrc (mi_remote->cfgCrc, remCrc[i].cfgCrc,
        		 &mi_remote->partnerCfgCrc,
        		 &matched, &matchedNew, mi_remote->name);
          if (matched)
            {
	    mi_remote->syncActive = SD_TRUE;
            if (matchedNew)
              {
              _irSetRemoteLinkages (mi_remote, SD_TRUE);
              if (rplMode == RplModeStandby)
	        {
                IR_LOG_CFLOW1 ("  Sending data sync request, Remote %s", mi_remote->name);
                _irSendDataSyncReq (RplDsReqTypeRemote, mi_remote);
                }
              }
            }
          else
	    {
	    mi_remote->syncActive = SD_FALSE;
            _irSetRemoteLinkages (mi_remote, SD_FALSE);
	    }
  	  break;
  	  }
        }
      if (i >= crcReq->numRemotes)
        {
        IR_LOG_CFLOW1 ("  Partner does not have Remote %s", mi_remote->name);
        mi_remote->partnerCfgCrc = 0;
        _irSetRemoteLinkages (mi_remote, SD_FALSE);
        }
  
      mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
      }
    }
  else 
    {
    IR_LOG_CFLOW0 ("  Not doing remote data sync, ignore");
    }
  }


/************************************************************************/
/*			_irCompareCrc					*/
/************************************************************************/

static ST_VOID _irCompareCrc (ST_INT32 ourCrc, ST_INT32 partCrc, 
		              ST_INT32 *partStoreCrcIo, 
		              ST_BOOLEAN *matchedOut,
		              ST_BOOLEAN *matchedNewOut, ST_CHAR *logStr)
  {
ST_INT32 prevPartCrc;
ST_BOOLEAN matched;
ST_BOOLEAN matchedNew;

/* See if we match */
  if (partCrc == ourCrc)
    {
    matched = SD_TRUE;
  
  /* Now check to see if we previously had a match on this scope 	*/
    prevPartCrc = *partStoreCrcIo; 
    if (prevPartCrc == 0 || partCrc != prevPartCrc)
      {
      IR_LOG_CFLOW3 ("  CRC matched new: %s (this %08lx, partner %08lx)",
      					 logStr, ourCrc, partCrc);
      matchedNew = SD_TRUE;
      }
    else
      {
      IR_LOG_CFLOW3 ("  CRC matched previously: %s (this %08lx, partner %08lx)",
      					 logStr, ourCrc, partCrc);
      matchedNew = SD_FALSE;
      }
    *partStoreCrcIo = partCrc; /* Save for later compare */
    }
  else
    {
    IR_LOG_CFLOW3 ("  CRC mismatch: %s (this %08lx, partner %08lx)",
      					 logStr, ourCrc, partCrc);
    matched = SD_FALSE;
    matchedNew = SD_FALSE;
    *partStoreCrcIo = 0;	
    }

/* Set the output parameters */
  *matchedOut = matched;
  *matchedNewOut = matchedNew;
  }

/************************************************************************/
/*			_irProcAssocState				*/
/************************************************************************/

static ST_VOID _irProcAssocState (RPL_MSG_HDR *msg)
  {
RPL_ASSOC_STATE_REQ *assocStateReq;
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;

  assocStateReq = (RPL_ASSOC_STATE_REQ *) (msg + 1);
  IR_LOG_FLOW2 ("Received Association State for remote '%s':%s",
    assocStateReq->remoteName, 
    assocStateReq->state == MI_ASSOC_STATE_ACTIVE ? "Active" : "Idle");

  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    if (!strcmp (mi_remote->name, assocStateReq->remoteName))
      {
      mi_assoc = mi_remote->mi_assoc_list;
      while (mi_assoc)
        {
	if (!strcmp (mi_assoc->name, assocStateReq->assocName))
	  {
          uIrAssocState (mi_assoc, assocStateReq->state);
          break;
          }
        mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
        }
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

/************************************************************************/
/*			_irProcSetModeReq				*/
/************************************************************************/

static ST_VOID _irProcSetModeReq (RPL_MSG_HDR *msg)
  {
RPL_SET_MODE_REQ  *smReq;
RPL_MSG_HDR *respMsg;
RPL_SET_MODE_RESP *smResp;
enum RPL_RESULT rplResult;
ST_RET rc;
RPL_STATE_INFO *rplState;


  rplState = _irCtrl->rplState;
  smReq = (RPL_SET_MODE_REQ *) (msg + 1);

  IR_LOG_FLOW1 ("Received Set Mode request: %s", 
     smReq->mode == RplModeActive ? "Active" : "Standby");

  rplResult = RplRsltSuccess;	/* We default to success */

  if (_irCtrl->rplState->modeLocked == SD_TRUE &&
      _irCtrl->rplState->rplMode != smReq->mode)
    {
    IR_LOG_NERR0 ("Mode locked, cannot switch mode"); 
    rplResult = RplRsltFailureLocked;
    }
  else if (_irCtrl->rplState->ready != SD_TRUE && 
           smReq->mode == RplModeActive &&
           _irCtrl->rplState->rplMode != RplModeActive)
    {
    IR_LOG_NERR0 ("Not ready, can't switch to active"); 
    rplResult = RplRsltFailureNotReady;
    }
  else if (_irCtrl->rplState->rplMode != smReq->mode) 
    { 	/* OK, ready, not locked, and mode change; ask the user		*/
    rc = uIrModeRequest (smReq->mode);
    if (rc == SD_SUCCESS)
      {
      rplResult = RplRsltSuccess;
      if (smReq->mode == RplModeActive)
        {	/* OK, partner want us active, user is OK with it ... 	*/
         IR_LOG_FLOW0 ("Manual switch to Active starting");
        _irCtrl->irState = IrStateStandbyManToActive;
        _irCtrl->switchToActiveTime = 
              sGetMsTime () + _irCtrl->rplCfg.switchToActiveDelay;
        }		   
      else if (smReq->mode == RplModeStandby)
        {	/* OK, partner want us standby, user is OK with it ... 	*/
        IR_LOG_FLOW0 ("Executing manual switch to Standby");
        rc =  _irCalluIrSetMode (RplModeStandby, IrStateStandbyRplOk);
        }
      else
        {
        IR_LOG_ERR1 ("Manual switch invalid mode: %d", smReq->mode);
        rplResult = RplRsltFailureOther;
        }
      }
    else
      {
      IR_LOG_NERR0 ("Mode switch refused by user");
      rplResult = RplRsltFailureOther;
      }
    }
  else		/* We are are already in the correct mode */
    {
    IR_LOG_CFLOW0 ("  Already is desired mode, no action required");
    rplResult = RplRsltSuccess;
    }

/* Send the Set Mode response */  
  respMsg = chk_calloc (1, sizeof (RPL_MSG_HDR) + sizeof (RPL_SET_MODE_RESP));
  respMsg->magicNumber = RPL_MAGIC_NUMBER;
  respMsg->op = RplOpSetModeResponse;
  respMsg->bufferSize = sizeof (RPL_SET_MODE_RESP);

  smResp = (RPL_SET_MODE_RESP *) (respMsg + 1);
  smResp->result = rplResult;
  smResp->newMode = smReq->mode;
  rc = rplSendMsg  (respMsg);
  if (rc != SD_SUCCESS)
    {
    IR_LOG_NERR1 ("Set Mode response send failed: %d", rc);
    chk_free (respMsg);
    }
  }


/************************************************************************/
/*			_irProcSetModeResp				*/
/************************************************************************/

static ST_VOID _irProcSetModeResp (RPL_MSG_HDR *msg)
  {
RPL_SET_MODE_RESP *smResp;
RPL_STATE_INFO *rplState;
ST_RET rc;

  rplState = _irCtrl->rplState;
  smResp = (RPL_SET_MODE_RESP *) (msg + 1);

  IR_LOG_FLOW1 ("Received Set Mode response: %s",  
	smResp->result == RplRsltSuccess ? "Success" : "Failure");

/* See if we need to tell the user */
  if (_irCtrl->irState != IrStateStandbyDetectedHbFailure)
    uIrSetModeResp (smResp);

  if (smResp->result == RplRsltSuccess)
    {
    if (smResp->newMode == RplModeActive)
      {
      IR_LOG_CFLOW0 ("  Entering Standby"); 
      rc =  _irCalluIrSetMode (RplModeStandby, IrStateStandbyRplOk);
      }
    else	/* We set the partner to Standby */
      {
      IR_LOG_CFLOW0 ("  Starting switch to Active");
      _irCtrl->irState    = IrStateStandbyManToActive;
      _irCtrl->switchToActiveTime = 
              sGetMsTime () + _irCtrl->rplCfg.switchToActiveDelay;
      }
    }
  else
    {
    if (_irCtrl->irState == IrStateStandbyDetectedHbFailure)
      {
      IR_LOG_CFLOW0 ("  Remote refuses HB timeout mode switch to standby");
      _irCtrl->hbSwitchTimeOutTime = 0; /* Time to switch */
      }
    }
  }



/************************************************************************/
/*			_irProcDataSyncReq				*/
/************************************************************************/

static ST_VOID _irProcDataSyncReq (RPL_MSG_HDR *msg)
  {
RPL_DATA_SYNC_REQ *dsReq;
RPL_MSG_HDR   *respMsg;
MI_REMOTE *mi_remote;
RPL_DATA_SYNC_RESP *syncResp;
ST_RET rc;

  dsReq = (RPL_DATA_SYNC_REQ *) (msg + 1);

  rc = SD_FAILURE;	/* Lot's of ways to fail */
  if (_irCtrl->crcReceived == SD_TRUE)
    {
    if (dsReq->syncType == RplDsReqTypeLocal)
      {
      IR_LOG_FLOW0 ("Received data sync request: Local");
      if (_irCtrl->syncActive == SD_TRUE)
        {
        rc = uIrDataSyncReq (dsReq);
	}
      else
        {
        IR_LOG_ERR0 ("Data sync for Local not active");
        }
      }
    else if (dsReq->syncType == RplDsReqTypeRemote)
      {
      IR_LOG_FLOW1 ("Received data sync request: Remote '%s'", dsReq->remoteName);
      mi_remote = mi_find_remote (dsReq->remoteName);
      if (mi_remote != NULL)
        {
        if (mi_remote->syncActive == SD_TRUE)
          rc = uIrDataSyncReq (dsReq);
        else
          {
          IR_LOG_ERR1 ("Data sync request for remote '%s', sync not active", dsReq->remoteName);
          }
        }
      else
        {
        IR_LOG_ERR1 ("Data sync request for remote '%s', remote not found", dsReq->remoteName);
        }
      }
    else if (dsReq->syncType == RplDsReqTypeAll)
      {
      IR_LOG_ERR0 ("Received invalid data sync request: All");
      }
    }
  else  /* CRC not received */
    {
    IR_LOG_ERR0 ("Data sync request before CRC received");
    }


  IR_LOG_FLOW0 ("Sending data sync response");
  respMsg = chk_calloc (1, sizeof (RPL_MSG_HDR) + sizeof (RPL_DATA_SYNC_RESP));
  respMsg->magicNumber = RPL_MAGIC_NUMBER;
  respMsg->op = RplOpDataSyncResponse;
  respMsg->bufferSize = sizeof (RPL_DATA_SYNC_RESP);

  syncResp = (RPL_DATA_SYNC_RESP *) (respMsg + 1);

  if (rc == SD_SUCCESS)
    syncResp->result = RplRsltSuccess;
  else
    syncResp->result = RplRsltFailureOther;

  rc = rplSendMsg  (respMsg);
  if (rc != SD_SUCCESS)
    {
    chk_free (respMsg);
    IR_LOG_NERR1 ("Data sync response send failed: %d", rc);
    }
  }

/************************************************************************/
/*			_irProcDataSyncResp				*/
/************************************************************************/

static ST_VOID _irProcDataSyncResp (RPL_MSG_HDR *msg)
  {
RPL_DATA_SYNC_RESP *dsResp;

  dsResp = (RPL_DATA_SYNC_RESP *) (msg + 1);
  IR_LOG_FLOW1 ("Received data sync response: %d", dsResp->result);
  }

/************************************************************************/
/*			_irProcDataValuesReq				*/
/************************************************************************/

static ST_VOID  _irProcDataValuesReq (RPL_MSG_HDR *msg)
  {
RPL_DATA_VALUE_REQ *duReq;
RPL_DV_VALUE  *dvValue;
RPL_DEV_VALUE *devValue;
RPL_MSG_HDR *respMsg;
RPL_DATA_VALUE_RESP *duResp;
ST_VOID *usrHandle;
ST_INT i;
ST_RET rc;

/* Our partner has sent us some data values, pass to the user		*/
  duReq = (RPL_DATA_VALUE_REQ *) (msg + 1);
  IR_LOG_FLOW2 ("Received data values (%d dv, %d dev)",
				duReq->numDv, duReq->numDev);

  respMsg = chk_calloc (1, sizeof (RPL_MSG_HDR) + sizeof (RPL_DATA_VALUE_RESP));
  respMsg->magicNumber = RPL_MAGIC_NUMBER;
  respMsg->op = RplOpDataValuesResponse;
  respMsg->bufferSize = sizeof (RPL_DATA_VALUE_RESP);

  duResp = (RPL_DATA_VALUE_RESP *) (respMsg + 1);
  duResp->numDv = duReq->numDv;
  duResp->numDev = duReq->numDev;
  _irCtrl->dataSyncRateCount += duReq->numDv;
  _irCtrl->dataSyncRateCount += duReq->numDev;

  rc =  uIrStartDataValue (duReq, duResp, &usrHandle);
  if (rc == SD_SUCCESS)
    {
    if (duReq->ccType == RplCcTypeLocalCc)
      {
      IR_LOG_CDATA0 ("   Local");
      dvValue = (RPL_DV_VALUE  *) (duReq + 1);
      for (i = 0; i < duReq->numDv; ++i, ++dvValue)
        {
        rc =  uIrLocalDvValue (dvValue, usrHandle);
        if (rc == SD_SUCCESS)
          ++duResp->numDvOk;
        }
      devValue = (RPL_DEV_VALUE  *) (dvValue);
      for (i = 0; i < duReq->numDev; ++i, ++devValue)
        {
        if (devValue->ref != IR_REF_INVALID)
          rc =  uIrLocalDevValue (devValue, usrHandle);
        if (rc == SD_SUCCESS)
          ++duResp->numDevOk;
        }
      }
    else	/* remote data value */
      {
      IR_LOG_CDATA1 ("   Remote: %s", duReq->remoteName);
      dvValue = (RPL_DV_VALUE  *) (duReq + 1);
      for (i = 0; i < duReq->numDv; ++i, ++dvValue)
        {
        if (dvValue->ref != IR_REF_INVALID)
          rc =  uIrRemoteDvValue (dvValue, usrHandle);
        if (rc == SD_SUCCESS)
          ++duResp->numDvOk;
        }
      devValue = (RPL_DEV_VALUE  *) (dvValue);
      for (i = 0; i < duReq->numDev; ++i, ++devValue)
        {
        if (devValue->ref != IR_REF_INVALID)
          rc =  uIrRemoteDevValue (devValue, usrHandle);
        if (rc == SD_SUCCESS)
          ++duResp->numDevOk;
        }
      }
    rc =  uIrEndDataValue (duReq, duResp, usrHandle);
    }
  else	/* Start failed */
    {
    IR_LOG_NERR0 ("User start data value returned failure");;
    }
  rc = rplSendMsg  (respMsg);
  if (rc != SD_SUCCESS)
    {
    IR_LOG_NERR1 ("Data Values response send failed: %d", rc);
    chk_free (respMsg);
    }
  }

/************************************************************************/
/*			_irProcDataValuesResp				*/
/************************************************************************/

static ST_VOID _irProcDataValuesResp (RPL_MSG_HDR *msg)
  {
RPL_DATA_VALUE_RESP  	*duResp;

  duResp = (RPL_DATA_VALUE_RESP *) (msg + 1);
  if (duResp->numDv)
    {
    IR_LOG_FLOW2 ("Received data values response- %d DV sent, %d ok", 
    		duResp->numDv, duResp->numDvOk);
    }
  else
    {
    IR_LOG_FLOW2 ("Received data values response- %d DEV sent, %d ok", 
    		duResp->numDev, duResp->numDevOk);
    }

  --_irCtrl->rplState->dataValueReqPend;
  }

/************************************************************************/
/*			_irHandleStartupTimeout				*/
/************************************************************************/

static ST_VOID _irHandleStartupTimeout ()
  {
RPL_STATE_INFO *rplState;
ST_BOOLEAN enterActive;
ST_RET rc;

  rplState = _irCtrl->rplState;
  enterActive = SD_FALSE;

  if (_irCtrl->irState == IrStatePrimaryStartup)
    {  /* We are the primary node, starting up */
    if (rplState->rplState != RplStateOkConnected ||  
        _irCtrl->partnerRplState.rplMode != RplModeActive)
      enterActive = SD_TRUE;
    }
  else if (_irCtrl->irState == IrStateSecondaryStartup)
    {  /* We are the Secondary node, starting up */
    if (rplState->rplState != RplStateOkConnected ||  
        (_irCtrl->partnerRplState.rplMode != RplModeActive &&
        _irCtrl->partnerRplState.rplMode != RplModeStartup))
      {
      enterActive = SD_TRUE;
      }
    }
  else
    {
    IR_LOG_ERR1 ("Invalid state: %d", _irCtrl->irState);
    }

  if (enterActive == SD_TRUE)  
    {
    IR_LOG_FLOW0 ("Entering Active Mode");
    rc =  _irCalluIrSetMode (RplModeActive, 
	       rplState->rplState == RplStateOkConnected ? IrStateActiveRplOk : IrStateActiveRplNotOk);
    }
  else
    {
    IR_LOG_FLOW0 ("Entering Standby Mode");
    rc =  _irCalluIrSetMode (RplModeStandby, 
  		       rplState->rplState == RplStateOkConnected ? 
  	       	       IrStateStandbyRplOk : IrStateStandbyRplNotOk);
    }
  _irSendHeartbeat ();
  }



/************************************************************************/
/************************************************************************/
/*			_irSendHeartbeat				*/
/************************************************************************/

static ST_VOID _irSendHeartbeat  ()
  {
RPL_MSG_HDR   *msg;
RPL_HEARTBEAT *hb;
RPL_STATE_INFO *rplState;
ST_RET rc;

  rplState = _irCtrl->rplState;
  if (rplState->rplState != RplStateOkConnected)
    return;

  IR_LOG_FLOW0 ("Sending heartbeat");
  msg = chk_calloc (1, sizeof (RPL_MSG_HDR) + sizeof (RPL_HEARTBEAT));
  msg->magicNumber = RPL_MAGIC_NUMBER;
  msg->op = RplOpHeartBeat;
  msg->bufferSize = sizeof (RPL_HEARTBEAT);

  hb = (RPL_HEARTBEAT *) (msg + 1);
  hb->stateInfo = *rplState;
  hb->holdTime =  rplState->rplCfg.heartbeatTxPeriod * 2;
  rc = rplSendMsg  (msg);
  if (rc != SD_SUCCESS)
    {
    chk_free (msg);
    IR_LOG_NERR1 ("Heartbeat send failed: %d", rc);
    }

  _irCtrl->heartbeatTransmitTime = 
  		sGetMsTime () + rplState->rplCfg.heartbeatTxPeriod;
  }

/************************************************************************/
/*			_irSendDataSyncReq				*/
/************************************************************************/

static ST_VOID _irSendDataSyncReq (enum RPL_DATA_SYNC_REQ_TYPE syncType, 
				   MI_REMOTE *mi_remote)
  {
RPL_MSG_HDR   *msg;
RPL_DATA_SYNC_REQ *syncReq;
ST_RET rc;

  msg = chk_calloc (1, sizeof (RPL_MSG_HDR) + sizeof (RPL_DATA_SYNC_REQ));
  msg->magicNumber = RPL_MAGIC_NUMBER;
  msg->op = RplOpDataSyncRequest;
  msg->bufferSize = sizeof (RPL_DATA_SYNC_REQ);

  syncReq = (RPL_DATA_SYNC_REQ *) (msg + 1);
  syncReq->syncType = syncType;

  if (syncType == RplDsReqTypeRemote)
    strcpy (syncReq->remoteName, mi_remote->name);

  rc = rplSendMsg  (msg);
  if (rc != SD_SUCCESS)
    {
    chk_free (msg);
    IR_LOG_NERR1 ("Data sync request send failed: %d", rc);
    }
  }

/************************************************************************/
/*			_irSendCrc  				*/
/************************************************************************/

static ST_VOID _irSendCrc (ST_BOOLEAN getpartnerCrc)
  {
RPL_MSG_HDR    *msg;
RPL_CRC_REQ    *crcReq;
RPL_REMOTE_CRC *remCrc;
MI_REMOTE  *mi_remote;
ST_RET rc;
ST_INT bufferSize;
ST_INT numRemotes;

  IR_LOG_FLOW0 ("Sending CRC report");
  numRemotes = list_get_sizeof (mi_remote_list);
  bufferSize = sizeof (RPL_DATA_SYNC_REQ) + (numRemotes * sizeof (RPL_REMOTE_CRC));
  msg = chk_calloc (1, sizeof (RPL_MSG_HDR) + bufferSize);
  msg->magicNumber = RPL_MAGIC_NUMBER;
  msg->op = RplOpCrc;
  msg->bufferSize = bufferSize;

  crcReq = (RPL_CRC_REQ *) (msg + 1);
  crcReq->localCfgCrc = _irLocalCrc ();
  crcReq->numRemotes = numRemotes;
  crcReq->sendCrcBack = getpartnerCrc;

  remCrc = (RPL_REMOTE_CRC *) (crcReq + 1);

  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {
    remCrc->cfgCrc = mi_remote->cfgCrc;
    strcpy (remCrc->name, mi_remote->name); 

    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    ++remCrc;
    }

  rc = rplSendMsg  (msg);
  if (rc != SD_SUCCESS)
    {
    chk_free (msg);
    IR_LOG_NERR1 ("CRC report send failed: %d", rc);
    }
  }

/************************************************************************/
/*			irSendAssocState  				*/
/************************************************************************/

ST_VOID irSendAssocState (MI_ASSOC_CTRL *mi_assoc)
  {
  _irSendAssocState (mi_assoc, mi_assoc->state);
  }


/************************************************************************/
/*			_irSendAssocState				*/
/************************************************************************/

static ST_VOID _irSendAssocState (MI_ASSOC_CTRL *mi_assoc, ST_INT state)
  {
RPL_MSG_HDR         *msg;
RPL_ASSOC_STATE_REQ *assocStateReq;
MI_REMOTE           *mi_remote;
ST_RET rc;

  mi_remote = mi_assoc->mi_remote;

/* Just send active and idle states */
  if (state != MI_ASSOC_STATE_ACTIVE && state != MI_ASSOC_STATE_IDLE)
    {
    IR_LOG_FLOW1 ("Not sending association '%s' state", mi_assoc->name);
    IR_LOG_CFLOW2 ("  Remote '%s':%d", mi_remote->name, state);
    return;
    }

  IR_LOG_FLOW1 ("Sending association '%s' state", mi_assoc->name);
  IR_LOG_CFLOW2 ("  Remote '%s':%s", mi_remote->name, 
    state == MI_ASSOC_STATE_ACTIVE ? "Active" : "Idle");

  msg = chk_calloc (1, sizeof (RPL_MSG_HDR) + sizeof (RPL_ASSOC_STATE_REQ));
  msg->magicNumber = RPL_MAGIC_NUMBER;
  msg->op = RplOpAssociationState;
  msg->bufferSize = sizeof (RPL_ASSOC_STATE_REQ);

  assocStateReq = (RPL_ASSOC_STATE_REQ  *) (msg + 1);
  strcpy (assocStateReq->remoteName, mi_remote->name); 
  strcpy (assocStateReq->assocName, mi_assoc->name); 
  assocStateReq->state = state;

  rc = rplSendMsg  (msg);
  if (rc != SD_SUCCESS)
    {
    chk_free (msg);
    IR_LOG_NERR1 ("Association State send failed: %d", rc);
    }
  }

/************************************************************************/
/* 		_irSendAllAssocStates					*/
/************************************************************************/
static ST_VOID _irSendAllAssocStates ()
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;
ST_INT state;

  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    mi_assoc = mi_remote->mi_assoc_list;
    while (mi_assoc)
      {
      /* treat all non-active states as idle */
      state = mi_assoc->state;
      if (state != MI_ASSOC_STATE_ACTIVE)
        state = MI_ASSOC_STATE_IDLE;

      _irSendAssocState (mi_assoc, state);
      mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }


/************************************************************************/
/************************************************************************/

static ST_RET _irCalluIrSetMode (enum RPL_MODE rplMode, enum IR_STATE irState)
  {
RPL_STATE_INFO *rplState;
ST_DOUBLE currTime;
ST_RET rc;

  currTime = sGetMsTime ();
  rc =  uIrSetMode (rplMode);
  if (rc == SD_SUCCESS)
    {
    _irCtrl->irState = irState;
    rplState = _irCtrl->rplState;
    rplState->rplMode = rplMode;
    if (rplState->rplCfg.linkConnectTimeoutEnable && rplMode == RplModeActive)
      {
      _irCtrl->linkConnectTimeoutTime = 
      	       		currTime + rplState->rplCfg.linkConnectTimeout;
      }
    _irCtrl->dataSyncRateCountStartTime = currTime;
    _irCtrl->dataSyncRateCount = 0;
    _irCtrl->heartBeatHwRxTime = 0;
    _irCtrl->lastHbRxTime = currTime;
    }
  else
    {
    IR_LOG_NERR1 ("Switch to %s refused by user", RPL_MODE_TEXT (rplMode));
    }
  return rc;
  }


/************************************************************************/
/************************************************************************/
/*			_irSetLocalLinkages				*/
/************************************************************************/

static ST_VOID _irSetLocalLinkages (ST_BOOLEAN setMatching)
  {
MIS_DV_INDEX dvIndex;
MIS_DEVICE_INDEX devIndex;

  for (dvIndex = 0; dvIndex < (MIS_DV_INDEX) mis_num_dv_ctrl; ++dvIndex)
    mis_dv_ctrl[dvIndex].partner_dv_ref = setMatching ? dvIndex : IR_REF_INVALID;

  for (devIndex = 0; devIndex < (MIS_DEVICE_INDEX) mis_num_device_ctrl; ++devIndex)
    mis_device_ctrl[devIndex].partner_dev_ref = setMatching ? devIndex : IR_REF_INVALID;
  }

/************************************************************************/
/*			_irSetAllRemoteLinkages				*/
/************************************************************************/

static ST_VOID _irSetAllRemoteLinkages (ST_BOOLEAN setMatching)
  {
MI_REMOTE *mi_remote;

  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {				      
    _irSetRemoteLinkages (mi_remote, setMatching);
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

/************************************************************************/
/*			_irSetRemoteLinkages				*/
/************************************************************************/

static ST_VOID _irSetRemoteLinkages (MI_REMOTE *mi_remote, ST_BOOLEAN setMatching)
  {
MIC_VCC *mic_vcc;
MIC_DV *mic_dv;
MIC_DEVICE *mic_dev;
ST_UINT32 idx;

  mic_vcc = mi_remote->mic_vcc;
  if (mic_vcc != NULL)	/* If we are a client to this remote	*/
    {
    for (idx = 0; idx < (ST_UINT32) mic_vcc->num_vcc_dv; ++idx)
      {
      mic_dv = mic_vcc->vcc_dv_tbl[idx];
      mic_dv->partner_dv_ref = setMatching ? idx : IR_REF_INVALID;
      }
    for (idx = 0; idx < (ST_UINT32) mic_vcc->num_icc_dv; ++idx)
      {
      mic_dv = mic_vcc->icc_dv_tbl[idx];
      mic_dv->partner_dv_ref = 
        setMatching ? idx | IR_REMOTE_HANDLE_ICC_FLAG : IR_REF_INVALID;
      }
  
    for (idx = 0; idx < (ST_UINT32) mic_vcc->num_vcc_devices; ++idx)
      {
      mic_dev = mic_vcc->vcc_device_tbl[idx];
      mic_dev->partner_dev_ref = setMatching ? idx : IR_REF_INVALID;
      }
    for (idx = 0; idx < (ST_UINT32) mic_vcc->num_icc_devices; ++idx)
      {
      mic_dev = mic_vcc->icc_device_tbl[idx];
      mic_dev->partner_dev_ref = 
        setMatching ? idx | IR_REMOTE_HANDLE_ICC_FLAG : IR_REF_INVALID;
      }
    }
  }

/************************************************************************/
/************************************************************************/
/*			_irFindMicDvIdx					*/
/************************************************************************/

ST_UINT32 irFindMicDvIdx (MIC_VCC *mic_vcc, MIC_DV *mic_dv)
  {
ST_UINT32 idx;

  if (mic_dv->dv_attrib & MIC_DV_SCOPE_ICC)
    {
    for (idx = 0; idx < (ST_UINT32) mic_vcc->num_icc_dv; ++idx)
      {
      if (mic_vcc->icc_dv_tbl[idx] == mic_dv)
        return (idx | IR_REMOTE_HANDLE_ICC_FLAG);
      }
    return (IR_REF_INVALID);
    }

  for (idx = 0; idx < (ST_UINT32) mic_vcc->num_vcc_dv; ++idx)
    {
    if (mic_vcc->vcc_dv_tbl[idx] == mic_dv)
      return (idx);
    }
  return (IR_REF_INVALID);
  }

/************************************************************************/
/************************************************************************/
/*			_irLocalCrc					*/
/************************************************************************/

static ST_UINT32 _irLocalCrc ()
  {
ST_UINT local_crc = 0;

  local_crc = _irLocalDvCrc(local_crc);
  local_crc = _irLocalDevCrc(local_crc);
  return (local_crc);
  }


/************************************************************************/
/*			     _irLocalDvCrc				*/
/************************************************************************/

static ST_UINT32 _irLocalDvCrc(ST_UINT32 crc)
  {
ST_INT 	  i;
ST_INT    idx;

  for (i = 0; i < mis_num_dv_ctrl; ++i)
    {
    idx = mis_dv_ctrl_index[i];
    crc = _irStringCrc(mis_dv_ctrl[idx].dv_name, 
    			 strlen(mis_dv_ctrl[idx].dv_name), crc);
    }
  
  return(crc);
  }

/************************************************************************/
/*			     _irLocalDevCrc				*/
/************************************************************************/

static ST_UINT32 _irLocalDevCrc(ST_UINT32 crc)
  {
ST_INT    i;
ST_INT    idx;

  for (i = 0; i < mis_num_device_ctrl; ++i)
    {
    idx = mis_device_ctrl_index[i];
    
    crc = _irStringCrc(mis_device_ctrl[idx].device_name, 
    			 strlen(mis_device_ctrl[idx].device_name), crc);
    crc = _irStringCrc((mis_device_ctrl[idx].sbo == SD_TRUE ? "T" : "F"), 1, crc);
    }
  
  return(crc);
  }

/************************************************************************/
/************************************************************************/

static ST_UINT32 _irRemoteCrc (MI_REMOTE *mi_remote, ST_UINT32 crc)
  {
  if (mi_remote->name != NULL)
    crc = _irStringCrc (mi_remote->name, strlen(mi_remote->name), crc);
    
  crc = _irMisDvCrc (mi_remote, crc);
  crc = _irMisDeviceCrc (mi_remote, crc);
  crc = _irMicDvCrc (mi_remote, crc);
  crc = _irMicDeviceCrc (mi_remote, crc);
  return(crc);
  }  
 
 
/************************************************************************/
/************************************************************************/

static ST_UINT32 _irMisDvCrc (MI_REMOTE *mi_remote, ST_UINT32 crc)
  {
MIS_DV_REF dv_ref;
MIS_DV 	   *mis_dv;
ST_INT     i;

  if (mi_remote->mis_vcc != NULL)
    {
    for (i = 0; i < mi_remote->mis_vcc->num_vcc_dv_ref; ++i)
      {
      dv_ref = mi_remote->mis_vcc->vcc_dv_ref_tbl[i];
      mis_dv = MIS_DV_REF_TO_DV (dv_ref);
  	
      crc = _irIntCrc(dv_ref, crc);
      crc = _irStringCrc(mis_dv->dv_name, strlen(mis_dv->dv_name), crc);
      }
    
    for (i = 0; i < mi_remote->mis_vcc->num_icc_dv_ref; ++i)
      {
      dv_ref = mi_remote->mis_vcc->icc_dv_ref_tbl[i];
      mis_dv = MIS_DV_REF_TO_DV (dv_ref);
  	
      crc = _irIntCrc(dv_ref, crc);
      crc = _irStringCrc(mis_dv->dv_name, strlen(mis_dv->dv_name), crc);
      }
    }
  
  return(crc);
  }  
    
/************************************************************************/
/************************************************************************/

static ST_UINT32 _irMisDeviceCrc (MI_REMOTE *mi_remote, ST_UINT32 crc)
  {
MIS_DEVICE     *mis_device;
MIS_DEVICE_REF device_ref;
ST_INT         i;

  for (i = 0; i < mi_remote->mis_vcc->num_vcc_device_ref; ++i)
    {
    device_ref = mi_remote->mis_vcc->vcc_device_ref_tbl[i];
    mis_device = MIS_DEV_REF_TO_DEV (device_ref);
    
    crc = _irIntCrc(device_ref, crc);
    crc = _irStringCrc(mis_device->device_name, strlen(mis_device->device_name), crc);
    }
  
  for (i = 0; i < mi_remote->mis_vcc->num_icc_device_ref; ++i)
    {
    device_ref = mi_remote->mis_vcc->icc_device_ref_tbl[i];
    mis_device = MIS_DEV_REF_TO_DEV (device_ref);
    
    crc = _irIntCrc(device_ref, crc);
    crc = _irStringCrc(mis_device->device_name, strlen(mis_device->device_name), crc);
    }
  
  return(crc);
  }  
  
/************************************************************************/
/************************************************************************/

static ST_UINT32 _irMicDvCrc(MI_REMOTE *mi_remote, ST_UINT32 crc)
  {
MIC_DV 	  *mic_dv;
ST_INT    i;

  for (i = 0; i < mi_remote->mic_vcc->num_vcc_dv; ++i)
    {
    mic_dv = mi_remote->mic_vcc->vcc_dv_tbl[i];

    crc = _irStringCrc(mic_dv->dv_name, strlen(mic_dv->dv_name), crc);
    crc = _irIntCrc((ST_INT)mic_dv->dv_attrib, crc);
    }
  
  for (i = 0; i < mi_remote->mic_vcc->num_icc_dv; ++i)
    {
    mic_dv = mi_remote->mic_vcc->icc_dv_tbl[i];

    crc = _irStringCrc(mic_dv->dv_name, strlen(mic_dv->dv_name), crc);
    crc = _irIntCrc((ST_INT)mic_dv->dv_attrib, crc);
    }
  
  return(crc);
  }  
    
/************************************************************************/
/************************************************************************/

static ST_UINT32 _irMicDeviceCrc (MI_REMOTE *mi_remote, ST_UINT32 crc)
  {
MIC_DEVICE *mic_device;
ST_INT     i;

  for (i = 0; i < mi_remote->mic_vcc->num_vcc_devices; ++i)
    {
    mic_device = mi_remote->mic_vcc->vcc_device_tbl[i];
    
    crc = _irStringCrc(mic_device->device_name, strlen(mic_device->device_name), crc);
    crc = _irIntCrc((ST_INT)mic_device->device_attrib, crc);
    }
    
  for (i = 0; i < mi_remote->mic_vcc->num_icc_devices; ++i)
    {
    mic_device = mi_remote->mic_vcc->icc_device_tbl[i];
    
    crc = _irStringCrc(mic_device->device_name, strlen(mic_device->device_name), crc);
    crc = _irIntCrc((ST_INT)mic_device->device_attrib, crc);
    }

  return(crc);
  }  


/************************************************************************/
/************************************************************************/
/*
  IEEE-802.3 32bit CRC
  --------------------
*/

static const ST_UINT crc32table[256] = {
                   /* 0x00 */ 0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
                   /* 0x04 */ 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
                   /* 0x08 */ 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
                   /* 0x0C */ 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
                   /* 0x10 */ 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
                   /* 0x14 */ 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
                   /* 0x18 */ 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
                   /* 0x1C */ 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
                   /* 0x20 */ 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
                   /* 0x24 */ 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
                   /* 0x28 */ 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
                   /* 0x2C */ 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
                   /* 0x30 */ 0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
                   /* 0x34 */ 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
                   /* 0x38 */ 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
                   /* 0x3C */ 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
                   /* 0x40 */ 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
                   /* 0x44 */ 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
                   /* 0x48 */ 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
                   /* 0x4C */ 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
                   /* 0x50 */ 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
                   /* 0x54 */ 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
                   /* 0x58 */ 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
                   /* 0x5C */ 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
                   /* 0x60 */ 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
                   /* 0x64 */ 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
                   /* 0x68 */ 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
                   /* 0x6C */ 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
                   /* 0x70 */ 0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
                   /* 0x74 */ 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
                   /* 0x78 */ 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
                   /* 0x7C */ 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
                   /* 0x80 */ 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
                   /* 0x84 */ 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
                   /* 0x88 */ 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
                   /* 0x8C */ 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
                   /* 0x90 */ 0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
                   /* 0x94 */ 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
                   /* 0x98 */ 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
                   /* 0x9C */ 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
                   /* 0xA0 */ 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
                   /* 0xA4 */ 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
                   /* 0xA8 */ 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
                   /* 0xAC */ 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
                   /* 0xB0 */ 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
                   /* 0xB4 */ 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
                   /* 0xB8 */ 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
                   /* 0xBC */ 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
                   /* 0xC0 */ 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
                   /* 0xC4 */ 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
                   /* 0xC8 */ 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
                   /* 0xCC */ 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
                   /* 0xD0 */ 0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
                   /* 0xD4 */ 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
                   /* 0xD8 */ 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
                   /* 0xDC */ 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
                   /* 0xE0 */ 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
                   /* 0xE4 */ 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
                   /* 0xE8 */ 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
                   /* 0xEC */ 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
                   /* 0xF0 */ 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
                   /* 0xF4 */ 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
                   /* 0xF8 */ 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
                   /* 0xFC */ 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
                   };

/************************************************************************/
/*			     _irStringCrc()				*/
/************************************************************************/

static ST_UINT _irStringCrc(ST_UCHAR *address, ST_UINT size, ST_UINT crc)
  {
  for (; (size > 0); size--)
    /* byte loop */
    crc = (((crc >> 8) & 0x00FFFFFF) ^
           crc32table[(crc ^ *address++) & 0x000000FF]);

  return(crc);
  }

/************************************************************************/
/*			     _irIntCrc()				*/
/************************************************************************/

static ST_UINT _irIntCrc(ST_INT number, ST_UINT crc)
  {
ST_UCHAR *address; 
ST_INT i;
  
  address = (ST_UCHAR *)&number;
  
  for(i = 0; i < sizeof(ST_INT); i++)
    {
    crc = (((crc >> 8) & 0x00FFFFFF) ^
           crc32table[(crc ^ *address++) & 0x000000FF]);
    i++;
    }

  return(crc);
  }

