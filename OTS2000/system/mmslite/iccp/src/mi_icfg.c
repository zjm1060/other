/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          2001-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mi_icfg.c	                                        */
/* PRODUCT(S)  : MI							*/
/*                                                                      */
/*                                                                      */
/* MODULE DESCRIPTION :    		       				*/
/*                                                                      */
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :                            */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev          Comments                                */
/* --------  ---  ------  --------------------------------------------  */
/* 07/02/02  MDE     19   Fixed MIC required supported services		*/
/* 06/20/02  RKR     18   Copyright year update                         */
/* 05/22/02  MDE     17   Now enforce service role			*/
/* 05/20/02  MDE     16   Fixed leaks on error				*/
/* 04/30/02  NAV     15   mi_icfg_free_remote_cc - free the association */
/* 04/26/02  MDE     14   Added logs for DSConditions Requested		*/
/* 04/25/02  ASK     13	  Added logs for validation settings		*/
/* 04/05/02  MDE     12   Use new MI_LOG_CFG macros			*/
/* 03/21/02  MDE     11   Now mirror MILOG_CFG to ICFG_LOG_FLOW  	*/
/* 03/21/02  MDE     10	  Don't call mi_add_ae_to_remote (obsolete)	*/
/* 02/26/02  MDE     09   Added mi_icfg_log_status			*/
/* 01/25/02  MDE     08   Moved	mi_icfg_db_load to mi_icfg_db.c		*/
/* 01/23/02  NAV     07   Add mi_icfg_db_load				*/
/* 01/09/02  ASK     07   Added special var conditions for Server DS	*/
/* 01/04/02  ASK     06   Fixed mi_icfg_load, always returned SD_SUCCESS*/
/* 11/28/01  NAV     05   Conditionally free remote_cc			*/
/* 11/28/01  NAV     04   Correction to mis_create_vcc # iccDS		*/
/* 11/21/01  NAV     03   Minor log correction in u_icfg_ServerDevice	*/
/* 11/15/01  MDE     02	  Fixed local device SBO handling 		*/
/* 10/19/01  MDE     01   Created from mit_xmlc.c        		*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "gen_list.h"
#include "mem_chk.h"
#include "mi.h"
#include "mi_icfg.h"
#include "mi_log.h"

/************************************************************************/
/************************************************************************/

static ST_CHAR *thisFileName = __FILE__;

/************************************************************************/
/************************************************************************/
/*			mi_icfg_load 					*/
/************************************************************************/

ST_RET mi_icfg_load (ST_CHAR *xmlFileName, ST_INT mode, 
		     ST_CHAR *remoteName, ST_VOID *usr)
  {
MI_ICFG_CTRL lc;
ST_RET rc;

/* We mirror the MI log masks into the ICFG log mask */
  if (mi_debug_sel & MILOG_ERR)
    icfg_debug_sel |= ICFG_LOG_ERR;

  if (mi_debug_sel & MILOG_NERR)
    icfg_debug_sel |= ICFG_LOG_NERR;

  if (mi_debug_sel & MILOG_CFG)
    icfg_debug_sel |= ICFG_LOG_FLOW;

  memset (&lc, 0, sizeof (lc));
  lc.usr = usr;
  lc.mode = mode;
  lc.remote_name = remoteName;
  rc = icfgXml (&lc, xmlFileName);

  if (rc == SD_SUCCESS && lc.mode == MI_ICFG_MODE_RELOAD)
    {
    if (lc.target_remote_found != SD_TRUE)
      {
      MI_LOG_NERR1 ("Reload error, remote name '%s' not found", remoteName);
      rc = MI_ICFG_ERR_REMOTE_NOT_FOUND;      
      }
    }

  if (rc != SD_SUCCESS)
    {
    if (lc.remote_cc != NULL)
      chk_free (lc.remote_cc);

    if (lc.local_cc != NULL)
      chk_free (lc.local_cc);
    }

  return (rc);
  }

/************************************************************************/
/************************************************************************/
/* Icfg Callback Functions                                              */
/************************************************************************/
/************************************************************************/
/* 		u_icfg_StartLocalControlCenter 				*/
/************************************************************************/

ST_RET  u_icfg_StartLocalControlCenter (ST_VOID *usr, ICFG_LOCAL_CC *icfgLocalCc)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
ST_RET        ret;

  MI_LOG_CFG0 ("u_icfg_StartLocalCtrlCenter");
  MI_LOG_CCFG1 ("  LOCAL CONTROL CENTER Name:    %s", icfgLocalCc->name);
  MI_LOG_CCFG1 ("  Maximum DSTS:                 %d", icfgLocalCc->maxDsTs);
  MI_LOG_CCFG1 ("  Maximum DS:                   %d", icfgLocalCc->maxDs);
  MI_LOG_CCFG1 ("  Maximum MMS PDU Size:         %d", icfgLocalCc->maxMmsPduSize);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->mode == MI_ICFG_MODE_INITIAL)
    {
    ret = mis_create_dsts_names (icfgLocalCc->maxDsTs);
    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR1 ("Error creating DS names (%d)", ret);
      return (ret);
      }
    }
  else
    ret = SD_SUCCESS;

  mi_icfg_ctrl->local_cc = icfgLocalCc;	/* Free it later ... */
  ret = u_miIcfgProcLocalControlCenter (mi_icfg_ctrl->usr, icfgLocalCc);  
  return (ret);
  }

/************************************************************************/
/* 		u_icfg_EndLocalControlCenter 				*/
/************************************************************************/

ST_RET  u_icfg_EndLocalControlCenter (ST_VOID *usr)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;

  MI_LOG_CFG0 ("u_icfg_EndLocalCtrlCenter");

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  icfgFree (mi_icfg_ctrl->local_cc);
  mi_icfg_ctrl->local_cc = NULL;
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* 		u_icfg_StartLocalObjects 				*/
/************************************************************************/

ST_RET  u_icfg_StartLocalObjects (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_StartLocalObjects");
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_EndLocalObjects 				*/
/************************************************************************/

ST_RET  u_icfg_EndLocalObjects (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndLocalObjects");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* 		u_icfg_StartLocalDataValues 				*/
/************************************************************************/

ST_RET  u_icfg_StartLocalDataValues (ST_VOID *usr, ST_INT numDv)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
ST_RET        ret;

  MI_LOG_CFG1 ("u_icfg_StartLocalDataValues total: %d", numDv);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->mode == MI_ICFG_MODE_INITIAL)
    ret = mis_create_dv_tables (numDv);
  else
    ret = mis_resize_dv_tables (numDv);

  if (ret != SD_SUCCESS)
    {
    MI_LOG_ERR1 ("Error creating DV tables (%d)", ret);
    return (ret);
    }
  return (ret);
  }

/************************************************************************/
/*			u_icfg_LocalDataValue 				*/
/************************************************************************/

ST_RET  u_icfg_LocalDataValue (ST_VOID *usr, ICFG_LOCAL_DV *localDv)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIS_DV_REF dv_ref;
ST_CHAR *dv_name;
ST_RET   ret;
ST_BOOLEAN newDv;

  MI_LOG_CFG0  ("u_icfg_LocalDataValue");
  MI_LOG_CCFG1  ("  Local Data Value name:        %s", localDv->name);
  MI_LOG_CCFG1  ("  Local Data Value type:        %s", icfgDvTypeValEnum[localDv->dvType]);
  MI_LOG_CCFG1  ("  Normal Source:                %s", icfgSourceValEnum[localDv->normalSource]);
  MI_LOG_CCFG1  ("  Map Info:                     %s", localDv->mapInfo);
  
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (!strncmp(icfgDvTypeValEnum[localDv->dvType],"Discrete", 8))		   
    {				      
    MI_LOG_CCFG1("  Val Min:                      %d", localDv->vals.discreteVals.valMin);
    MI_LOG_CCFG1("  Val Max:                      %d", localDv->vals.discreteVals.valMax);
    }
  if (!strncmp(icfgDvTypeValEnum[localDv->dvType],"Real", 4))		   
    {
    MI_LOG_CCFG1("  Val Min:                      %.2f", localDv->vals.realVals.valMin);
    MI_LOG_CCFG1("  Val Max:                      %.2f", localDv->vals.realVals.valMax);
    }

  ret = mis_find_dv (localDv->name, &dv_ref);
  if (ret == SD_FAILURE)
    {					/* OK, let's just add it ... */
    dv_name = chk_strdup (localDv->name);  
    ret = mis_add_dv_ex (dv_name, NULL, &dv_ref);
    newDv = SD_TRUE;
    }
  else
    newDv = SD_FALSE;

  if (ret == SD_SUCCESS)
    ret = u_miIcfgProcLocalDv (mi_icfg_ctrl->usr, dv_ref, localDv, newDv);  
  else
    {
    MI_LOG_ERR1 ("Error creating adding DV '%s'", localDv->name);
    }
  return (ret);
  }

/************************************************************************/
/* 		u_icfg_EndLocalDataValues 				*/
/************************************************************************/

ST_RET  u_icfg_EndLocalDataValues (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndLocalDataValues");
  mis_sort_dv_tables ();      /* Need to sort them when all are added */
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* 		u_icfg_StartLocalDevices 				*/
/************************************************************************/

ST_RET  u_icfg_StartLocalDevices (ST_VOID *usr, ST_INT numDev)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
ST_RET ret;

  MI_LOG_CFG1 ("u_icfg_StartLocalDevices total: %d", numDev);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->mode == MI_ICFG_MODE_INITIAL)
    ret = mis_create_device_tables (numDev);
  else
    ret = mis_resize_device_tables (numDev);

  if (ret != SD_SUCCESS)
    MI_LOG_ERR1 ("Error creating DEV tables (%d)", ret);
				  
  return (ret);
  }

/************************************************************************/
/*			u_icfg_LocalDevice 				*/
/************************************************************************/

ST_RET  u_icfg_LocalDevice (ST_VOID *usr, ICFG_LOCAL_DEV *localDev)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIS_DEVICE_REF dev_ref;
ST_CHAR    *dev_name;
ST_RET      ret;
ST_BOOLEAN newDev;

  MI_LOG_CFG0 ("u_icfg_LocalDevice");
  MI_LOG_CCFG1 ("  Local Device name:            %s", localDev->name);
  MI_LOG_CCFG1 ("  Local Data Values Type:       %s", icfgDvTypeValEnum[localDev->devType]);
  MI_LOG_CCFG1 ("  Map Info:                     %s", localDev->mapInfo);
  MI_LOG_CCFG1 ("  Sbo Device:                   %c", localDev->sbo ? 'Y':'N');
  if (localDev->sbo == 1)
    {
    MI_LOG_CCFG1  ("  Sbo TimeOut:                  %d",   localDev->selTime);
    MI_LOG_CCFG1  ("  CheckBack Id:                 %d",   localDev->checkBackId);
    }
  MI_LOG_CCFG1  ("  Tag Enable:                   %c", localDev->tagEnable ? 'Y':'N');

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  ret = mis_find_device (localDev->name, &dev_ref);
  if (ret == SD_FAILURE)
    {					/* OK, let's just add it ... */
    dev_name = chk_strdup (localDev->name);  
    ret = mis_add_device_ex (dev_name, localDev->checkBackId, 
  		      	     localDev->sbo, localDev->tagEnable, 
  		      	     NULL, &dev_ref);
    newDev = SD_TRUE;
    }
  else
    newDev = SD_FALSE;

  if (ret == SD_SUCCESS)
    ret = u_miIcfgProcLocalDev (mi_icfg_ctrl->usr, dev_ref, localDev, newDev);  
  else
    {
    MI_LOG_ERR1 ("Error adding device '%s'", dev_name);
    }
  return (ret);
  }

/************************************************************************/
/*			u_icfg_EndLocalDevices 				*/
/************************************************************************/

ST_RET  u_icfg_EndLocalDevices (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndLocalDevices");
  mis_sort_device_table();
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* 		u_icfg_StartLocalInfoMsgs 				*/
/************************************************************************/

ST_RET  u_icfg_StartLocalInfoMsgs (ST_VOID *usr, ST_INT numDv)	 
  {
  MI_LOG_CFG1 ("u_icfg_StartLocalInfoMsgs total: %d", numDv);
  return (SD_SUCCESS);
  }
 
/************************************************************************/
/*			u_icfg_LocalInfoMsg 				*/
/************************************************************************/

ST_RET  u_icfg_LocalInfoMsg (ST_VOID *usr, ICFG_LOCAL_INFOMSG *localIm)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
ST_RET ret;

  MI_LOG_CFG0 ("u_icfg_LocalInfoMsg");
  MI_LOG_CCFG1 ("  LOCAL Info Msg:               %d", localIm->infoRef);
  MI_LOG_CCFG1 ("  Map Info:                     %s", localIm->mapInfo);
  MI_LOG_CCFG1 ("  Max Size:                     %d", localIm->maxSize);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  ret = u_miIcfgProcLocalIm (mi_icfg_ctrl->usr, localIm); /* User is to free via 'icfgFree'*/
  return (ret);
  }

/************************************************************************/
/* 		u_icfg_EndLocalInfoMsgs 				*/
/************************************************************************/

ST_RET  u_icfg_EndLocalInfoMsgs (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndLocalInfoMsgs");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*  		u_icfg_StartRemoteControlCenters 				*/
/************************************************************************/

ST_RET  u_icfg_StartRemoteControlCenters (ST_VOID *usr, ST_INT numCC)
  {
  MI_LOG_CFG1 ("u_icfg_StartRemCCs total: %d", numCC);
  return (SD_SUCCESS);
  }

/************************************************************************/
/* 		u_icfg_EndRemoteControlCenters 				*/
/************************************************************************/

ST_RET  u_icfg_EndRemoteControlCenters (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndRemCC");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* 		u_icfg_StartRemoteControlCenter 				*/
/************************************************************************/

ST_RET  u_icfg_StartRemoteControlCenter (ST_VOID *usr, ICFG_REMOTE_CC *remote_cc)
  {
MI_ICFG_CTRL   *mi_icfg_ctrl;
MI_REMOTE      *mi_remote;
ST_RET ret;

  MI_LOG_CFG0 ("u_icfg_StartRemCC");
  MI_LOG_CCFG1 ("  REMOTE CONTROL CENTER Name:        %s", remote_cc->name);
  MI_LOG_CCFG1 ("  BiLatTblId:                        %s", remote_cc->blt.id);
  MI_LOG_CCFG1 ("  Local Dom:                         %s", remote_cc->blt.localDom);
  MI_LOG_CCFG1 ("  Remote Dom:                        %s", remote_cc->blt.remoteDom);
  MI_LOG_CCFG1 ("  Shortest Interval:                 %d", remote_cc->blt.shortestInterval);
  MI_LOG_CCFG1 ("  Blocks:                            0x%x", remote_cc->blt.blocks);
  MI_LOG_CCFG1 ("  Remote Data Value Validation:      %s", icfgRemDvValidationEnum[remote_cc->blt.remDvValidation]);
  MI_LOG_CCFG1 ("  Remote Data Value Get Config:      %s", icfgRemDvGetConfigEnum[remote_cc->blt.remDvGetConfig]);
  MI_LOG_CCFG1 ("  Remote Data Value Exclude Invalid: %c", remote_cc->blt.remDvExcludeInvalidFromDs ? 'Y':'N');

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;

  mi_icfg_ctrl->target_remote = SD_TRUE;
  if (mi_icfg_ctrl->mode == MI_ICFG_MODE_RELOAD)
    {
    if (mi_icfg_ctrl->remote_name == NULL || 
        strcmp (mi_icfg_ctrl->remote_name, remote_cc->name))
      {
      mi_icfg_ctrl->target_remote = SD_FALSE;
      MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
      icfgFree (remote_cc);
      return (SD_SUCCESS);
      }
    mi_icfg_ctrl->target_remote_found = SD_TRUE;
    }

  mi_remote = mi_create_remote();
  if (mi_remote == NULL)
    {
    MI_LOG_ERR0 ("Error adding remote");
    ret = SD_FAILURE;
    }
  else
    {
    mi_icfg_ctrl->serviceRole = 0;
    mi_remote->name = chk_strdup (remote_cc->name);  
    mi_icfg_ctrl->mi_remote = mi_remote;
    mi_icfg_ctrl->remote_cc = remote_cc;
    ret = u_miIcfgProcRemoteControlCenter (mi_icfg_ctrl->usr, mi_remote, remote_cc);  
    }
  return (ret);
  }

/************************************************************************/
/* 		u_icfg_EndRemoteControlCenter 				*/
/************************************************************************/

ST_RET  u_icfg_EndRemoteControlCenter (ST_VOID *usr)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;

  MI_LOG_CFG0 ("u_icfg_EndRemCC");

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote)
    {
    icfgFree (mi_icfg_ctrl->remote_cc);
    mi_icfg_ctrl->remote_cc = NULL;
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* 		u_icfg_StartAssociations 				*/
/************************************************************************/

ST_RET  u_icfg_StartAssociations (ST_VOID *usr, ST_INT numLinks)
  {
  MI_LOG_CFG1 ("u_icfg_StartLinks total: %d", numLinks);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_Association 				*/
/************************************************************************/

ST_RET  u_icfg_Association (ST_VOID *usr, ICFG_ASSOCIATION *assoc)
  {
MI_ICFG_CTRL  *mi_icfg_ctrl;
MI_REMOTE     *mi_remote;
MI_ASSOC_CTRL *mi_assoc;
ST_CHAR       *local_ar;
ST_INT 	       j;
ST_RET	       ret;
	
  MI_LOG_CFG0 ("u_icfg_Link");
  MI_LOG_CCFG1 ("  Name:                         %s",   assoc->name);
  MI_LOG_CCFG1 ("  Local Ar:                     %s",   assoc->localAr);
  for (j=0; j < assoc->totalRemoteArNames; j++)
    {
    MI_LOG_CCFG2  ("  Remote Ar[%d]:                 %s", j, assoc->remoteAr[j]);
    }
  MI_LOG_CCFG1 ("  Connect Role:                 0x%x",   assoc->connectRole);
  MI_LOG_CCFG1 ("  Service Role:                 0x%x",   assoc->serviceRole);
  MI_LOG_CCFG1 ("  Retry Time:                   %d",   assoc->retryTime);
  MI_LOG_CCFG1 ("  Max Mms Pdu Size:             %d",   assoc->maxMmsPduSize);
  MI_LOG_CCFG1 ("  Max Req Pend:                 %d",   assoc->maxReqPend);
  MI_LOG_CCFG1 ("  Max Ind Pend:                 %d",   assoc->maxIndPend);
  MI_LOG_CCFG1 ("  Max Nest:                     %d",   assoc->maxNest);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (assoc);
    return (SD_SUCCESS);
    }

  mi_remote = mi_icfg_ctrl->mi_remote;
  mi_assoc = mi_create_assoc (mi_remote);
  mi_assoc->name = chk_strdup (assoc->name);  

  local_ar = chk_strdup (assoc->localAr);  
  mi_assoc->local_ar = local_ar; 

  for (j = 0; j < assoc->totalRemoteArNames; ++j)
    {
    ret = mi_add_ae_to_assoc (mi_assoc, assoc->remoteAr[j]);
    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR0 ("Error adding AE to association");
      break;
      }
    }	  

/* Combine the connect and service roles into the control flags		*/
  mi_assoc->ctrl_flags |= assoc->connectRole;
  mi_assoc->ctrl_flags |= assoc->serviceRole;

  mi_assoc->connect_backoff_time = assoc->retryTime;
  mi_assoc->max_pdu_size = assoc->maxMmsPduSize;

  mi_assoc->def_locl_init_info.mms_p_context_pres = SD_TRUE;
  mi_assoc->def_locl_init_info.max_segsize_pres = SD_TRUE;
  mi_assoc->def_locl_init_info.max_segsize = assoc->maxMmsPduSize;
  mi_assoc->def_locl_init_info.maxreq_calling = (ST_INT16) assoc->maxReqPend;;
  mi_assoc->def_locl_init_info.maxreq_called = (ST_INT16) assoc->maxIndPend;
  mi_assoc->def_locl_init_info.max_nest_pres = SD_TRUE;
  mi_assoc->def_locl_init_info.max_nest = (ST_INT8) assoc->maxNest;
  mi_assoc->def_locl_init_info.mms_detail_pres = SD_TRUE;
  mi_assoc->def_locl_init_info.version = 1;
  mi_assoc->def_locl_init_info.num_cs_init = 0;
  mi_assoc->def_locl_init_info.core_position = 0;
  mi_assoc->def_locl_init_info.param_supp[0] = m_param[0];
  mi_assoc->def_locl_init_info.param_supp[1] = m_param[1];
  memcpy (mi_assoc->def_locl_init_info.serv_supp, m_service_resp,11);

/* Add the service role to the remote */
  mi_icfg_ctrl->serviceRole |= assoc->serviceRole;

  ret = u_miIcfgProcAssociation (mi_icfg_ctrl->usr, mi_assoc, assoc);  
  return (ret);
  }

/************************************************************************/
/*			u_icfg_EndAssociations 				*/
/************************************************************************/

ST_RET  u_icfg_EndAssociations (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndLinks total");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartServerObjects 				*/
/************************************************************************/

ST_RET  u_icfg_StartServerObjects (ST_VOID *usr, ICFG_SERVER_OBJ_COUNTS *serverObjCounts)
  {
MI_ICFG_CTRL   *mi_icfg_ctrl;
MI_REMOTE      *mi_remote;
MIS_VCC        *mis_vcc;
ICFG_REMOTE_CC *remote_cc;
ICFG_LOCAL_CC  *local_cc;
ST_RET          ret;
ST_CHAR        *icc_name;

  MI_LOG_CFG0 ("u_icfg_StartServerObjs");
  MI_LOG_CCFG1 ("  NumVccDv:         %d",   serverObjCounts->numVccDv);	     
  MI_LOG_CCFG1 ("  NumVccDs:         %d",   serverObjCounts->numVccDs);	     
  MI_LOG_CCFG1 ("  NumIccDv:         %d",   serverObjCounts->numIccDv);	     
  MI_LOG_CCFG1 ("  NumIccDs:         %d",   serverObjCounts->numIccDs);	     
  MI_LOG_CCFG1 ("  Num VCC Dev:      %d",   serverObjCounts->numVccDev);	     
  MI_LOG_CCFG1 ("  Num ICC Dev:      %d",   serverObjCounts->numIccDev);	     
  MI_LOG_CCFG1 ("  Num VCC InfoMsgs: %d",   serverObjCounts->numVccInfoMsg);     
  MI_LOG_CCFG1 ("  Num ICC InfoMsgs: %d",   serverObjCounts->numIccInfoMsg);     
  											     
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (serverObjCounts);
    return (SD_SUCCESS);
    }

  mi_remote = mi_icfg_ctrl->mi_remote;
  remote_cc = mi_icfg_ctrl->remote_cc;
  local_cc = mi_icfg_ctrl->local_cc;

/* Make sure it is OK to have server objects */
  if ((mi_icfg_ctrl->serviceRole & ICFG_ROLE_SERVER) == 0)
    {				   
    /* No server objects allowed */
    if (serverObjCounts->numVccDv != 0 ||	     
        serverObjCounts->numVccDs != 0 ||
        serverObjCounts->numIccDv != 0 ||
        serverObjCounts->numIccDs != 0 ||
        serverObjCounts->numVccDev != 0 ||
        serverObjCounts->numIccDev != 0 ||
        serverObjCounts->numVccInfoMsg != 0 ||
        serverObjCounts->numIccInfoMsg)
      {
      MI_LOG_ERR0 ("Configured Service Role does not allow server objects");
      return (SD_FAILURE);
      }
    }

  icc_name = chk_strdup (remote_cc->blt.localDom);
  mis_vcc = mis_create_vcc (mi_remote, remote_cc->blt.id, 
		        icc_name,
  		        serverObjCounts->numVccDv,	 	
		        serverObjCounts->numVccDs,	 		
  		        serverObjCounts->numIccDv,	 		
		        local_cc->maxDs,
  		        local_cc->maxDsTs,
		        remote_cc->blt.shortestInterval);			

  if (mis_vcc == NULL)
    {
    MI_LOG_ERR0 ("Error creating MIS_VCC");
    return (SD_FAILURE);
    }
  mi_icfg_ctrl->mis_vcc = mis_vcc;
  
  mis_vcc->misv.Supported_Features[0] = 
		(ST_UCHAR) ((remote_cc->blt.blocks >> 24) & 0x000000FF);

  mis_vcc->misv.Supported_Features[1] = 
		(ST_UCHAR) ((remote_cc->blt.blocks >> 16) & 0x000000FF);

  if (serverObjCounts->numVccDev != 0 || serverObjCounts->numIccDev != 0)
    {
    ret = mis_enable_device_ctrl (mis_vcc, serverObjCounts->numVccDev, 
				serverObjCounts->numIccDev);
    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR0 ("Error enabling device control");
      }
    }
  else
    ret = SD_SUCCESS;

  icfgFree (serverObjCounts);
  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndServerObjects 				*/
/************************************************************************/

ST_RET  u_icfg_EndServerObjects (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndServerObjs");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartServerDataValues 				*/
/************************************************************************/

ST_RET  u_icfg_StartServerDataValues (ST_VOID *usr, ST_INT numDv)
  {
  MI_LOG_CFG1 ("u_icfg_StartServerDataValues total: %d", numDv);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ServerDataValue 				*/
/************************************************************************/

ST_RET  u_icfg_ServerDataValue (ST_VOID *usr, ICFG_SERVER_DV *serverDv)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIS_VCC      *mis_vcc;
MIS_DV_REF    dv_ref;
ST_BOOLEAN    write_enable;
ST_RET	      ret;

  MI_LOG_CFG0 ("u_icfg_ServerDataValue");
  MI_LOG_CCFG1 ("  Server Data Value name:       %s", serverDv->name);
  MI_LOG_CCFG1 ("  Scope:                        %s", icfgScopeValEnum[serverDv->scope]);
  MI_LOG_CCFG1 ("  Server Data Value Type:       %s", icfgDvTypeValEnum[serverDv->dvType]);
  MI_LOG_CCFG1 ("  Read Only:                    %c", serverDv->readOnly ? 'Y':'N');
  MI_LOG_CCFG1 ("  Map Info:                     %s", serverDv->mapInfo);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (serverDv);
    return (SD_SUCCESS);
    }

  mis_vcc = mi_icfg_ctrl->mis_vcc;

  write_enable = serverDv->readOnly ? SD_FALSE : SD_TRUE;
  ret = mis_add_dv_to_vcc_ex (mis_vcc, serverDv->scope,
                       serverDv->name, 
                       serverDv->dvType, write_enable, &dv_ref);

  if (ret != SD_SUCCESS)
    {
    MI_LOG_ERR1 ("Error adding VCC DV '%s'", serverDv->name);
    }
  else
    ret = u_miIcfgProcServerDv (mi_icfg_ctrl->usr, mis_vcc, dv_ref, serverDv);  

  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndServerDataValues 				*/
/************************************************************************/

ST_RET  u_icfg_EndServerDataValues (ST_VOID *usr)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIS_VCC      *mis_vcc;

  MI_LOG_CFG0 ("u_icfg_EndServerDataValues");
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    return (SD_SUCCESS);
    }

  mis_vcc = mi_icfg_ctrl->mis_vcc;
  mis_sort_vcc_dv (mis_vcc);
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartServerDevices 				*/
/************************************************************************/

ST_RET  u_icfg_StartServerDevices (ST_VOID *usr, ST_INT numDev)
  {
  MI_LOG_CFG1 ("u_icfg_StartServerDevices total: %d", numDev);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ServerDevice 				*/
/************************************************************************/

ST_RET  u_icfg_ServerDevice (ST_VOID *usr, ICFG_SERVER_DEV *serverDev)
  {
MI_ICFG_CTRL   *mi_icfg_ctrl;
MIS_VCC        *mis_vcc;
MIS_DEVICE     *mis_device;
MIS_DEVICE_REF  dev_ref;
ST_RET          ret;

  MI_LOG_CFG0 ("u_icfg_ServerDevice");
  MI_LOG_CCFG1 ("  Server Device name:           %s", serverDev->name);
  MI_LOG_CCFG1 ("  Scope:                        %s", icfgScopeValEnum[serverDev->scope]);
  MI_LOG_CCFG1 ("  Server Device type:           %s", icfgDevTypeValEnum[serverDev->devType]);
  MI_LOG_CCFG1 ("  Map Info:                     %s", serverDev->mapInfo);
    
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (serverDev);
    return (SD_SUCCESS);
    }

  ret = mis_find_device (serverDev->name, &dev_ref);
  if (ret == SD_SUCCESS)
    {
    mis_vcc = mi_icfg_ctrl->mis_vcc;
    mis_device = MIS_DEV_REF_TO_DEV (dev_ref);
    ret = mis_add_device_to_vcc_ex (mis_vcc, serverDev->scope,
                                 serverDev->name, 
                                 serverDev->devType, 
                                 mis_device->sbo,
                                 SD_TRUE,
                                 mis_device->taggable, 
				 &dev_ref); 
    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR1 ("Error adding VCC Device '%s'", serverDev->name);
      }
    }
  else
    {
    MI_LOG_ERR1 ("Error, could not find local Device '%s'", serverDev->name);
    }

  if (ret == SD_SUCCESS)
    ret = u_miIcfgProcServerDev (mi_icfg_ctrl->usr, mis_vcc, dev_ref, serverDev);  

  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndServerDevices 				*/
/************************************************************************/

ST_RET  u_icfg_EndServerDevices (ST_VOID *usr)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIS_VCC      *mis_vcc;

  MI_LOG_CFG0 ("u_icfg_EndServerDevices");
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    return (SD_SUCCESS);
    }

  mis_vcc = mi_icfg_ctrl->mis_vcc;
  mis_sort_vcc_devices (mis_vcc);
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*			u_icfg_StartServerInfoMsgs 				*/
/************************************************************************/

ST_RET  u_icfg_StartServerInfoMsgs (ST_VOID *usr, ST_INT numDv)
  {
  MI_LOG_CCFG1 ("u_icfg_StartServerInfoMsgs total: %d", numDv);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ServerInfoMsg 				*/
/************************************************************************/

ST_RET  u_icfg_ServerInfoMsg (ST_VOID *usr, ICFG_SERVER_INFOMSG *serverIm)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIS_VCC      *mis_vcc;
ST_RET ret;

  MI_LOG_CFG0 ("u_icfg_ServerInfoMsg");
  MI_LOG_CCFG1 ("  SERVER Info Ref:              %d", serverIm->infoRef);
  MI_LOG_CCFG1 ("  Scope:                        %s", icfgScopeValEnum[serverIm->scope]);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (serverIm);
    return (SD_SUCCESS);
    }

  mis_vcc = mi_icfg_ctrl->mis_vcc;
  ret = u_miIcfgProcServerIM (mi_icfg_ctrl->usr, mis_vcc, serverIm); /* User free's via 'icfgFree'*/
  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndServerInfoMsgs 				*/
/************************************************************************/

ST_RET  u_icfg_EndServerInfoMsgs (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndServerInfoMsgs");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartServerDataSets 				*/
/************************************************************************/

ST_RET  u_icfg_StartServerDataSets (ST_VOID *usr, ST_INT numDv)
  {
  MI_LOG_CFG1 ("u_icfg_StartServerDataSets total: %d", numDv);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ServerDataSet 				*/
/************************************************************************/

ST_RET  u_icfg_ServerDataSet (ST_VOID *usr, ICFG_SERVER_DS *serverDs)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIS_VCC      *mis_vcc;
MIS_DS 	     *mis_ds;
MIS_DV_REF    mis_dv_ref;
MIS_DV 	     *mis_dv;
ST_INT 	      mi_type;
ST_INT	      newNumDv;	
ST_INT	      i;
ST_RET        ret;

  MI_LOG_CFG0 ("u_icfg_ServerDataSet");
  MI_LOG_CCFG1   ("  SERVER Data Set name:            %s", serverDs->name);
  MI_LOG_CCFG1   ("  Scope:                           %s", icfgScopeValEnum[serverDs->scope]);
  MI_LOG_CCFG1   ("  Include Transfer_Set_Name:       %c", serverDs->transferSetName == SD_TRUE ? 'Y' : 'N');
  MI_LOG_CCFG1   ("  Include Transfer_Set_Time_Stamp: %c", serverDs->transferSetTimeStamp == SD_TRUE ? 'Y' : 'N');
  MI_LOG_CCFG1   ("  Include DSConditions_Detected:   %c", serverDs->dsConditionsDetected == SD_TRUE ? 'Y' : 'N');
  MI_LOG_CCFG1   ("  Include Event_Code_Detected:     %c", serverDs->eventCodeDetected == SD_TRUE ? 'Y' : 'N');
  MI_LOG_CCFG1   ("  Num Dv:                          %d", serverDs->numDv);

  for (i = 0; i < serverDs->numDv;  ++i)
    {
    MI_LOG_CCFG3 ("    %d) %-32.32s %s", i, serverDs->dvArray[i].name,
				icfgScopeValEnum[serverDs->dvArray[i].scope]);
    }
  
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (serverDs);
    return (SD_SUCCESS);
    }

  mis_vcc = mi_icfg_ctrl->mis_vcc;

  newNumDv = serverDs->numDv;

  if(serverDs->transferSetName == SD_TRUE)
    newNumDv++;
  if(serverDs->transferSetTimeStamp == SD_TRUE)
    newNumDv++;
  if(serverDs->dsConditionsDetected == SD_TRUE)
    newNumDv++;
  if(serverDs->eventCodeDetected == SD_TRUE)
    newNumDv++;
  
  mis_ds = mis_add_ds_to_vcc (mis_vcc, serverDs->scope,
			      serverDs->name, newNumDv);
  if (mis_ds == NULL)
    {
    MI_LOG_ERR1 ("Error adding VCC DS '%s'", serverDs->name);
    icfgFree (serverDs);
    return (SD_FAILURE);
    }
  
  if(serverDs->transferSetName == SD_TRUE)
    {
    ret = mis_find_vcc_dv (mis_vcc,
                     (char *)"Transfer_Set_Name", 
                     ICC_SPEC, 
		     &mis_dv, &mis_dv_ref, &mi_type);

    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR0 ("Error finding Special Variable: Transfer_Set_Name");
      icfgFree (serverDs);
      return (SD_FAILURE);
      }

    mis_add_ds_dv (mis_vcc, mis_ds, mis_dv_ref);
    }
  
  if(serverDs->transferSetTimeStamp == SD_TRUE)
    {
    ret = mis_find_vcc_dv (mis_vcc,
                     "Transfer_Set_Time_Stamp", 
                     ICC_SPEC, 
		     &mis_dv, &mis_dv_ref, &mi_type);

    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR0 ("Error finding Special Variable: Transfer_Set_Time_Stamp");
      icfgFree (serverDs);
      return (SD_FAILURE);
      }

    mis_add_ds_dv (mis_vcc, mis_ds, mis_dv_ref);
    }
  
  if(serverDs->dsConditionsDetected == SD_TRUE)
    {
    ret = mis_find_vcc_dv (mis_vcc,
                     "DSConditions_Detected", 
                     ICC_SPEC, 
		     &mis_dv, &mis_dv_ref, &mi_type);

    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR0 ("Error finding Special Variable: DSConditions_Detected");
      icfgFree (serverDs);
      return (SD_FAILURE);
      }

    mis_add_ds_dv (mis_vcc, mis_ds, mis_dv_ref);
    }
  
  if(serverDs->eventCodeDetected == SD_TRUE)
    {
    ret = mis_find_vcc_dv (mis_vcc,
                     "Event_Code_Detected", 
                     ICC_SPEC, 
		     &mis_dv, &mis_dv_ref, &mi_type);

    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR0 ("Error finding Special Variable: Event_Code_Detected");
      icfgFree (serverDs);
      return (SD_FAILURE);
      }

    mis_add_ds_dv (mis_vcc, mis_ds, mis_dv_ref);
    }
  
  for (i = 0; i < serverDs->numDv;  ++i)
    {
    ret = mis_find_vcc_dv (mis_vcc,
                     serverDs->dvArray[i].name, 
                     serverDs->dvArray[i].scope, 
		     &mis_dv, &mis_dv_ref, &mi_type);

    if (ret != SD_SUCCESS)
      {
      MI_LOG_ERR1 ("Error finding DS DV '%s'", serverDs->dvArray[i].name);
      icfgFree (serverDs);
      return (SD_FAILURE);
      }

    mis_add_ds_dv (mis_vcc, mis_ds, mis_dv_ref);
    }

  ret = u_miIcfgProcServerDs (mi_icfg_ctrl->usr, mis_vcc, mis_ds, serverDs);  
  return (SD_SUCCESS);
  }

/************************************************************************/
/*		u_icfg_EndServerDataSets 				*/
/************************************************************************/

ST_RET  u_icfg_EndServerDataSets (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndServerDataSets");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartClientObjects 				*/
/************************************************************************/

ST_RET  u_icfg_StartClientObjects (ST_VOID *usr, ICFG_CLIENT_OBJ_COUNTS *clientObjCounts)
  {
MI_ICFG_CTRL   *mi_icfg_ctrl;
ICFG_REMOTE_CC *remote_cc;
MI_REMOTE      *mi_remote;
MIC_VCC	       *mic_vcc;
ST_CHAR        *icc_name;

  MI_LOG_CFG0 ("u_icfg_StartClientObjs");
  MI_LOG_CCFG1 ("  NumVccDv:         %d",   clientObjCounts->numVccDv);
  MI_LOG_CCFG1 ("  NumIccDv:         %d",   clientObjCounts->numIccDv);
  MI_LOG_CCFG1 ("  NumDs:            %d",   clientObjCounts->numDs);
  MI_LOG_CCFG1 ("  NumTsCtrl:        %d",   clientObjCounts->numDsTs);
  MI_LOG_CCFG1 ("  Num ICC Dev:      %d",   clientObjCounts->numIccDev);
  MI_LOG_CCFG1 ("  Num VCC Dev:      %d",   clientObjCounts->numVccDev);
  MI_LOG_CCFG1 ("  Num ICC InfoMsgs: %d",   clientObjCounts->numIccInfoMsg);
  MI_LOG_CCFG1 ("  Num VCC InfoMsgs: %d",   clientObjCounts->numVccInfoMsg);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (clientObjCounts);
    return (SD_SUCCESS);
    }

  mi_remote = mi_icfg_ctrl->mi_remote; 
  remote_cc = mi_icfg_ctrl->remote_cc;


/* Make sure it is OK to have client objects */
  if ((mi_icfg_ctrl->serviceRole & ICFG_ROLE_CLIENT) == 0)
    {				   
    /* No client objects allowed */
    if (clientObjCounts->numVccDv != 0 ||	     
        clientObjCounts->numIccDv != 0 ||
        clientObjCounts->numDs    != 0 ||
        clientObjCounts->numVccDev != 0 ||
        clientObjCounts->numIccDev != 0 ||
        clientObjCounts->numVccInfoMsg != 0 ||
        clientObjCounts->numIccInfoMsg)
      {
      MI_LOG_ERR0 ("Configured Service Role does not allow client objects");
      return (SD_FAILURE);
      }
    }

  icc_name = chk_strdup (remote_cc->blt.remoteDom);
  mic_vcc = mic_create_vcc (mi_remote,
		        remote_cc->blt.id,
		        icc_name,
  		        clientObjCounts->numVccDv,	 		
		        0, 	/* no VCC scope DS */
  		        clientObjCounts->numIccDv + 4, /* (+4 for sto) */
		        clientObjCounts->numDs,
  		        clientObjCounts->numDsTs);		 
  mi_icfg_ctrl->mic_vcc = mic_vcc;

  mic_add_dv_to_vcc (mic_vcc, ICC_SPEC, chk_strdup ((char *)"Transfer_Set_Name"), 
		     MI_TYPEID_MMS_OBJECT_NAME, NULL);
  mic_add_dv_to_vcc (mic_vcc, ICC_SPEC, chk_strdup ((char *)"DSConditions_Detected"), 
		     MI_TYPEID_DS_COND, NULL);
  mic_add_dv_to_vcc (mic_vcc, ICC_SPEC, chk_strdup ((char *)"Event_Code_Detected"), 
		     MI_TYPEID_INT16, NULL);
  mic_add_dv_to_vcc (mic_vcc, ICC_SPEC, chk_strdup ((char *)"Transfer_Set_Time_Stamp"), 
		     MI_TYPEID_GMT_BASED_S, NULL);

/* Block 1 and Block 2 required supported features are already set	*/
/* Now see if we need to enable blocks 4 and 5 				*/
  if (clientObjCounts->numVccDev + clientObjCounts->numIccDev > 0)
    {
    mic_enable_device_ctrl (mic_vcc, clientObjCounts->numVccDev, 
    				     clientObjCounts->numIccDev);
    }
  if (clientObjCounts->numVccInfoMsg + clientObjCounts->numIccInfoMsg > 0)
    mic_enable_IMTransfers (mic_vcc);
  
  icfgFree (clientObjCounts);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*		u_icfg_EndClientObjects 				*/
/************************************************************************/

ST_RET  u_icfg_EndClientObjects (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndClientObjs");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartClientDataValues 				*/
/************************************************************************/

ST_RET  u_icfg_StartClientDataValues (ST_VOID *usr, ST_INT numDv)
  {
  MI_LOG_CFG1 ("u_icfg_StartClientDataValues total: %d", numDv);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ClientDataValue 				*/
/************************************************************************/

ST_RET  u_icfg_ClientDataValue (ST_VOID *usr, ICFG_CLIENT_DV *clientDv)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIC_VCC      *mic_vcc;
MIC_DV       *mic_dv;
ST_CHAR      *dv_name;
ST_RET        ret;

  MI_LOG_CFG0  ("u_icfg_ClientDataValue");
  MI_LOG_CCFG1 ("  Name:      %s", clientDv->name);
  MI_LOG_CCFG1 ("  Scope:     %s", icfgScopeValEnum[clientDv->scope]);
  MI_LOG_CCFG1 ("  Type:      %s", icfgDvTypeValEnum[clientDv->dvType]);
  MI_LOG_CCFG1 ("  Read Only: %c", clientDv->readOnly ? 'Y' : 'N');
  MI_LOG_CCFG1 ("  Map Info:  %s", clientDv->mapInfo);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (clientDv);
    return (SD_SUCCESS);
    }

  mic_vcc = mi_icfg_ctrl->mic_vcc;

  dv_name = chk_strdup (clientDv->name);  
  mic_dv = mic_add_dv_to_vcc (mic_vcc, clientDv->scope,
                     	      dv_name,
                     	      clientDv->dvType, 
			      NULL);

  if (mic_dv != NULL)
    {
    ret = u_miIcfgProcClientDv (mi_icfg_ctrl->usr, mic_vcc, mic_dv, clientDv);  
    }
  else
    {
    ret = SD_FAILURE;
    MI_LOG_ERR1 ("Error creating adding DV '%s'", dv_name);
    }
  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndClientDataValues 				*/
/************************************************************************/

ST_RET  u_icfg_EndClientDataValues (ST_VOID *usr)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIC_VCC      *mic_vcc;

  MI_LOG_CFG0 ("u_icfg_EndClientDataValues");
  
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    return (SD_SUCCESS);
    }

  mic_vcc = mi_icfg_ctrl->mic_vcc;
  mic_sort_vcc_dv (mic_vcc);
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartClientDevices 				*/
/************************************************************************/

ST_RET  u_icfg_StartClientDevices (ST_VOID *usr, ST_INT numDev)
  {
  MI_LOG_CFG1 ("u_icfg_StartClientDevices total: %d", numDev);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ClientDevice 				*/
/************************************************************************/

ST_RET  u_icfg_ClientDevice (ST_VOID *usr, ICFG_CLIENT_DEV *clientDev)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIC_VCC      *mic_vcc;
MIC_DEVICE   *mic_device;
ST_CHAR      *dev_name;
ST_RET        ret;

  MI_LOG_CFG0 ("u_icfg_ClientDevice");
  MI_LOG_CCFG1 ("  CLIENT Device name: %s", clientDev->name);
  MI_LOG_CCFG1 ("  Scope:              %s", icfgScopeValEnum[clientDev->scope]);
  MI_LOG_CCFG1 ("  Local Device Type:  %s", icfgDvTypeValEnum[clientDev->devType]);
  MI_LOG_CCFG1 ("  Map Info:           %s", clientDev->mapInfo);
  MI_LOG_CCFG1 ("  Sbo:                %c", clientDev->sbo ? 'Y':'N');
  MI_LOG_CCFG1 ("  Tag Enable:         %c", clientDev->tagEnable ? 'Y':'N');

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (clientDev);
    return (SD_SUCCESS);
    }
  mic_vcc = mi_icfg_ctrl->mic_vcc;

  dev_name = chk_strdup (clientDev->name);  
  mic_device = mic_add_device_to_vcc (mic_vcc, 
                                     clientDev->scope, 
                                     dev_name, 
                                     clientDev->devType, 
                                     clientDev->sbo, 
                                     clientDev->tagEnable,
				     NULL); 

  if (mic_device != NULL)
    {
    ret = u_miIcfgProcClientDev (mi_icfg_ctrl->usr, mic_vcc, mic_device, clientDev);  
    }
  else
    {
    ret = SD_FAILURE;
    MI_LOG_ERR1 ("Error creating adding Device '%s'", dev_name);
    }
  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndClientDevices 				*/
/************************************************************************/

ST_RET  u_icfg_EndClientDevices (ST_VOID *usr)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIC_VCC      *mic_vcc;

  MI_LOG_CFG0 ("u_icfg_EndClientDevices");
  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    return (SD_SUCCESS);
    }

  mic_vcc = mi_icfg_ctrl->mic_vcc;
  mic_sort_vcc_devices (mic_vcc);
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartClientInfoMsgs 				*/
/************************************************************************/

ST_RET  u_icfg_StartClientInfoMsgs (ST_VOID *usr, ST_INT numDv)
  {
  MI_LOG_CFG1 ("u_icfg_StartClientInfoMsgs total: %d", numDv);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ClientInfoMsg 				*/
/************************************************************************/

ST_RET  u_icfg_ClientInfoMsg (ST_VOID *usr, ICFG_CLIENT_INFOMSG *clientIm)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIC_VCC      *mic_vcc;
ST_RET ret;

  MI_LOG_CFG0 ("u_icfg_ClientInfoMsg");

  MI_LOG_CCFG1 ("  CLIENT Data Value name:       %d", clientIm->infoRef);
  MI_LOG_CCFG1 ("  Scope:                        %s", icfgScopeValEnum[clientIm->scope]);
  MI_LOG_CCFG1 ("  Map Info:                     %s", clientIm->mapInfo);
  MI_LOG_CCFG1 ("  Max Size:                     %d", clientIm->maxSize);

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (clientIm);
    return (SD_SUCCESS);
    }

  mic_vcc = mi_icfg_ctrl->mic_vcc;
  ret = u_miIcfgProcClientIM (mi_icfg_ctrl->usr, mic_vcc, clientIm); /* User free's via 'icfgFree'*/
  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndClientInfoMsgs 				*/
/************************************************************************/

ST_RET  u_icfg_EndClientInfoMsgs (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndClientInfoMsgs");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartClientDataSets 				*/
/************************************************************************/

ST_RET  u_icfg_StartClientDataSets (ST_VOID *usr, ST_INT numDv)
  {
  MI_LOG_CCFG1 ("u_icfg_StartClientDataSets total: %d", numDv);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ClientDataSet 				*/
/************************************************************************/

ST_RET  u_icfg_ClientDataSet (ST_VOID *usr, ICFG_CLIENT_DS *clientDs)
  {
MI_ICFG_CTRL *mi_icfg_ctrl;
MIC_VCC      *mic_vcc;
ST_INT	      i;
ST_RET ret;

  MI_LOG_CFG0 ("u_icfg_ClientDataSet");
  MI_LOG_CCFG1   ("  CLIENT Data Set name:         %s",  clientDs->name);
  MI_LOG_CCFG1   ("  Num Dv:                       %d",  clientDs->numDv);

  for (i=0;  i < clientDs->numDv;  i++)
    {
    MI_LOG_CCFG3 ("    %d) %-32.32s %s", i, clientDs->dvArray[i].name,
			icfgScopeValEnum[clientDs->dvArray[i].scope]);
    }

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (clientDs);
    return (SD_SUCCESS);
    }

  mic_vcc = mi_icfg_ctrl->mic_vcc;
  ret = u_miIcfgProcClientDs (mi_icfg_ctrl->usr, mic_vcc, clientDs);  
  return (ret);
  }

/************************************************************************/
/*		u_icfg_EndClientDataSets 				*/
/************************************************************************/

ST_RET  u_icfg_EndClientDataSets (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndClientDataSets");
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*		u_icfg_StartClientDSTSs 				*/
/************************************************************************/

ST_RET  u_icfg_StartClientDSTSs (ST_VOID *usr, ST_INT numDv)
  {
  MI_LOG_CFG1 ("u_icfg_StartClientDSTSs total: %d", numDv);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_icfg_ClientDSTS 				*/
/************************************************************************/

ST_RET  u_icfg_ClientDSTS (ST_VOID *usr, ICFG_CLIENT_DSTS *clientDsTs)
  {
MI_ICFG_CTRL   *mi_icfg_ctrl;
ST_RET ret;

  MI_LOG_CFG0 ("u_icfg_ClientDSTS");
  MI_LOG_CCFG1 ("  CLIENT DS Transfer Set name:  %s", clientDsTs->dsName);
  MI_LOG_CCFG1 ("  Association:                  %s", clientDsTs->assocName);
  MI_LOG_CCFG1 ("  Interval:                     %d", clientDsTs->interval);
  MI_LOG_CCFG1 ("  Rbe:                          %c", clientDsTs->rbe ? 'Y':'N');
  MI_LOG_CCFG1 ("  Buf Time:                     %d", clientDsTs->bufferTime);
  MI_LOG_CCFG1 ("  Integrity:                    %d", clientDsTs->integrity);
  MI_LOG_CCFG1 ("  Start Time:                   %d", clientDsTs->startTime);
  MI_LOG_CCFG1 ("  Critical:                     %c", clientDsTs->critical ? 'Y':'N');
  MI_LOG_CCFG0 ("  DSConditions Requested:");
  MI_LOG_CCFG1 ("    Integrity: %s", 
	clientDsTs->dsConditionsRequested & MI_DSC_INTEGRITY ? "Y" : "N");
  MI_LOG_CCFG1 ("    Interval: %s", 
	clientDsTs->dsConditionsRequested & MI_DSC_INTERVAL ? "Y" : "N");
  MI_LOG_CCFG1 ("    Change: %s", 
	clientDsTs->dsConditionsRequested & MI_DSC_CHANGE ? "Y" : "N");

  mi_icfg_ctrl = (MI_ICFG_CTRL *)usr;
  if (mi_icfg_ctrl->target_remote == SD_FALSE)
    {
    MI_LOG_CCFG0 ("  >> Ignoring this remote <<");
    icfgFree (clientDsTs);
    return (SD_SUCCESS);
    }

  ret = u_miIcfgProcClientDsTs (mi_icfg_ctrl->usr, mi_icfg_ctrl->mic_vcc, clientDsTs);  
  return (ret);
  }

/************************************************************************/
/*			u_icfg_EndClientDSTSs 				*/
/************************************************************************/

ST_RET  u_icfg_EndClientDSTSs (ST_VOID *usr)
  {
  MI_LOG_CFG0 ("u_icfg_EndClientDSTSs");
  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			mi_icfg_free_local_cc 				*/
/************************************************************************/

ST_VOID mi_icfg_free_local_cc ()
  {
ST_INT i;
ST_INT j;
ST_BOOLEAN sv;

/* Take care of local data values */
  for (i = 0; i < mis_num_dv_ctrl; ++i)
    {
  /* We only free DV's that are not special variables */
    sv = SD_FALSE;
    for (j = 0; j < _mi_num_special_vars; ++j)
      {
      if (mis_dv_ctrl[i].dv_name ==_mis_special_vars[j].name)
        {
        sv = SD_TRUE;
        break;
        }      
      }
    if (sv == SD_FALSE)
      chk_free (mis_dv_ctrl[i].dv_name);
    }

/* Take care of local devices */
  for (i = 0; i < mis_num_device_ctrl; ++i)
    chk_free (mis_device_ctrl[i].device_name);
  }


/************************************************************************/
/*			mi_icfg_free_remote_cc 				*/
/************************************************************************/

ST_VOID mi_icfg_free_remote_cc (MI_REMOTE *mi_remote_sel)
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc, *next_assoc;
MIS_VCC       *mis_vcc;
MIC_VCC	      *mic_vcc;
MIC_DV	      *mic_dv;
MIC_DEVICE    *mic_device;
ST_INT i;

  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {				      
    if (mi_remote_sel == NULL || mi_remote == mi_remote_sel)
      {
      chk_free (mi_remote->name);
      mi_assoc = mi_remote->mi_assoc_list;
      while (mi_assoc)
        {
        chk_free (mi_assoc->name);
        chk_free (mi_assoc->local_ar);
        next_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
        mi_free_assoc (mi_assoc);         
        mi_assoc = next_assoc;
        }

      mis_vcc = mi_remote->mis_vcc;
      if (mis_vcc != NULL)
        chk_free (mis_vcc->icc_name);

      mic_vcc = mi_remote->mic_vcc;
      if (mic_vcc != NULL)
        {
        chk_free (mic_vcc->icc_name);
    
        for (i = 0; i < mic_vcc->num_vcc_dv; ++i)
          {
          mic_dv = mic_vcc->vcc_dv_tbl[i];
          chk_free (mic_dv->dv_name);
          }
        for (i = 0; i < mic_vcc->num_icc_dv; ++i)
          {
          mic_dv = mic_vcc->icc_dv_tbl[i];
          chk_free (mic_dv->dv_name);
          }
    
        for (i = 0; i < mic_vcc->num_vcc_devices; ++i)
          {
          mic_device = mic_vcc->vcc_device_tbl[i];
          chk_free (mic_device->device_name);
          }
        for (i = 0; i < mic_vcc->num_icc_devices; ++i)
          {
          mic_device = mic_vcc->icc_device_tbl[i];
          chk_free (mic_device->device_name);
          }
        }
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

/************************************************************************/
/************************************************************************/
/*			mi_icfg_log_cfg 				*/
/************************************************************************/

ST_VOID mi_icfg_log_cfg ()
  {
MI_REMOTE     *mi_remote;
MI_ASSOC_CTRL *mi_assoc;

  SLOGALWAYS0 ("ICCP Configuration");
  SLOGCALWAYS0 ("");
  mis_log_dv_tables ();

  SLOGCALWAYS0 ("");
  mis_log_dev_tables ();

  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {
    SLOGCALWAYS0 ("");
    SLOGCALWAYS1 ("****** Remote '%s' ******", mi_remote->name);
    if (mi_remote->mis_vcc != NULL)
      {
      SLOGCALWAYS0 ("");
      mis_log_vcc_dv_tables (mi_remote->mis_vcc);
      SLOGCALWAYS0 ("");
      mis_log_vcc_device_tables (mi_remote->mis_vcc);
      }
    if (mi_remote->mic_vcc != NULL)
      {
      SLOGCALWAYS0 ("");
      mic_log_vcc_device_tables (mi_remote->mic_vcc);
      SLOGCALWAYS0 ("");
      mic_log_vcc_dv_tables (mi_remote->mic_vcc);
      }

    mi_assoc = mi_remote->mi_assoc_list;
    while (mi_assoc)
      {
      SLOGCALWAYS0 ("");
      mi_log_assoc (mi_assoc);
      mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
      }

    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

/************************************************************************/
/*			mi_icfg_log_status				*/
/************************************************************************/

ST_VOID mi_icfg_log_status ()
  {
MI_REMOTE     *mi_remote;
MI_ASSOC_CTRL *mi_assoc;
ST_CHAR *stateStr;

  SLOGALWAYS0 ("ICCP Status");
  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {
    SLOGCALWAYS0 ("");
    SLOGCALWAYS1 ("Remote '%s'", mi_remote->name);
#ifdef MI_REDUNDANCY_SUPPORT
    SLOGCALWAYS1 ("  Configuration CRC:         %08x", mi_remote->cfgCrc);
    SLOGCALWAYS1 ("  Partner Configuration CRC: %08x", mi_remote->partnerCfgCrc);
    SLOGCALWAYS1 ("  Data Sync:                 %s", mi_remote->syncActive ? "Active" : "Inactive");
    SLOGCALWAYS1 ("  Data Sync Count:           %ld", mi_remote->dataSyncCount);
#endif
    mi_assoc = mi_remote->mi_assoc_list;
    while (mi_assoc)
      {
      SLOGCALWAYS1 ("  Association:              '%s'", mi_assoc->name);
      SLOGCALWAYS1 ("    Control Flags:           %08x", mi_assoc->ctrl_flags);
      if (mi_assoc->state == MI_ASSOC_STATE_IDLE)
        stateStr = "MI_ASSOC_STATE_IDLE";
      else if (mi_assoc->state == MI_ASSOC_STATE_ACTIVE)
        stateStr = "MI_ASSOC_STATE_ACTIVE";
      else if (mi_assoc->state == MI_ASSOC_STATE_PARAM_CHECK)
        stateStr = "MI_ASSOC_STATE_PARAM_CHECK";
      else if (mi_assoc->state == MI_ASSOC_STATE_INIT_REQ)
        stateStr = "MI_ASSOC_STATE_INIT_REQ";
      else if (mi_assoc->state == MI_ASSOC_STATE_INIT_IND)
        stateStr = "MI_ASSOC_STATE_INIT_IND";
      else if (mi_assoc->state == MI_ASSOC_STATE_START_CLIENT)
        stateStr = "MI_ASSOC_STATE_START_CLIENT";
      else if (mi_assoc->state == MI_ASSOC_STATE_START_SERVER)
        stateStr = "MI_ASSOC_STATE_START_SERVER";
      else if (mi_assoc->state == MI_ASSOC_STATE_CONCLUDING)
        stateStr = "MI_ASSOC_STATE_CONCLUDING";
      else if (mi_assoc->state == MI_ASSOC_STATE_ABORT)
        stateStr = "MI_ASSOC_STATE_ABORT";
      else if (mi_assoc->state == MI_ASSOC_STATE_ABORTING)
        stateStr = "MI_ASSOC_STATE_ABORTING";
      else if (mi_assoc->state == MI_ASSOC_STATE_BACKOFF)
        stateStr = "MI_ASSOC_STATE_BACKOFF";
      else if (mi_assoc->state == MI_ASSOC_STATE_INVALID)
        stateStr = "MI_ASSOC_STATE_INVALID";
      else
        stateStr = "<Invalid>";
      SLOGCALWAYS2 ("    State:                   %08x  %s", mi_assoc->state, stateStr);

      mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
      }

    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

