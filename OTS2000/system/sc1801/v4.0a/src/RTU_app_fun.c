/************************************************************
*  FILE NAME:           RTU_app_fun.c      				    *
*  FUNCTION:            Controlled Station					*
*						Application Layer Function			*
*  Version :            Win3.0a-->UNIX4.0a  by  zwj		    *
*  DATE:                2007.12                			    *
*															*
************************************************************/
#include "../inc/sc1801RTU.h"

#define	DEBUG
#define BY_RECORD

/*float	Datcvt();*/
BOOL	ResponseNAK();
BOOL	ReportRTUConfig();
BOOL	RepeatTransmit();
BOOL	CommandSend();
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

/*extern	unsigned char	sendbuf[1024];*/
ASDUCOMDDATA	recv_data;
ASDUDATA		send_data;
extern	unsigned char rtu_status,rtu_jumpter;
extern	char	infobuf[1024];
extern unsigned char	rtuconfig[16];
extern unsigned char	pntname[16][32][36];
extern unsigned short	soenum;
extern SOEDEF	soedef[16*24];
extern SOEBUF	soebuf;
extern float	lorange[16][12],hirange[16][12];
extern unsigned short	databand;
extern unsigned char	orgbuf[16*24];

extern	BOOL	lan_out();
extern	char	GetHost_Cmd(void);

int		soeflag[16];

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
		soeflag[j]=0;
		fscanf(fp,"%s",boardstr);
		if(strcmp(boardstr,"DI")==0)
			rtuconfig[j]=DI;
		else if(strcmp(boardstr,"SI")==0)
		{
			rtuconfig[j]=DI;
			soeflag[j]=1;
			rtu_jumpter ++;
		}
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
	return(TRUE);          
}

BOOL	GetComConfig(void)
{
	FILE	*fp;
	int		iTemp,iRet;
	char	strFlag[80];

 	if((fp=fopen("/home/ems/h9000/sc1801/Rtu_Com_config.def","r"))==NULL)
 		{
 		sprintf(infobuf,"can't open file /home/ems/h9000/sc1801Rtu_Com_config.def\n");
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
		return(FALSE);          
		}
		  
	rewind(fp);	

	for(;;)
	{
		fscanf(fp,"%s",strFlag);
		if(feof(fp))
		{
			sprintf(infobuf,"Config file sc1801Rtu_Com_config End!\n");
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
			fclose(fp);
			return(TRUE);
		}
		if(strcmp(strFlag,"STATION_ID:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file sc1801Rtu_Com_config Error! STATION_ID=%d\n",iTemp);
				STATION_ID = 1;
				continue;
			}
			if((iTemp<=0) || (iTemp>254))
			{
				printf("Config file sc1801Rtu_Com_config Error! STATION_ID=%d\n",iTemp);
				STATION_ID = 1;
			}
			else
			{
#ifdef DEBUG
				printf("STATION_ID = %d\n",iTemp);
#endif
				STATION_ID = iTemp;
			}
		}
		
		else if(strcmp(strFlag,"INTERFACE:")== 0)
		{
			iRet = fscanf(fp,"%s",strDev);
			if(iRet<=0)
			{
				printf("Config file sc1801Rtu_Com_config Error! INTERFACE=%s\n",strDev);
				strcpy(strDev,COM_PORT);
				continue;
			}
#ifdef DEBUG
			printf("INTERFACE = %s\n",strDev);
#endif

		}
		else if(strcmp(strFlag,"BAUDRATE:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file sc1801Rtu_Com_config Error! BAUDRATE = %d\n",iTemp);
				nBaudRate = Com_Baud_Rate;
				continue;
			}
			if((iTemp<=300) || (iTemp>19200) || (iTemp%100 != 0))
			{
				printf("Config file sc1801Rtu_Com_config Error! BAUDRATE = %d\n",iTemp);
				nBaudRate = Com_Baud_Rate;
			}
			else
			{
#ifdef DEBUG
				printf("BAUDRATE = %d\n",iTemp);
#endif
				nBaudRate = iTemp;
			}
		}
		else if(strcmp(strFlag,"DATA_BITS:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file sc1801Rtu_Com_config Error! DATA_BITS = %d\n",iTemp);
				Databits = 8;
				continue;
			}
			
			else
			{
#ifdef DEBUG
				printf("DATA_BITS = %d\n",iTemp);
#endif
				Databits = iTemp;
			}
		}
		else if(strcmp(strFlag,"PARITY:")== 0)
		{
			iRet = fscanf(fp,"%s",Parity);
			if(iRet<=0)
			{
				printf("Config file sc1801Rtu_Com_config Error! PARITY = %s\n",Parity);
				strcpy(Parity,ODD);
				continue;
			}
			
#ifdef DEBUG
				printf("PARITY = %s\n",Parity);
#endif

				if(strcmp(Parity,"NONE")== 0)
					par=0;
#ifdef SUN_OS
				if(strcmp(Parity,"EVEN")== 0)
					par=1;
				if(strcmp(Parity,"ODD")== 0)
					par=2;
#else
				if(strcmp(Parity,"EVEN")== 0)
					par=2;
				if(strcmp(Parity,"ODD")== 0)
					par=1;
#endif

		}
		else if(strcmp(strFlag,"STOP_BITS:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file sc1801Rtu_Com_config Error! STOP_BITS = %d\n",iTemp);
				Stopbits = 0;
				continue;
			}
			
			else
			{
#ifdef DEBUG
				printf("STOP_BITS = %d\n",iTemp);
#endif
				Stopbits = iTemp;
			}
		}

		else
		{
			printf("##### %s  ",strFlag);
			fscanf(fp,"%s",strFlag);
			printf("%s ##### ERROR!\n",strFlag);
		}
	}
	fclose(fp);
	return(TRUE);          
}

void GetDBNameRangeTable()		
{
	FILE			*fp;
	DMS_COMMON		dbentry2;
	/*int				data_type2;*/
	char			name_buf2[NAME_SIZE];
	unsigned short	savpnt;

	unsigned char	j,n;
	char			lgname[48];

	soenum = savpnt = 0;

	for(j=0;j<16;j++)
	{
/*		printf("In GetDBNameRangeTable slot2 %d\n",j);*/
		switch(rtuconfig[j])
		{
		case DI:
			 	if((fp=fopen(slotdefname[j],"r"))==NULL)
 				{
 					sprintf(infobuf,"can't open file %s\n",slotdefname[j]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					for(n=0;n<24;n++)
					{
						strcpy(pntname[j][n],"NULL");
					}
					savpnt = savpnt + 24;
					continue;
				}
				rewind(fp);	
				for(n=0;n<24;n++)
				{
					fscanf(fp,"%s %s",pntname[j][n],lgname);
					if(strcmp(pntname[j][n],"NULL")==0)
					{
						savpnt ++;
						continue;
					}
#ifdef DEBUG
					printf("%d slot, %d pnt: %s %s\n",j,n,pntname[j][n],lgname);
#endif
					strcpy(name_buf2,pntname[j][n]);
					if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
					{
						sprintf(infobuf,"%d slot2, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					}
					else
					{
						if(soeflag[j])
						{
							soedef[soenum].slot = j;
							soedef[soenum].pnt = n;
							soedef[soenum].orgpnt = savpnt/6;
							soedef[soenum].ptr = dbentry2.point;/***zwj***/
							soedef[soenum].status = dbentry2.var.ind.status.rt_state;
							soenum ++;
						}
					}
					savpnt ++;
				}
				fclose(fp);
 				printf("sc1801::soe_num=%d\n",soenum);
				break;
		case AI:
				savpnt = savpnt + 24;
			 	if((fp=fopen(slotdefname[j],"r"))==NULL)
 				{
 					sprintf(infobuf,"can't open file %s\n",slotdefname[j]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
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
					fscanf(fp,"%s %f %f %s",pntname[j][n],&lorange[j][n],&hirange[j][n],lgname);
					if(strcmp(pntname[j][n],"NULL")==0)
					{
						continue;
					}
#ifdef DEBUG
					printf("%d slot, %d pnt: %s %5.2f %5.2f %s\n",j,n,pntname[j][n],lorange[j][n],hirange[j][n],lgname);
#endif
					strcpy(name_buf2,pntname[j][n]);
					if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
					{
						sprintf(infobuf,"%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					}
				}
				fclose(fp);
				break;
		case DO:
			 	if((fp=fopen(slotdefname[j],"r"))==NULL)
 				{
 					sprintf(infobuf,"can't open file %s\n",slotdefname[j]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					for(n=0;n<32;n++)
					{
						strcpy(pntname[j][n],"NULL");
					}
					continue;
				}
				rewind(fp);	
				for(n=0;n<32;n++)
				{
					fscanf(fp,"%s %s",pntname[j][n],lgname);
					if(strcmp(pntname[j][n],"NULL")==0)
						continue;
#ifdef DEBUG
					printf("%d slot, %d pnt: %s %s\n",j,n,pntname[j][n],lgname);
#endif
					strcpy(name_buf2,pntname[j][n]);
					/*ReadEntryByName(name_buf2,&data_type2,&dbentry2);
					if(dbentry2.err_code==-1)*/
					if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
					{
						sprintf(infobuf,"%d slot, %d pnt: %s No this point in the database file!\n",j,0,name_buf2);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					}
				}
				fclose(fp);
				break;
		case TRD:
			 	if((fp=fopen(slotdefname[j],"r"))==NULL)
 				{
 					sprintf(infobuf,"can't open file %s\n",slotdefname[j]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					for(n=0;n<24;n++)
					{
						strcpy(pntname[j][n],"NULL");
					}
					continue;
				}
				rewind(fp);	
				for(n=0;n<16;n++)
				{
					fscanf(fp,"%s %s",pntname[j][n],lgname);
					if(strcmp(pntname[j][n],"NULL")==0)
						continue;
#ifdef DEBUG
					printf("%d slot2, %d pnt: %s %s\n",j,n,pntname[j][n],lgname);
#endif
					strcpy(name_buf2,pntname[j][n]);
					if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
					{
						sprintf(infobuf,"%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					}
				}
				fclose(fp);
				break;
		case AO:
			 	if((fp=fopen(slotdefname[j],"r"))==NULL)
 				{
 					sprintf(infobuf,"can't open file %s\n",slotdefname[j]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
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
					fscanf(fp,"%s %f %f %s",pntname[j][n],&lorange[j][n],&hirange[j][n],lgname);
					if(strcmp(pntname[j][n],"NULL")==0)
						continue;
#ifdef DEBUG
					printf("%d slot, %d pnt: %s %5.2f %5.2f %s\n",j,n,pntname[j][n],lorange[j][n],hirange[j][n],lgname);
#endif
					strcpy(name_buf2,pntname[j][n]);
					if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
					{
						sprintf(infobuf,"%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					}
				}
				fclose(fp);
				break;
		case PA:
			 	if((fp=fopen(slotdefname[j],"r"))==NULL)
 				{
 					sprintf(infobuf,"can't open file %s\n",slotdefname[j]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					for(n=0;n<24;n++)
					{
						strcpy(pntname[j][n],"NULL");
					}
					continue;
				}
				rewind(fp);	
				for(n=0;n<8;n++)
				{
					fscanf(fp,"%s %s",pntname[j][n],lgname);
					if(strcmp(pntname[j][n],"NULL")==0)
						continue;
#ifdef DEBUG
					printf("%d slot, %d pnt: %s %s\n",j,n,pntname[j][n],lgname);
#endif
					strcpy(name_buf2,pntname[j][n]);
					if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
					{
						sprintf(infobuf,"%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					}
				}
				fclose(fp);
				break;
		case CCO:
			 	if((fp=fopen(slotdefname[j],"r"))==NULL)
 				{
 					sprintf(infobuf,"can't open file %s\n",slotdefname[j]);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
					for(n=0;n<24;n++)
					{
						strcpy(pntname[j][n],"NULL");
					}
					continue;
				}
				rewind(fp);	
				for(n=0;n<8;n++)
				{
					fscanf(fp,"%s %s",pntname[j][n],lgname);
					if(strcmp(pntname[j][n],"NULL")==0)
						continue;
#ifdef DEBUG
					printf("%d slot, %d pnt: %s %s\n",j,n,pntname[j][n],lgname);
#endif
					strcpy(name_buf2,pntname[j][n]);
					if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
					{
						sprintf(infobuf,"%d slot, %d pnt: %s No this point in the database file!\n",j,n,name_buf2);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
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
		}/*switch(rtuconfig[j])*/
	}/*for*/
}


void	SC1801_Send()
{
#ifdef	BY_RECORD
	SHM_RECORD	*shm_rec_addr;
/*	int		shm_id_rec;*/
	IND_REC		*pInd_alm;
	unsigned char	type_id;
	short		rcv_end,rcv_pnt;
	POINTER		rcv_ptr;
/*	int		cmd_type;*/
#else
	int		data_type;
	unsigned char	name_buf[NAME_SIZE];
	DMS_COMMON	dbentry;
#endif
	unsigned short	pnt,soe_head,savpnt;
	unsigned char	grp,bitmask,bitnum;
	short	soe_stat;

#ifndef SUN_OS
	HANDLE	hMapObject;
#endif

	POINTER	ptr;
	IND_DATA	dbstat;

/*	cmd_end = shm_recv_addr->head;*/
	printf("------------SC1801_Send thread start!!!-----------\n");
#ifndef BY_RECORD
	for(;;)
	{
		sleep(1);
		for(pnt=0;pnt<soenum;pnt++)
		{
			ptr = soedef[pnt].ptr;

			if(ReadEntryById(&ptr,&dbptr)==-1)
 			{ 
				sprintf(infobuf,"devid=%d type=%d id=%d no this point in db!\n",
 						ptr.dev_id,ptr.data_type,ptr.point_id);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
				continue;
 			}
			/*soe_stat = (dbptr.entry.ind.variable.value.status&0x3c)>>2;**zwj*/
			soe_stat =	dbptr.var.ind.status.rt_state;
			if(soe_stat != soedef[pnt].status)
			{
				soe_head = soebuf.bufpnt;
				grp = soedef[pnt].pnt / 6;
				soebuf.soerec[soe_head].slot_grp = (grp << 4) | soedef[pnt].slot;
				bitnum = soedef[pnt].pnt % 6;
				bitmask = 0x01;
				bitmask = bitmask << bitnum;
				soebuf.soerec[soe_head].mask = bitmask;
				savpnt = soedef[pnt].orgpnt;
/*				soebuf.soerec[soe_head].stat = orgbuf[savpnt];*/
				if(soe_stat)
				{
					soebuf.soerec[soe_head].stat = orgbuf[savpnt] | bitmask;
				}
				else
				{
					soebuf.soerec[soe_head].stat = orgbuf[savpnt] & (~bitmask);
				}
				soedef[pnt].status = soe_stat;
				soebuf.soerec[soe_head].time = dbptr.var.ind.update_time;
				soebuf.soerec[soe_head].time.year = soebuf.soerec[soe_head].time.year-1900;
				soebuf.soerec[soe_head].time.ms = soebuf.soerec[soe_head].time.ms *10;
#ifdef DEBUG
				printf("soe: %d =%d =%d stat=%02x mask=%02x slot_gp=%02x %d/%d/%d %d:%d:%d ms=%d\n",
					ptr.dev_id, ptr.data_type,ptr.pt_id,soebuf.soerec[soe_head].stat,
					soebuf.soerec[soe_head].mask,soebuf.soerec[soe_head].slot_grp,
					soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
					soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
					soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
					soebuf.soerec[soe_head].time.ms/10);
#endif
				soe_head ++;
				if(soe_head >= SOE_NUM)
					soe_head = 0;
				soebuf.bufpnt = soe_head;

				if(soe_head == soebuf.sendpnt)
				{

					soe_head ++;
					if(soe_head >= SOE_NUM)
						soe_head = 0;
					soebuf.sendpnt = soe_head;

					rtu_status = rtu_status | 0x1C;
				}
				else
				{
					rtu_status = rtu_status | 0x04;
				}
			}
		}
	}
#else		

#ifdef SUN_OS
 /*	获取记录区共享内存地址  */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		sprintf(infobuf,"\nsc1801: Fail to shmat shm_rec_addr: errno=%d\n",errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		thr_exit((void *)-1);
	}
	sprintf(infobuf,"\nsc1801:: shmat shm_rec_addr: OK!\n");
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

#else
	printf("#################");

	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if (shm_rec_addr == NULL)
            printf("\nsc1801::Error MapViewOfFile shm_erc_addr: %d\n",GetLastError());
	else    printf("\nsc1801::MapViewOfFile shm_rec_addr: OK!\n");

	/*rcv_end = shm_rec_addr->alarm_rec.header_ptr;*/
#endif
	rcv_end = shm_rec_addr->head.header_ptr;

	for(;;)
	{
	/*	Sleep(1000);*/
		sleep(1);
printf("@@@@@@@@@@@@@@@@@@@@@\n");
		while( rcv_end!=shm_rec_addr->head.header_ptr )
		{
			rcv_pnt = rcv_end;
			rcv_end++;
			if(rcv_end >= MAX_SHM_RECORD_NUM)
				rcv_end = 0;

			/*pInd_alm = &shm_rec_addr->alarm_rec.rec[rcv_pnt].int_alm;
			rcv_ptr.data_type = pInd_alm->rtu_ind_alm.id.data_type;*/
			pInd_alm = &shm_rec_addr->rec[rcv_pnt].ind_rec;
			type_id = pInd_alm->type_id;
            printf("\nsc1801::Receive a record! data_type: %d\n",type_id);
		/*	if((type_id < DPS_INT_IND) || (type_id > DPS_SYSMSG))*/
				if((type_id != DPS_IND_ALM) && (type_id != DPS_MAN_STAT_SET) && (rcv_ptr.data_type != C_SYS_MODE_SET))
			{
				continue;
			}
			/*rcv_ptr.dev_id = pInd_alm->rtu_ind_alm.id.dev_id;
			rcv_ptr.point_id = pInd_alm->rtu_ind_alm.id.point_id;*/
			rcv_ptr.stn_id = pInd_alm->stn_id;
			rcv_ptr.dev_id = pInd_alm->dev_id;
			rcv_ptr.data_type = pInd_alm->data_type;
			rcv_ptr.pt_id = pInd_alm->pt_id;

            printf("\nsc1801::Receive a record!%d %d %d %d\n",rcv_ptr.stn_id,rcv_ptr.dev_id,rcv_ptr.data_type,rcv_ptr.pt_id);
			for(pnt=0;pnt<soenum;pnt++)
			{
				ptr = soedef[pnt].ptr;
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(ptr.data_type == rcv_ptr.data_type)&&(ptr.pt_id == rcv_ptr.pt_id))
					break;
			}
			if(pnt>=soenum)
				continue;

			/*soe_stat = (pInd_alm->rtu_ind_alm.value.status&0x3c)>>2;*/
				dbstat = pInd_alm->ind_status;
				if(dbstat.rt_state==1)
						soe_stat = 1;
					else
						soe_stat = 0;
			
				soe_head = soebuf.bufpnt;
				grp = soedef[pnt].pnt / 6;
				soebuf.soerec[soe_head].slot_grp = (grp << 4) | soedef[pnt].slot;
				bitnum = soedef[pnt].pnt % 6;
				bitmask = 0x01;
				bitmask = bitmask << bitnum;
				soebuf.soerec[soe_head].mask = bitmask;
				savpnt = soedef[pnt].orgpnt;
				if(soe_stat==1)
				{
					soebuf.soerec[soe_head].stat = orgbuf[savpnt] | bitmask;
				}
				else
				{
					soebuf.soerec[soe_head].stat = orgbuf[savpnt] & (~bitmask);
				}
				soedef[pnt].status = soe_stat;
				soebuf.soerec[soe_head].time.ms =  pInd_alm->ms*10;
				soebuf.soerec[soe_head].time.second = pInd_alm->second;
				soebuf.soerec[soe_head].time.minute = pInd_alm->minute;
				soebuf.soerec[soe_head].time.hour = pInd_alm->hour;
				soebuf.soerec[soe_head].time.day = pInd_alm->day;
				soebuf.soerec[soe_head].time.month = pInd_alm->month;
				soebuf.soerec[soe_head].time.year = pInd_alm->year-1900;

				sprintf(infobuf,"soe: %d =%d =%d stat=%02x mask=%02x slot_gp=%02x %d/%d/%d %d:%d:%d ms=%d\n",
					ptr.dev_id, ptr.data_type,ptr.pt_id,soebuf.soerec[soe_head].stat,
					soebuf.soerec[soe_head].mask,soebuf.soerec[soe_head].slot_grp,
					soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
					soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
					soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
					soebuf.soerec[soe_head].time.ms/10);
#ifdef DEBUG
				printf("soe: pnt=%d savpnt=%d orgstat=%02X\n",pnt,savpnt,orgbuf[savpnt]);
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				soe_head ++;
				if(soe_head >= SOE_NUM)
					soe_head = 0;

				if(soe_head == soebuf.sendpnt)
				{
					rtu_status = rtu_status | 0x1C;
				}
				else
				{
					rtu_status = rtu_status | 0x04;
					soebuf.bufpnt = soe_head;
				}

		}
	}

#endif
}/*SC1801_Send()*/
