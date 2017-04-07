/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*                      2001-2002, All Rights Reserved                  */
/*                                                                      */
/* MODULE NAME : rpl.c                                                  */
/* PRODUCT(S)  : ICCP-LITE                                              */
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :                            */
/*      NONE                                                            */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                  Comments                        */
/* --------  ---  ------   -------------------------------------------  */
/* 06/20/02  RKR     03    Copyright year update                        */
/* 02/26/02  MDE     02    Changed startup sequence a bit		*/
/* 12/27/01  MDE     01    New module                                   */
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
/* For debug version, use a static pointer to avoid duplication of      */
/* __FILE__ strings.                                                    */
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/
/************************************************************************/
/************************************************************************/

ST_UINT  rpl_debug_sel;
SD_CONST ST_CHAR *SD_CONST _rpl_flow_logstr = "RPL_LOG_FLOW";
SD_CONST ST_CHAR *SD_CONST _rpl_nerr_logstr = "RPL_LOG_NERR";
SD_CONST ST_CHAR *SD_CONST _rpl_err_logstr  = "RPL_LOG_ERR";

static RPL_STATE_INFO gRplState;
static ST_THREAD_HANDLE rpl_thread;
static ST_THREAD_ID rpl_threadId;
static GEN_SOCK *rplSock;
static ST_BOOLEAN rplTerminate;

ST_THREAD_RET ST_THREAD_CALL_CONV rplThreadEntry (ST_THREAD_ARG arg);

static ST_VOID uSockConnectInd (GEN_SOCK *pSock);
static ST_VOID uSockDisconnectInd (GEN_SOCK *pSock);
static ST_VOID uSockRxInd (GEN_SOCK *pSock);
static ST_VOID uSockTxInd (GEN_SOCK *pSock, ST_RET result);
static ST_VOID uSockHuntInd (GEN_SOCK *pSock, ST_INT *huntStateIo,
                            ST_CHAR *buf, ST_INT bufCount, ST_INT *lenOut);


static ST_VOID rplPrimary (ST_VOID);
static ST_VOID rplSecondary (ST_VOID);
static ST_CHAR *_rplGetOpString (enum RPL_OPCODE op);

/************************************************************************/
/************************************************************************/
/*			rplStart					*/
/************************************************************************/

ST_RET rplStart (RPL_CFG *cfgInfo)
  {
RPL_STATE_INFO *rplState;
ST_RET rc;

  RPL_LOG_FLOW0 ("Starting");

  rplState = &gRplState;
  rplSock = chk_calloc (1, sizeof (GEN_SOCK));
  rc = sockInit (rplSock);
  if (rc != SD_SUCCESS)
    {
    RPL_LOG_ERR1 ("SockInit failed: %d", rc);
    return (rc);
    }
  rplSock->hdrSize = sizeof (RPL_MSG_HDR);

/* Hook into the gensock function pointers */
  rplSock->uSockConnect         = uSockConnectInd;
  rplSock->uSockDisconnect      = uSockDisconnectInd;
  rplSock->uSockHunt            = uSockHuntInd;
  rplSock->uSockRx              = uSockRxInd;
  rplSock->uSockTx              = uSockTxInd;

  rplState->rplMode = RplModeStartup;
  rplState->rplState = RplStateOkIdle;
  rplState->ready = SD_FALSE;
  rplTerminate = SD_FALSE;

/* Save configuration info */
  rplState->rplCfg = *cfgInfo;
        
  if (cfgInfo->role == RplRoleSecondary)
    {   /* We are the called node */
    RPL_LOG_FLOW0 ("Role is Secondary");
    rc = sockBind (rplSock, cfgInfo->port);
    if (rc != SD_SUCCESS)
      {
      RPL_LOG_ERR1 ("Bind failure: %d", rc);
      return (rc);
      }
    }

/* OK, start the receive & send threads */
  rc = sockStartRecv (rplSock);
  if (rc != SD_SUCCESS)
    {
    RPL_LOG_ERR1 ("Receive thread start failed: %d", rc);
    return (rc);
    }
  rc = sockStartSend (rplSock);
  if (rc != SD_SUCCESS)
    {
    RPL_LOG_ERR1 ("Send thread start failed: %d", rc);
    return (rc);
    }

/* We will start another thread for RPL management */
  rc = gs_start_thread (rplThreadEntry, cfgInfo, &rpl_thread, &rpl_threadId);
  return (rc);
  }

/************************************************************************/
/*			rplThreadEntry					*/
/************************************************************************/

ST_THREAD_RET ST_THREAD_CALL_CONV rplThreadEntry (ST_THREAD_ARG arg)
  {
RPL_STATE_INFO *rplState;
ST_INT i;

  rplState = &gRplState;

/* Enter the service functions, which do not return until it is time 	*/
/* to terminate.							*/
  if (rplState->rplCfg.role == RplRolePrimary)
    rplPrimary ();
  else if (rplState->rplCfg.role == RplRoleSecondary)
    rplSecondary ();
  else
    {
    RPL_LOG_ERR1 ("Invalid Role: %d", rplState->rplCfg.role);
    return (SD_FAILURE);
    }

/* Now terminate the send and receive threads */
  RPL_LOG_FLOW0 ("Terminating");
  sockStopRecv (rplSock);
  sockStopSend (rplSock);

  i = 0;
  while (i < 100)
    {
    Sleep (100);
    if (rplSock->thRecv == 0 && rplSock->thSend == 0)
      break;
    ++i;
    }
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			rplPrimary					*/
/************************************************************************/

static ST_VOID rplPrimary ()
  {
RPL_CFG *cfgInfo;
RPL_STATE_INFO *rplState;
ST_DOUBLE currMsTime;
ST_INT intSleepTime;
ST_RET rc;

  rplState = &gRplState;
  cfgInfo =  &gRplState.rplCfg;

/******** ROLE IS PRIMARY *******/
/* OK, now we just service the RPL socket connection */

  intSleepTime = (ST_INT) cfgInfo->rplSleepTime;
  if (intSleepTime == 0)
    intSleepTime = 20;

  while (rplTerminate == SD_FALSE)
    {
    currMsTime = sGetMsTime ();
    if (rplState->enable == SD_TRUE)
      {
      switch (rplState->rplState)
        {
        case RplStateOkBackoff:
          if (currMsTime >= rplState->nextConnectTime)
            {
            RPL_LOG_FLOW0 ("Connecting");
            rplState->rplState = RplStateOkConnecting;
            rc = sockConnect (rplSock, cfgInfo->port, cfgInfo->partnerIPAddr);
            if (rc != SD_SUCCESS)
              {
              RPL_LOG_FLOW1 ("SockConnect failed: %d", rc);
              rplState->rplState = RplStateOkBackoff;
              rplState->nextConnectTime = 
              currMsTime + rplState->rplCfg.connectRetryTime;
              ++gRplState.numConnectsFailed;

              }
            }
        break;

        case RplStateOkIdle:
          RPL_LOG_FLOW0 ("Connecting");
          rplState->rplState = RplStateOkConnecting;
          rc = sockConnect (rplSock, cfgInfo->port, cfgInfo->partnerIPAddr);
          if (rc != SD_SUCCESS)
            {
            RPL_LOG_FLOW1 ("SockConnect failed: %d", rc);
            rplState->rplState = RplStateOkBackoff;
            rplState->nextConnectTime = currMsTime + rplState->rplCfg.connectRetryTime;
            ++gRplState.numConnectsFailed;
            }
        break;

        case RplStateOkConnected:
        case RplStateNotOk:
        case RplStateOkDisabled:
        case RplStateOkWaitingConnect:
        case RplStateOkConnectPending:
        case RplStateOkDisconnecting:
        default:
        break;
        }
      }
    else        /* We are disabled */
      {
      switch (rplState->rplState)
        {
        case RplStateOkBackoff:
          rplState->rplState = RplStateOkDisabled;
        break;

        case RplStateOkConnected:
          RPL_LOG_FLOW0 ("Disconnecting");
          rplState->rplState = RplStateOkDisconnecting;
          rc = sockClose (rplSock);
          if (rc != SD_SUCCESS)
            {
            RPL_LOG_NERR1 ("SockClose failed: %d", rc);
            }
          rplState->rplState = RplStateOkDisabled;
        break;

        case RplStateNotOk:
        case RplStateOkDisabled:
        case RplStateOkWaitingConnect:
        case RplStateOkConnectPending:
        case RplStateOkDisconnecting:
        case RplStateOkIdle:

        default:
        break;
        }
      }
    Sleep (intSleepTime);
    }
  }

/************************************************************************/
/*			rplSecondary					*/
/************************************************************************/

static ST_VOID rplSecondary ()
  {
RPL_CFG *cfgInfo;
RPL_STATE_INFO *rplState;
ST_DOUBLE currMsTime;
ST_INT intSleepTime;
ST_RET rc;

  rplState = &gRplState;
  cfgInfo =  &gRplState.rplCfg;

/******** ROLE IS SECONDARY *******/
/* OK, now we just service the RPL socket connection */
  intSleepTime = (ST_INT) cfgInfo->rplSleepTime;
  if (intSleepTime == 0)
    intSleepTime = 20;

  while (rplTerminate == SD_FALSE)
    {
    currMsTime = sGetMsTime ();
    if (rplState->enable == SD_TRUE)
      {
      switch (rplState->rplState)
        {
        case RplStateOkIdle:
          RPL_LOG_FLOW0 ("Starting socket listen");
          rc = sockListen (rplSock);
          if (rc == SD_SUCCESS)
            rplState->rplState = RplStateOkWaitingConnect;
          else
            {
            RPL_LOG_NERR1 ("SockListen failed: %d", rc);
            ++gRplState.numConnectsFailed;
            rplState->rplState = RplStateNotOk;
            }
        break;


        case RplStateNotOk:
        case RplStateOkDisabled:
        case RplStateOkWaitingConnect:
        case RplStateOkConnectPending:
        case RplStateOkBackoff:
        case RplStateOkConnected:
        case RplStateOkDisconnecting:
        default:
        break;
        }
      }
    else      /* We are disabled */
      {
      switch (rplState->rplState)
        {
        case RplStateOkBackoff:
          rplState->rplState = RplStateOkDisabled;
        break;

        case RplStateOkConnected:
          RPL_LOG_FLOW0 ("Disconnecting");
          rplState->rplState = RplStateOkDisconnecting;
          rc = sockClose (rplSock);
          if (rc != SD_SUCCESS)
            {
            RPL_LOG_ERR1 ("SockClose failed: %d", rc);
            }
          rplState->rplState = RplStateOkDisabled;
        break;

        case RplStateNotOk:
        case RplStateOkDisabled:
        case RplStateOkWaitingConnect:
        case RplStateOkConnectPending:
        case RplStateOkDisconnecting:
        case RplStateOkIdle:
        default:
        break;
        }
      }
    Sleep (intSleepTime);
    }
  }

/************************************************************************/
/************************************************************************/
/*			rplEnd					*/
/************************************************************************/

ST_RET rplEnd (ST_VOID)
  {
ST_INT intSleepTime;
ST_INT i;

/* Tell send/receive threads it is time to terminate */
  rplTerminate = SD_TRUE;

/* Give the Primary/Secondary threads time to wake up and terminate 	*/
/* which will cause the send/receive threads to terminate		*/
  intSleepTime = (ST_INT) gRplState.rplCfg.rplSleepTime;
  if (intSleepTime == 0)
    intSleepTime = 20;

  i = 0;
  while ((rplSock->thRecv != 0 || rplSock->thSend != 0) && i < 100)
    {
    Sleep (intSleepTime);
    ++i;
    }

/* See if they all terminated OK */
  if (rplSock->thRecv != 0 || rplSock->thSend != 0)
    return (SD_FAILURE);

  return (SD_SUCCESS);
  }

/************************************************************************/
/*			rplGetState					*/
/************************************************************************/

ST_RET rplGetState (RPL_STATE_INFO **stateOut)
  {
  *stateOut = &gRplState;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			rplEnable					*/
/************************************************************************/

ST_RET rplEnable (ST_VOID)
  {
  gRplState.enable = SD_TRUE;
  if (gRplState.rplState == RplStateOkDisabled)
    gRplState.rplState = RplStateOkIdle;

  return (SD_SUCCESS);
  }


/************************************************************************/
/*			rplDisable					*/
/************************************************************************/

ST_RET rplDisable (ST_VOID)
  {
  gRplState.enable = SD_FALSE;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			rplSendMsg					*/
/************************************************************************/

ST_RET rplSendMsg  (RPL_MSG_HDR *msg)
  {
ST_RET rc;
ST_INT msgLen;

  RPL_LOG_FLOW2 ("Sending message: op %d, len %d", 
                  _rplGetOpString (msg->op), 
                  msg->bufferSize + sizeof (RPL_MSG_HDR));


  if (gRplState.rplState != RplStateOkConnected) 
    {
    RPL_LOG_NERR0 ("RplSendMsg failed: state not RplStateConnectedOk");
    return (SD_FAILURE);
    }

  msgLen = msg->bufferSize + sizeof (RPL_MSG_HDR);
  rc = sockSend (rplSock, msg, msgLen);
  if (rc != SD_SUCCESS)
    {
    RPL_LOG_ERR1 ("SockSend failed: %d", rc);
    }
  return (rc);
  }

/************************************************************************/
/************************************************************************/
/*			uSockConnectInd					*/
/************************************************************************/

static ST_VOID uSockConnectInd (GEN_SOCK *pSock)
  {
  RPL_LOG_FLOW0 ("Connection active");
  gRplState.rplState = RplStateOkConnected; 
  ++gRplState.numConnectsOk;
  uRplActive ();
  }

/************************************************************************/
/*			uSockDisconnectInd				*/
/************************************************************************/

static ST_VOID  uSockDisconnectInd (GEN_SOCK *pSock)
  {
  RPL_LOG_FLOW0 ("Connection inactive");
  if (gRplState.rplState == RplStateOkConnecting)
    {
    RPL_LOG_ERR0 ("Connection failed");
    gRplState.rplState = RplStateOkBackoff;
    gRplState.nextConnectTime = sGetMsTime() + gRplState.rplCfg.connectRetryTime;
    ++gRplState.numConnectsFailed;
    }
  else
    gRplState.rplState = RplStateOkIdle;

  uRplInactive ();
  }

/************************************************************************/
/*			uSockHuntInd					*/
/************************************************************************/

static ST_VOID uSockHuntInd (GEN_SOCK *pSock, ST_INT *huntStateIo,
                            ST_CHAR *buf, ST_INT bufCount, ST_INT *lenOut)
  {
RPL_MSG_HDR *hdr;

/* Better get the whole header ... */
  if (bufCount != sizeof (RPL_MSG_HDR))
    {
    RPL_LOG_ERR0 ("Hunt size error");
    *huntStateIo = GENSOCK_HUNT_ERROR;
    return;
    }

  hdr = (RPL_MSG_HDR *) buf;
  if (hdr->magicNumber != RPL_MAGIC_NUMBER)
    {
    RPL_LOG_ERR0 ("Hunt magic number error");
    *huntStateIo = GENSOCK_HUNT_ERROR;
    return;
    }

/* All is well, let's get the rest of the message */
  *lenOut = hdr->bufferSize;
  *huntStateIo = GENSOCK_HUNT_DONE;
  }

/************************************************************************/
/*			uSockRxInd					*/
/************************************************************************/

static ST_VOID  uSockRxInd (GEN_SOCK *pSock)
  {
RPL_MSG_HDR *msg;
ST_INT msgLen;
ST_RET rc;

  rc = sockGetData (pSock, &msg, &msgLen);
  if (rc == SD_SUCCESS)
    {
    RPL_LOG_FLOW2 ("Message received, len %d, op = %s", 
    				msgLen, _rplGetOpString (msg->op));
    ++gRplState.numMsgsRcvd;
    uRplMsgRcvd  (msg);
    }
  else
    {
    RPL_LOG_NERR1 ("SockGetData failed: %d", rc);
    }
  }

/************************************************************************/
/*			uSockTxInd					*/
/************************************************************************/

static ST_VOID  uSockTxInd (GEN_SOCK *pSock, ST_RET result)
  {
  ++gRplState.numMsgsSent;
  }

/************************************************************************/
/************************************************************************/
/*			_rplGetOpString					*/
/************************************************************************/

typedef struct 
  {
  ST_INT   val;
  ST_CHAR *str;
  } RPL_VAL_STRING;

static RPL_VAL_STRING _rplOpStrTbl[] = 
  {
    {RplOpHeartBeat,		"RplOpHeartBeat"},
    {RplOpSetModeRequest,	"RplOpSetModeRequest"},
    {RplOpSetModeResponse,	"RplOpSetModeResponse"},
    {RplOpDataLinkageRequest,	"RplOpDataLinkageRequest"},
    {RplOpDataLinkageResponse,	"RplOpDataLinkageResponse"},
    {RplOpDataSyncRequest,	"RplOpDataSyncRequest"},
    {RplOpDataSyncResponse,	"RplOpDataSyncResponse"},
    {RplOpDataValuesRequest,	"RplOpDataValuesRequest"},
    {RplOpDataValuesResponse,	"RplOpDataValuesResponse"}
  };


static ST_CHAR *_rplGetOpString (enum RPL_OPCODE op)
  {
ST_INT i;
ST_INT numOpStr;

  numOpStr = sizeof (_rplOpStrTbl)/sizeof(RPL_VAL_STRING);
  for (i = 0; i < numOpStr; ++i)
    {
    if (_rplOpStrTbl[i].val = op)
      return (_rplOpStrTbl[i].str);
    }
  return ("Unknown");
  }
