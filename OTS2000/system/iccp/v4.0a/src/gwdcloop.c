/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,       */
/*          1996-2002, All Rights Reserved                              */
/*                                                                      */
/* MODULE NAME : gwdcloop.c (originally mit_main.c)                   	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Main and menu handling for the ICCP TASE.2 / MITEST application */
/*                                                                      */
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 05/xx/02  hm    101     converted to ICCP TASE.2 application         */
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

#define GWDLOOP_C

#include "cppinc.h"

#include "gwdatastructure.h"

#ifdef HPUX
int prt_write( sQueue *q, int nTyp, int nLvl, int nPrc, int nNum, char *strMsg, void *p0, void *p1, void *p2 );
#endif





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

#if defined (_WIN32)
void 	ShutDown( int p1 );
#endif



static char *thisFileName = __FILE__;

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

static ST_VOID Fct_Nothing( ST_VOID );
ST_VOID (*menu_set_fun)(ST_VOID) = Fct_Nothing;
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



ST_VOID gwtase2_service( ST_VOID );
ST_VOID gwtase2_inittable( ST_VOID );

ST_VOID Fct_StartAssociations( ST_VOID );
ST_VOID Fct_StopAssociations( ST_VOID );
ST_VOID Fct_AbortAssociations( ST_VOID );
ST_VOID Fct_GetAssociationSummary( sGWSigTASE2 *pSigTASE2 );

ST_VOID Fct_ReadRemoteDataValues( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_WriteRemoteDataValues( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteDataValueNames( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteDataValuesAttributes( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_ReadRemoteDataSet( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_WriteRemoteDataSet( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_CreateRemoteDataSet( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_DeleteRemoteDataSet( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteDataSetElementNames( sGWSigTASE2 *pSigTASE2 );
  
ST_VOID Fct_DisplayLocalDataValue( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_ModifyLocalDataValue( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_ReconfigureDataValueChanedCondition( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetLocalDataValueNames( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetLocalDataSetNames( sGWSigTASE2 *pSigTASE2 );
  
ST_VOID Fct_StartTransferSets( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_StopTransferSets( sGWSigTASE2 *pSigTASE2 );
  
ST_VOID Fct_StartInformatonMessages( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_StopInformationMessages( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_SendInformationMessage( sGWSigTASE2 *pSigTASE2 );
  
ST_VOID Fct_SelectDevice( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_OperateDevice( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_SetDeviceTag( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetDeviceTag( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetLocalDevices( sGWSigTASE2 *pSigTASE2 );
  
ST_VOID Fct_GetConfigurationRemotes( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteDateValueNames( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteDataSetNames( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteDataSetAttributes( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteTransferSetAttributes( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteInfoMessages( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetRemoteDevices( sGWSigTASE2 *pSigTASE2 );
  
ST_VOID Fct_DynMemCheck( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetStatistics( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_SetDebugLevels( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_ScreenLogOn( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_ScreenLogOff( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_GetSizes( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_ReconfigureChangeMode( sGWSigTASE2 *pSigTASE2 );
ST_VOID Fct_ModifyAddressMatching( sGWSigTASE2 *pSigTASE2 );





extern sGWProcessCntrl  *pPrcCntrlBas;
extern sGWProcessCntrl  *pPrcCntrl;

extern sQueue           *pQ_DCr;
extern sQueue           *pQ_DCw;
extern sQueue           *pQ_Prt;
extern sQueue            sQDat_DCr;
extern sQueue            sQDat_DCw;
extern sQueue            sQDat_Prt;

extern sSharedMemory    *pM_Data;
extern sSemaphore       *pS_Data;
extern sSharedMemory     sMDat_Data;
extern sSemaphore        sSDat_Data;

extern sGWSigTASE2       sSigTASE2_R;
extern sGWSigTASE2       sSigTASE2_W;

extern sGWProtocolMsg   *pPrtMsg;
extern sGWProtocolMsg    sPrtMsgDat;

extern char              strPrtBuf[256];

extern byte cAck_nDst; /*added for ack schedule information 2003.7.12 zy */

typedef struct sGWAssociation {
  char            strAssocNam[64];
  char            strRemoteNam[64];
  MIT_ASSOC_INFO *pAssocInf;
  MIT_MIS_VCC    *pMitMisVcc;
  int             nSta;
} sGWAssoc;

ST_INT	nAssocNum[4];		//Added for save association number for remotes
sGWAssoc  sAssocDat[4][2];	//Changed to two dimension for 2 association in one remote 2003.7.30 zy
    // sAssocDat[0] - Cascade Dispatch Center
    // sAssocDat[1] - National Dispatch Center
    // sAssocDat[2] - Central China Dispatch Center

int gwdc_timeout_flag;

void Net_Timeout(int Sig_no)
{
    signal( SIGALRM, Net_Timeout);
    gwdc_timeout_flag = 1;
    printf("+++++gwdc: recv time out!!!\n");
    alarm(3);
}


//------------------------------------------------------------------------------
// working loop
//------------------------------------------------------------------------------ 
void gwdcloop( void )

{

  ST_INT          cnt;
  MIT_ASSOC_INFO *pstMitAssoc;

/* m_mem_debug = SD_TRUE; */
/* mit_debug_sel = MITLOG_ERR | MIT_LOG_NERR; */
  mit_debug_sel = 0;
  icfg_debug_sel = ICFG_LOG_ERR;
  m_mem_debug = SD_TRUE;
  m_fill_en = SD_TRUE;
/* m_heap_check_enable= SD_TRUE; */
/* m_check_list_enable= SD_TRUE; */

  init_mem();	   // Set up memory allocation tools
  init_log_cfg();	   // Set up logging subsystem

  // read the config file
  MIT_LOG_FLOW0 ("Initializing ...");
printf ("GWDC - Reading configuration file ...\n");
  if (mitConfigure_xml(MIT_XML_MIT_CFG_FILE, &mvlCfg) != SD_SUCCESS) {    
    MIT_LOG_ERR0("Configuration File Errors - check log file.");
// printf ("GWDC - Configuration File Errors - check log file.\n");
    pPrcCntrl->nWork = 0;
    return;
  }

  mitConfigurePtChgTbl();

// printf("Initalizing local name\n");
  if (init_local_ar_name() != SD_SUCCESS) {
// printf("  : No Local AR Name\n");
    pPrcCntrl->nWork = 0;
    return;
  }

  // Start the lower layer subsystem / initializing acse
  init_acse();

  // initalizing MI
  mi_initialize();
  
  // start all associations
  nAssocNum[0] = 0;
  sAssocDat[0][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  sAssocDat[0][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  nAssocNum[1] = 0;
  sAssocDat[1][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  sAssocDat[1][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  nAssocNum[2] = 0;
  sAssocDat[2][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  sAssocDat[2][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  nAssocNum[3] = 0;
  sAssocDat[3][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  sAssocDat[3][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
  cnt = 0;
  pstMitAssoc = pstAssocInfoSet;
  while (pstMitAssoc) {
/*
    startAssociation (pstMitAssoc);
*/
//Following modified for support 2 association in one remote 2003.7.30 zy.
    if(nAssocNum[cnt] == 0)
    {
      sAssocDat[cnt][0].pAssocInf = pstMitAssoc;
      strcpy(sAssocDat[cnt][0].strAssocNam, pstMitAssoc->associationName);
      strcpy(sAssocDat[cnt][0].strRemoteNam, pstMitAssoc->remoteName);
      sAssocDat[cnt][0].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
      sAssocDat[cnt][0].nSta = pstMitAssoc->pstAssocCtrl->state;

      printf("GWDC - Association %d:1 '%s' (%08x), '%s' (%08x) - %d\n",
             cnt, sAssocDat[cnt][0].strAssocNam, sAssocDat[cnt][0].pAssocInf,
             sAssocDat[cnt][0].strRemoteNam, sAssocDat[cnt][0].pMitMisVcc,
             sAssocDat[cnt][0].nSta);
      nAssocNum[cnt] = 1;
    }
    else
    {
       if( strcmp(sAssocDat[cnt][0].strRemoteNam, pstMitAssoc->remoteName) == 0)
       {
         sAssocDat[cnt][1].pAssocInf = pstMitAssoc;
         strcpy(sAssocDat[cnt][1].strAssocNam, pstMitAssoc->associationName);
         strcpy(sAssocDat[cnt][1].strRemoteNam, pstMitAssoc->remoteName);
         sAssocDat[cnt][1].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
         sAssocDat[cnt][1].nSta = pstMitAssoc->pstAssocCtrl->state;

         printf("GWDC - Association %d:2 '%s' (%08x), '%s' (%08x) - %d\n",
                cnt, sAssocDat[cnt][1].strAssocNam, sAssocDat[cnt][1].pAssocInf,
                sAssocDat[cnt][1].strRemoteNam, sAssocDat[cnt][1].pMitMisVcc,
                sAssocDat[cnt][1].nSta);
         nAssocNum[cnt] = 2;
      }
      else
      {
         ++cnt;
         continue;
      }
   }

    pstMitAssoc = (MIT_ASSOC_INFO *)list_get_next((void *)pstAssocInfoSet, (void *)pstMitAssoc);
  }

  // initialization ready
  MIT_LOG_FLOW0 ("Initialization complete\n");

#if !defined(MOSI)
  init_event_handling();
#endif
  
  chk_malloc(1);    // marker

  cAck_nDst = 0x01; /*added for ack schedule information 2003.7.13 zy */

  gwtase2_service();



  mi_terminate();
  mvl_end_acse();  		// Stop the lower layer subsystem
  mitUnconfigure_xml();	   // deallocate all the configuration
  dyn_mem_ptr_status ();	  // Log memory allocation usage

}



//------------------------------------------------------------------------------
// tase.2 services
//------------------------------------------------------------------------------

/************************************************************************/
/*                        menu_service					*/
/************************************************************************/
/* There needs to be some limit on the number of time we service the    */
/* lower layers or it is possible to be get so much incoming traffic    */
/* that we spend large segments of time servicing the lower layers,     */
/* other interesting things in the application don't get proper service */
/* time. Todays limit is 10 calls max                                   */

#define MAX_SERVICE_CALLS	10

void gwtase2_service( void )

{
  ST_INT          cnt;
  MIT_ASSOC_INFO *pstMitAssoc;

  static unsigned char  bVal = 0x80;
  static double         rVal = 0.0;
  int    fchk_num;
  int    gwdc_start_assoctions;


  ST_LONG  msToWait;
  ST_INT   nCnt;
  ST_INT   nLng;
  ST_INT   nSta;
  ST_INT   serveCount;
  ST_LONG  nTim;
  ST_LONG  nTimSav;

  char     strBuf[256];
  FILE    *pFile;

  ST_INT   nTASE2_Sta[4];
  FILE    *pFil;
  struct tm timbuf;
  struct tm *timptr;
  long tim;

  

  nTimSav = 0;
  fchk_num = 0;
  gwdc_timeout_flag = 0;
  gwdc_start_assoctions = 0;
  signal( SIGALRM, Net_Timeout);


  while (pPrcCntrl->nWork) {
    
    time(&nTim);
    if ((nTim - nTimSav) >= 30) {
      nTimSav = nTim;

      pFil = fopen("/usr/gwaysw/gway/gwtmp/TASE2Status.txt", "w");
      if (pFil != (FILE *)NULL) {
        timptr = localtime((const long *)&nTim);
        fprintf(pFil, "%04d-%02d-%02d,%02d:%02d:%02d,",
                  timptr->tm_year + 1900, timptr->tm_mon + 1, timptr->tm_mday,
                  timptr->tm_hour, timptr->tm_min, timptr->tm_sec);
      }

/*
      for (nCnt = 0; nCnt < 3; ++nCnt) {
        if (sAssocDat[nCnt].pAssocInf != (MIT_ASSOC_INFO *)NULL) {
          sAssocDat[nCnt].nSta = sAssocDat[nCnt].pAssocInf->pstAssocCtrl->state;
          if (sAssocDat[nCnt].nSta != MI_ASSOC_STATE_ACTIVE) {
            nTASE2_Sta[nCnt] = 0;
printf("GWDC - ASSOCIATION '%s' NOT ACTICE\n", sAssocDat[nCnt].strAssocNam);
          } else {
            nTASE2_Sta[nCnt] = 1;
printf("GWDC - ASSOCIATION '%s' ACTIVE\n", sAssocDat[nCnt].strAssocNam);
          }
        }
*/
      for (nCnt = 0; nCnt < 3; ++nCnt)
      {
         if (sAssocDat[nCnt][0].pAssocInf != (MIT_ASSOC_INFO *)NULL)
         {
			nTASE2_Sta[nCnt] = 0;
            sAssocDat[nCnt][0].nSta = sAssocDat[nCnt][0].pAssocInf->pstAssocCtrl->state;
            if (sAssocDat[nCnt][0].nSta != MI_ASSOC_STATE_ACTIVE) {
               printf("GWDC - ASSOCIATION '%s' :0\tNOT ACTICE\t000000\n", sAssocDat[nCnt][0].strAssocNam);
            } else {
               nTASE2_Sta[nCnt] = 1;
               printf("GWDC - ASSOCIATION '%s' :0\tACTIVE\t111111\n", sAssocDat[nCnt][0].strAssocNam);
            }
         }
         if (sAssocDat[nCnt][1].pAssocInf != (MIT_ASSOC_INFO *)NULL)
         {
            sAssocDat[nCnt][1].nSta = sAssocDat[nCnt][1].pAssocInf->pstAssocCtrl->state;
            if (sAssocDat[nCnt][1].nSta != MI_ASSOC_STATE_ACTIVE) {
//               nTASE2_Sta[nCnt] = 0;
               printf("GWDC - ASSOCIATION '%s' :1\tNOT ACTICE\t000000\n", sAssocDat[nCnt][1].strAssocNam);
            } else {
               nTASE2_Sta[nCnt] = 1;
               printf("GWDC - ASSOCIATION '%s' :1\tACTIVE\t111111\n", sAssocDat[nCnt][1].strAssocNam);
            }
         }

#ifdef PROASSET_TEST
/*#######################################################################*/
{
/* Simulation: Receive Command/Setpoint */

  sSigTASE2_W.nDst = 0;
  sSigTASE2_W.nValBits = 0;
  sSigTASE2_W.nVal = 0;
  sSigTASE2_W.rVal = 0;
  sSigTASE2_W.nValidity = 0;
  sSigTASE2_W.nCurrentSource = 0;
  sSigTASE2_W.nNormalValue = 0;;
  sSigTASE2_W.nTimeStampQuality = 0;
  sSigTASE2_W.nOverflow = 0;
  sSigTASE2_W.sTimeStamp.nTim = 0;
  sSigTASE2_W.sTimeStamp.nMSec = 0;

printf("GWDC - TASE2 received command '%s' - %08x\n", "TGLBCTRL13FSTART", bVal);
  strcpy(sSigTASE2_W.strDCNam, "TGLBCTRL13FSTART");
  sSigTASE2_W.nFct = 101;
  sSigTASE2_W.nType = TYPE_COMMAND;
  sSigTASE2_W.nValBits = ((bVal >> 6) & 0x03);
  nSta = _q_write(pQ_DCw, sizeof(sGWSigTASE2), (unsigned char *)&sSigTASE2_W, 0);
  bVal = 0x40;

printf("GWDC - TASE2 received setpoint '%s' - %f\n", "TGLBSETPOINT13", rVal);
  strcpy(sSigTASE2_W.strDCNam, "TGLBSETPOINT13");
  sSigTASE2_W.nFct = 103;
  sSigTASE2_W.nType = TYPE_SETPOINT;
  sSigTASE2_W.rVal = rVal;
  nSta = _q_write(pQ_DCw, sizeof(sGWSigTASE2), (unsigned char *)&sSigTASE2_W, 0);
  rVal += 10.0;

printf("GWDC - TASE2 received command '%s' - %08x\n", "TGLBCTRL13FSTOP", bVal);
  strcpy(sSigTASE2_W.strDCNam, "TGLBCTRL13FSTOP");
  sSigTASE2_W.nFct = 101;
  sSigTASE2_W.nType = TYPE_COMMAND;
  sSigTASE2_W.nValBits = ((bVal >> 6) & 0x03);
  nSta = _q_write(pQ_DCw, sizeof(sGWSigTASE2), (unsigned char *)&sSigTASE2_W, 0);
  bVal = 0x80;

printf("GWDC - TASE2 received command '%s' - %08x\n", "TGLBCTRL137", bVal);
  strcpy(sSigTASE2_W.strDCNam, "TGLBCTRL137");
  sSigTASE2_W.nFct = 101;
  sSigTASE2_W.nType = TYPE_COMMAND;
  sSigTASE2_W.nValBits = ((bVal >> 6) & 0x03);
  nSta = _q_write(pQ_DCw, sizeof(sGWSigTASE2), (unsigned char *)&sSigTASE2_W, 0);
  bVal = 0x40;

}
/*#######################################################################*/
#endif

      }

      if (pFil != (FILE *)NULL) {
        fprintf(pFil, "1,%d,2,%d,3,%d",
                  nTASE2_Sta[0], nTASE2_Sta[1], nTASE2_Sta[2]);
        fclose(pFil);
      }

    }
    nLng = sizeof(sGWSigTASE2);  //added 2003.7.20 zy for check length in queue

    nSta = _q_read(pQ_DCr, &nLng, (unsigned char *)&sSigTASE2_R, 0);
    gwdc_timeout_flag = 0;
    alarm(3);                  //added 2004.8.14 zy for time out in net send
    if (nSta == 0) {
      switch (sSigTASE2_R.nFct) {
        case 11:
          Fct_StartAssociations();
          break;
        case 12:
          Fct_StopAssociations();
          break;
        case 32:
          Fct_ModifyLocalDataValue(&sSigTASE2_R);
          break;
        case 53:
          Fct_SendInformationMessage(&sSigTASE2_R);
          break;
        default:
          Fct_Nothing();
          break;
      }
    } else if (nSta == 1) {
      // no data in queue
    } else {
      // ERROR;
    }

/* Check if there are ack files from appws    added 2003.7.13 zy */

    fchk_num ++;
    if( fchk_num>= 2000)
    {
    fchk_num = 0;
#ifdef WINDOWS32
    sprintf(strBuf, "dir /B /OE %s >%s 2>/NUL", "R:\\", "T:\\Tmp\\ackfilelist.txt");
#endif
#ifdef HPUX
    sprintf(strBuf, "ls -1 %s*.OK* %s*.ER* >%s 2>/dev/null",
              "/usr/gwaysw/gway/gwapwsf/", "/usr/gwaysw/gway/gwapwsf/",
              "/usr/gwaysw/gway/gwtmp/ackfilelist.txt");
#endif
    system(strBuf);
    pFile = fopen("/usr/gwaysw/gway/gwtmp/ackfilelist.txt", "r");
    if (pFile != (FILE *)NULL)
       {
       if (fgets(strBuf, 256, pFile) != (char *)NULL)
          {
      // get first file name from directory listing
          nLng = strlen(strBuf);
          if (nLng > 1)
             {
             if (strBuf[nLng - 1] == 0x0a)
                {
                strBuf[nLng - 1] = '\0';
                }

             sSigTASE2_R.nVal = INFOMSG_REF_SCHEDULE;
             sSigTASE2_R.nDst = cAck_nDst;
             nLng = strlen("/usr/gwaysw/gway/gwapwsf/");
             strcpy(sSigTASE2_R.strDCNam, &strBuf[nLng]);
             Fct_SendInformationMessage(&sSigTASE2_R);

             }
          }
       fclose(pFile);
       }
    }
    if (mi_wait_service_ms == 0)
      msToWait = 5; /* give the processor a rest of 5 milliseconds */
    else
      msToWait = mi_wait_service_ms;
    wait_any_event(msToWait); 	/* Wait a little bit	*/
    serveCount = 0;
    while (mvl_comm_serve() && (serveCount < MAX_SERVICE_CALLS)) {	
      /* Loop until "mvl_comm_serve" return 0 */
      serveCount++;
    }
    mi_service();
    updatePtChgTbl();

//added 2004.8.14 zy for time out in net send
    if(gwdc_timeout_flag)
    {
 // stop all associations
        printf("&&&&&&&&&&&&time out now stop association!!!=============\n");
        Fct_AbortAssociations();
        printf("************time out now stop association out!!!------------\n");
        mi_service();
        gwdc_timeout_flag = 0;

 // start all associations
       nAssocNum[0] = 0;
       sAssocDat[0][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       sAssocDat[0][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       nAssocNum[1] = 0;
       sAssocDat[1][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       sAssocDat[1][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       nAssocNum[2] = 0;
       sAssocDat[2][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       sAssocDat[2][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       nAssocNum[3] = 0;
       sAssocDat[3][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       sAssocDat[3][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
       cnt = 0;
       Fct_StartAssociations();
       alarm(0);

       pstMitAssoc = pstAssocInfoSet;
       while (pstMitAssoc)
       {
//Following modified for support 2 association in one remote 2003.7.30 zy.
        if(nAssocNum[cnt] == 0)
        {
		sAssocDat[cnt][0].pAssocInf = pstMitAssoc;
		strcpy(sAssocDat[cnt][0].strAssocNam, pstMitAssoc->associationName);
		strcpy(sAssocDat[cnt][0].strRemoteNam, pstMitAssoc->remoteName);
		sAssocDat[cnt][0].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
		sAssocDat[cnt][0].nSta = pstMitAssoc->pstAssocCtrl->state;

      		printf("GWDC - Association %d:1 '%s' (%08x), '%s' (%08x) - %d\n",
            	 cnt, sAssocDat[cnt][0].strAssocNam, sAssocDat[cnt][0].pAssocInf,
           	  sAssocDat[cnt][0].strRemoteNam, sAssocDat[cnt][0].pMitMisVcc,
           	  sAssocDat[cnt][0].nSta);
     		nAssocNum[cnt] = 1;
	}
	else
	{
		if( strcmp(sAssocDat[cnt][0].strRemoteNam, pstMitAssoc->remoteName) == 0)
		{
			sAssocDat[cnt][1].pAssocInf = pstMitAssoc;
			strcpy(sAssocDat[cnt][1].strAssocNam, pstMitAssoc->associationName);
			strcpy(sAssocDat[cnt][1].strRemoteNam, pstMitAssoc->remoteName);
			sAssocDat[cnt][1].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
			sAssocDat[cnt][1].nSta = pstMitAssoc->pstAssocCtrl->state;

			printf("GWDC - Association %d:2 '%s' (%08x), '%s' (%08x) - %d\n",
				cnt, sAssocDat[cnt][1].strAssocNam, sAssocDat[cnt][1].pAssocInf,
				sAssocDat[cnt][1].strRemoteNam, sAssocDat[cnt][1].pMitMisVcc,
				sAssocDat[cnt][1].nSta);
				nAssocNum[cnt] = 2;
		}
		else
		{
			++cnt;
			continue;
		}
	}

	pstMitAssoc = (MIT_ASSOC_INFO *)list_get_next((void *)pstAssocInfoSet, (void *)pstMitAssoc);
      }

    }

  }
    pFil = fopen("/usr/gwaysw/gway/gwtmp/TASE2Status.txt", "w");
    if (pFil != (FILE *)NULL) {
    time(&nTim);
    timptr = localtime((const long *)&nTim);
    fprintf(pFil, "%04d-%02d-%02d,%02d:%02d:%02d,1,0,2,0,3,0",
                  timptr->tm_year + 1900, timptr->tm_mon + 1, timptr->tm_mday,
                  timptr->tm_hour, timptr->tm_min, timptr->tm_sec);
    fclose(pFil);
    }

}		      



//------------------------------------------------------------------------------
// init memory
//------------------------------------------------------------------------------

static ST_VOID mem_chk_error_detected(ST_VOID);
static ST_VOID *my_malloc_err(ST_UINT size);
static ST_VOID *my_calloc_err(ST_UINT num, ST_UINT size);
static ST_VOID *my_realloc_err(ST_VOID *old, ST_UINT size);

static ST_CHAR *spareMem;



static ST_VOID init_mem(ST_VOID)

{

#if defined(NO_GLB_VAR_INIT)
  mvl_init_glb_vars();
#endif

  // Allocate spare memory to allow logging/printing memory errors
  spareMem = (ST_CHAR *)malloc(500);

  // trap mem_chk errors
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



//------------------------------------------------------------------------------
// this function is called from the debug version of the mem library when an 
// error of any type is detected
//------------------------------------------------------------------------------

static ST_INT memErrDetected;

static ST_VOID mem_chk_error_detected(ST_VOID)

{

  if (!memErrDetected) {
    free(spareMem);
    memErrDetected = SD_TRUE;
    // printf("\n Memory Error Detected! Check log file");
    dyn_mem_ptr_status();
  }

}



//------------------------------------------------------------------------------
// Memory Allocation Error Handling Functions.
// These functions are called from mem_chk when it is unable to
// perform the requested operation. These functions must either return
// a valid buffer or not return at all.
//------------------------------------------------------------------------------
static ST_VOID *my_malloc_err(ST_UINT size)

{

  mem_chk_error_detected();
  // printf("\n Malloc");
  exit(2);
  return(NULL);   

}

static ST_VOID *my_calloc_err(ST_UINT num, ST_UINT size)

{
  
  mem_chk_error_detected();
  exit(3);
  return (NULL);   

}

static ST_VOID *my_realloc_err(ST_VOID *old, ST_UINT size)

{

  mem_chk_error_detected();
  exit(4);
  return(NULL);

}



//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
static void Fct_Nothing( void )

{

  return;

}




//------------------------------------------------------------------------------
// init_log_cfg

//------------------------------------------------------------------------------
static ST_VOID init_log_cfg(ST_VOID)

{

#ifdef DEBUG_SISCO
  if (logcfgx(MIT_XML_MIT_LOG_FILE) != SD_SUCCESS) {
    // printf("\n Parsing of 'logcfg.xml' configuration file failed");
    // if (sLogCtrl->fc.fileName)
    //   printf("\n Check log file '%s'.", sLogCtrl->fc.fileName);
    exit(5);
  }
  ml_mlog_install();
#endif

}


//------------------------------------------------------------------------------
// init_acse
//------------------------------------------------------------------------------
static ST_VOID init_acse()

{

  ST_RET ret;

  printf("\n############## i am in init_acse 00\n"); 

  // Added for Marben Stack
#if !defined(MAP30_ACSE)
  ret = osicfgx(MIT_XML_OSI_CFG_FILE, &mvlCfg);
  if (ret != SD_SUCCESS) {
    // printf("\nosicfgx failed, err = %d. Check configuration.\n", ret);
    exit(1);
  }
#endif 
   
  printf("\n############## i am in init_acse\n"); 
  ret = mvl_start_acse(&mvlCfg);
  if (ret != SD_SUCCESS)
    exit(1);

}


//------------------------------------------------------------------------------
// init_local_ar_name
//------------------------------------------------------------------------------
static ST_RET init_local_ar_name(ST_VOID)

{

  ST_RET 		ret = SD_FAILURE;
  MIT_MIS_VCC	*pstCurMisVcc;

  // Take the first local name we can find on an MIS VCC and use
  // it for the MVL local AR Name

  // hopefully we have an MisVcc

  pstCurMisVcc = pstMisVccSet;
  if (pstCurMisVcc) {
    if (pstCurMisVcc->localArName[0]) {
      strcpy(mvlCfg.local_ar_name, pstCurMisVcc->localArName);
      ret = SD_SUCCESS;
    }
    list_get_next (pstMisVccSet, pstCurMisVcc);
  }
  return (ret);

}



#if defined (_WIN32) && !defined(MOSI)

//------------------------------------------------------------------------------
// init_event_handling
//------------------------------------------------------------------------------
ST_VOID init_event_handling(ST_VOID)

{

  static char EventName[32];

  // For NT and WIN95, we use the SUIC and Keyboard event semaphores in the
  // 'wait for event' function

  // First create the SUIC event 						*/
  sprintf(EventName, "MmsEvent%d", getpid ());
  MMSEventName = EventName;
  hMMSEvent = CreateEvent(NULL, SD_FALSE, SD_FALSE, MMSEventName);
  if (hMMSEvent == NULL) {
    // printf("\n CreateEvent failed, rc = %d", GetLastError());
    // wait_msg("");
  }

}

//------------------------------------------------------------------------------
//  wait_any_event
//------------------------------------------------------------------------------
ST_VOID wait_any_event(ST_LONG max_ms_delay)

{

  HANDLE hArray[2];
  DWORD rc;
  int i;

  // Just do a muxwait on the keyboard and SUIC event semaphores
  hArray[0] = hMMSEvent;
  hArray[1] = kbdEvent;

  rc = WaitForMultipleObjects(2, hArray, SD_FALSE, max_ms_delay);
  if (rc == 0xFFFFFFFF) {
    // printf("\n WaitForMultipleObjects() failed, rc = %d", GetLastError());
    return;
  }

  if (rc == WAIT_TIMEOUT)
    return;    // nothing going on at the moment

  i = rc - WAIT_OBJECT_0;

  switch (i) {
    case 0:    // SUIC event, autoreset
      break;
    case 1:    // keyboard event, must reset
      if (ResetEvent(kbdEvent) == SD_FALSE)
        // printf("ERROR: Reset Kbd Event, rc = %d", GetLastError);
      break;
    default:
      break;
  }

}

#endif
