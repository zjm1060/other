/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mis.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 08/09/02  RKR     56    fixed problem with logging read only value   */
/* 07/24/02  JRB     55    Call mvl_send_resp_i, NOT mvl_send_resp.	*/
/* 07/10/02  JRB     54    Call mvl_ureq_bufs_avail.			*/
/* 06/20/02  RKR     53    Copyright year update                        */
/* 06/18/02  MDE     52	   Fixed handling of no available DSTS 		*/
/* 06/17/02  ASK     51	   correct MMS failure when DV is 'read-only'	*/
/* 06/10/02  MDE     50    Updated RPT service flow control for LEAN_T	*/
/* 05/08/02  MDE     49    Added num_dsts_rpt_pend			*/
/* 04/25/02  MDE     48    Write enable checked for DV writes		*/
/* 04/24/02  MDE     47    DSTS processing cleanup			*/
/* 04/17/02  MDE     46    Minor logging changes			*/
/* 04/10/02  MDE     45    Fixed ACSE resource check			*/
/* 03/21/02  MDE     44    Check ACSE resources before processing DSTS	*/
/* 02/25/02  MDE     43    Now get max PDU size from mvl_cfg_info	*/
/* 02/05/02  MDE     42    Fixed min interval chk, added for integrity	*/
/* 01/17/02  ASK     41    Made changes for asn1r 			*/
/* 01/03/02  ASK     40	   reset mis_num_dv_ctrl & mis_max_num_dv_ctrl	*/
/*			   in mis_free_dv_tables  			*/
/* 12/05/01  NAV     39    mis_handle_dsts_var_write check min_interval	*/
/* 10/23/01  NAV     38    Clean up tempTime				*/
/* 11/28/01  MDE     37	   Removed allocation of mmsl_enc_buf		*/
/* 11/28/01  MDE     36	   Added and used MIS_DV_INDEX			*/
/* 11/15/01  MDE     35	   Fixed problem with read ind of 0 var		*/
/* 11/15/01  MDE     34	   Changed the ST_UCHAR ???[1] to ST_UCHAR ???	*/
/* 11/07/01  MDE     33	   Fixed prob. with mis_dv_change (idx vs ref)	*/
/* 11/07/01  MDE     32	   Minor logging changes			*/
/* 10/29/01  MDE     31	   minor cleanup				*/
/* 10/29/01  MDE     30	   Added config logging				*/
/* 10/19/01  MDE     29	   Dynamic local object reconfiguration changes	*/
/* 10/19/01  MDE     28	   Removed limits on DV per DS			*/
/* 10/18/01  RKR     27    added code to do Read Resp specInResult      */
/*                         DefNVL looks at (num_var > MIS_MAX_DV_PER_DS)*/
/* 06/27/01  RKR     26    fixed error code in _mis_process_getvaa_ind 	*/
/* 06/27/01  RKR     25    fixed another problem handling write ind	*/
/* 06/26/01  RKR     24    merged corrections in mis_create_vcc and	*/
/*			   start_dsts_report				*/
/* 06/19/01  ASK     23    fixed return value in _mis_find_ca_dv_index	*/
/* 06/08/01  RKR     22    fixed problem handling write indication	*/
/* 12/14/00  RKR     21    changed DataSet name ordering                */
/* 12/08/00  RKR     20    took out code from _mis_process_info_ind     */
/* 12/07/00  RKR     19    added mis_create_dsts_names                  */
/* 12/05/00  RKR     18    added u_mvl_concl_ind			*/
/* 09/29/00  RKR     17    worked on mis_free_dv_tables			*/
/* 09/07/00  NAV     16    Use time_t to calc DsTs intervals		*/
/* 08/30/00  RKR     15    Fixed NACK_VARID in _mis_process_info_ind    */
/* 08/25/00  RKR     14    Compiled out a slog macro			*/
/* 08/23/00  NAV     13    Correct Error Responses			*/
/* 07/06/00  NAV     12    Add MISEM support				*/
/* 08/21/00  NAV     11    Declare some globals                         */
/* 07/27/00  RKR     10    Minor changes to log messages                */
/* 07/26/00  RKR     09    Increased MIS_MAX_DSTS from 10 to 30         */
/* 07/25/00  RKR     08    fixed up list_unlink problems for rd & wr    */
/* 04/19/00  RKR     07    SMEM changes					*/
/* 11/24/99  MDE     06    Fixed remote ar name logging			*/
/* 10/01/99  NAV     05    Add Block 4 and Block 5			*/
/* 04/19/99  MDE     04    Fixed problem handling with Alternate Access	*/
/* 04/12/99  MDE     03    Minor cleanup				*/
/* 04/01/99  MDE     02    Added MI_ASSOC_CTRL and associated code	*/
/* 12/31/98  MDE     01    Created					*/
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
MIS_DV_INDEX *mis_dv_ctrl_index;	/* Sorted index array for DV	*/	
MIS_DV *mis_dv_ctrl; 			/* table of ptrs to DV tables	*/
ST_INT mis_num_dv_ctrl;			/* Number of DV in tables	*/
ST_INT mis_num_sorted_dv_ctrl;		/* Number of sorted DV in tbl	*/
ST_INT mis_max_num_dv_ctrl; 		/* Size of table		*/
/************************************************************************/

ST_INT _mi_gnl_ind_vars (MVL_IND_PEND *mvl_ind_ctrl, NAMELIST_REQ_INFO *req_info, 
			 ST_CHAR **ptr, ST_BOOLEAN *more_follows_out, 
			 ST_INT max_names);

ST_INT _mi_gnl_ind_nvls (MVL_IND_PEND *mvl_ind_ctrl, 
			     NAMELIST_REQ_INFO *req_info, 
			     ST_CHAR **ptr, ST_BOOLEAN *more_follows_out, 
			     ST_INT max_names);

ST_INT _mi_gnl_ind_doms (MVL_IND_PEND *mvl_ind_ctrl, 
			     NAMELIST_REQ_INFO *req_info, 
			     ST_CHAR **ptr, ST_BOOLEAN *more_follows_out, 
			     ST_INT max_names);

/************************************************************************/
int _mi_dv_name_compare (const MIS_DV_INDEX *idx1, const MIS_DV_INDEX *idx2);
int _mi_dv_name_compare2 (const MIS_DV *dv1, const MIS_DV_INDEX *idx2);
int _mi_dv_ref_compare (const MIS_DV_REF *dv_ref1, const MIS_DV_REF *dv_ref2);
int _mi_dv_ref_compare2 (const MIS_DV *dv, const MIS_DV_REF *dv_ref2);

static ST_RET _mi_prep_lov_rd_ctrl (READ_REQ_INFO *read_req_info, 
				    MIS_READ_CTRL *mis_rd_ctrl);
static ST_RET _mi_prep_nvl_rd_ctrl (READ_REQ_INFO *read_req_info, 
				MIS_READ_CTRL *mis_rd_ctrl);

ST_RET mi_get_dv_asn1_data (ST_VOID *data, ST_INT mi_type,
                         ST_UCHAR *asn1_dest_buffer,
                         ST_INT asn1_buffer_len,
                         ST_INT *asn1_len_out);

static ST_RET _mi_prep_read_resp (MIS_READ_CTRL *mi_rd_ctrl,
                          	  READ_RESP_INFO **resp_info_out, 
                          	  ST_UCHAR *asn1_buf, ST_INT asn1_buf_size);

MIS_DV_REF _mis_find_ca_dv_index (ST_CHAR *ca_name, ST_INT num_dv_ref, 
				 MIS_DV_REF *mis_dv_ref_tbl);
ST_CHAR *_mis_dv_ref_to_name (MIS_DV_REF dv_ref);

ST_INT _mis_find_ca_ds_index (ST_CHAR *ca_name, ST_INT num_ds, 
			      MIS_DS **mis_ds_ptr_tbl);
ST_VOID _mis_handle_special_var_write (MIS_WR_DV_CTRL *mis_wr_dv_ctrl,
				    ST_UCHAR *asn1_data, ST_INT asn1_len);
ST_VOID _mis_handle_dsts_var_write (MIS_WR_DV_CTRL *mis_wr_dv_ctrl,
				    ST_UCHAR *asn1_data, ST_INT asn1_len);

ST_VOID _mis_handle_special_var_read (MIS_RD_DV_CTRL *mis_rd_dv_ctrl);
ST_INT _mis_get_special_var_type_id (ST_CHAR *_name);

MIS_RD_DV_CTRL *_mis_ds_to_rd_dv_ctrl (MIS_VCC *mis_vcc, MIS_DS *mis_ds,
					ST_VOID *ctrl, ST_INT rd_reason);

ST_INT _mis_dsts_process (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts);
ST_VOID _mis_start_dsts_report (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts);

static ST_RET _mis_dsts_saturated (MIS_DSTS *mis_dsts);
ST_RET _mis_send_dsts_rpt (MIS_INFO_RPT_CTRL *mis_info_ctrl);
MIS_DSTS *mis_find_dsts (MIS_VCC *mis_vcc, MI_MMS_OBJECT_NAME *ts_name);
ST_VOID _mis_ack_rcvd (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts, 
		       MI_MMS_OBJECT_NAME *ts_name, 
		       MI_TS_TIMESTAMP ts_timestamp);
ST_VOID _mis_nack_rcvd (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts, 
			MI_MMS_OBJECT_NAME *ts_name);

ST_DOUBLE _mi_curr_ms_time;
time_t _mi_curr_sec_time;

/************************************************************************/

#define MIS_MAX_DSTS	30
static ST_INT _mis_max_dsts = MIS_MAX_DSTS;
ST_CHAR **_mis_transfer_set_names;

/************************************************************************/

#define _MI_NUM_SPECIAL_VCC_DV	2
#define _MI_NUM_SPECIAL_ICC_DV	6
#define _MI_NUM_SPECIAL_DV (_MI_NUM_SPECIAL_VCC_DV + _MI_NUM_SPECIAL_ICC_DV)

MIS_SPECIAL_VAR_ATTRIB _mis_special_vars[] =
  {
    {
    "Supported_Features",		/* name 	*/
    MI_TYPEID_SUPPORTED_FEATURES,	/* mi_type	*/
    VCC_SPEC				/* scope	*/
    },
    {
    "TASE2_Version",			/* name 	*/
    MI_TYPEID_TASE2_VERSION,		/* mi_type	*/
    VCC_SPEC				/* scope	*/
    },
    {
    "Bilateral_Table_ID",		/* name 	*/
    MI_TYPEID_VISIBLE_STRING_32,	/* mi_type	*/
    ICC_SPEC				/* scope	*/
    },
    {
    "DSConditions_Detected",		/* name 	*/
    MI_TYPEID_DS_COND,			/* mi_type	*/
    ICC_SPEC				/* scope	*/
    },
    {
    "Event_Code_Detected",		/* name 	*/
    MI_TYPEID_INT16,			/* mi_type	*/
    ICC_SPEC				/* scope	*/
    },
    {
    "Next_DSTransfer_Set",		/* name 	*/
    MI_TYPEID_MMS_OBJECT_NAME,		/* mi_type	*/
    ICC_SPEC				/* scope	*/
    },
    {
    "Transfer_Set_Name",		/* name 	*/
    MI_TYPEID_MMS_OBJECT_NAME,		/* mi_type	*/
    ICC_SPEC				/* scope	*/
    },
    {
    "Transfer_Set_Time_Stamp",		/* name 	*/
    MI_TYPEID_GMT_BASED_S,		/* mi_type	*/
    ICC_SPEC				/* scope	*/
    },
  };
ST_INT _mi_num_special_vars = 
  	sizeof (_mis_special_vars)/sizeof (MIS_SPECIAL_VAR_ATTRIB);

/************************************************************************/
/*			_mi_is_special_var				*/
/************************************************************************/

ST_BOOLEAN _mi_is_special_var (ST_CHAR *name)
  {
ST_INT i;

  for (i = 0; i < _mi_num_special_vars; ++i)
    {
    if (!strcmp (_mis_special_vars[i].name, name))
      return (SD_TRUE);
    }
  return (SD_FALSE);
  }

/************************************************************************/
/*			_mis_start 					*/
/************************************************************************/

ST_RET _mis_start (MI_CONN *mi_conn)
  {
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                         mis_create_dsts_names                        */
/************************************************************************/

ST_RET mis_create_dsts_names(ST_INT max_num_dsts)
  {
  if (max_num_dsts<=0)
    {
    return (SD_FAILURE);
    }
  _mis_max_dsts = max_num_dsts;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			mis_create_dv_tables 				*/
/************************************************************************/

ST_RET mis_create_dv_tables (ST_INT total_num_dv)
  {
ST_INT i;

  total_num_dv += _MI_NUM_SPECIAL_DV;
  if (total_num_dv > MIS_MAX_TOTAL_DV)
    {
    MI_LOG_NERR0 ("Too many Data Values"); 
    return (SD_FAILURE);
    }

  S_MI_LOCK_COMMON ();
  mis_dv_ctrl_index = (unsigned long *)chk_calloc (total_num_dv, sizeof (MIS_DV_INDEX)); 
/*  mis_dv_ctrl = (_mis_dv *)chk_calloc (total_num_dv, sizeof (MIS_DV)); 
modified 2006.11.23 zy*/
  mis_dv_ctrl = (struct _mis_dv *)chk_calloc (total_num_dv, sizeof (MIS_DV)); 
  mis_max_num_dv_ctrl = total_num_dv;
  mis_num_dv_ctrl = 0;
  mis_num_sorted_dv_ctrl = 0;

/* Add the special variables to the global set */
  for (i = 0; i < _mi_num_special_vars; ++i)
    mis_add_dv (_mis_special_vars[i].name, NULL);

  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			mis_resize_dv_tables 				*/
/************************************************************************/

ST_RET mis_resize_dv_tables (ST_INT total_num_dv)
  {
  total_num_dv += _MI_NUM_SPECIAL_DV;
  if (total_num_dv > MIS_MAX_TOTAL_DV)
    {
    MI_LOG_NERR0 ("Too many Data Values"); 
    return (SD_FAILURE);
    }

/* See if the new count is less or equal to the previous */
  if (total_num_dv <= mis_max_num_dv_ctrl)
    {
    MI_LOG_FLOW2 ("Resize unnecessary (new %d, curr %d)", total_num_dv, mis_max_num_dv_ctrl); 
    return (SD_SUCCESS);
    }

/* OK, we just need to realloc the tables and set the new limit */
  S_MI_LOCK_COMMON ();
  mis_dv_ctrl_index = (unsigned long *)chk_realloc (mis_dv_ctrl_index, 
	total_num_dv * sizeof (MIS_DV_INDEX)); 

  mis_dv_ctrl = (struct _mis_dv *)chk_realloc (mis_dv_ctrl, total_num_dv * sizeof (MIS_DV)); 
  mis_max_num_dv_ctrl = total_num_dv;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			mis_free_dv_tables 				*/
/************************************************************************/

ST_RET mis_free_dv_tables (ST_VOID)
  {
  S_MI_LOCK_COMMON ();
  if (mis_dv_ctrl !=NULL)
    {
    chk_free (mis_dv_ctrl);
    chk_free (mis_dv_ctrl_index);
    mis_dv_ctrl = NULL;
    mis_dv_ctrl_index = NULL;
    mis_num_dv_ctrl = 0;
    mis_max_num_dv_ctrl = 0;
    }
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			mis_add_dv 					*/
/************************************************************************/

ST_RET mis_add_dv (ST_CHAR *dv_name, MIS_DV_ACCESS_INFO access_info)
  {
MIS_DV_REF dv_index;

  return (mis_add_dv_ex (dv_name, access_info, &dv_index));
  }

/************************************************************************/
/*			mis_add_dv_ex 					*/
/************************************************************************/

ST_RET mis_add_dv_ex (ST_CHAR *dv_name, MIS_DV_ACCESS_INFO access_info, 
		     MIS_DV_INDEX *dv_index_out)
  {
MIS_DV *mis_dv;

  S_MI_LOCK_COMMON ();

/* OK, this one is not in the dv table, add it if possible */
  if (mis_num_dv_ctrl >= mis_max_num_dv_ctrl)
    {
    MI_LOG_NERR0 ("Too many Data Values"); 
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }

  mis_dv = &mis_dv_ctrl[mis_num_dv_ctrl];
  mis_dv->dv_name = dv_name;
  mis_dv->access_info = access_info;
  mis_dv_ctrl_index[mis_num_dv_ctrl] = mis_num_dv_ctrl;
  *dv_index_out = mis_num_dv_ctrl;
  ++mis_num_dv_ctrl;

  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			mis_sort_dv_tables 				*/
/************************************************************************/

ST_VOID mis_sort_dv_tables (ST_VOID)
  {
  S_MI_LOCK_COMMON ();
  qsort (mis_dv_ctrl_index, mis_num_dv_ctrl, sizeof (MIS_DV_INDEX), (int(*)(const void *, const void *))_mi_dv_name_compare);
  mis_num_sorted_dv_ctrl = mis_num_dv_ctrl;
  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/*			_mi_dv_name_compare 				*/
/************************************************************************/
int _mi_dv_name_compare (const MIS_DV_INDEX *idx1, const MIS_DV_INDEX *idx2)
  {
MIS_DV *dv1;
MIS_DV *dv2;

  dv1 = &mis_dv_ctrl[*idx1];
  dv2 = &mis_dv_ctrl[*idx2];
  return (strcmp (dv1->dv_name, dv2->dv_name));
  }

/************************************************************************/
/*			mis_find_dv 					*/
/************************************************************************/

ST_RET mis_find_dv (ST_CHAR *dv_name, MIS_DV_INDEX *dv_index_out)
  {
MIS_DV_INDEX *dv_index_found;
MIS_DV dv;
ST_INT i;
MIS_DV_INDEX idx;

  S_MI_LOCK_COMMON ();
  dv.dv_name = dv_name;
  dv_index_found = (unsigned long *)bsearch (&dv, mis_dv_ctrl_index,
					       mis_num_sorted_dv_ctrl, 
		    sizeof (MIS_DV_INDEX), 
		    (int(*)(const void *, const void *))_mi_dv_name_compare2);
  if (dv_index_found == NULL)
    {
  /* We will now look in the unsorted devices, if any */

    for (i = mis_num_sorted_dv_ctrl; i < mis_num_dv_ctrl; ++i)
      {
      idx = mis_dv_ctrl_index[i];
      if (!strcmp (dv_name, mis_dv_ctrl[idx].dv_name))
        { 
        dv_index_found = &idx;
        break;
        }
      }
    if (i >= mis_num_dv_ctrl)
      {
      S_MI_UNLOCK_COMMON ();
      return (SD_FAILURE);
      }
    }
  *dv_index_out = *dv_index_found;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			_mi_dv_name_compare2 				*/
/************************************************************************/
int _mi_dv_name_compare2 (const MIS_DV *dv1, const MIS_DV_INDEX *idx2)
  {
MIS_DV *dv2;

  dv2 = &mis_dv_ctrl[*idx2];
  return (strcmp (dv1->dv_name, dv2->dv_name));
  }

/************************************************************************/
/*			mis_log_dv_tables 				*/
/************************************************************************/

ST_VOID mis_log_dv_tables ()
  {
ST_INT i;
MIS_DV_INDEX idx;

  SLOGCALWAYS2 ("Local Data Value Table (%d Data Values, %d sorted)", 
			mis_num_dv_ctrl, mis_num_sorted_dv_ctrl)
  for (i = 0; i < mis_num_dv_ctrl; ++i)
    {
    idx = mis_dv_ctrl_index[i];
    SLOGCALWAYS3 ("  %4d) %-32.32s   %-32.32s", i+1, 
                          mis_dv_ctrl[idx].dv_name,
			  mis_dv_ctrl[i].dv_name);
    }
  }

/************************************************************************/
/*			mis_create_vcc 					*/
/************************************************************************/

#define MAX_DSTS_NAME_LEN 10	

MIS_VCC *mis_create_vcc (MI_REMOTE *mi_remote, 
		       ST_CHAR *bi_lat_tbl_id, ST_CHAR *icc_name, 
  		       ST_INT num_vcc_dv_ref, ST_INT num_vcc_ds, 
  		       ST_INT num_icc_dv_ref, ST_INT num_icc_ds, 
		       ST_INT num_dsts_ctrl, ST_INT min_interval)
  {
MIS_VCC *mis_vcc;
ST_INT dv_count;
ST_INT i;
ST_CHAR *p;
ST_INT buf_size;

  S_MI_LOCK_COMMON ();

  if (num_dsts_ctrl > _mis_max_dsts)
    {
    MI_LOG_NERR0 ("Too many Dataset Transfer Sets"); 
    S_MI_UNLOCK_COMMON ();
    return (NULL);
    }

/* Allocate the VCC control and associated tables */
  mis_vcc = (struct _mis_vcc *)chk_calloc (1, sizeof (MIS_VCC));
  mi_remote->mis_vcc = mis_vcc;
  mis_vcc->mi_remote = mi_remote;

  dv_count = num_vcc_dv_ref + _MI_NUM_SPECIAL_VCC_DV;
  mis_vcc->max_vcc_dv_ref = dv_count;
  mis_vcc->vcc_dv_ref_tbl = (unsigned long *)chk_calloc (dv_count, sizeof (MIS_DV_REF));

  mis_vcc->icc_name = icc_name;
  dv_count = num_icc_dv_ref + _MI_NUM_SPECIAL_ICC_DV;
  mis_vcc->max_icc_dv_ref = dv_count;
  mis_vcc->icc_dv_ref_tbl = (unsigned long *)chk_calloc (dv_count, sizeof (MIS_DV_REF));

  mis_vcc->max_vcc_ds = num_vcc_ds;
  if (num_vcc_ds)
    mis_vcc->vcc_ds_ctrl_tbl = (struct _mis_ds **)chk_calloc (num_vcc_ds, sizeof (MIS_DS *));	

  mis_vcc->max_icc_ds = num_icc_ds;
  if (num_icc_ds)
    mis_vcc->icc_ds_ctrl_tbl = (struct _mis_ds **)chk_calloc (num_icc_ds, sizeof (MIS_DS *));

  mis_vcc->num_dsts_ctrl = num_dsts_ctrl;
  if (num_dsts_ctrl)
    mis_vcc->mis_dsts_tbl = (struct _mis_dsts *)chk_calloc (num_dsts_ctrl, sizeof (MIS_DSTS));

  mis_vcc->min_interval = min_interval;

/* Create the table of DSTS names once. All MIS_VCC structures share the*/
/* the same set of names                                                */
  if (_mis_transfer_set_names == NULL)
    {
    buf_size = _mis_max_dsts * (sizeof(ST_CHAR *) + (MAX_DSTS_NAME_LEN + 1));
    _mis_transfer_set_names = (char **)chk_calloc (1, buf_size);
    p = (ST_CHAR *) (_mis_transfer_set_names + _mis_max_dsts);
    for (i = 0; i < _mis_max_dsts; ++i)
      {
      sprintf (p, "DSTS%d", i);
      _mis_transfer_set_names[i] = p;
      p += (strlen (p) + 1);
      }
    }

  for (i = 0; i < num_dsts_ctrl; ++i)
    mis_vcc->mis_dsts_tbl[i].ts_name = _mis_transfer_set_names[i]; 

/* Add the built in ICCP variables to the VCC				*/
  for (i = 0; i < _mi_num_special_vars; ++i)
    {
    mis_add_dv_to_vcc (mis_vcc, _mis_special_vars[i].scope, 
				_mis_special_vars[i].name,
    		         	MI_TYPEID_DV_SPECIAL, SD_FALSE);
    }


/* Initialize Special ICCP Variables values */
  strcpy (mis_vcc->misv.Bilateral_Table_Id, bi_lat_tbl_id);
  mis_vcc->misv.TASE2_Version.MajorVersionNumber = 1996;
  mis_vcc->misv.TASE2_Version.MinorVersionNumber = 8;
  mis_vcc->misv.Supported_Features[0] = MI_BLOCK1 | MI_BLOCK2;
  mis_vcc->misv.Supported_Features[1] = 0;

  S_MI_UNLOCK_COMMON ();
  return (mis_vcc);
  }

/************************************************************************/
/*			mis_free_vcc 					*/
/************************************************************************/

ST_VOID mis_free_vcc (MIS_VCC *mis_vcc)
  {
MIS_DSTS *mis_dsts;
int i;

  S_MI_LOCK_COMMON ();

  for (i = 0; i < mis_vcc->max_icc_ds; ++i)
    {
    if (mis_vcc->icc_ds_ctrl_tbl[i] != NULL)
      {
      chk_free (mis_vcc->icc_ds_ctrl_tbl[i]);
      mis_vcc->icc_ds_ctrl_tbl[i] = NULL;
      }
    }

  if (mis_vcc->max_vcc_dv_ref > 0)
    chk_free (mis_vcc->vcc_dv_ref_tbl);

  if (mis_vcc->max_icc_dv_ref > 0)
    chk_free (mis_vcc->icc_dv_ref_tbl);

  if (mis_vcc->max_vcc_device_ref > 0)
    chk_free (mis_vcc->vcc_device_ref_tbl);

  if (mis_vcc->max_icc_device_ref > 0)
    chk_free (mis_vcc->icc_device_ref_tbl);

  if (mis_vcc->max_vcc_ds > 0)
    chk_free (mis_vcc->vcc_ds_ctrl_tbl);

  if (mis_vcc->max_icc_ds > 0)
    chk_free (mis_vcc->icc_ds_ctrl_tbl);

  if (mis_vcc->num_dsts_ctrl > 0)
    {
    for (i = 0; i < mis_vcc->num_dsts_ctrl; ++i)
      {
      mis_dsts = &mis_vcc->mis_dsts_tbl[i];
      if (mis_dsts->changed_dv != NULL)
        chk_free (mis_dsts->changed_dv);
      }
    chk_free (mis_vcc->mis_dsts_tbl);
    }
  chk_free (mis_vcc);
  S_MI_UNLOCK_COMMON ();
  }


/************************************************************************/
/*			mis_add_dv_to_vcc 				*/
/************************************************************************/

ST_RET mis_add_dv_to_vcc (MIS_VCC *mis_vcc, ST_INT dv_scope, 
			   ST_CHAR *dv_name, ST_INT mi_type, 
			   ST_BOOLEAN wr_enable)
  {
MIS_DV_REF dv_ref;

  return (mis_add_dv_to_vcc_ex (mis_vcc, dv_scope, 
			   dv_name, mi_type, wr_enable, &dv_ref));
  }

ST_RET mis_add_dv_to_vcc_ex (MIS_VCC *mis_vcc, ST_INT dv_scope, 
			   ST_CHAR *dv_name, ST_INT mi_type, 
			   ST_BOOLEAN wr_enable, MIS_DV_REF *dv_ref_out)
  {
MIS_DV_REF dv_index;
MIS_DV_REF dv_ref;
MIS_DV_REF mi_type_mask;
ST_INT rc;

  S_MI_LOCK_COMMON ();

/* Get the Data Value index */
  rc = mis_find_dv (dv_name, &dv_index);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("Could not find DV %s", dv_name); 
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }

/* Create the reference - start with the index */
  dv_ref = dv_index;

/* Set the scope and write enable bits */
  if (wr_enable == SD_TRUE)
    dv_ref |= MIS_DVREF_WRITE_EN;
  if (dv_scope == ICC_SPEC)
    dv_ref |= MIS_DVREF_ICC;

/* Now set the type */
  mi_type_mask = (MIS_DV_REF) mi_type;
  mi_type_mask <<= 26;
  mi_type_mask &= MIS_DVREF_TYPE_MASK_B;
  dv_ref |= mi_type_mask;

/* OK, go ahead and store the Data Value Reference */
  if (dv_scope == ICC_SPEC)
    {
    mis_vcc->icc_dv_ref_tbl[mis_vcc->num_icc_dv_ref] = dv_ref;
    ++mis_vcc->num_icc_dv_ref;
    }
  else
    {
    mis_vcc->vcc_dv_ref_tbl[mis_vcc->num_vcc_dv_ref] = dv_ref;
    ++mis_vcc->num_vcc_dv_ref;
    }
  *dv_ref_out = dv_ref;

  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			mis_sort_vcc_dv 				*/
/************************************************************************/

ST_VOID mis_sort_vcc_dv (MIS_VCC *mis_vcc)
  {
MIS_DV_REF *mis_dv_ref_tbl;
ST_INT num_dv_ref;

  S_MI_LOCK_COMMON ();

  num_dv_ref = mis_vcc->num_icc_dv_ref;
  mis_dv_ref_tbl = mis_vcc->icc_dv_ref_tbl;
  qsort (mis_dv_ref_tbl, num_dv_ref, sizeof (MIS_DV_REF), (int(*)(const void *, const void *))_mi_dv_ref_compare);

  num_dv_ref = mis_vcc->num_vcc_dv_ref;
  mis_dv_ref_tbl = mis_vcc->vcc_dv_ref_tbl;
  qsort (mis_dv_ref_tbl, num_dv_ref, sizeof (MIS_DV_REF), (int(*)(const void *, const void *))_mi_dv_ref_compare);

  S_MI_UNLOCK_COMMON ();
  }


/************************************************************************/
/*			_mi_dv_ref_compare 				*/
/************************************************************************/
int _mi_dv_ref_compare (const MIS_DV_REF *dv_ref1, const MIS_DV_REF *dv_ref2)
  {
ST_CHAR *dv_name1;
ST_CHAR *dv_name2;
MIS_DV_REF dv_index;

  dv_index = (*dv_ref1) & MIS_INDEX_MASK;
  dv_name1 = mis_dv_ctrl[dv_index].dv_name;
  dv_index = (*dv_ref2) & MIS_INDEX_MASK;
  dv_name2 = mis_dv_ctrl[dv_index].dv_name;
  return (strcmp (dv_name1, dv_name2));
  }
/************************************************************************/
/*			mis_log_vcc_dv_tables 				*/
/************************************************************************/

ST_VOID mis_log_vcc_dv_tables (MIS_VCC *mis_vcc)
  {
MIS_DV_REF dv_ref;
MIS_DV *mis_dv;
ST_INT i;

  SLOGCALWAYS0 ("MIS Data Values");
  SLOGCALWAYS1 ("  VCC Scope Data Values (%d Data Values)", mis_vcc->num_vcc_dv_ref)
  for (i = 0; i < mis_vcc->num_vcc_dv_ref; ++i)
    {
    dv_ref = mis_vcc->vcc_dv_ref_tbl[i];
    mis_dv = MIS_DV_REF_TO_DV (dv_ref);
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mis_dv->dv_name);
    }
  SLOGCALWAYS1 ("  ICC Scope Data Values (%d Data Values)", mis_vcc->num_icc_dv_ref)
  for (i = 0; i < mis_vcc->num_icc_dv_ref; ++i)
    {
    dv_ref = mis_vcc->icc_dv_ref_tbl[i];
    mis_dv = MIS_DV_REF_TO_DV (dv_ref);
    SLOGCALWAYS2 ("    %4d) %-32.32s", i+1, mis_dv->dv_name);
    }
  }

/************************************************************************/
/*                          _mis_ds_compare                             */
/* This function returns 0 when new->name matches table->name or	*/
/* when new->name can be inserted into list before table->name.		*/
/* This will cause bsearch to return "table".The user must check that it*/
/* is NOT a match before inserting "new" into array. If no insert point	*/
/* nor match is found, bsearch will return NULL. In this case the user	*/
/* can add "new" to the end of the array.				*/
/* WARNING:  The global variable "cur_element_array" must be set	*/
/*           before calling this function so start of array is known.	*/
/************************************************************************/

static MIS_DS **cur_element_array;

static ST_INT _mi_ds_compare (const void *op1, const void *op2)
  {
ST_INT ret;
MIS_DS *newEl  = (MIS_DS *) op1;
MIS_DS **tableEl = (MIS_DS **) op2;

  ret = strcmp (newEl->ds_name, (*tableEl)->ds_name);

  if (ret < 0)	/* If new element is less than the table element 	*/
    {
    if (tableEl == cur_element_array)
      {
      return (0); 		/* Insert at beginning of array.	*/
      }
    else	/* See if the table element above is less than the new 	*/
      {		/* If so, we need to insert before table element	*/
      if (strcmp (newEl->ds_name, (*(tableEl-1))->ds_name) > 0)
        return (0);		
      }
    }

  return (ret);
  }

/************************************************************************/

static ST_RET _insertDSElement (MIS_DS **ds_ctrl_table, 
	       	                ST_INT *num_ds, 
	                        MIS_DS *mis_ds)
  {
MIS_DS **ptr;
MIS_DS **key;

  if (*num_ds == 0)
    {
    ds_ctrl_table [*num_ds] = mis_ds; 
    }
  else	/* We need to insert this element in the correct position	*/
    {	
    cur_element_array = ds_ctrl_table;		/* CRITICAL	*/

  /* Find the element that is 'larger than' or 'equal to' this one	*/
    key = (struct _mis_ds **)bsearch (mis_ds, ds_ctrl_table, *num_ds,
                  sizeof (MIS_DS *), (int(*)(const void *, const void *))_mi_ds_compare);

    if (key == NULL)	/* This one is 'largest', add to end of array.	*/
      {
      ds_ctrl_table [*num_ds] = mis_ds;
      }
    else	/* We have a pointer to the first larger or equal	*/
      {		/* Verify that it is not equal 				*/
      if (strcmp ((*key)->ds_name, mis_ds->ds_name) == 0)
        return (SD_FAILURE);	/* Name already used. Cannot insert.	*/
      else
        {  /* Make room for this entry by moving all pointers down.	*/
           /* NOTE: memmove not used because too slow.	*/
	   
        ptr = &ds_ctrl_table[*num_ds]; /* Start at the bottom	*/
        do 
          { 
          *ptr = *(ptr-1); 
          } while (--ptr > key);

        *key = mis_ds;			/* Insert this entry.	*/
        }
      }
    }
  ++(*num_ds);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			mis_add_ds_to_vcc 					*/
/************************************************************************/

MIS_DS *mis_add_ds_to_vcc (MIS_VCC *mis_vcc, ST_INT ds_scope, ST_CHAR *ds_name, 
			ST_INT num_dv)
  {
MIS_DS **mis_ds_pp;
MIS_DS *mis_ds;
ST_INT *num_ds;
ST_CHAR *name_dest;
ST_RET	ret;

  S_MI_LOCK_COMMON ();

  if (ds_scope == ICC_SPEC)
    {
    if (mis_vcc->num_icc_ds >= mis_vcc->max_icc_ds)
      {
      MI_LOG_NERR0 ("No available ICC_SPEC DS control"); 
      S_MI_UNLOCK_COMMON ();
      return (NULL);
      }
    else
      {
      mis_ds_pp = mis_vcc->icc_ds_ctrl_tbl;
      num_ds = &mis_vcc->num_icc_ds;
      }
    }

  if (ds_scope == VCC_SPEC) 
    {
    if (mis_vcc->num_vcc_ds >= mis_vcc->max_vcc_ds)
      {
      MI_LOG_NERR0 ("No available VCC_SPEC DS control"); 
      S_MI_UNLOCK_COMMON ();
      return (NULL);
      }
    else
      {
      mis_ds_pp = mis_vcc->vcc_ds_ctrl_tbl;
      num_ds = &mis_vcc->num_vcc_ds;
      }
    }

  /* allocate a MIS_DS then see if it will fit into the alphabetized table		*/
  
  mis_ds = (struct _mis_ds *)chk_calloc(1, sizeof(MIS_DS) +
		              (num_dv * sizeof (MIS_DV_REF)) + (strlen(ds_name) + 1));
  mis_ds->mis_dv_ref_tbl = (MIS_DV_REF *)(mis_ds + 1);
  name_dest = (ST_CHAR *)(mis_ds->mis_dv_ref_tbl + num_dv);
  strcpy(name_dest, ds_name);
  mis_ds->ds_name = name_dest;
  mis_ds->num_dv_ref = num_dv;
  
  ret = _insertDSElement(mis_ds_pp, num_ds, mis_ds);
  if (ret!=SD_SUCCESS)
    {
    chk_free(mis_ds);
    mis_ds = NULL;
    }

  S_MI_UNLOCK_COMMON ();
  return (mis_ds);
  }

/************************************************************************/
/*			mis_delete_vcc_ds				*/
/************************************************************************/

ST_VOID mis_delete_vcc_ds (MIS_VCC *mis_vcc, MIS_DS *mis_ds, ST_INT ds_scope)
  {
MIS_DS **mis_ds_pp;
ST_INT *num_ds;
ST_INT i;

  S_MI_LOCK_COMMON ();

  if (ds_scope == ICC_SPEC)
    {
    mis_ds_pp = mis_vcc->icc_ds_ctrl_tbl;
    num_ds = &mis_vcc->num_icc_ds;
    }
  else
    {
    mis_ds_pp = mis_vcc->vcc_ds_ctrl_tbl;
    num_ds = &mis_vcc->num_vcc_ds;
    }

  /* find the MIS_DS */
  i = 0;
  while (i<*num_ds)
    {
    if (mis_ds_pp[i] == mis_ds)
      {
      chk_free (mis_ds);
      break;
      }
    else
      {
      i++;
      }
    }
  
  if (i==*num_ds)
    {
    MI_LOG_NERR0 ("MIS_DS not found"); 
    }
  else
    { /* don't leave a gap, move all MIS_DS from current position to    */
      /* the end up by one                                              */
    while(i<*num_ds)
      {
      if (i<(*num_ds-1))
        {
        mis_ds_pp[i] = mis_ds_pp[i+1];
        }
      else
        {
        mis_ds_pp[i] = NULL;
        }
      i++;
      }
    (*num_ds)--;
    }
  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/*			mis_add_ds_dv 					*/
/************************************************************************/

ST_VOID mis_add_ds_dv (MIS_VCC *mis_vcc, MIS_DS *mis_ds, 
		       MIS_DV_REF mis_dv_ref)
  {
ST_INT i;
MIS_DV_REF *ds_dv_ref;	

  S_MI_LOCK_COMMON ();

  ds_dv_ref = mis_ds->mis_dv_ref_tbl;
  for (i = 0; i < mis_ds->num_dv_ref; ++i)
    {
    if (ds_dv_ref[i] == 0)
      break;
    }
  if (i >= mis_ds->num_dv_ref)
    {
    MI_LOG_NERR0 ("Too many Data Values for Data Set"); 
    S_MI_UNLOCK_COMMON ();
    return;
    }
  ds_dv_ref[i] = mis_dv_ref;
  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/************************************************************************/
/*  		_mis_get_special_var_type_id 				*/
/************************************************************************/

ST_INT _mis_get_special_var_type_id (ST_CHAR *name)
  {
ST_INT i;

  for (i = 0; i < _mi_num_special_vars; ++i)
    {
    if (strcmp (name, _mis_special_vars[i].name) == 0)
      return (_mis_special_vars[i].mi_type);
    }
  MI_LOG_NERR0 ("Whoops! Can't get special variable mi_type");
  return (0);
  }

/************************************************************************/
/************************************************************************/
/*			mis_objname_to_dv 				*/
/************************************************************************/

ST_RET mis_objname_to_dv (MIS_VCC *mis_vcc, 
			  OBJECT_NAME *obj, 
			  MIS_DV **dv_out, 
			  MIS_DV_REF *dv_ref_out,
			  ST_INT *mi_type_out)
  {
ST_CHAR *dv_name;
ST_INT scope;
ST_RET rc;

  S_MI_LOCK_COMMON ();

  dv_name = obj->obj_name.vmd_spec;
  if (obj->object_tag == ICC_SPEC)
    {
    if (strcmp (obj->domain_id, mis_vcc->icc_name) != 0)
      {
      MI_LOG_NERR0 ("MIS Domain name mismatch");
      S_MI_UNLOCK_COMMON ();
      return (SD_FAILURE);
      }
    scope = ICC_SPEC;
    }
  else
    scope = VCC_SPEC;

  rc = mis_find_vcc_dv (mis_vcc, dv_name, scope,
			dv_out, dv_ref_out, mi_type_out);
  S_MI_UNLOCK_COMMON ();
  return (rc);
  }


/************************************************************************/
/*			mis_find_vcc_dv 				*/
/************************************************************************/

ST_RET mis_find_vcc_dv (MIS_VCC *mis_vcc, ST_CHAR *dv_name, ST_INT scope,
			MIS_DV **dv_out, MIS_DV_REF *dv_ref_out,
			ST_INT *mi_type_out)
  {
MIS_DV_REF dv_ref;
ST_INT num_dv_ref;
MIS_DV_REF *mis_dv_ref_tbl;
MIS_DV dv;
MIS_DV_REF *dv_ref_found;

  S_MI_LOCK_COMMON ();

  if (scope == ICC_SPEC)
    {
    num_dv_ref = mis_vcc->num_icc_dv_ref;
    mis_dv_ref_tbl = mis_vcc->icc_dv_ref_tbl;
    }
  else
    {
    num_dv_ref = mis_vcc->num_vcc_dv_ref;
    mis_dv_ref_tbl = mis_vcc->vcc_dv_ref_tbl;
    }

  dv.dv_name = dv_name;
  dv_ref_found = (unsigned long *)bsearch (&dv, mis_dv_ref_tbl, num_dv_ref, 
		    sizeof (MIS_DV_REF), (int(*)(const void *, const void *))_mi_dv_ref_compare2);
  if (dv_ref_found == NULL)
    {
    S_MI_UNLOCK_COMMON ();
    return (SD_FAILURE);
    }
  dv_ref = *dv_ref_found;
  *dv_ref_out = dv_ref;
  *dv_out = MIS_DV_REF_TO_DV (dv_ref);
  *mi_type_out = (dv_ref >> MIS_DVREF_TYPE_SHIFT) & MIS_DVREF_TYPE_MASK_A;
  S_MI_UNLOCK_COMMON ();
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			_mi_dv_ref_compare2 				*/
/************************************************************************/
int _mi_dv_ref_compare2 (const MIS_DV *dv, const MIS_DV_REF *dv_ref2)
  {
ST_CHAR *dv_name2;
MIS_DV_INDEX dv_index;

  dv_index = (*dv_ref2) & MIS_INDEX_MASK;
  dv_name2 = mis_dv_ctrl[dv_index].dv_name;
  return (strcmp (dv->dv_name, dv_name2));
  }
/************************************************************************/
/************************************************************************/
/*			mis_objname_to_ds 				*/
/************************************************************************/

ST_RET mis_objname_to_ds (MIS_VCC *mis_vcc, 
			  OBJECT_NAME *obj, 
			  MIS_DS **ds_out) 
  {
ST_CHAR *ds_name;
ST_INT max_num_ds;
MIS_DS *mis_ds;
MIS_DS **mis_ds_ctrl_tbl;
ST_INT i;

  S_MI_LOCK_COMMON ();

  ds_name = obj->obj_name.vmd_spec;
  if (obj->object_tag == ICC_SPEC)
    {
    if (strcmp (obj->domain_id, mis_vcc->icc_name) != 0)
      {
      MI_LOG_NERR0 ("MIS Domain name mismatch");
      S_MI_UNLOCK_COMMON ();
      return (SD_FAILURE);
      }
    max_num_ds = mis_vcc->max_icc_ds;
    mis_ds_ctrl_tbl = mis_vcc->icc_ds_ctrl_tbl;
    }
  else
    {
    max_num_ds = mis_vcc->max_vcc_ds;
    mis_ds_ctrl_tbl = mis_vcc->vcc_ds_ctrl_tbl;
    }

/* OK, now let's find the data set */
  for (i = 0; i < max_num_ds; ++i)
    {
    mis_ds = mis_ds_ctrl_tbl[i];
    if (mis_ds && (strcmp (mis_ds->ds_name, ds_name) == 0))
      {
      *ds_out = mis_ds;
      S_MI_UNLOCK_COMMON ();
      return (SD_SUCCESS);
      }
    }
  S_MI_UNLOCK_COMMON ();
  return (SD_FAILURE);
  }

/************************************************************************/
/************************************************************************/
/*			mis_dv_change 					*/
/************************************************************************/

ST_VOID mis_dv_change (MIS_DV_REF dv_ref)
  {
MI_REMOTE *mi_remote;
MIS_VCC *mis_vcc;
MIS_DS *mis_ds;
MIS_DSTS *mis_dsts;
MIS_DV_REF *dv_ref_ptr;
MIS_DV_INDEX changed_dv_index;
#ifdef DEBUG_SISCO
MIS_DV *mis_dv;
#endif
ST_INT i;
ST_INT j;
ST_DOUBLE curr_ms_time;
time_t curr_time;

/* Scan all VCC's and look for DSTS that reference this Data Value,	*/
/* and set the 'dirty' bit.						*/

  S_MI_LOCK_COMMON ();

/* Get the index of the local DV */
  changed_dv_index = dv_ref & MIS_INDEX_MASK;

#ifdef DEBUG_SISCO
  mis_dv = MIS_DV_REF_TO_DV (dv_ref);
  MI_LOG_FLOW1 ("Local DV '%s' changed", mis_dv->dv_name);
#endif

  curr_ms_time = sGetMsTime ();
  curr_time = time (NULL);
  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    mis_vcc = mi_remote->mis_vcc;
    if (mis_vcc != NULL && mi_remote->mi_conn_list != NULL)
      {
    /* OK, we are a server for this remote, and there is one or more	*/
    /* connections to it. See if anyone is monitoring this variable	*/
      mis_dsts = mis_vcc->mis_dsts_tbl;
      for (i = 0; i < mis_vcc->num_dsts_ctrl; ++i, ++mis_dsts)
        {
        if ((mis_dsts->dsts_flags & MIS_DSTS_FLAGS_IN_USE) &&
            (mis_dsts->mi_dsts_data.Status != 0))
          {
          mis_ds = mis_dsts->mis_ds;
          dv_ref_ptr = mis_ds->mis_dv_ref_tbl;
          for (j = 0; j < mis_ds->num_dv_ref; ++j, ++dv_ref_ptr)
            {
            if (((*dv_ref_ptr) & MIS_INDEX_MASK) == changed_dv_index)
              {
              MI_LOG_CFLOW1 ("  In DSTS for DS '%s'", mis_ds->ds_name);
              _mi_set_bit (mis_dsts->changed_dv, j);
              mis_dsts->dsts_flags |= MIS_DSTS_FIRST_CHANGE;
	      mi_wait_service_ms = 0;
	      /* need to set first changed timestamp  */
	      if (mis_dsts->first_change_reported == 0)
		{
		mis_dsts->first_change_reported = curr_ms_time;
		mis_dsts->next_buffer_rpt = curr_time + mis_dsts->mi_dsts_data.BufferTime;
		}
              }
            }
          }
        }
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/************************************************************************/
/*			mis_dsts_service 				*/
/************************************************************************/

ST_INT _mis_dsts_service (ST_DOUBLE *next_service)
  {
MI_REMOTE *remote;
MI_DSTS_DATA *mi_dsts_data;
MIS_VCC *mis_vcc;
MIS_DSTS *mis_dsts;
ST_INT i;
ST_INT rtn_val = 0;
ST_INT num_ms = 0;

  _mi_curr_ms_time = sGetMsTime ();
  _mi_curr_sec_time = time (NULL);
  remote = mi_remote_list;
  while (remote)
    {
    mis_vcc = remote->mis_vcc;
    if (mis_vcc != NULL)
      {
      mis_dsts = mis_vcc->mis_dsts_tbl;
      for (i = 0; i < mis_vcc->num_dsts_ctrl; ++i, ++mis_dsts)
       {
       mi_dsts_data = &mis_dsts->mi_dsts_data;
       if ((mis_dsts->dsts_flags & MIS_DSTS_FLAGS_IN_USE) &&
           (mi_dsts_data->Status != 0))
         {
         if (mi_dsts_data->StartTime <= _mi_curr_sec_time)
           {
           if (mis_dsts->last_complete_report_sent == 0)
             mis_dsts->last_complete_report_sent = _mi_curr_ms_time;	  

           num_ms = _mis_dsts_process (mis_vcc, mis_dsts);
	   if ((rtn_val == 0) ||
	       ((num_ms > 0)  && (num_ms < rtn_val)))
	     rtn_val = num_ms;
           }
         }
       }
      }
    remote = (MI_REMOTE *) list_get_next (mi_remote_list, remote);
    }

/*  rtn_val = min (1000, rtn_val); */	  /* max of 1 second  */
  *next_service = _mi_curr_ms_time + rtn_val;

  return rtn_val;
  }

/************************************************************************/

#define _DSTS_ACTION_NONE	0
#define _DSTS_ACTION_CHANGE	1
#define _DSTS_ACTION_FULL	2

#define _DSTS_PROCESS_BACKOFF_TIME	20

ST_INT _mis_dsts_process (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts)
  {
static ST_BOOLEAN firstAcseSendBufferProblem = SD_TRUE;
MI_CONN *mi_conn;
MI_DSTS_DATA *mi_dsts_data;
ST_INT action;
ST_INT next_service_ms;
ST_INT num_ms;
ST_INT num_change_bytes;
ST_INT bufAvail;

/* Verify that we can send this DSTS if we want to ... 			*/
/* Note that we need to consider the 'underway' reports as well as 	*/
/* confirmed requests we may want to issue				*/

#if 1
  mi_conn = mis_dsts->mi_conn;
  bufAvail = mvl_ureq_bufs_avail (mi_conn->net_info);
  bufAvail -= mi_conn->num_dsts_rpt_pend;
  if (bufAvail <= 0)
    {
    if (firstAcseSendBufferProblem == SD_TRUE)
      {
      MI_LOG_NERR0 ("Delay processing DSTS report, ACSE resources");
      firstAcseSendBufferProblem = SD_FALSE;
      }
    return (_DSTS_PROCESS_BACKOFF_TIME);
    }
#endif

  action = _DSTS_ACTION_NONE;
  mi_dsts_data = &mis_dsts->mi_dsts_data;
  next_service_ms = 0;

/* Check Integrity */
  if ((mi_dsts_data->DSConditionsRequested & MI_DSC_INTEGRITY) != 0)
    {
    /* Integrity is enabled, see if time to send it */
    if (_mi_curr_sec_time >= mis_dsts->next_integrity_rpt)	  
      {
      if (_mis_dsts_saturated (mis_dsts))
        return (_DSTS_PROCESS_BACKOFF_TIME);

      MI_LOG_FLOW1 ("Sending DSTS integrity report (DS %s)", mis_dsts->mis_ds->ds_name);
      mis_dsts->ds_conditions = MI_DSC_INTEGRITY;
      action = _DSTS_ACTION_FULL;
      mis_dsts->next_integrity_rpt = _mi_curr_sec_time + mi_dsts_data->IntegrityCheck;
      }
    else
      {
    /* Calculate time until next required service */
      if (mis_dsts->last_complete_report_sent != 0)
	{
	num_ms = (ST_INT) (mis_dsts->next_integrity_rpt - _mi_curr_ms_time);
	if (num_ms < next_service_ms)
	  next_service_ms = num_ms;
	}
      }
    }

/* If not sending Integrity, check Interval */
  if (action == _DSTS_ACTION_NONE && 
      (mi_dsts_data->DSConditionsRequested & MI_DSC_INTERVAL) != 0)
    {
  /* Interval is enabled, see if time to send it */
    if (_mi_curr_sec_time >= mis_dsts->next_interval_rpt)	  
      {
      if (mi_dsts_data->RBE == 0)
        {
        if (_mis_dsts_saturated (mis_dsts))
          return (_DSTS_PROCESS_BACKOFF_TIME);

        MI_LOG_FLOW1 ("Sending DSTS full interval report (DS %s)", mis_dsts->mis_ds->ds_name);
        mis_dsts->ds_conditions = MI_DSC_INTERVAL;
        action = _DSTS_ACTION_FULL;
	}
      else 
        { /* RBE, see if we have anything to send */
        if ((mis_dsts->dsts_flags & MIS_DSTS_FIRST_CHANGE) != 0)
	  {
          if (_mis_dsts_saturated (mis_dsts))
            return (_DSTS_PROCESS_BACKOFF_TIME);

          MI_LOG_FLOW1 ("Sending DSTS RBE interval report (DS %s)", mis_dsts->mis_ds->ds_name);
          mis_dsts->ds_conditions = MI_DSC_INTERVAL;
          action = _DSTS_ACTION_CHANGE;
	  }
	}
      mis_dsts->next_interval_rpt = _mi_curr_sec_time + mi_dsts_data->Interval;
      }
    else
      {
    /* Calculate time until next required service */
      if (mis_dsts->last_report_sent != 0)
	{
	num_ms = (ST_INT) (mis_dsts->next_interval_rpt - _mi_curr_ms_time);
	if (num_ms < next_service_ms)
	  next_service_ms = num_ms;
	}
      }
    }

/* If not sending integrity or interval, check Change */
  if (action == _DSTS_ACTION_NONE && 
      (mi_dsts_data->DSConditionsRequested & MI_DSC_CHANGE) != 0)
    {
  /* OK, Change is enabled, has anything changed? */
    if ((mis_dsts->dsts_flags & MIS_DSTS_FIRST_CHANGE) != 0)
      {
    /* Something changed, has our buffer time expired? */
      if (_mi_curr_sec_time >= mis_dsts->next_buffer_rpt)
        {
        mis_dsts->ds_conditions = MI_DSC_CHANGE;
        if (mi_dsts_data->RBE == 0)
	  {
          if (_mis_dsts_saturated (mis_dsts))
            return (_DSTS_PROCESS_BACKOFF_TIME);

          MI_LOG_FLOW1 ("Sending DSTS full change report (DS %s)", mis_dsts->mis_ds->ds_name);
  	  action = _DSTS_ACTION_FULL;
	  }
        else
	  {
          if (_mis_dsts_saturated (mis_dsts))
            return (_DSTS_PROCESS_BACKOFF_TIME);

          MI_LOG_FLOW1 ("Sending DSTS RBE change report (DS %s)", mis_dsts->mis_ds->ds_name);
          action = _DSTS_ACTION_CHANGE;
	  }
        }
       else
        {
      /* Calculate time until next required service */
        num_ms = (ST_INT) (mis_dsts->next_buffer_rpt - _mi_curr_ms_time);
        if (num_ms < next_service_ms)
          next_service_ms = num_ms;
        }
      }
    }

  if (action == _DSTS_ACTION_NONE)
    return next_service_ms;

  num_change_bytes = mis_dsts->mis_ds->num_dv_ref/8 + 1;
  if (action == _DSTS_ACTION_FULL)
    {
    memset (mis_dsts->changed_dv, 0xff, num_change_bytes);
    mis_dsts->last_complete_report_sent = _mi_curr_ms_time;
    }

  _mis_start_dsts_report (mis_vcc, mis_dsts);
  mis_dsts->last_report_sent = _mi_curr_ms_time;
  mis_dsts->dsts_flags &= ~MIS_DSTS_FIRST_CHANGE;
  memset (mis_dsts->changed_dv, 0, num_change_bytes);
  mis_dsts->first_change_reported = 0;
  return next_service_ms;
  }


/************************************************************************/
/*			_mis_dsts_saturated				*/
/************************************************************************/

static ST_RET _mis_dsts_saturated (MIS_DSTS *mis_dsts)
  {
  if (mis_dsts->dsts_flags & MIS_DSTS_REPORT_UNDERWAY)
    {
    if (mis_dsts->saturated == SD_FALSE)
      {
      MI_LOG_NERR0 ("Could not send DSTS report, saturated ...");
      mis_dsts->saturated = SD_TRUE;
      }
    return (SD_TRUE);
    }
  return (SD_FALSE);
  }

/************************************************************************/

ST_VOID _mis_start_dsts_report (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts)
  {
MIS_DS *mis_ds;
#ifdef DEBUG_SISCO
MIS_DV *mis_dv;
#endif
ST_INT num_var;
ST_INT i;
MIS_INFO_RPT_CTRL *mis_info_ctrl;
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
ST_UINT8 bit_mask;
ST_UINT8 *byte_ptr;
ST_INT	num_included;
ST_INT	num_data_done;


  mis_ds = mis_dsts->mis_ds;
  num_var = mis_ds->num_dv_ref;

  mis_info_ctrl =_mis_get_info_rpt_ctrl (mis_vcc, mis_dsts);
  if (mis_info_ctrl == NULL)
    return;

  num_included = mis_info_ctrl->num_included;
  num_data_done = 0;

  if (mis_info_ctrl->num_included == mis_info_ctrl->num_var)
    mis_info_ctrl->report_type = MIS_REPORT_TYPE_NVL;
  else
    mis_info_ctrl->report_type = MIS_REPORT_TYPE_LOV;
    
  mis_dsts->ts_timestamp = _mi_curr_sec_time;
  mis_dsts->dsts_flags |= MIS_DSTS_REPORT_UNDERWAY;
  ++mis_dsts->mi_conn->num_dsts_rpt_pend;

  u_mis_dsts_activity (mis_vcc, mis_dsts, MIS_DSTS_BEGIN_REPORT, 
		       mis_dsts->ts_timestamp);

/* Now call the user data value access functions */
  bit_mask = 0x80;
  byte_ptr = mis_dsts->changed_dv;
  mis_rd_dv_ctrl = mis_info_ctrl->mis_rd_dv_ctrl_tbl;
  for (i = 0; (i < num_var) && (num_data_done<num_included); ++i, ++mis_rd_dv_ctrl)
    {
    if ((*byte_ptr & bit_mask) || 
        (mis_rd_dv_ctrl->mi_type > MI_LAST_DV_TYPEID))
      {
      num_data_done++;
#ifdef DEBUG_SISCO
      mis_dv = MIS_DV_REF_TO_DV (mis_rd_dv_ctrl->mis_dv_ref);
      MI_LOG_CFLOW1 ("  Including variable: %s", mis_dv->dv_name);
#endif
      if (mis_rd_dv_ctrl->mi_type > MI_LAST_DV_TYPEID)
        _mis_handle_special_var_read (mis_rd_dv_ctrl);
      else
        u_mis_rd_dv (mis_rd_dv_ctrl);
      }

    if (bit_mask == 0x01)
      {
      bit_mask = 0x80;
      ++byte_ptr;
      }
    else
      bit_mask = bit_mask >> 1;
    }  
  }                                  

/************************************************************************/

MIS_INFO_RPT_CTRL *_mis_get_info_rpt_ctrl (MIS_VCC *mis_vcc, 
					   MIS_DSTS *mis_dsts)
  {
MIS_DS *mis_ds;
ST_INT num_var;
ST_INT i;
ST_INT num_included;
MIS_INFO_RPT_CTRL *mis_info_ctrl;
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
ST_UINT8 bit_mask;
ST_UINT8 *byte_ptr;

  mis_ds = mis_dsts->mis_ds;
  mis_info_ctrl = (struct _mis_info_rpt_ctrl *)chk_calloc (1, sizeof (MIS_INFO_RPT_CTRL));
  mis_info_ctrl->mis_vcc = mis_vcc;
  mis_info_ctrl->mis_dsts = mis_dsts;
  mis_info_ctrl->mis_rd_dv_ctrl_tbl = 
    _mis_ds_to_rd_dv_ctrl (mis_vcc, mis_ds, mis_info_ctrl, MIS_RD_REASON_REPORT);
  mis_info_ctrl->num_var = mis_ds->num_dv_ref;
  num_var = mis_ds->num_dv_ref;

/* First we need to count the data values to be sent */
  bit_mask = 0x80;
  byte_ptr = mis_dsts->changed_dv;
  num_included = 0;
  mis_rd_dv_ctrl = mis_info_ctrl->mis_rd_dv_ctrl_tbl;
  for (i = 0; i < num_var; ++i, ++mis_rd_dv_ctrl)
    {
    mis_rd_dv_ctrl->mis_dsts = mis_dsts;
    if ((*byte_ptr & bit_mask) || 
        (mis_rd_dv_ctrl->mi_type > MI_LAST_DV_TYPEID))
      {
      ++num_included;
      }
    if (bit_mask == 0x01)
      {
      bit_mask = 0x80;
      ++byte_ptr;
      }
    else
      bit_mask = bit_mask >> 1;
    }  

  if (num_included == 0)
    {
    chk_free (mis_info_ctrl->mis_rd_dv_ctrl_tbl);
    chk_free (mis_info_ctrl);
    return (NULL);
    }
  mis_info_ctrl->num_included = num_included;
  return (mis_info_ctrl);
  }                                  

/************************************************************************/

ST_VOID mis_info_dv_done (MIS_RD_DV_CTRL *mis_rd_dv_ctrl, ST_RET rslt)
  {
MIS_INFO_RPT_CTRL *mis_info_ctrl;

  mis_info_ctrl = mis_rd_dv_ctrl->mis_info_ctrl;

  if (rslt == SD_SUCCESS)
    mis_rd_dv_ctrl->acc_rslt_tag = ACC_RSLT_SUCCESS;

  ++mis_info_ctrl->num_data_done;
  if (mis_info_ctrl->num_data_done == mis_info_ctrl->num_included)
    {
  /* Time to send the report ... */
    _mis_send_dsts_rpt (mis_info_ctrl); 
    u_mis_dsts_activity (mis_info_ctrl->mis_vcc, 
			 mis_info_ctrl->mis_dsts, MIS_DSTS_END_REPORT, 
			 mis_info_ctrl->mis_dsts->ts_timestamp);
    mis_info_ctrl->mis_dsts->dsts_flags &= ~MIS_DSTS_REPORT_UNDERWAY;
    --mis_info_ctrl->mis_dsts->mi_conn->num_dsts_rpt_pend;
    chk_free (mis_info_ctrl->mis_rd_dv_ctrl_tbl);
    chk_free (mis_info_ctrl); 
    }
  }

/************************************************************************/

ST_RET _mis_send_dsts_rpt (MIS_INFO_RPT_CTRL *mis_info_ctrl)
  {
MI_REMOTE *mi_remote;
MIS_VCC *mis_vcc;
MIS_DSTS *mis_dsts;
MIS_DS *mis_ds;
MIS_DV *mis_dv;
ST_RET rc;
INFO_REQ_INFO *info_info;
ACCESS_RESULT *ar;
VARIABLE_LIST *vl;
ST_INT req_info_size;
ST_UCHAR *asn1_buf;
ST_UCHAR *asn1_start;
ST_INT asn1_len;
ST_INT asn1_buf_left;
ST_INT num_ds_var;
ST_INT num_var_to_send;
ST_INT i;
MIS_DV_REF dv_ref;
MIS_DV_REF dv_index;
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
MI_CONN *mi_conn;
ST_UINT8 bit_mask;
ST_UINT8 *byte_ptr;
ST_INT num_sent;
ST_INT est_asn1_size;
ST_INT max_pdu_size;

  mis_vcc = mis_info_ctrl->mis_vcc;
  mi_remote = mis_vcc->mi_remote;
  mis_dsts = mis_info_ctrl->mis_dsts;
  mis_ds = mis_dsts->mis_ds;
  num_var_to_send = mis_info_ctrl->num_included;
  num_ds_var = mis_ds->num_dv_ref;
  mi_conn = mis_dsts->mi_conn;

/* Calculate the required size for  the info report information		*/
  req_info_size = sizeof(INFO_REQ_INFO) + 
		    (num_var_to_send * sizeof(ACCESS_RESULT));
  if (mis_info_ctrl->report_type == MIS_REPORT_TYPE_LOV)
    req_info_size += (num_var_to_send * sizeof(VARIABLE_LIST));
    
  info_info = (INFO_REQ_INFO *) M_CALLOC (MSMEM_ENC_INFO, 1, req_info_size);
  info_info->num_of_acc_result = num_var_to_send;

/* If this is a LOV, verify that the ASN.1 will fit OK			*/
  if (mis_info_ctrl->report_type == MIS_REPORT_TYPE_LOV)
    {
    max_pdu_size = mi_conn->net_info->max_pdu_size;
    if (mis_dsts->max_asn1_size > max_pdu_size)
      {
      est_asn1_size = _mis_calc_info_rpt_size (mis_info_ctrl);
      MI_LOG_FLOW1 ("Estimated info report size: %d", est_asn1_size);
      if (est_asn1_size > max_pdu_size)
        {
        MI_LOG_FLOW0 ("Switching report to NVL");
        mis_info_ctrl->report_type = MIS_REPORT_TYPE_NVL;
        }
      }
    }

/* We need to create the variable specification - either a list of	*/
/* variables or a named variable list					*/
  if (mis_info_ctrl->report_type == MIS_REPORT_TYPE_NVL)
    {
    info_info->va_spec.var_acc_tag = VAR_ACC_NAMEDLIST;
    info_info->va_spec.vl_name.object_tag = ICC_SPEC;
    info_info->va_spec.vl_name.domain_id = mis_vcc->icc_name;
    info_info->va_spec.vl_name.obj_name.item_id = mis_ds->ds_name; 
    info_info->acc_rslt_list = (ACCESS_RESULT *) (info_info + 1);
    }
  else	/* List Of Variables */
    {
    info_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
    info_info->va_spec.num_of_variables = num_var_to_send;

    bit_mask = 0x80;
    byte_ptr = mis_dsts->changed_dv;
    vl = (VARIABLE_LIST *) (info_info + 1);
    num_var_to_send = mis_ds->num_dv_ref;
    mis_rd_dv_ctrl =  mis_info_ctrl->mis_rd_dv_ctrl_tbl;
    for (i = 0; i < num_ds_var; ++i, ++mis_rd_dv_ctrl)
      {
      if ((*byte_ptr & bit_mask) ||
          (mis_rd_dv_ctrl->mi_type > MI_LAST_DV_TYPEID))
        {
        dv_ref = mis_ds->mis_dv_ref_tbl[i];
        dv_index = dv_ref & MIS_INDEX_MASK;
        mis_dv = &mis_dv_ctrl[dv_index];

        vl->var_spec.var_spec_tag = VA_SPEC_NAMED;

     /* We know this is a union so the following line is OK ... */
        vl->var_spec.vs.name.obj_name.vmd_spec = mis_dv->dv_name;
        if (dv_ref & MIS_DVREF_ICC)
          {
          vl->var_spec.vs.name.object_tag = ICC_SPEC;
          vl->var_spec.vs.name.domain_id = mis_vcc->icc_name;
          }	 
        else
          vl->var_spec.vs.name.object_tag = VCC_SPEC;

        ++vl;
	}

      if (bit_mask == 0x01)
        {
        bit_mask = 0x80;
        ++byte_ptr;
        }
      else
        bit_mask = bit_mask >> 1;
      }	 
    info_info->acc_rslt_list = (ACCESS_RESULT *) (vl);
    }

  asn1_buf = (unsigned char *)M_MALLOC (MSMEM_ASN1_DATA_ENC, mvl_cfg_info->max_msg_size);
  asn1_buf_left = mvl_cfg_info->max_msg_size;
  asn1_start = asn1_buf;

/* Now encode the data, create the access result */
  bit_mask = 0x80;
  byte_ptr = mis_dsts->changed_dv;
  ar = info_info->acc_rslt_list;
  mis_rd_dv_ctrl =  mis_info_ctrl->mis_rd_dv_ctrl_tbl;
  num_sent = 0;
  for (i = 0; i < num_ds_var; ++i, ++mis_rd_dv_ctrl)
    {
    if ((*byte_ptr & bit_mask) ||
        (mis_rd_dv_ctrl->mi_type > MI_LAST_DV_TYPEID))
      {
      ++num_sent;
      dv_ref = mis_ds->mis_dv_ref_tbl[i];
      
      rc = mi_get_dv_asn1_data (mis_rd_dv_ctrl->data_buf, 
      				mis_rd_dv_ctrl->mi_type,
      				asn1_start, asn1_buf_left, &asn1_len);
      
      if (rc == SD_SUCCESS)
        {
        ar->acc_rslt_tag = ACC_RSLT_SUCCESS; 
        ar->va_data.len = asn1_len;
        ar->va_data.data = asn1_start;
        asn1_buf_left -= asn1_len;   /* adjust buffer control variables	*/
        asn1_start += asn1_len;
        }
      else
        {
        ar->acc_rslt_tag = ACC_RSLT_FAILURE;
        ar->failure = ARE_TYPE_INCONSISTENT;
        }
       ++ar; 
      }
    if (bit_mask == 0x01)
      {
      bit_mask = 0x80;
      ++byte_ptr;
      }
    else
      bit_mask = bit_mask >> 1;
    }

  /* Do a sanity check */
  if (num_sent != mis_info_ctrl->num_included)
    {
    MI_LOG_NERR0 ("Report : internal error, num_sent != num_included");
    }

  /* Build a info request               */
  rc = mpl_info (info_info);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("Info : mpl_info failed, ret = %x", rc);
    }
  else
    {  
    MI_LOG_FLOW1 ("Actual info report size: %d", mmsl_msg_len);
    rc = _mvl_send_msg (mmsl_msg_start, mmsl_msg_len, 
			  mi_conn->net_info);
    if (rc != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("MI DSTS Report: _mvl_send_msg failed, ret = %x", rc);
      }
    }
  M_FREE (MSMEM_ENC_INFO, info_info);
  M_FREE (MSMEM_ASN1_DATA_ENC, asn1_buf);
  return (rc);
  }                                  

/************************************************************************/
/*			_mis_process_info_ind 				*/
/************************************************************************/

#define _MIS_TS_ACK_VARID		1
#define _MIS_TS_TIMESTAMP_VARID		2
#define _MIS_TS_NACK_VARID		3

ST_VOID _mis_process_info_ind (MVL_COMM_EVENT *event)
  {
VAR_ACC_SPEC *va_spec;
INFO_REQ_INFO *info_info;
VARIABLE_LIST *vl;
ST_INT i;
ACCESS_RESULT *ar;
RUNTIME_TYPE *rt;
ST_INT rt_len;
MI_CONN *mi_conn;
MIS_VCC *mis_vcc;
ST_INT type_id;
ST_INT var_id;
ST_RET rc;
MIS_DSTS *mis_dsts;
MI_MMS_OBJECT_NAME ts_name;
MI_TS_TIMESTAMP    ts_timestamp;
ST_VOID *data_dest;
ST_BOOLEAN ack_rcvd;
ST_BOOLEAN timestamp_rcvd;
ST_BOOLEAN nack_rcvd;
ST_INT num_var;
ST_CHAR *var_name;

/* See if we are using ICCP on this connection */
  if (event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_info_ind (event);
    return;
    }

  mis_dsts = NULL;
  ack_rcvd = SD_FALSE;
  timestamp_rcvd = SD_FALSE;
  nack_rcvd = SD_FALSE;

  info_info = (INFO_REQ_INFO *) event->u.mms.dec_rslt.data_ptr;
  va_spec = &info_info->va_spec;

/* Get the MI connection and associated VCC */
  mi_conn = event->net_info->mi_conn;
  mis_vcc = mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("Warning: Received LOV info rpt ind on client only connection");
    _mvl_free_comm_event (event);	/* All done with this event	*/
    return;
    }

  vl = (VARIABLE_LIST *) (info_info + 1);
  ar = info_info->acc_rslt_list;
  num_var = info_info->va_spec.num_of_variables;
  for (i = 0; i < num_var; ++i, ++vl, ++ar)
    {
    if (vl->var_spec.var_spec_tag != VA_SPEC_NAMED)
      {
      MI_LOG_NERR1 ("Invalid var_spec_tag '%d'", vl->var_spec.var_spec_tag); 
      continue;
      }
    var_name = vl->var_spec.vs.name.obj_name.vmd_spec;
    /* 503 section 7.1.4.4.1 says the value of the Domain identifier 	*/
    /* portion of the object name for Transfer_Report_ACK shall be 	*/
    /* equal to the value of the Domain identifier of the Transfer Set 	*/
    /* Name contained in the Information Trport received by the TASE.2	*/
    /* client from the TASE.2 server					*/
    if (vl->var_spec.vs.name.object_tag == ICC_SPEC) 
      {
      if (strcmp (vl->var_spec.vs.name.domain_id, mis_vcc->icc_name) != 0)
        {
        MI_LOG_NERR1 ("Domain name '%s' mismatch ", vl->var_spec.vs.name.domain_id); 
        continue;
        }
      }
#if 0
    if (vl->var_spec.vs.name.object_tag == AA_SPEC) 
      {
      MI_LOG_NERR1 ("Invalid AA scope variable '%s' received", var_name); 
      continue;
      }
#endif
    if (ar->acc_rslt_tag != ACC_RSLT_SUCCESS)
      {
      MI_LOG_NERR0 ("Info Rpt var acc_rslt_tag != ACC_RSLT_SUCCESS"); 
      continue;
      }
 
  /* Check for critical acknowledge */
    if (vl->var_spec.vs.name.object_tag == ICC_SPEC) 
      {
      if (strcmp (var_name, "Transfer_Report_ACK") == 0)
        {
        type_id = _mi_get_type_id (MI_TYPEID_MMS_OBJECT_NAME);
        var_id = _MIS_TS_ACK_VARID;
        data_dest = &ts_name;
        }
      else if (strcmp (var_name, "Transfer_Set_Time_Stamp") == 0)
        {
        type_id = _mi_get_type_id (MI_TYPEID_GMT_BASED_S);
        var_id = _MIS_TS_TIMESTAMP_VARID;
        data_dest = &ts_timestamp;
        }
      else if (strcmp (var_name, "Transfer_Report_NACK") == 0)
        {
        type_id = _mi_get_type_id (MI_TYPEID_MMS_OBJECT_NAME);
        var_id = _MIS_TS_NACK_VARID;
        data_dest = &ts_name;
        }
      else
        {
        MI_LOG_NERR1 ("Invalid DOM scope variable '%s' received", var_name); 
	continue;
        }

    /* Convert the data ... */
      if (mvl_get_runtime (type_id, &rt, &rt_len) == SD_SUCCESS)
        {
        rc = ms_asn1_to_local (rt, rt_len, ar->va_data.data,
                               ar->va_data.len, (char *)data_dest);

        if (rc == SD_SUCCESS)
          {
          if (var_id == _MIS_TS_ACK_VARID)
            {
            mis_dsts = mis_find_dsts (mis_vcc, &ts_name);
            if (mis_dsts == NULL)
              {
              MI_LOG_NERR1 ("TS Ack - ts_name '%s' not found", ts_name.Name);
	      continue;
              }
            ack_rcvd = SD_TRUE;
            }
          else if (var_id == _MIS_TS_TIMESTAMP_VARID)
            {
            timestamp_rcvd = SD_TRUE;
            }
          else if (var_id == _MIS_TS_NACK_VARID)
            {
            nack_rcvd = SD_TRUE;
            }
          }                        
        else 
          {
          MI_LOG_NERR0 ("InfoRpt Ind : Data conversion error");
          }                        
        }
      }
    }

  if (mis_dsts != NULL)
    {
    if (ack_rcvd == SD_TRUE)
      {
      if (timestamp_rcvd == SD_TRUE)
        {
        _mis_ack_rcvd (mis_vcc, mis_dsts, &ts_name, ts_timestamp);
        }                        
      else
        {
        MI_LOG_NERR0 ("Warning: DSTS Ack received w/o timestamp");
        }                        
      }
    else if (nack_rcvd == SD_TRUE)
      _mis_nack_rcvd (mis_vcc, mis_dsts, &ts_name);
    }

  _mvl_free_comm_event (event);	/* All done with this event	*/
  }

/************************************************************************/

MIS_DSTS *mis_find_dsts (MIS_VCC *mis_vcc, MI_MMS_OBJECT_NAME *ts_name)
  {
MIS_DSTS *mis_dsts;
ST_INT i;
ST_CHAR *name;

  if (strcmp (ts_name->DomainName, mis_vcc->icc_name) != 0)
    {
    MI_LOG_NERR1 ("Domain name '%s' mismatch ", ts_name->DomainName); 
    return (NULL);
    }
  name = ts_name->Name; 
  mis_dsts = mis_vcc->mis_dsts_tbl;
  for (i = 0; i < mis_vcc->num_dsts_ctrl; ++i, ++mis_dsts)
    {
    if (strcmp (mis_dsts->ts_name, name) == 0)
      return (mis_dsts);
    }
  return (NULL);   
  }

/************************************************************************/
ST_VOID _mis_ack_rcvd (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts, 
		       MI_MMS_OBJECT_NAME *ts_name, 
		       MI_TS_TIMESTAMP ts_timestamp)
  {
  u_mis_dsts_activity (mis_vcc, mis_dsts, MIS_DSTS_ACK_RCVD, ts_timestamp);
  }


ST_VOID _mis_nack_rcvd (MIS_VCC *mis_vcc, MIS_DSTS *mis_dsts, 
			MI_MMS_OBJECT_NAME *ts_name)
  {
  u_mis_dsts_activity (mis_vcc, mis_dsts, MIS_DSTS_NACK_RCVD, 0);
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			_mis_process_read_ind				*/
/************************************************************************/

ST_VOID _mis_process_read_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
READ_REQ_INFO   *read_req_info;
VAR_ACC_SPEC	*va_spec;
MIS_READ_CTRL *mis_rd_ctrl;
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
MIS_DEVICE_IND_CTRL *mis_dev_ind_ctrl;
ST_INT num_var, num_dev;
ST_INT i;
ST_RET rc;


/* See if we are using ICCP on this connection */
  if (mvl_ind_ctrl->event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_read_ind (mvl_ind_ctrl);
    return;
    }

  read_req_info = (READ_REQ_INFO *) mvl_ind_ctrl->event->u.mms.dec_rslt.data_ptr;
  va_spec = &read_req_info->va_spec;

  if (read_req_info->va_spec.var_acc_tag == VAR_ACC_VARLIST)
    {
    if (read_req_info->va_spec.num_of_variables == 0)
      {
      mvlas_read_resp (mvl_ind_ctrl);
      return;
      }
    }

  mis_rd_ctrl = (MIS_READ_CTRL *) chk_calloc (1, sizeof (MIS_READ_CTRL));
  mis_rd_ctrl->read_req_info = read_req_info;
  mis_rd_ctrl->mvl_ind_ctrl = mvl_ind_ctrl;
  mis_rd_ctrl->mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_rd_ctrl->mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI read ind, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl,MMS_ECLASS_ACCESS, MMS_ECODE_OBJ_ACCESS_DENIED);
    chk_free (mis_rd_ctrl);
    return;
    }

  if (va_spec->var_acc_tag == VAR_ACC_NAMEDLIST)
    rc = _mi_prep_nvl_rd_ctrl (read_req_info, mis_rd_ctrl);
  else
    rc = _mi_prep_lov_rd_ctrl (read_req_info, mis_rd_ctrl);

  if (rc == SD_SUCCESS)
    {
    num_var = mis_rd_ctrl->num_data_vals;
    num_dev = mis_rd_ctrl->num_devices;
    mis_rd_dv_ctrl = mis_rd_ctrl->mis_rd_dv_ctrl_tbl;
    mis_dev_ind_ctrl = mis_rd_ctrl->mis_device_ctrl_tbl;

    /* pass out the data value requests				*/
    for (i = 0; i < num_var; ++i, ++mis_rd_dv_ctrl)
      {
      if (mis_rd_dv_ctrl->mi_type > MI_LAST_DV_TYPEID)
        _mis_handle_special_var_read (mis_rd_dv_ctrl);
      else if (mis_rd_dv_ctrl->failure == ARE_OBJ_NONEXISTENT)
        {
        mis_rd_dv_done (mis_rd_dv_ctrl, SD_FAILURE);
	continue;
        }
      else
        u_mis_rd_dv (mis_rd_dv_ctrl);
      }

    /* pass out the device requests				*/
    for (i = 0; i < num_dev; ++i, ++mis_dev_ind_ctrl)
      _mis_device_read_ind (mis_dev_ind_ctrl);

    }
  else  
    {
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_NONEXISTENT);
    }
  }

/************************************************************************/
/*			_mi_prep_lov_rd_ctrl				*/
/* It is possible though unlikely that the read request contains data	*/
/* value objects and device objects. This function will create object	*/
/* specific control structures suitable for passing to the user.	*/
/************************************************************************/

#define SBO_TEXT "_SBO"
#define SBO_TEXT_LEN 	4
#define TAG_TEXT "_TAG"
#define TAG_TEXT_LEN 	4

static ST_RET _mi_prep_lov_rd_ctrl (READ_REQ_INFO *read_req_info, 
				    MIS_READ_CTRL *mis_rd_ctrl)
  {
MIS_VCC *mis_vcc;
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
MIS_DV *mis_dv;
MIS_DEVICE_IND_CTRL *mis_dev_ind_ctrl;
VARIABLE_LIST *vl;
ST_INT num_var, num_dvs, num_devs;
ST_INT i, len, scope;   
ST_RET rc;
ST_CHAR obj_name[MAX_IDENT_LEN+1], *temp;
ST_BOOLEAN is_sbo, is_tag;

  /* This is a list of variables   					*/
  /* Simply ignore the 'spec_in_result' parameter for now		*/

  /* Allocate the read DV control data structures 			*/
  num_var = read_req_info->va_spec.num_of_variables;
  mis_rd_ctrl->num_var = num_var;
  mis_rd_ctrl->mis_rd_dv_ctrl_tbl = 
    (MIS_RD_DV_CTRL *) chk_calloc (num_var, sizeof(MIS_RD_DV_CTRL));
  num_dvs = 0;

  /* Allocate the device ind ctrl table					*/
  mis_rd_ctrl->mis_device_ctrl_tbl = 
    (MIS_DEVICE_IND_CTRL *) chk_calloc (num_var, sizeof (MIS_DEVICE_IND_CTRL));
  num_devs = 0;

  /* Prepare to process each variable in the list			*/
  mis_vcc = mis_rd_ctrl->mis_vcc;
  mis_rd_dv_ctrl = mis_rd_ctrl->mis_rd_dv_ctrl_tbl;
  mis_dev_ind_ctrl = mis_rd_ctrl->mis_device_ctrl_tbl;
  vl = (VARIABLE_LIST *) (read_req_info + 1);
  num_dvs = num_devs = 0;
  for (i = 0; i < num_var; ++i, ++vl)
    {
    /* determine what kind of object					*/
    scope = vl->var_spec.vs.name.object_tag;
    switch (scope)
      {
      case VCC_SPEC:
	strcpy (obj_name, vl->var_spec.vs.name.obj_name.vmd_spec);
      break;

      case ICC_SPEC:
	strcpy (obj_name, vl->var_spec.vs.name.obj_name.item_id);
      break;

      case AA_SPEC:
	strcpy (obj_name, vl->var_spec.vs.name.obj_name.aa_spec);
      break;
      }  

    /* if this is a device _SBO or _TAG will be at the end of the name  */
    len = strlen (obj_name);
    if (len >SBO_TEXT_LEN || len > TAG_TEXT_LEN)
      {
      temp = obj_name + len - SBO_TEXT_LEN;
      is_sbo = (strcmp (temp, SBO_TEXT) == 0) ? SD_TRUE : SD_FALSE;
      is_tag = (strcmp (temp, TAG_TEXT) == 0) ? SD_TRUE : SD_FALSE;
      if (is_sbo || is_tag)
	{
	temp[0] = 0;
	if (mis_find_vcc_device (mis_vcc, obj_name, scope, 
				 &mis_dev_ind_ctrl->mis_device,
				 &mis_dev_ind_ctrl->mis_device_ref,
				 &mis_dev_ind_ctrl->device_type) == SD_SUCCESS)
	  {
	  mis_dev_ind_ctrl->mis_vcc = mis_vcc;
	  mis_dev_ind_ctrl->op = (is_sbo) ? MI_DEVICE_OP_SELECT : 
	  				    MI_DEVICE_OP_GETTAG;  

          mis_dev_ind_ctrl->acc_rslt_tag = ACC_RSLT_FAILURE;
          mis_dev_ind_ctrl->failure = ARE_OBJ_ACC_UNSUPPORTED;

          mis_dev_ind_ctrl->data_buf = &mis_dev_ind_ctrl->device_data;
          mis_dev_ind_ctrl->mis_rd_ctrl = mis_rd_ctrl;
	  mis_dev_ind_ctrl->vl_index = i;
	  mis_dev_ind_ctrl++;
	  num_devs++;
	  continue;
	  }
	}
      }
    
    /* if it is not a device is must be a data value			*/
    /* take care of the basics	*/
    num_dvs++;
    mis_rd_dv_ctrl->vl_index = i;

    /* Assume the worst ... */
    mis_rd_dv_ctrl->acc_rslt_tag = ACC_RSLT_FAILURE;
    mis_rd_dv_ctrl->failure = ARE_OBJ_ACC_UNSUPPORTED;

    /* Initialize some of the mis_rd_dv_ctrl members */
    mis_rd_dv_ctrl->mis_rd_ctrl = mis_rd_ctrl;
    mis_rd_dv_ctrl->mis_vcc = mis_vcc;
    mis_rd_dv_ctrl->data_buf = &mis_rd_dv_ctrl->dv_data;
    mis_rd_dv_ctrl->rd_reason = MIS_RD_REASON_READ_IND;

    if (vl->alt_access_pres)  /* no alternate access support now	*/
      {
      MI_LOG_NERR0 ("Read Indication : Alternate Access not supported");
      mis_rd_dv_ctrl++;
      continue;
      }
    if (vl->var_spec.var_spec_tag != VA_SPEC_NAMED)
      {
      MI_LOG_NERR0 ("Read: Invalid VarSpecTag");
      mis_rd_dv_ctrl++;
      continue;
      }

    rc = mis_objname_to_dv (mis_vcc, 
			     &vl->var_spec.vs.name, 
			     &mis_dv,	
			     &mis_rd_dv_ctrl->mis_dv_ref,
			     &mis_rd_dv_ctrl->mi_type);

    if (rc != SD_SUCCESS)  /* could not find DV	*/
      {
      MI_LOG_NERR1 ("MI Read ind: variable '%s' not found", 
		          vl->var_spec.vs.name.obj_name.vmd_spec); 
      mis_rd_dv_ctrl->failure = ARE_OBJ_NONEXISTENT;
      }

    mis_rd_dv_ctrl++;
    }

  mis_rd_ctrl->num_data_vals = num_dvs;
  mis_rd_ctrl->num_devices = num_devs;

  /* only use the memory we need				*/
  if (num_dvs == 0)
    {
    chk_free (mis_rd_ctrl->mis_rd_dv_ctrl_tbl);
    mis_rd_ctrl->mis_rd_dv_ctrl_tbl = NULL;
    }

  if (num_devs == 0)
    {
    chk_free (mis_rd_ctrl->mis_device_ctrl_tbl);
    mis_rd_ctrl->mis_device_ctrl_tbl = NULL;
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*			_mi_prep_nvl_rd_ctrl				*/
/************************************************************************/

static ST_RET _mi_prep_nvl_rd_ctrl (READ_REQ_INFO *read_req_info, 
				MIS_READ_CTRL *mis_rd_ctrl)
  {
MIS_VCC *mis_vcc;
MIS_DS *mis_ds;
OBJECT_NAME *vl_name;
ST_RET rc;

/* Find the dataset */
  vl_name = &read_req_info->va_spec.vl_name; 
  rc = mis_objname_to_ds (mis_rd_ctrl->mis_vcc, vl_name, &mis_ds); 

  if (rc != SD_SUCCESS)
    {  
    MI_LOG_NERR1 ("Read DS : NVL '%s' not found",
				vl_name->obj_name.vmd_spec);
    return (MVL_ERR_NVL_NOT_FOUND);
    }

  mis_vcc = mis_rd_ctrl->mis_vcc;
  mis_rd_ctrl->mis_rd_dv_ctrl_tbl = 
    _mis_ds_to_rd_dv_ctrl (mis_vcc, mis_ds, mis_rd_ctrl, MIS_RD_REASON_READ_IND);
  mis_rd_ctrl->num_var = mis_ds->num_dv_ref;
  mis_rd_ctrl->num_data_vals = mis_ds->num_dv_ref;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			_mis_ds_to_rd_dv_ctrl 				*/
/************************************************************************/

MIS_RD_DV_CTRL *_mis_ds_to_rd_dv_ctrl (MIS_VCC *mis_vcc, MIS_DS *mis_ds,
				       ST_VOID *ctrl, ST_INT rd_reason)
  {
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
MIS_RD_DV_CTRL *rd_dv_ctrl;
MIS_DV_REF dv_ref;
MIS_DV_REF dv_index;
ST_INT i;
ST_INT num_var;

  num_var = mis_ds->num_dv_ref;
  mis_rd_dv_ctrl = (MIS_RD_DV_CTRL *) chk_calloc (num_var, 
					sizeof(MIS_RD_DV_CTRL));
  rd_dv_ctrl = mis_rd_dv_ctrl;
  for (i = 0; i < num_var; ++i, ++rd_dv_ctrl)
    {
    rd_dv_ctrl->mis_vcc = mis_vcc;
    rd_dv_ctrl->data_buf = &rd_dv_ctrl->dv_data;
    rd_dv_ctrl->rd_reason = rd_reason;
    rd_dv_ctrl->mis_rd_ctrl = (struct _mis_read_ctrl *)ctrl;
    rd_dv_ctrl->mis_info_ctrl = (struct _mis_info_rpt_ctrl *)ctrl;
    dv_ref = mis_ds->mis_dv_ref_tbl[i];
    dv_index = dv_ref & MIS_INDEX_MASK;
    rd_dv_ctrl->mis_dv_ref = dv_ref;
    rd_dv_ctrl->mi_type = 
             (dv_ref >> MIS_DVREF_TYPE_SHIFT) & MIS_DVREF_TYPE_MASK_A;
    rd_dv_ctrl->vl_index = i;
    }
  return (mis_rd_dv_ctrl);
  }

/************************************************************************/
/*		_mis_handle_special_var_read 				*/
/************************************************************************/

ST_VOID _mis_handle_special_var_read (MIS_RD_DV_CTRL *mis_rd_dv_ctrl)
  {
MIS_VCC *mis_vcc;
MIS_DSTS *mis_dsts;
MIS_DV *mis_dv;
ST_CHAR *name;
ST_RET rc;
MI_MMS_OBJECT_NAME *obj;
MI_TS_TIMESTAMP *ts_timestamp;
MI_DS_CONDITIONS *ds_conditions;
MI_EC_DETECTED *ec_detected;
ST_INT i;

  mis_vcc = mis_rd_dv_ctrl->mis_vcc;
  mis_dsts = mis_rd_dv_ctrl->mis_dsts;
  mis_dv = MIS_DV_REF_TO_DV (mis_rd_dv_ctrl->mis_dv_ref);
  name = mis_dv->dv_name;
  rc = SD_FAILURE;

/* DSTS Special Variables */
  if (mis_dsts != NULL)
    {
    if (strcmp (name, "Transfer_Set_Name") == 0)
      {
      obj = (MI_MMS_OBJECT_NAME *)mis_rd_dv_ctrl->data_buf;
      obj->Scope = ICC_SPEC;
      strcpy (obj->DomainName, mis_vcc->icc_name);
      strcpy (obj->Name, mis_dsts->ts_name);
      rc = SD_SUCCESS;
      }
    else if (strcmp (name, "Transfer_Set_Time_Stamp") == 0)
      {
      ts_timestamp = (long *)mis_rd_dv_ctrl->data_buf;
      *ts_timestamp = mis_dsts->ts_timestamp;
      rc = SD_SUCCESS;
      }
    else if (strcmp (name, "DSConditions_Detected") == 0)
      {
      ds_conditions = (unsigned char *)mis_rd_dv_ctrl->data_buf;
      *ds_conditions = mis_dsts->ds_conditions;
      rc = SD_SUCCESS;
      }
    else if (strcmp (name, "Event_Code_Detected") == 0)
      {
      ec_detected = (short *)mis_rd_dv_ctrl->data_buf;
      *ec_detected = mis_dsts->ec_detected;
      rc = SD_SUCCESS;
      }
    }

/* Other special variables */
  if (strcmp (name, "Next_DSTransfer_Set") == 0)
    {
    if (mis_rd_dv_ctrl->rd_reason == MIS_RD_REASON_READ_IND)
      {

    /* See if we have a DSTS available for this remote */
      mis_dsts = mis_vcc->mis_dsts_tbl;
      for (i = 0; i < mis_vcc->num_dsts_ctrl; ++i, ++mis_dsts)
        {
        if ((mis_dsts->dsts_flags & MIS_DSTS_FLAGS_IN_USE) == 0)
          {
        /* OK, we found an available DSTS				*/
          rc = SD_SUCCESS;

        /* Write the output data 	*/
          obj = (MI_MMS_OBJECT_NAME *)mis_rd_dv_ctrl->data_buf;
          obj->Scope = ICC_SPEC;
          strcpy (obj->DomainName, mis_vcc->icc_name);
          strcpy (obj->Name, mis_dsts->ts_name);

        /* Now set up the mic_dsts */
          mis_dsts->dsts_flags |= MIS_DSTS_FLAGS_IN_USE;
          mis_dsts->mi_conn = 
            mis_rd_dv_ctrl->mis_rd_ctrl->mvl_ind_ctrl->event->net_info->mi_conn;
       	  break;
          }
        }
      if (i >= mis_vcc->num_dsts_ctrl)
        {
        MI_LOG_NERR0 ("Read 'Next_DSTransfer_Set' error; no DSTS available"); 
        mis_rd_dv_ctrl->failure = ARE_TEMP_UNAVAIL;
	}
      }
    else
      {
      MI_LOG_NERR0 ("Error: 'Next_DSTransfer_Set' can't be in report"); 
      }
    }

/* Connection startup special variables */
  if (strcmp (name, "Supported_Features") == 0)
    {
    memcpy (mis_rd_dv_ctrl->data_buf, &mis_vcc->misv.Supported_Features, 
				  sizeof (mis_vcc->misv.Supported_Features));
    rc = SD_SUCCESS;
    }
  else if (strcmp (name, "TASE2_Version") == 0)
    {
    memcpy (mis_rd_dv_ctrl->data_buf, &mis_vcc->misv.TASE2_Version, 
				  sizeof (mis_vcc->misv.TASE2_Version));
    rc = SD_SUCCESS;
    }
  else if (strcmp (name, "Bilateral_Table_ID") == 0)
    {
    memcpy (mis_rd_dv_ctrl->data_buf, mis_vcc->misv.Bilateral_Table_Id, 
				  sizeof (mis_vcc->misv.Bilateral_Table_Id));
    rc = SD_SUCCESS;
    }

  if (rc == SD_SUCCESS)
    mis_rd_dv_ctrl->mi_type = _mis_get_special_var_type_id (name);
  else
    {
    MI_LOG_FLOW1 ("Read error: accessing special variable '%s'", name); 
    rc = SD_FAILURE;
    }

  mis_rd_dv_done (mis_rd_dv_ctrl, rc);
  }

/************************************************************************/
/************************************************************************/
/*			mis_rd_dv_done 					*/
/************************************************************************/

ST_VOID mis_rd_dv_done (MIS_RD_DV_CTRL *mis_rd_dv_ctrl, ST_RET rslt)
  {
MIS_READ_CTRL *mis_rd_ctrl;

  if (mis_rd_dv_ctrl->rd_reason == MIS_RD_REASON_REPORT)
    {
    mis_info_dv_done (mis_rd_dv_ctrl, rslt);
    return;
    }

  mis_rd_ctrl = mis_rd_dv_ctrl->mis_rd_ctrl;

  if (rslt == SD_SUCCESS)
    mis_rd_dv_ctrl->acc_rslt_tag = ACC_RSLT_SUCCESS;

  ++mis_rd_ctrl->num_data_done;
  if (mis_rd_ctrl->num_data_done == mis_rd_ctrl->num_var)
    {
    /* Time to send the response ... */
    _mis_send_read_resp (mis_rd_ctrl);
    }
  }

/************************************************************************/
/*			_ms_copyobj					*/
/* function to copy an object name structure				*/
/************************************************************************/

static ST_VOID _mis_copyobj (ST_UCHAR *to, ST_UCHAR *from)
  {
ST_INT i;

  for (i = 0; i < sizeof (OBJECT_NAME); ++i)
    *(to++) = *(from++);
  }

/************************************************************************/
/*			_mi_prep_read_resp					*/
/************************************************************************/

static ST_RET _mi_prep_read_resp (MIS_READ_CTRL *mis_rd_ctrl,
                          	  READ_RESP_INFO **resp_info_out, 
                          	  ST_UCHAR *asn1_buf, ST_INT asn1_buf_size)
  {
MIS_RD_DV_CTRL *mis_rd_dv_ctrl;
MIS_DEVICE_IND_CTRL *mis_dev_ind_ctrl;
READ_RESP_INFO  *resp_info;
READ_REQ_INFO  *req_info;
ACCESS_RESULT   *ar_ptr;
ST_UCHAR *asn1_start;
ST_INT asn1_len;
ST_INT asn1_buf_left;
ST_INT num_data;
ST_INT i;   
ST_INT num_var;
ST_INT spec_in_rslt_size;   
ST_RET rc;
ST_UCHAR *from;
ST_UCHAR  *to;
ST_INT dlen;

  num_data = mis_rd_ctrl->num_var;
  num_var = num_data;
  req_info = mis_rd_ctrl->read_req_info;

  /* if somebody asked us for Specification in result */
  if (req_info->spec_in_result && req_info->va_spec.var_acc_tag == 0)
    spec_in_rslt_size = sizeof (VARIABLE_LIST) * num_var;
  else
    spec_in_rslt_size = 0;

  resp_info = (READ_RESP_INFO *) M_CALLOC (MSMEM_ENC_INFO, 1, 
            (sizeof (READ_RESP_INFO) + (num_data * sizeof (ACCESS_RESULT))+
             spec_in_rslt_size));

  *resp_info_out = resp_info;

/* Create a read response info struct	*/
  if (req_info->spec_in_result)
    {					/* return specification w/ rslt */
    resp_info->va_spec_pres = SD_TRUE;
    resp_info->va_spec.var_acc_tag = req_info->va_spec.var_acc_tag;
    if (resp_info->va_spec.var_acc_tag == VAR_ACC_VARLIST)
      {
	  	/* list of variables : copy the array of structures	*/
      resp_info->va_spec.num_of_variables = num_var;
      dlen = sizeof (VARIABLE_LIST) * num_var;
      from = (ST_UCHAR *) (req_info  + 1);
      to   = (ST_UCHAR *) (resp_info + 1);
      memcpy (to, from, dlen);

		/* calculate pointer to start of result information	*/
      resp_info->acc_rslt_list = (ACCESS_RESULT *) 
	     ((((VARIABLE_LIST *)(resp_info+1)) + num_var));
      }
    else
      {		/* variable list name : copy list name			*/
      _mis_copyobj ((ST_UCHAR *)&resp_info->va_spec.vl_name,
				(ST_UCHAR *)&req_info->va_spec.vl_name);
		/* set pointer to start of result information		*/
      resp_info->acc_rslt_list = (ACCESS_RESULT *) (resp_info+1);
      }
    }
  else	/* Spec. in result not requested, don't return			*/
    {
    resp_info->va_spec_pres = SD_FALSE;
		/* set pointer to start of result information		*/
    resp_info->acc_rslt_list = (ACCESS_RESULT *) (resp_info+1);
    }

  resp_info->num_of_acc_result = num_data;  
  ar_ptr = resp_info->acc_rslt_list;
  asn1_start = asn1_buf;
  asn1_buf_left = asn1_buf_size;
  mis_rd_dv_ctrl = mis_rd_ctrl->mis_rd_dv_ctrl_tbl;
  mis_dev_ind_ctrl = mis_rd_ctrl->mis_device_ctrl_tbl;
  for (i = 0; i < num_data; ++i, ++ar_ptr)
    {
    /* is it a device or a data value					*/
    if (mis_rd_dv_ctrl && mis_rd_dv_ctrl->vl_index == i)
      {
    /* Assume that it will fail, for ease of coding */
      ar_ptr->acc_rslt_tag = ACC_RSLT_FAILURE;
      ar_ptr->failure = ARE_TYPE_INCONSISTENT;

      if (mis_rd_dv_ctrl->acc_rslt_tag == ACC_RSLT_SUCCESS)
        {
        rc = mi_get_dv_asn1_data (mis_rd_dv_ctrl->data_buf, 
				  mis_rd_dv_ctrl->mi_type,
				  asn1_start, asn1_buf_left, &asn1_len);
        if (rc == SD_SUCCESS)
          {
          ar_ptr->acc_rslt_tag = ACC_RSLT_SUCCESS; 
          ar_ptr->va_data.len = asn1_len;
          ar_ptr->va_data.data = asn1_start;
      
          asn1_buf_left -= asn1_len;   /* adjust buffer control variables	*/
          asn1_start += asn1_len;
          }
        }
      else
        ar_ptr->failure = mis_rd_dv_ctrl->failure;
      mis_rd_dv_ctrl++;
      }
    else if (mis_dev_ind_ctrl && mis_dev_ind_ctrl->vl_index == i)
      {
      /* Assume that it will fail, for ease of coding */
      ar_ptr->acc_rslt_tag = ACC_RSLT_FAILURE;
      ar_ptr->failure = ARE_TYPE_INCONSISTENT;

      if (mis_dev_ind_ctrl->acc_rslt_tag == ACC_RSLT_SUCCESS)
        {
        rc = _mi_get_device_asn1_data (mis_dev_ind_ctrl->data_buf, 
				       mis_dev_ind_ctrl->data_type,
				       asn1_start, asn1_buf_left, &asn1_len);
        if (rc == SD_SUCCESS)
          {
          ar_ptr->acc_rslt_tag = ACC_RSLT_SUCCESS; 
          ar_ptr->va_data.len = asn1_len;
          ar_ptr->va_data.data = asn1_start;
      
          asn1_buf_left -= asn1_len;   /* adjust buffer control variables	*/
          asn1_start += asn1_len;
          }
        }
      else
        ar_ptr->failure = mis_dev_ind_ctrl->failure;
      mis_dev_ind_ctrl++;
      }
    }

  return (SD_SUCCESS);
  }


/************************************************************************/
/* _mis_send_read_resp:							*/
/************************************************************************/

ST_VOID _mis_send_read_resp (MIS_READ_CTRL *mis_rd_ctrl)
  {
MVL_IND_PEND *mvl_ind_ctrl;
MVL_COMM_EVENT *event;
READ_RESP_INFO  *resp_info;
ST_UCHAR *asn1_buf;
ST_RET rc;
MVL_NET_INFO *net_info;

  mvl_ind_ctrl = mis_rd_ctrl->mvl_ind_ctrl;
  event = mvl_ind_ctrl->event;
  net_info = mvl_ind_ctrl->event->net_info;

  asn1_buf = (unsigned char *)M_MALLOC (MSMEM_ASN1_DATA_ENC, mvl_cfg_info->max_msg_size);
  rc = _mi_prep_read_resp (mis_rd_ctrl, &resp_info, asn1_buf, mvl_cfg_info->max_msg_size);

  if (rc == SD_SUCCESS)
    {
    /* Build and send the read response 				*/
    rc = mpl_read_resp (event->u.mms.dec_rslt.id, resp_info);
    
    /* Log the response */
/*    MVL_LOG_RESP (MMSOP_READ, resp_info); */
    
    /* The process functions allocate the response buffer		*/
    M_FREE (MSMEM_ENC_INFO, resp_info);
    }
  /* NOTE: This funct frees mvl_ind_ctrl. DO NOT use after this.*/
  /* NOTE: if rc != SD_SUCCESS, this funct sends error resp.	*/
  _mvl_send_resp_i (mvl_ind_ctrl, rc);

  M_FREE (MSMEM_ASN1_DATA_ENC, asn1_buf);
  if (mis_rd_ctrl->num_data_vals > 0)
    chk_free (mis_rd_ctrl->mis_rd_dv_ctrl_tbl);
  if (mis_rd_ctrl->num_devices > 0)
    chk_free (mis_rd_ctrl->mis_device_ctrl_tbl);
  chk_free (mis_rd_ctrl); 

  }

/************************************************************************/
/*			mvl_get_va_asn1_data 				*/
/************************************************************************/

ST_RET mi_get_dv_asn1_data (ST_VOID *data, ST_INT mi_type,
                         ST_UCHAR *asn1_dest_buffer,
                         ST_INT asn1_buffer_len,
                         ST_INT *asn1_len_out)
  {
RUNTIME_TYPE *rt;
ST_INT num_rt;
ST_RET rc;
ST_UCHAR *asn1_start;
ST_INT asn1_len;
ST_INT type_id;
ASN1_ENC_CTXT localEncCtx;	/* For readability, use "aCtx" to access this.*/
ASN1_ENC_CTXT *aCtx = &localEncCtx;

  type_id = _mi_get_type_id (mi_type);
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
/************************************************************************/
/*			_mis_process_write_ind				*/
/************************************************************************/

ST_VOID _mis_process_write_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
WRITE_REQ_INFO *write_req_info;
MIS_WRITE_CTRL *mis_wr_ctrl;
MIS_WR_DV_CTRL *mis_wr_dv_ctrl;
MIS_DEVICE_IND_CTRL *mis_dev_ind_ctrl;
MIS_IM_IND_CTRL *mis_im_ind_ctrl;
MIS_VCC *mis_vcc;
MIS_DS *mis_ds;
MIS_DSTS *mis_dsts;
MIS_DV *mis_dv;
MIS_DV_REF dv_ref;
MIS_DV_REF dv_index;
ST_INT num_data, num_dv, num_dev, num_im;
OBJECT_NAME *vl_name;
VARIABLE_LIST *vl;
VAR_ACC_SPEC	*va_spec;
VAR_ACC_DATA  *va_data;
RUNTIME_TYPE 	*rt;
ST_INT	num_rt;
ST_INT i;   
ST_INT j;
ST_RET rc;
ST_INT type_id;
ST_CHAR *var_name;
ST_INT mi_type, scope, len;
ST_CHAR obj_name[MAX_IDENT_LEN+1], *temp;
ST_BOOLEAN is_tag;

/* See if we are using ICCP on this connection */
  if (mvl_ind_ctrl->event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_write_ind (mvl_ind_ctrl);
    return;
    }

/* First find the MIS_VCC for this connection */
  mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI read ind, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    }

/* OK, now let's get the write request information */
  write_req_info = (WRITE_REQ_INFO *) mvl_ind_ctrl->event->u.mms.dec_rslt.data_ptr;
  num_data = write_req_info->num_of_data;
  va_spec = &write_req_info->va_spec;

/* Prepare the write control element */
  mis_wr_ctrl = (MIS_WRITE_CTRL *) chk_calloc (1, sizeof (MIS_WRITE_CTRL));
  mis_wr_ctrl->write_req_info = write_req_info;
  mis_wr_ctrl->mvl_ind_ctrl = mvl_ind_ctrl;
  mis_wr_ctrl->mis_vcc = mis_vcc;

  if (va_spec->var_acc_tag == VAR_ACC_NAMEDLIST)
    {
  /* Find the dataset */
    vl_name = &write_req_info->va_spec.vl_name; 
    rc = mis_objname_to_ds (mis_wr_ctrl->mis_vcc, vl_name, &mis_ds); 
    if (rc != SD_SUCCESS)
      {
      chk_free(mis_wr_ctrl);
      MI_LOG_NERR1 ("MI Write DS : NVL '%s' not found",
         		vl_name->obj_name.vmd_spec);
      _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		       MMS_ECODE_OBJ_NONEXISTENT);
      return;
      }

  /* Verify that the number of data sent matches the number of DV in DS	*/  
    if (num_data != mis_ds->num_dv_ref)
      {
      chk_free(mis_wr_ctrl);
      MI_LOG_NERR0 ("MI Write DS : number of data does not match num var in dataset");
      _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		       MMS_ECODE_OBJ_NONEXISTENT);
      return;
      }

    mis_wr_ctrl->mis_wr_dv_ctrl_tbl = (MIS_WR_DV_CTRL *) chk_calloc (num_data, 
  					sizeof(MIS_WR_DV_CTRL));
    mis_wr_dv_ctrl = mis_wr_ctrl->mis_wr_dv_ctrl_tbl;
    mis_wr_ctrl->num_var = num_data;
    for (i = 0; i < num_data; ++i, ++mis_wr_dv_ctrl)
      {
      mis_wr_dv_ctrl->mis_vcc = mis_vcc;
      mis_wr_dv_ctrl->ind_ctrl = mvl_ind_ctrl;

      mis_wr_dv_ctrl->data_buf = &mis_wr_dv_ctrl->dv_data;
      mis_wr_dv_ctrl->wr_reason = MIS_WR_REASON_WRITE_IND;
      mis_wr_dv_ctrl->mis_wr_ctrl = mis_wr_ctrl;
      dv_ref = mis_ds->mis_dv_ref_tbl[i];
      dv_index = dv_ref & MIS_INDEX_MASK;
      mis_wr_dv_ctrl->mis_dv_ref = dv_ref;
      mis_wr_dv_ctrl->mi_type = 
               (dv_ref >> MIS_DVREF_TYPE_SHIFT) & MIS_DVREF_TYPE_MASK_A;
      mis_wr_dv_ctrl->vl_index = i;
      }
    mis_wr_ctrl->num_data_vals = num_data;
    mis_wr_ctrl->num_IM = 0;
    mis_wr_ctrl->num_devices = 0;
    }
  else		/* VAR_ACC_VARLIST */
    {
    mis_wr_ctrl->num_var = write_req_info->va_spec.num_of_variables;
    if (mis_wr_ctrl->num_var != num_data)
      {
      chk_free(mis_wr_ctrl);
      MI_LOG_NERR0 ("MI write ind, num_var != num_data"); 
      _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		       MMS_ECODE_OBJ_NONEXISTENT);
      return;
      }

    num_dv = num_dev = num_im = 0;
    mis_wr_ctrl->mis_wr_dv_ctrl_tbl = 
      (MIS_WR_DV_CTRL *) chk_calloc (num_data, sizeof(MIS_WR_DV_CTRL));

    mis_wr_ctrl->mis_im_ctrl_tbl = 
      (MIS_IM_IND_CTRL *) chk_calloc (num_data, sizeof (MIS_IM_IND_CTRL));

    mis_wr_ctrl->mis_device_ctrl_tbl = 
      (MIS_DEVICE_IND_CTRL *) chk_calloc (num_data, sizeof (MIS_DEVICE_IND_CTRL));

    mis_wr_dv_ctrl = mis_wr_ctrl->mis_wr_dv_ctrl_tbl;
    mis_im_ind_ctrl = mis_wr_ctrl->mis_im_ctrl_tbl;
    mis_dev_ind_ctrl = mis_wr_ctrl->mis_device_ctrl_tbl;

    vl = (VARIABLE_LIST *) (write_req_info+1);
    for (i = 0; i < num_data; ++i, ++vl)
      {
      /* is this an IMTransfer start/stop request			*/
      if ((vl->var_spec.vs.name.object_tag == AA_SPEC) &&
          (strcmp (vl->var_spec.vs.name.obj_name.aa_spec, 
	           MI_IM_TRAN_SET_VAR_NAME) == 0))
        {
	/* we have an ImTransfer Start/Stop request			*/
	mis_im_ind_ctrl->data_buf = &mis_im_ind_ctrl->enable_transfer;
	mis_im_ind_ctrl->data_type = _mi_get_block4_type_id (MI_TYPEID_IMTRANSET);
        mis_im_ind_ctrl->acc_rslt_tag = WR_RSLT_FAILURE; 
	mis_im_ind_ctrl->failure = ARE_TYPE_INCONSISTENT;
	mis_im_ind_ctrl->mis_wr_ctrl = mis_wr_ctrl;
	mis_im_ind_ctrl->vl_index = i;
	num_im++;
	mis_im_ind_ctrl++;
	continue;
	}

      /* is this a device request					*/
      scope = vl->var_spec.vs.name.object_tag;
      switch (scope)
        {
        case VCC_SPEC:
	  strcpy (obj_name, vl->var_spec.vs.name.obj_name.vmd_spec);
        break;

        case ICC_SPEC:
	  strcpy (obj_name, vl->var_spec.vs.name.obj_name.item_id);
        break;

        case AA_SPEC:
	  strcpy (obj_name, vl->var_spec.vs.name.obj_name.aa_spec);
        break;
        }  

      /* watch for _TAG							*/
      len = strlen (obj_name);
      if (len > TAG_TEXT_LEN)
        {
        temp = obj_name + len - TAG_TEXT_LEN;
        is_tag = (strcmp (temp, TAG_TEXT) == 0) ? SD_TRUE : SD_FALSE;
        if (is_tag)
	  temp[0] = 0;
	}
      else
	is_tag = SD_FALSE;

      if (mis_find_vcc_device (mis_vcc, obj_name, scope, 
			       &mis_dev_ind_ctrl->mis_device,
			       &mis_dev_ind_ctrl->mis_device_ref,
			       &mis_dev_ind_ctrl->device_type) == SD_SUCCESS)
	{
	mis_dev_ind_ctrl->mis_vcc = mis_vcc;
	mis_dev_ind_ctrl->op = (is_tag) ? MI_DEVICE_OP_SETTAG : 
				          MI_DEVICE_OP_OPERATE;  

        mis_dev_ind_ctrl->data_type = (is_tag) ? MI_TYPEID_DEV_TAGVALUE : 
						 mis_dev_ind_ctrl->device_type;

        mis_dev_ind_ctrl->acc_rslt_tag = ACC_RSLT_FAILURE;
        mis_dev_ind_ctrl->failure = ARE_OBJ_ACC_UNSUPPORTED;

        mis_dev_ind_ctrl->data_buf = &mis_dev_ind_ctrl->device_data;
        mis_dev_ind_ctrl->mis_wr_ctrl = mis_wr_ctrl;
        mis_dev_ind_ctrl->vl_index = i;
        mis_dev_ind_ctrl++;
        num_dev++;
        continue;
        }

      /* It must be a data value					*/
      /* Assume the worst ... 						*/
      mis_wr_dv_ctrl->resp_tag = WR_RSLT_FAILURE; 
      mis_wr_dv_ctrl->failure = ARE_TYPE_INCONSISTENT; 
      
  /* Initialize some of the mis_wr_dv_ctrl members */
      mis_wr_dv_ctrl->mis_vcc = mis_vcc;
      mis_wr_dv_ctrl->ind_ctrl = mvl_ind_ctrl;
      mis_wr_dv_ctrl->data_buf = &mis_wr_dv_ctrl->dv_data;
      mis_wr_dv_ctrl->wr_reason = MIS_WR_REASON_WRITE_IND;
      mis_wr_dv_ctrl->mis_wr_ctrl = mis_wr_ctrl;

      if (vl->alt_access_pres)  /* no alternate access support now	*/
        {
        MI_LOG_NERR0 ("Write Indication : Alternate Access not supported");
        continue;
        }
      if (vl->var_spec.var_spec_tag != VA_SPEC_NAMED)
        {
        MI_LOG_NERR0 ("Write: Invalid VarSpecTag");
        continue;
        }
      
      rc = mis_objname_to_dv (mis_vcc, 
      			     &vl->var_spec.vs.name, 
      			     &mis_dv,	
      			     &mis_wr_dv_ctrl->mis_dv_ref,
      			     &mis_wr_dv_ctrl->mi_type);

      if (rc != SD_SUCCESS)  /* could not find DV, look for DSTS	*/
        {
	mis_dsts = mis_vcc->mis_dsts_tbl;
        var_name = vl->var_spec.vs.name.obj_name.vmd_spec;
	for (j = 0; j < mis_vcc->num_dsts_ctrl; ++j, ++mis_dsts)
          {
	  if ((mis_dsts->dsts_flags & MIS_DSTS_FLAGS_IN_USE) &&
	      (strcmp (var_name, mis_dsts->ts_name) == 0))
            {
            mis_wr_dv_ctrl->mis_dsts = mis_dsts;
      	    mis_wr_dv_ctrl->mi_type = MI_TYPEID_DSTS;
            rc = SD_SUCCESS;
            mi_wait_service_ms = 0;
	    break;
            }
          }
        }
      
      if (rc != SD_SUCCESS)  /* could not find DV	*/
        {
        MI_LOG_NERR1 ("MI Write ind: variable '%s' not found", 
		          vl->var_spec.vs.name.obj_name.vmd_spec); 

        mis_wr_dv_ctrl->failure = ARE_OBJ_NONEXISTENT;
        mis_wr_dv_ctrl->vl_index = i;
        num_dv++;
        mis_wr_dv_ctrl++;
        continue;
        }

      mis_wr_dv_ctrl->vl_index = i;
      num_dv++;
      mis_wr_dv_ctrl++;
      }
    
    if ((num_dev==0) && (num_dv==0) && (num_im==0))
      {
      MI_LOG_NERR0 ("MI Write : all variables returned with DAE.");
      return;
      }

    /* free up what we don't need. The assumption is that if we can't find it, it's a DataValue		*/
    if (num_dv == 0)
      {
      chk_free (mis_wr_ctrl->mis_wr_dv_ctrl_tbl);
      mis_wr_ctrl->mis_wr_dv_ctrl_tbl = NULL;
      }

    if (num_im == 0) 
      {
      chk_free (mis_wr_ctrl->mis_im_ctrl_tbl);
      mis_wr_ctrl->mis_im_ctrl_tbl  = NULL;
      }

    if (num_dev == 0) 
      {
      chk_free (mis_wr_ctrl->mis_device_ctrl_tbl);
      mis_wr_ctrl->mis_device_ctrl_tbl = NULL;
      }

    mis_wr_ctrl->num_data_vals = num_dv;
    mis_wr_ctrl->num_IM = num_im;
    mis_wr_ctrl->num_devices = num_dev;
    }

  /* OK, now convert the data to local and call the user function ...	*/
  mis_wr_dv_ctrl = mis_wr_ctrl->mis_wr_dv_ctrl_tbl;
  mis_im_ind_ctrl = mis_wr_ctrl->mis_im_ctrl_tbl;
  mis_dev_ind_ctrl = mis_wr_ctrl->mis_device_ctrl_tbl;
  va_data = write_req_info->va_data;
  for (i = 0; i < num_data; ++i, ++vl, ++va_data)
    {
    if (mis_wr_ctrl->num_data_vals && mis_wr_dv_ctrl->vl_index == i)
      {
      mi_type = mis_wr_dv_ctrl->mi_type;
      if (mi_type == MI_TYPEID_DSTS)
	{
        _mis_handle_dsts_var_write (mis_wr_dv_ctrl, va_data->data, va_data->len);
	}
      else if (mi_type > MI_LAST_DV_TYPEID)
        _mis_handle_special_var_write (mis_wr_dv_ctrl, va_data->data, va_data->len);
      else if (mis_wr_dv_ctrl->failure == ARE_OBJ_NONEXISTENT)
        {
        mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
        mis_wr_dv_ctrl++;
        continue;
        }
      else if ((mis_wr_dv_ctrl->mis_dv_ref & MIS_DVREF_WRITE_EN) == 0)  
        {
        mis_dv = MIS_DV_REF_TO_DV(mis_wr_dv_ctrl->mis_dv_ref);
        MI_LOG_NERR1 ("MI Write ind: variable '%s' read only", mis_dv->dv_name); 
	mis_wr_dv_ctrl->failure = ARE_OBJ_ACCESS_DENIED;
        mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
        mis_wr_dv_ctrl++;
        continue;
        }
      else
        {
        type_id = _mi_get_type_id (mis_wr_dv_ctrl->mi_type);
        rc = mvl_get_runtime (type_id, &rt, &num_rt);
        if (rc != SD_SUCCESS)
          {
          MI_LOG_NERR1 ("Could not get type_id %d", type_id);
          continue;
          }
        rc = ms_asn1_to_local (rt, num_rt, va_data->data, va_data->len, 
  			       (char *)mis_wr_dv_ctrl->data_buf);
  
        if (rc == SD_SUCCESS)
          u_mis_wr_dv (mis_wr_dv_ctrl);
        else
          {
          mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
          MI_LOG_NERR1 ("MI Write Ind: ms_asn1_to_local failed, rc = %x", rc);
          }
        }
      mis_wr_dv_ctrl++;
      }
    else if (mis_wr_ctrl->num_IM && mis_im_ind_ctrl->vl_index == i)
      {
      _mis_start_stop_im_transfer (mis_im_ind_ctrl, va_data);
      mis_im_ind_ctrl++;
      }
    else if (mis_wr_ctrl->num_devices && mis_dev_ind_ctrl->vl_index == i)
      {
      _mis_device_write_ind (mis_dev_ind_ctrl, va_data);
      mis_dev_ind_ctrl++;
      }
    }
  }

/************************************************************************/
/*    		_mis_handle_dsts_var_write 				*/
/************************************************************************/

ST_VOID _mis_handle_dsts_var_write (MIS_WR_DV_CTRL *mis_wr_dv_ctrl,
				    ST_UCHAR *asn1_data, ST_INT asn1_len)
  {
MIS_VCC *mis_vcc;
MIS_DSTS *mis_dsts;
MI_CONN *mi_conn;
MI_DSTS_DATA mi_dsts_data;
MIS_INFO_RPT_CTRL *mis_info_ctrl;
ST_RET rc;
RUNTIME_TYPE 	*rt;
ST_INT	num_rt;
ST_INT type_id;
ST_INT max_pdu_size;
ST_INT nvl_report_size;
ST_INT num_change_bytes;
OBJECT_NAME ds_obj;
time_t curr_time;

  mis_vcc = mis_wr_dv_ctrl->mis_vcc;
  mis_dsts = mis_wr_dv_ctrl->mis_dsts;

  if ((mis_dsts->dsts_flags & MIS_DSTS_FLAGS_IN_USE) == 0)
    {
    MI_LOG_NERR0 ("MIS_DSTS not active");
    mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
    }

  type_id = _mi_get_type_id (MI_TYPEID_DSTS);
  rc = mvl_get_runtime (type_id, &rt, &num_rt);
  if (rc != SD_SUCCESS)
    {
    mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
    mis_dsts->mi_dsts_data.Status = 0;
    MI_LOG_NERR1 ("Could not get type_id %d", type_id);
    mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
    return;
    }
  rc = ms_asn1_to_local (rt, num_rt, asn1_data, asn1_len, 
		         (ST_CHAR *) &mi_dsts_data);
  
  if (rc != SD_SUCCESS)
    {
    mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
    MI_LOG_NERR1 ("MI Write Ind: ms_asn1_to_local failed, rc = %x", rc);
    mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
    return;
    }

/* OK, we have the DSTS write data, copy it to the DSTS control */
  memcpy (&mis_dsts->mi_dsts_data, &mi_dsts_data, sizeof (MI_DSTS_DATA));

  if (mis_dsts->mi_dsts_data.Status != SD_FALSE)
    {
  /* Need to find the specified data set */
    ds_obj.obj_name.vmd_spec = mi_dsts_data.DataSetName.name;
    if (mi_dsts_data.DataSetName.scope == 1)
      {
      ds_obj.object_tag = ICC_SPEC;
      ds_obj.domain_id = mi_dsts_data.DataSetName.DomainName;
      }
    else
      ds_obj.object_tag = VCC_SPEC;

    rc = mis_objname_to_ds (mis_vcc, &ds_obj, &mis_dsts->mis_ds);
    if (rc != SD_SUCCESS)
      {
      mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
      mis_dsts->mi_dsts_data.Status = 0;
      MI_LOG_NERR1 ("MI Write DSTS Ind: DS '%s' not found", 
				mi_dsts_data.DataSetName.name);
      mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
      return;
      }

  /* If interval is selected, check the min interval */
    if ((mis_dsts->mi_dsts_data.DSConditionsRequested & MI_DSC_INTERVAL) &&
        (mis_dsts->mi_dsts_data.Interval < mis_vcc->min_interval))
      {
      mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
      mis_dsts->mi_dsts_data.Status = 0;
      MI_LOG_NERR2 ("MI Write DSTS Ind: Interval '%d' is less than min (%d)", 
		    mis_dsts->mi_dsts_data.Interval, mis_vcc->min_interval);
      mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
      return;
      }

  /* If integrity is selected, check the min interval */
    if ((mis_dsts->mi_dsts_data.DSConditionsRequested & MI_DSC_INTEGRITY) &&
        (mis_dsts->mi_dsts_data.IntegrityCheck < mis_vcc->min_interval))
      {
      mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
      mis_dsts->mi_dsts_data.Status = 0;
      MI_LOG_NERR2 ("MI Write DSTS Ind: IntegrityCheck '%d' is less than min (%d)", 
		    mis_dsts->mi_dsts_data.IntegrityCheck, mis_vcc->min_interval);
      mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
      return;
      }

    mis_dsts->last_report_sent = 0;
    mis_dsts->last_complete_report_sent = 0;
    mis_dsts->first_change_reported = 0;
    mis_dsts->saturated = SD_FALSE;

    curr_time = time (NULL);
    if (mi_dsts_data.StartTime == 0)
      {
      mis_dsts->next_interval_rpt = curr_time + mis_dsts->mi_dsts_data.Interval;
      mis_dsts->next_buffer_rpt = 0;
      mis_dsts->next_integrity_rpt = curr_time + mis_dsts->mi_dsts_data.IntegrityCheck;
      }
    else 
      {
      mis_dsts->next_interval_rpt = mi_dsts_data.StartTime + mis_dsts->mi_dsts_data.Interval;
      mis_dsts->next_buffer_rpt = 0;
      mis_dsts->next_integrity_rpt = mi_dsts_data.StartTime + 
				     mis_dsts->mi_dsts_data.IntegrityCheck;

      /* in case StartTime is in the past find the correct time for the next report */
      if (mis_dsts->mi_dsts_data.Interval > 0)
	{
	while (mis_dsts->next_interval_rpt < curr_time)
	  mis_dsts->next_interval_rpt += mis_dsts->mi_dsts_data.Interval;
	}

      if (mis_dsts->mi_dsts_data.IntegrityCheck > 0)
	{
	while (mis_dsts->next_integrity_rpt < curr_time)
	  mis_dsts->next_integrity_rpt += mis_dsts->mi_dsts_data.IntegrityCheck;
	}
      }

  /* Allocate the changed_dv byte array */
    if (mis_dsts->changed_dv != NULL)
      chk_free (mis_dsts->changed_dv);

    num_change_bytes = mis_dsts->mis_ds->num_dv_ref/8 + 1;
    mis_dsts->changed_dv = (unsigned char *)chk_calloc (num_change_bytes, sizeof (ST_UINT8));

   /* Verify that the dataset data will fit as NVL  */
    memset (mis_dsts->changed_dv, 0xff, num_change_bytes);
    mi_conn = mis_wr_dv_ctrl->ind_ctrl->event->net_info->mi_conn;
    max_pdu_size = mi_conn->net_info->max_pdu_size;
    mis_info_ctrl = _mis_get_info_rpt_ctrl (mis_vcc, mis_dsts);
    mis_info_ctrl->report_type = MIS_REPORT_TYPE_NVL;
    nvl_report_size = _mis_calc_info_rpt_size (mis_info_ctrl);
    chk_free (mis_info_ctrl->mis_rd_dv_ctrl_tbl);
    chk_free (mis_info_ctrl);
    if (nvl_report_size > max_pdu_size)
      {
      mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
      MI_LOG_NERR0 ("Dataset too large for PDU");
      mis_dsts->mi_dsts_data.Status = 0;
      mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
      return;
      }

   /* Now calculate the PDU size when all are included as LOV */
    mis_info_ctrl = _mis_get_info_rpt_ctrl (mis_vcc, mis_dsts);
    mis_info_ctrl->report_type = MIS_REPORT_TYPE_LOV;
    mis_dsts->max_asn1_size = _mis_calc_info_rpt_size (mis_info_ctrl);
    chk_free (mis_info_ctrl->mis_rd_dv_ctrl_tbl);
    chk_free (mis_info_ctrl);

    memset (mis_dsts->changed_dv, 0, num_change_bytes);
    u_mis_dsts_activity (mis_vcc, mis_dsts, MIS_DSTS_START_TRANSFER, 0);
    }
  else 	/* The client is disabling this DSTS */
    {
    mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
    mis_dsts->mi_dsts_data.Status = 0;
    u_mis_dsts_activity (mis_vcc, mis_dsts, MIS_DSTS_STOP_TRANSFER, 0);
    }

  mis_wr_dv_done (mis_wr_dv_ctrl, SD_SUCCESS);
  }

/************************************************************************/
/*    		_mis_handle_special_var_write 				*/
/************************************************************************/

ST_VOID _mis_handle_special_var_write (MIS_WR_DV_CTRL *mis_wr_dv_ctrl,
				    ST_UCHAR *asn1_data, ST_INT asn1_len)
  {
  mis_wr_dv_done (mis_wr_dv_ctrl, SD_FAILURE);
  }

/************************************************************************/
/*			mis_wr_dv_done 					*/
/************************************************************************/

ST_VOID mis_wr_dv_done (MIS_WR_DV_CTRL *mis_wr_dv_ctrl, ST_RET rslt)
  {
MIS_WRITE_CTRL *mis_wr_ctrl;

  mis_wr_ctrl = mis_wr_dv_ctrl->mis_wr_ctrl;

  if (rslt == SD_SUCCESS)
    mis_wr_dv_ctrl->resp_tag = WR_RSLT_SUCCESS;

  ++mis_wr_ctrl->num_data_done;
  if (mis_wr_ctrl->num_data_done == mis_wr_ctrl->num_var)
    _mis_send_write_resp (mis_wr_ctrl);

  }

/************************************************************************/
/* _mis_send_write_resp:						*/
/************************************************************************/

ST_VOID _mis_send_write_resp (MIS_WRITE_CTRL *mis_wr_ctrl)
  {
MVL_IND_PEND *mvl_ind_ctrl;
MVL_COMM_EVENT *event;
WRITE_RESP_INFO  *resp_info;
WRITE_RESULT     *wr_rslt;
MIS_WR_DV_CTRL *mis_wr_dv_ctrl;
MIS_IM_IND_CTRL *mis_im_ctrl;
MIS_DEVICE_IND_CTRL *mis_dev_ctrl;
ST_INT i;   
ST_RET rc;
ST_INT num_data;
MVL_NET_INFO *net_info;

  mvl_ind_ctrl = mis_wr_ctrl->mvl_ind_ctrl;
  event = mvl_ind_ctrl->event;
  num_data = mis_wr_ctrl->num_var;
  net_info = mvl_ind_ctrl->event->net_info;
    
  resp_info = (WRITE_RESP_INFO *) M_CALLOC (MSMEM_ENC_INFO, 1, 
            (sizeof(WRITE_RESP_INFO) + (num_data * sizeof (WRITE_RESULT))));

  /* Create a write response info struct	*/
  resp_info->num_of_result = num_data;
  wr_rslt = (WRITE_RESULT *) (resp_info +1);
         
  mis_wr_dv_ctrl = mis_wr_ctrl->mis_wr_dv_ctrl_tbl;
  mis_im_ctrl = mis_wr_ctrl->mis_im_ctrl_tbl;
  mis_dev_ctrl = mis_wr_ctrl->mis_device_ctrl_tbl;
  for (i = 0; i < num_data; ++i, ++wr_rslt)
    {
    if (mis_wr_dv_ctrl && mis_wr_dv_ctrl->vl_index == i)
      {
      wr_rslt->resp_tag = mis_wr_dv_ctrl->resp_tag;
      wr_rslt->failure = mis_wr_dv_ctrl->failure;
      mis_wr_dv_ctrl++;
      continue;
      }

    if (mis_im_ctrl && mis_im_ctrl->vl_index == i)
      {
      wr_rslt->resp_tag = mis_im_ctrl->acc_rslt_tag;
      wr_rslt->failure = mis_im_ctrl->failure;
      mis_im_ctrl++;
      continue;
      }

    if (mis_dev_ctrl && mis_dev_ctrl->vl_index == i)
      {
      wr_rslt->resp_tag = mis_dev_ctrl->acc_rslt_tag;
      wr_rslt->failure = mis_dev_ctrl->failure;
      mis_im_ctrl++;
      continue;
      }
    }

/* Build a write response 						*/
  rc = mpl_write_resp (event->u.mms.dec_rslt.id, resp_info);

  /* NOTE: This funct frees mvl_ind_ctrl. DO NOT use after this.*/
  /* NOTE: if rc != SD_SUCCESS, this funct sends error resp.	*/
  _mvl_send_resp_i (mvl_ind_ctrl, rc);
    
    /* Log the response */
/*  MVL_LOG_RESP (MMSOP_WRITE, resp_info); */

  M_FREE (MSMEM_ENC_INFO, resp_info);
  if (mis_wr_ctrl->mis_wr_dv_ctrl_tbl)
    chk_free (mis_wr_ctrl->mis_wr_dv_ctrl_tbl);
  if (mis_wr_ctrl->mis_im_ctrl_tbl)
    chk_free (mis_wr_ctrl->mis_im_ctrl_tbl);
  if (mis_wr_ctrl->mis_device_ctrl_tbl)
    chk_free (mis_wr_ctrl->mis_device_ctrl_tbl);

  chk_free (mis_wr_ctrl); 
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			_mi_process_getnam_ind				*/
/* A get name indication has been received, process it and respond	*/
/************************************************************************/

#define MI_GNL_RESP_OH		25

#define GETNAM_RESP_BUF_SIZE (sizeof(NAMELIST_RESP_INFO)+ \
			     (MVL_MAX_GNL_NAMES * sizeof(ST_CHAR *)))

ST_VOID _mis_process_getnam_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
NAMELIST_REQ_INFO   *req_info;
NAMELIST_RESP_INFO  *resp_info;
ST_CHAR **ptr;
ST_INT num_obj;
ST_INT i;   
ST_INT max_size;   
ST_INT16 mms_class;

/* See if we are using ICCP on this connection */
  if (mvl_ind_ctrl->event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_getnam_ind (mvl_ind_ctrl);
    return;
    }

  req_info = (NAMELIST_REQ_INFO *) mvl_ind_ctrl->event->u.mms.dec_rslt.data_ptr;
  mms_class = req_info->obj.mms_class;

/* Now create the response data structure	*/
  resp_info = (NAMELIST_RESP_INFO *) M_CALLOC (MSMEM_ENC_INFO, 1, GETNAM_RESP_BUF_SIZE);
  ptr = (ST_CHAR **)(resp_info + 1);

  num_obj = 0;
  switch (req_info->obj.mms_class)
    {
    case MMS_CLASS_VAR:                		/* NamedVariables 	*/
      num_obj = _mi_gnl_ind_vars (mvl_ind_ctrl, req_info, ptr, 
				&resp_info->more_follows, MVL_MAX_GNL_NAMES);
    break;

    case MMS_CLASS_VARLIST:                  	/* NamedVariableLists 	*/
      num_obj = _mi_gnl_ind_nvls (mvl_ind_ctrl, req_info, ptr, 
				&resp_info->more_follows, MVL_MAX_GNL_NAMES);
    break;

    case MMS_CLASS_DOM:                   	/* Domains 		*/
      num_obj = _mi_gnl_ind_doms (mvl_ind_ctrl, req_info, ptr, 
				&resp_info->more_follows, 
				MVL_MAX_GNL_NAMES);
    break;

    case MMS_CLASS_JOU:	      	/* Journals 		*/
    case MMS_CLASS_SCAT_ACC:	/* Scattered Access 	*/
    case MMS_CLASS_TYPE:	/* NamedTypes 		*/
    case MMS_CLASS_SEM:	      	/* Semaphores 		*/
    case MMS_CLASS_EV_COND:     /* Event Conditions 	*/
    case MMS_CLASS_EV_ACT:      /* EventActions 	*/
    case MMS_CLASS_EV_ENROLL:   /* EventEnrollments 	*/
    case MMS_CLASS_PI:	      	/* Program Invocations 	*/
    case MMS_CLASS_OPER_STA:    /* OperatorStations 	*/
    break;

    default:
    break;
    }

/* Let's make sure they will all fit into the PDU */
  max_size = mvl_ind_ctrl->event->net_info->max_pdu_size - MI_GNL_RESP_OH;
  resp_info->num_names = 0;  
  for (i = 0; i < num_obj; ++i, ++ptr)
    {
    max_size -= (2 + strlen (*ptr));
    if (resp_info->num_names > MVL_MAX_GNL_NAMES || max_size <= 0)
      {
      resp_info->more_follows = SD_TRUE;
      break;
      }
    ++resp_info->num_names;
    }

/* Build a Get Name List response 					*/
  mvl_ind_ctrl->u.namelist.resp_info = resp_info;
  mplas_namelist_resp (mvl_ind_ctrl);
  M_FREE (MSMEM_ENC_INFO, resp_info);
  }

/************************************************************************/
/*			_mi_gnl_ind_vars 				*/
/************************************************************************/

ST_INT _mi_gnl_ind_vars (MVL_IND_PEND *mvl_ind_ctrl, NAMELIST_REQ_INFO *req_info, 
			 ST_CHAR **ptr, ST_BOOLEAN *more_follows_out, 
			 ST_INT max_names)
  {
MIS_VCC *mis_vcc;
MIS_DV_REF num_dv_ref;
MIS_DV_REF *mis_dv_ref_tbl;
MIS_DV_REF dv_ref_index;
ST_INT i;


  mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI GNL, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    return (0);
    }

/* Get the DV REF table and number of DV for the scope */
  if (req_info->objscope == VCC_SPEC)
    {
    num_dv_ref = (MIS_DV_REF) mis_vcc->num_vcc_dv_ref;
    mis_dv_ref_tbl = mis_vcc->vcc_dv_ref_tbl;
    }
  else if (req_info->objscope == ICC_SPEC)
    {
    if (strcmp (req_info->dname, mis_vcc->icc_name) != 0)
      return (0);
    num_dv_ref = (MIS_DV_REF) mis_vcc->num_icc_dv_ref;
    mis_dv_ref_tbl = mis_vcc->icc_dv_ref_tbl;
    }
  else
    {
    *more_follows_out = SD_FALSE;
    return (0);
    }

  if (req_info->cont_after_pres)
    {
    dv_ref_index = _mis_find_ca_dv_index (req_info->continue_after, 
			       		 num_dv_ref, mis_dv_ref_tbl);
    }
  else
    dv_ref_index = 0;


/* Now make the list for the response */
  for (i = 0; i < max_names && dv_ref_index < num_dv_ref; ++i, ++dv_ref_index)
    {
    ptr[i] = _mis_dv_ref_to_name (mis_dv_ref_tbl[dv_ref_index]);
    }

  if (dv_ref_index < num_dv_ref)
    *more_follows_out = SD_TRUE;

  return (i);
  }

/************************************************************************/
/*			_mis_find_ca_dv_index 				*/
/************************************************************************/

MIS_DV_REF _mis_find_ca_dv_index (ST_CHAR *ca_name, ST_INT num_dv_ref, 
				 MIS_DV_REF *mis_dv_ref_tbl)
  {
ST_INT i;
ST_BOOLEAN found;
ST_CHAR *dv_name;
ST_INT c;

  found = SD_FALSE;
  for (i = 0; i < num_dv_ref; ++i)
    {
    dv_name = _mis_dv_ref_to_name (mis_dv_ref_tbl[i]);
    c = strcmp (dv_name, ca_name);
    if (c == 0)
      {
      ++i;		/* Found it, point to next name */
      found = SD_TRUE;
      break;
      }
    if (c > 0)		/* Whoops went past, stop here */
      {
      found = SD_FALSE;
      break;
      }
    }

  if (found == SD_FALSE)
    /* return (mis_dv_ref_tbl[0] & MIS_INDEX_MASK); */
    return (MIS_DV_REF) num_dv_ref; 
  else
    /* return (mis_dv_ref_tbl[i] & MIS_INDEX_MASK); */
    return (MIS_DV_REF) i; 
  }

/************************************************************************/

ST_CHAR *_mis_dv_ref_to_name (MIS_DV_REF dv_ref)
  {
MIS_DV_REF dv_index;

  dv_index = dv_ref & MIS_INDEX_MASK;
  return (mis_dv_ctrl[dv_index].dv_name);
  }

/************************************************************************/
/************************************************************************/
/*			_mi_gnl_ind_nvls 				*/
/************************************************************************/

ST_INT _mi_gnl_ind_nvls (MVL_IND_PEND *mvl_ind_ctrl, 
			     NAMELIST_REQ_INFO *req_info, 
			     ST_CHAR **ptr, ST_BOOLEAN *more_follows_out, 
			     ST_INT max_names)
  {
MIS_VCC *mis_vcc;
MIS_DS **mis_ds_ptr_tbl;
ST_INT num_ds;
ST_INT i;
ST_INT mis_ds_index;

  mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI GNL, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    return (0);
    }

/* Get the DS table and number of DS for the scope */
  if (req_info->objscope == VCC_SPEC)
    {
    num_ds = mis_vcc->num_vcc_ds;
    mis_ds_ptr_tbl = mis_vcc->vcc_ds_ctrl_tbl;
    }
  else if (req_info->objscope == ICC_SPEC)
    {
    if (strcmp (req_info->dname, mis_vcc->icc_name) != 0)
      return (0);
    num_ds = mis_vcc->num_icc_ds;
    mis_ds_ptr_tbl = mis_vcc->icc_ds_ctrl_tbl;
    }
  else
    {
    return (0);
    }

  *more_follows_out = SD_FALSE;
  if (req_info->cont_after_pres)
    {
    mis_ds_index = _mis_find_ca_ds_index (req_info->continue_after, 
			       		 num_ds, mis_ds_ptr_tbl);
    }
  else
    mis_ds_index = 0;

/* Now make the list for the response */
  for (i = 0; i < max_names && mis_ds_index < num_ds; ++i, ++mis_ds_index)
    {
    ptr[i] = mis_ds_ptr_tbl[mis_ds_index]->ds_name;
    }

  if (mis_ds_index < num_ds)
    *more_follows_out = SD_TRUE;

  return (i);
  }


/************************************************************************/
/*			_mis_find_ca_ds_index 				*/
/************************************************************************/

ST_INT _mis_find_ca_ds_index (ST_CHAR *ca_name, ST_INT max_ds, 
			      MIS_DS **mis_ds_ptr_tbl)
  {
ST_INT i;
ST_BOOLEAN found;
ST_CHAR *ds_name;
ST_INT c;

  found = SD_FALSE;
  for (i = 0; i < max_ds; ++i)
    {
    /* skip over and ignore NULL elements in this table                 */
    if (mis_ds_ptr_tbl[i] != NULL)
      {
      ds_name = mis_ds_ptr_tbl[i]->ds_name;
      c = strcmp (ds_name, ca_name);
      if (c == 0)
        {
        ++i;		/* Found it, point to next name */
        found = SD_TRUE;
        break;
        }
      if (c > 0)		/* Whoops went past, stop here */
        {
        found = SD_TRUE;
        break;
        }
      } /* end-if */
    }

  if (found == SD_FALSE)
    return (0);
  else
    return (i);
  }

/************************************************************************/
/************************************************************************/
/*			_mi_gnl_ind_doms 				*/
/* This is not part of TASE.2 spec					*/
/************************************************************************/

ST_INT _mi_gnl_ind_doms (MVL_IND_PEND *mvl_ind_ctrl, 
			     NAMELIST_REQ_INFO *req_info, 
			     ST_CHAR **ptr, ST_BOOLEAN *more_follows_out, 
			     ST_INT max_names)
  {
MIS_VCC *mis_vcc;

  mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI GNL, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl,7,2);
    return (0);
    }

  if (req_info->cont_after_pres)
    return (0);

  ptr[0] = mis_vcc->icc_name;
  *more_follows_out = SD_FALSE;
  return (1);
  }

/************************************************************************/
/************************************************************************/

ST_VOID _mis_process_getvaa_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
GETVAR_REQ_INFO  *req_info;
GETVAR_RESP_INFO  resp_info;
MIS_DV_REF mis_dv_ref;
MIS_VCC *mis_vcc;
MIS_DV *mis_dv;
ST_INT mi_type;
ST_INT type_id;
ST_RET rc;
#if defined(MVL_DYN_ASN1_TYPES)
ST_UCHAR *asn1_buf;
ST_INT asn1_len;
ST_UCHAR *asn1_start;
RUNTIME_TYPE *rt;
ASN1_ENC_CTXT localEncCtx;	/* For readability, use "aCtx" to access this.*/
ASN1_ENC_CTXT *aCtx = &localEncCtx;
ST_INT rt_num;
#endif

/* See if we are using ICCP on this connection */
  if (mvl_ind_ctrl->event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_getvaa_ind (mvl_ind_ctrl);
    return;
    }


  mis_vcc = (struct _mis_vcc *)mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI GetVaa, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    return;
    }
  req_info = (struct getvar_req_info *)mvl_ind_ctrl->event->u.mms.dec_rslt.data_ptr;

#if defined(MVL_DYN_ASN1_TYPES)
  asn1_buf = (unsigned char *)M_MALLOC (MSMEM_ASN1_DATA_ENC, mvl_cfg_info->max_msg_size);
#endif

  
  rc = SD_FAILURE;

  if (req_info->req_tag == 0)  /* Object Name */
    {

    rc = mis_objname_to_dv (mis_vcc, &req_info->name, 
			  &mis_dv, &mis_dv_ref, &mi_type);

    if (rc == SD_SUCCESS)
      {
      resp_info.mms_deletable = SD_FALSE;
      resp_info.address_pres = SD_FALSE;

      if (mi_type > MI_LAST_DV_TYPEID)
        mi_type = _mis_get_special_var_type_id (req_info->name.obj_name.vmd_spec);

      type_id = _mi_get_type_id (mi_type);

#if !defined(MVL_DYN_ASN1_TYPES)
  /* Here we assume that the ASN.1 is available and we can just return	*/
  /* it. This is good for systems with few types.			*/
      resp_info.type_spec.len  = mvl_type_ctrl[type_id].asn1_len;
      resp_info.type_spec.data = mvl_type_ctrl[type_id].asn1;
      rc = SD_SUCCESS;
#else
  /* We will derive the ASN.1 directly from the runtime type.		*/
  /* This takes a bit more code, but if we have Lot's of types we can	*/
  /* save space.							*/

      rt = (RUNTIME_TYPE *) mvl_type_ctrl[type_id].rt;
      rt_num = mvl_type_ctrl[type_id].num_rt;

      asn1r_strt_asn1_bld (aCtx, asn1_buf, mvl_cfg_info->max_msg_size);
      rc = ms_runtime_to_asn1 (aCtx, rt, rt_num);
      if (rc == SD_SUCCESS)
        {
        asn1_start = aCtx->asn1r_field_ptr+1;
        asn1_len = (asn1_buf + mvl_cfg_info->max_msg_size) - asn1_start;

        resp_info.type_spec.len  = asn1_len;
        resp_info.type_spec.data = asn1_start;
        }
      else
        {
        MI_LOG_NERR0 ("GetVarAccAttrib : Could not create ASN.1 from RUNTIME");
        }
#endif
      }
    else
      {
      MI_LOG_NERR1 ("GetVarAccAttrib : Data Value '%s' not found", 
			req_info->name.obj_name.vmd_spec);
      }
    } /* End 'if object (variable name) */

  if (rc != SD_SUCCESS)
    _mplas_err_resp (mvl_ind_ctrl,7,2);	/* resource, other	*/
  else
    {
    mvl_ind_ctrl->u.getvar.resp_info = &resp_info;
    mplas_getvar_resp (mvl_ind_ctrl);
    }

#if defined(MVL_DYN_ASN1_TYPES)
  M_FREE (MSMEM_ASN1_DATA_ENC, asn1_buf);
#endif
  }


/************************************************************************/
/************************************************************************/
/*			_mi_process_getvla_ind				*/
/************************************************************************/

ST_VOID _mis_process_getvla_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
GETVLIST_REQ_INFO  *req_info;
GETVLIST_RESP_INFO  *resp_info;
ST_INT num_var;
ST_INT i;
OBJECT_NAME *vl_name;
VARIABLE_LIST *vl;
MIS_VCC *mis_vcc;
MIS_DS *mis_ds;
MIS_DV_REF dv_ref;
MIS_DV_REF dv_index;
MIS_DV *mis_dv;
ST_INT rc;

/* See if we are using ICCP on this connection */
  if (mvl_ind_ctrl->event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_getvla_ind (mvl_ind_ctrl);
    return;
    }

  mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI GetVLA, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    return;
    }
  req_info = (struct getvlist_req_info *)mvl_ind_ctrl->event->u.mms.dec_rslt.data_ptr;

/* Find the dataset */
  vl_name = &req_info->vl_name;
  rc = mis_objname_to_ds (mis_vcc, vl_name, &mis_ds); 
  if (rc == SD_SUCCESS)
    {
    num_var = mis_ds->num_dv_ref;
    resp_info = (GETVLIST_RESP_INFO *) M_CALLOC (MSMEM_ENC_INFO, 1, 
		             (num_var * sizeof(VARIABLE_LIST)));
    resp_info->mms_deletable = mis_ds->mms_deletable;
    resp_info->num_of_variables = num_var;
    vl = (VARIABLE_LIST *) (resp_info + 1);
    for (i = 0; i < num_var; ++i, ++vl)
      {
      dv_ref = mis_ds->mis_dv_ref_tbl[i];
      dv_index = dv_ref & MIS_INDEX_MASK;
      mis_dv = &mis_dv_ctrl[dv_index];

      vl->alt_access_pres = SD_FALSE;
      vl->var_spec.var_spec_tag = VA_SPEC_NAMED;

     /* We know this is a union so the following line is OK ... */
      vl->var_spec.vs.name.obj_name.vmd_spec = mis_dv->dv_name;
      if (dv_ref & MIS_DVREF_ICC)
        {
        vl->var_spec.vs.name.object_tag = ICC_SPEC;
        vl->var_spec.vs.name.domain_id = mis_vcc->icc_name;
        }
      else
        vl->var_spec.vs.name.object_tag = VCC_SPEC;
      }
    mvl_ind_ctrl->u.getvlist.resp_info = resp_info;

    mplas_getvlist_resp (mvl_ind_ctrl);
    M_FREE (MSMEM_ENC_INFO, resp_info);
    }
  else
    {  
    MI_LOG_NERR1 ("Get NVL Attrib: could not find NVL '%s'", 
				vl_name->obj_name.vmd_spec);
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_NONEXISTENT);
    return;
    }
  }

/************************************************************************/
/************************************************************************/
/*			_mi_process_defvlist_ind				*/
/************************************************************************/

ST_VOID _mis_process_defvl_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
ST_INT i;
VARIABLE_LIST *vl;
VARIABLE_SPEC *vs;
DEFVLIST_REQ_INFO *req_info;
MIS_DS *mis_ds;
MIS_VCC *mis_vcc;
ST_INT num_var;
MIS_DV *mis_dv;
MIS_DV_REF mis_dv_ref;
ST_INT mi_type;
ST_CHAR *dv_name;
ST_INT scope;
ST_RET rc;
ST_BOOLEAN ds_found;

/* See if we are using ICCP on this connection */
  if (mvl_ind_ctrl->event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_defvlist_ind (mvl_ind_ctrl);
    return;
    }

  mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI DefVL, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    return;
    }
  req_info = (struct defvlist_req_info *)mvl_ind_ctrl->event->u.mms.dec_rslt.data_ptr;
  num_var = req_info->num_of_variables;
  if (num_var <= 0)
    {
    _mplas_err_resp (mvl_ind_ctrl,4,0);	/* Service error , other	*/
    return;
    }

  /* Tell the user all about it */
  rc = u_mis_create_ds_ind (mis_vcc, req_info);

  /* make sure the ds scope is ICC    */
  if (req_info->vl_name.object_tag != ICC_SPEC)
    { 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_DEFINITION, 
		     MMS_ECODE_OBJ_ATTR_INCONSISTENT);
    return;
    }

  /* make sure the ds does not already exist  */
  i = 0;
  ds_found = SD_FALSE;
  while ((i < mis_vcc->max_icc_ds) && !ds_found)
    {
    mis_ds = mis_vcc->icc_ds_ctrl_tbl[i];
    if (mis_ds && (strcmp (mis_ds->ds_name, req_info->vl_name.obj_name.item_id) == 0))
      ds_found = SD_TRUE;
    else
      i++;
    }

  if (ds_found)
    { 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_DEFINITION, 
		     MMS_ECODE_OBJ_EXISTS);
    return;
    }

  /* Go ahead and create the DataSet */
  mis_ds = mis_add_ds_to_vcc (mis_vcc, req_info->vl_name.object_tag,
			      req_info->vl_name.obj_name.vmd_spec,
			      num_var);
  if (mis_ds == NULL)
    {
    MI_LOG_NERR0 ("MI DefVL, could not create MIS_DS"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_RESOURCE, MMS_ECODE_MEM_UNAVAIL);
    return;
    }

  vl = (VARIABLE_LIST *) (req_info + 1);
  for (i = 0;  i < num_var;  ++i, ++vl)
    {
    vs = &vl->var_spec;
    if (vs->var_spec_tag != VA_SPEC_NAMED)
      {
      MI_LOG_NERR0 ("MI DefVL, var spec != named"); 
      _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_DEFINITION, 
		       MMS_ECODE_OBJ_UNDEFINED);
      return;
      }
    dv_name = vs->vs.name.obj_name.vmd_spec;
    scope = vs->vs.name.object_tag;
    if (scope == ICC_SPEC)
      {
      if (strcmp (vs->vs.name.domain_id, mis_vcc->icc_name) != 0)
        {
        MI_LOG_NERR0 ("MIS DefDS DV domain mismatch");
        _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_DEFINITION, 
			 MMS_ECODE_OBJ_UNDEFINED);
        return;
        }
      }
    rc = mis_find_vcc_dv (mis_vcc, dv_name, scope, 	      
		     &mis_dv, &mis_dv_ref, &mi_type);

    if (rc != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("MIS DefDS DV '%s' not found", dv_name);
      mis_delete_vcc_ds (mis_vcc, mis_ds, req_info->vl_name.object_tag);
      _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_DEFINITION, 
		       MMS_ECODE_OBJ_UNDEFINED);
      return;
      }
    mis_add_ds_dv (mis_vcc, mis_ds, mis_dv_ref);
    }

  mis_ds->mms_deletable = SD_TRUE;	/* MMS spec requires this*/
  mplas_defvlist_resp (mvl_ind_ctrl);
  }


/************************************************************************/
/************************************************************************/
/*			_mis_process_delvlist_ind				*/
/************************************************************************/

ST_VOID _mis_process_delvlist_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
ST_INT i;
DELVLIST_REQ_INFO *req_info;
DELVLIST_RESP_INFO resp_info;
MIS_DS *mis_ds;
MIS_VCC *mis_vcc;
ST_INT scope;
ST_RET rc;
OBJECT_NAME *vl_name;
MIS_DSTS *mis_dsts;
ST_BOOLEAN dsts_active;

/* See if we are using ICCP on this connection */
  if (mvl_ind_ctrl->event->net_info->mi_in_use == SD_FALSE)
    {
    _mvl_process_delvlist_ind (mvl_ind_ctrl);
    return;
    }

  mis_vcc = mvl_ind_ctrl->event->net_info->mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("MI DelDataSet, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    return;
    }
  req_info = (struct delvlist_req_info *)mvl_ind_ctrl->event->u.mms.dec_rslt.data_ptr;

  /* Tell the user all about it */
  rc = u_mis_delete_ds_ind (mis_vcc, req_info);

  resp_info.num_matched = 0;
  resp_info.num_deleted = 0;			/* Init count.		*/
  if (req_info->scope == DELVL_SPEC)
    {
    vl_name = (OBJECT_NAME *) (req_info + 1);
    for (i = 0; i < req_info->num_of_vnames; ++i, ++vl_name)
      {
      /* check for icc scope	      */
      scope = vl_name->object_tag;
      if (scope != ICC_SPEC)
        {
        MI_LOG_NERR1 ("Delete DataSet, Invalid Scope for '%s'", 
				vl_name->obj_name.vmd_spec);
        _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_DEFINITION, 
			 MMS_ECODE_OBJ_ATTR_INCONSISTENT);
	return;
        }

      /* Go ahead and find the DataSet */
      rc = mis_objname_to_ds (mis_vcc, vl_name, &mis_ds); 
      if (rc == SD_SUCCESS)
        {
	/* be sure data set is not part of active dsts	*/
	mis_dsts = mis_vcc->mis_dsts_tbl;
	dsts_active = SD_FALSE;
	for (i = 0; i < mis_vcc->num_dsts_ctrl; ++i, ++mis_dsts)
	  {
	  if ((mis_dsts->dsts_flags & MIS_DSTS_FLAGS_IN_USE) &&
	      (mis_dsts->mi_dsts_data.Status) &&
              (mis_dsts->mis_ds == mis_ds))
	    {
	    dsts_active = SD_TRUE;
	    break;
	    }
	  }

	if (dsts_active)
          {
          MI_LOG_NERR1 ("Unable to Delete DataSet '%s' - referenced by active DsTs", 
  				vl_name->obj_name.vmd_spec);
          _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
			   MMS_ECODE_OBJ_ACCESS_UNSUPPORT);
	  return;
	  }
	    
	    
	mis_delete_vcc_ds (mis_vcc, mis_ds, scope);
        ++resp_info.num_matched;
        ++resp_info.num_deleted;
        }
      else
        {
        MI_LOG_FLOW1 ("Delete NVL, could not find MIS_DS '%s'", 
				vl_name->obj_name.vmd_spec);
        _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_DEFINITION, 
			 MMS_ECODE_OBJ_UNDEFINED);
	return;
        }
      }
    }
  else if (req_info->scope == DELVL_DOM)    /* this option is not part of TASE.2 spec */
    {
    for (i = 0; i < mis_vcc->max_icc_ds; ++i)
      {
      mis_ds = mis_vcc->icc_ds_ctrl_tbl[i];
      if (mis_ds != NULL)
        {
        mis_delete_vcc_ds (mis_vcc, mis_ds, ICC_SPEC);
        ++resp_info.num_matched;
        ++resp_info.num_deleted;
        }
      }
    }
  else
    {
    MI_LOG_NERR0 ("MI DelVL, no MIS_VCC"); 
    _mplas_err_resp (mvl_ind_ctrl, MMS_ECLASS_ACCESS, 
		     MMS_ECODE_OBJ_ACCESS_DENIED);
    return;
    }

  mvl_ind_ctrl->u.delvlist.resp_info = &resp_info;
  mplas_delvlist_resp (mvl_ind_ctrl);
  }


/************************************************************************/
/*			mis_reset_conn_dsts 				*/
/************************************************************************/

ST_VOID _mis_reset_conn_dsts (MI_CONN *mi_conn)
  {
MIS_VCC *mis_vcc;
MIS_DSTS *mis_dsts;
ST_INT i;

  mis_vcc = mi_conn->mi_remote->mis_vcc;
  if (mis_vcc == NULL)
    return;

  mis_dsts = mis_vcc->mis_dsts_tbl;
  for (i = 0; i < mis_vcc->num_dsts_ctrl; ++i, ++mis_dsts)
    {
    if (mis_dsts->mi_conn == mi_conn)
      {
      if (mis_dsts->dsts_flags & MIS_DSTS_FLAGS_IN_USE)
        {
        mis_dsts->dsts_flags &= ~MIS_DSTS_FLAGS_IN_USE;
        mis_dsts->mi_dsts_data.Status = 0;
        u_mis_dsts_activity (mis_vcc, mis_dsts, MIS_DSTS_STOP_TRANSFER, 0);
        }
      }
    }
  }

/************************************************************************/
/************************************************************************/
/*			mis_clean_assoc 				*/
/************************************************************************/

ST_VOID mis_clean_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
MIS_VCC *mis_vcc;
MIS_DEVICE_REF device_ref;
MIS_DEVICE *mis_device;
ST_INT i, device_index;

  mis_vcc = mi_assoc->mi_remote->mis_vcc;


/* according to the spec - don't ditch data sets on linkdown	*/
/* just disable any transfer sets				*/
/* and reset armed devices					*/
  
  for (i = 0; i < mis_vcc->num_vcc_device_ref; ++i)
    {
    device_ref = mis_vcc->vcc_device_ref_tbl[i];
    device_index = device_ref & MIS_INDEX_MASK;
    mis_device = &mis_device_ctrl[device_index];
    if (mis_device->device_state == MIS_DEVICE_STATE_ARMED)
      mis_device->device_state = MIS_DEVICE_STATE_IDLE;
    }
  }

/************************************************************************/
/*		             u_mvl_concl_ind				*/
/************************************************************************/

ST_VOID u_mvl_concl_ind (MVL_COMM_EVENT *event)
  {
  /* This function should do all appropriate cleanup before sending the	*/
  /* Conclude response.							*/
  mplas_concl_resp (event);	/* Send response.	*/
  }
