
/************************************************************
*  FILE NAME:           Host_app_fun.c      				*
*  FUNCTION:            Controlling Station					*
*						Application Layer Functions			*
*  's NAME :  			              						*
*  DATE:                May,2000                			*
*															*
************************************************************/


#include	"..\inc\sc1801Host.h"

#define	DEBUG

extern	unsigned char	sendbuf[1024];
ASDUCOMDDATA	send_data;
ASDUDATA		recv_data;
extern	HANDLE	hRecvSemaphore;
extern	BOOL	resetlinkf,initflag,pollflag,comdflag;
extern	BOOL	counterflag,ClockSyncflag,interrogationflag;
extern	int		cmd_ack,localinitfg;
extern unsigned char	rtuconfig[16];
extern unsigned char	pntname[16][24][NAME_SIZE];
extern  float	lorange[16][12],hirange[16][12];
extern unsigned short	databand,bytecount;

extern	BOOL	lan_out();
extern	char	GetHost_Cmd(void);
extern	char	infobuf[1024];


BOOL	GetRTUConfig(void)
{
	FILE	*fp;
	char	boardstr[24];
	short	j;

 	if((fp=fopen("/home/ems/h9000/sc1801/Rtu_config.def","r"))==NULL)
 		{
 		sprintf(infobuf,"can't open file /home/ems/h9000/sc1801/Rtu_config.def\n");
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
		return(FALSE);          
		}

	rewind(fp);	
	
	for(j=0;j<16;j++)
	{
		fscanf(fp,"%s",boardstr);
		if(strcmp(boardstr,"DI")==0)
			rtuconfig[j]=DI;
		else if(strcmp(boardstr,"AI")==0)
			rtuconfig[j]=AI;
		else if(strcmp(boardstr,"DO")==0)
			rtuconfig[j]=DO;
		else if(strcmp(boardstr,"TRD")==0)
			rtuconfig[j]=TRD;
		else if(strcmp(boardstr,"AO")==0)
			rtuconfig[j]=AO;
		else if(strcmp(boardstr,"PA")==0)
			rtuconfig[j]=PA;
		else if(strcmp(boardstr,"CCO")==0)
			rtuconfig[j]=CCO;
		else
			rtuconfig[j]=0;
		printf("In read RTU Config slot %d %s\n",j,boardstr);
	}
	
	fclose(fp);
}

void GetDBNameRangeTable()		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	
	char			name_buf2[NAME_SIZE];

	short		j,n;

	for(j=0;j<16;j++)
	{
		//printf("In GetDBNameRangeTable slot2 %d\n",j);
		switch(rtuconfig[j])
		{
		case DI:
			if((fp=fopen(slotdefname[j],"r"))==NULL)
			{
				printf("can't open file %s\n",slotdefname[j]);
				for(n=0;n<24;n++)
				{
					strcpy(pntname[j][n],"NULL");
				}

				continue;
			}

			rewind(fp);	
			for(n=0;n<24;n++)
			{
				fscanf(fp,"%s",pntname[j][n]);
				if(strcmp(pntname[j][n],"NULL")==0)
					continue;
#ifdef DEBUG
				printf("%d slot, %d pnt: %s\n",j,n,pntname[j][n]);
#endif
				strcpy(name_buf2,pntname[j][n]);
				
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("%d slot2, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
				}
			}
			fclose(fp);

			break;

		case AI:
			if((fp=fopen(slotdefname[j],"r"))==NULL)
			{
				printf("can't open file %s\n",slotdefname[j]);
				for(n=0;n<12;n++)
				{
					strcpy(pntname[j][n],"NULL");
					lorange[j][n] = -RESOLUTE/2;
					hirange[j][n] = RESOLUTE/2;
				}
				continue;
			}

			rewind(fp);	
			for(n=0;n<12;n++)
			{
				fscanf(fp,"%s  %f %f",pntname[j][n],&lorange[j][n],&hirange[j][n]);
				if(strcmp(pntname[j][n],"NULL")==0)
					continue;
#ifdef DEBUG
				printf("%d slot, %d pnt: %s,%5.2f,%5.2f\n",j,n,pntname[j][n],lorange[j][n],hirange[j][n]);
#endif
				strcpy(name_buf2,pntname[j][n]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
				}
			}
			fclose(fp);

			break;

		case DO:
			if((fp=fopen(slotdefname[j],"r"))==NULL)
			{
				printf("can't open file %s\n",slotdefname[j]);
				for(n=0;n<24;n++)
				{
					strcpy(pntname[j][n],"NULL");
				}
				continue;
			}

			rewind(fp);	
			{
				fscanf(fp,"%s",pntname[j][0]);
				if(strcmp(pntname[j][0],"NULL")==0)
					continue;
#ifdef DEBUG
				printf("%d slot, %d pnt: %s\n",j,0,pntname[j][0]);
#endif
				strcpy(name_buf2,pntname[j][0]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("%d slot, %d pnt: %s No this point in the database file!\n",j,0,name_buf2);
				}
			}
			fclose(fp);

			break;
		
		case TRD:
			if((fp=fopen(slotdefname[j],"r"))==NULL)
			{
				printf("can't open file %s\n",slotdefname[j]);
				for(n=0;n<24;n++)
				{
					strcpy(pntname[j][n],"NULL");
				}
				continue;
			}

			rewind(fp);	

			for(n=0;n<16;n++)
			{
				fscanf(fp,"%s",pntname[j][n]);
				if(strcmp(pntname[j][n],"NULL")==0)
					continue;
#ifdef DEBUG
				printf("%d slot2, %d pnt: %s\n",j,n,pntname[j][n]);
#endif
				strcpy(name_buf2,pntname[j][n]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
				}
			}
			fclose(fp);
			break;

		case AO:
			if((fp=fopen(slotdefname[j],"r"))==NULL)
			{
				printf("can't open file %s\n",slotdefname[j]);
				for(n=0;n<12;n++)
				{
					strcpy(pntname[j][n],"NULL");
					lorange[j][n] = -RESOLUTE/2;
					hirange[j][n] = RESOLUTE/2;
				}
				continue;
			}

			rewind(fp);	
			for(n=0;n<8;n++)
			{
				fscanf(fp,"%s %f %f",pntname[j][n],&lorange[j][n],&hirange[j][n]);
				if(strcmp(pntname[j][n],"NULL")==0)
					continue;
#ifdef DEBUG
				printf("%d slot, %d pnt: %s %5.2f %5.2f\n",j,n,pntname[j][n],lorange[j][n],hirange[j][n]);
#endif
				strcpy(name_buf2,pntname[j][n]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
				}
			}
			fclose(fp);

			break;

		case PA:
			if((fp=fopen(slotdefname[j],"r"))==NULL)
			{
				printf("can't open file %s\n",slotdefname[j]);
				for(n=0;n<24;n++)
				{
					strcpy(pntname[j][n],"NULL");
				}
				continue;
			}

			rewind(fp);	
			for(n=0;n<8;n++)
			{
				fscanf(fp,"%s",pntname[j][n]);
				if(strcmp(pntname[j][n],"NULL")==0)
					continue;
#ifdef DEBUG
				printf("%d slot, %d pnt: %s\n",j,n,pntname[j][n]);
#endif
				strcpy(name_buf2,pntname[j][n]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
				}
			}
			fclose(fp);

			break;

		case CCO:
			if((fp=fopen(slotdefname[j],"r"))==NULL)
			{
				printf("can't open file %s\n",slotdefname[j]);
				for(n=0;n<24;n++)
				{
					strcpy(pntname[j][n],"NULL");
				}
				continue;
			}

			rewind(fp);	
			for(n=0;n<8;n++)
			{
				fscanf(fp,"%s",pntname[j][n]);
				if(strcmp(pntname[j][n],"NULL")==0)
					continue;
#ifdef DEBUG
				printf("%d slot, %d pnt: %s\n",j,n,pntname[j][n]);
#endif
				strcpy(name_buf2,pntname[j][n]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
				}
			}

			fclose(fp);

			break;

		default :
			printf("%d slot config %d NULL!\n",j,rtuconfig[j]);
			for(n=0;n<12;n++)
			{
				strcpy(pntname[j][n],"NULL");
				lorange[j][n] = -RESOLUTE/2;
				hirange[j][n] = RESOLUTE/2;
			}

			for(n=12;n<24;n++)
			{
				strcpy(pntname[j][n],"NULL");
			}
		}	//switch(rtuconfig[j])
	}		//for
}

/*
void	LanBcCmdError(dev,type,p,stat)
char    dev,type;
short   p,stat;
{
	LANBCDATA	lanbuf;
	LANH_PTR	head;

	lanbuf.ptr.dev_id   = dev;
	lanbuf.ptr.data_type   = type;
	lanbuf.ptr.point_id=p;
	lanbuf.status=stat;
	
	head.length=sizeof(LANBCDATA);
	head.dp_type=BJ_DB;
	head.dest_id=0;
	head.src_id=GetHost_Cmd();
	
	lan_out(&lanbuf,head);
}
*******zwj**/

void	SC1801_Recv()
{
	//LANBCDATA	lanbcdata[4];
	//LANH_PTR	lanhead;

	DWORD	dwWaitRecvASDU;
	USHORT	recvdatapt;

	/*
	int 	i,data_type,datapoint,lanpt;
	char	dbname[36];
	float	value;
	DWORD	dwcount;
	ANLG_DATA	anadata;
	struct	tm	*tmpt;
	time_t		tim;
	*/

	printf("============SC1801_Recv thread start!!!===========\n");

	recvdatapt = 0;

	for(;;)
	{
		dwWaitRecvASDU = WaitForSingleObject(hRecvSemaphore,INFINITE);

#ifdef DEBUG
		printf("SC1801_Recv: RecvSempahore set, remote ASDU has received...\n");
#endif

		while(recvdatapt != recv_data.pnt)
		{
			recvdatapt++;
			if( recvdatapt >= ASDU_ENTRY_NUMBER )
				recvdatapt = 0;
		}	//while(recvdatapt!= recv_data.pnt)
	}		//for(;;)
}


void	SC1801_Send()
{
	//DWORD	dwWaitComSem;
	//HANDLE	sem_com_id;

	short	cmd_end,sd_pnt;
	char	cmd_type;

	/*
	POINTER	ptr;
	short	cmd_stat,cmd_data,normv,cmd_pnt,length;
	float	ftemp,setpv;
	char	name[36],host_id;

	DMS_COMMON 	dbptr;
	ANLG_DATA	anadata;
	*/

	cmd_end = shm_recv_addr->head;
	printf("------------SC1801_Send thread start!!!-----------\n");

	/*
	sem_com_id=OpenSemaphore(SEMAPHORE_MODIFY_STATE|SEMAPHORE_ALL_ACCESS|SYNCHRONIZE,
							TRUE,"sem_recv_com");

	if(sem_com_id==NULL)
	{
#ifdef DEBUG
		printf("Error in OpenSemaphore sem_recv_com:%d",GetLastError());
#endif
	}
	*/

	sd_pnt = 0;
	cmd_type = 0;
	//initflag = 0;

	for(;;)
	{
		//dwWaitComSem = WaitForSingleObject(sem_com_id,INFINITE);

		Sleep(30000);

		send_data.data[sd_pnt][0] = cmd_type;

		//sd_pnt = send_data.pnt;
		sd_pnt ++;
		if(sd_pnt>=CMD_ENTRY_NUMBER)
			sd_pnt = 0;
		send_data.pnt = sd_pnt;

		cmd_type++;
		if(cmd_type>12)
			cmd_type = 0;

		/*
		while( cmd_end!=shm_recv_addr->head )
		{
			cmd_pnt = cmd_end;
			cmd_end++;
			if(cmd_end >= MAX_RECV_NUM)
				cmd_end = 0;

			if(shm_recv_addr->shm_recv_seg[cmd_pnt].lan_head.dp_type != COMM_RECORD)
				continue;
	   		
	   		length=shm_recv_addr->shm_recv_seg[cmd_pnt].lan_head.length;
			if(length<10)			  //the length of command buf is 10
				continue;
			
			ptr.dev_id=shm_recv_addr->shm_recv_seg[cmd_pnt].data[0];
			cmd_type=shm_recv_addr->shm_recv_seg[cmd_pnt].data[1];

			//if(ptr.dev_id<0 ||ptr.dev_id>(MAX_LCU_NUM+1) || ptr.data_type>12)
			if(ptr.dev_id < MIN_RMT_DEV_ID || ptr.dev_id > MAX_RMT_DEV_ID || cmd_type < 24)
				continue;

			if(pollflag==0)
			{
				LanBcCmdError(SYS_RTU,DPS_SYSMSG,CMDLINKERR,4);
				continue;
			}

			memcpy((char *)&ptr.point_id,&shm_recv_addr->shm_recv_seg[cmd_pnt].data[2],2);	
			memcpy((char *)&cmd_stat,&shm_recv_addr->shm_recv_seg[cmd_pnt].data[4],2);	
			memcpy((char *)&cmd_data,&shm_recv_addr->shm_recv_seg[cmd_pnt].data[6],2);	
			ptr.data_type = shm_recv_addr->shm_recv_seg[cmd_pnt].data[8];
			host_id = shm_recv_addr->shm_recv_seg[cmd_pnt].data[9];

#ifdef DEBUG
			printf("dev_id=%d  cmd_type=%d point_id=%d cmd_stat=%d cmd_data=%d type=%d host_id=%d\n",
					ptr.dev_id, cmd_type,ptr.point_id,cmd_stat,cmd_data,ptr.data_type,host_id);
#endif
			sd_pnt = send_data.pnt;

			switch	( cmd_type ) 
			{
			case C_SWITCH:
			case C_BREAKER:
			case C_DEVICE:
			case C_UNIT:
				send_data.data[sd_pnt][0] = C_DC_NA;	//double point control command
				base_addr = SW_CMD_BASE_ADDR;
				memset(name,0,36);
				strcpy(name,GetDevNameStr(ptr.dev_id));
				strcat(name,DATATYPE[ptr.data_type]);

				if(cmd_type == C_UNIT)
				{
					if((cmd_stat&0xF0)==0x10)			//DCO/RCO: S/E QU[3..7] DCS/RCS[1..2]
						send_data.data[sd_pnt][6] = 2;	//Start unit command	
					else if((cmd_stat&0xF0)==0x20)
						send_data.data[sd_pnt][6] = 1;	//Stop unit command
					else
						continue;						//other operation not allowed

					strcat(name,"JZ_STAT");
				}
				else
				{
					if((cmd_stat&0xF0)==0x10)			//DCO/RCO: S/E QU[3..7] DCS/RCS[1..2]
						send_data.data[sd_pnt][6] = 1;	//Off/next step lower command	
					else
						send_data.data[sd_pnt][6] = 2;	//On/next step high command

					ptr.point_id = cmd_data;
					if((dbptr=(DMS_COMMON *)ReadEntryByID(&ptr))==(DMS_COMMON*)-1)
					{ 
#ifdef	DEBUG
						printf("\ndevid=%d type=%d id=%d no this point in db!\n",
							ptr.dev_id,ptr.data_type,ptr.point_id); 
#endif	
						continue;
					}

					strcat(name,dbptr->entry.ind.fixed.entry_name);
				}

				if(cmd_type ==C_DEVICE)
				{
					for(i=0;i<stepctrl.number;i++)
					{
						if(strcmp(name,stepctrl.name[i])==0)
							break;
					}

					if(i<stepctrl.number)
					{
						send_data.data[sd_pnt][0] = C_RC_NA;	//regulating step command
						base_addr = STEP_CMD_BASE_ADDR;
					}
				}

				if(send_data.data[sd_pnt][0] != C_RC_NA)
				{	 
					for(i=0;i<dswctrl.number;i++)
					{
						if(strcmp(name,dswctrl.name[i])==0)
							break;
					}

					if(i>=dswctrl.number)
					{
						printf("No this point %s in control table!\n",name);
						continue;
					}
				}

				send_data.length[sd_pnt] = 7;			//cmd data length of ASDU
				send_data.data[sd_pnt][1] = 1;			//number of object(variable structure)
				send_data.data[sd_pnt][2] = ACT;		//cause of transmit
				send_data.data[sd_pnt][3] = 0;			//common address of ASDU

				ob_addr = i + base_addr;

				send_data.data[sd_pnt][4] = (unsigned char)ob_addr;		//object address (low octet)
				send_data.data[sd_pnt][5] = (unsigned char)((ob_addr>>8) & 0xFF);	//object address (high octet)

				sd_pnt ++;
				if(sd_pnt>=CMD_ENTRY_NUMBER)
					sd_pnt = 0;

				send_data.pnt = sd_pnt;

				break;

			case C_APOWER:
			case C_RPOWER:
				send_data.data[sd_pnt][0] = C_SE_NA;	//set-point command
				base_addr = SETP_CMD_BASE_ADDR;

				memset(name,0,36);
				strcpy(name,GetDevNameStr(ptr.dev_id));
				strcat(name,"ANA");

				if(cmd_type == C_APOWER)
				{
					strcat(name,"YG_MW");
				}

				if(cmd_type == C_RPOWER)
				{
					strcat(name,"WG_MVAR");
				}

				for(i=0;i<setpt.number;i++)
				{
					if(strcmp(name,setpt.name[i])==0)
						break;
				}

				if(i>=setpt.number)
				{
					printf("No this point %s in set point table!\n",name);
					continue;
				}

				send_data.length[sd_pnt] = 9;			//cmd data length of ASDU
				send_data.data[sd_pnt][1] = 1;			//number of object(variable structure)
				send_data.data[sd_pnt][2] = ACT;		//cause of transmit
				send_data.data[sd_pnt][3] = 0;			//common address of ASDU

				ob_addr = i + base_addr;
				send_data.data[sd_pnt][4] = (unsigned char)ob_addr;			//object address (low octet)
				send_data.data[sd_pnt][5] = (unsigned char)((ob_addr>>8) & 0xFF);	//object address (high octet)

				anadata.status = cmd_stat;
				anadata.data = cmd_data;
				setpv = *GetAnaValue ( &anadata );

				ftemp = setpv * 0x7FFF / setpt.param[i]; //float value to Normalized

				normv = (short)ftemp;

				send_data.data[sd_pnt][6] = (unsigned char)normv;			//set point data (low octet)
				send_data.data[sd_pnt][7] = (unsigned char)((normv>>8) & 0xFF);	//set point data (high octet)
				send_data.data[sd_pnt][8] = 0;

				sd_pnt ++;
				if(sd_pnt>=CMD_ENTRY_NUMBER)
					sd_pnt = 0;

				send_data.pnt = sd_pnt;
				
				break;
			}	//switch(cmd_type)
		}		//while(shm_recv_addr->head!= cmd_end)
		*/
	}			//for(;;)
}				//SC1801_Send()
