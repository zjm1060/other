/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_ssoc.c                                            	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Association management functions				*/
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 07/19/02  rkr    8      fixed setReqCapacity                         */
/* 06/20/02  rkr    7      Copyright year update                        */
/* 06/21/02  rkr    6      changes to u_mvl_connect_ind                 */
/* 06/18/02  rkr    5      Changes for tracking outstanding MMS Reqs	*/
/* 06/18/02  ask    4	   Display 'LISTEN' state, similar to AXS4-ICCP	*/
/* 06/11/02  ask    3      remove AR references from MIT_REMOTE structs */
/* 06/11/02  ask    2      use first assoc name in getAssociationName	*/
/* 03/17/99  rkr    1      Created                                      */
/************************************************************************/

#include "mit_main.h"
#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"

/************************************************************************/
/* function prototypes                           		        */
/************************************************************************/
ST_VOID Fct_StartAssociations( ST_VOID );
ST_VOID Fct_StopAssociations( ST_VOID );

/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/

static char *thisFileName = __FILE__;
extern	char	infobuf[1024];

/************************************************************************/
/*		           startAssociation				*/
/************************************************************************/
ST_VOID startAssociation (MIT_ASSOC_INFO *pstMitAssoc)

{

  MI_ASSOC_CTRL 	*mi_assoc;

  mi_assoc = pstMitAssoc->pstAssocCtrl;
  if (mi_assoc!=NULL) {
    MIT_LOG_FLOW1("Setting MI_ASSOC_CTRL_ENABLE for %s", pstMitAssoc->associationName);
    mi_assoc->ctrl_flags |= MI_ASSOC_CTRL_ENABLE;
    /* Set the application view for our own purposes */
    pstMitAssoc->ctrlFlags |= MI_ASSOC_CTRL_ENABLE;
  } else {
    MIT_LOG_NERR2("No MI_ASSOC_CTRL for '%s', check remote name '%s', assoc not started!",
                    pstMitAssoc->associationName, pstMitAssoc->remoteName);
  }

}

/************************************************************************/
/*			    getAssociationName				*/
/************************************************************************/
MIT_ASSOC_INFO	*getAssociationName ( ST_CHAR *buffer )
  {
static ST_CHAR	defaultAssociationName[256];
static ST_BOOLEAN firstAssociationName=SD_TRUE;
MIT_ASSOC_INFO	*pstAssociationInfo;
ST_BOOLEAN	dataEntered;

  /* Use the first association name until the user supplies one */
  buffer[0]=0;			
  pstAssociationInfo = pstAssocInfoSet;
  if(pstAssociationInfo->associationName)
    {
    strcpy(buffer, pstAssociationInfo->associationName);
    }
  
  if (firstAssociationName==SD_TRUE)
    {
    /* This is the first time an Association Name is being entered.	*/
    /* When a valid name is entered, it becomes the default.		*/

    //printf ("\n Enter Association Name: (default=%s)",buffer);
    //dataEntered = strget (buffer); //modified by yyp 2011.10.05 
    dataEntered = 1;
    if (dataEntered)
      {
      if ((pstAssociationInfo = findAssociation (buffer))!=NULL)
        {
	strcpy (defaultAssociationName, buffer);
	firstAssociationName=SD_FALSE;
	}			   
      }
    }
  else
    {
    /* There is already an AssociationName being used, it is the 	*/
    /* default supplied to the user and will change when the user 	*/
    /* supplies a different valid name.					*/

    strcpy (buffer, defaultAssociationName);
    printf ("\n Enter Association Name: (default=%s)",buffer);
    strget (buffer);
    if ((pstAssociationInfo = findAssociation (buffer))!=NULL)
      {
      strcpy (defaultAssociationName, buffer);
      }
    
    }
  return (pstAssociationInfo);
  }

/************************************************************************/
/* startAssociations:  start defined selected remotes 			*/
/************************************************************************/
// ST_VOID startAssociations (ST_VOID)
ST_VOID Fct_StartAssociations( ST_VOID )

{

  MIT_ASSOC_INFO *pstMitAssoc;

  pstMitAssoc = pstAssocInfoSet;
  while (pstMitAssoc) {
    sprintf(infobuf,"iccp: - Start Association: '%s'\n", pstMitAssoc->associationName);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
    startAssociation (pstMitAssoc);
    pstMitAssoc = (MIT_ASSOC_INFO *)list_get_next((ST_VOID *)pstAssocInfoSet,
                                                    (ST_VOID *)pstMitAssoc);
  }

}

/************************************************************************/
/*		           stopAssociation				*/
/************************************************************************/

static ST_VOID stopAssociation (MIT_ASSOC_INFO *pstMitAssoc)
{

  MI_ASSOC_CTRL 	*mi_assoc;

  MIT_LOG_FLOW1 ("Clearing MI_ASSOC_CTRL_ENABLE for %s", pstMitAssoc->associationName);
  mi_assoc = pstMitAssoc->pstAssocCtrl;
  mi_assoc->ctrl_flags &= ~MI_ASSOC_CTRL_ENABLE;
  /* Clear the application view for our own purposes */
  pstMitAssoc->ctrlFlags &= ~MI_ASSOC_CTRL_ENABLE;

}

/************************************************************************/
/* stopAssociations:  stop one or all remotes for select who			*/
/************************************************************************/
//  ST_VOID stopAssociations (ST_VOID)
ST_VOID Fct_StopAssociations( ST_VOID )

{

 MIT_ASSOC_INFO *pstMitAssoc;

 pstMitAssoc = pstAssocInfoSet;
 while (pstMitAssoc) {
   sprintf(infobuf,"iccp: - Stop Association: '%s'\n", pstMitAssoc->associationName);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
   stopAssociation (pstMitAssoc);
   pstMitAssoc = (MIT_ASSOC_INFO *)list_get_next((ST_VOID *)pstAssocInfoSet,
                                                  (ST_VOID *)pstMitAssoc);
 }

}

/************************************************************************/
/* AbortAssociations:  stop one or all remotes for select who			*/
/************************************************************************/
//  ST_VOID stopAssociations (ST_VOID)
ST_VOID Fct_AbortAssociations( ST_VOID )

{

 MIT_ASSOC_INFO *pstMitAssoc;

 pstMitAssoc = pstAssocInfoSet;
 while (pstMitAssoc) {
   sprintf(infobuf,"iccp: - Abort Association: '%s'\n", pstMitAssoc->associationName);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
   if(pstMitAssoc->pstAssocCtrl->state == MI_ASSOC_STATE_ACTIVE)
   	pstMitAssoc->pstAssocCtrl->state = MI_ASSOC_STATE_ABORT;
   stopAssociation (pstMitAssoc);
   pstMitAssoc = (MIT_ASSOC_INFO *)list_get_next((ST_VOID *)pstAssocInfoSet,
                                                  (ST_VOID *)pstMitAssoc);
 }

}

/************************************************************************/
/* 			       showAssocSummary				*/
/************************************************************************/

ST_VOID showAssocSummary (ST_VOID)
  {
MIT_ASSOC_INFO  *pstAI;
MIT_AR_NAME	*pstCurARName;

  pstAI = pstAssocInfoSet;
  while (pstAI)
    {
    printf ("\n\n\n\n\n");
    printf ("\n     Association Name: %s", pstAI->associationName);
    printf ("\n     Remote Name: %s", pstAI->remoteName);
    printf ("\n     Local Name: %s", pstAI->localArName);
    pstCurARName = pstAI->arList;
    
    while (pstCurARName)
      {
      printf ("\n     Remote AR Name: %s", pstCurARName->arName);
      pstCurARName = (MIT_AR_NAME *)list_get_next (pstAI->arList, pstCurARName);
      }

    printf ("\n     Ctrl Flags: 0x%x", pstAI->ctrlFlags);
    if (pstAI->ctrlFlags & MI_ASSOC_CTRL_ENABLE)
      printf ("\n           MI_ASSOC_CTRL_ENABLE");
    if (pstAI->ctrlFlags & MI_ASSOC_CTRL_CALLING)
      printf ("\n           MI_ASSOC_CTRL_CALLING");
    if (pstAI->ctrlFlags & MI_ASSOC_CTRL_CALLED)
      printf ("\n           MI_ASSOC_CTRL_CALLED");
    if (pstAI->ctrlFlags & MI_ASSOC_CTRL_CLIENT)
      printf ("\n           MI_ASSOC_CTRL_CLIENT");
    if (pstAI->ctrlFlags & MI_ASSOC_CTRL_SERVER)
      printf ("\n           MI_ASSOC_CTRL_SERVER");
    if (pstAI->pstAssocCtrl)
      {
      printf ("\n     State: 0x%x", pstAI->pstAssocCtrl->state);
      
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_IDLE)
        {
        if ((pstAI->ctrlFlags & MI_ASSOC_CTRL_ENABLE) &&
	   (pstAI->ctrlFlags & MI_ASSOC_CTRL_CALLED))
          {
          printf ("\n           LISTEN");
          }
	else
          {
          printf ("\n           MI_ASSOC_STATE_IDLE");
      	  }
      	}
      
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_ACTIVE)
        printf ("\n           MI_ASSOC_STATE_ACTIVE");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_PARAM_CHECK)
        printf ("\n           MI_ASSOC_STATE_PARAM_CHECK");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_INIT_REQ)
        printf ("\n           MI_ASSOC_STATE_INIT_REQ");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_INIT_IND)
        printf ("\n           MI_ASSOC_STATE_INIT_IND");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_START_CLIENT)
        printf ("\n           MI_ASSOC_STATE_START_CLIENT");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_CONCLUDING)
        printf ("\n           MI_ASSOC_STATE_CONCLUDING");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_ABORT)
        printf ("\n           MI_ASSOC_STATE_ABORT");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_ABORTING)
        printf ("\n           MI_ASSOC_STATE_ABORTING");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_BACKOFF)
        printf ("\n           MI_ASSOC_STATE_BACKOFF");
      if (pstAI->pstAssocCtrl->state == MI_ASSOC_STATE_INVALID)
        printf ("\n           MI_ASSOC_STATE_INVALID");
      }
    else
      printf ("\n     (ASSOCIATION NOT STARTED)");
    printf ("\n     Max MMS Message size: %d", pstAI->maxSegsize);
    printf ("\n     Max Num of outstanding Requests: %d", pstAI->maxReqCalling);
    printf ("\n     Max Num of outstanding Indications: %d", pstAI->maxReqCalled);
    printf ("\n     Number of remaining Requests: %d", pstAI->numReq);
    printf ("\n     Associate Back Off Time: %d", pstAI->pstAssocCtrl->connect_backoff_time);
    pstAI = (MIT_ASSOC_INFO *) list_get_next
            ((ST_VOID *)pstAssocInfoSet, (ST_VOID *)pstAI);
    pause_msg ("\n\n Press a key to continue!");
    }
  (*menu_set_fun)();
  }
	
/************************************************************************/
/* 			  displayConfiguredRemotes			*/
/************************************************************************/

ST_VOID  displayConfiguredRemotes (ST_VOID)
  {

MIT_REMOTE 	*pstRemote;
ST_INT		line;

  pstRemote = pstRemoteSet;
  line = 1;
  while (pstRemote)
    {
    printf ("\n Remote name: %s", pstRemote->remoteName);
    
    line++;
    if (line == 24)
      {
      pause_msg ("\n\n Press any key to continue");
      line =1;
      }
    pstRemote = (MIT_REMOTE *) list_get_next
    	     ((ST_VOID *)pstRemoteSet, (ST_VOID *)pstRemote);
    }
  pause_msg ("\n\n Press any key to continue");
  (*menu_set_fun)();
  }

/************************************************************************/
/*			         u_mvl_connect_ind 			*/
/************************************************************************/

ST_RET u_mvl_connect_ind (MVL_NET_INFO *net_info, INIT_INFO *init_ind_info)
  {
AARQ_APDU *apdu;

  printf ("\n Client is connecting without an AE-Title");
  apdu = &net_info->ass_ind_info;
  MIT_LOG_INDHEX(apdu->called_paddr.psel_len, apdu->called_paddr.psel);
  MIT_LOG_INDHEX(apdu->called_paddr.ssel_len, apdu->called_paddr.ssel);
  MIT_LOG_INDHEX(apdu->called_paddr.tsel_len, apdu->called_paddr.tsel);
  /*MIT_LOG_INDHEX(apdu->called_paddr.nsap_len, apdu->called_paddr.netAddr.nsap);*/
  return(SD_SUCCESS);
  }

/************************************************************************/
/*                                   setReqCapacity                     */
/************************************************************************/

ST_VOID	setReqCapacity(MIT_ASSOC_INFO *pstMitAssoc)
  {
  /* start with the negotiated value */
  pstMitAssoc->numReq = pstMitAssoc->pstAssocCtrl->mi_conn->net_info->maxpend_req;
  }

/************************************************************************/
/*                             hasReqCapacity                           */
/************************************************************************/
ST_BOOLEAN	hasReqCapacity(MIT_ASSOC_INFO *pstMitAssoc)
  {
ST_BOOLEAN	ret = SD_FALSE;

  if (pstMitAssoc->numReq > 0) 
    {
    ret = SD_TRUE;
    } 
  else
    MIT_LOG_NERR1("Link '%s' is out of requests",pstMitAssoc->associationName)
  return (ret);
  }

/************************************************************************/
/*                             incReqCapacity                           */
/************************************************************************/
ST_VOID		incReqCapacity(MIT_ASSOC_INFO *pstMitAssoc)
  {
  pstMitAssoc->numReq++;
  }

/************************************************************************/
/*                              decReqCapacity                          */
/************************************************************************/
ST_VOID		decReqCapacity(MIT_ASSOC_INFO *pstMitAssoc)
  {
  pstMitAssoc->numReq--;
  }

