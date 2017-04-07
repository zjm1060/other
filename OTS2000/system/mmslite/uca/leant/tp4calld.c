#if defined (TP4_ENABLED)
/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*	(c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*	1996-2002 All Rights Reserved					*/
/*									*/
/* MODULE NAME : tp4calld.c						*/
/* PRODUCT(S)  : Lean-T Stack						*/
/*									*/
/* MODULE DESCRIPTION :							*/
/*	TP4 functions for handling of "Called" side of connections.	*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	tp4_accept ()							*/
/*	tp_process_cr ()						*/
/*	tp_process_dr ()						*/
/*	tp_process_dc ()						*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev     Comments					*/
/* --------  ---  ------   -------------------------------------------  */
/* 06/20/02  JRB    15     Chg addr args to tp4_connect_ind.		*/
/* 05/09/01  JRB    14     On T-CONNECT.ind, save TSAPs in TP_CONN.	*/
/* 06/08/99  JRB    13     Chg to support 1024 connections.		*/
/* 08/13/98  JRB    12     Lint cleanup.				*/
/* 09/25/97  JRB    11     Don't generate exception for invalid state.	*/
/* 08/26/97  JRB    10     RFC1006 changes:				*/
/*			   tp4_accept call tp0_accept if RFC1006 conn.	*/
/*			   If !TP4_ENABLED, disable whole module.	*/
/* 08/14/97  JRB    09     Save "negotiated" TPDU len in tp_conn.	*/
/* 08/14/97  JRB    08     tp_process_cr cast conn_id to ST_ULONG	*/
/*			   so no risk of sign extension.		*/
/* 05/27/97  JRB   7.00    MMSEASE 7.0 release.				*/
/* 03/21/97  JRB    04     Add tp_free_spdu_all call to tp4_process_dr.	*/
/* 01/24/97  JRB    06     Just log if in illegal state to receive DC.	*/
/* 01/08/97  JRB    05     Don't call TP4_EXCEPT in tp_process_dc if	*/
/*			   rem_ref invalid. Just log and ignore.	*/
/* 10/04/96  JRB    04     Use max_tpdu_len_enc (binary encoded value).	*/
/* 10/04/96  JRB    03     Call tp_conn_clean from tp_process_cr.	*/
/* 09/09/96  JRB    02     tp_process_dr - don't care if rem_ref matches*/
/*					   handle TP_STATE_WFCC		*/
/*			   tp_process_dc - Fix rem_ref check.		*/
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

/*======================================================================*/
/*======================================================================*/
/* FUNCTIONS BELOW CALLED BY USER TO ENCODE AND SEND TPDUs.		*/
/*======================================================================*/
/*======================================================================*/

/************************************************************************/
/*			tp4_accept					*/
/* This function is called by the user to accept a connect request from	*/
/* a remote node							*/
/************************************************************************/

ST_RET tp4_accept (ST_LONG tp4_conn_id, ST_LONG user_conn_id,
		    ST_INT conndata_len,
		    char *conndata)
  {
TP_CONN *tp_conn;

#if defined (TP0_ENABLED)
  if (tp4_conn_id >= MIN_TP0_CONN_ID)
    return (tp0_accept (tp4_conn_id, user_conn_id, conndata_len, conndata));
#endif

  if ((ST_ULONG) tp4_conn_id >= tp_cfg.max_num_conns)
    return (TP4E_BADCONN);

  tp_conn = &tp_conn_arr [tp4_conn_id];
  if (tp_conn->state != TP_STATE_WFTRESP)
    return (TP4E_BADCONN);

  tp_conn->user_conn_id = user_conn_id;		/* Save User's ID	*/

  /* Set retransmission timer and # of transmissions.	*/
  tp_conn->retrans_time = tp_cfg.retrans_time;
  tp_conn->num_trans = 1;

  /* Save User Data in tp_conn for retransmission.	*/
  /* DEBUG: Can we blow this off to save space?		*/
  tp_conn->tpdu_cx.udata_len = min ((ST_UINT16) conndata_len, TP_MAX_CONN_UDATA);
  memcpy (tp_conn->tpdu_cx.udata_buf, conndata, tp_conn->tpdu_cx.udata_len);

  tp_conn->state = TP_STATE_AKWAIT;
  tp_send_cc (&tp_conn->tpdu_cx);
  return (SD_SUCCESS);
  }

/*======================================================================*/
/*======================================================================*/
/* FUNCTIONS BELOW CALLED BY TP4 TO PROCESS TPDUs RECEIVED		*/
/* (TPDUs ALREADY DECODED BY TP4).					*/
/*======================================================================*/
/*======================================================================*/

/************************************************************************/
/* tp_process_cr							*/
/* Process a CR (Connection Request) TPDU.				*/
/************************************************************************/
ST_VOID tp_process_cr (TPDU_CX *tpdu_cr_rx)
  {
ST_UINT16 conn_id;		/* index into "tp_conn_arr" array.		*/
TP_CONN *tp_conn;	/* ptr to struct in "tp_conn_arr" array.	*/
TPDU_DR tpdu_dr;

  /* Fill in TPDU_DR struct in case DR needs to be sent.		*/
  /* Do not fill in "reason" yet.					*/
  tpdu_dr.loc_ref = 0;				/* src-ref = 0 (always)	*/
  tpdu_dr.rem_ref = tpdu_cr_rx->rem_ref;
  tpdu_dr.rem_nsap = tpdu_cr_rx->rem_nsap;
  tpdu_dr.rem_mac = tpdu_cr_rx->rem_mac;

  /* Check if Local TSAP is correct.		*/
  if (memcmp (tpdu_cr_rx->loc_tsap, only_loc_tsap, only_loc_tsap[0]+1))
    {
    TP_LOG_ERR0 ("TP-ERROR: received CR with invalid TSAP. Sending DR.");
    tpdu_dr.reason = TP_DR_ADDR_UNKNOWN;
    tp_send_dr (&tpdu_dr);
    return;
    }

  /* Check for Transport Class 4.  Don't allow any other.		*/
  if (tpdu_cr_rx->preferred_class != 4)
    {
    TP_LOG_ERR1 ("TP-ERROR: received CR with invalid class %d. Sending DR.",
      tpdu_cr_rx->preferred_class);
    tpdu_dr.reason = TP_DR_CLASS_UNSUPPORTED;
    tp_send_dr (&tpdu_dr);
    return;
    }

  /* Local Ref must equal 0.  If not, it is protocol error.		*/
  if (tpdu_cr_rx->loc_ref != 0)
    {
    TP_LOG_ERR0 ("TP-ERROR: received CR with invalid reference. Sending DR.");
    tpdu_dr.reason = TP_DR_PROT_ERROR;
    tp_send_dr (&tpdu_dr);
    return;
    }

  /* A duplicate CR can be received when we're in almost any state.	*/
  /* So when we receive a CR, we must check all our connections to make	*/
  /* sure it is not a duplicate.					*/
  /* NOTE: this is not very efficient, but we must do it.		*/

  tp_conn = &tp_conn_arr [0];
  for (conn_id = 0;  conn_id < tp_cfg.max_num_conns;  conn_id++, tp_conn++)
    {
    /* DEBUG: TP_STATE_* defines MUST BE IN SAME ORDER AS 8073 Add.2 STATE TABLE	*/
    if (tp_conn->state >= TP_STATE_OPEN)	/* OPEN, WFTRESP, AKWAIT, CLOSING	*/
      {
      /* Check CR to see if it is duplicate of one already received.	*/
      /* NOTE: this must be as efficient as possible because it may be	*/
      /*       done several times for each CR received. Therefore, we	*/
      /*       check params most likely to be different first.		*/
      if (tp_conn->rem_ref == tpdu_cr_rx->rem_ref			&&
          memcmp (tpdu_cr_rx->rem_nsap, tp_conn->rem_nsap,
                  tp_conn->rem_nsap[0]+1) == 0			&&
          memcmp (tpdu_cr_rx->rem_tsap, tp_conn->tpdu_cx.rem_tsap,
                  tp_conn->tpdu_cx.rem_tsap[0]+1) == 0		&&
          memcmp (tpdu_cr_rx->loc_tsap, tp_conn->tpdu_cx.loc_tsap,
                  tp_conn->tpdu_cx.loc_tsap[0]+1) == 0)
        {	/* IT IS A DUPLICATE CR	*/
        switch (tp_conn->state)
          {
          case TP_STATE_OPEN:
            tp_conn->inact_time = tp_cfg.inact_time;
            break;
          case TP_STATE_WFTRESP:
            break;		/* Do nothing	*/
          case TP_STATE_AKWAIT:
            /* Everything needed to send CC should be in tp_conn	*/
            tp_send_cc (&tp_conn->tpdu_cx);
            break;
          case TP_STATE_CLOSING:
            tpdu_dr.reason = TP_DR_NO_REASON;
            tp_send_dr (&tpdu_dr);
            break;
          default:
            /* This means state >= TP_STATE_OPEN, but not one of the	*/
            /* above. The order of the defines must be messed up.	*/
            TP4_EXCEPT (); 
            break;
          }
        return;	/* Duplicate CR handled. Stop checking.		*/
        }	/* end "IT IS A DUPLICATE CR"	*/
      }		/* end "if (tp_conn->state >= TP_STATE_OPEN)"	*/
    }		/* end "for" loop		*/

  /* Look for a position in "tp_conn_arr" where state = TP_STATE_CLOSED	*/
  tp_conn = &tp_conn_arr [0];
  for (conn_id = 0;  conn_id < tp_cfg.max_num_conns;  conn_id++, tp_conn++)
    {
    if (tp_conn->state == TP_STATE_CLOSED)
      {
      tp_conn_clean (tp_conn);	/* Good time to clean up conn struct.	*/
      inc_loc_ref_offset (tp_conn->loc_ref_offset);
      tp_conn->num_trans = 1;	/* Increment # of transmissions.	*/

      /* Save Remote NSAP and MAC in "tp_conn".				*/
      memcpy (tp_conn->rem_nsap, tpdu_cr_rx->rem_nsap,
              tpdu_cr_rx->rem_nsap[0]+1);	/* First byte is len.	*/
      memcpy (tp_conn->rem_mac, tpdu_cr_rx->rem_mac, CLNP_MAX_LEN_MAC);

      /* Copy the CR info to the tp_conn struct and modify it for the CC.*/
      /* Most parameters we send back exactly as received (all except	*/
      /* max_tpdu_len, loc_ref, cdt, udata_buf/len).			*/
      /* NOTE: udata_buf/len not changed until tp4_accept.		*/
      /* CRITICAL: rem_mac and rem_nsap in tpdu_cr_rx point into NPDU.	*/
      /* In tp_conn->tpdu_cx they must point to saved ones in tp_conn.	*/

      memcpy (&tp_conn->tpdu_cx, tpdu_cr_rx, sizeof (TPDU_CX));
      tp_conn->tpdu_cx.rem_mac = tp_conn->rem_mac;
      tp_conn->tpdu_cx.rem_nsap = tp_conn->rem_nsap;
      tp_conn->tpdu_cx.max_tpdu_len_enc = min (tp_cfg.max_tpdu_len_enc,
                                               tpdu_cr_rx->max_tpdu_len_enc);
      /* Save negotiated TPDU len in tp_conn.	*/
      switch (tp_conn->tpdu_cx.max_tpdu_len_enc)
        {					/* code for PDU size	*/
        case TP_PDU_MAX_SIZE_128  :	tp_conn->max_tpdu_len =   128; break;
        case TP_PDU_MAX_SIZE_256  :	tp_conn->max_tpdu_len =   256; break;
        case TP_PDU_MAX_SIZE_512  :	tp_conn->max_tpdu_len =   512; break;
        case TP_PDU_MAX_SIZE_1024 :	tp_conn->max_tpdu_len =  1024; break;
        case TP_PDU_MAX_SIZE_2048 :	tp_conn->max_tpdu_len =  2048; break;
        case TP_PDU_MAX_SIZE_4096 :	tp_conn->max_tpdu_len =  4096; break;
        case TP_PDU_MAX_SIZE_8192 :	tp_conn->max_tpdu_len =  8192; break;
        default:
          TP_LOG_ERR0 ("TP-ERROR: received CR with illegal TPDU size. Sending DR.");
          tpdu_dr.reason = TP_DR_PROT_ERROR;
          tp_send_dr (&tpdu_dr);
          return;
        }	/* end "switch"	*/

      tp_conn->tpdu_cx.loc_ref = calc_loc_ref (conn_id, tp_conn->loc_ref_offset);
      tp_conn->tpdu_cx.cdt = tp_cfg.loc_cdt;	/* CDT we will send	*/

      /* Set "tp_conn" parameters for new connection.			*/
      /* Don't allow CDT greater than max we can handle.		*/
      tp_conn->adj_rem_cdt = min (tp_cfg.max_rem_cdt, tpdu_cr_rx->cdt);
      tp_conn->rem_ref = tpdu_cr_rx->rem_ref;	/* need for sending AKs	*/
      tp_conn->retrans_time = tp_cfg.retrans_time;
      tp_conn->state = TP_STATE_WFTRESP;

      /* Save TSAPs in TP_CONN struct.					*/
      memcpy (tp_conn->loc_tsap, tpdu_cr_rx->loc_tsap, tpdu_cr_rx->loc_tsap[0]+1);
      memcpy (tp_conn->rem_tsap, tpdu_cr_rx->rem_tsap, tpdu_cr_rx->rem_tsap[0]+1);

      tp4_connect_ind (only_user_bind_id, (ST_ULONG) conn_id,
                tpdu_cr_rx->loc_tsap,
                tpdu_cr_rx->rem_tsap,
                tpdu_cr_rx->rem_nsap,
                0,			/* IP addr: not used	*/
                (ST_INT) tpdu_cr_rx->udata_len,
                tpdu_cr_rx->udata_buf);
      break;		/* BREAK OUT OF LOOP. Only want 1 connection!!!	*/
      }
    }		/* end "for" loop	*/
  if (conn_id >= tp_cfg.max_num_conns)
    {				/* Did not find an available conn_id.	*/
    TP_LOG_ERR0 ("TP-ERROR: not enough resources for received CR. Sending DR.");
    tpdu_dr.reason = TP_DR_CONGESTION;
    tp_send_dr (&tpdu_dr);
    return;
    }
  return;
  }

/************************************************************************/
/* tp_process_dr							*/
/* Process a DR (Disconnect Request) TPDU.				*/
/************************************************************************/
ST_VOID tp_process_dr (TPDU_DR *tpdu_dr)
  {
ST_UINT16 conn_id;		/* index into "tp_conn_arr" array.		*/
TP_CONN *tp_conn;	/* ptr to struct in "tp_conn_arr" array.	*/
TPDU_DC *tpdu_dc;
ST_INT reason;		/* Disconnect reason	*/

  /* Fill in a DC TPDU right away.					*/
  /* NOTE: copying of the loc_ref takes care of setting src_ref=0 in DC	*/
  /*       if DST-REF=0 in the received DR, as in the State Tables.	*/
  /* NOTE: the DR TPDU may not necessarily be associated with a conn,	*/
  /*       so we use the rem_nsap and rem_mac from the DR for the DC.	*/
  tpdu_dc = tpdu_dr;	/* Params are all the same as received DR,	*/
			/* except "reason" param ignored for DC.	*/

  if ((conn_id = calc_conn_id (tpdu_dr->loc_ref)) >= tp_cfg.max_num_conns)
    {	/* Loc ref is invalid.  Treat as if state = TP_STATE_CLOSED	*/
    if (tpdu_dr->rem_ref != 0)
      tp_send_dc (tpdu_dc);		/* Send DC TPDU already built.	*/
    return;
    }
  
  tp_conn = &tp_conn_arr [conn_id];

  /* DEBUG: we should clear loc_ref and rem_ref when we close so we	*/
  /* don't accidentally match with obsolete values.			*/

  /* NOTE: if state was CLOSED awhile ago, this conn could be reused	*/
  /* already.  In this case the "loc_ref" would not match.  If this	*/
  /* happens, we treat as if the state is TP_STATE_CLOSED.		*/

  if (tpdu_dr->loc_ref != tp_conn->tpdu_cx.loc_ref)
    {    /* Local ref doesn't match.  Treat as if TP_STATE_CLOSED	*/
    if (tpdu_dr->rem_ref != 0)
      tp_send_dc (tpdu_dc);		/* Send DC TPDU already built.	*/
    return;
    }

  tp_free_spdu_all (tp_conn);	/* Free all SPDUs allocated for this conn*/

  switch (tp_conn->state)
    {
    case TP_STATE_CLOSED:
      if (tpdu_dr->rem_ref != 0)
        tp_send_dc (tpdu_dc);		/* Send DC TPDU already built.	*/
      break;
    case TP_STATE_WFCC:
      if (tpdu_dr->rem_ref != 0)
        tp_send_dc (tpdu_dc);		/* Send DC TPDU already built.	*/
      reason = 0;			/* Normal disconnection	*/
      /* NOTE: To save memory, user data is never passed up to user.	*/
      tp4_disconnect_ind (tp_conn->user_conn_id, reason,
                          0, NULL);	/* No user data passed up.	*/
      tp_conn->state = TP_STATE_CLOSED;	/* Skip REFWAIT state.		*/
      break;
    case TP_STATE_OPEN:
    case TP_STATE_WFTRESP:
    case TP_STATE_AKWAIT:
      tp_send_dc (tpdu_dc);		/* Send DC TPDU already built.	*/
      /* DEBUG: "reason" passed to tp4_disconnect_ind not same as	*/
      /*        tpdu_dr->reason. Should this be changed?		*/
      reason = 0;			/* Normal disconnection	*/
      /* NOTE: To save memory, user data is never passed up to user.	*/
      tp4_disconnect_ind (tp_conn->user_conn_id, reason,
                          0, NULL);	/* No user data passed up.	*/
      tp_conn->state = TP_STATE_CLOSED;	/* Skip REFWAIT state.		*/
      break;
    case TP_STATE_CLOSING:
      tp_conn->state = TP_STATE_CLOSED;	/* Skip REFWAIT state.		*/
      break;
    default:
      TP_LOG_ERR1 ("TP-ERROR: invalid state (%d) for received DR. Ignored.",
        tp_conn->state);
      break;
    }

  return;
  }

/************************************************************************/
/* tp_process_dc							*/
/* Process a DC (Disconnect Confirm) TPDU.				*/
/************************************************************************/
ST_VOID tp_process_dc (TPDU_DC *tpdu_dc)
  {
ST_UINT16 conn_id;		/* index into "tp_conn_arr" array.		*/
TP_CONN *tp_conn;	/* ptr to struct in "tp_conn_arr" array.	*/

  /* Get conn_id from loc_ref, if possible.				*/
  if ((conn_id = calc_conn_id (tpdu_dc->loc_ref)) >= tp_cfg.max_num_conns)
    return;	/* Loc ref is invalid.  Ignore this DC.			*/
  
  tp_conn = &tp_conn_arr [conn_id];

  /* Check that loc_ref is correct for this conn_id.			*/
  if (tpdu_dc->loc_ref != tp_conn->tpdu_cx.loc_ref)
    return;	/* Loc ref is invalid.  Ignore this DC.			*/

  /* Check that rem_ref is correct for this conn_id.			*/
  if (tpdu_dc->rem_ref != tp_conn->rem_ref)
    {
    TP_LOG_ERR1 ("TP-ERROR: TPDU-DC rem ref (%d) invalid. Ignored.", tpdu_dc->rem_ref);
    return;	/* Rem ref is invalid.  Ignore this DC.			*/
    }

  if (memcmp (tpdu_dc->rem_nsap, tp_conn->rem_nsap,
              tp_conn->rem_nsap[0]+1))	/* First byte is len.	*/
    {
    TP_LOG_ERR0 ("TP-ERROR: TPDU-DC rem NSAP invalid. Ignored.");
    return;	/* Rem NSAP is invalid.  Ignore this DC.		*/
    }

  switch (tp_conn->state)
    {
    case TP_STATE_CLOSED:
      /* NOTE: if state was CLOSED, this conn could be reused already.	*/
      /*       We don't do anything here anyway, so it's OK.		*/
      break;
    case TP_STATE_CLOSING:
      tp_conn->state = TP_STATE_CLOSED;	/* Skip REFWAIT state.		*/
      break;
    default:
      TP_LOG_ERR1 ("TP-ERROR: Illegal state to receive DC on conn %d. Ignored.",
                   conn_id);
      break;
    }

  return;
  }
#endif	/* defined (TP4_ENABLED)	*/

