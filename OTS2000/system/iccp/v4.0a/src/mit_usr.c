/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1986 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mit_usr.c    						*/
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
/* 07/26/02  RKR     12    added logging                                */
/* 07/25/02  RKR     11    added reorgDs                                */
/* 06/20/02  RKR     10    Copyright year update                        */
/* 06/18/02  RKR     09    Changes for tracking outstanding MMS Reqs	*/
/* 05/17/02  ASK     08    Updated Ident Resp info to version 2.02	*/
/* 02/13/02  ASK     07    Removed code associated with define          */
/*			   'HARD_CODED_CFG'				*/
/* 02/11/02  rkr     06    Added numRefDSTS				*/
/* 02/01/02  ASK     05	   Commented out restart assocs when stopped by */
/*			   user						*/
/* 01/16/02  ASK     04    Ported to Marben stack			*/
/* 01/09/02  ASK     03    Added Block 4 checking on link up msg	*/
/* 08/29/00  RKR     02    Added code to reset DS and DSTS startup lists*/
/* 01/28/98  MDE     01    New module					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "time.h"
#include "signal.h"
#include "mi.h"
#include "mi_icfg.h"
#include "icfg.h"

#include "mit_main.h"
#include "mitypeid.h"

static ST_BOOLEAN micEnable = SD_TRUE;

/* Identify server information						*/
IDENT_RESP_INFO identify_response_info =
  {
  "SISCO",  		/* Vendor 	*/
  "ICCP-Lite",  	/* Model  	*/
  "4.2150",  0,		/* Version 	*/
  };


/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/
/* MIC COMMUNICATIONS ACTIVITY INDICATION				*/
/* These functions are called by MI to tell the application about 	*/
/* client communications activity. 					*/
/************************************************************************/

/************************************************************************/
/* CONNECTION ACTIVITY							*/
/************************************************************************/

ST_VOID u_mi_association_active (MI_ASSOC_CTRL *mi_assoc)
  {
MIT_ASSOC_INFO *pstMitAssoc;
MIT_REMOTE     *pstCurRemote;                
MIT_DATA_SET	*pstMitDataSet;
MIT_MIC_VCC     *pstMitMicVcc;
ST_INT	i;

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING)
  {
    MIT_LOG_IND0 ("MI Association Active: Calling");
  }
  else
  {
    MIT_LOG_IND0 ("MI Association Active: Called");
  }
  
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT)
    {
    MIT_LOG_IND0 ("We are going to be a Client");

    MIT_LOG_FLOW0( "Time to start creating DataSets" );

/* the mi_assoc_ctrl references a MIT_ASSOC_INFO which in turn	*/
/* contains a reference to the MIT_MIC_VCC. We need to start Creating 	*/
/* any datasets configured in the MIT_MIC_VCC.				*/

    
    pstMitAssoc = (MIT_ASSOC_INFO *)mi_assoc->user_info[0];
    pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
    setReqCapacity(pstMitAssoc);

    if (pstMitMicVcc)
      {
      pstCurRemote = (MIT_REMOTE *) mi_assoc->mi_remote->user_info[0];
      if(pstCurRemote->blocks & ICFG_BLOCK4)
        {

        MIT_LOG_FLOW0 ("Starting Block 4");
        enableInformationMessages(pstMitAssoc);
      	}
      MIT_LOG_FLOW1 ("%d DataSets to create", pstMitMicVcc->numDataSets );
      if (pstMitMicVcc->numDataSets)
        {
        pstMitDataSet = pstMitMicVcc->pstDataSets;
        i=1;
        while (pstMitDataSet)
          {
          MIT_LOG_CFLOW2 ("DataSet %d, '%s'", i, pstMitDataSet->dsName);
          i++;
          pstMitDataSet = (struct Tag_MIT_DATA_SET *)list_get_next(
      			   pstMitMicVcc->pstDataSets,
			   pstMitDataSet);
          }					 
        createDataSets (pstMitAssoc);
        }
      }
    }
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_SERVER)
    {
    MIT_LOG_IND0 ("We are going to be a Server");
    }
  }

/************************************************************************/
/*			  u_mi_association_inactive			*/
/************************************************************************/

ST_VOID u_mi_association_inactive(MI_ASSOC_CTRL *mi_assoc, ST_INT reason)
  {
MIT_MIC_VCC	*pstMitMicVcc;
MIT_ASSOC_INFO  *pstMitAssoc;
MIT_DATA_SET	*pstCurDataSet;

  MIT_LOG_IND0 ("MI Association Inactive: ");
  if (reason == MI_ASSOC_INACTIVE_ABORT_IND)
    MIT_LOG_CIND0 ("Abort Indication");
  if (reason == MI_ASSOC_INACTIVE_ABORT_REQ)
    MIT_LOG_CIND0 ("Abort Confirm");
  if (reason == MI_ASSOC_INACTIVE_TERMINATE_IND)
    MIT_LOG_CIND0 ("Terminate Indication");
  if (reason == MI_ASSOC_INACTIVE_TERMINATE_CONF)
    MIT_LOG_CIND0 ("Terminate Confirm");

/* If we are calling, go ahead and re-enable the connection */
/*  if (micEnable && (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING))
    mi_assoc->ctrl_flags |= MI_ASSOC_CTRL_ENABLE; */

/* If we are a client and need to create DataSets or DSTransferSets 	*/
/* then reset the list.							*/
  if (micEnable && (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT))
    {
    pstMitAssoc = (MIT_ASSOC_INFO *)mi_assoc->user_info[0];
    if (pstMitAssoc->pstMitMicVcc)
      {
      /* there are DataSets to reset */
      if (pstMitAssoc->pstMitMicVcc->numDataSets)
        {
        MIT_LOG_FLOW1 ("%d future DataSets to create, resetting list", pstMitAssoc->pstMitMicVcc->numDataSets );
        pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
        pstMitMicVcc->pstNextDataSetToStart = pstMitMicVcc->pstDataSets;
        pstMitMicVcc->numStartedDataSets=0;


	pstCurDataSet = pstMitMicVcc->pstDataSets;
        while (pstCurDataSet)
          {
          pstCurDataSet->numRefDSTS=0;
          pstCurDataSet->reorgDs = SD_FALSE;
          pstCurDataSet->dsState = DS_IDLE;
          pstCurDataSet = (MIT_DATA_SET *) list_get_next 
                          ((ST_VOID *)pstMitMicVcc->pstDataSets, (ST_VOID *)pstCurDataSet);
          }
	} 
      /* there are DSTransferSets to reset */
      if (pstMitAssoc->pstMitMicVcc->numTransferSets)
        {
        MIT_LOG_FLOW1 ("%d future DSTransferSets to create, resetting list", pstMitAssoc->pstMitMicVcc->numTransferSets );
	pstMitMicVcc = pstMitAssoc->pstMitMicVcc;
        pstMitMicVcc->pstNextTransferSetToStart = pstMitMicVcc->pstTransferSets;
        pstMitMicVcc->numStartedTransferSets = 0;
	} 
      }
    }
  }

/************************************************************************/
/*			       u_mi_association_ind_err			*/
/************************************************************************/

ST_VOID u_mi_association_ind_err (ST_INT reason)
  {
  MIT_LOG_ERR0 ("MI Initiate Indication Error detected:");
  if (reason == MI_ASSOC_IND_ERR_NOREMOTE)
    MIT_LOG_ERR0 ("Could not match remote");
  if (reason == MI_ASSOC_IND_ERR_NOMISVCC)
    MIT_LOG_ERR0 ("Remote does not have a MIS_VCC");
  if (reason == MI_ASSOC_IND_ERR_NOASSOC)
    MIT_LOG_ERR0 ("Could find available MI_ASSOC");
  }

/************************************************************************/

ST_VOID u_mvl_getdom_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_getdom_resp (indCtrl);
  }
ST_VOID u_mvl_getvar_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_getvar_resp (indCtrl);
  }
ST_VOID u_mvl_namelist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_namelist_resp (indCtrl);
  }
ST_VOID u_mvl_ident_ind (MVL_IND_PEND *indCtrl)
  {
  indCtrl->u.ident.resp_info = &identify_response_info;
  mplas_ident_resp (indCtrl);
  }
ST_VOID u_mvl_write_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_write_resp (indCtrl);
  }
ST_VOID u_mvl_read_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_read_resp (indCtrl);
  }
ST_VOID u_mvl_defvlist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_defvlist_resp (indCtrl);
  }
ST_VOID u_mvl_delvlist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_delvlist_resp (indCtrl);
  }
ST_VOID u_mvl_getvlist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_getvlist_resp (indCtrl);
  }
ST_VOID u_mvl_info_rpt_ind (MVL_COMM_EVENT *event)
  {
  }

