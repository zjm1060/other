/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	1998 - 2002, All Rights Reserved		        */
/*									*/
/* MODULE NAME : mi_main.c    						*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*	NONE								*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/20/02  MDE     22	   Removed setting matching control variables	*/
/* 06/20/02  RKR     21    Copyright year update                        */
/* 06/20/02  MDE     11	   Now use default matching (selectors too)	*/
/* 04/16/02  MDE     10	   Now use mlog					*/
/* 03/21/02  MDE     09	   Added Association State request		*/
/* 02/25/02  MDE     08    Changes for updated MVL_CFG_INFO		*/
/* 02/20/02  MDE     07    Now work with Marben stack too		*/
/* 01/25/02  MDE     06	   Added redundancy support code		*/
/* 10/19/01  MDE     05	   Minor cleanup				*/
/* 10/13/99  NAV     04    Suport for Block4 and Block 5		*/
/* 04/12/99  MDE     03    Minor cleanup				*/
/* 04/01/99  MDE     02    Use MI_ASSOC_CTRL for connection management	*/
/* 01/28/98  MDE     01    New module					*/
/************************************************************************/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "time.h"
#include "signal.h"
#if defined(_WIN32)
#include "conio.h"
#endif

#include "mvl_acse.h"
#include "mvl_log.h"
#include "mloguser.h"
#include "miusrobj.h"
#include "mi.h"
#include "mi_usr.h"
#include "mitypeid.h"	/* include in one user source file only! */

#include "sx_defs.h"

#if defined(MAP30_ACSE)
#include "suicacse.h"
#endif

#if defined (MOSI)		/* If stack is MOSI, use events		*/
ST_VOID wait_any_event (ST_LONG max_ms_delay);
#endif

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_CHAR *thisFileName = __FILE__;
#endif

/************************************************************************/
/************************************************************************/
/* General program control variables */

ST_BOOLEAN miuDoIt = SD_TRUE;
ST_INT miuMode;
ST_BOOLEAN miuAuto = SD_FALSE;
ST_BOOLEAN miuFullSpeed = SD_FALSE;

ST_BOOLEAN micuAssocEnable = SD_TRUE;
ST_BOOLEAN micuLoopEnable = SD_FALSE;
MI_ASSOC_CTRL *miuClientRemoteAssoc;
MI_CONN *miuMiConn;

/************************************************************************/

/* Identify server information						*/
IDENT_RESP_INFO identify_response_info =
  {
  "SISCO",  		/* Vendor 	*/
  "ICCP-Lite",  	/* Model  	*/
  "3.0050",  0,		/* Version 	*/
  };

/************************************************************************/
/* Functions in this module						*/

static ST_VOID miuSetIccpMode (int argc, char *argv[]);
static ST_VOID miuInitLogCfg (ST_VOID);
static ST_VOID miuInitMem (ST_VOID);
static ST_VOID miuAutoService (ST_VOID);

/*######################################################################*/
#if defined (MOSI)		/* Events only supported for MOSI.	*/

extern int pipe_to_main;	/* DEBUG: move to include	*/

ST_VOID wait_any_event (ST_LONG max_ms_delay)
  {
  struct timeval stTimeVal;
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  int    nfds;   

  FD_ZERO (&readfds);
  FD_ZERO (&writefds);
  FD_ZERO (&exceptfds);
  FD_SET (0, &readfds);		/* select on "stdin"	*/
  FD_SET (pipe_to_main, &readfds );	/* select on MMS Event pipe */

  /* pipe_to_main is only fds to check, so set nfds one greater.	*/
  nfds = pipe_to_main+1;

  /* If you want to wait for other events too, add more FD_SET calls,	*/
  /* and recompute "nfds" (or just set it to FD_SETSIZE).		*/

  max_ms_delay *= 1000; /*** Convert to micro seconds ***/
  stTimeVal.tv_sec  = max_ms_delay / 1000000;
  stTimeVal.tv_usec = max_ms_delay % 1000000;

  nfds = select (nfds, (int *)&readfds, (int *)&writefds, (int *)&exceptfds, &stTimeVal);
  }

#endif		/* MOSI				*/
/*######################################################################*/
/************************************************************************/
/************************************************************************/
/*                       main						*/
/************************************************************************/

MMS_OBJ_ID asn1_obj_id;          /* ASN.1 Object ID      */
MMS_OBJ_ID acse_obj_id;          /* ACSE Object ID       */
MMS_OBJ_ID mms_obj_id;           /* ISO MMS Object ID    */

#if defined (VXWORKS)	/* Just make this look like "main" call.	*/
ST_VOID mmsserver ()
#else	/* !VXWORKS	*/  
int main (int argc, char *argv[])
#endif	/* !VXWORKS	*/  
  {
  miuInitMem ();  	/* Set up memory allocation tools 		*/
  miuInitLogCfg ();	/* Set up logging subsystem			*/
  miuSetIccpMode (argc, argv);	/* See if we are client, server, both	*/

  SLOGALWAYS0 ("Initializing ...");
  miuInitIccp ();		/* Initialize ICCP Lite			*/
  SLOGALWAYS0 ("Initialization complete");

/* We are all initialized, just service communications			*/
  printf ("\nInitialization complete, hit 'x' to exit ... ");
  chk_malloc (1);	/* marker */
  while (miuDoIt)
    {
#if defined (MOSI)		/* If stack is MOSI, use events		*/
    wait_any_event (20L);	/* Wait 1000 milliseconds		*/
    while (mvl_comm_serve ())	/* Perform communications service 	*/
      {				/* Loop until "mvl_comm_serve" returns 0*/
      }
#else
    mvl_comm_serve ();		/* Perform communications service 	*/
#endif
#if defined(_WIN32)
    Sleep (1);
#endif
    miUsrKbService ();

    if (miuAuto == SD_TRUE)
      miuAutoService ();

    mi_service ();
    }

  miuTerminate ();
  dyn_mem_ptr_status ();	/* Log memory allocation usage		*/
  printf ("\n\n");
  }

/************************************************************************/
/************************************************************************/
/* INITIALIZATION FUNCTIONS 						*/
/************************************************************************/
/*				miuInitMem				*/
/************************************************************************/

static ST_VOID mem_chk_error_detected (ST_VOID);
static ST_VOID *my_malloc_err (ST_UINT size);
static ST_VOID *my_calloc_err (ST_UINT num, ST_UINT size);
static ST_VOID *my_realloc_err (ST_VOID *old, ST_UINT size);

static ST_CHAR *spareMem;

static ST_VOID miuInitMem ()
  {
#if defined(NO_GLB_VAR_INIT)
  mvl_init_glb_vars ();
#endif

/* Allocate spare memory to allow logging/printing memory errors	*/
  spareMem = (ST_CHAR *) malloc (500);

/* trap mem_chk errors							*/
  mem_chk_err = mem_chk_error_detected;
  m_memerr_fun = my_malloc_err;
  c_memerr_fun = my_calloc_err;
  r_memerr_fun = my_realloc_err;
  m_mem_debug = SD_TRUE;
  }

/************************************************************************/
/*			miuSetIccpMode 					*/
/************************************************************************/

static ST_VOID miuSetIccpMode (int argc, char *argv[])
  {
ST_INT i;

  miuMode = MI_MODE_BOTH;
  for (i = 0; i < argc; ++i)
    {
    if (strcmp (argv[i], "-s") == 0)
      miuMode = MI_MODE_SERVER;
    else if (strcmp (argv[i], "-c") == 0)
      miuMode = MI_MODE_CLIENT;
    }
  }

/************************************************************************/
/*			miuInitLogCfg					*/
/************************************************************************/

static ST_VOID miuInitLogCfg ()
  {
#ifdef DEBUG_SISCO

  if (logcfgx ("logcfg.xml"))
    {
    printf ("\nParsing of 'logcfg.xml' failed.");
    if (sLogCtrl->fc.fileName)
      printf ("\nCheck log file '%s'.", sLogCtrl->fc.fileName);
    exit (5);
    }

  ml_mlog_install ();

#endif  /* DEBUG_SISCO */
  }

/************************************************************************/
/*			miuInitIccp 					*/
/************************************************************************/

ST_VOID miuInitIccp ()
  {
MVL_CFG_INFO mvlCfgInfo;
ST_RET ret;

  mi_initialize ();	/* Start the MI (ICCP) subsystem		*/

/* Using ICFG with MI_ICFG */
  ret = mi_icfg_load ("mics_usr.xml", MI_ICFG_MODE_INITIAL, NULL, NULL);
  if (ret != SD_SUCCESS)
    {
    printf ("\nICCP configuration using ICFG failed");
    exit (2);
    }

#if !defined(MAP30_ACSE)
/* Read osicfg.xml, for DIB and stack configuration */
  ret = osicfgx ("osicfg.xml", &mvlCfgInfo); 
  if (ret != SD_SUCCESS)
    {
    printf ("\nosicfgx failed, err = %d. Check configuration.\n", ret );
    exit (1);
    }
#endif

/* MI_ICFG has loaded the ICCP configuration, which contains some    	*/
/* information required to correctly set up MMS-EASE Lite.		*/

/* Get the ICCP configuration based parameters */
  mi_find_mvl_cfg_info (&mvlCfgInfo);

/* OK, finally we are ready to start */
  ret = mvl_start_acse (&mvlCfgInfo);
  if (ret != SD_SUCCESS)
    {
    printf ("\nCould not start MVL");
    exit (4);
    }

/* Enable all associations */
  mi_enable_remote_assoc (NULL);
  }

/************************************************************************/
/************************************************************************/
/* TERMINATION FUNCTIONS 						*/
/************************************************************************/
/*			miuTerminate 					*/
/************************************************************************/

ST_VOID miuTerminate (ST_VOID)
  {
MI_REMOTE *mi_remote;
MI_ASSOC_CTRL *mi_assoc;


/* Terminate all active connections */
  mi_disable_remote_assoc (NULL);

/* Now wait for all to go idle */
  miuDoIt = SD_TRUE;
  while (miuDoIt)
    {
    mi_remote = mi_remote_list;
    while (mi_remote)
      {
      mi_assoc = mi_remote->mi_assoc_list;
      while (mi_assoc)
        {
        if (mi_assoc->state != MI_ASSOC_STATE_IDLE)
          break;	/* Found one not idle */

        mi_assoc = (MI_ASSOC_CTRL *) list_get_next (mi_remote->mi_assoc_list, mi_assoc);
        }
      if (mi_assoc != NULL)
        break;

      mi_remote = (MI_REMOTE *) list_get_next (mi_remote_list, mi_remote);
      }
    if (mi_remote == NULL)
      break;

#if defined (MOSI)		/* If stack is MOSI, use events		*/
    wait_any_event (100L);	/* Wait 1000 milliseconds		*/
    while (mvl_comm_serve ())	/* Perform communications service 	*/
      { 			/* Loop until "mvl_comm_serve" returns 0*/
      }
#else
    mvl_comm_serve ();
#endif

    mi_service ();
    miUsrKbService ();
    }

  mvl_end_acse ();		/* Stop the lower layer subsystem	*/
  mi_terminate ();

/* Free memory allocated by MI_ICFG */
  mi_icfg_free_local_cc ();
  mi_icfg_free_remote_cc (NULL);

/* Free all associations */
  mi_free_remote_assoc (NULL);

/* Now free the MI objects that were allocated at configuration time	*/
  mi_free_remote (NULL);	/* Free all remotes 	*/
  mis_free_dv_tables ();	/* Free local DV's	*/
  mis_free_device_table ();	/* Free local DEV's	*/
  }

/************************************************************************/
/************************************************************************/
/* MI CONNECTION ACTIVITY 						*/
/************************************************************************/
/************************************************************************/
/* 		u_mi_association_active 				*/
/************************************************************************/

ST_VOID u_mi_association_active (MI_ASSOC_CTRL *mi_assoc)
  {
  printf ("\nMI Association Active: ");
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING)
    printf ("Calling");
  else
    printf ("Called");

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT)
    {
    printf (", Client");
    miuClientRemoteAssoc = mi_assoc;

    miuMiConn = mi_assoc->mi_conn;
    micuStartAssocActivity (mi_assoc->mi_conn);
    }
  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_SERVER)
    printf (", Server");

  }

/************************************************************************/
/* 		u_mi_association_inactive				*/
/************************************************************************/

ST_VOID u_mi_association_inactive(MI_ASSOC_CTRL *mi_assoc, ST_INT reason)
  {
  printf ("\nMI Association Inactive: ");
  if (reason == MI_ASSOC_INACTIVE_ABORT_IND)
    printf ("Abort Indication");
  if (reason == MI_ASSOC_INACTIVE_ABORT_REQ)
    printf ("Abort Confirm");
  if (reason == MI_ASSOC_INACTIVE_TERMINATE_IND)
    printf ("Terminate Indication");
  if (reason == MI_ASSOC_INACTIVE_TERMINATE_CONF)
    printf ("Terminate Confirm");
  if (reason == MI_ASSOC_FAILED_ASSOC_REQ)
    printf ("Associate Request Failed");

/* If we are calling, go ahead and re-enable the connection */
  if (micuAssocEnable && (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CALLING))
    mi_enable_assoc (mi_assoc);

  if (mi_assoc->ctrl_flags & MI_ASSOC_CTRL_CLIENT)
    miuMiConn = NULL;

  }

/************************************************************************/
/* 		u_mi_association_ind_err 				*/
/************************************************************************/

ST_VOID u_mi_association_ind_err (ST_INT reason)
  {
  printf ("\nMI Initiate Indication Error detected:");
  if (reason == MI_ASSOC_IND_ERR_NOREMOTE)
    printf ("Could not match remote");
  if (reason == MI_ASSOC_IND_ERR_NOMISVCC)
    printf ("Remote does not have a MIS_VCC");
  if (reason == MI_ASSOC_IND_ERR_NOASSOC)
    printf ("Could not find available MI_ASSOC");
  }

/************************************************************************/
/*			u_mvl_ident_ind 				*/
/************************************************************************/

ST_VOID u_mvl_ident_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  mvl_ind_ctrl->u.ident.resp_info = &identify_response_info;
  mplas_ident_resp (mvl_ind_ctrl);
  }

/************************************************************************/
/************************************************************************/

#define MISU_AUTO_DV_UPDATE_TIME	500.0
#define MISU_AUTO_IM_SEND_TIME	 	10000.0

/************************************************************************/
/*			miuAutoService 					*/
/************************************************************************/

static ST_VOID miuAutoService ()
  {
static ST_DOUBLE lastMisuDvUpdateTime;
static ST_DOUBLE lastMisuImSendTime;
ST_DOUBLE currTime = 0;

  currTime = sGetMsTime ();
  if (miuFullSpeed || (currTime - lastMisuDvUpdateTime > MISU_AUTO_DV_UPDATE_TIME))
    {
    misuChangeDv ();
    lastMisuDvUpdateTime = currTime;;
    }
  if (currTime - lastMisuImSendTime > MISU_AUTO_IM_SEND_TIME)
    {
    misuSendMessage ();
    lastMisuImSendTime = currTime;;
    }
  }			      

/************************************************************************/
/************************************************************************/
/* MISC. FUNCTIONS							*/
/************************************************************************/
/************************************************************************/
/*			miUsrKbService 					*/
/************************************************************************/

ST_RET miUsrKbService ()
  {
ST_CHAR c;
int kbhit(ST_VOID);
ST_CHAR get_a_char(ST_VOID);

  if (kbhit ())		/* Report test keyboard input */
    {
    c = get_a_char() /* getch ()*/ ;


    if (c == 'x')
      {
      miuDoIt = SD_FALSE;
      }
    if (c == 'a')
      {
      miuAuto = miuAuto ? SD_FALSE: SD_TRUE;
      }
    if (c == 'f')
      {
      miuFullSpeed = miuFullSpeed ? SD_FALSE: SD_TRUE;
      }
    if (c == 'c')
      {
      if (miuMode != MI_MODE_CLIENT)
        misuChangeDv ();
      }
    if (c == 'e')
      {
      if (miuMode != MI_MODE_SERVER)
        {
        mi_enable_assoc (miuClientRemoteAssoc);
        micuAssocEnable = SD_TRUE;
        }
      }
    if (c == 'd')
      {
      if (miuMode != MI_MODE_SERVER)
        {
        if (miuClientRemoteAssoc->state == MI_ASSOC_STATE_ACTIVE)
          {
          micuStopAssocActivity (miuClientRemoteAssoc->mi_conn);
          micuAssocEnable = SD_FALSE;
          }
        }
      }
    if (c == 'r')
      {
      if (miuMode != MI_MODE_SERVER && miuClientRemoteAssoc->state == MI_ASSOC_STATE_ACTIVE)
        micuReadAllDv (miuClientRemoteAssoc->mi_conn);
      }
    if (c == 'w')
      {
      if (miuMode != MI_MODE_SERVER && miuClientRemoteAssoc->state == MI_ASSOC_STATE_ACTIVE)
        micuWriteDv (miuClientRemoteAssoc->mi_conn);
      }
    if (c == 'l')
      {
      if (miuMode != MI_MODE_SERVER && miuClientRemoteAssoc != NULL)
        micuLoopEnable = micuLoopEnable ? SD_FALSE : SD_TRUE; 
      }
    if (c == 'm')
      {
      if (miuMode != MI_MODE_SERVER)
	printf ("\nOnly a Server can send a message.");
      else
        misuSendMessage ();
      }
    if (c == 'D')
      {
      if (miuMode != MI_MODE_SERVER)
        micuTestAllDevices (miuMiConn);
      }
    if (c == 'S')
      {
      printf ("\nLogging status ...");
      mi_icfg_log_status ();
      }

    if (c == 'C')
      mi_icfg_log_cfg ();

    if (c == '?')
      {
      printf ("\n\n Options:");
      printf ("\n   S = Log Status");
      printf ("\n   C = Log Configuration");
      printf ("\n   a = Auto mode toggle");
      printf ("\n   f = Full speed toggle");
      printf ("\n   c = Change DV values           (server option)");
      printf ("\n   m = send a message             (server option)");
      printf ("\n   e = Enable client association  (client option)");
      printf ("\n   d = Disable client association (client option)");
      printf ("\n   r = Read all DV                (client option)");
      printf ("\n   w = Write DV                   (client option)");
      printf ("\n   l = Loop Test enable           (client option)");
      printf ("\n   D = device test                (client option)");
      }

    while (kbhit ())	/* Flush keyboard */
      get_a_char() /* getch () */ ;
    }
  return (SD_TRUE);
  }

/************************************************************************/
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
    printf ("\nMemory Error Detected! Check log file");
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
/************************************************************************/

ST_VOID u_mvl_getvar_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  _mplas_err_resp (mvl_ind_ctrl,4,0);	/* class service, code other */
  }
ST_VOID u_mvl_namelist_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  _mplas_err_resp (mvl_ind_ctrl,4,0);	/* class service, code other */
  }
ST_VOID u_mvl_write_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  _mplas_err_resp (mvl_ind_ctrl,4,0);	/* class service, code other */
  }
ST_VOID u_mvl_read_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  _mplas_err_resp (mvl_ind_ctrl,4,0);	/* class service, code other */
  }
ST_VOID u_mvl_defvlist_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  _mplas_err_resp (mvl_ind_ctrl,4,0);	/* class service, code other */
  }
ST_VOID u_mvl_delvlist_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  _mplas_err_resp (mvl_ind_ctrl,4,0);	/* class service, code other */
  }
ST_VOID u_mvl_getvlist_ind (MVL_IND_PEND *mvl_ind_ctrl)
  {
  _mplas_err_resp (mvl_ind_ctrl,4,0);	/* class service, code other */
  }
ST_VOID u_mvl_info_rpt_ind (MVL_COMM_EVENT *event)
  {
  _mvl_free_comm_event (event);
  }

