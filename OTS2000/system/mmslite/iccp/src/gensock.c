/************************************************************************/
/* SOFTWARE MODULE HEADER ***********************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*          2001 - 2002, All Rights Reserved	   			*/
/*                                                                      */
/* MODULE NAME : gensock.c                                            	*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   general socket routines						*/
/*                                                                      */
/*                                                                      */
/* MODIFICATION LOG :                                                   */ 
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 03/25/02  MDE     13	   Updated copyright				*/
/* 03/21/02  MDE     12	   Disconnect handling, minor change		*/
/* 11/13/01  ask     11	   bug fix in sockWaitConnect			*/
/* 11/02/01  nav     10	   WIN32 changes				*/
/* 11/02/01  ask     09	   changed to while loop in sockWaitConnect	*/
/* 09/24/01  ask     08    checked for EADDRINUSE in sockBind           */
/* 09/10/01  nav     07    change while EINTR loop in send		*/
/*			   add sock state				*/
/* 08/21/01  ask     06    more catching of EINTR in sockThreadSend	*/
/* 08/17/01  ejv     05    Changed copyright to ABBNM.			*/
/* 08/16/01  ask     04    added errno.h, caught EINTR error in 	*/
/*                         sockThreadRecv				*/
/* 08/15/01  ejv     03    Added typecast.				*/
/* 08/07/01  ask,ejv 02    Ported to Tru64 UNIX				*/
/* 07/26/01  nav           creation		                        */
/************************************************************************/

#include "gensock.h"
#include <errno.h>

static char *thisFileName = __FILE__;

/************************************************************************/

#define GSOCK_LOG_ERR  	0x0001   
#define GSOCK_LOG_NERR 	0x0002   
#define GSOCK_LOG_FLOW 	0x0004   
ST_UINT  gsock_debug_sel;


SD_CONST ST_CHAR *SD_CONST _gsock_flow_logstr = "GSOCK_LOG_FLOW";
SD_CONST ST_CHAR *SD_CONST _gsock_nerr_logstr = "GSOCK_LOG_NERR";
SD_CONST ST_CHAR *SD_CONST _gsock_err_logstr  = "GSOCK_LOG_ERR";


static ST_VOID uDefHuntFun (GEN_SOCK *pSock, ST_INT *huntStateIo,
		            ST_CHAR *buf, ST_INT bufCount, ST_INT *lenOut);

#ifdef _WIN32
#define SOCK_EADDRINUSE WSAEADDRINUSE
#else
#define SOCK_EADDRINUSE EADDRINUSE
#endif

/************************************************************************/
/* unexposed function prototypes:					*/
/************************************************************************/

static ST_THREAD_RET ST_THREAD_CALL_CONV sockThreadRecv (ST_THREAD_ARG pArg);
static ST_THREAD_RET ST_THREAD_CALL_CONV sockThreadSend (ST_THREAD_ARG pArg);

static ST_RET sockWaitConnect (GEN_SOCK *pSock);
static ST_RET sockAddQueue (GEN_SOCK_QUE **ppQueue, ST_VOID *pBuf, ST_INT bufLen);
static ST_RET sockGetQueue (GEN_SOCK_QUE **ppQueue, ST_VOID **ppBuf, ST_INT *pBufLen);
static ST_VOID sockClearQueue (GEN_SOCK *pSock, ST_BOOLEAN sendQue);
static ST_VOID sockSleep (ST_LONG ms);

/************************************************************************/
/* sockInit: do the socket call						*/
/************************************************************************/

ST_RET sockInit (GEN_SOCK *pSock)
  {
SOCKET hSock;
#ifdef _WIN32
WSADATA	wsaData;	/* WinSock data	*/

/* init WinSock interface	*/
  if (WSAStartup(0x0101, &wsaData))
    {
    GSOCK_LOG_ERR0 ("Can't initialize WinSock interface");
    return (SD_FAILURE);
    }
#endif

  hSock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (hSock == INVALID_SOCKET)
    {
    GSOCK_LOG_ERR0 ("socket failure");
    return sockGetError ();
    }
  pSock->hSock = hSock;
  pSock->sockState = GS_STATE_IDLE;
  pSock->role = GS_ROLE_NONE;
  pSock->bStopRecv = SD_FALSE;
  pSock->bStopSend = SD_FALSE;

/* Set some reasonable defaults */
  pSock->hdrSize = sizeof (ST_INT32);
  pSock->sendSleepTime = 10;
  pSock->sendRetryTime = 50;
  pSock->bindRetryTime = 200;
  pSock->waitConnectSleepTime = 100;

  return SD_SUCCESS;
  }

/************************************************************************/
/* sockConnect: do the connect call					*/
/************************************************************************/

ST_RET sockConnect (GEN_SOCK *pSock, ST_UINT16 portNo, ST_CHAR *ipAddr)
  {
int err;
struct sockaddr_in sockAddrIn;
GEN_SOCK sockSave;

  sockAddrIn.sin_family = AF_INET;
  if (ipAddr)
    sockAddrIn.sin_addr.s_addr = inet_addr (ipAddr);
  sockAddrIn.sin_port = htons (portNo);

  pSock->role = GS_ROLE_CALLING;
  if (connect (pSock->hSock, (struct sockaddr *)&sockAddrIn, sizeof(sockAddrIn)) < 0)
    {
    err = sockGetError ();

#if defined (_WIN32)
    if (err == WSAEISCONN)
      {		/* We think we are connected, close it! */
      sockSave = *pSock;		/* Save user fields */
      sockClose (pSock);
      sockInit (pSock);

    /* Restore user fields */
      pSock->hdrSize 	      		= sockSave.hdrSize;
      pSock->sendSleepTime    		= sockSave.sendSleepTime;
      pSock->sendRetryTime 		= sockSave.sendRetryTime;
      pSock->bindRetryTime 		= sockSave.bindRetryTime;
      pSock->waitConnectSleepTime 	= sockSave.waitConnectSleepTime;
      }
#endif

#if defined (_WIN32)
    if (err != WSAEWOULDBLOCK)
#else
    if (err != EINPROGRESS)
#endif
      {
      GSOCK_LOG_FLOW1 ("socket connect error %d", err);
      return SD_FAILURE;
      }
    /* else the connect is pending  */
    pSock->sockState = GS_STATE_CONNECTING;
    }
  else	/* Connect successful immediately	*/
    {
    pSock->sockState = GS_STATE_CONNECTED;
    if (pSock->uSockConnect)
      (*pSock->uSockConnect)(pSock);
    }
  return SD_SUCCESS;
  }

/************************************************************************/
/* sockBind: do the bind call						*/
/************************************************************************/

ST_RET sockBind (GEN_SOCK *pSock, ST_UINT16 portNo)
  {
struct sockaddr_in localAddr;

  time_t currTime, startTime;
  localAddr.sin_family = AF_INET;
  localAddr.sin_port = htons(portNo);
  localAddr.sin_addr.s_addr = htonl (INADDR_ANY);
  if (bind (pSock->hSock, (struct sockaddr *)&localAddr, sizeof(localAddr)) < 0)
    return (SD_SUCCESS);

  if (errno != SOCK_EADDRINUSE)
    return sockGetError ();
  
  time(&startTime);
  time(&currTime);

  while(errno == SOCK_EADDRINUSE && (currTime < startTime + 60))
    {
    /* OS hasn't freed up the port yet, try for 60 seconds */ 
    sockSleep (pSock->bindRetryTime);
    if (bind (pSock->hSock, (struct sockaddr *)&localAddr, sizeof(localAddr)) == 0)
      return SD_SUCCESS;

    time(&currTime);
    }

  return sockGetError ();
  }

/************************************************************************/
/* sockListen: do the listen call and start thread to listen		*/
/************************************************************************/

static SOCKET hListenSock = INVALID_SOCKET;

ST_RET sockListen (GEN_SOCK *pSock)
  {
  if (hListenSock == INVALID_SOCKET)
    hListenSock = pSock->hSock;

  if (listen(hListenSock, SOMAXCONN) < 0)
    return sockGetError ();

  pSock->role = GS_ROLE_CALLED;
  pSock->sockState = GS_STATE_LISTENING;
  return SD_SUCCESS;
  }

/************************************************************************/
/* sockClose:								*/
/************************************************************************/

ST_RET sockClose (GEN_SOCK *pSock)
  {
#ifdef _WIN32
  if (closesocket (pSock->hSock) != 0)
#else
  if (close (pSock->hSock) != 0)
#endif
    return sockGetError ();

  pSock->sockState = GS_STATE_IDLE;
  pSock->role = GS_ROLE_NONE;
  return SD_SUCCESS;
  }

/************************************************************************/
/* sockStartRecv: create thread to receive data and queue it up		*/
/************************************************************************/

ST_RET sockStartRecv (GEN_SOCK *pSock)
  {
ST_RET rtnVal = SD_SUCCESS;

  if (pSock->thRecv == 0)
    {
    pSock->bStopRecv = SD_FALSE;
    rtnVal = gs_start_thread (sockThreadRecv, pSock, 
			      &pSock->thRecv, &pSock->tIdRecv);
    }
  return rtnVal;
  }

/************************************************************************/
/* sockStopRecv:							*/
/************************************************************************/

ST_RET sockStopRecv (GEN_SOCK *pSock)
  {
  pSock->bStopRecv = SD_TRUE;
  return SD_SUCCESS;
  }

/************************************************************************/
/* sockStartSend:							*/
/************************************************************************/

ST_RET sockStartSend (GEN_SOCK *pSock)
  {
ST_RET rtnVal = SD_SUCCESS;

  if (pSock->thSend == 0)
    {
    pSock->bStopSend = SD_FALSE;
    rtnVal = gs_start_thread (sockThreadSend, pSock, 
			      &pSock->thSend, &pSock->tIdSend);
    }

  return rtnVal;
  }


/************************************************************************/
/* sockStopSend:							*/
/************************************************************************/

ST_RET sockStopSend (GEN_SOCK *pSock)
  {
  pSock->bStopSend = SD_TRUE;
  return SD_SUCCESS;
  }

/************************************************************************/
/* sockSend: put data on the outbound queue				*/
/************************************************************************/

ST_RET sockSend (GEN_SOCK *pSock, ST_VOID *pBuf, ST_INT bufLen)
  {
ST_RET rtnVal;

  rtnVal = sockAddQueue (&pSock->sendQue, pBuf, bufLen);
  return rtnVal;
  }

/************************************************************************/
/* sockGetData: get data from the inbound queue				*/
/************************************************************************/

ST_RET sockGetData (GEN_SOCK *pSock, ST_VOID **ppBuf, ST_INT *pBufLen)
  {
ST_RET rtnVal;

  rtnVal = sockGetQueue (&pSock->recvQue, ppBuf, pBufLen);
  return rtnVal;
  }

/************************************************************************/
/* sockGetError:							*/
/************************************************************************/

ST_RET sockGetError (ST_VOID)
  {
#ifdef _WIN32
  return WSAGetLastError ();
#else
  return errno;
#endif
  }


/************************************************************************/
/************************************************************************/
/* sockThreadFunctions: separte threads to wait on recv/send		*/
/************************************************************************/
/************************************************************************/
/* sockThreadRecv: wait on recv then put em on the queue		*/
/************************************************************************/

static ST_THREAD_RET ST_THREAD_CALL_CONV sockThreadRecv (ST_THREAD_ARG pArg)
  {
GEN_SOCK *pSock;
ST_CHAR *pPkt;
ST_CHAR *rxDest;
ST_INT numRecv;
ST_INT totalSize;
ST_INT totalRcvd;
ST_INT bodySize;
ST_INT sockState;
ST_INT huntState;
ST_CHAR hdrBuf[GENSOCK_MAX_HEADER_SIZE];
ST_INT hdrRxCount;
ST_INT rxSize;

  pSock = (GEN_SOCK *) pArg;
  sockState = pSock->sockState;
  if (sockState == GS_STATE_CONNECTING)
    sockState = GS_STATE_IDLE; //??

  while (!pSock->bStopRecv)
    {
   /* We must wait for the socket to become connected */
   /* First wait until we are listening or connecting */
     if (pSock->sockState != GS_STATE_CONNECTED)
      {
      if (pSock->sockState != GS_STATE_LISTENING && 
          pSock->sockState != GS_STATE_CONNECTING)
        {
        sockSleep (pSock->waitConnectSleepTime);
	continue;
	}
   /* OK, we are listening or connecting ... */
      if (sockWaitConnect (pSock) != SD_SUCCESS)
	{
	GSOCK_LOG_NERR0 ("Socket wait connection failed");
	pSock->sockState = GS_STATE_IDLE;
        sockSleep (pSock->waitConnectSleepTime);
	continue;
	}
      }

  /* Now let's read the header and get the size of the buffer		*/
  /* Make sure user has hunt routine installed, or use default one	*/
    if (pSock->uSockHunt == NULL)
      {
      pSock->uSockHunt = uDefHuntFun;	/* Looks for simple 16 bit size header 	*/
      pSock->hdrSize = sizeof (ST_INT16);
      }
    if (pSock->hdrSize == 0)
      pSock->hdrSize = 1;	/* nibble hunt ... */


    huntState = GENSOCK_HUNT_START;
    hdrRxCount = 0;
    while (huntState != GENSOCK_HUNT_DONE && huntState != GENSOCK_HUNT_ERROR)
      {
      rxSize = pSock->hdrSize - hdrRxCount;
      numRecv = recv (pSock->hSock, &hdrBuf[hdrRxCount], rxSize, 0);
      if (numRecv > 0)	
        {
	hdrRxCount += rxSize;
	if (hdrRxCount == pSock->hdrSize)
	  {
          (*pSock->uSockHunt) (pSock, &huntState, hdrBuf, hdrRxCount, &bodySize);
	  hdrRxCount = 0;
  	  }
	}
      else
	{
	if (numRecv != 0)	/* 0 means orderly disconnect */
	  {
          if (WSAGetLastError () == WSAEINTR)
  	    continue;
          }

	/* Socket disconnected or a unrecoverable socket error */
	GSOCK_LOG_NERR1 ("Socket disconnect - errno = %d", errno);

        if (pSock->role == GS_ROLE_CALLED)
  	  pSock->sockState = GS_STATE_LISTENING;
	else
  	  pSock->sockState = GS_STATE_IDLE;
//          sockClose (pSock);

	if (pSock->uSockDisconnect)
	  (*pSock->uSockDisconnect) (pSock);
	sockClearQueue (pSock, SD_FALSE);
	break;
	}
      }
    if (huntState != GENSOCK_HUNT_DONE)
      continue;


  /* OK, the hunt is over and we have the length of the buffer to read	*/
    totalRcvd = pSock->hdrSize;
    totalSize = totalRcvd + bodySize;
    pPkt = chk_malloc (totalSize);
    memcpy (pPkt, hdrBuf, pSock->hdrSize);

    rxDest = pPkt + pSock->hdrSize;
    while (totalRcvd < totalSize)
      {
      numRecv = recv (pSock->hSock, rxDest, totalSize - totalRcvd, 0);
      if (numRecv <= 0)	
	{
	if(errno == EINTR)
	  continue;
	GSOCK_LOG_NERR0 ("Socket peer disconnect.");

        if (pSock->role == GS_ROLE_CALLED)
  	  pSock->sockState = GS_STATE_LISTENING;
	else
  	  pSock->sockState = GS_STATE_IDLE;
//          sockClose (pSock);

	if (pSock->uSockDisconnect)
	  (*pSock->uSockDisconnect) (pSock);

	sockClearQueue (pSock, SD_FALSE);
	break;
	}
      else
        {
	rxDest += numRecv;
	totalRcvd += numRecv;	
	}
      }	

  /* OK, we have hopefully received the buffer */    
    if (totalRcvd == totalSize)
      {
      sockAddQueue (&pSock->recvQue, pPkt, totalSize);
      if (pSock->uSockRx)
        (*pSock->uSockRx)(pSock);
      }
    else		/* Whoops, a disconnect ... */
      chk_free (pPkt);
    }

  pSock->thRecv = 0;
  if (pSock->uSockDisconnect)
    (*pSock->uSockDisconnect) (pSock);

  sockClearQueue (pSock, SD_FALSE);
  return ST_THREAD_RET_VAL;
  }

/************************************************************************/
/************************************************************************/


static ST_VOID uDefHuntFun (GEN_SOCK *pSock, ST_INT *huntStateIo,
		            ST_CHAR *buf, ST_INT bufCount, ST_INT *lenOut)
  {
ST_INT32 *pInt;

/* Better get our two bytes ... */
  if (bufCount != sizeof (ST_INT))
    {
    *huntStateIo = GENSOCK_HUNT_ERROR;
    return;
    }

  pInt = (ST_INT32 *) buf;
  *lenOut = ntohl (*pInt);
  *huntStateIo = GENSOCK_HUNT_DONE;
  }

/************************************************************************/
/* sockThreadSend: get em off the que and wait on send			*/
/************************************************************************/ 

static ST_THREAD_RET ST_THREAD_CALL_CONV sockThreadSend (ST_THREAD_ARG pArg)
  {
GEN_SOCK *pSock;
ST_CHAR *pPkt;
ST_INT numSent;
ST_INT pktLen;

  pSock = (GEN_SOCK *) pArg;
  while (!pSock->bStopSend)
    {
    if (sockGetQueue (&pSock->sendQue, &pPkt, &pktLen) == SD_SUCCESS)
      {
      while (SD_TRUE)
        {
        numSent = send (pSock->hSock, pPkt, pktLen, 0);
        if (numSent == pktLen) 	/* Send go OK? */
	  break;

      /* If we got interrupted, keep trying */
        if (numSent < 0 && errno == EINTR)
	  {
  	  sockSleep (pSock->sendRetryTime);
	  continue;
	  }

      /* OK, we have a unrecoverable send error on this socket */
        if (numSent > 0)
	  {	/* not expected to happen */
  	  GSOCK_LOG_NERR2 ("Socket: tried to send '%d', sent '%d'", 
  	  					pktLen, numSent);
	  }

	
	GSOCK_LOG_NERR0 ("Socket send error, clearing queue");
        if (pSock->role == GS_ROLE_CALLED)
  	  pSock->sockState = GS_STATE_LISTENING;
	else
  	  pSock->sockState = GS_STATE_IDLE;
//          sockClose (pSock);

	if (pSock->uSockDisconnect)
	  (*pSock->uSockDisconnect) (pSock);

	sockClearQueue (pSock, SD_TRUE);
	break;
        }


     /* Tell the user that we tried to send the packet */
      if (pSock->uSockTx)
        (*pSock->uSockTx)(pSock, numSent == pktLen ? SD_SUCCESS : SD_FAILURE);

      chk_free (pPkt);
      }
    else	/* Nothing to send ... take a nap */
      sockSleep (pSock->sendSleepTime);
    }

  sockClearQueue (pSock, SD_TRUE);
  pSock->thSend = 0;
  return ST_THREAD_RET_VAL;
  }

/************************************************************************/
/* sockWaitConnect: wait for a connection				*/
/************************************************************************/

static ST_RET sockWaitConnect (GEN_SOCK *pSock)
  {
fd_set readfds;
fd_set writefds;
fd_set exceptfds;
ST_INT nfds;
SOCKET hNewSock;

  /* Setup and call "select"	*/
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);

  if (pSock->sockState == GS_STATE_CONNECTING)
    {
    FD_SET(pSock->hSock, &writefds);
    FD_SET(pSock->hSock, &exceptfds);

    /* Do "blocking" select call.		*/
    nfds = pSock->hSock+1;    /* hSock is only fds to check, so set "nfds=hSock+1".*/
    nfds = select (nfds, &readfds, &writefds, &exceptfds, NULL);

    if (nfds > 0)
      {
      if (FD_ISSET(pSock->hSock, &writefds))
	{
	pSock->sockState = GS_STATE_CONNECTED;
        if (pSock->uSockConnect)
          (*pSock->uSockConnect)(pSock);
        return SD_SUCCESS;
	}
      }
    }

  while (pSock->sockState == GS_STATE_LISTENING)
    {
    FD_SET (hListenSock, &readfds);
    nfds = hListenSock + 1;   
    nfds = select (nfds, &readfds, NULL, NULL, NULL);
    if (nfds > 0)
      {		
      hNewSock = accept(hListenSock, NULL, NULL);
      
      if (hNewSock == INVALID_SOCKET)
	return sockGetError ();
      else
	{
	pSock->hSock = hNewSock;
	pSock->sockState = GS_STATE_CONNECTED;
        if (pSock->uSockConnect)
          (*pSock->uSockConnect)(pSock);
	return SD_SUCCESS;
	}
      }
    }

  return (SD_FAILURE);	
  }


/************************************************************************/
/* Socket Queue functions:						*/
/************************************************************************/

/************************************************************************/
/* sockAddQueue:							*/
/************************************************************************/

static ST_RET sockAddQueue (GEN_SOCK_QUE **ppQueue, ST_VOID *pBuf, ST_INT bufLen)
  {
GEN_SOCK_QUE *pQItem;
ST_RET rtnVal;

  pQItem = chk_malloc (sizeof (GEN_SOCK_QUE));
  pQItem->pBuf = pBuf;
  pQItem->bufLen = bufLen;

  S_LOCK_COMMON_RESOURCES ();
  rtnVal = list_add_last (ppQueue, pQItem);
  S_UNLOCK_COMMON_RESOURCES ();

  return rtnVal;
  }

/************************************************************************/
/* sockGetQueue:  get an item off the queue				*/
/************************************************************************/

static ST_RET sockGetQueue (GEN_SOCK_QUE **ppQueue, ST_VOID **ppBuf, ST_INT *pBufLen)
  {
ST_RET rtnVal;
GEN_SOCK_QUE *pQItem = NULL;

  S_LOCK_COMMON_RESOURCES ();
  if (*ppQueue)
    pQItem = list_get_first (ppQueue);
  S_UNLOCK_COMMON_RESOURCES ();

  if (pQItem)
    {
    *ppBuf = pQItem->pBuf;
    *pBufLen = pQItem->bufLen;
    chk_free (pQItem);
    rtnVal = SD_SUCCESS;
    }
  else
    rtnVal = SD_FAILURE;

  return rtnVal;
  }

/************************************************************************/
/* sockClearQueue:  free up the queue					*/
/************************************************************************/

static ST_VOID sockClearQueue (GEN_SOCK *pSock, ST_BOOLEAN sendQue)
  {
GEN_SOCK_QUE *pQItem = NULL;

  S_LOCK_COMMON_RESOURCES ();

  if (sendQue)
    {
    while (pSock->sendQue != NULL)
      {
      pQItem = list_get_first (&pSock->sendQue);

      if (pSock->uSockTx)
        (*pSock->uSockTx)(pSock, SD_FAILURE);

      chk_free (pQItem->pBuf);
      chk_free (pQItem);
      }
    }
  else
    {
    while (pSock->recvQue != NULL)
      {
      pQItem = list_get_first (&pSock->recvQue);

      if (pSock->uSockRx)
        (*pSock->uSockRx)(pSock);

      chk_free (pQItem->pBuf);
      chk_free (pQItem);
      }
    }


  S_UNLOCK_COMMON_RESOURCES ();
  }

/************************************************************************/
/* sockSleep: sleep for 100 msec					*/
/************************************************************************/

static ST_VOID sockSleep (ST_LONG ms)
  {
#ifdef _WIN32
  Sleep (ms);
#else
struct timespec rqtp;

  rqtp.tv_sec = 0;
  rqtp.tv_nsec =  ms *  1000000;    /* 1000 ms/us, 1000 us/ns ... 	*/
  nanosleep (&rqtp, NULL);
#endif
  }
