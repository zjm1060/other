
#include		"../inc/dl476_std.h" 
#include		"../inc/dl476_data.h" 


#ifdef MSWIN32
WORD   		wvr;
WSADATA WSAData; 
HANDLE	hMapObject;
#else 
void 	sig_dl476(int); 
void 	sig_dl476pipe(int);
int	shm_rec_id; 
pid_t	pid,tmppid;
FILE	*fp; 
char	tmp[600];
#endif  
int	childflag; 

short		almptr;
SOCKET		clientsock,serversock;
char		old_day;

//adde by wayne
extern char zLogText[200];

void main()															
{
#ifdef MSWIN32
	wvr=MAKEWORD(2,2);
	
	if (WSAStartup(wvr, &WSAData) != 0)
	{
		printf("dl476-92 client exit\n"); 
		return;
	}  
#endif 	

	/*数据库共享*/
	/*MountDB();*/
	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
	
	/*报警共享区初始化*/
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		printf("DL476_client:: can't attach  rec shared memory");
		return;
	}	
	
	/*Lan_init();*/
	if(lan_out_init() == FALSE)
	{
		printf("\nFail to lan_out_init()!");
		exit(-1);
	}

	Dsend_errnum=0;
	connect_errnum=0;
	dev_idtmpforcom=0;
	data_typeforcom=0;
	point_idforcom=0;
	statusforcom=0;
	/*数据文件名*/ 
	if(GenTableConf()==FALSE)	return; 

	/*基本数据表*/
	if(GenDATAIndex()==FALSE)	return;
	
	/*开关量控制表*/ 
	if(GenSWCTRLIndex()==FALSE)	return;
	
	/*生成节点定义表*/
	if(GenNodeTable()==FALSE) 
	{
		for(;;)
		{
#ifdef MSWIN32
			Sleep(1000000);
#else
			sleep(1000);
#endif
		}
	}	

#ifndef MSWIN32
	signal(SIGTERM,sig_dl476);
	signal(SIGINT,sig_dl476);
#endif
	
	ClientSend();

}

int ClientSend(void)
{
	unsigned short 	ret,len,k;
	struct tm       timeptr;
	time_t          t;
	FILE		*fp;
	short		hb;
	int allsend_count=0;

#ifndef MSWIN32	
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM,sig_dl476);
	signal(SIGINT,sig_dl476);
	signal(SIGKILL,sig_dl476); 
	signal(SIGPIPE,sig_dl476pipe); 
#endif	
	/*数据包初始化*/
	DatapduIntial();
	
	/*数据区初始化*/
	ReadOnlineDB();

	StoreRealValue();

	clientsock = INVALID_SOCKET;
	
	t=time(NULL);
	timeptr=*localtime(&t);
	old_day=timeptr.tm_mday;

	almptr = shm_rec_addr->head.header_ptr;
	
	/*added 2005.6.10*/
	fp=fopen("/home/ems/h9000/tmp/HotRnode.dat","rb");
	if(fp==NULL)
	{
		hot_rnode=0;
		printf("can't fopen(rb) /home/ems/h9000/tmp/HotRnode.dat\n");
	}
	else
	{
		fread(&hot_rnode,sizeof(short),1,fp);
		printf("=======hot_rnode %d read in file-----------------\n",hot_rnode);

		if(hot_rnode>=rnode.number)	hot_rnode=0;  /*added 2005.12.28*/
		
		fclose(fp);
	}

	printf("------- now    hot_rnode =%d   ----------\n",hot_rnode);

	/*ended 2005.6.10*/
	
	
	for(;;)
	{
#ifdef PRTLOG
		t=time(NULL);
		timeptr=*localtime(&t);
		sprintf(strTemp1,"/home/ems/h9000/dl476-92/dl476-92_%04d-%02d-%02d_log.txt" ,
			timeptr.tm_year+1900, timeptr.tm_mon+1,  timeptr.tm_mday);
#endif	

#ifdef MSWIN32
		Sleep(1000);
#else
		//sleep(1);
		usleep(200*1000);//modifyed by wayne
#endif 

		if(Dsend_errnum<0)
		{
			Dsend_errnum=0;
			//sleep(1);
		}
		else if(Dsend_errnum<=20)
		{
			//sleep(2);
		}
		else if(Dsend_errnum>20)
		{
			printf("\n ******%d times can not connect,conmunication waiting...*******",Dsend_errnum);
			//sleep(60);
		}
		if(Dsend_errnum>=100)
		{
			printf("\n ******20 times can not connect,conmunication waiting...*******");
			//sleep(600);
			Dsend_errnum=0;
		} 
		t=time(NULL);
		timeptr=*localtime(&t);
		
		if (old_day !=timeptr.tm_mday)
		{
			old_day=timeptr.tm_mday;
			/*	almptr = 0;*/
			printf("\n-------alarm set zero--------\n");
		}
		
		if(GetWanCtrlPrvg()!=CUR_WS_IN_HOST)
		{	
			almptr = shm_rec_addr->head.header_ptr;
			printf("\n本主机非主控状态！！！！！！！\n");			
			ReadOnlineDB();
			
			StoreRealValue();
			
			if(clientsock != INVALID_SOCKET)	
				Do_Disconnect(clientsock);
			
			continue;
		}
		
		/* 报警区*/

		ret=ReadAlmBuf();

		len=datapdu.head.length;    

		Swap2Byte(&len);

		hb=0;
		
		if(len !=0)
		{
			printf("报警区:: ptr=%d len=%d\n",ret,len);
			
			hb=1;
			
			clientsock=TcpSend(clientsock,datapdu);
			
			if(clientsock!=INVALID_SOCKET)		 
			{	 
				almptr=ret;
				datapdu.head.ns++; 
			}
		}
		
		/* 摹拟量*/

		ReadOnlineDB();
		printf("@@@@@@@@@@@@@@before ChangeRealPackage send!\n");//added by wayne
		for(k=0;k<MAX_TABLE_NUM;k++)
			ChangedRealPackage(k);
		printf("@@@@@@@@@@@@@end ChangedRealPackage send!\n");//added by wayne
		
		/*全送*/
		if(allsend_count==0)
		{
		printf("@@@@@@@@@@@@@@before all send!\n");//added by wayne
			for(k=0;k<MAX_TABLE_NUM;k++)
				AllPackage(k); 
		printf("@@@@@@@@@@@@@end all send!\n");//added by wayne
		}

		allsend_count= (allsend_count+1)%120; //120
		printf("\ndl476_client_send: allsend_count= %d", allsend_count);
		
	} 
	
	return(0);
}


#ifndef MSWIN32
void sig_dl476(int signo)
{ 	
	
	/*shutdown(clientsock,2);*/
	close(clientsock);
	
	printf("dl476_client_send recv signal %d, exit...\n", signo);

	exit(0);
	
}

void sig_dl476pipe(int signo)
{
	int 			type;

	DMS_COMMON      common_aa;

	char			name[30];

	FILE			*fp;

	
	printf ("dl476_client_send.c::  pipe broken !!!!!!!!!!! \n");
	
	strcpy(name,DL476_SYS_TAB_NAME);

	strcat(name,"RC_LINK_STA");

	/*ReadEntryByName(name,&type,&common_aa);
	if ( common_aa.err_code == -1 )*/
	if(ReadEntryByNameStr(name,&common_aa)==-1)
	{
		printf (" %s  not in Database!\n",name);
	}
	else
	{
		/*	if (common_aa.entry.ind.variable.value.status==0)
		{		
			Wdb(name,4);

			BdcastMsg(SYS_RTU,DPS_SYSMSG,common_aa.entry.ind.variable.ptr.point_id,4);
		}*/
		UpdateChannelFlag(1);
	}
	
	/*shutdown(clientsock,2);*/
	sprintf(zLogText,"\nin sig_dl476pipe:before close at line:%d",__LINE__);
	TraceLog(strTemp1,zLogText);
	PrintSockInfo(clientsock);

	//close(clientsock);
	Do_Disconnect(clientsock);
	//clientsock = INVALID_SOCKET;//added by wayne 20101231
	//sleep(300);//added by wayne
	//exit(0);
}
#endif


