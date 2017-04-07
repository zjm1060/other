/************************************************************
*  FILE NAME:           RTU_fun.c      						*
*  FUNCTION:            Controlled Station					*
*						Basic Communication Function		*
*  Version :            Win3.0a-->UNIX4.0a  by  zwj		              							*
*  DATE:                2007.12                			*
*															*
************************************************************/

#include "../inc/sc1801RTU.h"

#define	DEBUG
int retn;
extern	char	infobuf[1024];

#ifdef SUN_OS
size_t	byte_to_send;	
ssize_t	send_byte;
#else
DWORD	byte_to_send,send_byte;
#endif
/*extern	unsigned char	sendbuf[1024],recvbuf[1024];*/
unsigned short	bytecount;
unsigned char sendbuf[1024];
unsigned char recvbuf[1024];
unsigned char	cmd_src;
extern	ASDUDATA	send_data,send_data2;
extern	ASDUCOMDDATA		recv_data;
PROC_REC	sProcRec[MAX_PROC_NUM+1];
int	hPort_com;
extern unsigned char	rtu_status,rtu_jumpter;
extern unsigned char	rtuconfig[16];
extern unsigned char	pntname[16][32][NAME_SIZE];
extern unsigned short	soenum;
extern int		soeflag[16];
extern SOEDEF	soedef[16*24];
extern SOEBUF	soebuf;
extern float	lorange[16][12],hirange[16][12];
extern unsigned short	databand;

unsigned short	pulsedata[96];
unsigned char	orgbuf[16*24];
unsigned char	cmdsendbuf[24];

char	cmdrecfile[80];
unsigned	short	daysaved;
 
void	ClearRecvFault();
void	ClearSendRespFault();
void	PrintRecvData();
void	PrintSendData();
void	PrintSendCmdData();
void	PrintRecvData();

extern GetHost_Cmd();/****zwj**/

BOOL	ResponseNAK(hPort_com,cmd,err_code,sendbuf)
int	hPort_com;
unsigned char	cmd,err_code;
unsigned char	sendbuf[];
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = NAK | 0x40;

	bytecount = 3;
	
	sendbuf[2] = 0;
	sendbuf[3] = (unsigned char)bytecount;							/*L*/
	sendbuf[4] = rtu_status;
	sendbuf[5] = cmd;
	sendbuf[6] = err_code;

	byte_to_send = bytecount + 5;
	
	chksum = 0xff;

	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

        sprintf(infobuf,"\nRespondLinkStatus: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	ReportRTUConfig(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{
	unsigned char	chksum,n;
	sendbuf[0] = STATION_ID;
	sendbuf[1] = RRC | 0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	sendbuf[2] = 0;	/*High length*/
	sendbuf[3] = 18;	/*Low length*/
	sendbuf[4] = rtu_status;
	
	for(n=0;n<16;n++)
		sendbuf[n+5] = rtuconfig[n];

	/*sendbuf[21] = rtu_jumpter = 0x60;*/
	sendbuf[21] = rtu_jumpter;

	byte_to_send = 23;

	chksum = 0xff;

	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

	sprintf(infobuf,"\nRespondLinkStatus: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	RepeatTransmit(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{
	unsigned short byte_num,n;
	unsigned char	chksum;
	
	sendbuf[1] = sendbuf[1] | 0x80;

	byte_num = (sendbuf[2] << 8) | sendbuf[3];

	byte_to_send = byte_num + 5;

	chksum = 0xff;

	for(n=0;n<byte_to_send - 1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

	sprintf(infobuf,"\nRespondLinkStatus: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	CommandSend(hPort_com,comdpnt,sendbuf)
int	hPort_com;
unsigned int	comdpnt;
unsigned char	sendbuf[];
{
	unsigned char	chksum,n;

	memcpy(&sendbuf[0],&send_data.data[comdpnt],send_data.length[comdpnt]);

	byte_to_send = send_data.length[comdpnt];

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 /*checksum char*/

	sprintf(infobuf,"\nCommandSend: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	UpdateOrgData()
{
	unsigned char	slot,pntnum,dicode,bitmap;
	unsigned short	anaorgcode,bufpoint,stat;
	DMS_COMMON		dbentry;
/*	int				data_type;*/
	char			name_buf[NAME_SIZE];
	float			value;

	bufpoint = 0;
	for(slot=0;slot<16;slot++)
	{
		switch(rtuconfig[slot])
		{
			case AI:
#ifdef DEBUG
					printf("Slot %2d Analog Input Board\n",slot);
#endif
					for(pntnum=0;pntnum<12;pntnum++)
					{
						if(strcmp(pntname[slot][pntnum],"NULL")==0)
						{
							orgbuf[bufpoint]=0x40;
							bufpoint++;
							orgbuf[bufpoint]=0;
							bufpoint++;
							continue;
						}
						strcpy(name_buf,pntname[slot][pntnum]);
						if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
						{
							printf("%s No this point in the database file!\n",name_buf);
							orgbuf[bufpoint]=0x40;
							bufpoint++;
							orgbuf[bufpoint]=0;
							bufpoint++;
							continue;
						}
						value=(float)(dbentry.var.anlg.value.data/1000.0);
						if(value <= lorange[slot][pntnum])
							anaorgcode = 0x4000;
						else	if(value >= hirange[slot][pntnum])
							anaorgcode = 0x4FFF;
						else
						{
							anaorgcode=(unsigned short)((value-lorange[slot][pntnum])*RESOLUTE/(hirange[slot][pntnum]-lorange[slot][pntnum])+0.5);
							anaorgcode=(anaorgcode&0x0FFF)|0x4000;
						}
						orgbuf[bufpoint]=(unsigned char)(anaorgcode>>8);
						bufpoint++;
						orgbuf[bufpoint]=(unsigned char)anaorgcode;
						bufpoint++;
					}
					break;
		
			case DI:
#ifdef DEBUG
					printf("Slot %2d Digital Input Board\n",slot);
#endif
					dicode = 0x80;
					bitmap = 0x01;
					for(pntnum=0;pntnum<24;pntnum++)
					{
						if(strcmp(pntname[slot][pntnum],"NULL")==0)
						{
							dicode = dicode & (~bitmap);
						}
						else
						{
							strcpy(name_buf,pntname[slot][pntnum]);
							if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
							{
								printf("%s No this point in the database file!\n",name_buf);
								dicode = dicode & (~bitmap);
							}
							else
							{
								/*stat=(dbentry.entry.ind.variable.value.status&0x3c)>>2;*/
								stat=dbentry.var.ind.status.rt_state;
								if(stat != 1)
									stat = 0;	/* OFF:QDS IV NT SB BL RES3 SPI */

								if(dbentry.var.ind.status.man_set)
									stat = stat | 0x20;			/* SB=1 */
								if(dbentry.var.ind.status.chan_fail)
									stat = stat | 0xC0;			/* IV=1; NT=1 */
								if(dbentry.fixed.iost.scanEnable==0)
									stat = stat | 0x10;			/* BL=1 */
								if(stat==1)
									dicode = dicode | bitmap;
								else
									dicode = dicode & (~bitmap);
							}
						}
						bitmap = bitmap<<1;
						if(bitmap==0x40)
						{
							orgbuf[bufpoint]=dicode;
							bufpoint++;
							dicode = 0x80;
							bitmap = 0x01;
						}
					}
					break;

			default :
#ifdef DEBUG
				printf("sc1801 RTU Slot %2d = %x Not AI/DI :\n",slot,rtuconfig[slot]);
#endif
				break;			

		}/*switch(rtuconfig[slot])*/
	}/*for*/
	return(TRUE);
}

BOOL	ReportFullData(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{

	unsigned char	chksum,slot,pntnum,dicode,bitmap;
	unsigned short	n,anaorgcode,bufpoint,stat;
	DMS_COMMON		dbentry;
/*	int				data_type;*/
	char			name_buf[NAME_SIZE];
	float			value;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = DRF | 0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
/**	sendbuf[4] = rtu_status;*/
	bufpoint = 5;
	bytecount = 1;
	
	for(slot=0;slot<16;slot++)
	{

		switch(rtuconfig[slot])
		{
			case AI:
#ifdef DEBUG
					printf("Slot %2d Analog Input Board\n",slot);
#endif
					bytecount = bytecount + 24;
					for(pntnum=0;pntnum<12;pntnum++)
					{
						if(strcmp(pntname[slot][pntnum],"NULL")==0)
						{
							sendbuf[bufpoint]=0x40;
							bufpoint++;
							sendbuf[bufpoint]=0;
							bufpoint++;
							continue;
						}
						strcpy(name_buf,pntname[slot][pntnum]);
						if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
						{
							printf("%s No this point in the database file!\n",name_buf);
							sendbuf[bufpoint]=0x40;
							bufpoint++;
							sendbuf[bufpoint]=0;
							bufpoint++;
							continue;
						}
						value=(float)(dbentry.var.anlg.value.data/1000.0);
						if(value <= lorange[slot][pntnum])
							anaorgcode = 0x4000;
						else	if(value >= hirange[slot][pntnum])
							anaorgcode = 0x4FFF;
						else
						{
							anaorgcode=(unsigned short)((value-lorange[slot][pntnum])*RESOLUTE/(hirange[slot][pntnum]-lorange[slot][pntnum])+0.5);
							anaorgcode=(anaorgcode&0x0FFF)|0x4000;
						}
						sendbuf[bufpoint]=(unsigned char)(anaorgcode>>8);
						bufpoint++;
						sendbuf[bufpoint]=(unsigned char)anaorgcode;
						bufpoint++;
					}
					break;
		
			case DI:
#ifdef DEBUG
					printf("Slot %2d Digital Input Board\n",slot);
#endif
					bytecount = bytecount + 4;
					dicode = 0x80;
					bitmap = 0x01;
					for(pntnum=0;pntnum<24;pntnum++)
					{
						if(strcmp(pntname[slot][pntnum],"NULL")==0)
						{
							dicode = dicode & (~bitmap);
						}
						else
						{
							strcpy(name_buf,pntname[slot][pntnum]);
							if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
							{
								printf("%s No this point in the database file!\n",name_buf);
								dicode = dicode & (~bitmap);
							}
							else
							{
								/*stat=(dbentry.entry.ind.variable.value.status&0x3c)>>2;*/
								stat=dbentry.var.ind.status.rt_state;
								if(stat != 1)
									stat = 0;	/* OFF:QDS IV NT SB BL RES3 SPI */

								if(dbentry.var.ind.status.man_set)
									stat = stat | 0x20;			/* SB=1 */
								if(dbentry.var.ind.status.chan_fail)
									stat = stat | 0xC0;			/* IV=1; NT=1 */
								if(dbentry.fixed.iost.scanEnable==0)
									stat = stat | 0x10;			/* BL=1 */
								if(stat==1)
									dicode = dicode | bitmap;
								else
									dicode = dicode & (~bitmap);
							}
						}
						bitmap = bitmap<<1;
						if(bitmap==0x40)
						{
							sendbuf[bufpoint]=dicode;
							bufpoint++;
							dicode = 0x80;
							bitmap = 0x01;
						}
					}
					break;

			default :
#ifdef DEBUG
				printf("sc1801 RTU Slot %2d = %x Not AI/DI :\n",slot,rtuconfig[slot]);
#endif
				break;			

		}/*switch(rtuconfig[slot])*/
	}/*for*/
	sendbuf[2] = (unsigned char)(bytecount >> 8);	/*High length*/
	sendbuf[3] = (unsigned char)bytecount;	/*Low length*/

	if(soebuf.sendpnt == soebuf.bufpnt)
		rtu_status = rtu_status & 0xFB;
	else
		rtu_status = rtu_status | 0x04;
	sendbuf[4] = rtu_status;

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

	sprintf(infobuf,"\nReportFullData: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	ReportExceptData(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{
	unsigned char	chksum,slot,pntnum,dicode,bitmap,group,maskmap;
	unsigned short	n,anaorgcode,anasav,bufpoint,orgbufpnt,stat;
	DMS_COMMON		dbentry;
/*	int				data_type;*/
	char			name_buf[NAME_SIZE],soechanged;
	float			value;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = XRF | 0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
/*	sendbuf[4] = rtu_status;*/
	bufpoint = 5;
	orgbufpnt = 0;
	bytecount = 1;
	soechanged = 0;

	for(slot=0;slot<16;slot++)
	{
		switch(rtuconfig[slot])
		{
			case AI:
#ifdef DEBUG
					printf("Slot %2d Analog Input Board\n",slot);
#endif
					for(pntnum=0;pntnum<12;pntnum++)
					{
						if(strcmp(pntname[slot][pntnum],"NULL")==0)
						{
							orgbufpnt = orgbufpnt + 2;
							continue;
						}
						strcpy(name_buf,pntname[slot][pntnum]);
						if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
						{
							printf("%s No this point in the database file!\n",name_buf);
							orgbufpnt = orgbufpnt + 2;
							continue;
						}
						value=(float)(dbentry.var.anlg.value.data/1000.0);
						if(value <= lorange[slot][pntnum])
							anaorgcode = 0;
						else	if(value >= hirange[slot][pntnum])
							anaorgcode = 0x0FFF;
						else
						{
							anaorgcode=(unsigned short)((value-lorange[slot][pntnum])*RESOLUTE/(hirange[slot][pntnum]-lorange[slot][pntnum])+0.5);
							anaorgcode=anaorgcode&0x0FFF;
						}
						anasav = orgbuf[orgbufpnt] << 8;
						orgbufpnt ++;
						anasav = (anasav | orgbuf[orgbufpnt])& 0x0FFF;
						if(abs(anaorgcode - anasav) > databand)
						{
#ifdef DEBUG
							printf("%d %d real %x save %x diff %d databand %d\n",slot,pntnum,
								anaorgcode,anasav,abs(anaorgcode - anasav),databand);
#endif
							sendbuf[bufpoint]=slot<<4 | pntnum;
							bufpoint++;
							anaorgcode=anaorgcode|0x4000;
							orgbuf[orgbufpnt-1]=sendbuf[bufpoint]=(unsigned char)(anaorgcode>>8);
							bufpoint++;
							orgbuf[orgbufpnt]=sendbuf[bufpoint]=(unsigned char)anaorgcode;
							bufpoint++;
							bytecount = bytecount + 3;
						}
						orgbufpnt ++;
					}
					break;
		
			case DI:
#ifdef DEBUG
					printf("Slot %2d Digital Input Board\n",slot);
#endif
					dicode = 0x80;
					group = 0;
					bitmap = 0x01;
					for(pntnum=0;pntnum<24;pntnum++)
					{
						if(strcmp(pntname[slot][pntnum],"NULL")==0)
						{
							dicode = dicode & (~bitmap);
						}
						else
						{
							strcpy(name_buf,pntname[slot][pntnum]);
							if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
							{
								printf("%s No this point in the database file!\n",name_buf);
								dicode = dicode & (~bitmap);
							}
							else
							{
							/*	stat=(dbentry.entry.ind.variable.value.status&0x3c)>>2;*/
								stat=dbentry.var.ind.status.rt_state;
								if(stat != 1)
									stat = 0;	/* OFF:QDS IV NT SB BL RES3 SPI */

								if(dbentry.var.ind.status.man_set)
									stat = stat | 0x20;			/* SB=1 */
								if(dbentry.var.ind.status.chan_fail)
									stat = stat | 0xC0;			/* IV=1; NT=1 */
								if(dbentry.fixed.iost.scanEnable==0)
									stat = stat | 0x10;			/* BL=1 */
								if(stat==1)
									dicode = dicode | bitmap;
								else
									dicode = dicode & (~bitmap);
							}
						}
						bitmap = bitmap<<1;
						if(bitmap==0x40)
						{
							maskmap = orgbuf[orgbufpnt] ^ dicode;
							if(maskmap)
							{
#ifdef DEBUG
								printf("%d %d real %x save %x\n",slot,group,
									dicode,orgbuf[orgbufpnt]);
#endif
								sendbuf[bufpoint]=(group << 4) | slot;
								bufpoint ++;
								orgbuf[orgbufpnt]=sendbuf[bufpoint]=dicode;
								bufpoint ++;
								sendbuf[bufpoint]=maskmap;
								bufpoint ++;
								bytecount = bytecount + 3;
								if(soeflag[slot]==1)
									soechanged = 1;
							}
							dicode = 0x80;
							bitmap = 0x01;
							orgbufpnt ++;
							group ++;
							group = group &0x3;
						}
					}
					break;

			default :
#ifdef DEBUG
				printf("sc1801 RTU Slot %2d = %x Not AI/DI :\n",slot,rtuconfig[slot]);
#endif
				break;			

		}/*switch(rtuconfig[slot])*/
	}/*for*/
	sendbuf[2] = (unsigned char)(bytecount >> 8);	/*High*/
	sendbuf[3] = (unsigned char)bytecount;	/*Low*/
	if(soebuf.sendpnt == soebuf.bufpnt && !soechanged)
		rtu_status = rtu_status & 0xFB;
	else
		rtu_status = rtu_status | 0x04;
	sendbuf[4] = rtu_status;

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

	sprintf(infobuf,"\nReportExceptData: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	SequenceOfEvents(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{

	unsigned char	chksum;	/*Modify "unsigned char n" to unsigned short 2005.1.15 zy*/
	unsigned short	bufpoint,soepnt,n;
	time_t		tTim;
	struct tm	sTimeStart;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = SOE | 0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 1;
	bufpoint = 5;
	soepnt = soebuf.sendpnt;

	if(soepnt != soebuf.bufpnt)
	while(soepnt != soebuf.bufpnt)
	{
		sTimeStart.tm_sec = soebuf.soerec[soepnt].time.second;
		sTimeStart.tm_min = soebuf.soerec[soepnt].time.minute;
		sTimeStart.tm_hour = soebuf.soerec[soepnt].time.hour;
		sTimeStart.tm_mday = soebuf.soerec[soepnt].time.day;
		sTimeStart.tm_mon = soebuf.soerec[soepnt].time.month-1;
		sTimeStart.tm_year =  soebuf.soerec[soepnt].time.year;
		sTimeStart.tm_isdst = 0;
		if((tTim = mktime(&sTimeStart)) == (time_t)-1 )
		{
			printf("\nSOE mktime error: %d/%02d/%02d %02d:%02d:%02d\n",sTimeStart.tm_year+1900,
				sTimeStart.tm_mon,sTimeStart.tm_mday,sTimeStart.tm_hour,sTimeStart.tm_min,sTimeStart.tm_sec);
			tTim=time(&tTim);
		}

		tTim = tTim-DRFT_SEC;	/*1970.1.1 -- 2000.1.1 GMT*/
		sendbuf[bufpoint] = (unsigned char)(tTim>>24);	/*High tim 24-31*/
		sendbuf[bufpoint+1] = (unsigned char)(tTim>>16);	/*High tim 16-23*/
		sendbuf[bufpoint+2] = (unsigned char)(tTim>>8);	/*low tim 8-15*/
		sendbuf[bufpoint+3] = (unsigned char)tTim;		/*Low tim 0-8*/
		sendbuf[bufpoint+4] = (unsigned char)(soebuf.soerec[soepnt].time.ms>>8);	/*/1/10 ms*/
		sendbuf[bufpoint+5] = (unsigned char)soebuf.soerec[soepnt].time.ms;		/*//1/10 ms*/
		sendbuf[bufpoint+6] = soebuf.soerec[soepnt].slot_grp;
		sendbuf[bufpoint+7] = soebuf.soerec[soepnt].mask;
		sendbuf[bufpoint+8] = soebuf.soerec[soepnt].stat | 0x80;

		bufpoint = bufpoint+9;
		bytecount = bytecount + 9;
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;

		if(bytecount > 900)
			break;
	}
	soebuf.sendpnt = soepnt;

	sendbuf[2] = (unsigned char)(bytecount>>8);	/*High length*/
	sendbuf[3] = (unsigned char)bytecount;		/*Low length*/

	if(soebuf.sendpnt == soebuf.bufpnt)
		rtu_status = rtu_status & 0xFB;
	else
		rtu_status = rtu_status | 0x04;

	sendbuf[4] = rtu_status ;

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

	sprintf(infobuf,"\nSequenceOfEvents: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	AckCommand(hPort_com,cmd,sendbuf)
int	hPort_com;
unsigned char cmd;
unsigned char	sendbuf[];
{
	unsigned char	chksum;
	unsigned short	n;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = cmd |0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 1;
/**	bufpoint = 5;**/
	
	sendbuf[2] = 0;	/*High*/
	sendbuf[3] = (unsigned char)bytecount;	/*Low*/
	sendbuf[4] = rtu_status;

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

	sprintf(infobuf,"\nAckCommand: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	PulseAccumulatorReport(hPort_com,ppnum,sendbuf)
int	hPort_com;
unsigned short	ppnum;
unsigned char	sendbuf[];
{
	unsigned char	chksum;
	unsigned short	bufpoint,n;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = PAR |0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	sendbuf[4] = rtu_status;

	bufpoint = 5;

	for(n=0;n<ppnum;n++)
	{
/*		memcpy(&sendbuf[bufpoint],&pulsedata[n],2); //Note the H/L sequence*/
		sendbuf[bufpoint] = (unsigned char)(pulsedata[n] >>8);	/*High*/
		bufpoint ++;
		sendbuf[bufpoint] = (unsigned char)pulsedata[n];	/*Low*/
		bufpoint ++;
	}

	bytecount = ppnum*2 + 1;
	sendbuf[2] = (unsigned char)(bytecount>>8);	/*High length*/
	sendbuf[3] = (unsigned char)bytecount;	/*Low length*/
		
	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 /*checksum char*/

	sprintf(infobuf,"\nPulseAccumulatorReport: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	ReportDataBand(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{
	unsigned char	chksum;
	unsigned short	bufpoint,n;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = RDB |0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 3;
	sendbuf[2] = 0;	/*High*/
	sendbuf[3] = (unsigned char)bytecount;	/*Low*/
	sendbuf[4] = rtu_status;

	bufpoint = 5;

/*	memcpy(&sendbuf[bufpoint],&databand,2);*/
	sendbuf[bufpoint] = (unsigned char)(databand>>8);	/*High databand*/
	sendbuf[bufpoint+1] = (unsigned char)databand;	/*Low databand*/
	
	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 /*checksum char*/

	sprintf(infobuf,"\nReportDataBand: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	AckCtrlOutputARM(hPort_com,timeout,slot_pnt,sendbuf)
int	hPort_com;
unsigned char timeout,slot_pnt;
unsigned char	sendbuf[];
{
	unsigned char	chksum;
	unsigned short	bufpoint,n;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = COA | 0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 3;
	sendbuf[2] = 0;
	sendbuf[3] = (unsigned char)bytecount;			/*L*/
	sendbuf[4] = rtu_status;

	bufpoint = 5;

	sendbuf[5] = ~timeout;
	sendbuf[6] = ~slot_pnt;

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 /*checksum char*/

	sprintf(infobuf,"\nAckCtrlOutputARM: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	AckDataReqLatch(hPort_com,slot,dvalue,sendbuf)
int	hPort_com;
unsigned char slot;
size_t dvalue;
unsigned char	sendbuf[];
{
	unsigned char	chksum,n;

	unsigned char	dataty,pntnum,name_buf[NAME_SIZE];
	unsigned int	bitfg;
	size_t dlvalue;
	DMS_COMMON		dbentry;
	/*IND_REC		*pInd_alm;***080226*/
/*	unsigned int	data_type;*/

	sendbuf[0] = STATION_ID;
	sendbuf[1] = DRL | 0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 5;
	sendbuf[2] = 0;
	sendbuf[3] = (unsigned char)bytecount;	   
	sendbuf[4] = rtu_status;;		/*RTU status*/

	dlvalue = 0;
	bitfg = 0x1;
	for(pntnum = 0; pntnum < 32; pntnum ++)
	{
		if(strcmp(pntname[slot][pntnum],"NULL")==0)
		{
			bitfg = bitfg << 1;
			continue;
		}
		strcpy(name_buf,pntname[slot][pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this DO point in the database file!\n",name_buf);
			bitfg = bitfg << 1;
			continue;
		}
		dataty=dbentry.point.data_type;
		if((dataty != SOE_TYPE) && (dataty != POL_TYPE)
			&& (dataty != SYSMSG_TYPE))

		{
			printf("%s this point is not INT/POL/SYSMSG in the database file!\n",name_buf);
			bitfg = bitfg << 1;
			continue;
		}
		/*if((dbentry.entry.ind.variable.value.status&0x0c)==4)*/
		if((dbentry.var.ind.status.rt_state)==4)  /****zwj why 4?****/
		{
			dlvalue = dlvalue | bitfg;
		}
		bitfg = bitfg << 1;
	}
/*	memcpy(&sendbuf[5],&dlvalue,4);	//32bits digital output****zwj*/
	sendbuf[5] = (unsigned char)(dvalue>>24);	/*High dvalue 24-31*/
	sendbuf[6] = (unsigned char)(dvalue>>16);	/*High dvalue 16-23*/
	sendbuf[7] = (unsigned char)(dvalue>>8);	/*low dvalue 8-15*/
	sendbuf[8] = (unsigned char)dvalue;			/*Low dvalue 0-8*/

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 /*checksum char*/

	sprintf(infobuf,"\nAckDataReqLatch: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	AckSynchronizeSystemTime(hPort_com,recvtime,sendbuf)
int	hPort_com;
unsigned char	*recvtime[];
unsigned char	sendbuf[];
{
	
	time_t	tim;
#ifndef SUN_OS
	SYSTEMTIME sys_tm;/**zwj????***/
#else
	struct timeval	tout;
#endif
	USHORT	sst_ms;
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = SST | 0x40;
/*
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 7;
	sendbuf[2] = 0;
	sendbuf[3] = (unsigned char)bytecount;	/*L*/

	sendbuf[4] = rtu_status;;		/*RTU status*/

/***	memcpy(&sendbuf[5],recvtime,6);	//48 bits time**/
	tim=time(&tim);

	tim = tim - DRFT_SEC;	//1970.1.1 -- 2000.1.1 GMT
/***	memcpy(&sendbuf[5],&tim,4);	//seconds since 2000.1.1***/
	sendbuf[5] = (unsigned char)(tim>>24);	/*High tim 24-31*/
	sendbuf[6] = (unsigned char)(tim>>16);	/*High tim 16-23*/
	sendbuf[7] = (unsigned char)(tim>>8);	/*low tim 8-15*/
	sendbuf[8] = (unsigned char)tim;		/*Low tim 0-8*/

#ifdef SUN_OS
	gettimeofday(&tout,NULL);
	sst_ms=tout.tv_usec/100;   /**0.1 ms***zwj*/
#else
	GetLocalTime(&sys_tm);
	sst_ms = sys_tm.wMilliseconds*10;/**0.1 ms***zwj*/
#endif

	sendbuf[9] = (unsigned char)(sst_ms>>8);	/**1/10 ms**/
	sendbuf[10] = (unsigned char)sst_ms;		/**1/10 ms**/

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;


	sprintf(infobuf,"\nAckSynchronizeSystemTime: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	AckReportSysTime(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{
	time_t	tim;
#ifndef SUN_OS
	SYSTEMTIME sys_tm;/**zwj????***/
#else
	struct timeval	tout;
#endif
	USHORT	sst_ms=0;
/**	struct tm	*tmpt;**/
	unsigned char	chksum,n;
/**	DWORD	ttsec;**/
/**	WORD	tms;**/

	sendbuf[0] = STATION_ID;
	sendbuf[1] = RST | 0x40;
/*
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 7;
	sendbuf[2] = 0;
	sendbuf[3] = (unsigned char)bytecount;						

	sendbuf[4] = rtu_status;;		/**RTU status**/

/**	GetLocalTime(&sys_tm);**/

	tim=time(&tim);
/**	tmpt=localtime(&tim);**/

/**	tms = sys_tm.wMilliseconds * 10;**/
/**	ttsec = (sys_tm.wYear - 100) * 365;**/

	tim = tim - DRFT_SEC;	/**1970.1.1 -- 2000.1.1 GMT**/
/**	memcpy(&sendbuf[5],&tim,4);	//seconds since 2000.1.1**/
	sendbuf[5] = (unsigned char)(tim>>24);	/*High tim 24-31*/
	sendbuf[6] = (unsigned char)(tim>>16);	/*High tim 16-23*/
	sendbuf[7] = (unsigned char)(tim>>8);	/*low tim 8-15*/
	sendbuf[8] = (unsigned char)tim;		/*Low tim 0-8*/

#ifdef SUN_OS
	gettimeofday(&tout,NULL);
	sst_ms=tout.tv_usec/100;   /**0.1 ms***zwj*/
#else
	GetLocalTime(&sys_tm);
	sst_ms = sys_tm.wMilliseconds*10;/**0.1 ms***zwj*/
#endif
	sendbuf[9] = (unsigned char)(sst_ms>>8);	/***1/10 ms***/
	sendbuf[10] = (unsigned char)sst_ms;		/**1/10 ms**/

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 /**checksum char**/


	sprintf(infobuf,"\nAckReportSysTime: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

BOOL	AckReportInterfaceMode(hPort_com,sendbuf)
int	hPort_com;
unsigned char	sendbuf[];
{

	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	sendbuf[1] = RIM | 0x40;
/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
*/
	bytecount = 2;
	sendbuf[2] = 0;
	sendbuf[3] = (unsigned char)bytecount;	/*Low*/

	sendbuf[4] = rtu_status;;		/*RTU status*/

	sendbuf[5] = 0x0F;	/*Interface Mode of P3--P0*/

	byte_to_send = bytecount + 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 /*checksum char*/

	sprintf(infobuf,"\nAckReportInterfaceMode: %u byte to send",byte_to_send);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	retn = SendData();
	return(retn);
}

void	RTURecvFun(port,hPort_com)
short	port;
int	hPort_com;
{
	BOOL	fSuccess;					  	
	/*unsigned char recvbuf[1024];
	unsigned char sendbuf[1024];*/
	unsigned char comd_id;
	/*char	zwjbuf[5];*/
	
	ANA_FMT		ana_buf;
	IND_FMT		ind_buf;
/**	LANH_PTR	lanhd;**/

	time_t	tim;
/**	SYSTEMTIME sys_tm;**/
	struct tm *timeptr,tmpt;
	FILE	*fagc;
	fd_set	fset;
	int	dwByteRecvd,nOffset;
	int	ret,j;
	struct timeval	tout;/**zwj**/
	char		cmdtype;

	unsigned char coa_time,coa_slotpnt;
	unsigned short sst_ms;
	size_t	cod_time;
	BOOL	coa_flag;
	BOOL	ClockSyncflag;
	unsigned int	repeatnum,timeoutnum;
	unsigned int	i,pnum;
	unsigned char	chksum,slot,pntnum,name_buf[NAME_SIZE];
	unsigned short	datacode,datacount;
	float	value;
	DMS_COMMON		dbentry;
/*	unsigned int	data_type;**080226**/

	size_t	byte_recvd,dvlatch[16];
	unsigned int	bitfg;

/**	MountDB();**/
/**	Lan_init();**/

	repeatnum=timeoutnum=port-1;

/*	if(PurgeComm(hPort_com,PURGE_TXABORT|PURGE_RXABORT|
					PURGE_RXCLEAR|PURGE_TXCLEAR)==FALSE)
		printf("INIT: PurgeComm ERRNO= %d \n",GetLastError());*/

	tim=time(&tim);
	timeptr=localtime(&tim);
	daysaved = timeptr->tm_mday;

	for(;;)
    {
#ifdef SUN_OS
		tim=time(&tim);
	
		memset(recvbuf, 0, VARFRAMELEN);
		nOffset = 0;

		FD_ZERO(&fset);
		FD_SET(hPort_com,&fset);
		tout.tv_sec = 2;
		tout.tv_usec = 0;

		ret = select(hPort_com+1, &fset, NULL, NULL, &tout);
		printf("====ret=%d=====",ret);

		if(ret <= 0)
#else
		fSuccess = ReadFile(hPort_com,(LPSTR)recvbuf,VARFRAMELEN,&byte_recvd,NULL);
		sleep(2);
		printf("====fSuccess=%d=====\n",fSuccess);
		printf("====byte_recvd=%d=====\n",byte_recvd);
		if(!fSuccess)
		{
		printf("port%d ReceiveData: ReadFile ERRNO= %d \n",port,GetLastError());
		ClearRecvFault(hPort_com);
		continue;
		}
		if(byte_recvd == 0)
#endif
		
		
		/*dwByteRecvd = read(hPort_com, &recvbuf, 10);*/
		
		
		/*if( dwByteRecvd<= 0)*/
		{
			timeoutnum++;

			/*sprintf(infobuf,"RTU_fun.c: %s select() TIMEOUT No: %d! ret=%d\n",sProcRec[pnum].NameID,timeoutnum,ret);*/
			sprintf(infobuf,"RTU_fun.c: TIMEOUT No: %d! \n",timeoutnum);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);

			if(timeoutnum%20==0)
			{
				sprintf(infobuf,"RTU_fun.c: ReceiveData: timeout! \n");
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				UpdateChannelFlag(pnum,(unsigned short)1);

			}
			//continue;///zwj for win
			
		}
		else
		{
#ifdef SUN_OS
			dwByteRecvd = read(hPort_com, &recvbuf[nOffset], 2*VARFRAMELEN);
			printf("=====dwByteRecvd=%d=====",dwByteRecvd);
			for(j=0;j<dwByteRecvd;j++)
			printf("=====recvbuf[%d]=%02x=====\n",j,recvbuf[j]);
			dwByteRecvd += nOffset;
			if (dwByteRecvd  <= 0)
			{
				/*sprintf(infobuf,"+++RTU_fun.c: Controlled Station %s read error: %s\n",sProcRec[pnum].NameID,strerror(errno));*/
				sprintf(infobuf,"+++RTU_fun.c: Controlled Station %s read error+++\n",sProcRec[pnum].NameID);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
				continue;
			}
		
			timeoutnum =0;
			sprintf(infobuf,"\nRTU_fun.c: Controlled Stationreceived data byte nNumber %u :\n",dwByteRecvd);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);

		UpdateChannelFlag(pnum,(unsigned short)0);
		byte_recvd=dwByteRecvd;
#endif

		datacount = recvbuf[2] << 8;
		datacount = datacount + recvbuf[3];

/**		Sleep(500);**/
		
		if((recvbuf[0]!=STATION_ID)||((unsigned short)byte_recvd!= datacount+5))
			{
			printf("\nport%d SC1801 RTU received Frame ID %d or Count %d Error!\n",port,recvbuf[0],datacount);
			repeatnum++;
/**			ResponseNAK(hPort_com,recvbuf[2] & 0x3F,0xFC,sendbuf);	//comd_id,errcode**/
			continue;
			}
			
		chksum = 0xff;
		for(i=0;i<byte_recvd-1;i++)
			chksum = chksum ^ recvbuf[i];	

		if(chksum != recvbuf[byte_recvd-1])
			{
			sprintf(infobuf,"\nport%d SC1801 RTU received Frame XOR Check Error!\n",port);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);

			repeatnum++;
			ResponseNAK(hPort_com,recvbuf[2]&0x3F,0xFC,sendbuf);	/**comd_id,errcode**/
			continue;
			}
	

		comd_id = recvbuf[1] & 0x3F;

		if(recvbuf[1] & 0x80)
		{
			if(comd_id == (sendbuf[1] & 0x3F))
			{
				RepeatTransmit(hPort_com,sendbuf);
				continue;
			}
		}


		switch (comd_id)
			{
			case NAK:
						printf("port%d NOT ACKNOWLEDGED frame Command= %02x Err code= %02x\n",
								port,recvbuf[4],recvbuf[5]);
						repeatnum++;
						timeoutnum =0;
						break;
			case RRC:
#ifdef DEBUG
						printf("port%d Report RTU Configuration Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						ReportRTUConfig(hPort_com,sendbuf);
						break;

			case DRF:
#ifdef DEBUG
						printf("port%d DATA Request Full Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						ReportFullData(hPort_com,sendbuf);
						break;

			case XRF:
#ifdef DEBUG
						printf("port%d Exception Request Full Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						ReportExceptData(hPort_com,sendbuf);
						break;
		
			case SOE:
#ifdef DEBUG
						printf("port%d Sequence of Events Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						rtu_status = rtu_status & 0xE3;
						SequenceOfEvents(hPort_com,sendbuf);
						break;
		
			case PAR:
#ifdef DEBUG
						printf("port%d Pulse Accumulator Report Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						PulseAccumulatorReport(hPort_com,2,sendbuf);	/**2 Pulse for test**/
						break;
		
			case PAZ:
#ifdef DEBUG
						printf("port%d Pulse Accumulator ZERO Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						rtu_status = rtu_status | 0x20;
						AckCommand(hPort_com,PAZ,sendbuf);
						break;
		
			case PAF:
#ifdef DEBUG
						printf("port%d Pulse Accumulator Freeze Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						rtu_status = rtu_status | 0x20;
						AckCommand(hPort_com,PAF,sendbuf);
						break;
		
			case PAT:
#ifdef DEBUG
						printf("port%d Pulse Accumulator Thaw Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						rtu_status = rtu_status & 0xDF;
						AckCommand(hPort_com,PAT,sendbuf);
						break;
		
			case SDB:
#ifdef DEBUG
						printf("port%d Set databand Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						rtu_status = rtu_status & 0xFE;
/**						memcpy(&databand,&recvbuf[4],2);**/
						databand = recvbuf[4] << 8;
						databand = databand + recvbuf[5];
						AckCommand(hPort_com,SDB,sendbuf);
						break;
		
			case RDB:
#ifdef DEBUG
						printf("port%d Read databand Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						ReportDataBand(hPort_com,sendbuf);
						break;
		
			case COA:
#ifdef DEBUG
						printf("port%d Control Output ARM Command Received!\n",port);
#endif
						coa_time = recvbuf[4];
						coa_slotpnt = recvbuf[5];
						coa_flag = 1;   /**Control Output ARM**/
						slot = (coa_slotpnt>>4)&0x0F;
						if(rtuconfig[slot] != TRD)
						{
							printf("port%d Control Output ARM Command slot %d not TRD module\n",port,slot);
							ResponseNAK(hPort_com,COA,0x01,sendbuf);		/*cmd,err_code*/
							continue;
						}

						timeoutnum =0;
						repeatnum = 0;
						AckCtrlOutputARM(hPort_com,coa_time,coa_slotpnt,sendbuf);
						break;
		
			case COD:
						if((recvbuf[5] & 0xC0) == 0x40)
						{					/*AO*/
#ifdef DEBUG
							printf("port%d AO Control Output Direct Command Received!\n",port);
#endif
							slot = (recvbuf[4] >> 4) & 0x0F;
							if(rtuconfig[slot] != AO)
							{
								printf("port%d Control Output ARM Command slot %d not AO module\n",port,slot);
								ResponseNAK(hPort_com,COD,0x01,sendbuf);		/*cmd,err_code*/
								continue;
							}
							pntnum = recvbuf[4] & 0x0F;
							datacode = recvbuf[5] << 8;
							datacode = (datacode + recvbuf[6]) & 0x0FFF;
							value=(datacode*(hirange[slot][pntnum]-lorange[slot][pntnum])/RESOLUTE) + lorange[slot][pntnum];
							strcpy(name_buf,pntname[slot][pntnum]);
							/*ReadEntryByName(name_buf,&data_type,&dbentry);
							if(dbentry.err_code==-1)*/
							if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
							{
								printf("%s No this AO point in the database file!\n",name_buf);
								ResponseNAK(hPort_com,COD,0x01,sendbuf);		/*cmd,err_code*/
								continue;
							}
							
							/*ana_buf.ptr = dbentry.entry.anlg.variable.ptr;
							if(ana_buf.ptr.data_type != DPS_ANALOG)*/
							ana_buf.ptr=dbentry.point;
							if(ana_buf.ptr.data_type != ANA_TYPE)
							{
								printf("%s this point is not ANA in the database file!\n",name_buf);
								ResponseNAK(hPort_com,COD,0x01,sendbuf);		/*cmd,err_code*/
								continue;
							}
						/**	ana_buf.ptr.data_type=ANA_SET;
							ana_buf.value = FloatToAnaData(value);**ZWJ**/
			cmdtype = C_OP_VALUE_SET;
			SEComdSendToLCU(cmdtype, value, &dbentry);


#ifdef REC_YF_AGC_CMD
			tmpt=*localtime(&tim);
			sprintf(cmdrecfile,"%s%02d.REC",cmdrecbuf,tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
				fagc = fopen(cmdrecfile,"w");
				daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(cmdrecfile,"a");
			if (fagc==NULL)
			{
				sprintf(infobuf,"iec101s:can't open(wb) the file %s!\n",cmdrecfile);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f \n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value);
				fclose(fagc);
			}
#endif
						}
						else if((recvbuf[5] & 0xC0) == 0xC0)
						{					/*CCO*/
#ifdef DEBUG
							printf("port%d CCO Control Output Direct Command Received!\n",port);
#endif
							slot = (recvbuf[4] >> 4) & 0x0F;
							if(rtuconfig[slot] != CCO)
							{
								printf("Control Output ARM Command %d slot %x not CCO module\n",slot,rtuconfig[slot]);
								ResponseNAK(hPort_com,COD,0x01,sendbuf);		/*cmd,err_code*/
								continue;
							}
							pntnum = recvbuf[4] & 0x07;
							datacode = recvbuf[5] << 8;
							datacode = (datacode + recvbuf[6]) & 0x3FFF;
							if(recvbuf[6] == 0)
								cod_time = datacode * 5; /*time resoulation 5*/
							else
								cod_time = datacode;
							strcpy(name_buf,pntname[slot][pntnum]);
							/*ReadEntryByName(name_buf,&data_type,&dbentry);
							if(dbentry.err_code==-1)*/
							if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
							{
								printf("%s No this DO point in the database file!\n",name_buf);
								ResponseNAK(hPort_com,COD,0x01,sendbuf);		/*cmd,err_code*/
								continue;
							}
							/*ind_buf.ptr=dbentry.entry.ind.variable.ptr;*/
							ind_buf.ptr=dbentry.point;
							if((ind_buf.ptr.data_type != SOE_TYPE) && (ind_buf.ptr.data_type != POL_TYPE)
								&& (ind_buf.ptr.data_type != SYSMSG_TYPE))
							{
								printf("%s this point is not INT/POL/SYSMSG in the database file!\n",name_buf);
								ResponseNAK(hPort_com,COD,0x01,sendbuf);		/*cmd,err_code*/
								continue;
							}

							ind_buf.status=4;
							cmdtype = C_SYS_MODE_SET;
							ComdSendToLCU(cmdtype,ind_buf.status,&dbentry);
							AckCommand(hPort_com,COD,sendbuf);
						}
						timeoutnum =0;
						repeatnum = 0;
						break;
		
			case COE:
#ifdef DEBUG
						printf("port%d Control Output Execute Command Received!\n",port);
#endif
						if(coa_slotpnt != recvbuf[4])
						{
							printf("slot pont %x is diff from COA slot pnt%x!\n",recvbuf[4],coa_slotpnt);
							ResponseNAK(hPort_com,COE,0x01,sendbuf);		/*cmd,err_code*/
							continue;
						}
						slot = (recvbuf[4] >> 4) & 0x0F;
					/*	pntnum = recvbuf[4] & 0x07;*/
						pntnum = recvbuf[4] & 0x0F;

						strcpy(name_buf,pntname[slot][pntnum]);
						/*ReadEntryByName(name_buf,&data_type,&dbentry);
						if(dbentry.err_code==-1)*/
						if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
						{
							printf("%s No this DO point in the database file!\n",name_buf);
							ResponseNAK(hPort_com,COE,0x01,sendbuf);		/*cmd,err_code*/
							continue;
						}
						/*ind_buf.ptr=dbentry.entry.ind.variable.ptr;
						if((ind_buf.ptr.data_type != DPS_INT_IND) && (ind_buf.ptr.data_type != DPS_POL_IND)
							&& (ind_buf.ptr.data_type != DPS_SYSMSG))*/
						ind_buf.ptr=dbentry.point;
						if((ind_buf.ptr.data_type != SOE_TYPE) && (ind_buf.ptr.data_type != POL_TYPE)
								&& (ind_buf.ptr.data_type != SYSMSG_TYPE))
						{
							printf("%s this point is not INT/POL/SYSMSG in the database file!\n",name_buf);
							ResponseNAK(hPort_com,COE,0x01,sendbuf);		/*cmd,err_code*/
							continue;
						}

						ind_buf.status=4;
						cmdtype = C_SYS_MODE_SET;
						ComdSendToLCU(cmdtype,ind_buf.status,&dbentry);
/*						tim=time(&tim);
						timeptr=localtime(&tim);
						ind_buf.minute  =timeptr->tm_min;
						ind_buf.second  =timeptr->tm_sec;
						ind_buf.ms = 0;

						lanhd.dest_id=0;
						lanhd.length=sizeof(ind_buf);
						lanhd.src_id=GetHost_Cmd();
						lanhd.dp_type=BJ_DB;
#ifdef DEBUG
						printf("TRD COE %d=%d=%d %d:%d:%d\n",ind_buf.ptr.dev_id,ind_buf.ptr.data_type,
							ind_buf.ptr.point_id,ind_buf.status,ind_buf.minute,ind_buf.second);
#endif
						lan_out((char *)&ind_buf,lanhd);*/
						AckCommand(hPort_com,COE,sendbuf);
					
						timeoutnum =0;
						repeatnum = 0;
						break;

			case COL:
#ifdef DEBUG
						printf("port%d Control Output Latched Command Received!\n",port);
#endif
						slot = (recvbuf[4] >> 4) & 0x0F;
						if(rtuconfig[slot] != DO)
						{
							printf("Control Output Latch Command %d slot %x not DO module\n",slot,rtuconfig[slot]);
							ResponseNAK(hPort_com,COL,0x01,sendbuf);		/*cmd,err_code*/
							continue;
						}
/*						memcpy(&dvlatch[slot],&recvbuf[5],4);	//32bits digital output*/
#ifdef SUN_OS
						CrossCopy(&dvlatch[slot],&recvbuf[5],4);	/* address of object */
#else
						memcpy(&dvlatch[slot],&recvbuf[5],4);	 /*address of object */
#endif
/*						dvlatch = sendbuf[5] + recvbuf[6] << 8;
						dvlatch = dvlatch + recvbuf[7] << 16;
						dvlatch = dvlatch + recvbuf[8] << 24;
*/
#ifdef DEBUG
						printf("dvlatch = %04x\n",dvlatch[slot]);
#endif
						bitfg = 0x1;
						for(pntnum = 0; pntnum < 32; pntnum ++)
						{
							if(strcmp(pntname[slot][pntnum],"NULL")==0)
							{
								bitfg = bitfg << 1;
								continue;
							}
							strcpy(name_buf,pntname[slot][pntnum]);
							if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
							{
								printf("%s No this DO point in the database file!\n",name_buf);
/***								ResponseNAK(hPort_com,COL,0x01,sendbuf);		//cmd,err_code***/
								bitfg = bitfg << 1;
								continue;
							}
							
						ind_buf.ptr=dbentry.point;
						if((ind_buf.ptr.data_type != SOE_TYPE) && (ind_buf.ptr.data_type != POL_TYPE)
								&& (ind_buf.ptr.data_type != SYSMSG_TYPE))
							{
								printf("%s this point is not INT/POL/SYSMSG in the database file!\n",name_buf);
/****								ResponseNAK(hPort_com,COL,0x01,sendbuf);		//cmd,err_code****/
								bitfg = bitfg << 1;
								continue;
							}
							if(dvlatch[slot] & bitfg )
							{
								/*if((dbentry.entry.ind.variable.value.status&0x0c)==4)*/
								if(dbentry.var.ind.status.rt_state==4)
								{
									bitfg = bitfg << 1;
									continue;
								}
								else
								{
									ind_buf.status=4;
								}
							}
							else
							{
								if(dbentry.var.ind.status.rt_state==0)
								{
									bitfg = bitfg << 1;
									continue;
								}
								else
								{
									ind_buf.status=0;
								}
							}
							bitfg = bitfg << 1;
						cmdtype = C_SYS_MODE_SET;
						ComdSendToLCU(cmdtype,ind_buf.status,&dbentry);
/*							tim=time(&tim);
							timeptr=localtime(&tim);
							ind_buf.minute  =timeptr->tm_min;
							ind_buf.second  =timeptr->tm_sec;
							ind_buf.ms = 0;

							lanhd.dest_id=0;
							lanhd.length=sizeof(ind_buf);
							lanhd.src_id=GetHost_Cmd();
							lanhd.dp_type=BJ_DB;
#ifdef DEBUG
							printf("COL DO %d=%d=%d %d:%d:%d\n",ind_buf.ptr.dev_id,ind_buf.ptr.data_type,
								ind_buf.ptr.point_id,ind_buf.status,ind_buf.minute,ind_buf.second);
#endif
							lan_out((char *)&ind_buf,lanhd);*/
						}

						AckCommand(hPort_com,COL,sendbuf);
						timeoutnum =0;
						repeatnum = 0;
						break;
		
			case DRL:
#ifdef DEBUG
						printf("port%d DATA Request Latched Command Received!\n",port);
#endif
						slot = (recvbuf[4] >> 4) & 0x0F;
						if(rtuconfig[slot] != DO)
						{
							printf("DATA Request Latched Command %d slot %x not DO module\n",slot,rtuconfig[slot]);
							ResponseNAK(hPort_com,DRL,0x01,sendbuf);		/*cmd,err_code*/
							continue;
						}
						AckDataReqLatch(hPort_com,slot,dvlatch[slot],sendbuf);	/*slot,dvalue*/
						timeoutnum =0;
						repeatnum = 0;
						break;
		
			case SST:
						if(byte_recvd != SST_LEN)
							{
							printf("port%d Synchronize System Time frame length Wrong!\n",port);
						/*	ClearRecvFault();*/
							ResponseNAK(hPort_com,SST,0xFC,sendbuf);	/*cmd,err_code frame length*/
							repeatnum++;	/*let host timeout*/
							continue;
							}
/***						memcpy(&tim,&recvbuf[4],4);	//seconds since 2000.1.1***/
						tim = (time_t)(recvbuf[4]<<24);	/*High tim 24-31*/
						tim = tim + (time_t)(recvbuf[5]<<16);	/*High tim 16-23*/
						tim = tim + (time_t)(recvbuf[6]<<8);	/*low tim 8-15*/
						tim = tim + (time_t)recvbuf[7];	/*High low 0-7*/
						tim = tim + DRFT_SEC;	/*1970.1.1 -- 2000.1.1 GMT*/
/***						memcpy(&sst_ms,&recvbuf[8],2);	//1/10 ms***/
						sst_ms = recvbuf[8]<<8;
						sst_ms = sst_ms + recvbuf[9];
						if(sst_ms > 9999)
						{
							printf("port%d Synchronize System Time frame ms %d > 9999\n",port,sst_ms);
							ResponseNAK(hPort_com,SST,0x01,sendbuf);	/*cmd,err_code*/
							continue;
						}
						timeptr=localtime(&tim);
						/**GetLocalTime(&sys_tm);
						sys_tm.wYear = timeptr->tm_year + 1900;
						sys_tm.wMonth = timeptr->tm_mon+1;	  
						sys_tm.wDay = timeptr->tm_mday;	
						sys_tm.wHour = timeptr->tm_hour;
						sys_tm.wMinute = timeptr->tm_min;
						sys_tm.wSecond = timeptr->tm_sec; 
						sys_tm.wDayOfWeek = timeptr->tm_wday;
						sys_tm.wMilliseconds = (unsigned short)(sst_ms/10);
#ifdef DEBUG
						printf("port%d Synchronize System Time Command Received!\n",port);
						printf("%4d/%2d/%2d %2d:%2d:%2d %3dms DayOfWeek %2d\n",sys_tm.wYear,
								sys_tm.wMonth,sys_tm.wDay,sys_tm.wHour,sys_tm.wMinute,
								sys_tm.wSecond,sys_tm.wMilliseconds,sys_tm.wDayOfWeek);
#endif***zwj***/
/****						if(!SetLocalTime(&sys_tm)) 
//							printf("SetLocalTime() error!\n");****/

						timeoutnum =0;
						repeatnum = 0;
						ClockSyncflag = 1;
						rtu_status = rtu_status & 0xFD;
						AckSynchronizeSystemTime(hPort_com,&recvbuf[4],sendbuf);
						break;

			case RST:
#ifdef DEBUG
						printf("port%d Report System Time Command Received!\n",port);
#endif
						AckReportSysTime(hPort_com,sendbuf);
						timeoutnum =0;
						repeatnum = 0;
						break;

			case RIM:
#ifdef DEBUG
						printf("port%d Report Interface Mode Command Received!\n",port);
#endif
						AckReportInterfaceMode(hPort_com,sendbuf);
						timeoutnum =0;
						repeatnum = 0;
						break;
		
			case SIM:
#ifdef DEBUG
						printf("port%d Set Interface Mode Command Received!\n",port);
#endif
						AckCommand(hPort_com,SIM,sendbuf);
						timeoutnum =0;
						repeatnum = 0;
						break;
		
			case VER:
#ifdef DEBUG
						printf("port%d Read RTU And RMF Version Identifiers Command Received!\n",port);
#endif
						timeoutnum =0;
						repeatnum = 0;
						break;
		
			case RES:
#ifdef DEBUG
						printf("port%d Perform RTU Cold Start Command Received!\n",port);
#endif
						AckCommand(hPort_com,COL,sendbuf);
						timeoutnum =0;
						repeatnum = 0;
						break;
		
			case RERROR:
						printf("port%d Report Error DATA Command Received!\n",port);
						timeoutnum =0;
						repeatnum = 0;
						break;
/*
							recvbufheadpt = recv_data.pnt;
							memset(&recv_data.data[recvbufheadpt],0,CMD_DATA_SIZE);
							memcpy(&recv_data.data[recvbufheadpt],&recvbuf[4],byte_recvd - 6);
							recv_data.length[recvbufheadpt] =(unsigned short) (byte_recvd - 6);
							
							printf("recv point = %d\n",recvbufheadpt);
							
							IEC870_5_Recv(recvbufheadpt);

							recvbufheadpt ++;
							if(recvbufheadpt >= ASDU_ENTRY_NUMBER)
								recvbufheadpt = 0;
							recv_data.pnt = recvbufheadpt;
//							ReleaseSemaphore(hRecvSemaphore,1,NULL);
*/
		default :
#ifdef DEBUG
				printf("port%d sc1801 RTU received command id error! %2x :\n",port,recvbuf[0]);
#endif
				ResponseNAK(hPort_com,comd_id,0xFF,sendbuf);	/*cmd,err_code*/
				/*ClearRecvFault(hPort_com);*/
				break;			

		}/*switch(comd_id)*/
	    }/**else**/
	}/*for(;;)*/
}


void	PrintSendData(unsigned int nbsend)
{
	unsigned int d;

	for(d=0;d<nbsend;d++)
	{
		if((d%20)==0)
		{
			sprintf(infobuf,"\n<< ");
#ifdef DEBUG
			printf("\n<< ");
#endif
			WriteDspInfo(infobuf);
		}
		else if((d%10)==0)
		{
			sprintf(infobuf," ");
#ifdef DEBUG
			printf(" ");
#endif
			WriteDspInfo(infobuf);
		}
		sprintf(infobuf," %02X",sendbuf[d]);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	sprintf(infobuf,"\n");
#ifdef DEBUG
	printf("\n");
#endif
	WriteDspInfo(infobuf);
}

void	PrintRecvData(unsigned int nbrecv)
{
	unsigned int	d;

	for(d=0;d<nbrecv;d++)
	{
		if((d%20)==0)
		{
			sprintf(infobuf,"\n>> ");
#ifdef DEBUG
			printf("\n>> ");
#endif
			WriteDspInfo(infobuf);
		}
		else if((d%10)==0)
		{
			sprintf(infobuf," ");
#ifdef DEBUG
			printf(" ");
#endif
			WriteDspInfo(infobuf);
		}
		sprintf(infobuf," %02X",recvbuf[d]);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	sprintf(infobuf,"\n");
#ifdef DEBUG
	printf("\n");
#endif
	WriteDspInfo(infobuf);
}

void SEComdSendToLCU(char type_id, float value, DMS_COMMON *pdbcomm)
{
	struct	tm	tmpt;
	time_t		tim;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;

	cmd_buf.type_id = type_id;
	cmd_buf.stn_id = pdbcomm->point.stn_id;
	cmd_buf.dev_id = pdbcomm->point.dev_id;
	cmd_buf.data_type = pdbcomm->point.data_type;
	cmd_buf.point_id = pdbcomm->point.pt_id;

	cmd_buf.status.state=1;
	cmd_buf.status.result=0;
	cmd_buf.status.opr=0;
	cmd_buf.status.src=cmd_src;
	cmd_buf.data.iValue=(int)(value*1000);
	cmd_buf.ctrl_addr=pdbcomm->fixed.ctrl_addr;
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"sc1801RTU");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif
	if((type_id == C_OP_VALUE_SET)||(type_id == DPS_MAN_VALUE_SET))
	{
		lanhead.dest_id = 0;
		lanhead.dest_stn = 0;
		lanhead.dp_type=COMM_RECORD_TIME;

		sprintf(infobuf,"Set Point Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		sprintf(infobuf,"Set Point Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}


void ComdSendToLCU(char type_id,char cmd_stat,DMS_COMMON *pdbcomm)
{
	struct	tm	tmpt;
	time_t		tim;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;

	cmd_buf.type_id = type_id;
	cmd_buf.stn_id = pdbcomm->point.stn_id;
	cmd_buf.dev_id = pdbcomm->point.dev_id;
	cmd_buf.data_type = pdbcomm->point.data_type;
	cmd_buf.point_id = pdbcomm->point.pt_id;

	cmd_buf.status.opr=cmd_stat;
	cmd_buf.status.state=1;
	cmd_buf.status.result=0;
	cmd_buf.status.src=cmd_src;
	cmd_buf.data.iValue=0;
	cmd_buf.ctrl_addr=pdbcomm->fixed.ctrl_addr;
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"sc1801RTU");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif
	if((type_id == C_SYS_MODE_SET)||(type_id == DPS_MAN_STAT_SET))
	{
		lanhead.dest_id = 0;
		lanhead.dest_stn = 0;
		lanhead.dp_type=COMM_RECORD_TIME;

		sprintf(infobuf,"Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		sprintf(infobuf,"Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

int	SendData(void)
{
	send_byte = write(hPort_com, sendbuf, byte_to_send);

	if( send_byte != byte_to_send )
	{
		/*sprintf(infobuf,"\nIEC101s: write %d byte error: %s !!!\n",byte_to_send,strerror(errno));*/
		sprintf(infobuf,"\nSC1801RTU: write %d byte error!!!\n",byte_to_send);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
		PrintSendData(byte_to_send);
		sleep(1);
		return(-1);
	}
	sprintf(infobuf," %u byte transmitted!",send_byte);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);
	PrintSendData(send_byte);
	return(0);
}

#ifndef SUN_OS
void	ClearRecvFault(hPort_com)
HANDLE	hPort_com;
{
DWORD	dwErrors;
COMSTAT	cstat;

	if(!ClearCommError(hPort_com,&dwErrors,&cstat))
		printf("ReadFile: ClearCommError ERRNO= %d \n",GetLastError());
	printf("ReadFile ClearCommError dwErrors = %xH cbInQue=%d\n",
			dwErrors,cstat.cbInQue);
	if((dwErrors !=0)||(cstat.cbInQue !=0))
		if(PurgeComm(hPort_com,PURGE_RXCLEAR)==FALSE)
			printf("ClearRecvFault: PurgeComm ERRNO= %d \n",GetLastError());	
}


void ClearSendRespFault(hPort_com)
HANDLE	hPort_com;
{
DWORD	dwErrors;
COMSTAT	cstat;

	if(ClearCommError(hPort_com,&dwErrors,&cstat)==FALSE)
		printf("WriteFile: ClearCommError ERRNO= %d \n",GetLastError());	
	printf("WriteFile ClearCommError dwErrors = %xH cbOutQue=%d\n",
			dwErrors,cstat.cbOutQue);
	if((dwErrors !=0)||(cstat.cbOutQue !=0))
		if(PurgeComm(hPort_com,PURGE_TXCLEAR)==FALSE)
			printf("ClearSendRespFault: PurgeComm ERRNO= %d \n",GetLastError());	
}

#endif