/*########################################################################################################################
FILENAME:	nettcp.c

FUNCTION:	
	
	source code of tcp sending and tcp receiving,include:
	BOOL Net_recv(SOCKET sock);
	BOOL Net_send();  

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

########################################################################################################################*/

#include "../inc/lanhead_all.h"

/*############## Net_recv function, receive every seg data broadcasted and save it in shm_recv continually #############*/ 
int net_recv(int sockfd)
{
	short 		i,dcount,length;
	SHM_DATA_SEG	datapipe;
	short		buf_num;	
	time_t 		return_t;	
	extern   void    sig_term(int);	/*modified 2003.4.11 zy */
	
	signal(SIGKILL,sig_term);
	signal(SIGINT,sig_term);
	signal(SIGTERM,sig_term);
	
	for(;;)
	{                
	  	/* receive data */
		dcount=Tcp_recv(sockfd,(char *)&datapipe.lan_head,LANH_SIZE);
		if (dcount<=0)   break;
		dcount=Tcp_recv(sockfd,datapipe.data,datapipe.lan_head.length);
		length=datapipe.lan_head.length;
#ifdef _BIG_ENDIAN_
			Swap2Byte(&length);
#endif
		if (dcount!=length)
		{
			continue;
		}
		for(;;)
		{
			if (shm_recv_addr->flag==IDLE_FLAG)
				break;
				
			printf("shm_recv_addr->flag is locked\n");
			
			usleep(50000);	/*added 2003.4.11 zy */
		}
		
		shm_recv_addr->flag=LOCK_FLAG;
		buf_num=shm_recv_addr->head;
		memset(shm_recv_addr->shm_data_seg[buf_num].data,0,MAX_BUF_SIZE);
		memcpy(shm_recv_addr->shm_data_seg[buf_num].data,datapipe.data,dcount);
		shm_recv_addr->shm_data_seg[buf_num].lan_head.dp_type=datapipe.lan_head.dp_type;
		shm_recv_addr->shm_data_seg[buf_num].lan_head.dest_id=datapipe.lan_head.dest_id;
		shm_recv_addr->shm_data_seg[buf_num].lan_head.src_id=datapipe.lan_head.src_id;
		shm_recv_addr->shm_data_seg[buf_num].lan_head.length=datapipe.lan_head.length;
#ifdef DEBUG
		printf ("length=%d ",length);
		printf ("dp_type=%d	",shm_recv_addr->shm_data_seg[buf_num].lan_head.dp_type);
		printf ("dest_id=%d	",shm_recv_addr->shm_data_seg[buf_num].lan_head.dest_id);
		printf ("src_id=%d\n",shm_recv_addr->shm_data_seg[buf_num].lan_head.src_id);
		for(i=0;i<dcount;i++)
		{
			printf ("%d=",shm_recv_addr->shm_data_seg[buf_num].data[i]);
		}
		printf ("\n\n");
#endif		

		buf_num++;
		if(buf_num=MAX_RECV_NUM)	buf_num=0;
		shm_recv_addr->head=buf_num;
		Set_Flag(sem_id,RECV_TURN); 
	}
	return(TRUE);
}

/*#################### Net_send function, broadcast every seg in shm_send continually #############*/ 

int Net_send()
{
	int	count,sockfd,data_type;
	int	ret;	
	int	comm_null;
	LCUID	dev_id;
	char	dest_stn,dest_id,type;
	short	status;
	short	length,hot_net,standby_net,i,send_ptr;
	short	cmd_length;	/* added 2003.4.11 zy */
	char           	hostname[20];
	char		stn_name[STATION_NAME_SIZE];
	char		dev_name[GROUP_NAME_SIZE];
	char            buf[60];
	LANH_PTR        head;
	SHM_DATA_SEG	datapipe;
	extern   void    sig_term(int);
	signal(SIGKILL,sig_term);
	signal(SIGINT,sig_term);
	signal(SIGTERM,sig_term);
	send_ptr=shm_send_addr->head;
	for(;;)
	{
		gethostname(hostname,20);
		
		if(WaitforSem(SEM_RECVDATA, -1) == -1)
		{
			Err_log(DPS_MODULE, "Fail to wait recv_data semaphore!\n");
			exit(-1);
		}
			
	/*	usleep(50000);   */
	
		while(send_ptr!=shm_send_addr->head)
		{
			i=send_ptr;
			send_ptr++;
			if (send_ptr==MAX_SEND_NUM)  send_ptr=0;
			dest_stn=shm_send_addr->shm_data_seg[i].lan_head.dest_stn;
			dest_id=shm_send_addr->shm_data_seg[i].lan_head.dest_id;
			
			/*added by yyp 05.12.12 */
			dev_id.devid=dest_id;
			if(dest_id==0) continue;
			length=shm_send_addr->shm_data_seg[i].lan_head.length;
#ifdef _BIG_ENDIAN_
			Swap2Byte(&length);
#endif
			if(length<=0 || length >COMM_BUF_SIZE )
			{
				continue;
			}
			
			type=shm_send_addr->shm_data_seg[i].data[1];
			if (shm_send_addr->shm_data_seg[i].lan_head.src_id<=MAX_LCU_NUM)
			{
				shm_send_addr->shm_data_seg[i].lan_head.src_id=GetHost_Cmd();
			}
				
			printf ("开始连接第1条网\n");
			ret=-1;
			hot_net=rdb_net(dest_stn,dest_id);
			if (hot_net==-1)
			{
				printf ("nettcp:: %s.SYSMSG.NET_WK_STA error in db \n",dev_name);
				hot_net=0;
			}
			if (type==C_NET_SWITCH)
			{
				if ((shm_send_addr->shm_data_seg[i].data[4] & 16) ==0)
				{		
					hot_net=0;
				}
				else
				{
				hot_net=1;
				}
			}					
			for(count=0;count<3;count++)
			{
				sockfd=Tcp_open(TCP_PORT,dest_id,hot_net);	
				if (sockfd>0)		break;
				usleep(50000);
			}
			if (sockfd>0)
			{
				ret=Tcp_send(sockfd,(char *)&shm_send_addr->shm_data_seg[i],(LANH_SIZE+length));
			}
			if (sockfd<0 && dest_id<=MAX_LCU_NUM)
			{
				tcp_close(sockfd);
				printf ("LAN_nettcp.c::1send data to  dev%d  via  %d failure\n\n",dest_id,hot_net); 

				if(GetStnNameByID(dest_stn,stn_name)==-1) continue;
				if(GetGroupNameByID(dest_stn,dest_id,dev_name)==-1) continue;
				strcpy(buf,stn_name);
				strcat(buf,".");
				strcat(buf,dev_name);
				strcat(buf,".SYSMSG.");
				strcat(buf,netname[hot_net]);
				gethostname(hostname,20);
				strcat(buf,hostname);
				if(GetPtIdByNameStr(buf,&pt)==-1) continue;
				if (MAX_NET_NUM==1 || type==C_NET_SWITCH)
				{
					if (netmsg.err_code != -1) 
					   BdcastMsg(dest_stn,dest_id,IND,pt.pt_id,LINK_ERR);
					else 
					   printf("%s not in DB\n",buf);
				}
				if (MAX_NET_NUM==2 && type!=C_NET_SWITCH)
				{
					if (hot_net==0)
					{
						hot_net=1;
					}
					else
					{
						hot_net=0;
					}
					printf ("开始连接第2条网\n");
					for(count=0;count<3;count++)
					{
						sockfd=Tcp_open(TCP_PORT,dest_id,hot_net);
						if (sockfd>0)            break;
						/*usleep(50000); */
					}
					if (sockfd>0)
					{
#ifdef DEBUG
						printf ("LAN_nettcp.c::2connect dev%d  via  %d success\n\n",dest_id,hot_net);
#endif
						datapipe.lan_head.length = cmd_length=10 ;
#ifdef _BIG_ENDIAN_
						Swap2Byte(&datapipe.lan_head.length);
#endif
						datapipe.lan_head.src_id=GetHost_Cmd();
						datapipe.data[0]=dest_id;
						datapipe.data[1]=C_NET_SWITCH;

						strcpy(buf,stn_name);
						strcat(buf,".");
						strcat(buf,dev_name);
						strcat(buf,".SYSMSG.NET_WK_STA");
						if(GetPtIdByNameStr(buf,&pt)==-1) continue;
						memcpy(datapipe.data+2,&(pt.pt_id),2);

#ifdef _BIG_ENDIAN_
						Swap2Byte(&datapipe.data[2]);
#endif 					
						if (hot_net==0){ datapipe.data[4]=0;}
						if (hot_net==1){ datapipe.data[4]=16;}
						datapipe.data[5]=0;
						datapipe.data[6]=0;
						datapipe.data[7]=0;
						datapipe.data[8]=0;
						datapipe.data[9]=MAX_USERS-1;    /* usr_id */
						ret=Tcp_send(sockfd,(char *)&datapipe,(LANH_SIZE+cmd_length));
						tcp_close(sockfd);
						printf ("Network Switch\n");
						wdb_net(dev_name,hot_net*4);

						if (netmsg.err_code != -1)
 							BdcastMsg(dev_id,IND,pt.pt_id,LINK_ERR);
						else
							printf("%s not in  DB\n",buf); 
						BdNetHost(dev_id,hot_net*16);
						sleep(3);
						sockfd=tcp_open(TCP_PORT,dest_id,hot_net);
						if (sockfd>0){
#ifdef DEBUG
        	                			printf ("LAN_nettcp.c::connect dev%d  via  %d success\n\n",dest_id,hot_net);
#endif	
						
							ret=Tcp_send(sockfd,(char *)&shm_send_addr->shm_data_seg[i],(LANH_SIZE+length));
						}
					}
					if (  sockfd<0)
					{
						tcp_close(sockfd);
 						printf ("LAN_nettcp.c::connect to dev%d by %d#net failure\n",dest_id,hot_net);
						if (netmsg.err_code != -1) 
							 BdcastMsg(dev_id,IND,pt.pt_id,LINK_ERR);
						else
							printf("%s not in DB\n",buf);


						strcpy(buf,stn_name);
						strcat(buf,".");
						strcat(buf,dev_name);
						strcat(buf,".SYSMSG.");
						strcat(buf,netname[hot_net]);
						gethostname(hostname,20);
 						strcat(buf,hostname);
						if(GetPtIdByNameStr(buf,&pt)!=-1) 
 							BdcastMsg(dest_stn,dest_id,IND,pt.pt_id,LINK_ERR);
						else
							printf("%s not in DB\n",buf);
					}
				}/* end if */	
			}/* end if */
			if (sockfd<0)
			{
				if(type==C_NET_SWITCH) continue;			

				printf ("Network Fault\n");
				strcpy(buf,stn_name);
				strcat(buf,".");
				strcat(buf,dev_name);
				strcat(buf,".SYSMSG.LINK_STA");
				if(GetPtIdByNameStr(buf,&pt)!=-1) 	
					BdcastMsg(dest_stn,dest_id,IND,pt.pt_id,LCU_ERR);
				else
					printf("%s not in DB\n",buf);
				continue;
			}
			else 
			{
				tcp_close(sockfd);
				head.dest_id=0;
				head.dp_type=COMM_RECORD;
				head.src_id=shm_send_addr->shm_data_seg[i].lan_head.src_id;
				head.length=shm_send_addr->shm_data_seg[i].lan_head.length;
				memcpy(buf,shm_send_addr->shm_data_seg[i].data,length);
				memset(shm_send_addr->shm_data_seg[i].data,0,COMM_BUF_SIZE);
				lan_out(buf,head);

				strcpy(buf,stn_name);
				strcat(buf,".");
				strcat(buf,dev_name);
				strcat(buf,".SYSMSG.");
				strcat(buf,netname[hot_net]);
 				gethostname(hostname,20);
				strcat(buf,hostname);

				if(GetPtIdByNameStr(buf,&pt)==-1) 	continue;

				if(GetIndPtStateByNameStr(buf,&status)==-1) 	continue;
				
				if(status != LCU_ON)
					BdcastMsg(dest_stn,dest_id,IND,pt.pt_id,LCU_ON);

			}/* end if */
			/* sleep(3); */
		}/* end while */
	}/* end for(;;)	*/	
	return(TRUE);
}


