/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : micb5.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	Handle Block 5 Client Functionality				*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     15    Copyright year update                        */
/* 05/13/02  MDE     14	   Now free request control on send error	*/
/* 04/05/02  MDE     13    Fixed leaks for request on inactive conn	*/
/* 11/28/01  MDE     12	   Corrected mic_add_device_to_vcc parameter	*/
/* 11/15/01  MDE     11	   Changed some mic_xxx function argument order	*/
/* 11/07/01  MDE     10	   Minor logging changes			*/
/* 10/29/01  MDE     09	   Added 'handle_info' to mic_add_device_to_vcc	*/
/* 10/29/01  MDE     08	   minor cleanup				*/
/* 10/29/01  MDE     07	   Added config logging				*/
/* 06/28/01  RKR     06    Fixed error conversion in Select & Operate	*/
/* 06/08/01  RKR     05    Added checking for MVL_ERR_CNF_DISCONNECTED  */
/* 07/06/00  NAV     04    Add MISEM support				*/
/* 08/11/00  RKR     03    changed mic_device_gettag_done               */
/* 11/21/99  NAV     02    Add mi_op_err support			*/
/* 10/08/99  NAV     01    Created					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mi.h"
#include "misem.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif


/************************************************************************/
/* support function prototypes						*/
/************************************************************************/
int _mic_device_name_compare (const MIC_DEVICE **dev1, const MIC_DEVICE **dev2);

ST_VOID _mic_device_select_done (MVL_REQ_PEND *mvl_req);
ST_VOID _mic_device_operate_done (MVL_REQ_PEND *mvl_req);
ST_VOID _mic_device_gettag_done (MVL_REQ_PEND *mvl_req);
ST_VOID _mic_device_settag_done (MVL_REQ_PEND *mvl_req);

ST_RET _mic_acc_result_to_dev_err (ACCESS_RESULT *acc_rslt);

/************************************************************************/
/* functions to manage control center devices				*/
/************************************************************************/

/************************************************************************/
/* mic_enable_device_ctrl: set Supported_Features and allocate memory	*/
/************************************************************************/

ST_VOID mic_enable_device_ctrl (MIC_VCC *mic_vcc, ST_INT num_vcc_dev,
			        ST_INT num_icc_dev)
  {
  S_MI_LOCK_COMMON ();

  mic_vcc->misv.Supported_Features[0] |= MI_BLOCK5;

  mic_vcc->max_vcc_devices = num_vcc_dev;
  if (num_vcc_dev)
/*    mic_vcc->vcc_device_tbl = (_mic_device **)chk_calloc (num_vcc_dev, sizeof (MIC_DEVICE *));
modified 2006.11.23 zy*/
    mic_vcc->vcc_device_tbl = (struct _mic_device **)chk_calloc (num_vcc_dev, sizeof (MIC_DEVICE *));

  mic_vcc->max_icc_devices = num_icc_dev;
  if (num_icc_dev)
    mic_vcc->icc_device_tbl = (struct _mic_device **)chk_calloc (num_icc_dev, sizeof (MIC_DEVICE *));

  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/* mic_add_device_to_vcc: 						*/
/************************************************************************/

MIC_DEVICE *mic_add_device_to_vcc (MIC_VCC *mic_vcc, ST_INT device_scope,
                                   ST_CHAR *device_name, ST_INT device_type,
			           ST_BOOLEAN selectable,
			           ST_BOOLEAN taggable, 
				   MIC_DEVICE_ACCESS_INFO handle_info)
  {
MIC_DEVICE *device;

  S_MI_LOCK_COMMON ();
  if (device_scope == ICC_SPEC && 
     (mic_vcc->num_icc_devices == mic_vcc->max_icc_devices))
    {
    MI_LOG_NERR1 ("Unable to add client ICC device %s, Too Many Devices", device_name);
    S_MI_UNLOCK_COMMON ();
    return NULL;
    }
  else if (device_scope == VCC_SPEC &&
          (mic_vcc->num_vcc_devices == mic_vcc->max_vcc_devices))
    {
    MI_LOG_NERR1 ("Unable to add client VCC device %s, Too Many Devices", device_name);
    S_MI_UNLOCK_COMMON ();
    return NULL;
    }

  /* Allocate, Initialize, and put in table */
  device = (struct _mic_device *)chk_calloc (1, sizeof (MIC_DEVICE));
  device->device_name = device_name;
  device->handle_info = handle_info;

  device->device_attrib = device_type;

  if (selectable)
    device->device_attrib |= MIC_DEVICE_SBO;

  if (taggable)
    device->device_attrib |= MIC_DEVICE_TAGGABLE;
    
  if (device_scope == ICC_SPEC)
    {
    device->device_attrib |= MIC_DEVICE_SCOPE_ICC;
    mic_vcc->icc_device_tbl[mic_vcc->num_icc_devices] = device;
    ++mic_vcc->num_icc_devices;
    }
  else
    {
    mic_vcc->vcc_device_tbl[mic_vcc->num_vcc_devices] = device;
    ++mic_vcc->num_vcc_devices;
    }

  S_MI_UNLOCK_COMMON ();
  return (device);
  }

/************************************************************************/
/* mic_sort_vcc_devices:						*/
/************************************************************************/

ST_VOID mic_sort_vcc_devices (MIC_VCC *mic_vcc)
  {
  S_MI_LOCK_COMMON ();

  qsort (mic_vcc->vcc_device_tbl, mic_vcc->num_vcc_devices, 
         sizeof (MIC_DEVICE *), (int(*)(const void *, const void *))_mic_device_name_compare);
  qsort (mic_vcc->icc_device_tbl, mic_vcc->num_icc_devices, 
         sizeof (MIC_DEVICE *), (int(*)(const void *, const void *))_mic_device_name_compare);

  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/* _mic_device_name_compare:		 				*/
/************************************************************************/
int _mic_device_name_compare (const MIC_DEVICE **dev1, const MIC_DEVICE **dev2)
  {
  return (strcmp ((*dev1)->device_name, (*dev2)->device_name));
  }

/************************************************************************/
/* 		mic_log_vcc_device_tables 				*/
/************************************************************************/

ST_VOID mic_log_vcc_device_tables (MIC_VCC *mic_vcc)
  {
MIC_DEVICE *mic_device;
ST_INT i;

  SLOGCALWAYS0 ("MIC Devices");
  SLOGCALWAYS1 ("  VCC Scope Devices (%d Devices)", mic_vcc->num_vcc_devices);
  for (i = 0; i < mic_vcc->num_vcc_devices; ++i)
    {
    mic_device = mic_vcc->vcc_device_tbl[i];
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mic_device->device_name);
    }
  SLOGCALWAYS1 ("  ICC Scope Devices (%d Devices)", mic_vcc->num_icc_devices);
  for (i = 0; i < mic_vcc->num_icc_devices; ++i)
    {
    mic_device = mic_vcc->icc_device_tbl[i];
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mic_device->device_name);
    }
  }

/************************************************************************/
/* mic_find_device:							*/
/************************************************************************/

MIC_DEVICE *mic_find_device (MIC_VCC *mic_vcc, ST_CHAR *name, ST_INT scope)
  {
MIC_DEVICE mic_dev;
MIC_DEVICE *devp;
MIC_DEVICE **devp_out;

  S_MI_LOCK_COMMON ();
  mic_dev.device_name = name;
  devp = &mic_dev;
  devp_out = NULL;
  if (scope == ICC_SPEC || scope == MI_UNKNOWN_SCOPE)
    {
    devp_out = (struct _mic_device **)bsearch (&devp, mic_vcc->icc_device_tbl, 
    		        mic_vcc->num_icc_devices, 
		        sizeof (MIC_DEVICE *), 
			(int(*)(const void *, const void *))_mic_device_name_compare);
    }
  if (scope == VCC_SPEC || 
     (scope == MI_UNKNOWN_SCOPE && devp_out == NULL))
    {
    devp_out = (struct _mic_device **)bsearch (&devp, mic_vcc->vcc_device_tbl, 
    			mic_vcc->num_vcc_devices, 
		    	sizeof (MIC_DEVICE *), 
			(int(*)(const void *, const void *))_mic_device_name_compare);
    }

  S_MI_UNLOCK_COMMON ();
  if (devp_out != NULL)
    return (*devp_out);

  return (NULL);
  }

/************************************************************************/
/* routines to control devices						*/
/************************************************************************/

#define SBO_TEXT "_SBO"
#define SBO_TEXT_LEN 	4
#define TAG_TEXT "_TAG"
#define TAG_TEXT_LEN 	4

/************************************************************************/
/* mic_device_select: issue MMS Read request to retrieve check back id	*/
/************************************************************************/

MI_REQ_CTRL *mic_device_select  (MI_CONN *mi_conn, MIC_DEVICE *mic_device)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
MVL_READ_RESP_PARSE_INFO *parse_info;
READ_REQ_INFO *req_info;
ST_RET rc;
VARIABLE_LIST *vl;
VARIABLE_SPEC *vs;
MIC_VCC *mic_vcc;
ST_CHAR tempName[MAX_IDENT_LEN + 1];

  S_MI_LOCK_COMMON ();

  mic_vcc = mi_conn->mi_remote->mic_vcc;

  /* Create a read request info struct  */
  req_info = (READ_REQ_INFO *) chk_calloc (1, sizeof (READ_REQ_INFO) +
				           sizeof (VARIABLE_LIST));
  req_info->spec_in_result = SD_FALSE;
  req_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
  req_info->va_spec.num_of_variables = 1;


  parse_info = (MVL_READ_RESP_PARSE_INFO *) chk_calloc (1, 
			               sizeof (MVL_READ_RESP_PARSE_INFO));

  vl = (VARIABLE_LIST *) (req_info + 1);
  vs = &vl->var_spec;
  vs->var_spec_tag = VA_SPEC_NAMED;

  /* We can do this regardless of scope because it is a union */
  sprintf (tempName, "%s%s", mic_device->device_name, SBO_TEXT);
  vs->vs.name.obj_name.vmd_spec = tempName;
  if (mic_device->device_attrib & MIC_DEVICE_SCOPE_ICC)
      {
      vs->vs.name.object_tag = DOM_SPEC;
      vs->vs.name.domain_id = mic_vcc->icc_name;
      }
    else
      vs->vs.name.object_tag = VMD_SPEC;

  /* Now take care of the MVL parse information */
  parse_info->type_id = _mi_get_block5_type_id (MI_TYPEID_DEV_CHECKBACK);
  parse_info->dest = chk_calloc (1, sizeof (MI_SBO_CHECKBACK));
  parse_info->descr_arr = SD_FALSE; 	

  /* OK, the MMS request info is ready, send the request	*/
  rc = mvla_read_variables (mi_conn->net_info, req_info, 1,
			    parse_info, &mvl_req);
  chk_free (req_info);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("DeviceSelect: mvla_read_variables request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    chk_free (parse_info->dest);
    chk_free (parse_info);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_device_select_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mic_device = mic_device;

  /* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/* _mic_device_select_done: the read conf				*/
/************************************************************************/

ST_VOID _mic_device_select_done (MVL_REQ_PEND *mvl_req)
  {
ST_RET rc;
MVL_READ_RESP_PARSE_INFO *parse_info;
MI_REQ_CTRL *mi_req;
MIC_DEVICE *mic_device;
MI_SBO_CHECKBACK *p_check_back_id;
READ_RESP_INFO  *read_info;
ACCESS_RESULT *acc_rslt;


  mi_req = mvl_req->mi_req;
  mic_device = mi_req->mic_device;
  rc = mvl_req->result;
  parse_info = mvl_req->u.rd.parse_info;
  if (rc != MVL_ERR_CNF_DISCONNECTED)
    {
    if (parse_info->result == SD_SUCCESS)
      {
      p_check_back_id = (MI_SBO_CHECKBACK *)parse_info->dest;
      mi_req->o.check_back_id = *p_check_back_id;
      }
    else 
      {
      read_info = (READ_RESP_INFO *) mvl_req->event->u.mms.dec_rslt.data_ptr;
      acc_rslt = read_info->acc_rslt_list;
      rc = _mic_acc_result_to_dev_err (acc_rslt);
      }
    }

  mi_req->result = rc;
  _mi_req_done (mi_req);

  chk_free (parse_info->dest);
  chk_free (parse_info);
  }

/************************************************************************/
/* mic_device_operate: issue MMS Write request to set device value	*/
/************************************************************************/

MI_REQ_CTRL *mic_device_operate (MI_CONN *mi_conn, MIC_DEVICE *mic_device,
                                 MI_DEV_VALUE_UNION *device_value)
  {
MVL_WRITE_REQ_INFO *mvl_write_info;
VARIABLE_SPEC *vs;
WRITE_REQ_INFO *write_info;
VARIABLE_LIST *vl;
ST_RET rc;
MVL_REQ_PEND *mvl_req;
ST_BOOLEAN bEnabled = SD_TRUE;
ST_INT device_type;
MI_REQ_CTRL *mi_req;

  S_MI_LOCK_COMMON ();

  /* Create a write request info struct  */
  write_info = (WRITE_REQ_INFO *) chk_calloc (1, sizeof (WRITE_REQ_INFO) +
			sizeof (VARIABLE_LIST) + sizeof (VAR_ACC_DATA));

  write_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
  write_info->va_spec.num_of_variables =1;

  mvl_write_info = (MVL_WRITE_REQ_INFO *) chk_calloc (1,
		    sizeof (MVL_WRITE_REQ_INFO));

  vl = (VARIABLE_LIST *) (write_info + 1);
  vs = &vl->var_spec;
  vs->var_spec_tag = VA_SPEC_NAMED;

  /* We can do this regardless of scope because it is a union */
  vs->vs.name.obj_name.vmd_spec = mic_device->device_name;
  if (mic_device->device_attrib & MIC_DEVICE_SCOPE_ICC)
      {
      vs->vs.name.object_tag = DOM_SPEC;
      vs->vs.name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
      }
    else
      vs->vs.name.object_tag = VMD_SPEC;

  device_type = mic_device->device_attrib & MIC_DEVICE_MI_TYPE_MASK;
  mvl_write_info->type_id = _mi_get_block5_type_id (device_type);
  if (mvl_write_info->type_id == -1)
    {
    MI_LOG_NERR1 ("DeviceOperate: Invalid device Type for client device %s",
      		   mic_device->device_name);
    chk_free (write_info);
    chk_free (mvl_write_info);
    mi_op_err = MVL_ERR_BAD_TYPE;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  mvl_write_info->local_data = device_value;

  /* send write request and do not wait for reply */
  rc = mvla_write_variables (mi_conn->net_info, write_info, 1,
			     mvl_write_info, &mvl_req);

  chk_free (write_info);
  if (rc != SD_SUCCESS)	/* The service failed				*/
    {
    mvl_free_req_ctrl (mvl_req);
    chk_free (mvl_write_info);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_device_operate_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mic_device = mic_device;

  /* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/* _mic_device_operate_done: write conf	is in				*/
/************************************************************************/

ST_VOID _mic_device_operate_done (MVL_REQ_PEND *mvl_req)
  {
ST_RET rc;
MI_REQ_CTRL *mi_req;
MIC_DEVICE *mic_device;
WRITE_RESP_INFO  *write_info;
ACCESS_RESULT *acc_rslt;
MVL_WRITE_REQ_INFO *mvl_write_info;

  mvl_write_info = mvl_req->u.wr.w_info;

  mi_req = mvl_req->mi_req;
  mic_device = mi_req->mic_device;
  rc = mvl_req->result;
  if ((rc != MVL_ERR_CNF_DISCONNECTED) && (mvl_write_info->result != SD_SUCCESS))
    {
    write_info = (WRITE_RESP_INFO *) mvl_req->event->u.mms.dec_rslt.data_ptr;
    acc_rslt    = (ACCESS_RESULT *) (write_info+1);
    rc = _mic_acc_result_to_dev_err (acc_rslt);
    }

  mi_req->result = rc;
  _mi_req_done (mi_req);

  chk_free (mvl_write_info); 
  }

/************************************************************************/
/* mic_device_settag: issue MMS Write to set tag value			*/
/************************************************************************/

MI_REQ_CTRL *mic_device_settag  (MI_CONN *mi_conn, MIC_DEVICE *mic_device,
			         MI_TAG_VALUE *tag_value)
  {
MVL_WRITE_REQ_INFO *mvl_write_info;
VARIABLE_SPEC *vs;
WRITE_REQ_INFO *write_info;
VARIABLE_LIST *vl;
ST_RET rc;
MVL_REQ_PEND *mvl_req;
ST_CHAR tempName[MAX_IDENT_LEN + 1];
MI_REQ_CTRL *mi_req;

  S_MI_LOCK_COMMON ();

  /* Create a write request info struct  */
  write_info = (WRITE_REQ_INFO *) chk_calloc (1, sizeof (WRITE_REQ_INFO) +
			sizeof (VARIABLE_LIST) + sizeof (VAR_ACC_DATA));

  write_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
  write_info->va_spec.num_of_variables =1;

  mvl_write_info = (MVL_WRITE_REQ_INFO *) chk_calloc (1,
		    sizeof (MVL_WRITE_REQ_INFO));

  vl = (VARIABLE_LIST *) (write_info + 1);
  vs = &vl->var_spec;
  vs->var_spec_tag = VA_SPEC_NAMED;

  /* We can do this regardless of scope because it is a union */
  sprintf (tempName, "%s%s", mic_device->device_name, TAG_TEXT);
  vs->vs.name.obj_name.vmd_spec = tempName;
  if (mic_device->device_attrib & MIC_DEVICE_SCOPE_ICC)
    {
    vs->vs.name.object_tag = DOM_SPEC;
    vs->vs.name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
    }
  else
    vs->vs.name.object_tag = VMD_SPEC;

  mvl_write_info->type_id = _mi_get_block5_type_id (MI_TYPEID_DEV_TAGVALUE);
  mvl_write_info->local_data = tag_value;

  /* send write request and do not wait for reply */
  rc = mvla_write_variables (mi_conn->net_info, write_info, 1,
			     mvl_write_info, &mvl_req);

  chk_free (write_info);
  if (rc != SD_SUCCESS)	/* The service failed				*/
    {
    mvl_free_req_ctrl (mvl_req);
    chk_free (mvl_write_info);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_device_settag_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mic_device = mic_device;

  /* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/* _mic_device_settag_done: write conf is in				*/
/************************************************************************/

ST_VOID _mic_device_settag_done (MVL_REQ_PEND *mvl_req)
  {
ST_RET rc;
MI_REQ_CTRL *mi_req;
MIC_DEVICE *mic_device;
WRITE_RESP_INFO  *write_info;
ACCESS_RESULT *acc_rslt;
MVL_WRITE_REQ_INFO *mvl_write_info;

  mvl_write_info = mvl_req->u.wr.w_info;

  mi_req = mvl_req->mi_req;

  mic_device = mi_req->mic_device;
  rc = mvl_req->u.wr.w_info->result;

  if (rc == SD_FAILURE)
    {
    write_info = (WRITE_RESP_INFO *) mvl_req->event->u.mms.dec_rslt.data_ptr;
    acc_rslt    = (ACCESS_RESULT *) (write_info+1);
    rc = _mic_acc_result_to_dev_err (acc_rslt);
    }
  else if (rc != SD_SUCCESS)
    {
    rc = MIC_RESULT_OTHER_MMS_ERR;
    MI_LOG_NERR1 ("DeviceSetTag received MMS error 0x%x", mvl_req->result);
    }

  mi_req->result = rc;
  _mi_req_done (mi_req);

  chk_free (mvl_write_info); 
  }

/************************************************************************/
/* mic_device_gettag: issue MMS Read request to retrieve TagValue	*/
/************************************************************************/

MI_REQ_CTRL *mic_device_gettag  (MI_CONN *mi_conn, MIC_DEVICE *mic_device)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
MVL_READ_RESP_PARSE_INFO *parse_info;
READ_REQ_INFO *req_info;
ST_RET rc;
VARIABLE_LIST *vl;
VARIABLE_SPEC *vs;
MIC_VCC *mic_vcc;
ST_CHAR tempName[MAX_IDENT_LEN + 1];
ST_INT type_id;

  S_MI_LOCK_COMMON ();

  mic_vcc = mi_conn->mi_remote->mic_vcc;

  /* Create a read request info struct  */
  req_info = (READ_REQ_INFO *) chk_calloc (1, sizeof (READ_REQ_INFO) +
				           sizeof (VARIABLE_LIST));
  req_info->spec_in_result = SD_FALSE;
  req_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
  req_info->va_spec.num_of_variables = 1;

  parse_info = (MVL_READ_RESP_PARSE_INFO *) chk_calloc (1, 
			               sizeof (MVL_READ_RESP_PARSE_INFO));

  vl = (VARIABLE_LIST *) (req_info + 1);
  vs = &vl->var_spec;
  vs->var_spec_tag = VA_SPEC_NAMED;

  /* We can do this regardless of scope because it is a union */
  sprintf (tempName, "%s%s", mic_device->device_name, TAG_TEXT);
  vs->vs.name.obj_name.vmd_spec = tempName;
  if (mic_device->device_attrib & MIC_DEVICE_SCOPE_ICC)
      {
      vs->vs.name.object_tag = DOM_SPEC;
      vs->vs.name.domain_id = mic_vcc->icc_name;
      }
    else
      vs->vs.name.object_tag = VMD_SPEC;

  /* Now take care of the MVL parse information */
  type_id = _mi_get_block5_type_id (MI_TYPEID_DEV_TAGVALUE);
  parse_info->type_id = type_id;
  parse_info->dest = chk_calloc (1, sizeof (MI_TAG_VALUE));
  parse_info->descr_arr = SD_FALSE; 	

  /* OK, the MMS request info is ready, send the request	*/
  rc = mvla_read_variables (mi_conn->net_info, req_info, 1,
			    parse_info, &mvl_req);
  chk_free (req_info);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("DeviceSelect: mvla_read_variables request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_device_gettag_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mic_device = mic_device;

  /* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/* _mic_device_gettag_done: the read conf is in				*/
/************************************************************************/

ST_VOID _mic_device_gettag_done (MVL_REQ_PEND *mvl_req)
  {
ST_RET rc;
MVL_READ_RESP_PARSE_INFO *parse_info;
MI_REQ_CTRL *mi_req;
MIC_DEVICE *mic_device;
READ_RESP_INFO  *read_info;
ACCESS_RESULT *acc_rslt;

  mi_req = mvl_req->mi_req;
  mic_device = mi_req->mic_device;
  rc = mvl_req->result;
  parse_info = mvl_req->u.rd.parse_info;

  if (rc == SD_SUCCESS)
    {
    if (mvl_req->u.rd.parse_info->result==SD_SUCCESS)
      {
      mi_req->o.tag_value = (MI_TAG_VALUE *)parse_info->dest;
      }
    else if (mvl_req->u.rd.parse_info->result==SD_FAILURE)
      {
      read_info = (READ_RESP_INFO *) mvl_req->event->u.mms.dec_rslt.data_ptr;
      acc_rslt = read_info->acc_rslt_list;
      rc = _mic_acc_result_to_dev_err (acc_rslt);
      }
    else
      {
      rc = MIC_RESULT_OTHER_MMS_ERR;
      MI_LOG_NERR1 ("DeviceGetTag received MMS error 0x%x", mvl_req->result);
      }
    }

  mi_req->result = rc;
  _mi_req_done (mi_req);

  chk_free (parse_info->dest);
  chk_free (parse_info);
  }

/************************************************************************/
/* _mic_acc_result_to_dev_err: convert MMS ACCESS RESULT error to 	*/
/*			       ICCP Device Control Error		*/
/************************************************************************/

ST_RET _mic_acc_result_to_dev_err (ACCESS_RESULT *acc_rslt)
  {
ST_RET rc;

  if (acc_rslt->acc_rslt_tag == ACC_RSLT_SUCCESS)
    rc = MIC_RESULT_SUCCESS;
  else
    switch (acc_rslt->failure)
      {
      case ARE_OBJ_NONEXISTENT:
      case ARE_OBJ_UNDEFINED:
        rc = MIC_RESULT_OBJ_NON_EXISTENT;
      break;

      case ARE_OBJ_ACCESS_DENIED:
        rc = MIC_RESULT_ACCESS_DENIED;
      break;

      case ARE_HW_FAULT:
        rc = MIC_RESULT_HARDWARE_FAULT;
      break;

      case ARE_TEMP_UNAVAIL:
        rc = MIC_RESULT_TEMP_UNAVAIL;
      break;

      default:
        rc = MIC_RESULT_OTHER_MMS_ERR;
      break;
      }

    return (rc);
  }
