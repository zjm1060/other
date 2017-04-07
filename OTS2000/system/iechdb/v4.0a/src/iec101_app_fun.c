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

SSWDEF		sswdef;
DSWDEF		dswdef;
ANADEF		anadef;
ITDEF		itdef;
SSWCTRL		sswctrl;
DSWCTRL		dswctrl;
SETPTCTRL	setpt;
STEPCTRL	stepctrl;

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

BOOL GenSSwTable()		
{
	FILE			*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];

	short		j;
	/* modified by yyp */

 	if((fp=fopen("/home/ems/h9000/iec101/ND_IEC101_SP_DATA.DEF","r"))==NULL)
 		{
 		printf("can't open file  /home/ems/h9000/iec101/SSW_DATA.DEF\n");
 		sswdef.number = 0;
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&sswdef.number);
	if(sswdef.number >= MAX_SSW_NUM)
		{
		printf("GenSSwTable: above the MAX_SSW_NUM %d\n",MAX_SSW_NUM);
		sswdef.number = MAX_SSW_NUM;
		} 

	for(j=0;j<sswdef.number;j++)
		{
		fscanf(fp,"%s",sswdef.name[j]);
		if(strcmp(sswdef.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("SP %d pnt: %s\n",j,sswdef.name[j]);
#endif
			strcpy(name_buf2,sswdef.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
					printf("#### SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
	printf("GenSSwTable: point number = %d\n",sswdef.number);
	fclose(fp);
	return(TRUE);          
}

BOOL GenDSwTable()		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];
	char			longname[100];
	int			iTemp;
	char		iStr[5];
	short		j;

 	if((fp=fopen("/home/ems/h9000/iec101/ND_IEC101_DP_DATA.DEF","r"))==NULL)
 		{
 		printf("can't open file /home/ems/h9000/iec101/DSW_DATA.DEF\n");
 		dswdef.number = 0;
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&dswdef.number);
	if(dswdef.number >= MAX_DSW_NUM)
		{
		printf("GenDSwTable: above the MAX_DSW_NUM %d\n",MAX_DSW_NUM);
		dswdef.number = MAX_DSW_NUM;
		} 

	for(j=0;j<dswdef.number;j++)
		{
		fscanf(fp,"%s	%s	%s",iStr,dswdef.name[j],longname);
		if(strcmp(dswdef.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("DP %d pnt: %s\n",j,dswdef.name[j]);
#endif
			strcpy(name_buf2,dswdef.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
				printf("#### DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
	printf("GenDSwTable: point number = %d\n",j);
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

BOOL GenITTable()		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];

	short		j;
 	if((fp=fopen("/home/ems/h9000/iec101/IT_DATA.DEF","r"))==NULL)
 		{
 		printf("can't open file /home/ems/h9000/iec101/IT_DATA.DEF\n");
 		itdef.number = 0;
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&itdef.number);
	if(itdef.number >= MAX_IT_NUM)
		{
		printf("GenITTable: above the MAX_IT_NUM %d\n",MAX_IT_NUM);
		itdef.number = MAX_IT_NUM;
		} 

	for(j=0;j<itdef.number;j++)
		{
		fscanf(fp,"%s",itdef.name[j]);
		if(strcmp(itdef.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("IT %d pnt: %s\n",j,itdef.name[j]);
#endif
			strcpy(name_buf2,itdef.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
				printf("#### IT %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
	printf("GenITTable: point number = %d\n",itdef.number);
	fclose(fp);
	return(TRUE);
}

BOOL	GenSSwCtrlTable()		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];

	short		j;
 	if((fp=fopen("/home/ems/h9000/iec101/SSW_CTRL.DEF","r"))==NULL)
 		{
 		printf("can't open file /home/ems/h9000/iec101/SSW_CTRL.DEF\n");
		sswctrl.number = 0;
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&sswctrl.number);
	if(sswctrl.number >= MAX_SSW_CTRL)
		{
		printf("GenSSwCtrlTable: above the MAX_SSW_CTRL %d\n",MAX_SSW_CTRL);
		sswctrl.number = MAX_SSW_CTRL;
		} 

	for(j=0;j<sswctrl.number;j++)
		{
		fscanf(fp,"%s",sswctrl.name[j]);
		if(strcmp(sswctrl.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("SC %d pnt: %s\n",j,sswctrl.name[j]);
#endif
			strcpy(name_buf2,sswctrl.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
				printf("#### SC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
	printf("GenSSwCtrlTable: point number = %d\n",sswctrl.number);
	fclose(fp);
	return(TRUE);
}

BOOL	GenDSwCtrlTable()		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];

	short		j;
 	if((fp=fopen("/home/ems/h9000/iec101/DSW_CTRL.DEF","r"))==NULL)
 		{
 		printf("can't open file /home/ems/h9000/iec101/DSW_CTRL.DEF\n");
	 	dswctrl.number = 0;
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&dswctrl.number);
	if(dswctrl.number >= MAX_DSW_CTRL)
		{
		printf("GenDSwCtrlTable: above the MAX_DSW_CTRL %d\n",MAX_DSW_CTRL);
		dswctrl.number = MAX_DSW_CTRL;
		} 

	for(j=0;j<dswctrl.number;j++)
		{
		fscanf(fp,"%s",dswctrl.name[j]);
		if(strcmp(dswctrl.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("DC %d pnt: %s\n",j,dswctrl.name[j]);
#endif
			strcpy(name_buf2,dswctrl.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
				printf("#### DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
	printf("GenDSwCtrlTable: point number = %d\n",dswctrl.number);
	fclose(fp);
	return(TRUE);
}

BOOL GenSetPCtrlTable()		   //set point control point define file
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];

	short		j;
 	if((fp=fopen("/home/ems/h9000/iec101/SETP_CTRL.DEF","r"))==NULL)
 		{
 		setpt.number = 0;
 		printf("can't open file /home/ems/h9000/iec101/SETP_CTRL.DEF\n");
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&setpt.number);
	if(setpt.number >= MAX_ANA_CTRL)
		{
		printf("GenSetPCtrlTable: above the MAX_ANA_CTRL %d\n",MAX_ANA_CTRL);
		setpt.number = MAX_ANA_CTRL;
		} 

	for(j=0;j<setpt.number;j++)
		{
		fscanf(fp,"%s %f %f",setpt.name[j],&setpt.lorange[j],&setpt.hirange[j]);
		if(strcmp(setpt.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("Set Pt %d pnt: %s %f %f\n",j,setpt.name[j],setpt.lorange[j],setpt.hirange[j]);
#endif
			strcpy(name_buf2,setpt.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
				printf("#### Set Pt %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
	printf("GenSetPCtrlTable: point number = %d\n",setpt.number);
	fclose(fp);
	return(TRUE);
}

BOOL	GenStepCtrlTable()		
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[NAME_SIZE];

	short		j;
 	if((fp=fopen("/home/ems/h9000/iec101/STEP_CTRL.DEF","r"))==NULL)
 		{
 		printf("can't open file /home/ems/h9000/iec101/STEP_CTRL.DEF\n");
	 	stepctrl.number = 0;
		return(FALSE);          
		}
		  
	rewind(fp);	
	
	fscanf(fp,"%d",&stepctrl.number);
	if(stepctrl.number >= MAX_STEP_CTRL)
		{
		printf("GenStepCtrlTable: above the MAX_STEP_CTRL %d\n",MAX_STEP_CTRL);
		stepctrl.number = MAX_STEP_CTRL;
		} 

	for(j=0;j<stepctrl.number;j++)
		{
		fscanf(fp,"%s",stepctrl.name[j]);
		if(strcmp(stepctrl.name[j],"NULL")!=0)
			{
#ifdef DEBUG
			printf("Step Ctrl %d pnt: %s\n",j,stepctrl.name[j]);
#endif
			strcpy(name_buf2,stepctrl.name[j]);
				if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				{
				printf("#### Step Ctrl %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				}
			}
		}
	printf("GenStepCtrlTable: point number = %d\n",dswctrl.number);
	fclose(fp);
	return(TRUE);
}

void	LanBcCmdError(POINTER ptr,short state)
{
	SYS_MSG		bj_buf;
	LAN_HEAD	lanhead;
	struct	tm	tmpt;
	time_t		tim;

	bj_buf.type_id =DPS_IND_ALM;
	bj_buf.stn_id =ptr.stn_id;
	bj_buf.dev_id =ptr.dev_id;
	bj_buf.data_type=ptr.data_type;
	bj_buf.number = 1;
	bj_buf.point_id = ptr.pt_id;

	bj_buf.status=state;
	bj_buf.ms=0;
	GetLocalHostId(&bj_buf.host_id);

	tim=time (NULL);
	tmpt=*localtime (&tim);
	bj_buf.sec=tmpt.tm_sec;
	bj_buf.min=tmpt.tm_min;
	bj_buf.hour=tmpt.tm_hour;
	bj_buf.day=tmpt.tm_mday;
	bj_buf.month=tmpt.tm_mon+1;
	bj_buf.year=tmpt.tm_year+1900;
	lanhead.length=sizeof(SYS_MSG);

#ifdef SUN_OS
	Swap2Byte(&bj_buf.number);
	Swap2Byte(&bj_buf.point_id);
	Swap2Byte(&bj_buf.status);
	Swap2Byte(&bj_buf.ms);
	Swap2Byte(&bj_buf.year);
	Swap2Byte(&lanhead.length);
#endif
	lanhead.dp_type=BJ_DB_TIME;
/*
	GetLocalHostId(&lanhead.src_id);
*/
	lanhead.dest_id =0;
	lanhead.src_id =bj_buf.host_id;
	lanhead.dest_stn =bj_buf.stn_id;
	lanhead.packet_num =0;
	lan_out ((char *)&bj_buf,lanhead,(unsigned char)0);
}


void	IEC870_5_Recv()
{

LAN_HEAD	lanhead;
DWORD	dwWaitRecvASDU;
USHORT	recvdatapt,infoaddr,infoptr,datalength,lengthfactor,ms,year;
short	ana16data;		/*USGORT to short. zy*/
int 	i,datapoint,data_type;
char	dbname[36];
unsigned char	stnd,type_id,varstru,causetra,infonum,qoc,lanpt;

SW_MSG		sw_buf[70];
ALL_SW_MSG	all_sw_buf[70];
unsigned char	me_buf[1440];
IND_DATA	sw_status;
ANLG_STATUS	me_status;

float	value;
int		count;

DMS_COMMON	dbcommpt;
ANLG_DATA	anadata;
struct	tm	tmpt;
time_t		tim;

printf("============IEC870_5_Recv thread start!!!===========\n");

GenSSwTable();		
GenDSwTable();		
GenAnaTable();		
//GenITTable();

recvdatapt = 0;


for(;;)
	{
	dwWaitRecvASDU = WaitForSingleObject(hRecvSemaphore,INFINITE);
	
#ifdef DEBUG
	printf("#####################IEC870_5_Recv: RecvSempahore set, %d remote ASDU has received...\n",recvdatapt);
#endif
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
					tim=time(NULL);
					tmpt=*localtime(&tim);

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
							if(infoptr > sswdef.number)
								{
#ifdef DEBUG
								printf("M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,sswdef.number);
#endif					
								break;
								}
							strcpy(dbname,sswdef.name[infoptr]);
						}
						else
						{
							if(infoptr > dswdef.number)
								{
#ifdef DEBUG
								printf("M_DP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dswdef.number);
#endif					
								break;
								}
								strcpy(dbname,dswdef.name[infoptr]);
						}

						infoptr ++;

						if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
						{
							printf("\nIEC No this point name=%s in db\n",dbname);
							continue;
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
						if(dbcommpt.var.ind.status.rt_state == sw_status.rt_state)
						{
							continue;
						}

						sw_buf[lanpt].type_id =DPS_IND_ALM;
						sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
						sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
						sw_buf[lanpt].data_type=dbcommpt.point.data_type;
						sw_buf[lanpt].number = 1;
						sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

						sw_buf[lanpt].status=sw_status;
						
						sw_buf[lanpt].status.alm_type = 1;
						
						sw_buf[lanpt].ms=0;
						GetLocalHostId(&sw_buf[lanpt].host_id);

						sw_buf[lanpt].sec=tmpt.tm_sec;
						sw_buf[lanpt].min=tmpt.tm_min;
						sw_buf[lanpt].hour=tmpt.tm_hour;
						sw_buf[lanpt].day=tmpt.tm_mday;
						sw_buf[lanpt].month=tmpt.tm_mon+1;
						sw_buf[lanpt].year=tmpt.tm_year+1900;
						lanpt ++;
						if( lanpt >= 70 )
						{
							lanhead.length = lanpt*sizeof(SW_MSG);
							lanhead.dp_type=BJ_DB_TIME;
							GetLocalHostId(&lanhead.src_id);
							lanhead.dest_id =0;
							lanhead.dest_stn =sw_buf[0].stn_id;
							lanhead.packet_num =0;
							lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
							lanpt = 0;
						}
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

					lanpt = 0;
					for(i=0;i<infonum;i++)
						{
						datapoint = i*3+6;
						memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
						if(type_id == M_SP_NA)
							{
							infoptr = infoaddr - SP_BASE_ADDR;
							if(infoptr > sswdef.number)
								{
#ifdef DEBUG
								printf("M_SP_NA SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,sswdef.number);
#endif					
								break;
								}
							strcpy(dbname,sswdef.name[infoptr]);
							infoaddr ++;
							}
						else
							{
							infoptr = infoaddr - DP_BASE_ADDR;
							if(infoptr > dswdef.number)
								{
#ifdef DEBUG
								printf("M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dswdef.number);
#endif					
								break;
								}
							strcpy(dbname,dswdef.name[infoptr]);
							infoaddr += 2;
							}

						if(strcmp(dbname,"NULL")==0)
							continue;
						if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
						{
							printf("\nIEC No this point name=%s in db\n",dbname);
							continue;
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
						if(dbcommpt.var.ind.status.rt_state == sw_status.rt_state)
						{
							continue;
						}

						sw_buf[lanpt].type_id =DPS_IND_ALM;
						sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
						sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
						sw_buf[lanpt].data_type=dbcommpt.point.data_type;
						sw_buf[lanpt].number = 1;
						sw_buf[lanpt].point_id =dbcommpt.point.pt_id;
						sw_buf[lanpt].status=sw_status;
						sw_buf[lanpt].status.alm_type = 1;

						sw_buf[lanpt].ms=0;
						GetLocalHostId(&sw_buf[lanpt].host_id);

						sw_buf[lanpt].sec=tmpt.tm_sec;
						sw_buf[lanpt].min=tmpt.tm_min;
						sw_buf[lanpt].hour=tmpt.tm_hour;
						sw_buf[lanpt].day=tmpt.tm_mday;
						sw_buf[lanpt].month=tmpt.tm_mon+1;
						sw_buf[lanpt].year=tmpt.tm_year+1900;
						lanpt ++;
						if( lanpt >= 70 )
						{
							lanhead.length = lanpt*sizeof(SW_MSG);
							lanhead.dp_type=BJ_DB_TIME;
							GetLocalHostId(&lanhead.src_id);
							lanhead.dest_id =0;
							lanhead.dest_stn =sw_buf[0].stn_id;
							lanhead.packet_num =0;
							lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
							lanpt = 0;
						}
					}//end for
					}//if(SQ)
				if( lanpt > 0 )
					{
						lanhead.length = lanpt*sizeof(SW_MSG);
						lanhead.dp_type=BJ_DB_TIME;
						lanhead.dest_stn =sw_buf[0].stn_id;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.packet_num =0;
						lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
					}		
				break;

			//modified by yyp for M_SP_TA		
			case M_SP_TA:	//single point with time tag {data class 2 }
			case M_DP_TA:	//double point with time tag {data class 4 }
					infonum = varstru & 0x7F;
					datalength = 6*infonum+6;
					if(datalength != recv_data.length[recvdatapt])
					{
						printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
								type_id,infonum,recv_data.length[recvdatapt]);
						break;
					}

					lanpt = 0;
					for(i=0;i<infonum;i++)
						{
						datapoint = 6*i+6;
						memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
						if(type_id ==M_SP_TA)
							{
							infoptr = infoaddr - SP_BASE_ADDR;
							if(infoptr > sswdef.number)
								{
#ifdef DEBUG
								printf("M_SP_TA SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,sswdef.number);
#endif					
								break;
								}
							strcpy(dbname,sswdef.name[infoptr]);
							}
						else
							{
							infoptr = infoaddr - DP_BASE_ADDR;
							if(infoptr > dswdef.number)
								{
#ifdef DEBUG
								printf("M_DP_TA SQ=0: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dswdef.number);
#endif					
								break;
								}
							strcpy(dbname,dswdef.name[infoptr]);
							}

						if(strcmp(dbname,"NULL")==0)
							continue;
						if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
						{
							printf("\nIEC No this point name=%s in db\n",dbname);
							continue;
						}

						qoc = recv_data.data[recvdatapt][datapoint+2];
						if(type_id ==M_SP_TA)
							{
							if(qoc & 1)				   //SIQ:= IV NT SB BL 0 0 0 SPI
								sw_status.rt_state = 1;
							else
								sw_status.rt_state = 0;
							}
						else
							{
							switch(qoc & 0x03)		   //DIQ:= IV NT SB BL 0 0 DPI[0..1] 
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
							sw_status.chan_fail = 1;
						else
							sw_status.chan_fail = 0;	//channel fault
						sw_status.fault_tag = 0;
						//modified by yyp 07.03.11
						if(dbcommpt.var.ind.status.rt_state == sw_status.rt_state)
						{
							continue;
						}

						sw_buf[lanpt].type_id =DPS_IND_ALM;
						sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
						sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
						sw_buf[lanpt].data_type=dbcommpt.point.data_type;
						
						sw_buf[lanpt].number = 1;
						sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

						sw_buf[lanpt].status=sw_status;
						sw_buf[lanpt].status.alm_type = 1;

						GetLocalHostId(&sw_buf[lanpt].host_id);
						memcpy(&ms,&recv_data.data[recvdatapt][datapoint+4],2);
						sw_buf[lanpt].ms = ms%1000;
						sw_buf[lanpt].sec=(char)(ms/1000);
						sw_buf[lanpt].min=recv_data.data[recvdatapt][datapoint+6] & 0x3F;
						sw_buf[lanpt].hour=tmpt.tm_hour;
						sw_buf[lanpt].day=tmpt.tm_mday;
						sw_buf[lanpt].month=tmpt.tm_mon+1;
						sw_buf[lanpt].year=tmpt.tm_year+1900;
						lanpt ++;
						if( lanpt>=70 )
						{
							lanhead.length = lanpt*sizeof(SW_MSG);
							lanhead.dp_type=BJ_DB_TIME;
							GetLocalHostId(&lanhead.src_id);
							lanhead.dest_id =0;
							lanhead.dest_stn =sw_buf[0].stn_id;
							lanhead.packet_num =0;
							lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
							lanpt = 0;
						}
					}//end for
					if( lanpt>0 )
					{
						lanhead.length = lanpt*sizeof(SW_MSG);
						lanhead.dp_type=BJ_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
					}				
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

					lanpt = 0;
					for(i=0;i<infonum;i++)
					{
						datapoint = 10*i+6;
						memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
						if(type_id ==M_SP_TB)
						{
							infoptr = infoaddr - SP_BASE_ADDR;
							if((infoptr < 0) || (infoptr > dswdef.number))
							{
			#ifdef DEBUG
								printf("M_SP_TB SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,dswdef.number);
			#endif
								break;
							}
							strcpy(dbname,dswdef.name[infoptr]);
						}
						else
						{
							infoptr = infoaddr - DP_BASE_ADDR;
							if((infoptr < 0) || (infoptr > dswdef.number))
							{
			#ifdef DEBUG
								printf("M_DP_TB SQ=0: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dswdef.number);
			#endif
								break;
							}
							strcpy(dbname,dswdef.name[infoptr]);
						}
						if(strcmp(dbname,"NULL")==0)
							continue;
						if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
						{
							printf("\nIEC No this point name=%s in db\n",dbname);
							continue;
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
						if(dbcommpt.var.ind.status.rt_state == sw_status.rt_state)
						{
							continue;
						}

						sw_buf[lanpt].type_id =DPS_IND_ALM;
						sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
						sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
						sw_buf[lanpt].data_type=dbcommpt.point.data_type;
						sw_buf[lanpt].number = 1;
						sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

						sw_buf[lanpt].status=sw_status;
						sw_buf[lanpt].status.alm_type = 1;
						GetLocalHostId(&sw_buf[lanpt].host_id);
						memcpy(&ms,&recv_data.data[recvdatapt][datapoint+3],2);

						sw_buf[lanpt].ms = ms%1000;
						sw_buf[lanpt].sec=(char)(ms/1000);
						sw_buf[lanpt].min=recv_data.data[recvdatapt][datapoint+5] & 0x3F;
						sw_buf[lanpt].hour=recv_data.data[recvdatapt][datapoint+6] & 0x1F;
						sw_buf[lanpt].day=recv_data.data[recvdatapt][datapoint+7] & 0x1F;
						sw_buf[lanpt].month=recv_data.data[recvdatapt][datapoint+8] & 0x0F;
						sw_buf[lanpt].year=(recv_data.data[recvdatapt][datapoint+9] & 0x7F)+2000;
						lanpt ++;
						if( lanpt>=70 )
						{
							lanhead.length = lanpt*sizeof(SW_MSG);
							lanhead.dp_type=BJ_DB_TIME;
							GetLocalHostId(&lanhead.src_id);
							lanhead.dest_id =0;
							lanhead.dest_stn =sw_buf[0].stn_id;
							lanhead.packet_num =0;
							lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
							lanpt = 0;
						}
					}
					if( lanpt>0 )
					{
						lanhead.length = lanpt*sizeof(SW_MSG);
						lanhead.dp_type=BJ_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
					}		
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

				
			case M_ME_ND:	//measure value without quality:21
	

					infonum = varstru & 0x7F;			//number of information object
					if(varstru & 0x80)	//SQ=1
						{
						lengthfactor = 2;
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
								printf("M_ME_ND SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,anadef.number);
#endif					
								break;
								}

							strcpy(dbname,anadef.name[infoptr]);

							if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
							{
								printf("\nIEC No this point name=%s in db\n",dbname);
								continue;
							}

							memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);

							value=(float)((ana16data*(anadef.hirange[infoptr]-anadef.lorange[infoptr]))/RESOLUTE
									 + anadef.lorange[infoptr]);

							infoptr ++;

							*(USHORT *)&anadata.status = 0;
							if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
								anadata.status.ctrl_block = 1;	/* manual set */
							if(qoc & 0xD0)
								anadata.status.chan_fail = 1;	/* channel fault */

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
							
							me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
							me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
							me_buf[lanpt*22+3] =dbcommpt.point.data_type;
							me_buf[lanpt*22+4] = 1;		/* number low byte */
							me_buf[lanpt*22+5] = 0;		/* number high byte */
							memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
							memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
							memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
							GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */
							me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
							me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
							me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
							me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
							me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
							year=tmpt.tm_year+1900;	/* year */
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
					else  //sq=0
						{
						lengthfactor = 4;
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
#ifdef DEBUG
							printf("M_ME_ND SQ=0: infoaddr=%x  infoptr=%d infonum=%d\n",infoaddr,infoptr,infonum);
#endif
							if(infoptr > anadef.number)
								{
#ifdef DEBUG
								printf("M_ME_ND SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,anadef.number);
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
							memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint+3],2);
							value=(float)((ana16data*(anadef.hirange[infoptr]-anadef.lorange[infoptr]))/32767.0
								 + anadef.lorange[infoptr]);

							*(USHORT *)&anadata.status = 0;
							if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
								anadata.status.ctrl_block = 1;	/* manual set */
							if(qoc & 0xD0)
								anadata.status.chan_fail = 1;	/* channel fault */

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
							
							me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
							me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
							me_buf[lanpt*22+3] =dbcommpt.point.data_type;
							me_buf[lanpt*22+4] = 1;		/* number low byte */
							me_buf[lanpt*22+5] = 0;		/* number high byte */
							memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
							memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
							memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
							GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

							me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
							me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
							me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
							me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
							me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
							year=tmpt.tm_year+1900;	/* year */
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

			case M_ST_NA:	//step point without time tag:5
			case M_ST_TA:	//step point with time tag:6
					infonum = varstru & 0x7F;			//number of information object
					if(type_id == M_ST_NA)
						{
						tim=time(NULL);
						tmpt=*localtime(&tim);

						lengthfactor = 4;
						}
					else
						lengthfactor = 7;
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
						strcpy(dbname,anadef.name[infoptr]);

						if(strcmp(dbname,"NULL")==0)
							continue;
						if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
						{
							printf("\nIEC No this point name=%s in db\n",dbname);
							continue;
						}
						qoc = recv_data.data[recvdatapt][datapoint+4];

						anadata.data = (recv_data.data[recvdatapt][datapoint+3] & 0x3F)*1000;

						if(recv_data.data[recvdatapt][datapoint+3] & 0x40)	/* VTI = T VALUE:=I7[1..7]<-64..63> */
							anadata.data = -anadata.data;	/* negative */

						if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
							anadata.status.ctrl_block = 1;	/* manual set */
						if(qoc & 0xD0)
							anadata.status.chan_fail = 1;	/* channel fault */

						if(causetra == SPONT)
							me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */
						else
							me_buf[lanpt*22] =DPS_ANA_ALL;	/* type_id */

						me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
						me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
						me_buf[lanpt*22+3] =dbcommpt.point.data_type;
						me_buf[lanpt*22+4] = 1;		/* number low byte */
						me_buf[lanpt*22+5] = 0;		/* number high byte */
						memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
						memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
						memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
						GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

						if(type_id ==M_ST_NA)
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
							memcpy(&ms,&recv_data.data[recvdatapt][datapoint+5],2);
							me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
							me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+7] & 0x3F;	/* minute */
							if(type_id ==M_ST_TA)
							{
								me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
								me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
								me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
								year=tmpt.tm_year+1900;		/* year */
							}
							else		/* M_ST_TB */
							{
								me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
								me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
								me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
								year=recv_data.data[recvdatapt][datapoint+11] & 0x7F+2000;		/* year */
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

			case M_IT_NA:	//integrated totals without time tag:15
			case M_IT_TA:	//integrated totals with time tag:16
					infonum = varstru & 0x7F;			/* number of information object */
					if(varstru & 0x80)	/* SQ=1 */
					{
						if(type_id == M_IT_NA)
						{
							lengthfactor = 5;
						}
						else if(type_id == M_IT_TA)
							lengthfactor = 8;
						else
							lengthfactor = 12;
						datalength = lengthfactor*infonum+9;
						if(datalength != recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
						{
							printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
									type_id,infonum,recv_data.length[recvdatapt]);
							break;
						}

						lanpt = 0;
						memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
						infoptr = infoaddr - IT_BASE_ADDR;
						for(i=0;i<infonum;i++)
						{
							datapoint = lengthfactor*i+9;

							if((infoptr < 0) || (infoptr >= itdef.number))
							{
			#ifdef DEBUG
								printf("M_IT_NA SQ=1: infoaddr=%x  infoptr=%d > itnum=%d\n",infoaddr,infoptr,itdef.number);
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

							memcpy(&count,&recv_data.data[recvdatapt][datapoint],4);
							infoptr ++;

							*(USHORT *)&anadata.status = 0;
							if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
								anadata.status.ctrl_block = 1;	/* manual set */
							if(qoc & 0xD0)
								anadata.status.chan_fail = 1;	/* channel fault */

							if(dbcommpt.point.data_type == IMP_TYPE)
							{
								anadata.data = (GLONG)count & 0x7FFF;
								if(causetra == SPONT)
									me_buf[lanpt*22] =DPS_IMPULSE;	/* type_id */
								else
									me_buf[lanpt*22] =DPS_IMP_ALL;	/* type_id */
							}
							else
							{
								anadata.data = (GLONG)((count & 0x7FFF)*1000);
								if(causetra == SPONT)
									me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */
								else
									me_buf[lanpt*22] =DPS_ANA_ALL;	/* type_id */
							}
							
							me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
							me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
							me_buf[lanpt*22+3] =dbcommpt.point.data_type;
							me_buf[lanpt*22+4] = 1;		/* number low byte */
							me_buf[lanpt*22+5] = 0;		/* number high byte */
							memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
							memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
							memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
							GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

							if(type_id ==M_IT_NA)
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

								memcpy(&ms,&recv_data.data[recvdatapt][datapoint+5],2);
								me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
								me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+7] & 0x3F;	/* minute */
								if(type_id ==M_IT_TA)
								{
									me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
									me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
									me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
									year=tmpt.tm_year+1900;		/* year */
								}
								else		/* M_IT_TB */
								{
									me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
									me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
									me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
									year=recv_data.data[recvdatapt][datapoint+11] & 0x7F+2000;		/* year */
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
					else		/* SQ=0 */
					{
						if(type_id == M_IT_NA)
						{
							lengthfactor = 8;
						}
						else if(type_id == M_IT_TA)
							lengthfactor = 11;
						else
							lengthfactor = 15;
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
							memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
							infoptr = infoaddr - IT_BASE_ADDR;
							if((infoptr < 0) || (infoptr > itdef.number))
							{
			#ifdef DEBUG
								printf("M_IT_NA SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,itdef.number);
			#endif
								break;
							}
							strcpy(dbname,itdef.name[infoptr]);

							if(strcmp(dbname,"NULL")==0)
								continue;
							if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
							{
								printf("\nIEC No this point name=%s in db\n",dbname);
								continue;
							}
							qoc = recv_data.data[recvdatapt][datapoint+7];
							memcpy(&count,&recv_data.data[recvdatapt][datapoint+3],4);
							*(USHORT *)&anadata.status = 0;
							if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
								anadata.status.ctrl_block = 1;	/* manual set */
							if(qoc & 0xD0)
								anadata.status.chan_fail = 1;	/* channel fault */

							if(dbcommpt.point.data_type == IMP_TYPE)
							{
								anadata.data = (GLONG)(count & 0x7FFF);
								if(causetra == SPONT)
									me_buf[lanpt*22] =DPS_IMPULSE;	/* type_id */
								else
									me_buf[lanpt*22] =DPS_IMP_ALL;	/* type_id */
							}
							else
							{
								anadata.data = (GLONG)((count & 0x7FFF)*1000);
								if(causetra == SPONT)
									me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */
								else
									me_buf[lanpt*22] =DPS_ANA_ALL;	/* type_id */
							}
							
							me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
							me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
							me_buf[lanpt*22+3] =dbcommpt.point.data_type;
							me_buf[lanpt*22+4] = 1;		/* number low byte */
							me_buf[lanpt*22+5] = 0;		/* number high byte */
							memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
							memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
							memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
							GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

							if(type_id ==M_IT_NA)
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
								memcpy(&ms,&recv_data.data[recvdatapt][datapoint+8],2);
								me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
								me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+10] & 0x3F;	/* minute */
								if(type_id ==M_IT_TA)
								{
									me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
									me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
									me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
									year=tmpt.tm_year+1900;		/* year */
								}
								else		/* M_IT_TB */
								{
									me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+11] & 0x1F;	/* hour */
									me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+12] & 0x1F;	/* day */
									me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+13] & 0x0F;	/* month */
									year=recv_data.data[recvdatapt][datapoint+14] & 0x7F+2000;		/* year */
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

			case C_SE_NA:	//confirm of set-point command
					
#ifdef DEBUG
					memcpy(&infoaddr, &recv_data.data[recvdatapt][6],2);
					memcpy(&ana16data, &recv_data.data[recvdatapt][8],2);
#endif
					if((causetra & 0x3F) == ACTCON)
						{
						if(causetra & 0x40)		//0 positive 1 negative confirm
							{
							cmd_ack[stn_id] = -1;		//negative confirm
#ifdef DEBUG
							printf("set-point %u NACK INF=%d stval=%x cause of transmission=%2x received!\n",type_id,infoaddr,ana16data,causetra);
#endif
							}
						else
							{
							cmd_ack[stn_id] = 0;		//execute command confirm
							comdflag=comdflag1 = 0;
#ifdef DEBUG
							printf("set-point %u ACK INF=%d stval=%x cause of transmission=%2x received!\n",type_id,infoaddr,ana16data,causetra);
#endif
							}
						}
					else
						{
						cmd_ack[stn_id] = 0;
#ifdef DEBUG
						printf("set-point %u INF=%d com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,qoc,causetra);
#endif
						}
					
					break;


			case P_ME_NA:	//parameter for measurements,normalized
					
#ifdef DEBUG
					memcpy(&infoaddr, &recv_data.data[recvdatapt][6],2);
					memcpy(&ana16data, &recv_data.data[recvdatapt][8],2);
#endif
					if((causetra & 0x3F) == ACTCON)
						{
						if(causetra & 0x40)		//0 positive 1 negative confirm
							{
							cmd_ack[stn_id] = -1;		//negative confirm
#ifdef DEBUG
							printf("parameter set %u NACK INF=%d stval=%x kind of parameter=%2x received!\n",type_id,infoaddr,ana16data,recv_data.data[recvdatapt][10]);
#endif
							}
						else
							{
							cmd_ack[stn_id] = 0;		//execute command confirm
							comdflag=comdflag1 = 0;
#ifdef DEBUG
							printf("parameter set %u ACK INF=%d stval=%x kind of parameter=%2x received!\n",type_id,infoaddr,ana16data,recv_data.data[recvdatapt][10]);
#endif
							}
						}
					else
						{
						cmd_ack[stn_id] = 0;
#ifdef DEBUG
						printf("parameter set %u INF=%d com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,qoc,causetra);
#endif
						}
					
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

// Send Thread Function
void	IEC870_5_Send()
{
	short	sd_pnt,cmd_end,cmd_pnt,length;
	char	send_dev_id,cmd_type;
//	char	name[36];

	POINTER	ptr;
	short	cmd_stat,cmd_data,normv;
//	char	host_id;
	short	ob_addr,base_addr,i;
	float	ftemp,setpv;
	char	name[36];

//	DMS_COMMON 	dbptr;

//	printf("------------IEC870_5_Send thread start!!!-----------\n");

//	GenSSwCtrlTable();
//	Lan_init();
	//read h9000 recv shm 
	cmd_end = shm_recv_addr->head;
	sd_pnt = 0;
	cmd_type = 0;
//		initflag[stn_id] = 0;
	for(;;)
		{
		Sleep(30000);
#ifdef YYP
					sd_pnt ++;
					if(sd_pnt>=CMD_ENTRY_NUMBER)
						sd_pnt = 0;
					send_data.pnt = sd_pnt;
/*
       for(;;){

			if(shm_send_addr->flag!=LOCK_FLAG)	break;
		
			Sleep(100);
		}
		cmd_end=shm_send_addr->head;
		shm_send_addr->shm_send_seg[cmd_end].lan_head.length=head.length;	  

		shm_send_addr->shm_send_seg[cmd_end].lan_head.dp_type=head.dp_type;
		
		shm_send_addr->shm_send_seg[cmd_end].lan_head.dest_id=head.dest_id;

   	  	shm_send_addr->shm_send_seg[cmd_end].lan_head.src_id=head.src_id;

		memcpy(shm_send_addr->shm_send_seg[cmd_end].data,buff,head.length);

		cmd_end++;

		if(cmd_end>=MAX_SEND_NUM)	cmd_end=0;

		shm_send_addr->flag=IDLE_FLAG;
*/
//	printf("---cmd_end=%d shm_recv_addr->head %d---\n",cmd_end,shm_recv_addr->head);
		while( cmd_end!=shm_recv_addr->head )
			{
			cmd_pnt = cmd_end;
			cmd_end++;
			if(cmd_end >= MAX_RECV_NUM)
				cmd_end = 0;
			//if recv shm seg type is not broadcasting command record,the next
			if(shm_recv_addr->shm_recv_seg[cmd_pnt].lan_head.dp_type != COMM_RECORD)
				continue;
	   		
	   		length=shm_recv_addr->shm_recv_seg[cmd_pnt].lan_head.length;
			if(length<10)			  //the length of command buf is 10
				continue;
			
			send_dev_id=shm_recv_addr->shm_recv_seg[cmd_pnt].data[0];
			cmd_type=shm_recv_addr->shm_recv_seg[cmd_pnt].data[1];

			if(send_dev_id < MIN_RMT_DEV_ID || send_dev_id > MAX_RMT_DEV_ID || cmd_type < 24)
				continue;
/*
			if(pollflag==0)
				{
				LanBcCmdError(SYS_RTU,DPS_SYSMSG,CMDLINKERR,4);
				continue;
				}
*/
			switch	( cmd_type ) 
				{
				case C_SWITCH:
				case C_BREAKER:
				case C_DEVICE:
				case C_GATE:
				case C_UNIT:
				case C_APOWER:
				case C_RPOWER:
#ifdef DEBUG
//					printf("dev_id=%d  cmd_type=%d point_id=%d cmd_stat=%d cmd_data=%d type=%d host_id=%d\n",
//						ptr.dev_id, cmd_type,ptr.point_id,cmd_stat,cmd_data,ptr.data_type,host_id);
#endif
/*
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
*/
					memcpy((char *)&send_data.data[sd_pnt][0],&shm_recv_addr->shm_recv_seg[cmd_pnt].data[0],10);	

					send_data.length[sd_pnt] = 10;			//cmd data length of ASDU
/*					

					send_data.data[sd_pnt][1] = 1;			//number of object(variable structure)
					send_data.data[sd_pnt][2] = ACT;		//cause of transmit
					send_data.data[sd_pnt][3] = 0;			//common address of ASDU
					ob_addr = i + base_addr;
					send_data.data[sd_pnt][4] = (unsigned char)ob_addr;		//object address (low octet)
					send_data.data[sd_pnt][5] = (unsigned char)((ob_addr>>8) & 0xFF);	//object address (high octet)
*/
					sd_pnt ++;
					if(sd_pnt>=CMD_ENTRY_NUMBER)
						sd_pnt = 0;
					send_data.pnt = sd_pnt;
					break;
//				case C_APOWER:
//				case C_RPOWER:
					send_data.data[sd_pnt][0] = C_SE_NA;	//set-point command
					base_addr = SETP_CMD_BASE_ADDR;
					memset(name,0,36);
					ptr.dev_id = shm_recv_addr->shm_recv_seg[cmd_pnt].data[0];
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

					memcpy(&cmd_stat,&shm_recv_addr->shm_recv_seg[cmd_pnt].data[4],2);	
					anadata.status = cmd_stat;
					memcpy(&cmd_data,&shm_recv_addr->shm_recv_seg[cmd_pnt].data[6],2);	
					anadata.data = cmd_data;
					setpv = GetAnaValue ( &anadata );

					ftemp = (float)(setpv - setpt.lorange[i])* RESOLUTE / (setpt.hirange[i]-setpt.lorange[i]);
						 	 //float value to Normalized

					normv = (short)ftemp;
					send_data.data[sd_pnt][6] = (unsigned char)normv;			//set point data (low octet)
					send_data.data[sd_pnt][7] = (unsigned char)((normv>>8) & 0xFF);	//set point data (high octet)
					send_data.data[sd_pnt][8] = 0;

					sd_pnt ++;
					if(sd_pnt>=CMD_ENTRY_NUMBER)
						sd_pnt = 0;
					send_data.pnt = sd_pnt;
					break;

			}//switch(cmd_type)
		}//while(shm_send_addr->head!= cmd_end)
#endif //YYP
  }//for(;;)

}//IEC870_5_Send()
