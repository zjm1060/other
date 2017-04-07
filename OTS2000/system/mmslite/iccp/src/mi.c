/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 1999, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mi.c    						*/
/* PRODUCT(S)  : MI							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/16/02  RKR     49    Minor logging changes                        */
/* 07/07/02  GLB     48    Remove unreferenced mi_free_mvl_req_ctrl     */
/* 06/20/02  MDE     47    Now compare selectors for LEAN too		*/
/* 06/20/02  MDE     46    Worked on aborting process			*/
/* 05/22/02  MDE     45    Cannot call on server only connection	*/
/* 05/21/02  MDE     44    Fixed u_mi_assoc.._active for server, called */
/* 05/21/02  MDE     43    Initialize remote and assoc name fields	*/
/* 05/20/02  MDE     42    Fixed rebind errors for multiple local AR	*/
/* 05/20/02  MDE     41    Fixed leak on rebind				*/
/* 05/09/02  MDE     40    Added mi_rebind_remote and associated	*/
/* 05/03/02  MDE     39    Added mi_bind_remote				*/
/* 04/23/02  MDE     38    Fixed alloc 0				*/
/* 04/17/02  MDE     37    Minor logging changes			*/
/* 04/05/02  MDE     36    Added more FLOW logging			*/
/* 04/05/02  MDE     35    Don't send abort if idle, misc. minor 	*/
/* 04/05/02  MDE     34    Added MI_LOG_CFG string			*/
/* 03/26/02  MDE     33    New matching for LeanT			*/
/* 03/21/02  MDE     32    Added matching options galore 		*/
/* 03/14/02  NAV     31    Fix dumb mistakes in mi_match_remote         */
/* 03/07/02  MDE     30    Better handling of def. local AR (calling)	*/
/* 03/05/02  NAV     29    Set _mvla_initiate globals                   */
/* 02/28/02  NAV     28    MAP30_ACSE Only: modify rem matching         */
/* 02/25/02  MDE     27    Changes for updated MVL_CFG_INFO		*/
/* 02/25/02  MDE     26    Changed ifdefs from MARBEN to MAP30_ACSE	*/
/* 02/20/02  MDE     25    Changes for new MVL_CFG_INFO, multiple bind	*/
/* 02/07/02  MDE     24	   Added mi_abort_all_assoc			*/
/* 02/05/02  MDE     23	   Added mi_find_mvl_cfg_info			*/
/* 01/16/02  NAV     22    mi_assoc_start_client_done: return to valid	*/
/*			     state after param mismatch			*/
/* 11/26/01  MDE     21	   Cleaned up log string			*/
/* 11/19/01  NAV     20    Accomodate marben dib			*/
/* 11/07/01  MDE     19	   Added & enhanced a number of functions 	*/
/* 10/29/01  MDE     18	   Added 'mi_find remote'			*/
/* 10/29/01  MDE     17	   Moved assoc list under remote		*/
/* 06/28/01  RKR     16    defaulted mi_debug_sel = MILOG_ERR		*/
/* 03/27/01  ASK     15    fixed time calculation in mi_service         */
/* 12/07/00  RKR     14    correction in mi_connect_ind_fun             */
/* 10/16/00  JRB     13    Don't need to set funct ptrs in mi_initialize*/
/* 09/07/00  NAV     12    Keep track of next DsTs service required	*/
/* 08/30/00  RKR     11    Check _CTRL_ENABLE flag in mi_connect_ind_fun*/
/* 07/06/00  NAV     10	   Add MISEM support				*/    
/* 08/21/00  NAV     09    Corrections to mi_free_req_ctrl              */
/*                              declare mi_assoc_list                   */
/* 04/24/00  RKR     08    Added _mi_free_mvl_req_ctrl			*/
/* 04/20/00  RKR     07    Added logging				*/
/* 11/24/99  MDE     06    Fixed remote ar name logging			*/
/* 11/21/99  NAV     05    Add mi_op_err				*/
/* 04/12/99  MDE     04    Cleanup, logging				*/
/* 04/12/99  MDE     03    Multiple Remote AR usage, use selected local */
/*			   AR's AE title.				*/
/* 04/01/99  MDE     02    Added MI_ASSOC_CTRL and associated code	*/
/* 12/31/98  MDE     01    Created					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mi.h"
#include "misem.h"

#ifdef MAP30_ACSE
#include "acse2.h"
#endif

/* prototype found in marb_def.h - not part of lite! */
#ifdef MAP30_ACSE
DIB_ENTRY *find_dib_entry (ST_CHAR *name);
#endif

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

ST_UINT mi_debug_sel = MILOG_ERR;
#ifdef DEBUG_SISCO
ST_CHAR *_milog_err_logstr = "MILOG_ERR";
ST_CHAR *_milog_nerr_logstr = "MILOG_NERR";
ST_CHAR *_milog_flow_logstr = "MILOG_FLOW";
ST_CHAR *_milog_debug_logstr = "MILOG_DEBUG";
ST_CHAR *_milog_addr_logstr = "MILOG_ADDR";
ST_CHAR *_milog_cfg_logstr = "MILOG_CFG";
#endif

ST_RET mi_op_err = SD_SUCCESS;
ST_INT mi_wait_service_ms = 0;

ST_DOUBLE _mi_next_service_time = 0.0;

/************************************************************************/

static ST_VOID _mi_clean_assoc (MI_ASSOC_CTRL *mi_assoc, ST_INT reason);
static ST_VOID _mi_init_req_done (MVL_REQ_PEND *mvl_req);
static ST_CHAR *_mi_get_remote_ar (AARQ_APDU *ass_ind_info);

static ST_RET _mi_match_assoc (MVL_NET_INFO   *net_info, 
			       MI_ASSOC_CTRL **mi_assoc_out);
#ifdef MAP30_ACSE
static ST_INT _mi_match_local_addr (ASS_INFO *ass_info, DIB_ENTRY *dib_entry);
static ST_INT _mi_match_remote_addr (ASS_INFO *ass_info, DIB_ENTRY *dib_entry);
#else

static ST_INT _mi_match_local_addr (AARQ_APDU *ass_info, DIB_ENTRY *dib_entry);
static ST_INT _mi_match_remote_addr (AARQ_APDU *ass_info, DIB_ENTRY *dib_entry);
#endif

static ST_VOID _mi_match_mem (ST_INT *matchLevel,
		      ST_INT ind_sel_len, ST_UCHAR *ind_sel, 
		      ST_INT dib_sel_len, ST_UCHAR *dib_sel, 
		      ST_BOOLEAN must_match, ST_CHAR *sel_text);
static ST_VOID _mi_match_ap_title (ST_INT *matchLevel,
		      ST_BOOLEAN ind_pres, MMS_OBJ_ID *ind_apTitle, 
		      ST_BOOLEAN dib_pres, MMS_OBJ_ID *dib_apTitle,
		      ST_BOOLEAN must_match);
static ST_VOID _mi_match_ae_int (ST_INT *matchLevel,
		      ST_BOOLEAN ind_pres, ST_INT32 ind_val, 
		      ST_BOOLEAN dib_pres, ST_INT32 dib_val, 
		      ST_BOOLEAN must_match, ST_CHAR *itemText);
static ST_RET _mi_match_present_flags (ST_INT *matchLevelIo,
				       ST_CHAR *aeElText,
				       ST_BOOLEAN dibPres, 
				       ST_BOOLEAN remotePres,
				       ST_BOOLEAN *doCompareOut);

ST_VOID _mi_free_conn (MI_CONN *mi_conn);

static ST_VOID _mi_assoc_service_calling (MI_ASSOC_CTRL *mi_assoc);
static ST_VOID _mi_assoc_service_called (MI_ASSOC_CTRL *mi_assoc);
static ST_VOID _mi_assoc_start_client (MI_ASSOC_CTRL *mi_assoc);
static ST_VOID _mi_assoc_start_client_done (MI_REQ_CTRL *mi_req);
static ST_VOID _mi_terminate_assoc (MI_ASSOC_CTRL *mi_assoc);
static ST_VOID _mi_terminate_assoc_done (MVL_REQ_PEND *mvl_req);
static ST_VOID _mi_find_mvl_cfg_info (MVL_CFG_INFO *mvl_cfg_info, 
			              MI_REMOTE *mi_remote_filter);

ST_VOID _mi_associate (MI_ASSOC_CTRL *mi_assoc);
ST_VOID _mi_abort_assoc (MI_ASSOC_CTRL *mi_assoc);

/************************************************************************/

MI_REMOTE *mi_remote_list;

/************************************************************************/
/*			mi_service 					*/
/************************************************************************/

ST_VOID mi_service (ST_VOID)
  {
ST_DOUBLE cur_time;


  S_MI_LOCK_COMMON ();
  _mi_assoc_service ();

  cur_time = sGetMsTime ();

  if ((mi_wait_service_ms == 0) ||
      (cur_time >= _mi_next_service_time))
    {
    mi_wait_service_ms = _mis_dsts_service (&_mi_next_service_time);
    }
  else
    {
    _mi_next_service_time = (ST_INT)(cur_time - mi_wait_service_ms);
    }

  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/*			mi_initialize 					*/
/************************************************************************/


ST_RET mi_initialize (ST_VOID)
  {
  /* Nothing to initialize now.	*/
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			mi_terminate 					*/
/************************************************************************/

ST_RET mi_terminate (ST_VOID)
  {
  MI_LOG_FLOW0 ("MI terminate");
  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			mi_create_remote 				*/
/************************************************************************/

MI_REMOTE *mi_create_remote (ST_VOID)
  {
MI_REMOTE *mi_remote;

  S_MI_LOCK_COMMON ();
  mi_remote = (MI_REMOTE *)chk_calloc (1, sizeof (MI_REMOTE));
  mi_remote->name = "<unnamed>"; 
  
  list_add_last((ST_VOID **) &mi_remote_list, (ST_VOID *) mi_remote);
  S_MI_UNLOCK_COMMON ();
  return (mi_remote);  
  }

/************************************************************************/
/*			mi_find_remote 					*/
/************************************************************************/

MI_REMOTE *mi_find_remote (ST_CHAR *name)
  {
MI_REMOTE *mi_remote;

  mi_remote = mi_remote_list;
  while (mi_remote != NULL)
    {
    if (mi_remote->name != NULL && !strcmp (mi_remote->name, name))
      return (mi_remote);
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  return (NULL);
  }


/************************************************************************/
/*			mi_free_remote 					*/
/************************************************************************/

ST_VOID mi_free_remote (MI_REMOTE *mi_remote_sel)
  {
MI_REMOTE *mi_remote;
MI_REMOTE *next_mi_remote;

  S_MI_LOCK_COMMON ();
  mi_remote = mi_remote_list;
  while (mi_remote != NULL)
    {
    next_mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);

    if (mi_remote_sel == NULL || mi_remote == mi_remote_sel)
      {
      if (mi_remote->mis_vcc != NULL)
        mis_free_vcc (mi_remote->mis_vcc);
    
      if (mi_remote->mic_vcc != NULL)
        mic_free_vcc (mi_remote->mic_vcc);
    
      list_unlink ((ST_VOID **) &mi_remote_list, (ST_VOID *) mi_remote);
      chk_free (mi_remote);
      }  
    mi_remote = next_mi_remote;
    }
  
  S_MI_UNLOCK_COMMON ();
  }


/************************************************************************/
/*			mi_add_ae_to_remote 				*/
/************************************************************************/

ST_RET mi_add_ae_to_remote (MI_REMOTE *mi_remote, ST_CHAR *ar_name)
  {
  MI_LOG_NERR0 ("Obsolete function called: mi_add_ae_to_remote"); 
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*			mi_create_assoc 				*/
/************************************************************************/

MI_ASSOC_CTRL *mi_create_assoc (MI_REMOTE *mi_remote)
  {
MI_ASSOC_CTRL *mi_assoc;

  S_MI_LOCK_COMMON ();
  mi_assoc = (MI_ASSOC_CTRL *)chk_calloc (1, sizeof (MI_ASSOC_CTRL));
  mi_assoc->name = "<unnamed>"; 
/*  mi_assoc->param_check_state = MI_PARAM_UNKNOWN; done for us */

  list_add_last((ST_VOID **) &mi_remote->mi_assoc_list, (ST_VOID *) mi_assoc);
  mi_assoc->mi_remote = mi_remote;
  S_MI_UNLOCK_COMMON ();
  return (mi_assoc);  
  }

/************************************************************************/
/*			mi_find_assoc 				*/
/************************************************************************/

MI_ASSOC_CTRL *mi_find_assoc (MI_REMOTE *mi_remote, ST_CHAR *name)
  {
MI_ASSOC_CTRL *mi_assoc;

  mi_assoc = mi_remote->mi_assoc_list;
  while (mi_assoc != NULL)
    {
    if (mi_remote == NULL || mi_assoc->mi_remote == mi_remote)
      {
      if (mi_assoc->name != NULL && !strcmp (mi_assoc->name, name))
        return (mi_assoc);
      }
    mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
    }
  return (NULL);  
  }

/************************************************************************/
/*			mi_free_assoc 					*/
/************************************************************************/

ST_VOID mi_free_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
ST_INT i;

  S_MI_LOCK_COMMON ();
  for (i = 0; i < mi_assoc->num_remote_ar; ++i)
    chk_free (mi_assoc->remote_ar[i]);

  list_unlink ((ST_VOID **) &mi_assoc->mi_remote->mi_assoc_list, (ST_VOID *) mi_assoc);
  chk_free (mi_assoc);
  S_MI_UNLOCK_COMMON ();
  }

/************************************************************************/
/*			mi_free_remote_assoc 				*/
/************************************************************************/

ST_VOID mi_free_remote_assoc (MI_REMOTE *mi_remote_sel)
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;
MI_ASSOC_CTRL *next_mi_assoc;

  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {				      
    if (mi_remote_sel == NULL || mi_remote == mi_remote_sel)
      {
      mi_assoc = mi_remote->mi_assoc_list;
      while (mi_assoc)		      /* for each association ... 	*/
        {
        next_mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
        mi_free_assoc (mi_assoc);
	mi_assoc = next_mi_assoc;
        }
      mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
      }
    }
  }

/************************************************************************/
/*			mi_add_ae_to_assoc 				*/
/************************************************************************/

ST_RET mi_add_ae_to_assoc (MI_ASSOC_CTRL *mi_assoc, ST_CHAR *ar_name)
  {
  if (mi_assoc->num_remote_ar >= MI_MAX_AE_PER_VCC)
    return (SD_FAILURE);

  S_MI_LOCK_COMMON ();
  mi_assoc->remote_ar[mi_assoc->num_remote_ar] = chk_strdup (ar_name);
  ++mi_assoc->num_remote_ar;
  S_MI_UNLOCK_COMMON ();

  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			mi_enable_named_assoc 				*/
/************************************************************************/

ST_VOID mi_enable_named_assoc (ST_CHAR *remote_name, ST_CHAR *assoc_name)
  {
  _mi_set_named_assoc_state (remote_name, assoc_name, SD_TRUE);
  }

/************************************************************************/
/*			mi_disable_named_assoc 				*/
/************************************************************************/

ST_VOID mi_disable_named_assoc (ST_CHAR *remote_name, ST_CHAR *assoc_name)
  {
  _mi_set_named_assoc_state (remote_name, assoc_name, SD_FALSE);
  }

/************************************************************************/
/* 		_mi_set_named_assoc_state 				*/
/************************************************************************/

ST_VOID _mi_set_named_assoc_state (ST_CHAR *remote_name, ST_CHAR *assoc_name, ST_BOOLEAN enable)
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;

  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {				      /* If all remotes, or current one	*/
    if ((remote_name == NULL) || !strcmp (remote_name, mi_remote->name))
      {
      mi_assoc = mi_remote->mi_assoc_list;
      while (mi_assoc)		      /* for each association ... 	*/
        {			      /* If all assoc, or current one	*/
        if ((assoc_name == NULL) || !strcmp (assoc_name, mi_assoc->name))
          {
          if (enable == SD_TRUE)
            mi_enable_assoc (mi_assoc);
          else
            mi_disable_assoc (mi_assoc);
          }    
        mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
        }
      mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
      }
    }
  }

/************************************************************************/
/************************************************************************/
/*		mi_enable_remote_assoc 					*/
/************************************************************************/

ST_VOID mi_enable_remote_assoc (MI_REMOTE *mi_remote)
  {
  _mi_set_remote_assoc_state (mi_remote, SD_TRUE);
  }

/************************************************************************/
/*		mi_disable_remote_assoc 				*/
/************************************************************************/

ST_VOID mi_disable_remote_assoc (MI_REMOTE *mi_remote)
  {
  _mi_set_remote_assoc_state (mi_remote, SD_FALSE);
  }

/************************************************************************/
/* 		_mi_set_remote_assoc_state 				*/
/************************************************************************/

ST_VOID _mi_set_remote_assoc_state (MI_REMOTE *mi_remote_sel, ST_BOOLEAN enable)
  {
MI_ASSOC_CTRL *mi_assoc;
MI_REMOTE *mi_remote;

  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {				      
    if (mi_remote_sel == NULL || mi_remote == mi_remote_sel)
      {
      mi_assoc = mi_remote->mi_assoc_list;
      while (mi_assoc)
        {
        if (enable == SD_TRUE)
          mi_enable_assoc (mi_assoc);
        else
          mi_disable_assoc (mi_assoc);
    
        mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
        }
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }

/************************************************************************/
/************************************************************************/
/*			mi_enable_assoc 				*/
/************************************************************************/

ST_VOID mi_enable_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
  mi_assoc->ctrl_flags |= MI_ASSOC_CTRL_ENABLE;
  }

/************************************************************************/
/*			mi_disable_assoc 				*/
/************************************************************************/

ST_VOID mi_disable_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
  mi_assoc->ctrl_flags &= ~MI_ASSOC_CTRL_ENABLE;
  }


/************************************************************************/
/************************************************************************/
/*			mi_abort_all_assoc				*/
/************************************************************************/

ST_VOID mi_abort_all_assoc ()
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;

  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    mi_assoc = mi_remote->mi_assoc_list;
    while (mi_assoc)
      {
      mi_assoc->ctrl_flags &= ~MI_ASSOC_CTRL_ENABLE;
      switch (mi_assoc->state)
        {
        case MI_ASSOC_STATE_ACTIVE:      
        case MI_ASSOC_STATE_PARAM_CHECK: 
        case MI_ASSOC_STATE_INIT_REQ:    
        case MI_ASSOC_STATE_CONCLUDING:  
        case MI_ASSOC_STATE_START_CLIENT:
        case MI_ASSOC_STATE_START_SERVER:
        case MI_ASSOC_STATE_INIT_IND:
          mi_assoc->state = MI_ASSOC_STATE_ABORT;
        break;

        case MI_ASSOC_STATE_IDLE:
        case MI_ASSOC_STATE_BACKOFF:
        case MI_ASSOC_STATE_ABORT:
        case MI_ASSOC_STATE_ABORTING:    
        case MI_ASSOC_STATE_INVALID:     
        break;				/* nothing to do here ... */

        default:
          MI_LOG_NERR1 ("MI_ASSOC state invalid: %d", mi_assoc->state); 
          mi_assoc->state = MI_ASSOC_STATE_INVALID;
        break;
        }

      mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }


/************************************************************************/
/************************************************************************/

typedef struct
  {
  DBL_LNK l;
  ST_INT numCalled;
  ST_INT numCalling;
  ST_CHAR *arName;
  } _MI_CFG_LOCAL_AR;
static _MI_CFG_LOCAL_AR *_mi_local_ar_list;


static _MI_CFG_LOCAL_AR * _mi_add_local_ar (ST_CHAR *arName)
  {
_MI_CFG_LOCAL_AR *cfgAr;

/* See if it is already in the list */
  cfgAr = _mi_local_ar_list;
  while (cfgAr != NULL)
    {
    if (!strcmp (cfgAr->arName, arName))
      return (cfgAr);

    cfgAr = (_MI_CFG_LOCAL_AR *) list_get_next (_mi_local_ar_list, cfgAr);
    }

/* OK, we need to add another */
  cfgAr = (_MI_CFG_LOCAL_AR *)chk_calloc (1, sizeof (_MI_CFG_LOCAL_AR));
  cfgAr->arName = arName;
  list_add_last((ST_VOID **) &_mi_local_ar_list, cfgAr);
  return (cfgAr);
  }



/************************************************************************/
/*			_mi_clean_local_ar				*/
/************************************************************************/

static ST_VOID _mi_clean_local_ar ()
  {
_MI_CFG_LOCAL_AR *cfgAr;

  cfgAr = _mi_local_ar_list;
  while (_mi_local_ar_list != NULL)
    {
    cfgAr = (_MI_CFG_LOCAL_AR *) list_get_first (&_mi_local_ar_list);
    chk_free (cfgAr);
    }		       
  }

/************************************************************************/
/*			mi_bind_remote					*/
/************************************************************************/

ST_RET mi_bind_remote (MI_REMOTE *mi_remote)
  {
MVL_CFG_INFO mvlCfgInfo;
ST_INT i;
ST_RET rc;

  memset (&mvlCfgInfo, 0, sizeof (mvlCfgInfo));
  _mi_find_mvl_cfg_info (&mvlCfgInfo, mi_remote);
  if (mvlCfgInfo.num_bind_ctrl == 0)
    {
    MI_LOG_NERR1 ("mi_bind_remote error: no local AR for remote '%s'", 
		   mi_remote->name);
    return (SD_FAILURE);
    }

  rc = SD_SUCCESS;
  for (i = 0; i < mvlCfgInfo.num_bind_ctrl; ++i)
    {
    rc |= mvl_add_bind_ctrl (&mvlCfgInfo.bind_ctrl[i]);
    }
  return (rc);
  }

/************************************************************************/
/*			mi_rebind_remote				*/
/************************************************************************/

ST_RET mi_rebind_remote (MI_REMOTE *mi_remote, MVL_CFG_INFO *prevBindInfo)
  {
MVL_CFG_INFO mvlCfgInfo;
NET_BIND_CTRL *newBc;
NET_BIND_CTRL *oldBc;
ST_BOOLEAN change;
ST_INT i;
ST_RET rc;

  memset (&mvlCfgInfo, 0, sizeof (mvlCfgInfo));
  _mi_find_mvl_cfg_info (&mvlCfgInfo, mi_remote);
  if (mvlCfgInfo.num_bind_ctrl == 0)
    {
    MI_LOG_NERR1 ("mi_bind_remote error: no local AR for remote '%s'", 
		   mi_remote->name);
    return (SD_FAILURE);
    }

/* If the bind info has not changed, we have nothing to do */
  change = SD_FALSE;
  if (mvlCfgInfo.num_bind_ctrl == prevBindInfo->num_bind_ctrl)
    {
    for (i = 0; i < mvlCfgInfo.num_bind_ctrl; ++i)
      {
      newBc = &mvlCfgInfo.bind_ctrl[i];
      oldBc = &(prevBindInfo->bind_ctrl[i]);
      if (strcmp (newBc->ar_name, oldBc->ar_name) ||
          newBc->num_calling != oldBc->num_calling ||
          newBc->num_called != oldBc->num_called)
        {
        change = SD_TRUE;
	break;
	}
      }
    }
  else
    change = SD_TRUE;

  rc = SD_SUCCESS;
  if (change == SD_TRUE)
    {
/* OK, the bind information changed, we need to add the new bindings */
    for (i = 0; i < mvlCfgInfo.num_bind_ctrl; ++i)
      {
      rc |= mvl_add_bind_ctrl (&mvlCfgInfo.bind_ctrl[i]);
      }
    }
  chk_free (mvlCfgInfo.bind_ctrl);
  return (rc);
  }

/************************************************************************/
/*			mi_get_remote_bind_info				*/
/************************************************************************/

ST_VOID mi_get_remote_bind_info (MI_REMOTE *mi_remote, MVL_CFG_INFO *mvlCfgInfoOut)
  {
  memset (mvlCfgInfoOut, 0, sizeof (MVL_CFG_INFO));
  _mi_find_mvl_cfg_info (mvlCfgInfoOut, mi_remote);
  }

/************************************************************************/
/*			mi_find_mvl_cfg_info 				*/
/************************************************************************/

ST_VOID mi_find_mvl_cfg_info (MVL_CFG_INFO *mvl_cfg_info)
  {
  _mi_find_mvl_cfg_info (mvl_cfg_info, NULL);
  }

/************************************************************************/
/*			mi_free_mvl_cfg_info				*/
/************************************************************************/

ST_VOID mi_free_remote_bind_info (MVL_CFG_INFO *mvl_cfg_info)
  {
#if defined(MAP30_ACSE)
  if (mvl_cfg_info->num_bind_ctrl != 0)
    chk_free (mvl_cfg_info->bind_ctrl);
#endif
  }

/************************************************************************/
/*			_mi_find_mvl_cfg_info				*/
/************************************************************************/

static ST_VOID _mi_find_mvl_cfg_info (MVL_CFG_INFO *mvl_cfg_info, 
			              MI_REMOTE *mi_remote_filter)
  {
MI_REMOTE     *mi_remote;
MI_ASSOC_CTRL *mi_assoc;
_MI_CFG_LOCAL_AR *cfgAr;
ST_INT numLocalAr;
#if defined(MAP30_ACSE)
ST_INT i;
#endif

  memset (mvl_cfg_info, 0, sizeof (MVL_CFG_INFO));
  _mi_clean_local_ar (); /* Prevent leaks during reconfiguration	*/

/* Count the number of distinct local AR names used */
  mi_remote = mi_remote_list;
  while (mi_remote)		      /* for each remote ... 		*/
    {
    if (mi_remote_filter == NULL || mi_remote == mi_remote_filter)
      {
      mi_assoc = mi_remote->mi_assoc_list;
      while (mi_assoc)
        {
        cfgAr = _mi_add_local_ar (mi_assoc->local_ar);
      
        if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING)
          {
          ++mvl_cfg_info->num_calling;
      	  ++cfgAr->numCalling;
      	  }
        if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLED)
          {
          ++mvl_cfg_info->num_called;
       	  ++cfgAr->numCalled;
      	  }
      
        if (mi_assoc->max_pdu_size > mvl_cfg_info->max_msg_size)
          mvl_cfg_info->max_msg_size = mi_assoc->max_pdu_size;
      
        mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, 
        					    mi_assoc);
        }
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }

/* OK, now we build the bind information */
  numLocalAr = list_get_sizeof (_mi_local_ar_list);
#if defined(MAP30_ACSE)
  mvl_cfg_info->use_bind_ctrl = SD_TRUE;
  mvl_cfg_info->num_bind_ctrl = numLocalAr;
  mvl_cfg_info->max_bind_ctrl = numLocalAr;
  if (numLocalAr != 0)
    mvl_cfg_info->bind_ctrl = chk_calloc (numLocalAr, sizeof (NET_BIND_CTRL));

  i = 0;
  cfgAr = _mi_local_ar_list;
  while (cfgAr != NULL)
    {
    strcpy (mvl_cfg_info->bind_ctrl[i].ar_name, cfgAr->arName);
    mvl_cfg_info->bind_ctrl[i].num_calling = cfgAr->numCalling;
    mvl_cfg_info->bind_ctrl[i].num_called = cfgAr->numCalled;
    ++i;
    cfgAr = (_MI_CFG_LOCAL_AR *) list_get_next (_mi_local_ar_list, cfgAr);
    }
#else
  cfgAr = _mi_local_ar_list;
  if (cfgAr != NULL)
    strcpy (mvl_cfg_info->local_ar_name, cfgAr->arName);
#endif
  }


/************************************************************************/
/************************************************************************/
/*			mi_log_assoc 					*/
/************************************************************************/

ST_VOID mi_log_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
ST_INT i;

  SLOGCALWAYS1 ("Association '%s'", mi_assoc->name);

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING)
    SLOGCALWAYS0 ("  Calling");

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLED)
    SLOGCALWAYS0 ("  Called");

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT)
    SLOGCALWAYS0 ("  Client");

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_SERVER)
    SLOGCALWAYS0 ("  Server");

  SLOGCALWAYS1 ("  Local AR        : '%s'", mi_assoc->local_ar);

  for (i = 0; i < mi_assoc->num_remote_ar; ++i)
    SLOGCALWAYS1 ("  Remote AR       : '%s'", mi_assoc->remote_ar[i]);
    
  SLOGCALWAYS1 ("  Retry Time      : '%ld'", mi_assoc->connect_backoff_time);
  SLOGCALWAYS1 ("  Max MMS PDU Size: '%d'", mi_assoc->max_pdu_size);
  }

/************************************************************************/
/************************************************************************/
/* MI UTILITY FUNCTIONS 						*/
/************************************************************************/
#if 0
/************************************************************************/
/*			   _mi_free_mvl_req_ctrl			*/
/************************************************************************/

static ST_VOID _mi_free_mvl_req_ctrl (MVL_REQ_PEND *req)
  {
  mvl_free_req_ctrl (req);
  }
#endif
/************************************************************************/
/*			_mi_get_req_ctrl 				*/
/************************************************************************/

MI_REQ_CTRL *_mi_get_req_ctrl (MI_CONN *mi_conn, ST_VOID *req_ctrl)
  {
MI_REQ_CTRL *mi_req;
MVL_REQ_PEND *mvl_req;

  mvl_req = (MVL_REQ_PEND *)req_ctrl;
  mi_req = (MI_REQ_CTRL *) chk_calloc (1, sizeof (MI_REQ_CTRL));
  mi_req->mi_conn = mi_conn;
  mi_req->mvl_req = mvl_req;
  mvl_req->mi_req = mi_req;
  return (mi_req);
  }

/************************************************************************/
/*			_mi_req_done 					*/
/************************************************************************/

ST_VOID _mi_req_done (MI_REQ_CTRL *mi_req)
  {
MVL_REQ_PEND *mvl_req;

  mvl_req = mi_req->mvl_req;
  mi_req->done = SD_TRUE;
  if (mi_req->result == SD_SUCCESS)
    mi_req->result = mvl_req->result;

  if (mi_req->u_req_done)    	/* Call the user done function		*/
    (*mi_req->u_req_done)(mi_req);
  }

/************************************************************************/
/*			mi_free_req_ctrl 				*/
/************************************************************************/

ST_VOID mi_free_req_ctrl (MI_REQ_CTRL *req)
  {
  mvl_free_req_ctrl (req->mvl_req);
  chk_free (req);
  }


/************************************************************************/
/*			_mi_free_conn 					*/
/************************************************************************/

ST_VOID _mi_free_conn (MI_CONN *mi_conn)
  {
MI_REMOTE *mi_remote;

  mi_conn->net_info->mi_in_use = SD_FALSE;
  _mis_reset_conn_dsts (mi_conn);
  mi_remote = mi_conn->mi_remote;
  list_unlink ((ST_VOID **) &mi_remote->mi_conn_list, (ST_VOID *) mi_conn);
  chk_free (mi_conn);
  }

/************************************************************************/
/************************************************************************/
/*			_mi_get_type_id 				*/
/************************************************************************/

ST_INT _mi_get_type_id (ST_INT mi_type)
  {
  if (mi_type < MI_NUM_MI_TYPES)
    return (mi_type_to_type_id_tbl[mi_type]);
  
  MI_LOG_NERR1 ("MI Type out of range: %d", mi_type); 
  return (-1);
  }

/************************************************************************/
/*			_mi_get_mi_type 				*/
/************************************************************************/

ST_INT _mi_get_mi_type (ST_INT type_id)
  {
ST_INT i;

  for (i = 0; i < MI_NUM_MI_TYPES; ++i)
    {
    if (mi_type_to_type_id_tbl[i] == type_id)
      return (i);
    }

  MI_LOG_NERR1 ("type_id %d not matched to MI Type", type_id); 
  return (-1);
  }

/************************************************************************/
/*			_mi_set_bit 					*/
/************************************************************************/

ST_VOID _mi_set_bit (ST_UCHAR *bit_array, ST_INT bit_num)
  {
ST_INT byte_offset;
ST_UINT8 bit_mask;

  byte_offset = bit_num/8;
  bit_mask = 0x80;
  bit_mask >>= bit_num % 8;
  bit_array[byte_offset] |= bit_mask;
  }

/************************************************************************/
/*			_mi_get_block4_type_id 				*/
/************************************************************************/

ST_INT _mi_get_block4_type_id (ST_INT mi_type)
  {
  if (mi_type < MI_NUM_BLOCK4_TYPES)
    return (mi_block4_type_id_tbl[mi_type]);
  
  MI_LOG_NERR1 ("MI Block 4Type out of range: %d", mi_type); 
  return (-1);
  }

/************************************************************************/
/*			_mi_get_block4_mi_type 				*/
/************************************************************************/

ST_INT _mi_get_block4_mi_type (ST_INT type_id)
  {
ST_INT i;

  for (i = 0; i < MI_NUM_BLOCK4_TYPES; ++i)
    {
    if (mi_block4_type_id_tbl[i] == type_id)
      return (i);
    }

  MI_LOG_NERR1 ("type_id %d not matched to MI Block 4Type", type_id); 
  return (-1);
  }

/************************************************************************/
/*			_mi_get_block5_type_id 				*/
/************************************************************************/

ST_INT _mi_get_block5_type_id (ST_INT mi_type)
  {
  if (mi_type < MI_NUM_BLOCK5_TYPES)
    return (mi_block5_type_id_tbl[mi_type]);
  
  MI_LOG_NERR1 ("MI Block5 Type out of range: %d", mi_type); 
  return (-1);
  }

/************************************************************************/
/*			_mi_get_block5_mi_type 				*/
/************************************************************************/

ST_INT _mi_get_block5_mi_type (ST_INT type_id)
  {
ST_INT i;

  for (i = 0; i < MI_NUM_BLOCK5_TYPES; ++i)
    {
    if (mi_block5_type_id_tbl[i] == type_id)
      return (i);
    }

  MI_LOG_NERR1 ("type_id %d not matched to MI Block 5 Type", type_id); 
  return (-1);
  }

/************************************************************************/
/************************************************************************/
/* ASSOCIATION SERVICE							*/
/************************************************************************/
/************************************************************************/
/*			mi_assoc_service 				*/
/************************************************************************/

ST_VOID _mi_assoc_service ()
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;

  _mi_curr_sec_time = time (NULL);

  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    mi_assoc = mi_remote->mi_assoc_list;
    while (mi_assoc)
      {
      if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING)
      {
        _mi_assoc_service_calling (mi_assoc);
      }
      else if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLED)
      {
        _mi_assoc_service_called (mi_assoc);
      }
    
      mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }
  }


/************************************************************************/
/* 		_mi_assoc_service_calling 				*/
/************************************************************************/

static ST_VOID _mi_assoc_service_calling (MI_ASSOC_CTRL *mi_assoc)
  {
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_ENABLE)
    {
    switch (mi_assoc->state)
      {
      case MI_ASSOC_STATE_ACTIVE:      /* all is as should be 		*/
      case MI_ASSOC_STATE_PARAM_CHECK: /* waiting for read response 	*/
      case MI_ASSOC_STATE_INIT_REQ:    /* waiting for initiate response */
      case MI_ASSOC_STATE_CONCLUDING:  /* waiting for conclude complete */
      case MI_ASSOC_STATE_ABORTING:    /* waiting for abort complete 	*/
        return;		
      break;

      case MI_ASSOC_STATE_ABORT:
        _mi_abort_assoc(mi_assoc);
      break;

      case MI_ASSOC_STATE_BACKOFF:
        if (mi_assoc->time_to_initiate > _mi_curr_sec_time)
          return;		/* neeed to wait some more ... */
        /* Note: no break; must stay above IDLE! */

      case MI_ASSOC_STATE_IDLE:
        if ((mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT) == 0)
	  {
          MI_LOG_NERR1 ("Error: remote '%s calling association not set as client", mi_assoc->mi_remote->name); 
          mi_assoc->state = MI_ASSOC_STATE_INVALID;
	  }
	else
	  {
          _mi_associate (mi_assoc);
	  }
      break;

      case MI_ASSOC_STATE_START_CLIENT:
        _mi_assoc_start_client (mi_assoc);
      break;

      case MI_ASSOC_STATE_INVALID:     /* nothing to do here ... */
        return;
      break;

      case MI_ASSOC_STATE_START_SERVER: /* Valid for called only */
      case MI_ASSOC_STATE_INIT_IND:
      default:
        MI_LOG_NERR1 ("MI_ASSOC state invalid: %d", mi_assoc->state); 
        mi_assoc->state = MI_ASSOC_STATE_INVALID;
      break;
      }
    }
  else  /* not enabled */
    {
    switch (mi_assoc->state)
      {
      case MI_ASSOC_STATE_IDLE:
      case MI_ASSOC_STATE_CONCLUDING:  /* waiting for conclude complete */
      case MI_ASSOC_STATE_ABORTING:    /* waiting for abort complete 	*/
      case MI_ASSOC_STATE_PARAM_CHECK: /* waiting for read response 	*/
      case MI_ASSOC_STATE_INIT_REQ:    /* waiting for initiate response */
        return;		
      break;

      case MI_ASSOC_STATE_ACTIVE:      /* We need to conclude now	*/
        _mi_terminate_assoc (mi_assoc);
      break;

      case MI_ASSOC_STATE_START_CLIENT:
      case MI_ASSOC_STATE_ABORT:
        _mi_abort_assoc(mi_assoc);
      break;

      case MI_ASSOC_STATE_BACKOFF:
        mi_assoc->state = MI_ASSOC_STATE_IDLE;
        return;
      break;

      case MI_ASSOC_STATE_INVALID:     /* nothing to do here ... */
        return;
      break;

      case MI_ASSOC_STATE_START_SERVER: /* Valid for called only */
      case MI_ASSOC_STATE_INIT_IND:
      default:
        MI_LOG_NERR1 ("MI_ASSOC state invalid: %d", mi_assoc->state); 
        mi_assoc->state = MI_ASSOC_STATE_INVALID;
      break;
      }
    }
  }

/************************************************************************/
/* 		_mi_assoc_service_called 				*/
/************************************************************************/

static ST_VOID _mi_assoc_service_called (MI_ASSOC_CTRL *mi_assoc)
  {
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_ENABLE)
    {
    switch (mi_assoc->state)
      {
      case MI_ASSOC_STATE_ACTIVE:      
      case MI_ASSOC_STATE_IDLE:
      case MI_ASSOC_STATE_INIT_IND:
      case MI_ASSOC_STATE_CONCLUDING:  /* waiting for conclude complete */
      case MI_ASSOC_STATE_ABORTING:    /* waiting for abort complete 	*/
        return;		
      break;

      case MI_ASSOC_STATE_ABORT:
        _mi_abort_assoc(mi_assoc);
      break;

      case MI_ASSOC_STATE_START_CLIENT:
        _mi_assoc_start_client (mi_assoc);
      break;

      case MI_ASSOC_STATE_START_SERVER:
        mi_assoc->state = MI_ASSOC_STATE_ACTIVE;
        u_mi_association_active (mi_assoc);
      break;  

      case MI_ASSOC_STATE_INVALID:     /* nothing to do here ... */
      case MI_ASSOC_STATE_PARAM_CHECK:	
        return;
      break;

      case MI_ASSOC_STATE_INIT_REQ:    
      case MI_ASSOC_STATE_BACKOFF:
      default:
        MI_LOG_NERR1 ("MI_ASSOC state invalid: %d", mi_assoc->state); 
        mi_assoc->state = MI_ASSOC_STATE_INVALID;
      break;
      }
    }
  else  /* not enabled */
    {
    switch (mi_assoc->state)
      {
      case MI_ASSOC_STATE_IDLE:
      case MI_ASSOC_STATE_CONCLUDING:  /* waiting for conclude complete */
      case MI_ASSOC_STATE_ABORTING:    /* waiting for abort complete 	*/
        return;		
      break;

      case MI_ASSOC_STATE_ACTIVE:      /* We need to conclude now	*/
        _mi_terminate_assoc (mi_assoc);
      break;

      case MI_ASSOC_STATE_ABORT:
        _mi_abort_assoc(mi_assoc);
      break;

      case MI_ASSOC_STATE_INVALID:     /* nothing to do here ... */
        return;
      break;

      case MI_ASSOC_STATE_PARAM_CHECK: /* waiting for read response 	*/
      case MI_ASSOC_STATE_INIT_REQ:    /* waiting for initiate response */
      case MI_ASSOC_STATE_BACKOFF:
      case MI_ASSOC_STATE_INIT_IND:
      default:
        MI_LOG_NERR1 ("MI_ASSOC state invalid: %d", mi_assoc->state); 
        mi_assoc->state = MI_ASSOC_STATE_INVALID;
      break;
      }
    }
  }

/************************************************************************/
/************************************************************************/
/* CONNECTION CONTROL REQUEST FUNCTIONS					*/
/************************************************************************/
/************************************************************************/
/*			_mi_associate 					*/
/************************************************************************/

ST_VOID _mi_associate (MI_ASSOC_CTRL *mi_assoc)
  {
MI_REMOTE *mi_remote;
MI_CONN *mi_conn;
MVL_REQ_PEND *mvl_req;
MVL_NET_INFO *net_info;
ST_RET rc;

  mi_remote = mi_assoc->mi_remote;
  MI_LOG_FLOW1 ("Initiate association '%s'", mi_assoc->name);

/* Allocate a nice new connection control element */
  mi_conn = (MI_CONN *)chk_calloc (1, sizeof (MI_CONN));
  mi_conn->mi_remote = mi_remote;
  mi_conn->remote_ar = mi_assoc->remote_ar[mi_assoc->remote_ar_sel];

  memcpy (&mi_assoc->locl_init_info, 
  	  &mi_assoc->def_locl_init_info, sizeof (INIT_INFO));

  if (mi_assoc->local_ar != NULL)
    {
    MI_LOG_CFLOW1 ("Use local AR '%s'", mi_assoc->local_ar);
#ifdef MAP30_ACSE
    _mvla_initiate_locDe = find_dib_entry (mi_assoc->local_ar);
#else
    _mvla_initiate_locDe = find_loc_dib_entry (mi_assoc->local_ar);
#endif
    if ( _mvla_initiate_locDe == NULL)
      {
      MI_LOG_CFLOW0 ("AR not found, using local default AR");
      mvla_set_init_ar (NULL);
      }
    else
      mvla_set_init_ar (mi_assoc->local_ar);
    }
  else
    {
    MI_LOG_CFLOW0 ("mi_assoc->local_ar == NULL, using local default AR");
    _mvla_initiate_locDe = NULL;
    mvla_set_init_ar (NULL);
    }

/* Send the initiate request */
  MI_LOG_CFLOW1 ("Remote AR '%s'", mi_conn->remote_ar);

  rc = mvla_initiate_req (mi_conn->remote_ar, 
			&mi_assoc->locl_init_info, 
			&mi_assoc->rem_init_info,
			&net_info,
                        &mvl_req);

  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("Initiate request error : %04x", rc); 
    mi_assoc->time_to_initiate = _mi_curr_sec_time + 
    			         mi_assoc->connect_backoff_time;
    mi_assoc->state = MI_ASSOC_STATE_BACKOFF;
    _mi_set_next_rem_ar (mi_assoc);
    chk_free (mi_conn);
    return;
    }
  MI_LOG_CFLOW0 ("Initiate request issued OK");
  list_add_last((ST_VOID **) &mi_remote->mi_conn_list, (ST_VOID *) mi_conn);

  mi_conn->net_info = net_info;
  net_info->mi_in_use = SD_TRUE;
  net_info->mi_conn = mi_conn;

/* OK, the initiate request is on the way, now we need to set the MVL 	*/
/* done function to point to us, and get a MI request control 		*/

  mvl_req->u_req_done = _mi_init_req_done;

  mi_assoc->mi_conn = mi_conn;
  mi_conn->mi_assoc = mi_assoc;
  mi_assoc->state = MI_ASSOC_STATE_INIT_REQ;
  mi_assoc->param_check_state = MI_PARAM_UNKNOWN;
  mvl_req->v = mi_assoc;
  }

/************************************************************************/
/*			_mi_init_req_done 				*/
/************************************************************************/

static ST_VOID _mi_init_req_done (MVL_REQ_PEND *mvl_req)
  {
MI_ASSOC_CTRL *mi_assoc;

  mi_assoc = (MI_ASSOC_CTRL *)mvl_req->v;
  if (mvl_req->result == SD_SUCCESS)
    {
    _mi_assoc_start_client (mi_assoc);
    }
  else
    {
    MI_LOG_FLOW1 ("Initiate confirm error: %04x", mvl_req->result); 
    mi_assoc->time_to_initiate = time (NULL) + mi_assoc->connect_backoff_time;
    mi_assoc->state = MI_ASSOC_STATE_BACKOFF;
    _mi_set_next_rem_ar (mi_assoc);
    _mi_free_conn (mi_assoc->mi_conn);
    u_mi_association_inactive (mi_assoc, MI_ASSOC_FAILED_ASSOC_REQ);
    }
  mvl_free_req_ctrl (mvl_req);
  }

/************************************************************************/
/************************************************************************/
/*			_mi_terminate_assoc 				*/
/************************************************************************/

static ST_VOID _mi_terminate_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
MI_CONN *mi_conn;
MVL_REQ_PEND *mvl_req;
ST_RET rc;

  mi_conn = mi_assoc->mi_conn;

/* Send the conclude request */
  rc = mvla_concl (mi_conn->net_info, &mvl_req);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR1 ("Conclude confirm error: %04x, aborting", mvl_req->result); 
    _mi_abort_assoc(mi_assoc);
    return;
    }

  mvl_req->u_req_done = _mi_terminate_assoc_done;
  mi_assoc->state = MI_ASSOC_STATE_CONCLUDING;
  mvl_req->v = mi_assoc;
  }

/************************************************************************/
/*			_mi_terminate_assoc_done 				*/
/************************************************************************/

static ST_VOID _mi_terminate_assoc_done (MVL_REQ_PEND *mvl_req)
  {
MI_ASSOC_CTRL *mi_assoc;
MI_CONN *mi_conn;

  mi_assoc = (MI_ASSOC_CTRL *)mvl_req->v;
  mi_conn = mi_assoc->mi_conn;
  if (mvl_req->result == SD_SUCCESS)
    _mi_clean_assoc (mi_assoc, MI_ASSOC_INACTIVE_TERMINATE_CONF);
  else
    {
    MI_LOG_NERR1 ("Conclude confirm error: %04x", mvl_req->result); 
    if (mvl_req->result != MVL_ERR_CNF_DISCONNECTED)
      _mi_abort_assoc (mi_assoc);
    }
  mvl_free_req_ctrl (mvl_req);
  }

/************************************************************************/
/************************************************************************/
/*			_mi_abort_assoc 				*/
/************************************************************************/

ST_VOID _mi_abort_assoc (MI_ASSOC_CTRL *mi_assoc)
  {
MI_CONN *mi_conn;

  if (mi_assoc->state == MI_ASSOC_STATE_IDLE ||
      mi_assoc->state == MI_ASSOC_STATE_ABORTING ||
      mi_assoc->state == MI_ASSOC_STATE_BACKOFF ||
      mi_assoc->state == MI_ASSOC_STATE_INVALID)
    {
    return;
    }
  mi_conn = mi_assoc->mi_conn;
  mvl_abort_req (mi_conn->net_info);
  _mi_clean_assoc (mi_assoc, MI_ASSOC_INACTIVE_ABORT_REQ);
  }

/************************************************************************/
/************************************************************************/
/* CONNECTION CONTROL INDICATION FUNCTIONS				*/
/************************************************************************/
/************************************************************************/
/*			mi_connect_ind_fun 				*/
/************************************************************************/

ST_INT mi_connect_ind_fun (MVL_NET_INFO *net_info, INIT_INFO *init_ind_info)
  {
static INIT_INFO init_resp_info;
MI_REMOTE *mi_remote;
MI_CONN *mi_conn;
MI_ASSOC_CTRL *mi_assoc;
ST_RET rc;

/* OK, who is calling us? We need to match up a remote client ...	*/
/* and if not, let the user catch the association as a non-ICCP connect	*/
  rc = _mi_match_assoc (net_info, &mi_assoc);
  if (rc != SD_SUCCESS)
    {
    MI_LOG_NERR0 ("Initiate ind, MIS_VCC not matched ...");
    if (u_mvl_connect_ind_fun)
      rc = (*u_mvl_connect_ind_fun) (net_info, init_ind_info);
    else
      {
      rc = SD_FAILURE;
      u_mi_association_ind_err (MI_ASSOC_IND_ERR_NOREMOTE);
      }

    net_info->mi_in_use = SD_FALSE;
    return (rc);
    }

  mi_remote = mi_assoc->mi_remote;

  MI_LOG_CADDR2 ("Matched: Remote '%s', Asociation '%s'", 
  			mi_remote->name, mi_assoc->name);

  mi_conn = (MI_CONN *)chk_calloc (1, sizeof (MI_CONN));
  mi_conn->mi_remote = mi_remote;
  mi_conn->net_info = net_info;
  mi_conn->remote_ar = mi_assoc->remote_ar[mi_assoc->active_ar_name_index];
  net_info->mi_conn = mi_conn;
  net_info->mi_in_use = SD_TRUE;

/* If the remote node is a client of ours, allow ICCP activity to take 	*/
/* place								*/
  if (mi_remote->mis_vcc == NULL)
    {
    MI_LOG_NERR0 ("Initiate indication, no MIS_VCC defined for remote");
    chk_free (mi_conn);
    u_mi_association_ind_err (MI_ASSOC_IND_ERR_NOMISVCC);
    return (SD_FAILURE);
    }

  mi_assoc->mi_conn = mi_conn;
  mi_conn->mi_assoc = mi_assoc;
  mi_assoc->param_check_state = MI_PARAM_UNKNOWN;

/* We are ready to be a server on this connection */  
  _mis_start (mi_conn);

/* If we are a client on this association, we will need to start  	*/
/* after the initiate response is sent.					*/
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT)
    mi_assoc->state = MI_ASSOC_STATE_START_CLIENT;
  else
    mi_assoc->state = MI_ASSOC_STATE_START_SERVER;

  list_add_last((ST_VOID **) &mi_remote->mi_conn_list, (ST_VOID *) mi_conn);

/* Prepare the initiate response information */
  memcpy (&init_resp_info, 
   	  &mi_assoc->def_locl_init_info, sizeof (INIT_INFO));
  mvl_init_resp_info = &init_resp_info;

  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* General address matching control */
ST_BOOLEAN mi_match_allow_missing_ae_elements = SD_FALSE;
ST_BOOLEAN mi_match_allow_extra_ae_elements   = SD_TRUE;

/* Local address matching control */   
ST_BOOLEAN mi_match_local_ap_title     	= SD_FALSE;
ST_BOOLEAN mi_match_local_ae_qualifier 	= SD_FALSE;
ST_BOOLEAN mi_match_local_ap_invoke    	= SD_FALSE;
ST_BOOLEAN mi_match_local_ae_invoke    	= SD_FALSE;
ST_BOOLEAN mi_match_local_psel         	= SD_TRUE;
ST_BOOLEAN mi_match_local_ssel         	= SD_TRUE;
ST_BOOLEAN mi_match_local_tsel         	= SD_TRUE;
ST_BOOLEAN mi_match_local_net_addr     	= SD_FALSE;

/* Remote address matching control */
ST_BOOLEAN mi_match_remote_ap_title     = SD_TRUE;
ST_BOOLEAN mi_match_remote_ae_qualifier = SD_TRUE;
ST_BOOLEAN mi_match_remote_ap_invoke    = SD_FALSE;
ST_BOOLEAN mi_match_remote_ae_invoke    = SD_FALSE;
ST_BOOLEAN mi_match_remote_psel         = SD_TRUE;
ST_BOOLEAN mi_match_remote_ssel         = SD_TRUE;
ST_BOOLEAN mi_match_remote_tsel         = SD_TRUE;
ST_BOOLEAN mi_match_remote_net_addr     = SD_FALSE;

#define MI_MATCH_NOT	0
#define MI_MATCH_CLOSE	1
#define MI_MATCH_EXACT	2

/************************************************************************/
/*			_mi_match_assoc 				*/
/************************************************************************/


#ifdef MAP30_ACSE
#define _FIND_LOC_DIB_ENTRY(ar_name) find_dib_entry (ar_name)
#define _FIND_REM_DIB_ENTRY(ar_name) find_dib_entry (ar_name)
#else
#define _FIND_LOC_DIB_ENTRY(ar_name) find_loc_dib_entry (ar_name)
#define _FIND_REM_DIB_ENTRY(ar_name) find_rem_dib_entry (ar_name)
#endif

static ST_RET _mi_match_assoc (MVL_NET_INFO   *net_info, 
			       MI_ASSOC_CTRL **mi_assoc_out)
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;
MI_ASSOC_CTRL *bestMiAssoc;
ST_INT bestMatchLoc;
ST_INT bestMatchRem;
ST_INT locMatch;
ST_INT remMatch;
DIB_ENTRY *dib_entry;
ST_CHAR *remote_ar;
ST_INT i;
#ifdef MAP30_ACSE
ACSE_CONN *acse_conn;
ASS_INFO *ass_info;
#else
AARQ_APDU *ass_info;
#endif


  MI_LOG_ADDR0 ("Association Indication: find the target Remote/Association");
#ifdef MAP30_ACSE
  acse_conn = (ACSE_CONN *) net_info->acse_conn_id;
  ass_info = &s_ass_info[acse_conn->chan];
#else
  ass_info = &net_info->ass_ind_info;
#endif

  bestMatchLoc = MI_MATCH_NOT;
  bestMatchRem = MI_MATCH_NOT;

  mi_remote = mi_remote_list;
  while (mi_remote)
    {
    MI_LOG_CADDR1 ("  Looking at Remote '%s'", mi_remote->name);

  /* Look at all associations in this remote */
    mi_assoc = mi_remote->mi_assoc_list;
    while (mi_assoc)
      {
      MI_LOG_CADDR1 ("    Association '%s'", mi_assoc->name);
    /* First Make sure this association is available */
      if ((mi_assoc->state != MI_ASSOC_STATE_IDLE) ||
      	  ((mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLED) == 0) ||
          ((mi_assoc->ctrl_flags & MI_ASSOC_CTRL_SERVER) == 0) ||
          ((mi_assoc->ctrl_flags & MI_ASSOC_CTRL_ENABLE) == 0))
        {
        mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
        MI_LOG_CADDR0 ("      Not available");
        continue;
        }

    /* OK, this association is available, see if is the right one ... */
      dib_entry = _FIND_LOC_DIB_ENTRY (mi_assoc->local_ar);
      if (dib_entry)
        {
        locMatch = _mi_match_local_addr (ass_info, dib_entry);
        MI_LOG_CADDR1 ("        Local address match level %d", locMatch);
        if (locMatch == MI_MATCH_EXACT || locMatch == MI_MATCH_CLOSE)
          {
        /* OK, the local info matched, see if a remote matches */
          i = 0;
          while (i < mi_assoc->num_remote_ar)
            {
            remote_ar = mi_assoc->remote_ar[i];
            dib_entry = _FIND_REM_DIB_ENTRY (remote_ar);
            if (dib_entry)
	      {
              remMatch = _mi_match_remote_addr (ass_info, dib_entry);
              MI_LOG_CADDR1 ("        Remote address match level %d", remMatch);
              if (remMatch == MI_MATCH_EXACT || remMatch == MI_MATCH_CLOSE)
        	{
		if ((remMatch > bestMatchRem) ||
		    (remMatch == bestMatchRem && locMatch > bestMatchLoc)) 
		  {
                  MI_LOG_CADDR0 ("      * Best association match so far");
                  mi_assoc->active_ar_name_index = i;
  		  bestMiAssoc = mi_assoc;
		  bestMatchRem = remMatch;
		  bestMatchLoc = locMatch; 
		  }
        	}
              }
	    else
              {
              MI_LOG_NERR2 ("Remote DIB entry '%s' not found, used in association '%s'", 
        			remote_ar, mi_assoc->name);
    	      }
            i++;
            }
          }
        }
      else
        {
        MI_LOG_NERR2 ("Local DIB entry '%s' not found, used in association '%s'", 
        			mi_assoc->local_ar, mi_assoc->name);
	}
      mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
      }
    mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
    }

  if (bestMatchLoc == MI_MATCH_NOT || bestMatchRem == MI_MATCH_NOT)
    return (SD_FAILURE);

  *mi_assoc_out = bestMiAssoc;
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
#ifdef MAP30_ACSE
/************************************************************************/
/************************************************************************/
/*                      _mi_match_local_addr                                 */
/************************************************************************/

static ST_INT _mi_match_local_addr (ASS_INFO *ass_info, DIB_ENTRY *dib_entry)
  {
ST_INT matchLevel;

/* The best we can do ... */
  matchLevel = MI_MATCH_EXACT;

/* Compare local network address with dib entry */
  MI_LOG_CADDR1 ("      Local AR '%s'", dib_entry->name);

  if (ass_info->transport == TCP)
    {
    _mi_match_mem (&matchLevel, 
    		   strlen (ass_info->ip_addr), ass_info->ip_addr, 
		   strlen (dib_entry->ip_addr), dib_entry->ip_addr, 
		   mi_match_local_net_addr, "Local IP Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else if (ass_info->transport == TP4)
    {
    _mi_match_mem (&matchLevel,
    		   ass_info->net_addr_len,ass_info->net_addr, 
		   dib_entry->net_addr_len, dib_entry->net_addr, 
		   mi_match_local_net_addr, "Local Network Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else
    {
    MI_LOG_NERR1 ("Transport type problem: %d", ass_info->transport);
    return (MI_MATCH_NOT);
    }


/* Compare selectors with dib entry */
  /* PSEL */
  _mi_match_mem (&matchLevel, ass_info->p_sel_len, ass_info->p_sel, 
		 dib_entry->p_sel_len, dib_entry->p_sel, 
		 mi_match_local_psel, "PSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* SSEL */
  _mi_match_mem (&matchLevel, ass_info->s_sel_len, ass_info->s_sel, 
		 dib_entry->s_sel_len, dib_entry->s_sel, 
		 mi_match_local_ssel, "SSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* TSEL */
  _mi_match_mem (&matchLevel, ass_info->t_sel_len, ass_info->t_sel, 
		 dib_entry->t_sel_len, dib_entry->t_sel, 
		 mi_match_local_tsel, "TSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

/* Now compare AE Titles */
  /* AP Title */
  _mi_match_ap_title (&matchLevel,
		    ass_info->AP_title_pres, &ass_info->AP_title, 
		    dib_entry->AP_title_pres, &dib_entry->AP_title, 
		    mi_match_local_ap_title);
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Qualifier */
  _mi_match_ae_int (&matchLevel,
		    ass_info->AE_qual_pres, ass_info->AE_qual, 
		    dib_entry->AE_qual_pres, dib_entry->AE_qual, 
		    mi_match_local_ae_qualifier, "AE Qualifier");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AP Invoke ID */
  _mi_match_ae_int (&matchLevel,
		    ass_info->AP_inv_id_pres, ass_info->AP_invoke_id, 
		    dib_entry->AP_inv_id_pres, dib_entry->AP_invoke_id, 
		    mi_match_local_ap_invoke, "AP invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Invoke ID */  
  _mi_match_ae_int (&matchLevel,
		    ass_info->AE_inv_id_pres, ass_info->AE_invoke_id, 
		    dib_entry->AE_inv_id_pres, dib_entry->AE_invoke_id, 
		    mi_match_local_ae_invoke, "AE invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  return (matchLevel);	/* Close or exact */
  }


/************************************************************************/
/*			_mi_match_remote_addr				*/
/************************************************************************/

static ST_INT _mi_match_remote_addr (ASS_INFO *ass_info, DIB_ENTRY *dib_entry)
  {
ST_INT matchLevel;

/* The best we can do ... */
  matchLevel = MI_MATCH_EXACT;

/* Compare partner network address with dib entry */
  MI_LOG_CADDR1 ("      Remote AR '%s'", dib_entry->name);


  if (ass_info->transport == TCP)
    {
    _mi_match_mem (&matchLevel, 
    		   strlen (ass_info->part_ip_addr), ass_info->part_ip_addr, 
		   strlen (dib_entry->ip_addr), dib_entry->ip_addr, 
		   mi_match_remote_net_addr, "Remote IP Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else if (ass_info->transport == TP4)
    {
    _mi_match_mem (&matchLevel,
    		   ass_info->part_net_addr_len,ass_info->part_net_addr, 
		   dib_entry->net_addr_len, dib_entry->net_addr, 
		   mi_match_remote_net_addr, "Remote Network Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else
    {
    MI_LOG_NERR1 ("Transport type problem: %d", ass_info->transport);
    return (MI_MATCH_NOT);
    }


/* Compare selectors with dib entry */
  /* PSEL */
  _mi_match_mem (&matchLevel, ass_info->part_p_sel_len, ass_info->part_p_sel, 
		 dib_entry->p_sel_len, dib_entry->p_sel, 
		 mi_match_remote_psel, "PSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* SSEL */
  _mi_match_mem (&matchLevel, ass_info->part_s_sel_len, ass_info->part_s_sel, 
		 dib_entry->s_sel_len, dib_entry->s_sel, 
		 mi_match_remote_ssel, "SSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* TSEL */
  _mi_match_mem (&matchLevel, ass_info->part_t_sel_len, ass_info->part_t_sel, 
		 dib_entry->t_sel_len, dib_entry->t_sel, 
		 mi_match_remote_tsel, "TSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

      
/* Now compare AE Titles */
/* AP Title */
  _mi_match_ap_title (&matchLevel,
		    ass_info->part_AP_title_pres, &ass_info->part_AP_title, 
		    dib_entry->AP_title_pres, &dib_entry->AP_title, 
		    mi_match_remote_ap_title);
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Qualifier */
  _mi_match_ae_int (&matchLevel,
		    ass_info->part_AE_qual_pres, ass_info->part_AE_qual, 
		    dib_entry->AE_qual_pres, dib_entry->AE_qual, 
		    mi_match_remote_ae_qualifier, "AE Qualifier");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AP Invoke ID */
  _mi_match_ae_int (&matchLevel,
		    ass_info->part_AP_inv_id_pres, ass_info->part_AP_invoke_id, 
		    dib_entry->AP_inv_id_pres, dib_entry->AP_invoke_id, 
		    mi_match_remote_ap_invoke, "AP invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Invoke ID */  
  _mi_match_ae_int (&matchLevel,
		    ass_info->part_AE_inv_id_pres, ass_info->part_AE_invoke_id, 
		    dib_entry->AE_inv_id_pres, dib_entry->AE_invoke_id, 
		    mi_match_remote_ae_invoke, "AE invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  return (matchLevel);
  }


/************************************************************************/
/************************************************************************/
#else	/* #ifdef MAP30_ACSE */
/************************************************************************/
/*                 _mi_match_local_addr                                 */
/************************************************************************/

static ST_INT _mi_match_local_addr (AARQ_APDU *ass_info, DIB_ENTRY *dib_entry)
  {
ST_INT matchLevel;

/* The best we can do ... */
  matchLevel = MI_MATCH_EXACT;

/* Compare local network address with dib entry */
  MI_LOG_CADDR1 ("      Local AR '%s'", dib_entry->name);

#if 0
  if (ass_info->transport == TCP)
    {
    _mi_match_mem (&matchLevel, 
    		   strlen (ass_info->ip_addr), ass_info->ip_addr, 
		   strlen (dib_entry->ip_addr), dib_entry->ip_addr, 
		   mi_match_local_net_addr, "Local IP Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else if (ass_info->transport == TP4)
    {
    _mi_match_mem (&matchLevel,
    		   ass_info->net_addr_len,ass_info->net_addr, 
		   dib_entry->net_addr_len, dib_entry->net_addr, 
		   mi_match_local_net_addr, "Local Network Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else
    {
    MI_LOG_NERR1 ("Transport type problem: %d", ass_info->transport);
    return (MI_MATCH_NOT);
    }
#endif


/* Compare selectors with dib entry */
  /* PSEL */
  _mi_match_mem (&matchLevel, ass_info->called_paddr.psel_len, ass_info->called_paddr.psel, 
		 dib_entry->pres_addr.psel_len, dib_entry->pres_addr.psel, 
		 mi_match_local_psel, "PSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* SSEL */
  _mi_match_mem (&matchLevel, ass_info->called_paddr.ssel_len, ass_info->called_paddr.ssel, 
		 dib_entry->pres_addr.ssel_len, dib_entry->pres_addr.ssel,
		 mi_match_local_ssel, "SSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* TSEL */
  _mi_match_mem (&matchLevel, ass_info->called_paddr.tsel_len, ass_info->called_paddr.tsel, 
		 dib_entry->pres_addr.tsel_len, dib_entry->pres_addr.tsel,
		 mi_match_local_tsel, "TSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

/* Now compare AE Titles */
  /* AP Title */
  _mi_match_ap_title (&matchLevel,
		    ass_info->called_ae_title.AP_title_pres, &ass_info->called_ae_title.AP_title, 
		    dib_entry->AP_title_pres, &dib_entry->AP_title, 
		    mi_match_local_ap_title);
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Qualifier */
  _mi_match_ae_int (&matchLevel,
		    ass_info->called_ae_title.AE_qual_pres, ass_info->called_ae_title.AE_qual, 
		    dib_entry->AE_qual_pres, dib_entry->AE_qual, 
		    mi_match_local_ae_qualifier, "AE Qualifier");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AP Invoke ID */
  _mi_match_ae_int (&matchLevel,
		    ass_info->called_ae_title.AP_inv_id_pres, ass_info->called_ae_title.AP_inv_id, 
		    dib_entry->AP_inv_id_pres, dib_entry->AP_invoke_id, 
		    mi_match_local_ap_invoke, "AP invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Invoke ID */  
  _mi_match_ae_int (&matchLevel,
		    ass_info->called_ae_title.AE_inv_id_pres, ass_info->called_ae_title.AE_inv_id, 
		    dib_entry->AE_inv_id_pres, dib_entry->AE_invoke_id, 
		    mi_match_local_ae_invoke, "AE invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  return (matchLevel);	/* Close or exact */
  }


/************************************************************************/
/*			_mi_match_remote_addr				*/
/************************************************************************/

static ST_INT _mi_match_remote_addr (AARQ_APDU *ass_info, DIB_ENTRY *dib_entry)
  {
ST_INT matchLevel;

/* The best we can do ... */
  matchLevel = MI_MATCH_EXACT;

/* Compare partner network address with dib entry */
  MI_LOG_CADDR1 ("      Remote AR '%s'", dib_entry->name);


#if 0
  if (ass_info->transport == TCP)
    {
    _mi_match_mem (&matchLevel, 
    		   strlen (ass_info->part_ip_addr), ass_info->part_ip_addr, 
		   strlen (dib_entry->ip_addr), dib_entry->ip_addr, 
		   mi_match_remote_net_addr, "Remote IP Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else if (ass_info->transport == TP4)
    {
    _mi_match_mem (&matchLevel,
    		   ass_info->part_net_addr_len,ass_info->part_net_addr, 
		   dib_entry->net_addr_len, dib_entry->net_addr, 
		   mi_match_remote_net_addr, "Remote Network Address");
    if (matchLevel == MI_MATCH_NOT)
      return (MI_MATCH_NOT);
    }
  else
    {
    MI_LOG_NERR1 ("Transport type problem: %d", ass_info->transport);
    return (MI_MATCH_NOT);
    }

#endif

/* Compare selectors with dib entry */
  /* PSEL */
  _mi_match_mem (&matchLevel, ass_info->calling_paddr.psel_len, ass_info->calling_paddr.psel, 
		 dib_entry->pres_addr.psel_len, dib_entry->pres_addr.psel, 
		 mi_match_remote_psel, "PSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* SSEL */
  _mi_match_mem (&matchLevel, ass_info->calling_paddr.ssel_len, ass_info->calling_paddr.ssel, 
		 dib_entry->pres_addr.ssel_len, dib_entry->pres_addr.ssel,
		 mi_match_remote_ssel, "SSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* TSEL */
  _mi_match_mem (&matchLevel, ass_info->calling_paddr.tsel_len, ass_info->calling_paddr.tsel, 
		 dib_entry->pres_addr.tsel_len, dib_entry->pres_addr.tsel,
		 mi_match_remote_tsel, "TSEL");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

      
/* Now compare AE Titles */
/* AP Title */
  _mi_match_ap_title (&matchLevel,
		    ass_info->calling_ae_title.AP_title_pres, &ass_info->calling_ae_title.AP_title, 
		    dib_entry->AP_title_pres, &dib_entry->AP_title, 
		    mi_match_remote_ap_title);
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Qualifier */
  _mi_match_ae_int (&matchLevel,
		    ass_info->calling_ae_title.AE_qual_pres, ass_info->calling_ae_title.AE_qual, 
		    dib_entry->AE_qual_pres, dib_entry->AE_qual, 
		    mi_match_remote_ae_qualifier, "AE Qualifier");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AP Invoke ID */
  _mi_match_ae_int (&matchLevel,
		    ass_info->calling_ae_title.AP_inv_id_pres, ass_info->calling_ae_title.AP_inv_id, 
		    dib_entry->AP_inv_id_pres, dib_entry->AP_invoke_id, 
		    mi_match_remote_ap_invoke, "AP invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  /* AE Invoke ID */  
  _mi_match_ae_int (&matchLevel,
		    ass_info->calling_ae_title.AE_inv_id_pres, ass_info->calling_ae_title.AE_inv_id, 
		    dib_entry->AE_inv_id_pres, dib_entry->AE_invoke_id, 
		    mi_match_remote_ae_invoke, "AE invoke ID");
  if (matchLevel == MI_MATCH_NOT)
    return (MI_MATCH_NOT);

  return (matchLevel);
  }


/************************************************************************/
/************************************************************************/
#endif	/* Not MAP30_ACSE */
/************************************************************************/

/************************************************************************/
/*			_mi_match_ap_title				*/
/************************************************************************/

/* Set match level to MI_MATCH_CLOSE or MI_MATCH_NOT, or leave alone	*/

static ST_VOID _mi_match_ap_title (ST_INT *matchLevel,
		      ST_BOOLEAN ind_pres, MMS_OBJ_ID *ind_apTitle, 
		      ST_BOOLEAN dib_pres, MMS_OBJ_ID *dib_apTitle,
		      ST_BOOLEAN must_match)
  {
ST_CHAR *itemText = "AP Title";
ST_BOOLEAN doCompare;
ST_RET rc;

  rc = _mi_match_present_flags (matchLevel, itemText, dib_pres, 
  				ind_pres, &doCompare);
  if (rc != SD_SUCCESS)
    {
    if (must_match)
      {
      *matchLevel = MI_MATCH_NOT;
      return;
      }
    MI_LOG_CADDR1 ("        Ignoring %s", itemText);
    *matchLevel = MI_MATCH_CLOSE;
    return;
    }

/* OK, we either matched or are ignoring, see if we need to compare	*/
  if (doCompare)
    {
    if (asn1_objidcmp (ind_apTitle, dib_apTitle))
      {
      if (must_match)
        {
        MI_LOG_CADDR1 ("        %s mismatch", itemText);
        *matchLevel = MI_MATCH_NOT;
        return;
        }
      MI_LOG_CADDR1 ("        Ignoring %s", itemText);
      *matchLevel = MI_MATCH_CLOSE;
      return;
      }
    }
  }

/************************************************************************/
/*			_mi_match_ae_int					*/
/************************************************************************/

/* Set match level to MI_MATCH_CLOSE or MI_MATCH_NOT, or leave alone	*/

static ST_VOID _mi_match_ae_int (ST_INT *matchLevel,
		      ST_BOOLEAN ind_pres, ST_INT32 ind_val, 
		      ST_BOOLEAN dib_pres, ST_INT32 dib_val, 
		      ST_BOOLEAN must_match, ST_CHAR *itemText)
  {
ST_INT rc;
ST_BOOLEAN doCompare;

  rc = _mi_match_present_flags (matchLevel, itemText, dib_pres, 
  				ind_pres,&doCompare);
  if (rc != SD_SUCCESS)
    {
    if (must_match)
      {
      *matchLevel = MI_MATCH_NOT;
      return;
      }
    MI_LOG_CADDR1 ("        Ignoring %s", itemText);
    *matchLevel = MI_MATCH_CLOSE;
    return;
    }

/* OK, we either matched or are ignoring, see if we need to compare	*/
  if (doCompare)
    {
    if (ind_val != dib_val)
      {
      if (must_match)
        {
        MI_LOG_CADDR1 ("        %s mismatch", itemText);
        *matchLevel = MI_MATCH_NOT;
        return;
        }
      MI_LOG_CADDR1 ("        Ignoring %s", itemText);
      *matchLevel = MI_MATCH_CLOSE;
      return;
      }
    }
  }

/************************************************************************/
/*			_mi_match_mem					*/
/************************************************************************/

/* Set match level to MI_MATCH_CLOSE or MI_MATCH_NOT, or leave alone	*/

static ST_VOID _mi_match_mem (ST_INT *matchLevel,
		      ST_INT ind_sel_len, ST_UCHAR *ind_sel, 
		      ST_INT dib_sel_len, ST_UCHAR *dib_sel, 
		      ST_BOOLEAN must_match, ST_CHAR *sel_text)
  {
/* See if the selectors match */
  if (ind_sel_len == dib_sel_len &&
      !memcmp (ind_sel, dib_sel, ind_sel_len))
    {
    *matchLevel = MI_MATCH_EXACT;
    return;
    }

/* If they must, return match level 0 */
  if (must_match)
    {
    MI_LOG_CADDR1 ("        %s mismatch", sel_text);
    *matchLevel = MI_MATCH_NOT;
    return;
    }

/* If they don't have to, return match level 1 */
  MI_LOG_CADDR1 ("        Ignoring %s mismatch", sel_text);
  *matchLevel = MI_MATCH_CLOSE;
  }

/************************************************************************/
/*			_mi_match_present_flags				*/
/************************************************************************/

/* Set match level to MI_MATCH_CLOSE or MI_MATCH_NOT, or leave alone	*/

static ST_RET _mi_match_present_flags (ST_INT *matchLevelIo,
				       ST_CHAR *aeElText,
				       ST_BOOLEAN dibPres, 
				       ST_BOOLEAN remotePres,
				       ST_BOOLEAN *doCompareOut)
  {

/* If the remote sent the parameter and it is in the DIB, SUCCESS	*/
  if (dibPres == remotePres)
    {
    *doCompareOut = dibPres;  /* If it is present, then compare it	*/
    return (SD_SUCCESS);
    }
  *doCompareOut = SD_FALSE;	/* Can't compare */

/* OK, this is more complicated. We need to find out what the mismatch	*/
/* is and what the user wants to do about it.				*/

  if (!remotePres)
    {
    if (mi_match_allow_missing_ae_elements)
      {
      MI_LOG_CADDR1 ("        Remote did not send: %s, ignoring", aeElText);
      *matchLevelIo = MI_MATCH_CLOSE;
      }
    else
      {
      MI_LOG_CADDR1 ("        Remote did not send required: %s", aeElText);
      return (SD_FAILURE);
      }
    }

  if (remotePres)
    {
    if (mi_match_allow_extra_ae_elements)
      {
      MI_LOG_CADDR1 ("        Remote sent unexpected: %s, ignoring", aeElText);
      *matchLevelIo = MI_MATCH_CLOSE;
      }
    else
      {
      MI_LOG_CADDR1 ("        Remote sent unexpected: %s", aeElText);
      return (SD_FAILURE);
      }
    }
/* The user is OK with the parameter mismatch */
  return (SD_SUCCESS);
  }


/************************************************************************/
/************************************************************************/
/*			mi_release_ind_fun 				*/
/************************************************************************/

ST_VOID mi_release_ind_fun (MVL_NET_INFO *mvl_net_info)
  {
MI_CONN *mi_conn;
MI_ASSOC_CTRL *mi_assoc;

  mi_conn = mvl_net_info->mi_conn;
  mi_assoc = mi_conn->mi_assoc;
  _mi_clean_assoc (mi_assoc, MI_ASSOC_INACTIVE_TERMINATE_IND);
  }

/************************************************************************/
/************************************************************************/
/*			mi_abort_ind_fun 				*/
/************************************************************************/

ST_VOID mi_abort_ind_fun (MVL_NET_INFO *mvl_net_info)
  {
MI_CONN *mi_conn;
MI_ASSOC_CTRL *mi_assoc;

  mi_conn = mvl_net_info->mi_conn;
  mi_assoc = mi_conn->mi_assoc;
  _mi_clean_assoc (mi_assoc, MI_ASSOC_INACTIVE_ABORT_IND);
  }

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			_mi_assoc_start_client 	 			*/
/************************************************************************/

static ST_VOID _mi_assoc_start_client (MI_ASSOC_CTRL *mi_assoc)
  {
MI_CONN *mi_conn;
MI_REQ_CTRL *mi_req;

  mi_conn = mi_assoc->mi_conn;
  mi_req = _mic_start (mi_conn);
  if (!mi_req)
    {
    MI_LOG_NERR0 ("Start Client request error, aborting"); 
    _mi_abort_assoc (mi_assoc);
    mi_assoc->time_to_initiate = _mi_curr_sec_time + 
    			mi_assoc->connect_backoff_time;
    mi_assoc->state = MI_ASSOC_STATE_BACKOFF;
    _mi_set_next_rem_ar (mi_assoc);
    return;
    }
  mi_req->u_req_done = _mi_assoc_start_client_done;
  mi_req->user_info[0] = mi_assoc;
  mi_assoc->state = MI_ASSOC_STATE_PARAM_CHECK;
  }

/************************************************************************/
/*		   _mi_assoc_start_client_done 				*/
/************************************************************************/

static ST_VOID _mi_assoc_start_client_done (MI_REQ_CTRL *mi_req)
  {
MI_ASSOC_CTRL *mi_assoc;

  mi_assoc = (MI_ASSOC_CTRL *)mi_req->user_info[0];
  if (mi_req->result == SD_SUCCESS)
    {
    mi_assoc->state = MI_ASSOC_STATE_ACTIVE;
    mi_free_req_ctrl (mi_req);
    u_mi_association_active (mi_assoc);
    }
  else
    {
    MI_LOG_NERR1 ("Start Client confirm error: 0x%04x, aborting", mi_req->result); 
    if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING)
      {
      _mi_abort_assoc (mi_assoc);
      mi_assoc->time_to_initiate = time (NULL) + mi_assoc->connect_backoff_time;
      mi_assoc->state = MI_ASSOC_STATE_BACKOFF;
      }
    else
      mi_assoc->state = MI_ASSOC_STATE_ABORT;	
    _mi_set_next_rem_ar (mi_assoc);
    mi_free_req_ctrl (mi_req);
    }
  }



/************************************************************************/
/*			mi_clean_assoc 					*/
/************************************************************************/

static ST_VOID _mi_clean_assoc (MI_ASSOC_CTRL *mi_assoc, ST_INT reason)
  {
  mi_assoc->state = MI_ASSOC_STATE_IDLE;
  u_mi_association_inactive (mi_assoc, reason);

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT)
    mic_clean_assoc (mi_assoc);
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_SERVER)
    mis_clean_assoc (mi_assoc);

  _mi_free_conn (mi_assoc->mi_conn);
  }

/************************************************************************/
/*			_mi_set_next_rem_ar 				*/
/************************************************************************/

ST_VOID _mi_set_next_rem_ar (MI_ASSOC_CTRL *mi_assoc)
  {
  ++mi_assoc->remote_ar_sel;
  if (mi_assoc->remote_ar_sel >= mi_assoc->num_remote_ar)
    mi_assoc->remote_ar_sel = 0;
  }

/************************************************************************/
/************************************************************************/
/* Miscellaneous association utility functions:				*/
/************************************************************************/

/************************************************************************/
/* _mi_num_active_assoc: return the number of active associations for	*/
/*		         the remote					*/
/************************************************************************/

ST_INT _mi_num_active_assoc (MI_REMOTE *mi_remote)
  {
MI_CONN *mi_conn;
ST_INT num_active = 0;

  mi_conn = mi_remote->mi_conn_list;
  while (mi_conn)
    {
    if (mi_conn->mi_assoc->state == MI_ASSOC_STATE_ACTIVE)
      num_active++;
    mi_conn = (MI_CONN *)list_get_next (mi_remote->mi_conn_list, mi_conn);
    }

  return (num_active);
  }
