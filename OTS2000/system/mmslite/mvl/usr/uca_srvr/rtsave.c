/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1986 - 2001, All Rights Reserved		        */
/*									*/
/* MODULE NAME : rtsave.c    						*/
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
/* 10/18/01  JRB     03    Add dummy vars & functions to satisfy linker.*/
/* 08/12/98  MDE     02    Now just save binary image to file 		*/
/* 02/24/98  MDE     01    New						*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "time.h"
#include "signal.h"

#include "mmsdefs.h"
#include "mms_pvar.h"
#include "mms_vvar.h"
#include "mms_pvmd.h"
#include "mms_err.h"
#include "mms_pcon.h"
#include "asn1defs.h"

#include "mvl_acse.h"
#include "mvl_log.h"
#include "uca_obj.h"
#include "tp4api.h"
#include "uca_srvr.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/

#define BIN_FILE_NAME "rt_data.bin"

static ST_VOID init_mem (ST_VOID);
static ST_VOID init_log_cfg (ST_VOID);

FILE *bFp;

/************************************************************************/
/* Need to satisfy uca_obj.odf references ...				*/

DI_TDEF lun0_di;
DI_TDEF pbro_di;

/* Some PBRO objects, with dynamic data */
ST_UINT16 Pbro_SG_Pu_Phsi_val;
ST_UINT16 Pbro_SG_Pu_neutrali_val;
ST_UINT16 Pbro_SG_Pu_hzi_val;

PBRO_ST pbro_st;

/************************************************************************/

ST_VOID main ()
  {
ST_INT i;
ST_INT j;
RUNTIME_TYPE *rt;

  init_mem ();		/* Set up memory allocation tools 		*/
  init_log_cfg ();	/* Set up logging subsystem			*/
  SLOGALWAYS0 ("Initializing ...");
  mvl_init_mms_objs ();

  bFp = fopen (BIN_FILE_NAME, "wb");
  for (i = 0; i < mvl_num_types; ++i)
    {
    if (mvl_type_ctrl[i].num_rt == 0)
      break;
    rt = mvl_type_ctrl[i].rt;
    for (j = 0; j < mvl_type_ctrl[i].num_rt; ++j, ++rt)
      fwrite (rt, sizeof(RUNTIME_TYPE), 1, bFp);
    }
  fclose (bFp);
  printf ("\n\n");
  }

/************************************************************************/
/************************************************************************/
/* INDICATION HANDLING FUNCTIONS					*/
/************************************************************************/
/*			u_mvl_ident_ind					*/
/************************************************************************/

ST_VOID u_mvl_ident_ind (MVL_IND_PEND *indCtrl)
  {
  }

/************************************************************************/
/*			u_mvl_status_ind				*/
/************************************************************************/

ST_VOID u_mvl_status_ind (MVL_IND_PEND *indCtrl)
  {
  }

/************************************************************************/
/*			u_mvl_getvlist_ind				*/
/************************************************************************/

ST_VOID u_mvl_getvlist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_getvlist_resp (indCtrl);
  }

/************************************************************************/
/*			u_mvl_getvar_ind				*/
/************************************************************************/

ST_VOID u_mvl_getvar_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_getvar_resp (indCtrl);
  }

/************************************************************************/
/*			u_mvl_namelist_ind				*/
/************************************************************************/

ST_VOID u_mvl_namelist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_namelist_resp (indCtrl);
  }

/************************************************************************/
/*			u_mvl_concl_ind					*/
/************************************************************************/
ST_VOID u_mvl_concl_ind (MVL_COMM_EVENT *event)
  {	/* Only to satisfy linker. Never called. Do nothing.	*/
  }
/************************************************************************/
/*			u_mvl_getdom_ind				*/
/************************************************************************/
ST_VOID u_mvl_getdom_ind (MVL_IND_PEND *indCtrl)
  {	/* Only to satisfy linker. Never called. Do nothing.	*/
  }
/************************************************************************/
/*			u_mvl_defvlist_ind				*/
/************************************************************************/
ST_VOID u_mvl_defvlist_ind (MVL_IND_PEND *indCtrl)
  {	/* Only to satisfy linker. Never called. Do nothing.	*/
  }
/************************************************************************/
/*			u_mvl_delvlist_ind				*/
/************************************************************************/
ST_VOID u_mvl_delvlist_ind (MVL_IND_PEND *indCtrl)
  {	/* Only to satisfy linker. Never called. Do nothing.	*/
  }
/************************************************************************/
/*			u_mvlu_rpt_scan_done				*/
/************************************************************************/
ST_VOID u_mvlu_rpt_scan_done (MVLU_RPT_SCAN_CTRL *scanCtrl)
  {	/* Only to satisfy linker. Never called. Do nothing.	*/
  }
/************************************************************************/
/************************************************************************/
/* MISC. FUNCTIONS							*/
/************************************************************************/
/*			init_log_cfg					*/
/************************************************************************/

static ST_VOID init_log_cfg (ST_VOID)
  {
#ifdef DEBUG_SISCO
/* Use File logging							*/
  sLogCtrl->logCtrl = LOG_FILE_EN;

/* Use time/date time log						*/
  sLogCtrl->logCtrl |= LOG_TIME_EN;

/* File Logging Control defaults 					*/
  sLogCtrl->fc.fileName = "rtsave.log";
  sLogCtrl->fc.maxSize = 1000000L;
  sLogCtrl->fc.ctrl = (FIL_CTRL_WIPE_EN | 
                 FIL_CTRL_WRAP_EN | 
		 FIL_CTRL_NO_APPEND |
                 FIL_CTRL_MSG_HDR_EN);


#endif  /* DEBUG_SISCO */
  }


/************************************************************************/
/************************************************************************/
/*				init_mem				*/
/************************************************************************/

static ST_VOID mem_chk_error_detected (ST_VOID);
static ST_VOID *my_malloc_err (ST_UINT size);
static ST_VOID *my_calloc_err (ST_UINT num, ST_UINT size);
static ST_VOID *my_realloc_err (ST_VOID *old, ST_UINT size);

static ST_CHAR *spareMem;

static ST_VOID init_mem ()
  {
#if defined(NO_GLB_VAR_INIT)
  mvl_init_glb_vars ();
#endif

/* Allocate spare memory to allow logging/printing memory errors	*/
  spareMem = malloc (500);

/* trap mem_chk errors							*/
  mem_chk_err = mem_chk_error_detected;
  m_memerr_fun = my_malloc_err;
  c_memerr_fun = my_calloc_err;
  r_memerr_fun = my_realloc_err;

#if 1
  m_heap_check_enable = SD_TRUE;
  m_check_list_enable = SD_TRUE;
  m_no_realloc_smaller = SD_TRUE;
  m_fill_en = SD_TRUE;
#endif
  m_mem_debug = SD_TRUE;
  }

/************************************************************************/
/* This function is called from the DEBUG version of the mem library	*/
/* when an error of any type is detected.				*/
/************************************************************************/

static ST_INT memErrDetected;

static ST_VOID mem_chk_error_detected (ST_VOID)
  {
  if (!memErrDetected)
    {
    free (spareMem);
    memErrDetected = SD_TRUE;
    printf ("\n Memory Error Detected! Check log file");
    dyn_mem_ptr_status ();
    }
  }

/************************************************************************/
/* Memory Allocation Error Handling Functions.				*/
/* These functions are called from mem_chk when it is unable to 	*/
/* perform the requested operation. These functions must either return 	*/
/* a valid buffer or not return at all.					*/
/************************************************************************/

static ST_VOID *my_malloc_err (ST_UINT size)
  {
  mem_chk_error_detected ();
  printf ("\n Malloc");
  exit (2);
  return (NULL);   
  }

static ST_VOID *my_calloc_err (ST_UINT num, ST_UINT size)
  {
  mem_chk_error_detected ();
  exit (3);
  return (NULL);   
  }

static ST_VOID *my_realloc_err (ST_VOID *old, ST_UINT size)
  {
  mem_chk_error_detected ();
  exit (4);
  return (NULL);   
  }

