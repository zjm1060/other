/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_imsg.c                                            	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Transfer group functions					*/
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 06/20/02  RKR    6      Copyright year update                        */
/* 06/18/02  RKR    5      Changes for tracking outstanding MMS Reqs	*/
/* 01/09/02  ASK    4	   Added enableInformationMessages		*/
/* 01/08/02  ASK    3      Added code for max size of info msgs		*/
/* 11/21/01  ASK    2      cleanup for mi lib changes			*/
/* 04/19/00  RKR    1      Created                                      */
/************************************************************************/

#include "mit_main.h"
#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"
#include <sys/timeb.h>

/************************************************************************/
/* static function prototypes						*/
/************************************************************************/


/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/

static char *thisFileName = __FILE__;

/************************************************************************/
/*			  mic_startIMTransferDone			*/
/************************************************************************/

static ST_VOID mic_startIMTransferDone(MI_REQ_CTRL *mi_req)
  {
MIT_ASSOC_INFO *pstMitAssoc;

  if (mi_req->result != SD_SUCCESS)
    {
    printf ("\n Error in Start IM Transfer done: %d", mi_req->result);
    }
/*  pstMitAssoc = (Tag_MIT_ASSOC_INFO *)mi_req->user_info[0];
modified 2006.11.24 zy*/
  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)mi_req->user_info[0];
  incReqCapacity(pstMitAssoc);
  /* in case there is only one outstanding request check to see if there */
  /* are DataSets to create */
  if (pstMitAssoc->pstMitMicVcc->numDataSets)
    createDataSets (pstMitAssoc);
  mi_free_req_ctrl (mi_req);
  }

/************************************************************************/
/*			 enableInformationMessages			*/
/************************************************************************/

ST_VOID    enableInformationMessages (MIT_ASSOC_INFO *pstAssociationInfo)
  {
MI_REQ_CTRL *mi_req = NULL;
  
  if (hasReqCapacity(pstAssociationInfo)== SD_TRUE)
    {
    mi_req = mic_start_IMTransfer (pstAssociationInfo->pstAssocCtrl->mi_conn);
    if (mi_req == NULL)
      {
      MIT_LOG_FLOW1 ("Start IMTransfer failed (0x%04x)", mi_op_err);
      }
    else
      {
      mi_req->user_info[0] = pstAssociationInfo;
      decReqCapacity(pstAssociationInfo);
      MIT_LOG_FLOW0 ( "Sent start of IMTransfer OK");
      mi_req->u_req_done = mic_startIMTransferDone;

      }
    }
  }

/************************************************************************/
/*			 startInformationMessages			*/
/************************************************************************/

ST_VOID    startInformationMessages (ST_VOID)
  {
MI_REQ_CTRL *mi_req;
MIT_ASSOC_INFO	*pstAssociationInfo;
ST_CHAR buffer[256];

  pstAssociationInfo = getAssociationName (buffer);
  if (pstAssociationInfo==NULL)
    {
    return;
    }

  if ((pstAssociationInfo->pstAssocCtrl->state & MI_ASSOC_STATE_ACTIVE)&&
      (hasReqCapacity(pstAssociationInfo))==SD_TRUE)
    {
    mi_req = mic_start_IMTransfer (pstAssociationInfo->pstAssocCtrl->mi_conn);
    if (mi_req==NULL)
      {
      MIT_LOG_FLOW1 ("Start IMTransfer failed (0x%04x)", mi_op_err);
      }
    else
      {
      MIT_LOG_FLOW0 ( "Sent start of IMTransfer OK");
      mi_req->u_req_done = mic_startIMTransferDone;
      decReqCapacity(pstAssociationInfo);
	  mi_req->user_info[0] = pstAssociationInfo;
      }
    }
  else
    {
    printf ( "\n Association not ACTIVE.");
    pause_msg ("\n Press any key to continue");
    }
  }

/************************************************************************/
/*			  mic_stopIMTransferDone			*/
/************************************************************************/

static ST_VOID mic_stopIMTransferDone(MI_REQ_CTRL *mi_req)
  {
  if (mi_req->result != SD_SUCCESS)
    {
    printf ("\n Error in Stop IM Transfer: %d ",mi_req->result);
    }
  incReqCapacity((MIT_ASSOC_INFO *)mi_req->user_info[0]);
  mi_free_req_ctrl (mi_req);
  }

/************************************************************************/
/*			 stopInformationMessages 			*/
/************************************************************************/

ST_VOID    stopInformationMessages (ST_VOID)
  {
MI_REQ_CTRL *mi_req;
MIT_ASSOC_INFO	*pstAssociationInfo;
ST_CHAR buffer[256];

  pstAssociationInfo = getAssociationName (buffer);
  if (pstAssociationInfo==NULL)
    {
    return;
    }

  if ((pstAssociationInfo->pstAssocCtrl->state & MI_ASSOC_STATE_ACTIVE)&&
      (hasReqCapacity(pstAssociationInfo)==SD_TRUE))
    {
    mi_req = mic_stop_IMTransfer (pstAssociationInfo->pstAssocCtrl->mi_conn);
    if (mi_req==NULL)
      {
      MIT_LOG_FLOW1 ("Sending Stop IMTransfer failed (0x%04x)", mi_op_err);
      }
    else
      {
      decReqCapacity(pstAssociationInfo);
      mi_req->user_info[0] = pstAssociationInfo;
      MIT_LOG_FLOW0 ( "Sent Stop of IMTransfer OK");
      mi_req->u_req_done = mic_stopIMTransferDone;
      }
    }
  else
    {
    printf ( "\n Association not ACTIVE.");
    pause_msg ("\n Press any key to continue");
    }
  }

/************************************************************************/
/* 		          sendInformationMessage 			*/
/************************************************************************/

ST_VOID    sendInformationMessage (ST_VOID)
  {
ST_CHAR buffer[256];
MIT_ASSOC_INFO	*pstAssociationInfo;
MI_INFO_BUF_HEADER buf_header;
ST_UCHAR *info_buf;
ST_INT buf_len;
ST_INT msg_scope = VCC_SPEC;	 /* or ICC_SPEC */
ST_RET ret;
MI_CONN **connArray;
int arrayIndex;
MIT_MIS_VCC	  *pstMitMisVcc;
ST_INT	infoRef;
ST_BOOLEAN found;
MIT_INFO_MSG *pstMitInfoMsg;

  /* Get the list of MI_ASSOC_CTRLS */

  connArray = (struct _mi_conn **)chk_calloc (list_get_sizeof (pstAssocInfoSet), 
  			  sizeof (MI_CONN *));

  pstAssociationInfo = getAssociationName (buffer);
  arrayIndex = 0;
  connArray[arrayIndex++] = pstAssociationInfo->pstAssocCtrl->mi_conn;
  /* Get a ptr to the Remote so we can ask later for the msgID */
  pstMitMisVcc = getRemoteMitMisVcc(pstAssociationInfo-> remoteName);

  if (pstMitMisVcc!=NULL)
    {
    printf ( "\n Enter infoRef: ");
    intget (&infoRef);

  /* see if we entered a valid number */
    pstMitInfoMsg = pstMitMisVcc->pstInfoMsgs; /* look at the last VCC */
    found = SD_FALSE;
    while ((found==SD_FALSE) && (pstMitInfoMsg!=NULL))
      {
      if (pstMitInfoMsg->infoRef == infoRef)
        found = SD_TRUE;
      else
        pstMitInfoMsg = (struct Tag_MIT_INFO_MSG *)list_get_next(pstMitMisVcc->pstInfoMsgs,pstMitInfoMsg);
      }

    if ( found==SD_TRUE)
      {
if (infoRef == 10100) {
char buffer[64], *c;
int i;
unsigned char flag;
struct timeb zeit;
ST_BOOLEAN dataEntered;
c = &buffer[0];
i = 64;
while (i--) *c++ = '\0';
info_buf = (unsigned char *)chk_malloc(pstMitInfoMsg->maxSize);
buf_len = 44;
buf_header.InfoReference = infoRef;
buf_header.LocalReference = 1;
pstMitInfoMsg->msgID++;
buf_header.MessageId = pstMitInfoMsg->msgID;
buf_header.Size = 44;
*info_buf = 1;
*(info_buf + 1) = 42;
printf("\n Enter Signal Name for Soe Message: ", buffer);
dataEntered = strget(buffer);
printf("%s\n", buffer);
if (dataEntered) {
  c = (char *)(info_buf + 2);
  for (i = 0; i < 32; ++i) *c++ = ' ';
  c = buffer;
  i = 2;
  while (*c) {
    if (*c > 0x20) info_buf[i] = *c; else break;
    ++i;
    ++c;
  }
  info_buf[34] = 0;
  printf("\n Enter Flag: ");
  input_hex(&flag, 1);
  info_buf[35] = flag;
  info_buf[36] = 0;
  info_buf[37] = 0;
  ftime(&zeit);
  info_buf[38] = ((zeit.time & 0xff000000) >> 24);
  info_buf[39] = ((zeit.time & 0x00ff0000) >> 16);
  info_buf[40] = ((zeit.time & 0x0000ff00) >> 8);
  info_buf[41] = zeit.time & 0x000000ff;
  info_buf[42] = ((zeit.millitm & 0xff00) >> 8);
  info_buf[43] = zeit.millitm & 0x00ff;
  ret = mis_send_IMTransfer (arrayIndex, connArray, &buf_header, buf_len, 
  				info_buf, msg_scope);
  if (ret != SD_SUCCESS)
    printf ("\nFailure Sending Message: '%s'", info_buf);
  else {
    printf ("\nSent message: '%s'", info_buf);
printf("\nSoE Message:\n");
printf("  Info Reference:  %d\n", infoRef);
printf("  Local Reference: %d\n", buf_header.LocalReference);
printf("  MessageID:       %d\n", buf_header.MessageId);
printf("  Size:            %d\n", buf_header.Size);
for (i = 0; i <  44; ++i) {
  printf(" %02x", info_buf[i]);
  if (i % 16 == 0) printf("\n");
}
printf("\n");
printf("  TimeStamp:       %s - %d\n", ctime(&zeit.time), zeit.millitm);
pause_msg("\nType any key to continue ... ");
}
}
} else if (infoRef == 10101) {
int i;
info_buf = (unsigned char *)chk_malloc(pstMitInfoMsg->maxSize);
buf_len = 1;
buf_header.InfoReference = infoRef;
buf_header.LocalReference = 1;
pstMitInfoMsg->msgID++;
printf("\n Enter MessageId: ");
intget(&i);
buf_header.MessageId = i;
buf_header.Size = 1;
printf("\n Enter Acknowledge Value: ");
intget(&i);
*info_buf = i;
  ret = mis_send_IMTransfer (arrayIndex, connArray, &buf_header, buf_len, 
  				info_buf, msg_scope);
  if (ret != SD_SUCCESS)
    printf ("\nFailure Sending Message: '%s'", info_buf);
  else
    printf ("\nSent message: '%s'", info_buf);
} else {
      info_buf = (unsigned char *)chk_malloc(pstMitInfoMsg->maxSize);
      buf_len = pstMitInfoMsg->maxSize;
      sprintf ((ST_CHAR *) info_buf, "This is an Information Message #%d", infoRef);
      buf_header.InfoReference = infoRef;
      buf_header.LocalReference = pstMitInfoMsg->localRef;
      pstMitInfoMsg->msgID++;
      buf_header.MessageId = pstMitInfoMsg->msgID;
      buf_header.Size = strlen ((const char *)info_buf) + 1;
      ret = mis_send_IMTransfer (arrayIndex, connArray, &buf_header, buf_len, 
      				info_buf, msg_scope);
      if (ret != SD_SUCCESS)
        printf ("\nFailure Sending Message: '%s'", info_buf);
      else
        printf ("\nSent message: '%s'", info_buf);
}
      
      chk_free(info_buf);
      }
    else
      {
      printf ("\n InfoMsg %d not found", infoRef);
      pause_msg ("\n Press any key to continue");
      }
    }

  chk_free (connArray);
  (*menu_set_fun)();
  }

/************************************************************************/
/*			u_mis_stop_IM_Transfer				*/
/* notification that the transfer set has been disabled by the client	*/
/************************************************************************/

ST_VOID u_mis_stop_IM_Transfer (MI_CONN *mi_conn)
  {
  printf ("\n IM_Transfer_Set disabled for remote AR %s", mi_conn->remote_ar);
  }

/************************************************************************/
/*			u_mis_start_IM_Transfer				*/
/* notification that the transfer set has been enabled by the client	*/
/************************************************************************/

ST_VOID u_mis_start_IM_Transfer (MI_CONN *mi_conn)
  {
  printf ("\n IM_Transfer_Set enabled for remote AR %s", mi_conn->remote_ar);
  }


/************************************************************************/
/* u_mic_receive_IMTransfer: called when an Information Message is recd*/
/************************************************************************/

ST_VOID u_mic_receive_IMTransfer (MI_CONN *mi_conn,
				 MI_INFO_BUF_HEADER *buf_header, 
			         ST_UCHAR *info_buf, ST_INT msg_scope)
  {
  printf ("\n Incoming IMTransfer Report:  ");
  printf ("\n\t InfoReference = %d", buf_header->InfoReference);
  printf ("\n\t LocalReference = %d", buf_header->LocalReference);
  printf ("\n\t MessageId = %d", buf_header->MessageId);
  printf ("\n\t Size = %d", buf_header->Size);
  printf ("\n\t Message = %s", info_buf);
  }

/************************************************************************/
/*			    displayRemoteInfoMsgs			*/
/************************************************************************/

ST_VOID displayRemoteInfoMsgs (ST_VOID)
  {
ST_CHAR 	buffer[256];
MIT_MIS_VCC 	*pstMitMisVcc;
MIT_INFO_MSG 	*pstCurMisVccInfoMsg;
ST_CHAR		*strScope;
ST_INT		line;

  pstMitMisVcc = getRemoteMitMisVcc (buffer);
  printf ("\n\n");
  if (pstMitMisVcc)
    {						     
    pstCurMisVccInfoMsg = pstMitMisVcc->pstInfoMsgs;
    printf ("\n Local Domain: '%s' BLTID: '%s'\n",
             pstMitMisVcc->localDomainName,
	     pstMitMisVcc->bilateralTableId);
    line = 1;
    while (pstCurMisVccInfoMsg)
      {
      strScope = iccp_scope_to_str(pstCurMisVccInfoMsg->msgScope);
      printf ("\n InfoMessage Ref#: %d Scope: %s maximum size: %d", 
              pstCurMisVccInfoMsg->infoRef, strScope,
              pstCurMisVccInfoMsg->maxSize);
      line++;
      if (line == 22)
        {
        pause_msg ("\n\n Press any key to continue");
        printf ("\n Local Domain: '%s' BLTID: '%s'\n",
                pstMitMisVcc->localDomainName,
                pstMitMisVcc->bilateralTableId);
        line =1;
        }
      pstCurMisVccInfoMsg = (struct Tag_MIT_INFO_MSG *)list_get_next (pstMitMisVcc->pstInfoMsgs,
                                   pstCurMisVccInfoMsg);
      }
	if (line!= 1 )
		pause_msg ("\n\n Press any key to continue");
    }
  else
    {
    printf ("\n Remote '%s' not found", buffer);
    pause_msg (" ");
    }			
  (*menu_set_fun)();
  }
