/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1986 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mvl_acse.c    						*/
/* PRODUCT(S)  : MMSEASE-LITE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/19/02  ASK     81    Fix net_info variable in _mvl_send_msg	*/
/* 07/17/02  MDE     80    Added mvl_conn_filtered_logging to send	*/
/* 07/10/02  JRB     79    Add maxpend_ind, numpend_ind.		*/
/* 07/09/02  MDE     78    Fixed bad free for bad decode of init resp	*/
/* 05/24/02  JRB     77    Put back MVL_COMM_EVENT linked list.		*/
/*			   Occasionally copp_event gets 2 events, one	*/
/*			   from TP0 and one from TP4.			*/
/* 05/17/02  JRB     76    mvla_init..: set *req_out=NULL if freed	*/
/*			   so user CAN'T free it again.			*/
/* 05/13/02  MDE     75    Don't scan unbound mvl_net_info		*/
/* 05/13/02  MDE     74    Now clean up requests for release conf	*/
/* 05/03/02  MDE     73    Added mvl_add_bind_ctrl, associated changes	*/
/* 04/29/02  MDE     72    Added MLOG for initiate			*/
/* 04/29/02  JRB     71    DON'T re-use MVL_NET_INFO until req_pend_list*/
/*			   is empty, assert if pend_ind is not empty.	*/
/* 03/07/02  MDE     70    mvla_set_init_ar now takes NULL		*/
/* 03/04/02  JRB     69    Start with clean "temp_bind_ctrl" so garbage	*/
/*			   doesn't get copied. 				*/
/*			   Return error if num_call*>0 but not supported*/
/* 02/25/02  MDE     68    More changes to MVL_CFG_INFO			*/
/* 02/20/02  MDE     67    Changes for new MVL_CFG_INFO, multiple bind	*/
/* 02/14/02  RKR     66    call a_associate_reqm if Marben stack	*/
/* 02/08/02  JRB     65	   Pass MVL_CFG_INFO struct to mvl_start_acse.	*/
/*			   Del mvl_local_ar_name,mvl_set_num_connections*/
/* 01/30/02  JRB     64    Add args to copp_initialize (same as MAP30..)*/
/*			   Del call to a_set_msgsize.			*/
/*			   Make sure mmsl_max_msg_size, mvl_num_calling,*/
/*			   mvl_num_called are set.			*/
/*			   Use MMS_INIT_EN instead of MVL_NUM_CALL*.	*/
/*			   Del use of COACSE define.			*/
/*			   Del mvl_max_comm_event (not used).		*/
/*			   Merge mvl_init, _mvl_init_conn_ctrl into	*/
/*			   mvl_start_acse.				*/
/*			   Replace mvl_init_cl_netinfo funct with	*/
/*			   mvl_init_audt_addr.				*/
/*			   Del u_a_unit_data_ind & all use of stack_sel.*/
/* 01/09/02  JRB     63    Del all use of clpp_bind & bind id.		*/
/*			   Del unused u_cla_except callback function.	*/
/* 12/04/01  MDE     62    Change to work with MAP30_ACSE		*/
/* 11/30/01  GLB     61    changed MVL_LOG_ERR1 to MVL_LOG_ERR0         */
/* 11/28/01  MDE     60    mvl_set_num_connections,mvl_set_max_msg_size */
/* 11/21/01  MDE     59    mvl_loc_ar_name now auto set			*/
/* 11/21/01  MDE     58    Safer num channel operation			*/
/* 11/05/01  JRB     57    mvl_wait_req_done now returns ST_RET.	*/
/* 09/21/01  JRB     56    Alloc global bufs only once at startup.	*/
/*			   Fix M_FREE context.				*/
/* 07/31/01  MDE     55    Added mvl_conn_filtered_logging		*/
/* 05/21/01  MDE     54    Free NET_INFO				*/
/* 03/13/01  JRB     53    Check for (MMS_INIT_EN & REQ_EN).		*/
/* 02/26/01  MDE     52    Moved m_smem_ctxt to mem_chks.c      	*/
/* 11/28/00  EJV     51    Changes to interface with Marben stack SUIC	*/
/*                         (see MAP30_ACSE define):			*/
/*                         Use SUIC find_loc_..., find_rem_dib_entry.	*/
/*			   Added   param  in copp_initialize().		*/
/*			   Added   param  in mvla_initiate_req().	*/
/*			   Changed params in copp_bind().		*/
/* 10/25/00  JRB     50    Del MVL_COMM_EVENT linked list. Only one used*/
/*			   Del call to _mvl_free_req_pend.		*/
/* 09/22/00  MDE     49    Changed index init				*/
/* 08/21/00  RKR     48    Changed prototype of mvl_init_cl_netinfo 	*/
/* 08/23/00  MDE     47    Call '_mvl_set_net_req_done' on aborts too	*/
/* 05/15/00  MDE     46    Added MMS statistics				*/
/* 04/27/00  JRB     45    Fixed calloc (0) problem			*/
/* 04/14/00  JRB     44    Lint cleanup.				*/
/* 04/12/00  MDE     43    Added AA resize, MVL_STATISTICS		*/
/* 03/15/00  JRB     42    Del #ifdef MVL_REQ_BUF_ENABLED: not used.	*/
/* 03/09/00  MDE     41    Added _mvl_init_conn_ctrl			*/
/* 01/21/00  MDE     40    Now use MEM_SMEM for dynamic memory		*/
/* 12/20/99  GLB     39    Remove extra initialization and add asserts  */
/* 11/23/99  MDE     38    Added mvla_check_req_que (from mvl_serv.c)	*/
/* 10/20/99  NAV     37    Add maxpend_req support			*/
/* 11/03/99  RKR     36    Set aborted pend req results to DISCONN	*/
/* 10/29/99  GLB     35    Added check for lack of connection           */
/* 09/07/99  MDE     34    Added _mvlu_rpt_disconnect_rcvd_fun 		*/
/* 07/30/99  JRB     33    Added MVL_GOOSE_SUPPORT define use		*/
/*			   Chg ma_clbind_ctrl from static to global.	*/
/* 07/14/99  JRB     32    On assoc ind, save acse_conn_id first.	*/
/* 04/13/99  MDE     31    Added '_mvla_initiate_locDe'			*/
/* 04/07/99  MDE     30    Fixed mvla_initiate_req return code		*/
/* 04/01/99  MDE     29    Eliminated connection handling memory leaks	*/
/* 04/01/99  MDE     28    Changes to decode buffer allocation scheme   */
/* 03/18/99  MDE     27    Made # connection limits changable runtime 	*/
/* 03/08/99  MDE     26    Added ICCP support				*/
/* 12/08/98  MDE     25    Cleanup on request control handling		*/
/* 12/08/98  MDE     24    Allow user to initiate w/o supplying a 	*/
/*			   'resp_info' buffer.				*/
/* 11/16/98  MDE     23    Conclude/Release cleanup, misc cleanup	*/
/* 10/22/98  JRB     22    mvl_start_acse init mvl_debug_sel. Other lint*/
/* 09/21/98  MDE     21    Minor lint cleanup				*/
/* 08/25/98  MDE     20    Fixed bind/unbind bug for CLACSE		*/
/* 08/14/98  MDE     19    Removed !ALLOW_COMPLEX_NET_EVENTS code	*/
/* 08/14/98  MDE     18    Updated 'u_a_data_ind' parameters as reqd	*/
/* 07/20/98  JRB     17    If MVL_NUM_CALLED not defined, reject all	*/
/*			   associate indications.			*/
/* 07/16/98  MDE     16    Addded 'mvl_dib_entry_to_app_ref'		*/
/* 07/06/98  JRB     15    mvl_init_cl_netinfo use new DIB_ENTRY and	*/
/*			   init rem_mac_valid so it sends to ALL-ES.	*/
/* 06/18/98  JRB     14    Bind to TP4 and TCP if tp_type=TP_TYPE_TPX.	*/
/* 02/10/98  MDE     13    Added 'index' to connection control.		*/
/* 12/29/97  JRB     12    Call c?pp_terminate if bind fails.		*/
/* 12/22/97  MDE     11    Changed '_mvl_net_service' to report activity	*/
/* 09/18/97  MDE     10    Now call 'a_set_msgsize'			*/
/* 02/07/97  MDE     09    Changed to use new MMS-EASE opcode defines	*/
/* 12/13/96  MDE     08    Now support CLACSE, other enhancements	*/
/* 11/18/96  MDE     07    Minor name cleanup, some CLACSE stuff	*/
/* 11/15/96  MDE     06    Global variable work				*/
/* 11/11/96  MDE     05    Added async functions, other cleanup		*/
/* 10/31/96  MDE     04    Added ALLOW_COMPLEX_NET_EVENTS code		*/
/* 08/19/96  MDE     03    Now save all remote node's initiate info	*/
/* 07/22/96  MDE     02    Changes towards A-Unit-Data support		*/
/* 01/21/94  MDE     01    Initial                       		*/
/************************************************************************/
#include <assert.h>

#include "glbtypes.h"
#include "sysincs.h"
#include "glbsem.h"

#include "mmsdefs.h"
#include "mms_pvar.h"
#include "mms_vvar.h"
#include "mms_err.h"
#include "mms_pcon.h"
#include "asn1defs.h"
#include "slog.h"
#include "tp4api.h"
#include "mvl_acse.h"
#include "mvl_defs.h"
#include "mvl_log.h"
#include "mloguser.h"
#include "mmsop_en.h"

#if defined(MAP30_ACSE)
/* To limit the number of Marben stack files in the \mmslite\inc we	*/
/* prototype few referenced SUIC functions here.			*/
extern DIB_ENTRY *find_dib_entry (ST_CHAR *name);
extern DIB_ENTRY *find_loc_dib_entry (ST_CHAR *name);
#endif

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/

MVL_MMS_STATISTICS mvl_mms_statistics;

/************************************************************************/

#define NEGIOTIATE_INITIATE_PARAM
#define MVL_MMSOP_RELEASE 512	/* Add a MMS opcode define ... */

/************************************************************************/
/* Global variables - Connection Oriented Only				*/
/* The user must fill in a table of DIB entries for local and remote	*/
/* nodes of interest. This can be done by the user via the 'init_dirser'*/
/* function, can be hard coded, etc..					*/

#if !defined(MAP30_ACSE)    /* For MAP30_ACSE use SUIC function instead */
ST_INT num_loc_dib_entries;
DIB_ENTRY *loc_dib_table;
ST_INT num_rem_dib_entries;
DIB_ENTRY *rem_dib_table;
#endif

DIB_ENTRY *_mvla_initiate_locDe;
NET_BIND_CTRL *_mvla_initiate_bc;

/* Our connection controls - we support multiple called and/or calling	*/
/* simultanious connections.						*/

MVL_NET_INFO *mvl_calling_conn_ctrl;
MVL_NET_INFO *mvl_called_conn_ctrl;

/* Other user configurable parameters					*/
ST_CHAR *mvl_local_cl_ar_name;

#ifndef MMS_INIT_EN
#error MMS_INIT_EN not defined. Must include mmsop_en.h
#endif
#if (MMS_INIT_EN & RESP_EN) 
INIT_INFO *mvl_init_resp_info;
#endif

ST_VOID (*u_mvl_disc_ind_fun) (MVL_NET_INFO *cc, ST_INT discType);
ST_INT (*u_mvl_connect_ind_fun) (MVL_NET_INFO *cc, INIT_INFO *init_info);
ST_VOID (*_mvlu_rpt_disconnect_rcvd_fun)(MVL_NET_INFO *netInfo);

/************************************************************************/
/* This variable is set SD_TRUE when the ACSE interface is viable		*/
ST_INT mvl_acse_ok;

/************************************************************************/
/* These arrays are created in mmsop_en.c */
extern SD_CONST ST_UCHAR  m_param[2];
extern SD_CONST ST_UCHAR  m_service_resp[11];

/************************************************************************/
/************************************************************************/
/* Bind Control								*/


NET_BIND_CTRL ma_clbind_ctrl;	/* to save clpp_bind info.	*/

/* These variables are used in making some asynchronous ACSE functions 	*/
/* work in a blocking mode						*/
static ST_INT ma_cmd_done;
static ST_INT ma_cmd_rslt;

/************************************************************************/

MVL_CFG_INFO *mvl_cfg_info;

/************************************************************************/
/* MMS COMM EVENT CONTROL MANAGEMENT */

static MVL_COMM_EVENT *mvl_comm_event_up_head;	/* ptr to linked list	*/

/************************************************************************/
/* MVL_NET_INFO handling for incoming A-Unit Data				*/

#if defined(CLACSE)
ST_INT16 mvl_max_net_info = 10;
static ST_INT16 mvl_net_info_count;
static MVL_NET_INFO *mvl_net_info_avail_head;

static MVL_NET_INFO *mvl_get_net_info (ST_VOID);
static ST_VOID mvl_free_net_info (MVL_NET_INFO *netInfo);
#endif

/************************************************************************/

/* Internal functions */
static ST_VOID ma_wait_req_done (ST_RET *retOut);
static MVL_NET_INFO *ma_get_called_conn_ctrl (NET_BIND_CTRL *);
static MVL_NET_INFO *ma_get_calling_conn_ctrl (NET_BIND_CTRL *);
static ST_VOID copp_unbind_sync (ST_LONG copp_bind_id);
static ST_RET ma_bind (NET_BIND_CTRL *bc, ST_BOOLEAN activate);

static MVL_COMM_EVENT *mvl_get_com_event (ST_VOID);

/************************************************************************/
/************************************************************************/
/*                       _mvl_net_service				*/
/************************************************************************/

ST_BOOLEAN _mvl_net_service (MVL_COMM_EVENT **eventOut)
  {
ST_BOOLEAN activityFlag;

  *eventOut = NULL;
  if (mvl_acse_ok != SD_TRUE)
    return (SD_FALSE);

  S_LOCK_COMMON_RESOURCES ();

/* May cause the 'acse' user function to be called if something is	*/
/* going on that we need to know about. If so, the functions will	*/
/* get the event and put them onto the 'mvl_comm_event_up_head' list	*/

  activityFlag = copp_event ();
  /* Occasionally copp_event gets 2 events, one from TP0 and one from TP4,*/
  /* so they are put on this linked list. "Usually" only one on list.	*/
  if (mvl_comm_event_up_head)
    *eventOut = (MVL_COMM_EVENT *) list_get_first((ST_VOID **) &mvl_comm_event_up_head);

  S_UNLOCK_COMMON_RESOURCES ();
  return (activityFlag);
  }

/************************************************************************/
/*			mvl_get_com_event				*/
/************************************************************************/
/* This function is used to get a MVL_COMM_EVENT control buffer to be	*/
/* passed up to the caller of '_mvl_net_service'. 			*/

static MVL_COMM_EVENT *mvl_get_com_event (ST_VOID)
  {
MVL_COMM_EVENT *commEvent;

  S_LOCK_COMMON_RESOURCES ();
  commEvent = (MVL_COMM_EVENT *) M_CALLOC (MSMEM_COM_EVENT, 1, sizeof (MVL_COMM_EVENT));  
  if (commEvent == NULL)
    {
    MVL_LOG_ERR0 ("Error: No MVL_COMM_EVENT available");
    S_UNLOCK_COMMON_RESOURCES ();
    return (NULL);
    }

  list_add_last((ST_VOID **) &mvl_comm_event_up_head, (ST_VOID *) commEvent);
  S_UNLOCK_COMMON_RESOURCES ();
  return (commEvent);
  }

/************************************************************************/
/*			_mvl_free_comm_event				*/
/************************************************************************/

ST_VOID _mvl_free_comm_event (MVL_COMM_EVENT *event)
  {
  S_LOCK_COMMON_RESOURCES ();
  if (event->event_type == MMS_MSG_RCVD)
    {
    M_FREE (MSMEM_RXPDU, event->u.mms.pdu);

    if (event->u.mms.dec_rslt.data_pres)
      {
      M_FREE (MSMEM_DEC_OS_INFO, event->u.mms.dec_rslt.data_ptr);
      }  
    }

  M_FREE (MSMEM_COM_EVENT, event);
  S_UNLOCK_COMMON_RESOURCES ();
  }

/************************************************************************/
/*		 	mvl_set_max_msg_size 				*/
/************************************************************************/

ST_VOID mvl_set_max_msg_size (ST_INT max_msg_size)
  {
  mvl_cfg_info->max_msg_size = max_msg_size;
  mmsl_enc_buf_size = max_msg_size + ASN1_MAX_ELEMENT_OVERHEAD;
  if (mmsl_enc_buf != NULL)
    M_FREE (MSMEM_STARTUP, mmsl_enc_buf);

  mmsl_enc_buf = (unsigned char *)M_MALLOC (MSMEM_STARTUP, mmsl_enc_buf_size);
  }

/************************************************************************/
/*                       mvl_start_acse					*/
/************************************************************************/

ST_INT mvl_start_acse (MVL_CFG_INFO *cfgInfo)
  {
NET_BIND_CTRL *bind_ctrl_src;
NET_BIND_CTRL *bind_ctrl_dest;
NET_BIND_CTRL temp_bind_ctrl;
ST_INT mvl_cfg_info_size;
ST_INT ret;
ST_INT i;
ST_INT index = 0;
  char tmpstr[200];
  

  MVL_LOG_ACSE0 ("ACSE: Starting");

  mvl_debug_sel |= MVLLOG_ERR;		/* Always enable error logging.	*/

/* Here we will make the MVL configuration information persistant 	*/
#if defined(MAP30_ACSE)
  if (cfgInfo->use_bind_ctrl != SD_TRUE)
#endif
    {	/* Create the required bind control structures, if not supplied */
    /* Use memset to start with clean structure	*/
    memset (&temp_bind_ctrl, 0, sizeof (NET_BIND_CTRL));	/* CRITICAL*/
    strcpy (temp_bind_ctrl.ar_name, cfgInfo->local_ar_name);
    temp_bind_ctrl.num_calling = cfgInfo->num_calling;
    temp_bind_ctrl.num_called = cfgInfo->num_called;
    cfgInfo->bind_ctrl = &temp_bind_ctrl;
    cfgInfo->max_bind_ctrl = 1;
    cfgInfo->num_bind_ctrl = 1;
    }

/* OK, now copy the MVL configuration info */
  mvl_cfg_info_size = sizeof (MVL_CFG_INFO) + 
  			  (cfgInfo->max_bind_ctrl * sizeof (NET_BIND_CTRL));
  mvl_cfg_info = (MVL_CFG_INFO *)chk_calloc (1, mvl_cfg_info_size);
  *mvl_cfg_info = *cfgInfo;

  if (mvl_cfg_info->max_bind_ctrl == 0)
    mvl_cfg_info->max_bind_ctrl = mvl_cfg_info->num_bind_ctrl;

  mvl_cfg_info->calling_used = 0;
  mvl_cfg_info->called_used = 0;
  mvl_cfg_info->bind_ctrl = (NET_BIND_CTRL *) (mvl_cfg_info + 1);
  bind_ctrl_dest = mvl_cfg_info->bind_ctrl;
  bind_ctrl_src = cfgInfo->bind_ctrl;
  for (i = 0; i < mvl_cfg_info->num_bind_ctrl; ++i)
    {
    *bind_ctrl_dest = *bind_ctrl_src;
    ++bind_ctrl_src;
    ++bind_ctrl_dest;
    }

/* Make sure max msg size has been set.					*/
  if (mvl_cfg_info->max_msg_size == 0)
    {
    MVL_LOG_ERR0 ("Max MMS message size must be set");
    return (SD_FAILURE);
    }
  mvl_set_max_msg_size (mvl_cfg_info->max_msg_size);	/* save to global & alloc bufs*/

/* Initialize function pointers for MMS allocation.			*/
  m_calloc_os_fun = _mvl_calloc_os;
  m_realloc_os_fun = _mvl_realloc_os;
  m_free_os_fun = _mvl_free_os;

#if !(MMS_INIT_EN & REQ_EN)
  if (mvl_cfg_info->num_calling != 0)
    {
    MVL_LOG_ERR0 ("num_calling > 0 is illegal because Initiate.Req is NOT enabled");
    return (SD_FAILURE);
    }
#endif
#if !(MMS_INIT_EN & RESP_EN)
  if (mvl_cfg_info->num_called != 0)
    {
    MVL_LOG_ERR0 ("num_called > 0 is illegal because Initiate.Resp is NOT enabled");
    return (SD_FAILURE);
    }
#endif

/* Make sure "num_calling" AND/OR "num_called" has been set.		*/
  if (mvl_cfg_info->num_calling == 0  &&  mvl_cfg_info->num_called == 0)
    {
    MVL_LOG_ERR0 ("num_calling AND num_called both 0, must set at least one." );
    return (SD_FAILURE);
    }

/* Alloc & fill in array of "calling" connection control structures.	*/
#if (MMS_INIT_EN & REQ_EN)
  if (mvl_cfg_info->num_calling != 0)
    mvl_calling_conn_ctrl = (struct mvl_net_info *)M_CALLOC (MSMEM_GEN, mvl_cfg_info->num_calling, sizeof (MVL_NET_INFO));
  for (i = 0; i < mvl_cfg_info->num_calling; ++i)
    mvl_calling_conn_ctrl[i].index = index++;
#endif

/* Alloc & fill in array of "called" connection control structures.	*/
#if (MMS_INIT_EN & RESP_EN)
  if (mvl_cfg_info->num_called != 0)
    mvl_called_conn_ctrl = (struct mvl_net_info *)M_CALLOC (MSMEM_GEN, mvl_cfg_info->num_called, sizeof (MVL_NET_INFO));
  for (i = 0; i < mvl_cfg_info->num_called; ++i)
    mvl_called_conn_ctrl[i].index = index++;
#endif

/* Initialize Connection Oriented OSI Stack.	*/
  ret = copp_initialize (mvl_cfg_info->num_calling + mvl_cfg_info->num_called, mmsl_enc_buf_size);
  if (ret != SD_SUCCESS)
    {
    MVL_LOG_ERR1 ("Error: copp_initialize returned error '0x%x'", ret);
    return (ret);
    }

/* Initialize Connectionless OSI Stack.	*/
#if defined(CLACSE)
  ret = clpp_initialize ();
  if (ret != SD_SUCCESS)
    {
    MVL_LOG_ERR1 ("Error: clpp_initialize returned error '0x%x'", ret);
    return (ret);
    }
#endif

/* Bind local AR names */


  for (i = 0; i < mvl_cfg_info->num_bind_ctrl; ++i)
    {	
    ret = ma_bind (&mvl_cfg_info->bind_ctrl[i], SD_TRUE);
    if (ret != SD_SUCCESS)
      return (ret);
    }

/* Binding not necessary for CLACSE.	*/

/* Initialize all mms objects (generated by foundry).			*/
  if (ret == SD_SUCCESS) {
    mvl_init_mms_objs ();	/* This should be ONLY call to this funct*/
  }

  if (ret == SD_SUCCESS)
    mvl_acse_ok = SD_TRUE;
  else
    {
    copp_terminate ();
#if defined(CLACSE)
    clpp_terminate ();
#endif
    }

  MVL_LOG_ACSE1 ("ACSE: Start complete, returning '0x%x'", ret);

  return (ret);
  }

/************************************************************************/
/*                       mvl_end_acse					*/
/************************************************************************/

ST_INT mvl_end_acse (ST_VOID)
  {
NET_BIND_CTRL *bc;
ST_INT i;

  MVL_LOG_ACSE0 ("ACSE: Ending");

/* Unbind all */
  for (i = 0; i < mvl_cfg_info->num_bind_ctrl; ++i)
    {
    bc = &mvl_cfg_info->bind_ctrl[i];
#ifdef LEAN_T
    if (bc->copp_bind_id_tcp)
      copp_unbind_sync (bc->copp_bind_id_tcp);
    if (bc->copp_bind_id_tp4)
      copp_unbind_sync (bc->copp_bind_id_tp4);
#else	/* !LEAN_T	*/
    copp_unbind_sync (bc->copp_bind_id);
#endif	/* !LEAN_T	*/
    }

/* Shut down the ACSE interface, as well as it's subsystems		*/

  copp_terminate ();
#if defined(CLACSE)
  clpp_terminate ();
#endif

  MVL_LOG_ACSE0 ("ACSE: Ended");
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			_mvl_send_msg					*/
/************************************************************************/

ST_INT _mvl_send_msg (ST_UCHAR *pdu, ST_INT pdu_len, MVL_NET_INFO *net_info)
  {
ST_INT ret;

/* See if we are supposed to log ACSE data for this connection		*/
  if (mvl_conn_filtered_logging == SD_FALSE || net_info->log_enable)
    {
    MVL_LOG_ACSEDATA2 ("ACSE: Sending %d bytes on MVL_NET_INFO 0x%08lx", 
						pdu_len, net_info);
    MVL_LOG_ACSEDATAH (pdu_len, pdu);
    }

  ret = SD_FAILURE;

#if (MMS_INIT_EN != REQ_RESP_DIS)
    if (net_info->conn_active == SD_FALSE)
    { /* no connection */
      MVL_LOG_NERR1 ("ACSE: no connection '0x%x'", ret);
    }
    else
    { /* valid connection */
      ret = a_data_req (net_info->acse_conn_id, pdu_len, pdu);
      if (ret != SD_SUCCESS)
        {
          MVL_LOG_NERR1 ("ACSE: a_data_req error '0x%x'", ret);
        }
    }
#endif
  return (ret);
  }

/************************************************************************/
/************************************************************************/
/*		DIRECTORY SERVICES FUNCTIONS				*/
/************************************************************************/
/************************************************************************/
/*                      find_loc_dib_entry                              */
/************************************************************************/
#if !defined(MAP30_ACSE)    /* For MAP30_ACSE use SUIC function instead */
DIB_ENTRY *find_loc_dib_entry (ST_CHAR *name)
  {
ST_INT i;
DIB_ENTRY *de;

  for (i = 0; i < num_loc_dib_entries; i++)
    {
    de = &loc_dib_table[i];
    if (!strcmp (de->name, name))
      return (de);
    }
  return (NULL);
  }
#endif  /* !defined(MAP30_ACSE) */

/************************************************************************/
/*                      find_rem_dib_entry                              */
/************************************************************************/

#if defined(CLACSE) ||  (MMS_INIT_EN & REQ_EN)
DIB_ENTRY *find_rem_dib_entry (ST_CHAR *name)
  {
#if !defined(MAP30_ACSE)
ST_INT i;
#endif
DIB_ENTRY *de;

#if defined(MAP30_ACSE)
  /* For Marben stack use SUIC function					*/
  de = find_dib_entry (name);	/* searches first loc then rem tables	*/
  return (de);
#else   /* !defined(MAP30_ACSE) */
  for (i = 0; i < num_rem_dib_entries; i++)
    {
    de = &rem_dib_table[i];
    if (!strcmp (de->name, name))
      return (de);
    }
  return (NULL);
#endif  /* !defined(MAP30_ACSE) */
  }
#endif

/************************************************************************/
/*			mvl_dib_entry_to_app_ref			*/
/************************************************************************/

ST_VOID mvl_dib_entry_to_app_ref (DIB_ENTRY *de, APP_REF *appRef)
  {
ST_INT i;

  appRef->form = APP_REF_FORM2;
  appRef->ap_title_pres = de->AP_title_pres;
  if (appRef->ap_title_pres == SD_TRUE)
    {
    appRef->ap_title.form_2.num_comps = de->AP_title.num_comps;
    for (i = 0; i < de->AP_title.num_comps; ++i)
      appRef->ap_title.form_2.comps[i] = de->AP_title.comps[i];
    }
  appRef->ap_invoke_pres = de->AP_inv_id_pres;
  appRef->ap_invoke = de->AP_invoke_id;
  appRef->ae_qual_pres = de->AE_qual_pres;
  appRef->ae_qual.form_2 = de->AE_qual;
  appRef->ae_invoke_pres = de->AE_inv_id_pres;
  appRef->ae_invoke = de->AE_invoke_id;
  }

/************************************************************************/
/************************************************************************/
#if (MMS_INIT_EN != REQ_RESP_DIS)	/* COACSE SPECIFIC FUNCTIONS	*/
/************************************************************************/

ST_RET mvla_set_init_ar (ST_CHAR *ar_name)
  {
NET_BIND_CTRL *bc;
ST_INT i;

/* See if user want's to use the default AR */
 if (ar_name == NULL) 
   {
   _mvla_initiate_bc = NULL;
   return (SD_SUCCESS);
   }

  for (i = 0; i < mvl_cfg_info->num_bind_ctrl; ++i)
    {
    bc = &mvl_cfg_info->bind_ctrl[i];
    if (!strcmp (ar_name, bc->ar_name))
      {
      _mvla_initiate_bc = bc;
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }

/************************************************************************/
/*			mvl_initiate_req				*/
/************************************************************************/

#if (MMS_INIT_EN & REQ_EN)

ST_INT mvl_initiate_req (ST_CHAR *remAr,
			INIT_INFO *req_info, 
			INIT_INFO *resp_info,
			MVL_NET_INFO **net_info_out)
  {
MVL_REQ_PEND *req;
ST_INT rc;

  if (_mvl_comm_serve_active == SD_TRUE)
    {
    MVL_LOG_NERR0 ("Error: mvl_comm_serv active");
    return (SD_FAILURE);
    }

  rc = mvla_initiate_req (remAr, req_info, resp_info, net_info_out, &req);
  if (rc != SD_SUCCESS)
    return (rc);

  rc = mvl_wait_req_done (req);
  if (rc != SD_SUCCESS)
    {
    MVL_LOG_NERR0 ("ACSE: Initiate failed");
    }

  mvl_free_req_ctrl (req);        /* we are done with this req ctrl	*/
  return (rc);
  }

/************************************************************************/
/*			mvla_initiate_req				*/
/************************************************************************/

ST_INT mvla_initiate_req (ST_CHAR *remAr,
			INIT_INFO *req_info, 
			INIT_INFO *resp_info,
			MVL_NET_INFO **net_info_out,
			MVL_REQ_PEND **req_out)
  {
ST_INT rc;
MVL_NET_INFO *cc;
DIB_ENTRY *locDe;
DIB_ENTRY *remDe;
AARQ_APDU aarq_apdu;
ST_INT32 acse_conn_id;
MVL_REQ_PEND *req;

  *req_out = NULL;			/* make sure it's initialized	*/

  MVL_LOG_ACSE1 ("ACSE: Initiate Request to '%s'", remAr);
  MVL_LOG_ERR0 ("############################## yyp i am in mvla_initiate_req");
  /* Get a calling connection control structure					*/

  cc = ma_get_calling_conn_ctrl (_mvla_initiate_bc);
  if (!cc)
    {
    MVL_LOG_ERR0 ("Error - Could not get calling MVL_NET_INFO");
    return (MVL_ERR_NO_CONN_CTRL);
    }

  req = _mvl_get_req_ctrl (cc, MMSOP_INITIATE);
  if (!req)
    {
    cc->in_use = SD_FALSE;
    return (MVL_ERR_NO_REQ_CTRL);
    }
  *req_out = req;
  req->invoke_id = -1;

  rc = mpl_init (req_info);
  if (rc == SD_SUCCESS)
    {
   /* We can log here if so desired */
    if ((mms_debug_sel & MMS_LOG_REQ) && ml_log_req_info_fun != NULL)
        (*ml_log_req_info_fun) (0, MMSOP_INITIATE, req_info);

  /* Save the output pointer in the MMS request control element		*/
    req->u.init.resp_info = resp_info;

  /* Save our initiate request information ... */
    memcpy (&cc->locl_init_info, req_info, sizeof (INIT_INFO));

  /* Retrieve the DIB_ENTRY's for local and remote. These are used to set	*/
  /* the AARQ_APDU information						*/
    if (_mvla_initiate_locDe != NULL)
      locDe = _mvla_initiate_locDe;
    else
      locDe = cc->bind_ctrl->de;

    remDe = find_rem_dib_entry (remAr);
    if (remDe)
      {  
    /* Now set up all AARQ_APDU information based on DIB_ENTRY information	*/
    
#if !defined(MAP30_ACSE)  /* most info not needed for Lite over Marben stack */
    /* AP Context is MMS */
      aarq_apdu.ASO_context_name_pres = SD_TRUE;
      aarq_apdu.ASO_context_name.num_comps = 5;
      aarq_apdu.ASO_context_name.comps[0] = 1;
      aarq_apdu.ASO_context_name.comps[1] = 0;
      aarq_apdu.ASO_context_name.comps[2] = 9506;
      aarq_apdu.ASO_context_name.comps[3] = 2;
      aarq_apdu.ASO_context_name.comps[4] = 3;

    /* Calling AE Title */
      aarq_apdu.calling_ae_title.AP_title_pres = locDe->AP_title_pres;
      memcpy (&aarq_apdu.calling_ae_title.AP_title, 
    	  &locDe->AP_title, 
    	  sizeof (MMS_OBJ_ID));
    
      aarq_apdu.calling_ae_title.AE_qual_pres   = locDe->AE_qual_pres;
      aarq_apdu.calling_ae_title.AE_qual        = locDe->AE_qual;
      aarq_apdu.calling_ae_title.AP_inv_id_pres = locDe->AP_inv_id_pres;
      aarq_apdu.calling_ae_title.AP_inv_id      = locDe->AP_invoke_id;
      aarq_apdu.calling_ae_title.AE_qual_pres   = locDe->AE_qual_pres;
      aarq_apdu.calling_ae_title.AE_qual        = locDe->AE_qual;
      aarq_apdu.calling_ae_title.AE_inv_id_pres = locDe->AE_inv_id_pres;
      aarq_apdu.calling_ae_title.AE_inv_id      = locDe->AE_invoke_id;
  
    /* Called AE Title */
      aarq_apdu.called_ae_title.AP_title_pres = remDe->AP_title_pres;
      memcpy (&aarq_apdu.called_ae_title.AP_title, 
    	  &remDe->AP_title, 
    	  sizeof (MMS_OBJ_ID));
    
      aarq_apdu.called_ae_title.AE_qual_pres   = remDe->AE_qual_pres;
      aarq_apdu.called_ae_title.AE_qual        = remDe->AE_qual;
      aarq_apdu.called_ae_title.AP_inv_id_pres = remDe->AP_inv_id_pres;
      aarq_apdu.called_ae_title.AP_inv_id      = remDe->AP_invoke_id;
      aarq_apdu.called_ae_title.AE_qual_pres   = remDe->AE_qual_pres;
      aarq_apdu.called_ae_title.AE_qual        = remDe->AE_qual;
      aarq_apdu.called_ae_title.AE_inv_id_pres = remDe->AE_inv_id_pres;
      aarq_apdu.called_ae_title.AE_inv_id      = remDe->AE_invoke_id;
#endif  /* !defined(MAP30_ACSE) */
  
#ifdef REDUCED_STACK
  /* For Reduced Stack, the presentation address consists only of the 	*/
  /* ADLC address.							*/
      aarq_apdu.called_paddr.adlcAddr  = remDe->adlcAddr;
#endif

#ifdef LEAN_T
    /* For Trim 7, the presentation address has all the parts		*/
      memcpy (&aarq_apdu.called_paddr, &remDe->pres_addr, sizeof (PRES_ADDR));
#endif

    /* User Data */
      aarq_apdu.user_info.ptr = mmsl_msg_start;
      aarq_apdu.user_info.len = mmsl_msg_len;
  
      MVL_LOG_ACSEDATAH (mmsl_msg_len, mmsl_msg_start);
    
    /* Send the associate request						*/
#if defined(MAP30_ACSE)
      acse_conn_id = a_associate_reqm (cc->bind_ctrl->copp_bind_id, 
			(ST_LONG) cc, &aarq_apdu, 
			cc->bind_ctrl->ar_name, remAr);
#elif defined(LEAN_T)
      if (remDe->pres_addr.tp_type == TP_TYPE_TCP)
        acse_conn_id = a_associate_req (cc->bind_ctrl->copp_bind_id_tcp, (ST_INT32) cc, &aarq_apdu);
      else
        acse_conn_id = a_associate_req (cc->bind_ctrl->copp_bind_id_tp4, (ST_INT32) cc, &aarq_apdu);
#else
      acse_conn_id = a_associate_req (cc->bind_ctrl->copp_bind_id, (ST_INT32) cc, &aarq_apdu);
#endif
      if (!acse_conn_id)
        {
        MVL_LOG_NERR0 ("ACSE: a_associate_req failed");
        rc = MVL_ERR_ASSOC_REQ;
        }
      else
        ++mvl_mms_statistics.callingConn;
      }
    else
      {
      MVL_LOG_NERR1 ("ACSE: Remote name '%s' not found", remAr);
      rc = MVLE_REMOTE_NAME;
      }
    }
  else
    {
    MVL_LOG_NERR1 ("ACSE: mpl_init error '0x%x'", rc);
    }

  if (rc == SD_SUCCESS)
    *net_info_out = cc;
  else
    {
    cc->in_use = SD_FALSE;
    mvl_free_req_ctrl (req);
    *req_out = NULL;			/* So user CANNOT free it too.	*/
    }

/* All is good, return the connection control too			*/
  return (rc);
  }
/************************************************************************/
#endif /* #if (MMS_INIT_EN & REQ_EN)					*/
/************************************************************************/

/************************************************************************/
/*			mvl_abort_req					*/
/************************************************************************/

ST_INT mvl_abort_req (MVL_NET_INFO *cc)
  {
ABRT_APDU abrtinfo;
ST_INT ret;

  MVL_LOG_ACSE1 ("ACSE: sending abort for MVL_NET_INFO %08lx", cc);

  abrtinfo.source = 0;		/* user	*/
  abrtinfo.user_info.len = 0;

  ret = a_abort_req (cc->acse_conn_id, &abrtinfo);
  if (ret == SD_SUCCESS) 
    {
    ++mvl_mms_statistics.localAbort;
    cc->in_use = SD_FALSE;
    cc->conn_active = SD_FALSE;
    if (_mvlu_rpt_disconnect_rcvd_fun != NULL)
       (*_mvlu_rpt_disconnect_rcvd_fun)(cc);
    _mvl_set_net_req_done (NULL, cc, MVL_ERR_CNF_DISCONNECTED);
    }
  else
    {
    MVL_LOG_NERR1 ("ACSE: Abort error '0x%x'", ret);
    }
  return (ret);
  }

/************************************************************************/
/*                       mvl_release_req					*/
/*			 mvla_release_req				*/
/************************************************************************/

ST_INT mvl_release_req (MVL_NET_INFO *cc) 
  {
  MVL_LOG_ERR0 ("ACSE: Using obsolete function 'mvl_release_req'");
  return (SD_FAILURE);
  }

ST_INT mvla_release_req (MVL_NET_INFO *cc, MVL_REQ_PEND **req_out)
  {
  MVL_LOG_ERR0 ("ACSE: Using obsolete function 'mvla_release_req'");
  return (SD_FAILURE);
  }

/************************************************************************/
/*			_mvla_release					*/
/************************************************************************/
/* Called from conclude confirm						*/

ST_VOID _mvl_release (MVL_REQ_PEND *req)
  {
RLRQ_APDU rlinfo;
ST_INT ret;
MVL_NET_INFO *net_info;

  net_info = req->net_info;
  req->op = MVL_MMSOP_RELEASE;	/* Change the opcode in the req ctrl	*/

  MVL_LOG_ACSE1 ("ACSE: sending release for MVL_NET_INFO %08lx", net_info);

  rlinfo.reason_pres = SD_TRUE;
  rlinfo.reason = 1;
  rlinfo.user_info.len = 0;
  ret = a_release_req (net_info->acse_conn_id, &rlinfo);
  if (ret != SD_SUCCESS)
    {
    MVL_LOG_NERR1 ("ACSE: Release error '0x%x'", ret);
    _mvl_set_req_done (req, SD_FAILURE);
    }
  else
    ++mvl_mms_statistics.clientConcl;
  }

/************************************************************************/
/************************************************************************/
/*			mvl_add_bind_ctrl				*/
/************************************************************************/

ST_RET mvl_add_bind_ctrl (NET_BIND_CTRL *bc)
  {
NET_BIND_CTRL *mvlBc;
ST_BOOLEAN activate;
ST_INT i;
ST_RET rc;

/* First let's check to see if we have room for one more */
  if (mvl_cfg_info->num_bind_ctrl >= mvl_cfg_info->max_bind_ctrl)
    {
    MVL_LOG_NERR0 ("Error: no availble bind control");
    return (SD_FAILURE);
    }

/* Let's see if this local AR has already been activated	*/
  activate = SD_TRUE;
  for (i = 0; i < mvl_cfg_info->num_bind_ctrl; ++i)
    {
    if (!strcmp (mvl_cfg_info->bind_ctrl[i].ar_name, bc->ar_name))
      {
      activate = SD_FALSE;
      break;
      }
    }

/* OK, now we can copy the bind control information */
  
  mvlBc = &mvl_cfg_info->bind_ctrl[mvl_cfg_info->num_bind_ctrl];
  *mvlBc = *bc;
  ++mvl_cfg_info->num_bind_ctrl;
  
  rc = ma_bind (mvlBc, activate);
  return (rc);
  }

/************************************************************************/
/*			mvl_bind					*/
/************************************************************************/

static ST_RET ma_bind (NET_BIND_CTRL *bc, ST_BOOLEAN activate)
  {
#if !defined(MAP30_ACSE)
DIB_ENTRY *de;
PRES_ADDR loc_pres_addr;
#endif
ST_INT ret;
ST_INT numConn;
MVL_NET_INFO *ni;
ST_INT j;

  MVL_LOG_ACSE1 ("ACSE: binding local name '%s'", bc->ar_name);

#if !defined(MAP30_ACSE)
/* Locate the local DIB entry for this name, so we can find the addr	*/
  de = find_loc_dib_entry (bc->ar_name);
  if (!de)
    {
    MVL_LOG_ERR1 ("Error: Local AR name '%s' not found", bc->ar_name);
    return (MVLE_LOCAL_NAME);
    }
  bc->de = de;
#endif

  if (((bc->num_calling + mvl_cfg_info->calling_used) > mvl_cfg_info->num_calling) ||
      ((bc->num_called + mvl_cfg_info->called_used) > mvl_cfg_info->num_called))
    {							       
    MVL_LOG_ERR0 ("Error: not enough connections");
    return (MVLE_NUM_CONNECTIONS);
    }
  numConn = bc->num_calling + bc->num_called;

#ifdef REDUCED_STACK
/* For Reduced Stack, the presentation address consists only of the 	*/
/* ADLC address.							*/
  loc_pres_addr.adlcAddr = de->adlcAddr;
/* We make the binds synchronous */
  ma_cmd_done = SD_FALSE;
  ma_cmd_rslt = REQ_INCOMPLETE;
  ret = copp_bind ((ST_INT32) bc, &loc_pres_addr, SD_FALSE, numConn);
  ma_wait_req_done (&ret);
  if (ret != SD_SUCCESS)
    {
    MVL_LOG_ERR2 ("Error: copp_bind (TCP) of AR '%s' returned '0x%x'", 
  				bc->ar_name, ret);
    return (ret);
    }
#endif

#ifdef LEAN_T
/* For Trim 7, the presentation address has all the parts		*/
  memcpy (&loc_pres_addr, &de->pres_addr, sizeof (PRES_ADDR));
  if (de->pres_addr.tp_type == TP_TYPE_TPX)
    {				/* do 2 binds	*/
    /* Bind for TP4 (overwrite the "tp_type" in loc_pres_addr).	*/
    bc->tp_type = loc_pres_addr.tp_type = TP_TYPE_TP4;
    ma_cmd_done = SD_FALSE;
    ma_cmd_rslt = REQ_INCOMPLETE;
    ret = copp_bind ((ST_INT32) bc, &loc_pres_addr, SD_FALSE, numConn);
    ma_wait_req_done (&ret);
    if (ret != SD_SUCCESS)
      {
      MVL_LOG_ERR2 ("Error: copp_bind (TP4) of AR '%s' returned '0x%x'", 
    				bc->ar_name, ret);
      return (ret);
      }

    /* Bind for TCP (overwrite the "tp_type" in loc_pres_addr).	*/
    bc->tp_type = loc_pres_addr.tp_type = TP_TYPE_TCP;
    ret = copp_bind ((ST_INT32) bc, &loc_pres_addr, SD_FALSE, numConn);
    ma_wait_req_done (&ret);
    if (ret != SD_SUCCESS)
      {
      MVL_LOG_ERR2 ("Error: copp_bind (TCP) of AR '%s' returned '0x%x'", 
    				bc->ar_name, ret);
      return (ret);
      }
    }
  else	/* Not TPX */
    {
    bc->tp_type = loc_pres_addr.tp_type;
    ma_cmd_done = SD_FALSE;
    ma_cmd_rslt = REQ_INCOMPLETE;
    ret = copp_bind ((ST_INT32) bc, &loc_pres_addr, SD_FALSE, numConn);
    ma_wait_req_done (&ret);
    if (ret != SD_SUCCESS)
      {
      MVL_LOG_ERR2 ("Error: copp_bind of AR '%s' returned '0x%x'", 
    				bc->ar_name, ret);
      return (ret);
      }
    }
#endif

#if defined(MAP30_ACSE)
  ma_cmd_done = SD_FALSE;
  ma_cmd_rslt = REQ_INCOMPLETE;
  ret = _copp_bind ((ST_LONG) bc, bc->ar_name, bc->num_calling, bc->num_called, activate);
  ma_wait_req_done (&ret);
  if (ret != SD_SUCCESS)
    {
    MVL_LOG_ERR2 ("Error: copp_bind of AR '%s' failed '0x%x'", 
    				bc->ar_name, ret);
    return (ret);
    }
#endif  /* defined(MAP30_ACSE) */

  MVL_LOG_ACSE0 ("ACSE: bind OK");


/* OK, now attach the bind controls to the MVL_NET_INFO */      
  ni = mvl_called_conn_ctrl + mvl_cfg_info->called_used;
  for (j = 0; j < bc->num_called; ++j, ++ni)
    ni->bind_ctrl = bc;

  ni = mvl_calling_conn_ctrl + mvl_cfg_info->calling_used;
  for (j = 0; j < bc->num_calling; ++j, ++ni)
    ni->bind_ctrl = bc;

  mvl_cfg_info->calling_used += bc->num_calling;
  mvl_cfg_info->called_used += bc->num_called;
  return (ret);
  }

/************************************************************************/
/*			copp_unbind_sync				*/
/************************************************************************/

static ST_VOID copp_unbind_sync (ST_LONG copp_bind_id)
  {
ST_RET ret;

  ma_cmd_done = SD_FALSE;
  ma_cmd_rslt = REQ_INCOMPLETE;
  ret = copp_unbind (copp_bind_id);
  if (ret != SD_SUCCESS)
    {
    MVL_LOG_ERR1 ("Error: unbind result '0x%x'", ret);
    }
  else /* Now just wait for the 'confirm' function to be called		*/
    ma_wait_req_done (&ret);
  }

/************************************************************************/
/* 		     COACSE USER FUNCTIONS	       			*/
/************************************************************************/
/************************************************************************/
/*                       u_copp_bind_cnf				*/
/************************************************************************/

ST_VOID u_copp_bind_cnf (ST_INT32 user_bind_id, ST_INT32 copp_bind_id, ST_INT result)
  {
NET_BIND_CTRL *bc;

/* Save the acse interface's bind ID for future use */
  bc = (NET_BIND_CTRL *) user_bind_id;
#ifdef LEAN_T
  if (bc->tp_type == TP_TYPE_TP4)
    bc->copp_bind_id_tp4 = copp_bind_id;
  else
    bc->copp_bind_id_tcp = copp_bind_id;
#else
  bc->copp_bind_id = copp_bind_id;
#endif

/* Allow sync binds to complete */
  ma_cmd_done = SD_TRUE;
  ma_cmd_rslt = result;
  }

/************************************************************************/
/*                       u_copp_unbind_cnf				*/
/************************************************************************/

ST_VOID u_copp_unbind_cnf (ST_INT32 user_bind_id)
  {
/* Allow the request function to complete */
  ma_cmd_done = SD_TRUE;
  ma_cmd_rslt = SD_SUCCESS;
  }

/************************************************************************/
/*                       u_a_associate_ind				*/
/************************************************************************/

ST_VOID u_a_associate_ind (ST_INT32 u_bind_id, ST_INT32 acse_conn_id, 
			   AARQ_APDU *info)
  {
#if (MMS_INIT_EN & RESP_EN)

MMSDEC_INFO rslt;
INIT_INFO *init_req; 
INIT_INFO init_resp; 
MVL_NET_INFO *cc;
ST_INT ret;
AARE_APDU reinfo;
ST_INT accept;
ST_BOOLEAN free_dec_info;

  ++mvl_mms_statistics.calledConn;

  MVL_LOG_ACSE0 ("ACSE: Associate Indication received");
  MVL_LOG_ACSEDATAH (info->user_info.len, info->user_info.ptr);

  free_dec_info = SD_FALSE;

/* Assume the worst ... */
  accept = SD_FALSE;
  reinfo.user_info.len = 0;

/* Get a 'called' connection control structure, if one is available 	*/
  cc = ma_get_called_conn_ctrl ((NET_BIND_CTRL *) u_bind_id);
  if (cc)
    {
    MVL_LOG_CACSE1 ("Using MVL_NET_INFO %08lx", cc);

    /* Copy the whole AARQ Info struct, for the user to see		*/
    memcpy (&cc->ass_ind_info, info, sizeof (AARQ_APDU));

    /* IMPORTANT: Save the ACSE connection ID before calling user funtions.*/
    cc->acse_conn_id = acse_conn_id;

    /* We need to parse the MMS Initiate PDU 				*/
    free_dec_info = SD_TRUE;
    _ms_mms_decode (info->user_info.ptr, info->user_info.len, &rslt);
    if (rslt.err_code == NO_DECODE_ERR && rslt.type == MMSINITREQ)
      {
      init_req = (INIT_INFO *) rslt.data_ptr;
      init_req->mms_p_context_pres = SD_TRUE;

   /* We can log here if so desired */
      if ((mms_debug_sel & MMS_LOG_IND) && ml_log_ind_info_fun != NULL)
          (*ml_log_ind_info_fun) (0, MMSOP_INITIATE, init_req);

#ifdef ICCP_LITE
      ret = mi_connect_ind_fun (cc, init_req);
#else
    /* Let the user have a look to see if they want to accept		*/
      if (u_mvl_connect_ind_fun)
        ret = (*u_mvl_connect_ind_fun) (cc, init_req);
      else
        ret = SD_SUCCESS;
#endif
      if (ret == SD_SUCCESS)
        {
    /* Build an initiate response PDU, negiotiating the calling init	*/
    /* parameters vs. what we support					*/

#ifdef NEGIOTIATE_INITIATE_PARAM
      /* Start with the default initiate response info			*/
        memcpy (&init_resp, mvl_init_resp_info, sizeof (INIT_INFO));
        if (init_req->version < init_resp.version)/* thiers is lower	*/
          init_resp.version = init_req->version;	/* accept lower version */
        if (init_req->max_segsize_pres)
          {
          if (init_req->max_segsize < init_resp.max_segsize)
            init_resp.max_segsize = init_req->max_segsize;  
          }
        if (init_req->maxreq_calling < init_resp.maxreq_calling)
          init_resp.maxreq_calling = init_req->maxreq_calling;
        if (init_req->maxreq_called < init_resp.maxreq_called)
          init_resp.maxreq_called = init_req->maxreq_called;
        if (init_req->max_nest_pres)
          {
          if (init_req->max_nest < init_resp.max_nest)
            init_resp.max_nest = init_req->max_nest;
          }
      /* These 'supported' bitstring parameters are set up in mmsop_en.c	*/
        memcpy (init_resp.serv_supp, m_service_resp, 11);
        init_resp.param_supp[0] = m_param[0] & init_req->param_supp[0];
        init_resp.param_supp[1] = m_param[1] & init_req->param_supp[1];
#else
        /* Just send back what the user wants to send */ 
        memcpy (&init_resp, mvl_init_resp_info, sizeof (INIT_INFO));
#endif

        ret = mpl_init_resp (&init_resp);
        if (ret == SD_SUCCESS)
          {
         /* We can log here if so desired */
          if ((mms_debug_sel & MMS_LOG_RESP) && ml_log_resp_info_fun != NULL)
            (*ml_log_resp_info_fun) (0, MMSOP_INITIATE, &init_resp);

          accept = SD_TRUE;
      
          cc->conn_active = SD_TRUE;
        
          /* Save the remote node's initiate information			*/
          memcpy (&cc->rem_init_info, init_req, sizeof (INIT_INFO));
          memcpy (&cc->locl_init_info, &init_resp, sizeof (INIT_INFO));
          cc->max_pdu_size = (ST_INT) init_resp.max_segsize;
          /* NOTE: maxreq_calling = num of outstanding indications	*/
          /* allowed at the calling app (similar for maxreq_called).	*/
          cc->maxpend_req = init_resp.maxreq_calling;
          cc->maxpend_ind = init_resp.maxreq_called;
          cc->numpend_req = 0;
          cc->numpend_ind = 0;
          }
        }

      if (ret != SD_SUCCESS)
        {
        MVL_LOG_NERR0 ("ACSE: User does not want connection");
        ret = mpl_init_err (8, 0);
        cc->in_use = SD_FALSE;
        }

      if (ret == SD_SUCCESS)
	{
        reinfo.user_info.len = mmsl_msg_len;
        reinfo.user_info.ptr = mmsl_msg_start;
	}
      }
    else
      {
      MVL_LOG_NERR0 ("ACSE: Initiate indication PDU parse error");
      cc->in_use = SD_FALSE;
      }
    }
  else
    {
    MVL_LOG_ERR0 ("Error - Could not get called MVL_NET_INFO");
    }
    
/* Initiate Response (or error) PDU is built, or len set to 0. 		*/
/* Set the remaining ACSE parameters and send the response		*/

/* Use the calling context as the responding, to be friendly		*/  
  reinfo.ASO_context_name_pres = info->ASO_context_name_pres; 
  memcpy (&reinfo.ASO_context_name, &info->ASO_context_name, 
  	  sizeof (MMS_OBJ_ID)); 
 
/* For now, just reflect the ACSE indication information 		*/
/* Note that it may (or may not) be more appropriate to use the 'local'	*/
/* DIB_ENTRY info for this purpose.					*/
  memcpy (&reinfo.responding_ae_title, &info->called_ae_title, 
            sizeof (AE_TITLE));

  if (accept)
    {
    MVL_LOG_ACSE0 ("Accepting the connection");
    reinfo.result = 0;	/* ACCEPT	*/
    reinfo.result_source_diag_pres = SD_FALSE;
    }
  else
    {
    MVL_LOG_ACSE0 ("Rejecting the connection");
    reinfo.result = 1;	/* REJECT	*/
    reinfo.result_source_diag_pres = SD_TRUE;
    reinfo.result_source = 1;
    reinfo.result_diag = 9;
    }

  /* Send ACSE response	*/
  MVL_LOG_ACSEDATAH (reinfo.user_info.len, reinfo.user_info.ptr);
  ret = a_associate_rsp (acse_conn_id, (ST_INT32) cc, &reinfo);
  if (ret != SD_SUCCESS)
    {
    MVL_LOG_NERR1 ("ACSE: a_associate_rsp error '0x%x", ret);
    cc->in_use = SD_FALSE;
    ++mvl_mms_statistics.calledConnFail;
    }
  else
    ++mvl_mms_statistics.calledConnOK;


  if (free_dec_info == SD_TRUE)
    M_FREE (MSMEM_DEC_OS_INFO, _mms_dec_info);

#else	/* !(MMS_INIT_EN & RESP_EN)	*/

AARE_APDU reinfo;
ST_LONG user_conn_id;
ST_INT ret;

  ++mvl_mms_statistics.calledConn;

/* Incoming connections not supported, so Reject the association.	*/
  MVL_LOG_ACSE0 ("ACSE: Associate Indication received. Rejecting.");

/* Just use the ASO-context from the request in the response.		*/
  reinfo.ASO_context_name_pres = info->ASO_context_name_pres;
  memcpy (&reinfo.ASO_context_name, &info->ASO_context_name,
  	  sizeof (MMS_OBJ_ID));
 
/* Just use the called AE-Title as the responding AE-Title.		*/
  memcpy (&reinfo.responding_ae_title, &info->called_ae_title,
          sizeof (AE_TITLE));

  /* Reject	*/
  reinfo.result = 1;		/* Rejected-permanent	*/
  reinfo.result_source_diag_pres = SD_TRUE;
  reinfo.result_source = 1;	/* Source = user.	*/
  reinfo.result_diag = 1;	/* no-reason-given	*/
  /* No User Information	*/
  reinfo.user_info.len = 0;
  reinfo.user_info.ptr = NULL;

  user_conn_id = 0;	/* id doesn't matter because rejecting	*/
  if ((ret = a_associate_rsp (acse_conn_id, user_conn_id, &reinfo))!=SD_SUCCESS)
    {
    MVL_LOG_ERR1 ("Error sending a_associate_rsp. Error = %d", ret);
    }
  ++mvl_mms_statistics.calledConnFail;
#endif	/* !(MMS_INIT_EN & RESP_EN)	*/
  }


/************************************************************************/
/*			u_a_associate_cnf				*/
/************************************************************************/

ST_VOID u_a_associate_cnf (ST_INT32 acse_conn_id, ST_INT32 user_conn_id,
			AARE_APDU *info)
  {
#if (MMS_INIT_EN & REQ_EN)
MMSDEC_INFO rslt;
MVL_NET_INFO *cc;
INIT_INFO *init_resp; 
MVL_REQ_PEND *req;
ST_BOOLEAN free_dec_info;
ST_RET result;

/* Need to save off the ACSE interface's connection ID for future use	*/
  cc = (MVL_NET_INFO *) user_conn_id;
  cc->acse_conn_id = acse_conn_id;

  MVL_LOG_ACSE1 ("ACSE: associate confirm for MVL_NET_INFO %08lx", cc); 

  req = _mvl_find_req_ctrl (cc, MMSOP_INITIATE, -1);
  if (!req)
    {
    MVL_LOG_ERR0 ("ACSE: Associate Confirm - no matching req ctrl");
    info->result = SD_FAILURE;		/* Cause disconnect		*/
    cc->in_use = SD_FALSE;
    return;
    }

  free_dec_info = SD_FALSE;
  result = info->result;

  if (info->result == SD_SUCCESS) 
    {
    cc->conn_active = SD_TRUE;

    MVL_LOG_ACSEDATAH (info->user_info.len, info->user_info.ptr);

  /* OK, now we need to parse the MMS Initiate PDU 			*/
    free_dec_info = SD_TRUE;
    _ms_mms_decode (info->user_info.ptr, info->user_info.len, &rslt);
    if (rslt.err_code == NO_DECODE_ERR)
      {
      switch (rslt.type)
        {
        case MMSINITRESP :
          init_resp = (INIT_INFO *) rslt.data_ptr;
          init_resp->mms_p_context_pres = SD_TRUE;

         /* We can log here if so desired */
          if ((mms_debug_sel & MMS_LOG_CONF) && ml_log_conf_info_fun != NULL)
            (*ml_log_conf_info_fun) (0, MMSOP_INITIATE, init_resp);

          if (req->u.init.resp_info != NULL)
            memcpy (req->u.init.resp_info, init_resp, sizeof (INIT_INFO));

          memcpy (&cc->rem_init_info, init_resp, sizeof (INIT_INFO));
          MVL_LOG_CACSE0 ("ACSE: initiate confirm OK"); 
          cc->max_pdu_size = init_resp->max_segsize;
          /* NOTE: maxreq_calling = num of outstanding indications	*/
          /* allowed at the calling app (similar for maxreq_called).	*/
          cc->maxpend_req = init_resp->maxreq_called;
          cc->maxpend_ind = init_resp->maxreq_calling;
          cc->numpend_req = 0;
          cc->numpend_ind = 0;
          ++mvl_mms_statistics.callingConnOk;
        break;
  
        case MMSINITERR	:
          result = SD_FAILURE;
          cc->in_use = SD_FALSE;
          MVL_LOG_NERR0 ("ACSE: initiate confirm error"); 
          ++mvl_mms_statistics.callingConnFail;
        break;
  
        default :
          MVL_LOG_NERR1 ("ACSE: associate confirm PDU wrong type (%d)", 
								rslt.type);
          result = SD_FAILURE;	
          cc->in_use = SD_FALSE;
          info->result = SD_FAILURE;	/* Cause disconnect		*/
          ++mvl_mms_statistics.callingConnFail;
        break;
        }   
      }
    else  /* PDU decode error, abort the connection			*/
      {
      MVL_LOG_NERR0 ("ACSE: associate confirm PDU Decode Error");
      result = SD_FAILURE;
      info->result = SD_FAILURE;	/* Cause disconnect		*/
      cc->in_use = SD_FALSE;
      ++mvl_mms_statistics.callingConnFail;
      }
    }
  else
    {
    MVL_LOG_NERR0 ("ACSE: associate confirm result != SD_SUCCESS");
    cc->in_use = SD_FALSE;
    ++mvl_mms_statistics.callingConnFail;
    }

  _mvl_set_req_done (req, result);

  if (free_dec_info == SD_TRUE && rslt.data_pres)
    M_FREE (MSMEM_DEC_OS_INFO, rslt.data_ptr);
#endif
  }

/************************************************************************/
/*                       u_a_data_ind					*/
/************************************************************************/

ST_VOID u_a_data_ind (ST_LONG user_conn_id, ST_UINT data_len, ST_UCHAR *data)
  {
MVL_NET_INFO *ni;
MVL_COMM_EVENT *commEvent;

  ni = (MVL_NET_INFO *) user_conn_id;

/* See if we are supposed to log ACSE data for this connection		*/
  if (mvl_conn_filtered_logging == SD_FALSE || ni->log_enable)
    {
    MVL_LOG_ACSEDATA2 ("ACSE: Rx'd %d bytes on MVL_NET_INFO 0x%08lx", 
					data_len, ni);
    MVL_LOG_ACSEDATAH (data_len, data);
    }

  commEvent = mvl_get_com_event ();
  if (!commEvent)
    {
    MVL_LOG_ERR0 ("ACSE: Error - could not get MVL_COMM_EVENT");
    u_a_except (user_conn_id, SD_FAILURE, __FILE__, __LINE__);
    return;
    }

  commEvent->event_type = MMS_MSG_RCVD;
  commEvent->net_info = ni;
  commEvent->u.mms.pdu_len = data_len;
  commEvent->u.mms.pdu = (ST_UCHAR *) M_MALLOC (MSMEM_RXPDU,data_len);
  memcpy (commEvent->u.mms.pdu, data, data_len);
  }

/************************************************************************/
/*                       u_a_release_ind			       	*/
/************************************************************************/

ST_VOID u_a_release_ind (ST_INT32 user_conn_id, RLRQ_APDU *info)
  {
RLRE_APDU reinfo;
MVL_NET_INFO *cc;

  MVL_LOG_ACSE0 ("ACSE: Release Indication received");
  ++mvl_mms_statistics.serverConcl;

  cc = (MVL_NET_INFO *) user_conn_id;
  cc->conn_active = SD_FALSE;
  cc->in_use = SD_FALSE;
  if (_mvlu_rpt_disconnect_rcvd_fun != NULL)
     (*_mvlu_rpt_disconnect_rcvd_fun)(cc);

  reinfo.reason_pres = SD_TRUE;
  reinfo.reason = 1;

/* Send no data on the release response */
  reinfo.user_info.len = 0;
  reinfo.user_info.ptr = NULL;
  a_release_rsp (cc->acse_conn_id, &reinfo);

/* Set any outstanding requests to 'done' */
  _mvl_set_net_req_done (NULL, cc, SD_FAILURE);

#ifdef ICCP_LITE
  if (cc->mi_in_use)
    mi_release_ind_fun (cc);
  else
    {
    if (u_mvl_disc_ind_fun)
      (*u_mvl_disc_ind_fun) (cc, MVL_ACSE_RELEASE_IND);
    }
#else
  if (u_mvl_disc_ind_fun)
    (*u_mvl_disc_ind_fun) (cc, MVL_ACSE_RELEASE_IND);
#endif

  cc->maxpend_req = 0;
  cc->numpend_req = 0;
  }

/************************************************************************/
/*                       u_a_abort_ind					*/
/************************************************************************/

ST_VOID u_a_abort_ind (ST_INT32 user_conn_id, ABRT_APDU *info)
  {
MVL_NET_INFO *cc;
ST_BOOLEAN conn_active;

  MVL_LOG_ACSE0 ("ACSE: Abort Indication received");
  ++mvl_mms_statistics.remoteAbort;

  cc = (MVL_NET_INFO *) user_conn_id;
  conn_active = cc->conn_active;
  cc->conn_active = SD_FALSE;
  cc->in_use = SD_FALSE;
  if (_mvlu_rpt_disconnect_rcvd_fun != NULL)
     (*_mvlu_rpt_disconnect_rcvd_fun)(cc);

/* Set any outstanding requests to 'done' */
  _mvl_set_net_req_done (NULL, cc, MVL_ERR_CNF_DISCONNECTED);
  if (conn_active == SD_TRUE)
    {
#ifdef ICCP_LITE
    if (cc->mi_in_use == SD_TRUE)
      mi_abort_ind_fun (cc);
    else
      {
      if (u_mvl_disc_ind_fun)
        (*u_mvl_disc_ind_fun) (cc, MVL_ACSE_ABORT_IND);
      }
#else
    if (u_mvl_disc_ind_fun)
      (*u_mvl_disc_ind_fun) (cc, MVL_ACSE_ABORT_IND);
#endif
    }

  cc->maxpend_req = 0;
  cc->numpend_req = 0;
  }

/************************************************************************/
/*                       u_a_release_cnf				*/
/************************************************************************/

ST_VOID u_a_release_cnf (ST_INT32 user_conn_id, RLRE_APDU *info)
  {
MVL_NET_INFO *cc;
MVL_REQ_PEND *req;


  cc = (MVL_NET_INFO *) user_conn_id;

  MVL_LOG_ACSE1 ("ACSE: release confirm received for MVL_NET_INFO %08lx", cc);

/* Find the conclude request */
  req = _mvl_find_req_ctrl (cc, MVL_MMSOP_RELEASE, -1);
  if (!req)
    {
    MVL_LOG_ERR0 ("ACSE: Release Confirm - no matching req ctrl");
    return;
    }

/* Set any outstanding requests to 'done' */
  _mvl_set_net_req_done (req, cc, MVL_ERR_CNF_DISCONNECTED);

  cc->conn_active = SD_FALSE;
  cc->in_use = SD_FALSE;
  if (_mvlu_rpt_disconnect_rcvd_fun != NULL)
     (*_mvlu_rpt_disconnect_rcvd_fun)(cc);

  _mvl_set_req_done (req, SD_SUCCESS);
  cc->maxpend_req = 0;
  cc->numpend_req = 0;
  }

/************************************************************************/
/*                       u_a_except					*/
/************************************************************************/

ST_VOID u_a_except (ST_INT32 user_conn_id, ST_INT code, 
		 ST_CHAR *filename, ST_INT line)
  {
  MVL_LOG_ERR2 ("ACSE: Error - Exception, code 0x%x, MVL_NET_INFO %08lx", 
			code, user_conn_id);
  MVL_LOG_CERR2 ("File %s, Line %d", filename, line);
  mvl_acse_ok = SD_FALSE;
  }

/************************************************************************/
/*		CONNECTION CONTROL RELATED STUFF			*/
/************************************************************************/
/*			ma_get_called_conn_ctrl				*/
/************************************************************************/

#if (MMS_INIT_EN & RESP_EN)

static MVL_NET_INFO *ma_get_called_conn_ctrl (NET_BIND_CTRL *bc)
  {
MVL_NET_INFO *ni;
ST_INT i;

  ni = mvl_called_conn_ctrl;
  for (i = 0; i < mvl_cfg_info->called_used; ++i, ++ni)
    {
    /* CRITICAL: don't reuse 'ni' until req_pend_list cleaned up from	*/
    /* last conn. May be waiting for user to call mvl_free_req_ctrl.	*/
    if (!ni->in_use && ni->req_pend_list==NULL)
      {
      if (bc == NULL || !strcmp (bc->ar_name, ni->bind_ctrl->ar_name))
        {
        ni->in_use = SD_TRUE;
        ni->conn_active = SD_FALSE;
        return (ni);
        }
      }
    }
  return (NULL);
  }

#endif

/************************************************************************/
/*			ma_get_calling_conn_ctrl			*/
/************************************************************************/
#if (MMS_INIT_EN & REQ_EN)

static MVL_NET_INFO *ma_get_calling_conn_ctrl (NET_BIND_CTRL *bc)
  {
  char tmpstr[200];
MVL_NET_INFO *ni;
ST_INT i;
      sprintf(tmpstr,"\n################ yyp  ma_get_calling_conn_ctrl yyy num=%d \n",mvl_cfg_info->calling_used );
      MVL_LOG_ERR0(tmpstr);
  ni = mvl_calling_conn_ctrl;
  for (i = 0; i < mvl_cfg_info->calling_used; ++i, ++ni)
    {
    /* CRITICAL: don't reuse 'ni' until req_pend_list cleaned up from	*/
    /* last conn. May be waiting for user to call mvl_free_req_ctrl.	*/
         sprintf(tmpstr, " i=%d bc-ar=%s ni-ai=%s ",i,bc->ar_name,ni->bind_ctrl->ar_name);
      MVL_LOG_ERR0(tmpstr); 
      MVL_LOG_ERR0("############ yyp how are you?????"); 
    if (!ni->in_use && ni->req_pend_list==NULL)
      {

      if (bc == NULL || !strcmp (bc->ar_name, ni->bind_ctrl->ar_name))
        {
        ni->in_use = SD_TRUE;
        ni->conn_active = SD_FALSE;
        return (ni);
        }
      }
    }
  return (NULL);
  }
#endif


/************************************************************************/
#endif	/* End of COACSE specific functions 				*/
/************************************************************************/

/************************************************************************/
/************************************************************************/
#if defined(CLACSE)	/* CONNECTIONLESS ACSE FUNCTIONS					*/
/************************************************************************/

/************************************************************************/
/*			mvl_get_com_event				*/
/************************************************************************/
/* This function is used to get a MVL_COMM_EVENT control buffer to be	*/
/* passed up to the caller of '_mvl_net_service'. 			*/

static MVL_NET_INFO *mvl_get_net_info (ST_VOID)
  {
MVL_NET_INFO *netInfo;

#if 1
  netInfo = (MVL_NET_INFO *) M_CALLOC (MSMEM_NETINFO, 1, sizeof (MVL_NET_INFO));  
  if (netInfo == NULL)
    {
    MVL_LOG_ERR0 ("Error: No MVL_NET_INFO available");
    return (NULL);
    }
#else
  if (mvl_net_info_avail_head)
    netInfo = (MVL_NET_INFO *) list_get_first((ST_VOID **) &mvl_net_info_avail_head);
  else
    {
    if (mvl_net_info_count >= mvl_max_net_info)
      {
      MVL_LOG_ERR0 ("Error: No MVL_NET_INFO available");
      return (NULL);
      }
    ++mvl_net_info_count;
    netInfo = (MVL_NET_INFO *) M_CALLOC (MSMEM_GEN, 1, sizeof (MVL_NET_INFO));  
    }
#endif

  return (netInfo);
  }

/************************************************************************/
/*			mvl_init_audt_addr				*/
/* User must pass pointer (AUDT_APDU *) to their struct.		*/
/* This function fills in the addressing info in the struct.		*/
/************************************************************************/

ST_RET mvl_init_audt_addr (AUDT_APDU *audt, ST_CHAR *locArName, ST_CHAR *remArName)
  {
DIB_ENTRY *locDe;
DIB_ENTRY *remDe;

/* Retrieve the DIB_ENTRY's for local and remote. These are used to set	*/
/* the AUDT_APDU information						*/

  locDe = find_loc_dib_entry (locArName);
  if (!locDe)
    {
    MVL_LOG_NERR1 ("ACSE: Local name '%s' not found", locArName);
    return (MVLE_LOCAL_NAME);
    }

  remDe = find_rem_dib_entry (remArName);
  if (!remDe)
    {
    MVL_LOG_NERR1 ("ACSE: Remote name '%s' not found", remArName);
    return (MVLE_REMOTE_NAME);
    }

  /* AP Context is MMS */
  audt->ASO_context_name_pres = SD_TRUE;
  audt->ASO_context_name.num_comps = 5;
  audt->ASO_context_name.comps[0] = 1;
  audt->ASO_context_name.comps[1] = 0;
  audt->ASO_context_name.comps[2] = 9506;
  audt->ASO_context_name.comps[3] = 2;
  audt->ASO_context_name.comps[4] = 3;

/* Calling Addressing information */
  audt->calling_ae_title.AP_title_pres = locDe->AP_title_pres;
  memcpy (&audt->calling_ae_title.AP_title, &locDe->AP_title, 
	  sizeof (MMS_OBJ_ID));

  audt->calling_ae_title.AE_qual_pres   = locDe->AE_qual_pres;
  audt->calling_ae_title.AE_qual        = locDe->AE_qual;
  audt->calling_ae_title.AP_inv_id_pres = locDe->AP_inv_id_pres;
  audt->calling_ae_title.AP_inv_id      = locDe->AP_invoke_id;
  audt->calling_ae_title.AE_qual_pres   = locDe->AE_qual_pres;
  audt->calling_ae_title.AE_qual        = locDe->AE_qual;
  audt->calling_ae_title.AE_inv_id_pres = locDe->AE_inv_id_pres;
  audt->calling_ae_title.AE_inv_id      = locDe->AE_invoke_id;

  memcpy (&audt->calling_paddr, &locDe->pres_addr, sizeof (PRES_ADDR));

/* Called Addressing information */
  audt->called_ae_title.AP_title_pres = remDe->AP_title_pres;
  memcpy (&audt->called_ae_title.AP_title, &remDe->AP_title, 
	  sizeof (MMS_OBJ_ID));
  audt->called_ae_title.AE_qual_pres   = remDe->AE_qual_pres;
  audt->called_ae_title.AE_qual        = remDe->AE_qual;
  audt->called_ae_title.AP_inv_id_pres = remDe->AP_inv_id_pres;
  audt->called_ae_title.AP_inv_id      = remDe->AP_invoke_id;
  audt->called_ae_title.AE_qual_pres   = remDe->AE_qual_pres;
  audt->called_ae_title.AE_qual        = remDe->AE_qual;
  audt->called_ae_title.AE_inv_id_pres = remDe->AE_inv_id_pres;
  audt->called_ae_title.AE_inv_id      = remDe->AE_invoke_id;

  memcpy (&audt->called_paddr, &remDe->pres_addr, sizeof (PRES_ADDR));

  audt->rem_mac_valid = SD_FALSE;	/* CRITICAL: must send to ALL-ES MAC*/

  return (SD_SUCCESS);
  }

/************************************************************************/
/*			mvl_free_net_info				*/
/************************************************************************/

static ST_VOID mvl_free_net_info (MVL_NET_INFO *netInfo)
  {
#if 1
  M_FREE (MSMEM_NETINFO, netInfo);
#else
  list_add_last((ST_VOID **) &mvl_net_info_avail_head, (ST_VOID *) netInfo);
#endif
  }

/************************************************************************/
#endif		/* End of CLACSE specific functions			*/
/************************************************************************/

/************************************************************************/
/************************************************************************/
/* UTILITY FUNCTIONS							*/
/************************************************************************/
/*                     ma_wait_req_done					*/
/************************************************************************/

static ST_VOID ma_wait_req_done (ST_RET *retOut)
  {
/* See if the call failed */
  if (*retOut != SD_SUCCESS)
    return;

/* Now just wait for the 'confirm' function to be called		*/
  while (ma_cmd_done != SD_TRUE)
    {
    copp_event ();
    if (u_mvl_check_timeout && (*u_mvl_check_timeout)())
      break;
    }

/* OK, the function is done or the user gave up */
  if (ma_cmd_done == SD_TRUE)
    *retOut = ma_cmd_rslt;
  else
    { /* User timeout */
    if (ma_cmd_rslt != SD_SUCCESS)
      *retOut = ma_cmd_rslt;
    else
      *retOut = SD_FAILURE;
    }
  }

/************************************************************************/
/************************************************************************/
/* AA OBJECT HANDLING FUNCTIONS						*/
/* These functions must be here because the sizeof MVL_NET_INFO		*/
/* varies depending on the stack selection.				*/
/************************************************************************/
/************************************************************************/
/*			mvl_aa_resize 					*/
/************************************************************************/

ST_VOID mvl_aa_resize (ST_INT max_num_var, ST_INT max_num_nvl, 
		       ST_INT max_num_jou)
  {
MVL_AA_OBJ_CTRL *aa_objs;
ST_INT i;

/* Do AA specific variables */
  for (i = 0; i < mvl_cfg_info->num_called; ++i)
    {
    aa_objs = (MVL_AA_OBJ_CTRL *) mvl_called_conn_ctrl[i].aa_objs;
    if (aa_objs == NULL)
      { 
      aa_objs = mvl_called_conn_ctrl[i].aa_objs = 
		(MVL_AA_OBJ_CTRL *) M_CALLOC (NULL, 1, sizeof(MVL_AA_OBJ_CTRL));
      }
    mvl_aa_resize_conn (aa_objs, max_num_var, max_num_nvl, max_num_jou);
    }

  for (i = 0; i < mvl_cfg_info->num_calling; ++i)
    {
    aa_objs = (MVL_AA_OBJ_CTRL *) mvl_calling_conn_ctrl[i].aa_objs;
    if (aa_objs == NULL)
      { 
      aa_objs = mvl_calling_conn_ctrl[i].aa_objs = 
		(MVL_AA_OBJ_CTRL *) M_CALLOC (NULL, 1, sizeof(MVL_AA_OBJ_CTRL));
      }
    mvl_aa_resize_conn (aa_objs, max_num_var, max_num_nvl, max_num_jou);
    }
  }

/************************************************************************/
/*			_mvl_clear_aa_objs 				*/
/************************************************************************/

ST_VOID _mvl_clear_aa_objs (MVL_AA_OBJ_CTRL *aa_objs)
  {
ST_INT i;

/* Delete NVL's. We need to delete the 'entries' pointer table for each	*/
/* NVL, then delete the table of NVL's.					*/
  if (aa_objs->max_num_nvlist)
    {
    for (i = 0; i < aa_objs->num_nvlist; ++i)
      mvl_nvl_destroy (aa_objs->nvlist_tbl[i]);
    }    

/* Delete the variable association table.				*/
  if (aa_objs->max_num_var_assoc)
    {
    for (i = 0; i < aa_objs->num_var_assoc; ++i)
      M_FREE (MSMEM_GEN, aa_objs->var_assoc_tbl[i]);
    }

/* Delete the Journal control table.					*/
  if (aa_objs->max_num_jou)
    {
    for (i = 0; i < aa_objs->num_jou; ++i)
      M_FREE (MSMEM_GEN, aa_objs->jou_tbl[i]);
    }

  aa_objs->num_var_assoc = 0;
  aa_objs->num_nvlist = 0;
  aa_objs->num_jou = 0;
  }

/************************************************************************/

ST_VOID mvl_aa_resize_conn (MVL_AA_OBJ_CTRL *aa_objs, ST_INT max_num_var, ST_INT max_num_nvl, 
		       ST_INT max_num_jou)
  {
  if (max_num_var < aa_objs->num_var_assoc)
    {
    MVL_LOG_NERR0 ("Resizing VMD: new max VA is less than current"); 
    max_num_var = aa_objs->num_var_assoc;
    }
  if (max_num_nvl < aa_objs->num_nvlist)
    {
    MVL_LOG_NERR0 ("Resizing VMD: new max NVL is less than current"); 
    max_num_nvl = aa_objs->num_nvlist;
    }
  if (max_num_jou < aa_objs->num_jou)
    {
    MVL_LOG_NERR0 ("Resizing VMD: new max JOU is less than current"); 
    max_num_jou = aa_objs->num_jou;
    }
    
  _mvl_resize_ptr_tbl (&(aa_objs->max_num_var_assoc), 
  		         (ST_VOID ***)&(aa_objs->var_assoc_tbl), max_num_var);
  _mvl_resize_ptr_tbl (&(aa_objs->max_num_nvlist), 
    		         (ST_VOID ***)&(aa_objs->nvlist_tbl), max_num_nvl);
  _mvl_resize_ptr_tbl (&(aa_objs->max_num_jou), 
    		         (ST_VOID ***)&(aa_objs->jou_tbl), max_num_jou);
  }
