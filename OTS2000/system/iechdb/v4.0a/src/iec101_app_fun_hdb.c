/************************************************************
*  FILE NAME:           iec101_app_fun.c      				*
*  FUNCTION:            Controlling Station					*
*						Application Layer Functions			*
*  's NAME :  			              						*
*  DATE:                March,2007                			*
*															*
************************************************************/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#include "..\inc\iec101_com.h"

#define	DEBUG

extern	unsigned char	sendbuf[1024];
ASDUCOMDDATA	send_data;
ASDUDATA		recv_data;
extern	HANDLE	hRecvSemaphore;
extern	BOOL	resetlinkf[STN_NUM+1],initflag[STN_NUM+1];
extern	BOOL	counterflag[STN_NUM+1],ClockSyncflag[STN_NUM+1],interrogationflag[STN_NUM+1];
extern	int		cmd_ack[STN_NUM+1],localinitfg;
extern	BOOL	fcb[STN_NUM+1];
extern	unsigned char	stn_id,comdflag,comdflag1;

int DswNum=0;							
int SswNum=0;							
int AlPtr=0;
int AlPtrEnd=0;							
BHDSW ArrayDsw[MAX_DSW_NUM];
BHAL ArrayAL[MAX_BHAL_NUM];
ANADEF		anadef;
SETPTCTRL	setpt;

extern	BOOL	lan_out();
extern	float	GetAnaValue();

float	Datcvt(status,data)
	short		status;
	unsigned short	data;
{
	float	result;

	switch(status&0x060)
		{
		case 0x20:
			result=(float)(data*0.1);
			break;
		case 0x40:
			result=(float)(data*0.01);
			break;
		case 0x60:
			result=(float)(data*0.001);
			break;
		default:
			result=(float)data;
		}

	if(status&0x80)
		result=-result;

	return(result);
}

BOOL GenDSwTable()		
{
	FILE		*fp;
	int			iTmp=0;
	short		j;
 	if((fp=fopen("/home/ems/h9000/iec101/ND_IEC101_DP_DATA.DEF","r"))==NULL)
 		{
 		printf("can't open file /home/ems/h9000/iec101/DSW_DATA.DEF\n");
 		DswNum = 0;
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&DswNum);
	if(DswNum >= MAX_DSW_NUM)
	{
		printf("GenDSwTable: above the MAX_DSW_NUM %d\n",MAX_DSW_NUM);
 		DswNum = 0;
		return(FALSE);  
	}

	for(j=0;j<DswNum;j++)
	{
		fscanf(fp,"%d	%s	%s	%d	%d",&iTmp,ArrayDsw[j].devname,ArrayDsw[j].infoname,&ArrayDsw[j].bhdevid,&ArrayDsw[j].infoid);
		if(j != iTmp)
		{
			printf("GenDSwTable: the point sequence id %d is wrong %d\n",iTmp);
 			DswNum = 0;
			return(FALSE); 
		}
		ArrayDsw[j].id = j;
			
	}
	printf("GenDSwTable: point number = %d\n",DswNum);
	fclose(fp);
	return(TRUE);
}

BOOL GenAnaTable()		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];
	char			longname[100];
	char			iStr[5];
	short		j;
 	if((fp=fopen("/home/ems/h9000/iec101/ND_IEC101_ME_DATA.DEF","r"))==NULL)
 	{
 		printf("can't open file /home/ems/h9000/iec101/ND_IEC101_ME_DATA.DEF\n");
 		anadef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&anadef.number);
	if(anadef.number >= MAX_ANA_NUM)
	{
		printf("GenAnaTable: above the MAX_ANA_NUM %d\n",MAX_ANA_NUM);
		anadef.number = MAX_ANA_NUM;
	} 

	for(j=0;j<anadef.number;j++)
		{
		fscanf(fp,"%s	%s %f %f	%s",iStr,anadef.name[j],&anadef.lorange[j],&anadef.hirange[j],longname);
		if(strcmp(anadef.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("ME %d pnt: %s %f %f\n",j,anadef.name[j],anadef.lorange[j],anadef.hirange[j]);
#endif
			strcpy(name_buf2,anadef.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
				printf("#### ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
 	anadef.number = j;
	printf("GenAnaTable: point number = %d\n",j);
	fclose(fp);

	return(TRUE);
}

void	IEC870_5_Recv()
{

	LAN_HEAD	lanhead;
	USHORT	recvdatapt,infoaddr,infoptr,datalength,lengthfactor,ms,year;
	short	ana16data;		/*USGORT to short. zy*/
	int 	i,datapoint;
	char	dbname[36];
	unsigned char	stnd,type_id,varstru,causetra,infonum,qoc,lanpt;
	
	unsigned char	me_buf[1440];
	IND_DATA	sw_status;
	
	float	value;
	
	DMS_COMMON	dbcommpt;
	ANLG_DATA	anadata;
	struct	tm	tmpt;
	time_t		tim;
	printf("============IEC870_5_Recv thread start!!!===========\n");
	
	
	GenDSwTable();		
	GenAnaTable();		
	
	recvdatapt = 0;
	
	
	for(;;)
	{
		MySleep(1000);
		tim=time(NULL);
		tmpt=*localtime(&tim);
		sw_status.rt_state = 2;
		ArrayDsw[0].status = sw_status;
		ArrayDsw[0].update_time = tmpt;
		AddOneAl(ArrayDsw[0]);

		/* dwWaitRecvASDU = WaitForSingleObject(hRecvSemaphore,INFINITE); */
		
		while(recvdatapt != recv_data.pnt)
		{
			/*
			//added by yyp
			recv_data.data[recvdatapt][0] = 168;
			recv_data.data[recvdatapt][1] = 1;
			recv_data.data[recvdatapt][2] = 3;
			recv_data.data[recvdatapt][3] = 1;
			recv_data.data[recvdatapt][4] = 3;
			recv_data.data[recvdatapt][5] = 1;
			recv_data.data[recvdatapt][6] = 1;
			recv_data.data[recvdatapt][7] = 3;
			recv_data.data[recvdatapt][8] = 2;
			recv_data.length[recvdatapt] = 9;	 
			*/
			tim=time(NULL);
			tmpt=*localtime(&tim);
			stnd = recv_data.data[recvdatapt][1];
			type_id = recv_data.data[recvdatapt][2];
			varstru = recv_data.data[recvdatapt][3];
			causetra = recv_data.data[recvdatapt][4];
			
			switch(type_id)
			{
				//modified by yyp for M_DP_NA
			case M_SP_NA:	//single point without time tag: 1
			case M_DP_NA:	//double point without time tag: 3
				if(varstru & 0x80)				//SQ=1 sequence
				{
					infonum = varstru & 0x7F;
					datalength = infonum+8;
					if(datalength != recv_data.length[recvdatapt])
					{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
							type_id,infonum,recv_data.length[recvdatapt]);
						break;
					}
					
					lanpt = 0;
					memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);
					if(type_id ==M_SP_NA)
					{
						infoptr = infoaddr - SP_BASE_ADDR;
					}
					else
					{
						infoptr = infoaddr - DP_BASE_ADDR;
					}
					for(i=0;i<infonum;i++)
					{
						datapoint = i+8;
						
						if(type_id ==M_SP_NA)
						{
							if(infoptr > SswNum)
							{
#ifdef DEBUG
								printf("M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,SswNum);
#endif					
								break;
							}
						}
						else
						{
							if(infoptr > DswNum)
							{
#ifdef DEBUG
								printf("M_DP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,DswNum);
#endif					
								break;
							}
						}
						qoc = recv_data.data[recvdatapt][datapoint];
						*(USHORT *)&sw_status = 0;
						if(type_id ==M_SP_NA)
						{
							if(qoc & 1)
								sw_status.rt_state = 4;
							else
								sw_status.rt_state = 0;
						}
						else
						{
							switch(qoc & 0x03)
							{
							case 0:	sw_status.rt_state = 2; //transit
								break;
							case 1:	sw_status.rt_state = 0; //off
								break;
							case 2:	sw_status.rt_state = 1; //on
								break;
							case 3:	sw_status.rt_state = 3; //error
								break;
							}
						}
						if(qoc & 0x20)	  			//SIQ:= IV NT SB BL 0 0 0 SPI
							sw_status.man_set = 1;	//manual set
						else
							sw_status.man_set = 0;
						
						if(qoc & 0xD0)
							sw_status.chan_fail = 1;	//channel fault
						else
							sw_status.chan_fail = 0;
						
						sw_status.fault_tag = 0;
						
						//modified by yyp 07.03.11
						if(ArrayDsw[infoptr].status.rt_state == sw_status.rt_state)
						{
							infoptr ++;
							continue;
						}
						ArrayDsw[infoptr].status = sw_status;
						ArrayDsw[infoptr].update_time = tmpt;
						AddOneAl(ArrayDsw[infoptr]);
						infoptr ++;
					} //for
					
				}//if(SQ)
				else		//SQ=0
				{
					infonum = varstru & 0x7F;
					datalength = infonum*3+6;
					if(datalength != recv_data.length[recvdatapt])
					{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
							type_id,infonum,recv_data.length[recvdatapt]);
						break;
					}
					
					tim=time(NULL);
					tmpt=*localtime(&tim);
					
					for(i=0;i<infonum;i++)
					{
						datapoint = i*3+6;
						memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
						if(type_id == M_SP_NA)
						{
							infoptr = infoaddr - SP_BASE_ADDR;
							if(infoptr > SswNum)
							{
#ifdef DEBUG
								printf("M_SP_NA SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,SswNum);
#endif					
								break;
							}
							infoaddr ++;
						}
						else
						{
							infoptr = infoaddr - DP_BASE_ADDR;
							if(infoptr > DswNum)
							{
#ifdef DEBUG
								printf("M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,DswNum);
#endif					
								break;
							}
							infoaddr += 2;
						}
						
						qoc = recv_data.data[recvdatapt][datapoint+2];
						
						if(type_id == M_SP_NA)
						{
							if(qoc & 1)
							{
								sw_status.rt_state = 1;
							}
							else
							{	
								sw_status.rt_state = 0;
							}
						}
						else
						{	
							switch(qoc & 0x03)
							{
							case 0:	sw_status.rt_state = 2; //transit
								break;
							case 1:	sw_status.rt_state = 0; //off
								break;
							case 2:	sw_status.rt_state = 1; //on
								break;
							case 3:	sw_status.rt_state = 3; //error
								break;
								
							}
						}
						if(qoc & 0x20)	  			//SIQ:= IV NT SB BL 0 0 0 SPI
							sw_status.man_set = 1;	//manual set
						else
							sw_status.man_set = 0;
						if(qoc & 0xD0)
							sw_status.chan_fail = 1;	//channel fault
						else
							sw_status.chan_fail = 0;
						sw_status.fault_tag = 0;
						
						//modified by yyp 07.03.11
						if(ArrayDsw[infoptr].status.rt_state == sw_status.rt_state)
						{
							infoptr ++;
							continue;
						}
						ArrayDsw[infoptr].status = sw_status;
						ArrayDsw[infoptr].update_time = tmpt;
						AddOneAl(ArrayDsw[infoptr]);
						infoptr ++;
					}//end for
				}//if(SQ)
				
				break;
				
				
				case M_SP_TB:	/* single point with time tag {data class 30 } */
				case M_DP_TB:	/* double point with time tag {data class 31 } */
					
					infonum = varstru & 0x7F;
					datalength = 10*infonum+6;
					if(datalength != recv_data.length[recvdatapt])
					{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
							type_id,infonum,recv_data.length[recvdatapt]);
						break;
					}
					for(i=0;i<infonum;i++)
					{
						datapoint = 10*i+6;
						memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
						if(type_id ==M_SP_TB)
						{
							infoptr = infoaddr - SP_BASE_ADDR;
							if((infoptr < 0) || (infoptr > DswNum))
							{
#ifdef DEBUG
								printf("M_SP_TB SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,DswNum);
#endif
								break;
							}
						}
						else
						{
							infoptr = infoaddr - DP_BASE_ADDR;
							if((infoptr < 0) || (infoptr > DswNum))
							{
#ifdef DEBUG
								printf("M_DP_TB SQ=0: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,DswNum);
#endif
								break;
							}
						}
						qoc = recv_data.data[recvdatapt][datapoint+2];
						*(USHORT *)&sw_status = 0;
						if(type_id ==M_SP_TB)
						{
							if(qoc & 1)				   /* SIQ:= IV NT SB BL 0 0 0 SPI */
								sw_status.rt_state = 1;
							else
								sw_status.rt_state = 0;
						}
						else
						{
							switch(qoc & 0x03)
							{
							case 0:	sw_status.rt_state = 2; /* transit */
								break;
							case 1:	sw_status.rt_state = 0; /* off */
								break;
							case 2:	sw_status.rt_state = 1; /* on */
								break;
							case 3:	sw_status.rt_state = 3; /* error */
								break;
							}
						}
						
						if(qoc & 0x20)	  			/* SIQ:= IV NT SB BL 0 0 0 SPI */
							sw_status.man_set = 1;	/* manual set */
						else
							sw_status.man_set = 0;
						if(qoc & 0xD0)
							sw_status.chan_fail = 1;	/* channel fault */
						else
							sw_status.chan_fail = 0;
						
						sw_status.fault_tag = 0;
						
						//modified by yyp 07.03.11
						if(ArrayDsw[infoptr].status.rt_state == sw_status.rt_state)
						{
							infoptr ++;
							continue;
						}
						ArrayDsw[infoptr].status = sw_status;
						ArrayDsw[infoptr].update_time = tmpt;
						AddOneAl(ArrayDsw[infoptr]);
						infoptr ++;
						
					} //end for
					
					break;
					
					//modified by yyp
				case M_ME_NA:	//measure value without time tag:9
				case M_ME_TA:	//measure value with time tag:10
					infonum = varstru & 0x7F;			//number of information object
					if(varstru & 0x80)	//SQ=1
					{
						if(type_id == M_ME_NA)
						{
							lengthfactor = 3;
						}
						else
							lengthfactor = 6;
						datalength = lengthfactor*infonum+8;
						if(datalength != recv_data.length[recvdatapt])
						{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
							break;
						}
						
						lanpt = 0;
						memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 //address of object
						
						infoptr = infoaddr - ME_BASE_ADDR;
						for(i=0;i<infonum;i++)
						{
							datapoint = lengthfactor*i+8;
							
							if(infoptr > anadef.number)
							{
#ifdef DEBUG
								printf("M_ME_NA SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,anadef.number);
#endif					
								break;
							}
							strcpy(dbname,anadef.name[infoptr]);
							
							if(strcmp(dbname,"NULL")==0)
							{
								infoptr ++;
								continue;
							}
							if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
							{
								printf("\nIEC No this point name=%s in db\n",dbname);
								infoptr ++;
								continue;
							}
							qoc = recv_data.data[recvdatapt][datapoint+4];
							
							memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint+2],2);
							
							value=(ana16data*(anadef.hirange[infoptr]-anadef.lorange[infoptr])/RESOLUTE)
								+ anadef.lorange[infoptr];		 //Normalized value to float
							
							//modified by yyp 
							if(dbcommpt.point.data_type == IMP_TYPE)
							{
								anadata.data = (GLONG)value;
								if(causetra == SPONT)
									me_buf[lanpt*22] =DPS_IMPULSE;	/* type_id */
								else
									me_buf[lanpt*22] =DPS_IMP_ALL;	/* type_id */
							}
							else
							{
								anadata.data = (GLONG)(value*1000);
								if(causetra == SPONT)
									me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */
								else
									me_buf[lanpt*22] =DPS_ANA_ALL;	/* type_id */
							}
							
							*(USHORT *)&anadata.status = 0;
							if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
								anadata.status.ctrl_block = 1;	/* manual set */
							if(qoc & 0xD0)
								anadata.status.chan_fail = 1;	/* channel fault */
							
							infoptr ++;
							
							me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
							me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
							me_buf[lanpt*22+3] =dbcommpt.point.data_type;
							me_buf[lanpt*22+4] = 1;		/* number low byte */
							me_buf[lanpt*22+5] = 0;		/* number high byte */
							memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
							memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
							memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
							GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */
							
							if(type_id ==M_ME_NA)
							{
								me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
								me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
								me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
								me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
								me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
								year=tmpt.tm_year+1900;	/* year */
							}
							else
							{
								memcpy(&ms,&recv_data.data[recvdatapt][datapoint+3],2);
								me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
								me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+5] & 0x3F;	/* minute */
								if(type_id ==M_ME_TA)
								{
									me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
									me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
									me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
									year=tmpt.tm_year+1900;		/* year */
								}
								else		/* M_ME_TD */
								{
									me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+6] & 0x1F;	/* hour */
									me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+7] & 0x1F;	/* day */
									me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+8] & 0x0F;	/* month */
									year=recv_data.data[recvdatapt][datapoint+9] & 0x7F+2000;		/* year */
								}
							}
							memcpy(&me_buf[lanpt*22+20],&year,2);
							
							lanpt ++;
							if(lanpt>=64)
							{
								lanhead.length = lanpt*22;
								if(causetra == SPONT)
									lanhead.dp_type=BJ_DB_TIME;
								else
									lanhead.dp_type=ALL_DB_TIME;
								GetLocalHostId(&lanhead.src_id);
								lanhead.dest_id =0;
								lanhead.dest_stn =me_buf[1];
								lanhead.packet_num =0;
								lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
								lanpt = 0;
							}
							}
						}
						else		//SQ=0
						{
							if(type_id == M_ME_NA)
							{
								lengthfactor = 5;
							}
							else
								lengthfactor = 8;
							datalength = lengthfactor*infonum+6;
							if(datalength != recv_data.length[recvdatapt])
							{
								printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
								break;
							}
							
							lanpt = 0;
							for(i=0;i<infonum;i++)
							{
								datapoint = lengthfactor*i+6;
								
								memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);	 //address of object
								
								infoptr = infoaddr - ME_BASE_ADDR;
								if(infoptr > anadef.number)
								{
#ifdef DEBUG
									printf("M_ME_NA SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,anadef.number);
#endif					
									break;
								}
								strcpy(dbname,anadef.name[infoptr]);
								
								if(strcmp(dbname,"NULL")==0)
									continue;
								if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
								{
									printf("\nIEC No this point name=%s in db\n",dbname);
									continue;
								}
								qoc = recv_data.data[recvdatapt][datapoint+4];
								
								memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint+2],2);
								
								value=(ana16data*(anadef.hirange[infoptr]-anadef.lorange[infoptr])/RESOLUTE)
									+ anadef.lorange[infoptr];		 //Normalized value to float
								
								//modified by yyp 
								if(dbcommpt.point.data_type == IMP_TYPE)
								{
									anadata.data = (GLONG)value;
									if(causetra == SPONT)
										me_buf[lanpt*22] =DPS_IMPULSE;	/* type_id */
									else
										me_buf[lanpt*22] =DPS_IMP_ALL;	/* type_id */
								}
								else
								{
									anadata.data = (GLONG)(value*1000);
									if(causetra == SPONT)
										me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */
									else
										me_buf[lanpt*22] =DPS_ANA_ALL;	/* type_id */
								}
								
								*(USHORT *)&anadata.status = 0;
								
								if(qoc & 0x20)			//QDS:= IV NT SB BL 0 0 0 OV
									anadata.status.ctrl_block = 1;	/* manual set */	//manual set
								else if(qoc & 0xD0)
									anadata.status.chan_fail = 1;	/* channel fault */	//channel fault
								me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
								me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
								me_buf[lanpt*22+3] =dbcommpt.point.data_type;
								me_buf[lanpt*22+4] = 1;		/* number low byte */
								me_buf[lanpt*22+5] = 0;		/* number high byte */
								memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
								memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
								memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
								GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */
								
								if(type_id ==M_ME_NA)
								{
									me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
									me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
									me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
									me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
									me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
									year=tmpt.tm_year+1900;	/* year */
								}
								else
								{
									memcpy(&ms,&recv_data.data[recvdatapt][datapoint+6],2);
									me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
									me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+8] & 0x3F;	/* minute */
									if(type_id ==M_ME_TA)
									{
										me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
										me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
										me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
										year=tmpt.tm_year+1900;		/* year */
									}
									else		/* M_ME_TD */
									{
										me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* hour */
										me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+10] & 0x1F;	/* day */
										me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+11] & 0x0F;	/* month */
										year=recv_data.data[recvdatapt][datapoint+12] & 0x7F+2000;		/* year */
									}
								}
								memcpy(&me_buf[lanpt*22+20],&year,2);
								lanpt ++;
								if(lanpt>=64)
								{
									lanhead.length = lanpt*22;
									if(causetra == SPONT)
										lanhead.dp_type=BJ_DB_TIME;
									else
										lanhead.dp_type=ALL_DB_TIME;
									GetLocalHostId(&lanhead.src_id);
									lanhead.dest_id =0;
									lanhead.dest_stn =me_buf[1];
									lanhead.packet_num =0;
									lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
									lanpt = 0;
								}
							}
							
						}
						if(lanpt>0)
						{
							lanhead.length = lanpt*22;
							if(causetra == SPONT)
								lanhead.dp_type=BJ_DB_TIME;
							else
								lanhead.dp_type=ALL_DB_TIME;
							GetLocalHostId(&lanhead.src_id);
							lanhead.dest_id =0;
							lanhead.dest_stn =me_buf[1];
							lanhead.packet_num =0;
							lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
						}		
						break;
						
			case C_CS_NA:	//confirm of clock synchronization command
				
				if((causetra & 0x3F) == ACTCON)
				{
					if(causetra & 0x40)		//0 positive 1 negative confirm
					{
						cmd_ack[stn_id] = -1;		//negative confirm
						ClockSyncflag[stn_id] = 0;
#ifdef DEBUG
						printf("clock synchronization command is NACK by remote system!\n");
#endif
					}
					else
					{
						cmd_ack[stn_id] = 1;		//execute command confirm
						ClockSyncflag[stn_id] = 0;
#ifdef DEBUG
						printf("clock synchronization command is ACK by remote system!\n");
#endif
					}
				}
				else
#ifdef DEBUG
					printf("clock synchronization command: Received cause of transmission=%2x wrong!\n",causetra);
#endif
				
				break;
				
			case C_IC_NA:	//confirm of general interrogation command
				
				switch(causetra & 0x3F)
				{
				case ACTCON:		//general interrogation command act confirm
					if(causetra & 0x40)		//0 positive 1 negative confirm
					{
						cmd_ack[stn_id] = -1;		//negative confirm
						interrogationflag[stn_id] = 0;
#ifdef DEBUG
						printf("IEC general interrogation command is NACK by remote system!\n");
#endif
					}
					else
					{
						cmd_ack[stn_id] = 1;		//command confirm
						interrogationflag[stn_id] = 2;
#ifdef DEBUG
						printf("IEC general interrogation command is ACK by remote system!\n");
#endif
					}
					break;
					
				case ACTTERM:				//general interrogation command act terminal
					
					interrogationflag[stn_id] = 0;
					if(initflag[stn_id] ==2)
					{
						/* removed by yyp 07.03.11 */
						// initflag[stn_id] =3;
					}
#ifdef DEBUG
					printf("IEC general interrogation command is terminal in remote system!\n");
#endif
					break;
					
				case DEACTCON:
					
					if(causetra & 0x40)		//0 positive 1 negative confirm
					{
						cmd_ack[stn_id] = -1;		//negative confirm
#ifdef DEBUG
						printf("IEC general interrogation deact command is NACK by remote system!\n");
#endif
					}
					else
					{
						cmd_ack[stn_id] = 1;		//positive confirm
						interrogationflag[stn_id] = 0;
#ifdef DEBUG
						printf("IEC general interrogation deact command is ACK by remote system!\n");
#endif
					}
					break;
				default :
					printf("C_IC_NA: Received cause of transmission=%2x wrong!\n",causetra);
					break;
				}//switch(causetra & 0x3F)
				
				break;
				
				case C_CI_NA:	//confirm of counter interrogation command
					
					switch(causetra & 0x3F)
					{
					case ACTCON:		//counter interrogation command act confirm
						if(causetra & 0x40)		//0 positive 1 negative confirm
						{
							counterflag[stn_id] = 0;
							cmd_ack[stn_id] = -1;		//negative confirm
#ifdef DEBUG
							printf("IEC counter interrogation command is NACK by remote system!\n");
#endif
						}
						else
						{
							cmd_ack[stn_id] = 1;		//execute command confirm
							counterflag[stn_id] = 2;
#ifdef DEBUG
							printf("IEC counter interrogation command is ACK by remote system!\n");
#endif
						}
						break;
						
					case ACTTERM:				//counter interrogation command act terminal
						
						counterflag[stn_id] = 0;
#ifdef DEBUG
						printf("IEC counter interrogation command is terminal in remote system!\n");
#endif
						break;
						
					case DEACTCON:
						
						if(causetra & 0x40)		//0 positive 1 negative confirm
						{
							cmd_ack[stn_id] = -1;		//negative confirm
#ifdef DEBUG
							printf("IEC counter interrogation deact command is NACK by remote system!\n");
#endif
						}
						else
						{
							cmd_ack[stn_id] = 1;		//positive confirm
#ifdef DEBUG
							counterflag[stn_id] = 0;
#endif
							printf("IEC counter interrogation deact command is ACK by remote system!\n");
						}
						break;
						
					default :
#ifdef DEBUG
						printf("C_CI_NA: Received cause of transmission=%2x wrong!\n",causetra);
#endif
						break;
					}//switch(causetra & 0x3F)
					
					break;
					
					default :
						printf("IEC type_id = %u is unavailable at this time!\n",type_id);
						break;
			}//switch(type_id)
			
			recvdatapt++;
			if( recvdatapt >= ASDU_ENTRY_NUMBER )
				recvdatapt = 0;
		}//while(recvdatapt!= recv_data.pnt)
	}//for(;;)
}


int AddOneAl(BHDSW dsw)
{
	ArrayAL[AlPtr].id = dsw.id;
	tmtostring(dsw.update_time, ArrayAL[AlPtr].update_time);
	memcpy(&ArrayAL[AlPtr].status,&dsw.status,2);
	AlPtr ++;
	if(AlPtr>=MAX_BHAL_NUM)
	{
		AlPtr = 0;
	}
	return TRUE;
}
	

int tmtostring(struct tm dtime, char *str)
{
	sprintf(str,"%4d-%02d-%02d %02d:%02d:%02d",dtime.tm_year+1900,dtime.tm_mon+1,\
		dtime.tm_mday,dtime.tm_hour,dtime.tm_min,dtime.tm_sec);
	return TRUE;
}
