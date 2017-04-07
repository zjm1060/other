/************************************************************************************************************************
FILENAME:	netudp.c

FUNCTION:	
	
	source code of udp sending and udp receiving,include:
	BOOL Net_recv(SOCKET sock);
	BOOL Net_send();  

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

*************************************************************************************************************************/

#include "../inc/lanhead_all.h" 

/*********************************************************************************************************#
	Net_recv function, receive every seg data broadcasted and save it in shm_recv continually	
********************************************************************************************************#*/
BOOL net_recv(SOCKET sock)
{
	short		dcount,length;
	int		i;
	SHM_DATA_SEG    datapipe;
	struct net_cmd_seg  cmd_rec;
	CMD_STATUS cmd_status;
	char		src_id;
	UCHAR		groups_id;
	short	cmd_head;
	unsigned short	pkg_num,last_pkg_num[MAX_WS_HOST_NUM];
	int		buf_num;
	int		buf_num_cmd;
	PKGID		recvid[MAX_IDBUF_NUM];
	unsigned char type_id,stnid,idx,devid;	
	short		curpos=0;
	char	typestr[20];
	LAN_HEAD head;
#ifdef SUN
	extern   void    sig_term(int);
	signal(SIGKILL,sig_term);
	signal(SIGINT,sig_term);
	signal(SIGTERM,sig_term);
#endif	
	
	/*****************# initialise the recvid queue *********************/
	for(i=0;i<MAX_IDBUF_NUM;i++)
	{
		recvid[i].src_id=-1;
		recvid[i].pkg_num=-1;	
	}
		
	/*****************# loop to  receive data ************************#*/
	for(;;)
	{
		dcount=udp_recv(sock,(char *)&datapipe,(MAX_BUF_SIZE+LANH_SIZE));
		if(dcount<0)
		{
			printf("netupd.c: recv dcount=%d,recv error\n",dcount);
			continue;
			
		}	
		if( shm_recv_addr->island != g_is_island )
		{
			g_is_island = shm_recv_addr->island;
			printf("island status chged to %d\n",g_is_island);
		}
		length=datapipe.lan_head.length;
#ifdef _BIG_ENDIAN_
		Swap2Byte(&length);
#endif
		if(dcount<=LANH_SIZE)
		{
			printf("netupd.c: recv dcount=%d, <LANH_SIZE\n",dcount);
			continue;
			
		}	    
		dcount= dcount-LANH_SIZE;

		if(dcount==length && dcount<MAX_BUF_SIZE)
		{
			src_id=datapipe.lan_head.src_id;
			groups_id=datapipe.lan_head.dest_id;
			if(src_id<=0 || src_id > MAX_WS_HOST_NUM)
			{
				printf("\nnetudp.c:: src_id invalid,src_id=%d\n",src_id);     	
				continue;				
			}
			/********************************用户分组***************************/
			if(__argc == 1 && src_id != g_localhost_id)
			{
				if (groups_id == -1 || groups_id == 255)//throw the package of groups_id is -1;
				{
					//printf("\nnetudp.c:: groups_id is -1,groups_id=%d\n",groups_id);     	
					continue;	
				}	

				if(groups_id != shm_ots_addr->groups_id)//throw the package of groups_id is different;
				{
					//printf("\nnetudp.c:: groups_id is different,groups_id=%d\n",groups_id);     	
					continue;				
				}
			}
			/*******************************************************************/
			
			if(g_is_island == TRUE)
			{
				if(src_id != g_localhost_id) 
				{
					continue;
				}
			}
		
			pkg_num=datapipe.lan_head.packet_num;
#ifdef _BIG_ENDIAN_
			Swap2Byte(&pkg_num);
#endif					
			
			if(IsExistID(src_id,pkg_num,recvid)==TRUE) 
			{
				if(g_loglevel >= 2)
				{				
					printf("src_id %d, pkg_num %d exist in queue!\n",src_id,pkg_num);
				}
				continue;
			}
			
			if(last_pkg_num[src_id]+1<pkg_num)
			{
				char	log_str[256];
				sprintf(log_str,"pkg may be lost: src_id=%d, last_pkg_num=%d, cur_pkg_num=%d\n",src_id,last_pkg_num[src_id],pkg_num);
				log_write(H9000_LOG_ERROR,1,log_str);
			}
			/***************仿真服务器只接收仿真全送令****************/
			if (g_is_simuserver && __argc == 1 && src_id != g_localhost_id)
			{
				type_id = datapipe.data[0];
				head = datapipe.lan_head;
				if (head.dp_type != COMM_SIM || type_id != C_DATA_POLLING)
				{
					continue;
				}
			}
			/*********************************************************/
			last_pkg_num[src_id] = pkg_num;			
			buf_num=shm_recv_addr->head;

			/* write one seg to shm_recv */
			memset(shm_recv_addr->shm_data_seg[buf_num].data,0,MAX_BUF_SIZE);
			memcpy(shm_recv_addr->shm_data_seg[buf_num].data,datapipe.data,dcount);
			shm_recv_addr->shm_data_seg[buf_num].lan_head.dp_type=datapipe.lan_head.dp_type;
			shm_recv_addr->shm_data_seg[buf_num].lan_head.length=datapipe.lan_head.length;
			shm_recv_addr->shm_data_seg[buf_num].lan_head.src_id=datapipe.lan_head.src_id; 
			shm_recv_addr->shm_data_seg[buf_num].lan_head.dest_id=datapipe.lan_head.dest_id; 
			shm_recv_addr->shm_data_seg[buf_num].lan_head.packet_num=datapipe.lan_head.packet_num; 
			if(g_loglevel >= 2)
			{
				printf("\nnetudp.c:: received length=%d ,dp_type=%d,dest_id=%d,src_id=%d pkg_num=%d\n",
									length,  
									datapipe.lan_head.dp_type,
        							datapipe.lan_head.dest_id, 
									datapipe.lan_head.src_id,
        							pkg_num);  
			}
#ifdef DEBUG
			if(datapipe.lan_head.dp_type==ALL_DB)
			{
				printf("\nnetudp.c::  recv LCU%d all db(%d) \n",datapipe.lan_head.src_id,datapipe.data[1]);
			}
			else
			{
 				for(i=0;i<dcount;i++)
				{
 			 		printf("=%d",shm_recv_addr->shm_data_seg[buf_num].data[i]);
				}
			}
			printf("\n"); 	  
#endif

			buf_num++;
			if(buf_num>=MAX_RECV_NUM)	buf_num=0;
			shm_recv_addr->head=buf_num;
			
			shm_recv_addr->flag=IDLE_FLAG;

			//wzg20110221 move from behind
			if(-1 == ReleaseReadSem(SEM_RECVDATA))
				printf("\nFail to release sem\n");

			/* save pkg_num to queue */
			recvid[curpos].src_id=src_id;
			recvid[curpos].pkg_num=pkg_num;
			curpos++;
			if(curpos>=MAX_IDBUF_NUM)	curpos=0;
			//wzg20110221 added  modified by yyp 2011.3.09
			if(dcount>SHM_NETCMD_SEG_SIZE)
			{
				//printf("lanlib.c : command package length error: %d, max 32 bytes\n",dcount);
				continue;
			}
			//added by yyp 2011.03.14 
			type_id = datapipe.data[0];
			head = datapipe.lan_head;
			if( g_cascade == TRUE && type_id>=C_LCU_ONOFF )
			{
				stnid=datapipe.data[1];
				memcpy(cmd_rec.net_cmd_buf.data,datapipe.data,dcount);
				cmd_status = cmd_rec.net_cmd_buf.netdata.status;
#ifdef _BIG_ENDIAN_
				Swap2Byte(&cmd_status);
#endif			
				if( stnid<1 || stnid>MAX_STN_NUM)
				{
					printf("lanlib.c : stn id error: %d\n",stnid);
					continue;
				}
				idx = index_stn[stnid];
				if( cmd_status.state != 2 )
				{
					if(-1!=WaitforSem(SEM_WR_COMMCMD, 2))                                                                                             
					{	
						cmd_head=shm_comm_cmd->stn_data[idx].head;
						memcpy(shm_comm_cmd->stn_data[idx].seg[cmd_head].net_cmd_buf.data,datapipe.data,dcount);
						shm_comm_cmd->stn_data[idx].seg[cmd_head].lan_head.length=head.length;
						shm_comm_cmd->stn_data[idx].seg[cmd_head].lan_head.dp_type=head.dp_type;
						shm_comm_cmd->stn_data[idx].seg[cmd_head].lan_head.dest_id=head.dest_id;
						shm_comm_cmd->stn_data[idx].seg[cmd_head].lan_head.src_id=head.src_id;
						cmd_head++;
						if(cmd_head==MAX_NETCMD_SEG_NUM)   
							cmd_head=0;
						shm_comm_cmd->stn_data[idx].head=cmd_head;
						ReleaseWriteSem(SEM_WR_COMMCMD);
					}
					else
					{	
						printf("lan_inb(): WaitforSem SEM_WR_COMMCMD return -1 err, msg lost\n");
					}				
				}			

			}
			
			/* added by yyp for simulate 2008.9.27 */
			if (datapipe.lan_head.dp_type==COMM_SIM || datapipe.lan_head.dp_type==COMMB_TIME) 
			{
				head = datapipe.lan_head;
				stnid=datapipe.data[1];
				devid=datapipe.data[2]; 
				if(GetPlcTypeStr(stnid,devid,typestr)==FALSE)
				{
					printf("lan_inb(): call GetPlcTypeStr(%d,%d)  error \n",stnid,devid);
					continue;
				}
				/* modified by yyp 07.12.20 */
				if ((strstr(typestr,"GE")) 
	 			|| (strstr(typestr,"SIEMENS")) 
	 			|| (strstr(typestr,"QUANTUM"))  
	 			||(strstr(typestr,"PREMIUM")) 
	 			||(strstr(typestr,"AB")) 
//added by yyp 2012.11.09
	 			||(strstr(typestr,"SYS")) 
	 			||(strstr(typestr,"RTP")))

//added by wzg20110408
				{
					if(-1==WaitforSem(SEM_WR_NETCMD, 2))                                                                                             
					{	
						printf("lan_inb(): WaitforSem return -1 err, msg lost\n");
						continue;
					}
					buf_num_cmd=shm_net_cmd->head;
					memcpy(shm_net_cmd->seg[buf_num_cmd].net_cmd_buf.data,datapipe.data,dcount);
					shm_net_cmd->seg[buf_num_cmd].lan_head.length=head.length;
					shm_net_cmd->seg[buf_num_cmd].lan_head.dp_type=head.dp_type;
					shm_net_cmd->seg[buf_num_cmd].lan_head.dest_id=head.dest_id;
					shm_net_cmd->seg[buf_num_cmd].lan_head.src_id=head.src_id;
					buf_num_cmd++;
					if(buf_num_cmd==MAX_NETCMD_SEG_NUM)   buf_num_cmd=0;
					shm_net_cmd->head=buf_num_cmd;
					ReleaseWriteSem(SEM_WR_NETCMD);	
				}
			}			
			if(-1 == ReleaseReadSem(SEM_RECVDATA))
				printf("\nFail to release sem\n");
		}
		else
		{
			printf("\nnetudp.c:: data length error,dcount=%d\n",dcount);     	
			continue;
		}
	
	}/* end for */	
}
 
/*********************************************************************************************************#
	Net_send function, broadcast every seg in shm_send continually	
********************************************************************************************************#*/

BOOL net_send()
{
	SOCKET		sock_a,sock_b;					/* socket connect to net a and net b	*/
	unsigned short	pkg_num;					/* package id added for dual net broadcast */
	short		i,ret,length,sendb_ptr;
	char  		dest_id;
	unsigned char	netid;
	SHM_DATA_SEG  	sendseg;

#ifdef SUN	
	extern   void    sig_term(int);
	
	signal(SIGKILL,sig_term);
	signal(SIGINT,sig_term);
	signal(SIGTERM,sig_term);
#endif
	/******* Get random package id *******/
	pkg_num=GetRandID();
#ifdef DEBUG
	printf("Get random pkg_num=%d \n",pkg_num);
#endif	

	/******* ?*******/
	sendb_ptr=shm_send_addr->head;
	
	/******* ?******/
	shm_send_addr->flag = IDLE_FLAG;
	
	sock_a=INVALID_SOCKET;
	sock_b=INVALID_SOCKET;
	

	printf("\nwaiting for new recordset to be sent......\n");
 
	
	for(;;)
	{
		MySleep(5);
		if(sock_a==INVALID_SOCKET)
		{
 			sock_a=udp_open(0,0);
			if(sock_a==INVALID_SOCKET)
			{ 
 				printf("\nnetudp.c:: Udp_open net A error\n");
			}
			else
			{
 				printf("\nnetudp.c:: Udp_open net A success,sock_a=%d\n",sock_a);				
			}
		}
		
		if(g_sys_dualnet == YES)
		{
			if( sock_b==INVALID_SOCKET )
			{
 				sock_b=udp_open(0,1);
				if(sock_b==INVALID_SOCKET)
				{ 
 					printf("\nnetudp.c:: Udp_open net B error\n");
       					/*return FALSE; removed by yh*/
				}
				else
				{
 					printf("\nnetudp.c:: Udp_open net B success,sock_b=%d\n",sock_b);				
				}			
			}
		}
		
		if(g_sys_dualnet == YES)
		{
			if((sock_a==INVALID_SOCKET) && (sock_b==INVALID_SOCKET))
				continue;
		}
		else
		{
			if(sock_a==INVALID_SOCKET)
				continue;
		}
					
		while(sendb_ptr!=shm_send_addr->head)
		{
			i=sendb_ptr;
			netid=shm_send_addr->shm_data_seg[i].netid;
			if((netid != 1) && (netid !=2)) 
			{
				netid = 0;
			}
			dest_id=shm_send_addr->shm_data_seg[i].lan_head.dest_id;
			//if( dest_id!=0 && shm_send_addr->shm_data_seg[i].lan_head.dp_type != COMM_SIM && g_cascade != TRUE)	
			//if(shm_send_addr->shm_data_seg[i].lan_head.dp_type != COMM_SIM && g_cascade != TRUE)	
		//	{
		//		/* modified by yyp 2008.11.14 */
		//		sendb_ptr++;
		//		if(sendb_ptr==MAX_SEND_NUM)  sendb_ptr=0;				
		//		continue;
		//	}
			length=shm_send_addr->shm_data_seg[i].lan_head.length;
#ifdef _BIG_ENDIAN_
			Swap2Byte(&length);
#endif
			if(length==0 || length>MAX_BUF_SIZE)	
			{
				/* modified by yyp 2008.11.14 */
				sendb_ptr++;
				if(sendb_ptr==MAX_SEND_NUM)  sendb_ptr=0;
				continue;
			}
			memcpy(&sendseg,&shm_send_addr->shm_data_seg[i],LANH_SIZE+length);
			
			sendseg.lan_head.packet_num=pkg_num;
			sendseg.lan_head.dest_id = shm_ots_addr->groups_id;
#ifdef _BIG_ENDIAN_
			Swap2Byte(&(sendseg.lan_head.packet_num));
#endif			
			
			if(sock_a==INVALID_SOCKET)
			{ 
 				printf("\nnetudp.c:: sock_a==INVALID_SOCKET error\n");
       				if(g_sys_dualnet != YES)
       					break; 
			}
			else
			{
				if(netid == 0 || netid ==1)
				{
					ret=udp_send(sock_a,(char *)&sendseg,(LANH_SIZE+length),0);
					if(ret==-1) 
					{
						printf("\nnetudp.c:: broadcast via %d  error \n",0);
						udp_close(sock_a);
						sock_a=INVALID_SOCKET;
					}
					else
					{
						if(g_loglevel >= 2)
						{
							printf("\nnetudp.c:: broadcase via net %d successfully, pkg_num=%d!\n",0,pkg_num);
						}
				
					}
				}

			}
			
			if( g_sys_dualnet == YES )
			{
 				if(sock_b==INVALID_SOCKET )
				{ 
 					printf("\nnetudp.c:: sock_b==INVALID_SOCKET error\n");
				} 
				else
				{
					if(netid == 0 || netid ==2)
					{
						ret=udp_send(sock_b,(char *)&sendseg,(LANH_SIZE+length),1);
						if(ret==-1)
						{
							printf("\nnetudp.c:: broadcast via %d  error \n",1);
							udp_close(sock_b);
							sock_b=INVALID_SOCKET;
						}
						else
						{
							if(g_loglevel >= 2)
							{						
								printf("\nnetudp.c:: broadcast via net %d successfully, pkg_num=%d!\n",1,pkg_num);
							}
						}
					}
				}
			}
#ifdef DEBUG			

 			for(j=0;j<length;j++)
			{
 			 	printf("=%d",sendseg.data[j]);
			}			
#endif				
			shm_send_addr->shm_data_seg[i].lan_head.packet_num = pkg_num;
#ifdef _BIG_ENDIAN_
			Swap2Byte(&(shm_send_addr->shm_data_seg[i].lan_head.packet_num));
#endif 			
			sendb_ptr++;
			if(sendb_ptr==MAX_SEND_NUM)  sendb_ptr=0;

			pkg_num++;
			if(pkg_num>=MAX_PKG_NUM)	pkg_num=0;
			
		}/* end while */
		
	}/* end for(;;) */
	
	udp_close(sock_a);
	if( g_sys_dualnet == YES )
	{
		udp_close(sock_b);
	}
	return(TRUE);			
}

/*********************************************************************************************************#
	get random package id, return: the pkg_num of head pointer add 1	
********************************************************************************************************#*/

unsigned short GetRandID()
{
	unsigned short	RanID;		
#ifdef MSWIN32
	srand( (unsigned)time( NULL ) );
	RanID= (unsigned short)(rand()%MAX_PKG_NUM);
#else
	RanID= time( NULL ) * 77  % MAX_PKG_NUM;
#endif
	if(RanID==0) RanID =1;
	return RanID;
/*			
	unsigned short lastid;						
	short		head;
	head=shm_send_addr->head;
	if(head<1)
	{
		return 0;
	}
	lastid=shm_send_addr->shm_data_seg[head-1].lan_head.packet_num;
	
	lastid++;
	if(lastid>=MAX_PKG_NUM || lastid<0)	
	{
		return 0;
	}
	else
	{
		return lastid;
	}
*/
	
}

/*********************************************************************************************************#
	if the pkg_num given exists in queue,return true, otherwise, return false
********************************************************************************************************#*/

BOOL IsExistID(char src_id, unsigned short pkg_num,PKGID *recvid)
{
	short i;
	for(i=0;i<MAX_IDBUF_NUM;i++)
	{
		if( recvid[i].src_id==src_id && recvid[i].pkg_num==pkg_num)
			return TRUE;
	}
	return FALSE;
}
