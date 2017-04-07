/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	2001-2002, All Rights Reserved		        	*/
/*									*/
/* MODULE NAME : mi_grobj.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 04/24/02  MDE     03    Handle case where configured DV not exist	*/
/* 04/17/02  MDE     02    Added mic_reset_all_dv_mismatch, cleanup	*/
/* 04/05/02  MDE     01    Created					*/
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

static ST_VOID _mic_grobj_gnl_dv_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mic_grobj_getva_done (MVL_REQ_PEND *mvl_req);
static int _mic_grobj_name_compare (const MIC_GROBJ_DV  **dv1, const MIC_GROBJ_DV  **dv2);
static ST_RET _mic_check_dv_cfg_list (MI_REMOTE *mi_remote, 
		             MIC_GROBJ_CTRL *mi_grobj_ctrl, ST_INT scope);
static ST_VOID _mi_grobj_list_type_errs (MIC_GROBJ_DV *grobj_dv_list, 
					 ST_CHAR *scopeName, 
					 ST_INT *errCountIo);
static ST_VOID _mic_clean_rem_ds_dv (MIC_VCC *mic_vcc, MIC_DV *mic_dv);

/************************************************************************/
/************************************************************************/
/*			mic_get_rem_dv					*/
/************************************************************************/

MI_REQ_CTRL *mic_get_rem_dv (MI_CONN *mi_conn, MIC_GROBJ_CTRL *mi_grobj_ctrl, 
 			     ST_BOOLEAN get_attrib)
  {
MVL_REQ_PEND *mvl_req;
MI_REQ_CTRL *mi_req;

  S_MI_LOCK_COMMON ();

/* Start with VCC DV */
  mvl_req = _mic_send_namelist (mi_conn, MIC_OBJ_CLASS_VCC_DV, NULL);
  if (mvl_req == NULL)
    {
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }
/* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_grobj_gnl_dv_done;

/* Now do the MI_REQ_CTRL thing */
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  if (mi_grobj_ctrl == NULL)
/*    mi_grobj_ctrl = (_mic_grobj_ctrl *)chk_calloc (1, sizeof (MIC_GROBJ_CTRL));
modified 2006.11.23 zy*/
    mi_grobj_ctrl = (struct _mic_grobj_ctrl *)chk_calloc (1, sizeof (MIC_GROBJ_CTRL));

  mi_req->mi_grobj_ctrl = mi_grobj_ctrl;
  mi_grobj_ctrl->state = _MIC_GROBJ_STATE_VCCDV;
  mi_grobj_ctrl->mms_class = MIC_OBJ_CLASS_VCC_DV;
  mi_grobj_ctrl->get_attrib = get_attrib;

/* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/*			_mic_grobj_gnl_dv_done				*/
/************************************************************************/

static ST_VOID _mic_grobj_gnl_dv_done (MVL_REQ_PEND *mvl_req)
  {
MVL_REQ_PEND *new_mvl_req;
MI_REQ_CTRL *mi_req;
MI_CONN *mi_conn;
MIC_GROBJ_CTRL *mi_grobj_ctrl;
NAMELIST_RESP_INFO *gnl;
ST_CHAR **name_tbl;
MIC_GROBJ_DV *grobj_dv;
ST_INT scope;
ST_INT i;
GETVAR_REQ_INFO getva_req;
ST_RET rc;

  mi_req = mvl_req->mi_req;
  mi_conn = mi_req->mi_conn;
  mi_grobj_ctrl = mi_req->mi_grobj_ctrl;
  if (mvl_req->result != SD_SUCCESS)
    {
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    _mi_req_done (mi_req);
    return;
    }

  gnl = mvl_req->u.getnam.resp_info;

  if (mi_grobj_ctrl->mms_class == MIC_OBJ_CLASS_VCC_DV)
    scope = VCC_SPEC;
  else
    scope = ICC_SPEC;

  name_tbl = (ST_CHAR **) (gnl + 1);
  for (i = 0; i < gnl->num_names; ++i)
    {
    if (_mi_is_special_var (name_tbl[i]))
      continue;		/* This is a special var, ignore it ... */

    grobj_dv = (MIC_GROBJ_DV *)chk_calloc (1, sizeof (MIC_GROBJ_DV));
    if (scope == VCC_SPEC)
      {
      ++mi_grobj_ctrl->vcc_dv_count;
      list_add_last((ST_VOID **) &mi_grobj_ctrl->vcc_dv_list, grobj_dv);
      }
    else
      {
      ++mi_grobj_ctrl->icc_dv_count;
      list_add_last((ST_VOID **) &mi_grobj_ctrl->icc_dv_list, grobj_dv);
      }

    grobj_dv->scope = scope;
    grobj_dv->mi_type = -1;
    strcpy (grobj_dv->name, name_tbl[i]);
    }

/* See if we need to get more names for the current scope */
  if (gnl->more_follows && gnl->num_names > 0)
    {
    new_mvl_req = _mic_send_namelist (mi_conn, mi_grobj_ctrl->mms_class, name_tbl[i-1]);
    if (new_mvl_req == NULL)
      {	/* Could not send the new request */
      mic_free_grobj_ctrl (mi_grobj_ctrl);
      mi_req->result = SD_FAILURE;
      _mi_req_done (mi_req);
      return;
      }
  /* The MMS request has gone out just fine, let's catch the confirm	*/
    new_mvl_req->u_req_done = _mic_grobj_gnl_dv_done;
    new_mvl_req->mi_req = mi_req;
    mi_req->mvl_req = new_mvl_req;

  /* Done with this MVL request */
    mvl_free_req_ctrl (mvl_req);
    return;
    }

/* See if we need to get the ICC data value list			*/
  if (mi_grobj_ctrl->mms_class == MIC_OBJ_CLASS_VCC_DV)
    {
    new_mvl_req = _mic_send_namelist (mi_conn, MIC_OBJ_CLASS_ICC_DV, NULL);
    if (new_mvl_req == NULL)
      {
      _mi_req_done (mi_req);
      return;
      }
  /* The MMS request has gone out just fine, let's catch the confirm	*/
    new_mvl_req->u_req_done = _mic_grobj_gnl_dv_done;
    new_mvl_req->mi_req = mi_req;
    mi_req->mvl_req = new_mvl_req;
    mi_grobj_ctrl->state = _MIC_GROBJ_STATE_ICCDV;
    mi_grobj_ctrl->mms_class = MIC_OBJ_CLASS_ICC_DV;

  /* Done with this MVL request */
    mvl_free_req_ctrl (mvl_req);
    return;
    }

/* OK, we have a nice list of all data values, see if we are to get	*/
/* the types too ...							*/
  if (mi_grobj_ctrl->get_attrib == SD_FALSE || 
      (mi_grobj_ctrl->vcc_dv_count == 0 && mi_grobj_ctrl->icc_dv_count == 0))
    {
    _mi_req_done (mi_req);	/* Nope, we are really done		*/
    return;
    }

/* OK, we need to get the attributes of each data value */
/* Start with VCC list, if there are any ... */
  if (mi_grobj_ctrl->vcc_dv_count != 0)
    {
    grobj_dv = mi_grobj_ctrl->vcc_dv_list;
    mi_grobj_ctrl->state = _MIC_GROBJ_STATE_VCC_VA;
    }
  else
    {
    grobj_dv = mi_grobj_ctrl->icc_dv_list;
    mi_grobj_ctrl->state = _MIC_GROBJ_STATE_ICC_VA;
    }

  getva_req.req_tag = GETVAR_NAME;
  getva_req.name.object_tag = grobj_dv->scope;
  getva_req.name.obj_name.vmd_spec = grobj_dv->name;
  if (grobj_dv->scope == ICC_SPEC)
    getva_req.name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;

  rc = mvla_getvar (mi_conn->net_info, &getva_req, &new_mvl_req);
  if (rc != SD_SUCCESS)
    {
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    mvl_free_req_ctrl (new_mvl_req);
    mi_req->result = SD_FAILURE;
    _mi_req_done (mi_req);
    return;
    }

/* Done with this MVL request */
  mvl_free_req_ctrl (mvl_req);

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  new_mvl_req->u_req_done = _mic_grobj_getva_done;
  new_mvl_req->mi_req = mi_req;
  mi_req->mvl_req = new_mvl_req;
  mi_grobj_ctrl->curr_dv = grobj_dv;
  }

/************************************************************************/
/*			_mic_grobj_getva_done				*/
/************************************************************************/

static ST_VOID _mic_grobj_getva_done (MVL_REQ_PEND *mvl_req)
  {
MVL_REQ_PEND *new_mvl_req;
MI_REQ_CTRL *mi_req;
MI_CONN *mi_conn;
MIC_GROBJ_CTRL *mi_grobj_ctrl;
MIC_GROBJ_DV *grobj_dv;
GETVAR_REQ_INFO getva_req;
GETVAR_RESP_INFO *getva_resp;
ST_INT errCount;
ST_RET rc;

  mi_req = mvl_req->mi_req;
  mi_conn = mi_req->mi_conn;
  mi_grobj_ctrl = mi_req->mi_grobj_ctrl;
  if (mvl_req->result != SD_SUCCESS)
    {
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    mi_req->result = SD_FAILURE;
    _mi_req_done (mi_req);
    return;
    }

  grobj_dv = mi_grobj_ctrl->curr_dv;

/* Handle the confirm information */
  getva_resp = mvl_req->u.getvar.resp_info;
  grobj_dv->mi_type = _mi_asn1type_to_mi_type (getva_resp->type_spec.len,
					    getva_resp->type_spec.data);
  if (grobj_dv->mi_type > MI_LAST_DV_TYPEID)
    grobj_dv->mi_type *= -1;	/* make it negative */

/* See if we need to get more VA attributes */
  if (mi_grobj_ctrl->state == _MIC_GROBJ_STATE_VCC_VA)
    {
    grobj_dv = (MIC_GROBJ_DV *) list_get_next (mi_grobj_ctrl->vcc_dv_list, grobj_dv);
    if (grobj_dv == NULL)
      {
      mi_grobj_ctrl->state = _MIC_GROBJ_STATE_ICC_VA;
      grobj_dv = mi_grobj_ctrl->icc_dv_list;
      }
    }
  else
    grobj_dv = (MIC_GROBJ_DV *) list_get_next (mi_grobj_ctrl->icc_dv_list, grobj_dv);

  if (grobj_dv == NULL)
    {
  /* OK, we are done getting attributes, now we can list errors */
    errCount = SD_TRUE;
    _mi_grobj_list_type_errs (mi_grobj_ctrl->vcc_dv_list, "VCC", &errCount);
    _mi_grobj_list_type_errs (mi_grobj_ctrl->icc_dv_list, "ICC", &errCount);
    _mi_req_done (mi_req);
    return;
    }

  getva_req.req_tag = GETVAR_NAME;

  getva_req.name.object_tag = grobj_dv->scope;
  getva_req.name.obj_name.vmd_spec = grobj_dv->name;
  if (grobj_dv->scope == ICC_SPEC)
    getva_req.name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;

  rc = mvla_getvar (mi_conn->net_info, &getva_req, &new_mvl_req);
  if (rc != SD_SUCCESS)
    {
    mvl_free_req_ctrl (new_mvl_req);
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    mi_req->result = SD_FAILURE;
    _mi_req_done (mi_req);
    return;
    }

/* Done with this MVL request */
  mvl_free_req_ctrl (mvl_req);

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  new_mvl_req->u_req_done = _mic_grobj_getva_done;
  new_mvl_req->mi_req = mi_req;
  mi_req->mvl_req = new_mvl_req;
  mi_grobj_ctrl->curr_dv = grobj_dv;
  }


/************************************************************************/
/*			_mi_asn1type_to_mi_type				*/
/************************************************************************/

#define	MAX_MI_TYPE_RT		20

ST_INT _mi_asn1type_to_mi_type (ST_INT asn1_len, ST_UCHAR *asn1)
  {
RUNTIME_TYPE *asn1_rt_tbl;
ST_INT asn1_rt_count;
RUNTIME_TYPE *mi_type_rt;
ST_INT num_mi_rt;
ST_BOOLEAN match;
ST_BOOLEAN exact_match;
ST_INT mi_type;
ST_INT found_mi_type;
ST_INT type_id;

/*  asn1_rt_tbl = (runtime_type *)chk_calloc (MAX_MI_TYPE_RT, sizeof (RUNTIME_TYPE));
modified 2006.11.23 zy*/
  asn1_rt_tbl = (struct runtime_type *)chk_calloc (MAX_MI_TYPE_RT, sizeof (RUNTIME_TYPE));
  asn1_rt_count = ms_asn1_to_runtime (asn1, asn1_len, asn1_rt_tbl, MAX_MI_TYPE_RT);
  if (asn1_rt_count <= 0)
    {
    MI_LOG_NERR0 ("Could not convert ASN.1 to type");
    return (-1);
    }

  found_mi_type = -1;
  for (mi_type = 0; mi_type < MI_NUM_MI_TYPES; ++mi_type)
    {
    type_id = mi_type_to_type_id_tbl[mi_type];
    if (type_id < 0)
      continue;

    mvl_get_runtime (type_id, &mi_type_rt, &num_mi_rt);
    _mi_cmp_rt_types (asn1_rt_tbl, asn1_rt_count, 
    		      mi_type_rt, num_mi_rt, &match, &exact_match);

    if (match == SD_TRUE)
      {
     /* If first match or exact match, save the mi_type */
      if (found_mi_type == -1 || exact_match == SD_TRUE)
	found_mi_type = mi_type;

     /* If exact, time to stop looking */
      if (exact_match == SD_TRUE)
        break;
      }
    }
  chk_free (asn1_rt_tbl);
  return (found_mi_type);
  }


/************************************************************************/
/*			_mi_grobj_list_type_errs			*/
/************************************************************************/

static ST_VOID _mi_grobj_list_type_errs (MIC_GROBJ_DV *grobj_dv_list, 
					 ST_CHAR *scopeName, 
					 ST_INT *errCountIo)
  {
MIC_GROBJ_DV *grobj_dv;
ST_INT mi_type;
ST_INT errCount;

  errCount = *errCountIo;
  grobj_dv = grobj_dv_list;
  while (grobj_dv != NULL)
    {
    mi_type = grobj_dv->mi_type;
    if (mi_type == -1)
      {
      if (errCount++ == 0)
        {
        MI_LOG_NERR2 ("Unknown type for %s DV '%s'", scopeName, grobj_dv->name);
	}
      else
        {
        MI_LOG_CNERR2 ("Unknown type for %s DV '%s'", scopeName, grobj_dv->name);
	}
      }
    if (mi_type < -1)
      {
      if (errCount++ == 0)
	{
        MI_LOG_NERR3 ("%s Variable '%s' has non-DV type %s", 
    		scopeName, grobj_dv->name, 
    		mi_type_to_string (mi_type * -1));
	}
      else
	{
        MI_LOG_CNERR3 ("%s Variable '%s' has non-DV type %s", 
    		scopeName, grobj_dv->name, 
    		mi_type_to_string (mi_type * -1));
        }
      }
    grobj_dv = (MIC_GROBJ_DV *)list_get_next (grobj_dv_list, grobj_dv);
    }
  *errCountIo = errCount;
  }

/************************************************************************/
/*			_mi_cmp_rt_types				*/
/************************************************************************/

ST_VOID _mi_cmp_rt_types (RUNTIME_TYPE *rt1, ST_INT num_rt1, 
			  RUNTIME_TYPE *rt2, ST_INT num_rt2,
			  ST_BOOLEAN *matchOut,
			  ST_BOOLEAN *exactMatchOut)
  {
ST_BOOLEAN match;
ST_BOOLEAN exact_match;
ST_INT i;

  if (num_rt1 != num_rt2)
    {
    *matchOut = SD_FALSE;
    *exactMatchOut = SD_FALSE;
    return;
    }

  match = SD_TRUE;
  exact_match = SD_TRUE;
  for (i = 0; i < num_rt1 && match == SD_TRUE; ++i, ++rt1, ++rt2)
    {
    if (rt1->el_tag != rt2->el_tag)
      {
      match = SD_FALSE;
      break;
      }
    if (rt1->name_index != rt2->name_index)
      exact_match = SD_FALSE;

    switch (rt1->el_tag)
      {
      case RT_ARR_START :
      case RT_ARR_END :			/* array done			*/
	if (rt1->u.arr.num_rt_blks != rt2->u.arr.num_rt_blks)
	  match = SD_FALSE;
      break;

      case RT_STR_START :
      case RT_STR_END :			/* structure done		*/
	if (rt1->u.str.num_rt_blks != rt2->u.str.num_rt_blks)
	  match = SD_FALSE;
      break;

      case RT_BOOL :
      case RT_BIT_STRING :			
      case RT_INTEGER :			
      case RT_UNSIGNED : 			
      case RT_FLOATING_POINT : 		
      case RT_OCTET_STRING : 		
      case RT_VISIBLE_STRING : 		
#ifdef     TIME_DATA_SUPPORT
      case RT_GENERAL_TIME : 		
#endif    
#ifdef     BTOD_DATA_SUPPORT
      case RT_BINARY_TIME : 		
#endif    
      case RT_BCD : 			
      case RT_UTC_TIME : 		
	    if (rt1->u.p.el_len != rt2->u.p.el_len)
	      match = SD_FALSE;
      break;
      }
    }

  *matchOut = match;
  *exactMatchOut = exact_match;
  }

/************************************************************************/
/************************************************************************/
/*			mic_free_grobj_ctrl				*/
/************************************************************************/

ST_VOID mic_free_grobj_ctrl (MIC_GROBJ_CTRL *mi_grobj_ctrl)
  {
MIC_GROBJ_DV *grobj_dv;

  while (mi_grobj_ctrl->vcc_dv_list != NULL)
    {
    grobj_dv = mi_grobj_ctrl->vcc_dv_list;
    list_unlink ((ST_VOID **) &mi_grobj_ctrl->vcc_dv_list, grobj_dv);
    chk_free (grobj_dv);
    }
  while (mi_grobj_ctrl->icc_dv_list != NULL)
    {
    grobj_dv = mi_grobj_ctrl->icc_dv_list;
    list_unlink ((ST_VOID **) &mi_grobj_ctrl->icc_dv_list, grobj_dv);
    chk_free (grobj_dv);
    }
  chk_free (mi_grobj_ctrl);
  }

/************************************************************************/
/************************************************************************/
/*			mic_check_dv_cfg					*/
/************************************************************************/

ST_RET mic_check_dv_cfg (MI_REMOTE *mi_remote, MIC_GROBJ_CTRL *mi_grobj_ctrl)
  {
ST_RET rc;

  rc = _mic_check_dv_cfg_list (mi_remote, mi_grobj_ctrl, ICC_SPEC);
  rc |= _mic_check_dv_cfg_list (mi_remote, mi_grobj_ctrl, VCC_SPEC);
  return (rc);
  }

/************************************************************************/
/*			_mic_check_dv_cfg_list				*/
/************************************************************************/

static ST_RET _mic_check_dv_cfg_list (MI_REMOTE *mi_remote, 
		              MIC_GROBJ_CTRL *mi_grobj_ctrl, ST_INT scope)
  {
MIC_VCC  *mic_vcc;
MIC_GROBJ_DV *grobj_dv_list;
MIC_GROBJ_DV **sorted_grobj_dv;
MIC_GROBJ_DV *grobj_dv;
ST_INT grobj_dv_count;
ST_INT   cfg_dv_count;
MIC_DV  **cfg_dv_tbl;
MIC_DV  *mic_dv;
ST_INT grobj_dv_index;
ST_INT mic_dv_type_id;
ST_CHAR *mic_dv_type_str;
ST_CHAR *grobj_dv_type_str;

ST_BOOLEAN match;
ST_BOOLEAN exact_match;
RUNTIME_TYPE *micdv_rt;
ST_INT micdv_num_rt;
RUNTIME_TYPE *grdv_rt;
ST_INT grdv_num_rt;
ST_INT cmp;
ST_INT i;
ST_INT rc;
ST_INT mismatch_count;

/* NOTE: The MIC_VCC must have it's objects sorted */
  mic_vcc = mi_remote->mic_vcc;
  
/* Depending on the scope, pick one list or another */
  if (scope == VCC_SPEC)
    {
    grobj_dv_list = mi_grobj_ctrl->vcc_dv_list;
    grobj_dv_count = mi_grobj_ctrl->vcc_dv_count;
    cfg_dv_count = mic_vcc->num_vcc_dv;
    cfg_dv_tbl = mic_vcc->vcc_dv_tbl;
    }
  else
    {
    grobj_dv_list = mi_grobj_ctrl->icc_dv_list;
    grobj_dv_count = mi_grobj_ctrl->icc_dv_count;
    cfg_dv_count = mic_vcc->num_icc_dv;
    cfg_dv_tbl = mic_vcc->icc_dv_tbl;
    }

  if (grobj_dv_count == 0)
    return (SD_SUCCESS);
    
/* Allocate a table of pointers */
  sorted_grobj_dv = (MIC_GROBJ_DV **)chk_calloc (grobj_dv_count, sizeof (MIC_GROBJ_DV *));

/* Fill up the table from the list */
  i = 0;
  grobj_dv = grobj_dv_list;
  while (grobj_dv != NULL)
    {
    sorted_grobj_dv[i] = grobj_dv;
    ++i;
    grobj_dv = (MIC_GROBJ_DV *)list_get_next (grobj_dv_list, grobj_dv);
    }

/* OK, now we sort the table */
  qsort (sorted_grobj_dv, grobj_dv_count, sizeof (MIC_GROBJ_DV *), 
	 (int(*)(const void *, const void *))_mic_grobj_name_compare);

/* OK, now go through the MIC_VCC and see if all the configured 	*/
/* data values are in the list.						*/

  mismatch_count = 0;
  rc = SD_SUCCESS;		/* Plan on success */
  grobj_dv_index = 0;
  for (i = 0; i < cfg_dv_count; ++i)
    {
    mic_dv = cfg_dv_tbl[i];
    mic_dv->dv_attrib &= ~MIC_DV_MISMATCH;

    if (_mi_is_special_var (mic_dv->dv_name))
      continue;		/* This is a special var, ignore it ... */

    cmp = 1;
    while (grobj_dv_index < grobj_dv_count)
      {
      grobj_dv = sorted_grobj_dv[grobj_dv_index];
      cmp = strcmp (mic_dv->dv_name, grobj_dv->name);

      if (cmp < 0)
	break;  /* We won't find the configured DV in the list */

      ++grobj_dv_index;  /* configured is <= list */
      if (cmp == 0)
        break;
      }

    if (cmp == 0)
      {
      if (mi_grobj_ctrl->get_attrib)
        {
	mic_dv_type_id = mic_dv->dv_attrib & MIC_DV_MI_TYPE_MASK;
	if (mic_dv_type_id != grobj_dv->mi_type)
	  {
          if (((mic_dv_type_id != MI_TYPEID_STATE_Q && 
                mic_dv_type_id != MI_TYPEID_STATE)) || 
              ((grobj_dv->mi_type != MI_TYPEID_STATE_Q && 
               grobj_dv->mi_type != MI_TYPEID_STATE)))
	    {
            mvl_get_runtime (mic_dv_type_id, &micdv_rt, &micdv_num_rt);
            mvl_get_runtime (grobj_dv->mi_type, &grdv_rt, &grdv_num_rt);
            _mi_cmp_rt_types (micdv_rt, micdv_num_rt, grdv_rt, grdv_num_rt,
	    		    &match, &exact_match);
  	    if (match == SD_FALSE)
	      {
	      rc = SD_FAILURE;
              mic_dv_type_str = mi_type_to_string (mic_dv_type_id);
              grobj_dv_type_str = mi_type_to_string (grobj_dv->mi_type);
              mic_dv->dv_attrib |= MIC_DV_MISMATCH;
	      if (mismatch_count++ == 0)
	        {
  	        MI_LOG_NERR1 ("Configured DV '%s' type mismatch", mic_dv->dv_name);
	        }
	      else
	        {
  	        MI_LOG_CNERR1 ("Configured DV '%s' type mismatch", mic_dv->dv_name);
	        }
  	      MI_LOG_CNERR1 ("  Configured: '%s'", mic_dv_type_str);
  	      MI_LOG_CNERR1 ("  Actual    : '%s'", grobj_dv_type_str);
	      }
	    }
	  }
	}
      }
    else
      {
      rc = SD_FAILURE;
      mic_dv->dv_attrib |= MIC_DV_MISMATCH;
      if (mismatch_count++ == 0)
        {
        MI_LOG_NERR1 ("Configured DV '%s' not found in remote", 
      						mic_dv->dv_name);
	}
      else
        {
        MI_LOG_CNERR1 ("Configured DV '%s' not found in remote", 
      						mic_dv->dv_name);
	}
      }
    }
        
/* Time to free the table of pointers */
  chk_free (sorted_grobj_dv);
  return (rc);
  }

/************************************************************************/
/*			_mic_grobj_name_compare				*/
/************************************************************************/

static int _mic_grobj_name_compare (const MIC_GROBJ_DV  **dv1, const MIC_GROBJ_DV  **dv2)
  {
  return (strcmp ((*dv1)->name, (*dv2)->name));
  }


/************************************************************************/
/************************************************************************/
/*			mic_reset_all_dv_mismatch			*/
/************************************************************************/
/* This function resets the MIC_DV_MISMATCH flag in all remote DV	*/

ST_VOID mic_reset_all_dv_mismatch (MI_REMOTE *mi_remote)
  {
MIC_VCC *mic_vcc;
MIC_DV  *mic_dv;
ST_INT  i;

  mic_vcc = mi_remote->mic_vcc;
  for (i = 0; i < mic_vcc->num_vcc_dv; ++i)
    {
    mic_dv = mic_vcc->vcc_dv_tbl[i];
    mic_dv->dv_attrib &= ~ MIC_DV_MISMATCH;
    }
  for (i = 0; i < mic_vcc->num_icc_dv; ++i)
    {
    mic_dv = mic_vcc->icc_dv_tbl[i];
    mic_dv->dv_attrib &= ~ MIC_DV_MISMATCH;
    }
  }

/************************************************************************/
/************************************************************************/
/* Utility Functions							*/
/************************************************************************/

typedef struct
  {
  ST_CHAR *name;
  ST_INT mi_type;
  } MI_TYPE_TO_STRING;

MI_TYPE_TO_STRING miTypeToString[] = 
  {
    {"REAL", MI_TYPEID_REAL},
    {"STATE", MI_TYPEID_STATE},
    {"DISCRETE", MI_TYPEID_DISCRETE},
    {"REAL_Q", MI_TYPEID_REAL_Q},
    {"STATE_Q", MI_TYPEID_STATE_Q},
    {"DISCRETE_Q", MI_TYPEID_DISCRETE_Q},
    {"REAL_Q_TIMETAG", MI_TYPEID_REAL_Q_TIMETAG},
    {"STATE_Q_TIMETAG", MI_TYPEID_STATE_Q_TIMETAG},
    {"DISCRETE_Q_TIMETAG", MI_TYPEID_DISCRETE_Q_TIMETAG},
    {"REAL_EXTENDED", MI_TYPEID_REAL_EXTENDED},
    {"STATE_EXTENDED", MI_TYPEID_STATE_EXTENDED},
    {"DISCRETE_EXTENDED", MI_TYPEID_DISCRETE_EXTENDED},
    {"DV_SPECIAL", MI_TYPEID_DV_SPECIAL},
    {"DV_RESERVED_1", MI_TYPEID_DV_RESERVED_1},
    {"DV_RESERVED_2", MI_TYPEID_DV_RESERVED_2},
    {"DV_RESERVED_3", MI_TYPEID_DV_RESERVED_3},
    {"SUPPORTED_FEATURES", MI_TYPEID_SUPPORTED_FEATURES},
    {"VISIBLE_STRING_32", MI_TYPEID_VISIBLE_STRING_32},
    {"TASE2_VERSION", MI_TYPEID_TASE2_VERSION},
    {"MMS_OBJECT_NAME", MI_TYPEID_MMS_OBJECT_NAME},
    {"DSTS", MI_TYPEID_DSTS},
    {"GMT_BASED_S", MI_TYPEID_GMT_BASED_S},
    {"INT16", MI_TYPEID_INT16},
    {"DS_COND", MI_TYPEID_DS_COND}
  };

/************************************************************************/
/*			mi_type_to_string				*/
/************************************************************************/

ST_CHAR *mi_type_to_string (ST_INT mi_type)		
  {								
ST_INT i;							
								
  for (i = 0; miTypeToString[i].name != NULL; ++i)		
    {								
    if (miTypeToString[i].mi_type == mi_type)		
      return (miTypeToString[i].name);			
    }								
  return ("<Unknown>");
  } 								

/************************************************************************/
/************************************************************************/
static ST_VOID _mic_grds_gnl_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mic_grobj_get_vl_done (MVL_REQ_PEND *mvl_req);
/************************************************************************/
/*			mic_get_rem_ds					*/
/************************************************************************/

MI_REQ_CTRL *mic_get_rem_ds (MI_CONN *mi_conn, MIC_GROBJ_CTRL *mi_grobj_ctrl, 
 			     ST_BOOLEAN get_attrib)
  {
MVL_REQ_PEND *mvl_req;
MI_REQ_CTRL *mi_req;

  S_MI_LOCK_COMMON ();

/* Start with VCC DV */
  mvl_req = _mic_send_namelist (mi_conn, MIC_OBJ_CLASS_VCC_DS, NULL);
  if (mvl_req == NULL)
    {
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }
/* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_grds_gnl_done;

/* Now do the MI_REQ_CTRL thing */
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  if (mi_grobj_ctrl == NULL)
/*    mi_grobj_ctrl = (_mic_grobj_ctrl *)chk_calloc (1, sizeof (MIC_GROBJ_CTRL));
modified 2006.11.23 zy*/
    mi_grobj_ctrl = (struct _mic_grobj_ctrl *)chk_calloc (1, sizeof (MIC_GROBJ_CTRL));

  mi_req->mi_grobj_ctrl = mi_grobj_ctrl;
  mi_grobj_ctrl->state = _MIC_GROBJ_STATE_VCCDS;
  mi_grobj_ctrl->mms_class = MIC_OBJ_CLASS_VCC_DS;
  mi_grobj_ctrl->get_attrib = get_attrib;

/* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/*			_mic_grds_gnl_done				*/
/************************************************************************/

static ST_VOID _mic_grds_gnl_done (MVL_REQ_PEND *mvl_req)
  {
MVL_REQ_PEND *new_mvl_req;
MI_REQ_CTRL *mi_req;
MI_CONN *mi_conn;
NAMELIST_RESP_INFO *gnl;
GETVLIST_REQ_INFO getVl;
MIC_GROBJ_CTRL *mi_grobj_ctrl;
ST_CHAR **name_tbl;
MIC_GROBJ_DS *grobj_ds;
ST_INT scope;
ST_INT i;
ST_RET rc;

  mi_req = mvl_req->mi_req;
  mi_conn = mi_req->mi_conn;
  mi_grobj_ctrl = mi_req->mi_grobj_ctrl;
  if (mvl_req->result != SD_SUCCESS)
    {
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    mi_req->result = SD_FAILURE;
    _mi_req_done (mi_req);
    return;
    }
  gnl = mvl_req->u.getnam.resp_info;

  if (mi_grobj_ctrl->mms_class == MIC_OBJ_CLASS_VCC_DS)
    scope = VCC_SPEC;
  else
    scope = ICC_SPEC;

  name_tbl = (ST_CHAR **) (gnl + 1);
  for (i = 0; i < gnl->num_names; ++i)
    {
    grobj_ds = (MIC_GROBJ_DS *)chk_calloc (1, sizeof (MIC_GROBJ_DS));
    if (scope == VCC_SPEC)
      {
      ++mi_grobj_ctrl->vcc_ds_count;
      list_add_last((ST_VOID **) &mi_grobj_ctrl->vcc_ds_list, grobj_ds);
      }
    else
      {
      ++mi_grobj_ctrl->icc_ds_count;
      list_add_last((ST_VOID **) &mi_grobj_ctrl->icc_ds_list, grobj_ds);
      }

    grobj_ds->scope = scope;
    strcpy (grobj_ds->name, name_tbl[i]);
    }

/* See if we need to get more names for the current scope */
  if (gnl->more_follows && gnl->num_names > 0)
    {
    new_mvl_req = _mic_send_namelist (mi_conn, mi_grobj_ctrl->mms_class, name_tbl[i-1]);
    if (new_mvl_req == NULL)
      {	/* Could not send the new request */
      mic_free_grobj_ctrl (mi_grobj_ctrl);
      mi_req->result = SD_FAILURE;
      _mi_req_done (mi_req);
      return;
      }
  /* The MMS request has gone out just fine, let's catch the confirm	*/
    new_mvl_req->u_req_done = _mic_grds_gnl_done;
    new_mvl_req->mi_req = mi_req;
    mi_req->mvl_req = new_mvl_req;

  /* Done with this MVL request */
    mvl_free_req_ctrl (mvl_req);
    return;
    }

/* See if we need to get the ICC data value list			*/
  if (mi_grobj_ctrl->mms_class == MIC_OBJ_CLASS_VCC_DS)
    {
    new_mvl_req = _mic_send_namelist (mi_conn, MIC_OBJ_CLASS_ICC_DS, NULL);
    if (new_mvl_req == NULL)
      {
      mic_free_grobj_ctrl (mi_grobj_ctrl);
      mi_req->result = SD_FAILURE;
      _mi_req_done (mi_req);
      return;
      }
  /* The MMS request has gone out just fine, let's catch the confirm	*/
    new_mvl_req->u_req_done = _mic_grds_gnl_done;
    new_mvl_req->mi_req = mi_req;
    mi_req->mvl_req = new_mvl_req;
    mi_grobj_ctrl->state = _MIC_GROBJ_STATE_ICCDS;
    mi_grobj_ctrl->mms_class = MIC_OBJ_CLASS_ICC_DS;

  /* Done with this MVL request */
    mvl_free_req_ctrl (mvl_req);
    return;
    }

/* OK, we have a nice list of all data sets, see if we are to get	*/
/* the SD attributes too ...							*/
  if (mi_grobj_ctrl->get_attrib == SD_FALSE || 
      (mi_grobj_ctrl->vcc_ds_count == 0 && mi_grobj_ctrl->icc_ds_count == 0))
    {
    _mi_req_done (mi_req);	/* Nope, we are really done		*/
    return;
    }

/* OK, we need to get the attributes of each data value */
/* Start with VCC list, if there are any ... */
  if (mi_grobj_ctrl->vcc_ds_count != 0)
    {
    grobj_ds = mi_grobj_ctrl->vcc_ds_list;
    mi_grobj_ctrl->state = _MIC_GROBJ_STATE_VCC_DSA;
    }
  else
    {
    grobj_ds = mi_grobj_ctrl->icc_ds_list;
    mi_grobj_ctrl->state = _MIC_GROBJ_STATE_ICC_DSA;
    }

  getVl.vl_name.object_tag = grobj_ds->scope;
  if (grobj_ds->scope == ICC_SPEC)
    getVl.vl_name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
  getVl.vl_name.obj_name.vmd_spec = grobj_ds->name;

  rc = mvla_getvlist (mi_conn->net_info, &getVl, &new_mvl_req);
  if (rc != SD_SUCCESS)
    {
    mvl_free_req_ctrl (new_mvl_req);
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    mi_req->result = SD_FAILURE;
    _mi_req_done (mi_req);
    return;
    }

/* Done with this MVL request */
  mvl_free_req_ctrl (mvl_req);

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  new_mvl_req->u_req_done = _mic_grobj_get_vl_done;
  new_mvl_req->mi_req = mi_req;
  mi_req->mvl_req = new_mvl_req;
  mi_grobj_ctrl->curr_ds = grobj_ds;
  }

/************************************************************************/
/*			_mic_grobj_get_vl_done				*/
/************************************************************************/

static ST_VOID _mic_grobj_get_vl_done (MVL_REQ_PEND *mvl_req)
  {
MVL_REQ_PEND *new_mvl_req;
MI_REQ_CTRL *mi_req;
MI_CONN *mi_conn;
MIC_GROBJ_CTRL *mi_grobj_ctrl;
MIC_GROBJ_DS *grobj_ds;
GETVLIST_REQ_INFO getVl;
GETVLIST_RESP_INFO *getVlResp;
ST_RET rc;

  mi_req = mvl_req->mi_req;
  mi_conn = mi_req->mi_conn;
  mi_grobj_ctrl = mi_req->mi_grobj_ctrl;
  if (mvl_req->result != SD_SUCCESS)
    {
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    mi_req->result = SD_FAILURE;
    _mi_req_done (mi_req);
    return;
    }

  grobj_ds = mi_grobj_ctrl->curr_ds;

/* Handle the confirm information */
  getVlResp = mvl_req->u.getvlist.resp_info;


/* See if we need to get more VA attributes */
  if (mi_grobj_ctrl->state == _MIC_GROBJ_STATE_VCC_DSA)
    {
    grobj_ds = (MIC_GROBJ_DS *) list_get_next (mi_grobj_ctrl->vcc_ds_list, grobj_ds);
    if (grobj_ds == NULL)
      {
      mi_grobj_ctrl->state = _MIC_GROBJ_STATE_ICC_DSA;
      grobj_ds = mi_grobj_ctrl->icc_ds_list;
      }
    }
  else
    grobj_ds = (MIC_GROBJ_DS *) list_get_next (mi_grobj_ctrl->icc_ds_list, grobj_ds);

  if (grobj_ds == NULL)
    {
    _mi_req_done (mi_req);
    return;
    }

  getVl.vl_name.object_tag = grobj_ds->scope;
  if (grobj_ds->scope == ICC_SPEC)
    getVl.vl_name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
  getVl.vl_name.obj_name.vmd_spec = grobj_ds->name;

  rc = mvla_getvlist (mi_conn->net_info, &getVl, &new_mvl_req);
  if (rc != SD_SUCCESS)
    {
    mvl_free_req_ctrl (new_mvl_req);
    mic_free_grobj_ctrl (mi_grobj_ctrl);
    mi_req->result = SD_FAILURE;
    _mi_req_done (mi_req);
    return;
    }

/* Done with this MVL request */
  mvl_free_req_ctrl (mvl_req);

  /* The MMS request has gone out just fine, let's catch the confirm	*/
  new_mvl_req->u_req_done = _mic_grobj_get_vl_done;
  new_mvl_req->mi_req = mi_req;
  mi_req->mvl_req = new_mvl_req;
  mi_grobj_ctrl->curr_ds = grobj_ds;
  }


