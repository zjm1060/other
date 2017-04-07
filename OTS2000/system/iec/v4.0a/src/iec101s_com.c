/****************************************************************
*  FILE NAME:           iec101s_com.c      			*
*  FUNCTION:            Controlled Station Main Process		*
*			Link Layer Communication Program	*
*  NAME :  			              			*
*  DATE:                Apr,2006                		*
*								*
****************************************************************/
#include "../inc/iec101s_com.h"
#include "../../../dps/v4.0a/inc/dps_functions_def.h"

#define	DEBUG


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

HANDLE	hPort_com;

IEC101S_DSP_BUF *shm_buf_addr= (IEC101S_DSP_BUF *)-1;
int	shm_id_buf;
FILE	*dsp_info_file;
int		ouput_info_filter= OUTPUT_ERROR;

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
CSBUF		csbuf;

int	fcb,acd,dfc,coi;
int	comdflag,pollflag,cmdterm;
int	Readflag,ResetProcessflag,localinitflag;
int	Paramflag,MirrorNAckflag;
int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
int	interrogationflag,counterflag,icgroupflag,cigroupflag;
int	delayflag;
int	cyclicflag,scanflag;
int link_normal_flag;

unsigned short	delayalarm;
unsigned char	cot_interrogation,cot_counter;
unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
unsigned int	repeatnum;
unsigned short	seladdr;


unsigned short	nStnId;
unsigned short	nASDUId;
unsigned char	cmd_src;
int		nBaudRate;

int		fixframe_len;
int		varframe_min_len;
int linkaddr_length;
int cmmaddr_length;
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

int		local_stn_id;
char local_hostname[36], pair_hostname[36];

#ifdef REC_YF_AGC_CMD
extern	unsigned short	daysaved;
#endif

void ResetOrgData(void);
void InitFlag(void);

typedef	struct
{
	UCHAR			type_id;
	UCHAR			stn_id;
	UCHAR			dev_id;
	UCHAR			data_type;
	USHORT			pt_num;
	USHORT			pt_id;
	ANLG_STATUS		ana_status;
	char			data[4];
	UCHAR			host_id;
    UCHAR			second;
    UCHAR			minute;
	UCHAR			hour;
	UCHAR			day;
	UCHAR			month;
	USHORT			year;
}SP_ANA_INFO_PKG1;

void GetSysShortTimeStr1( char *timeStr)
{
	DATE_TIME	date_time;
	
	GetSysTime(&date_time);
	sprintf(timeStr, "%02d:%02d:%02d.%03d", date_time.hour,
		date_time.minute, date_time.second, date_time.ms);
}

int BroadCastAnaPt1(POINTER point, UCHAR net, float value, ANLG_STATUS status, DATE_TIME *date_time)
{
	LAN_HEAD		head;
	SP_ANA_INFO_PKG1	info;
	UCHAR			hostId;
	GULONG			data;
	
	head.length= sizeof(SP_ANA_INFO_PKG1);
	Swap2Byte(&head.length);
	head.dp_type= BJ_DB_TIME;
	head.dest_id= 0;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;
	head.dest_stn= GetModuleId();
	
	if(status.alm_type)
		info.type_id= DPS_ANA_ALM;
	else
		info.type_id= DPS_ANALOG;
	info.stn_id= point.stn_id;
	info.dev_id= point.dev_id;
	info.data_type= point.data_type;
	info.pt_num= 1;
	Swap2Byte(&info.pt_num);
	info.pt_id= point.pt_id;
	Swap2Byte(&info.pt_id);
	memcpy(&info.ana_status, &status, 2);
	Swap2Byte(&info.ana_status);
	data= (GULONG)(value * 1000.);
	memcpy(info.data, &data, 4);
	Swap4Byte(&info.data);
	GetLocalHostId(&hostId);
	info.host_id= (char)hostId;
	info.second= date_time->second;
	info.minute= date_time->minute;
	info.hour= date_time->hour;
	info.day= date_time->day;
	info.month= date_time->month;
	info.year= date_time->year;
	Swap2Byte(&info.year);
	
	lan_out((char*)&info, head, net);
	return 0;
}


                                                                
void MainExitRoutine ( DWORD signal_type )
{
	UINT	uExitCode= 0;
	
	switch ( signal_type )
	{
	case CTRL_C_EVENT:
		OutputDspInfo(0,  "\n!!! CTRL_C_EVENT received !!! \n" ); 
		break;
		
	case CTRL_BREAK_EVENT:
		OutputDspInfo(0,  "\n!!! CTRL_BREAK_EVENT received !!! \n" ); 
		break;
		
	case CTRL_CLOSE_EVENT:
		OutputDspInfo(0, "\n!!! CTRL_CLOSE_EVENT received !!! \n" ); 
		break;
		
	case CTRL_LOGOFF_EVENT:
		OutputDspInfo(0, "\n!!! CTRL_LOGOFF_EVENT received !!! \n" ); 
		break;
		
	case CTRL_SHUTDOWN_EVENT:
		OutputDspInfo(0,  "\n!!! CTRL_SHUTDOWN_EVENT received !!! \n" ); 
		break;
		
	default:
		break;
	}
	
	UpdateChannelFlag(0,(unsigned short)1);

	OutputDspInfo(0, "\n !!!iec101s_com 通信进程 Terminate Now!!! \n");
	if(dsp_info_file)
		fclose(dsp_info_file);
	exit (0 );
}


BOOL InitHostName(void)
{
	gethostname(local_hostname, sizeof(local_hostname));
	if(Dualhost)
	{
		if(-1==GetDhswHostName(pair_hostname))
			return FALSE;
		else
			return TRUE;
	}
	return TRUE;
}

int GetFixFrameLength(void)
{
	return 4+linkaddr_length;
}

int GetVarFrameMinLength(void)
{
	return 12+linkaddr_length+cmmaddr_length;
}

void InitOutputFilter(void)
{
	char dsp_filename[256];
	
	ouput_info_filter= OUTPUT_ERROR;
	sprintf(dsp_filename, "%sIEC101S_LOG.def", IEC101_HOMEPATH);
	if(dsp_info_file= fopen(dsp_filename, "r"))
	{
		char	buff[256];
		
		while(fgets(buff,256,dsp_info_file))
		{
			if(buff[0]=='!' || buff[0]=='#')
				continue;
			
			if(1!=sscanf(buff, "%d", &ouput_info_filter))
				continue;
			break;
		}
		
		fclose(dsp_info_file);
	}
	
	sprintf(dsp_filename, "%sIEC101S_COM_LOG.txt", IEC101_LOGPATH);
	dsp_info_file= fopen(dsp_filename, "w");
}

void main(int argc,char *argv[])
{
	HANDLE	hThread_SOE;
	int		ThreadID_SOE;


	InitOutputFilter("IEC101S", 0);

	if ( SetConsoleCtrlHandler ((void *)MainExitRoutine,TRUE ) == 0 )
	{	
		OutputDspInfo(0, "SetConsoleCtrlHandler ERROR %d!!! \n",GetLastError());
	}

	for(;;)
	{
		if( ReadStnDef(STN_DEF_FILE) == TRUE )
			break;
            	
		OutputDspInfo(0, "\n###ALARM!!! IEC101s::Read Station define file %s Error!\n",STN_DEF_FILE);
		sleep(5);
	}

	if(-1==MountDB(NULL))
	{
		OutputDspInfo(0, "\nFail to MountDB!");
		exit(-1);
	}

	if(lan_out_init() == FALSE)
	{
		OutputDspInfo(0, "\nFail to lan_out_init()!");
		exit(-1);
	}
	if(-1==InterLockFileLoad())
	{
		OutputDspInfo(0, "Fail to Load InterLock File!\n");
		exit(-1);
	}


// 	signal(SIGTERM,Endproc);
// 	signal(SIGINT,Endproc);

	local_stn_id = 0;
	InitConfig(sProcRec[0].NameID);
	UpdateChannelFlag(local_stn_id,(unsigned short)1);

	if(!InitHostName())
	{
		OutputDspInfo(0, "need dual host define");
		exit(0);
	}

	hThread_SOE = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)IEC101_SOE,
                                 NULL, CREATE_SUSPENDED, &ThreadID_SOE);
	if (hThread_SOE == NULL)
		OutputDspInfo(0, "Error in CreateThread ThreadID_SOE:%d\n",GetLastError());
	else
		OutputDspInfo(0, "hThread_SOE: %d OK!\n",hThread_SOE);

	SetThreadPriority(hThread_SOE,THREAD_PRIORITY_BELOW_NORMAL);	 

	ResumeThread( hThread_SOE);

	fixframe_len=GetFixFrameLength();
	varframe_min_len=GetVarFrameMinLength();
	
	Com_Link(local_stn_id);

	MainExitRoutine(0);
}



int GetNackSoeTimePtId(POINTER *point)
{
	char	name_buf[128];

	if(0!=GetHostStnId(local_hostname, &point->stn_id))
	{
		OutputDspInfo(0, "GetHostStnId() %s No this hostname point in database file!\n",local_hostname);
		return -1;
	}
	
	if(0!=GetHostSysLcuId(local_hostname, &point->dev_id))
	{
		OutputDspInfo(0, "GetHostSysLcuId() %s No this hostname point in database file!\n",local_hostname);
		return -1;
	}
	
	point->data_type= ANA_TYPE;
	
	strcpy(name_buf,sProcRec[local_stn_id].NameID);
	strcat(name_buf,"_SOETIME");
	if(-1==GetEntryIdByName(name_buf, point))
	{
		OutputDspInfo(0, "%s %s %s No this com channel status point in stn:%d dev:%d the database file!\n",
			sProcRec[local_stn_id].NameID,sProcRec[local_stn_id].Ip,name_buf,point->stn_id,point->dev_id);
		return -1;
	}
	
	return 0;
}


void UpdateSoeAck(void)
{
	POINTER		point;

	if(!GetNackSoeTimePtId(&point))
	{
		USHORT	sendpnt;
		ANLG_STATUS	status;
		
		memset(&status, 0, sizeof(status));
		sendpnt= (soebuf.sendpnt+SOE_NUM-1)%SOE_NUM;
		BroadCastAnaPt1(point, 0, (float)soebuf.soerec[sendpnt].time.ms, status, &soebuf.soerec[sendpnt].time);
		OutputDspInfo(0, "UpdateSoeAck: soe time ms=%d", soebuf.soerec[sendpnt].time.ms);
	}
	soebuf.count -= (soebuf.sendpnt+SOE_NUM-soebuf.ackpnt)%SOE_NUM;
	soebuf.ackpnt= soebuf.sendpnt;
	OutputDspInfo(0, "UpdateSoeAck: head=%d send=%d ack=%d count=%d", 
		soebuf.bufpnt, soebuf.sendpnt, soebuf.ackpnt, soebuf.count);
}

void ResetSoeSendPtr(void)
{
	POINTER		point;
	USHORT		bufpnt;
	DATE_TIME	date_time;
	time_t		nack_time_sec;
	int			nack_time_ms;
	float		fValue;
	unsigned short count;
	
	OutputDspInfo(0, "Reset soe send ptr because get ctrl priv");
	OutputDspInfo(0, "ResetSoeSendPtr begin: head=%d send=%d ack=%d count=%d", 
		soebuf.bufpnt, soebuf.sendpnt, soebuf.ackpnt, soebuf.count);
	
	if(soebuf.count<=0)
		return;
	
	if(GetNackSoeTimePtId(&point))
		return;

	GetPtUpdateTimeById(&point, &date_time);
	nack_time_sec= GetAbsTimeFromDateTime(&date_time);
	if(nack_time_sec==-1)
		return;

	GetAnaPtfValueById(&point, &fValue);
	nack_time_ms= (int)fValue;

	count= soebuf.count;
	bufpnt= (soebuf.bufpnt+SOE_NUM-count)%SOE_NUM; 

	while(count>0)
	{
		int soe_ms= soebuf.soerec[bufpnt].time.ms;
		time_t	soe_time;
		
		soe_time= GetAbsTimeFromDateTime(&soebuf.soerec[bufpnt].time);
		if(soe_time==-1)
			break;

		if(soe_time > nack_time_sec) //SOE时间大于确认SOE时间
			break;
		if((soe_time == nack_time_sec) && (soe_ms >= nack_time_ms)) //SOE时间大于或等于确认SOE时间
			break;
		
		count--;

		bufpnt= (bufpnt+1)%SOE_NUM;
		
	}

	soebuf.ackpnt= soebuf.sendpnt= bufpnt;	
	soebuf.count= (soebuf.bufpnt+SOE_NUM-soebuf.sendpnt)%SOE_NUM;

	OutputDspInfo(0, "ResetSoeSendPtr end: head=%d send=%d ack=%d count=%d", 
		soebuf.bufpnt, soebuf.sendpnt, soebuf.ackpnt, soebuf.count);
}

void TransformRecvFrame(char *recvbuf, char *data_buff, int *dwByteRecvd, USHORT *recv_cmmaddr)
{
	int	frame_len= *dwByteRecvd;

	memset(data_buff,0,CMD_DATA_SIZE);
	
	if(linkaddr_length==1 && cmmaddr_length==1)
	{
		memcpy(&data_buff[2], &recvbuf[6], frame_len-8); //ASDU len= frame_len-8
		*recv_cmmaddr= recvbuf[9];
		return;
	}
	
	if(linkaddr_length==2 && cmmaddr_length==1)
	{
		memcpy(&data_buff[2], &recvbuf[7], frame_len-9); //ASDU len= frame_len-9
		*recv_cmmaddr= recvbuf[10];
		*dwByteRecvd= frame_len-1;
		return;
	}
	else if(linkaddr_length==1 && cmmaddr_length==2)
	{
		memcpy(&data_buff[2], &recvbuf[6], 3); 
		memcpy(recv_cmmaddr, &recvbuf[9], 2);
		Swap2Byte(recv_cmmaddr);
		memcpy(&data_buff[6], &recvbuf[11], frame_len-13); //info len= frame_len-13
		*dwByteRecvd= frame_len-1;
		return;
	}
	else if(linkaddr_length==2 && cmmaddr_length==2)
	{
		memcpy(&data_buff[2], &recvbuf[7], 3); 
		memcpy(recv_cmmaddr, &recvbuf[10], 2);
		Swap2Byte(recv_cmmaddr);
		memcpy(&data_buff[6], &recvbuf[12], frame_len-14); //info len= frame_len-14
		*dwByteRecvd= frame_len-2;
		return;		
	}
	return;
}

void SendSwitchCmd(void)
{
	extern int byte_to_send;
	
	byte_to_send= 10;
	memset(sendbuf, 0, byte_to_send);
	SendData();
}

int Com_Link(int stn_id)
{				  	
	int		dwByteRecvd;
	BOOL	fSuccess;

	unsigned int	recvbufheadpt;
	unsigned char	chksum,ctrlfield,ctrlfunc,framelength;
	unsigned int	i;
	int		ctrl_prvg_state;

	time_t	tim, timeout_tim;
	struct tm *timeptr;
	USHORT	recv_cmmaddr;

	localinitflag = 1;
	link_normal_flag= 0;

	hPort_com=ComInit(sProcRec[stn_id].Ip,nBaudRate,8,EVENPARITY,ONESTOPBIT);
	if(INVALID_HANDLE_VALUE==hPort_com)
	{
		OutputDspInfo(0, "Fail to init com port: %s", sProcRec[stn_id].Ip);
		return 0;
	}

	OutputDspInfo(0, "\nopen %s BaudRate= %d init compelete fd=%d!!!\n\n",sProcRec[stn_id].Ip,nBaudRate,hPort_com);

	/* IEC 870-5 Link layer communication service */

	tim=time(&tim);
#ifdef REC_YF_AGC_CMD
	timeptr=localtime(&tim);
	daysaved = timeptr->tm_mday;
#endif

	ResetOrgData();
	scan_tim = cycle_tim = timeout_tim= tim;

	if(GetCtrlPrvg(stn_id))
		ctrl_prvg_state= 1;
	else
		ctrl_prvg_state= 0;

	for(;;)
	{
		/*Read from Interface Communication port */
		fSuccess = ReadFile(hPort_com,(LPSTR)recvbuf,VARFRAMELEN,&dwByteRecvd,NULL);
		if(!fSuccess)
		{
			OutputDspInfo(0, "ReceiveData: ReadFile ERRNO= %d \n",GetLastError());
			sleep(1);
			continue;
		}

		tim=time(&tim);
		if(dwByteRecvd == 0)
		{
			if((tim- timeout_tim)>=30)
			{
				OutputDspInfo(OUTPUT_WARN, "ReceiveData: 30 seconds timeout!");
				UpdateChannelFlag(stn_id,(unsigned short)1);
				timeout_tim= tim;
				link_normal_flag= 0;
			}
			continue;
		}
		timeout_tim= tim;

		PrintRecvData(dwByteRecvd);

		OutputDspInfo(0, "soebuf: head=%d send=%d ack=%d count=%d", 
			soebuf.bufpnt, soebuf.sendpnt, soebuf.ackpnt, soebuf.count);

		if(!GetCtrlPrvg(stn_id))
		{
			OutputDspInfo(OUTPUT_INFO, "no ctrl priv!");
			ctrl_prvg_state= 0;
			link_normal_flag= 0;
			scan_tim = cycle_tim = timeout_tim= tim;
			continue;
		}
		else
		{
			OutputDspInfo(OUTPUT_INFO, "have ctrl priv!");
			if(ctrl_prvg_state==0)
			{
				link_normal_flag= 0;
				SendSwitchCmd();
				ResetSoeSendPtr();
				ResetOrgData();
			}
			ctrl_prvg_state= 1;
		}


// 		if(!cyclicflag)
// 		{
// 			if((tim - cycle_tim) > cycle_period)
// 			{
// 				cyclicflag = 1;
// 				m_cCyclicGrpNum = 0;
// 				cycle_tim = tim;
// 			}
// 		}
// 		else
// 			cycle_tim = tim;


// 		if((!scanflag)&&(!interrogationflag))
// 		{
// 			if((tim - scan_tim) > scan_period)
// 			{
// 				m_cScanGrpNum = 0;
// 				scanflag = 1;
// 			}
// 		}
// 		else
// 			scan_tim = tim;

		if((cmdbuf.selected == 1) && ((tim-cmdbuf.sel_tim)>cmd_period))
		{
			cmdbuf.selected= 0;
			OutputDspInfo(OUTPUT_ERROR, "Wait execute command timeout, selected command cancel\n");
		}


		switch (recvbuf[0])
		{
		case 0x10: /*fix frame */

			if(linkaddr_length==1)
			{
				if((dwByteRecvd<fixframe_len)||(recvbuf[4]!=0x16)||(dwByteRecvd>2*fixframe_len))
				{
					OutputDspInfo(OUTPUT_ERROR, "Frame recv error for fix length frame\n");
					break;
				}
			}
			else if(linkaddr_length==2)
			{
				if((dwByteRecvd<fixframe_len)||(recvbuf[5]!=0x16)||(dwByteRecvd>=2*fixframe_len))
				{
					OutputDspInfo(OUTPUT_ERROR, "Frame recv error for fix length frame\n");
					break;
				}
			}

			if(linkaddr_length==1)
			{
				chksum = recvbuf[1] + recvbuf[2];	
				
				if(chksum != recvbuf[3])
				{
					OutputDspInfo(OUTPUT_ERROR, "Frame check sum error for fix length frame\n");
					break;
				}
				
				if(((UCHAR)nStnId != recvbuf[2])&&(recvbuf[2] != BROADCAST_LINKADRESS1))
				{
					OutputDspInfo(OUTPUT_ERROR, "Station No %d error! Not %d\n",recvbuf[2],nStnId);
					break;
				}
			}
			else if(linkaddr_length==2)
			{
				USHORT	linkaddr;

				chksum = recvbuf[1] + recvbuf[2] + recvbuf[3];	
				
				if(chksum != recvbuf[4])
				{
					OutputDspInfo(OUTPUT_ERROR, "Frame check sum error for fix length frame\n");
					break;
				}
				
				memcpy(&linkaddr, &recvbuf[2], 2);
				Swap2Byte(&linkaddr);
				if((nStnId != linkaddr)&&(linkaddr != BROADCAST_LINKADRESS2))
				{
					OutputDspInfo(OUTPUT_ERROR, "Station No %d error! Not %d\n",linkaddr,nStnId);
					break;
				}
			}

			UpdateChannelFlag(stn_id,(unsigned char)0);

			ctrlfield = recvbuf[1] & 0x50;
			ctrlfunc = recvbuf[1] & 0x0F;
		
			switch(ctrlfunc)
			{
			case 0:	/*Reset link status frame */

				if(ctrlfield != 0x40)
				{
					OutputDspInfo(0, "Reset link status frame control field received wrong!\n");
					break;
				}
				OutputDspInfo(0, "Reset link status frame received!\n");

				InitFlag(); //fcb = 0;
				soebuf.sendpnt= soebuf.ackpnt;
				link_normal_flag= 1;

				if(AckCmd())
					OutputDspInfo(OUTPUT_ERROR, "ACK Reset link status frame wrong!\n");
				break;

			case 1:	/*Reset process frame */
				
				if(ctrlfield != 0x40)
				{
					OutputDspInfo(OUTPUT_ERROR, "Reset process frame control field received wrong!\n");
					break;
				}
				OutputDspInfo(0, "Reset process frame received!\n");
				
				InitFlag(); //fcb = 0;
				
				if(AckCmd())
					OutputDspInfo(OUTPUT_ERROR, "ACK Reset process frame wrong!\n");
				break;
				
			case 8:	/*Request demand bit frame or Quick check frame(not used) */
				if(ctrlfield != 0x40)
				{
					OutputDspInfo(OUTPUT_ERROR, "Request demand bit frame control field received wrong!\n");
					break;
				}
				OutputDspInfo(0, "Request demand bit frame received!\n");

				if(RespondLinkStatus())
					OutputDspInfo(OUTPUT_ERROR, "ACK Request demand bit frame wrong!\n");
				break;

			case 9:	/*Request Link status frame */
				if(ctrlfield != 0x40)
				{
					OutputDspInfo(OUTPUT_ERROR, "Request Link status frame control field received wrong!\n");
					break;
				}
				OutputDspInfo(0, "Request Link status frame received!\n");

				if(RespondLinkStatus())
					OutputDspInfo(OUTPUT_ERROR, "ACK Request Link status frame wrong!\n");
				break;

			case 10:	/*Request data class 1 */
				if(ctrlfield != 0x50)
				{
					OutputDspInfo(OUTPUT_ERROR, "Request data class 1 Control field received wrong!\n");
					break;
				}
				OutputDspInfo(0, "Request data class 1 frame received!\n");

				if(link_normal_flag==0)
				{
					OutputDspInfo(OUTPUT_ERROR, "link status is abnormal, discard this frame");
					break;
				}

				if((int)((recvbuf[1]>>5) & 0x01) == fcb)
				{
					OutputDspInfo(OUTPUT_WARN, "Repeat fcb received! Previous class 1 frame send repeated!\n");
					RepeatTransmit();
					break;
				}
				fcb = !fcb;
				UpdateSoeAck();

				if(localinitflag == 1)
				{
					acd = 0;
					coi= 0;
					EndofInitialization();
					localinitflag = 0;
					fcb = (int)(recvbuf[1] & 0x20);
					break;
				}

				RespondUserDataClass1();

				if(rpbuf.ack_flag==0 && ResetProcessflag==1)
					MainExitRoutine(0);

				break;
				
			case 11:	/*Request data class 2 */
				if(ctrlfield != 0x50)
				{
					OutputDspInfo(OUTPUT_ERROR, "Request data class 2 Control field received wrong!\n");
					break;
				}
				OutputDspInfo(0, "Request data class 2 frame received!\n");

				if(link_normal_flag==0)
				{
					OutputDspInfo(OUTPUT_ERROR, "link status is abnormal, discard this frame");
					break;
				}
				
				if((int)((recvbuf[1]>>5) & 0x01) == fcb)
				{
					OutputDspInfo(OUTPUT_WARN, "Repeat fcb received! Previous class 2 frame send repeated!\n");
					RepeatTransmit();
					break;
				}
				fcb = !fcb;
				UpdateSoeAck();

				if((localinitflag == 1)||(ResetProcessflag == 1))
				{
					acd = 1;
					NoRequestData();
					break;
				}
				RespondUserDataClass2();
				break;

			default :
				OutputDspInfo(OUTPUT_WARN, "unsupported function code %2x\n",ctrlfunc);
				NAckCmd();
				break;
			}/*switch(ctrlfunc) */

			break;

		case 0x68:	/*variable frame for send or request data */

			if((dwByteRecvd<varframe_min_len)||(dwByteRecvd < (recvbuf[1]+6)))
			{
				OutputDspInfo(OUTPUT_ERROR, "Byte number received=%d less than min variable frame length\n",dwByteRecvd);
				break;
			}
		
			if((recvbuf[3] != 0x68)||(recvbuf[1] != recvbuf[2])||(recvbuf[dwByteRecvd - 1] != 0x16))
			{
				OutputDspInfo(OUTPUT_ERROR, "Frame check error for variable length frame\n");
				break;
			}

			framelength = recvbuf[1]+4;
			chksum = 0;
			for(i=4;i<framelength;i++)
				chksum = chksum + recvbuf[i];	

			if(chksum != recvbuf[framelength])
			{
				OutputDspInfo(OUTPUT_ERROR, "Frame check sum %02X %02Xerror for variable length frame\n",chksum,recvbuf[framelength]);
				break;
			}

			if(linkaddr_length==1)
			{
				if(((UCHAR)nStnId != recvbuf[5])&&(recvbuf[5] != BROADCAST_LINKADRESS1))
				{
					OutputDspInfo(OUTPUT_ERROR, "Station No %d error! Not %d\n",recvbuf[5],nStnId);
					break;
				}
			}
			else if(linkaddr_length==2)
			{
				USHORT	linkaddr;
				
				memcpy(&linkaddr, &recvbuf[5], 2);
				Swap2Byte(&linkaddr);
				if((nStnId != linkaddr)&&(linkaddr != BROADCAST_LINKADRESS2))
				{
					OutputDspInfo(OUTPUT_ERROR, "Station No %d error! Not %d\n",linkaddr,nStnId);
					break;
				}
			}

			if(link_normal_flag==0)
			{
				OutputDspInfo(OUTPUT_ERROR, "link status is abnormal, discard this frame");
				break;
			}
			
			ctrlfunc = recvbuf[4] & 0x0F;
			ctrlfield = recvbuf[4] & 0xD0;
	
			switch(ctrlfunc)
			{
			case 3:	/*Send/confirm command frame */
				if(ctrlfield != 0x50)
				{
					OutputDspInfo(OUTPUT_ERROR, "Command send/confirm frame control field received wrong!\n");
					break;
				}
				OutputDspInfo(0, "Command send/confirm frame %d received!\n",recvbuf[6]);

				if((int)((recvbuf[4]>>5) & 0x01) == fcb)
				{
					OutputDspInfo(OUTPUT_WARN, "!!!+++Repeat fcb Command %d received! former frame will be send repeated!\n",recvbuf[6]);
					RepeatTransmit();
					break;
				}

				fcb = !fcb;
				UpdateSoeAck();

				AckCmd();	/*Link Lay ACK with ACD=1 */

				recvbufheadpt = recv_data.pnt;

				TransformRecvFrame(recvbuf, recv_data.data[recvbufheadpt], &dwByteRecvd, &recv_cmmaddr);
				recv_data.length[recvbufheadpt] =(unsigned short) (dwByteRecvd - 6);

				IEC870_5_Recv(stn_id,recvbufheadpt,recv_cmmaddr);

				recvbufheadpt ++;
				if(recvbufheadpt >= CMD_ENTRY_NUMBER)
					recvbufheadpt = 0;
				recv_data.pnt = recvbufheadpt;
				break;


			default :
				OutputDspInfo(OUTPUT_ERROR, "Error link control function code %2x\n",ctrlfunc);
				NAckCmd();
				break;
			}/*switch(ctrlfunc) */
			break;

		default :
			OutputDspInfo(OUTPUT_ERROR, "IEC received data start byte error! %2x :\n",recvbuf[0]);
			break;			

		}/*switch(recvbuf[0]) */
		
	}/*for(;;) */
}

BOOL GetCtrlPrvg(int stn_id)
{
	char	entry_name[256];
	POINTER		point;
	USHORT		comm_state, host_state, pair_state;
	
	if(!Dualhost)
		return TRUE;

	if(-1==GetLocalHostState(&host_state))
		return TRUE;
	if(host_state==1) //main host
	{
		if(-1==GetHostState(pair_hostname, &pair_state))
			return TRUE;
		if(pair_state==1)
			return FALSE;
		else
			return TRUE;
	}
	
	if(host_state!=2) //vice
		return FALSE;

	//vice host: check main host comm state
	if(-1==GetHostStnId(local_hostname, &point.stn_id))
		return TRUE;
	
	if(-1==GetSysLcuId(point.stn_id, &point.dev_id))
		return TRUE;
	
	point.data_type= SYSMSG_TYPE;
	
	sprintf(entry_name, "%s_%s", pair_hostname, sProcRec[stn_id].NameID);
	if(-1==GetEntryIdByName(entry_name, &point))
		return TRUE;
	
	GetIndPtStateById(&point, &comm_state);
	if(comm_state==1) //main host comm state error
		return TRUE;
	else //main host comm state OK
		return FALSE;
}


int UpdateChannelFlag(int stn_id, unsigned short status)
{
	char	name_buf[128];
	POINTER		point;
	USHORT	old_state;
	
	gethostname(local_hostname, 32);
	if(0!=GetHostStnId(local_hostname, &point.stn_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostStnId() %s No this hostname point in database file!\n",local_hostname);
		return -1;
	}
	
	if(0!=GetHostSysLcuId(local_hostname, &point.dev_id))
	{
		OutputDspInfo(OUTPUT_ERROR, "GetHostSysLcuId() %s No this hostname point in database file!\n",local_hostname);
		return -1;
	}
	
	point.data_type= SYSMSG_TYPE;
	
	sprintf(name_buf, "%s_%s", local_hostname, sProcRec[stn_id].NameID);
	if(-1==GetEntryIdByName(name_buf, &point))
	{
		OutputDspInfo(OUTPUT_ERROR, "%s %s %s No this com channel status point in stn:%d dev:%d the database file!\n",
			sProcRec[stn_id].NameID,sProcRec[stn_id].Ip,name_buf,point.stn_id,point.dev_id);
		return -1;
	}
	
	GetIndPtRtStateById(&point, &old_state);
	if(old_state != status)
	{
		WR_VAR_UNION	new_value;
		IOST		iost={0};
		DATE_TIME	update_time;
		
		BroadCastIndPtState(point, 0, status);	
		new_value.ind.rt_state= status;
		GetSysTime(&update_time);
		SetPtValueIostById(&point, new_value, iost, update_time, RW_UPDATE_TIME | RW_RT_VALUE);
	}
	return 0;
}


void OutputDspInfo(int nFilter,	// how to handle and where to report
				  // 0: default - normal flag
				  // else flags
				  const char* format,// from here the function uses printf syntax
				  ...
				  )
{
	int		bufptr,len,charlen;
    va_list ap;
    char	buf[1024], *ptr= buf;
	char	timeStr[128];
	
	if(nFilter<ouput_info_filter)
		return;

	GetSysShortTimeStr1(timeStr);
	
	ptr += sprintf(buf, "\n%s: ", timeStr);
	
    va_start(ap, format);
    ptr += vsprintf(ptr, format, ap);
	va_end(ap);
	*ptr= 0;
	
	printf("%s", buf);

	if(dsp_info_file)
	{
		fprintf(dsp_info_file, "%s", buf);
		fflush(dsp_info_file);
	}
	
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


  

