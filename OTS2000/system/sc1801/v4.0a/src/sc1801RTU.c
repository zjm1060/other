/************************************************************
*  FILE NAME:           sc1801RTU.c      						*
*  FUNCTION:            Controlled Station Main Process		*
*						Link Layer Communication Program	*
*  Version :            Win3.0a-->UNIX4.0a  by  zwj		              							*
*  DATE:                2007.12                			*
*															*
************************************************************/
#include "../inc/sc1801RTU.h"

#define	DEBUG
      
void	SC1801_Recv(void);
void	SC1801_Send(void);
float	Datcvt();
BOOL	ResponseNAK();
BOOL	ReportRTUConfig();
BOOL	RepeatTransmit();
BOOL	CommandSend();
BOOL	UpdateOrgData();
BOOL	ReportFullData();
BOOL	ReportExceptData();
BOOL	SequenceOfEvents();
BOOL	AckCommand();
BOOL	PulseAccumulatorReport();
BOOL	ReportDataBand();
BOOL	AckCtrlOutputARM();
BOOL	AckDataReqLatch();
BOOL	AckSynchronizeSystemTime();
BOOL	AckReportSysTime();
BOOL	AckReportInterfaceMode();
void	ClearRecvFault();
void	ClearSendRespFault();
void	PrintRecvData();
void	PrintSendCmdData();
void	RTURecvFun();

BOOL	GetRTUConfig();
BOOL	GetComConfig();

void	GetDBNameRangeTable();		

extern	char	infobuf[1024];
SHM_BUF	*shm_buf_addr;
extern	ASDUCOMDDATA	recv_data;
extern	ASDUDATA		send_data,send_data2;

int	ProcNum;
extern	PROC_REC	sProcRec[MAX_PROC_NUM+1];
/*HANDLE	hPort_com1,hPort_com2;*/
#ifdef SUN_OS
extern	int	hPort_com;
#else
HANDLE	hPort_com;
HANDLE	hRecvSemaphore;
#endif

int	shm_id_buf;

unsigned char rtu_status,rtu_jumpter;
unsigned char pntname[16][32][36],rtuconfig[16];
unsigned short	soenum;
SOEDEF	soedef[16*24];
SOEBUF	soebuf;
float	lorange[16][12],hirange[16][12];
unsigned short	databand;

/************************************************/
#ifdef SUN_OS
void Endproc(int sig_no)
{
	int i;

	if( getpid()==sProcRec[MAX_PROC_NUM].Pid )
	{
		sprintf(infobuf,"  ---- sc1801RTU: exit signal %d received!  ----\n",sig_no);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		for(i=0;i<ProcNum;i++)
		{
			if(sProcRec[i].Pid != -1)
			{
				sprintf(infobuf," kill ---- %s sc1801RTU: ! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				kill( sProcRec[i].Pid, SIGTERM);
				UpdateChannelFlag(i,(unsigned short)1);
			}
		}
	}
	else
	{
		UpdateChannelFlag(0,(unsigned short)1);
		sprintf(infobuf,"  ---- sc1801RTU: exit signal %d received! Exit ----\n",sig_no);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
	UnMountDB();
	lan_finish(0);
	exit(0);
} 
                                                                
void Timer(int sig_no)
{
	signal(SIGALRM,Timer);
/*	timeout=YES; */
}

void Waitchild(int nouse)
{
	int status,wpid,i;

	sprintf(infobuf,"  ---- sc1801RTU Master in Waitchild() !!!\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	alarm(1);
	signal(SIGALRM,Timer);
	wpid = wait(&status);
	alarm(0);
	if(wpid != -1)
	{
		for(i=0;i<ProcNum;i++)
		{
			if(sProcRec[i].Pid == wpid)
			{
				sprintf(infobuf,"  ---- %s sc1801RTU: stop! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				sProcRec[i].Pid = -1;
				break;
			}
		}
	}
	signal(SIGCHLD,Waitchild); 
}

/************************************************/
int	main(int argc, char *argv[])
{	/*char	sendbuf[1024];
	char	recvbuf[1024];*/
	int	dwByteRecvd,t;
	int	hThread_Recv;
	int	hThread_Send;
	/*int	stn_id=0;*/
	thread_t	ThreadID_Recv,ThreadID_Send,hThread_ret;

	SetModuleName("SC1801RTU");
	SetErrLogMode(argc, argv);
	//CreateLockFile(".1801.dxe");

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}

	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);

	/*	打开显示信息记录共享内存      */
/*	if ((shm_id_buf=shmget(ftok(SC1801_FILE_KEY,(SHM_BUF_KEY_CHAR+DSP_NUM)),sizeof(SHM_BUF),0777|IPC_CREAT|IPC_EXCL))==-1)
	{
		if ( errno==EEXIST)
		{
			shm_id_buf= shmget ( ftok (SC1801_FILE_KEY,(SHM_BUF_KEY_CHAR+DSP_NUM)),sizeof (SHM_BUF),0);
#ifdef DEBUG
			printf ( "sc1801 :shmkey %d shmid %d is existed already\n",(SHM_BUF_KEY_CHAR+DSP_NUM),shm_id_buf);
#endif
		}
		else
			printf( "create_shm shm_id_buf:shmget");
	}
#ifdef DEBUG
	printf ( "the shm_id_buf is:%d\n",shm_id_buf);
#endif*/
	/*	获取记录区共享内存地址  */
/*	shm_buf_addr=( SHM_BUF *)shmat( shm_id_buf,0,SHM_RND);
#ifdef DEBUG
	printf ( "the shm_buf_addr is:0x%x\n",shm_buf_addr );
#endif
	if ((long *)shm_buf_addr==(long *)-1)
		printf("\nsc1801::Error shmat shm_buf_addr: errno=%d\n",errno);
	else
		printf("\nsc1801:: shmat shm_buf_addr: OK!\n");

	if((shm_buf_addr->header_ptr < 0)||(shm_buf_addr->header_ptr >= DSP_INFO_SIZE))
		shm_buf_addr->header_ptr = 0;

	sprintf(infobuf,"H9000通信系统: %s sc1801协议(sc1801_RTU) Start!\n",sProcRec[stn_id].NameID);
*/	sprintf(infobuf,"H9000通信系统: sc1801协议(sc1801_RTU) Start!\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	rtu_status = 0x01;	/*CLOCK=0, RESET = 1; Use SST & SDB to Clear!*/
	rtu_jumpter = 0x00;	/*Display RTU id, No Timer board*/
	databand = 20;	/*Initial databand*/

	GetComConfig();
	GetRTUConfig();  /*Get modules type of slots*/
	GetDBNameRangeTable();	/*Get database name of each point*/

	soebuf.bufpnt = soebuf.sendpnt = 0;

	UpdateOrgData();

	for(;;)
	{
		
		
	/*	hPort_com = ComInit(COM_PORT1,Com_Baud_Rate,8,ODD,0);*/
		hPort_com = ComInit(strDev,nBaudRate,Databits,par,Stopbits);
		if(hPort_com < 0)
		{
			sprintf(infobuf,"\nsc1801: device %s init failed!!! Exit now!!!\n\n",strDev);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			sleep(3);
			
			continue;
		}
		sprintf(infobuf,"\nsc1801RTU: device %s init OK!!!!!!\n\n",strDev);
 			printf("%s",infobuf);
		break;
	}


	hThread_Recv = thr_create(NULL, 0, SC1801_Recv, NULL, 0, &ThreadID_Recv);
	if (hThread_Recv != 0)
	{
		sprintf(infobuf,"sc1801:Error in thr_create ThreadID_Recv:%d error: %d!\n ", ThreadID_Recv, errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(-1);
	}
	else
	{
		sprintf(infobuf,"sc1801:thr_create ThreadID_Recv: %d OK!\n",hThread_Recv);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}

	hThread_Send = thr_create(NULL, 0, SC1801_Send, NULL, 0, &ThreadID_Send);
	if (hThread_Send != 0)
	{
		sprintf(infobuf,"sc1801:Error in thr_create ThreadID_Send:%d error: %d!\n ", ThreadID_Send, errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(-1);
	}
	else
	{
		sprintf(infobuf,"sc1801:thr_create ThreadID_Send: %d OK!\n",hThread_Send);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
		while( !thr_join(0, &hThread_ret, NULL) )
	{
		if( hThread_ret==ThreadID_Recv )
		{
			sprintf(infobuf,"sc1801: restart thread ThreadID_Recv %d\n", hThread_ret);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			hThread_Recv = thr_create(NULL, 0, SC1801_Recv, NULL, 0, &ThreadID_Recv);
			if (hThread_Recv != 0)
			{
				sprintf(infobuf,"sc1801:Error in thr_create ThreadID_SOE:%d error: %d!\n ", ThreadID_Recv, errno);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				exit(-1);
			}
			else
			{
				sprintf(infobuf,"sc1801:thr_create ThreadID_Recv: %d OK!\n",hThread_Recv);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if( hThread_ret==ThreadID_Send )
		{
			sprintf(infobuf,"sc1801Send: restart thread ThreadID_Send %d\n", hThread_ret);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			hThread_Send = thr_create(NULL, 0, SC1801_Send, NULL, 0, &ThreadID_Send);
			if (hThread_Send != 0)
			{
				sprintf(infobuf,"sc1801Send:Error in thr_create ThreadID_Send:%d error: %d!\n ", ThreadID_Send, errno);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				exit(-1);
			}
			else
			{
				sprintf(infobuf,"sc1801Send:thr_create ThreadID_Send: %d OK!\n",hThread_Send);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}
	}

/*	sc1801 Link layer communication service*/

/*	RTURecvFun(1,hPort_com1); ****zwj****why open two COM***/  

	/*RTURecvFun(1,hPort_com);*/
}

#else
int	main(int argc, char *argv[])
{
	HANDLE	hThread_Recv;
	int		ThreadID_Recv;
	HANDLE	hThread_Send;
	int		ThreadID_Send;

	LONG	cMaxCount,cInitCount;
	/*HANDLE	hPort_com;
	HANDLE	hRecvSemaphore;*/

	SetModuleName("SC1801RTU");
/*	SetErrLogMode(argc, argv);*/
	//CreateLockFile(".1801.dxe");

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}


	/*sprintf(infobuf,"H9000通信系统: %s sc1801协议(sc1801_RTU) Start!\n",sProcRec[stn_id].NameID);*/
	sprintf(infobuf,"H9000通信系统: sc1801协议(sc1801_RTU) Start!\n");
	printf("%s",infobuf);
	/*WriteDspInfo(infobuf);*/


	rtu_status = 0x01;	//CLOCK=0, RESET = 1; Use SST & SDB to Clear!
	rtu_jumpter = 0x00;	//Display RTU id, No Timer board
	databand = 20;	//Initial databand

	GetComConfig();
	GetRTUConfig();  //Get modules type of slots
	GetDBNameRangeTable();	//Get database name of each point

	soebuf.bufpnt = soebuf.sendpnt = 0;

	UpdateOrgData();

	cMaxCount= 10;
	cInitCount = 0;

/*	hPort_com=ComInit(COM_PORT,Com_Baud_Rate,8,ODDPARITY,ONESTOPBIT);
	printf("\n %s init compelete BaudRate=%d,Databits=8,Par=%d,Stopbits=%d!!!\n\n",COM_PORT,
		Com_Baud_Rate,ODDPARITY,ONESTOPBIT);*/
	hPort_com = ComInit(strDev,nBaudRate,Databits,par,Stopbits);
	printf("\n %s init compelete BaudRate=%d,Databits=%d,Parity=%d,Stopbits=%d!!!\n\n",strDev,
		nBaudRate,Databits,par,Stopbits);

	/*hPort_com2=ComInit(COM_PORT2,Com_Baud_Rate2,8,ODDPARITY,ONESTOPBIT);
	printf("\n %s init compelete !!!\n\n",COM_PORT2);*/

	hRecvSemaphore = CreateSemaphore(NULL,cInitCount,cMaxCount,"sem_recv");

	if (hRecvSemaphore == NULL)
		printf("Error in CreateSemaphore sem_recv:%d\n",GetLastError());
	else
		printf("sem_recv: OK!\n");

	hThread_Recv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SC1801_Recv,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Recv);
	if (hThread_Recv == NULL)
		printf("Error in CreateThread ThreadID_Recv:%d\n",GetLastError());
	else
		printf("hThread_Recv: %d OK!\n",hThread_Recv);

/*	SetThreadPriority(hThread_Recv,THREAD_PRIORITY_BELOW_NORMAL);*/	 

 	hThread_Send = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SC1801_Send,
                                 NULL, CREATE_SUSPENDED, &ThreadID_Send);
	if (hThread_Send == NULL)
		printf("Error in CreateThread ThreadID_Send:%d\n",GetLastError());
	else
		printf("hThread_Send: %d OK!\n",hThread_Send);

	SetThreadPriority(hThread_Send,THREAD_PRIORITY_BELOW_NORMAL);	 


	ResumeThread( hThread_Recv);	
	ResumeThread( hThread_Send);	

//	sc1801 Link layer communication service

/*	RTURecvFun(1,hPort_com);  */
	for(;;)
		Sleep(10000);
	UnMountDB();
}
#endif

void	SC1801_Recv(void)
{
     printf("============SC1801_RTU Recv Channel 1 start!!!===========\n");
	RTURecvFun(1,hPort_com);
}

void WriteDspInfo(char *buf)
{
/*	int	bufptr,len,charlen;

	if ((long *)shm_buf_addr==(long *)-1)
		return;
	bufptr = shm_buf_addr->header_ptr;
	len = strlen(buf);
	charlen = DSP_INFO_SIZE - bufptr;
	if(charlen < len)
	{
		memcpy(&shm_buf_addr->buf[bufptr],buf,charlen);
		bufptr = len - charlen;
		memcpy(&shm_buf_addr->buf[0],&buf[charlen],bufptr);
		shm_buf_addr->header_ptr = bufptr;
	}
	else
	{
		memcpy(&shm_buf_addr->buf[bufptr],buf,len);
		charlen  = bufptr + len;
		if(charlen >= DSP_INFO_SIZE)
			shm_buf_addr->header_ptr = 0;
		else
			shm_buf_addr->header_ptr = charlen;
	}*/
	Err_log(buf);
}


int UpdateChannelFlag(int pnum, unsigned short status)
{
	char	name_buf[36];
	char	host_name[36];
	int	data_type;

	POINTER		point;
	DMS_COMMON	dbentry;
	POINTER		pnt;
	SYS_MSG		lanbcdata;
	LAN_HEAD	lanhd;

	WR_VAR_UNION	new_value;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	time_t		tim;
	struct tm	tmpt;
	DATE_TIME	update_time;

/*	strcpy(name_buf,SYS_TAB_NAME);
*/
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

	strcpy(name_buf,host_name);
	strcat(name_buf,"_COM1");
	printf("$$$$$$$%s$$$$$$$",name_buf);
	/*strcat(name_buf,sProcRec[pnum].NameID);*/
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		sprintf(infobuf,"%s %s %s No this com channel status point in stn:%d dev:%d the database file!\n",
			sProcRec[pnum].NameID,sProcRec[pnum].Ip,name_buf,point.stn_id,point.dev_id);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return -1;
	}

	if(ReadEntryById(&point,&dbentry)==-1)
	{
		sprintf(infobuf,"%s %s %s No this com channel status point in stn:%d dev:%d the database file!\n",
			sProcRec[pnum].NameID,sProcRec[pnum].Ip,name_buf,point.stn_id,point.dev_id);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(FALSE);
	}
	
	else if(dbentry.var.ind.status.rt_state != status)
	{	printf("%s %s %s com channel status point in stn:%d dev:%d the database file!\n",sProcRec[pnum].NameID,sProcRec[pnum].Ip,name_buf,point.stn_id,point.dev_id);
		lanbcdata.type_id =DPS_IND_ALM;
		lanbcdata.stn_id =dbentry.point.stn_id;
		lanbcdata.dev_id =dbentry.point.dev_id;
		lanbcdata.data_type=dbentry.point.data_type;
		lanbcdata.number = 1;
		lanbcdata.point_id = dbentry.point.pt_id;

		lanbcdata.status=status+0x0100;	/*communication channel status: 0:normal; 1:Fail */
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

		lanhd.length=sizeof(SYS_MSG);
#ifdef SUN
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
		printf("$$$$$@@@@@@@@@$$$$$$");

		pnt = dbentry.point;
		new_value.ind.rt_state = status;
		new_value.ind.fault_tag = status;
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

