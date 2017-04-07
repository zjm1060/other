/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1986 - 2008, All Rights Reserved		        */
/*									*/
/* MODULE NAME : uca_srvr.c    						*/
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
/* 09/10/10  JRB	   Del log of memory usage at startup.		*/
/* 06/21/10  JRB	   Del IEC GSSE reception code (see client sample)*/
/* 11/17/09  JRB     89    Del HARD_CODED_CFG example.			*/
/*			   Del MAP30_ACSE support.			*/
/*			   Del refs to secManCfgXmlFile (obsolete).	*/
/* 05/21/08  JRB     88    Don't allow conclude if requests pending or	*/
/*			   if any files are open.			*/
/* 05/14/08  GLB     87    Added slog_ipc_std_cmd_service		*/
/* 01/03/08  LWP     86	   Memchk statistics no longer crash when written*/
/* 02/26/07  JRB     85    Add customized u_ml_get_rt_type & test code.	*/
/* 02/21/07  JRB     84    Fix more mvl_vmd_* calls.			*/
/* 02/19/07  JRB     83    Add call to mvl61850_rpt_service (61850 rpt	*/
/*			   servicing is now separate from UCA).		*/
/*			   Add call to mvl61850_brcb_entryid_init.	*/
/* 02/16/07  JRB     82    Use new "*_rpt_ctrl_destroy_all".		*/
/* 11/01/06  JRB     81    Use new mvl_vmd_* object handling functions.	*/
/* 10/13/06  JRB     80    Use also mvl61850_free_rpt_ctrl for 61850.	*/
/* 09/13/06  JRB     79    Use new mvl61850_create_rpt_ctrl for 61850	*/
/*			   rpts (now mvlu_create_rpt_ctrl only for UCA).*/
/* 06/23/06  MDE     78    Added time_t cast				*/
/* 10/25/05  EJV     77    S_SEC_ENABLED: added identify_response_info.	*/
/*			   Call slog_start, slog_end, slogIpcEvent.	*/
/* 09/09/05  JRB     76    Don't print BUILD_NUM.			*/
/* 08/05/05  EJV     75    Added ssleLogMaskMapCtrl.			*/
/* 07/08/05  JRB     74    Add sample "u_mvl61850_ctl_oper_*" functs.	*/
/* 06/27/05  JRB     73    Use mvl_nvl_add (mvlu_rpt_nvl_add is obsolete).*/
/* 06/07/05  EJV     72    Set gsLogMaskMapCtrl if S_MT_SUPPORT defined	*/
/* 06/06/05  EJV     71    gsLogMaskMapCtrl not on QNX.			*/
/* 05/24/05  EJV     70    Added logCfgAddMaskGroup (&xxxLogMaskMapCtrl)*/
/*			   Use logcfgx_ex to parse logging config file.	*/
/* 05/20/05  JRB     69    Don't print "Check log file". May not be one.*/
/* 05/20/05  JRB     68    Fix keyboard input: Use getch on WIN32, QNX.	*/
/*			   Use term_init, term_rest, getchar elsewhere.	*/
/* 08/23/04  NAV     67    Set ms in report time values on WIN32.	*/
/* 08/13/04  JRB     66    Del unneeded includes.			*/
/* 08/10/04  EJV     65    secManCfgXmlFile: all lowcase "secmancfg.xml"*/
/*			   u_mvl_connect_ind_ex: typecasted ret		*/
/* 08/06/04  ASK     64    Change path to secManCfg.xml for !WIN32	*/
/* 07/08/04  JRB     63    Del mvl_local_cl_ar_name.			*/
/*			   Use new asn1_convert_timet_to_btime6.	*/
/*			   Use mvl_type_id_create_from_tdl, del new_type*/
/* 07/01/04  JRB     62    Del test_dynamic_type_creation: this used	*/
/*			   obsolete functions based on old SCL standard.*/
/* 01/27/04  EJV     61    Added Temparature var.			*/
/* 01/21/04  EJV     60    Don't call ulFreeAssocSecurity on disconnect.*/
/* 01/04/04  EJV     59    Added security support (S_SEC_ENABLED code).	*/
/*			   Added LITESECURE version printf.		*/
/*			   Added 'u' option in reportKbService().	*/
/*			   Replaced Sleep().				*/
/* 12/11/03  JRB     58    DON'T use MVLLOG_REQ/RESP/IND/CONF, they	*/
/*			   don't exist.					*/
/* 11/08/03  JRB     57    MVLU_USR_FUN_TBLS option deleted, so del	*/
/*			   leaf function tables.			*/
/*			   Chg dom to LD0 for 61850 objs (need new ODF).*/
/* 09/30/03  EJV     56    Added u_mvl_connect_ind_ex.  		*/
/*			   Chg ASN1 log setting (was logging all)	*/
/*			   Del old password auth (OBSOLETE_ACSE_AUTH)	*/
/* 04/22/03  JRB     55    Del mvlu_load_xml_leaf_file call,		*/
/*			   not appropriate for Foundry generated types.	*/
/* 02/04/03  JRB     54    Add dynamic type creation test code.		*/
/*			   Replace u_mvlu_rpt_scan_done with user funct	*/
/*			   my_rpt_scan_done & set indCtrl->usr_resp_fun	*/
/*			   to point to it.				*/
/*			   Add init_rpt_ctrl_iec_brcb to test IEC URCB.	*/
/*			   Chg main return from "void" to "int".	*/
/* 12/16/02  JRB     53    Chg mvlu_create_rpt_ctrl args.		*/
/*			   Del RPT *typeId global vars, not needed.	*/
/*			   Add mvl_uca.h				*/
/*			   Del refs to  OBSOLETE rptTim_va in MVLU_RPT_CTRL*/
/*			   Add callback u_mvlu_rpt_time_get		*/
/*			   Add init_rpt_ctrl_iec_brcb to test IEC BR.	*/
/*			   Chg ifdef to ifndef USE_DERIVED_UCA_REP...	*/
/*			   Fix proto & call to incOptTestSet.		*/
/*			   Update UtcTimeNow each time through loop.	*/
/* 12/09/02  MDE     52    Updated Report and SBO handling		*/
/* 12/02/02  MDE     51    Now allow load LAP parameters		*/
/* 10/18/02  ASK     50    Changed to OBSELETE_ACSE_AUTH to remove old  */
/*			   authentication sample code.			*/	 
/* 04/01/02  JRB     49    init_named_vars return SUCCESS or err code.	*/
/* 03/29/02  JRB     48    Pass (MVLU_RPT_CTRL *) to fcts, chk for NULL.*/
/* 03/21/02  JRB     47    Move init_named_vars, clnp_read_thread_start	*/
/*			   after init_acse. Send & rcv same GOOSE MAC.	*/
/* 03/13/02  JRB     46    Chg clnp_snet_add_multicast_mac to		*/
/*			   clnp_snet_set_multicast_filter.		*/
/* 03/06/02  JRB     45    Del ADLC code.				*/
/* 02/25/02  MDE     44    Now get max PDU size from mvl_cfg_info	*/
/* 02/08/02  JRB     43    Pass MVL_CFG.. to osicfgx & mvl_start_acse,	*/
/*			   del mvl_init_mms.. call & mvl_local_ar . var.*/
/* 02/04/02  JRB     42    Del mvl_init_cl.. call, use mvl_init_audt_addr*/
/*			   Del use of COACSE define.			*/
/* 11/07/01  JRB     41    Chg goose_tx_thread proto.			*/
/* 10/12/01  JRB     40    Do NOT enable SMEM_LOG_FLOW, SMEM_LOG_DEBUG	*/
/* 10/09/01  JRB     39    Use new MMSLITE_* defines for product ID.	*/ 
/* 09/21/01  JRB     38    Call init_mem_chk before smemcfgx.		*/
/*			   Chg args for u_smem_get_pool_params &	*/
/*			   u_smem_need_buffers.				*/
/*			   Del u_smem_cfg_pool (never called).		*/
/* 05/25/01  RKR     37    Updated version to 4.01P12			*/
/* 05/24/01  MDE     36    QNX warning cleanup				*/
/* 03/10/01  GLB     35    Added smemcfgx.c cfg. parser                 */
/* 01/04/01  EJB     34    On UNIX reportKbService needs ukey.c linked.	*/
/*			   Commented out #defined _REPORT_TEST		*/
/* 11/27/00  JRB     33    Use old CFG files for Reduced Stack & Trim-7.*/
/* 11/07/00  GLB     32    Added logcfgx.c and osicfgx.c cfg. parsers   */
/* 10/25/00  JRB     31    Add u_mvl_getcl_ind & u_mvl_concl_ind.	*/
/* 07/17/00  RKR     30    Updated version to 4.01P6			*/
/* 07/11/00  MDE     29    Changed seqNum to sqNum, made INT8U		*/
/* 05/01/00  RKR     28    Updated version to 4.02			*/
/* 03/17/00  RKR     27    Added init_named_vars			*/
/* 03/16/00  RKR     26    Changed (Q) and (SIT) models in the UCA RPT	*/
/* 03/14/00  JRB     25    Fill in scan_va_scope for rpt.		*/
/* 02/15/00  JRB     24    Fix init of "GOOSE_INFO" in goose_tx.	*/
/* 12/20/99  GLB     23    Print error when exiting                     */
/* 09/27/99  NAV     22    A few changes to help test AX-S4 MMS		*/
/* 09/07/99  MDE     21    Upgraded to use new UCA report mechanisms	*/
/* 09/07/99  MDE     20    Migrated to GOMSGE .9			*/
/* 07/15/99  JRB     19    Add sample "u_a_auth_password_chk" funct.	*/
/* 03/19/99  RKR     18    Changed server to local1 			*/
/* 01/22/98  JRB     17    Don't need _UCA_ prefix on NVL anymore.	*/
/*			   "createDataS.." use "mvl_nvl_add" (simpler).	*/
/* 11/17/98  MDE     16    Added define/delete namelist support		*/
/* 11/13/98  JRB     15    Wait for event before calling mvl_comm_serve	*/
/*			   if stack is MOSI (i.e. osi???e.lib used).	*/
/* 10/16/98  MDE     14    Added samples of configured dataset rpt NVL	*/
/* 10/15/98  MDE     13    Now call sTimeInit explicitly		*/
/* 10/13/98  MDE     12    More Report testing enhancements		*/
/* 10/01/98  MDE     11    Report testing enhancements			*/
/* 09/16/98  MDE     10    Report use update to latest & cleanup, SBO	*/
/* 08/12/98  MDE     09    Changes to report handling, other minor	*/
/* 07/13/98  MDE     08    Report, other cleanup & fixup		*/
/* 05/14/98  MDE     07    Removed 'ml_init_buffers' call		*/
/* 12/29/97  MDE     06    Now use 'mktypes' for configured MMS objects	*/
/* 12/29/97  MDE     05    Added ^c handler, removed keyboard stuff,	*/
/* 			   removed program arguments			*/
/* 12/10/97  MDE     04    Added VXWORKS, Journal Read/Init		*/
/* 10/09/97  MDE     03    Minor cleanups				*/
/* 09/18/97  MDE     02    Fixed getnamelist code, misc. minor		*/
/* 09/10/97  MDE     01    MMS-LITE V4.0 Release			*/
/************************************************************************/

#if 0
  #define ACSE_AUTH_ENABLED		/* for password authentication	*/
					/* without security		*/
/*  S_SEC_ENABLED		for certificate based authentication &	*/
/*				SSL encryption, put this define in	*/
/*				application's DSP/MAK file 		*/
#endif



/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include <signal.h>

#include "mmsdefs.h"
#include "mms_pvar.h"
#include "mms_vvar.h"
#include "mms_pvmd.h"
#include "mms_err.h"
#include "mms_pcon.h"
#include "asn1defs.h"
#include "stime.h"

#include "mvl_acse.h"
#include "mvl_uca.h"	/* UCA/IEC defs	*/
#include "mvl_log.h"
#include "uca_obj.h"
#include "tp4api.h"
#include "uca_srvr.h"

#if defined (MVL_GOOSE_SUPPORT)
#include "goose.h"
#include "glbsem.h"
#include <process.h>
#endif

#ifdef SMEM_ENABLE
#include "smem.h"
#endif

/*----------------------------------------------------------------------*/
/* NOTE: The MMS-EASE Lite Secured applications (Client, Server) 	*/
/*	 are designed to work with Security Toolkit for MMS-EASE Lite	*/
/*	 (LITESECURE-000-001).						*/
/*       The S_SEC_ENABLED delimits the security related code.		*/
/*----------------------------------------------------------------------*/
#if defined(S_SEC_ENABLED) && defined(ACSE_AUTH_ENABLED)
#error Only one S_SEC_ENABLED or ACSE_AUTH_ENABLED may be defined
#endif

#if defined(S_SEC_ENABLED)
#include "mmslusec.h"
#endif

#include "fkeydefs.h"
#ifdef kbhit	/* CRITICAL: fkeydefs may redefine kbhit. DO NOT want that.*/
#undef kbhit
#endif

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/

ST_RET read_log_cfg_file (ST_CHAR *filename);
ST_VOID wait_any_event (ST_LONG max_ms_delay);
ST_RET my_rpt_scan_done (MVL_IND_PEND *indCtrl);

/************************************************************************/
/* MMS SERVER VARIABLES							*/
/* These are variables that are visible to MMS Clients			*/

DI_TDEF pbro_di = 
  {
  "pbro",			/* Name	*/
  "class x",			/* Class */
  "whats d?",			/* d	*/
  "SISCO",			/* Own	*/
  "Sterling Hgts",		/* Loc	*/
    {
    "Herb",			/* VndID.Vnd	*/
    "Eh?",			/* VndID.DevMdls	*/
    "123-456",			/* VndID.SerNum	*/
    "1.00P0",			/* VndID.HwRev */
    "0.99P22",			/* VndID.SftRev	*/
    },
    {
    "55611",			/* CommID.CommAdr	*/
    "1.2",			/* CommID.CommRev	*/
    1,				/* CommID.Pro	*/
    2,				/* CommID.Med	*/
    3				/* CommID.MAC	*/
    }
  };

DI_TDEF lun0_di = 
  {
  "lun0",			/* Name	*/
  "class x",			/* Class */
  "whats d?",			/* d	*/
  "SISCO",			/* Own	*/
  "Sterling Hgts",		/* Loc	*/
    {
    "Herb",			/* VndID.Vnd	*/
    "Eh?",			/* VndID.DevMdls	*/
    "123-456",			/* VndID.SerNum	*/
    "1.00P0",			/* VndID.HwRev */
    "0.99P22",			/* VndID.SftRev	*/
    },
    {
    "55611",			/* CommID.CommAdr	*/
    "1.2",			/* CommID.CommRev	*/
    1,				/* CommID.Pro	*/
    2,				/* CommID.Med	*/
    3				/* CommID.MAC	*/
    }
  };

ST_INT16 Temperature = 77;
MMS_UTC_TIME UtcTimeNow =
  {
  0,
  17,
  0x0E
  };

ST_CHAR POPFa1 [400];	/* test dynamic variable	*/

/* Some PBRO objects, with dynamic data */
ST_UINT16 Pbro_SG_Pu_Phsi_val;
ST_UINT16 Pbro_SG_Pu_neutrali_val;
ST_UINT16 Pbro_SG_Pu_hzi_val;

PBRO_ST pbro_st;

/************************************************************************/
/************************************************************************/
/* MMS IDENTIFY AND STATUS RESPONSE INFORMATION				*/
    
/* Identify server information						*/
#if defined(S_SEC_ENABLED)
IDENT_RESP_INFO identify_response_info =
  {
  "SISCO",  				/* Vendor 	*/
  S_SEC_LITESECURE_NAME,  		/* Model  	*/
  S_SEC_LITESECURE_VERSION_STR	,  0,	/* Version 	*/
  };
#else
static IDENT_RESP_INFO identify_response_info =
  {
  "SISCO",  		/* Vendor 	*/
  MMSLITE_NAME,  	/* Model  	*/
  MMSLITE_VERSION,  0,	/* Version 	*/
  };
#endif

/* Status server information						*/
static STATUS_RESP_INFO status_resp_info =
  {
  0,		/* logical_stat 	*/
  0, 		/* physical_stat 	*/
  0, 		/* local_detail_pres 	*/
  };

/************************************************************************/
/************************************************************************/
/* Functions in this module						*/

/* Initialization */
static ST_VOID init_acse (ST_VOID);
static ST_VOID init_log_cfg (ST_VOID);
static ST_VOID init_mem (ST_VOID);
static ST_RET init_named_vars (ST_VOID);

/* Connection Management */
static ST_INT connect_ind_fun (MVL_NET_INFO *cc, INIT_INFO *init_info);
static ST_VOID disc_ind_fun (MVL_NET_INFO *cc, ST_INT discType);

/* Misc. */
ST_VOID ctrlCfun (int);

/************************************************************************/

#if defined (MVL_GOOSE_SUPPORT)
static ST_THREAD_ID gooseThreadId;
static ST_THREAD_HANDLE gooseThreadHandle;
static ST_LONG gooseCount;
ST_THREAD_RET ST_THREAD_CALL_CONV goose_tx_thread (ST_THREAD_ARG threadArg);
ST_VOID goose_tx (HANDLE threadId);
#endif

/************************************************************************/
/* Report control */

static ST_BOOLEAN autoRptTestSet = SD_FALSE;
static ST_DOUBLE nextRptVarChangeTime = 0.0;

/*#define _REPORT_TEST*/ 	/* Used for testing UCA reports */
#define USE_DERIVED_UCA_REPORT_DATASET
#define USE_UCA_REPORT_SCAN
#define USE_UCA_REPORT_ASYNC_DATA_CHANGE
#define DATA_CHANGE_PERIOD 1000.0

static MVLU_RPT_CTRL *init_rpt_ctrl (ST_VOID);
static MVLU_RPT_CTRL *init_rpt_ctrl_iec_brcb (ST_VOID);
static MVLU_RPT_CTRL *init_rpt_ctrl_iec_urcb (ST_VOID);
static ST_VOID reportKbService (MVLU_RPT_CTRL *rptCtrl);
static ST_VOID changeRptVars (MVLU_RPT_CTRL *rptCtrl);
static ST_VOID *change_rpt_var_val (ST_INT i);

static ST_VOID incReasonTestSet (ST_VOID);
#if defined (_REPORT_TEST)
static ST_VOID incVaChangeTestSet (ST_VOID);
static ST_VOID incOptTestSet (MVLU_RPT_CTRL *rptCtrl);
#endif

/************************************************************************/

/* General program control variables */
static ST_BOOLEAN doIt = SD_TRUE;

/************************************************************************/
/************************************************************************/
/*                       main						*/
/************************************************************************/

#if defined (VXWORKS)	/* Just make this look like "main" call.	*/
ST_VOID mmsserver ()
  {
#else	/* !VXWORKS	*/  
int main ()
  {
#endif	/* !VXWORKS	*/  
#ifdef SMEM_ENABLE
ST_INT j;
#endif
ST_RET ret;
MVLU_RPT_CTRL *globeStRptCtrl;
MVLU_RPT_CTRL *RptCtrlIecBrcb;
MVLU_RPT_CTRL *RptCtrlIecUrcb;

  setbuf (stdout, NULL);    /* do not buffer the output to stdout   */
  setbuf (stderr, NULL);    /* do not buffer the output to stderr   */

  printf ("%s Version %s\n", MMSLITE_NAME, MMSLITE_VERSION);
#if defined(S_SEC_ENABLED)
  printf ("%s Version %s\n", S_SEC_LITESECURE_NAME, S_SEC_LITESECURE_VERSION_STR);
#endif
  puts (MMSLITE_COPYRIGHT);

#if defined(NO_GLB_VAR_INIT)
  mvl_init_glb_vars ();
#endif
  init_mem ();		/* Set up memory allocation tools 		*/
  init_log_cfg ();	/* Set up logging subsystem			*/

  SLOGALWAYS2 ("%s Version %s", MMSLITE_NAME, MMSLITE_VERSION);

#ifdef SMEM_ENABLE
  init_mem_chk();	/* MUST call before smemcfgx or any alloc funct.*/
  ret = smemcfgx ("smemcfg.xml");
  if (ret != SD_SUCCESS)
    {
    printf ("\n Parsing of Smem configuration failed, err = 0x%X. Check configuration.\n", ret );
    exit (1);
    }
  smem_debug_sel = SMEM_LOG_ERR | SMEM_LOG_NERR;
#endif

#if defined(UCA_SMP)
  smp_init ();
#endif

  SLOGALWAYS0 ("Initializing ...");
  mvlu_install ();	/* Install UCA object handler			*/

  init_acse ();		/* Start the lower layer subsystem		*/

  ret = init_named_vars ();		/* must be after mvl_start_acse	*/
  if (ret != SD_SUCCESS)
    {
    printf ("\ninit_named_vars failed. Exiting.");
    exit (1);
    }
  

  /* Create base types. These may also be created by Foundry by using
   * ":TK" in the ODF file.
   * CRITICAL: Create new types ONLY AFTER initialization, especially init_acse.
   */
  if ((mvl_type_id_create_from_tdl ("INT32S", "Long")) < 0)
    printf ("Error creating type for INT32S\n");

  /* TESTING: To test our customized "u_ml_get_rt_type", here we create	*/
  /* a complex type using base types (<RTYP_INT32U> & <RTYP_INT16U>).	*/
  /* This should cause "u_ml_get_rt_type" to be called.			*/
  if ((mvl_type_id_create_from_tdl ("TEST_TYPE", "{(val1)<RTYP_INT32U>,(val2)<RTYP_INT16U>}")) < 0)
    printf ("Error creating type for TEST_TYPE\n");

  SLOGALWAYS0 ("Initialization complete, entering service loop");

/* Set the ^c catcher */
  signal (SIGINT, ctrlCfun);

/* Initialize our report control block					*/
  globeStRptCtrl = init_rpt_ctrl ();
  if (globeStRptCtrl == NULL)
    {
    printf ("\ninit_rpt_ctrl failed. Exiting.");
    exit (1);
    }
    

/* Initialize IEC-61850 Buffered report control block	*/
  RptCtrlIecBrcb = init_rpt_ctrl_iec_brcb ();
  if (RptCtrlIecBrcb == NULL)
    {
    printf ("\ninit_rpt_ctrl_iec_brcb failed. Exiting.");
    exit (1);
    }
  else
    { /* DEBUG: code to test mvl61850_brcb_entryid_init  */
    /* If you'd like the default initial EntryID, delete this code.	*/
    /* If you'd like a different initial EntryID, change this code.	*/ 
    ST_UINT8 init_entryid[8] = {0x11, 0x22, 0x33, 0x44, 0x00, 0x00, 0x00, 0x00};
    mvl61850_brcb_entryid_init (RptCtrlIecBrcb, init_entryid);
    } 
/* Initialize IEC-61850 Unbuffered report control block	*/
  RptCtrlIecUrcb = init_rpt_ctrl_iec_urcb ();
  if (RptCtrlIecUrcb == NULL)
    {
    printf ("\ninit_rpt_ctrl_iec_urcb failed. Exiting.");
    exit (1);
    }

/* We are all initialized, just service communications			*/
  chk_malloc (1);		/* Marker 				*/
  printf ("\n Entering 'server' mode, hit ^c to exit ... \n");

#if !defined(NO_KEYBOARD) && !defined(_WIN32) && !defined(__QNX__)
  term_init ();	/* makes getchar work right	*/
#endif

#if defined (MVL_GOOSE_SUPPORT)
  gs_start_thread (goose_tx_thread, (ST_THREAD_ARG) 0, 
		     &gooseThreadHandle, &gooseThreadId);
#endif

  while (doIt)
    {
#if defined (MOSI)		/* If stack is MOSI, use events		*/
    wait_any_event (1000);	/* Wait 1000 milliseconds		*/
    
    while (mvl_comm_serve ())	/* Perform communications service 	*/
      {				/* Loop until "mvl_comm_serve" returns 0*/
      }
#else
    mvl_comm_serve ();		/* Perform communications service 	*/
#endif

    reportKbService (globeStRptCtrl);
    changeRptVars (globeStRptCtrl);
    mvlu_rpt_service ();	/* process UCA 2.0 reports	*/
    mvl61850_rpt_service ();	/* process IEC 61850 reports	*/

    UtcTimeNow.secs = (ST_UINT32) time(NULL);	/* Update UTC time	*/
					/* leave fraction,qflags alone*/
#if defined(S_SEC_ENABLED)
    /* check for security configuration updates */
    if (ulCheckSecurityConfiguration () != SD_SUCCESS)
      printf("\n Security Reconfiguration failed\n");
#endif /* defined(S_SEC_ENABLED) */

#if defined(DEBUG_SISCO)
    if (sLogCtrl->logCtrl & LOG_IPC_EN)
      slogIpcEvent ();	/* required for IPC Logging if gensock2.c is	*/
    			/* not compiled with GENSOCK_THREAD_SUPPORT	*/
     
    /*  At runtime, periodically need to service SLOG commands and calling connections. */
    /*  The timing of this service is not critical, but to be responsive a default of   */
    /*  100ms works well.                                                               */
    slog_ipc_std_cmd_service ("logcfg.xml", NULL, NULL, SD_TRUE,  NULL, NULL);
#endif
    }

  mvlu_rpt_ctrl_destroy_all ();		/* destroy UCA rpt_ctrls	*/
  mvl61850_rpt_ctrl_destroy_all ();	/* destroy 61850 rpt_ctrls	*/

  ret = mvl_end_acse ();	/* Stop the lower layer subsystem	*/
#ifdef SMEM_ENABLE
  for (j=0; j<M_SMEM_MAX_CONTEXT; j++)
    {  
    smem_log_state (&m_smem_ctxt [j]);
    }
#endif
  dyn_mem_ptr_status ();	/* Log memory allocation usage		*/

#if defined(DEBUG_SISCO)
  /* terminate logging services and save current log file position */
  slog_end (sLogCtrl);
#endif

  printf ("\n\n");

#if defined (MVL_GOOSE_SUPPORT)
  printf ("\n GooseCount = %ld", gooseCount);
#endif

#if !defined(NO_KEYBOARD) && !defined(_WIN32) && !defined(__QNX__)
  term_rest ();	/* Must be called before exit if term_init used.	*/
#endif

  return (0);
  }

/************************************************************************/
/************************************************************************/
/* GOOSE FUNCTIONS							*/
#if defined (MVL_GOOSE_SUPPORT)

/************************************************************************/
/*			goose_tx_thread 				*/
/************************************************************************/

ST_THREAD_RET ST_THREAD_CALL_CONV goose_tx_thread (ST_THREAD_ARG threadArg) 
  {
HANDLE threadId;
ST_INT i;

  i = (ST_INT) threadArg;
  gooseCount = 0;
  threadId = GetCurrentThread ();
  while (1)
    {
    sMsSleep (10000);
    goose_tx (threadId);
    ++gooseCount;
    }
  }

/************************************************************************/
/*			goose_tx 					*/
/************************************************************************/

ST_VOID goose_tx (HANDLE threadId)
  {
GOOSE_INFO *gi;
ST_RET rc;
static ST_BOOLEAN addressIsSet = SD_FALSE;
static GOOSE_INFO gooseTxInfo;	/* CRITICAL: static	*/
static ST_UINT16 SqNum = 0;

  gi = &gooseTxInfo;

/* We will do this only once, to speed things up a bit ... */
  if (addressIsSet == SD_FALSE)
    {
    memset(&gooseTxInfo, 0, sizeof (GOOSE_INFO)); 

    /* Fill in addressing information. For simplicity we will use the	*/
    /* 'mvl_init_audt_addr' function. 					*/
    /* NOTE: make sure local1 and remote1 are in lean.cfg.		*/
    rc = mvl_init_audt_addr (&gooseTxInfo.audtApdu, "local1", "remote1");
    if (rc != SD_SUCCESS)
      {
      printf ("goose_tx error: can't get addressing info.\n");
      return;	/* Check log file for error.	*/
      }

    addressIsSet = SD_TRUE;
    }

  /* fill in Local & Remote MAC addresses                                              */
  gi->audtApdu.rem_mac_valid = TRUE;
  gi->audtApdu.rem_mac[0] = 0x09;
  gi->audtApdu.rem_mac[1] = 0x09;
  gi->audtApdu.rem_mac[2] = 0x09;
  gi->audtApdu.rem_mac[3] = 0x09;
  gi->audtApdu.rem_mac[4] = 0x09;
  gi->audtApdu.rem_mac[5] = 0x09;

  gi->audtApdu.loc_mac[0] = 0x01;
  gi->audtApdu.loc_mac[1] = 0x01;
  gi->audtApdu.loc_mac[2] = 0x01;
  gi->audtApdu.loc_mac[3] = 0x01;
  gi->audtApdu.loc_mac[4] = 0x01;
  gi->audtApdu.loc_mac[5] = 0x01;

/* Now fill in the GOOSE MMS information */
  strcpy (gi->SendingIED, "Mike Eick!");
  gi->t.form = MMS_BTOD6;
  gi->t.ms = 1234;
  gi->t.day = 5678;

  gi->SqNum = SqNum;
  gi->StNum = 2;
  gi->HoldTim = 1000;
  gi->BackTim = 4;
  gi->PhsID = 5;

  gi->num_dna_bits = GOOSE_DEF_NUM_DNA_BITS;
  memset (gi->DNA, 0x55, GOOSE_DEF_NUM_DNA_BITS/8);

  gi->num_usr_bits = GOOSE_DEF_NUM_USR_BITS;
  memset (gi->UserSt, 0xAA, GOOSE_DEF_NUM_USR_BITS/8);

  rc = mmsl_send_goose (gi);
  ++SqNum;
  }

#endif	/* MVL_GOOSE_SUPPORT	*/

#if defined(DEBUG_SISCO)
/************************************************************************/
/*			                   preReadCurrMemCount                      */
/************************************************************************/

ST_UINT32 tempCurrMemCount;
ST_UINT32 tempMaxDynMemPtrs;
ST_UINT32 tempMaxMemAllocated;
ST_UINT32 tempCurrMemSize;

MVL_VAR_PROC procsCurrMemCount;
MVL_VAR_PROC procsMaxDynMemPtrs;
MVL_VAR_PROC procsMaxMemAllocated;
MVL_VAR_PROC procsCurrMemSize;


ST_RET preReadCurrMemCount (MVL_VAR_ASSOC **va, 
	                        ST_BOOLEAN alt_acc_pres,
	                        ALT_ACCESS *alt_acc,
	                        MVL_NET_INFO *ni,
	                        ST_INT *aa_mode_out)
  {
  if (alt_acc_pres)
    {
    return (SD_FAILURE);
    }

  tempCurrMemCount = chk_curr_mem_count;
  return (SD_SUCCESS);
  }
ST_RET preReadMaxDynMemPtrs (MVL_VAR_ASSOC **va, 
	                        ST_BOOLEAN alt_acc_pres,
	                        ALT_ACCESS *alt_acc,
	                        MVL_NET_INFO *ni,
	                        ST_INT *aa_mode_out)
  {
  if (alt_acc_pres)
    {
    return (SD_FAILURE);
    }

  tempMaxDynMemPtrs = chk_max_dyn_mem_ptrs;
  return (SD_SUCCESS);
  }

ST_RET preReadMaxMemAllocated (MVL_VAR_ASSOC **va, 
	                        ST_BOOLEAN alt_acc_pres,
	                        ALT_ACCESS *alt_acc,
	                        MVL_NET_INFO *ni,
	                        ST_INT *aa_mode_out)
  {
  if (alt_acc_pres)
    {
    return (SD_FAILURE);
    }

  tempMaxMemAllocated = chk_max_mem_allocated;
  return (SD_SUCCESS);
  }

ST_RET preReadCurrMemSize (MVL_VAR_ASSOC **va, 
	                        ST_BOOLEAN alt_acc_pres,
	                        ALT_ACCESS *alt_acc,
	                        MVL_NET_INFO *ni,
	                        ST_INT *aa_mode_out)
  {
  if (alt_acc_pres)
    {
    return (SD_FAILURE);
    }

  tempCurrMemSize = chk_curr_mem_size;
  return (SD_SUCCESS);
  }
#endif

/************************************************************************/
/*			     init_named_vars				*/
/************************************************************************/

static ST_RET init_named_vars ()
  {
#if defined(DEBUG_SISCO)
OBJECT_NAME obj;
MVL_VAR_ASSOC *varptr;

  /* supply a function for read processing for all variables in the MEMCHK library */
  /* to eliminate exposing the actual library data                                 */

  procsCurrMemCount.pre_read_aa = preReadCurrMemCount;
  obj.object_tag = VMD_SPEC;
  obj.obj_name.vmd_spec = "chk_curr_mem_count";
  varptr = mvl_vmd_var_add (&mvl_vmd, &obj, 
			NULL,
			U32_TYPEID,
			&tempCurrMemCount, 
			&procsCurrMemCount,
			SD_TRUE);
  if (varptr == NULL)
    return (SD_FAILURE);

  obj.obj_name.vmd_spec = "chk_max_dyn_mem_ptrs";
  procsMaxDynMemPtrs.pre_read_aa = preReadMaxDynMemPtrs;

  varptr = mvl_vmd_var_add (&mvl_vmd, &obj, 
			NULL,
			U32_TYPEID,
			&tempMaxDynMemPtrs, 
			&procsMaxDynMemPtrs,
			SD_TRUE);
  if (varptr == NULL)
    return (SD_FAILURE);

  obj.obj_name.vmd_spec = "chk_max_mem_allocated";
  procsMaxMemAllocated.pre_read_aa = preReadMaxMemAllocated;

  varptr = mvl_vmd_var_add (&mvl_vmd, &obj, 
			NULL,
			U32_TYPEID,
			&tempMaxMemAllocated, 
			&procsMaxMemAllocated,
			SD_TRUE);
  if (varptr == NULL)
    return (SD_FAILURE);

  obj.obj_name.vmd_spec = "chk_curr_mem_size";
  procsCurrMemSize.pre_read_aa = preReadCurrMemSize;

  varptr = mvl_vmd_var_add (&mvl_vmd, &obj, 
			NULL,
			U32_TYPEID,
			&tempCurrMemSize, 
			&procsCurrMemSize,
			SD_TRUE);
  if (varptr == NULL)
    return (SD_FAILURE);

  obj.obj_name.vmd_spec = "Temperature";
  varptr = mvl_vmd_var_add (&mvl_vmd, &obj, 
			NULL,
			I16_TYPEID,
			&Temperature, 
			NULL,
			SD_TRUE);
  if (varptr == NULL)
    return (SD_FAILURE);
#endif
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/* REPORTING FUNCTIONS 							*/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/*			init_rpt_ctrl					*/
/************************************************************************/

#ifndef USE_DERIVED_UCA_REPORT_DATASET
ST_CHAR *rptVarNames[] = 
  {
  "PBRO$ST$Out",
  "PBRO$ST$Tar",
  "PBRO$ST$FctDS",
  "PBRO$ST$AuxIn1",
  "PBRO$ST$PuGrp"
  };
ST_INT numRptVarNames = sizeof(rptVarNames)/sizeof(ST_CHAR *);
#endif

static MVLU_RPT_CTRL *init_rpt_ctrl ()
  {
OBJECT_NAME baseVarName;
MVL_VAR_ASSOC *baseVa;
MVL_NVLIST_CTRL *nvl;
MVLU_RPT_SCAN_CTRL *scanCtrl;
ST_INT i;
#ifdef USE_DERIVED_UCA_REPORT_DATASET
ST_CHAR *nodeNames[1];
#endif
#ifndef USE_DERIVED_UCA_REPORT_DATASET
OBJECT_NAME nvlObjName;
OBJECT_NAME varObjNames[5];
#endif
MVLU_RPT_CTRL *globeStRptCtrl = NULL;

/* Use millisecond time for report pacing */
  stimeInit ();

/* Add the GLOBE reports - there are two - MX and ST, we only have ST 	*/
/* Find the base VA, to which the report control block belongs		*/
  baseVarName.object_tag = DOM_SPEC;
  baseVarName.domain_id = "pbroDev";
  baseVarName.obj_name.item_id = "GLOBE";
  baseVa = mvl_vmd_find_var (&mvl_vmd, &baseVarName, NULL);
  if (baseVa == NULL)
    {
    printf ("\nError: Could not find base VA for reports");
    exit (3);
    }

/* First do the globe$RP$brcbST report control */
/* Create the dataset NVL						*/

/* First do the globe$RP$brcbST report control */
/* Create the dataset NVL						*/

#ifdef USE_DERIVED_UCA_REPORT_DATASET
  nodeNames[0] = "PBRO$ST";
  nvl = mvlu_derive_rpt_ds ("pbroDev", "LogDev$ST", 1, nodeNames);
#else
  nvlObjName.object_tag = DOM_SPEC;
  nvlObjName.domain_id = "pbroDev";
  nvlObjName.obj_name.item_id = "LogDev$ST";

  for (i = 0; i < numRptVarNames; ++i)
    {
    varObjNames[i].object_tag = DOM_SPEC;
    varObjNames[i].domain_id = "pbroDev";
    varObjNames[i].obj_name.item_id = rptVarNames[i];
    }
  nvl = mvl_vmd_nvl_add (&mvl_vmd, &nvlObjName, NULL, 5, varObjNames, SD_TRUE);
#endif

  if (nvl != NULL)
    {
  /* Now go ahead and create the report control */
    globeStRptCtrl = mvlu_create_rpt_ctrl ("GLOBE$RP$brcbST", nvl, 
				baseVa,
				RCB_TYPE_UCA,
                                MVLU_RPT_BUFTIM_SEND_NOW,
				10000,		/* BRCB bufsize	*/
				1);		/* ConfRev	*/
#ifdef USE_UCA_REPORT_SCAN
  /* Let MVLU scan our data for changes, using Rd/Wr indication funs	*/
    scanCtrl = mvlu_rpt_create_scan_ctrl (nvl->num_of_entries);
    for (i = 0; i < nvl->num_of_entries; ++i)
      scanCtrl->scan_va[i]= nvl->entries[i];
    memcpy (scanCtrl->scan_va_scope, nvl->va_scope,
            nvl->num_of_entries * sizeof (MVL_SCOPE));
    scanCtrl->enable = SD_TRUE;
    /* Optional user function to be called when each scan completes.	*/
    scanCtrl->indCtrl.usr_resp_fun = my_rpt_scan_done;
#endif
    }

/* Initialize some of our reported variable data */
  pbro_st.Out = 1;
  pbro_st.Tar = 10;
  pbro_st.FctDS.b2[0] = 3;
  pbro_st.FctDS.q[0] = 0xa5;
  pbro_st.FctDS.q[1] = 0xa5;
  pbro_st.FctDS.t_ms = 1999;	/* return same time as mvlu_null_rd_ind	*/
  pbro_st.FctDS.t_days = 0;
  pbro_st.AuxIn1[0] = 0x5a;
  pbro_st.AuxIn1[1] = 0x5a;
  pbro_st.PuGrp = 20;

  autoRptTestSet = SD_TRUE;

  return (globeStRptCtrl);
  }

/************************************************************************/
/*			init_rpt_ctrl_iec_brcb				*/
/* Create IEC-61850 Buffered Report control block			*/
/************************************************************************/
static MVLU_RPT_CTRL *init_rpt_ctrl_iec_brcb ()
  {
OBJECT_NAME baseVarName;
MVL_VAR_ASSOC *baseVa;
MVL_NVLIST_CTRL *nvl;
ST_CHAR *nodeNames[1];
MVLU_RPT_CTRL *RptCtrl = NULL;

  baseVarName.object_tag = DOM_SPEC;
  baseVarName.domain_id = "LD0";
  baseVarName.obj_name.item_id = "POPF";
  baseVa = mvl_vmd_find_var (&mvl_vmd, &baseVarName, NULL);
  if (baseVa == NULL)
    {
    printf ("\nError: Could not find base VA for IEC-61850 Buffered report");
    return (NULL);
    }
  nodeNames[0] = "POPF$ST";
  nvl = mvlu_derive_rpt_ds ("LD0", "LogDevIec$ST", 1, nodeNames);

  if (nvl != NULL)
    RptCtrl = mvl61850_create_rpt_ctrl ("POPF$BR$brcbST", nvl, 
				baseVa,
				RCB_TYPE_IEC_BRCB,
                                MVLU_RPT_BUFTIM_SEND_NOW,
				10000,		/* BRCB bufsize	*/
				1);		/* ConfRev	*/
  return (RptCtrl);
  }

/************************************************************************/
/*			init_rpt_ctrl_iec_urcb				*/
/* Create IEC-61850 Unbuffered Report control block			*/
/************************************************************************/
static MVLU_RPT_CTRL *init_rpt_ctrl_iec_urcb ()
  {
OBJECT_NAME baseVarName;
MVL_VAR_ASSOC *baseVa;
MVL_NVLIST_CTRL *nvl;
ST_CHAR *nodeNames[1];
MVLU_RPT_CTRL *RptCtrl = NULL;

  baseVarName.object_tag = DOM_SPEC;
  baseVarName.domain_id = "LD0";
  baseVarName.obj_name.item_id = "POPF";
  baseVa = mvl_vmd_find_var (&mvl_vmd, &baseVarName, NULL);
  if (baseVa == NULL)
    {
    printf ("\nError: Could not find base VA for IEC-61850 Unbuffered report");
    return (NULL);
    }
  nodeNames[0] = "POPF$ST";
  nvl = mvlu_derive_rpt_ds ("LD0", "LogDevIecUrcb$ST", 1, nodeNames);

  if (nvl != NULL)
    RptCtrl = mvl61850_create_rpt_ctrl ("POPF$RP$urcbST", nvl, 
				baseVa,
				RCB_TYPE_IEC_URCB,
                                MVLU_RPT_BUFTIM_SEND_NOW,
				10000,		/* BRCB bufsize	*/
				1);		/* ConfRev	*/
  if (RptCtrl)
    {	/* Start with some default options we want	*/
    RptCtrl->common_basrcb.IntgPd = 5000;
    BSTR_BIT_SET_ON (&RptCtrl->common_basrcb.OptFlds.data_1, OPTFLD_BITNUM_SQNUM);
    }
  return (RptCtrl);
  }

/************************************************************************/
/*			my_rpt_scan_done				*/
/************************************************************************/
/* This optional function is called when a report scan completes because*/
/* the function pointer "scanCtrl->indCtrl.usr_resp_fun" points to it	*/
/* (see init_rpt_ctrl).							*/
/* This function is NOT required. It is only called if the function	*/
/* pointer in "scanCtrl->indCtrl.usr_resp_fun" points to it.		*/

#ifdef USE_UCA_REPORT_SCAN
ST_RET my_rpt_scan_done (MVL_IND_PEND *indCtrl)
  {
MVLU_RPT_SCAN_CTRL *scanCtrl = (MVLU_RPT_SCAN_CTRL *)indCtrl->usr_ind_ctrl;
  if (scanCtrl->num_va_changes != 0)
    {
    }
  return (SD_SUCCESS);
  }
#endif

/************************************************************************/
/************************************************************************/

/* Inclusion bitstring test cases */
#define MAX_INC_TEST_SETS	6
ST_UINT8 vaChangeTestValues[MAX_INC_TEST_SETS] =
  {
  0x80,  0x40,  0x20,  0x10,  0x08,  0xF1
  };
ST_INT nextVaChangeTestSet = 0;


/* Reasons test cases */
#define MAX_REASONS_TEST_SETS	3
ST_UINT8 reasonsTestValues[MAX_REASONS_TEST_SETS] =
  {
  MVLU_TRGOPS_DATA, MVLU_TRGOPS_QUALITY, MVLU_TRGOPS_FREEZE
  };
ST_INT nextReasonsTestSet = 0;


/* Options bitstring test cases */
#define MAX_OPT_TEST_SETS 16
ST_UINT8 optTestValues[MAX_OPT_TEST_SETS] =
  {
									0 , 
                                                          MVLU_OUTDAT_MASK, 
                                      MVLU_REASONS_MASK                   , 
                                      MVLU_REASONS_MASK | MVLU_OUTDAT_MASK,
                   MVLU_RPTTIM_MASK                                       ,
                   MVLU_RPTTIM_MASK |                     MVLU_OUTDAT_MASK,
                   MVLU_RPTTIM_MASK | MVLU_REASONS_MASK 		  ,
                   MVLU_RPTTIM_MASK | MVLU_REASONS_MASK | MVLU_OUTDAT_MASK,
MVLU_SQNUM_MASK  							  ,
MVLU_SQNUM_MASK |                                        MVLU_OUTDAT_MASK,
MVLU_SQNUM_MASK |                    MVLU_REASONS_MASK                   ,
MVLU_SQNUM_MASK |                    MVLU_REASONS_MASK | MVLU_OUTDAT_MASK,
MVLU_SQNUM_MASK | MVLU_RPTTIM_MASK                                       ,
MVLU_SQNUM_MASK | MVLU_RPTTIM_MASK |                     MVLU_OUTDAT_MASK,
MVLU_SQNUM_MASK | MVLU_RPTTIM_MASK | MVLU_REASONS_MASK 		  ,
MVLU_SQNUM_MASK | MVLU_RPTTIM_MASK | MVLU_REASONS_MASK | MVLU_OUTDAT_MASK,
  };
ST_INT nextOptTestSet = 0;

/************************************************************************/
/************************************************************************/
/*			changeRptVars					*/
/************************************************************************/


static ST_VOID changeRptVars (MVLU_RPT_CTRL *rptCtrl)
  {
ST_INT i;
MVL_NVLIST_CTRL *dsNvl;
ST_UINT8 bitMask;
ST_UINT8 *bytePtr;
ST_UINT8 reason_data;
ST_UINT8 varsToChange;
ST_DOUBLE timeNow;
ST_VOID *new_data;



/* See if it is time to send reports */
  timeNow = sGetMsTime ();
  if (timeNow < nextRptVarChangeTime)
    return;
  nextRptVarChangeTime = timeNow + DATA_CHANGE_PERIOD;

#if defined(_REPORT_TEST)
  incVaChangeTestSet ();
#endif

  reason_data = reasonsTestValues[nextReasonsTestSet];

  dsNvl = rptCtrl->dsNvl;
  bitMask = 0x80;
  varsToChange = vaChangeTestValues[nextVaChangeTestSet];
  bytePtr = &varsToChange;
  for (i = 0; i < dsNvl->num_of_entries; ++i)
    {
    if (*bytePtr & bitMask)
      {
      new_data = change_rpt_var_val (i);
#ifdef USE_UCA_REPORT_ASYNC_DATA_CHANGE
      mvlu_rpt_va_change (dsNvl->entries[i], reason_data, new_data);
#endif
      }  

    if (bitMask == 0x01)
      {
      bitMask = 0x80;
      ++bytePtr;
      }
    else
      bitMask = bitMask >> 1;
    }  

  if (autoRptTestSet == SD_TRUE)
    incReasonTestSet ();
  }


/************************************************************************/
/*			change_rpt_var_val 				*/
/************************************************************************/

static ST_VOID *change_rpt_var_val (ST_INT i)
  {
ST_VOID *new_data;
time_t curTime;
MMS_BTIME6 curBtod;

  switch (i)
    {
    case 0:
      pbro_st.Out = pbro_st.Out ? 0 : 1;
      new_data = &pbro_st.Out;
    break;

    case 1:
      ++pbro_st.Tar;
      new_data = &pbro_st.Tar;
    break;

    case 2:
      pbro_st.FctDS.b2[0] ^= 0xff;
      pbro_st.FctDS.q[0] = 0xa5;
      pbro_st.FctDS.q[1] = 0xa5;
      curTime = time (NULL);
      asn1_convert_timet_to_btime6 (curTime, &curBtod);
      pbro_st.FctDS.t_ms = curBtod.ms;
      pbro_st.FctDS.t_days = curBtod.day;
#ifdef _WIN32
      {	/* Get milliseconds from SYSTEMTIME	*/
      SYSTEMTIME systime;
      GetLocalTime (&systime);
      pbro_st.FctDS.t_ms += systime.wMilliseconds;	/* add ms this sec, to ms this day*/
      }
#endif
      new_data = &pbro_st.FctDS;
    break;

    case 3:
      pbro_st.AuxIn1[0] ^= 0xff;
      pbro_st.AuxIn1[1] ^= 0xff;
      new_data = &pbro_st.AuxIn1;
    break;

    case 4:
      ++pbro_st.PuGrp;
      new_data = &pbro_st.PuGrp;
    break;
    }
  return (new_data);
  }

/************************************************************************/
/*                       incVaChangeTestSet				*/
/************************************************************************/
#if defined (_REPORT_TEST)

static ST_VOID incVaChangeTestSet ()
  {
  ++nextVaChangeTestSet;
  if (nextVaChangeTestSet >=	MAX_INC_TEST_SETS)
    nextVaChangeTestSet = 0;
  if (autoRptTestSet == SD_FALSE)
    {
    printf ("\n Inclusion test set %d, mask 0x%02x", 
			nextVaChangeTestSet, vaChangeTestValues[nextVaChangeTestSet]);
    printf ("\n");
    }
  }
#endif
/************************************************************************/
/*                       incReasonTestSet				*/
/************************************************************************/

static ST_VOID incReasonTestSet ()
  {
  ++nextReasonsTestSet;
  if (nextReasonsTestSet >= MAX_REASONS_TEST_SETS)
    nextReasonsTestSet = 0;
  if (autoRptTestSet == SD_FALSE)
    {
    printf ("\n Reason test set %d, value 0x%02x", 
		nextReasonsTestSet, reasonsTestValues[nextReasonsTestSet]);
    printf ("\n");
    }
  }

/************************************************************************/
/*                       incOptTestSet					*/
/************************************************************************/

#if defined (_REPORT_TEST)

static ST_VOID incOptTestSet (MVLU_RPT_CTRL *globeStRptCtrl)
  {
  ++nextOptTestSet;
  if (nextOptTestSet >= MAX_OPT_TEST_SETS)
    nextOptTestSet = 0;
  if (autoRptTestSet == SD_FALSE)
    {
    printf ("\n Options test set %.2d, mask 0x%02x: ", 
    		nextOptTestSet, optTestValues[nextOptTestSet]);
    
    if (optTestValues[nextOptTestSet] & MVLU_OUTDAT_MASK)
      printf ("OUTDAT ");
    if (optTestValues[nextOptTestSet] & MVLU_REASONS_MASK)
      printf ("REASONS ");
    if (optTestValues[nextOptTestSet] & MVLU_RPTTIM_MASK)
      printf ("RPTTIM ");
    if (optTestValues[nextOptTestSet] & MVLU_SQNUM_MASK)
      printf ("SQNUM ");
    printf ("\n");
    }

  globeStRptCtrl->common_basrcb.OptFlds.data_1[0] = 
					optTestValues[nextOptTestSet];
  }      
#endif

/************************************************************************/
/************************************************************************/
/* INDICATION HANDLING FUNCTIONS					*/
/************************************************************************/
/*			u_mvl_ident_ind					*/
/************************************************************************/

ST_VOID u_mvl_ident_ind (MVL_IND_PEND *indCtrl)
  {
  indCtrl->u.ident.resp_info = &identify_response_info;
  mplas_ident_resp (indCtrl);
  }

/************************************************************************/
/*			u_mvl_status_ind				*/
/************************************************************************/

ST_VOID u_mvl_status_ind (MVL_IND_PEND *indCtrl)
  {
  indCtrl->u.status.resp_info = &status_resp_info;
  mplas_status_resp (indCtrl);
  }

/************************************************************************/
/*			u_mvl_getdom_ind				*/
/************************************************************************/

ST_VOID u_mvl_getdom_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_getdom_resp (indCtrl);
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
/*			u_mvl_defvlist_ind				*/
/************************************************************************/

ST_VOID u_mvl_defvlist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_defvlist_resp (indCtrl);
  }

/************************************************************************/
/*			u_mvl_delvlist_ind				*/
/************************************************************************/
ST_VOID u_mvl_delvlist_ind (MVL_IND_PEND *indCtrl)
  {
  mvlas_delvlist_resp (indCtrl);
  }

/************************************************************************/
/*			u_mvl_getcl_ind					*/
/************************************************************************/
ST_VOID u_mvl_getcl_ind (MVL_IND_PEND *ind)
  {
ST_INT16 num_of_capab;
GETCL_RESP_INFO *info;	/* Same as "ind->u.getcl.resp_info".	*/
			/* Only used to simpify filling it in.	*/
ST_CHAR **capab_list;

  num_of_capab = 4;
  /* Allocate space for Resp struct AND array of pointers to capabilities.*/
  info = ind->u.getcl.resp_info = chk_malloc (sizeof(GETCL_RESP_INFO) +
                                         num_of_capab * sizeof(ST_CHAR *));

  /* Fill in response structure.		*/
  info->more_follows = SD_FALSE;
  info->num_of_capab = num_of_capab;

  /* Fill in array of ptrs to capabilities.	*/
  capab_list = (ST_CHAR **) (info + 1);	/* Point after struct.	*/
  if (num_of_capab > 0)
    capab_list [0] = "SampleCapability_0";
  if (num_of_capab > 1)
    capab_list [1] = "SampleCapability_1";
  if (num_of_capab > 2)
    capab_list [2] = "SampleCapability_2";
  if (num_of_capab > 3)
    capab_list [3] = "SampleCapability_3";
    
  mplas_getcl_resp (ind);	/* Primitive		*/
  chk_free (info);		/* Done with struct, so free it.	*/
  }

/************************************************************************/
/*			u_mvl_concl_ind					*/
/************************************************************************/
ST_VOID u_mvl_concl_ind (MVL_COMM_EVENT *event)
  {
  /* This function should do all appropriate cleanup before sending the	*/
  /* Conclude response.							*/

  /* CRITICAL: If any requests or indications are pending, or if any	*/
  /* files are open, don't allow conclude.				*/
  if (event->net_info->numpend_ind > 0 || event->net_info->numpend_req > 0
      || event->net_info->file_list != NULL)
    mplas_concl_err (event, MMS_ERRCLASS_CONCLUDE,
                     MMS_ERRCODE_FURTHER_COMM_REQUIRED);
  else
    mplas_concl_resp (event);	/* Send response.	*/
  }

/************************************************************************/
/************************************************************************/
/* CONNECTION MANAGEMENT FUNCTIONS					*/
/************************************************************************/
/*			connect_ind_fun					*/
/************************************************************************/
/* This function is called when a remote node has connected to us. We	*/
/* can look at the assReqInfo to see who it is (assuming that the 	*/
/* AE-Titles are used), or at the cc->rem_init_info to see initiate 	*/
/* request parameters.							*/

static ST_INT  connect_ind_fun (MVL_NET_INFO *cc, INIT_INFO *init_info)
  {
static INIT_INFO initRespInfo;

  initRespInfo.mms_p_context_pres = SD_TRUE;
  initRespInfo.max_segsize_pres = SD_TRUE;
  initRespInfo.max_segsize = mvl_cfg_info->max_msg_size;
  initRespInfo.maxreq_calling = 1;
  initRespInfo.maxreq_called = 3;
  initRespInfo.max_nest_pres = SD_TRUE;
  initRespInfo.max_nest = 5;
  initRespInfo.mms_detail_pres = SD_TRUE;
  initRespInfo.version = 1;
  initRespInfo.num_cs_init = 0;
  initRespInfo.core_position = 0;
  initRespInfo.param_supp[0] = m_param[0];
  initRespInfo.param_supp[1] = m_param[1];
  memcpy (initRespInfo.serv_supp, m_service_resp,11);

  mvl_init_resp_info = &initRespInfo;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			u_mvl_connect_ind_ex				*/
/************************************************************************/
/* This function is called when a remote node has connected to us and	*/
/* requires us to look at the authentication info sent to us in the	*/
/* req_auth_info pointer. After we have verified ACSE authentication, we*/
/* can fill out an authentication structure of our own to send to our   */
/* partner and continue processing the connection normally.		*/

ST_ACSE_AUTH u_mvl_connect_ind_ex (MVL_NET_INFO *cc, INIT_INFO *init_info, 
            ACSE_AUTH_INFO *req_auth_info, ACSE_AUTH_INFO *rsp_auth_info)
  {
ST_RET ret;

#if defined(S_SEC_ENABLED)
  ret = ulCheckSecureAssocInd (cc, rsp_auth_info);
  if (ret != ACSE_AUTH_SUCCESS)
    {
    printf ("\n Connection not authenticated");
    return ((ST_ACSE_AUTH) ret);
    }
#elif defined(ACSE_AUTH_ENABLED)
  if(req_auth_info->auth_pres == SD_TRUE)
    {
    /* Looks like we have some authentication to look at, simply print  */
    /* the password and continue as normal.				*/
    if(req_auth_info->mech_type == ACSE_AUTH_MECH_PASSWORD)
      {
      printf("\nPassword received from peer: %s\n", req_auth_info->u.pw_auth.password);
      }
    /* We were sent a mechanism we don't support, let's reject the 	*/
    /* the connection with the appropriate diagnostic.			*/
    else
      {
      return ACSE_DIAG_AUTH_MECH_NAME_NOT_RECOGNIZED;
      }
    }
  else
    {
    /* Hmm... looks like we weren't sent any authentication even though */
    /* we require it. Let's reject with the appropriate diagnostic.	*/
    return ACSE_DIAG_AUTH_REQUIRED;
    }
  
  /* Fill out a response authentication structure, must use the pointer */
  /* provided to us here.						*/
  rsp_auth_info->auth_pres = SD_TRUE;
  rsp_auth_info->mech_type = ACSE_AUTH_MECH_PASSWORD;
  /* this is the password */
  strcpy(rsp_auth_info->u.pw_auth.password, "secret");
#endif /* defined(ACSE_AUTH_ENABLED) */
  
  /* Now proceed processing the connection as normal */   
  ret = connect_ind_fun(cc, init_info);
  
  if (ret == SD_SUCCESS)
    return ACSE_AUTH_SUCCESS;
  else
    return ACSE_DIAG_NO_REASON;
  }

/************************************************************************/
/*			disc_ind_fun					*/
/************************************************************************/
/* This function is called when connection is terminated.		*/

static ST_VOID disc_ind_fun (MVL_NET_INFO *cc, ST_INT discType)
  {
  printf ("disconnect indication received.\n");
  mvlu_clr_pend_sbo (cc);
  }

/************************************************************************/
/************************************************************************/
/* INITIALIZATION FUNCTIONS 						*/
/************************************************************************/

/************************************************************************/
/*				init_acse				*/
/************************************************************************/

static ST_VOID init_acse ()
  {
ST_RET ret;
MVL_CFG_INFO mvlCfg;

/* We want to know about connection activity				*/
  u_mvl_disc_ind_fun     = disc_ind_fun;
  
  ret = osicfgx ("osicfg.xml", &mvlCfg);	/* This fills in mvlCfg	*/
  if (ret != SD_SUCCESS)
    {
    printf ("Stack configuration failed, err = 0x%X. Check configuration.\n", ret );
    exit (1);
    }

  ret = mvl_start_acse (&mvlCfg);	/* MAKE SURE mvlCfg is filled in*/
  if (ret != SD_SUCCESS)
    {
    printf ("mvl_start_acse () failed, err = %d.\n", ret );
    exit (1);
    }
  }

/************************************************************************/
/*			init_log_cfg					*/
/************************************************************************/

static ST_VOID init_log_cfg (ST_VOID)
  {
#ifdef DEBUG_SISCO

#if defined(S_SEC_ENABLED)
  logCfgAddMaskGroup (&secLogMaskMapCtrl);
  logCfgAddMaskGroup (&ssleLogMaskMapCtrl);
#endif
  logCfgAddMaskGroup (&mvlLogMaskMapCtrl);
  logCfgAddMaskGroup (&mmsLogMaskMapCtrl);
  logCfgAddMaskGroup (&acseLogMaskMapCtrl);
  logCfgAddMaskGroup (&tp4LogMaskMapCtrl);
  logCfgAddMaskGroup (&clnpLogMaskMapCtrl);
  logCfgAddMaskGroup (&asn1LogMaskMapCtrl);
  logCfgAddMaskGroup (&sxLogMaskMapCtrl);
#if defined(S_MT_SUPPORT)
  logCfgAddMaskGroup (&gsLogMaskMapCtrl);
#endif
  logCfgAddMaskGroup (&sockLogMaskMapCtrl);
  logCfgAddMaskGroup (&memLogMaskMapCtrl);
  logCfgAddMaskGroup (&memDebugMapCtrl);
  
  /* At initialization, install a SLOGIPC command handler. The        */
  /* build in SLOGIPC handler just receives the command and put's     */
  /* on a list to be handled by the application at it's leisure ...   */
  sLogCtrl->ipc.slog_ipc_cmd_fun = slog_ipc_std_cmd_fun;              
      
  if (logcfgx_ex (sLogCtrl, "logcfg.xml", NULL, SD_FALSE, SD_FALSE) != SD_SUCCESS)
    {
    printf ("\n Parsing of 'logging' configuration file failed.");
    if (sLogCtrl->fc.fileName)
      printf ("\n Check log file '%s'.", sLogCtrl->fc.fileName);
    exit (5);
    }

  slog_start (sLogCtrl, MAX_LOG_SIZE);  /* call after logging parameters are configured	*/

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

/* Allocate spare memory to allow logging/printing memory errors	*/
  spareMem = malloc (500);

/* trap mem_chk errors							*/
  mem_chk_err = mem_chk_error_detected;
  m_memerr_fun = my_malloc_err;
  c_memerr_fun = my_calloc_err;
  r_memerr_fun = my_realloc_err;

#if 0
  m_heap_check_enable = SD_TRUE;
  m_check_list_enable = SD_TRUE;
  m_no_realloc_smaller = SD_TRUE;
  m_fill_en = SD_TRUE;
#endif
  m_mem_debug = SD_TRUE;

  }

#ifdef SMEM_ENABLE
/************************************************************************/
/*			u_smem_get_pool_params				*/
/************************************************************************/

ST_VOID  u_smem_get_pool_params ( SMEM_CONTEXT *contextName,
                                  ST_UINT8 *numBuf, ST_UINT *bufSize,
			          ST_BOOLEAN *autoClone,
			          ST_INT *maxClones,
			          ST_CHAR **poolName)
  {
  *bufSize = 9999;
  *numBuf = 9;
  *autoClone = SD_FALSE;
  *maxClones = 0;
  *poolName = NULL;		/* pool name not very useful.	*/
  SLOGALWAYS0 ("CALLBACK: u_smem_get_pool_params");
  }

/************************************************************************/
/*			u_smem_need_buffers				*/
/************************************************************************/

SMEM_POOL_CTRL *u_smem_need_buffers (SMEM_CONTEXT *smemContext, 
				     ST_UINT8 numBuf, ST_UINT bufSize)
  {
SMEM_POOL_CTRL *newSc;

  /* numBuf = # of bufs in last pool created for this bufSize  OR	*/
  /*          0 if this is the first pool created for this bufSize.	*/
  /* If numBuf = 0, then the user must choose an appropriate value.	*/
  if (numBuf == 0)
    numBuf = 10;	/* choose a value	*/
  if (bufSize == 0)
    bufSize = 1000;	/* choose a value	*/

  newSc = smem_add_pool (smemContext, numBuf, bufSize,
                         SD_FALSE,	/* autoClone	*/
                         0,		/* maxClones	*/
                         NULL);		/* poolName	*/
  return (newSc);
  }
#endif	/* SMEM_ENABLE	*/

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
    printf ("\n Memory Error Detected!");
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

/************************************************************************/
/* MISC. FUNCTIONS							*/
/************************************************************************/
/************************************************************************/
/*				ctrlCfun				*/
/************************************************************************/
/* This function handles the ^c, and allows us to cleanup on exit	*/

ST_VOID ctrlCfun (int i)
  {
  doIt = SD_FALSE;
  }


/************************************************************************/
/*			reportKbService ()				*/
/************************************************************************/

/* NOTE for some UNIX systems:						*/
/*	This function, if enebled, needs ukey.c to be linked into the	*/
/*	project (for the kbhit function).				*/
static ST_VOID reportKbService (MVLU_RPT_CTRL *globeStRptCtrl)
  {
#if defined (_REPORT_TEST) || defined(S_SEC_ENABLED)
#if !defined(NO_KEYBOARD)
ST_CHAR c;

  if (kbhit ())		/* Report test keyboard input */
    {
#if defined(_WIN32) || defined(__QNX__)
    c = getch ();
#else
    c = (ST_CHAR) getchar ();	/* works only if term_init called first	*/
#endif

#if defined (_REPORT_TEST)
    if (c == 'c')
      {
      nextRptVarChangeTime = 0.0;
      changeRptVars (globeStRptCtrl);
      }
    if (c == 'o')
      incOptTestSet (globeStRptCtrl);
    if (c == 'i')
      incVaChangeTestSet ();
    if (c == 'r')
      incReasonTestSet ();
    if (c == 'a')
      {
      autoRptTestSet = SD_TRUE;
      printf ("\n Automatic Report Test Mode\n");
      }
    if (c == 'm')
      {
      autoRptTestSet = SD_FALSE;
      printf ("\n Manual Report Test Mode\n");
      }
#endif /* defined (_REPORT_TEST) */

#if defined(S_SEC_ENABLED)
    if (c == 'u')
      secManCfgChange();
#endif /* defined(S_SEC_ENABLED) */

    if (c == '?')
      {
#if defined (_REPORT_TEST)
      printf ("\n o : Change Report Options (must reconnect to make effective)");
      printf ("\n c : Change Values of Active Report Variables");
      printf ("\n i : Change Active Report Variable Set");
      printf ("\n r : Change Reason Value");
      printf ("\n a : Auto Report Test Mode");
      printf ("\n m : Manual Report Test Mode");
#endif /* defined (_REPORT_TEST) */
#if defined(S_SEC_ENABLED)
      printf ("\n u : Update Security Configuration");
#endif /* defined(S_SEC_ENABLED) */
      }
    }
#endif	/* !defined(NO_KEYBOARD)	*/
#endif /* defined (_REPORT_TEST) || defined(S_SEC_ENABLED) */
  }

/************************************************************************/
/************************************************************************/
#if defined MVLU_LEAF_FUN_LOOKUP_ENABLE
/************************************************************************/
/* DYNAMIC LEAF ACCESS PARAMETER MANIPULATION				*/
/************************************************************************/
/*			u_mvlu_resolve_leaf_ref				*/
/************************************************************************/
/* This callback function is invoked from within the MVLU LAP file	*/
/* load function mvlu_load_xml_leaf_file from mvlu_set_leaf_param_name.	*/

/* We are to provide the leaf reference value for the named leaf via	*/
/* the outparameter refOut, nominally by looking at the reference text	*/
/* that is passed to us.						*/

/* The bit masked input/output parameter setFlagsIo tells us whether	*/
/* the reference text was present in the LAP file, and we can set or	*/
/* clear the bit to control whether the reference value is written.	*/

ST_RET u_mvlu_resolve_leaf_ref (ST_CHAR *leafName, ST_INT *setFlagsIo, 
				ST_CHAR *refText, ST_RTREF *refOut)
  {
ST_CHAR *p;

/* Note: In this example  we won't really set the reference, because 	*/
/* the references we need are already set in the leaf at compile time 	*/
/* by the Foundry LAP input file					*/

/* First let's see if the reference text was provided 			*/
  if (*setFlagsIo & MVLU_SET_REF)
    {
  /* The refText text was provided in the LAP file.			*/
  /* We need to check for a special PREFIX (used especially for SBO).	*/
    if (!strncmp (refText, MVLU_STRING_REF_PREFIX, strlen (MVLU_STRING_REF_PREFIX)))
      {		/* Write everything after PREFIX as quoted string.	*/
      p = refText + strlen(MVLU_STRING_REF_PREFIX);
      *refOut = (ST_RTREF) chk_strdup (p);
      }
    else	/* If no PREFIX, we don't handle it.			*/
      *setFlagsIo &= ~MVLU_SET_REF;
    }
  else
    {
  /* The refText text was NOT provided in the LAP file.			*/
  /* If we wanted, we could still provide a reference to be used ...	*/
#if 0
    *refOut = (ST_RTREF) 0;
    *setFlagsIo |= MVLU_SET_REF;
#endif
    }

/* We always return success, otherwise the LAP will not be set		*/
  return (SD_SUCCESS);
  }
#endif

/************************************************************************/
/*			u_mvlu_rpt_time_get				*/
/* UCA/IEC reporting callback function to get accurate rpt time.	*/
/* If UCA/IEC Reports are enabled, user must provide this function.	*/
/************************************************************************/
ST_VOID u_mvlu_rpt_time_get (MMS_BTIME6 *TimeOfEntry)
  {
#if 1	/* This is a very simple example using the standard "time" fct.	*/
  time_t curTime;

  curTime = time (NULL);
  asn1_convert_timet_to_btime6 (curTime, TimeOfEntry);
#ifdef _WIN32
  {	/* Get milliseconds from SYSTEMTIME	*/
  SYSTEMTIME systime;
  GetLocalTime (&systime);
  TimeOfEntry->ms += systime.wMilliseconds;	/* add ms this sec, to ms this day*/
  }
#endif

#else	/* This is simplest possible code if user doesn't care about time.*/
  TimeOfEntry->ms = 0;
  TimeOfEntry->day = 0;
#endif
  return;
  }

/************************************************************************/
/*			u_mvl61850_ctl_oper_begin			*/
/* Sample user callback function.					*/
/************************************************************************/
ST_VOID u_mvl61850_ctl_oper_begin (ST_CHAR *oper_ref)
  {
  }
/************************************************************************/
/*			u_mvl61850_ctl_oper_end				*/
/* Sample user callback function.					*/
/* NOTE: see scl_srvr.c for a better sample function.			*/
/************************************************************************/
ST_VOID u_mvl61850_ctl_oper_end (MVL_NET_INFO *net_info, ST_CHAR *oper_ref, MVL_VAR_ASSOC *base_var)
  {
  }
/************************************************************************/
/*			u_ml_get_rt_type				*/
/* Customized function to find a base type for constructing a more	*/
/* complex type. This replaces the dummy function in the "mmsl" library.*/
/************************************************************************/
ST_RET u_ml_get_rt_type (ASN1_DEC_CTXT *aCtx, OBJECT_NAME *type_name,
			RUNTIME_TYPE **rt_out, ST_INT *num_rt_out)
  {
ST_INT type_id;
ST_RET rc;

  type_id = mvl_typename_to_typeid (type_name->obj_name.vmd_spec);
  /* mvl_get_runtime chks type_id, so no need to check it here.		*/
  rc = mvl_get_runtime (type_id, rt_out, num_rt_out);
  return (rc);
  }
