/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : mit_main.c                                            	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Main and menu handling for the MITEST application		*/
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 07/22/02  ask    32	   added wait_any_event if !MOSI, now use one 	*/
/*			   menu_service					*/
/* 07/16/02  rkr    31     addeddoModifyAddressMatching			*/
/* 07/15/02  rkr    30     added MLOG                                   */
/* 07/09/02  ask    29     add menu_service fun when MOSI is undefined	*/
/* 07/03/02  rkr    28     removed mi matching, more main loop work     */
/* 07/02/02  rkr    27     changed timing of the main loop              */
/* 06/28/02  ask    26     remove icfg_debug_sel, caused warning in QNX	*/
/* 06/26/02  rkr    25     defaulted NERR logging on                    */
/* 06/26/02  ask    24     Cleanup for QNX				*/
/* 06/20/02  rkr    23     added mi matching                            */
/* 06/12/02  ask    22     Increased copyright date			*/
/* 02/19/02  rkr    21     QNX changes					*/
/* 02/13/02  ask    20     Changes for mmslite init                     */ 
/* 01/16/02  ask    19     Ported to Marben stack			*/
/* 01/14/02  ask    18     Removed references to mit_cfg.c		*/
/* 11/07/01  ask    17     Removed icfg log macro prototypes		*/
/* 10/15/01  ask    16     Updated for new ICFG tools, new phone #      */
/* 06/28/01  rkr    15     Defaulted mit_debug_sel = MITLOG_ERR		*/
/* 06/25/01  rkr    14     Deleted prototypes for term_init, term_rest	*/
/* 06/13/01  rkr    13     Changes for new DataValue test mechanism	*/
/* 06/07/01  ask    12     Changed menu title 'Read Remote Data Values' */
/*                         to 'Set Remote Data Values'                  */
/* 04/17/01  ask    11	   Support for XML file mitest.xml in main()	*/
/* 04/16/01  ask    10	   Changed menu titles to match names of 	*/
/*			   operations in 870-6-503			*/
/* 04/12/01  ask    9	   fixed menu title in locVarMenu function	*/ 
/* 04/12/01  ask    8	   Added createRemoteDataSet in 'Remote		*/
/*			   Variables' menu				*/
/* 04/12/01  ask    7      Added deleteRemoteDataSet in 'Remote	    	*/ 
/*			   Variables' menu				*/
/* 04/12/01  ask    6	   Added getRemoteDataSetElementNames in 'Remote*/
/*			   Variables' menu				*/
/* 04/12/01  ask    5      Added getRemoteDataValueNames in 'Remote	*/ 
/*			   Variables' menu				*/
/* 04/06/01  ask    4	   Support for XML OSI config in init_acse()	*/
/* 04/06/01  ask    3      Support for XML logging in init_log_cfg()	*/
/* 12/15/00  rkr    2      added _mitlog_always_logstr 			*/
/* 03/17/99  rkr    1      Created                                      */
/************************************************************************/

#include "mit_main.h"

#include "scrndefs.h"
#include "gvaldefs.h"
#include "fkeydefs.h"
#include "suicacse.h"
#include "mi_log.h"
#include "mloguser.h"
#include "icfg.h"

#if defined(_AIX) || defined(__alpha)
#if defined(__alpha)
#include <sys/time.h>
#endif
#include <sys/select.h>
#include <errno.h>
#endif

#if	(SYSTEM_SEL & (SYSTEM_SEL_SYS_QNX4))
#include <termio.h>
#include <signal.h>
static struct  termio *save_state; /* struct to save terminal info */
void sigfun(int sig);
void sigbus(int sig);
struct	termio	tc_block; 
#endif

#if !defined(MOSI)
#include <process.h> /* need for getpid() proto */
extern ST_CHAR *MMSEventName; /* Pointer to the globally unique name of Event Object */
#endif

#if defined (_WIN32)
#include <windows.h>
extern HANDLE kbdEvent;		
#endif

#if (SYSTEM_SEL & (SYSTEM_SEL_SYS_QNX4))
ST_VOID	term_init();
ST_VOID	term_rest();
#endif

/* #if defined (_WIN32) */
void 	ShutDown( int p1 );
/* #endif */

static char *thisFileName = __FILE__;

#define MIT_XML_OSI_CFG_FILE	"osicfg.xml"
#define MIT_XML_MIT_CFG_FILE	"mitest.xml"
#define MIT_XML_MIT_LOG_FILE	"logcfg.xml"

/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/
ST_DOUBLE beginTime, endTime;		/* for elapsed time	*/
ST_INT compSleepTime;
MVL_CFG_INFO mvlCfg;
time_t compStartTime, compStopTime;
ST_UINT mit_debug_sel;
ST_CHAR *_mitlog_err_logstr = "MIT_ERROR";
ST_CHAR *_MIT_LOG_NERR_logstr = "MIT_NERR";
ST_CHAR *_mitlog_flow_logstr = "MIT_FLOW";
ST_CHAR *_mitlog_cfg_logstr = "MIT_CONFIG";
ST_CHAR *_mitlog_ind_logstr = "MIT_INDICATIONS";
ST_CHAR *_mitlog_cnf_logstr = "MIT_CONFIRMS";
ST_CHAR *_mitlog_always_logstr = "MIT_ALWAYS";
							  
/************************************************************************/
/*      Static Function Declarations                           		*/
/************************************************************************/

static ST_VOID setMainMenu (ST_VOID);
static ST_VOID do_assocMgtMenu (ST_VOID);
static ST_VOID do_remVarMenu (ST_VOID);
static ST_VOID do_locVarMenu (ST_VOID);
static ST_VOID do_configMenu (ST_VOID);
static ST_VOID do_dspCfgMenu (ST_VOID);
static ST_VOID do_utilMenu (ST_VOID);
static ST_VOID do_tranSetMenu (ST_VOID);
static ST_VOID do_infoMsgMenu (ST_VOID);
static ST_VOID do_devCtrlMenu (ST_VOID);
static ST_VOID assocMgtMenu (ST_VOID);
static ST_VOID remVarMenu (ST_VOID);
static ST_VOID locVarMenu (ST_VOID);
static ST_VOID infoMsgMenu (ST_VOID);
static ST_VOID devCtrlMenu (ST_VOID);
static ST_VOID cfgMenu (ST_VOID);
static ST_VOID utilMenu (ST_VOID);
static ST_VOID testNoFun (ST_VOID);
static ST_VOID exitTestApp (ST_VOID);
static ST_VOID switchMenu(ST_VOID);
static ST_VOID testSetMain (ST_VOID);
static ST_VOID tranSetMenu (ST_VOID);
	       
#if !defined(MOSI)
ST_VOID init_event_handling (ST_VOID);
ST_VOID wait_any_event (ST_LONG max_ms_delay);
#endif

ST_VOID (*menu_set_fun)(ST_VOID) = testNoFun; 
static ST_VOID init_mem (ST_VOID);
static ST_VOID init_log_cfg (ST_VOID);
static ST_VOID init_acse (ST_VOID);
static ST_RET init_local_ar_name (ST_VOID);
ST_RET read_log_cfg_file (ST_CHAR *filename);
ST_VOID menu_service  (ST_VOID);
ST_VOID wait_any_event (ST_LONG max_ms_delay);
ST_VOID mitUnconfigure_xml (ST_VOID);
/************************************************************************/
/************************************************************************/
/* General program control variables */

static ST_BOOLEAN doIt = SD_TRUE;

/************************************************************************/
/* main:  entry point							*/
/************************************************************************/
 
MMS_OBJ_ID asn1_obj_id;
MMS_OBJ_ID acse_obj_id;
MMS_OBJ_ID mms_obj_id;

void main ( int argc, char *argv[] )
  {
MIT_ASSOC_INFO *pstMitAssoc;

/*  m_mem_debug = SD_TRUE;						*/
/* First do any system specific initialization, for screen, etc.	*/
#if	(SYSTEM_SEL & (SYSTEM_SEL_SYS_QNX4))
  save_state = &tc_block;
  term_init(save_state);
  set_echo_off();
#endif

/* #if defined(_WIN32) */
  term_init();
  signal(SIGINT, ShutDown);
  signal(SIGTERM,ShutDown);
/* #endif */

  mit_debug_sel = MITLOG_ERR | MIT_LOG_NERR;
  icfg_debug_sel = ICFG_LOG_ERR;
  m_mem_debug = SD_TRUE;
  m_fill_en = SD_TRUE;
/* m_heap_check_enable= SD_TRUE; */
/* m_check_list_enable= SD_TRUE; */

  init_mem ();		/* Set up memory allocation tools 		*/
  init_log_cfg ();	/* Set up logging subsystem			*/

  /* Show log file entires on the screen				*/
  
  _slog_dyn_log_fun = mit_app_dyn_log_fun;

  printf("\n");
  /*get_ldebug ((ST_ULONG *)&mit_debug_sel,MIT_LOG_NERR,"MIT Normal Errors");*/
  get_ldebug ((ST_ULONG *)&mit_debug_sel,MITLOG_FLOW,"MIT Flow");

  MIT_LOG_FLOW0 ("Initializing ...");
  printf ("\n Reading in configuration file ...");
  
  /* read the config file	*/
  if(mitConfigure_xml (MIT_XML_MIT_CFG_FILE, &mvlCfg) != SD_SUCCESS)
    {    
    MIT_LOG_ERR0("Configuration File Errors - check log file.");
    printf ("\nConfiguration File Errors - check log file.");
    exit(1);
    }

  mitConfigurePtChgTbl();

  printf ("\n Initalizing local name");

  if (init_local_ar_name() != SD_SUCCESS)
    {
    printf(": No Local AR Name");
    exit(2);
    }

  printf ("\n Initalizing ACSE");
  init_acse ();		/* Start the lower layer subsystem		*/
  printf ("\n Initalizing MI");
#if 0
  /* set up the handler for Clients connecting without AE titles	*/
  u_mvl_connect_ind_fun = u_mvl_connect_ind;
#endif

  mi_initialize ();
  
  if (ask ("\n Start all Associations (y:TRUE, n:FALSE) (default = TRUE) : ",1))
    {
    pstMitAssoc = pstAssocInfoSet;
    while (pstMitAssoc)
      {
      printf( "\n Starting Association: '%s'", pstMitAssoc->associationName);
      startAssociation (pstMitAssoc);
      pstMitAssoc = (MIT_ASSOC_INFO *) list_get_next
	       ((ST_VOID *)pstAssocInfoSet, (ST_VOID *)pstMitAssoc);
      }
    }

  MIT_LOG_FLOW0 ("\n Initialization complete");
  setMainMenu();
/* We are all initialized, just service communications			*/

#if !defined(MOSI)
  init_event_handling();
#endif
  
  chk_malloc (1);	/* marker */
  
  while (doIt)
    {
    menu_service ();	/* Service keyboard/MMS until exit from menu	*/

    CLEARSCR;
    if (ask ("\n\n\n Really want to exit ? ",0))
      doIt = 0;
    else
      {
      if (ask ("\n Modify Print Levels ? ",0))
        do_debugset ();
      else
        setMainMenu();
      }
    }

  mi_terminate ();
  mvl_end_acse ();		/* Stop the lower layer subsystem	*/
  printf ("\n Deallocating demo rescources");
  
  mitUnconfigure_xml();	/* deallocate all the configuration */
  
  dyn_mem_ptr_status ();	/* Log memory allocation usage		*/
  printf ("\n\n");

  } /* end main */

/************************************************************************/
/*                        menu_service					*/
/************************************************************************/
/* There needs to be some limit on the number of time we service the    */
/* lower layers or it is possible to be get so much incoming traffic    */
/* that we spend large segments of time servicing the lower layers,     */
/* other interesting things in the application don't get proper service */
/* time. Todays limit is 10 calls max                                   */

#define MAX_SERVICE_CALLS	10
ST_VOID menu_service (ST_VOID)
  {
ST_INT serveCount;
//ST_INT i;
ST_LONG	msToWait;

  while (check_key () != 1)	/* Execute function keys until F10 hit	*/
    {
     		            /* If stack is MOSI, use events		*/     
    if (mi_wait_service_ms==0)
      msToWait=500; /* 1/2 a second to give the processor a rest */
    else
      msToWait = mi_wait_service_ms;
    wait_any_event (msToWait); 	/* Wait a littlt bit	*/
    serveCount = 0;
    while (mvl_comm_serve () && (serveCount < MAX_SERVICE_CALLS))	
      {				/* Loop until "mvl_comm_serve" returns 0*/
      serveCount++;
      }
#if 0
    i = tp4_buffers_avail(0);
    if (serveCount>0) printf (" (%d - %d,%d)",msToWait, serveCount, i);
#endif
    mi_service ();
    updatePtChgTbl ();
    }
  }		      

/************************************************************************/
/*				init_mem				*/
/************************************************************************/

static ST_VOID mem_chk_error_detected (ST_VOID);
static ST_VOID *my_malloc_err (ST_UINT size);
static ST_VOID *my_calloc_err (ST_UINT num, ST_UINT size);
static ST_VOID *my_realloc_err (ST_VOID *old, ST_UINT size);

static ST_CHAR *spareMem;

static ST_VOID init_mem (ST_VOID)
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

#if 0
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

/************************************************************************/
/* 			showHeading					*/
/************************************************************************/
static ST_VOID showHeading (ST_VOID)
  {
  printf("\n\n\n");
  printf ("************************************************************\n");
  printf ("***      MMS-EASE Lite ICCP TASE.2 Test App V%-6s      ***\n", ICCP_RELEASE);
  printf ("***              (c) 1996-2002 SISCO, Inc.               ***\n");
  printf ("***                   (586) 254-0020                     ***\n");
  printf ("************************************************************");
  }

/************************************************************************/
/*				testNoFun				*/
/* They pressed the wrong function key - do something friendly!		*/
/************************************************************************/

static ST_VOID testNoFun(ST_VOID)
  {
  printf("\n\nInvalid Function Key Selected!!");
  pause_msg("\nPress a key to continue!");
  (*menu_set_fun)();
  }

/************************************************************************/
/*				setMainMenu				*/
/************************************************************************/

static ST_VOID setMainMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  ICCP Toolkit Test Application:  Main Menu \n\n");
  printf ("\t  1  - Association Management\n");
  printf ("\t  2  - Remote Variables\n");
  printf ("\t  3  - Local Variables\n");
  printf ("\t  4  - Transfer Sets\n");
  printf ("\t  5  - Information Messages\n");
  printf ("\t  6  - Device Control\n");
  printf ("\t  7  - Display Configuration\n");
  printf ("\t  8  - Miscellaneous\n");
  printf ("\t  0  - Exit\n");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();

  funct_1 = do_assocMgtMenu;
  funct_2 = do_remVarMenu;
  funct_3 = do_locVarMenu;
  funct_4 = do_tranSetMenu;
  funct_5 = do_infoMsgMenu;
  funct_6 = do_devCtrlMenu;
  funct_7 = do_dspCfgMenu;
  funct_8 = do_utilMenu;
  funct_9 = testNoFun;

  menu_set_fun = setMainMenu;	/* used to reset the menu	*/
  }

/************************************************************************/
/*			do_assocMgtMenu					*/
/* This functions executes the context management menu			*/
/************************************************************************/

static ST_VOID do_assocMgtMenu (ST_VOID)
  {
  assocMgtMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/*		     	assocMgtMenu					*/
/* set up the association management menu				*/
/************************************************************************/

static ST_VOID assocMgtMenu (ST_VOID)
  {
  CLEARSCR;
  showHeading();

  printf ("\n\n");
  printf ("\t  Association Management Menu \n\n");
  printf ("\t  1  - Start Association(s)\n");
  printf ("\t  2  - Stop Associations(s)\n");
  printf ("\t  3  - Show Association Summary\n");
  printf ("\t  0  - Main Menu\n");
  printf ("\t  \n");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();

  funct_1 = startAssociations;
  funct_2 = stopAssociations;
  funct_3 = showAssocSummary;
  funct_4 = testNoFun;
  funct_5 = testNoFun;
  funct_6 = testNoFun;
  funct_7 = testNoFun;
  funct_8 = testNoFun;
  funct_9 = testNoFun;

  menu_set_fun = assocMgtMenu;	/* used to reset the menu	*/
  }

/************************************************************************/
/*				do_remVarMenu				*/
/************************************************************************/

static ST_VOID do_remVarMenu (ST_VOID)
  {
  remVarMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/*				remVarMenu					*/
/************************************************************************/

static ST_VOID remVarMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  Remote Variables Menu \n\n");
  printf ("\t  1  - Get Data Values\n");
  printf ("\t  2  - Set Data Values\n");
  printf ("\t  3  - Get Data Value Names\n");
  printf ("\t  4  - Get Data Value Type\n");
  printf ("\t  5  - Get Data Set Element Values\n");
  printf ("\t  6  - Set Data Set Element Values\n");
  printf ("\t  7  - Create Data Set\n");
  printf ("\t  8  - Delete Data Set\n");
  printf ("\t  9  - Get Data Set Element Names\n");
  printf ("\t  0  - Main Menu\n");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();

  funct_1 = readRemoteDataValues;
  funct_2 = writeRemoteDataValues;
  funct_3 = getRemoteDataValueNames;
  funct_4 = getRemoteDataValueAttributes;
  funct_5 =	readRemoteDataSet;
  funct_6 = writeRemoteDataSet;
  funct_7 = createRemoteDataSet;
  funct_8 = deleteRemoteDataSet;
  funct_9 = getRemoteDataSetElementNames;
   

  menu_set_fun = remVarMenu;
  }

/************************************************************************/
/*				do_locVarMenu				*/
/************************************************************************/

static ST_VOID do_locVarMenu (ST_VOID)
  {
  locVarMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/*				locVarMenu				*/
/************************************************************************/

static ST_VOID locVarMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  Local Variables Menu \n\n");
  printf ("\t  1  - Display local DataValue\n");
  printf ("\t  2  - Modify local DataValue\n");
  printf ("\t  3  - Modify local DataValue Change Condition\n");
  printf ("\t  4  - Display local DataValue Names\n");
  printf ("\t  5  - Display local DataSet Names\n");
  printf ("\t  0  - Main Menu\n");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();

  funct_1 = displayLocalDataValue;
  funct_2 = modifyLocalDatavalue;
  funct_3 = doReconfigurePtChgTbl;
  funct_4 = displayLocalDataValueNames;
  funct_5 = logLocalDataSetNames;
  funct_6 = testNoFun;
  funct_7 = testNoFun;
  funct_8 = testNoFun;
  funct_9 = testNoFun;

  menu_set_fun = locVarMenu;
  }

/************************************************************************/
/*				do_tranSetMenu				*/
/************************************************************************/

static ST_VOID do_tranSetMenu (ST_VOID)
  {
  tranSetMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/*				tranSetMenu				*/
/* set up the transfer set menu						*/
/************************************************************************/

static ST_VOID tranSetMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  Transfer Sets Menu \n\n");
  printf ("\t  1 - Start Transfer\n");
  printf ("\t  2 - Stop Transfer\n");
  printf ("\t  0 - Main Menu\n");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();


  funct_1= startTransferSets;
  funct_2= stopTransferSets;
  funct_3= testNoFun;
  funct_4= testNoFun;
  funct_5= testNoFun;
  funct_6= testNoFun;
  funct_7= testNoFun;
  funct_8= testNoFun;
  funct_9= testNoFun;

  menu_set_fun = tranSetMenu;
  }

/************************************************************************/
/*			      do_infoMsgMenu				*/
/************************************************************************/

static ST_VOID do_infoMsgMenu (ST_VOID)
  {
  infoMsgMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/*			      infoMsgMenu				*/
/* set up the Information Message menu					*/
/************************************************************************/

static ST_VOID infoMsgMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  Information Message Menu \n\n");
  printf ("\t  1 - Start Information Messages (Client Only)\n");
  printf ("\t  2 - Stop Information Messages (Client Only)\n");
  printf ("\t  3 - Send Message (Server Only)\n");
  printf ("\t  0 - Main Menu\n");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();


  funct_1= startInformationMessages;
  funct_2= stopInformationMessages;
  funct_3= sendInformationMessage;
  funct_4= testNoFun;
  funct_5= testNoFun;
  funct_6= testNoFun;
  funct_7= testNoFun;
  funct_8= testNoFun;
  funct_9= testNoFun;

  menu_set_fun = infoMsgMenu;
  }

/************************************************************************/
/*			      do_devCtrlMenu				*/
/************************************************************************/

static ST_VOID do_devCtrlMenu (ST_VOID)
  {
  devCtrlMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/*			      devCtrlMenu				*/
/* set up the Device Control menu					*/
/************************************************************************/

static ST_VOID devCtrlMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  Device Control Menu \n\n");
  printf ("\t  1 - Select Operation\n");
  printf ("\t  2 - Operate Operation\n");
  printf ("\t  3 - Set Tag\n");
  printf ("\t  4 - Get Tag\n");
  printf ("\t  5 - Display local Devices\n");
  printf ("\t  0 - Main Menu\n");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();


  funct_1= selectDevice; 
  funct_2= operateDevice;
  funct_3= setDeviceTag;
  funct_4= getDeviceTag; 
  funct_5= displayLocalDevices;
  funct_6= testNoFun;
  funct_7= testNoFun;
  funct_8= testNoFun;
  funct_9= testNoFun;

  menu_set_fun = devCtrlMenu;
  }

/************************************************************************/
/*			      do_dspCfgMenu				*/
/************************************************************************/

static ST_VOID do_dspCfgMenu (ST_VOID)
  {
  cfgMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/* 			      	cfgMenu					*/
/************************************************************************/

static ST_VOID cfgMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  Display Configuration Menu \n\n");
  printf ("\t  1  - Show Configured Remotes\n");
  printf ("\t  2  - Show Remote DataValue Names\n");
  printf ("\t  3  - Show Remote DataSet Names\n");
  printf ("\t  4  - Show Remote DataSet Attributes\n");
  printf ("\t  5  - Show Remote TransferSet Attributes\n");
  printf ("\t  6  - Show Remote Info Messages\n");
  printf ("\t  7  - Show Remote Devices\n");
  printf ("\t  0  - Main Menu\n");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();

  funct_1 = displayConfiguredRemotes;
  funct_2 = displayRemoteDataValueNames;
  funct_3 = displayRemoteDataSetNames;
  funct_4 = displayRemoteDataSetAttributes;
  funct_5 = displayRemoteTransferSetAttributes;
  funct_6 = displayRemoteInfoMsgs;
  funct_7 = displayRemoteDevices;
  funct_8 = testNoFun;
  funct_9 = testNoFun;

  menu_set_fun = cfgMenu;
  }

/************************************************************************/
/*				do_utilMenu				*/
/************************************************************************/

static ST_VOID do_utilMenu (ST_VOID)
  {
  utilMenu();
  menu_service ();
  setMainMenu ();
  }

/************************************************************************/
/*				utilMenu				*/
/* set up the miscellaneous utilities menu				*/
/************************************************************************/

static ST_VOID utilMenu (ST_VOID)
  {
  CLEARSCR;	
  showHeading();
  printf ("\n\n");
  printf ("\t  Miscellaneous Utilities Menu \n\n");
  printf ("\t  1 -  Check Allocated Memory Pointers \n"); 
  printf ("\t  2  - Show ICCP Statistics\n");
  printf ("\t  3  - Set ICCP-LITE Debug Levels\n");
  printf ("\t  4  - Enable Log Messages to Screen\n");
  printf ("\t  5  - Disable Log Messages to Screen\n");
  printf ("\t  6  - Show Sizes\n");
  printf ("\t  7  - Reconfigure DataValue Changes\n");
  printf ("\t  8  - Modify Address Matching\n");
  printf ("\t  0  - Main Menu\n");
  printf ("\t\n  ");
  printf ("\t\n  ");
  printf("\n\n\n\n");

  flush_keys ();
  fun_null ();

  funct_1 = do_dyn_mem_chk;
  funct_2 = showMITStatistics;
  funct_3 = do_debugset;
  funct_4 = scrnLogOn;
  funct_5 = scrnLogOff;
  funct_6 = showSizes;
  funct_7 = doReconfigureChgMode;
  funct_8 = doModifyAddressMatching;
  funct_9 = testNoFun;

  menu_set_fun = utilMenu;
  }

/* #ifdef _WIN32 */
/************************************************************************/
/* ShutDown:  caught a signal - close it all up				*/
/************************************************************************/

void ShutDown( int p1 ) 
  { 
  printf("\n[[[[ Shutdown! ]]]]\n");
   term_rest();
  exit(1);
  }
/* #endif */

/************************************************************************/
/*			init_log_cfg					*/
/************************************************************************/

static ST_VOID init_log_cfg (ST_VOID)
  {
#ifdef DEBUG_SISCO

  if (logcfgx (MIT_XML_MIT_LOG_FILE) != SD_SUCCESS) /* Use new XML file	*/
    {
    printf ("\n Parsing of 'logcfg.xml' configuration file failed");

    if (sLogCtrl->fc.fileName)
      printf ("\n Check log file '%s'.", sLogCtrl->fc.fileName);
    
    exit (5);
    }

  ml_mlog_install();

#endif  /* DEBUG_SISCO */
  }

/************************************************************************/
/*			       init_acse				*/
/************************************************************************/

static ST_VOID init_acse ()
  {
ST_RET ret;

/* Added for Marben Stack */
#if !defined(MAP30_ACSE)
  ret = osicfgx (MIT_XML_OSI_CFG_FILE, &mvlCfg);	/* Use new XML file */
  
  if (ret != SD_SUCCESS)
    {
    printf ("\nosicfgx failed, err = %d. Check configuration.\n", ret );
    exit (1);
    }
#endif 

  ret = mvl_start_acse (&mvlCfg);
  if (ret != SD_SUCCESS)
    exit (1);
  }

/************************************************************************/
/*			    init_local_ar_name				*/
/************************************************************************/

static ST_RET init_local_ar_name (ST_VOID)
  {
ST_RET 		ret = SD_FAILURE;
MIT_MIS_VCC	*pstCurMisVcc;

/* Take the first local name we can find on an MIS VCC and use		*/
/* it for the MVL local AR Name.					*/

/* hopefully we have an MisVcc */

  pstCurMisVcc = pstMisVccSet;
  if (pstCurMisVcc)
    {
    if (pstCurMisVcc->localArName[0])
      {
      strcpy(mvlCfg.local_ar_name, pstCurMisVcc->localArName);
      ret = SD_SUCCESS;
      }
    list_get_next (pstMisVccSet, pstCurMisVcc);
    }
  return (ret);
  }

#if defined (_WIN32) && !defined(MOSI)

/************************************************************************/
/*			init_event_handling				*/
/************************************************************************/
ST_VOID init_event_handling (ST_VOID)
  {
static char EventName[32];

/* For NT and WIN95, we use the SUIC and Keyboard event semaphores in the */
/* 'wait for event' function						*/

/* First create the SUIC event 						*/
  sprintf (EventName, "MmsEvent%d", getpid ());
  MMSEventName = EventName;
  hMMSEvent = CreateEvent (NULL, SD_FALSE, SD_FALSE, MMSEventName);
  if (hMMSEvent == NULL)
    {
    printf ("\n CreateEvent failed, rc = %d", GetLastError ());
    wait_msg ("");
    }
  }

/************************************************************************/
/*                          wait_any_event				*/
/************************************************************************/

ST_VOID wait_any_event (ST_LONG max_ms_delay)
  {
HANDLE hArray[2];
DWORD rc;
int i;

/* Just do a muxwait on the keyboard and SUIC event semaphores		*/
  hArray[0] = hMMSEvent;
  hArray[1] = kbdEvent;

  rc = WaitForMultipleObjects (2, hArray, SD_FALSE, max_ms_delay);

  if (rc == 0xFFFFFFFF)
    {
    printf ("\n WaitForMultipleObjects () failed, rc = %d", GetLastError ());
    return;
    }

  if (rc == WAIT_TIMEOUT)
    return;		/* nothing going on at the moment */

  i = rc - WAIT_OBJECT_0;

  switch (i)
    {
    case 0:		/* SUIC event, autoreset */
    break;

    case 1:		/* keyboard event, must reset */
      if (ResetEvent (kbdEvent) == SD_FALSE)
        printf ("ERROR: Reset Kbd Event, rc = %d", GetLastError);
    break;

    default:
    break;
    }
  }

/************************************************************************/
#endif		/* End of NT Event handling code			*/
/************************************************************************/
