/************************************************************************/
/* CIWA SOFTWARE MODULE HEADER                                          */
/************************************************************************/
/*                                                                      */
/* MODULE NAME : iccp_main.c (originally mit_main.c)                   	*/
/* PRODUCT(S)  : ICCP TASE.2 Extensions for MMS-EASE Lite		*/
/*                                                                      */
/* MODULE DESCRIPTION :                                                 */
/*   	Main for the ICCP TASE.2 / application                          */
/*                                                                      */
/*                                                                      */
/* MODIFICATION LOG :                                                   */
/*  Date     Who   Rev                     Comments                     */
/* --------  ---  ------   -------------------------------------------  */
/* 03/28/07  zy    2       updated                                      */
/* 12/06/06  zy    1       Created for SUN ICCP TASE.2 application      */
/************************************************************************/

#include "../inc/mit_main.h"

#include "../inc/cppinc.h"

#include "../inc/iccp_main.h"

/*
#define	DEBUG
*/
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

extern MIT_DEVICE	    *pstDeviceSet;
extern ST_VOID    DisplayDeviceYYP (ST_VOID);
static char *thisFileName = __FILE__;

/************************************************************************/
/* Global and Local variables						*/
/************************************************************************/
ST_INT	DC_cnt; /*remote dispatch center num */


ST_DOUBLE	beginTime, endTime;		/* for elapsed time	*/
ST_INT		compSleepTime;
MVL_CFG_INFO	mvlCfg;
time_t	compStartTime, compStopTime;
ST_UINT mit_debug_sel;
unsigned char cur_stn_id;		//added by yyp 2011.03.09
unsigned char cur_remote_id;
short	cmd_end;
extern int  chann_priv_old, chann_priv;				//added by yyp 2011.10.09
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
ST_VOID wait_any_event (ST_LONG max_ms_delay);
ST_VOID mitUnconfigure_xml (ST_VOID);
int GetICCPChannPriv() ;
/************************************************************************/
/************************************************************************/
/* General program control variables */

static ST_BOOLEAN doIt = SD_TRUE;

void Tase2_Service( void );
void SaveAssocStatus(ST_LONG nTimsav);

ST_VOID Fct_StartAssociations( ST_VOID );
ST_VOID Fct_StopAssociations( ST_VOID );
ST_VOID Fct_AbortAssociations( ST_VOID );

extern char	strPrtBuf[256];

extern byte	cAck_nDst; /*added for ack schedule information 2003.7.12 zy */
/*
typedef struct sGWAssociation
{
	char            strAssocNam[64];
	char            strRemoteNam[64];
	MIT_ASSOC_INFO *pAssocInf;
	MIT_MIS_VCC    *pMitMisVcc;
	int             nSta;
} sGWAssoc;
*/
ST_INT	nAssocNum[MAX_DC_NUM];		/* Added for save association number for remotes */
sGWAssoc  sAssocDat[MAX_DC_NUM][2];	/* Changed to two dimension for 2 association in one remote 2003.7.30 zy */
/* sAssocDat[0] - Cascade Dispatch Center
   sAssocDat[1] - National Dispatch Center
   sAssocDat[2] - Central China Dispatch Center
*/

ST_INT   nTASE2_Sta[4];

int	iccp_timeout_flag;
char	infobuf[1024];
SHM_BUF *shm_buf_addr;
//SHM_ICCP_DATA *shm_iccp_data_addr;
int	shm_id_buf,shm_id_iccp;

SOEBUF	soebuf;
int	soe_overflow_flag, soe_overflow_Sendflag;
unsigned char	cmd_src;




void Net_Timeout(int Sig_no)
{
    signal( SIGALRM, Net_Timeout);
    iccp_timeout_flag = 1;
    printf("+++++iccp: recv time out!!!\n");
    alarm(3);
}

void Endproc(int sig_no)
{
	int	i,nCnt;
	char	filebuf[128];
	FILE	*pFil;
	ST_LONG		nTim;
	struct tm	*timptr;

	sprintf(infobuf,"  ---- iccp: exit signal %d received! Exit ----\n",sig_no);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	mi_terminate();
	mvl_end_acse();  		/* Stop the lower layer subsystem */
	printf ("\n+++iccp: Deallocating rescources\n\n");

	mitUnconfigure_xml();		/* deallocate all the configuration */
	dyn_mem_ptr_status ();		/* Log memory allocation usage */

	strcpy(filebuf,ICCP_TMP_PATH);
	strcat(filebuf,ICCP_STATUS_FILE);
	pFil = fopen(filebuf, "w");
	if (pFil != (FILE *)NULL)
	{
		time(&nTim);
		timptr = localtime((const long *)&nTim);
		fprintf(pFil, "%04d-%02d-%02d,%02d:%02d:%02d ",
				timptr->tm_year + 1900, timptr->tm_mon + 1, timptr->tm_mday,
				timptr->tm_hour, timptr->tm_min, timptr->tm_sec);
		for (nCnt = 0; nCnt < DC_cnt; ++nCnt)
		{
			UpdateChannelFlag(nCnt,(unsigned short)1);
			fprintf(pFil, "DC%d:0 ",nCnt+1);
		}
		fclose(pFil);
	}
	InterLockFree();
	UnMountDB();
	lan_finish(0);
	exit(0);
} 

/************************************************************************/
/* main:  entry point: working loop					*/
/************************************************************************/

void main ( int argc, char *argv[] )
{
	ST_INT          nCnt;
	MIT_ASSOC_INFO *pstMitAssoc;
	MIT_MIS_VCC	*pstCurMisVcc;

	if(-1==MountDB(DATABASE_NAME))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}
	//removed by yyp 2011.10.09
	/*
	if(LinktoSemSet(SEM_WR_COMMCMD)==-1) 
	{
		printf("\nFail to LinktoSemSet(SEM_WR_COMMCMD)!");
		exit(-1);	
	}
	*/
	if(get_lan_shm(4)== FALSE)
	{
		printf("\nFail to get comm_cmd shm!");
		exit(-1);
	}	
	if(InterLockFileLoad()==-1)
	{
		printf("InterLock.dat error!\n");
		exit(1);
	}

	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);
	
	/*	创建并获取iccp buf共享区      */
	if ((shm_id_buf=shmget(ftok(ICCP_BUF_FILE_KEY,SHM_BUF_KEY_CHAR_ICCP),sizeof(SHM_BUF),0777|IPC_CREAT|IPC_EXCL))==-1)
	{
		if ( errno==EEXIST)
		{
			shm_id_buf= shmget ( ftok (ICCP_BUF_FILE_KEY,SHM_BUF_KEY_CHAR_ICCP),sizeof (SHM_BUF),0);
#ifdef DEBUG
			printf ( "iccp :shm buf key %d shmid %d is existed already\n",SHM_BUF_KEY_CHAR_ICCP,shm_id_buf);
			//return;
#endif
		}
		else
			printf( "create_shm shm_id_buf:shmget");
	}
#ifdef DEBUG
	printf ( "the shm_id_buf is:%d\n",shm_id_buf);
#endif

	shm_buf_addr=( SHM_BUF *)shmat( shm_id_buf,0,SHM_RND);
#ifdef DEBUG
	printf ( "the shm_buf_addr is:0x%x\n",shm_buf_addr );
#endif
	if ((long *)shm_buf_addr==(long *)-1)
		printf("\niccp::Error shmat shm_buf_addr: errno=%d\n",errno);
	else
		printf("\niccp:: shmat shm_buf_addr: OK!\n");
	/* create buf share memory */
	shm_buf_addr= CreateShareMemory((char)ICCP_BUF_FILE_KEY, sizeof(SHM_BUF), "iccp_buf");
	if((void*)-1==shm_buf_addr)
    	{	
    		return ;
	}
#ifdef DEBUG
	printf ( "the shm_buf_addr is:0x%x\n",shm_buf_addr);
#endif
	if((shm_buf_addr->header_ptr < 0)||(shm_buf_addr->header_ptr >= DSP_INFO_SIZE))
		shm_buf_addr->header_ptr = 0;
		
	/* create iccp data share memory */
	shm_iccp_data_addr= CreateShareMemory((char)SHM_DATA_KEY_CHAR_ICCP, sizeof(SHM_ICCP_DATA), "iccp_data");
	if((void*)-1==shm_iccp_data_addr)
    	{	
    		return ;
	}
#ifdef DEBUG
	printf ( "the shm_data_iccp_addr is:0x%x\n",shm_iccp_data_addr );
#endif



	sprintf(infobuf,"H9000通信系统: TASE.2协议(iccp_main) Start!\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

/*	m_mem_debug = SD_TRUE; */
	mit_debug_sel = MITLOG_ERR | MIT_LOG_NERR;
/*	mit_debug_sel = 0; */
	icfg_debug_sel = ICFG_LOG_ERR;
	m_mem_debug = SD_TRUE;
	m_fill_en = SD_TRUE;
/*	m_heap_check_enable= SD_TRUE; */
/*	m_check_list_enable= SD_TRUE; */

	init_mem();	   /* Set up memory allocation tools */
	init_log_cfg();	   /* Set up logging subsystem */

	/* read the config file */
	MIT_LOG_FLOW0 ("Initializing ...");
	sprintf (infobuf,"iccp: - Reading in configuration file ...\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	if (mitConfigure_xml(MIT_XML_MIT_CFG_FILE, &mvlCfg) != SD_SUCCESS)
	{    
		MIT_LOG_ERR0("Configuration File Errors - check log file.");
		sprintf (infobuf,"iccp_main: - Configuration File Errors - check log file.\n");
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(1);
	}
	mitConfigurePtChgTbl();
	sleep(5);
	sprintf(infobuf,"Initalizing local name\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	if (init_local_ar_name() != SD_SUCCESS)
	{
		sprintf(infobuf,"init_local_ar_name(): No Local AR Name\n");
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		MIT_LOG_ERR0("init_local_ar_name(): No Local AR Name");
		exit(2);
	}

	/* Start the lower layer subsystem / initializing acse */
	sprintf (infobuf,"\n Initalizing ACSE");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	init_acse();

	/* initalizing MI */
	sprintf (infobuf,"\n Initalizing MI\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
#if 0
/* set up the handler for Clients connecting without AE titles */
/*	u_mvl_connect_ind_fun = u_mvl_connect_ind; */
#endif
	mi_initialize();

	/* init and read the signal files */
	InitData();
	/* start all associations */
	for(nCnt=0; nCnt<MAX_DC_NUM; nCnt++)
	{
		nTASE2_Sta[nCnt] = -1;
		nAssocNum[nCnt] = 0;
		sAssocDat[nCnt][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
		sAssocDat[nCnt][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
	}
	nCnt = 0;
	pstMitAssoc = pstAssocInfoSet;
	pstCurMisVcc=pstMisVccSet;
	while (pstMitAssoc)
	{
	/* Following modified for support 2 association in one remote 2003.7.30 zy. */
		if(nAssocNum[nCnt] == 0)
		{
			
			sAssocDat[nCnt][0].pAssocInf = pstMitAssoc;
			strcpy(sAssocDat[nCnt][0].strAssocNam, pstMitAssoc->associationName);
			strcpy(sAssocDat[nCnt][0].strRemoteNam, pstMitAssoc->remoteName);
			sAssocDat[nCnt][0].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
			sAssocDat[nCnt][0].nSta = pstMitAssoc->pstAssocCtrl->state;
			sAssocDat[nCnt][0].nSta = pstMitAssoc->pstAssocCtrl->state;
			nAssocNum[nCnt] = 1;

			sprintf(infobuf,"iccp: - Association %d:1 '%s' (%08x), '%s' (%08x) - %d\n",
					nCnt, sAssocDat[nCnt][0].strAssocNam, sAssocDat[nCnt][0].pAssocInf,
					sAssocDat[nCnt][0].strRemoteNam, sAssocDat[nCnt][0].pMitMisVcc,
					sAssocDat[nCnt][0].nSta);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
		}
		else
		{
			if( strcmp(sAssocDat[nCnt][0].strRemoteNam, pstMitAssoc->remoteName) == 0)
			{
				sAssocDat[nCnt][1].pAssocInf = pstMitAssoc;
				strcpy(sAssocDat[nCnt][1].strAssocNam, pstMitAssoc->associationName);
				strcpy(sAssocDat[nCnt][1].strRemoteNam, pstMitAssoc->remoteName);
				sAssocDat[nCnt][1].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
				sAssocDat[nCnt][1].nSta = pstMitAssoc->pstAssocCtrl->state;
				nAssocNum[nCnt] = 2;

				sprintf(infobuf,"iccp: - Association %d:2 '%s' (%08x), '%s' (%08x) - %d\n",
						nCnt, sAssocDat[nCnt][1].strAssocNam, sAssocDat[nCnt][1].pAssocInf,
						sAssocDat[nCnt][1].strRemoteNam, sAssocDat[nCnt][1].pMitMisVcc,
						sAssocDat[nCnt][1].nSta);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				++nCnt;
				continue;
			}
		}
		pstCurMisVcc = (MIT_MIS_VCC *) list_find_last((struct dbl_lnk *)pstMisVccSet);		
		pstMitAssoc = (MIT_ASSOC_INFO *)list_get_next((void *)pstAssocInfoSet, (void *)pstMitAssoc);
	}

	DC_cnt = nCnt + 1;

	/* initialization ready */
	sprintf(infobuf,"Initialization complete! Remote Association num = %d\n",DC_cnt);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	MIT_LOG_FLOW0 ("Initialization complete");

#if !defined(MOSI)
	init_event_handling();
#endif
  
	chk_malloc(1); /* marker */

	cAck_nDst = 0x01; /* added for ack schedule information 2003.7.13 zy */
	InitICCPShm();	
	Tase2_Service();

	mi_terminate();
	mvl_end_acse();  		/* Stop the lower layer subsystem */

	sprintf (infobuf,"\n+++iccp: Deallocating rescources\n\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	mitUnconfigure_xml();		/* deallocate all the configuration */
	dyn_mem_ptr_status ();		/* Log memory allocation usage */

} /* end main */

void SaveAssocStatus(ST_LONG nTimsav)
{
	char	filebuf[128];
	FILE	*pFil;
	ST_INT	nCnt;
	struct tm	*timptr;
	unsigned short chann_flag;
	strcpy(filebuf,ICCP_TMP_PATH);
	strcat(filebuf,ICCP_STATUS_FILE);
	pFil = fopen(filebuf, "w");
	if (pFil != (FILE *)NULL)
	{
		timptr = localtime((const long *)&nTimsav);
		fprintf(pFil, "%04d-%02d-%02d %02d:%02d:%02d ",
				timptr->tm_year + 1900, timptr->tm_mon + 1, timptr->tm_mday,
				timptr->tm_hour, timptr->tm_min, timptr->tm_sec);
	}
	for (nCnt = 0; nCnt < DC_cnt; ++nCnt)
	{
		nTASE2_Sta[nCnt] = -1;
		if (sAssocDat[nCnt][0].pAssocInf != (MIT_ASSOC_INFO *)NULL)
		{
			nTASE2_Sta[nCnt] = 0;
			sAssocDat[nCnt][0].nSta = sAssocDat[nCnt][0].pAssocInf->pstAssocCtrl->state;
			if (sAssocDat[nCnt][0].nSta != MI_ASSOC_STATE_ACTIVE)
			{
				sprintf(infobuf,"iccp: - ASSOCIATION '%s' :%d 0\tNOT ACTICE\t000000\n", sAssocDat[nCnt][0].strAssocNam,nCnt);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				nTASE2_Sta[nCnt] = 1;
				sprintf(infobuf,"iccp: - ASSOCIATION '%s' :%d 0\tACTIVE\t\t111111\n", sAssocDat[nCnt][0].strAssocNam,nCnt);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
		}
		if (sAssocDat[nCnt][1].pAssocInf != (MIT_ASSOC_INFO *)NULL)
		{
			sAssocDat[nCnt][1].nSta = sAssocDat[nCnt][1].pAssocInf->pstAssocCtrl->state;
			if (sAssocDat[nCnt][1].nSta != MI_ASSOC_STATE_ACTIVE)
			{
			/*	nTASE2_Sta[nCnt] = 0; */
				sprintf(infobuf,"iccp: - ASSOCIATION '%s' :%d 1\tNOT ACTICE\t000000\n", sAssocDat[nCnt][1].strAssocNam,nCnt);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				nTASE2_Sta[nCnt] = 1;
				sprintf(infobuf,"iccp: - ASSOCIATION '%s' :%d 1\tACTIVE\t\t111111\n", sAssocDat[nCnt][1].strAssocNam,nCnt);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
		}
		chann_flag = 	CHANSTAT_FAIL;
		if(nTASE2_Sta[nCnt] == 0)
		{
			chann_flag = 	CHANSTAT_FAIL;
		}
		else
		{
			chann_flag=chann_priv;
			if( chann_flag == CHANPRIV_REPAIR )
			{
				chann_flag = 	CHANSTAT_WORK;
			}
		}

		UpdateChannelFlag(nCnt,chann_flag);
	}
	if (pFil != (FILE *)NULL)
	{
		for (nCnt = 0; nCnt < DC_cnt; ++nCnt)
		{
			fprintf(pFil, "DC%d STATUS:%d ",nCnt+1,nTASE2_Sta[nCnt]);
		}
		fprintf(pFil, "\n");
		fclose(pFil);
	}
} /* end of assoc status check */


#define MAX_SERVICE_CALLS	10

void Call_MI_Service( void )
{
	ST_INT   serveCount;
/*
	ST_LONG  msToWait;

	if (mi_wait_service_ms == 0)
		msToWait = 5;  */ /* give the processor a rest of 5 milliseconds */
/*	else
		msToWait = mi_wait_service_ms;

	wait_any_event(msToWait); */	/* Wait a little bit	*/

	serveCount = 0;

	while (mvl_comm_serve() && (serveCount < MAX_SERVICE_CALLS))
	{	
	/* Loop until "mvl_comm_serve" return 0 */
		serveCount++;
	}

	mi_service();
	updatePtChgTbl();
}

/*------------------------------------------------------------------------------
// tase.2 services
//------------------------------------------------------------------------------*/

void Tase2_Service( void )
{
	MIT_ASSOC_INFO *pstMitAssoc;
	
	int    iccp_start_assoctions;
	USHORT	host_state;
	ST_LONG  msToWait;
	ST_INT   nCnt;
	ST_INT   nLng;
	ST_INT   nSta;
	ST_INT   serveCount;
	ST_LONG  nTim;
	ST_LONG  nTimSav,nAckTimSav,nSendTimSav,nCycTimSav;
	int      nDst;

	char     filebuf[128];
	char     strBuf[256];
	FILE    *pFile;
	
	FILE    *pFil;
	struct tm timbuf;
	struct tm *timptr;
	long tim;
	char STName[40];			//needed to be modified yyp 2011.09.28
	time(&nTim);
	nTimSav = nAckTimSav = nSendTimSav = nCycTimSav = nTim;
	iccp_timeout_flag = 0;
	iccp_start_assoctions = 0;
	signal( SIGALRM, Net_Timeout);
	strcpy(STName,"XJB");		//needed to be modified yyp 2011.09.28
	//added by yyp 2011.03.09
	cur_stn_id=0;
	cur_remote_id = 0;
	if(GetStnIdByName( STName, &cur_stn_id ) == -1)
	{
		printf("\nIEC104: STName %s error\n",STName);
		//return NULL;
	}
	if(cur_stn_id<1 || cur_stn_id>MAX_STN_NUM)
	{
		printf("\nIEC104: cur_stn_id %d error\n",cur_stn_id);
		//return NULL;
	}

	cmd_end = shm_comm_cmd->stn_data[cur_stn_id].head;//yyp
	ClearCommShm(cur_stn_id,0,TYPE_ALL_SEG);	


	Fct_StartAssociations();
	Call_MI_Service();


	for(;;)
	{
		//MySleep(50);
		MySleep(500);			//yyp debug
		/*
		host_state = -1;  
		GetLocalHostState(&host_state);
		host_state=1;			//yyp debug
		if( host_state!=1 )
		{
			continue;
		}
		*/
		time(&nTim);
		if ((nTim - nTimSav) >= 10)  /*do assoc status check every 30 seconds */
		{
			nTimSav = nTim;
			SaveAssocStatus(nTim);
		}
		
		chann_priv_old	= chann_priv;
		chann_priv = GetICCPChannPriv();
		chann_priv = CHANPRIV_ONDUTY;
		if( chann_priv == CHANPRIV_WAIT)
		{
			MySleep(2000);
			printf("chnn_priv is CHANPRIV_WAIT\n");
			//continue;
		}
		if( chann_priv == CHANPRIV_TEST)
		{
			MySleep(2000);
			printf("chnn_priv is CHANPRIV_TEST\n");			
			//continue;
		}

		iccp_timeout_flag = 0;
		alarm(3);                  /* added 2004.8.14 zy for time out in net send */


		if( chann_priv == CHANPRIV_ONDUTY)
		{
			ICCPSendCmd();				
		}
		else
		{
			if(cmd_end != shm_comm_cmd->stn_data[cur_stn_id].head)
			{
				ClearCommShm(cur_stn_id,0,TYPE_ALL_SEG);
				cmd_end = shm_comm_cmd->stn_data[cur_stn_id].head;//yyp				
			}
		}

	   /* Check if there are ack files from appws    added 2003.7.13 zy */
		if ((nTim - nAckTimSav) >= 10)  /*do ack files check every 10 seconds */
		{
			nAckTimSav = nTim;

#ifdef WINDOWS32
			sprintf(strBuf, "dir /B /OE %s >%s 2>/NUL", "R:\\", "T:\\Tmp\\ackfilelist.txt");
#else
			strcpy(filebuf,ICCP_TMP_PATH);
			strcat(filebuf,ICCP_ACK_FILELIST);
			sprintf(strBuf, "ls -1 %s*.OK* %s*.ER* >%s 2>/dev/null",
				ICCP_ACK_PATH, ICCP_ACK_PATH, filebuf);
#endif
			system(strBuf);

			pFile = fopen(filebuf, "r");
			if (pFile != (FILE *)NULL)
			{
				memset(strBuf, 0, 256);
				if (fgets(strBuf, 256, pFile) != (char *)NULL)
				{
				/* get first file name from directory listing */
					nLng = strlen(strBuf);
					if (nLng > 1)
					{
						if (strBuf[nLng - 1] == 0x0a)
						{
							strBuf[nLng - 1] = '\0';
						}
						nDst = 0;
						strcpy(filebuf,strBuf);
						SendScheduleAckInform(nDst, strBuf);
#ifdef WINDOWS32
						sprintf(strBuf, "move %s %s 2>/NUL", filebuf, ICCP_TMP_PATH, "ScheduleAckFile.txt");
						system(strBuf);
#else
						sprintf(strBuf, "mv %s %s%s", filebuf, ICCP_TMP_PATH,"ScheduleAckFile.txt");
						system(strBuf);
#endif
					}
				}
				fclose(pFile);
			}
		}

		if (mi_wait_service_ms == 0)
			msToWait = 5;   /* give the processor a rest of 5 milliseconds */
		else
			msToWait = mi_wait_service_ms;

		wait_any_event(msToWait); 	/* Wait a little bit	*/
		Call_MI_Service();

		/* added 2004.8.14 zy for time out in net send */
		if(iccp_timeout_flag)
		{
		/* stop all associations */
			sprintf(infobuf,"&&&&&&&&&&&&time out now stop association!!!=============\n");
			printf("%s",infobuf);
			WriteDspInfo(infobuf);

			Fct_AbortAssociations();

			sprintf(infobuf,"************time out now stop association out!!!------------\n");
			printf("%s",infobuf);
			WriteDspInfo(infobuf);

			mi_service();
			iccp_timeout_flag = 0;

		/* start all associations */
			for(nCnt=0; nCnt<MAX_DC_NUM; nCnt++)
			{
				nTASE2_Sta[nCnt] = -1;
				nAssocNum[nCnt] = 0;
				sAssocDat[nCnt][0].pAssocInf = (MIT_ASSOC_INFO *)NULL;
				sAssocDat[nCnt][1].pAssocInf = (MIT_ASSOC_INFO *)NULL;
			}
			nCnt = 0;
/*
			Fct_StartAssociations();
			Call_MI_Service();
			alarm(0);
*/
			pstMitAssoc = pstAssocInfoSet;
			while (pstMitAssoc)
			{
			/* Following modified for support 2 association in one remote 2003.7.30 zy. */
				if(nAssocNum[nCnt] == 0)
				{
					sAssocDat[nCnt][0].pAssocInf = pstMitAssoc;
					strcpy(sAssocDat[nCnt][0].strAssocNam, pstMitAssoc->associationName);
					strcpy(sAssocDat[nCnt][0].strRemoteNam, pstMitAssoc->remoteName);
					sAssocDat[nCnt][0].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
					sAssocDat[nCnt][0].nSta = pstMitAssoc->pstAssocCtrl->state;
					nAssocNum[nCnt] = 1;

					sprintf(infobuf,"iccp: - Association %d:1 '%s' (%08x), '%s' (%08x) - %d\n",
							nCnt, sAssocDat[nCnt][0].strAssocNam, sAssocDat[nCnt][0].pAssocInf,
							sAssocDat[nCnt][0].strRemoteNam, sAssocDat[nCnt][0].pMitMisVcc,
							sAssocDat[nCnt][0].nSta);
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
				}
				else
				{
					if( strcmp(sAssocDat[nCnt][0].strRemoteNam, pstMitAssoc->remoteName) == 0)
					{
						sAssocDat[nCnt][1].pAssocInf = pstMitAssoc;
						strcpy(sAssocDat[nCnt][1].strAssocNam, pstMitAssoc->associationName);
						strcpy(sAssocDat[nCnt][1].strRemoteNam, pstMitAssoc->remoteName);
						sAssocDat[nCnt][1].pMitMisVcc = findMitMisVcc(pstMitAssoc->remoteName);
						sAssocDat[nCnt][1].nSta = pstMitAssoc->pstAssocCtrl->state;
						nAssocNum[nCnt] = 2;

						sprintf(infobuf,"iccp: - Association %d:2 '%s' (%08x), '%s' (%08x) - %d\n",
							nCnt, sAssocDat[nCnt][1].strAssocNam, sAssocDat[nCnt][1].pAssocInf,
							sAssocDat[nCnt][1].strRemoteNam, sAssocDat[nCnt][1].pMitMisVcc,
							sAssocDat[nCnt][1].nSta);
						printf("%s",infobuf);
						WriteDspInfo(infobuf);
					}
					else
					{
						++nCnt;
						continue;
					}
				}
				pstMitAssoc = (MIT_ASSOC_INFO *)list_get_next((void *)pstAssocInfoSet, (void *)pstMitAssoc);
			}
			Fct_StartAssociations();
			Call_MI_Service();
			alarm(0);
		}
	}
}		      


/*------------------------------------------------------------------------------
// init memory
//------------------------------------------------------------------------------*/

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

	/* Allocate spare memory to allow logging/printing memory errors */
	spareMem = (ST_CHAR *)malloc(500);

	/* trap mem_chk errors */
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



/*------------------------------------------------------------------------------
// this function is called from the debug version of the mem library when an 
// error of any type is detected
//-----------------------------------------------------------------------------*/

static ST_INT memErrDetected;

static ST_VOID mem_chk_error_detected(ST_VOID)
{

	if (!memErrDetected)
	{
		free(spareMem);
		memErrDetected = SD_TRUE;
		printf("\n Memory Error Detected! Check log file");
		dyn_mem_ptr_status();
	}
}



/*------------------------------------------------------------------------------
// Memory Allocation Error Handling Functions.
// These functions are called from mem_chk when it is unable to
// perform the requested operation. These functions must either return
// a valid buffer or not return at all.
//------------------------------------------------------------------------------*/
static ST_VOID *my_malloc_err(ST_UINT size)
{

	mem_chk_error_detected();
	/* printf("\n Malloc"); */
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



/*------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------*/
static void Fct_Nothing( void )
{

	return;

}


/*------------------------------------------------------------------------------
// init_log_cfg

//------------------------------------------------------------------------------*/
static ST_VOID init_log_cfg(ST_VOID)
{

#ifdef DEBUG_SISCO
	if (logcfgx(MIT_XML_MIT_LOG_FILE) != SD_SUCCESS)
	{
/*
		printf("\n Parsing of 'logcfg.xml' configuration file failed");
		if (sLogCtrl->fc.fileName)
			printf("\n Check log file '%s'.", sLogCtrl->fc.fileName);
*/
		exit(5);
	}
	ml_mlog_install();
#endif

}


/*------------------------------------------------------------------------------
// init_acse
//------------------------------------------------------------------------------*/
static ST_VOID init_acse()
{

	ST_RET ret;
	char     strBuf[256];
  /* Added for Marben Stack */
#if !defined(MAP30_ACSE)
	ret = osicfgx(MIT_XML_OSI_CFG_FILE, &mvlCfg);
	if (ret != SD_SUCCESS)
	{
		printf("\nosicfgx failed, err = %d. Check configuration.\n", ret);
		exit(1);
	}
#endif 
	ret = mvl_start_acse(&mvlCfg);
	if (ret != SD_SUCCESS)
	{
		sprintf(strBuf, "pkill slistend; pkill sreadd");
		system(strBuf);
		exit(1);
	}
}


/*------------------------------------------------------------------------------
// init_local_ar_name
//------------------------------------------------------------------------------*/
static ST_RET init_local_ar_name(ST_VOID)
{

	ST_RET 		ret = SD_FAILURE;
	MIT_MIS_VCC	*pstCurMisVcc;

  /* Take the first local name we can find on an MIS VCC and use */
  /* it for the MVL local AR Name */

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

/*------------------------------------------------------------------------------
// init_event_handling
//------------------------------------------------------------------------------*/
ST_VOID init_event_handling(ST_VOID)
{

	static char EventName[32];

  /* For NT and WIN95, we use the SUIC and Keyboard event semaphores in the */
  /* 'wait for event' function */

  /* First create the SUIC event 						*/
	sprintf(EventName, "MmsEvent%d", getpid ());
	MMSEventName = EventName;
	hMMSEvent = CreateEvent(NULL, SD_FALSE, SD_FALSE, MMSEventName);
	if (hMMSEvent == NULL)
	{
    /* printf("\n CreateEvent failed, rc = %d", GetLastError()); */
    /* wait_msg(""); */
	}
}

/*------------------------------------------------------------------------------
//  wait_any_event
//------------------------------------------------------------------------------*/
ST_VOID wait_any_event(ST_LONG max_ms_delay)
{

	HANDLE hArray[2];
	DWORD rc;
	int i;

  /* Just do a muxwait on the keyboard and SUIC event semaphores */
	hArray[0] = hMMSEvent;
	hArray[1] = kbdEvent;

	rc = WaitForMultipleObjects(2, hArray, SD_FALSE, max_ms_delay);
	if (rc == 0xFFFFFFFF)
	{
    /* printf("\n WaitForMultipleObjects() failed, rc = %d", GetLastError()); */
		return;
	}

	if (rc == WAIT_TIMEOUT)
		return;    /* nothing going on at the moment */

	i = rc - WAIT_OBJECT_0;

	switch (i)
	{
	case 0:    /* SUIC event, autoreset */
		break;
	case 1:    /* keyboard event, must reset */
		if (ResetEvent(kbdEvent) == SD_FALSE)
			printf("ERROR: Reset Kbd Event, rc = %d", GetLastError);
		break;
	default:
		break;
  }
}

#endif

int UpdateChannelFlag(int pnum, unsigned short status)
{
	char	name_buf[36];
	char	host_name[36];
	int	data_type;

	POINTER		point;
	DMS_COMMON	dbentry;
	POINTER		pnt;
	SW_MSG		lanbcdata;
	LAN_HEAD	lanhd;

	WR_VAR_UNION	new_value;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	time_t		tim;
	struct tm	tmpt;
	DATE_TIME	update_time;

	gethostname(host_name, 10);
	if(0!=GetHostStnId(host_name, &point.stn_id))
	{
		sprintf(infobuf,"GetHostStnId() %s No this hostname point in database file!\n",host_name);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return -1;
	}

	if(0!=GetHostSysLcuId(host_name, &point.dev_id))
	{
		sprintf(infobuf,"GetHostSysLcuId() %s No this hostname point in database file!\n",host_name);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return -1;
	}

	point.data_type= SYSMSG_TYPE;
	strcpy(name_buf,"XJB");
	strcat(name_buf,"_");
	strcat(name_buf,host_name);
	strcat(name_buf,"_C");
/*
	strcat(name_buf,sProcRec[pnum].NameID);
*/
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		sprintf(infobuf,"%s No this com channel status point in stn:%d dev:%d the database file!\n",
			name_buf,point.stn_id,point.dev_id);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return -1;
	}

	if(ReadEntryById(&point,&dbentry)==-1)
	{
		sprintf(infobuf,"%s No this com channel status point in %d:%d:%d:%d the database file!\n",
			name_buf,point.stn_id,point.dev_id,point.data_type,point.pt_id);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(FALSE);
	}
	else if(dbentry.var.ind.status.rt_state != status)
	{
		lanbcdata.type_id =DPS_IND_ALM;
		lanbcdata.stn_id =dbentry.point.stn_id;
		lanbcdata.dev_id =dbentry.point.dev_id;
		lanbcdata.data_type=dbentry.point.data_type;
		lanbcdata.number = 1;
		lanbcdata.point_id = dbentry.point.pt_id;

		lanbcdata.status.rt_state = status;
		lanbcdata.status.chan_fail = 0;	/*communication channel status: 0:normal; 1:Fail */
		lanbcdata.status.fault_tag = status;
		lanbcdata.status.alm_type = 1;
		lanbcdata.ms=0;
		GetLocalHostId(&lanbcdata.host_id);

		tim=time (NULL);
		tmpt=*localtime (&tim);
		lanbcdata.sec=tmpt.tm_sec;
		lanbcdata.min=tmpt.tm_min;
		lanbcdata.hour=tmpt.tm_hour;
		lanbcdata.day=tmpt.tm_mday;
		lanbcdata.month=tmpt.tm_mon+1;
		lanbcdata.year=tmpt.tm_year+1900;

		lanhd.length=sizeof(SW_MSG);
#ifdef _BIG_ENDIAN_
		Swap2Byte(&lanbcdata.number);
		Swap2Byte(&lanbcdata.point_id);
		Swap2Byte(&lanbcdata.status);
		Swap2Byte(&lanbcdata.ms);
		Swap2Byte(&lanbcdata.year);
		Swap2Byte(&lanhd.length);
#endif
		lanhd.dp_type=BJ_DB_TIME;

		lanhd.dest_id =0;
		lanhd.src_id =lanbcdata.host_id;
		lanhd.dest_stn =lanbcdata.stn_id;
		lanhd.packet_num =0;
		lan_out ((char *)&lanbcdata,lanhd,(unsigned char)0);

		pnt = dbentry.point;
		new_value.ind = lanbcdata.status;
		update_time.ms		= 0;
		update_time.second	= tmpt.tm_sec;
		update_time.minute	= tmpt.tm_min;
		update_time.hour	= tmpt.tm_hour;
		update_time.day		= tmpt.tm_mday;
		update_time.month	= tmpt.tm_mon+1;
		update_time.year	= tmpt.tm_year+1900;
		rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;

		if(SetPtValueIostById(&pnt, new_value, iost, update_time, rw_flag) == -1)
			printf ( "\nIEC101: %d-%d-%d-%d No this point in the database when Wdb!\n",
				pnt.stn_id, pnt.dev_id, pnt.data_type, pnt.pt_id);

	}
	return(TRUE);
}


int GetICCPChannPriv()   //返回0，不建链；返回1，建链发测试帧；返回2，为工作通道，采数，但不广播；返回3，为值班通道，采数且广播数据
{
	DMS_COMMON	dbentry;
	char	host_name[36];
	char	name_buf[256];	
	int		vir_chan_pri;
	gethostname(host_name, 10);	
	strcpy(name_buf,"CD.CD_SYS.SYS.");
	strcat(name_buf,"XJB");
	strcat(name_buf,"_");
	strcat(name_buf,host_name);
	strcat(name_buf,"_");
	strcat(name_buf,"C");
	strcat(name_buf,"_");
	strcat(name_buf,"P");
	//printf("\nvirtual_channel_pri_name==%s\n",sProcRec[pro_no].virtual_channel_pri_name);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this com channel status point in the database file!\n",name_buf);
		return(false);
	}
	vir_chan_pri=dbentry.var.ind.status.state;
	//printf("\nsProcRec[pro_no].vir_chan_pri==%d\n",dbentry.var.ind.status.state);
	return(vir_chan_pri);
}

