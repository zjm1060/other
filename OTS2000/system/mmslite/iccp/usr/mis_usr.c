/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mis_usr.c  						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     10    Copyright year update                        */
/* 06/20/02  MDE     09	   Use cleaned up mics_icfg names		*/
/* 04/16/02  MDE     08	   Moved miu_dv_list, miu_dev_list to mics_cfg	*/
/* 01/25/02  MDE     07	   Added redundancy support code		*/
/* 12/07/01  MDE     06	   Don't alloc 0 size conn array for no assoc	*/
/* 10/29/01  MDE     05	   Added USE_ICFG code				*/
/* 10/19/01  MDE     04	   Dynamic local object reconfiguration changes	*/
/* 10/11/99  NAV     03    Add Block 4 and 5 support			*/
/* 04/01/99  MDE     02    Use MI_ASSOC_CTRL for connection management	*/
/* 12/31/98  MDE     01    Created					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mi_usr.h"
#include "miusrobj.h"
#include "mi_icfg.h"
#include "mics_cfg.h"


/************************************************************************/
#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/
/* SAMPLE MI SERVER CODE */
/************************************************************************/
/************************************************************************/
/* DATA VALUE HANDLING							*/

/* In our sample a configurable number of data points. The data storgae	*/
/* for these data points will be is stored locally in memory in 	*/
/* structures of type 'MISU_DV', which is defined in mi_usr.h and 	*/
/* contains a union of all extended data types.				*/

/* We will use the MIS_DV_ACCESS_INFO as a ST_VOID * that will point	*/
/* to the MISU_DV, which makes it easy to retrieve the data as required.	*/

/************************************************************************/
/*			misuChangeDv					*/
/************************************************************************/

ST_VOID misuChangeDv ()
  {
static MISU_DV *miuDv = NULL;
static ST_INT numToChange = 1;
static ST_INT totalChangeCount = 0;
MIS_DV *dv;
ST_INT i;

  if (misu_dv_list == NULL)
    return;

  for (i = 0; i < numToChange; ++i)
    {
    ++totalChangeCount;

    if (miuDv == NULL)
      miuDv = misu_dv_list;

    dv = MIS_DV_REF_TO_DV (miuDv->dvRef);
    printf ("\nChanging DV '%s'", dv->dv_name);

    switch (miuDv->mi_type)
      {
      case MI_TYPEID_REAL_EXTENDED:
        miuDv->data.re.Value += (ST_FLOAT) 0.01;
      break;

      case MI_TYPEID_DISCRETE_EXTENDED:
        miuDv->data.de.Value += 1;
      break;

      case MI_TYPEID_STATE_EXTENDED:
        if (totalChangeCount % 2)
          {
          MI_SET_DATA_STATE (miuDv->data.se.Flags,MI_DATA_STATE_ON);
 	  }
        else
          {
          MI_SET_DATA_STATE (miuDv->data.se.Flags,MI_DATA_STATE_OFF);
	  }
      break;
      }

    mis_dv_change (miuDv->dvRef);
    miuDv = (MISU_DV *) list_get_next (misu_dv_list, miuDv);
    }
  }

/************************************************************************/
/************************************************************************/
/* USER DEFINED DATA ACCESS FUNCTIONS 					*/
/************************************************************************/
/************************************************************************/
/*			u_mis_rd_dv 					*/
/************************************************************************/

/* This function is called when MI needs to get Data Value data, either	*/
/* for responding to a read indication or for sending an information	*/
/* report (Data Set Transfer Set).					*/

ST_VOID u_mis_rd_dv (MIS_RD_DV_CTRL *rd_dv_ctrl)
  {
MISU_DV *miuDv;
MIS_DV *dv;

MI_REAL_EXTENDED 	*reSrc;	/* Our data is stored in extended form	*/
MI_STATE_EXTENDED    	*seSrc;
MI_DISCRETE_EXTENDED 	*deSrc;

MI_REAL 		*rDest; /* Destination data pointers		*/
MI_STATE 	   	*sDest;
MI_DISCRETE 	   	*dDest;
MI_REAL_Q 	   	*rqDest;
MI_STATE_Q 	   	*sqDest;
MI_DISCRETE_Q 	   	*dqDest;
MI_REAL_Q_TIMETAG 	*rqtDest;
MI_STATE_Q_TIMETAG    	*sqtDest;
MI_DISCRETE_Q_TIMETAG 	*dqtDest;
MI_REAL_EXTENDED 	*reDest;
MI_STATE_EXTENDED    	*seDest;
MI_DISCRETE_EXTENDED 	*deDest;

ST_RET rc;

/* For our sample, the MIS_DV (Data Value control) has the 		*/
/* 'access_info' element set to point to the data structure containing	*/
/* all information about the point. We will use this to fill in the 	*/
/* destination data, depending on the types involved.			*/

  rc = SD_SUCCESS;
  dv = MIS_DV_REF_TO_DV (rd_dv_ctrl->mis_dv_ref);
  miuDv = (MISU_DV *)dv->access_info;

/* Make sure this one that we have mapped, and if not just let it go ..	*/
  if (miuDv == NULL)
    {
    mis_rd_dv_done (rd_dv_ctrl, SD_SUCCESS);	
    return;
    }
   
  switch (rd_dv_ctrl->mi_type)
    {
    case MI_TYPEID_REAL:
      reSrc = &miuDv->data.re;
      rDest = (float *)rd_dv_ctrl->data_buf;
      *rDest = reSrc->Value;
    break;
  
    case MI_TYPEID_STATE:
      seSrc = &miuDv->data.se;
      sDest = (unsigned char *)rd_dv_ctrl->data_buf;
      *sDest = seSrc->Flags;
    break;
  
    case MI_TYPEID_DISCRETE:
      deSrc = &miuDv->data.de;
      dDest = (long *)rd_dv_ctrl->data_buf;
      *dDest = deSrc->Value;
    break;
  
    case MI_TYPEID_REAL_Q:
      reSrc = &miuDv->data.re;
      rqDest = (MI_REAL_Q *)rd_dv_ctrl->data_buf;
      rqDest->Value = reSrc->Value;
      rqDest->Flags = reSrc->Flags;
    break;
  
    case MI_TYPEID_STATE_Q:
      seSrc = &miuDv->data.se;
      sqDest = (unsigned char *)rd_dv_ctrl->data_buf;
      *sqDest = seSrc->Flags;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      deSrc = &miuDv->data.de;
      dqDest = (MI_DISCRETE_Q *)rd_dv_ctrl->data_buf;
      dqDest->Value = deSrc->Value;
      dqDest->Flags = deSrc->Flags;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      reSrc = &miuDv->data.re;
      rqtDest = (MI_REAL_Q_TIMETAG *)rd_dv_ctrl->data_buf;
      rqtDest->Value = reSrc->Value;
      rqtDest->Flags = reSrc->Flags;
      rqtDest->TimeStamp = reSrc->TimeStamp;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      seSrc = &miuDv->data.se;
      sqtDest = (MI_STATE_Q_TIMETAG *)rd_dv_ctrl->data_buf;
      sqtDest->Flags = seSrc->Flags;
      sqtDest->TimeStamp = seSrc->TimeStamp;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      deSrc = &miuDv->data.de;
      dqtDest = (MI_DISCRETE_Q_TIMETAG *)rd_dv_ctrl->data_buf;
      dqtDest->Value = deSrc->Value;
      dqtDest->Flags = deSrc->Flags;
      dqtDest->TimeStamp = deSrc->TimeStamp;
    break;

  /* extended forms match our local storage, just copy the data	*/
    case MI_TYPEID_REAL_EXTENDED:
      reSrc = &miuDv->data.re;
      reDest = (MI_REAL_EXTENDED *)rd_dv_ctrl->data_buf;
      memcpy (reDest, reSrc, sizeof (MI_REAL_EXTENDED));
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      seSrc = &miuDv->data.se;
      seDest = (MI_STATE_EXTENDED *)rd_dv_ctrl->data_buf;
      memcpy (seDest, seSrc, sizeof (MI_STATE_EXTENDED));
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      deSrc = &miuDv->data.de;
      deDest = (MI_DISCRETE_EXTENDED *)rd_dv_ctrl->data_buf;
      memcpy (deDest, deSrc, sizeof (MI_DISCRETE_EXTENDED));
    break;
  
    default:
      printf ("\nError - Check Me Out!");
      rc = SD_FAILURE;
    break;
    }

/* When the data has been put in the destination, we let MI know. In	*/
/* our sample this happens immediately, but could be done some time 	*/
/* later as apprioriate.						*/
  mis_rd_dv_done (rd_dv_ctrl, rc);
  }


/************************************************************************/
/*			u_mis_wr_dv 					*/
/************************************************************************/

ST_VOID u_mis_wr_dv (MIS_WR_DV_CTRL *wr_dv_ctrl)
  {
MISU_DV *miuDv;
MIS_DV *dv;

MI_REAL_EXTENDED 	*reDest;	/* Data stored in extended form	*/
MI_STATE_EXTENDED    	*seDest;
MI_DISCRETE_EXTENDED 	*deDest;

MI_REAL 		*rSrc; 		/* Source data pointers		*/
MI_STATE 	   	*sSrc;
MI_DISCRETE 	   	*dSrc;
MI_REAL_Q 	   	*rqSrc;
MI_STATE_Q 	   	*sqSrc;
MI_DISCRETE_Q 	   	*dqSrc;
MI_REAL_Q_TIMETAG 	*rqtSrc;
MI_STATE_Q_TIMETAG    	*sqtSrc;
MI_DISCRETE_Q_TIMETAG 	*dqtSrc;
MI_REAL_EXTENDED 	*reSrc;
MI_STATE_EXTENDED    	*seSrc;
MI_DISCRETE_EXTENDED 	*deSrc;
ST_RET rc;

  rc = SD_SUCCESS;
  dv = MIS_DV_REF_TO_DV (wr_dv_ctrl->mis_dv_ref);
  miuDv = (MISU_DV *)dv->access_info;

/* Make sure this one that we have mapped, and if not just let it go ..	*/
  if (miuDv == NULL)
    {
    mis_wr_dv_done (wr_dv_ctrl, SD_SUCCESS);	
    return;
    }

  switch (wr_dv_ctrl->mi_type)
    {
    case MI_TYPEID_REAL:
      reDest = &miuDv->data.re;
      rSrc = (float *)wr_dv_ctrl->data_buf;
      reDest->Value = *rSrc;
    break;
  
    case MI_TYPEID_STATE:
      seDest = &miuDv->data.se;
      sSrc = (unsigned char *)wr_dv_ctrl->data_buf;
      seDest->Flags = *sSrc;
    break;
  
    case MI_TYPEID_DISCRETE:
      deDest = &miuDv->data.de;
      dSrc = (long *)wr_dv_ctrl->data_buf;
      deDest->Value = *dSrc;
    break;
  
    case MI_TYPEID_REAL_Q:
      reDest = &miuDv->data.re;
      rqSrc = (MI_REAL_Q *)wr_dv_ctrl->data_buf;
      reDest->Value = rqSrc->Value;
      reDest->Flags = rqSrc->Flags;
    break;
  
    case MI_TYPEID_STATE_Q:
      seDest = &miuDv->data.se;
      sqSrc = (unsigned char *)wr_dv_ctrl->data_buf;
      seDest->Flags = *sqSrc;
    break;
  
    case MI_TYPEID_DISCRETE_Q:
      deDest = &miuDv->data.de;
      dqSrc = (MI_DISCRETE_Q *)wr_dv_ctrl->data_buf;
      deDest->Value = dqSrc->Value;
      deDest->Flags = dqSrc->Flags;
    break;
  
    case MI_TYPEID_REAL_Q_TIMETAG:
      reDest = &miuDv->data.re;
      rqtSrc = (MI_REAL_Q_TIMETAG *)wr_dv_ctrl->data_buf;
      reDest->Value = rqtSrc->Value;
      reDest->Flags = rqtSrc->Flags;
      reDest->TimeStamp = rqtSrc->TimeStamp;
    break;
  
    case MI_TYPEID_STATE_Q_TIMETAG:
      seDest = &miuDv->data.se;
      sqtSrc = (MI_STATE_Q_TIMETAG *)wr_dv_ctrl->data_buf;
      seDest->Flags = sqtSrc->Flags;
      seDest->TimeStamp = sqtSrc->TimeStamp;
    break;
  
    case MI_TYPEID_DISCRETE_Q_TIMETAG:
      deDest = &miuDv->data.de;
      dqtSrc = (MI_DISCRETE_Q_TIMETAG *)wr_dv_ctrl->data_buf;
      deDest->Value = dqtSrc->Value;
      deDest->Flags = dqtSrc->Flags;
      deDest->TimeStamp = dqtSrc->TimeStamp;
    break;

  /* extended forms match our local storage, just copy the data	*/
    case MI_TYPEID_REAL_EXTENDED:
      reDest = &miuDv->data.re;
      reSrc = (MI_REAL_EXTENDED *)wr_dv_ctrl->data_buf;
      memcpy (reDest, reSrc, sizeof (MI_REAL_EXTENDED));
    break;
  
    case MI_TYPEID_STATE_EXTENDED:
      seDest = (MI_STATE_EXTENDED *)&miuDv->data.se;
      seSrc = (MI_STATE_EXTENDED *)wr_dv_ctrl->data_buf;
      memcpy (seDest, seSrc, sizeof (MI_STATE_EXTENDED));
    break;
  
    case MI_TYPEID_DISCRETE_EXTENDED:
      deDest = &miuDv->data.de;
      deSrc = (MI_DISCRETE_EXTENDED *)wr_dv_ctrl->data_buf;
      memcpy (deDest, deSrc, sizeof (MI_DISCRETE_EXTENDED));
    break;
  
    default:
      printf ("\nError - Check Me Out!");
      rc = SD_FAILURE;
    break;
    }
  mis_wr_dv_done (wr_dv_ctrl, rc);
  }

/************************************************************************/
/************************************************************************/
/* MIS COMMUNICATIONS ACTIVITY INDICATION				*/
/* These functions are called by MI to tell the application about 	*/
/* server communications activity. Typically, no action is required ...	*/
/************************************************************************/
/************************************************************************/
/* DATA SET ACTIVITY							*/
/************************************************************************/
/************************************************************************/
/*			u_mis_create_ds_ind 				*/
/************************************************************************/

/* This function is called by MI when a client is creating a data set.	*/
/* No activity is required on our part ...				*/

ST_RET u_mis_create_ds_ind (MIS_VCC *vcc, DEFVLIST_REQ_INFO *req_info)
  {
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_mis_delete_ds_ind 				*/
/************************************************************************/

/* This function is called by MI when a client is deleting a data set.	*/
/* No activity is required on our part ...				*/

ST_RET u_mis_delete_ds_ind (MIS_VCC *vcc, DELVLIST_REQ_INFO *req_info)
  {
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* TRANSFER SET ACTIVITY						*/
/************************************************************************/
/*			u_mis_dsts_activity 				*/
/************************************************************************/

/* This function is called by MI when there is activity associated with	*/
/* a Data Set Transfer Set. No activity is required on our part ...	*/

ST_RET u_mis_dsts_activity (MIS_VCC *vcc, MIS_DSTS *misDsts, 
			    ST_INT activity_type, 
			    MI_TS_TIMESTAMP ts_timestamp)
  {
ST_CHAR *tsName;

  tsName = misDsts->ts_name;
  switch (activity_type)
    {
    case MIS_DSTS_START_TRANSFER :
      printf ("\nStart Transfer for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_STOP_TRANSFER :
      printf ("\nStop Transfer for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_BEGIN_REPORT :
      printf ("\nBegin Report for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_END_REPORT :
      printf ("\nEnd Report for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_ACK_RCVD : 
      printf ("\nACK received for DSTS '%s'", tsName);
    break;

    case MIS_DSTS_NACK_RCVD :
      printf ("\nNACK received for DSTS '%s'", tsName);
    break;

    default:
    break;
    }
  return (SD_SUCCESS);
  }


/************************************************************************/
/* misuSendMessage:  send an IMTransfer Report				*/
/************************************************************************/
static ST_INT32 messageId = 0;

ST_VOID misuSendMessage (ST_VOID)
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;
MI_INFO_BUF_HEADER buf_header;
ST_UCHAR info_buf[128];
ST_INT buf_len = 128;
ST_INT msg_scope = VCC_SPEC;	 /* or ICC_SPEC */
ST_RET ret;
MI_CONN **connArray;
ST_INT arrayIndex;

  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    if (mi_remote->mis_vcc == NULL)	/* if we are not a server */
      {
      mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
      continue;
      }

    if (mi_remote->mi_assoc_list != NULL)
      { 
/*      connArray = (_mi_conn **)chk_calloc (list_get_sizeof (mi_remote->mi_assoc_list), 
modified 2006.11.24 zy*/
      connArray = (struct _mi_conn **)chk_calloc (list_get_sizeof (mi_remote->mi_assoc_list), 
			      sizeof (MI_CONN *));
      }

    arrayIndex = 0;
    mi_assoc = mi_remote->mi_assoc_list;
    while (mi_assoc)
      {
      if (mi_assoc->state == MI_ASSOC_STATE_ACTIVE)
        connArray[arrayIndex++] = mi_assoc->mi_conn;
      mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
      }

    sprintf ((ST_CHAR *) info_buf, "This is an Information Message #%d", messageId);
    buf_header.InfoReference = 3693;
    buf_header.LocalReference = 5280;
    buf_header.MessageId = messageId++;
    buf_header.Size = strlen ((const char *)info_buf) + 1;
  
    ret = mis_send_IMTransfer (arrayIndex, connArray,
                               &buf_header, buf_len, info_buf, msg_scope); 
    if (ret != SD_SUCCESS)
      printf ("\nFailure Sending Message: '%s'", info_buf);
    else
      printf ("\nSent message: '%s'", info_buf);
  
    chk_free (connArray);
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
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
/*			u_mis_stop_IM_Transfer				*/
/* notification that the transfer set has been disabled by the client	*/
/************************************************************************/

ST_VOID u_mis_stop_IM_Transfer (MI_CONN *mi_conn)
  {
  printf ("\n IM_Transfer_Set disabled for remote AR %s", mi_conn->remote_ar);
  }

/************************************************************************/
/************************************************************************/
/*			u_mis_device_select				*/
/* notification of device select request				*/
/************************************************************************/

ST_RET u_mis_device_select (MIS_DEVICE_IND_CTRL *dev_ind_ctrl)
  {
char * devName;

  devName = (char *) dev_ind_ctrl->mis_device->device_name;
  printf ("\n Device Select for %s, send default response.", devName);
  return (MIS_SEND_DEFAULT_RESP);
  }

/************************************************************************/
/*			u_mis_device_operate				*/
/* notification of device operate request				*/
/************************************************************************/

ST_RET u_mis_device_operate (MIS_DEVICE_IND_CTRL *dev_ind_ctrl)
  {   
char * devName;
MI_CONTROL_COMMAND  *pCommand;
MI_CONTROL_SETPOINT_REAL *pReal;
MI_CONTROL_SETPOINT_DISCRETE *pDiscrete;

  devName = (char *) dev_ind_ctrl->mis_device->device_name;
  printf ("\n Device Operate for %s, send default response.", devName);

  switch (dev_ind_ctrl->device_type)
    {
    case MI_TYPEID_DEV_COMMAND:
      pCommand = (MI_CONTROL_COMMAND *) dev_ind_ctrl->data_buf;
      printf ("\n OperateValue = %d", *pCommand);
    break;

    case MI_TYPEID_DEV_DISCRETE:
      pDiscrete = (MI_CONTROL_SETPOINT_DISCRETE *) dev_ind_ctrl->data_buf;
      printf ("\n OperateValue = %d", *pDiscrete);
    break;

    case MI_TYPEID_DEV_REAL:
      pReal = (MI_CONTROL_SETPOINT_REAL *) dev_ind_ctrl->data_buf;
      printf ("\n OperateValue = %d", *pReal);
    break;

    default:
      printf ("\n Unknown OperateValue Type.");
    break;
    }

  return (MIS_SEND_DEFAULT_RESP);
  }

/************************************************************************/
/*		      u_mis_device_settag				*/
/* notification of a device set tag request				*/
/************************************************************************/

ST_RET u_mis_device_settag (MIS_DEVICE_IND_CTRL *dev_ind_ctrl)
  { 
char * devName;
MI_TAG_VALUE *pTagValue;

  devName = (char *) dev_ind_ctrl->mis_device->device_name;
  pTagValue = (MI_TAG_VALUE *) dev_ind_ctrl->data_buf;

  printf ("\n Device Set Tag for %s, send default response.", devName);
  printf ("\n TagFlags = 0x%x, Reason = %s", pTagValue->TagFlags, pTagValue->Reason);

  return (MIS_SEND_DEFAULT_RESP);
  }

/************************************************************************/
/*		      u_mis_device_gettag				*/
/* notification of a device get tag request				*/
/************************************************************************/

ST_RET u_mis_device_gettag (MIS_DEVICE_IND_CTRL *dev_ind_ctrl)
  {
char * devName;

  devName = (char *) dev_ind_ctrl->mis_device->device_name;
  printf ("\n Device Get Tag for %s, send default response.", devName);
  return (MIS_SEND_DEFAULT_RESP);
  }


