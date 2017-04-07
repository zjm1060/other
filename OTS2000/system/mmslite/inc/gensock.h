/************************************************************************/
/* SOFTWARE MODULE HEADER ***********************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*          2001 - 2002, All Rights Reserved	   			*/
/*									*/
/* MODULE NAME : gensock.h 						*/
/* PRODUCT(S)  : General Sockets Interface				*/
/*									*/
/*	    								*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 03/25/02  MDE    05	   Updated copyright				*/
/* 02/28/01  kcr    04	   Changed glbtypes.h and sysincs.h ordering	*/
/* 09/10/01  nav    03	   Add sockState				*/
/* 08/17/01  ejv    02     Changed copyright to ABBNM.			*/
/* 07/26/01  nav    01     Created					*/
/************************************************************************/

#ifndef GENSOCK_INCLUDED
#define GENSOCK_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif


#include "glbtypes.h"
#include "sysincs.h"
#include "gen_list.h"
#include "glbsem.h"
#include "mem_chk.h"

#ifdef _WIN32
#include <windows.h>		/* HANDLE def, etc.			*/
#include <winsock.h>
#include <process.h>		/* for _beginthread, _endthread	*/
#else
// guessing at these
#include <signal.h>		/* for "kill", etc.		*/
#include <sys/socket.h>
#include <netinet/in.h>		/* IPPROTO_*, etc.			*/
#endif

/************************************************************************/

#define GSOCK_LOG_ERR  	0x0001   
#define GSOCK_LOG_NERR 	0x0002   
#define GSOCK_LOG_FLOW 	0x0004   
extern ST_UINT  gsock_debug_sel;

/************************************************************************/
/* defines:								*/
/************************************************************************/

#ifndef _WIN32
#define SOCKET	int
#define INVALID_SOCKET	(-1)
#endif

/************************************************************************/
/* data structures:							*/
/************************************************************************/

#define	GENSOCK_MAX_HEADER_SIZE	20

typedef struct tag_GEN_SOCK_QUE
  {
  DBL_LNK   l;
  ST_VOID   *pBuf;	/* malloc'd				*/
  ST_INT  bufLen;

/* User fields */
  ST_VOID	   *usr1;
  ST_VOID	   *usr2;
  } GEN_SOCK_QUE;

#define GS_ROLE_NONE	    1
#define GS_ROLE_CALLING	    1
#define GS_ROLE_CALLED	    2

#define GS_STATE_IDLE	    0
#define GS_STATE_CONNECTING 1
#define GS_STATE_LISTENING  2
#define GS_STATE_CONNECTED  3

/* Hunt state defines */
#define GENSOCK_HUNT_START	1	
#define GENSOCK_HUNT_CHAR  	2	
#define GENSOCK_HUNT_CONTINUE	3
#define GENSOCK_HUNT_DONE	4
#define GENSOCK_HUNT_ERROR	5

typedef struct tag_GEN_SOCK
  {
/* Socket state */
  ST_INT	   sockState;
  ST_INT	   role;		/*

/* Config parameters */
  ST_INT	   hdrSize;		/* Default sizeof (ST_INT)	*/
  ST_LONG 	   sendSleepTime;	/* Default 10ms			*/
  ST_LONG 	   sendRetryTime;	/* Default 20ms			*/
  ST_LONG 	   bindRetryTime;	/* Default 100ms 		*/
  ST_LONG 	   waitConnectSleepTime;/* Default 100ms 		*/

/* User callbacks */
  ST_VOID (*uSockConnect) (struct tag_GEN_SOCK *pSock);
  ST_VOID (*uSockDisconnect) (struct tag_GEN_SOCK *pSock);
  ST_VOID (*uSockHunt) (struct tag_GEN_SOCK *pSock, ST_INT *huntStateIo,
		            ST_CHAR *buf, ST_INT bufCount, ST_INT *lenOut);
  ST_VOID (*uSockRx) (struct tag_GEN_SOCK *pSock);
  ST_VOID (*uSockTx) (struct tag_GEN_SOCK *pSock, ST_RET result);

/* User fields */
  ST_VOID	   *usr1;
  ST_VOID	   *usr2;
  ST_VOID	   *usr3;
  ST_VOID	   *usr4;

/* Internal */
  SOCKET	   hSock;		
  GEN_SOCK_QUE	   *recvQue;		
  ST_BOOLEAN	   bStopRecv;
  ST_THREAD_HANDLE thRecv;
  ST_THREAD_ID	   tIdRecv;

  GEN_SOCK_QUE	   *sendQue;
  ST_BOOLEAN	   bStopSend;
  ST_THREAD_HANDLE thSend;
  ST_THREAD_ID	   tIdSend;
  } GEN_SOCK;


/************************************************************************/
/* prototypes:								*/
/************************************************************************/


ST_RET sockInit (GEN_SOCK *pSock);
ST_RET sockConnect (GEN_SOCK *pSock, ST_UINT16 portNo, ST_CHAR *ipAddr);
ST_RET sockBind (GEN_SOCK *pSock, ST_UINT16 portNo);
ST_RET sockListen (GEN_SOCK *pSock);	/* this guy waits */
ST_RET sockClose (GEN_SOCK *pSock);

ST_RET sockStartRecv (GEN_SOCK *pSock);
ST_RET sockStopRecv (GEN_SOCK *pSock);
ST_RET sockStartSend (GEN_SOCK *pSock);
ST_RET sockStopSend (GEN_SOCK *pSock);

ST_RET sockSend (GEN_SOCK *pSock, ST_VOID *pBuf, ST_INT bufLen);
ST_RET sockGetData (GEN_SOCK *pSock, ST_VOID **ppBuf, ST_INT *pBufLen);

ST_RET sockGetError (ST_VOID);



/************************************************************************/
/************************************************************************/
/* INTERNAL                                                             */
/************************************************************************/
#ifdef DEBUG_SISCO
/************************************************************************/

/* Log type strings */
extern SD_CONST ST_CHAR *SD_CONST _gsock_flow_logstr;
extern SD_CONST ST_CHAR *SD_CONST _gsock_nerr_logstr;
extern SD_CONST ST_CHAR *SD_CONST _gsock_err_logstr;


#define GSOCK_LOG_ERR0(a) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slog (sLogCtrl,_gsock_err_logstr,\
                             thisFileName,__LINE__,a);\
                    }

#define GSOCK_LOG_ERR1(a,b) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slog (sLogCtrl,_gsock_err_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }

#define GSOCK_LOG_ERR2(a,b,c) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slog (sLogCtrl,_gsock_err_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

#define GSOCK_LOG_ERR3(a,b,c,d) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slog (sLogCtrl,_gsock_err_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }

#define GSOCK_LOG_CERR0(a) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slogc (sLogCtrl,a);\
                    }
#define GSOCK_LOG_CERR1(a,b) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define GSOCK_LOG_CERR2(a,b,c) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define GSOCK_LOG_CERR3(a,b,c,d) {\
                     if (gsock_debug_sel & GSOCK_LOG_ERR)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }

/*********************** NERR Macros *************************************/

#define GSOCK_LOG_NERR0(a) {\
                     if (gsock_debug_sel & GSOCK_LOG_NERR)\
                       _slog (sLogCtrl,_gsock_nerr_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define GSOCK_LOG_NERR1(a,b) {\
                     if (gsock_debug_sel & GSOCK_LOG_NERR)\
                       _slog (sLogCtrl,_gsock_nerr_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define GSOCK_LOG_NERR2(a,b,c) {\
                     if (gsock_debug_sel & GSOCK_LOG_NERR)\
                       _slog (sLogCtrl,_gsock_nerr_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }

/*********************** FLOW Macros *******************************/

#define GSOCK_LOG_FLOW0(a) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slog (sLogCtrl,_gsock_flow_logstr,\
                             thisFileName,__LINE__,a);\
                    }
#define GSOCK_LOG_FLOW1(a,b) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slog (sLogCtrl,_gsock_flow_logstr,\
                             thisFileName,__LINE__,a,b);\
                    }
#define GSOCK_LOG_FLOW2(a,b,c) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slog (sLogCtrl,_gsock_flow_logstr,\
                             thisFileName,__LINE__,a,b,c);\
                    }
#define GSOCK_LOG_FLOW3(a,b,c,d) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slog (sLogCtrl,_gsock_flow_logstr,\
                             thisFileName,__LINE__,a,b,c,d);\
                    }

#define GSOCK_LOG_CFLOW0(a) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slogc (sLogCtrl,a);\
                    }
#define GSOCK_LOG_CFLOW1(a,b) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b);\
                    }
#define GSOCK_LOG_CFLOW2(a,b,c) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c);\
                    }
#define GSOCK_LOG_CFLOW3(a,b,c,d) {\
                     if (gsock_debug_sel & GSOCK_LOG_FLOW)\
                       _slogc (sLogCtrl,a,b,c,d);\
                    }
/************************************************************************/
#else /* Not debug */
/************************************************************************/

#define GSOCK_LOG_ERR0(a)
#define GSOCK_LOG_ERR1(a,b)
#define GSOCK_LOG_ERR2(a,b,c)
#define GSOCK_LOG_ERR3(a,b,c,d)
#define GSOCK_LOG_CERR0(a)
#define GSOCK_LOG_CERR1(a,b)
#define GSOCK_LOG_CERR2(a,b,c)
#define GSOCK_LOG_NERR0(a)
#define GSOCK_LOG_NERR1(a,b)
#define GSOCK_LOG_NERR2(a,b,c)
#define GSOCK_LOG_FLOW0(a)
#define GSOCK_LOG_FLOW1(a,b)
#define GSOCK_LOG_FLOW2(a,b,c)
#define GSOCK_LOG_FLOW3(a,b,c,d)
#define GSOCK_LOG_CFLOW0(a)
#define GSOCK_LOG_CFLOW1(a,b)
#define GSOCK_LOG_CFLOW2(a,b,c)
#define GSOCK_LOG_CFLOW3(a,b,c,d)

#endif


/************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
