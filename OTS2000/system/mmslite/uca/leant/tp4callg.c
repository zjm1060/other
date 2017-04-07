#if defined (TP4_ENABLED)
/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	1996-2002 All Rights Reserved					*/
/*									*/
/* MODULE NAME : tp4callg.c						*/
/* PRODUCT(S)  : Lean-T Stack						*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	TP4 functions for handling of "Calling" side of connections.	*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	tp4_connect							*/
/*	tp4_disconnect							*/
/*	tp_process_cc							*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev     Comments					*/
/* --------  ---  ------   -------------------------------------------  */
/* 06/20/02  JRB    10     Chg to use new netAddr union in PRES_ADDR.	*/
/* 06/08/99  JRB    09     Chg to support 1024 connections.		*/
/* 08/13/98  JRB    08     Lint cleanup.				*/
/* 06/03/98  JRB    07     Increment loc_ref_offset before encoding CR	*/
/*			   to avoid sending duplicate CR.		*/
/* 12/04/97  JRB    06     Don't chk TSAPs on CC (may not be sent).	*/
/* 09/25/97  JRB    05     Don't generate exception for invalid state.	*/
/* 08/26/97  JRB    04     RFC1006 changes:				*/
/*			   tp4_* functs call tp0_* if RFC1006 conn.	*/
/*			   If !TP4_ENABLED, disable whole module.	*/
/* 08/14/97  JRB    03     Save "negotiated" TPDU len in tp_conn.	*/
/* 07/28/97  JRB    02     Del tp4_reldata funct. Use new dummy macro.	*/
/* 07/03/97  MDE    01     Eliminated unused local variables		*/
/* 05/27/97  JRB  7.00     MMSEASE 7.0 release.				*/
/* 03/26/97  JRB    05     tp4_connect don't call tp4_disconnect_ind	*/
/*			   on error (just return SD_FAILURE).		*/
/* 03/21/97  JRB    04     Add tp_free_spdu_all call to tp4_disconnect.	*/
/* 10/04/96  JRB    03     Use max_tpdu_len_enc (binary encoded value).	*/
/* 09/09/96  JRB    02     Add "Calling" support.			*/
/* 07/17/96  JRB    01     Created					*/
/************************************************************************/
static char *thisFileName = __FILE__;	/* For TP4_EXCEPT and slog macros*/

#include "glbtypes.h"
#include "sysincs.h"
#include "mem_chk.h"
#include "tp4api.h"		/* User definitions for tp4	*/
#include "tp4.h"		/* Internal definitions for tp4	*/
#include "tp4_encd.h"		/* Encode/decode defs for tp4	*/
#include "tp4_log.h"

/************************************************************************/
/*			tp4_connect					*/
/* This function is called by the user to initiate a connect request to	*/
/* a remote node							*/
/************************************************************************/

ST_RET tp4_connect (ST_LONG tp4_bind_id, ST_LONG user_conn_id,
		     TP4_ADDR *rem_tp4_addr,
		     ST_INT conndata_len,
		     char *conndata)
  {
ST_UINT16 conn_id;		/* index into "tp_conn_arr" array.		*/
TP_CONN *tp_conn;
TPDU_CX *tpdu_cr;

  if (tp4_bind_id != only_tp4_bind_id)
    {
#if defined (TP0_ENABLED)
    return (tp0_connect (tp4_bind_id, user_conn_id, rem_tp4_addr,
            conndata_len, conndata));
#else
    TP4_EXCEPT ();	/* If this happens, user code is invalid.	*/
    return (SD_FAILURE);
#endif
    }

  if (rem_tp4_addr->tp_type != TP_TYPE_TP4)
    {
    TP_LOG_ERR0 ("TP-ERROR: tp4_connect: invalid transport type");
    return (SD_FAILURE);
    }

  if (rem_tp4_addr->tp4_sel_len > MAX_TSEL_LEN  ||
      rem_tp4_addr->net_addr_len > CLNP_MAX_LEN_NSAP )
    {
    TP4_EXCEPT ();	/* If this happens, user code is invalid.	*/
    return (SD_FAILURE);
    }

  /* Look for available connection.	*/
  tp_conn = &tp_conn_arr [0];
  for (conn_id = 0;  conn_id < tp_cfg.max_num_conns;  conn_id++, tp_conn++)
    {
    if (tp_conn->state == TP_STATE_CLOSED)
      break;		/* Found one!!  "tp_conn" points to it now.	*/
    }	/* end "for"	*/
  if (conn_id >= tp_cfg.max_num_conns)
    {			/* Didn't find one!!	*/
    TP_LOG_ERR0 ("TP-ERROR: tp4_connect: all connections in use");
    return (SD_FAILURE);
    }
  /* "tp_conn" now points to a connection in TP_STATE_CLOSED state.	*/    
  tp_conn_clean (tp_conn);	/* Good time to clean up conn struct.	*/

  /* Increment "loc_ref_offset" to avoid duplicate CR.			*/
  inc_loc_ref_offset (tp_conn->loc_ref_offset);

  /* Save "rem_nsap" in "tp_conn" struct. Don't know "rem_mac" yet.	*/
  tp_conn->rem_nsap [0] = (ST_UCHAR) rem_tp4_addr->net_addr_len;
  memcpy (&tp_conn->rem_nsap [1], rem_tp4_addr->netAddr.nsap, rem_tp4_addr->net_addr_len);

  /* Set "tpdu_cr" pointing to "tp_conn->tpdu_cx" and fill in.		*/
  /* Info stored in "tp_conn" in case retransmissions needed.		*/
  tpdu_cr = &tp_conn->tpdu_cx;
  memcpy (tpdu_cr->loc_tsap, only_loc_tsap, only_loc_tsap [0]+1);
  tpdu_cr->rem_tsap [0] = (ST_UCHAR) rem_tp4_addr->tp4_sel_len;
  memcpy (&tpdu_cr->rem_tsap [1], rem_tp4_addr->tp4_sel, rem_tp4_addr->tp4_sel_len);
  tpdu_cr->loc_ref   = calc_loc_ref (conn_id, tp_conn->loc_ref_offset);
  tpdu_cr->rem_ref   = 0;
  tpdu_cr->max_tpdu_len_enc = tp_cfg.max_tpdu_len_enc;
  tpdu_cr->preferred_class = 4;	
  tpdu_cr->cdt       = tp_cfg.loc_cdt;
  /*tpdu_cr->rem_mac = Don't know yet!	*/
  tpdu_cr->rem_nsap  = tp_conn->rem_nsap;	/* ptr to addr in struct*/
  tpdu_cr->udata_len = min ((ST_UINT16) conndata_len, TP_MAX_CONN_UDATA);
  memcpy (tpdu_cr->udata_buf, conndata, tpdu_cr->udata_len);

  tp_send_cr (tpdu_cr);
  tp_conn->user_conn_id = user_conn_id;	/* Save User's ID for confirm.	*/
  tp_conn->retrans_time = tp_cfg.retrans_time;	/* Reset timer	*/
  tp_conn->num_trans = 1;	/* Reset & Inc # of transmissions	*/
  tp_conn->state = TP_STATE_WFCC;

  return (SD_SUCCESS);
  }


/************************************************************************/
/*			tp4_disconnect					*/
/* This function is called by the user to break connection		*/
/************************************************************************/

ST_RET tp4_disconnect (ST_LONG tp4_conn_id,
		        ST_INT conndata_len,
		        char *conndata)
  {
TP_CONN *tp_conn;	/* ptr to struct in "tp_conn_arr" array.	*/
TPDU_DR tpdu_dr;

#if defined (TP0_ENABLED)
  if (tp4_conn_id >= MIN_TP0_CONN_ID)
    return (tp0_disconnect (tp4_conn_id, conndata_len, conndata));
#endif

  if ((ST_ULONG) tp4_conn_id >= tp_cfg.max_num_conns)
    return (TP4E_BADCONN);

  tp_conn = &tp_conn_arr [tp4_conn_id];

  tp_free_spdu_all (tp_conn);	/* Free all SPDUs allocated for this conn*/

  /* Fill in TPDU_DR struct in case DR needs to be sent.		*/
  tpdu_dr.loc_ref = calc_loc_ref ((ST_UINT16)tp4_conn_id, tp_conn->loc_ref_offset);
  tpdu_dr.rem_ref = tp_conn->rem_ref;
  tpdu_dr.rem_nsap = tp_conn->rem_nsap;
  tpdu_dr.rem_mac = tp_conn->rem_mac;
  tpdu_dr.reason = TP_DR_NORMAL;

  switch (tp_conn->state)
    {
    case TP_STATE_OPEN:
      tp_conn->window_time = 0;		/* Stop window timer.		*/
      tp_conn->inact_time = 0;		/* Stop inactivity timer	*/
      /* DO NOT "break".  Fall through to WFCC/AKWAIT code.		*/

    case TP_STATE_WFCC:
    case TP_STATE_AKWAIT:
      tp_conn->num_trans = 1;	/* Reset & Inc # of transmissions	*/
      tp_conn->retrans_time = tp_cfg.retrans_time;	/* Reset timer	*/
      tp_send_dr (&tpdu_dr);
      tp_conn->state = TP_STATE_CLOSING;
      break;

    case TP_STATE_WFTRESP:
      /* Overwrite assumed "loc_ref". Not valid in this state.	*/
      tpdu_dr.loc_ref = 0;
      tp_send_dr (&tpdu_dr);
      tp_conn->state = TP_STATE_CLOSED;
      break;		/* Do nothing	*/

    default:
      TP_LOG_ERR1 ("TP-ERROR: invalid state (%d) for sending DR. Not sent.",
        tp_conn->state);
      return (SD_FAILURE);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/* tp_process_cc							*/
/************************************************************************/
ST_VOID tp_process_cc (TPDU_CX *tpdu_cc)
  {
ST_UINT16 conn_id;		/* index into "tp_conn_arr" array.		*/
TP_CONN *tp_conn;	/* ptr to struct in "tp_conn_arr" array.	*/
TPDU_DR tpdu_dr;
ST_INT unacceptable_cc = SD_FALSE;
ST_INT reason;


  /* Fill in TPDU_DR struct in case DR needs to be sent.		*/
  /* Do not fill in "reason" yet.					*/
  tpdu_dr.loc_ref = tpdu_cc->loc_ref;
  tpdu_dr.rem_ref = tpdu_cc->rem_ref;
  tpdu_dr.rem_nsap = tpdu_cc->rem_nsap;
  tpdu_dr.rem_mac = tpdu_cc->rem_mac;

  /* Find the correct "tp_conn" by looking for matching "loc_ref"	*/
  tp_conn = &tp_conn_arr [0];
  for (conn_id = 0;  conn_id < tp_cfg.max_num_conns;  conn_id++, tp_conn++)
    {
    if (tpdu_cc->loc_ref == calc_loc_ref (conn_id, tp_conn->loc_ref_offset))
      break;
    }		/* end "for" loop		*/
  if (conn_id >= tp_cfg.max_num_conns)
    {			/* Didn't find one!!	*/
    /* Treat as if we're in CLOSED state and just send DR.	*/
    TP_LOG_ERR0 ("TP-ERROR: Invalid CC received");
    tpdu_dr.reason = TP_DR_INVALID_REF;
    tp_send_dr (&tpdu_dr);
    return;
    }
  /* "tp_conn" now points to a connection with matching "loc_ref".	*/

  switch (tp_conn->state)
    {
    case TP_STATE_CLOSED:
      tpdu_dr.reason = TP_DR_INVALID_REF;
      tp_send_dr (&tpdu_dr);
      break;
    case TP_STATE_WFCC:
      /* Check for Transport Class 4.  Don't allow any other.		*/
      if (tpdu_cc->preferred_class != 4)
        {
        tpdu_dr.reason = TP_DR_CLASS_UNSUPPORTED;
        unacceptable_cc = SD_TRUE;
        }
      /* Save negotiated TPDU len in tp_conn->max_tpdu_len.	*/
      switch (tpdu_cc->max_tpdu_len_enc)
        {
        case TP_PDU_MAX_SIZE_128  :	tp_conn->max_tpdu_len =   128; break;
        case TP_PDU_MAX_SIZE_256  :	tp_conn->max_tpdu_len =   256; break;
        case TP_PDU_MAX_SIZE_512  :	tp_conn->max_tpdu_len =   512; break;
        case TP_PDU_MAX_SIZE_1024 :	tp_conn->max_tpdu_len =  1024; break;
        case TP_PDU_MAX_SIZE_2048 :	tp_conn->max_tpdu_len =  2048; break;
        case TP_PDU_MAX_SIZE_4096 :	tp_conn->max_tpdu_len =  4096; break;
        case TP_PDU_MAX_SIZE_8192 :	tp_conn->max_tpdu_len =  8192; break;
        /* Default to ILLEGAL value (i.e. 0)	*/
        default:			tp_conn->max_tpdu_len =     0; break;
        }	/* end "switch"	*/
      /* If max_tpdu_len ILLEGAL or ">" proposed, this is protocol error.*/
      if (tp_conn->max_tpdu_len == 0  ||		/* ILLEGAL	*/
          tp_conn->max_tpdu_len > tp_cfg.max_tpdu_len)	/* ">" proposed	*/
        {
        tpdu_dr.reason = TP_DR_PROT_ERROR;
        unacceptable_cc = SD_TRUE;
        }

      if (unacceptable_cc)
        {
        tp_conn->retrans_time = tp_cfg.retrans_time;	/* Reset timer	*/
        tp_conn->num_trans = 1;	/* Reset & Inc # of transmissions	*/
        tp_send_dr (&tpdu_dr);
        reason = 0xff;			/* Local user error	*/
        tp4_disconnect_ind (tp_conn->user_conn_id, reason,
                            0, NULL);	/* No user data passed up.	*/
        tp_conn->state = TP_STATE_CLOSING;
        return;
        }

      /* THIS IS AN ACCEPTABLE ASN1_CLASS_METHOD 4 CC TPDU.				*/

      /* Save received CDT.						*/
      /* Update adj_rem_cdt based on tpdu_cc->cdt. Don't allow greater	*/
      /* than max_rem_cdt, and don't allow less than current value.	*/
      if (tp_conn->adj_rem_cdt < tpdu_cc->cdt )
        tp_conn->adj_rem_cdt = tpdu_cc->cdt;	/* Increase if less than rcvd	*/
      if (tp_conn->adj_rem_cdt > tp_cfg.max_rem_cdt)
        tp_conn->adj_rem_cdt = tp_cfg.max_rem_cdt;	/* Reduce if greater than max	*/

      /* Save "rem_ref" and "rem_mac" in "tp_conn".			*/
      tp_conn->rem_ref = tpdu_cc->rem_ref;
      memcpy (tp_conn->rem_mac, tpdu_cc->rem_mac, CLNP_MAX_LEN_MAC);

      /* Set timers & # of transmissions.				*/
      tp_conn->num_trans = 0;		/* Reset # of transmissions	*/
      tp_conn->retrans_time = 0;	/* Stop timer	*/
      tp_conn->window_time = tp_cfg.window_time;	/* Reset timer	*/
      tp_conn->inact_time = tp_cfg.inact_time;		/* Reset timer	*/

      tp_conn->state = TP_STATE_OPEN;	/* DEBUG: must set before sending AK*/

      tp_setup_and_send_ak (tp_conn);			/* Send AK TPDU	*/

      tp4_connect_cnf (tp_conn->user_conn_id, (ST_ULONG) conn_id, 
                      (ST_UINT) tpdu_cc->udata_len, tpdu_cc->udata_buf);
      break;

    case TP_STATE_OPEN:
      tp_setup_and_send_ak (tp_conn);			/* Send AK TPDU	*/
      tp_conn->inact_time = tp_cfg.inact_time;		/* Reset timer	*/
      break;

    case TP_STATE_CLOSING:
      break;

    default:
      TP_LOG_ERR1 ("TP-ERROR: invalid state (%d) for received CC. Ignored.",
        tp_conn->state);
      break;
    }		/* end "switch"	*/
  return;
  }
#endif	/* defined (TP4_ENABLED)	*/

