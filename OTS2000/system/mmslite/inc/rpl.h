/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	2001-2002, All Rights Reserved		        	*/
/*									*/
/* MODULE NAME : rpl.h   						*/
/* PRODUCT(S)  : AX-S4 ICCP						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 03/21/02  MDE     03	   Added Association State request		*/
/* 02/18/02  MDE     02	   Misc. fix and clean				*/
/* 12/27/01  MDE     01    Created					*/
/************************************************************************/

#ifndef RPL_INCLUDED
#define RPL_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/

#include "glbtypes.h"
#include "mi.h"

/************************************************************************/
/************************************************************************/

/* Maximum size of a single RPL message */
#define MAX_RPL_MSG_SIZE	64000

/* Used in RPL Message Header */
#define RPL_MAGIC_NUMBER	0x9F2EC831

/************************************************************************/
/************************************************************************/
/* RPL ENUMS								*/

enum RPL_RESULT
  {
  RplRsltSuccess  		= 000,
  RplRsltPartialSuccess 	= 001,
  RplRsltFailureOther		= 002,	/* Try not to use */
  RplRsltFailureLocked		= 003,
  RplRsltFailureNotReady	= 004
  };

enum RPL_OPCODE
  {
  RplOpHeartBeat		= 100,
  RplOpSetModeRequest		= 101,
  RplOpSetModeResponse		= 102,
  RplOpDataLinkageRequest 	= 103,
  RplOpDataLinkageResponse	= 104,
  RplOpDataSyncRequest		= 105,
  RplOpDataSyncResponse		= 106,
  RplOpDataValuesRequest	= 107,
  RplOpDataValuesResponse	= 108,
  RplOpCrc			= 109,
  RplOpAssociationState		= 110
  };

enum RPL_ROLE
  {
  RplRolePrimary		= 201,
  RplRoleSecondary		= 202
  };

enum RPL_MODE
  {
  RplModeActive			= 301,
  RplModeStandby		= 302,
  RplModeStartup		= 303
  };

enum RPL_DATA_SYNC_MODE
  {
  RplDsModeNone			= 401,
  RplDsModeLocal		= 402,
  RplDsModeLocalAndRemote	= 403
  };

enum RPL_DATA_SYNC_REQ_TYPE
  {
  RplDsReqTypeAll		= 701, /* Local and all remotes		*/
  RplDsReqTypeLocal		= 702, /* Local only			*/
  RplDsReqTypeRemote		= 703  /* Single specified remote 	*/
  };

enum RPL_CC_TYPE
  {
  RplCcTypeLocalCc		= 501,
  RplCcTypeRemoteCc		= 502
  };


enum RPL_STATE
  {
  RplStateNotOk			= 601,
  RplStateOkDisabled		= 602,
  RplStateOkWaitingConnect	= 603,
  RplStateOkConnectPending	= 604,
  RplStateOkBackoff		= 605,
  RplStateOkConnecting		= 606,
  RplStateOkConnected		= 607,
  RplStateOkDisconnecting	= 608,
  RplStateOkIdle		= 609
  };


/************************************************************************/
/************************************************************************/
/* RPL API								*/

#define RPL_LOG_ERR  	0x0001   
#define RPL_LOG_NERR 	0x0002   
#define RPL_LOG_FLOW  	0x0004   
extern  ST_UINT  rpl_debug_sel;


#define IR_LOG_ERR  	0x0001   
#define IR_LOG_NERR 	0x0002   
#define IR_LOG_FLOW  	0x0004   
#define IR_LOG_DATA  	0x0008   
extern  ST_UINT  ir_debug_sel;

typedef struct 
  {
  enum RPL_ROLE role;
  enum RPL_DATA_SYNC_MODE dataSyncMode;

  ST_INT16   port;
  ST_CHAR    partnerIPAddr[80]; 
  ST_DOUBLE  connectRetryTime;
  ST_DOUBLE  rplStartupTime;
  ST_DOUBLE  rplRequestTimeout1;    /* Not currently used 		*/
  ST_DOUBLE  rplSleepTime;	    /* Used in service loop 		*/
  ST_DOUBLE  rplRequestTimeout2;    /* Heartbeat timeout last gasp 	*/
  ST_DOUBLE  switchToActiveDelay;   /* For manual switchover		*/
  ST_DOUBLE  heartbeatTxPeriod;
  ST_INT     maxDataValueReqPend;
  ST_BOOLEAN linkConnectTimeoutEnable;
  ST_DOUBLE  linkConnectTimeout;
  } RPL_CFG;
 

typedef struct _rplStateInfo
  {
  RPL_CFG 		rplCfg;
  ST_BOOLEAN 		enable;
  enum RPL_STATE 	rplState;	/* RPL connected, etc.		*/
  enum RPL_MODE		rplMode;  	/* Active/Standby/Startup 	*/
  ST_BOOLEAN		modeLocked;

/* The following are not used by RPL, but are set by the RPL user and	*/
/* can be used by the RPL user (IR) and the partner node. 		*/
  ST_BOOLEAN		ready;
  ST_INT32		totalLinks;
  ST_INT32		activeLinks;

/* RPL Statistics */
  ST_INT32  		numConnectsOk;
  ST_INT32  		numConnectsFailed;
  ST_INT32  		numMsgsSent;
  ST_INT32  		numMsgsRcvd;

/* Internal RPL */
  ST_DOUBLE    		nextConnectTime;
  ST_INT    		dataValueReqPend;
  } RPL_STATE_INFO;


/************************************************************************/
/************************************************************************/
/* RPL Messages 	 						*/
/************************************************************************/

typedef struct 
  {
  RPL_STATE_INFO 	stateInfo;
  ST_DOUBLE		holdTime;
  } RPL_HEARTBEAT;

/************************************************************************/

typedef struct 
  {
  enum RPL_MODE		mode;
  } RPL_SET_MODE_REQ;

typedef struct 
  {
  enum RPL_RESULT  	result;
  enum RPL_MODE		newMode;
  } RPL_SET_MODE_RESP;

/************************************************************************/
#if 0 	/* Not Implemented */

typedef struct 
  {
  ST_CHAR 	name[MAX_IDENT_LEN+1];
  ST_UINT8 	scope;
  ST_UINT32	ref;		/* Standby node's handle for the DV	*/
  } RPL_DATA_LINKAGE;

typedef struct 
  {
  ST_BOOLEAN   resetAllLinkage;	/* Clear all linkage in scope	*/
  enum RPL_CC_TYPE ccType;
  ST_INT32     numDv;
  ST_INT32     numDev;
  ST_CHAR      remoteName[MAX_IDENT_LEN+1];	/* for remote only	*/
  ST_CHAR      domName[MAX_IDENT_LEN+1];	/* for remote only	*/
  } RPL_DATA_LINKAGE_REQ;
/* Followed by array of RPL_DATA_LINKAGE[numDv]				*/
/* Followed by array of RPL_DATA_LINKAGE[numDev]			*/

typedef struct 
  {
  enum RPL_RESULT result;
  ST_INT32 numOk;
  ST_INT32 numFailed;
  } RPL_DATA_LINKAGE_RESP;

#endif	/* Not Implemented */

/************************************************************************/

typedef struct 
  {
  enum RPL_DATA_SYNC_REQ_TYPE syncType;
  ST_CHAR      remoteName[MAX_IDENT_LEN+1];	/* for remote only	*/
  } RPL_DATA_SYNC_REQ;

typedef struct 
  {
  enum RPL_RESULT result;
  } RPL_DATA_SYNC_RESP;

/************************************************************************/

#define IR_REF_INVALID 0xFFFFFFFF


typedef struct 
  {
  ST_UINT32	ref;		/* Standby node's handle for the DV	*/
  ST_INT32	type_id;	/* MI_TYPEID_xxx			*/
  union
    {
    MI_REAL			r;
    MI_STATE			s;
    MI_DISCRETE			d;
    MI_REAL_Q			rq;
    MI_STATE_Q			sq;
    MI_DISCRETE_Q		dq;
    MI_REAL_Q_TIMETAG		rqt;
    MI_STATE_Q_TIMETAG		sqt;
    MI_DISCRETE_Q_TIMETAG	dqt;
    MI_REAL_EXTENDED		re;
    MI_STATE_EXTENDED		se;
    MI_DISCRETE_EXTENDED	de;
    } value;
  } RPL_DV_VALUE;

typedef struct 
  {
  ST_UINT32	ref;		/* Standby node's handle for the DEV	*/
  ST_INT32	typeId;		/* MI_TYPEID_xxx			*/
  union
    {
    MI_SBO_CHECKBACK		 check_back_id;
    MI_CONTROL_COMMAND		 cmd;
    MI_CONTROL_SETPOINT_REAL	 set_pt_r;
    MI_CONTROL_SETPOINT_DISCRETE set_pt_d;
    MI_TAG_VALUE		 tag_value;
    } value;
  } RPL_DEV_VALUE;


typedef struct 
  {
  enum RPL_CC_TYPE  ccType;
  ST_CHAR      remoteName[MAX_IDENT_LEN+1];	/* for remote only	*/
  ST_INT32 numDv;
  ST_INT32 numDev;
  } RPL_DATA_VALUE_REQ;
/* Followed by array of RPL_DV_VALUE[numDv]				*/
/* Followed by array of RPL_DEV_VALUE[numDev]				*/


typedef struct 
  {
  enum RPL_RESULT result;
  ST_INT32 numDv;
  ST_INT32 numDev;
  ST_INT32 numDvOk;
  ST_INT32 numDevOk;
  } RPL_DATA_VALUE_RESP;


/************************************************************************/

typedef struct 
  {
  ST_CHAR    name[MAX_IDENT_LEN+1];
  ST_INT32   cfgCrc;		 
  } RPL_REMOTE_CRC;

typedef struct 
  {
  ST_INT32   localCfgCrc;
  ST_INT32   numRemotes;
  ST_BOOLEAN sendCrcBack;
  } RPL_CRC_REQ;
/* Followed by array of RPL_REMOTE_CRC[numRemote]				*/


/************************************************************************/

typedef struct 
  {
  ST_CHAR remoteName[MAX_IDENT_LEN+1];
  ST_CHAR assocName[MAX_IDENT_LEN+1];
  ST_INT  state;	/* MI_ASSOC_STATE_ACTIVE, MI_ASSOC_STATE_IDLE	*/ 
  } RPL_ASSOC_STATE_REQ;

/************************************************************************/
/************************************************************************/

typedef struct 
  {
  ST_INT32  magicNumber;	/* Must be == RPL_MAGIC_NUMBER 		*/
  enum RPL_OPCODE op;
  ST_INT32	bufferSize;	/* # bytes that belong to this message	*/
  } RPL_MSG_HDR;
/* Followed by one of -	    */
/*    RPL_HEARTBEAT	    */
/*    RPL_SET_MODE_REQ	    */
/*    RPL_SET_MODE_RESP     */
/*    RPL_DATA_LINKAGE_REQ  */
/*    RPL_DATA_LINKAGE_RESP */
/*    RPL_DATA_SYNC_REQ     */
/*    RPL_DATA_SYNC_RESP    */
/*    RPL_DATA_VALUE_REQ   */
/*    RPL_DATA_VALUE_RESP  */
/*    RPL_ASSOC_STATE_REQ  */

/************************************************************************/
/************************************************************************/

ST_RET rplStart    (RPL_CFG *cfgInfo);
ST_RET rplEnd      (ST_VOID);
ST_RET rplEnable   (ST_VOID);
ST_RET rplDisable  (ST_VOID);
ST_RET rplSendMsg  (RPL_MSG_HDR *msg);
ST_RET rplGetState (RPL_STATE_INFO **stateOut);

ST_VOID uRplActive   (ST_VOID);
ST_VOID uRplInactive (ST_VOID);
ST_VOID uRplMsgRcvd  (RPL_MSG_HDR *msg);


/************************************************************************/
/************************************************************************/
/************************************************************************/
/* Data Sync */
/************************************************************************/

#define IR_DATA_VALUE_BUFFER_TIME_DEF	10000
extern ST_DOUBLE _irDataValueBufferTime;

#define IR_DATA_VALUE_BUFFER_COUNT_DEF	100
extern ST_INT _irDataValueBufferCount;

#define IR_DATA_VALUE_LOCAL_LIST_MAX_DEF 3
extern ST_INT _irDataValueLocalListMax;

#define IR_DATA_VALUE_REMOTE_LIST_MAX_DEF 3
extern ST_INT _irDataValueRemoteListMax;

typedef struct irDataValueCtrl
  {
  DBL_LNK l;
  ST_DOUBLE nextSendTime;
  RPL_MSG_HDR   *msg;
  RPL_DATA_VALUE_REQ *rplValueReq;	/* for convenience */
  } IR_DATA_VALUE_CTRL;

typedef struct _irDataValueListCtrl
  {
  ST_INT msgSize;
  enum RPL_CC_TYPE ccType;
  MI_REMOTE *mi_remote;
  ST_INT numDataValueCtrl;
  ST_INT numDv;
  IR_DATA_VALUE_CTRL *listHead;
  IR_DATA_VALUE_CTRL *nextSend;
  IR_DATA_VALUE_CTRL *active;
  } IR_DATA_VALUE_LIST_CTRL;

/************************************************************************/
/* IR level information							*/
/************************************************************************/

#define IR_REMOTE_HANDLE_ICC_FLAG 0x80000000

enum IR_STATE
  {
  IrStatePrimaryStartup			= 1000,
  IrStateSecondaryStartup		= 1001,
  IrStateActiveRplOk			= 1002,
  IrStateActiveRplNotOk			= 1003,
  IrStateActiveRplConnStarting		= 1004,
  IrStateActiveManToStandby		= 1005,
  IrStateStandbyRplOk			= 1007,
  IrStateStandbyRplNotOk		= 1008,
  IrStateStandbyRplConnStarting		= 1009,
  IrStateStandbyManToActive		= 1010,
  IrStateStandbyDetectedHbFailure	= 1011
  };

typedef struct 
  {
  RPL_CFG   		rplCfg;
  RPL_STATE_INFO       *rplState;

  enum IR_STATE		irState;
  RPL_STATE_INFO 	partnerRplState;

  ST_DOUBLE 		heartbeatTransmitTime;
  ST_DOUBLE 		heartbeatExpireTime;
  ST_DOUBLE		startupTimeOut;
  ST_DOUBLE		switchToActiveTime;
  ST_DOUBLE		hbSwitchTimeOutTime;
  ST_DOUBLE    		linkConnectTimeoutTime;

/* Stats */
  ST_DOUBLE		dataSyncRateCountStartTime;
  ST_LONG		dataSyncRateCount;

  ST_DOUBLE		heartBeatHwRxTime;

  ST_DOUBLE		lastHbRxTime;
  ST_BOOLEAN 		firstHbReceived;

/* Local data sync stats */
  ST_BOOLEAN 		syncActive;
  ST_LONG 		dataSyncCount;

/* Internal */
  ST_BOOLEAN 		crcReceived;
  ST_INT32 	        localCfgCrc;
  ST_INT32 	        partnerLocalCfgCrc;
  IR_DATA_VALUE_LIST_CTRL *_irDvListCtrl;

  } ICCP_REDUNDANCY_CTRL;


/************************************************************************/

ST_RET  irStart (RPL_CFG *rplCfg);
ST_RET  irEnd (ST_VOID);
ST_RET  irService (ST_VOID);
ST_RET  irModeSelect (enum RPL_MODE mode);
ST_RET  irModeLock (ST_VOID);
ST_RET  irModeUnlock (ST_VOID);
ST_VOID irGetStatus (ICCP_REDUNDANCY_CTRL **irCtrlOut);
ST_VOID irCfgChange (ST_VOID);
ST_VOID irSendAssocState (MI_ASSOC_CTRL *mi_assoc);

ST_VOID irLocalDvValue (MIS_DV_REF dvRef, 
			 ST_INT type_id, ST_VOID *data);
ST_UINT32 irFindMicDvIdx (MIC_VCC *mic_vcc, MIC_DV *mic_dv);

ST_VOID irRemoteDvValue (MI_REMOTE *mi_remote, ST_INT idx,
			ST_INT type_id, ST_VOID *data);

ST_RET uIrSetMode (enum RPL_MODE mode);
ST_RET uIrModeRequest (enum RPL_MODE mode);
ST_RET uIrStartDataValue (RPL_DATA_VALUE_REQ *duReq, 
			   RPL_DATA_VALUE_RESP *duResp, ST_VOID **usrHandleOut);

ST_RET uIrDataSyncReq  (RPL_DATA_SYNC_REQ *dsReq);
ST_RET uIrLocalDvValue (RPL_DV_VALUE  *dvValue, ST_VOID *usrHandle);
ST_RET uIrLocalDevValue (RPL_DEV_VALUE  *devValue, ST_VOID *usrHandle);
ST_RET uIrRemoteDvValue (RPL_DV_VALUE  *dvValue, ST_VOID *usrHandle);
ST_RET uIrRemoteDevValue (RPL_DEV_VALUE  *devValue, ST_VOID *usrHandle);

ST_RET uIrEndDataValue (RPL_DATA_VALUE_REQ *duReq, 
			   RPL_DATA_VALUE_RESP *duResp, ST_VOID **usrHandleOut);
ST_VOID uIrLinkConnectTimeout (ST_VOID);
ST_VOID uIrRplMsgRcvd (ST_VOID);
ST_VOID uIrSetModeResp (RPL_SET_MODE_RESP *setModeResp);

ST_VOID uIrAssocState (MI_ASSOC_CTRL *mi_assoc, ST_INT remoteState);

/************************************************************************/
/************************************************************************/
/************************************************************************/
/* INTERNAL                                                             */
/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern SD_CONST ST_CHAR *SD_CONST _rpl_flow_logstr;
extern SD_CONST ST_CHAR *SD_CONST _rpl_nerr_logstr;
extern SD_CONST ST_CHAR *SD_CONST _rpl_err_logstr;

#define RPL_LOG_ERR0(a) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slog (sLogCtrl,_rpl_err_logstr,\
                             thisFileName,__LINE__,a);\
                    }

#define RPL_LOG_ERR1(a,b) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slog (sLogCtrl,_rpl_err_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }

#define RPL_LOG_ERR2(a,b,c) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slog (sLogCtrl,_rpl_err_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

#define RPL_LOG_ERR3(a,b,c,d) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slog (sLogCtrl,_rpl_err_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }

#define RPL_LOG_CERR0(a) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define RPL_LOG_CERR1(a,b) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define RPL_LOG_CERR2(a,b,c) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define RPL_LOG_CERR3(a,b,c,d) {\
                     if (rpl_debug_sel & RPL_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** NERR Macros *************************************/

#define RPL_LOG_NERR0(a) {\
                     if (rpl_debug_sel & RPL_LOG_NERR)\
                       _slog (sLogCtrl,_rpl_nerr_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define RPL_LOG_NERR1(a,b) {\
                     if (rpl_debug_sel & RPL_LOG_NERR)\
                       _slog (sLogCtrl,_rpl_nerr_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define RPL_LOG_NERR2(a,b,c) {\
                     if (rpl_debug_sel & RPL_LOG_NERR)\
                       _slog (sLogCtrl,_rpl_nerr_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

#define RPL_LOG_CNERR0(a) {\
                     if (rpl_debug_sel & RPL_LOG_NERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define RPL_LOG_CNERR1(a,b) {\
                     if (rpl_debug_sel & RPL_LOG_NERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define RPL_LOG_CNERR2(a,b,c) {\
                     if (rpl_debug_sel & RPL_LOG_NERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define RPL_LOG_CNERR3(a,b,c,d) {\
                     if (rpl_debug_sel & RPL_LOG_NERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** FLOW Macros *******************************/

#define RPL_LOG_FLOW0(a) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slog (sLogCtrl,_rpl_flow_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define RPL_LOG_FLOW1(a,b) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slog (sLogCtrl,_rpl_flow_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define RPL_LOG_FLOW2(a,b,c) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slog (sLogCtrl,_rpl_flow_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }
#define RPL_LOG_FLOW3(a,b,c,d) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slog (sLogCtrl,_rpl_flow_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }

#define RPL_LOG_CFLOW0(a) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define RPL_LOG_CFLOW1(a,b) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define RPL_LOG_CFLOW2(a,b,c) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define RPL_LOG_CFLOW3(a,b,c,d) {\
                     if (rpl_debug_sel & RPL_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
/************************************************************************/
#else /* Not debug */
/************************************************************************/

#define RPL_LOG_ERR0(a)
#define RPL_LOG_ERR1(a,b)
#define RPL_LOG_ERR2(a,b,c)
#define RPL_LOG_ERR3(a,b,c,d)
#define RPL_LOG_CERR0(a)
#define RPL_LOG_CERR1(a,b)
#define RPL_LOG_CERR2(a,b,c)
#define RPL_LOG_NERR0(a)
#define RPL_LOG_NERR1(a,b)
#define RPL_LOG_NERR2(a,b,c)
#define RPL_LOG_CNERR0(a)
#define RPL_LOG_CNERR1(a,b)
#define RPL_LOG_CNERR2(a,b,c)
#define RPL_LOG_FLOW0(a)
#define RPL_LOG_FLOW1(a,b)
#define RPL_LOG_FLOW2(a,b,c)
#define RPL_LOG_FLOW3(a,b,c,d)
#define RPL_LOG_CFLOW0(a)
#define RPL_LOG_CFLOW1(a,b)
#define RPL_LOG_CFLOW2(a,b,c)
#define RPL_LOG_CFLOW3(a,b,c,d)

#endif

/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern SD_CONST ST_CHAR *SD_CONST _ir_flow_logstr;
extern SD_CONST ST_CHAR *SD_CONST _ir_nerr_logstr;
extern SD_CONST ST_CHAR *SD_CONST _ir_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _ir_data_logstr;

#define IR_LOG_ERR0(a) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slog (sLogCtrl,_ir_err_logstr,\
                             thisFileName,__LINE__,a);\
                    }

#define IR_LOG_ERR1(a,b) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slog (sLogCtrl,_ir_err_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }

#define IR_LOG_ERR2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slog (sLogCtrl,_ir_err_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

#define IR_LOG_ERR3(a,b,c,d) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slog (sLogCtrl,_ir_err_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }

#define IR_LOG_CERR0(a) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define IR_LOG_CERR1(a,b) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define IR_LOG_CERR2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define IR_LOG_CERR3(a,b,c,d) {\
                     if (ir_debug_sel & IR_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** NERR Macros *************************************/

#define IR_LOG_NERR0(a) {\
                     if (ir_debug_sel & IR_LOG_NERR)\
                       _slog (sLogCtrl,_ir_nerr_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define IR_LOG_NERR1(a,b) {\
                     if (ir_debug_sel & IR_LOG_NERR)\
                       _slog (sLogCtrl,_ir_nerr_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define IR_LOG_NERR2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_NERR)\
                       _slog (sLogCtrl,_ir_nerr_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

#define IR_LOG_CNERR0(a) {\
                     if (ir_debug_sel & IR_LOG_NERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define IR_LOG_CNERR1(a,b) {\
                     if (ir_debug_sel & IR_LOG_NERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define IR_LOG_CNERR2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_NERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define IR_LOG_CNERR3(a,b,c,d) {\
                     if (ir_debug_sel & IR_LOG_NERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** FLOW Macros *******************************/

#define IR_LOG_FLOW0(a) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slog (sLogCtrl,_ir_flow_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define IR_LOG_FLOW1(a,b) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slog (sLogCtrl,_ir_flow_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define IR_LOG_FLOW2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slog (sLogCtrl,_ir_flow_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }
#define IR_LOG_FLOW3(a,b,c,d) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slog (sLogCtrl,_ir_flow_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }
#define IR_LOG_FLOW4(a,b,c,d,e) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slog (sLogCtrl,_ir_flow_logstr,\
                             thisFileName,__LINE__,a,b,c,d,e);\
                    }
#define IR_LOG_CFLOW0(a) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define IR_LOG_CFLOW1(a,b) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define IR_LOG_CFLOW2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define IR_LOG_CFLOW3(a,b,c,d) {\
                     if (ir_debug_sel & IR_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** DATA Macros *******************************/

#define IR_LOG_DATA0(a) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slog (sLogCtrl,_ir_data_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define IR_LOG_DATA1(a,b) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slog (sLogCtrl,_ir_data_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define IR_LOG_DATA2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slog (sLogCtrl,_ir_data_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }
#define IR_LOG_DATA3(a,b,c,d) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slog (sLogCtrl,_ir_data_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }
#define IR_LOG_DATA4(a,b,c,d,e) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slog (sLogCtrl,_ir_data_logstr,\
                             thisFileName,__LINE__,a,b,c,d,e);\
                    }
#define IR_LOG_CDATA0(a) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slogc (sLogCtrl,a);\
                    }
#define IR_LOG_CDATA1(a,b) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define IR_LOG_CDATA2(a,b,c) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define IR_LOG_CDATA3(a,b,c,d) {\
                     if (ir_debug_sel & IR_LOG_DATA)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/************************************************************************/
#else /* Not debug */
/************************************************************************/

#define IR_LOG_ERR0(a)
#define IR_LOG_ERR1(a,b)
#define IR_LOG_ERR2(a,b,c)
#define IR_LOG_ERR3(a,b,c,d)
#define IR_LOG_CERR0(a)
#define IR_LOG_CERR1(a,b)
#define IR_LOG_CERR2(a,b,c)
#define IR_LOG_NERR0(a)
#define IR_LOG_NERR1(a,b)
#define IR_LOG_NERR2(a,b,c)
#define IR_LOG_CNERR0(a)
#define IR_LOG_CNERR1(a,b)
#define IR_LOG_CNERR2(a,b,c)
#define IR_LOG_FLOW0(a)
#define IR_LOG_FLOW1(a,b)
#define IR_LOG_FLOW2(a,b,c)
#define IR_LOG_FLOW3(a,b,c,d)
#define IR_LOG_FLOW3(a,b,c,d,e)
#define IR_LOG_CFLOW0(a)
#define IR_LOG_CFLOW1(a,b)
#define IR_LOG_CFLOW2(a,b,c)
#define IR_LOG_CFLOW3(a,b,c,d)
#define IR_LOG_DATA0(a)
#define IR_LOG_DATA1(a,b)
#define IR_LOG_DATA2(a,b,c)
#define IR_LOG_DATA3(a,b,c,d)
#define IR_LOG_DATA3(a,b,c,d,e)
#define IR_LOG_CDATA0(a)
#define IR_LOG_CDATA1(a,b)
#define IR_LOG_CDATA2(a,b,c)
#define IR_LOG_CDATA3(a,b,c,d)

#endif

/************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* IR_INCLUDED */
/************************************************************************/
