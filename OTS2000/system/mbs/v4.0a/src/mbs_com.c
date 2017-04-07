/************************************************************
*  FILE NAME:           mbs_com.C      						*
*  FUNCTION:            Controlling Station Main Process	*
*						Link Layer Communication Program	*
*  NAME :  					        						*
*  DATE:                DEC,2009                			*
*															*
************************************************************/

#define DEBUG
#include "..\inc\mbs_com.h"

int	MasterNum;
MASTER_REC	sMasterRec[MAX_MASTER_NUM+1];
MBDATA mbdata[MAX_MASTER_NUM+1];
SOCKET mainsock; 

void sig_termp();
int CreatChildThread();

int main(int argc, char *argv[])
{
	int i;
	int id;

	CreateLockFile(".mbs_com.dxe");
	if(-1==InitSysLog(H9000_MODULE_MBS, "MBS"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	
	wsa_startup();

	
	gethostname(g_hostname,30);
	//Initialization
#ifndef MSWIN32
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
#endif
	if(-1==MountDB(DB_NAME))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}


	if(lan_out_init () == FALSE)
	{
		printf("\nFail to lan_out_init!");
		exit(-1);
	}

	for(i=0;i<=MAX_MASTER_NUM;i++)
	{
		mbdata[i].newsock = INVALID_SOCKET;
		mbdata[i].err_code = 0;
	}
	
	if(ReadMbsStnDef(MBS_STN_DEF_FILE)== FALSE)
	{
		err_write("\nReadMbsStnDef error\n");

		exit(-1);
	}

	sprintf(log_str,"Start to get point def file...\n");	
	log_write(INF,1,log_str);	
	for(id=0;id<MasterNum;id++)
	{
		sMasterRec[id].valid = TRUE;
		if(GenAnaSendTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenAnaSendTable (%d) error",id);
			err_write(log_str);
			sMasterRec[id].valid = FALSE;
			continue;
		}
		if(GenAnaRecvTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenAnaRecvTable (%d) error",id);
			err_write(log_str);
			sMasterRec[id].valid = FALSE;
			continue;
		}
		if(GenPolSendTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenPolSendTable (%d) error",id);
			err_write(log_str);
			sMasterRec[id].valid = FALSE;
			continue;
		}
		
		if(GenPolRecvTable(id)== FALSE)
		{
			sprintf(log_str,"\nGenPolRecvTable (%d) error",id);
			err_write(log_str);
			sMasterRec[id].valid = FALSE;
			continue;
		}
		
		if(mbdata[id].anas.number > MAX_ANA_SEND_NUM || mbdata[id].anar.number > MAX_ANA_RECV_NUM
			|| mbdata[id].pols.number > MAX_POL_SEND_NUM || mbdata[id].polr.number > MAX_POL_RECV_NUM
			)
		{
			err_write("error: point num is too big");
			sMasterRec[id].valid = FALSE;
			continue;
		}
	}
	
	if( CreatChildThread() == FALSE)
	{
		err_write("CreateChileThread() error");
		exit(0);
	}

	mbs_comm();

	for(;;)
	{
		MySleep(2000);
	}
	sprintf(log_str,"mbs_com main process exit! \n"); 
	log_write(INF,1,log_str);
#ifdef MSWIN32
	WSACleanup();
#endif	
	UnMountDB();
}


void	mbs_comm()
{
	int clientaddrlen;
	int loop_count=0;

	clientaddrlen=sizeof(struct sockaddr_in);	
	for(;;)
	{
		MySleep(1000);
		if( CreatListenSock(&mainsock) != TRUE )
		{
			sprintf(log_str,"CreatListenSock error\n");
			err_write(log_str);
			MySleep(2000);
			continue;
		}
		MySleep(1000);
		for(;;)
		{
			if( WaitForConnect(&mainsock)!=TRUE )
			{
				MySleep(5000);
				continue;
			}
			MySleep(1000);
		}//for(;;)
	}//for(;;)
}


int CreatListenSock(int *p_sock)
{
	unsigned short mbsport;
	struct sockaddr_in	clientaddr;
	
	*p_sock = socket(AF_INET, SOCK_STREAM,0);
	
	if( *p_sock<0 )
	{ 
		sprintf(log_str,"mbs_com:: creat socket() failed!\n");
		err_write(log_str);
		return FALSE;
	}
	
	mbsport = (unsigned short)MBPORT;
	//bzero (( char*)&clientaddr, sizeof ( clientaddr ));
	clientaddr.sin_family      = AF_INET;
	clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientaddr.sin_port        = htons(mbsport);  
	
	
	if(bind(*p_sock,(struct sockaddr *)&clientaddr,sizeof(clientaddr))<0)
	{
		sprintf(log_str,"mbss_com:: bind() error: %d!\n ", errno);
		err_write(log_str);
		close(*p_sock);
		MySleep(5000);
		return FALSE;
	}
	
	if(listen(*p_sock,8)<0)
	{
		sprintf(log_str,"mbs_com::listen() error: %d!\n ", errno);
		err_write(log_str);
		close(*p_sock);
		MySleep(2000);
		return FALSE;
	}
	return TRUE;

}

int WaitForConnect(int *p_sock)
{

	struct sockaddr_in	clientaddr;
	int	clientaddrlen;
	char	rmt_ip_str[16];
	int stn_id;
	int newsock;

	sprintf(log_str,"\nmbs_com:: waiting for connection...\n");
	log_write(INF,1,log_str);	
	clientaddrlen=sizeof(struct sockaddr_in);	
	newsock=accept(*p_sock,(struct sockaddr *) &clientaddr,(socklen_t *)&clientaddrlen);
	
	if(newsock < 0)
	{
		sprintf(log_str,"\nmbs_com::accept() return %d wrong errcode =%d !\n", newsock, GetLastError());
		err_write(log_str);
		return FALSE;
		
	} 	
	strcpy(rmt_ip_str,inet_ntoa(clientaddr.sin_addr));

	for(stn_id = 0; stn_id < MasterNum; stn_id++)
	{
		if(strcmp(rmt_ip_str,sMasterRec[stn_id].Ip) == 0)
		{
			break;
		}
	}
	
	if(stn_id >= MasterNum)
	{
		sprintf(log_str,"\nmbs_com:: remote ip=%s not find! close socket!\n",rmt_ip_str,MBS_STN_DEF_FILE);
		err_write(log_str);
		close(newsock);
		return FALSE;
	}
	
	if( sMasterRec[stn_id].valid == TRUE)
	{
		if( mbdata[stn_id].newsock != INVALID_SOCKET)
		{
			sprintf(log_str,"\nmbs_com:: remote ip=%s socket exist, err!\n",rmt_ip_str);
			err_write(log_str);
			close(newsock);
			close(mbdata[stn_id].newsock);
			return FALSE;
		}
		mbdata[stn_id].newsock = newsock;		
	}
	else
	{
		close(newsock);
		return FALSE;
	}
	sprintf(log_str,"\nmbs_com:: New connection! id=%d %s ip=%s socket=%d\n",stn_id,sMasterRec[stn_id].NameID,rmt_ip_str,newsock);	
	log_write(INF,1,log_str);
#ifndef MULTITHREAD
	ThreadCommS(&stn_id);
#endif
	return TRUE;
}

/************************************************ Child Process function of each PLC ***********************************************************************************/
int CreatChildThread()
{
	int  i;
	DWORD thrid;
	HANDLE p_thrid;
	/******* Create Thread for MSWIN32  **********/

#ifndef MULTITHREAD
	MySleep(2000);		
	return TRUE;
#endif
	for( i=0; i<MasterNum; i++ )
	{
		MySleep(2000);			

		p_thrid=CreateThread(NULL,
				MAX_STACK_SIZE, 
				(LPTHREAD_START_ROUTINE)ThreadCommS, 
		 		&sMasterRec[i].stnid,
		 		0,
		 		(LPDWORD)&thrid);

		if( p_thrid !=NULL)
		{
			sprintf(log_str,"\nmbs_com.c: Create thread id= %d,thrid=%d OK",
				i,thrid);	
			log_write(INF,1,log_str);
		}
		else
		{
			sprintf(log_str,"mbs_com.c: Create thread id= %d failed", i);
			log_write(INF,1,log_str);
			return FALSE;
		}
	}
			
	return TRUE;
}


void sig_termp()
{
	int i;
	for(i=0;i<MasterNum;i++)
	{
		if(mbdata[i].newsock != INVALID_SOCKET)
		closesocket(mbdata[i].newsock);
		mbdata[i].newsock = INVALID_SOCKET;
	}
	exit(0);
}

