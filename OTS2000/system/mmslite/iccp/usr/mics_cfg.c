/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mics_cfg.c  						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     08    Copyright year update                        */
/* 06/20/02  MDE     07    Removed ICFG naming				*/
/* 06/20/02  DGE     06    Added micuFindDv.				*/
/* 04/26/02  MDE     05    Moved DSCondReq helper code to icfgxml.c	*/
/* 04/24/02  MDE     04    Use new DSConditions for DSTS		*/
/* 04/17/02  MDE     03	   Added remote DV config options  		*/
/* 01/21/02  MDE     02    Now add special variables to client DS	*/
/* 11/01/01  MDE     01    Created					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mvl_acse.h"
#include "mi.h"
#include "mics_cfg.h"
#include "mi_icfg.h"

/************************************************************************/
#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/

MIU_REMOTE *miu_remote_list;
MISU_DV    *misu_dv_list;
MISU_DEV   *misu_dev_list;

/************************************************************************/
/*			miuFindRemote 					*/
/************************************************************************/

MIU_REMOTE *miuFindRemote (ST_CHAR *remoteName)
  {
MIU_REMOTE *miuRemote;

  miuRemote = miu_remote_list;
  while (miuRemote != NULL)
    {
    if (!strcmp (miuRemote->name, remoteName))
      break;

    miuRemote = (MIU_REMOTE *)list_get_next (miu_remote_list, miuRemote);
    }
  return (miuRemote);
  }

/************************************************************************/
/*			miuFindMiRemote 				*/
/************************************************************************/

MIU_REMOTE *miuFindMiRemote (MI_REMOTE *mi_remote)
  {
MIU_REMOTE *miuRemote;

  miuRemote = miu_remote_list;
  while (miuRemote != NULL)
    {
    if (miuRemote->mi_remote == mi_remote)
      break;

    miuRemote = (MIU_REMOTE *)list_get_next (miu_remote_list, miuRemote);
    }
  return (miuRemote);
  }

/************************************************************************/
/*			micuFindDv					*/
/************************************************************************/

MICU_DV *micuFindDv (MIU_REMOTE *miuRemote, 
		     ST_CHAR *dvName, ST_INT scope)
  {
MICU_DV *dv_list;

  dv_list = miuRemote->dv_list;
  while (dv_list != NULL)
    {
    if (!strcmp (dv_list->mic_dv->dv_name, dvName))
      break;

    dv_list = (MICU_DV *)list_get_next (miuRemote->dv_list, dv_list);
    }
  return (dv_list);
  }

/************************************************************************/
/*			micuFindDs 					*/
/************************************************************************/

MICU_DS *micuFindDs (MIU_REMOTE *miuRemote, ST_CHAR *dsName)
  {
MICU_DS *ds_list;

  ds_list = miuRemote->ds_list;
  while (ds_list != NULL)
    {
    if (!strcmp (ds_list->ds_name, dsName))
      break;

    ds_list = (MICU_DS *)list_get_next (miuRemote->ds_list, ds_list);
    }
  return (ds_list);
  }

/************************************************************************/
/*			micuFindDsTs 					*/
/************************************************************************/

MICU_DSTS *micuFindDsTs (MIU_REMOTE *miuRemote, ST_CHAR *dsTsName)
  {
MICU_DSTS *dsts;

  dsts = miuRemote->dsts_list;
  while (dsts != NULL)
    {
    if (!strcmp (dsts->dsTsName, dsTsName))
      break;

    dsts = (MICU_DSTS *)list_get_next (miuRemote->dsts_list, dsts);
    }
  return (dsts);
  }

/************************************************************************/
/************************************************************************/
/* ICFG Specific Code: Setting up mics_cfg data structures		*/
/************************************************************************/
/************************************************************************/

static MIU_REMOTE *miuCurrRemote;

/************************************************************************/
/* These are callback functions from the MiIcfg subsystem, and are 	*/
/* called during configuration load time. We can perform any desired	*/
/* processing here, and in our case we will asociate the network	*/
/* visible data values with our local storage.				*/
/************************************************************************/
/* 		u_miIcfgProcLocalControlCenter 				*/
/************************************************************************/

ST_RET u_miIcfgProcLocalControlCenter (ST_VOID *usr, ICFG_LOCAL_CC *icfgLocalCc)
  {
/* Note: icfgLocalCc must not be free'd in this function, and will be	*/
/* free'd (by mi_icfg) when configuration is complete 			*/

  return (SD_SUCCESS);
  }

/************************************************************************/
/* 		u_miIcfgProcRemoteControlCenter 			*/
/************************************************************************/

ST_RET u_miIcfgProcRemoteControlCenter (ST_VOID *usr, MI_REMOTE *mi_remote, 
					ICFG_REMOTE_CC *icfgRemoteCc)
  {
MIU_REMOTE *miuRemote;

  miuRemote = (MIU_REMOTE *)chk_calloc (1, sizeof (MIU_REMOTE));
  list_add_last ((ST_VOID **)&miu_remote_list, miuRemote);
  miuCurrRemote = miuRemote;

  strcpy (miuRemote->name, icfgRemoteCc->name);
  miuRemote->mi_remote = mi_remote;
  miuRemote->remDvValidation = icfgRemoteCc->blt.remDvValidation;
  miuRemote->remDvGetConfig = icfgRemoteCc->blt.remDvGetConfig;
  miuRemote->remDvExcludeInvalidFromDs = icfgRemoteCc->blt.remDvExcludeInvalidFromDs;

/* Note: icfgRemoteCc must not be free'd in this function, and will be	*/
/* free'd (by mi_icfg) when configuration is complete 			*/
  return (SD_SUCCESS);
  }

/************************************************************************/
/* 		u_miIcfgProcAssociation 				*/
/************************************************************************/

ST_RET u_miIcfgProcAssociation (ST_VOID *usr, MI_ASSOC_CTRL *mi_assoc, 
				ICFG_ASSOCIATION *icfgAssoc)
  {
  mi_icfg_free (icfgAssoc);		/* We have no need for this information */
  return (SD_SUCCESS);
  }  

/************************************************************************/
/*			u_miIcfgProcLocalDv 				*/
/************************************************************************/

ST_RET u_miIcfgProcLocalDv (ST_VOID *usr, MIS_DV_REF misDvRef, 
			    ICFG_LOCAL_DV *icfgLocalDv, ST_BOOLEAN newDv)
  {
MISU_DV *miuDv;
static ST_INT dvCount = 0;

  miuDv = (MISU_DV *)chk_calloc (1, sizeof (MISU_DV));
  list_add_last((ST_VOID **) &misu_dv_list, (ST_VOID *) miuDv);
  miuDv->dvRef = misDvRef;
  miuDv->mi_type = icfgLocalDv->dvType;
  MIS_DV_REF_TO_DV(misDvRef)->access_info = miuDv;

  switch (miuDv->mi_type)
    {
    case MI_TYPEID_REAL_EXTENDED:
      miuDv->data.re.Value = ((ST_FLOAT) 1.01) * (ST_FLOAT) dvCount;

    /* Set up the various quality flags */      
      MI_QSET_VALIDITY    (miuDv->data.re.Flags, MI_QFLAG_VALIDITY_VALID);
      MI_QSET_CURR_SOURCE (miuDv->data.re.Flags, MI_QFLAG_CURR_SOURCE_TELEMETERED);
      MI_QSET_NORMAL_VAL  (miuDv->data.re.Flags, MI_QFLAG_NORMAL_VAL_NORMAL);
    break;

    case MI_TYPEID_DISCRETE_EXTENDED:
      miuDv->data.de.Value = dvCount;

    /* Set up the various quality flags */      
      MI_QSET_VALIDITY    (miuDv->data.de.Flags, MI_QFLAG_VALIDITY_VALID);
      MI_QSET_CURR_SOURCE (miuDv->data.de.Flags, MI_QFLAG_CURR_SOURCE_TELEMETERED);
      MI_QSET_NORMAL_VAL  (miuDv->data.de.Flags, MI_QFLAG_NORMAL_VAL_NORMAL);
    break;

    case MI_TYPEID_STATE_EXTENDED:

      if (dvCount % 2)
        {
        MI_SET_DATA_STATE (miuDv->data.se.Flags,MI_DATA_STATE_ON);
        }
      else
        {
        MI_SET_DATA_STATE (miuDv->data.se.Flags,MI_DATA_STATE_OFF);
        }

    /* Set up the various quality flags */      
      MI_QSET_VALIDITY    (miuDv->data.se.Flags, MI_QFLAG_VALIDITY_VALID);
      MI_QSET_CURR_SOURCE (miuDv->data.se.Flags, MI_QFLAG_CURR_SOURCE_TELEMETERED);
      MI_QSET_NORMAL_VAL  (miuDv->data.se.Flags, MI_QFLAG_NORMAL_VAL_NORMAL);
    break;
    }
  ++dvCount;

  mi_icfg_free (icfgLocalDv);	/* We have no need for this information */
  return (SD_SUCCESS);
  }  

/************************************************************************/
/*			u_miIcfgProcLocalDev 				*/
/************************************************************************/

ST_RET u_miIcfgProcLocalDev (ST_VOID *usr, MIS_DEVICE_REF misDevRef, 
			     ICFG_LOCAL_DEV *icfgLocalDev, ST_BOOLEAN newDev)  
  {
MISU_DEV *miuDev;

  miuDev = (MISU_DEV *)chk_calloc (1, sizeof (MISU_DEV));
  list_add_last((ST_VOID **) &misu_dev_list, (ST_VOID *) miuDev);
  miuDev->devRef = misDevRef;
  miuDev->dev_type = icfgLocalDev->devType;
  miuDev->selTime = icfgLocalDev->selTime;  

  MIS_DEV_REF_TO_DEV(misDevRef)->access_info = miuDev;

  mi_icfg_free (icfgLocalDev);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcLocalIm 				*/
/************************************************************************/

ST_RET u_miIcfgProcLocalIm (ST_VOID *usr, ICFG_LOCAL_INFOMSG *icfgLocalIm)  
  {
/* We don't require any linkage to real message elements ... 	*/
  mi_icfg_free (icfgLocalIm);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcServerDv  				*/
/************************************************************************/

ST_RET u_miIcfgProcServerDv  (ST_VOID *usr, MIS_VCC *mis_vcc, 
			      MIS_DV_REF dv_ref, ICFG_SERVER_DV *icfgServerDv)
  {
  mi_icfg_free (icfgServerDv);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcServerDev 				*/
/************************************************************************/

ST_RET u_miIcfgProcServerDev (ST_VOID *usr, MIS_VCC *mis_vcc, 
			      MIS_DEVICE_REF dev_ref, ICFG_SERVER_DEV *icfgServerDev)
  {
  mi_icfg_free (icfgServerDev);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcServerIM  				*/
/************************************************************************/

ST_RET u_miIcfgProcServerIM  (ST_VOID *usr, MIS_VCC *mis_vcc, 
			      ICFG_SERVER_INFOMSG *icfgServerIm)
  {
  mi_icfg_free (icfgServerIm);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcServerDs  				*/
/************************************************************************/

ST_RET u_miIcfgProcServerDs (ST_VOID *usr, MIS_VCC *mis_vcc, 
			     MIS_DS *mis_ds, ICFG_SERVER_DS *icfgServerDs)
  {
  mi_icfg_free (icfgServerDs);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcClientDv 				*/
/************************************************************************/

ST_RET u_miIcfgProcClientDv (ST_VOID *usr, MIC_VCC *mic_vcc, MIC_DV *micDv, 
			     ICFG_CLIENT_DV *icfgClientDv)
  {
MICU_DV *micuDv;

  micuDv = (MICU_DV *)chk_calloc (1, sizeof (MICU_DV));
  list_add_last((ST_VOID **) &miuCurrRemote->dv_list, (ST_VOID *) micuDv);
  micuDv->mic_dv = micDv;
  micuDv->mi_type = icfgClientDv->dvType;
  micDv->handle_info = micuDv;

  mi_icfg_free (icfgClientDv); /* We have no need for this information */
  return (SD_SUCCESS);
  }
 
/************************************************************************/
/*			u_miIcfgProcClientDev 				*/
/************************************************************************/

ST_RET u_miIcfgProcClientDev (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      MIC_DEVICE *micDev, ICFG_CLIENT_DEV *icfgClientDev)
  {
MICU_DEV *micuDev;

  micuDev = (MICU_DEV *)chk_calloc (1, sizeof (MICU_DEV));
  list_add_last((ST_VOID **) &miuCurrRemote->dev_list, (ST_VOID *) micuDev);
  micuDev->mic_dev = micDev;
  micuDev->dev_type = icfgClientDev->devType;
  micuDev->sbo = icfgClientDev->sbo;

  micDev->handle_info = micuDev;

  mi_icfg_free (icfgClientDev); /* We have no need for this information */
  return (SD_SUCCESS);
  }  

/************************************************************************/
/*			u_miIcfgProcClientIM  				*/
/************************************************************************/

ST_RET u_miIcfgProcClientIM  (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      ICFG_CLIENT_INFOMSG *icfgClientIm)
  {
  mi_icfg_free (icfgClientIm);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcClientDs  				*/
/************************************************************************/

ST_RET u_miIcfgProcClientDs  (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      ICFG_CLIENT_DS *icfgDs)
  {
ICFG_CLIENT_DS_DV *icfg_ds_dv;
MICU_DS     *miu_ds;
MIC_DV      *mic_dv;
ST_INT	     i;
ST_INT	     dvIndex;


/* Allow room for the special variables */
  miu_ds = (MICU_DS *)chk_calloc (1, sizeof (MICU_DS) + 
			  ((icfgDs->numDv + 4) * sizeof (MIC_DV *)));

  list_add_last ((ST_VOID **) &miuCurrRemote->ds_list, miu_ds);
  strcpy (miu_ds->ds_name, icfgDs->name);
  miu_ds->ds_scope = ICC_SPEC;
  miu_ds->num_dv = icfgDs->numDv;
  miu_ds->mic_dv = (MIC_DV **)(miu_ds + 1);

/* Add the special variables, if selected */
  dvIndex = 0;
  if(icfgDs->transferSetName == SD_TRUE)
    {
    mic_dv = mic_find_dv (mic_vcc, "Transfer_Set_Name", ICC_SPEC); 
    if (mic_dv != NULL)
      {
      miu_ds->mic_dv[dvIndex++] = mic_dv;
      ++miu_ds->num_dv;
      }
    else
      return (SD_FAILURE);
    }
  
  if(icfgDs->transferSetTimeStamp == SD_TRUE)
    {
    mic_dv = mic_find_dv (mic_vcc, "Transfer_Set_Time_Stamp", ICC_SPEC); 
    if (mic_dv != NULL)
      {
      miu_ds->mic_dv[dvIndex++] = mic_dv;
      ++miu_ds->num_dv;
      }
    else
      return (SD_FAILURE);
    }
  
  if(icfgDs->dsConditionsDetected == SD_TRUE)
    {
    mic_dv = mic_find_dv (mic_vcc, "DSConditions_Detected", ICC_SPEC); 
    if (mic_dv != NULL)
      {
      miu_ds->mic_dv[dvIndex++] = mic_dv;
      ++miu_ds->num_dv;
      }
    else
      return (SD_FAILURE);
    }
  
  if(icfgDs->eventCodeDetected == SD_TRUE)
    {
    mic_dv = mic_find_dv (mic_vcc, "Event_Code_Detected", ICC_SPEC); 
    if (mic_dv != NULL)
      {
      miu_ds->mic_dv[dvIndex++] = mic_dv;
      ++miu_ds->num_dv;
      }
    else
      return (SD_FAILURE);
    }

  for (i = 0; i < icfgDs->numDv; ++i, ++dvIndex)
    {
    icfg_ds_dv = &icfgDs->dvArray[i];
    mic_dv = mic_find_dv (mic_vcc, icfg_ds_dv->name, icfg_ds_dv->scope); 
    if (mic_dv != NULL)
      miu_ds->mic_dv[dvIndex] = mic_dv;
    else
      return (SD_FAILURE);
    }

  mi_icfg_free (icfgDs);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_miIcfgProcClientDsTs				*/
/************************************************************************/

ST_RET u_miIcfgProcClientDsTs (ST_VOID *usr, MIC_VCC *mic_vcc, 
			       ICFG_CLIENT_DSTS *icfgDsTs)
  {
MICU_DSTS  *miu_dsts;
MICU_DS    *miu_ds;

  miu_dsts = (MICU_DSTS *) chk_calloc (1, sizeof (MICU_DSTS));
  list_add_last ((ST_VOID **)&miuCurrRemote->dsts_list, miu_dsts);

  strcpy (miu_dsts->dsTsName, icfgDsTs->dsName);
  miu_ds = micuFindDs (miuCurrRemote, icfgDsTs->dsName);
  if (miu_ds == NULL)
    {
    return (SD_FAILURE);
    }
  miu_dsts->miu_ds = miu_ds;

  miu_dsts->dsts_data.StartTime = icfgDsTs->startTime;
  miu_dsts->dsts_data.Interval = (ST_INT16)icfgDsTs->interval;
  miu_dsts->dsts_data.TLE = (ST_INT16)icfgDsTs->tle;
  miu_dsts->dsts_data.BufferTime = (ST_INT16)icfgDsTs->bufferTime;
  miu_dsts->dsts_data.IntegrityCheck = (ST_INT16)icfgDsTs->integrity;
  miu_dsts->dsts_data.Critical = icfgDsTs->critical;
  miu_dsts->dsts_data.RBE = icfgDsTs->rbe;

  miu_dsts->dsts_data.DSConditionsRequested = 
  	     			icfgDsTs->dsConditionsRequested;

  miu_dsts->dsts_data.BlockData = SD_FALSE;
  miu_dsts->dsts_data.Status = SD_TRUE;
  miu_dsts->dsts_data.EventCodeRequested = 0;

  mi_icfg_free (icfgDsTs);	/* We have no need for this information */
  return (SD_SUCCESS);
  }

