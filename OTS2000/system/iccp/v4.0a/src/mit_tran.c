/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_tran.c                                            	*/
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
/* 06/20/02  rkr    5      Copyright year update                        */
/* 06/18/02  rkr    4      Changes for tracking outstanding MMS Reqs	*/
/* 02/11/02  rkr    3      Added numRefDSTS				*/
/* 06/12/01  rkr    2      Turned createTransferSets into a loop	*/
/* 03/17/99  rkr    1      Created                                      */
/************************************************************************/

#include "mit_main.h"
#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"

/************************************************************************/
/* static function prototypes						*/
/************************************************************************/

static ST_VOID startTsDone 	(MI_REQ_CTRL *req);
static ST_VOID stopTransferSetsDone (MI_REQ_CTRL *req);

/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/

static char *thisFileName = __FILE__;

/************************************************************************/
/* 		           startTransferSets				*/
/************************************************************************/

ST_VOID 	startTransferSets (ST_VOID)
  {
MIT_ASSOC_INFO 		*pstMitAssoc;
MIT_TRANSFER_SET	*pstCurTransferSet;
ST_CHAR 		buffer[100];
ST_BOOLEAN 		dataEntered;

  pstMitAssoc= getAssociationName (buffer);
  if (pstMitAssoc!=NULL)
    {
    if ((pstMitAssoc->pstMitMicVcc!=NULL)&&
        (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE))
      {
      if (ask ("\n Start all Transfer Sets (y:TRUE, n:FALSE) (default = TRUE) : ",1))
        {
        pstMitAssoc->pstMitMicVcc->pstNextTransferSetToStart = 
                          pstMitAssoc->pstMitMicVcc->pstTransferSets;
        createTransferSets (pstMitAssoc);
        }
      else
        {
        printf ("\n Enter name of Transfer Set to start: ");
        dataEntered = strget(buffer);
        if (dataEntered &&
	    (pstCurTransferSet = findMitTransferSet (
           pstMitAssoc->pstMitMicVcc->pstTransferSets, buffer))!=NULL)
          {
          createTransferSet (pstMitAssoc, pstCurTransferSet);
    	  }
        }
      }
    else
      {
      printf ("\n No MIC_VCC configured for this Association");
      }
    }
  else
    {
    printf ("\n Association '%s' not found", buffer);
    }	 	/* end if we can find the Association */
  (*menu_set_fun)();
  }

/************************************************************************/
/*			      stopTransferSets				*/
/************************************************************************/

ST_VOID 		stopTransferSets (ST_VOID)
  {
MIT_ASSOC_INFO 		*pstMitAssoc;
MIT_TRANSFER_SET	*pstCurTransferSet;
ST_CHAR 		buffer[100];
ST_BOOLEAN 		dataEntered;
MIC_DSTS 		*pstMicDsTs;
MI_REQ_CTRL 		*req;

  pstMitAssoc= getAssociationName (buffer);
  if (pstMitAssoc!=NULL)
    {
    if ((pstMitAssoc->pstMitMicVcc!=NULL)&&
        (pstMitAssoc->pstAssocCtrl->state==MI_ASSOC_STATE_ACTIVE)&&
        (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
      if (ask ("\n Stop all Transfer Sets (y:TRUE, n:FALSE) (default = TRUE) : ",1))
        {
      /* Stop the entire list of Transfer Sets			*/
      /* inactive TransferSets have null pointer references	*/
        pstCurTransferSet = pstMitAssoc->pstMitMicVcc->pstTransferSets;
        while (pstCurTransferSet)
          {
          pstMicDsTs = pstCurTransferSet->pstDSTS;
	  if (pstMicDsTs)
	    {
            req = mic_stop_dsts (pstMitAssoc->pstAssocCtrl->mi_conn, 
	                       pstMicDsTs);
            if (req)
	      {
	      req->u_req_done = stopTransferSetsDone;
              req->user_info[0] = (ST_VOID *)pstCurTransferSet;
              decReqCapacity(pstMitAssoc);
	      }
            }
           pstCurTransferSet = (MIT_TRANSFER_SET *) list_get_next 
               ((ST_VOID *)pstMitAssoc->pstMitMicVcc->pstTransferSets, 
  	         (ST_VOID *)pstCurTransferSet);
          }
        }
      else
        {
        printf ("\n Enter name of Transfer Set to stop: ");
        dataEntered = strget(buffer);
        if (dataEntered &&
	    (pstMicDsTs = findTransferSet (pstMitAssoc->pstMitMicVcc->pstTransferSets,
	                                   buffer))!=NULL)
          {
          req = mic_stop_dsts (pstMitAssoc->pstAssocCtrl->mi_conn,
	                     pstMicDsTs);
          if (req)
	    {
            req->u_req_done = stopTransferSetsDone;

	    pstCurTransferSet = findMitTransferSet (pstMitAssoc->pstMitMicVcc->pstTransferSets, buffer);
            req->user_info[0] = (ST_VOID *)pstCurTransferSet;
	    }
    	  }
        }
      }
    else
      {
      printf ("\n No MIC_VCC configured for this Association");
      }
    }
  else
    {
    printf ("\n Association '%s' not found", buffer);
    }	 	/* end if we can find the Association */
  (*menu_set_fun)();
  }

/************************************************************************/
/*		          stopTransferSetsDone				*/
/************************************************************************/

static ST_VOID  stopTransferSetsDone (MI_REQ_CTRL *req)
  {
MIT_TRANSFER_SET *pstCurTransferSet;
MIT_ASSOC_INFO   *pstCurAssocInfo;
MIT_DATA_SET	 *pstMitDataSet;

   /* start with the mi_conn, get to the MIT_ASSOC_INFO ptr */
  pstCurAssocInfo = (MIT_ASSOC_INFO *)req->mi_conn->mi_assoc->user_info[0];
  incReqCapacity(pstCurAssocInfo);
  /* save this before we free the MI_REQ_CTRL				*/
  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_FLOW0 ("ICCP stopped TransferSet OK");
    pstCurTransferSet = (MIT_TRANSFER_SET *)req->user_info[0];

    /* start with the mi_conn, get to the MIT_ASSOC_INFO ptr */
    pstCurAssocInfo = (MIT_ASSOC_INFO *)req->mi_conn->mi_assoc->user_info[0];


    /* then look up the DS name in the context of the Client BLT belonging to this association*/
    pstMitDataSet = findMitDataSet(pstCurAssocInfo->pstMitMicVcc->pstDataSets, pstCurTransferSet->dataSetName);
    if (pstMitDataSet)
      {
      pstMitDataSet->numRefDSTS--; /* decrement the thing */
      }

    mi_free_req_ctrl (req);
    }
  else
    {
    MIT_LOG_ERR1 ("ICCP stop TransferSet failed result %d", req->result);
    mi_free_req_ctrl (req);
    }
  }

/************************************************************************/
/*		       displayRemoteTransferSetNames			*/
/************************************************************************/

ST_VOID		displayRemoteTransferSetNames(ST_VOID)
  {
  pause_msg("\n Press any key to continue");
  (*menu_set_fun)();
  }

/************************************************************************/
/*		    	   createTransferSet				*/
/************************************************************************/

ST_RET     createTransferSet (MIT_ASSOC_INFO *pstMitAssoc,
			      MIT_TRANSFER_SET *pstCurTransferSet)
  {
MIT_MIC_VCC		*pstMitMicVcc;
ST_RET			ret = SD_FAILURE;

/* See which TransferSet we have to start.  If we started them all then	*/
/* return a FAILURE.							*/

  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
  if (pstMitMicVcc)
    {
    /* See if we have any to start, if not return SD_FAILURE		*/
    ret = startTransferSet (pstMitAssoc->pstAssocCtrl->mi_conn, 
                             pstCurTransferSet,
    	               	     pstMitAssoc);
    pstMitMicVcc->numStartedTransferSets++;
    pstMitMicVcc->pstNextTransferSetToStart = NULL;
    }
  return (ret);
  }

/************************************************************************/
/*		    	   createTransferSets				*/
/************************************************************************/

ST_RET     createTransferSets (MIT_ASSOC_INFO *pstMitAssoc)
  {
MIT_MIC_VCC	*pstMitMicVcc;
ST_RET		ret;

/* See which TransferSet we have to start.  If we started them all then	*/
/* return a FAILURE.							*/

  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
  if (pstMitMicVcc)
    {
    /* See if we have any to start, if not return SD_FAILURE		*/
    ret = SD_FAILURE;
    
    while ((pstMitMicVcc->pstNextTransferSetToStart) && (ret == SD_FAILURE)&&
           (hasReqCapacity(pstMitAssoc)==SD_TRUE))
      {
      ret = startTransferSet (pstMitAssoc->pstAssocCtrl->mi_conn, 
                              pstMitMicVcc->pstNextTransferSetToStart,
		              pstMitAssoc);
      pstMitMicVcc->numStartedTransferSets++;
      pstMitMicVcc->pstNextTransferSetToStart = (struct Tag_MIT_TRANSFER_SET *)list_get_next(
      			   pstMitMicVcc->pstTransferSets,
			   pstMitMicVcc->pstNextTransferSetToStart);
      }					 
    }
  return (ret);
  }

/************************************************************************/
/*			      startTransferSet				*/
/************************************************************************/

ST_RET 		startTransferSet (MI_CONN *mi_conn, 
                                  MIT_TRANSFER_SET *pstMitTransferSet,
				  MIT_ASSOC_INFO *pstMitAssoc)
  {
ST_RET		ret;
MI_REQ_CTRL 	*req;
MI_DSTS_DATA 	dsts_data;
MIC_DSTS 	*mic_dsts;
MIC_DS		*micDs;
MIT_DATA_SET	*pstMitDataSet;

  ret = SD_FAILURE;
  dsts_data.StartTime = pstMitTransferSet->startTime;
  dsts_data.Interval = pstMitTransferSet->interval;
  dsts_data.TLE = pstMitTransferSet->tLE;
  dsts_data.BufferTime = pstMitTransferSet->bufferTime;
  dsts_data.IntegrityCheck = pstMitTransferSet->integrityCheck;
  dsts_data.DSConditionsRequested = pstMitTransferSet->dSConditionsRequested;
  dsts_data.BlockData = pstMitTransferSet->blockData;
  dsts_data.Critical = pstMitTransferSet->critical;
  dsts_data.RBE = pstMitTransferSet->rBE;
  dsts_data.Status = pstMitTransferSet->status;
  dsts_data.EventCodeRequested = 0;

  pstMitDataSet = findMitDataSet (pstMitAssoc->pstMitMicVcc->pstDataSets,
				  pstMitTransferSet->dataSetName);
  if (pstMitDataSet)
    {
    pstMitDataSet->numRefDSTS++;
    }


  micDs = (MIC_DS *)findDataSet (pstMitAssoc->pstMitMicVcc->pstDataSets, 
                                 pstMitTransferSet->dataSetName);
  if (micDs==NULL)
    {
    MI_LOG_ERR2 ("\n DataSet '%s' referenced by TransferSet '%s' not found",
                 pstMitTransferSet->dataSetName,
	         pstMitTransferSet->dsTsName);
    return (ret);
    }

  req = mic_start_dsts (mi_conn, micDs, &dsts_data, &mic_dsts);
  if (!req)
    {
    MIT_LOG_ERR1 ("Could not start the DSTS '%s'", pstMitTransferSet->dsTsName);
    ret = SD_FAILURE;
    }
  else
    {
    req->u_req_done =  startTsDone;
    decReqCapacity(pstMitAssoc);
    req->user_info[0] = (ST_VOID *)pstMitAssoc;
    pstMitTransferSet->pstDSTS = mic_dsts;
    ret = SD_SUCCESS;
    }
  return (ret);
  }

/************************************************************************/
/*			     startTsDone 				*/
/************************************************************************/

static ST_VOID startTsDone (MI_REQ_CTRL *req)
  {
MIT_MIC_VCC		*pstMitMicVcc;
MIT_ASSOC_INFO 		*pstMitAssoc;
ST_RET			ret;

  /* save this before we free the MI_REQ_CTRL				*/

  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)req->user_info[0];
  incReqCapacity(pstMitAssoc);
  if (req->result == SD_SUCCESS)
    {
    MIT_LOG_CNF0 ("ICCP Start TS OK");
    mi_free_req_ctrl (req);
    }
  else
    {
    MIT_LOG_ERR1 ("ICCP Start TS Error %d", req->result);
    mi_free_req_ctrl (req);
    }

  /* Retrieve the pointer of the MIT_MIC_VCC the XferSet was created for*/
  /* If there is a next XferSet to start then we will do that		*/

  pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
  if (pstMitMicVcc)
    {
    if (pstMitMicVcc->pstNextTransferSetToStart)
      {
      ret = createTransferSets (pstMitAssoc);
      MIT_LOG_FLOW1 ("Created another TransferSet ret = %d", ret);
      }
    }
  }

/************************************************************************/
/*	            displayRemoteTransferSetAttributes			*/
/************************************************************************/

ST_VOID displayRemoteTransferSetAttributes(ST_VOID)
  {
ST_CHAR  	  buffer[256];
MIT_MIC_VCC	  *pstMitMicVcc;
MIT_TRANSFER_SET  *pstCurTransferSet;

  pstMitMicVcc = getRemoteMitMicVcc (buffer);
  if (pstMitMicVcc)
    {						     
    pstCurTransferSet = pstMitMicVcc->pstTransferSets;
    while (pstCurTransferSet)
      {
      printf ("\n\n\n\n");
      printf ("\n     TransferSet Name: %s", pstCurTransferSet->dsTsName);
      printf ("\n     DataSet Name: %s", pstCurTransferSet->dataSetName);
      printf ("\n     Start Time: %d", pstCurTransferSet->startTime);
      printf ("\n     Interval: %d", pstCurTransferSet->interval);
      printf ("\n     TLE: %d", pstCurTransferSet->tLE);
      printf ("\n     Buffer Time: %d", pstCurTransferSet->bufferTime);
      printf ("\n     Integrity Check: %d", pstCurTransferSet->integrityCheck);
      printf ("\n     DS Conditions Requested: %d", pstCurTransferSet->dSConditionsRequested);
      printf ("\n     Block Data: %d", pstCurTransferSet->blockData);
      printf ("\n     Critical: %d", pstCurTransferSet->critical);
      printf ("\n     Report by Exception: %d", pstCurTransferSet->rBE);
      printf ("\n     Status: %d", pstCurTransferSet->status);
      pause_msg ("\n\n Press any key to continue");
      pstCurTransferSet = (struct Tag_MIT_TRANSFER_SET *)list_get_next (pstMitMicVcc->pstTransferSets,
                                   pstCurTransferSet);
      }
    }
  else
    {
    printf ("\n Remote '%s' not found", buffer);
    pause_msg (" ");
    }			
  (*menu_set_fun)();
  }
