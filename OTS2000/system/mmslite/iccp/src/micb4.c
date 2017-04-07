/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : micb4.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	Handle Block 4 Client Functionality				*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  RKR     07    Copyright year update                        */
/* 05/13/02  MDE     06	   Now free request control on send error	*/
/* 03/21/02  MDE     05	   Remote AR info moved to MI_ASSOC (logged) 	*/
/* 11/15/01  MDE     04	   Changed some mic_xxx function argument order	*/
/* 07/06/00  NAV     03    Add MISEM support				*/
/* 11/21/99  NAV     02    Support mi_op_err				*/
/* 10/04/99  NAV     01    Created					*/
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



static ST_VOID _mic_start_IMTransfer_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mic_stop_IMTransfer_done (MVL_REQ_PEND *mvl_req);

/************************************************************************/
/* mic_enable_IMTransfers: add block 4 support to the client vcc	*/
/************************************************************************/

ST_RET mic_enable_IMTransfers (MIC_VCC *vcc)
  {

  S_MI_LOCK_COMMON ();
  vcc->misv.Supported_Features[0] |= MI_BLOCK4;
  S_MI_UNLOCK_COMMON ();

  return (SD_SUCCESS);
  }


/************************************************************************/
/* mic_start_IMTransfer: enable IM Transfers at the remote server	*/
/************************************************************************/

MI_REQ_CTRL *mic_start_IMTransfer (MI_CONN *mi_conn)
  {
MVL_WRITE_REQ_INFO *mvl_write_info;
VARIABLE_SPEC *vs;
WRITE_REQ_INFO *write_info;
VARIABLE_LIST *vl;
ST_RET rc;
MVL_REQ_PEND *mvl_req;
ST_INT num_var = 1;
ST_BOOLEAN bEnabled = SD_TRUE;
MI_REQ_CTRL *mi_req;

  S_MI_LOCK_COMMON ();

  /* Create a write request info struct  */
  write_info = (WRITE_REQ_INFO *) chk_calloc (1, sizeof (WRITE_REQ_INFO) +
				(num_var * sizeof (VARIABLE_LIST)) +
				(num_var * sizeof (VAR_ACC_DATA)));

  write_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
  write_info->va_spec.num_of_variables = num_var;

  mvl_write_info = (MVL_WRITE_REQ_INFO *) chk_calloc (num_var,
		    sizeof (MVL_WRITE_REQ_INFO));

  vl = (VARIABLE_LIST *) (write_info + 1);
  vs = &vl->var_spec;
  vs->var_spec_tag = VA_SPEC_NAMED;
  vs->vs.name.object_tag = AA_SPEC;
  vs->vs.name.obj_name.aa_spec = MI_IM_TRAN_SET_VAR_NAME;
  mvl_write_info->type_id = _mi_get_block4_type_id (MI_TYPEID_IMTRANSET);
  mvl_write_info->local_data = &bEnabled;

  /* send write request and do not wait for reply */
  rc = mvla_write_variables (mi_conn->net_info, write_info, num_var,
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

  mvl_req->v = mi_conn;
  mvl_req->u_req_done = _mic_start_IMTransfer_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);

  /* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/* _mic_start_IMTransfer_done: write request is done			*/
/************************************************************************/

static ST_VOID _mic_start_IMTransfer_done (MVL_REQ_PEND *mvl_req)
  {
MI_CONN *mi_conn;
MVL_WRITE_REQ_INFO *mvl_write_info;
MI_REQ_CTRL *mi_req;


  mvl_write_info = mvl_req->u.wr.w_info;
  mi_req = mvl_req->mi_req;

  mi_conn = (MI_CONN *) mvl_req->v;

  mi_conn->IM_Transfer_Set = (mvl_req->result == SD_SUCCESS) ? SD_TRUE : SD_FALSE;

  _mi_req_done (mi_req);
  chk_free (mvl_write_info); 
  }

/************************************************************************/
/* mic_stop_IMTransfer: disable IM Transfers at the remote server	*/
/************************************************************************/

MI_REQ_CTRL *mic_stop_IMTransfer (MI_CONN *mi_conn)
  {
MVL_WRITE_REQ_INFO *mvl_write_info;
VARIABLE_SPEC *vs;
WRITE_REQ_INFO *write_info;
VARIABLE_LIST *vl;
ST_RET rc;
MVL_REQ_PEND *mvl_req;
ST_INT num_var = 1;
ST_BOOLEAN bEnabled = SD_FALSE;
MI_REQ_CTRL *mi_req;

  S_MI_LOCK_COMMON ();
  /* Create a write request info struct  */
  write_info = (WRITE_REQ_INFO *) chk_calloc (1, sizeof (WRITE_REQ_INFO) +
				(num_var * sizeof (VARIABLE_LIST)) +
				(num_var * sizeof (VAR_ACC_DATA)));

  write_info->va_spec.var_acc_tag = VAR_ACC_VARLIST;
  write_info->va_spec.num_of_variables = num_var;

  mvl_write_info = (MVL_WRITE_REQ_INFO *) chk_calloc (num_var,
		    sizeof (MVL_WRITE_REQ_INFO));

  vl = (VARIABLE_LIST *) (write_info + 1);
  vs = &vl->var_spec;
  vs->var_spec_tag = VA_SPEC_NAMED;
  vs->vs.name.object_tag = AA_SPEC;
  vs->vs.name.obj_name.aa_spec = MI_IM_TRAN_SET_VAR_NAME;
  mvl_write_info->type_id = _mi_get_block4_type_id (MI_TYPEID_IMTRANSET);
  mvl_write_info->local_data = &bEnabled;

  /* send write request and do not wait for reply */
  rc = mvla_write_variables (mi_conn->net_info, write_info, num_var,
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

  mvl_req->v = mi_conn;
  mvl_req->u_req_done = _mic_stop_IMTransfer_done;
  mi_req = _mi_get_req_ctrl (mi_conn, mvl_req);

  /* All is OK, return success */
  S_MI_UNLOCK_COMMON ();
  return (mi_req);
  }

/************************************************************************/
/* _mic_stop_IMTransfer_done: write request is done			*/
/************************************************************************/

static ST_VOID _mic_stop_IMTransfer_done (MVL_REQ_PEND *mvl_req)
  {
MI_CONN *mi_conn;
MVL_WRITE_REQ_INFO *mvl_write_info;

  mvl_write_info = mvl_req->u.wr.w_info;

  mi_conn = (MI_CONN *) mvl_req->v;

  mi_conn->IM_Transfer_Set = (mvl_req->result == SD_SUCCESS) ? SD_FALSE : SD_TRUE;

  _mi_req_done (mvl_req->mi_req);
  chk_free (mvl_write_info);
 
  }

/************************************************************************/
/* _mic_process_info_message:						*/
/************************************************************************/

ST_VOID _mic_process_info_message (MVL_COMM_EVENT *event)
  {
VAR_ACC_SPEC *va_spec;
INFO_REQ_INFO *info_ptr;
VARIABLE_LIST *vl;
ACCESS_RESULT *ar_ptr;
RUNTIME_TYPE *rt, rt_fixed;
ST_INT rt_len;
MI_CONN *mi_conn;
MIC_VCC *mic_vcc;
ST_INT type_id;
ST_RET rc;
MI_INFO_BUF_HEADER buf_header;
ST_UCHAR *info_buf;
ST_INT scope;
char *var_name;
int buf_size = 0;

  info_ptr = (INFO_REQ_INFO *) event->u.mms.dec_rslt.data_ptr;
  va_spec = &info_ptr->va_spec;
  ar_ptr = info_ptr->acc_rslt_list;
  vl = (VARIABLE_LIST *) (info_ptr + 1);
  vl++;
  scope = vl->var_spec.vs.name.object_tag;

  /* Get the MI connection and associated VCC */
  mi_conn = event->net_info->mi_conn;
  mic_vcc = mi_conn->mi_remote->mic_vcc;
  if (mic_vcc == NULL)
    {
    MI_LOG_NERR0 ("Warning: Received info rpt ind on server only connection");
    _mvl_free_comm_event (event);	/* All done with this event	*/
    return;
    }

  /* first variable is "Info_Mess_Header" - decode it			*/
  if (ar_ptr->acc_rslt_tag == ACC_RSLT_SUCCESS)
    {
    type_id = _mi_get_block4_type_id (MI_TYPEID_INFO_HEADER);
    if (mvl_get_runtime (type_id, &rt, &rt_len) == SD_SUCCESS)
      {
      rc = ms_asn1_to_local (rt, rt_len, ar_ptr->va_data.data,
                             ar_ptr->va_data.len, (ST_CHAR *)&buf_header);

      if (rc != SD_SUCCESS)
        {
	/* if we can't decode - log it and discard it		*/
	MI_LOG_NERR1 ("Unable to decode Info_Mess_Header for remote %s", 
		       mi_conn->mi_assoc->remote_ar);
        _mvl_free_comm_event (event);	
	}
      }
    else
      {
      MI_LOG_NERR0 ("Incoming IMTransfer - unable to get runtime type");
      _mvl_free_comm_event (event);	
      }
    }
  else
    {
    MI_LOG_NERR1 ("AccessResult Failure in incoming IMTransfer for remote %s",
	           mi_conn->mi_assoc->remote_ar);
    _mvl_free_comm_event (event);	
    }
  ar_ptr++;

  /* Take care of the Info_Buff_XXX var					*/
  if (ar_ptr->acc_rslt_tag == ACC_RSLT_SUCCESS)
    {
    var_name = vl->var_spec.vs.name.obj_name.vmd_spec; 
    buf_size = atoi (var_name+10);

    type_id = _mi_get_block4_type_id (MI_TYPEID_INFO_BUFF);
    if (mvl_get_runtime (type_id, &rt, &rt_len) == SD_SUCCESS)
      {
      info_buf = (unsigned char *)chk_calloc (1, buf_size);
      /* lets take a few liberties with the RUNTIME_TYPE so the same type
         can handle any size buffer					*/
      memcpy (&rt_fixed, rt, sizeof (RUNTIME_TYPE));
      rt_fixed.el_size = (ST_INT16) buf_size;
      rt_fixed.offset_to_last = (ST_INT16) buf_size;
      rt_fixed.u.p.el_len = (ST_INT16) buf_size;
      rc = ms_asn1_to_local (&rt_fixed, rt_len, ar_ptr->va_data.data,
                             ar_ptr->va_data.len, (ST_CHAR *)info_buf);

      if (rc != SD_SUCCESS)
        {
	/* if we can't decode - log it and discard it		*/
	MI_LOG_NERR1 ("Unable to decode Info_Buff_XXX for remote %s", 
		       mi_conn->mi_assoc->remote_ar);
	}
      else
        u_mic_receive_IMTransfer (mi_conn, &buf_header, info_buf, scope);

      }
    else
      {
      MI_LOG_NERR0 ("Incoming IMTransfer - unable to get runtime type");
      }
    }
  else
    {
    MI_LOG_NERR1 ("AccessResult Failure in incoming IMTransfer for remote %s",
	           mi_conn->mi_assoc->remote_ar);
    }

  /* clean it up - your done						*/
  chk_free (info_buf);
  _mvl_free_comm_event (event);	/* All done with this event	*/
  }


