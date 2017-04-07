/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mic_usr.c    						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     12    Copyright year update                        */
/* 06/20/02  MDE     11	   Use cleaned up mics_icfg names		*/
/* 04/16/02  MDE     10	   Corrected handling of StartTime		*/
/* 04/16/02  MDE     09	   Changed startup to use mic_get_rem_dv, etc..	*/
/* 03/25/02  MDE     08	   Added tolerance of mis-configured DS		*/
/* 03/21/02  MDE     07	   Remote AR info moved to MI_ASSOC (printed) 	*/
/* 01/25/02  MDE     06	   Added redundancy support code		*/
/* 01/22/02  MDE     05	   minor data value display changes		*/
/* 10/29/01  MDE     04	   Added USE_ICFG code				*/
/* 10/11/99  NAV     03    Add block 4 and 5 support			*/
/* 04/01/99  MDE     02    Use MI_ASSOC_CTRL for connection management	*/
/* 12/31/98  MDE     01    Created					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mi_usr.h"
#include "mi_icfg.h"
#include "mics_cfg.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/
/************************************************************************/
/* SAMPLE MI CLIENT CODE 						*/
/************************************************************************/
/************************************************************************/
/* Static Local functions */
      
static ST_VOID micuCreateAllDs (MI_CONN *mi_conn);
static ST_VOID micuCreateDs (MI_CONN *mi_conn, MICU_DS *miIcfgDs);
static ST_VOID micuCreateDsDone (MI_REQ_CTRL *req);

static ST_VOID micuStartAllDsTs (MI_CONN *mi_conn);
static ST_VOID micuStartDsTs (MI_CONN *mi_conn, MICU_DSTS  *miuDsTs);
static ST_VOID micuStartDsTsDone (MI_REQ_CTRL *req);

static ST_VOID micuStartIMTransfers (MI_CONN *mi_conn);
static ST_VOID micuStartIMTransferDone (MI_REQ_CTRL *req);

static ST_VOID micuStopAllDsTs (MI_CONN *mi_conn);
static ST_VOID micuStopDsTs (MI_CONN *mi_conn, MICU_DSTS  *miuDsTs);
static ST_VOID micuStopDsTsDone (MI_REQ_CTRL *req);

static ST_VOID micuStopIMTransfers (MI_CONN *mi_conn);
static ST_VOID micuStopIMTransfersDone (MI_REQ_CTRL *req);

static ST_VOID micuDvReadDone (MI_REQ_CTRL *req);
static ST_VOID micuReadDv (MI_CONN *mi_conn, MICU_DV *micIcfgDv);

static ST_VOID micuSelectDone  (MI_REQ_CTRL *miReq);
static ST_VOID micuOperateDone (MI_REQ_CTRL *miReq);
static ST_VOID micuSetTagDone  (MI_REQ_CTRL *miReq);
static ST_VOID micuGetTagDone  (MI_REQ_CTRL *miReq);
static ST_VOID micuShowDeviceResult (ST_RET mic_result);

static ST_VOID micuDvWrite (MI_CONN *mi_conn);
static ST_VOID micuDvWriteDone (MI_REQ_CTRL *req);

static ST_VOID micuGetDvsDone (MI_REQ_CTRL *miReq);
/*
ST_VOID micuCreateAllDs (MI_CONN *mi_conn);
ST_VOID micuCreateDs (MI_CONN *mi_conn, MICU_DS *miIcfgDs);
ST_VOID micuCreateDsDone (MI_REQ_CTRL *req);

ST_VOID micuStartAllDsTs (MI_CONN *mi_conn);
ST_VOID micuStartDsTs (MI_CONN *mi_conn, MICU_DSTS  *miuDsTs);
ST_VOID micuStartDsTsDone (MI_REQ_CTRL *req);

ST_VOID micuStartIMTransfers (MI_CONN *mi_conn);
ST_VOID micuStartIMTransferDone (MI_REQ_CTRL *req);

ST_VOID micuStopAllDsTs (MI_CONN *mi_conn);
ST_VOID micuStopDsTs (MI_CONN *mi_conn, MICU_DSTS  *miuDsTs);
ST_VOID micuStopDsTsDone (MI_REQ_CTRL *req);

ST_VOID micuStopIMTransfers (MI_CONN *mi_conn);
ST_VOID micuStopIMTransfersDone (MI_REQ_CTRL *req);

ST_VOID micuDvReadDone (MI_REQ_CTRL *req);
ST_VOID micuReadDv (MI_CONN *mi_conn, MICU_DV *micIcfgDv);

ST_VOID micuSelectDone  (MI_REQ_CTRL *miReq);
ST_VOID micuOperateDone (MI_REQ_CTRL *miReq);
ST_VOID micuSetTagDone  (MI_REQ_CTRL *miReq);
ST_VOID micuGetTagDone  (MI_REQ_CTRL *miReq);
ST_VOID micuShowDeviceResult (ST_RET mic_result);

ST_VOID micuDvWrite (MI_CONN *mi_conn);
ST_VOID micuDvWriteDone (MI_REQ_CTRL *req);

ST_VOID micuGetDvsDone (MI_REQ_CTRL *miReq);
*/

/************************************************************************/
/************************************************************************/
/*			micuStartAssocActivity 				*/
/************************************************************************/

/* Kick off a state machine for client activity on this association	*/
/*   1. Create all configured Data Sets					*/
/*   2. Start all configured DS Transfer Sets				*/
/*   4. Enable IM transfers						*/

ST_VOID micuStartAssocActivity (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;
MI_REQ_CTRL *miReq;
ST_BOOLEAN getTypes;

/* See if we need to get a list of remote DV */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  if (miuRemote->remDvValidation != ValNone ||
      miuRemote->remDvGetConfig != GetConfigNone)
    {

  /* OK, see if we need to get types too */
    if (miuRemote->remDvValidation == ValNamesAndTypes ||
        miuRemote->remDvGetConfig != GetConfigNone)
      {
      getTypes = SD_TRUE;
      }
    else
      getTypes = SD_FALSE;

    miReq = mic_get_rem_dv (mi_conn, NULL, getTypes);
    if (!miReq)
      {
      printf ("\nmic_get_rem_dv failed");
      return;
      }
    miReq->u_req_done = micuGetDvsDone;
    }
  else	/* No need to get the DV list */
    micuCreateAllDs (mi_conn);
  }
      
/************************************************************************/
/*			micuGetDvsDone  				*/
/************************************************************************/

static ST_VOID micuGetDvsDone (MI_REQ_CTRL *miReq)
  {
MI_REMOTE *mi_remote;
MI_CONN *mi_conn;
MIC_GROBJ_CTRL *mi_grobj_ctrl;
MIU_REMOTE *miuRemote;
ST_INT rc;
ST_CHAR xmlOutFile[200];

  mi_conn = miReq->mi_conn;
  mi_remote = mi_conn->mi_remote;
  miuRemote = miuFindMiRemote (mi_remote);

  if (miReq->result == MIC_RESULT_SUCCESS)
    {
    printf ("\nDataGetObj complete OK");
    mi_grobj_ctrl = miReq->mi_grobj_ctrl;

    if (miuRemote->remDvValidation != ValNone)
      {
      rc = mic_check_dv_cfg (mi_remote, mi_grobj_ctrl);
      if (rc == SD_SUCCESS)
        printf ("\n   Remote DV set compatible with configured DV set");
      else
        {
        printf ("\n   Warning: Remote DV set not compatible with configured DV set");
        printf ("\n            See log file for details");
        if (miuRemote->remDvExcludeInvalidFromDs == SD_FALSE)
          mic_reset_all_dv_mismatch (mi_remote);
        }
      }

    if (miuRemote->remDvGetConfig != GetConfigNone)
      {
      strcpy (xmlOutFile, "rem_");
      strcat (xmlOutFile, mi_conn->mi_remote->name);
      strcat (xmlOutFile, ".xml");
      rc = mic_encode_remote_xml (mi_conn->mi_assoc,  mi_grobj_ctrl, 
			        "RemoteList", xmlOutFile);
      if (rc == SD_SUCCESS)
        printf ("\nCreated IccpCfg XML file '%s'", xmlOutFile);
      else
        printf ("\nCould not create IccpCfg XML file '%s'", xmlOutFile);
      }

    mic_free_grobj_ctrl (mi_grobj_ctrl);
    }
  else
    {
    printf ("\nDataGetObj Failed");
    }
  mi_free_req_ctrl (miReq);
  micuCreateAllDs (mi_conn);
  }

/************************************************************************/
/************************************************************************/
/*			micuCreateAllDs					*/
/************************************************************************/

static ST_VOID micuCreateAllDs (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;
MICU_DS    *miIcfgDs;

/* Start with the first dataset */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  miIcfgDs = miuRemote->ds_list;
  if (!miIcfgDs)
    {
    printf ("\nNo remote datasets configured");
    micuStartAllDsTs (mi_conn);
    return;
    }

 /* Create a Data Set in the server */
  micuCreateDs (mi_conn, miIcfgDs);
  }

/************************************************************************/
/*			micuCreateDs					*/
/************************************************************************/

static ST_VOID micuCreateDs (MI_CONN *mi_conn, MICU_DS *miIcfgDs)
  {
MI_REQ_CTRL *miReq;

/* Create a Data Set in the server */
  miIcfgDs->mic_ds_ok = SD_FALSE;
  miReq = mic_create_ds (mi_conn, miIcfgDs->ds_name, 
				miIcfgDs->ds_scope,
				miIcfgDs->num_dv, 
				miIcfgDs->mic_dv, &miIcfgDs->mic_ds);

  if (miReq != NULL)
    {
    miReq->user_info[0] = miIcfgDs;
    miReq->u_req_done = micuCreateDsDone;
    }
  else
    {
    printf ("\n Error! Could not create the dataset");
    micuStartAllDsTs (mi_conn);
    }
  }

/************************************************************************/
/*			micuCreateDsDone				*/
/************************************************************************/

static ST_VOID micuCreateDsDone (MI_REQ_CTRL *miReq)
  {
MIU_REMOTE *miuRemote;
MICU_DS    *miIcfgDs;
MI_CONN *mi_conn;

  mi_conn = miReq->mi_conn;
  miIcfgDs = (MICU_DS *)miReq->user_info[0];
  if (miReq->result == SD_SUCCESS)
    {
    printf ("\nICCP Create DS '%s' OK", miIcfgDs->ds_name);
    miIcfgDs->mic_ds_ok = SD_TRUE;
    }
  else
    printf ("\nICCP Create DS '%s' Error", miIcfgDs->ds_name);
  mi_free_req_ctrl (miReq);

/* Ok, now define the next dataset, if there is one */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  miIcfgDs = (MICU_DS *)list_get_next (miuRemote->ds_list, miIcfgDs);
  if (miIcfgDs != NULL)
    micuCreateDs (mi_conn, miIcfgDs);
  else
    micuStartAllDsTs (mi_conn);
  }

/************************************************************************/
/************************************************************************/
/*			micuStartAllDsTs 				*/
/************************************************************************/

static ST_VOID micuStartAllDsTs (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;
MICU_DSTS  *miuDsTs;

/* Start with the first DSTS */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  miuDsTs = miuRemote->dsts_list;
  while (miuDsTs)
    {
    if (miuDsTs->miu_ds->mic_ds_ok)
      {
      micuStartDsTs (mi_conn, miuDsTs);
      break;
      }
    miuDsTs = (MICU_DSTS *)list_get_next (miuRemote->dsts_list, miuDsTs);
    }

  if (miuDsTs == NULL)
    {
    printf ("\nNo remote DSTS started");
    micuStartIMTransfers (mi_conn);
    }
  }


/************************************************************************/
/*			micuStartDsTs					*/
/************************************************************************/

static ST_VOID micuStartDsTs (MI_CONN *mi_conn, MICU_DSTS  *miuDsTs)
  {
MI_REQ_CTRL *miReq;
MI_DSTS_DATA mi_dsts_data;

  mi_dsts_data = miuDsTs->dsts_data;
  if (miuDsTs->dsts_data.StartTime != 0)
    mi_dsts_data.StartTime = miuDsTs->dsts_data.StartTime + time (NULL);

  miReq = mic_start_dsts (mi_conn, miuDsTs->miu_ds->mic_ds,  
			&mi_dsts_data, &miuDsTs->mic_dsts);

  if (miReq != NULL)
    {
    miReq->u_req_done =  micuStartDsTsDone;
    miReq->user_info[0] = miuDsTs;
    }
  else
    {
    printf ("\n Error! Could not start the DSTS");
    micuStartIMTransfers (mi_conn);
    }
  }

/************************************************************************/
/*	       	micuStartDsTsDone 					*/
/************************************************************************/

static ST_VOID micuStartDsTsDone (MI_REQ_CTRL *miReq)
  {
MIU_REMOTE *miuRemote;
MICU_DSTS  *miuDsTs;
MI_CONN *mi_conn;

  mi_conn = miReq->mi_conn;
  miuDsTs = (MICU_DSTS *)miReq->user_info[0];
  if (miReq->result == SD_SUCCESS)
    printf ("\nICCP Start DSTS '%s' OK", miuDsTs->dsTsName);
  else
    printf ("\nICCP Start DSTS '%s' Error", miuDsTs->dsTsName);
  mi_free_req_ctrl (miReq);

/* Ok, now define the next dataset */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  miuDsTs = (MICU_DSTS *)list_get_next (miuRemote->dsts_list, miuDsTs);
  while (miuDsTs)
    {
    if (miuDsTs->miu_ds->mic_ds_ok)
      {
      micuStartDsTs (mi_conn, miuDsTs);
      break;
      }
    miuDsTs = (MICU_DSTS *)list_get_next (miuRemote->dsts_list, miuDsTs);
    }
  if (miuDsTs == NULL)
    micuStartIMTransfers (mi_conn);
  }

/************************************************************************/
/************************************************************************/
/*		      micuStartIMTransfers				*/
/************************************************************************/

static ST_VOID micuStartIMTransfers (MI_CONN *mi_conn)
  {
MI_REQ_CTRL *miReq;

  printf ("\n Starting IMTransfers for remote '%s'", mi_conn->remote_ar);
  miReq = mic_start_IMTransfer (mi_conn);
  if (miReq != NULL)
    miReq->u_req_done = micuStartIMTransferDone;
  else
    {
    printf ("\n Failed!!!");
    if (micuLoopEnable == SD_TRUE)
      micuStopAssocActivity (mi_conn);
    }
  }

/************************************************************************/
/* 		micuStartIMTransferDone 				*/
/************************************************************************/

static ST_VOID micuStartIMTransferDone (MI_REQ_CTRL *miReq)
  {
MI_CONN *mi_conn;
MVL_REQ_PEND *mvlReqPend;

  mvlReqPend = miReq->mvl_req;
  mi_conn = mvlReqPend->net_info->mi_conn; 
  printf ("\n START IMTransfer result = 0x%x", miReq->result);
  mi_free_req_ctrl (miReq);

  if (micuLoopEnable == SD_TRUE)
    micuStopAssocActivity (mi_conn);
  }

/************************************************************************/
/************************************************************************/
/*			micuStopAssocActivity 				*/
/************************************************************************/

/* Kick off a state machine to stop client activity on this association	*/
/*   1. Stop all configured DS Transfer Sets				*/
/*   2. Disable IM transfers						*/
/*   3. Disable the association						*/

ST_VOID micuStopAssocActivity (MI_CONN *mi_conn)
  {
  micuStopAllDsTs (mi_conn);
  }

/************************************************************************/
/*			micuStopAllDsTs 					*/
/************************************************************************/

static ST_VOID micuStopAllDsTs (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;
MICU_DSTS  *miuDsTs;

/* Start with the first DSTS */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  miuDsTs = miuRemote->dsts_list;
  while (miuDsTs)
    {
    if (miuDsTs->miu_ds->mic_ds_ok)
      {
      micuStopDsTs (mi_conn, miuDsTs);
      break;
      }
    miuDsTs = (MICU_DSTS *)list_get_next (miuRemote->dsts_list, miuDsTs);
    }

  if (miuDsTs == NULL)
    micuStopIMTransfers (mi_conn);
  }

/************************************************************************/
/*			micuStopDsTs 					*/
/************************************************************************/

static ST_VOID micuStopDsTs (MI_CONN *mi_conn, MICU_DSTS *miuDsTs)
  {
MI_REQ_CTRL *miReq;

  miReq = mic_stop_dsts (mi_conn, miuDsTs->mic_dsts);
  if (miReq != NULL)
    {
    miReq->u_req_done =  micuStopDsTsDone;
    miReq->user_info[0] = miuDsTs;
    }
  else
    {
    printf ("\n Error! Could not stop the DSTS");
    micuStopIMTransfers (mi_conn);
    }
  }

/************************************************************************/
/*	       	micuStopDsTsDone 					*/
/************************************************************************/

static ST_VOID micuStopDsTsDone (MI_REQ_CTRL *miReq)
  {
MIU_REMOTE *miuRemote;
MICU_DSTS  *miuDsTs;
MI_CONN *mi_conn;

  mi_conn = miReq->mi_conn;
  miuDsTs = (MICU_DSTS *)miReq->user_info[0];
  if (miReq->result == SD_SUCCESS)
    printf ("\nICCP Stop TS OK");
  else
    printf ("\nICCP Stop TS Error");
  mi_free_req_ctrl (miReq);


/* Ok, now stop the next one ... */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  miuDsTs = (MICU_DSTS *)list_get_next (miuRemote->dsts_list, miuDsTs);
  while (miuDsTs)
    {
    if (miuDsTs->miu_ds->mic_ds_ok)
      {
      micuStartDsTs (mi_conn, miuDsTs);
      break;
      }
    miuDsTs = (MICU_DSTS *)list_get_next (miuRemote->dsts_list, miuDsTs);
    }
  if (miuDsTs == NULL)
    micuStopIMTransfers (mi_conn);
  }

/************************************************************************/
/************************************************************************/
/*		      micuStopIMTransfers					*/
/************************************************************************/

static ST_VOID micuStopIMTransfers (MI_CONN *mi_conn)
  {
MI_REQ_CTRL *miReq;

  printf ("\n Stopping IMTransfers for remote '%s'", mi_conn->remote_ar);
  miReq = mic_stop_IMTransfer (mi_conn);
  if (!miReq)
    miReq->u_req_done = micuStopIMTransfersDone;
  else
    {
    printf ("\n Failed!!!");
    mi_disable_assoc (mi_conn->mi_assoc);
    }
  }

/************************************************************************/
/*			micuStopIMTransfersDone 				*/
/************************************************************************/

static ST_VOID micuStopIMTransfersDone (MI_REQ_CTRL *miReq)
  {
MI_CONN *mi_conn;
MVL_REQ_PEND *mvlReqPend;

  mvlReqPend = miReq->mvl_req;
  mi_conn = mvlReqPend->net_info->mi_conn;
  printf ("\n STOP IMTransfer for '%s' Result = 0x%x", 
	  mi_conn->mi_assoc->remote_ar, miReq->result);
  mi_free_req_ctrl (miReq);
  mi_disable_assoc (mi_conn->mi_assoc);
  }

/************************************************************************/
/************************************************************************/
/*			micuReadAllDv 					*/
/************************************************************************/

ST_VOID micuReadAllDv (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;

/* Start with the first data value */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  micuReadDv (mi_conn, miuRemote->dv_list);
  }

/************************************************************************/
/*			micuReadDv					*/
/************************************************************************/

#define MAX_DV_PER_READ	100
ST_INT numReadOut = 0;

ST_VOID micuReadDv (MI_CONN *mi_conn, MICU_DV *micIcfgDv)
  {
MIU_REMOTE *miuRemote;
MI_REQ_CTRL *miReq;
MIC_DV *dv[MAX_DV_PER_READ];
ST_INT i;

  if (micIcfgDv == NULL)
    return;

/* Read a number of DV ... */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  for (i = 0; i < MAX_DV_PER_READ && micIcfgDv; ++i)
    {
    dv[i] = micIcfgDv->mic_dv;
    micIcfgDv = (MICU_DV *)list_get_next (miuRemote->dv_list, micIcfgDv);
    }

  miReq = mic_read_dv (mi_conn, i, dv);
  if (!miReq)
    {
    printf ("\n Error! Could not read the data values");
    return;
    }

  ++numReadOut;
  miReq->u_req_done =  micuDvReadDone;
  miReq->user_info[0] = micIcfgDv;
  }

/************************************************************************/
/*			micuDvReadDone 					*/
/************************************************************************/

static ST_VOID micuDvReadDone (MI_REQ_CTRL *miReq)
  {
MI_CONN *mi_conn;
MIU_REMOTE *miuRemote;
MICU_DV *micIcfgDv;

  --numReadOut;

  mi_conn = miReq->mi_conn;
  micIcfgDv = (MICU_DV *)miReq->user_info[0];
  if (miReq->result == SD_SUCCESS)
    printf ("\nDV Read OK");
  else
    printf ("\nDV Read Error");
  mi_free_req_ctrl (miReq);

/* Ok, now read the next bunch */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);

  if (micIcfgDv != NULL)  /* If this was not the last ... */
    micIcfgDv = (MICU_DV *)list_get_next (miuRemote->dv_list, micIcfgDv);

  micuReadDv (mi_conn, micIcfgDv);
  }

/************************************************************************/
/************************************************************************/
/* DATA VALUE ACTIVITY							*/
/************************************************************************/
/*			u_mic_dv_ind_start 				*/
/************************************************************************/

/* This function is called when a set of new Data Value values is 	*/
/* received, and before MI starts calling 'u_mic_dv_ind'. After all 	*/
/* calls to 'u_mic_dv_ind' have been made for the MMS PDU, 		*/
/* 'u_mic_dv_ind_end' will be called.					*/

ST_VOID u_mic_dv_ind_start (MI_CONN *mi_conn, ST_INT reason,
			    MIC_DSTS *mic_dsts)
  {
  if (reason == MIC_DV_IND_REASON_READ)
    printf ("\n\nRead response DV indications starting ...");
  if (reason == MIC_DV_IND_REASON_REPORT)
    printf ("\n\nReport DV indications starting ...");
  }

/************************************************************************/
/*			u_mic_dv_ind 					*/
/************************************************************************/

/* This function is called once for each Data Value received via Read	*/
/* confirm or Information Report indication.				*/

/* The application can do what it pleases with the data; in the case of	*/
/* our sample we will simply print it. The 'handle_info' is a 'tag'	*/
/* string to use in the print.						*/

ST_VOID u_mic_dv_ind (MI_CONN *mi_conn, MIC_DV *dv, ST_RET result,
		      ST_VOID *data_buf, ST_INT mi_type)
  {
MICU_DV 		*micIcfgDv;

MI_REAL 		*dr;
MI_STATE 	   	*ds;
MI_DISCRETE 	   	*dd;
MI_REAL_Q 	   	*drq;
MI_STATE_Q 	   	*dsq;
MI_DISCRETE_Q 	   	*ddq;
MI_REAL_Q_TIMETAG 	*drqt;
MI_STATE_Q_TIMETAG    	*dsqt;
MI_DISCRETE_Q_TIMETAG 	*ddqt;
MI_REAL_EXTENDED 	*dre;
MI_STATE_EXTENDED    	*dse;
MI_DISCRETE_EXTENDED 	*dde;

MI_MMS_OBJECT_NAME 	*ts_name;
MI_TS_TIMESTAMP    	*ts_timestamp;
MI_DS_CONDITIONS   	*ds_cond;
MI_EC_DETECTED     	*ec_detected;
ST_CHAR timeBuf[30];

  if (result != SD_SUCCESS)
    {
    printf ("\n Read DV Error!");
    return;
    }

  printf ("\nDV:%s\t", dv->dv_name);
  micIcfgDv = (MICU_DV *)dv->handle_info;

  switch (mi_type)
    {
    case MI_TYPEID_REAL:
      dr = (float *)data_buf;
      printf ("Val: %f  ", *dr);

    /* Store the received remote data for later use */
      micIcfgDv->data.r = *dr;
    break;
  
    case MI_TYPEID_STATE:
      ds = (unsigned char *)data_buf;
      printf ("Flags: %02x  ", (int) *ds);

    /* Store the received remote data for later use */
      micIcfgDv->data.s = *ds;
    break;
  
    case MI_TYPEID_DISCRETE:
      dd = (long *)data_buf;
      printf ("Val: %ld  ", *dd);

    /* Store the received remote data for later use */
      micIcfgDv->data.d = *dd;
    break;
  
    case MI_TYPEID_REAL_Q:
      drq = (MI_REAL_Q *)data_buf;
      printf ("Val: %f  ", drq->Value);
      printf ("Flags: %02x  ", (int) drq->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.rq = *drq;
    break;
  
    case MI_TYPEID_STATE_Q:
      dsq = (unsigned char *)data_buf;
      printf ("Flags: %02x  ", (int) dsq);

    /* Store the received remote data for later use */
      micIcfgDv->data.sq = *dsq;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      ddq = (MI_DISCRETE_Q *)data_buf;
      printf ("Val: %ld  ", ddq->Value);
      printf ("Flags: %02x  ", (int) ddq->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.dq = *ddq;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      drqt = (MI_REAL_Q_TIMETAG *)data_buf;
      printf ("Val: %f  ", drqt->Value);
      strcpy (timeBuf, ctime(&drqt->TimeStamp));
      timeBuf[24] = 0;
      printf ("TS: %s  ", timeBuf);
      printf ("Flags: %02x  ", (int) drqt->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.rqt = *drqt;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      dsqt = (MI_STATE_Q_TIMETAG *)data_buf;
      strcpy (timeBuf, ctime(&dsqt->TimeStamp));
      timeBuf[24] = 0;
      printf ("TS: %s  ", timeBuf);
      printf ("Flags: %02x  ", (int) dsqt->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.sqt = *dsqt;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      ddqt = (MI_DISCRETE_Q_TIMETAG *)data_buf;
      printf ("Val: %ld  ", ddqt->Value);
      strcpy (timeBuf, ctime(&ddqt->TimeStamp));
      timeBuf[24] = 0;
      printf ("TS: %s  ", timeBuf);
      printf ("Flags: %02x  ", (int) ddqt->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.dqt = *ddqt;
    break;
  
    case MI_TYPEID_REAL_EXTENDED:
      dre = (MI_REAL_EXTENDED *)data_buf;
      printf ("Val: %f  ", dre->Value);
      strcpy (timeBuf, ctime(&dre->TimeStamp));
      timeBuf[24] = 0;
      printf ("TS: %s  ", timeBuf);
      printf ("COV: %d  ", (int)dre->COV);
      printf ("Flags: %02x  ", (int) dre->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.re = *dre;
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      dse = (MI_STATE_EXTENDED *)data_buf;
      strcpy (timeBuf, ctime(&dse->TimeStamp));
      timeBuf[24] = 0;
      printf ("TS: %s  ", timeBuf);
      printf ("COV: %d  ", (int)dse->COV);
      printf ("Flags: %02x  ", (int) dse->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.se = *dse;
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      dde = (MI_DISCRETE_EXTENDED *)data_buf;
      printf ("Val: %ld  ", dde->Value);
      strcpy (timeBuf, ctime(&dde->TimeStamp));
      timeBuf[24] = 0;
      printf ("TS: %s  ", timeBuf);
      printf ("COV: %d  ", (int)dde->COV);
      printf ("Flags: %02x  ", (int) dde->Flags);

    /* Store the received remote data for later use */
      micIcfgDv->data.de = *dde;
    break;

  /* the rest are not data values */
    case MI_TYPEID_MMS_OBJECT_NAME:
      ts_name = (MI_MMS_OBJECT_NAME *)data_buf;
      printf ("TS Name: %s  ", ts_name->Name);
      if (ts_name->Scope == 1)
        printf (  ", Domain: %s  ", ts_name->DomainName);
    break;

    case MI_TYPEID_GMT_BASED_S:
      ts_timestamp = (long *)data_buf;
      printf ("TS TimeStamp: %ld  ", (long) *ts_timestamp);
    break;

    case MI_TYPEID_INT16:
      ec_detected = (short *)data_buf;
      printf ("Event Code Detected: 0x%02x  ", (int) *ec_detected);
    break;

    case MI_TYPEID_DS_COND:
      ds_cond = (unsigned char *)data_buf;
      printf ("DSConditions Detected:");
      if (*ds_cond & MI_DSC_INTERVAL)
        printf (" Interval");
      if (*ds_cond & MI_DSC_INTEGRITY)
        printf (" Integrity");
      if (*ds_cond & MI_DSC_CHANGE)
        printf (" Change");
      if (*ds_cond & MI_DSC_OPERATOR)
        printf (" Operator");
      if (*ds_cond & MI_DSC_EXTERNAL)
        printf (" External");
    break;
  
    default:
      printf ("\nError - Check Me Out!");
    break;
    }
  }

/************************************************************************/
/*			u_mic_dv_ind_end   				*/
/************************************************************************/

/* This function is called when all calls to 'u_mic_dv_ind' have been 	*/
/* made for the MMS PDU.						*/

ST_VOID u_mic_dv_ind_end   (MI_CONN *mi_conn, ST_INT reason,
			    MIC_DSTS *mic_dsts, MIC_DSTS_STO *sto)
  {
ST_RET rc;
static ST_INT ackCount = 0;

  if (reason == MIC_DV_IND_REASON_READ)
    printf ("\nRead response DV indications complete");
  if (reason == MIC_DV_IND_REASON_REPORT)
    {
    printf ("\nReport DV indications complete");
    if (mic_dsts->mi_dsts_data.Critical)
      {
      ++ackCount;
      if (ackCount % 3)
        rc = mic_ack_dsts (mi_conn, mic_dsts, sto, SD_FALSE);
      else
        rc = mic_ack_dsts (mi_conn, mic_dsts, sto, SD_TRUE);

      if (rc != SD_SUCCESS)
        printf ("\nReport ACK error %d", rc);
      }
    }
  }

/************************************************************************/
/************************************************************************/
/*			micuDvWrite 					*/
/************************************************************************/

#define MAX_DV_PER_WRITE	100

ST_VOID micuWriteDv (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;
MICU_DV 	*micIcfgDv;
MI_REQ_CTRL *miReq;
MIC_WR_DV_CTRL *wrDvCtrl;
MIC_DV_WR_INFO *dvWrInfo;
ST_INT i;

/*  wrDvCtrl = (_mic_wr_dv_ctrl *)chk_calloc (1, sizeof (MIC_WR_DV_CTRL) +
modified 2006.11.24 zy*/
  wrDvCtrl = (struct _mic_wr_dv_ctrl *)chk_calloc (1, sizeof (MIC_WR_DV_CTRL) +
                            MAX_DV_PER_WRITE * sizeof (MIC_DV_WR_INFO));
  wrDvCtrl->mic_dv_wr_info = (MIC_DV_WR_INFO *) (wrDvCtrl+1);
  dvWrInfo = wrDvCtrl->mic_dv_wr_info;

/* Write a number of DV; assume that the data has been set 		*/
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  micIcfgDv = miuRemote->dv_list;
  for (i = 0; i < MAX_DV_PER_WRITE && micIcfgDv; ++i)
    {
    ++wrDvCtrl->num_dv;
    dvWrInfo[i].mic_dv = micIcfgDv->mic_dv;
    dvWrInfo[i].wr_data = &micIcfgDv->data;
    micIcfgDv = (MICU_DV *)list_get_next (miuRemote->dv_list, micIcfgDv);
    }

  miReq = mic_write_dv (mi_conn, wrDvCtrl);
  if (!miReq)
    {
    printf ("\n Error! Could not issue the write");
    return;
    }
  miReq->u_req_done =  micuDvWriteDone;
  }

/************************************************************************/
/*			micuDvWriteDone 	      			*/
/************************************************************************/

static ST_VOID micuDvWriteDone (MI_REQ_CTRL *miReq)
  {
MIC_WR_DV_CTRL *wrDvCtrl;
MIC_DV_WR_INFO *dvWrInfo;
ST_INT i;

  wrDvCtrl = miReq->mic_wr_dv_ctrl;
  dvWrInfo = wrDvCtrl->mic_dv_wr_info;

  if (miReq->result == SD_SUCCESS)
    {
    printf ("\nICCP DV Write Confirm OK");
    for (i = 0; i < wrDvCtrl->num_dv; ++i)
      {
      if (dvWrInfo[i].result != SD_SUCCESS)
        printf ("\n DV '%d' Write Error 0x%02x", i, dvWrInfo[i].result);
      }
    }
  else
    printf ("\nICCP DV Write Error");

  chk_free (wrDvCtrl);
  mi_free_req_ctrl (miReq);
  }

/************************************************************************/
/* Block 4								*/
/************************************************************************/
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
/* Block 5								*/
/************************************************************************/
/************************************************************************/

static ST_VOID micuDeviceTest (MI_CONN *mi_conn, MICU_DEV *micIcfgDev);
static ST_VOID micuTestNextDevice (MI_CONN *mi_conn, 
			    MIU_REMOTE *miuRemote, 
			    MICU_DEV *micIcfgDev);

static ST_VOID micuSelectDev (MI_CONN *mi_conn, MICU_DEV *micIcfgDev);
static ST_VOID micuOperateDev (MI_CONN *mi_conn, MICU_DEV *micIcfgDev);
static ST_VOID micuSetTag (MI_CONN *mi_conn, MICU_DEV *micIcfgDev);
static ST_VOID micuGetTag (MI_CONN *mi_conn, MICU_DEV *micIcfgDev);

static ST_INT numDevTested;
static ST_INT numDevTestErr;

/************************************************************************/
/************************************************************************/
/*			micuTestAllDevices 				*/
/************************************************************************/

ST_VOID micuTestAllDevices (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;
MICU_DEV *micIcfgDev;

  numDevTested = 0;
  numDevTestErr = 0;

/* Start with the first configured device */
  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  if (miuRemote->dev_list != NULL)
    {
    micIcfgDev = miuRemote->dev_list;
    micuDeviceTest (mi_conn, micIcfgDev);
    }
  else
    printf ("\nNo Devices To Test");
  }

/************************************************************************/
/*			micuTestNextDevice 				*/
/************************************************************************/

static ST_VOID micuTestNextDevice (MI_CONN *mi_conn, 
			    MIU_REMOTE *miuRemote, 
			    MICU_DEV *micIcfgDev)
  {

/* Get the next configured device */
  micIcfgDev = (MICU_DEV *)list_get_next (miuRemote->dev_list, micIcfgDev);
  if (micIcfgDev != NULL)
    micuDeviceTest (mi_conn, micIcfgDev);
  else
    printf ("\n%d Devices Tested, %d errors", numDevTested, numDevTestErr);
  }

/************************************************************************/
/*			micuDeviceTest 					*/
/************************************************************************/

static ST_VOID micuDeviceTest (MI_CONN *mi_conn, MICU_DEV *micIcfgDev)
  {
MIU_REMOTE *miuRemote;
MIC_DEVICE *mic_dev;

  ++numDevTested;

  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  mic_dev = micIcfgDev->mic_dev;

/* If this is a SBO device, need to select first 			*/
  if (mic_dev->device_attrib & MIC_DEVICE_SBO)
    micuSelectDev (mi_conn, micIcfgDev);
  else	/* Not a SBO device, just operate it ... */
    micuOperateDev (mi_conn, micIcfgDev);
  }

/************************************************************************/
/*			micuSelectDev 					*/
/************************************************************************/

static ST_VOID micuSelectDev (MI_CONN *mi_conn, MICU_DEV *micIcfgDev)
  {
MIU_REMOTE *miuRemote;
MIC_DEVICE *mic_dev;
MI_REQ_CTRL *miReq;

  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  mic_dev = micIcfgDev->mic_dev;

/* If this is a SBO device, need to select first 			*/
  miReq = mic_device_select (mi_conn, micIcfgDev->mic_dev);
  if (!miReq)
    {
    SLOGALWAYS0 ("mic_device_select Failed.");
    ++numDevTestErr;
    }
  else
    {
    miReq->u_req_done = micuSelectDone;
    miReq->user_info[0] = micIcfgDev;
    }
  }

/************************************************************************/
/*			micuSelectDone  				*/
/************************************************************************/

static ST_VOID micuSelectDone  (MI_REQ_CTRL *miReq)
  {
MICU_DEV *micIcfgDev;
MI_CONN *mi_conn;
MIC_DEVICE *mic_dev;

  mi_conn = miReq->mi_conn;
  micIcfgDev = (MICU_DEV *)miReq->user_info[0];
  mic_dev = miReq->mic_device;

  SLOGALWAYS1 ("DeviceSelect for device %s completed", mic_dev->device_name);
  micuShowDeviceResult (miReq->result);

  if (miReq->result == MIC_RESULT_SUCCESS)
    SLOGCALWAYS1 ("CheckBack ID = %d", miReq->o.check_back_id);

  mi_free_req_ctrl (miReq);
  micuOperateDev (mi_conn, micIcfgDev);
  }

/************************************************************************/
/************************************************************************/
/*			micuOperateDev 					*/
/************************************************************************/

static ST_VOID micuOperateDev (MI_CONN *mi_conn, MICU_DEV *micIcfgDev)
  {
MIC_DEVICE *mic_dev;
MI_DEV_VALUE_UNION devValue;
MI_REQ_CTRL *miReq;
static MI_CONTROL_COMMAND devCmdValue = 0;
static MI_CONTROL_SETPOINT_REAL devRealValue = 0.0;
static MI_CONTROL_SETPOINT_DISCRETE devDiscreteValue = 0;

  mic_dev = micIcfgDev->mic_dev;
  switch (micIcfgDev->dev_type)
    {
    case MI_TYPEID_DEV_COMMAND:
      devValue.cmd = ++devCmdValue;
    break;

    case MI_TYPEID_DEV_REAL:
      devRealValue += (MI_CONTROL_SETPOINT_REAL) 1.1;
      devValue.set_pt_r = devRealValue;
    break;

    case MI_TYPEID_DEV_DISCRETE:
      devValue.set_pt_d = ++devDiscreteValue;
    break;
    }

  SLOGALWAYS1 ("Operating device %s", mic_dev->device_name);
  miReq = mic_device_operate (mi_conn, mic_dev, &devValue);
  if (!miReq)
    {
    SLOGCALWAYS0 ("mic_device_operate failed.");
    ++numDevTestErr;
    }
  else
    {
    miReq->u_req_done = micuOperateDone;
    miReq->user_info[0] = micIcfgDev;
    }
  }

/************************************************************************/
/*			micuOperateDone 				*/
/************************************************************************/

static ST_VOID micuOperateDone (MI_REQ_CTRL *miReq)
  {
MIU_REMOTE *miuRemote;
MICU_DEV *micIcfgDev;
MI_CONN *mi_conn;
MIC_DEVICE *mic_dev;

  mi_conn = miReq->mi_conn;
  micIcfgDev = (MICU_DEV *)miReq->user_info[0];
  mic_dev = miReq->mic_device;

  SLOGALWAYS1 ("DeviceOperate for device %s completed", mic_dev->device_name);
  micuShowDeviceResult (miReq->result);

  mi_free_req_ctrl (miReq);

/* If this device is taggable, let's tag it */
  if (mic_dev->device_attrib & MIC_DEVICE_TAGGABLE)
    micuSetTag (mi_conn, micIcfgDev);
  else /* not taggable, we are done with this one ... */
    {
    miuRemote = miuFindMiRemote (mi_conn->mi_remote);
    micuTestNextDevice (mi_conn, miuRemote, micIcfgDev);
    }
  }

/************************************************************************/
/************************************************************************/
/*			micuSetTag 					*/
/************************************************************************/

static ST_VOID micuSetTag (MI_CONN *mi_conn, MICU_DEV *micIcfgDev)
  {
MIC_DEVICE *mic_dev;
MI_REQ_CTRL *miReq;
MI_TAG_VALUE tagValue;

  mic_dev = micIcfgDev->mic_dev;
  SLOGALWAYS1 ("Tagging device %s", mic_dev->device_name);
  tagValue.TagFlags = 0x80;
  strcpy (tagValue.Reason, "A Reason");
  miReq = mic_device_settag (mi_conn, mic_dev, &tagValue);
  if (!miReq)
    {
    SLOGCALWAYS1 ("mic_device_settag failed (0x%04x)", mi_op_err);
    ++numDevTestErr;
    }
  else
    {
    miReq->u_req_done = micuSetTagDone;
    miReq->user_info[0] = micIcfgDev;
    }
  }

/************************************************************************/
/*			micuSetTagDone  				*/
/************************************************************************/

static ST_VOID micuSetTagDone  (MI_REQ_CTRL *miReq)
  {
MICU_DEV *micIcfgDev;
MI_CONN *mi_conn;
MIC_DEVICE *mic_dev;

  mi_conn = miReq->mi_conn;
  micIcfgDev = (MICU_DEV *)miReq->user_info[0];
  mic_dev = miReq->mic_device;

  SLOGALWAYS1 ("DeviceSetTag for device %s completed", mic_dev->device_name);
  micuShowDeviceResult (miReq->result);

  mi_free_req_ctrl (miReq);

  /* now try to get the tag		*/
  micuGetTag (mi_conn, micIcfgDev);
  }

/************************************************************************/
/************************************************************************/
/*			micuGetTag 					*/
/************************************************************************/

static ST_VOID micuGetTag (MI_CONN *mi_conn, MICU_DEV *micIcfgDev)
  {
MIC_DEVICE *mic_dev;
MI_REQ_CTRL *miReq;

  mic_dev = micIcfgDev->mic_dev;
  SLOGALWAYS1 ("Getting device %s tag", mic_dev->device_name);
  miReq = mic_device_gettag (mi_conn, mic_dev);
  if (!miReq)
    {
    SLOGCALWAYS0 ("mic_device_gettag failed");
    ++numDevTestErr;
    }
  else
    {
    miReq->u_req_done = micuGetTagDone;
    miReq->user_info[0] = micIcfgDev;
    }
  }

/************************************************************************/
/*			micuGetTagDone  				*/
/************************************************************************/

static ST_VOID micuGetTagDone  (MI_REQ_CTRL *miReq)
  {
MIU_REMOTE *miuRemote;
MICU_DEV *micIcfgDev;
MI_CONN *mi_conn;
MIC_DEVICE *mic_dev;

  mi_conn = miReq->mi_conn;
  micIcfgDev = (MICU_DEV *)miReq->user_info[0];
  mic_dev = miReq->mic_device;

  SLOGALWAYS1 ("DeviceGetTag complete for device '%s'", mic_dev->device_name);
  micuShowDeviceResult (miReq->result);
  if (miReq->result == MIC_RESULT_SUCCESS)
    {
    SLOGCALWAYS1 ("  TagFlags = 0x%x", miReq->o.tag_value->TagFlags);
    SLOGCALWAYS1 ("  Reason = '%s'", miReq->o.tag_value->Reason);
    }
  mi_free_req_ctrl (miReq);

  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  micuTestNextDevice (mi_conn, miuRemote, micIcfgDev);
  }

/************************************************************************/
/*			micuShowDeviceResult 				*/
/************************************************************************/

static ST_VOID micuShowDeviceResult (ST_RET mic_result)
  {
   switch (mic_result)
    {
    case MIC_RESULT_SUCCESS:
      SLOGCALWAYS0 ("  SUCCESS");
    break;

    case MIC_RESULT_OBJ_NON_EXISTENT:
      SLOGCALWAYS0 ("  Device does not exist.");
      ++numDevTestErr;
    break;

    case MIC_RESULT_ACCESS_DENIED:
      SLOGCALWAYS0 ("  Access Denied.");
      ++numDevTestErr;
    break;

    case MIC_RESULT_HARDWARE_FAULT:
      SLOGCALWAYS0 ("  Hardware Fault");
      ++numDevTestErr;
    break;

    case MIC_RESULT_TEMP_UNAVAIL:
      SLOGCALWAYS0 ("  Temporarily Unavailable.");
      ++numDevTestErr;
    break;

    default:
      SLOGCALWAYS1 ("  Undefined error %d", mic_result);
      ++numDevTestErr;
    break;
    }
  }




/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			micuGetDvAttrib					*/
/************************************************************************/

static ST_VOID micuGetDvAttribDone (MI_REQ_CTRL *miReq);


ST_VOID micuGetDvAttrib (MI_CONN *mi_conn)
  {
MIU_REMOTE *miuRemote;
MI_REQ_CTRL *miReq;
MICU_DV *micIcfgDv;
ST_INT scope;

  miuRemote = miuFindMiRemote (mi_conn->mi_remote);
  micIcfgDv = miuRemote->dv_list;
  if (micIcfgDv == NULL)
    return;

  if (micIcfgDv->mic_dv->dv_attrib & MIC_DV_SCOPE_ICC)
    scope = ICC_SPEC;
  else
    scope = VCC_SPEC;

  miReq = mic_get_dv_attrib (mi_conn, micIcfgDv->mic_dv->dv_name, scope);
  if (!miReq)
    {
    printf ("\n Error! Could not get DV attrib");
    return;
    }
  miReq->u_req_done =  micuGetDvAttribDone;
  }

/************************************************************************/
/*			micuGetDvAttribDone 				*/
/************************************************************************/

static ST_VOID micuGetDvAttribDone (MI_REQ_CTRL *miReq)
  {
MI_CONN *mi_conn;

  mi_conn = miReq->mi_conn;
  if (miReq->result == SD_SUCCESS)
    printf ("\nGet DV Attrib OK");
  else
    printf ("\nGet DV Attrib Error");
  mi_free_req_ctrl (miReq);
  }
