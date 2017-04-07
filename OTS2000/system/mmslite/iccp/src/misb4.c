/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : misb4.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*		Handle Block 4 for an ICCP Server			*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     08    Copyright year update                        */
/* 02/25/02  MDE     07    Now get max PDU size from mvl_cfg_info	*/
/* 01/17/02  ASK     06    Made changes for asn1r 			*/
/* 11/28/01  MDE     05	   No longer allocate mmsl_enc_buf		*/
/* 11/15/01  MDE     04	   Changed some mic_xxx function argument order	*/
/* 07/06/00  NAV     03	   Add MISEM support				*/
/* 04/19/00  RKR     02    SMEM changes					*/
/* 10/01/99  nav     01    Created					*/
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

ST_VOID _mis_start_stop_im_transfer_done (MIS_IM_IND_CTRL *im_ind_ctrl);

/************************************************************************/
/* _mis_start_stop_im_transfer:  respond to write indication enabling	*/
/*				 or disabling IMTransferSets for this	*/
/*				 connection				*/
/************************************************************************/

ST_VOID _mis_start_stop_im_transfer (MIS_IM_IND_CTRL *im_ind_ctrl,
				     VAR_ACC_DATA  *va_data)

  {
MVL_IND_PEND *mvl_ind_ctrl;
MI_CONN *mi_conn;			/* these guys are AA Specific!	*/
MIS_VCC *mis_vcc;
RUNTIME_TYPE 	*rt;
ST_INT		num_rt, type_id;
ST_RET rc;

  /* find the mi_conn *							*/
  mvl_ind_ctrl = im_ind_ctrl->mis_wr_ctrl->mvl_ind_ctrl;
  mi_conn = mvl_ind_ctrl->event->net_info->mi_conn;

  /* check access rights						*/
  mis_vcc = im_ind_ctrl->mis_wr_ctrl->mis_vcc;
  if (mis_vcc->misv.Supported_Features && MI_BLOCK4)
    {
    /* stash the data							*/
    type_id = _mi_get_block4_type_id (MI_TYPEID_IMTRANSET);
    rc = mvl_get_runtime (type_id, &rt, &num_rt);	  
    if (rc == SD_SUCCESS)
      {
      rc = ms_asn1_to_local (rt, num_rt, va_data->data, va_data->len, 
  			     (char *)im_ind_ctrl->data_buf);
  
      if (rc != SD_SUCCESS)
        {
        MI_LOG_NERR1 ("MI Write Ind - IM_Transfer_Set: ms_asn1_to_local failed, rc = %x", rc);
        im_ind_ctrl->acc_rslt_tag = WR_RSLT_FAILURE;
        im_ind_ctrl->failure = ARE_TYPE_INCONSISTENT;
        }
      else
        {
        MI_LOG_FLOW2 ("IM_Transfer_Set = %d for connection remote ar %s",
      		       mi_conn->IM_Transfer_Set, mi_conn->remote_ar);
	mi_conn->IM_Transfer_Set = im_ind_ctrl->enable_transfer;
	/* tell the user							*/
        if (mi_conn->IM_Transfer_Set)
          u_mis_start_IM_Transfer (mi_conn);
        else
          u_mis_stop_IM_Transfer (mi_conn);
        im_ind_ctrl->acc_rslt_tag = WR_RSLT_SUCCESS;
        }
      }
    else
      {
      MI_LOG_NERR1 ("MI Write Ind - IM_Transfer_Set: can't find type_id, rc = %x", rc);
      im_ind_ctrl->acc_rslt_tag = WR_RSLT_FAILURE;
      im_ind_ctrl->failure = ARE_TYPE_UNSUPPORTED;
      }
    }
  else
    {
    im_ind_ctrl->acc_rslt_tag = WR_RSLT_FAILURE;
    im_ind_ctrl->failure = ARE_OBJ_ACCESS_DENIED;
    }

  _mis_start_stop_im_transfer_done (im_ind_ctrl);
  }

/************************************************************************/
/* mis_enable_IMTransfers: do block 4 setup work for the local server	*/
/************************************************************************/

ST_RET mis_enable_IMTransfers (MIS_VCC *mis_vcc)
  {

  S_MI_LOCK_COMMON ();
  mis_vcc->misv.Supported_Features[0] |= MI_BLOCK4;
  S_MI_UNLOCK_COMMON ();

  return (SD_SUCCESS);
  }

/************************************************************************/
/* mis_send_IMTransfer: send information message			*/
/************************************************************************/

ST_RET mis_send_IMTransfer (ST_INT num_conn, MI_CONN **send_to, 
                            MI_INFO_BUF_HEADER *buf_header,
                            ST_INT buf_len, ST_UCHAR *info_buf, ST_INT msg_scope)
  {
ST_RET rc;
INFO_REQ_INFO *info_info;
ACCESS_RESULT *ar;
VARIABLE_LIST *vl;
ST_INT req_info_size;
ST_UCHAR *asn1_buf;
ST_UCHAR *asn1_start;
ST_INT asn1_len;
ST_INT asn1_buf_left;
ST_INT num_var_to_send = 2;
ST_INT i;
ST_INT est_rpt_size;
MI_CONN *mi_conn;
ST_CHAR tempName[33];
RUNTIME_TYPE *rt, rt_fixed;
ST_INT num_rt, type_id;
ASN1_ENC_CTXT localEncCtx;	/* For readability, use "aCtx" to access this.*/
ASN1_ENC_CTXT *aCtx = &localEncCtx;

  S_MI_LOCK_COMMON ();

  /* Calculate the required size for  the info report 			*/
  req_info_size = sizeof(INFO_REQ_INFO) + 
		  (num_var_to_send * sizeof(ACCESS_RESULT)) +
    		  (num_var_to_send * sizeof(VARIABLE_LIST));

  MI_LOG_FLOW1 ("IM SendMessage for %d connections.", num_conn);

  /* see if we can send all the reports					*/
  for (i=0; i<num_conn; i++)
    {
    mi_conn = send_to[i];

    /* verify that the ASN.1 will fit OK				*/
    est_rpt_size = _mis_calc_info_msg_size (buf_len, msg_scope, mi_conn);
    if (est_rpt_size > mi_conn->net_info->max_pdu_size)
      {
      MI_LOG_NERR2 ("IM SendMessage to %s Failed - Msg Too Large %d.",
      		     mi_conn->remote_ar, est_rpt_size);
      S_MI_UNLOCK_COMMON ();
      return SD_FAILURE;
      }

    /* verify IMTransfers are enabled					*/
    if (!mi_conn->IM_Transfer_Set)
      {
      MI_LOG_NERR1 ("IM SendMessage to %s Failed - IM Transfers Sets are disabled.",
      		     mi_conn->remote_ar);
      S_MI_UNLOCK_COMMON ();
      return SD_FAILURE;
      }
    }

  /* if we get this far we are good to go!				*/
  for (i=0; i<num_conn; i++)
    {
    info_info = (INFO_REQ_INFO *) M_CALLOC (MSMEM_ENC_INFO, 1, req_info_size);
    info_info->num_of_acc_result = num_var_to_send;

    info_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
    info_info->va_spec.num_of_variables = num_var_to_send;

    vl = (VARIABLE_LIST *) (info_info + 1);
    vl->var_spec.var_spec_tag = VA_SPEC_NAMED;

    /* set up the Info_Mess_Header variable				*/
    if (msg_scope == DOM_SPEC)
      {
      vl->var_spec.vs.name.obj_name.item_id = MI_IM_INFO_MESS_HEADER; 
      vl->var_spec.vs.name.object_tag = DOM_SPEC;
      vl->var_spec.vs.name.domain_id = mi_conn->mi_remote->mis_vcc->icc_name;
      }	 
    else
      {
      vl->var_spec.vs.name.obj_name.vmd_spec = MI_IM_INFO_MESS_HEADER; 
      vl->var_spec.vs.name.object_tag = VMD_SPEC;
      }

    ++vl;

    /* set up the Info_Buff_XXX variable				*/
    vl->var_spec.var_spec_tag = VA_SPEC_NAMED;
    sprintf (tempName, "%s%d", MI_IM_INFO_BUFF_NAME, buf_len);
    if (msg_scope == DOM_SPEC)
      {
      vl->var_spec.vs.name.obj_name.item_id = tempName;
      vl->var_spec.vs.name.object_tag = DOM_SPEC;
      vl->var_spec.vs.name.domain_id = mi_conn->mi_remote->mis_vcc->icc_name;
      }	 
    else
      {
      vl->var_spec.vs.name.obj_name.vmd_spec = tempName;
      vl->var_spec.vs.name.object_tag = VMD_SPEC;
      }

    info_info->acc_rslt_list = (ACCESS_RESULT *) (vl);

	asn1_buf = (unsigned char *)M_MALLOC (MSMEM_ASN1_DATA_ENC, mvl_cfg_info->max_msg_size);
    asn1_buf_left = mvl_cfg_info->max_msg_size;
    asn1_start = asn1_buf;

    /* Now encode the data, create the access result */
    info_info->acc_rslt_list = (ACCESS_RESULT *)
      (((VARIABLE_LIST *)(info_info + 1)) + num_var_to_send);
    ar = info_info->acc_rslt_list;

    type_id = _mi_get_block4_type_id (MI_TYPEID_INFO_HEADER);
    mvl_get_runtime (type_id, &rt, &num_rt);
    asn1r_strt_asn1_bld (aCtx, asn1_buf, asn1_buf_left);
    rc = ms_local_to_asn1 (aCtx, rt, num_rt, (ST_CHAR *)buf_header);
    if (rc == SD_SUCCESS)
      {
      asn1_start = aCtx->asn1r_field_ptr+1;
      asn1_len = (asn1_buf + asn1_buf_left) - asn1_start;
      memmove (asn1_buf,asn1_start,asn1_len);
      ar->acc_rslt_tag = ACC_RSLT_SUCCESS; 
      ar->va_data.len = asn1_len;
      ar->va_data.data = asn1_start;
      asn1_buf_left -= asn1_len;   /* adjust buffer control variables	*/
      asn1_start += asn1_len;
      }
    else
      {
      MI_LOG_NERR1 ("IM Send Message: data conversion error: 0x%04x", rc);
      ar->acc_rslt_tag = ACC_RSLT_FAILURE;
      ar->failure = ARE_TYPE_INCONSISTENT;
      }
     ++ar; 

    type_id = _mi_get_block4_type_id (MI_TYPEID_INFO_BUFF);
    mvl_get_runtime (type_id, &rt, &num_rt);

    /* lets take a few liberties with the RUNTIME_TYPE so the same type
       can handle any size buffer					*/
    memcpy (&rt_fixed, rt, sizeof (RUNTIME_TYPE));
    rt_fixed.el_size = (ST_INT16) buf_len;
    rt_fixed.offset_to_last = (ST_INT16) buf_len;
    rt_fixed.u.p.el_len = (ST_INT16) buf_len;
    asn1r_strt_asn1_bld (aCtx, asn1_buf, asn1_buf_left);
    rc = ms_local_to_asn1 (aCtx, &rt_fixed, num_rt, (char *)info_buf);
    if (rc == SD_SUCCESS)
      {
      asn1_start = aCtx->asn1r_field_ptr+1;
      asn1_len = (asn1_buf + asn1_buf_left) - asn1_start;
      memmove (asn1_buf,asn1_start,asn1_len);
      ar->acc_rslt_tag = ACC_RSLT_SUCCESS; 
      ar->va_data.len = asn1_len;
      ar->va_data.data = asn1_start;
      }
    else
      {
      MI_LOG_NERR1 ("IM Send Message: data conversion error: 0x%04x", rc);
      ar->acc_rslt_tag = ACC_RSLT_FAILURE;
      ar->failure = ARE_TYPE_INCONSISTENT;
      }

    /* just send the damn thing!					*/
    rc = mpl_info (info_info);
    if (rc != SD_SUCCESS)
      {
      MI_LOG_NERR1 ("IM SendMessage : mpl_info failed, ret = %x", rc);
      }
    else
      {  
      MI_LOG_FLOW1 ("IM Send Message - Actual info report size: %d", mmsl_msg_len);
      rc = _mvl_send_msg (mmsl_msg_start, mmsl_msg_len, 
			  mi_conn->net_info);
      M_FREE (MSMEM_ENC_INFO, info_info);
      M_FREE (MSMEM_ASN1_DATA_ENC, asn1_buf);
      if (rc != SD_SUCCESS)
        {
        MI_LOG_NERR1 ("IM Send Message: _mvl_send_msg failed, ret = %x", rc);
	break;
        }
      }
    }


  S_MI_UNLOCK_COMMON ();
  return (rc);
  }

/************************************************************************/
/* _mis_start_stop_im_tranfser_done: send the write response		*/
/************************************************************************/

ST_VOID _mis_start_stop_im_transfer_done (MIS_IM_IND_CTRL *im_ind_ctrl)
  {
MIS_WRITE_CTRL *mis_wr_ctrl;

  mis_wr_ctrl = im_ind_ctrl->mis_wr_ctrl;

  ++mis_wr_ctrl->num_data_done;
  if (mis_wr_ctrl->num_data_done == mis_wr_ctrl->num_var)
    _mis_send_write_resp (mis_wr_ctrl);

  }
 


