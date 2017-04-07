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
/* 12/20/06  ZY     8      Transplanted to H9000 application            */
/* 07/30/03  ZY     7      Power Schedule added                         */
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

#include "cppinc.h"
#include "../inc/iccp_main.h"

/************************************************************************/
/* static function prototypes						*/
/************************************************************************/


/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/

static char *thisFileName = __FILE__;

/************************************************************************/
/* External variables                   				*/
/************************************************************************/

extern char              strPrtBuf[256];

extern ST_INT  nAssocNum[4];           /* Added for save association number for remotes 2003.7.30 zy */

extern sGWAssoc          sAssocDat[4][2];   /* Changed to two dimension for 2 association in one remote */
    /* sAssocDat[0] - Cascade Dispatch Center
       sAssocDat[1] - National Dispatch Center
       sAssocDat[2] - Central China Dispatch Center */
extern ST_INT DC_cnt;	/*remote dispatch center num */

char cAck_nDst;	/*added for ack schedule information 2003.7.12 zy */
extern	char	infobuf[1024];

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
/*# start insertion ##########################################################*/
ST_UCHAR *cd;
ST_UCHAR *cs;
ST_BOOLEAN dataEntered;
ST_CHAR date[256];
ST_CHAR filetype[256];
ST_CHAR ind_name[256];
ST_INT lng;
ST_LONG n;
ST_LONG num;
ST_UCHAR s;
ST_CHAR schedule[256];
ST_LONG ltime;
struct timeb timebuffer;
char *timeline;
/*# end insertion ############################################################*/
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

  connArray = (MI_CONN **)chk_calloc (list_get_sizeof (pstAssocInfoSet), 
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
      info_buf = (unsigned char *)chk_malloc(pstMitInfoMsg->maxSize + 1);
      buf_len = pstMitInfoMsg->maxSize + 1;

      if (infoRef == INFOMSG_REF_SOE) {
        /*---- soe message ----*/
        lng = buf_len;
        cd = info_buf;
        while (lng--) *cd++ = 0x00;
        printf("\n Enter Indication Name: ");
        dataEntered = strget(ind_name);
        if (dataEntered) {
          *info_buf = 0x01;
          *(info_buf + 1) = 0x2a; /* = 42 */
          lng = strlen(ind_name);
          cs = (unsigned char *)ind_name;
          cd = info_buf + 2;
          while (lng--) *cd++ = *cs++;
          *(info_buf + 34) = 0x00;
          s = 0x00;
          input_hex(&s, 1);
          *(info_buf + 34) = 0x00;
          *(info_buf + 35) = s;
          *(info_buf + 36) = 0x00;
          *(info_buf + 37) = 0x00;
/*
          time(&ltime);
          cd = info_buf + 38;
          *cd++ = (ST_UCHAR *)((ltime >> 24) & 0x000000ff);
          *cd++ = (ST_UCHAR *)((ltime >> 16) & 0x000000ff);
          *cd++ = (ST_UCHAR *)((ltime >> 8) & 0x000000ff);
          *cd++ = (ST_UCHAR *)(ltime & 0x000000ff);
*/
          ftime(&timebuffer);
          timeline = ctime(&(timebuffer.time));
          cd = info_buf + 38;
          *cd++ = (ST_UCHAR)((timebuffer.time >> 24) & 0x000000ff);
          *cd++ = (ST_UCHAR)((timebuffer.time >> 16) & 0x000000ff);
          *cd++ = (ST_UCHAR)((timebuffer.time >> 8) & 0x000000ff);
          *cd++ = (ST_UCHAR)(timebuffer.time & 0x000000ff);
/*
          *(info_buf + 42) = 0x00;
          *(info_buf + 43) = 0x00;
*/
          *cd++ = (ST_UCHAR)((timebuffer.millitm >> 8) & 0x000000ff);
          *cd++ = (ST_UCHAR)(timebuffer.millitm & 0x000000ff);
          buf_header.InfoReference = infoRef;
          buf_header.LocalReference = 1;
          pstMitInfoMsg->msgID++;
          buf_header.MessageId = pstMitInfoMsg->msgID;
          buf_header.Size = 44;
          printf("\nInfo Reference: %d", buf_header.InfoReference);
          printf("\nLocal Reference: %d", buf_header.LocalReference);
          printf("\nMessage ID: %d", buf_header.MessageId);
          printf("\nSize: %d", 44);
          printf("\nNumber of SOE sections: %d", *info_buf);
          printf("\nSize of each SOE section: %d", *(info_buf + 1));
          printf("\nPoint name: '%s'", info_buf + 2);
          printf("\nPoint type: %02x", *(info_buf + 34));
          printf("\nState: %02x", *(info_buf + 35));
          printf("\nTimestamp: %d = %08x / %d = %08x [%02x %02x %02x %02x] - [%02x %02x]",
            timebuffer.time, timebuffer.time, timebuffer.millitm, timebuffer.millitm, 
            *(info_buf + 38), *(info_buf + 39), *(info_buf + 40), *(info_buf + 41),
            *(info_buf + 42), *(info_buf + 43));

          printf("\nThe time is %.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );

          ret = mis_send_IMTransfer(arrayIndex, connArray, &buf_header, buf_len - 1, 
          				info_buf, msg_scope);
          if (ret != SD_SUCCESS)
            printf("\nFailure Sending Message");
          else
            printf("\nSent message");
          chk_free(info_buf);
        }

      } else if (infoRef == INFOMSG_REF_SCHEDULE) {

        /*---- power schedule acknowledge message ----*/
/*
OK-File (02051505LBSEG1.OK1)
----------------------------
$_DATE 2002-05-15
$_SCHEDULE LBSEG1
$_FILENAME 02051505LBSEG1.OK1

ERROR-File (02051505LBSEG1.ER1)
-------------------------------
$_DATE 2002-05-15
$_SCHEDULE LBSEG1
$_FILENAME 02051505LBSEG1.ER1
$_ERROR: Line 3 - 'Number' and 'Time' do not match
$_ERROR: Line 97 - Wrong checksum in column 'Voltage'
*/
        lng = buf_len;
        cd = info_buf;
        while (lng--) *cd++ = 0x00;
        printf("\n Enter date (2002-12-13): ");
        dataEntered = strget(date);
        if (!dataEntered) strcpy(date, "2002-12-13");
        printf("\n Enter schedule (LBSEG1): ");
        dataEntered = strget(schedule);
        if (!dataEntered) strcpy(schedule, "LBSEG1");
        printf("\n Enter schedule number (3): ");
        num = 3;
        longget(&num);
        printf("\n Enter filetype (OK1): ");
        dataEntered = strget(filetype);
        if (!dataEntered) strcpy(filetype, "OK1"); else strcpy(filetype, "ER1");
        buf_header.InfoReference = infoRef;
        buf_header.LocalReference = 2;
        printf("\n Enter MessageId: ");
        n = 0;
        longget(&n);
        buf_header.MessageId = n;
        pstMitInfoMsg->msgID++;
        buf_header.Size = 1024;
        if (*filetype == 'O') {
          sprintf((char *)info_buf, "$_DATE %s%c$_SCHEDULE %s%c$_FILENAME %c%c%c%c%c%c%02d%s.OK1",
            date, 0x0a, schedule, 0x0a,
            *(date + 2), *(date + 3), *(date + 5), *(date + 6), *(date + 8), *(date + 9),
            num, schedule);
        } else {
          sprintf((char *)info_buf, "$_DATE %s%c$_SCHEDULE %s%c$_FILENAME %c%c%c%c%c%c%02d%s.ER1%c%s%c%s",
            date, 0x0a, schedule, 0x0a,
            *(date + 2), *(date + 3), *(date + 5), *(date + 6), *(date + 8), *(date + 9),
            num, schedule, 0x0a,
            "$_ERROR: Line 3 - <error description>", 0x0a,
            "$_ERROR: Line 97 - <error description>");
        }
        printf("\nInfo Reference: %d", buf_header.InfoReference);
        printf("\nLocal Reference: %d", buf_header.LocalReference);
        printf("\nMessage ID: %d", buf_header.MessageId);
        printf("\nSize: %d", strlen((char *)info_buf) + 1);
        printf("\nBegin of message:\n");
        printf("%s", info_buf);
        printf("\nEnd of message\n");
        ret = mis_send_IMTransfer(arrayIndex, connArray, &buf_header, buf_len - 1, 
        				info_buf, msg_scope);
        if (ret != SD_SUCCESS)
          printf("\nFailure Sending Message");
        else
          printf("\nSent message");
        chk_free(info_buf);
      }
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
	sprintf (infobuf,"\n++++++ IM_Transfer_Set enabled for remote AR %s\n", mi_conn->remote_ar);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
  }


/************************************************************************/
/* u_mic_schedule_process: called when an power schedule is recd.       */
/*  called by u_mic_receive_IMTransfer()             added 2003.7.12 zy */
/*                                                 updated 2006.12.6 zy */
/************************************************************************/

ST_VOID u_mic_schedule_process (int buf_Size,
			         ST_UCHAR *info_buf)
{
	FILE *fP;
	char cRcvDate[32];
	char cRcvSch[32];
	char cRcvFilename[32];
	char cAckDirFilename[80];
	int  i,n;
	unsigned char *pstr;
	char  strbuf[128];

	for(i=0; i<buf_Size; i++)
	{
		if((*(info_buf+i) == '$') && (*(info_buf+i+1) == '_'))
		{
			i = i + 2;
			if(strncmp((const char *)(info_buf+i),"DATE",4) == 0)
			{
				for(i=i+4; i<buf_Size; i++)
				{
					if((*(info_buf+i) != ' ')&&(*(info_buf+i) != 9))
						break;
				}
				for(n=0; n<30; i++,n++)
				{
					if((*(info_buf+i)==0x0a)||(*(info_buf+i)==0x0d)||(*(info_buf+i)==' ')||(*(info_buf+i)==9))
						break;
					cRcvDate[n] = *(info_buf+i);
				}
				cRcvDate[n] = '\0';
			}
			else if(strncmp((const char *)(info_buf+i),"SCHEDULE",8) == 0)
			{
				for(i=i+8; i<buf_Size; i++)
				{
					if((*(info_buf+i) != ' ')&&(*(info_buf+i) != 9))
						break;
				}
				for(n=0; n<30; i++,n++)
				{
					if((*(info_buf+i)==0x0a)||(*(info_buf+i)==0x0d)||(*(info_buf+i)==' ')||(*(info_buf+i)==9))
						break;
					cRcvSch[n] = *(info_buf+i);
				}
				cRcvSch[n] = '\0';
			}
			else if(strncmp((const char *)(info_buf+i),"FILENAME",8) == 0)
			{
				for(i=i+8; i<buf_Size; i++)
				{
					if((*(info_buf+i) != ' ')&&(*(info_buf+i) != 9))
					break;
				}
				for(n=0; n<30; i++,n++)
				{
					if((*(info_buf+i)==0x0a)||(*(info_buf+i)==0x0d)||(*(info_buf+i)==' ')||(*(info_buf+i)==9))
						break;
					cRcvFilename[n] = *(info_buf+i);
				}
				cRcvFilename[n] = '\0';
				break;
			}
		}
	}
	sprintf(infobuf,"\ni= %d date = %s  schedule = %s  filename = %s\n",i,cRcvDate,cRcvSch,cRcvFilename);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	if(i >= buf_Size)
		return;

	strcpy(cAckDirFilename,ICCP_SCH_PATH);
	strcat(cAckDirFilename,cRcvFilename);
	if ((fP = fopen(cAckDirFilename,"w"))== NULL)
	{
		sprintf(infobuf,"*****iccp: Can't open file %s to write Information Message *****\n",cAckDirFilename);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return;
	}
	pstr = info_buf;
	while( (*pstr != '\0') && fputc(*(pstr++), fP) != EOF );
	fclose(fP);

	/*---- Change power schedule file own to ems 2007.5.30 zy ----*/

	sprintf(strbuf, "chown ems %s", cAckDirFilename);
	system(strbuf);
}

/************************************************************************/
/* u_mic_receive_IMTransfer: called when an Information Message is recd */
/************************************************************************/

ST_VOID u_mic_receive_IMTransfer (MI_CONN *mi_conn,
				 MI_INFO_BUF_HEADER *buf_header, 
			         ST_UCHAR *info_buf, ST_INT msg_scope)
{
	ST_INT   nCnt;
	sprintf (infobuf,"\n *** Incoming IMTransfer Report: ***\n\t InfoReference = %d",
		buf_header->InfoReference);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	sprintf (infobuf,"\n\t LocalReference = %d\n\t MessageId = %d\n\t Size = %d\n",
		buf_header->LocalReference, buf_header->MessageId, buf_header->Size);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

/* Following added for ack information message zy*/

	if(buf_header->InfoReference == INFOMSG_REF_SCHEDULE)
	{
		u_mic_schedule_process ( buf_header->Size, info_buf);

		cAck_nDst = 0x01;
		for (nCnt = 0; nCnt < DC_cnt; ++nCnt)
		{
			if(mi_conn == sAssocDat[nCnt][0].pAssocInf->pstAssocCtrl->mi_conn)
			{
				cAck_nDst = nCnt + 1;
				break;
			}
			else if((nAssocNum[nCnt] == 2) && (mi_conn == sAssocDat[nCnt][1].pAssocInf->pstAssocCtrl->mi_conn))
			{
				cAck_nDst =  nCnt + 1;
				break;
			}
		}
/*		sSigTASE2Ack.nVal = INFOMSG_REF_SCHEDULE;
		Fct_SendInformationMessage(&sSigTASE2Ack);
*/
	}
	//added by yyp 2011.10.05
	else if(buf_header->InfoReference == INFOMSG_REF_SOE)
	{
		ClientStateFromSOEProc (buf_header->Size, info_buf);
	}

	
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





ST_VOID Fct_SendInformationMessage( sGWSigTASE2 *pSigTASE2 )

{

/*# start insertion ##########################################################*/
ST_UCHAR *cd;
ST_UCHAR *cs;
ST_BOOLEAN dataEntered;
ST_CHAR date[256];
ST_CHAR filetype[256];
ST_CHAR ind_name[256];
ST_INT lng;
ST_LONG n;
ST_LONG num;
ST_UCHAR s;
ST_CHAR schedule[256];
ST_LONG ltime;
struct timeb timebuffer;
char *timeline;
/*# end insertion ############################################################*/
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
FILE  *fPr;
ST_CHAR ack_filename[128];
char  strbuf[128];

/*
typedef struct GWSignalTASE2 {
  int          nFct;
  byte         nDst;
  char         strDCNam[33];
  FLAGS        nType;
  byte         nValBits;
  long int     nVal;
  double       rVal;
  FLAGS        nValidity;
  FLAGS        nCurrentSource;
  FLAGS        nNormalValue;
  FLAGS        nTimeStampQuality;
  FLAGS        nOverflow;
  sGWTimeStamp sTimeStamp;
} sGWSigTASE2;
*/



  switch (pSigTASE2->nDst) {
    case 0x01:
      pstAssociationInfo = sAssocDat[0][0].pAssocInf;
      pstMitMisVcc = sAssocDat[0][0].pMitMisVcc;
      if((pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE) && (nAssocNum[0] == 2))
      {
         pstAssociationInfo = sAssocDat[0][1].pAssocInf;
         pstMitMisVcc = sAssocDat[0][1].pMitMisVcc;
      }
      break;
    case 0x02:
      pstAssociationInfo = sAssocDat[1][0].pAssocInf;
      pstMitMisVcc = sAssocDat[1][0].pMitMisVcc;
      if((pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE) && (nAssocNum[1] == 2))
      {
         pstAssociationInfo = sAssocDat[1][1].pAssocInf;
         pstMitMisVcc = sAssocDat[1][1].pMitMisVcc;
      }
      break;
    case 0x04:
      pstAssociationInfo = sAssocDat[2][0].pAssocInf;
      pstMitMisVcc = sAssocDat[2][0].pMitMisVcc;
      if((pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE) && (nAssocNum[2] == 2))
      {
         pstAssociationInfo = sAssocDat[2][1].pAssocInf;
         pstMitMisVcc = sAssocDat[2][1].pMitMisVcc;
      }
      break;
  }

/*return if association not ACTIVE added 2003.7.9 zy */

  if (pstAssociationInfo->pstAssocCtrl->state != MI_ASSOC_STATE_ACTIVE)
	return;

  /* Get the list of MI_ASSOC_CTRLS */
  connArray = (MI_CONN **)chk_calloc(list_get_sizeof(pstAssocInfoSet), 
                           sizeof(MI_CONN *));
  arrayIndex = 0;
  connArray[arrayIndex++] = pstAssociationInfo->pstAssocCtrl->mi_conn;

  if (pstMitMisVcc!=NULL) {

    infoRef = pSigTASE2->nVal;

    /* see if we entered a valid number */
    pstMitInfoMsg = pstMitMisVcc->pstInfoMsgs; /* look at the last VCC */
    found = SD_FALSE;
    while ((found == SD_FALSE) && (pstMitInfoMsg != (struct Tag_MIT_INFO_MSG *)NULL)) {
      if (pstMitInfoMsg->infoRef == infoRef)
        found = SD_TRUE;
      else
        pstMitInfoMsg = (struct Tag_MIT_INFO_MSG *)list_get_next(pstMitMisVcc->pstInfoMsgs, pstMitInfoMsg);
    }

    if (found == SD_TRUE) {
      info_buf = (unsigned char *)chk_malloc(pstMitInfoMsg->maxSize + 1);
      buf_len = pstMitInfoMsg->maxSize + 1;

      if (infoRef == INFOMSG_REF_SOE) {

        /*---- soe message ----*/
        lng = buf_len;
        cd = info_buf;
        while (lng--) *cd++ = 0x00;
        *info_buf = 0x01;
        *(info_buf + 1) = 0x2a; /* = 42 */
        lng = strlen(pSigTASE2->strDCNam);
        cs = (ST_UCHAR *)pSigTASE2->strDCNam;
        cd = info_buf + 2;
        while (lng--) *cd++ = *cs++;
        *(info_buf + 34) = 0x00;
        *(info_buf + 34) = 0x00;
        *(info_buf + 35) = (ST_UCHAR)((pSigTASE2->nValBits << 6)
                                        + (pSigTASE2->nValidity << 4));
        *(info_buf + 36) = 0x00;
        *(info_buf + 37) = 0x00;
        lng = sizeof(sGWTimeStamp);
        lng = lng -2;	/* modified 2003.7.4 zy for lng = 8 */
        cs = (ST_UCHAR *)&pSigTASE2->sTimeStamp;
        cd = info_buf + 38;
        while (lng--) *cd++ = *cs++;
        buf_header.InfoReference = infoRef;
        buf_header.LocalReference = 1;
        pstMitInfoMsg->msgID++;
        buf_header.MessageId = pstMitInfoMsg->msgID;
        buf_header.Size = 44;
/*
printf("Info Reference: %d\n", buf_header.InfoReference);
printf("Local Reference: %d\n", buf_header.LocalReference);
printf("Message ID: %d\n", buf_header.MessageId);
printf("Size: %d\n", 44);
printf("Number of SOE sections: %d\n", *info_buf);
printf("Size of each SOE section: %d\n", *(info_buf + 1));
*/
printf("GWDC - soe msg - point name: '%s'\n", info_buf + 2);
/*
printf("Point type: %02x\n", *(info_buf + 34));
*/
printf("GWDC - soe msg - state: %02x\n", *(info_buf + 35));
printf("GWDC - soe msg - timestamp: %d = %08x / %d\n",
  pSigTASE2->sTimeStamp.nTim, pSigTASE2->sTimeStamp.nTim, pSigTASE2->sTimeStamp.nMSec);

        ret = mis_send_IMTransfer(arrayIndex, connArray, &buf_header, buf_len - 1, 
                                    info_buf, msg_scope);
        if (ret == SD_SUCCESS)
          printf("Sent InformationMessage\n");
/*
        else
          printf("Failure Sending Information Message\n");
*/
        chk_free(info_buf);
      } else if (infoRef == INFOMSG_REF_SCHEDULE) {

        /*---- power schedule acknowledge message ----*/
/*
OK-File (02051505LBSEG1.OK1)
----------------------------
$_DATE 2002-05-15
$_SCHEDULE LBSEG1
$_FILENAME 02051505LBSEG1.OK1

ERROR-File (02051505LBSEG1.ER1)
-------------------------------
$_DATE 2002-05-15
$_SCHEDULE LBSEG1
$_FILENAME 02051505LBSEG1.ER1
$_ERROR: Line 3 - 'Number' and 'Time' do not match
$_ERROR: Line 97 - Wrong checksum in column 'Voltage'
*/

        lng = buf_len;
        cd = info_buf;
        while (lng--) *cd++ = 0x00;

        strcpy(ack_filename,"/usr/gwaysw/gway/gwapwsf/");
        strcat(ack_filename,pSigTASE2->strDCNam);
        if ((fPr = fopen(ack_filename,"r"))== NULL)
           {
           printf("*****gwdc: Can't open file %s to read Information Message *****\n",ack_filename);
           return;
           }
        fread(info_buf,1,1024,fPr);
        fclose(fPr);

        buf_header.InfoReference = infoRef;
        buf_header.LocalReference = 2;
        pstMitInfoMsg->msgID++;
        buf_header.MessageId = pstMitInfoMsg->msgID;
        buf_header.Size = 1024;
        ret = mis_send_IMTransfer(arrayIndex, connArray, &buf_header, buf_len - 1, 
        				info_buf, msg_scope);
        if (ret != SD_SUCCESS)
          printf("Failure Sending information Message\n");
        else
          printf("Sent Ack Information Message\n");
        chk_free(info_buf);

#ifdef WINDOWS32
        sprintf(strbuf, "move %s %s 2>/NUL", ack_filename, DAT_SCHACKFILE_NAM);
        system(strbuf);
#endif
#ifdef HPUX
        sprintf(strbuf, "mv %s %s", ack_filename, "/usr/gwaysw/gway/gwtmp/ScheduleAckFile.txt");
        system(strbuf);
#endif

      }
    } else {

      printf ("GWDC, mit_imsg - InfoMsg %d not found\n", infoRef);
    }
  }
  chk_free (connArray);
}

