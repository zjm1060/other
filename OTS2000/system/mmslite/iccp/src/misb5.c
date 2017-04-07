/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : misB5.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*		Handle Server Block 5 Functionality			*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     15    Copyright year update                        */
/* 01/17/02  ASK     14    Made changes for asn1r 			*/
/* 12/07/01  MDE     13    Don't alloc 0 size tables for no devices	*/
/* 12/05/01  MDE     12    Handle MIS_SEND_DENIED_RESP for gettag	*/
/* 12/03/01  MDE     11	   Fixed mis_add_device_ex, mis_add_device	*/
/* 11/23/01  NAV     10    Handle MIS_SEND_DENIED_RESP			*/
/* 11/28/01  MDE     10	   Added and used MIS_DV_INDEX			*/
/* 11/15/01  MDE     09	   Changed the ST_UCHAR ???[1] to ST_UCHAR ???	*/
/* 11/07/01  MDE     08	   mis_free_device_tables now checks for NULL	*/
/* 11/07/01  MDE     07	   Minor logging changes			*/
/* 10/29/01  MDE     06	   Added config logging				*/
/* 10/19/01  MDE     05	   Dynamic local object reconfiguration changes	*/
/* 07/19/00  NAV     04    Handle MIS_SEND_UNAVAIL_RESP			*/
/* 07/06/00  NAV     03    Add MISEM support				*/
/* 08/21/00  NAV     02    Declare some globals                         */
/* 10/05/99  NAV     01    Created					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mi.h"
#include "stime.h"
#include "misem.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif



/************************************************************************/
MIS_DEVICE *mis_device_ctrl;		/* table of ptrs to Device table*/
MIS_DEVICE_INDEX *mis_device_ctrl_index;	/* sorted index			*/
ST_INT mis_num_device_ctrl;     	/* Number of Devices in table	*/
ST_INT mis_num_sorted_device_ctrl;     	/* Number of sorted Devices 	*/
ST_INT mis_max_num_device_ctrl; 	/* Size of table		*/
/************************************************************************/

/************************************************************************/
/* local function prototypes:						*/
/************************************************************************/
int _mi_device_name_compare (const void *idx1, 
			     const void *idx2);
int _mi_device_name_compare2 (const void *dev1, 
			      const void *idx2);
int _mi_device_ref_compare (const void *device_ref1, 
			    const void *device_ref2);
int _mi_device_ref_compare2 (const void *device, 
			    const void *device_ref2);

ST_VOID _mis_device_select_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind);
ST_VOID _mis_device_gettag_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind);
ST_VOID _mis_device_operate_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind);
ST_VOID _mis_device_settag_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind);
ST_VOID  mis_dev_rd_done (MIS_DEVICE_IND_CTRL *mis_dev_ind, ST_RET rslt);
ST_VOID _mis_dev_wr_done (MIS_DEVICE_IND_CTRL *mis_dev_ind);

/************************************************************************/
/* Block 5 - Device Control: Local Server Support Functions		*/
/************************************************************************/

/************************************************************************/
/* mis_create_device_tables: 						*/
/************************************************************************/

ST_RET mis_create_device_tables (ST_INT max_num_device)
  {

  if (max_num_device > MIS_MAX_TOTAL_DEVREF)
    {
    MI_LOG_NERR0 ("Too many Devices"); 
    return (SD_FAILURE);
    }

  S_MI_LOCK_COMMON ();

  if (max_num_device != 0)
    {
    mis_device_ctrl_index = (unsigned long *)chk_calloc (max_num_device, sizeof (MIS_DEVICE_INDEX)); 
/*    mis_device_ctrl = (_mis_device *)chk_calloc (max_num_device, sizeof (MIS_DEVICE)); 
modified 2006.11.23 zy*/
    mis_device_ctrl = (struct _mis_device *)chk_calloc (max_num_device, sizeof (MIS_DEVICE)); 
    }
  mis_max_num_device_ctrl = max_num_device;
  mis_num_device_ctrl = 0;
  mis_num_sorted_device_ctrl = 0;

  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/* mis_resize_device_table: 						*/
/************************************************************************/

ST_RET mis_resize_device_tables (ST_INT max_num_device)
  {
  if (max_num_device > MIS_MAX_TOTAL_DEVREF)
    {
    MI_LOG_NERR0 ("Too many Devices"); 
    return (SD_FAILURE);
    }

/* See if the new count is less or equal to the previous */
  if (max_num_device <= mis_max_num_device_ctrl)
    {
    MI_LOG_FLOW2 ("Resize unnecessary (new %d, curr %d)", max_num_device, mis_max_num_device_ctrl); 
    return (SD_SUCCESS);
    }

/* OK, we just need to realloc the tables and set the new limit */
  S_MI_LOCK_COMMON ();
  mis_device_ctrl_index = (unsigned long *)chk_realloc (mis_device_ctrl_index, max_num_device * sizeof (MIS_DEVICE_INDEX)); 
  mis_device_ctrl = (struct _mis_device *)chk_realloc (mis_device_ctrl, max_num_device * sizeof (MIS_DEVICE)); 
  mis_max_num_device_ctrl = max_num_device;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/* mis_free_device_tables:						*/
/************************************************************************/

ST_RET mis_free_device_tables (ST_VOID)
  {
  S_MI_LOCK_COMMON ();
  if (mis_device_ctrl !=NULL)
    {
    chk_free (mis_device_ctrl_index);
    chk_free (mis_device_ctrl);
    mis_max_num_device_ctrl = 0;
    mis_num_device_ctrl = 0;
    mis_num_sorted_device_ctrl = 0;
    }
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/* mis_add_device:							*/
/************************************************************************/

ST_RET mis_add_device (ST_CHAR *device_name, MI_SBO_CHECKBACK check_back_id,
                       ST_BOOLEAN sbo, ST_BOOLEAN taggable,
		       MIS_DEVICE_ACCESS_INFO access_info)
  {
MIS_DEVICE_REF dev_index;

  return (mis_add_device_ex (device_name, check_back_id, sbo, taggable,
		       	     access_info, &dev_index));
  }


/************************************************************************/
/* mis_add_device_ex:							*/
/************************************************************************/

ST_RET mis_add_device_ex (ST_CHAR *device_name, MI_SBO_CHECKBACK check_back_id,
                       ST_BOOLEAN sbo, ST_BOOLEAN taggable,
		       MIS_DEVICE_ACCESS_INFO access_info, MIS_DEVICE_REF *dev_index_out)
  {
MIS_DEVICE *pdevice;

  S_MI_LOCK_COMMON ();

  if (mis_num_device_ctrl >= mis_max_num_device_ctrl)
    {
    MI_LOG_NERR0 ("Too many Devices"); 
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }

  pdevice = &mis_device_ctrl[mis_num_device_ctrl];
  pdevice->device_name = device_name;
  pdevice->device_state = MIS_DEVICE_STATE_IDLE;
  pdevice->sbo = sbo;
  pdevice->check_back_id = check_back_id;
  pdevice->taggable = taggable;
  pdevice->access_info = access_info;

  mis_device_ctrl_index[mis_num_device_ctrl] = mis_num_device_ctrl;
  *dev_index_out = mis_num_device_ctrl;
  ++mis_num_device_ctrl;

  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/* mis_sort_device_table:						*/
/************************************************************************/

ST_VOID mis_sort_device_table (ST_VOID)
  {

  S_MI_LOCK_COMMON ();
  qsort (mis_device_ctrl_index, mis_num_device_ctrl, sizeof (MIS_DEVICE_INDEX), 
         (int(*)(const void *, const void *))_mi_device_name_compare);

  mis_num_sorted_device_ctrl = mis_num_device_ctrl;
  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/*			_mi_device_name_compare 			*/
/************************************************************************/

int _mi_device_name_compare (const void *idx1, 
			     const void *idx2)
  {
MIS_DEVICE *dev1;
MIS_DEVICE *dev2;

  dev1 = &mis_device_ctrl[*((const MIS_DEVICE_INDEX *)idx1)];
  dev2 = &mis_device_ctrl[*((const MIS_DEVICE_INDEX *)idx2)];
  return (strcmp (dev1->device_name, dev2->device_name));
  }


/************************************************************************/
/* mis_find_device:							*/
/************************************************************************/

ST_RET mis_find_device (ST_CHAR *device_name, MIS_DEVICE_INDEX *device_index_out)
  {
MIS_DEVICE dev;
MIS_DEVICE_INDEX *dev_index_found;
ST_INT i;
MIS_DEVICE_INDEX idx;

  S_MI_LOCK_COMMON ();

  dev.device_name = device_name;
  dev_index_found = (unsigned long *)bsearch (&dev, mis_device_ctrl_index, mis_num_sorted_device_ctrl, 
		       sizeof (MIS_DEVICE_INDEX), (int(*)(const void *, const void *))_mi_device_name_compare2);

  if (dev_index_found == NULL)
    {
  /* We will now look in the unsorted devices, if any */

    for (i = mis_num_sorted_device_ctrl; i < mis_num_device_ctrl; ++i)
      {
      idx = mis_device_ctrl_index[i];
      if (!strcmp (device_name, mis_device_ctrl[idx].device_name))
        { 
        dev_index_found = &idx;
        break;
        }
      }
    if (i >= mis_num_device_ctrl)
      {
      S_MI_UNLOCK_COMMON ();
      return (SD_FAILURE);
      }
    }
  *device_index_out = *dev_index_found;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			_mi_device_name_compare2 			*/
/************************************************************************/

int _mi_device_name_compare2 (const void *dev1, 
			      const void *idx2)
  {
MIS_DEVICE *dev2;

  dev2 = &mis_device_ctrl[*((const MIS_DEVICE_INDEX *)idx2)];
  return (strcmp (((const MIS_DEVICE *)dev1)->device_name, dev2->device_name));
  }


/************************************************************************/
/*			mis_log_dev_tables 				*/
/************************************************************************/

ST_VOID mis_log_dev_tables ()
  {
ST_INT i;
MIS_DEVICE_INDEX idx;

  SLOGCALWAYS2 ("Local Device Table (%d Devices, %d sorted)", 
		mis_num_device_ctrl, mis_num_sorted_device_ctrl)
  for (i = 0; i < mis_num_device_ctrl; ++i)
    {
    idx = mis_device_ctrl_index[i];
    SLOGCALWAYS3 ("  %4d) %-32.32s   %-32.32s", i+1, 
                          mis_device_ctrl[idx].device_name,
			  mis_device_ctrl[i].device_name);
    }
  }


/************************************************************************/
/* Functions that maintain the bilateral table for the ICCP Server	*/
/************************************************************************/

/************************************************************************/
/* mis_enable_device_ctrl: set supported features and allocate resources*/
/************************************************************************/

ST_RET mis_enable_device_ctrl (MIS_VCC *mis_vcc, ST_INT num_vcc_dev,
			       ST_INT num_icc_dev)
  {

  S_MI_LOCK_COMMON ();

  mis_vcc->max_vcc_device_ref = num_vcc_dev;
  if (num_vcc_dev > 0)
    mis_vcc->vcc_device_ref_tbl = (unsigned long *)chk_calloc (num_vcc_dev, sizeof (MIS_DEVICE_REF));

  mis_vcc->max_icc_device_ref = num_icc_dev;
  if (num_icc_dev > 0)
    mis_vcc->icc_device_ref_tbl = (unsigned long *)chk_calloc (num_icc_dev, sizeof (MIS_DEVICE_REF));

  /* add Block 5 to the Supported_Features				*/
  mis_vcc->misv.Supported_Features[0] |= MI_BLOCK5;

  S_MI_UNLOCK_COMMON ();
  return SD_SUCCESS;
  }

/************************************************************************/
/* mis_add_device_to_vcc:						*/
/************************************************************************/

ST_RET mis_add_device_to_vcc (MIS_VCC *mis_vcc, ST_INT device_scope,
                              ST_CHAR *device_name, ST_INT device_type,
			      ST_BOOLEAN select_access,
			      ST_BOOLEAN operate_access,
			      ST_BOOLEAN settag_access)
  {
MIS_DEVICE_REF device_ref;

  return (mis_add_device_to_vcc_ex (mis_vcc, device_scope, device_name, 
				    device_type, select_access,
			      	    operate_access, settag_access, &device_ref));
  }

ST_RET mis_add_device_to_vcc_ex (MIS_VCC *mis_vcc, ST_INT device_scope,
                              ST_CHAR *device_name, ST_INT device_type,
			      ST_BOOLEAN select_access,
			      ST_BOOLEAN operate_access,
			      ST_BOOLEAN settag_access, MIS_DEVICE_REF *device_ref_out)
  {
MIS_DEVICE_REF device_index;
MIS_DEVICE_REF device_ref;
MIS_DEVICE_REF mi_type_mask;
ST_RET rc;

  S_MI_LOCK_COMMON ();

  /* Get the Device index */
  rc = mis_find_device (device_name, &device_index);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("Could not find Device %s", device_name); 
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }

  /* Create the reference - start with the index	*/
  device_ref = device_index;

  /* Set the scope 					*/
  if (device_scope == ICC_SPEC)
    device_ref |= MIS_DEVREF_ICC;

  /* Set the Access Control bits			*/
  if (select_access)
    device_ref |= MIS_DEVREF_SELECT;

  if (operate_access)
    device_ref |= MIS_DEVREF_OPERATE;

  if (settag_access)
    device_ref |= MIS_DEVREF_SETTAG;

  /* Now set the type */
  mi_type_mask = (MIS_DEVICE_REF) device_type;
  mi_type_mask <<= MIS_DEVREF_TYPE_SHIFT;
  mi_type_mask &= MIS_DEVREF_TYPE_MASK_B;
  device_ref |= mi_type_mask;

  /* OK, go ahead and store the Device Reference */
  if (device_scope == ICC_SPEC)
    {
    mis_vcc->icc_device_ref_tbl[mis_vcc->num_icc_device_ref] = device_ref;
    ++mis_vcc->num_icc_device_ref;
    }
  else
    {
    mis_vcc->vcc_device_ref_tbl[mis_vcc->num_vcc_device_ref] = device_ref;
    ++mis_vcc->num_vcc_device_ref;
    }

  *device_ref_out = device_ref;

  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/* mis_sort_vcc_devices: 						*/
/************************************************************************/

ST_VOID mis_sort_vcc_devices (MIS_VCC *mis_vcc)
  {
MIS_DEVICE_REF *mis_device_ref_tbl;
ST_INT num_device_ref;

  S_MI_LOCK_COMMON ();

  num_device_ref = mis_vcc->num_icc_device_ref;
  mis_device_ref_tbl = mis_vcc->icc_device_ref_tbl;
  qsort (mis_device_ref_tbl, num_device_ref, sizeof (MIS_DEVICE_REF), 
         (int(*)(const void *, const void *))_mi_device_ref_compare);

  num_device_ref = mis_vcc->num_vcc_device_ref;
  mis_device_ref_tbl = mis_vcc->vcc_device_ref_tbl;
  qsort (mis_device_ref_tbl, num_device_ref, sizeof (MIS_DEVICE_REF), 
         (int(*)(const void *, const void *))_mi_device_ref_compare);
  S_MI_UNLOCK_COMMON ();
  }


/************************************************************************/
/* _mi_device_ref_compare:						*/
/************************************************************************/

int _mi_device_ref_compare (const void *device_ref1, 
			    const void *device_ref2)
  {
ST_CHAR *device_name1;
ST_CHAR *device_name2;
MIS_DEVICE_REF device_index;

  device_index = (*((const MIS_DEVICE_REF *)device_ref1)) & MIS_INDEX_MASK;
  device_name1 = mis_device_ctrl[device_index].device_name;
  device_index = (*((const MIS_DEVICE_REF *)device_ref2)) & MIS_INDEX_MASK;
  device_name2 = mis_device_ctrl[device_index].device_name;
  return (strcmp (device_name1, device_name2));
  }

/************************************************************************/
/* mis_find_vcc_device:							*/
/************************************************************************/

ST_RET mis_find_vcc_device (MIS_VCC *mis_vcc,
                            ST_CHAR *device_name,
			    ST_INT scope,
			    MIS_DEVICE **device_out,
			    MIS_DEVICE_REF *device_ref_out,
			    ST_INT *mi_type_out)
  {
MIS_DEVICE device;
MIS_DEVICE_REF device_ref;
MIS_DEVICE_REF *device_ref_found;
ST_INT num_device_ref;
MIS_DEVICE_REF *mis_device_ref_tbl;

  S_MI_LOCK_COMMON ();

  if (scope == ICC_SPEC)
    {
    num_device_ref = mis_vcc->num_icc_device_ref;
    mis_device_ref_tbl = mis_vcc->icc_device_ref_tbl;
    }
  else
    {
    num_device_ref = mis_vcc->num_vcc_device_ref;
    mis_device_ref_tbl = mis_vcc->vcc_device_ref_tbl;
    }

  device.device_name = device_name;
  device_ref_found = (unsigned long *)bsearch (&device, mis_device_ref_tbl, num_device_ref, 
		    sizeof (MIS_DEVICE_REF), (int(*)(const void *, const void *))_mi_device_ref_compare2);
  if (device_ref_found == NULL)
    {
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }

  device_ref = *device_ref_found;
  *device_ref_out = device_ref;
  *device_out = MIS_DEV_REF_TO_DEV (device_ref);
  *mi_type_out = (device_ref >> MIS_DEVREF_TYPE_SHIFT) & MIS_DEVREF_TYPE_MASK_A;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/* _mi_device_ref_compare2:						*/
/************************************************************************/

int _mi_device_ref_compare2 (const void *device, 
			    const void *device_ref2)
  {
ST_CHAR *device_name2;
MIS_DEVICE_REF device_index;

  device_index = (*((const MIS_DEVICE_REF *)device_ref2)) & MIS_INDEX_MASK;
  device_name2 = mis_device_ctrl[device_index].device_name;
  return (strcmp (((const MIS_DEVICE *)device)->device_name, device_name2));
  }


/************************************************************************/
/*			mis_log_vcc_device_tables 				*/
/************************************************************************/

ST_VOID mis_log_vcc_device_tables (MIS_VCC *mis_vcc)
  {
MIS_DEVICE_REF device_ref;
MIS_DEVICE *mis_device;
ST_INT i;

  SLOGCALWAYS0 ("MIS Devices");
  SLOGCALWAYS1 ("  VCC Scope Devices (%d Devices)", mis_vcc->num_vcc_device_ref)
  for (i = 0; i < mis_vcc->num_vcc_device_ref; ++i)
    {
    device_ref = mis_vcc->vcc_device_ref_tbl[i];
    mis_device = MIS_DEV_REF_TO_DEV (device_ref);
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mis_device->device_name);
    }
  SLOGCALWAYS1 ("  ICC Scope Devices (%d Devices)", mis_vcc->num_icc_device_ref)
  for (i = 0; i < mis_vcc->num_icc_device_ref; ++i)
    {
    device_ref = mis_vcc->icc_device_ref_tbl[i];
    mis_device = MIS_DEV_REF_TO_DEV (device_ref);
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mis_device->device_name);
    }
  }

/************************************************************************/
/* mis_device_reset: transition from ARMED to IDLE state		*/
/************************************************************************/

ST_RET mis_device_reset (MIS_DEVICE *pdevice)
  {
  S_MI_LOCK_COMMON ();
  pdevice->device_state = MIS_DEVICE_STATE_IDLE;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/* mis_device_busy: transition from IDLE to BUSY			*/
/************************************************************************/

ST_RET mis_device_busy (MIS_DEVICE *pdevice)
  {
  S_MI_LOCK_COMMON ();
  pdevice->device_state = MIS_DEVICE_STATE_BUSY;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/* Function to handle device indications.				*/
/************************************************************************/

/************************************************************************/
/* _mis_device_read_ind: someone wants to select a device or get a tag  */
/************************************************************************/

ST_VOID _mis_device_read_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind)
  {

  switch (mis_dev_ind->op)
    {
    case MI_DEVICE_OP_SELECT:
      _mis_device_select_ind (mis_dev_ind);
    break;

    case MI_DEVICE_OP_GETTAG:
     _mis_device_gettag_ind (mis_dev_ind);
    break;

    default:
     MI_LOG_NERR0 ("DeviceReadInd - Invalid Operation");
    break;
    }

  }

/************************************************************************/
/* _mis_device_select_ind: do the grunt work for a select		*/
/************************************************************************/

ST_VOID _mis_device_select_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind)
  {
ST_RET rc;
MIS_DEVICE *mis_device;

  mis_device = mis_dev_ind->mis_device;

  /* check access						*/
  if (mis_dev_ind->mis_device_ref & MIS_DEVREF_SELECT)
    {
    /* let the user know					*/
    mis_dev_ind->data_type = _mi_get_block5_type_id (MI_TYPEID_DEV_CHECKBACK);
    rc = u_mis_device_select (mis_dev_ind);
    if (rc == MIS_SEND_DEFAULT_RESP)
      {
      /* do the default response				*/
      mis_device = mis_dev_ind->mis_device;
      if (mis_device->device_state == MIS_DEVICE_STATE_BUSY)
        {
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_HW_FAULT;
        mis_dev_rd_done (mis_dev_ind, SD_FAILURE);
	return;
	}
      
      if ((mis_device->device_state == MIS_DEVICE_STATE_ARMED) ||
          (mis_device->device_state == MIS_DEVICE_STATE_SELECTING) ||
	  (mis_device->device_state == MIS_DEVICE_STATE_TAGGED && 
	   (ST_INT8) mis_device->tag_value.TagFlags == MI_TAG_FLAG_OPEN_CLOSE_INHIBIT))
	{
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
        mis_dev_rd_done (mis_dev_ind, SD_FAILURE);
	return;
	}

      memcpy (mis_dev_ind->data_buf, &mis_device->check_back_id,
      	      sizeof (MI_SBO_CHECKBACK));
      mis_dev_ind->acc_rslt_tag = ACC_RSLT_SUCCESS;
      mis_device->device_state = MIS_DEVICE_STATE_ARMED;
      mis_device->selecter = mis_dev_ind->mis_vcc->mi_remote;
      mis_dev_rd_done (mis_dev_ind, SD_SUCCESS);
      }
    else if (rc == MIS_SEND_UNAVAIL_RESP)
      {
      mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
      mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
      mis_dev_rd_done (mis_dev_ind, SD_FAILURE);
      return;
      }
    else if (rc == MIS_SEND_DENIED_RESP)
      {
      mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
      mis_dev_ind->failure = ARE_OBJ_ACCESS_DENIED;
      mis_dev_rd_done (mis_dev_ind, SD_FAILURE);
      return;
      }
    else
      mis_device->device_state = MIS_DEVICE_STATE_SELECTING;
    }
  else		/* access denied					*/
    {
    mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
    mis_dev_ind->failure = ARE_OBJ_ACCESS_DENIED;
    mis_dev_rd_done (mis_dev_ind, SD_FAILURE);
    }

  }

/************************************************************************/
/* _mis_device_gettag_ind: do the grunt work for a gettag		*/
/************************************************************************/

ST_VOID _mis_device_gettag_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind)
  {
ST_RET rc;
MIS_DEVICE *mis_device;

  mis_device = mis_dev_ind->mis_device;

  /* let the user know					*/
  mis_dev_ind->data_type = _mi_get_block5_type_id (MI_TYPEID_DEV_TAGVALUE);
  rc = u_mis_device_gettag (mis_dev_ind);
  if (rc == MIS_SEND_DEFAULT_RESP)
    {
    /* do the default response				*/
    if (mis_device->device_state == MIS_DEVICE_STATE_ARMED)
      mis_device->tag_value.TagFlags |= MI_TAG_FLAG_ARMED;
    else
      mis_device->tag_value.TagFlags &= ~MI_TAG_FLAG_ARMED;

    memcpy (mis_dev_ind->data_buf, &mis_device->tag_value,
     	    sizeof (MI_TAG_VALUE));
    mis_dev_ind->acc_rslt_tag = ACC_RSLT_SUCCESS;
    mis_dev_rd_done (mis_dev_ind, SD_SUCCESS);
    }
  else if ((rc == MIS_SEND_DENIED_RESP) ||
	   (rc == MIS_SEND_UNAVAIL_RESP))
    {
    mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
    mis_dev_ind->failure = ARE_OBJ_ACCESS_DENIED;
    mis_dev_rd_done (mis_dev_ind, SD_FAILURE);
    return;
    }
  }

/************************************************************************/
/* _mis_device_read_ind: someone wants to operate a device or set a tag */
/************************************************************************/

ST_VOID _mis_device_write_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind,
			       VAR_ACC_DATA  *va_data)
  {
ST_RET rc;
RUNTIME_TYPE *rt;
ST_INT num_rt;
ST_INT type_id;


  /* stash the data							*/
  type_id = _mi_get_block5_type_id (mis_dev_ind->data_type);
  rc = mvl_get_runtime (type_id, &rt, &num_rt);
  if (rc == SD_SUCCESS)
    {
    rc = ms_asn1_to_local (rt, num_rt, va_data->data, va_data->len, 
  			   (char *)mis_dev_ind->data_buf);
  
    if (rc != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("MI Write Ind - Device Control: ms_asn1_to_local failed, rc = 0x%x", rc);

      mis_dev_ind->acc_rslt_tag = WR_RSLT_FAILURE;
      mis_dev_ind->failure = ARE_TYPE_INCONSISTENT;
      _mis_dev_wr_done (mis_dev_ind);
      }
    else
      {
      switch (mis_dev_ind->op)
        {
        case MI_DEVICE_OP_OPERATE:
          _mis_device_operate_ind (mis_dev_ind);
        break;

        case MI_DEVICE_OP_SETTAG:
          _mis_device_settag_ind (mis_dev_ind);
        break;

        default:
          MI_LOG_NERR0 ("DeviceWriteInd - Invalid Operation");
        break;
	}
      }
    }
  else
    {
    MI_LOG_NERR1 ("MI Write Ind - DeviceWriteInd: can't find type_id, rc = %x", rc);
    mis_dev_ind->acc_rslt_tag = WR_RSLT_FAILURE;
    mis_dev_ind->failure = ARE_TYPE_UNSUPPORTED;
    _mis_dev_wr_done (mis_dev_ind);
    }
  }

/************************************************************************/
/* _mis_device_operate_ind:						*/
/************************************************************************/

ST_VOID _mis_device_operate_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind)
  {
ST_RET rc;
MIS_DEVICE *mis_device;

  mis_device = mis_dev_ind->mis_device;

  /* check access						*/
  if (mis_dev_ind->mis_device_ref & MIS_DEVREF_OPERATE)
    {
    /* let the user know					*/
    rc = u_mis_device_operate (mis_dev_ind);
    if (rc == MIS_SEND_DEFAULT_RESP)
      {
      /* do the default response				*/
      mis_device = mis_dev_ind->mis_device;
      if (mis_device->device_state == MIS_DEVICE_STATE_BUSY)
        {
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_HW_FAULT;
        _mis_dev_wr_done (mis_dev_ind);
	return;
	}
      
      if (mis_device->sbo &&
         ((mis_device->device_state != MIS_DEVICE_STATE_ARMED) ||
          (mis_device->selecter != mis_dev_ind->mis_vcc->mi_remote)))
	{
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
        _mis_dev_wr_done (mis_dev_ind);
	return;
	}

      if (!mis_device->sbo && 
         (mis_device->device_state == MIS_DEVICE_STATE_OPERATING))
	{
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
        _mis_dev_wr_done (mis_dev_ind);
	return;
	}

      mis_dev_ind->acc_rslt_tag = ACC_RSLT_SUCCESS;
      mis_device->device_state = MIS_DEVICE_STATE_IDLE;
      _mis_dev_wr_done (mis_dev_ind);
      }
    else if (rc == MIS_SEND_UNAVAIL_RESP)
      {
      mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
      mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
      _mis_dev_wr_done (mis_dev_ind);
      return;
      }
    else if (rc == MIS_SEND_DENIED_RESP)
      {
      mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
      mis_dev_ind->failure = ARE_OBJ_ACCESS_DENIED;
      _mis_dev_wr_done (mis_dev_ind);
      return;
      }
    else
      mis_device->device_state = MIS_DEVICE_STATE_OPERATING;
    }
  else		/* access denied					*/
    {
    mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
    mis_dev_ind->failure = ARE_OBJ_ACCESS_DENIED;
    _mis_dev_wr_done (mis_dev_ind);
    }
  }

/************************************************************************/
/* _mis_device_settag_ind:						*/
/************************************************************************/

ST_VOID _mis_device_settag_ind (MIS_DEVICE_IND_CTRL *mis_dev_ind)
  {
ST_RET rc;
MIS_DEVICE *mis_device;

  /* check access						*/
  if (mis_dev_ind->mis_device_ref & MIS_DEVREF_SETTAG)
    {
    /* let the user know					*/
    rc = u_mis_device_settag (mis_dev_ind);
    if (rc == MIS_SEND_DEFAULT_RESP)
      {
      /* do the default response				*/
      mis_device = mis_dev_ind->mis_device;
      if (mis_device->device_state == MIS_DEVICE_STATE_BUSY)
        {
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_HW_FAULT;
        _mis_dev_wr_done (mis_dev_ind);
	return;
	}
      
      if ((mis_device->device_state == MIS_DEVICE_STATE_ARMED) ||
          (mis_device->device_state == MIS_DEVICE_STATE_SELECTING) ||
          (mis_device->device_state == MIS_DEVICE_STATE_OPERATING))
	{
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
        _mis_dev_wr_done (mis_dev_ind);
	return;
	}

      if ((!mis_dev_ind->mis_device_ref & MIS_DEVREF_ICC) &&	/* vcc scope */
          ((ST_INT8) mis_device->tag_value.TagFlags != MI_TAG_FLAG_NO_TAG)  &&
	  (mis_device->tagger != mis_dev_ind->mis_vcc->mi_remote))
        {
	mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
	mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
        _mis_dev_wr_done (mis_dev_ind);
	return;
	}

      mis_dev_ind->acc_rslt_tag = ACC_RSLT_SUCCESS;
      memcpy (&mis_device->tag_value, mis_dev_ind->data_buf, sizeof (MI_TAG_VALUE));
      mis_device->tagger = mis_dev_ind->mis_vcc->mi_remote;
      _mis_dev_wr_done (mis_dev_ind);
      }
    else if (rc == MIS_SEND_UNAVAIL_RESP)
      {
      mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
      mis_dev_ind->failure = ARE_TEMP_UNAVAIL;
      _mis_dev_wr_done (mis_dev_ind);
      return;
      }
    }
  else		/* access denied					*/
    {
    mis_dev_ind->acc_rslt_tag = ACC_RSLT_FAILURE;
    mis_dev_ind->failure = ARE_OBJ_ACCESS_DENIED;
    _mis_dev_wr_done (mis_dev_ind);
    }
  }

/************************************************************************/
/* Functions for the MI Server to use to respond to device indications	*/
/************************************************************************/

/************************************************************************/
/* mis_device_select_done: send select device response to remote client	*/
/************************************************************************/

ST_VOID mis_device_select_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc)
  {
  MI_LOG_FLOW1 ("DeviceSelectDone for %s", dev_ind_ctrl->mis_device->device_name);
  mis_dev_rd_done (dev_ind_ctrl, rc);
  }

/************************************************************************/
/* mis_device_operate_done: send operate device response to remote client	*/
/************************************************************************/

ST_VOID mis_device_operate_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc)
  {
  MI_LOG_FLOW1 ("DeviceOperateDone for %s", dev_ind_ctrl->mis_device->device_name);
  if (rc != SD_SUCCESS)
    dev_ind_ctrl->acc_rslt_tag = ACC_RSLT_FAILURE;

  _mis_dev_wr_done (dev_ind_ctrl);
  } 

/************************************************************************/
/* mis_device_gettag_done: send get device tag response to remote client*/
/************************************************************************/

ST_VOID mis_device_gettag_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc)
  {
  MI_LOG_FLOW1 ("DeviceGetTagDone for %s", dev_ind_ctrl->mis_device->device_name);
  mis_dev_rd_done (dev_ind_ctrl, rc);
  }

/************************************************************************/
/* mis_device_settag_done: send set device tag response to remote client*/
/************************************************************************/

ST_VOID mis_device_settag_done (MIS_DEVICE_IND_CTRL *dev_ind_ctrl, ST_RET rc)
  {
  MI_LOG_FLOW1 ("DeviceSetTagDone for %s", dev_ind_ctrl->mis_device->device_name);
  if (rc != SD_SUCCESS)
    dev_ind_ctrl->acc_rslt_tag = ACC_RSLT_FAILURE;

  _mis_dev_wr_done (dev_ind_ctrl);
  }


/************************************************************************/
/* mis_dev_rd_done:		 					*/
/************************************************************************/

ST_VOID mis_dev_rd_done (MIS_DEVICE_IND_CTRL *mis_dev_ind, ST_RET rslt)
  {
MIS_READ_CTRL *mis_rd_ctrl;

  mis_rd_ctrl = mis_dev_ind->mis_rd_ctrl;

  if (rslt == SD_SUCCESS)
    mis_dev_ind->acc_rslt_tag = ACC_RSLT_SUCCESS;

  ++mis_rd_ctrl->num_data_done;
  if (mis_rd_ctrl->num_data_done == mis_rd_ctrl->num_var)
    {
    /* Time to send the response ... */
    _mis_send_read_resp (mis_rd_ctrl);
    }
  }


/************************************************************************/
/* _mi_get_device_asn1_data: 						*/
/************************************************************************/

ST_RET _mi_get_device_asn1_data (ST_VOID *data, ST_INT type_id,
                                 ST_UCHAR *asn1_dest_buffer,
                                 ST_INT asn1_buffer_len,
                                 ST_INT *asn1_len_out)
  {
RUNTIME_TYPE *rt;
ST_INT num_rt;
ST_RET rc;
ST_UCHAR *asn1_start;
ST_INT asn1_len;
ASN1_ENC_CTXT localEncCtx;	/* For readability, use "aCtx" to access this.*/
ASN1_ENC_CTXT *aCtx = &localEncCtx;

  mvl_get_runtime (type_id, &rt, &num_rt);
  asn1r_strt_asn1_bld (aCtx, asn1_dest_buffer, asn1_buffer_len);
  rc = ms_local_to_asn1 (aCtx, rt, num_rt, (char *)data);

/* When the ASN.1 encode is complete, 'asn1_field_ptr' points 1 byte ahead 	*/
/* of the start of the message. The ASN.1 message is build from back to	*/
/* front, so that is now we calculate the length of the PDU		*/
  if (rc == SD_SUCCESS)
    {
    asn1_start = aCtx->asn1r_field_ptr+1;
    asn1_len = (asn1_dest_buffer + asn1_buffer_len) - asn1_start;
    memmove (asn1_dest_buffer,asn1_start,asn1_len);
    *asn1_len_out = asn1_len;
    }
  else
    {
    MI_LOG_NERR1 ("MI Read: data conversion error: 0x%04x", rc);
    }
  return (rc);
  }


/************************************************************************/
/* _mis_dev_wr_done:							*/
/************************************************************************/

ST_VOID _mis_dev_wr_done (MIS_DEVICE_IND_CTRL *mis_dev_ind)
  {
MIS_WRITE_CTRL *mis_wr_ctrl;

  mis_wr_ctrl = mis_dev_ind->mis_wr_ctrl;

  ++mis_wr_ctrl->num_data_done;
  if (mis_wr_ctrl->num_data_done == mis_wr_ctrl->num_var)
    _mis_send_write_resp (mis_wr_ctrl);

  }

