/****************************************************************
*  FILE NAME:           cc_iec101s_com.c      			*
*  FUNCTION:            Controlled Station Main Process		*
*			Link Layer Communication Program	*
*  NAME :  			              			*
*  DATE:                Apr,2006                		*
*								*
****************************************************************/
#include "../inc/iec101s_com.h"

/*#define	DEBUG */
#define	STN_DEF_FILE		"/home/ems/h9000/iec101/CC101S_STATION.DEF"	
#define EXE_FILE		"/home/ems/h9000/bin/cc_iec104s_com.exe"
#define DSP_NUM			6

int		ProcNum;
PROC_REC	sProcRec[MAX_PROC_NUM+1];

extern	ASDUCOMDDATA	recv_data;
extern	ASDUDATA	send_data;
extern	SPDEF		spdef;
extern	DPDEF		dpdef;
extern	MEDEF		medef;
extern	CYCDEF		cycdef;
extern	DCDEF		dcdef;
extern	SEDEF		sedef;
extern	ITDEF		itdef;
extern	SCDEF		scdef;
extern	RCDEF		rcdef;

int	hPort_com;

char	infobuf[1024];
SHM_BUF *shm_buf_addr;
int	shm_id_buf;

unsigned char	recvbuf[1024],cmdsendbuf[24];
unsigned char	sendbuf[1024];
SOEBUF		soebuf;
CMDBUF		cmdbuf;
SEBUF		sebuf;
PMBUF		pmbuf;
ICBUF		icbuf;
CIBUF		cibuf;
RDBUF		rdbuf;
RPBUF		rpbuf;
TESTBUF		testbuf;
CDBUF		cdbuf;
MIRBUF		mirbuf;

int	fcb,acd,dfc;
int	resetlinkf,initflag,comdflag,pollflag,cmdterm;
int	ClockSyncflag;
int	Readflag,Testflag,Resetflag;
int	Paramflag,MirrorNAckflag;
int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
int	interrogationflag,counterflag,icgroupflag,cigroupflag;
int	delayflag;
int	cyclicflag,scanflag;

unsigned short	delayalarm;
unsigned char	cot_interrogation,cot_counter;
unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
unsigned int	repeatnum,timeoutnum;
unsigned short	seladdr;
short	databand;

unsigned char	nStnId;
unsigned char	nASDUId;
unsigned char	cmd_src;
char		strDev[36];
int		nBaudRate;

int	Dualhost;	/* 0: Without Dual host switch; 1: With Dual host switch */
int	Gps;		/* 0: Without GPS; 1: With GPS */
int	GiType;		/*General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_ND; 3: M_ME_NC */
int	MeType;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
int	SEType;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
short	me_code;	/* 0 or -32768 */
short	se_code;	/* 0 or -32768 */
int	TimeTag;	/*Time tag> 0: no; 1: short; 2: long */
int	sp_cos;		/*sp_cos> 0: sp/dp transmit once; 1: twice */
int	Cmd_Sel;	/*  0: cmd without select; 1: cmd with select */

time_t	scan_tim;
time_t	cycle_tim;

time_t	scan_period;
time_t	cycle_period;
time_t	cmd_period;
time_t	cmd_trans_delay;

#ifdef REC_YF_AGC_CMD
extern	unsigned short	daysaved;
#endif

void Endproc(int sig_no)
{
	int i;

	if( getpid()==sProcRec[MAX_PROC_NUM].Pid )
	{
		sprintf(infobuf,"  ---- cc_iec101s_com: exit signal %d received!  ----\n",sig_no);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		for(i=0;i<MAX_PROC_NUM;i++)
		{
			if(sProcRec[i].Pid != -1)
			{
				sprintf(infobuf," kill ---- %s cc_iec101s_com: ! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				kill( sProcRec[i].Pid,SIGKILL );
			}
		}
	}
	else
	{
		sprintf(infobuf,"  ---- cc_iec101s_com: exit signal %d received! Exit ----\n",sig_no);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
	InterLockFree();
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

	sprintf(infobuf,"  ---- cc_iec101s_com Master in Waitchild() !!!\n");
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	alarm(1);
	signal(SIGALRM,Timer);
	wpid = wait(&status);
	alarm(0);
	if(wpid != -1)
	{
		for(i=0;i<MAX_PROC_NUM;i++)
		{
			if(sProcRec[i].Pid = wpid)
			{
				sprintf(infobuf,"  ---- %s cc_iec101s_com: stop! ( pid=%05d ) ----\n",sProcRec[i].NameID,sProcRec[i].Pid);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				sProcRec[i].Pid = -1;
				break;
			}
		}
	}
	signal(SIGCHLD,Waitchild); 
}

void main(int argc,char *argv[])
{
	int		hThread_SOE,hThread_com;
	thread_t	ThreadID_SOE,ThreadID_com,hThread_ret;

	int		stn_id;
/*
	BOOL	host_priv;
*/
	USHORT	HostState;

	for(;;)
	{
		if( ReadStnDef(STN_DEF_FILE) == TRUE )
			break;
            	printf("\n###ALARM!!! IEC101s::Read Station define file %s Error!\n",STN_DEF_FILE);
		sleep(5);
		continue;
	}

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
/*
	if(GetLocalHostPrvg("GW_SERVER", &host_priv) != 0)
	{
		sprintf(infobuf,"\n###ALARM!!! iec101s_com.dxe:: Can not run in this host!\n");
		exit(-1);
	}
	if(host_priv != TRUE)
	{
		sprintf(infobuf,"\n###ALARM!!! iec101s_com.dxe:: this host is not GW server!\n");
		exit(-1);
	}
*/
	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}
	if(-1==InterLockFileLoad())
	{
		printf("Fail to Load InterLock File!\n");
		exit(-1);
	}

	stn_id = 0;

	signal(SIGKILL,Endproc);
	signal(SIGTERM,Endproc);
	signal(SIGINT,Endproc);

	/*	打开显示信息记录共享内存      */
	if ((shm_id_buf=shmget(ftok(IEC_FILE_KEY,(SHM_BUF_KEY_CHAR+DSP_NUM)),sizeof(SHM_BUF),0777|IPC_CREAT|IPC_EXCL))==-1)
	{
		if ( errno==EEXIST)
		{
			shm_id_buf= shmget ( ftok (IEC_FILE_KEY,(SHM_BUF_KEY_CHAR+DSP_NUM)),sizeof (SHM_BUF),0);
#ifdef DEBUG
			printf ( "cc_iec101s :shmkey %d shmid %d is existed already\n",(SHM_BUF_KEY_CHAR+DSP_NUM),shm_id_buf);
#endif
		}
		else
			printf( "create_shm shm_id_buf:shmget");
	}
#ifdef DEBUG
	printf ( "the shm_id_buf is:%d\n",shm_id_buf);
#endif
	/*	获取记录区共享内存地址  */
	shm_buf_addr=( SHM_BUF *)shmat( shm_id_buf,0,SHM_RND);
#ifdef DEBUG
	printf ( "the shm_buf_addr is:0x%x\n",shm_buf_addr );
#endif
	if ((long *)shm_buf_addr==(long *)-1)
		printf("\ncc_iec101s::Error shmat shm_buf_addr: errno=%d\n",errno);
	else
		printf("\ncc_iec101s:: shmat shm_buf_addr: OK!\n");

	shm_buf_addr->header_ptr = 0;

	sprintf(infobuf,"H9000通信系统: %s iec101协议(cc_iec101s_com) Start!\n",sProcRec[stn_id].NameID);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	InitData(sProcRec[stn_id].NameID);

	hThread_SOE = thr_create(NULL, 0, IEC101_SOE, NULL, 0, &ThreadID_SOE);
	if (hThread_SOE != 0)
	{
		sprintf(infobuf,"cc_iec101s_com:Error in thr_create ThreadID_SOE:%d error: %d!\n ", ThreadID_SOE, errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(-1);
	}
	else
	{
		sprintf(infobuf,"cc_iec101s_com:thr_create ThreadID_SOE: %d OK!\n",hThread_SOE);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}

	hThread_com = thr_create(NULL, 0, Com_Link, (void *)stn_id, 0, &ThreadID_com);
	if (hThread_com != 0)
	{
		sprintf(infobuf,"cc_iec101s_com:Error in thr_create ThreadID_com:%d error: %d!\n ", ThreadID_com, errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(-1);
	}
	else
	{
		sprintf(infobuf,"cc_iec101s_com:thr_create ThreadID_com: %d OK!\n",ThreadID_com);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}

	while( !thr_join(0, &hThread_ret, NULL) )
	{
		if( hThread_ret==ThreadID_SOE )
		{
			sprintf(infobuf,"cc_iec101s_com: restart thread ThreadID_SOE %d\n", hThread_ret);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			hThread_SOE = thr_create(NULL, 0, IEC101_SOE, NULL, 0, &ThreadID_SOE);
			if (hThread_SOE != 0)
			{
				sprintf(infobuf,"cc_iec101s_com:Error in thr_create ThreadID_SOE:%d error: %d!\n ", ThreadID_SOE, errno);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				exit(-1);
			}
			else
			{
				sprintf(infobuf,"cc_iec101s_com:thr_create ThreadID_SOE: %d OK!\n",hThread_SOE);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if( hThread_ret==ThreadID_com )
		{
			sprintf(infobuf,"cc_iec101s_com: restart thread ThreadID_com %d\n", hThread_ret);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			hThread_com = thr_create(NULL, 0, Com_Link, (void *)stn_id, 0, &ThreadID_com);
			if (hThread_com != 0)
			{
				sprintf(infobuf,"cc_iec101s_com:Error in thr_create ThreadID_com:%d error: %d!\n ", ThreadID_com, errno);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				exit(-1);
			}
			else
			{
				sprintf(infobuf,"cc_iec101s_com:thr_create ThreadID_com: %d OK!\n",hThread_com);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		}
	}
}

void * Com_Link(void * arg)
{
	int	fSuccess;					  	
	int	dwByteRecvd,nOffset;
	int	ret;

	unsigned char	nChkSum;
/*	int	dataflag,dataflow; */

	unsigned int	sendbufheadpt,recvbufheadpt;
	unsigned int	sendbufptr1,sendbufptr2;
	unsigned char	chksum,ctrlfield,ctrlfunc,framelength;
	unsigned int	i,localinitflag,pnum;

	unsigned short	infoaddress,HostStat;
	unsigned char	qrp;

	fd_set	fset;
	struct timeval	tout;

	time_t	tim;
	struct tm *timeptr;
	DMS_COMMON	dbentry;
	int		data_type;
	char		name_buf[NAME_SIZE];

	sendbufheadpt = 0;
	sendbufptr1 = 0;
	sendbufptr2 = 0;
	localinitflag = 1;
	pnum = 0;
/*
	Initial Serial Communication port COM_PORT
	COM_PORT, Com_Baud_Rate is defined in IEC101S_CONFIG.DEF
*/
	for(;;)
	{
		hPort_com = ComInit(strDev,nBaudRate,8,EVEN,0);

		if(hPort_com < 0)
		{
			sprintf(infobuf,"\ncc_iec101s_com: device %s init failed!!! repeat init now!!!\n\n",strDev);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			sleep(3);
			/*thr_exit(NULL);*/
			continue;
		}
		break;
	}
	sprintf(infobuf,"\ncc_iec101s_com: %s BaudRate= %d init compelete fd = %d!!!\n\n",strDev,nBaudRate,hPort_com);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

/* IEC 870-5 Link layer communication service */

	tim=time(&tim);
#ifdef REC_YF_AGC_CMD
	timeptr=localtime(&tim);
	daysaved = timeptr->tm_mday;
#endif
	scan_tim = cycle_tim = tim;

	for(;;)
	{
/*Read from Interface Communication port */

		memset(recvbuf, 0, VARFRAMELEN);
		nOffset = 0;

		FD_ZERO(&fset);
		FD_SET(hPort_com,&fset);
		tout.tv_sec = 2;
		tout.tv_usec = 0;
		ret = select(hPort_com+1, &fset, NULL, NULL, &tout);
		tim=time(&tim);
		if(ret <= 0)
		{
			timeoutnum++;

			sprintf(infobuf,"cc_iec101s_com: %s select() TIMEOUT No: %d! ret=%d\n",sProcRec[pnum].NameID,timeoutnum,ret);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);

			if(timeoutnum%20==0)
			{
				sprintf(infobuf,"cc_iec101s_com: ReceiveData: timeout! No: %d",timeoutnum);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				UpdateChannelFlag(pnum,(unsigned short)1);
			}
		}
		else
		{
			dwByteRecvd = read(hPort_com, &recvbuf[nOffset], 2*VARFRAMELEN);

			dwByteRecvd += nOffset;
			if (dwByteRecvd  <= 0)
			{
				sprintf(infobuf,"+++cc_iec101s_com: Controlled Station %s read error: %s\n",sProcRec[pnum].NameID,strerror(errno));
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		
			timeoutnum =0;
			sprintf(infobuf,"\ncc_iec101s_com: Controlled Station %s received data byte nNumber %u :\n",sProcRec[pnum].NameID,dwByteRecvd);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			PrintRecvData(dwByteRecvd);

			if(Dualhost)
			{
				if(-1==GetLocalHostState(&HostStat))
				{
					sprintf(infobuf,"!!! Host Stat not in database: %s\n",sProcRec[pnum].NameID);
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
					sleep(5);
					continue;
				}
				if( HostStat!=1 )
				{
					sprintf(infobuf,"!!! Host Stat is Slave ! Receive Only for %s\n",sProcRec[pnum].NameID);
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
					localinitflag = 1;
					scan_tim = cycle_tim = tim;
					continue;
				}
			}
			tim=time(&tim);

			if(!cyclicflag)
			{
				if((tim - cycle_tim) > cycle_period)
				{
					cyclicflag = 1;
					m_cCyclicGrpNum = 0;
					cycle_tim = tim;
				}
			}
			else
				cycle_tim = tim;
			if((!scanflag)&&(!interrogationflag))
			{
				if((tim - scan_tim) > scan_period)
				{
					m_cScanGrpNum = 0;
					scanflag = 1;
				}
			}
			else
				scan_tim = tim;

			if(cmdbuf.selected)
			{
				if((tim - cmdbuf.sel_tim) > cmd_period)
				{
					cmdbuf.selected = 0;
				}
			}

/*			sendbufheadpt = send_data.pnt; */

			switch (recvbuf[0])
			{
			case 0x10: /*fix frame */

				if((dwByteRecvd<FIXFRAMELEN)||(recvbuf[4]!=0x16)||(dwByteRecvd>=2*FIXFRAMELEN))
				{
					sprintf(infobuf,"Frame recv error for fix length frame\n");
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
					break;
				}

				chksum = recvbuf[1] + recvbuf[2];	
		
				if(chksum != recvbuf[3])
				{
					sprintf(infobuf,"Frame check sum error for fix length frame\n");
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
					break;
				}

				if((nStnId != recvbuf[2])&&(recvbuf[2] != 255))
				{
					sprintf(infobuf,"Station No %d error! Not %d\n",recvbuf[2],nStnId);
					printf("%s",infobuf);
					WriteDspInfo(infobuf);
					break;
				}

				timeoutnum = 0;
				UpdateChannelFlag(pnum,(unsigned char)0);

				ctrlfield = recvbuf[1] & 0x50;
				ctrlfunc = recvbuf[1] & 0x0F;
			
				switch(ctrlfunc)
				{
				case 0:	/*Reset link status frame */

					if(ctrlfield != 0x40)
					{
						sprintf(infobuf,"Reset link status frame control field received wrong!\n");
						printf("%s",infobuf);
						WriteDspInfo(infobuf);
						break;
					}
					sprintf(infobuf,"Reset link status frame received!\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					fcb = 0;
					if(localinitflag == 1)
					{
						sprintf(infobuf,"Local initialization end!\n");
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						acd = 1;
					}
					if(Resetflag == 1)
					{
						sprintf(infobuf,"Remote initialization end!\n");
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						acd = 1;
					}
					if(!ResetLinkStatus())
						sprintf(infobuf,"ACK Reset link status frame wrong!\n");
					break;

				case 8:	/*Request demand bit frame or Quick check frame(not used) */
					if(ctrlfield != 0x40)
					{
						sprintf(infobuf,"Request demand bit frame control field received wrong!\n");
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						break;
					}
					sprintf(infobuf,"Request demand bit frame received!\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);

					if(!RespondLinkStatus())
						sprintf(infobuf,"ACK Request demand bit frame wrong!\n");
					break;

				case 9:	/*Request Link status frame */
					if(ctrlfield != 0x40)
					{
						sprintf(infobuf,"Request Link status frame control field received wrong!\n");
						break;
					}
					sprintf(infobuf,"Request Link status frame received!\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);

					if(!RespondLinkStatus())
						sprintf(infobuf,"ACK Request Link status frame wrong!\n");
					break;

				case 10:	/*Request data class 1 */
					if(ctrlfield != 0x50)
					{
						sprintf(infobuf,"Request data class 1 Control field received wrong!\n");
						break;
					}
					sprintf(infobuf,"Request data class 1 frame received!\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);

					if((localinitflag == 1)||(Resetflag == 1))
					{
						acd = 0;
						EndofInitialization();
						localinitflag = 0;
						Resetflag = 0;
						fcb = (int)(recvbuf[1] & 0x20);
						break;
					}
					if((int)(recvbuf[1] & 0x20) == fcb)
					{
						sprintf(infobuf,"Repeat fcb received! former class 1 frame send repeated!\n");
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						RepeatTransmit();
						break;
					}
					fcb = !fcb;
					RespondUserDataClass1();
/*
					if(acd || cmdterm)
						RespondUserDataClass1();
					else
						NoRequestData();
*/
					break;

				case 11:	/*Request data class 2 */
					if(ctrlfield != 0x50)
					{
						sprintf(infobuf,"Request data class 2 Control field received wrong!\n");
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						break;
					}
					sprintf(infobuf,"Request data class 2 frame received!\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					if((int)(recvbuf[1] & 0x20) == fcb)
					{
						sprintf(infobuf,"Repeat fcb received! former class 2 frame send repeated!\n");
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						RepeatTransmit();
						break;
					}
					fcb = !fcb;

					if((localinitflag == 1)||(Resetflag == 1))
					{
						acd = 1;
						NoRequestData();
						break;
					}
					RespondUserDataClass2();
					break;

				default :
					sprintf(infobuf,"Error response function code %2x\n",ctrlfunc);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					break;
				}/*switch(ctrlfunc) */

				break;

			case 0x68:	/*variable frame for send or request data */

				if((dwByteRecvd<MINFRAMELEN)||(dwByteRecvd < (recvbuf[1]+6)))
				{
					sprintf(infobuf,"Byte number received=%d less than min variable frame length\n",dwByteRecvd);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					break;
				}
			
				if((recvbuf[3] != 0x68)||(recvbuf[1] != recvbuf[2])||(recvbuf[dwByteRecvd - 1] != 0x16))
				{
					sprintf(infobuf,"Frame check error for variable length frame\n");
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					break;
				}

				framelength = recvbuf[1]+4;
				chksum = 0;
				for(i=4;i<framelength;i++)
					chksum = chksum + recvbuf[i];	

				if(chksum != recvbuf[framelength])
				{
					sprintf(infobuf,"Frame check sum %02X %02Xerror for variable length frame\n",chksum,recvbuf[framelength]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					break;
				}

				if((nStnId != recvbuf[5])&&(recvbuf[5] != 255))
				{
					sprintf(infobuf,"Station No: %d error! Not %d\n",recvbuf[5],nStnId);
					break;
				}
				if((nASDUId != recvbuf[9])&&(recvbuf[9] != 255))
				{
					sprintf(infobuf,"ASDU CMM Address: %d error! Not %d\n",recvbuf[9],nASDUId);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					break;
				}

				ctrlfunc = recvbuf[4] & 0x0F;
				ctrlfield = recvbuf[4] & 0xD0;
		
				switch(ctrlfunc)
				{
				case 3:	/*Send/confirm command frame */
					if(ctrlfield != 0x50)
					{
						sprintf(infobuf,"Command send/confirm frame control field received wrong!\n");
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						break;
					}
					sprintf(infobuf,"Command send/confirm frame %d received!\n",recvbuf[6]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					if((int)(recvbuf[4] & 0x20) == fcb)
					{
						sprintf(infobuf,"!!!+++Repeat fcb Command %d received! former frame will be send repeated!\n",recvbuf[6]);
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						RepeatTransmit();
						break;
					}
					else
						fcb = !fcb;

					AckCmd();	/*Link Lay ACK with ACD=1 */

					recvbufheadpt = recv_data.pnt;
					memset(&recv_data.data[recvbufheadpt],0,CMD_DATA_SIZE);
					memcpy(&recv_data.data[recvbufheadpt],&recvbuf[4],dwByteRecvd - 6);
					recv_data.length[recvbufheadpt] =(unsigned short) (dwByteRecvd - 6);

					sprintf(infobuf,"recv point = %d\n",recvbufheadpt);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);

					IEC870_5_Recv(pnum,recvbufheadpt);

					recvbufheadpt ++;
					if(recvbufheadpt >= CMD_ENTRY_NUMBER)
						recvbufheadpt = 0;
					recv_data.pnt = recvbufheadpt;
/*					ReleaseSemaphore(hRecvSemaphore,1,NULL); */

					break;

				case 1:	/*Reset process command frame */

					if(ctrlfield != 0x40)
					{
						sprintf(infobuf,"Reset process command frame Control field %d wrong!\n",recvbuf[4]);
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);
						NAckCmd();	/*Link Lay NACK */
					}
					AckCmd();	/*Link Lay ACK with ACD=1 */

					if(recvbuf[6] == C_RP_NA)	/*Reset process command 105 */
					{
#ifdef SUN_OS
						CrossCopy(&infoaddress,&recvbuf[10],2);	/* address of object */
#else
						memcpy(&infoaddress,&recvbuf[10],2);	 /*address of object */
#endif
						qrp = recvbuf[12];	/*QRP[1..8]:= 1: general reset 2: SOE buffer reset */

						rpbuf.tpyeId = recvbuf[6];
						rpbuf.qrp = qrp;
						rpbuf.ack_flag = 1;

						if((recvbuf[8] & 0x3F) == ACT)
						{
							if(infoaddress!=0)
							{
								testbuf.cot = (unsigned char)(0x40|UKINFOAD);
							}
							else if(qrp==1)
							{
								sprintf(infobuf,"reset process command QRP=1 general process reset frame!\n");
#ifdef DEBUG
								printf("%s",infobuf);
#endif
								WriteDspInfo(infobuf);

								Resetflag = 1;
								rpbuf.cot = ACTCON;
								soebuf.sendpnt = soebuf.bufpnt =0;
							}
							else if(qrp==2)
							{
								sprintf(infobuf,"reset process command QRP=2 SOE buffer reset frame!\n");
#ifdef DEBUG
								printf("%s",infobuf);
#endif
								WriteDspInfo(infobuf);

								Resetflag = 2;
								rpbuf.cot = ACTCON;
								soebuf.sendpnt = soebuf.bufpnt =0;
							}
							else
							{
								sprintf(infobuf,"reset process command QRP=%d frame?\n");
#ifdef DEBUG
								printf("%s",infobuf);
#endif
								WriteDspInfo(infobuf);

								rpbuf.cot = ACTCON;
							}
						}
						else
						{
							sprintf(infobuf,"reset process command QRP=%d cause of transmit %x wrong!\n",qrp,recvbuf[8]);
#ifdef DEBUG
							printf("%s",infobuf);
#endif
							WriteDspInfo(infobuf);

							rpbuf.cot = (unsigned char)(0x40|UKCOT);;
						}
					}
					else
					{
						sprintf(infobuf,"Wrong type id %d with ctrl %x FC=1 frame!\n",recvbuf[6],recvbuf[4]);
#ifdef DEBUG
						printf("%s",infobuf);
#endif
						WriteDspInfo(infobuf);

					}
					fcb = !fcb;
					break;

				default :

					sprintf(infobuf,"Error link control function code %2x\n",ctrlfunc);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);

					break;
				}/*switch(ctrlfunc) */
				break;

			default :
				sprintf(infobuf,"IEC received data start byte error! %2x :\n",recvbuf[0]);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				break;			

			}/*switch(recvbuf[0]) */
		}
	}/*for(;;) */
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
		return -1;

	if(0!=GetHostSysLcuId(host_name, &point.dev_id))
		return -1;

	point.data_type= SYSMSG_TYPE;

	strcpy(name_buf,host_name);
	strcat(name_buf,"_");
	strcat(name_buf,sProcRec[pnum].NameID);
	if(-1==GetEntryIdByName(name_buf, &point))
		return -1;

	if(ReadEntryById(&point,&dbentry)==-1)
	{
		sprintf(infobuf,"%s %s %s No this com channel status point in stn:%d dev:%d the database file!\n",
			sProcRec[pnum].NameID,sProcRec[pnum].Ip,name_buf,point.stn_id,point.dev_id);
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

		lanbcdata.status=status+0x1100;	/*communication channel status: 0:normal; 1:Fail */
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

#ifdef SUN
		Swap2Byte(&lanbcdata.number);
		Swap2Byte(&lanbcdata.point_id);
		Swap2Byte(&lanbcdata.status);
		Swap2Byte(&lanbcdata.ms);
		Swap2Byte(&lanbcdata.year);
		Swap2Byte(&lanhd.length);
#endif
		lanhd.length=sizeof(SYS_MSG);
		lanhd.dp_type=BJ_DB_TIME;

		lanhd.dest_id =0;
		lanhd.src_id =lanbcdata.host_id;
		lanhd.dest_stn =lanbcdata.stn_id;
		lanhd.packet_num =0;
		lan_out ((char *)&lanbcdata,lanhd,(unsigned char)0);

		pnt = dbentry.point;
		new_value.ind.state = status;
		update_time.ms		= 0;
		update_time.second	= tmpt.tm_sec;
		update_time.minute	= tmpt.tm_min;
		update_time.hour	= tmpt.tm_hour;
		update_time.day		= tmpt.tm_mday;
		update_time.month	= tmpt.tm_mon+1;
		update_time.year	= tmpt.tm_year+1900;
		rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;

		if(SetPtValueIostById(&pnt, new_value, iost, update_time, rw_flag) == -1)
			printf ( "\nIEC104: %d-%d-%d-%d No this point in the database when Wdb!\n",
				pnt.stn_id, pnt.dev_id, pnt.data_type, pnt.pt_id);

	}
	return(TRUE);
}

void WriteDspInfo(char *buf)
{
	int	bufptr,len,charlen;

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
	}
}





  

