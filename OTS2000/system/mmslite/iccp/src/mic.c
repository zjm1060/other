/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mic.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/08/02  NAV     32    Delete mic_ds on create_ds not clean_assoc   */
/* 07/05/02  NAV     31    Add a couple S_MI_UNLOCK_COMMON              */
/* 06/20/02  RKR     30    Copyright year update                        */
/* 06/18/02  MDE     29    Fixed crash for get DV attrib error response	*/
/* 04/24/02  MDE     28    Return better error codes for read errors	*/
/* 04/17/02  MDE     27    Minor logging changes			*/
/* 04/05/02  MDE     26    Moved grobj stuff to (new) mi_grobj.c  	*/
/* 04/05/02  MDE     25    Added mic_define_ds				*/
/* 04/05/02  MDE     24    Added more FLOW logging			*/
/* 04/05/02  MDE     23    Added mi_get_rem_dv, mi_check_rem_dv_list	*/
/* 04/05/02  MDE     22    Fixed leaks for request on inactive conn	*/
/* 04/05/02  MDE     21    Cleaned up abort handling, other misc	*/
/* 03/25/02  MDE     20    Fixed read free'd memory: abort after rel	*/
/* 03/21/02  MDE     19    Corrected check of supported features	*/
/* 03/21/02  MDE     18    Fixed possible crash for untimely disconnect	*/
/* 02/25/02  MDE     17    Now get max PDU size from mvl_cfg_info	*/
/* 01/17/02  NAV     16    mic_start_done: dont disable assoc on param	*/
/*			    mismatch					*/
/* 01/17/02  ASK     15    Made changes for asn1r 			*/
/* 10/23/01  NAV     14    Modify clean to watch for active associations*/
/* 11/15/01  MDE     13	   Minor logging changes			*/
/* 11/07/01  MDE     12	   Minor logging changes			*/
/* 10/29/01  MDE     11	   minor cleanup				*/
/* 10/29/01  MDE     10	   Added config logging				*/
/* 10/19/01  MDE     09	   Fixed log that could cause crash		*/
/* 06/12/01  ASK     08    fixed minor typos				*/
/* 08/26/00  RKR     07    return from error cond in _mi_del_nvl_done   */
/* 07/06/00  NAV     06    Add MISEM support, mic_delete_ds		*/
/* 04/19/00  RKR     05    SMEM changes					*/
/* 11/21/99  NAV     04    Add mi_op_err support			*/
/* 10/04/99  NAV     03    Add Block 4 and Block 5 Support		*/
/* 04/01/99  MDE     02    Added MI_ASSOC_CTRL and associated code	*/
/* 12/31/98  MDE     01    Created					*/
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

MIC_DSTS *mic_find_dsts_by_asn1_name (MIC_VCC *mic_vcc, ACCESS_RESULT *ar_ptr);
int _mic_dv_name_compare (const MIC_DV **dv1, const MIC_DV **dv2);
static ST_VOID _mic_read_mvl_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mi_create_ds_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mi_delete_ds_done (MVL_REQ_PEND *mvl_req);
static ST_RET  _mic_read (MI_CONN *mi_conn, MI_READ_REQ *mi_rd_req, 
		  	  MVL_REQ_PEND **mvl_req_out);
static ST_VOID _mic_read_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mic_write_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mi_del_nvl_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mic_start_done (MI_READ_REQ *mi_rd_req);
ST_VOID _mic_process_read_conf (MVL_REQ_PEND *mvl_req);
ST_RET _mic_del_nvl (MI_CONN *mi_conn, OBJECT_NAME *nvl, 
		  MVL_REQ_PEND **mvl_req_out);
static ST_VOID _mi_rd_next_dsts_done (MI_READ_REQ *mi_rd_req);
static ST_VOID _mi_start_dsts_done (MI_WRITE_REQ *mi_wr_req);
static ST_VOID _mic_write_dv_done (MI_WRITE_REQ *mi_wr_req);
static ST_VOID _mi_stop_dsts_done (MI_WRITE_REQ *mi_wr_req);
static ST_VOID _mic_get_namelist_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mic_getvlist_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mic_get_dv_attrib_done (MVL_REQ_PEND *mvl_req);

/************************************************************************/
/*			mic_create_vcc 					*/
/************************************************************************/

MIC_VCC *mic_create_vcc (MI_REMOTE *mi_remote, 
		       ST_CHAR *bi_lat_tbl_id, ST_CHAR *icc_name, 
  		       ST_UINT num_vcc_dv, ST_INT num_vcc_ds, 
  		       ST_UINT num_icc_dv, ST_INT num_icc_ds, 
		       ST_UINT num_dsts_ctrl)
  {
MIC_VCC *mic_vcc;

  S_MI_LOCK_COMMON ();

/* Allocate the VCC control and associated tables */
/*  mic_vcc = (_mic_vcc *)chk_calloc (1, sizeof (MIC_VCC));
modified 2006.11.23 zy*/
  mic_vcc = (struct _mic_vcc *)chk_calloc (1, sizeof (MIC_VCC));
  mi_remote->mic_vcc = mic_vcc;
  mic_vcc->mi_remote = mi_remote;
  mic_vcc->icc_name = icc_name;

  mic_vcc->max_vcc_dv = num_vcc_dv;
  if (num_vcc_dv)
/*    mic_vcc->vcc_dv_tbl = (_mic_dv **)chk_calloc (num_vcc_dv, sizeof (MIC_DV *));
modified 2006.11.23 zy*/
    mic_vcc->vcc_dv_tbl = (struct _mic_dv **)chk_calloc (num_vcc_dv, sizeof (MIC_DV *));

  mic_vcc->max_icc_dv = num_icc_dv;
  if (num_icc_dv)
    mic_vcc->icc_dv_tbl = (struct _mic_dv **)chk_calloc (num_icc_dv, sizeof (MIC_DV *));

  mic_vcc->max_vcc_ds = num_vcc_ds;
  if (num_vcc_ds)
    mic_vcc->vcc_ds_ctrl_tbl = (struct _mic_ds **)chk_calloc (num_vcc_ds, sizeof (MIC_DS *));	

  mic_vcc->max_icc_ds = num_icc_ds;
  if (num_icc_ds)
    mic_vcc->icc_ds_ctrl_tbl = (struct _mic_ds **)chk_calloc (num_icc_ds, sizeof (MIC_DS *));

  mic_vcc->num_dsts_ctrl = num_dsts_ctrl;
  if (num_dsts_ctrl)
    mic_vcc->mic_dsts_tbl = (struct _mic_dsts *)chk_calloc (num_dsts_ctrl, sizeof (MIC_DSTS));

/* Initialize Special ICCP Variables values */
  strcpy (mic_vcc->misv.Bilateral_Table_Id, bi_lat_tbl_id);
  mic_vcc->misv.TASE2_Version.MajorVersionNumber = 1996;
  mic_vcc->misv.TASE2_Version.MinorVersionNumber = 8;
  mic_vcc->misv.Supported_Features[0] = MI_BLOCK1 | MI_BLOCK2;
  mic_vcc->misv.Supported_Features[1] = 0;

  S_MI_UNLOCK_COMMON ();
  return (mic_vcc);
  }

/************************************************************************/
/*			mic_free_vcc 					*/
/************************************************************************/

ST_VOID mic_free_vcc (MIC_VCC *mic_vcc)
  {
int i;

  S_MI_LOCK_COMMON ();
  if (mic_vcc->max_vcc_dv > 0)
    {
    for (i=0; i < mic_vcc->num_vcc_dv; i++)
      chk_free (mic_vcc->vcc_dv_tbl[i]);
    chk_free (mic_vcc->vcc_dv_tbl);
    }

  if (mic_vcc->max_icc_dv > 0)
    {
    for (i=0; i < mic_vcc->num_icc_dv; i++)
      chk_free (mic_vcc->icc_dv_tbl[i]);
    chk_free (mic_vcc->icc_dv_tbl);
    }

  if (mic_vcc->max_vcc_ds > 0)
    chk_free (mic_vcc->vcc_ds_ctrl_tbl);

  if (mic_vcc->max_icc_ds > 0)
    chk_free (mic_vcc->icc_ds_ctrl_tbl);

  if (mic_vcc->num_dsts_ctrl > 0)
    chk_free (mic_vcc->mic_dsts_tbl);

  if (mic_vcc->max_vcc_devices > 0)
    {
    for (i=0; i < mic_vcc->num_vcc_devices; i++)
      chk_free (mic_vcc->vcc_device_tbl[i]);
    chk_free (mic_vcc->vcc_device_tbl);
    }

  if (mic_vcc->max_icc_devices > 0)
    {
    for (i=0; i < mic_vcc->num_icc_devices; i++)
      chk_free (mic_vcc->icc_device_tbl[i]);
    chk_free (mic_vcc->icc_device_tbl);
    }

  chk_free (mic_vcc);
  S_MI_UNLOCK_COMMON ();
  }


/************************************************************************/
/*			mic_add_dv_to_vcc_ex				*/
/* same as mic_add_dv_to_vcc but pointer is returned			*/
/************************************************************************/


MIC_DV *mic_add_dv_to_vcc (MIC_VCC *mic_vcc, ST_INT dv_scope, 
			      ST_CHAR *dv_name, ST_INT mi_type, 
			      MIC_DV_ACCESS_INFO handle_info)
  {
MIC_DV *dv;

  S_MI_LOCK_COMMON ();

/* Allocate, Initialize, and put in table */
  dv = (struct _mic_dv *)chk_calloc (1, sizeof (MIC_DV));
  dv->dv_name = dv_name;
  dv->handle_info = handle_info;
  dv->dv_attrib = (ST_INT16) mi_type;
  if (dv_scope == ICC_SPEC)
    {
    dv->dv_attrib = mi_type | MIC_DV_SCOPE_ICC;
    mic_vcc->icc_dv_tbl[mic_vcc->num_icc_dv] = dv;
    ++mic_vcc->num_icc_dv;
    }
  else
    {
    mic_vcc->vcc_dv_tbl[mic_vcc->num_vcc_dv] = dv;
    ++mic_vcc->num_vcc_dv;
    }
  S_MI_UNLOCK_COMMON ();
  return (dv);
  }

/************************************************************************/
/*			mic_sort_vcc_dv 				*/
/************************************************************************/

ST_VOID mic_sort_vcc_dv (MIC_VCC *mic_vcc)
  {
  S_MI_LOCK_COMMON ();
  qsort (mic_vcc->vcc_dv_tbl, mic_vcc->num_vcc_dv, sizeof (MIC_DV *), 
          (int(*)(const void *, const void *))_mic_dv_name_compare);
  qsort (mic_vcc->icc_dv_tbl, mic_vcc->num_icc_dv, sizeof (MIC_DV *),\
          (int(*)(const void *, const void *))_mic_dv_name_compare);
  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/*			mic_find_dv 					*/
/************************************************************************/

MIC_DV *mic_find_dv (MIC_VCC *mic_vcc, ST_CHAR *dv_name, ST_INT dv_scope)
  {
MIC_DV mic_dv;
MIC_DV *dvp;
MIC_DV **dvp_out;

  S_MI_LOCK_COMMON ();
  mic_dv.dv_name = dv_name;
  dvp = &mic_dv;
  dvp_out = NULL;
  if (dv_scope == ICC_SPEC || dv_scope == MI_UNKNOWN_SCOPE)
    {
    dvp_out = (struct _mic_dv **)bsearch (&dvp,
				     mic_vcc->icc_dv_tbl, mic_vcc->num_icc_dv, 
		                     sizeof (MIC_DV *),
				     (int(*)(const void *, const void *))_mic_dv_name_compare);
    }
  if (dv_scope == VCC_SPEC || 
      (dv_scope == MI_UNKNOWN_SCOPE && dvp_out == NULL))
    {
    dvp_out = (struct _mic_dv **)bsearch (&dvp, 
				     mic_vcc->vcc_dv_tbl, mic_vcc->num_vcc_dv, 
		                     sizeof (MIC_DV *),
				     (int(*)(const void *, const void *))_mic_dv_name_compare);
    }

  S_MI_UNLOCK_COMMON ();
  if (dvp_out != NULL)
    return (*dvp_out);

  return (NULL);
  }

/************************************************************************/
/*			_mic_dv_name_compare 				*/
/************************************************************************/
int _mic_dv_name_compare (const MIC_DV **dv1, const MIC_DV **dv2)
  {
  return (strcmp ((*dv1)->dv_name, (*dv2)->dv_name));
  }

/************************************************************************/
/* 			mic_log_vcc_dv_tables 				*/
/************************************************************************/

ST_VOID mic_log_vcc_dv_tables (MIC_VCC *mic_vcc)
  {
MIC_DV *mic_dv;
ST_INT i;

  SLOGCALWAYS0 ("MIC Data Values");
  SLOGCALWAYS1 ("  VCC Scope Data Values (%d Data Values)", mic_vcc->num_vcc_dv);
  for (i = 0; i < mic_vcc->num_vcc_dv; ++i)
    {
    mic_dv = mic_vcc->vcc_dv_tbl[i];
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mic_dv->dv_name);
    }
  SLOGCALWAYS1 ("  ICC Scope Data Values (%d Data Values)", mic_vcc->num_icc_dv);
  for (i = 0; i < mic_vcc->num_icc_dv; ++i)
    {
    mic_dv = mic_vcc->icc_dv_tbl[i];
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mic_dv->dv_name);
    }
  }

/************************************************************************/
/************************************************************************/
/*			mic_find_ds 					*/
/************************************************************************/

MIC_DS *mic_find_ds (MIC_VCC *mic_vcc, OBJECT_NAME *ds_obj)
  {
MIC_DS **ds_tbl;
MIC_DS *mic_ds;
ST_INT i;
ST_INT num_ds;

  S_MI_LOCK_COMMON ();
  if (ds_obj->object_tag == ICC_SPEC)
    {
    if (strcmp (ds_obj->domain_id, mic_vcc->icc_name) != 0)
      {
      MI_LOG_NERR0 ("Domain name mismatch");
      S_MI_UNLOCK_COMMON ();
      return (NULL);
      }
    ds_tbl = mic_vcc->icc_ds_ctrl_tbl;
    num_ds = mic_vcc->max_icc_ds;
    }
  else
    {
    ds_tbl = mic_vcc->vcc_ds_ctrl_tbl;
    num_ds = mic_vcc->max_vcc_ds;
    }

  for (i = 0; i < num_ds; ++i)
    {
    mic_ds = ds_tbl[i];
    if (mic_ds != NULL && !strcmp (mic_ds->ds_name, 
				   ds_obj->obj_name.vmd_spec))
      {
      S_MI_UNLOCK_COMMON ();
      return (mic_ds);
      } 
    }
  S_MI_UNLOCK_COMMON ();
  return (NULL);
  }


/************************************************************************/
/*			mic_find_dsts 					*/
/************************************************************************/

MIC_DSTS *mic_find_dsts (MIC_VCC *mic_vcc, MIC_DS *mic_ds)
  {
MIC_DSTS *mic_dsts;
ST_INT i;

  S_MI_LOCK_COMMON ();
  mic_dsts = mic_vcc->mic_dsts_tbl;
  for (i = 0; i < mic_vcc->num_dsts_ctrl; ++i, ++mic_dsts)
    {
    if ((mic_dsts->dsts_flags & MIC_DSTS_FLAGS_IN_USE) &&
        (mic_dsts->mic_ds == mic_ds))
      {
      S_MI_UNLOCK_COMMON ();
      return (mic_dsts);
      }
    }
  S_MI_UNLOCK_COMMON ();
  return (NULL);
  }

/************************************************************************/
/************************************************************************/
/*			_mic_start 					*/
/************************************************************************/

MI_REQ_CTRL *_mic_start (MI_CONN *mi_conn)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
ST_RET rc;
MIC_VCC *mic_vcc;
MI_ASSOCIATION_ATTRIBUTES *misv;
MI_READ_REQ *mi_rd_req;
MI_READ_VAR_SPEC *mi_vs;

  MI_LOG_FLOW1 ("Starting Client activity for remote '%s'", mi_conn->mi_remote->name);
  MI_LOG_CFLOW0 ("Read Bi-Lateral table ID, Supported Features, and Version");

  mic_vcc = mi_conn->mi_remote->mic_vcc;

/* destination buffer */
  misv = (MI_ASSOCIATION_ATTRIBUTES *)chk_calloc (1, sizeof (MI_ASSOCIATION_ATTRIBUTES));

/* make the request */
  mi_rd_req = (struct _mi_read_req *)chk_calloc (1, sizeof (MI_READ_REQ) + 
		      (3 * sizeof (MI_READ_VAR_SPEC)));
  mi_vs = (MI_READ_VAR_SPEC *) (mi_rd_req + 1);
  mi_rd_req->num_var = 3;
  mi_rd_req->vs = mi_vs;
  mi_vs->name = "Bilateral_Table_ID";
  mi_vs->scope = ICC_SPEC;
  mi_vs->domain = mi_conn->mi_remote->mic_vcc->icc_name;
  mi_vs->mi_type = MI_TYPEID_VISIBLE_STRING_32;
  mi_vs->dest = misv->Bilateral_Table_Id;

  ++mi_vs;
  mi_vs->name = "Supported_Features";
  mi_vs->scope = VCC_SPEC;
  mi_vs->mi_type = MI_TYPEID_SUPPORTED_FEATURES;
  mi_vs->dest = misv->Supported_Features;

  ++mi_vs;
  mi_vs->name = "TASE2_Version";
  mi_vs->scope = VCC_SPEC;
  mi_vs->mi_type = MI_TYPEID_TASE2_VERSION;
  mi_vs->dest = &misv->TASE2_Version;

  rc = _mic_read (mi_conn, mi_rd_req, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("_mic_read request error : %04x", rc);
    chk_free (mi_rd_req);
    chk_free (misv);
    return (NULL);
    }

  MI_LOG_CFLOW0 ("Read request issued OK");

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mi_rd_req->u_req_done = _mic_start_done;

  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mi_rd_req = mi_rd_req;
  mi_req->misv = misv;

  mi_rd_req->mi_req = mi_req;

/* All is OK, return success */
  return (mi_req);
  }

/************************************************************************/
/*			_mic_start_done 				*/
/************************************************************************/

static ST_VOID _mic_start_done (MI_READ_REQ *mi_rd_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MI_ASSOC_CTRL *mi_assoc;
MI_READ_VAR_SPEC *mi_rd_vs;
MI_ASSOCIATION_ATTRIBUTES *misv;
ST_INT16 *supportedFeatures;
ST_INT16 *requiredFeatures;

  mi_req = mi_rd_req->mi_req;
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;
  mi_rd_vs = mi_rd_req->vs;
  misv = mi_req->misv;
  mi_req->result = mi_rd_req->result;

  if (mi_rd_req->result != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("MIC Start confirm error: %04x", mi_rd_req->result); 
    }
  else
    {
    MI_LOG_FLOW0 ("Special variable read completed OK");

    if (mi_rd_req->vs[0].result != SD_SUCCESS ||
        strcmp (misv->Bilateral_Table_Id, mic_vcc->misv.Bilateral_Table_Id) != 0)
      {
      MI_LOG_NERR0 ("Bilateral Table ID Mismatch"); 
      mi_req->result = SD_FAILURE;
      } 

    if (mi_rd_req->vs[1].result == SD_SUCCESS)
      {
      supportedFeatures = (ST_INT16 *) misv->Supported_Features;
      requiredFeatures = (ST_INT16 *) mic_vcc->misv.Supported_Features;
      if (((*requiredFeatures) & (*supportedFeatures)) != *requiredFeatures)
        {
        MI_LOG_NERR0 ("Remote does not support required features"); 
        mi_req->result = SD_FAILURE;
        } 
      } 
    else
      {
      MI_LOG_NERR0 ("Support_Features read error"); 
      mi_req->result = SD_FAILURE;
      } 

    if (mi_rd_req->vs[2].result != SD_SUCCESS &&
        ((misv->TASE2_Version.MajorVersionNumber != mic_vcc->misv.TASE2_Version.MajorVersionNumber) ||
        (misv->TASE2_Version.MinorVersionNumber != mic_vcc->misv.TASE2_Version.MinorVersionNumber)))
      {
      MI_LOG_NERR0 ("TASE2 Version Mismatch"); 
      mi_req->result = SD_FAILURE;
      } 
    }

  mi_assoc = mi_req->mi_conn->mi_assoc;
  if (mi_req->result == SD_SUCCESS)
    {
    MI_LOG_CFLOW0 ("All values acceptable");
    mi_assoc->param_check_state = MI_PARAM_OK;
    }
  else
    {
    mi_assoc->param_check_state = MI_PARAM_MISMATCH;
    mi_req->result = SD_FAILURE;
    }

  chk_free (mi_rd_req);
  chk_free (misv);
  _mi_req_done (mi_req);
  }

/************************************************************************/
/************************************************************************/
/*			mic_delete_ds 					*/
/************************************************************************/

MI_REQ_CTRL *mic_delete_ds (MI_CONN *mi_conn, MIC_DS *mic_ds)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
ST_RET rc;
MIC_VCC *mic_vcc;
OBJECT_NAME nvl_obj;


  S_MI_LOCK_COMMON ();

  MI_LOG_FLOW2 ("Deleting DataSet '%s' in remote '%s'", 
  			mic_ds->ds_name, mi_conn->mi_remote->name);
  mic_vcc = mi_conn->mi_remote->mic_vcc;

  nvl_obj.obj_name.vmd_spec = mic_ds->ds_name;
  if (mic_ds->ds_scope == ICC_SPEC)
    {
    nvl_obj.object_tag = ICC_SPEC;
    nvl_obj.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
    }
  else
    nvl_obj.object_tag = VCC_SPEC;

  rc = _mic_del_nvl (mi_conn, &nvl_obj, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  MI_LOG_CFLOW0 ("Delete NVL request issued OK"); 

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mi_delete_ds_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mic_ds = mic_ds;

/* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/*			_mi_delete_ds_done 				*/
/************************************************************************/

static ST_VOID _mi_delete_ds_done (MVL_REQ_PEND *mvl_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MIC_DS *mic_ds;

  mi_req = mvl_req->mi_req;		/* retrieve the MI request control	*/
  mic_ds = mi_req->mic_ds;
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;

  if (mvl_req->result != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("Delete DataSet confirm error: %04x", mvl_req->result); 
    }
  else
    {
    MI_LOG_FLOW0 ("Delete NVL completed OK");
    if (mic_ds->ds_scope == ICC_SPEC)
      mic_vcc->icc_ds_ctrl_tbl[mic_ds->ds_index] = NULL;
    else
      mic_vcc->vcc_ds_ctrl_tbl[mic_ds->ds_index] = NULL;

    chk_free (mic_ds);
    }

  _mi_req_done (mi_req);
  }

/************************************************************************/
/************************************************************************/
/*			mic_define_ds 					*/
/************************************************************************/

ST_INT mic_define_ds (MI_REMOTE *mi_remote, ST_CHAR *ds_name, 
			    ST_INT ds_scope, ST_INT num_dv, 
			    MIC_DV **dv, MIC_DS **ds_out)
  {
MIC_DS *mic_ds;
MIC_DV *mic_dv;
ST_INT i;
MIC_VCC *mic_vcc;
ST_INT ds_index;
ST_INT num_dsdv;
ST_INT skipCount;

  S_MI_LOCK_COMMON ();
  MI_LOG_FLOW2 ("Defining DataSet '%s' for remote '%s'", 
  			ds_name, mi_remote->name);

  mic_vcc = mi_remote->mic_vcc;
  ds_index = _mic_find_avail_ds (mic_vcc, ds_scope);
  if (ds_index == -1)
    {
    MI_LOG_NERR0 ("No DS available");
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }

/* OK, now build the DS control element */
  mic_ds = (struct _mic_ds *)chk_calloc (1, sizeof (MIC_DS) + 
		       (2 * (num_dv * sizeof (MIC_DV **))));
  mic_ds->ds_name = ds_name;
  mic_ds->num_dv = 0;
  mic_ds->ds_scope = ds_scope;
  mic_ds->ds_flags = MIC_DS_MMS_REQ_PEND;
  mic_ds->ds_index = ds_index;

  mic_ds->mic_dv_tbl = (MIC_DV **) (mic_ds + 1);
  mic_ds->mic_sorted_dv_tbl = mic_ds->mic_dv_tbl + num_dv;
  num_dsdv = 0;
  skipCount = 0;
  for (i = 0; i < num_dv; ++i)
    {
    mic_dv = dv[i];
    if ((mic_dv->dv_attrib & MIC_DV_MISMATCH) == 0)
      {
      mic_ds->mic_dv_tbl[num_dsdv] = mic_dv;
      mic_ds->mic_sorted_dv_tbl[num_dsdv] = mic_dv;
      ++num_dsdv;
      }
    else
      {
      if (skipCount++ == 0)
        {
        MI_LOG_NERR2 ("Removing invalid DV '%s' from DS '%s", 
  			mic_dv->dv_name, mic_ds->ds_name);
	}
      else
        {
        MI_LOG_CNERR2 ("Removing invalid DV '%s' from DS '%s", 
  			mic_dv->dv_name, mic_ds->ds_name);
	}
      }
    }
  if (num_dsdv == 0)
    {
    MI_LOG_NERR1 ("No valid DV included in DS '%s'", mic_ds->ds_name);
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }

  mic_ds->num_dv = num_dsdv;
  qsort (mic_ds->mic_sorted_dv_tbl, mic_ds->num_dv, sizeof (MIC_DV *),
	   (int(*)(const void *, const void *))_mic_dv_name_compare);

  if (ds_scope == ICC_SPEC)
    mic_vcc->icc_ds_ctrl_tbl[ds_index] = mic_ds;
  else
    mic_vcc->vcc_ds_ctrl_tbl[ds_index] = mic_ds;

/* All is OK, return success */
  *ds_out = mic_ds;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			_mic_find_avail_ds 				*/
/************************************************************************/

ST_INT _mic_find_avail_ds (MIC_VCC *mic_vcc, ST_INT ds_scope)
  {
ST_INT i;
ST_INT num_ds;
MIC_DS **ds_ctrl_tbl;	

  if (ds_scope == ICC_SPEC)
    {
    num_ds = mic_vcc->max_icc_ds;
    ds_ctrl_tbl = mic_vcc->icc_ds_ctrl_tbl;
    }
  else
    {
    num_ds = mic_vcc->max_vcc_ds;
    ds_ctrl_tbl = mic_vcc->vcc_ds_ctrl_tbl;
    }

  for (i = 0; i < num_ds; ++i)
    {
    if (ds_ctrl_tbl[i] == NULL)
      return (i);
    }
  return (-1);
  }

/************************************************************************/
/*			mic_create_ds 					*/
/************************************************************************/

MI_REQ_CTRL *mic_create_ds (MI_CONN *mi_conn, ST_CHAR *ds_name, 
			    ST_INT ds_scope, ST_INT num_dv, 
			    MIC_DV **dv, MIC_DS **ds_out)
  {
MVL_REQ_PEND *mvl_req;
MI_REQ_CTRL *mi_req;
MIC_DS *mic_ds;
ST_RET rc;
MIC_VCC *mic_vcc;
OBJECT_NAME nvl_obj;
ST_INT8 ds_index;

  S_MI_LOCK_COMMON ();

  mic_vcc = mi_conn->mi_remote->mic_vcc;
  nvl_obj.obj_name.vmd_spec = ds_name;
  if (ds_scope == ICC_SPEC)
    {
    nvl_obj.object_tag = ICC_SPEC;
    nvl_obj.domain_id = mic_vcc->icc_name;
    }
  else
    nvl_obj.object_tag = VCC_SPEC;

  /* if the data set already exists in mic_vcc - delete it  */
  mic_ds = mic_find_ds (mic_vcc, &nvl_obj);
  if (mic_ds)
    {
    ds_index = mic_ds->ds_index;
    chk_free (mic_ds);
    if (ds_scope == ICC_SPEC)
      mic_vcc->icc_ds_ctrl_tbl[ds_index] = NULL;
    else
      mic_vcc->vcc_ds_ctrl_tbl[ds_index] = NULL;
    }

  rc = mic_define_ds (mi_conn->mi_remote, ds_name, ds_scope, num_dv, dv, ds_out);
  if (rc != SD_SUCCESS)
    {
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  mic_ds = *ds_out;

  MI_LOG_CFLOW0 ("Start by deleting the NVL just in case it exists");
  rc = _mic_del_nvl (mi_conn, &nvl_obj, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

  MI_LOG_CFLOW0 ("Delete NVL request issued OK");

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mi_del_nvl_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mic_ds = mic_ds;

/* All is OK, return success */
  *ds_out = mic_ds;
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }


/************************************************************************/
/*			_mi_del_nvl_done 				*/
/************************************************************************/

static ST_VOID _mi_del_nvl_done (MVL_REQ_PEND *mvl_req)
  {
MI_REQ_CTRL *mi_req;
MI_CONN *mi_conn;
MIC_VCC *mic_vcc;
MIC_DS *mic_ds;
MVL_REQ_PEND *new_mvl_req;
ST_INT i;
ST_RET rc;
DEFVLIST_REQ_INFO *req_info;
VARIABLE_LIST *vl;
VARIABLE_SPEC *vs;
MIC_DV **dv;

  if (mvl_req->result != SD_SUCCESS)
    {
    MI_LOG_FLOW1 ("Delete NVL confirm error: %04x", mvl_req->result); 
    }
  else
    {
    MI_LOG_FLOW0 ("Delete NVL completed OK");
    }

  mi_req = mvl_req->mi_req;	/* retrieve the MI request control	*/
  mic_ds = mi_req->mic_ds;
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;
  mi_conn = mi_req->mi_conn;

/* Issue the define NVL request */
  req_info = (struct defvlist_req_info *)chk_calloc (1, sizeof (DEFVLIST_REQ_INFO) +
			 (mic_ds->num_dv * sizeof (VARIABLE_LIST)));

  /* Use the fact that this is a union to save a few bytes */
  req_info->vl_name.obj_name.vmd_spec = mic_ds->ds_name;
  if (mic_ds->ds_scope == ICC_SPEC)
    {
    req_info->vl_name.object_tag = ICC_SPEC;
    req_info->vl_name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
    }
  else
    req_info->vl_name.object_tag = VCC_SPEC;

  req_info->num_of_variables = mic_ds->num_dv;
  vl = (VARIABLE_LIST *) (req_info +1);
  dv = mic_ds->mic_dv_tbl;
  for (i = 0; i < mic_ds->num_dv; ++i, ++vl)
    {
    vs = &vl->var_spec;
    vs->var_spec_tag = VA_SPEC_NAMED;
  /* We can do this regardless of scope because it is a union */
    vs->vs.name.obj_name.vmd_spec = dv[i]->dv_name;
    if (dv[i]->dv_attrib & MIC_DV_SCOPE_ICC)
      {
      vs->vs.name.object_tag = ICC_SPEC;
      vs->vs.name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
      }
    else
      vs->vs.name.object_tag = VCC_SPEC;
    }

  MI_LOG_CFLOW2 ("Sending define NVL request for DataSet '%s' in remote '%s'", 
  			mic_ds->ds_name, mi_conn->mi_remote->name);


/* OK, the MMS request info is ready, send the request	*/
  rc = mvla_defvlist (mi_conn->net_info, req_info, &new_mvl_req);
  chk_free (req_info);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mvla_defvlist request error : %04x", rc); 
    mvl_free_req_ctrl (new_mvl_req);
    mi_req->result = SD_FAILURE;

    if (mic_ds->ds_scope == ICC_SPEC)
      mic_vcc->icc_ds_ctrl_tbl[mic_ds->ds_index] = NULL;
    else
      mic_vcc->vcc_ds_ctrl_tbl[mic_ds->ds_index] = NULL;

    chk_free (mic_ds);
    _mi_req_done (mi_req);
    return;
    }

  MI_LOG_CFLOW0 ("Define NVL request issued OK");

/* The MMS request has gone out just fine, let's catch the confirm	*/
  new_mvl_req->u_req_done = _mi_create_ds_done;
  new_mvl_req->result = SD_SUCCESS;

/* Done with the old MVL request.					*/
  mvl_free_req_ctrl (mvl_req);

/* Associate the new MVL request with our MI request			*/
  mi_req->mvl_req = new_mvl_req;
  new_mvl_req->mi_req = mi_req;
  }

/************************************************************************/
/*			_mi_create_ds_done 				*/
/************************************************************************/

static ST_VOID _mi_create_ds_done (MVL_REQ_PEND *mvl_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MIC_DS *mic_ds;

  mi_req = mvl_req->mi_req;		/* retrieve the MI request control	*/
  mic_ds = mi_req->mic_ds;
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;

  if (mvl_req->result != SD_SUCCESS)
    {
    MI_LOG_NERR2 ("Create DataSet '%s' confirm error: %04x", 
    				mic_ds->ds_name, mvl_req->result); 
    if (mic_ds->ds_scope == ICC_SPEC)
      mic_vcc->icc_ds_ctrl_tbl[mic_ds->ds_index] = NULL;
    else
      mic_vcc->vcc_ds_ctrl_tbl[mic_ds->ds_index] = NULL;

    chk_free (mic_ds);
    }
  else
    {
    MI_LOG_FLOW1 ("Create DataSet '%s' completed OK", mic_ds->ds_name);
    mic_ds->ds_flags = MIC_DS_READY;
    }

  _mi_req_done (mi_req);
  }

/************************************************************************/
/************************************************************************/
/*			mic_start_dsts 					*/
/************************************************************************/

MI_REQ_CTRL *mic_start_dsts (MI_CONN *mi_conn, MIC_DS *mic_ds, 
			     MI_DSTS_DATA *mi_dsts_data, MIC_DSTS **dsts_out)
  {
MVL_REQ_PEND *mvl_req;
MI_REQ_CTRL *mi_req;
MI_READ_REQ *mi_rd_req;
MI_READ_VAR_SPEC *mi_rd_vs;
ST_RET rc;
MIC_VCC *mic_vcc;
MIC_DSTS *mic_dsts;
ST_INT i;

  S_MI_LOCK_COMMON ();

  MI_LOG_FLOW2 ("Starting DSTS for DS '%s' in remote '%s'", 
  			mic_ds->ds_name, mi_conn->mi_remote->name);

  mic_vcc = mi_conn->mi_remote->mic_vcc;

  mic_dsts = mic_vcc->mic_dsts_tbl;
  for (i = 0; i < mic_vcc->num_dsts_ctrl; ++i, ++mic_dsts)
    {
    if ((mic_dsts->dsts_flags & MIC_DSTS_FLAGS_IN_USE) == 0)
      break;
    }
  if (i >= mic_vcc->num_dsts_ctrl)
    {
    MI_LOG_NERR0 ("No DSTS available");
    mi_op_err = MVL_ERR_RESOURCE_NOT_AVAIL;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }
  mic_dsts->mic_ds = mic_ds;
  mic_dsts->mi_conn = mi_conn;

/* First we need to read the "Next_DSTransfer_Set" variable */

/* make the request */
  mi_rd_req = (struct _mi_read_req *)chk_calloc (1, sizeof (MI_READ_REQ) + 
		             (1 * sizeof (MI_READ_VAR_SPEC)));
  mi_rd_vs = (MI_READ_VAR_SPEC *) (mi_rd_req + 1);
  mi_rd_req->num_var = 1;
  mi_rd_req->vs = mi_rd_vs;
  mi_rd_vs->name = "Next_DSTransfer_Set";
  mi_rd_vs->scope = ICC_SPEC;
  mi_rd_vs->domain = mi_conn->mi_remote->mic_vcc->icc_name;
  mi_rd_vs->mi_type = MI_TYPEID_MMS_OBJECT_NAME;
  mi_rd_vs->dest = &mic_dsts->ts_name;

  rc = _mic_read (mi_conn, mi_rd_req, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("_mic_read request error : %04x", rc); 
    chk_free (mi_rd_req);
    mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }
  MI_LOG_CFLOW0 ("Read 'Next_DSTransfer_Set' request issued OK");

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mi_rd_req->u_req_done = _mi_rd_next_dsts_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->mi_rd_req = mi_rd_req;
  memcpy (&mic_dsts->mi_dsts_data, mi_dsts_data, sizeof (MI_DSTS_DATA)); 
  mi_req->mic_dsts = mic_dsts; 
  mi_req->mic_ds = mic_ds; 
  mi_rd_req->mi_req = mi_req;

/* All is OK, return success */
  mic_dsts->dsts_flags |= MIC_DSTS_FLAGS_IN_USE;
  *dsts_out = mic_dsts;
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/*			_mi_rd_next_dsts_done 				*/
/************************************************************************/

static ST_VOID _mi_rd_next_dsts_done (MI_READ_REQ *mi_rd_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MI_CONN *mi_conn;
MI_READ_VAR_SPEC *mi_rd_vs;
MI_MMS_OBJECT_NAME *next_dsts;
MVL_REQ_PEND *mvl_req;
MVL_REQ_PEND *new_mvl_req;
MI_WRITE_REQ *mi_wr_req;
MI_WRITE_VAR_SPEC *mi_wr_vs;
ST_RET rc;
MIC_DSTS *mic_dsts;
MIC_DS *mic_ds;
MI_MMS_OBJECT_NAME *ts_name;	

  mi_req = mi_rd_req->mi_req;
  mvl_req = mi_req->mvl_req;
  mi_conn= mi_req->mi_conn;
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;
  mi_rd_req = mi_req->mi_rd_req;
  mi_rd_vs = mi_rd_req->vs;
  next_dsts = mi_req->next_dsts;

  mic_dsts = mi_req->mic_dsts;
  mic_ds = mi_req->mic_ds;
  mi_req->result = mi_rd_req->result;

  if (mi_rd_req->result != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("MIC read next DSTS confirm error: %04x", mi_rd_req->result); 
    mi_req->result = mi_rd_req->result;
    }
  else
    {
    if (mi_rd_req->vs[0].result != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("MIC read next DSTS access error: %04x", mi_rd_req->vs[0].result); 
      mi_req->result = mi_rd_req->vs[0].result;
      } 
    }

  ts_name = (MI_MMS_OBJECT_NAME *)mi_rd_req->vs[0].dest;
  chk_free (mi_rd_req);
  if (mi_req->result != SD_SUCCESS)
    {
    _mi_req_done (mi_req);
    mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
    return;
    }

  MI_LOG_FLOW1 ("Read 'Next_DSTransfer_Set' OK, use DSTS '%s'", ts_name->Name);

/* OK, we now have the next DSTS name, go ahead and write it ...	*/
  mi_wr_req = (struct _mi_write_req *)chk_calloc (1, sizeof (MI_WRITE_REQ) + 
		          (1 * sizeof (MI_WRITE_VAR_SPEC)));
  mi_wr_vs = (MI_WRITE_VAR_SPEC *) (mi_wr_req + 1);
  mi_wr_req->num_var = 1;
  mi_wr_req->vs = mi_wr_vs;

  mi_wr_vs->name = ts_name->Name;
  mi_wr_vs->scope = ts_name->Scope;
  mi_wr_vs->domain = ts_name->DomainName;
  mi_wr_vs->mi_type = MI_TYPEID_DSTS;
  mi_wr_vs->src = &mic_dsts->mi_dsts_data;

  mic_dsts->mi_dsts_data.DataSetName.scope = mic_ds->ds_scope;
  strcpy (mic_dsts->mi_dsts_data.DataSetName.name, mic_ds->ds_name); 
  strcpy (mic_dsts->mi_dsts_data.DataSetName.DomainName, mic_vcc->icc_name);

  rc = _mic_write (mi_conn, mi_wr_req, &new_mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("_mic_write request error : %04x", rc); 
    mvl_free_req_ctrl (new_mvl_req);
    _mi_req_done (mi_req);
    mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
    }
  else
    {
    MI_LOG_CFLOW0 ("Write request issued OK");

  /* The MMS request has gone out just fine, let's catch the confirm	*/
    mi_wr_req->u_req_done = _mi_start_dsts_done;
    mi_wr_req->mi_req = mi_req;

  /* Save the pointer to write req info */
    mi_req->mi_wr_req = mi_wr_req;

  /* Done with the old MVL request.					*/
    mvl_free_req_ctrl (mvl_req);

  /* Associate the new MVL request with our MI request			*/
    mi_req->mvl_req = new_mvl_req;
    new_mvl_req->mi_req = mi_req;
    }
  }

/************************************************************************/
/*			_mi_start_dsts_done 				*/
/************************************************************************/

static ST_VOID _mi_start_dsts_done (MI_WRITE_REQ *mi_wr_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MIC_DSTS *mic_dsts;

  mi_req = mi_wr_req->mi_req;		/* retrieve the MI request control	*/
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;
  mi_wr_req = mi_req->mi_wr_req;
  mic_dsts = mi_req->mic_dsts;

  if (mi_wr_req->result != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("DSTS Start write confirm error: %04x", mi_wr_req->result); 
    mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
    }
  else
    {
    if (mi_wr_req->vs[0].result != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("DSTS Start write error: %04x", mi_wr_req->vs[0].result); 
      mi_req->result = SD_FAILURE;
      mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
      } 
    else
      {
      MI_LOG_FLOW0 ("DSTS Start complete OK");
      }
    }

  chk_free (mi_wr_req);
   _mi_req_done (mi_req);
  }

/************************************************************************/
/************************************************************************/
/*			mic_stop_dsts 					*/
/************************************************************************/

MI_REQ_CTRL *mic_stop_dsts (MI_CONN *mi_conn, MIC_DSTS *mic_dsts)
  {
MVL_REQ_PEND *mvl_req;
MI_REQ_CTRL *mi_req;
MI_WRITE_REQ *mi_wr_req;
MI_WRITE_VAR_SPEC *mi_wr_vs;
ST_RET rc;
MIC_VCC *mic_vcc;

  S_MI_LOCK_COMMON ();
  MI_LOG_FLOW2 ("Stopping DSTS '%s' in remote '%s'", 
  			mic_dsts->ts_name.Name, mi_conn->mi_remote->name);

  mic_vcc = mi_conn->mi_remote->mic_vcc;

  mi_wr_req = (struct _mi_write_req *)chk_calloc (1, sizeof (MI_WRITE_REQ) + 
		          (1 * sizeof (MI_WRITE_VAR_SPEC)));
  mi_wr_vs = (MI_WRITE_VAR_SPEC *) (mi_wr_req + 1);
  mi_wr_req->num_var = 1;
  mi_wr_req->vs = mi_wr_vs;

  mi_wr_vs->name = mic_dsts->ts_name.Name;
  mi_wr_vs->scope = mic_dsts->ts_name.Scope;
  mi_wr_vs->domain = mic_dsts->ts_name.DomainName;
  mi_wr_vs->mi_type = MI_TYPEID_DSTS;
  mi_wr_vs->src = &mic_dsts->mi_dsts_data;
  mic_dsts->mi_dsts_data.Status = SD_FALSE;

  rc = _mic_write (mi_conn, mi_wr_req, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("_mic_write request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let's catch the confirm	*/
  MI_LOG_CFLOW0 ("Write request issued OK");
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_wr_req->u_req_done = _mi_stop_dsts_done;
  mi_wr_req->mi_req = mi_req;
  mi_req->mi_wr_req = mi_wr_req;
  mi_req->mic_dsts = mic_dsts; 

/* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }


/************************************************************************/
/*			_mi_stop_dsts_done 				*/
/************************************************************************/

static ST_VOID _mi_stop_dsts_done (MI_WRITE_REQ *mi_wr_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MIC_DSTS *mic_dsts;

  mi_req = mi_wr_req->mi_req;		/* retrieve the MI request control	*/
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;
  mi_wr_req = mi_req->mi_wr_req;
  mic_dsts = mi_req->mic_dsts;

  if (mi_wr_req->result != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("DSTS Stop write confirm error: %04x", mi_wr_req->result); 
    }
  else
    {
    if (mi_wr_req->vs[0].result != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("DSTS Stop write error: %04x", mi_wr_req->vs[0].result); 
      mi_req->result = SD_FAILURE;
      } 
    else
      {
      MI_LOG_FLOW0 ("DSTS Stop complete OK");
      mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
      }
    }

  chk_free (mi_wr_req);
  _mi_req_done (mi_req);
  }

/************************************************************************/
/*			mic_ack_dsts 					*/
/************************************************************************/

ST_RET mic_ack_dsts (MI_CONN *mi_conn, MIC_DSTS *mic_dsts, 
		     MIC_DSTS_STO *sto, ST_BOOLEAN nack)
  {
ST_RET ret;
MVL_NVLIST_CTRL nvl;
MVL_VA_SCOPE va_scope[2];
MVL_VAR_ASSOC *entry[2];
MVL_VAR_ASSOC va[2];
MVL_DOM_CTRL icc_dom;

  S_MI_LOCK_COMMON ();
  
  nvl.num_of_entries = 2;
  nvl.entries = entry;
  nvl.va_scope = va_scope;
  nvl.altAcc = NULL;
  entry[0] = &va[0];
  entry[1] = &va[1];
  icc_dom.name = mi_conn->mi_remote->mic_vcc->icc_name;

  if (nack == SD_TRUE)
    va[0].name = "Transfer_Report_NACK";
  else
    va[0].name = "Transfer_Report_ACK";

  MI_LOG_FLOW1 ("Sending '%s'", va[0].name);

  if (sto->ts_name_pres)
    va[0].data = &sto->ts_name;	
  else
    va[0].data = &mic_dsts->ts_name;	
  va[0].type_id = _mi_get_type_id (MI_TYPEID_MMS_OBJECT_NAME);
  va[0].proc = NULL;
  va_scope[0].scope = ICC_SPEC;
  va_scope[0].dom = &icc_dom;

  if (sto->ts_timestamp_pres && !nack)
    {
    va[1].name = "Transfer_Set_Time_Stamp";
    va[1].data = &sto->ts_timestamp;	
    va[1].type_id = _mi_get_type_id (MI_TYPEID_GMT_BASED_S);
    va[1].proc = NULL;
    va_scope[1].scope = ICC_SPEC;
    va_scope[1].dom = &icc_dom;
    }
  else
    nvl.num_of_entries = 1;

  ret = mvl_info_variables (mi_conn->net_info, &nvl, SD_TRUE);
  if (ret != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mic_ack_dsts InfoReport Error : 0x%d",ret);
    }
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*			mic_read_dv 					*/
/************************************************************************/

MI_REQ_CTRL *mic_read_dv (MI_CONN *mi_conn, ST_INT num_dv, MIC_DV **dv)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
MVL_READ_RESP_PARSE_INFO *parse_info;
MVL_READ_RESP_PARSE_INFO *pi;
READ_REQ_INFO *req_info;
ST_INT i;
ST_RET rc;
VARIABLE_LIST *vl;
VARIABLE_SPEC *vs;
MIC_VCC *mic_vcc;

  S_MI_LOCK_COMMON ();

  mic_vcc = mi_conn->mi_remote->mic_vcc;

  /* Create a read request info struct  */
  req_info = (READ_REQ_INFO *) chk_calloc (1, sizeof (READ_REQ_INFO) +
				(num_dv * sizeof (VARIABLE_LIST)));
  req_info->spec_in_result = SD_FALSE;
  req_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
  req_info->va_spec.num_of_variables = num_dv;


  parse_info = (MVL_READ_RESP_PARSE_INFO *) chk_calloc (num_dv, 
			               sizeof (MVL_READ_RESP_PARSE_INFO));

  vl = (VARIABLE_LIST *) (req_info + 1);
  pi = parse_info;
  for (i = 0; i < num_dv; ++i, ++vl, ++pi)
    {
    vs = &vl->var_spec;
    vs->var_spec_tag = VA_SPEC_NAMED;

  /* We can do this regardless of scope because it is a union */
    vs->vs.name.obj_name.vmd_spec = dv[i]->dv_name;
    if (dv[i]->dv_attrib & MIC_DV_SCOPE_ICC)
      {
      vs->vs.name.object_tag = ICC_SPEC;
      vs->vs.name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
      }
    else
      vs->vs.name.object_tag = VCC_SPEC;

  /* Now take care of the MVL parse information */
    pi->type_id = _mi_get_type_id (dv[i]->dv_attrib & MIC_DV_MI_TYPE_MASK);
    pi->dest = dv[i];		/* We will intercept confirm 		*/
    /* pi->descr_arr = SD_FALSE; 	Already done 			*/
    }


/* OK, the MMS request info is ready, send the request	*/
  rc = mvla_read_variables (mi_conn->net_info,req_info, num_dv,
			     parse_info, &mvl_req);
  chk_free (req_info);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mvla_read_variables request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    chk_free (parse_info);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let MI handle the confirm	*/
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->data_value_read = SD_TRUE;
  mvl_req->u_req_done =_mic_read_mvl_done; 

/* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }


/************************************************************************/
/************************************************************************/
/*			mic_read_ds 					*/
/************************************************************************/

MI_REQ_CTRL *mic_read_ds (MI_CONN *mi_conn, MIC_DS *ds)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
MVL_READ_RESP_PARSE_INFO *parse_info;
MVL_READ_RESP_PARSE_INFO *pi;
READ_REQ_INFO *req_info;
ST_INT i;
ST_RET rc;
MIC_VCC *mic_vcc;
ST_INT num_dv;
MIC_DV **dv;

  S_MI_LOCK_COMMON ();
  
  mic_vcc = mi_conn->mi_remote->mic_vcc;
  num_dv = ds->num_dv;
  dv = ds->mic_dv_tbl;

  /* Create a read request info struct  */
  req_info = (READ_REQ_INFO *) chk_calloc (1, sizeof (READ_REQ_INFO) +
				(num_dv * sizeof (VARIABLE_LIST)));
  req_info->spec_in_result = SD_FALSE;
  req_info->va_spec.var_acc_tag = VAR_ACC_NAMEDLIST;

  /* We can do this regardless of scope because it is a union */
  req_info->va_spec.vl_name.obj_name.vmd_spec = ds->ds_name;
  if (ds->ds_scope == ICC_SPEC)
    {
    req_info->va_spec.vl_name.object_tag = ICC_SPEC;
    req_info->va_spec.vl_name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
    }
  else
    req_info->va_spec.vl_name.object_tag = VCC_SPEC;


  parse_info = (MVL_READ_RESP_PARSE_INFO *) chk_calloc (num_dv, 
			               sizeof (MVL_READ_RESP_PARSE_INFO));

  pi = parse_info;
  for (i = 0; i < num_dv; ++i, ++pi)
    {

  /* Now take care of the MVL parse information */
    pi->type_id = _mi_get_type_id (dv[i]->dv_attrib & MIC_DV_MI_TYPE_MASK);
    pi->dest = dv[i];		/* We will intercept confirm 		*/
    /* pi->descr_arr = SD_FALSE; 	Already done 			*/
    }

/* OK, the MMS request info is ready, send the request	*/
  rc = mvla_read_variables (mi_conn->net_info,req_info, num_dv,
			     parse_info, &mvl_req);
  chk_free (req_info);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mvla_read_variables request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    chk_free (parse_info);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let MI handle the confirm	*/
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_req->data_value_read = SD_TRUE;
  mvl_req->u_req_done =_mic_read_mvl_done; 

/* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/*			_mic_process_read_conf 				*/
/************************************************************************/

ST_VOID _mic_process_read_conf (MVL_REQ_PEND *mvl_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
ST_INT i;
RUNTIME_TYPE *rt;
ST_INT rt_len;
READ_RESP_INFO  *read_info;
ACCESS_RESULT   *ar_ptr;
MVL_READ_RESP_PARSE_INFO *parse_table;
ST_INT num_data;
MI_DV_DATA_UNION data_buf;
MI_CONN *mi_conn;
ST_INT mi_type;
ST_RET result;

/* If this is not an ICCP connection, let MVL handle it */
  if (mvl_req->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_read_conf (mvl_req);
    return;
    }

/* This is an ICCP connection. If it is a data value read, 	*/
/* we handle it internally, since there is no persistant	*/
/* storage for remote data values.				*/
  mi_req = mvl_req->mi_req;		
  if (mi_req->data_value_read == SD_FALSE)
    {
    _mvl_process_read_conf (mvl_req);
    return;
    }

/* OK, we handle this without MVL, and do our own conversions	*/
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;
  mi_conn = mvl_req->net_info->mi_conn;

  read_info = (READ_RESP_INFO *) mvl_req->event->u.mms.dec_rslt.data_ptr;
  ar_ptr    = read_info->acc_rslt_list;

  /* retrieve parse information						*/
  parse_table = mvl_req->u.rd.parse_info;
  num_data = mvl_req->u.rd.num_data;
  
  /* just a sanity check						*/
  if (num_data != read_info->num_of_acc_result)
    {
    MI_LOG_NERR0 ("Read Confirmation : num var != num result");
    mvl_req->result = SD_FAILURE;
    return;
    }

/* Tell the user to get ready for some data ... */
  u_mic_dv_ind_start (mi_conn, MIC_DV_IND_REASON_READ, NULL);

  /* For each variable returned in the read response ... */
  for (i = 0; i < num_data; ++i, ++ar_ptr, ++parse_table)
    {      /* check for successfull read of this variable */
    if (ar_ptr->acc_rslt_tag == ACC_RSLT_SUCCESS)
      {
      if (mvl_get_runtime (parse_table->type_id, &rt, &rt_len) == SD_SUCCESS)
        {
        result = ms_asn1_to_local (rt, rt_len,
                                      ar_ptr->va_data.data,
                                      ar_ptr->va_data.len,
                                      (ST_CHAR *) &data_buf);
        }                        
      else  /* bad type ID						*/
        result = SD_FAILURE;
      }
    else /* access result == failure					*/
      result = SD_FAILURE;

  /* OK, now give the data to the user */
    mi_type = _mi_get_mi_type (parse_table->type_id);
    u_mic_dv_ind (mi_conn, (struct _mic_dv *)parse_table->dest, result, &data_buf, mi_type);
    } /* for each variable						*/

/* Tell the user no more data is coming just now ... */
  u_mic_dv_ind_end (mi_conn, MIC_DV_IND_REASON_READ, NULL, NULL);

/* Free up the parse table and call the user done function 		*/
/* Will be done using the MVL done function  				*/
  }

/************************************************************************/
/* 		_mic_read_mvl_done				*/
/************************************************************************/

static ST_VOID _mic_read_mvl_done (MVL_REQ_PEND *mvl_req)
  {
  chk_free (mvl_req->u.rd.parse_info);
  _mi_req_done (mvl_req->mi_req);
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			_mvl_process_info_ind				*/
/************************************************************************/

ST_VOID _mic_process_info_ind (MVL_COMM_EVENT *event)
  {
VAR_ACC_SPEC *va_spec;
INFO_REQ_INFO *info_ptr;
VARIABLE_LIST *vl;
ST_INT i;
ACCESS_RESULT *ar_ptr;
RUNTIME_TYPE *rt;
ST_INT rt_len;
MI_DV_DATA_UNION data_buf;
MIC_DS *mic_ds;
MI_CONN *mi_conn;
MIC_VCC *mic_vcc;
ST_INT num_dv;
MIC_DV **dv;
MIC_DV *mic_dv;	
ST_INT mi_type;
ST_INT type_id;
ST_RET rc;
MIC_DSTS *mic_dsts;
MIC_DSTS_STO sto;
ST_CHAR *var_name;

  info_ptr = (INFO_REQ_INFO *) event->u.mms.dec_rslt.data_ptr;
  va_spec = &info_ptr->va_spec;
  ar_ptr = info_ptr->acc_rslt_list;

/* Both MIS and MIC get information reports - see who this one is for	*/
  if (va_spec->var_acc_tag == VAR_ACC_VARLIST)
    {
  /* It is a list of variables, see if is an ack for the server	*/ 
    vl = (VARIABLE_LIST *) (info_ptr + 1);
    var_name = vl->var_spec.vs.name.obj_name.vmd_spec; 
    if (vl->var_spec.vs.name.object_tag == ICC_SPEC && 
        ((strcmp (var_name, "Transfer_Report_ACK") == 0) || 
         (strcmp (var_name, "Transfer_Report_NACK") == 0))) 
      {
      _mis_process_info_ind (event);
      return;
      }

  /* See if it is an Information Message for the Client			*/
    if (strcmp (var_name, MI_IM_INFO_MESS_HEADER) == 0)
      {
      _mic_process_info_message (event);
      return;
      }

  /* If it is a LOV, the first must be the DSTS name (MI constraint)	*/
    if (vl->var_spec.vs.name.object_tag != ICC_SPEC || 
        (strcmp (var_name, "Transfer_Set_Name") != 0))
      {
      MI_LOG_NERR0 ("MIC LOV info rpt, first var not 'Transfer_Set_Name'");
      _mvl_free_comm_event (event);	/* All done with this event	*/
      return;
      }
    }
    

/* Get the MI connection and associated VCC */
  mi_conn = event->net_info->mi_conn;
  mic_vcc = mi_conn->mi_remote->mic_vcc;
  if (mic_vcc == NULL)
    {
    MI_LOG_NERR0 ("Warning: Received info rpt ind on server only connection");
    _mvl_free_comm_event (event);	/* All done with this event	*/
    return;
    }

  if (va_spec->var_acc_tag == VAR_ACC_NAMEDLIST)
    {
  /* Find the data set */
    mic_ds = mic_find_ds (mic_vcc, &va_spec->vl_name);
    if (mic_ds == NULL)
      {
      MI_LOG_NERR0 ("Warning: Received info rpt ind w/ no matching MIC_DS");
      _mvl_free_comm_event (event);	/* All done with this event	*/
      return;
      }
    mic_dsts = mic_find_dsts (mic_vcc, mic_ds);
    if (mic_dsts == NULL)
      {
      MI_LOG_NERR0 ("Warning: Received info rpt ind w/ no matching MIC_DSTS");
      _mvl_free_comm_event (event);	/* All done with this event	*/
      return;
      }
    num_dv = mic_ds->num_dv;
    if (num_dv != info_ptr->num_of_acc_result)
      {
      MI_LOG_NERR2 ("InfoRpt Ind : NumVar (%d) vs. MIC_DS.num _dv (%d) mismatch", 
    	  va_spec->num_of_variables, mic_ds->num_dv); 
      _mvl_free_comm_event (event);	/* All done with this event	*/
      return;
      }
    }
  else	/* List Of Variables */
    {
    mic_dsts = mic_find_dsts_by_asn1_name (mic_vcc, ar_ptr);
    if (mic_dsts == NULL)
      {
      MI_LOG_NERR0 ("Warning: Received info rpt ind w/ no matching MIC_DSTS");
      _mvl_free_comm_event (event);	/* All done with this event	*/
      return;
      }
    mic_ds = mic_dsts->mic_ds;
    num_dv = mic_ds->num_dv;
    }

/* OK, we have resolved the MIC_DSTS and MIC_DS control structures */
/* Tell the user to get ready for some data ... */
  
  u_mic_dv_ind_start (mi_conn, MIC_DV_IND_REASON_REPORT, mic_dsts);

/* Here we go through the Info Rpt */
  sto.ts_name_pres = SD_FALSE;
  sto.ts_timestamp_pres = SD_FALSE;
  sto.ds_cond_pres = SD_FALSE;
  sto.ec_detected_pres = SD_FALSE;
  
  dv = mic_ds->mic_dv_tbl;
  for (i = 0; i < info_ptr->num_of_acc_result; ++i, ++ar_ptr, ++vl)
    {
    if (va_spec->var_acc_tag == VAR_ACC_NAMEDLIST)
      mic_dv = dv[i];
    else
      {
      if (vl->var_spec.var_spec_tag != VA_SPEC_NAMED)
        {
        MI_LOG_NERR1 ("Invalid var_spec_tag '%d', must be named", vl->var_spec.var_spec_tag); 
        continue;
        }
      if (vl->var_spec.vs.name.object_tag == ICC_SPEC) 
        {
        if (strcmp (vl->var_spec.vs.name.domain_id, mic_vcc->icc_name) != 0)
          {
          MI_LOG_NERR1 ("Domain name '%s' mismatch ", vl->var_spec.vs.name.domain_id); 
          continue;
          }
        }
      mic_dv = mic_find_dv_in_ds (mic_ds, vl->var_spec.vs.name.obj_name.vmd_spec); 

      if (mic_dv == NULL)
        {
	MI_LOG_NERR1 ("InfoRpt Ind: MIC_DV %s not found", 
				vl->var_spec.vs.name.obj_name.vmd_spec);
        continue;
        }
      }

  /* We have the MIC_DV, go ahead and convert the data */
    if (ar_ptr->acc_rslt_tag == ACC_RSLT_SUCCESS)
      {
      mi_type = mic_dv->dv_attrib & MIC_DV_MI_TYPE_MASK;
      type_id = _mi_get_type_id (mi_type);
      if (mvl_get_runtime (type_id, &rt, &rt_len) == SD_SUCCESS)
        {
        rc = ms_asn1_to_local (rt, rt_len, ar_ptr->va_data.data,
                               ar_ptr->va_data.len, (ST_CHAR *)&data_buf);

        if (rc == SD_SUCCESS)
          {
        /* OK, now give the data to the user */
          u_mic_dv_ind (mi_conn, mic_dv, SD_SUCCESS, &data_buf, mi_type);
          if (!strcmp (mic_dv->dv_name, "Transfer_Set_Name"))
            {
            sto.ts_name_pres = SD_TRUE;
            memcpy (&sto.ts_name, &data_buf, sizeof (MI_MMS_OBJECT_NAME));
            }
          else if (!strcmp (mic_dv->dv_name, "Transfer_Set_Time_Stamp"))
            {
            sto.ts_timestamp_pres = SD_TRUE;
            memcpy (&sto.ts_timestamp, &data_buf, sizeof (MI_TS_TIMESTAMP));
            }
          else if (!strcmp (mic_dv->dv_name, "DSConditions_Detected"))
            {
            sto.ds_cond_pres = SD_TRUE;
            memcpy (&sto.ds_cond, &data_buf, sizeof (MI_DS_CONDITIONS));
            }
          else if (!strcmp (mic_dv->dv_name, "Event_Code_Detected"))
            {
            sto.ec_detected_pres = SD_TRUE;
            memcpy (&sto.ec_detected, &data_buf, sizeof (MI_EC_DETECTED));
            }
          }                        
        else 
          {
          MI_LOG_NERR0 ("InfoRpt Ind : Data conversion error");
          }                        
        }
      else  /* bad type ID					       */
        {
        MI_LOG_NERR1 ("InfoRpt Ind : Bad Type ID '%d'", type_id);
        }
      }
    else /* access result == failure					*/
      {
      MI_LOG_NERR0("InfoRpt Ind : Access Result == SD_FAILURE");
      }
    } /* for each variable						*/

/* Tell the user no more data is coming just now ... */
  u_mic_dv_ind_end (mi_conn, MIC_DV_IND_REASON_REPORT, mic_dsts, &sto);
  _mvl_free_comm_event (event);	/* All done with this event	*/
  }

/************************************************************************/
/*		mic_find_dsts_by_asn1_name 				*/
/************************************************************************/

MIC_DSTS *mic_find_dsts_by_asn1_name (MIC_VCC *mic_vcc, ACCESS_RESULT *ar_ptr)
  {
ST_INT type_id;
RUNTIME_TYPE *rt;
ST_INT rt_len;
MI_MMS_OBJECT_NAME ts_name;
MIC_DSTS *mic_dsts;
ST_INT i;
ST_RET rc;

  type_id = _mi_get_type_id (MI_TYPEID_MMS_OBJECT_NAME);
  if (mvl_get_runtime (type_id, &rt, &rt_len) == SD_SUCCESS)
    {
    rc = ms_asn1_to_local (rt, rt_len, ar_ptr->va_data.data,
                               ar_ptr->va_data.len, (ST_CHAR *)&ts_name);
    if (rc != SD_SUCCESS)
      {
      MI_LOG_NERR0 ("Could not convert transfer set name to local");
      return (NULL);
      }

    mic_dsts = mic_vcc->mic_dsts_tbl;
    for (i = 0; i < mic_vcc->num_dsts_ctrl; ++i, ++mic_dsts)
      {
      if ((mic_dsts->dsts_flags & MIC_DSTS_FLAGS_IN_USE) &&
          (strcmp (mic_dsts->ts_name.Name, ts_name.Name) == 0))
        {
        return (mic_dsts);
        }
      }
    }
  else
    {
    MI_LOG_NERR0 ("Could not get type_id for MI_TYPEID_MMS_OBJECT_NAME");
    }
  return (NULL);
  }

/************************************************************************/
/*			mic_find_dv_in_ds 				*/
/************************************************************************/

MIC_DV *mic_find_dv_in_ds (MIC_DS *mic_ds, ST_CHAR *dv_name)
  {
MIC_DV mic_dv;
MIC_DV *dvp;
MIC_DV **dvp_out;

  mic_dv.dv_name = dv_name;
  dvp = &mic_dv;
  dvp_out = (struct _mic_dv **)bsearch (&dvp, 
				   mic_ds->mic_sorted_dv_tbl, mic_ds->num_dv, 
		                   sizeof (MIC_DV *),
				   (int(*)(const void *, const void *))_mic_dv_name_compare);

  if (dvp_out != NULL)
    return (*dvp_out);

  return (NULL);
  }


/************************************************************************/
/************************************************************************/
/*			mic_write_dv 					*/
/************************************************************************/

MI_REQ_CTRL *mic_write_dv (MI_CONN *mi_conn, MIC_WR_DV_CTRL *mic_wr_dv_ctrl)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MVL_REQ_PEND *mvl_req;
MI_WRITE_REQ *mi_wr_req;
MI_WRITE_VAR_SPEC *mi_wr_vs;
ST_RET rc;
MIC_DV_WR_INFO *mic_dv_wr_info;
ST_INT num_dv;
ST_INT i;
MIC_DV *mic_dv;

  S_MI_LOCK_COMMON ();
  mic_vcc = mi_conn->mi_remote->mic_vcc;
  num_dv = mic_wr_dv_ctrl->num_dv;
  mi_wr_req = (struct _mi_write_req *)chk_calloc (1, sizeof (MI_WRITE_REQ) + 
		          (num_dv * sizeof (MI_WRITE_VAR_SPEC)));
  mi_wr_vs = (MI_WRITE_VAR_SPEC *) (mi_wr_req + 1);
  mi_wr_req->num_var = num_dv;
  mi_wr_req->vs = mi_wr_vs;

  mic_dv_wr_info = mic_wr_dv_ctrl->mic_dv_wr_info;
  for (i = 0; i < num_dv; ++i, ++mi_wr_vs, ++mic_dv_wr_info)
    {
    mic_dv = mic_dv_wr_info->mic_dv;
    mi_wr_vs->name = mic_dv->dv_name;
    if (mic_dv->dv_attrib & MIC_DV_SCOPE_ICC)
      {
      mi_wr_vs->scope = ICC_SPEC;
      mi_wr_vs->domain = mic_vcc->icc_name;
      }
    else
      mi_wr_vs->scope = VCC_SPEC;

    mi_wr_vs->mi_type = mic_dv->dv_attrib & MIC_DV_MI_TYPE_MASK;
    mi_wr_vs->src = mic_dv_wr_info->wr_data;
    }

  rc = _mic_write (mi_conn, mi_wr_req, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("_mic_write request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mi_wr_req->u_req_done = _mic_write_dv_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_wr_req->mi_req = mi_req;
  mi_req->mi_wr_req = mi_wr_req;
  mi_req->mic_wr_dv_ctrl = mic_wr_dv_ctrl;
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/*			mic_write_ds 					*/
/************************************************************************/

MI_REQ_CTRL *mic_write_ds (MI_CONN *mi_conn, MIC_DS *mic_ds, 
			   MIC_WR_DV_CTRL *mic_wr_dv_ctrl)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MVL_REQ_PEND *mvl_req;
MI_WRITE_REQ *mi_wr_req;
MI_WRITE_VAR_SPEC *mi_wr_vs;
ST_RET rc;
MIC_DV_WR_INFO *mic_dv_wr_info;
ST_INT num_dv;
ST_INT i;
MIC_DV *dv;

  S_MI_LOCK_COMMON ();
  mic_vcc = mi_conn->mi_remote->mic_vcc;
  num_dv = mic_ds->num_dv;
  mi_wr_req = (struct _mi_write_req *)chk_calloc (1, sizeof (MI_WRITE_REQ) + 
		          (num_dv * sizeof (MI_WRITE_VAR_SPEC)));
  mi_wr_vs = (MI_WRITE_VAR_SPEC *) (mi_wr_req + 1);
  mi_wr_req->num_var = num_dv;
  mi_wr_req->vs = mi_wr_vs;
  mi_wr_req->nvl_flag = SD_TRUE;
  mi_wr_req->nvl_name.object_tag = mic_ds->ds_scope;
  mi_wr_req->nvl_name.obj_name.vmd_spec = mic_ds->ds_name;
  mi_wr_req->nvl_name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;

  mic_dv_wr_info = mic_wr_dv_ctrl->mic_dv_wr_info;
  for (i = 0; i < num_dv; ++i, ++mi_wr_vs, ++mic_dv_wr_info)
    {
    dv = mic_ds->mic_dv_tbl[i];
    if (dv->dv_attrib & MIC_DV_SCOPE_ICC)
      {
      mi_wr_vs->scope = ICC_SPEC;
      mi_wr_vs->domain = mic_vcc->icc_name;
      }
    else
      mi_wr_vs->scope = VCC_SPEC;

    mi_wr_vs->mi_type = dv->dv_attrib & MIC_DV_MI_TYPE_MASK;
    mi_wr_vs->src = mic_dv_wr_info->wr_data;
    }

  rc = _mic_write (mi_conn, mi_wr_req, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("_mic_write request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mi_wr_req->u_req_done = _mic_write_dv_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  mi_wr_req->mi_req = mi_req;
  mi_req->mi_wr_req = mi_wr_req;
  mi_req->mic_wr_dv_ctrl = mic_wr_dv_ctrl;
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }


/************************************************************************/
/*			_mic_write_dv_done 				*/
/************************************************************************/

static ST_VOID _mic_write_dv_done (MI_WRITE_REQ *mi_wr_req)
  {
MI_REQ_CTRL *mi_req;
MIC_VCC *mic_vcc;
MI_WRITE_VAR_SPEC *mi_wr_vs;
ST_INT i;
MIC_WR_DV_CTRL *mic_wr_dv_ctrl;
MIC_DV_WR_INFO *mic_dv_wr_info;

  mi_req = mi_wr_req->mi_req;		/* retrieve the MI request control	*/
  mic_vcc = mi_req->mi_conn->mi_remote->mic_vcc;
  mi_wr_req = mi_req->mi_wr_req;

  if (mi_wr_req->result != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mic_write confirm error: %04x", mi_wr_req->result); 
    }

  mic_wr_dv_ctrl = mi_req->mic_wr_dv_ctrl;
  mic_dv_wr_info = mic_wr_dv_ctrl->mic_dv_wr_info;
  mi_req->result = mi_wr_req->result;
  mi_wr_vs = mi_wr_req->vs;
  for (i = 0; i < mi_wr_req->num_var; ++i, ++mi_wr_vs)
    {
    if (mi_wr_req->result == SD_SUCCESS)
      mic_dv_wr_info[i].result = mi_wr_vs->result;
    else
      mic_dv_wr_info[i].result = SD_FAILURE;

    if (mic_dv_wr_info[i].result != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("Write result error: %04x", mi_wr_vs->result); 
      } 
    }

  chk_free (mi_wr_req);
  _mi_req_done (mi_req);
  }


/************************************************************************/
/*			_mic_send_namelist				*/
/************************************************************************/

MVL_REQ_PEND *_mic_send_namelist (MI_CONN *mi_conn, 
			       ST_INT mi_obj_class, ST_CHAR *ca_name)
  {
NAMELIST_REQ_INFO name_info;
MIC_VCC *mic_vcc;
MVL_REQ_PEND *mvl_req;
ST_RET rc;

  S_MI_LOCK_COMMON ();
  mic_vcc = mi_conn->mi_remote->mic_vcc;
  name_info.cs_objclass_pres = SD_FALSE;

  switch (mi_obj_class)
    {
    case MIC_OBJ_CLASS_VCC_DV:
      name_info.obj.mms_class = 0;
      name_info.objscope = VCC_SPEC;
    break;

    case MIC_OBJ_CLASS_ICC_DV:
      name_info.obj.mms_class = 0;
      name_info.objscope = ICC_SPEC;
      strcpy (name_info.dname, mic_vcc->icc_name);
    break;

    case MIC_OBJ_CLASS_VCC_DS:
      name_info.obj.mms_class = 2;
      name_info.objscope = VCC_SPEC;
    break;

    case MIC_OBJ_CLASS_ICC_DS:
      name_info.obj.mms_class = 2;
      name_info.objscope = ICC_SPEC;
      strcpy (name_info.dname, mic_vcc->icc_name);
    break;

    default:
      MI_LOG_NERR1 ("mic_send_namelist: Invalid Object Class %d", mi_obj_class);
      S_MI_UNLOCK_COMMON ();
      return NULL;
    break;
    }
    
  if (ca_name)
    {
    name_info.cont_after_pres = SD_TRUE;
    strcpy (name_info.continue_after, ca_name);
    }
  else
    name_info.cont_after_pres = SD_FALSE;

  rc = mvla_getnam (mi_conn->net_info, &name_info, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mvla_getnam request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let's catch the confirm	*/
  S_MI_UNLOCK_COMMON ();
  return (mvl_req);
  }

/************************************************************************/
/*			mic_get_namelist 				*/
/************************************************************************/

MI_REQ_CTRL *mic_get_namelist (MI_CONN *mi_conn, 
			       ST_INT mi_obj_class, ST_CHAR *ca_name)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;

  S_MI_LOCK_COMMON ();
  mvl_req = _mic_send_namelist (mi_conn, mi_obj_class, ca_name);

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_get_namelist_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }


/************************************************************************/
/*			mic_get_namelist_done 				*/
/************************************************************************/

static ST_VOID _mic_get_namelist_done (MVL_REQ_PEND *mvl_req)
  {
MI_REQ_CTRL *mi_req;

  mi_req = mvl_req->mi_req;
  mi_req->o.gnl = mvl_req->u.getnam.resp_info;
  _mi_req_done (mi_req);
  }

/************************************************************************/
/*			mic_get_ds_names 				*/
/************************************************************************/

MI_REQ_CTRL *mic_get_ds_names (MI_CONN *mi_conn, ST_CHAR *ds_name, 
			       ST_INT ds_scope)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
GETVLIST_REQ_INFO req_info;
ST_RET rc;

  S_MI_LOCK_COMMON ();
  req_info.vl_name.object_tag = ds_scope;
  if (ds_scope == ICC_SPEC)
    {
    req_info.vl_name.obj_name.item_id = ds_name;
    req_info.vl_name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
    }
  else
    req_info.vl_name.obj_name.vmd_spec = ds_name;

  rc = mvla_getvlist (mi_conn->net_info, &req_info, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mvla_getvlist request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_getvlist_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }


/************************************************************************/
/*			_mic_getvlist_done 				*/
/************************************************************************/

static ST_VOID _mic_getvlist_done (MVL_REQ_PEND *mvl_req)
  {
MI_REQ_CTRL *mi_req;

  mi_req = mvl_req->mi_req;
  mi_req->o.get_ds_names = mvl_req->u.getvlist.resp_info;
  _mi_req_done (mi_req);
  }

/************************************************************************/
/*			mic_get_dv_attrib 				*/
/************************************************************************/

MI_REQ_CTRL *mic_get_dv_attrib (MI_CONN *mi_conn, ST_CHAR *dv_name, 
				ST_INT dv_scope)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;
GETVAR_REQ_INFO req_info;
ST_RET rc;

  S_MI_LOCK_COMMON ();
  req_info.req_tag = GETVAR_NAME;
  req_info.name.object_tag = dv_scope;
  if (dv_scope == ICC_SPEC)
    {
    req_info.name.obj_name.item_id = dv_name;
    req_info.name.domain_id = mi_conn->mi_remote->mic_vcc->icc_name;
    }
  else
    req_info.name.obj_name.vmd_spec = dv_name;


  rc = mvla_getvar (mi_conn->net_info, &req_info, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mvla_getvar request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    mi_op_err = rc;
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* The MMS request has gone out just fine, let's catch the confirm	*/
  mvl_req->u_req_done = _mic_get_dv_attrib_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/*			mic_get_dv_attrib_done 				*/
/************************************************************************/

static ST_VOID _mic_get_dv_attrib_done (MVL_REQ_PEND *mvl_req)
  {
MI_REQ_CTRL *mi_req;
GETVAR_RESP_INFO *resp_info;
ST_INT type_id = -1;
ST_INT i;
ST_UCHAR *asn1_data;
ST_INT asn1_len = 0;
#if defined(MVL_DYN_ASN1_TYPES)
ST_UCHAR *asn1_buf = NULL;
ST_UCHAR *asn1_start;
RUNTIME_TYPE *rt;
ASN1_ENC_CTXT localEncCtx;	/* For readability, use "aCtx" to access this.*/
ASN1_ENC_CTXT *aCtx = &localEncCtx;
ST_INT rt_num;
ST_RET rc;
#endif

  mi_req = mvl_req->mi_req;
  if (mvl_req->result == SD_SUCCESS)
    {
    resp_info = mvl_req->u.getvar.resp_info;

  /* figure out mi_type by comparing asn1   */
#if defined(MVL_DYN_ASN1_TYPES)
    asn1_buf = (unsigned char *)M_MALLOC (MSMEM_ASN1_DATA_ENC, mvl_cfg_info->max_msg_size);
#endif

    for (i = 0; i < mvl_num_types; ++i)
      {
#if !defined(MVL_DYN_ASN1_TYPES)
      asn1_len = mvl_type_ctrl[i].asn1_len;
      asn1_data = mvl_type_ctrl[i].asn1;
#else
      mvl_get_runtime (i, &rt, &rt_num);

      asn1r_strt_asn1_bld (aCtx, asn1_buf, mvl_cfg_info->max_msg_size);
      rc = ms_runtime_to_asn1 (aCtx, rt, rt_num);
      if (rc == SD_SUCCESS)
        {
        asn1_start = aCtx->asn1r_field_ptr+1;
        asn1_len = (asn1_buf + mvl_cfg_info->max_msg_size) - asn1_start;
	asn1_data = asn1_start;
        }
      else
        {
        MI_LOG_NERR0 ("GetVarAttrib : Could not create ASN.1 from RUNTIME");
	asn1_len = 0;
	asn1_data = NULL;
        }
#endif
      if ((asn1_len == resp_info->type_spec.len) &&
	  (memcmp (asn1_data, resp_info->type_spec.data, asn1_len) == 0))
	{
	type_id = i;
	break;
	}
      }
#if defined(MVL_DYN_ASN1_TYPES)
    M_FREE (MSMEM_ASN1_DATA_ENC, asn1_buf);
#endif
   if (type_id != -1)
     mi_req->o.mi_type = _mi_get_mi_type (type_id);
   else
     mi_req->result = SD_FAILURE;
    }
  else /* MVL request error */
     mi_req->result = mvl_req->result;



  _mi_req_done (mi_req);
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			_mic_read 					*/
/************************************************************************/

static ST_RET _mic_read (MI_CONN *mi_conn, MI_READ_REQ *mi_rd_req, 
		         MVL_REQ_PEND **mvl_req_out)
  {
MVL_READ_RESP_PARSE_INFO *parse_info;
MVL_READ_RESP_PARSE_INFO *pi;
VARIABLE_SPEC *vs;
MI_READ_VAR_SPEC *mi_rd_vs;
READ_REQ_INFO *read_info;
VARIABLE_LIST *vl;
ST_RET rc;
MVL_REQ_PEND *mvl_req;
ST_INT num_var;
ST_INT i;

  /* Create a read request info struct  */
  num_var = mi_rd_req->num_var;
  if (mi_rd_req->nvl_flag == SD_FALSE)
    {
    read_info = (READ_REQ_INFO *) chk_calloc (1, sizeof (READ_REQ_INFO) +
  				(num_var * sizeof (VARIABLE_LIST)));
    read_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
    read_info->va_spec.num_of_variables = num_var;
    }
  else
    {
    read_info = (READ_REQ_INFO *) chk_calloc (1, sizeof (READ_REQ_INFO));
    read_info->va_spec.var_acc_tag = VAR_ACC_NAMEDLIST;
    read_info->va_spec.vl_name.object_tag = mi_rd_req->nvl_name.object_tag;
    read_info->va_spec.vl_name.obj_name.vmd_spec = mi_rd_req->nvl_name.obj_name.vmd_spec;
    read_info->va_spec.vl_name.domain_id = mi_rd_req->nvl_name.domain_id;
    }
  read_info->spec_in_result = SD_FALSE;

  parse_info = (MVL_READ_RESP_PARSE_INFO *) chk_calloc (num_var,
		    sizeof (MVL_READ_RESP_PARSE_INFO));

  mi_rd_vs = mi_rd_req->vs;
  vl = (VARIABLE_LIST *) (read_info + 1);
  pi = parse_info;
  for (i = 0; i < num_var; ++i, ++vl, ++pi, ++mi_rd_vs)
    {
    if (mi_rd_req->nvl_flag == SD_FALSE)
      {
      vs = &vl->var_spec;
      vs->var_spec_tag = VA_SPEC_NAMED;

      vs->vs.name.object_tag = mi_rd_vs->scope;
      vs->vs.name.obj_name.vmd_spec = mi_rd_vs->name;
      vs->vs.name.domain_id = mi_rd_vs->domain;
      }

  /* Now take care of the MVL parse information */
    pi->type_id = _mi_get_type_id (mi_rd_vs->mi_type);
    pi->dest = mi_rd_vs->dest;
    /* pi->descr_arr = SD_FALSE; 	Already done 			*/
    }

  /* send read request and do not wait for reply */
  rc = mvla_read_variables (mi_conn->net_info, read_info, num_var,
			     parse_info, &mvl_req);

  chk_free (read_info);
  *mvl_req_out = mvl_req;
  if (rc != SD_SUCCESS)	/* The service failed				*/
    {
    mvl_free_req_ctrl (mvl_req);
    chk_free (parse_info);
    return (SD_FAILURE);
    }

  mvl_req->v = mi_rd_req;
  mvl_req->u_req_done = _mic_read_done;
  mi_rd_req->done = SD_FALSE;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			_mic_read_done 					*/
/************************************************************************/

static ST_VOID _mic_read_done (MVL_REQ_PEND *mvl_req)
  {
ST_RET rc;
MVL_READ_RESP_PARSE_INFO *parse_info;
MVL_READ_RESP_PARSE_INFO *pi;
MI_READ_REQ *mi_rd_req;
MI_READ_VAR_SPEC *mi_rd_vs;
ST_INT num_var;
ST_INT i;

  mi_rd_req = (struct _mi_read_req *)mvl_req->v;
  rc = mvl_req->result;
  mi_rd_req->result = rc;

  num_var = mi_rd_req->num_var;
  parse_info = mvl_req->u.rd.parse_info;
  pi = parse_info;
  mi_rd_vs = mi_rd_req->vs;
  
  for (i = 0; i < num_var; ++i, ++pi, ++mi_rd_vs)
    {
    if (rc == SD_SUCCESS)
      mi_rd_vs->result = pi->result;
    else
      mi_rd_vs->result = SD_FAILURE;
    }

  chk_free (parse_info);
  mi_rd_req->done = SD_TRUE;
  if (mi_rd_req->u_req_done)		/* Call the user done function		*/
    (*mi_rd_req->u_req_done)(mi_rd_req);
  }

/************************************************************************/
/************************************************************************/
/*			_mic_write 					*/
/************************************************************************/

ST_RET _mic_write (MI_CONN *mi_conn, MI_WRITE_REQ *mi_wr_req, 
		   MVL_REQ_PEND **mvl_req_out)
  {
MVL_WRITE_REQ_INFO *mvl_write_info;
MVL_WRITE_REQ_INFO *mwi;
VARIABLE_SPEC *vs;
MI_WRITE_VAR_SPEC *mi_wr_vs;
WRITE_REQ_INFO *write_info;
VARIABLE_LIST *vl;
ST_RET rc;
MVL_REQ_PEND *mvl_req;
ST_INT num_var;
ST_INT i;

  /* Create a write request info struct  */
  num_var = mi_wr_req->num_var;

  if (mi_wr_req->nvl_flag == SD_FALSE)
    {
    write_info = (WRITE_REQ_INFO *) chk_calloc (1, sizeof (WRITE_REQ_INFO) +
				(num_var * sizeof (VARIABLE_LIST)) +
				(num_var * sizeof (VAR_ACC_DATA)));

    write_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
    write_info->va_spec.num_of_variables = num_var;
    }
  else
    {
    write_info = (WRITE_REQ_INFO *) chk_calloc (1, sizeof (WRITE_REQ_INFO) +
				(num_var * sizeof (VAR_ACC_DATA)));

    write_info->va_spec.var_acc_tag = VAR_ACC_NAMEDLIST;
    write_info->va_spec.vl_name.object_tag = mi_wr_req->nvl_name.object_tag;
    write_info->va_spec.vl_name.obj_name.vmd_spec = mi_wr_req->nvl_name.obj_name.vmd_spec;
    write_info->va_spec.vl_name.domain_id = mi_wr_req->nvl_name.domain_id;
    }


  mvl_write_info = (MVL_WRITE_REQ_INFO *) chk_calloc (num_var,
		    sizeof (MVL_WRITE_REQ_INFO));

  mi_wr_vs = mi_wr_req->vs;
  vl = (VARIABLE_LIST *) (write_info + 1);
  mwi = mvl_write_info;
  for (i = 0; i < num_var; ++i, ++vl, ++mwi, ++mi_wr_vs)
    {
    if (mi_wr_req->nvl_flag == SD_FALSE)
      {
      vs = &vl->var_spec;
      vs->var_spec_tag = VA_SPEC_NAMED;

      vs->vs.name.object_tag = mi_wr_vs->scope;
      vs->vs.name.obj_name.vmd_spec = mi_wr_vs->name;
      vs->vs.name.domain_id = mi_wr_vs->domain;
      }
    mwi->type_id = _mi_get_type_id (mi_wr_vs->mi_type);
    mwi->local_data = mi_wr_vs->src;
    }

  /* send write request and do not wait for reply */
  rc = mvla_write_variables (mi_conn->net_info, write_info, num_var,
			     mvl_write_info, &mvl_req);

  chk_free (write_info);
  *mvl_req_out = mvl_req;
  if (rc != SD_SUCCESS)	/* The service failed				*/
    {
    mvl_free_req_ctrl (mvl_req);
    chk_free (mvl_write_info);
    return (SD_FAILURE);
    }

  mvl_req->v = mi_wr_req;
  mvl_req->u_req_done = _mic_write_done;
  mi_wr_req->done = SD_FALSE;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			_mic_write_done 				*/
/************************************************************************/

static ST_VOID _mic_write_done (MVL_REQ_PEND *mvl_req)
  {
ST_RET rc;
MVL_WRITE_REQ_INFO *mvl_write_info;
MVL_WRITE_REQ_INFO *mwi;
MI_WRITE_REQ *mi_wr_req;
MI_WRITE_VAR_SPEC *mi_wr_vs;
ST_INT num_var;
ST_INT i;

  mi_wr_req = (struct _mi_write_req *)mvl_req->v;
  rc = mvl_req->result;
  mi_wr_req->result = rc;

  num_var = mi_wr_req->num_var;
  mvl_write_info = mvl_req->u.wr.w_info;
  mwi = mvl_write_info;
  mi_wr_vs = mi_wr_req->vs;
  
  for (i = 0; i < num_var; ++i, ++mwi, ++mi_wr_vs)
    {
    if (rc == SD_SUCCESS)
      mi_wr_vs->result = mwi->result;
    else
      mi_wr_vs->result = SD_FAILURE;
    }

  chk_free (mvl_write_info);
  mi_wr_req->done = SD_TRUE;
  if (mi_wr_req->u_req_done)		/* Call the user done function		*/
    (*mi_wr_req->u_req_done)(mi_wr_req);
  }

/************************************************************************/
/************************************************************************/
/*			_mic_del_nvl 					*/
/************************************************************************/

ST_RET _mic_del_nvl (MI_CONN *mi_conn, OBJECT_NAME *nvl, 
		  MVL_REQ_PEND **mvl_req_out)
  {
MVL_REQ_PEND *mvl_req;
ST_RET rc;
DELVLIST_REQ_INFO *req_info;
MIC_VCC *mic_vcc;
OBJECT_NAME *obj_name;

  mic_vcc = mi_conn->mi_remote->mic_vcc;

/* Issue the request */
  req_info = (struct delvlist_req_info *)chk_calloc (1, sizeof (DELVLIST_REQ_INFO) +
			 (1 * sizeof (OBJECT_NAME)));

  req_info->scope = DELVL_SPEC;
  req_info->dname_pres = SD_FALSE;
  req_info->vnames_pres = 1;
  req_info->num_of_vnames = 1;
  obj_name = (OBJECT_NAME *) (req_info +1);
  memcpy (obj_name, nvl, sizeof (OBJECT_NAME));

/* OK, the MMS request info is ready, send the request	*/
  rc = mvla_delvlist (mi_conn->net_info, req_info, &mvl_req);
  chk_free (req_info);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("mvla_delvlist request error : %04x", rc); 
    mvl_free_req_ctrl (mvl_req);
    return (rc);
    }

/* All is OK, return success */
  *mvl_req_out = mvl_req;
  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			mic_clean_assoc 				*/
/************************************************************************/

ST_VOID mic_clean_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
MIC_VCC *mic_vcc;
MIC_DSTS *mic_dsts;
ST_INT i;

  mic_vcc = mi_assoc->mi_remote->mic_vcc;

  for (i = 0; i < mic_vcc->num_dsts_ctrl; ++i)
    {
    mic_dsts = &mic_vcc->mic_dsts_tbl[i];
    if (mic_dsts->dsts_flags & MIC_DSTS_FLAGS_IN_USE)
      {
      if (mic_dsts->mi_conn->mi_assoc == mi_assoc)
        mic_dsts->dsts_flags &= ~MIC_DSTS_FLAGS_IN_USE;
      }
    }
  }


