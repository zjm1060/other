/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          2000 - 2002, All Rights Reserved                            */
/*                                                                      */
/* MODULE NAME : mit_xmlc.c                                             */
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/*                                                                      */
/* MODULE DESCRIPTION :    Handle the configurable items for the MITest */
/*                         application.  Config file is in XML format.  */
/*                                                                      */
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :                            */
/*                                                                      */
/* mitConfigure_xml							*/
/* mitUnconfigure_xml							*/
/*									*/
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev          Comments                                */
/* --------  ---  ------  --------------------------------------------  */
/* 05/17/07  zy     19    Put WriteDspInfo() in mitUnconfigure_xml()    */
/*                         and u_mic_dv_end                             */
/* 06/28/02  ASK    18    numLocalDataValue is declared for QNX		*/
/* 06/20/02  RKR    17    Copyright year update                         */
/* 04/29/02  ASK    16    now get dsConditionsRequested from mi_icfg	*/
/* 04/10/02  ASK    15    set the mis dev to point back to the mit dev	*/
/* 02/13/02  ASK    14    changes for mmslite init                      */
/* 02/12/02  RKR    13    inc numTransferSets				*/
/* 01/30/02  ASK    12    added "sysincs.h"				*/
/* 01/25/02  RKR    11    Only turn on integrity bit when value > 0	*/
/* 01/14/02  ASK    10    Moved MIT_CFG.c structs to mit_xmlc.c		*/
/* 01/09/02  ASK    09    Save supported features in MIT_REMOTE		*/
/* 01/08/02  ASK    08    Cleanup for special vars in Client DS		*/
/* 01/03/02  RKR    07    Added calls to mi_icfg_free                   */
/* 12/17/01  RKR    06    Set mmsl_max_msg_size				*/
/* 11/21/01  ASK    05    Cleanup for new MI Library			*/
/* 11/07/01  ASK    04    Updated for new MI_ICFG tools			*/
/* 10/30/01  ASK    03    Misc. Cleanup					*/
/* 10/15/01  ASK    02    Updated for new ICFG tools			*/
/* 04/17/01  ASK    01    Module created from existing mit_cfg.c        */
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "gen_list.h"
#include "mem_chk.h"
#include "slog.h"
#include "sx_defs.h"
#include "sx_log.h"
#include "mi.h"
#include "mi_icfg.h"
#include "icfg.h"
#include "mit_main.h"

#include "iccp_main.h"

static ST_CHAR *thisFileName = __FILE__;

extern	char	infobuf[1024];

/* Lists of stuff our application uses */
MIT_DEVICE *pstDeviceSet;
MIT_ASSOC_INFO *pstAssocInfoSet;
MIT_MIS_VCC *pstMisVccSet;
MIT_MIC_VCC *pstMicVccSet;
MIT_DATA_VALUE *pstDataValueSet;
MIT_REMOTE *pstRemoteSet;
MIT_INFO_MSG *pstInfoMsgSet;	

ST_INT numLocalDataValue;

/************************************************************************/
/*                      Static and Local variables                      */
/************************************************************************/

static ST_INT miType[] = { MI_TYPEID_REAL,
			   MI_TYPEID_STATE,
			   MI_TYPEID_DISCRETE,
			   MI_TYPEID_REAL_Q,
			   MI_TYPEID_STATE_Q,
			   MI_TYPEID_DISCRETE_Q,
			   MI_TYPEID_REAL_Q_TIMETAG,
			   MI_TYPEID_STATE_Q_TIMETAG,
			   MI_TYPEID_DISCRETE_Q_TIMETAG,
			   MI_TYPEID_REAL_EXTENDED,
			   MI_TYPEID_STATE_EXTENDED,
			   MI_TYPEID_DISCRETE_EXTENDED };


/* This struct will be passed to callback functions via the "usr" parameter */

typedef struct user_info 
  {
  /* Number Counts */
  ST_INT mitMaxDSTS;
  ST_INT numVccDevs;
  ST_INT numIccDevs;
  ST_INT clientNumDs;
  
  MVL_CFG_INFO *mvlCfg;

  } USER_INFO;


/************************************************************************/
/* 		       	   mitConfigure_xml    		 		*/
/*									*/
/* Called when the application is configuring an XML based ICCP-Lite	*/
/************************************************************************/

ST_RET  mitConfigure_xml (ST_CHAR *xmlFileName, MVL_CFG_INFO *mvlCfg)
  {
USER_INFO *usrStruct;

  usrStruct = (USER_INFO *) chk_calloc(1, sizeof(USER_INFO));

//  numLocalDataValue	 = 0;
//  usrStruct->mitMaxDSTS  = 0;
//  usrStruct->numVccDevs  = 0;
//  usrStruct->numIccDevs  = 0;
//  usrStruct->clientNumDs = 0;
  usrStruct->mvlCfg	 = mvlCfg;
  
  return mi_icfg_load (xmlFileName, MI_ICFG_MODE_INITIAL, NULL, usrStruct); 
  }

/************************************************************************/
/*                         Callback routines                            */
/************************************************************************/


/************************************************************************/
/* 		      u_miIcfgProcLocalControlCenter                    */
/************************************************************************/
#define DEF_MMS_PDU_SIZE	16000

ST_RET  u_miIcfgProcLocalControlCenter (ST_VOID *usr, ICFG_LOCAL_CC *icfgLocalCc)
  {
USER_INFO *usrStruct;

  usrStruct = (USER_INFO *) usr;
  usrStruct->mitMaxDSTS = icfgLocalCc->maxDsTs;

  if (icfgLocalCc->maxMmsPduSize>0)
    {
    usrStruct->mvlCfg->max_msg_size = icfgLocalCc->maxMmsPduSize;  
    }
  else
    {
    MIT_LOG_NERR1 ("MMS PDU size not set, defaulting to %d",DEF_MMS_PDU_SIZE);
    usrStruct->mvlCfg->max_msg_size = DEF_MMS_PDU_SIZE;
    }
  
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		       u_miIcfgProcLocalDv     				*/
/************************************************************************/

ST_RET u_miIcfgProcLocalDv (ST_VOID *usr, MIS_DV_REF misDvRef, 
			    ICFG_LOCAL_DV *localDv, ST_BOOLEAN newDv)
  {
MIT_DATA_VALUE *pstCurDataValue;          

  if(newDv == SD_TRUE)
    numLocalDataValue++;  
  
  /* Create a new data value */
  pstCurDataValue = (MIT_DATA_VALUE *) chk_calloc(1, sizeof(MIT_DATA_VALUE));
  
  /* And set its attributes */
  strcpy(pstCurDataValue->dvName, localDv->name);
  pstCurDataValue->dvType = miType[localDv->dvType];
  pstCurDataValue->source = localDv->normalSource;
  
  if(localDv->minPresent)
    {
    switch (pstCurDataValue->dvType)
      {
      case MI_TYPEID_REAL:
      case MI_TYPEID_REAL_Q:
      case MI_TYPEID_REAL_Q_TIMETAG:
      case MI_TYPEID_REAL_EXTENDED:
        pstCurDataValue->min.f = localDv->vals.realVals.valMin;
      break;
    	
      case MI_TYPEID_DISCRETE:
      case MI_TYPEID_DISCRETE_Q:
      case MI_TYPEID_DISCRETE_Q_TIMETAG:
      case MI_TYPEID_DISCRETE_EXTENDED:
        pstCurDataValue->min.d = localDv->vals.discreteVals.valMin;
      break;				
      
      default:
      break;	
      }
    }

  if(localDv->maxPresent)
    {
    switch (pstCurDataValue->dvType)
      {
      case MI_TYPEID_REAL:
      case MI_TYPEID_REAL_Q:
      case MI_TYPEID_REAL_Q_TIMETAG:
      case MI_TYPEID_REAL_EXTENDED:
        pstCurDataValue->max.f = localDv->vals.realVals.valMax;
      break;
    	
      case MI_TYPEID_DISCRETE:
      case MI_TYPEID_DISCRETE_Q:
      case MI_TYPEID_DISCRETE_Q_TIMETAG:
      case MI_TYPEID_DISCRETE_EXTENDED:
        pstCurDataValue->max.d = localDv->vals.discreteVals.valMax;
      break;

      default:
      break;	
      }
    }
  
  MIS_DV_REF_TO_DV(misDvRef)->access_info = pstCurDataValue;
  list_add_last((ST_VOID **)&pstDataValueSet, pstCurDataValue);
  mi_icfg_free (localDv);
  
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		         u_miIcfgProcLocalDev     			*/
/************************************************************************/

ST_RET u_miIcfgProcLocalDev (ST_VOID *usr, MIS_DEVICE_REF misDevRef, 
			     ICFG_LOCAL_DEV *localDev,  ST_BOOLEAN newDev)
  {
MIT_DEVICE *pstCurDevice;	       
        
  /* Create a new device */
  pstCurDevice = (MIT_DEVICE *)chk_calloc( 1, sizeof(MIT_DEVICE));
  
  /* And set its attributes */
  strcpy (pstCurDevice->devName, localDev->name);
  pstCurDevice->type 	    = localDev->devType;
  pstCurDevice->bSelectable = localDev->sbo;
  pstCurDevice->bTaggable   = localDev->tagEnable;
	
  if(pstCurDevice->bSelectable)
    {
    pstCurDevice->sboTimeOut = localDev->selTime;
    pstCurDevice->chkBackId  = (ST_INT16) localDev->checkBackId;
    }
  
  /* Add our device to the end of the device list */
  list_add_last((ST_VOID **)&pstDeviceSet, pstCurDevice);
  mi_icfg_free (localDev);
  
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		         u_miIcfgProcLocalIm     			*/
/************************************************************************/
ST_RET u_miIcfgProcLocalIm (ST_VOID *usr, ICFG_LOCAL_INFOMSG *localIm)  
  {
MIT_INFO_MSG *pstCurInfoMsg;      
  
  pstCurInfoMsg = (MIT_INFO_MSG *) chk_calloc(1, sizeof(MIT_INFO_MSG));
  
  pstCurInfoMsg->infoRef = localIm->infoRef;
  pstCurInfoMsg->maxSize = localIm->maxSize;
  
  list_add_last( (ST_VOID **)&pstInfoMsgSet, pstCurInfoMsg);
  
  mi_icfg_free (localIm);
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		    u_miIcfgProcRemoteControlCenter     		*/
/************************************************************************/

ST_RET u_miIcfgProcRemoteControlCenter (ST_VOID *usr, MI_REMOTE *mi_remote, 
					ICFG_REMOTE_CC *icfgRemoteCc)
  {
MIT_REMOTE  *pstCurRemote;                
MIT_MIS_VCC *pstCurMisVcc;             
MIT_MIC_VCC *pstCurMicVcc;             
USER_INFO   *usrStruct = (USER_INFO *) usr;
  	
  /* Create Remote */
  pstCurRemote = (MIT_REMOTE *)chk_calloc(1, sizeof(MIT_REMOTE));
  list_add_last( (ST_VOID **)&pstRemoteSet, pstCurRemote);
  
  /* store the mi_remote in our application wrapper */
  pstCurRemote->pstMiRemote = mi_remote;
  strcpy(pstCurRemote->remoteName, icfgRemoteCc->name);

  mi_remote->user_info[0] = pstCurRemote;
  
  /* Store our supported features */
  pstCurRemote->blocks = icfgRemoteCc->blt.blocks;
  
  /* Set up Server Bilateral Table */
  pstCurMisVcc = (MIT_MIS_VCC *)chk_calloc( 1, sizeof(MIT_MIS_VCC));
  strcpy(pstCurMisVcc->bilateralTableId, icfgRemoteCc->blt.id);
  strcpy(pstCurMisVcc->localDomainName, icfgRemoteCc->blt.localDom);
  strcpy(pstCurMisVcc->remoteName, icfgRemoteCc->name);
  pstCurMisVcc->minInterval = icfgRemoteCc->blt.shortestInterval;
  pstCurMisVcc->maxNumTsCtrl = usrStruct->mitMaxDSTS;

  list_add_last((ST_VOID **)&pstMisVccSet, pstCurMisVcc);

  /* Set up Client Bilateral Table */
  pstCurMicVcc = (MIT_MIC_VCC *)chk_calloc( 1, sizeof(MIT_MIC_VCC));
  strcpy(pstCurMicVcc->bilateralTableId, icfgRemoteCc->blt.id);
  strcpy(pstCurMicVcc->remoteDomainName, icfgRemoteCc->blt.remoteDom);
  strcpy(pstCurMicVcc->remoteName, icfgRemoteCc->name);
  
  list_add_last((ST_VOID **)&pstMicVccSet, pstCurMicVcc);
  
  return SD_SUCCESS;
  }

/************************************************************************/
/* 		      	   u_miIcfgProcAssociation    	 		*/
/************************************************************************/

ST_RET u_miIcfgProcAssociation (ST_VOID *usr, MI_ASSOC_CTRL *mi_assoc, 
			        ICFG_ASSOCIATION *assoc)  
  {
MI_REMOTE      *pstRemote;
MIT_ASSOC_INFO *pstCurAssocInfo;          
MIT_AR_NAME    *pstCurARName;
MIT_MIC_VCC    *pstMitMicVcc;
MIT_MIS_VCC    *pstCurMisVcc;             
MIT_REMOTE     *pstMitRemote;
MIT_REMOTE     *pstCurRemote;                
ST_INT 	       j;
	
USER_INFO   *usrStruct = (USER_INFO *) usr;
  
  
  /* Grab Remote */
/*  pstCurRemote = (MIT_REMOTE *) list_find_last((struct dbl_lnk *)pstRemoteSet);
modified 2006.11.24 zy*/  
  pstCurRemote = (MIT_REMOTE *) list_find_last((struct dbl_lnk *)pstRemoteSet);
  /* Create the Link */
  pstCurAssocInfo = (MIT_ASSOC_INFO *)chk_calloc (1, sizeof(MIT_ASSOC_INFO));
  list_add_last ((ST_VOID **)&pstAssocInfoSet, pstCurAssocInfo);
  mi_assoc->user_info[0] = (ST_VOID *)pstCurAssocInfo;
  
  /* And set its attributes */
  strcpy(pstCurAssocInfo->associationName, assoc->name);
  strcpy(pstCurAssocInfo->localArName, assoc->localAr);
  strcpy(pstCurAssocInfo->remoteName, pstCurRemote->remoteName);
  
  pstCurAssocInfo->numArNames = assoc->totalRemoteArNames;
  
  /* Add all of our Remote ARs */
  for (j=0; j < assoc->totalRemoteArNames; j++)
    {
    pstCurARName = (MIT_AR_NAME *)chk_calloc (1, sizeof(MIT_AR_NAME));
    strcpy (pstCurARName->arName, assoc->remoteAr[j]);
    list_add_last ((ST_VOID **)&pstCurAssocInfo->arList, pstCurARName);
    }
  
  /* Set our control flags and misc timers */
  if(assoc->connectRole & ICFG_ROLE_CALLED)
    {
    usrStruct->mvlCfg->num_called++;
    pstCurAssocInfo->ctrlFlags |= MI_ASSOC_CTRL_CALLED;
    }
  if(assoc->connectRole & ICFG_ROLE_CALLING)
    {
    usrStruct->mvlCfg->num_calling++;
    pstCurAssocInfo->ctrlFlags |= MI_ASSOC_CTRL_CALLING;
    }
  if(assoc->serviceRole & ICFG_ROLE_CLIENT)  pstCurAssocInfo->ctrlFlags |= MI_ASSOC_CTRL_CLIENT;
  if(assoc->serviceRole & ICFG_ROLE_SERVER)  pstCurAssocInfo->ctrlFlags |= MI_ASSOC_CTRL_SERVER;
 
  pstCurAssocInfo->connectBackOffTime = assoc->retryTime;
  pstCurAssocInfo->maxSegsize 	      = assoc->maxMmsPduSize;
  pstCurAssocInfo->maxReqCalling      = assoc->maxReqPend;   
  pstCurAssocInfo->maxReqCalled       = assoc->maxIndPend;   
  pstCurAssocInfo->maxNest 	      = assoc->maxNest;      
  
  pstCurMisVcc = (MIT_MIS_VCC *) list_find_last((struct dbl_lnk *)pstMisVccSet);
  strcpy(pstCurMisVcc->localArName, assoc->localAr);

  /* Add the assoc to the Remote */
  pstMitRemote = findMitRemote (pstCurAssocInfo->remoteName);
  
  if(pstMitRemote)
    {
    /* look the thing up and set up the cross reference between the */
    /* MI_ASSOC_CTRL and the MIT_ASSOC_INFO.     		    */

    pstRemote = pstMitRemote->pstMiRemote;
    pstCurAssocInfo->pstAssocCtrl = mi_assoc;
    pstMitMicVcc = findMitMicVcc (pstCurAssocInfo->remoteName);
    
    if (pstMitMicVcc!=NULL)
      {
      pstCurAssocInfo->pstMitMicVcc = pstMitMicVcc;
      }

    pstCurARName = pstCurAssocInfo->arList;
    
    if((pstCurARName==NULL) && (pstCurAssocInfo->ctrlFlags & MI_ASSOC_CTRL_CALLING))
      {
      MIT_LOG_ERR1 ("Calling Association '%s' not configured with RemoteAR's, contents invalid!",
         			 pstCurAssocInfo->associationName);
      return SD_FAILURE;
      }

    }

  else
    {
    MIT_LOG_ERR2 ("Remote '%s' not found for Association '%s'. No Association setup, contents invalid!",
                  pstCurAssocInfo->remoteName, pstCurAssocInfo->associationName); 
    
    return SD_FAILURE;
    }
  mi_icfg_free (assoc);
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		       u_miIcfgProcServerDv    		 		*/
/************************************************************************/

ST_RET u_miIcfgProcServerDv  (ST_VOID *usr, MIS_VCC *mis_vcc, MIS_DV_REF dv_ref, 
			      ICFG_SERVER_DV *serverDv)  
  {
MIT_REMOTE	*pstMitRemote;
MIT_MIS_VCC     *pstCurMisVcc;             
MIT_DATA_VALUE 	*pstMitDataValue;

  pstCurMisVcc = (MIT_MIS_VCC *) list_find_last((struct dbl_lnk *)pstMisVccSet);
  pstMitRemote = findMitRemote (pstCurMisVcc->remoteName);
  
  if (!pstMitRemote)
    {
    MIT_LOG_ERR2 ("Remote name '%s' not found for MIS VCC '%s'",
                  pstCurMisVcc->remoteName,
                  pstCurMisVcc->vccName);
    return SD_FAILURE;
    }

  if(!pstCurMisVcc->pstMisVcc)
    {
    pstCurMisVcc->maxNumVccDvRef = mis_vcc->max_vcc_dv_ref;
    pstCurMisVcc->maxNumVccDs    = mis_vcc->max_vcc_ds;
    pstCurMisVcc->maxNumIccDvRef = mis_vcc->max_icc_dv_ref;
    pstCurMisVcc->maxNumIccDs    = mis_vcc->max_icc_ds;
    pstCurMisVcc->pstMisVcc 	 = mis_vcc;
    }
    
  pstMitDataValue = (MIT_DATA_VALUE *) (MIS_DV_REF_TO_DV(dv_ref)->access_info);
  pstMitDataValue->dvRef = dv_ref;
  mi_icfg_free (serverDv);
  
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		    	   u_miIcfgProcServerDev    	 		*/
/************************************************************************/

ST_RET u_miIcfgProcServerDev (ST_VOID *usr, MIS_VCC *mis_vcc, MIS_DEVICE_REF dev_ref, 
			      ICFG_SERVER_DEV *serverDev)  
  {
MIT_DEVICE_REF  *pstCurMisVccDevice;       
MIT_MIS_VCC     *pstCurMisVcc;             
USER_INFO   	*usrStruct = (USER_INFO *) usr;
  
  pstCurMisVccDevice = (MIT_DEVICE_REF *)chk_calloc (1, sizeof(MIT_DEVICE_REF));
  
  pstCurMisVcc = (MIT_MIS_VCC *) list_find_last((struct dbl_lnk *)pstMisVccSet);
  
  list_add_last ((ST_VOID **)&pstCurMisVcc->pstDeviceSet, pstCurMisVccDevice);
  strcpy (pstCurMisVccDevice->devName, serverDev->name);
  pstCurMisVccDevice->scope	= serverDev->scope;
  pstCurMisVccDevice->type 	= serverDev->devType;
  pstCurMisVccDevice->bOperable = SD_TRUE;
  pstCurMisVccDevice->bTaggable = MIS_DEV_REF_TO_DEV(dev_ref)->taggable;
  MIS_DEV_REF_TO_DEV(dev_ref)->access_info = pstCurMisVccDevice;
    
  (serverDev->scope == 1) ? usrStruct->numIccDevs++ : usrStruct->numVccDevs++;
  mi_icfg_free (serverDev);
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		    	 u_miIcfgProcServerIM    	 		*/
/************************************************************************/

ST_RET u_miIcfgProcServerIM  (ST_VOID *usr, MIS_VCC *mis_vcc, 
			      ICFG_SERVER_INFOMSG *serverIm)  
  {
MIT_INFO_MSG *pstCurMisVccInfoMsg;      
MIT_INFO_MSG *pstCurLocalInfoMsg;      
MIT_MIS_VCC  *pstCurMisVcc;             
  
  pstCurMisVcc = (MIT_MIS_VCC *) list_find_last((struct dbl_lnk *)pstMisVccSet);
  
  pstCurMisVccInfoMsg = (MIT_INFO_MSG *)chk_calloc( 1, sizeof(MIT_INFO_MSG));
  list_add_last((ST_VOID **)&(pstCurMisVcc->pstInfoMsgs),pstCurMisVccInfoMsg);
  
  pstCurLocalInfoMsg = pstInfoMsgSet;
  while (pstCurLocalInfoMsg)
    {
    if(pstCurLocalInfoMsg->infoRef == serverIm->infoRef)
      {
      pstCurMisVccInfoMsg->infoRef  = serverIm->infoRef;
      pstCurMisVccInfoMsg->msgScope = serverIm->scope;
      pstCurMisVccInfoMsg->maxSize  = pstCurLocalInfoMsg->maxSize;
      break;
      }
    pstCurLocalInfoMsg = (MIT_INFO_MSG *)list_get_next (pstInfoMsgSet, pstCurLocalInfoMsg);
    } 
  
  mi_icfg_free (serverIm);
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		       	u_miIcfgProcServerDs    	 		*/
/************************************************************************/

ST_RET u_miIcfgProcServerDs  (ST_VOID *usr, MIS_VCC *mis_vcc, MIS_DS *mis_ds, 
			      ICFG_SERVER_DS *serverDs)  
  {
MIT_DATA_POINT 	*pstCurDataPoint; 	       
MIT_DATA_SET    *pstCurMisVccDataSet;
MIT_MIS_VCC  	*pstCurMisVcc;             
ST_INT		newNumDv;
ST_INT		i;

  newNumDv = 0;

  pstCurMisVcc = (MIT_MIS_VCC *) list_find_last((struct dbl_lnk *)pstMisVccSet);
  
  pstCurMisVccDataSet = (MIT_DATA_SET *)chk_calloc (1, sizeof(MIT_DATA_SET));
  list_add_last ((ST_VOID **)&pstCurMisVcc->pstDataSets, pstCurMisVccDataSet);
  
  if(serverDs->transferSetName == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMisVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "Transfer_Set_Name");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  if(serverDs->transferSetTimeStamp == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMisVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "Transfer_Set_Time_Stamp");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  if(serverDs->dsConditionsDetected == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMisVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "DSConditions_Detected");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  if(serverDs->eventCodeDetected == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMisVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "Event_Code_Detected");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  strcpy (pstCurMisVccDataSet->dsName, serverDs->name);
  pstCurMisVccDataSet->dsScope   = serverDs->scope;
  pstCurMisVccDataSet->numPoints = serverDs->numDv + newNumDv;
  
  for (i=0;  i < serverDs->numDv;  i++)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    list_add_last ((ST_VOID **)&pstCurMisVccDataSet->pointList, pstCurDataPoint);
    strcpy (pstCurDataPoint->pointName, serverDs->dvArray[i].name);
    pstCurDataPoint->scope = serverDs->dvArray[i].scope;
    }
  
  pstCurMisVccDataSet->pstDS = (ST_VOID *)mis_ds; 
  mi_icfg_free (serverDs);

  return SD_SUCCESS;
  }


/************************************************************************/
/* 		       u_miIcfgProcClientDv  	  	 		*/
/************************************************************************/

ST_RET u_miIcfgProcClientDv  (ST_VOID *usr, MIC_VCC *mic_vcc, MIC_DV *micDv, 
			      ICFG_CLIENT_DV *clientDv)  
  {
MI_REMOTE    *pstRemote;
MIT_MIC_VCC  *pstCurMicVcc;             
USER_INFO    *usrStruct = (USER_INFO *) usr;
MIT_DVREF_INFO *pstCurMitDvRefInfo;       
  
  pstCurMicVcc = (MIT_MIC_VCC *) list_find_last((struct dbl_lnk *)pstMicVccSet);
  
  pstRemote = findRemote (pstCurMicVcc->remoteName);
  if(!pstRemote)
    {
    MIT_LOG_ERR1("Could not find remote %s, check configuration", pstCurMicVcc->remoteName);
    return SD_FAILURE;
    }
  
  pstCurMicVcc->pstMicVcc = mic_vcc;  
  pstCurMicVcc->numDvRefs = (mic_vcc->num_vcc_dv + mic_vcc->num_icc_dv + 3);
  
  pstCurMitDvRefInfo = (MIT_DVREF_INFO*)chk_calloc( 1, sizeof(MIT_DVREF_INFO));
  list_add_last( (ST_VOID **)&pstCurMicVcc->pstDvRefSet, pstCurMitDvRefInfo);
  
  pstCurMitDvRefInfo->dvName = chk_strdup (clientDv->name);
  pstCurMitDvRefInfo->type   = clientDv->dvType;
  pstCurMitDvRefInfo->scope  = clientDv->scope;
  
  /* We're going to use the mapInfo passed from the XML as our DV Tag */
  pstCurMitDvRefInfo->tag    = chk_strdup (clientDv->mapInfo);
  mi_icfg_free (clientDv);
  
  
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		       	  u_miIcfgProcClientDev    	 		*/
/************************************************************************/

ST_RET u_miIcfgProcClientDev (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      MIC_DEVICE *micDev, ICFG_CLIENT_DEV *clientDev)  
  {
MIT_DEVICE_REF *pstCurMicVccDevice;       
MIT_MIC_VCC    *pstCurMicVcc;             
USER_INFO      *usrStruct = (USER_INFO *) usr;
  
  pstCurMicVcc = (MIT_MIC_VCC *) list_find_last((struct dbl_lnk *)pstMicVccSet);
  
  pstCurMicVccDevice = (MIT_DEVICE_REF *)chk_calloc (1, sizeof(MIT_DEVICE_REF));
  list_add_last ((ST_VOID **)&pstCurMicVcc->pstDeviceSet, pstCurMicVccDevice);
  
  pstCurMicVccDevice->bSelectable = (clientDev->sbo == 1) ? SD_TRUE : SD_FALSE;
  pstCurMicVccDevice->bTaggable = (clientDev->tagEnable == 1) ? SD_TRUE : SD_FALSE;
  
  strcpy (pstCurMicVccDevice->devName, clientDev->name);

  pstCurMicVccDevice->scope 	= clientDev->scope;
  pstCurMicVccDevice->type  	= clientDev->devType;
  pstCurMicVccDevice->bOperable = SD_TRUE;

  (clientDev->scope == 1) ? usrStruct->numIccDevs++ : usrStruct->numVccDevs++;
  pstCurMicVccDevice->pstDevice = micDev;
  mi_icfg_free (clientDev);
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		        u_miIcfgProcClientIM    	 		*/
/************************************************************************/

ST_RET u_miIcfgProcClientIM  (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      ICFG_CLIENT_INFOMSG *clientIm)  
  {
  mi_icfg_free (clientIm);
  return SD_SUCCESS;
  }


/************************************************************************/
/* 		        u_miIcfgProcClientDs    	 		*/
/************************************************************************/

ST_RET u_miIcfgProcClientDs  (ST_VOID *usr, MIC_VCC *mic_vcc, 
			      ICFG_CLIENT_DS *clientDs)  
  {
MIT_DATA_SET   *pstCurMicVccDataSet;      
MIT_DATA_POINT *pstCurDataPoint; 	       
MIT_MIC_VCC    *pstCurMicVcc;             
USER_INFO      *usrStruct = (USER_INFO *) usr;
ST_INT	       newNumDv;
ST_INT 	       i;
  
  newNumDv = 0;
  usrStruct->clientNumDs++;
  
  pstCurMicVcc = (MIT_MIC_VCC *) list_find_last((struct dbl_lnk *)pstMicVccSet);
  pstCurMicVcc->numDataSets = usrStruct->clientNumDs;
  
  pstCurMicVccDataSet = (MIT_DATA_SET *)chk_calloc (1, sizeof(MIT_DATA_SET));
  
  strcpy(pstCurMicVccDataSet->dsName, clientDs->name);
  pstCurMicVccDataSet->dsScope 	 = ICC_SPEC;
  pstCurMicVccDataSet->numPoints = clientDs->numDv;

  /* Remember the pointer to the first DataSet to start */
  if (pstCurMicVcc->pstDataSets==NULL)
    pstCurMicVcc->pstNextDataSetToStart = pstCurMicVccDataSet;
  
  list_add_last ((ST_VOID **)&pstCurMicVcc->pstDataSets, pstCurMicVccDataSet);

  if(clientDs->transferSetName == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMicVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "Transfer_Set_Name");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  if(clientDs->transferSetTimeStamp == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMicVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "Transfer_Set_Time_Stamp");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  if(clientDs->dsConditionsDetected == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMicVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "DSConditions_Detected");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  if(clientDs->eventCodeDetected == SD_TRUE)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMicVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, "Event_Code_Detected");
    pstCurDataPoint->scope = ICC_SPEC;	     
    newNumDv++;
    }
  
  for (i=0; i < clientDs->numDv; i++)
    {
    pstCurDataPoint = (struct Tag_MIT_DATA_POINT *)chk_calloc (1, sizeof(MIT_DATA_POINT));
    
    list_add_last ((ST_VOID **)&pstCurMicVccDataSet->pointList,
                   pstCurDataPoint);
    
    strcpy (pstCurDataPoint->pointName, clientDs->dvArray[i].name);
    pstCurDataPoint->scope = clientDs->dvArray[i].scope;	     
    }
  
  pstCurMicVccDataSet->numPoints += newNumDv;
  
  mi_icfg_free (clientDs);
  return SD_SUCCESS;
  }

/************************************************************************/
/* 		      	u_miIcfgProcClientDsTs    	 		*/
/************************************************************************/

ST_RET  u_miIcfgProcClientDsTs (ST_VOID *usr, MIC_VCC *mic_vcc,
			        ICFG_CLIENT_DSTS *clientDsTs) 
  {
MIT_MIC_VCC  	 *pstCurMicVcc;             
MIT_TRANSFER_SET *pstCurMicVccTransferSet;  
USER_INFO  	 *usrStruct = (USER_INFO *) usr;
  
  pstCurMicVcc = (MIT_MIC_VCC *) list_find_last((struct dbl_lnk *)pstMicVccSet);
  
  pstCurMicVccTransferSet = (MIT_TRANSFER_SET *)chk_calloc (1, sizeof(MIT_TRANSFER_SET));

  /* Remember the pointer to the first DataSet to start */
  if (pstCurMicVcc->pstTransferSets == NULL)
    pstCurMicVcc->pstNextTransferSetToStart = pstCurMicVccTransferSet;
    
  strcpy (pstCurMicVccTransferSet->dsTsName,clientDsTs->dsName);
  strcpy (pstCurMicVccTransferSet->dataSetName,clientDsTs->dsName);
  pstCurMicVccTransferSet->startTime  	= clientDsTs->startTime;
  pstCurMicVccTransferSet->interval   	= clientDsTs->interval;
  pstCurMicVccTransferSet->tLE 	    	= 0;
  pstCurMicVccTransferSet->bufferTime		= clientDsTs->bufferTime;
  pstCurMicVccTransferSet->integrityCheck	= clientDsTs->integrity;
  pstCurMicVccTransferSet->status		= 1;
  pstCurMicVccTransferSet->critical		= clientDsTs->critical;
  pstCurMicVccTransferSet->dSConditionsRequested = clientDsTs->dsConditionsRequested;
  pstCurMicVccTransferSet->rBE = (clientDsTs->rbe == 1) ? SD_TRUE : SD_FALSE;
  
  list_add_last ((ST_VOID **)&pstCurMicVcc->pstTransferSets,
                   pstCurMicVccTransferSet);
  pstCurMicVcc->numTransferSets++;
  mi_icfg_free (clientDsTs);
  
  return SD_SUCCESS;
  }

/************************************************************************/
/* 		       	   mitUnconfigure_xml    	 		*/
/*									*/
/* Called when the application is shutting down. For cleanliness /    	*/
/* completeness only.							*/
/************************************************************************/

ST_VOID  mitUnconfigure_xml ()
  {
MIT_DATA_VALUE       	*pstCurDataValue;          
MIT_DATA_POINT 	        *pstCurDataPoint; 	       
MIT_INFO_MSG 		*pstCurInfoMsg;      
MIT_DATA_SET         	*pstCurMisVccDataSet;      
MIT_DATA_SET         	*pstCurMicVccDataSet;      
MIT_REMOTE           	*pstCurRemote;                
MIT_MIS_VCC          	*pstCurMisVcc;             
MIT_MIC_VCC          	*pstCurMicVcc;             
MIT_DEVICE           	*pstCurDevice;	       
MIT_DEVICE_REF       	*pstCurMisVccDevice;       
MIT_DEVICE_REF       	*pstCurMicVccDevice;       
MIT_ASSOC_INFO       	*pstCurAssocInfo;          
MIT_INFO_MSG         	*pstCurMisVccInfoMsg;      
MIT_DVREF_INFO       	*pstCurMitDvRefInfo;       
MIT_TRANSFER_SET     	*pstCurMicVccTransferSet;  

MIT_DATA_VALUE 		*pstCurDataValueSav;
MIT_INFO_MSG	    	*pstCurInfoMsgSav;
MIT_REMOTE     		*pstCurRemoteSav;
MIT_MIS_VCC    		*pstCurMisVccSav;
MIT_MIC_VCC    		*pstCurMicVccSav;
MIT_DEVICE_REF 		*pstCurMicVccDeviceSav;
MIT_DEVICE_REF		*pstCurMisVccDeviceSav;
MIT_DEVICE 		*pstCurDeviceSav;
MIT_TRANSFER_SET     	*pstCurMicVccTransferSetSav;
MIT_INFO_MSG   		*pstCurMisVccInfoMsgSav;
MIT_ASSOC_INFO 		*pstCurAssocInfoSav;
MIT_AR_NAME    		*pstCurArNameSav;
MIT_AR_NAME    		*pstCurArName;
MIT_DATA_SET   		*pstCurMicVccDataSetSav;
MIT_DATA_SET  	        *pstCurMisVccDataSetSav;
MIT_DATA_POINT 		*pstCurDataPointSav;
MIT_DVREF_INFO 		*pstCurMitDvRefInfoSav;
ST_INT	       		i;
  
  /* get rid of the data values */
  sprintf (infobuf,"\n Deallocating DataValues");
#ifdef DEBUG
  printf("%s",infobuf);
#endif
  WriteDspInfo(infobuf);

  pstCurDataValue = pstDataValueSet;
  i = 0;
  while (pstCurDataValue)
    {
    pstCurDataValueSav = pstCurDataValue;
    pstCurDataValue = (MIT_DATA_VALUE *)list_get_next 
                          (pstDataValueSet, pstCurDataValue);
    list_unlink(&pstDataValueSet,pstCurDataValueSav);
    chk_free(pstCurDataValueSav);
    i++;
    if (i==1000)
      {
      printf(".");
      i=0;
      }
    }
  
  /* free up Local Device references */
  sprintf(infobuf,"\n Deallocating Devices");
#ifdef DEBUG
  printf("%s",infobuf);
#endif
  WriteDspInfo(infobuf);

  pstCurDevice = pstDeviceSet;
  while (pstCurDevice)
    {
    pstCurDeviceSav = pstCurDevice;
    pstCurDevice = (MIT_DEVICE *)list_get_next (pstDeviceSet, pstCurDevice);
                           
    list_unlink(&pstDeviceSet, pstCurDeviceSav);
    chk_free(pstCurDeviceSav);
    } 
  
  /* free up Local Info Msg references */
  sprintf(infobuf,"\n Deallocating Info Messages");
#ifdef DEBUG
  printf("%s",infobuf);
#endif
  WriteDspInfo(infobuf);
  pstCurInfoMsg = pstInfoMsgSet;
  while (pstCurInfoMsg)
    {
    pstCurInfoMsgSav = pstCurInfoMsg;
    pstCurInfoMsg = (MIT_INFO_MSG *)list_get_next (pstInfoMsgSet, pstCurInfoMsg);
                           
    list_unlink(&pstInfoMsgSet, pstCurInfoMsgSav);
    chk_free(pstCurInfoMsgSav);
    } 
  
  /* get rid of the remotes */
  sprintf (infobuf,"\n Deallocating Remotes");
#ifdef DEBUG
  printf("%s",infobuf);
#endif
  WriteDspInfo(infobuf);
  pstCurRemote = pstRemoteSet;
  while (pstCurRemote)
    {
    pstCurRemoteSav = pstCurRemote;
    pstCurRemote = (MIT_REMOTE *)list_get_next 
                          (pstRemoteSet, pstCurRemote);
    list_unlink(&pstRemoteSet,pstCurRemoteSav);
	mi_icfg_free_remote_cc (pstCurRemoteSav->pstMiRemote);
    chk_free(pstCurRemoteSav);
	}

  /* get rid of the Server VitualContolCenters */
  sprintf (infobuf,"\n Deallocating ServerVCCs");
  #ifdef DEBUG
  printf("%s",infobuf);
#endif
  WriteDspInfo(infobuf);
pstCurMisVcc = pstMisVccSet;
  while (pstCurMisVcc)
    {
    pstCurMisVccSav = pstCurMisVcc;
    pstCurMisVcc = (MIT_MIS_VCC *)list_get_next 
                          (pstMisVccSet, pstCurMisVcc);
    list_unlink(&pstMisVccSet,pstCurMisVccSav);
    
    /* free up Server Device references */
    sprintf(infobuf,"\n Deallocating ServerVCC Devices");
#ifdef DEBUG
    printf("%s",infobuf);
#endif
    WriteDspInfo(infobuf);
    pstCurMisVccDevice = pstCurMisVccSav->pstDeviceSet;
    while (pstCurMisVccDevice)
      {
      pstCurMisVccDeviceSav = pstCurMisVccDevice;
      pstCurMisVccDevice = (MIT_DEVICE_REF *)list_get_next 
                           (pstCurMisVccSav->pstDeviceSet, pstCurMisVccDevice);
      list_unlink(&(pstCurMisVccSav->pstDeviceSet), pstCurMisVccDeviceSav);
      chk_free(pstCurMisVccDeviceSav);
      }
    
    /* free up Server DataSet references */
    sprintf(infobuf,"\n Deallocating ServerVCC DataSets");
#ifdef DEBUG
    printf("%s",infobuf);
#endif
    WriteDspInfo(infobuf);
    pstCurMisVccDataSet = pstCurMisVccSav->pstDataSets;
    while (pstCurMisVccDataSet)
      {
      pstCurMisVccDataSetSav = pstCurMisVccDataSet;
      pstCurMisVccDataSet = (MIT_DATA_SET *)list_get_next 
                          (pstCurMisVccSav->pstDataSets, pstCurMisVccDataSet);
      list_unlink(&(pstCurMisVccSav->pstDataSets),pstCurMisVccDataSetSav);
      /* free up the data points */
      printf ("\n Deallocating ServerVCC DataSet PointLists");
      pstCurDataPoint = pstCurMisVccDataSetSav->pointList;
      while (pstCurDataPoint)
        {
        pstCurDataPointSav = pstCurDataPoint;
        pstCurDataPoint = (MIT_DATA_POINT *)list_get_next 
                          (pstCurMisVccDataSetSav->pointList, pstCurDataPoint);
        list_unlink(&(pstCurMisVccDataSetSav->pointList),
                      pstCurDataPointSav);
        chk_free(pstCurDataPointSav);
	}

      chk_free(pstCurMisVccDataSetSav);
      }
    
    /* free up Server InfoRefs */
    sprintf(infobuf,"\n Deallocating ServerVCC Information References");
#ifdef DEBUG
    printf("%s",infobuf);
#endif
    WriteDspInfo(infobuf);
    pstCurMisVccInfoMsg = pstCurMisVccSav->pstInfoMsgs;
    while(pstCurMisVccInfoMsg)
      {
      pstCurMisVccInfoMsgSav = pstCurMisVccInfoMsg;
      pstCurMisVccInfoMsg = (MIT_INFO_MSG *)list_get_next
                          (pstCurMisVccSav->pstInfoMsgs, pstCurMisVccInfoMsg);
      list_unlink(&(pstCurMisVccSav->pstInfoMsgs), pstCurMisVccInfoMsgSav);
      chk_free(pstCurMisVccInfoMsgSav);
      }    
    
    chk_free(pstCurMisVccSav);
    }
  
  /* get rid of the ClientVirtualControlCenters */
  sprintf (infobuf,"\n Deallocating ClientVCCs");
#ifdef DEBUG
  printf("%s",infobuf);
#endif
  WriteDspInfo(infobuf);
  pstCurMicVcc = pstMicVccSet;
  while (pstCurMicVcc)
    {
    pstCurMicVccSav = pstCurMicVcc;
    pstCurMicVcc = (MIT_MIC_VCC *)list_get_next 
                          (pstMicVccSet, pstCurMicVcc);
    list_unlink(&pstMicVccSet,pstCurMicVccSav);

    /* free up the DataValue References */
    sprintf (infobuf,"\n Deallocating ClientVCC DataValue References");
#ifdef DEBUG
    printf("%s",infobuf);
#endif
    WriteDspInfo(infobuf);
    pstCurMitDvRefInfo = pstCurMicVccSav->pstDvRefSet;
    while (pstCurMitDvRefInfo)
      {
      pstCurMitDvRefInfoSav = pstCurMitDvRefInfo;
      pstCurMitDvRefInfo = (MIT_DVREF_INFO *)list_get_next 
                            (pstCurMicVccSav->pstDvRefSet, pstCurMitDvRefInfo);
      list_unlink(&(pstCurMicVccSav->pstDvRefSet),pstCurMitDvRefInfoSav );
      chk_free(pstCurMitDvRefInfoSav->tag);
      chk_free(pstCurMitDvRefInfoSav->dvName);
      chk_free(pstCurMitDvRefInfoSav);
      }
	
    /* free up Client Device references */
    sprintf(infobuf,"\n Deallocating ClientVCC Devices");
#ifdef DEBUG
    printf("%s",infobuf);
#endif
    WriteDspInfo(infobuf);
    pstCurMicVccDevice = pstCurMicVccSav->pstDeviceSet;
    while (pstCurMicVccDevice)
      {
      pstCurMicVccDeviceSav = pstCurMicVccDevice;
      pstCurMicVccDevice = (MIT_DEVICE_REF *)list_get_next 
                           (pstCurMicVccSav->pstDeviceSet, pstCurMicVccDevice);
      list_unlink(&(pstCurMicVccSav->pstDeviceSet), pstCurMicVccDeviceSav);
      chk_free(pstCurMicVccDeviceSav->pstDevice);
      chk_free(pstCurMicVccDeviceSav);
      }
    
    /* free up the datasets and data points associated */
    sprintf (infobuf,"\n Deallocating ClientVCC DataSets");
#ifdef DEBUG
    printf("%s",infobuf);
#endif
    WriteDspInfo(infobuf);
    pstCurMicVccDataSet = pstCurMicVccSav->pstDataSets;
    while (pstCurMicVccDataSet)
      {
      pstCurMicVccDataSetSav = pstCurMicVccDataSet;
      pstCurMicVccDataSet = (MIT_DATA_SET *)list_get_next 
                          (pstCurMicVccSav->pstDataSets, pstCurMicVccDataSet);
      list_unlink(&(pstCurMicVccSav->pstDataSets),pstCurMicVccDataSetSav);
      /* free up the data points */
      printf ("\n Deallocating ClientVCC DataSet PointLists");
      pstCurDataPoint = pstCurMicVccDataSetSav->pointList;
      while (pstCurDataPoint)
        {
        pstCurDataPointSav = pstCurDataPoint;
        pstCurDataPoint = (MIT_DATA_POINT *)list_get_next 
                          (pstCurMicVccDataSetSav->pointList, pstCurDataPoint);
        list_unlink(&(pstCurMicVccDataSetSav->pointList),
                      pstCurDataPointSav);
        chk_free(pstCurDataPointSav);
	}

      chk_free(pstCurMicVccDataSetSav);
      }

    /* free up Client DSTSs */
    sprintf(infobuf,"\n Deallocating Client DSTSs");
#ifdef DEBUG
    printf("%s",infobuf);
#endif
    WriteDspInfo(infobuf);
    pstCurMicVccTransferSet = pstCurMicVccSav->pstTransferSets;
    while(pstCurMicVccTransferSet)
      {
      pstCurMicVccTransferSetSav = pstCurMicVccTransferSet;
      pstCurMicVccTransferSet = (MIT_TRANSFER_SET *)list_get_next
				    (pstCurMicVccSav->pstTransferSets, pstCurMicVccTransferSet);
      list_unlink(&(pstCurMicVccSav->pstTransferSets), pstCurMicVccTransferSetSav);
      chk_free(pstCurMicVccTransferSetSav);
      } 
    chk_free(pstCurMicVccSav);
    }

  /* get rid of the AssociateInfo List */
  sprintf (infobuf,"\n Deallocating Association Info");
#ifdef DEBUG
  printf("%s",infobuf);
#endif
  WriteDspInfo(infobuf);
  pstCurAssocInfo = pstAssocInfoSet;
  while (pstCurAssocInfo)
    {
    pstCurAssocInfoSav = pstCurAssocInfo;
    pstCurAssocInfo = (MIT_ASSOC_INFO *)list_get_next 
                          (pstAssocInfoSet, pstCurAssocInfo);
    list_unlink(&pstAssocInfoSet,pstCurAssocInfoSav);
    /* for each AssociateInfo there is a list of AR Names to */
    /* deallocate                                            */
    pstCurArName = pstCurAssocInfoSav->arList;
    while (pstCurArName)
      {
      pstCurArNameSav = pstCurArName;
      pstCurArName = (MIT_AR_NAME *)list_get_next 
                          (pstCurAssocInfoSav->arList, 
                            pstCurArName);
      list_unlink (&(pstCurAssocInfoSav->arList), pstCurArNameSav);
      chk_free(pstCurArNameSav);
      }


    chk_free(pstCurAssocInfoSav);
    }
  mi_icfg_free_local_cc ();
  }

