/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_devc.c                                            	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Device control functions					*/
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 06/20/02  rkr    6      Copyright year update                        */
/* 06/18/02  rkr    5      Changes for tracking outstanding MMS Reqs	*/
/* 11/21/01  ASK    4      more cleanup for mi lib changes		*/
/* 06/07/01  ASK    3	   misc cleanup with devices                    */
/* 08/30/00  RKR    2      changed printf format in u_mis_device_operate*/
/* 04/19/00  RKR    1      Created                                      */
/************************************************************************/

#include "mit_main.h"
#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"

/************************************************************************/
/* static function prototypes						*/
/************************************************************************/

static ST_VOID showDeviceResult (ST_RET mic_result);
static MIC_DEVICE *getMicDevice (MIT_DEVICE_REF *DeviceSet);
static ST_VOID *findDevice (MIT_DEVICE_REF *pstDeviceSet, ST_CHAR *devName);

/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/

static char *thisFileName = __FILE__;

/************************************************************************/
/*     		               doSelectDone 				*/
/************************************************************************/

static ST_VOID doSelectDone  (MI_REQ_CTRL *miReq)
  {
MIC_DEVICE *pDevice;
MIT_ASSOC_INFO *pstMitAssoc;

  pDevice = miReq->mic_device;
/*  pstMitAssoc = (Tag_MIT_ASSOC_INFO *)miReq->user_info[0];
modified 2006.11.24 zy*/
  pstMitAssoc = (struct Tag_MIT_ASSOC_INFO *)miReq->user_info[0];
  incReqCapacity(pstMitAssoc);
  if (miReq->result != MIC_RESULT_SUCCESS)
    {
    printf ("\n Error in DeviceSelect:  ");
    showDeviceResult (miReq->result);
    MIT_LOG_ERR2 ("DeviceSelect Resp error %d for device %s", miReq->result, 
	          pDevice->device_name);
    return;
    }

  printf ("DeviceSelect Resp Device %s, checkback %d", 
           pDevice->device_name, miReq->o.check_back_id);
  MIT_LOG_CCNF2 ("DeviceSelect Resp Device %s, checkback %d", 
                pDevice->device_name, miReq->o.check_back_id);
  mi_free_req_ctrl (miReq);
  }

/************************************************************************/
/* 		          selectDevice			*/
/************************************************************************/

ST_VOID    selectDevice (ST_VOID)
  {
MIT_MIC_VCC *pstMitMicVcc;
MI_REQ_CTRL *req;
MIC_DEVICE  *pDevice;
MIT_ASSOC_INFO *pstAssociationInfo;
ST_CHAR buffer[MAX_IDENT_LEN +1];

  /* Get the association name and see if it's active			*/

  pstAssociationInfo = getAssociationName (buffer);
  if (pstAssociationInfo && (pstAssociationInfo->pstAssocCtrl->state ==  MI_ASSOC_STATE_ACTIVE)&&
      (hasReqCapacity(pstAssociationInfo)==SD_TRUE))
    {
    pstMitMicVcc = findMitMicVcc (pstAssociationInfo->remoteName);

    /* Get a device name and see if it exists as a client in this remote */
    pDevice = getMicDevice (pstMitMicVcc->pstDeviceSet);
    if (pDevice!=NULL)
      {
      if (pDevice->device_attrib & MIC_DEVICE_SBO)
        {
        req = mic_device_select (pstAssociationInfo->pstAssocCtrl->mi_conn, pDevice);
        if (req==NULL)
          {
          MIT_LOG_FLOW1 ("Select Device failed (0x%04x)", mi_op_err);
          }
        else
          {
          MIT_LOG_FLOW0 ("Select Device issued");
          req->u_req_done = doSelectDone;
          req->user_info[0] = pstAssociationInfo;
          decReqCapacity(pstAssociationInfo);
	       }
	     }
      else
        {
        printf ("Device is not selectable");
        pause_msg ("\n Press any key to continue");
	     }
      }
    }
  else
    {
    printf ( "\n Association not active");
    pause_msg ("\n Press any key to continue");
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*			           getDevValue				*/
/************************************************************************/

static MI_DEV_VALUE_UNION *getDevValue(ST_INT typeId)
  {
static MI_DEV_VALUE_UNION devValue;
static MI_CONTROL_COMMAND lastCmdValue;  /* ST_INT16 */
static MI_CONTROL_SETPOINT_REAL lastRealValue = (ST_FLOAT)4.235; /* ST_FLOAT */
static MI_CONTROL_SETPOINT_DISCRETE lastDiscreteValue = 1; /* ST_INT16 */
ST_INT tint;

  switch (typeId)
    {
    case MI_TYPEID_DEV_COMMAND:
      printf ("\n Enter Device Command Value: ");
      tint = lastCmdValue;
      intget (&tint);
      lastCmdValue = tint;
      devValue.cmd = lastCmdValue;
    break;

    case MI_TYPEID_DEV_REAL:
      printf ("\n Enter Device Real Value: ");
      floatget (&lastRealValue);
      devValue.set_pt_r = lastRealValue;
    break;

    case MI_TYPEID_DEV_DISCRETE:
      printf ("\n Enter Device Discrete Value: ");
      tint = lastDiscreteValue;
      intget (&tint);
      lastDiscreteValue = tint;
      devValue.set_pt_d = lastDiscreteValue;
    break;
    }

  return (&devValue);
  }

/************************************************************************/
/*			             doOperateDone 			*/
/************************************************************************/

static ST_VOID doOperateDone (MI_REQ_CTRL *miReq)
  {
MIC_DEVICE *pDevice;
MIT_ASSOC_INFO *pstAssociationInfo;

  pDevice = miReq->mic_device;
  pstAssociationInfo = (struct Tag_MIT_ASSOC_INFO *)miReq->user_info[0];
  incReqCapacity(pstAssociationInfo);
  if (miReq->result != MIC_RESULT_SUCCESS)
    {
    printf ("\n Error in DeviceOperate:  ");
    showDeviceResult (miReq->result);
    MIT_LOG_ERR2 ("DeviceOperate Resp error %d for device %s", miReq->result, 
	          pDevice->device_name);
    return;
    }

  printf ("\n DeviceOperate request for %s was completed SUCCESSFULLY.", 
	  pDevice->device_name);
  MIT_LOG_CNF1 ("DeviceOperate Resp SUCCESS for %s", pDevice->device_name);

  mi_free_req_ctrl (miReq);
  }

/************************************************************************/
/* 		          operateDevice			*/
/************************************************************************/

ST_VOID    operateDevice (ST_VOID)
  {
MIT_MIC_VCC *pstMitMicVcc;
ST_CHAR buffer[MAX_IDENT_LEN +1];
MI_REQ_CTRL *req;
MIC_DEVICE  *pDevice;
MIT_ASSOC_INFO *pstAssociationInfo;
MI_DEV_VALUE_UNION *devValue;

  /* Get the association name and see if it's active			*/

  pstAssociationInfo = getAssociationName (buffer);
  if (pstAssociationInfo && (pstAssociationInfo->pstAssocCtrl->state ==  MI_ASSOC_STATE_ACTIVE)&&
     (hasReqCapacity(pstAssociationInfo)==SD_TRUE))
    {
    pstMitMicVcc = findMitMicVcc (pstAssociationInfo->remoteName);

    /* Get a device name and see if it exists as a client in this remote */
    pDevice = getMicDevice (pstMitMicVcc->pstDeviceSet);
    if (pDevice!=NULL)
      {
      devValue = getDevValue (pDevice->device_attrib & MIC_DEVICE_MI_TYPE_MASK);

      req = mic_device_operate (pstAssociationInfo->pstAssocCtrl->mi_conn, pDevice, devValue);
      if (req==NULL)
        {
        MIT_LOG_FLOW1 ("Operate Device failed (0x%04x)", mi_op_err);
        }
      else
        {
        MIT_LOG_FLOW0 ("Operate Device issued");
        req->u_req_done = doOperateDone;
        req->user_info[0] = pstAssociationInfo;
        decReqCapacity(pstAssociationInfo);
        }
      }
    }
  else
    {
    printf ( "\n Association not active");
    pause_msg ("\n Press any key to continue");
    }

  (*menu_set_fun)();
  }


/************************************************************************/
/* 				              showDeviceResult							*/
/************************************************************************/

static ST_VOID showDeviceResult (ST_RET mic_result)
  {
   switch (mic_result)
    {
    case MIC_RESULT_SUCCESS:
      printf ("SUCCESS");
    break;

    case MIC_RESULT_OBJ_NON_EXISTENT:
      printf ("Device does not exist.");
    break;

    case MIC_RESULT_ACCESS_DENIED:
      printf ("Access Denied.");
    break;

    case MIC_RESULT_HARDWARE_FAULT:
      printf ("Hardware Fault");
    break;

    case MIC_RESULT_TEMP_UNAVAIL:
      printf ("Device is Temporarily Unavailable.");
    break;

    default:
      printf ("Undefined error %d", mic_result);
    break;
    }
  }

/************************************************************************/
/*		               displayTagFlags				*/
/************************************************************************/

static ST_VOID displayTagFlags (ST_UCHAR flag)
  {
  if (flag == 0)
    printf ( "NO_FLAGS_SET" );
  if (flag & MI_TAG_FLAG_NO_TAG)
    printf ( "NO_TAG, " );
  if (flag & MI_TAG_FLAG_OPEN_CLOSE_INHIBIT)
    printf ( "OPEN_CLOSE_INHIBIT, " );
  if (flag & MI_TAG_FLAG_CLOSE_INHIBIT)
    printf ( "CLOSE_INHIBIT, " );
  if (flag & MI_TAG_FLAG_ARMED)
    printf ( "ARMED, " );
  }

/************************************************************************/
/*			       doGetTagDone				*/
/************************************************************************/

static ST_VOID doGetTagDone  (MI_REQ_CTRL *miReq)
  {
MIC_DEVICE *pDevice;
MIT_ASSOC_INFO *pstAssociationInfo;

  pDevice = miReq->mic_device;
  pstAssociationInfo = (struct Tag_MIT_ASSOC_INFO *)miReq->user_info[0];
  incReqCapacity(pstAssociationInfo);
  if (miReq->result != MIC_RESULT_SUCCESS)
    {
    printf ("\n Error in DeviceGettag:  ");
    showDeviceResult (miReq->result);
    return;
    }
  else
    {
    printf ("\n DeviceGetTag request for %s was completed SUCCESSFULLY.", 
	  pDevice->device_name);
    printf ("\n     TagFlags = 0x%x", miReq->o.tag_value->TagFlags);
    printf ("\n        ");
    displayTagFlags (miReq->o.tag_value->TagFlags);
    printf ("\n     Reason = '%s'", miReq->o.tag_value->Reason);
    }

  mi_free_req_ctrl (miReq);
  }

/************************************************************************/
/* 		             getDeviceTag				*/
/************************************************************************/

ST_VOID    getDeviceTag (ST_VOID)
  {
MIT_MIC_VCC *pstMitMicVcc;
ST_CHAR buffer[MAX_IDENT_LEN +1];
MI_REQ_CTRL *req;
MIC_DEVICE  *pDevice;
MIT_ASSOC_INFO *pstAssociationInfo;

  /* Get the association name and see if it's active			*/

  pstAssociationInfo = getAssociationName (buffer);
  if (pstAssociationInfo && (pstAssociationInfo->pstAssocCtrl->state ==  MI_ASSOC_STATE_ACTIVE)&&
     (hasReqCapacity(pstAssociationInfo)==SD_TRUE))
    {
    pstMitMicVcc = findMitMicVcc (pstAssociationInfo->remoteName);

    /* Get a device name and see if it exists as a client in this remote */
    pDevice = getMicDevice (pstMitMicVcc->pstDeviceSet);
    if (pDevice!=NULL)
      {
      req = mic_device_gettag (pstAssociationInfo->pstAssocCtrl->mi_conn, pDevice);
      if (req==NULL)
        {
        MIT_LOG_FLOW1 ("Gettag Device failed (0x%04x)", mi_op_err);
        }
      else
        {
        MIT_LOG_FLOW0 ("Gettag Device issued");
        req->u_req_done = doGetTagDone;
        req->user_info[0] = pstAssociationInfo;
        decReqCapacity(pstAssociationInfo);
 	     }
      }
    }
  else
    {
    printf ( "\n Association not active");
    pause_msg ("\n Press any key to continue");
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/* doSetTagDone: results of device settag operation are in		*/
/************************************************************************/

static ST_VOID doSetTagDone  (MI_REQ_CTRL *miReq)
  {
MIC_DEVICE *pDevice;
MIT_ASSOC_INFO *pstAssociationInfo;

  pDevice = miReq->mic_device;
  pstAssociationInfo = (struct Tag_MIT_ASSOC_INFO *)miReq->user_info[0];
  incReqCapacity(pstAssociationInfo);
  if (miReq->result != MIC_RESULT_SUCCESS)
    {
    printf ("\n Error in DeviceSettag:  ");
    showDeviceResult (miReq->result);
    printf ("DeviceSetTag Resp error %d for device %s", miReq->result, 
	     pDevice->device_name);
    return;
    }

  printf ("\n DeviceSetTag request for %s was completed SUCCESSFULLY.", 
	  pDevice->device_name);
  printf ("DeviceSetTag Resp for %s, result=%d", pDevice->device_name, miReq->result);

  mi_free_req_ctrl (miReq);
  }

/************************************************************************/
/*			        getTag					*/
/************************************************************************/

static ST_VOID      	getTag (ST_UCHAR *s)
  {
  printf (" Tag: ");
  input_hex (s, 1);
  }

/************************************************************************/
/* 		             setDeviceTag				*/
/************************************************************************/

ST_VOID    setDeviceTag (ST_VOID)
  {
MIC_DEVICE 	*pDevice;
MI_TAG_VALUE 	tagValue;
MI_REQ_CTRL 	*req;
MIT_MIC_VCC *pstMitMicVcc;
ST_CHAR buffer[MAX_IDENT_LEN +1];
MIT_ASSOC_INFO *pstAssociationInfo;

  /* Get the association name and see if it's active			*/

  pstAssociationInfo = getAssociationName (buffer);
  if (pstAssociationInfo && (pstAssociationInfo->pstAssocCtrl->state ==  MI_ASSOC_STATE_ACTIVE)&&
     (hasReqCapacity(pstAssociationInfo)==SD_TRUE))
    {
    pstMitMicVcc = findMitMicVcc (pstAssociationInfo->remoteName);

    /* Get a device name and see if it exists as a client in this remote */
    pDevice = getMicDevice (pstMitMicVcc->pstDeviceSet);
    if (pDevice!=NULL)
      {
      printf ("\n Enter Tag value in hex: ");
      tagValue.TagFlags = 0x80;
      getTag (&tagValue.TagFlags);

      printf ("\n Enter Reason: ");
      tagValue.Reason[0]=0;
      strget (tagValue.Reason);

      req = mic_device_settag (pstAssociationInfo->pstAssocCtrl->mi_conn, pDevice, &tagValue);
      if (req==NULL)
        {
        MIT_LOG_FLOW1 ("Settag Device failed (0x%04x)", mi_op_err);
        }
      else
        {
        MIT_LOG_FLOW0 ("Settag Device issued");
        req->u_req_done = doSetTagDone;
        req->user_info[0] = pstAssociationInfo;
        decReqCapacity(pstAssociationInfo);
        }
      }
    }
  else
    {
    printf ( "\n Association not active");
    pause_msg ("\n Press any key to continue");
    }



  (*menu_set_fun)();
  }

/************************************************************************/
/* 		             displayLocalDevices			*/
/************************************************************************/

ST_VOID    displayLocalDevices (ST_VOID)
  {
ST_INT line;
MIT_DEVICE *pstCurDevice;

  pstCurDevice = pstDeviceSet;
  line = 1;
  while (pstCurDevice)
    {
    printf ("\n DeviceName '%s', ChkBackId = %d, SBOTimeout = %d, Taggable = %d",
            pstCurDevice->devName,
            pstCurDevice->chkBackId,
            pstCurDevice->sboTimeOut,
            pstCurDevice->bTaggable);
    line++;
    if (line == 24)
      {
      pause_msg ("\n\n Press any key to continue");
      line =1;
      }

    pstCurDevice = (struct Tag_MIT_DEVICE *)list_get_next(pstDeviceSet, pstCurDevice);
    }
  pause_msg ("\n\n Press any key to continue");
  (*menu_set_fun)();
  }

/************************************************************************/
/*			     iccp_devtype_to_str 				*/
/************************************************************************/

static ST_CHAR *commandStr ="MI_TYPEID_DEV_COMMAND";
static ST_CHAR *realStr ="MI_TYPEID_DEV_REAL";
static ST_CHAR *discreteStr ="MI_TYPEID_DEV_DISCRETE";
static ST_CHAR *unknownStr ="<Unknown>";

ST_CHAR  *iccp_devtype_to_str (ST_INT typeId)
  {

  if (typeId==MI_TYPEID_DEV_COMMAND)
    return (commandStr);
  if (typeId==MI_TYPEID_DEV_REAL)
    return (realStr);
  if (typeId==MI_TYPEID_DEV_DISCRETE)
    return (discreteStr);
  return (unknownStr);
  }

/************************************************************************/
/* 		             displayRemoteDevices			*/
/************************************************************************/

ST_VOID    displayRemoteDevices (ST_VOID)
  {
ST_CHAR 	buffer[256];
MIT_MIS_VCC 	*pstMitMisVcc;
MIT_MIC_VCC 	*pstMitMicVcc;
MIT_DEVICE_REF  *pstCurMisVccDevice;
MIT_DEVICE_REF  *pstCurMicVccDevice;
ST_CHAR		*strScope;
ST_CHAR		*strType;
ST_INT		line;
ST_BOOLEAN expandDescription;
ST_BOOLEAN showServerDevices;

  if (ask ("\n Display Server devices <y,n>(default=y) ? ",SD_TRUE))
    showServerDevices = SD_TRUE;
  else
    showServerDevices = SD_FALSE;

  if (showServerDevices==SD_TRUE)
    {
    pstMitMisVcc = getRemoteMitMisVcc (buffer);
    if (ask ("\n Expand Device description <y,n>(default=n) ? ",SD_FALSE))
      expandDescription = SD_TRUE;
    else
      expandDescription = SD_FALSE;
  
    printf ("\n\n");
    if (pstMitMisVcc)
      {						     
      pstCurMisVccDevice = pstMitMisVcc->pstDeviceSet;
      printf ("\n Local Domain: '%s' BLTID: '%s'\n",
               pstMitMisVcc->localDomainName,
               pstMitMisVcc->bilateralTableId);
      line = 1;
      while (pstCurMisVccDevice)
        {
        strScope = iccp_scope_to_str(pstCurMisVccDevice->scope);
        strType = iccp_devtype_to_str(pstCurMisVccDevice->type);
        if (expandDescription==SD_TRUE)
          {
          printf ("\n DeviceName '%s'",pstCurMisVccDevice->devName);
          printf ("\n     Scope = %s", strScope);
          printf ("\n     Type = %s", strType);
          printf ("\n     Select = %d", pstCurMisVccDevice->bSelectable);
          printf ("\n     Operable = %d", pstCurMisVccDevice->bOperable);
          printf ("\n     Taggable = %d", pstCurMisVccDevice->bTaggable); 
          pause_msg ("\n\n Press any key to continue");
  	  }
        else
          {
          printf ("\n DeviceName '%s'", pstCurMisVccDevice->devName);
          line++;
          if (line == 22)
            {
            pause_msg ("\n\n Press any key to continue");
            printf ("\n Local Domain: '%s' BLTID: '%s'\n",
                    pstMitMisVcc->localDomainName,
                    pstMitMisVcc->bilateralTableId);
            line =1;
            }
         }
        pstCurMisVccDevice = (struct Tag_MIT_DEVICE_REF *)list_get_next (pstMitMisVcc->pstDeviceSet,
                                            pstCurMisVccDevice);
        }
	if (line!= 1 )
	  pause_msg ("\n\n Press any key to continue");
      }
    else
      {
      printf ("\n Remote '%s' not found", buffer);
      pause_msg (" ");
      }
    }
  else  /* list the client devices accessible through this remote */
    {
    pstMitMicVcc = getRemoteMitMicVcc (buffer);
    if (ask ("\n Expand Device description <y,n>(default=n) ? ",SD_FALSE))
      expandDescription = SD_TRUE;
    else
      expandDescription = SD_FALSE;
  
    printf ("\n\n");
    if (pstMitMicVcc)
      {						     
      pstCurMicVccDevice = pstMitMicVcc->pstDeviceSet;
      printf ("\n Remote Domain: '%s' BLTID: '%s'\n",
               pstMitMicVcc->remoteDomainName,
               pstMitMicVcc->bilateralTableId);
      line = 1;
      while (pstCurMicVccDevice)
        {
        strScope = iccp_scope_to_str(pstCurMicVccDevice->scope);
        strType = iccp_devtype_to_str(pstCurMicVccDevice->type);
        if (expandDescription==SD_TRUE)
          {
          printf ("\n DeviceName '%s'",pstCurMicVccDevice->devName);
          printf ("\n     Scope = %s", strScope);
          printf ("\n     Type = %s", strType);
          printf ("\n     Select = %d", pstCurMicVccDevice->bSelectable);
          printf ("\n     Taggable = %d", pstCurMicVccDevice->bTaggable); 
          pause_msg ("\n\n Press any key to continue");
  	  }
        else
          {
          printf ("\n DeviceName '%s'", pstCurMicVccDevice->devName);
          line++;
          if (line == 22)
            {
            pause_msg ("\n\n Press any key to continue");
            printf ("\n Remote Domain: '%s' BLTID: '%s'\n",
                    pstMitMicVcc->remoteDomainName,
                    pstMitMicVcc->bilateralTableId);
            line =1;
            }
         }
        pstCurMicVccDevice = (struct Tag_MIT_DEVICE_REF *)list_get_next (pstMitMicVcc->pstDeviceSet,
                                            pstCurMicVccDevice);
        }
	if (line!= 1 )
	  pause_msg ("\n\n Press any key to continue");
      }
    else
      {
      printf ("\n Remote '%s' not found", buffer);
      pause_msg (" ");
      }
    }
  (*menu_set_fun)();
  }

/************************************************************************/
/*			u_mis_device_select				*/
/* notification of device select request				*/
/************************************************************************/

ST_RET u_mis_device_select (MIS_DEVICE_IND_CTRL *dev_ind_ctrl)
  {
MIT_DEVICE *pstCurDevice;

  pstCurDevice = (MIT_DEVICE *)dev_ind_ctrl->mis_device->access_info;
  printf ("\n Device Select for %s, send default response.", 
           pstCurDevice->devName);
  return (MIS_SEND_DEFAULT_RESP);
  }

/************************************************************************/
/*			u_mis_device_operate				*/
/* notification of device operate request				*/
/************************************************************************/

ST_RET u_mis_device_operate (MIS_DEVICE_IND_CTRL *dev_ind_ctrl)
  {   
MI_CONTROL_COMMAND  *pCommand;
MI_CONTROL_SETPOINT_REAL *pReal;
MI_CONTROL_SETPOINT_DISCRETE *pDiscrete;
MIT_DEVICE *pstCurDevice;

  pstCurDevice = (MIT_DEVICE *)dev_ind_ctrl->mis_device->access_info;
  printf ("\n Device Operate for %s, send default response.", 
           pstCurDevice->devName);

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
      printf ("\n OperateValue = %f", *pReal);
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
MIT_DEVICE *pstCurDevice;
MI_TAG_VALUE *pTagValue;

  pstCurDevice = (MIT_DEVICE *)dev_ind_ctrl->mis_device->access_info;
  printf ("\n Device Set Tag for %s, send default response.", 
           pstCurDevice->devName);

  pTagValue = (MI_TAG_VALUE *) dev_ind_ctrl->data_buf;
  printf ("\n TagFlags = 0x%x, Reason = %s", pTagValue->TagFlags, pTagValue->Reason);

  return (MIS_SEND_DEFAULT_RESP);
  }

/************************************************************************/
/*		      u_mis_device_gettag				*/
/* notification of a device get tag request				*/
/************************************************************************/

ST_RET u_mis_device_gettag (MIS_DEVICE_IND_CTRL *dev_ind_ctrl)
  {
MIT_DEVICE *pstCurDevice;

  pstCurDevice = (MIT_DEVICE *)dev_ind_ctrl->mis_device->access_info;
  printf ("\n Device Get Tag for %s, send default response.", 
          pstCurDevice->devName);
  return (MIS_SEND_DEFAULT_RESP);
  }

/************************************************************************/
/*			        getMicDevice				*/
/************************************************************************/

static MIC_DEVICE *getMicDevice (MIT_DEVICE_REF *deviceSet)
  {
static	ST_CHAR defaultDevName[MAX_IDENT_LEN+1] = "";
static ST_BOOLEAN firstDeviceName = SD_TRUE;
ST_BOOLEAN dataEntered;
ST_CHAR buffer[256];
MIC_DEVICE *pDevice;

  pDevice = NULL;
  if (firstDeviceName==SD_TRUE)
    {
    /* This is the first time a Remote Name is being entered.		*/
    /* When a valid name is entered, it becomes the default.		*/

    printf ("\n Enter Device Name: ");
    dataEntered = strget (buffer);
    if (dataEntered)
      {
      if ((pDevice = (struct _mic_device *)findDevice (deviceSet, buffer))!=NULL)
        {
        strcpy (defaultDevName, buffer);
        firstDeviceName=SD_FALSE;
        }			   
		else
        {
        printf ( "\n Device '%s' not found", buffer);
        pause_msg ("\n Press any key to continue");
        }
      }
    }
  else
    {
    /* There is already an RemoteName being used, it is the 		*/
    /* default supplied to the user and will change when the user 	*/
    /* supplies a different valid name.					*/

    strcpy (buffer, defaultDevName);
    printf ("\n Enter Device Name: (default=%s)",buffer);
    strget (buffer);
    if ((pDevice = (struct _mic_device *)findDevice (deviceSet, buffer))!=NULL)
      {
      strcpy (defaultDevName, buffer);
      }
	 else
      {
      printf ( "\n Device '%s' not found", buffer);
      pause_msg ("\n Press any key to continue");
      }
    }
  return (pDevice);
  }

/************************************************************************/
/* 			       findDevice 				*/
/************************************************************************/

static ST_VOID *findDevice (MIT_DEVICE_REF *pstDeviceSet, ST_CHAR *devName)
  {
MIT_DEVICE_REF	*pstCurDevice;
ST_BOOLEAN bFound = SD_FALSE;

  pstCurDevice = pstDeviceSet;
  while (pstCurDevice && !bFound)
    {
    if (strcmp (pstCurDevice->devName, devName) == 0)
      {
      bFound = SD_TRUE;
      }
    else
      pstCurDevice = (MIT_DEVICE_REF *) list_get_next 
                 ((ST_VOID *)pstDeviceSet, (ST_VOID *)pstCurDevice);
    }
  if (bFound)
    return (pstCurDevice->pstDevice);
  else
    return (NULL);
  }								      

