/***********************************************************************************

FILENAME:	lanlib.c

FUNCTION:		source code of lan.lib, common function of lan communication

VERSION:		v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

************************************************************************************/
#include "../inc/lanhead_all.h"

#include "../../../dps/v4.0a/inc/ipc_common.h"

SHM_SEND_DATA		data_send_addr, *shm_send_addr;
SHM_RECV_DATA		data_recv_addr, *shm_recv_addr;
struct shm_log		data_log,*shm_log_addr;
struct shm_hdb		data_hdb,*shm_hdb_addr;
struct shm_ots		data_ots,*shm_ots_addr;

SHM_RECORD		data_rec,*shm_rec_addr; 
struct	plc_data   	plc_data_addr,*shm_plc_data;
struct	plc_cmd		plc_cmd_addr, *shm_plc_cmd;
struct	net_cmd		net_cmd_addr, *shm_net_cmd;
struct	comm_cmd	comm_cmd_addr, *shm_comm_cmd;
struct	plc_send	plc_send_addr, *shm_plc_send;

char sms_buf[200];
int app_id = 0;
BOOL g_ctrl_dualnet = YES;
BOOL g_sys_dualnet = YES;
BOOL g_cascade = NO ;

unsigned char g_stnid =1;
int g_sys_netnum;
int g_ctrl_netnum;
short segid[MAX_STN_NUM+1][MAX_LCU_NUM+1];
unsigned char index_stn[MAX_STN_ID+1];
unsigned char id_stn[MAX_STN_NUM+1];				
int g_stn_num;
int g_dev_num[MAX_STN_NUM+1];
unsigned char g_sys_lcu[MAX_STN_NUM+1];
char g_brd_ip[4][40];
char g_grp_brd_ip[40];
int  g_plc_ip_num[MAX_STN_NUM+1][MAX_LCU_NUM+1];
char g_plc_ip[MAX_STN_NUM+1][MAX_LCU_NUM+1][4][40];
BOOL g_cpu_net_mode[MAX_STN_NUM+1][MAX_LCU_NUM+1];
int  g_plc_ip_num[MAX_STN_NUM+1][MAX_LCU_NUM+1];
int g_plcscan_host_num[MAX_STN_NUM+1][MAX_LCU_NUM+1];
char g_plcscan_host[MAX_STN_NUM+1][MAX_LCU_NUM+1][MAX_SCAN_HOST_NUM][40];
char g_sid[50];
int g_loglevel;
int g_ipwithcpu;
int g_ipaddone;
char g_hostname[100];
unsigned char g_localhost_id;
BOOL g_is_island;
BOOL g_is_grp_brd;
int g_updatefirst;
BOOL g_wranyhost;
BOOL g_wrcond;
BOOL g_wroffset;
char g_prog_name[100][20];
char g_ots_hdb_ip[40];
int	g_is_simuserver;
struct ipc_conf	ipcconf;
char g_mbpaddr[100];
BOOL g_ctrl_net_independ;
BOOL g_plc_mode;
BOOL g_sync_time;
BOOL g_simumode;

//add by wayne 09-01-12
char g_hostname_mysql[50];


struct wsconf g_ws_host[MAX_STN_NUM+1][MAX_WS_HOST_NUM+1];
#ifdef MSWIN32
 HANDLE	sem_recv_id,sem_send_id,sem_plc_data_id,sem_net_cmd_id,sem_plc_net_id,sem_plc_send_id;
 char	name[20];
#endif

/*********************************** start for common *******************************************/

void wsa_startup()
{
#ifdef MSWIN32
	WORD   		wvr;
	WSADATA 	WSAData;
	int 		status;
	wvr=MAKEWORD(2,2);
	if ((status = WSAStartup(wvr, &WSAData)) != 0) 
	{
		printf("\nWSAStartup:: %d is the err\n",status);
	}
#endif
	return;
}

int	lan_init_all ()
{
	if(LinktoSemSet(SEM_RD_SEND)==-1) return FALSE;
	if(LinktoSemSet(SEM_WR_SEND)==-1) return FALSE;
	if(LinktoSemSet(SEM_WR_NETCMD)==-1) return FALSE;	
	if(LinktoSemSet(SEM_RECVDATA)==-1) return FALSE;
	if(LinktoSemSet(SEM_WR_COMMCMD)==-1) return FALSE;
	g_stnid = 1;
	if(GetEmsConfig() == FALSE)
	{
		printf("lanlib.c: call GetEmsConfig() error\n");
		return FALSE; 
	}

	if(get_lan_shm(0) !=TRUE )
	{
		return FALSE;
	}
	if(get_lcu_shm(0) !=TRUE ) 
	{
		return FALSE;
	}

	return(TRUE);
}

int	lan_out_init ()
{
	if(LinktoSemSet(SEM_RD_SEND)==-1) return FALSE;
	if(LinktoSemSet(SEM_WR_SEND)==-1) return FALSE;
	if(LinktoSemSet(SEM_WR_NETCMD)==-1) return FALSE;	
	if(LinktoSemSet(SEM_WR_SYSLOG)==-1) return FALSE;
	g_stnid = 1;
	if(GetEmsConfig() == FALSE)
	{
		printf("lanlib.c: call GetEmsConfig() error\n");
		/* return FALSE; */
	}	
	if(get_lan_shm(1) !=TRUE )
	{
		return FALSE;
	}
	if(get_lan_shm(3) !=TRUE )
	{
		return FALSE;
	}
	return(TRUE);
}


void lan_finish()
{
	/*Needed to be added 2006.7.3*/
}

BOOL lan_out(char * buff, LAN_HEAD head,unsigned char netid)
{
	short	buf_num,length;
	unsigned char stnid,devid;
	char	typestr[20];
	length=head.length;
#ifdef _BIG_ENDIAN_
	Swap2Byte(&length);
#endif
	if(length > MAX_BUF_SIZE || length<= LANH_SIZE)
	{
		printf("head length=%d  error \n",length);
		return(FALSE);
	}
	if(head.dp_type == COMM_TIME && g_simumode == 1)
	{
		head.dp_type = COMM_SIM;
	}

	/* modified by yyp 2008.12.23 */
	if (head.dp_type==COMM_TIME && g_cascade == TRUE )
	{
		head.dp_type = COMMB_TIME;
		head.dest_id = 0;
	}

	if ( head.dp_type==COMM_TIME ) 
	{
		stnid=buff[1];
		devid=buff[2];
		if(GetPlcTypeStr(stnid,devid,typestr)==FALSE)
		{
			printf("lanlib.c : call GetPlcTypeStr(%d,%d)  error \n",stnid,devid);
			return(FALSE);
		}
		/* modified by yyp 07.12.20 */
		if ((strstr(typestr,"GE")!=0) 
	 	|| (strstr(typestr,"SIEMENS")!=0) 
	 	|| (strstr(typestr,"QUANTUM")!=0)  
	 	|| (strstr(typestr,"PREMIUM")!=0) 
	 	|| (strstr(typestr,"AB")!=0) 
	 	|| (strstr(typestr,"SYS")!=0) )
		{
			if(-1==WaitforSem(SEM_WR_NETCMD, 2))                                                                                                         
			{	
				printf("lan_out(): WaitforSem return -1 err, msg lost\n");
				return FALSE;
			}

			buf_num=shm_net_cmd->head;
		
			memcpy(shm_net_cmd->seg[buf_num].net_cmd_buf.data, buff,length);
			shm_net_cmd->seg[buf_num].lan_head.length=head.length;
			shm_net_cmd->seg[buf_num].lan_head.dp_type=head.dp_type;
			shm_net_cmd->seg[buf_num].lan_head.dest_id=head.dest_id;
			shm_net_cmd->seg[buf_num].lan_head.src_id=head.src_id;
			buf_num++;
			if(buf_num==MAX_NETCMD_SEG_NUM)   buf_num=0;
			shm_net_cmd->head=buf_num;

			ReleaseWriteSem(SEM_WR_NETCMD);		 
		}
		else if( strstr(typestr,"RTP")!=0 )
		{
			/******  write send buffer ******/ 
			if(-1==WaitforSem(SEM_WR_SEND, 2))                                                                                                         
			{	
				printf("lan_out(): WaitforSem return -1 err, msg lost\n");
				return FALSE;
			}
			head.dp_type = COMMB_TIME;
			head.dest_id = 0;
			buf_num=shm_send_addr->head;
			memcpy(shm_send_addr->shm_data_seg[buf_num].data,buff,length);
			shm_send_addr->shm_data_seg[buf_num].lan_head.length=head.length;
			//WZG20110408shm_send_addr->shm_data_seg[buf_num].lan_head.dp_type=head.dp_type;
			shm_send_addr->shm_data_seg[buf_num].lan_head.dp_type=COMMB_TIME;
			//shm_send_addr->shm_data_seg[buf_num].lan_head.dest_id=head.dest_id;
			shm_send_addr->shm_data_seg[buf_num].lan_head.dest_id=0;
			shm_send_addr->shm_data_seg[buf_num].lan_head.src_id=head.src_id;
			shm_send_addr->shm_data_seg[buf_num].lan_head.dest_stn=head.dest_stn;
			shm_send_addr->shm_data_seg[buf_num].netid = netid;
			buf_num++;
			if(buf_num==MAX_SEND_NUM)   buf_num=0;
			shm_send_addr->head=buf_num;
			ReleaseWriteSem(SEM_WR_SEND);
		}
	}
	else
	{
		/******  write send buffer ******/ 
		if(-1==WaitforSem(SEM_WR_SEND, 2))                                                                                                         
		{	
			printf("lan_out(): WaitforSem return -1 err, msg lost\n");
			return FALSE;
		}	
		
		buf_num=shm_send_addr->head;
		memcpy(shm_send_addr->shm_data_seg[buf_num].data,buff,length);
		shm_send_addr->shm_data_seg[buf_num].lan_head.length=head.length;
		shm_send_addr->shm_data_seg[buf_num].lan_head.dp_type=head.dp_type;
		shm_send_addr->shm_data_seg[buf_num].lan_head.dest_id=0;
		shm_send_addr->shm_data_seg[buf_num].lan_head.src_id=head.src_id;
		shm_send_addr->shm_data_seg[buf_num].lan_head.dest_stn=head.dest_stn;
		shm_send_addr->shm_data_seg[buf_num].netid = netid;
		buf_num++;
		if(buf_num==MAX_SEND_NUM)   buf_num=0;
		shm_send_addr->head=buf_num;
		ReleaseWriteSem(SEM_WR_SEND);	
	} 

	return(TRUE);
}

int InitPlcDataShm( unsigned char type )
{

	unsigned char i,j;
	unsigned int lcunum[MAX_STN_NUM];
	short total = 0;
	short curid = 0;
	unsigned char devid;
	unsigned int stnnum;
	unsigned char idx;
	if(shm_plc_data ==NULL )
	{
		printf("InitPlcDataShm(): error,shm_plc_data has not been mapped!\n");
		return FALSE;
	}
	for(i=1;i<MAX_STN_NUM;i++)
	{
		lcunum[i]=0;
	}
	if(GetStationNum(&stnnum) == -1)
	{
		printf("InitPlcDataShm(): call GetStationNum error\n");
		return FALSE;
	}
 	for(i=1;i<=stnnum;i++) 
  	{  	
		if(GetGroupNumById( id_stn[i] , &lcunum[i] )==-1)
		{
		   printf("InitPlcDataShm(): call GetGroupNumById error,stnid=%d \n", id_stn[i] );
		   continue;
		}
		else
		{
		   total += lcunum[i];	
		}
	}
	if( total >= MAX_PLCDATA_SEG_NUM )
	{
		printf("InitPlcDataShm()failed!,the lcu count %d is big than MAX\n", total);
		return FALSE;			
	}
 	for(i=1;i<=stnnum;i++) 	
	{
		for(j=1;j<=lcunum[i];j++ )
		{
		  	curid++;
			if( type == 1 )	
			{					
				shm_plc_data->seg[curid].stnid =  id_stn[i] ;
				shm_plc_data->seg[curid].devid = j;
			}	
			segid[i][j] = curid ;	  		
		}
	}
	
	if(total == curid)
	{
		if( type == 1 )	
		{		
			shm_plc_data->valid = YES;
		}	

	}
	else
	{
		printf("InitPlcDataShm()failed!\n");
		if( type == 1 )	
		{		
			shm_plc_data->valid = NO;
		}			
		return FALSE;		
	}
	for(i=1;i<MAX_STN_NUM;i++)
	{
		for(devid=1;devid<MAX_LCU_NUM;devid++)
		{
			shm_plc_send->seg[i][devid].send_update_flag =0;
			shm_plc_send->seg[i][devid].ai_size =0;
			shm_plc_send->seg[i][devid].pol_size =0;
		}
	}
	return TRUE;	
}

/***************************************** end for common ******************************************/


/***************************************** start  code for SUN *************************************/
#ifdef SUN

/********************************* get the lan shared  memory segment and attach it ****************/

int	get_lan_shm(unsigned char shmid)
{
	int	shm_send_id,net_cmd_id,comm_cmd_id;
	int	shm_recv_id,shm_test_id;
	int plc_data_id, plc_cmd_id;
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	
	switch(shmid)
	{
	case 0:
		;
	case 1:
		 /****** get the shared send memory segment and attach it ******/
		shm_send_addr=&data_send_addr;
		if((shm_send_id=shmget (ftok(keyfile,(char )SHM_SEND_KEY_CHAR),SHM_SEND_DATA_SIZE,0))<0)
		{
				printf("lanlib.c: can't get send shared memory,size=%d\n",SHM_SEND_DATA_SIZE);
				return(-1);
		}
		shm_send_addr=(SHM_SEND_DATA *)shmat (shm_send_id,0,SHM_RND);
		if((long *)shm_send_addr==(long *)-1)
		{
				printf("lanlib.c: can't attach send shared memory\n");
				return(-1);
		}	
		if(shmid==1)	break;
	case 2:
 		/****** get the shared receive memory segment and attach it ******/
		shm_recv_addr=&data_recv_addr;
		if((shm_recv_id=shmget (ftok(keyfile,(char )SHM_RECV_KEY_CHAR),SHM_RECV_DATA_SIZE,0))<0)
		{
			printf("lanlib.c: can't get recv shared memory\n");
			return(-1);
		}
		shm_recv_addr=(SHM_RECV_DATA *)shmat (shm_recv_id,0,SHM_RND);
		if((long *)shm_recv_addr==(long *)-1)
		{
			printf("lanlib.c: can't attach  recv shared memory\n");
			return(-1);
		}
 		
		if(shmid==2)	break;
	case 3:
		/****** get the shared  plc send memory segment and attach it ******/
		shm_net_cmd=&net_cmd_addr;
		if((net_cmd_id=shmget (ftok(keyfile,(char )SHM_NETCMD_KEY_CHAR) ,NET_CMD_SIZE, 0))<0)
		{
			printf("lanlib.c: can't get shared memory SHM_NETCMD_KEY_CHAR \n");
			return(-1);
		}
		shm_net_cmd=(struct net_cmd *)shmat (net_cmd_id,0,SHM_RND);
		if((long *)shm_net_cmd==(long *)-1)
		{
			printf("lanlib.c: can't attach  net_cmd shared memory\n");
			return(-1);
		}
		if(shmid==3)	break;	

	case 4:
		/****** get the shared  plc send memory segment and attach it ******/
		shm_comm_cmd=&comm_cmd_addr;
		if((comm_cmd_id=shmget (ftok(keyfile,(char )SHM_COMMCMD_KEY_CHAR) ,COMM_CMD_SIZE, 0))<0)
		{
			printf("lanlib.c: can't get shared memory SHM_COMMCMD_KEY_CHAR \n");
			return(-1);
		}
		shm_comm_cmd=(struct comm_cmd *)shmat (comm_cmd_id,0,SHM_RND);
		if((long *)shm_comm_cmd==(long *)-1)
		{
			printf("lanlib.c: can't attach  comm_cmd shared memory\n");
			return(-1);
		}
		if(shmid==4)	break;	
	default:
		break;
	} /****** end of switch ******/
	return(TRUE);
}


/********************************* get the lcu shared  memory segment and attach it ********************/
int get_lcu_shm(unsigned char shmid)
{
	int  plc_data_id, plc_cmd_id,plc_send_id;
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
  	shm_plc_data=&plc_data_addr;
	switch(shmid)
	{
	case 0:	
		;
	case 1:
		if((plc_data_id=shmget (ftok(keyfile,(char )SHM_PLCDATA_KEY_CHAR), PLC_DATA_SIZE, 0))<0)
		{
			printf("lanlib.c:  can't get shared memory SHM_PLCDATA_KEY_CHAR,size =%d\n",PLC_DATA_SIZE);
			return(-1);
		}
		shm_plc_data=(struct plc_data *)shmat (plc_data_id,0,SHM_RND);
		if((long *)shm_plc_data==(long *)-1)
		{
			printf("lanlib.c: can't attach  plc_data shared memory\n");
			return(-1);
		}
		if(shmid==1)	break;	
	
	case 2:
		shm_plc_cmd=&plc_cmd_addr;	
		if((plc_cmd_id=shmget (ftok(keyfile,(char )SHM_PLCCMD_KEY_CHAR), PLC_CMD_SIZE, 0))<0)
		{
			printf("lanlib.c: can't get shared memory SHM_PLCMD_KEY_CHAR ,size=%d\n",PLC_CMD_SIZE);
			return(-1);
		}
		shm_plc_cmd=(struct plc_cmd *)shmat (plc_cmd_id,0,SHM_RND);
		if((long *)shm_plc_cmd==(long *)-1)
		{
			printf("lanlib.c: can't attach plc_cmd shared memory\n");
			return(-1);
		}
		if(shmid==2)	break;	

	case 3:
		shm_plc_send=&plc_send_addr;
		if((plc_send_id=shmget (ftok(keyfile,(char )SHM_PLCSEND_KEY_CHAR), PLC_SEND_SIZE, 0))<0)
		{
			printf("lanlib.c: can't get shared memory SHM_PLCSEND_KEY_CHAR,size=%d\n",PLC_SEND_SIZE);
			return(-1);
		}
		shm_plc_send=(struct plc_send *)shmat (plc_send_id,0,SHM_RND);
		if((long *)shm_plc_send==(long *)-1)
		{
			printf("lanlib.c: can't attach plc_send shared memory\n");
			return(-1);
		}
		if(shmid==3)	break;	
	default:
		break;	
	}/* end switch */
	return(TRUE);
}

/****** map log shared memory added by yyp on 2005.12.2 ******/
int get_log_shm()
{
	int	shm_id_log;
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	shm_log_addr=&data_log;
	if((shm_id_log=shmget (ftok(keyfile,(char )SHM_SYSLOG_KEY_CHAR),SHM_LOG_SIZE,0))<0)
	{
		printf("can't get log shared memory ,size=%d\n",SHM_LOG_SIZE);
		return(FALSE);
	}
	shm_log_addr=(struct shm_log *)shmat (shm_id_log,0,SHM_RND);
	if((long *)shm_log_addr==(long *)-1)
	{
		printf("can't attach log shared memory\n");
		return(FALSE);
	}
	return(TRUE);
}



/****** map log shared memory added by yyp on 2005.12.2 ******/
int get_hdb_shm()
{
	int	shm_id_hdb;
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	shm_hdb_addr=&data_hdb;
	if((shm_id_hdb=shmget (ftok(keyfile,(char )SHM_HDB_KEY_CHAR),SHM_HDB_SIZE,0))<0)
	{
		printf("can't get hdb shared memory ,size=%d\n",SHM_HDB_SIZE);
		return(FALSE);
	}
	shm_hdb_addr=(struct shm_hdb *)shmat (shm_id_hdb,0,SHM_RND);
	if((long *)shm_hdb_addr==(long *)-1)
	{
		printf("can't attach hdb shared memory\n");
		return(FALSE);
	}
	return(TRUE);
}

/****** map log shared memory added by yyp on 2005.12.2 ******/
int get_ots_shm()
{
	int	shm_id_ots;
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	shm_ots_addr=&data_ots;
	if((shm_id_ots=shmget (ftok(keyfile,(char )SHM_OTS_KEY_CHAR),SHM_OTS_SIZE,0))<0)
	{
		printf("can't get ots shared memory ,size=%d\n",SHM_OTS_SIZE);
		return(FALSE);
	}
	shm_ots_addr=(struct shm_ots *)shmat (shm_id_ots,0,SHM_RND);
	if((long *)shm_ots_addr==(long *)-1)
	{
		printf("can't attach ots shared memory\n");
		return(FALSE);
	}
	return(TRUE);
}

/*********************** get rec shared memory added by yyp 05.12.14 ********************************/
int get_rec_shm()
{
	int	shm_id_rec;
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	shm_rec_addr=&data_rec;
	
	if((shm_id_rec=shmget (ftok(keyfile,(char )SHM_REC_KEY_CHAR),SHM_REC_SIZE,0))<0)
	{
		printf("can't get rec shared memory ,size=%d\n",SHM_REC_SIZE);
		return(FALSE);
	}
	shm_rec_addr=(SHM_RECORD  *)shmat (shm_id_rec,0,SHM_RND);

	if((long *)shm_rec_addr==(long *)-1)
	{
		printf("can't attach rec shared memory\n");
		return(FALSE);
	}
	return(TRUE);
	

}

#endif


/********************************** end for SUN **************************************************/




/********************************** start code for MSWIN32 ***************************************/

#ifdef MSWIN32

/****** get the lan shared  memory segment and attach it ******/
int	get_lan_shm(unsigned char shmid)
{

	HANDLE	shm_send_id,shm_recv_id,net_cmd_id,comm_cmd_id;
	switch(shmid)
	{
		case 0:
		case 1:
			 /****** get the shared send memory segment and attach it ******/
			shm_send_addr=&data_send_addr;
			sprintf(name, "SHM_NAME_%c", SHM_SEND_KEY_CHAR);
			shm_send_id=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,name);
			if(shm_send_id==NULL)
			{
 				printf("\nlan_lib.c::Error in  OpenFileMapping shm_send_id :%d",GetLastError());
 				return(FALSE);
			}
			shm_send_addr=(SHM_SEND_DATA *)MapViewOfFile(shm_send_id,FILE_MAP_ALL_ACCESS,0,0,0);
			if(shm_send_addr==NULL)
			{
 				printf("\nlan_lib.c::Error in  MapViewFile shm_send_addr:%d",GetLastError());
 				return(FALSE);
			}
			/*
			sem_send_id=OpenSemaphore(	SEMAPHORE_MODIFY_STATE|SEMAPHORE_ALL_ACCESS|SYNCHRONIZE,
				TRUE, "sem_send_turn");
			if(sem_send_id==NULL)
			{
				printf("\nlan_lib.c::Error in OpenSemaphore sem_send_turn:%d",GetLastError());
				return(FALSE);
			}
			*/
			if(shmid !=0)
			{
				break;
			}
		case 2:
 			/****** get the shared receive memory segment and attach it ******/
			shm_recv_addr = &data_recv_addr;
			sprintf(name, "SHM_NAME_%c", SHM_RECV_KEY_CHAR);
			shm_recv_id = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,name);
			if(shm_recv_id == NULL)
			{
				printf("\nlan_lib.c::Error in  OpenFileMapping shm_recv_id :%d",GetLastError());
				return(FALSE);
			}
			shm_recv_addr =(SHM_RECV_DATA *)MapViewOfFile(shm_recv_id,FILE_MAP_ALL_ACCESS,0,0,0);
			if(shm_recv_addr==NULL)
			{
				printf("\nlan_lib.c::Error in  MapViewFile shm_recv_addr:%d",GetLastError());
				return(FALSE);
			}
			/*
			sem_recv_id=OpenSemaphore(	SEMAPHORE_MODIFY_STATE|SEMAPHORE_ALL_ACCESS|SYNCHRONIZE,
				TRUE, "sem_recv_turn");
			if(sem_recv_id==NULL)
			{
				printf("\nlan_lib.c::Error in OpenSemaphore sem_recv_turn:%d",GetLastError());
				return(FALSE);
			}
			*/
			if(shmid !=0)
			{
				break;
			}
		case 3:
			/****** get the shared  plc netcmd memory segment and attach it ******/
			shm_net_cmd = &net_cmd_addr;
			sprintf(name, "SHM_NAME_%c", SHM_NETCMD_KEY_CHAR);
			net_cmd_id = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,name);
			if(net_cmd_id == NULL)
			{
				printf("\nlan_lib.c::Error in  OpenFileMapping net_cmd_id :%d",GetLastError());
				return(FALSE);
			}
			shm_net_cmd =(struct net_cmd *)MapViewOfFile(net_cmd_id,FILE_MAP_ALL_ACCESS,0,0,0);
			if(shm_net_cmd==NULL)
			{
				printf("\nlan_lib.c::Error in  MapViewFile shm_net_cmd:%d",GetLastError());
				return(FALSE);
			}
			/*
			sem_net_cmd_id=OpenSemaphore(	SEMAPHORE_MODIFY_STATE|SEMAPHORE_ALL_ACCESS|SYNCHRONIZE,
				TRUE, "sem_net_cmd_turn");
			if(sem_net_cmd_id==NULL)
			{
				printf("\nlan_lib.c::Error in OpenSemaphore sem_net_cmd_turn:%d",GetLastError());
				return(FALSE);
			}
			*/
			if(shmid !=0)
			{
				break;
			}
		case 4:
			/****** get the shared  plc netcmd memory segment and attach it ******/
			shm_comm_cmd = &comm_cmd_addr;
			sprintf(name, "SHM_NAME_%c", SHM_COMMCMD_KEY_CHAR);
			comm_cmd_id = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,name);
			if(comm_cmd_id == NULL)
			{
				printf("\nlan_lib.c::Error in  OpenFileMapping comm_cmd_id :%d",GetLastError());
				return(FALSE);
			}
			shm_comm_cmd =(struct comm_cmd *)MapViewOfFile(comm_cmd_id,FILE_MAP_ALL_ACCESS,0,0,0);
			if(shm_comm_cmd==NULL)
			{
				printf("\nlan_lib.c::Error in  MapViewFile shm_comm_cmd:%d",GetLastError());
				return(FALSE);
			}
			if(shmid !=0)
			{
				break;
			}
		default:
			;
	} /****** end of switch ******/
	return(TRUE);
}

/****** get the lcu shared  memory segment and attach it ******/
int get_lcu_shm(unsigned char shmid)
{
	HANDLE  hPlcData;
	HANDLE  hPlcCmd;
	HANDLE  hPlcSend;
  	shm_plc_data=&plc_data_addr;
	shm_plc_cmd=&plc_cmd_addr;
	shm_plc_send=&plc_send_addr;
	switch(shmid)
	{
		case 0:
			;
		case 1:
			sprintf(name, "SHM_NAME_%c", SHM_PLCDATA_KEY_CHAR);
			hPlcData = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, name);
			if (hPlcData == NULL)
			{
				printf("\nget_lcu_shm::Error OpenFileMapping shm_plc_data: %d\n",GetLastError());
				return(FALSE);
			}
			else
			{
#ifdef DEBUG
				printf("\nget_lcu_shm::OpenFileMapping shm_plc_data: OK!\n");
#endif
			}
			shm_plc_data = (struct plc_data *) MapViewOfFile (hPlcData,FILE_MAP_ALL_ACCESS,0, 0, 0);  
			if (shm_plc_data == NULL)
			{
				printf("\nget_lcu_shm::Error MapViewOfFile shm_plc_data: %d\n",GetLastError());
				return(FALSE);
			}
			else
			{
#ifdef DEBUG
				printf("\nget_lcu_shm::MapViewOfFile shm_plc_data: OK!\n");
#endif
			}
			/*
			sem_plc_data_id=OpenSemaphore(	SEMAPHORE_MODIFY_STATE|SEMAPHORE_ALL_ACCESS|SYNCHRONIZE,
				TRUE, "sem__plc_data_turn");
			if(sem_plc_data_id==NULL)
			{
				printf("\nlan_lib.c::Error in OpenSemaphore sem__plc_data_turn:%d",GetLastError());
				return(FALSE);
			}
			*/
			if(shmid !=0)
			{
				break;
			}	

		case 2:
			sprintf(name, "SHM_NAME_%c", SHM_PLCCMD_KEY_CHAR);
			hPlcCmd = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, name);
			if (hPlcCmd == NULL)
			{
				printf("\nget_lcu_shm::Error OpenFileMapping shm_plc_cmd: %d\n",GetLastError());
				return(FALSE);
			}
			else
			{
#ifdef DEBUG
				printf("\nget_lcu_shm::OpenFileMapping shm_plc_cmd: OK!\n");
#endif
			}
			shm_plc_cmd = (struct plc_cmd *) MapViewOfFile (hPlcCmd,FILE_MAP_ALL_ACCESS,0, 0, 0);  
			if (shm_plc_cmd == NULL)
			{
				printf("\nget_lcu_shm::Error MapViewOfFile shm_plc_cmd: %d\n",GetLastError());
				return(FALSE);
			}
			else
			{
#ifdef DEBUG
				printf("\nget_lcu_shm::MapViewOfFile shm_plc_cmd: OK!\n");
#endif
			}	
			if(shmid !=0)
			{
				break;
			}
			
		case 3:
			sprintf(name, "SHM_NAME_%c", SHM_PLCSEND_KEY_CHAR);
			hPlcSend = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, name);
			if (hPlcSend == NULL)
			{
				printf("\nget_lcu_shm::Error OpenFileMapping shm_plc_send: %d\n",GetLastError());
				return(FALSE);
			}
			else
			{
#ifdef DEBUG
				printf("\nget_lcu_shm::OpenFileMapping shm_plc_send: OK!\n");
#endif
			}
			shm_plc_send = (struct plc_send *) MapViewOfFile (hPlcSend,FILE_MAP_ALL_ACCESS,0, 0, 0);  
			if (shm_plc_send == NULL)
			{
				printf("\nget_lcu_shm::Error MapViewOfFile shm_plc_send: %d\n",GetLastError());
				return(FALSE);
			}
			else
			{
#ifdef DEBUG
				printf("\nget_lcu_shm::MapViewOfFile shm_plc_send: OK!\n");
#endif
			}				
			if(shmid !=0)
			{
				break;
			}	

		default:
			break;
	} /* end switch */
	return(TRUE);
}


/****** map rec shared memory added by yyp on 2006.8.2 ******/
int get_rec_shm()
{
	HANDLE  shm_id_rec;
	shm_rec_addr=&data_rec;
	sprintf(name, "SHM_NAME_%c", SHM_REC_KEY_CHAR);
	shm_id_rec = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, name);
	if (shm_id_rec == NULL)
	{
		printf("\nget_rec_shm::Error OpenFileMapping shm_rec: %d\n",GetLastError());
		return(FALSE);
	}
	else
	{
#ifdef DEBUG
		printf("\nget_rec_shm::OpenFileMapping shm_rec: OK!\n");
#endif
	}
	shm_rec_addr = (SHM_RECORD *) MapViewOfFile (shm_id_rec,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_rec_addr == NULL)
	{
		printf("\nget_rec_shm::Error MapViewOfFile shm_rec_addr: %d\n",GetLastError());
		return(FALSE);
	}
	else
	{
#ifdef DEBUG
		printf("\nget_rec_shm::MapViewOfFile shm_rec_addr: OK!\n");
#endif
	}
	return(TRUE);
}


int get_iccp_data_shm()
{
	return TRUE;
}

/****** map hdb shared memory added by yyp on 2006.8.2 ******/
int get_hdb_shm()
{
	HANDLE  shm_id_hdb;
	shm_hdb_addr=&data_hdb;
	sprintf(name, "SHM_NAME_%c", SHM_HDB_KEY_CHAR);
	shm_id_hdb = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, name);
	if (shm_id_hdb == NULL)
	{
		printf("\nget_hdb_shm::Error OpenFileMapping shm_hdb: %d\n",GetLastError());
		return(FALSE);
	}
	else
	{
#ifdef DEBUG
		printf("\nget_hdb_shm::OpenFileMapping shm_hdb: OK!\n");
#endif
	}
	shm_hdb_addr = (struct shm_hdb *) MapViewOfFile (shm_id_hdb,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_hdb_addr == NULL)
	{
		printf("\nget_hdb_shm::Error MapViewOfFile shm_hdb_addr: %d\n",GetLastError());
		return(FALSE);
	}
	else
	{
#ifdef DEBUG
		printf("\nget_hdb_shm::MapViewOfFile shm_hdb_addr: OK!\n");
#endif
	}
	return(TRUE);
}

/****** map ots shared memory added by yyp on 2009.7.7 ******/
int get_ots_shm()
{
	HANDLE  shm_id_ots;
	shm_ots_addr=&data_ots;
	sprintf(name, "SHM_NAME_%c", SHM_OTS_KEY_CHAR);
	shm_id_ots = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, name);
	if (shm_id_ots == NULL)
	{
		printf("\nget_ots_shm::Error OpenFileMapping shm_ots: %d\n",GetLastError());
		return(FALSE);
	}
	else
	{
#ifdef DEBUG
		printf("\nget_ots_shm::OpenFileMapping shm_ots: OK!\n");
#endif
	}
	shm_ots_addr = (struct shm_ots *) MapViewOfFile (shm_id_ots,FILE_MAP_ALL_ACCESS,0, 0, 0);  
	if (shm_ots_addr == NULL)
	{
		printf("\nget_ots_shm::Error MapViewOfFile shm_ots_addr: %d\n",GetLastError());
		return(FALSE);
	}
	else
	{
#ifdef DEBUG
		printf("\nget_ots_shm::MapViewOfFile shm_ots_addr: OK!\n");
#endif
	}
	return(TRUE);
}

/* CreateShareDataMapping created by wdk 2003.7.5 */
void * CreateShm (char *name, int size )
{
	HANDLE		hMapObject;

	hMapObject = CreateFileMapping (( HANDLE ) 0xFFFFFFFF,
		NULL,						  
		PAGE_READWRITE,
		0,
		size,
		name );
	
	if ( hMapObject == NULL )
		printf ( "Error in CreateFileMapping %s:%d\n", name, GetLastError ());
	else
		printf ( "\n%s: OK!\n", name );
	
	return ( hMapObject );
}

#endif



/*********************************** end for MSWIN32 ********************************************/
