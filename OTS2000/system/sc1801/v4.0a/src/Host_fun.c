
/************************************************************
*  FILE NAME:           Host_fun.c      					*
*  FUNCTION:            Controlling Station					*
*						Basic Communication Functions		*
*  's NAME :  			              						*
*  DATE:                May,2000                			*
************************************************************/


#include	"..\inc\sc1801Host.h"

#define	DEBUG
extern	char	infobuf[1024];
extern	unsigned char	sendbuf[1024];
extern	unsigned char	recvbuf[1024];
extern	ASDUCOMDDATA	send_data;
extern	ASDUDATA		recv_data;
extern	unsigned char	send_comd_id,rtu_status,rtu_jumpter;
extern	HANDLE	hPort_com;
/*extern	DWORD	dwErrors;
extern	COMSTAT	cstat;*/
extern	BOOL	fcb;
extern	BOOL	resetlinkf,initflag,pollflag,comdflag;
extern	unsigned int	repeatnum,timeoutnum;
extern	unsigned char	rtuconfig[16];
extern	unsigned char	pntname[16][24][NAME_SIZE];
extern  float	lorange[16][12],hirange[16][12];
extern	unsigned short	databand,bytecount,retry;
extern	time_t	tim;

BOOL	WriteSuccessf;
DWORD	byte_to_send;	
DWORD	send_byte;

void	ClearRecvFault();
void	ClearSendRespFault();
void	ClearSendCmdFault();
void	PrintSendData();

BOOL	RequestRTUConfig(void)
{
	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = RRC;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/

	sendbuf[2] = 0;
	sendbuf[3] = 0;
	sendbuf[4] = sendbuf[0] ^ sendbuf[1] ^ 0xff;

	byte_to_send = 5;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);

	if(WriteSuccessf==FALSE)
		printf("RequestRTUConfig: WriteFile ERRNO=%d\n",GetLastError());
	
#ifdef DEBUG
	printf("\nRequestRTUConfig: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}


BOOL	ClockSynchronisation(void)
{
	//SYSTEMTIME	sys_tm;
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = SST;
	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;

	bytecount = 6;
	sendbuf[2] = 0;
	sendbuf[3] = (unsigned char)bytecount;							//L

	//GetLocalTime(&sys_tm);

	tim=time(&tim);
	//tmpt=localtime(&tim);

	tim = tim - DRFT_SEC;	//1970.1.1 -- 2000.1.1 GMT
	//memcpy(&sendbuf[4],&tim,4);	//seconds since 2000.1.1
	sendbuf[4] = (unsigned char)(tim>>24);	//High tim 24-31
	sendbuf[5] = (unsigned char)(tim>>16);	//High tim 16-23
	sendbuf[6] = (unsigned char)(tim>>8);	//low tim 8-15
	sendbuf[7] = (unsigned char)tim;		//Low tim 0-8
	
	sendbuf[8] = 0;		//1/10 ms
	sendbuf[9] = 0;		//1/10 ms

	byte_to_send = bytecount + 5;

	chksum = 0xff;

	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);

#ifdef DEBUG
	printf("\nClockSynchronisation: %u byte transmitted\n",send_byte);
	PrintSendData(byte_to_send);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	RepeatTransmit(void)
{
	unsigned short byte_num;
	unsigned char	chksum,n;
	
	send_comd_id = sendbuf[1] & 0x3F;
	sendbuf[1] = sendbuf[1] | 0x80;

	//memcpy(&byte_num,&sendbuf[2],2);
	byte_num = (sendbuf[2] << 8) | sendbuf[3];

	byte_to_send = byte_num + 5;

	chksum = 0xff;

	for(n=0;n<byte_to_send - 1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);

#ifdef DEBUG
	printf("\nHost repeat transmitted data byte number %u :\n",send_byte);
	PrintSendData(send_byte);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	CommandSend(comdpnt)
unsigned int	comdpnt;
{
	unsigned char	chksum,n;

	memcpy(&sendbuf[0], &send_data.data[comdpnt], send_data.length[comdpnt]);

	byte_to_send = send_data.length[comdpnt];

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\n%u byte command  data transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	RequestFullData(void)
{
	//unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = DRF;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 0;							//L
	sendbuf[4] = sendbuf[0] ^ sendbuf[1] ^ 0xff;

	byte_to_send = 5;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nRequestFullData: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	RequestExceptData(void)
{
	//unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = XRF;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 0;							//L
	sendbuf[4] = sendbuf[0] ^ sendbuf[1] ^ 0xff;

	byte_to_send = 5;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nRequestExceptData: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	SequenceOfEvents(void)
{
	//unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = SOE;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 0;							//L
	sendbuf[4] = sendbuf[0] ^ sendbuf[1] ^ 0xff;

	byte_to_send = 5;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nSequenceOfEvents: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	PulseCommand(cmd)
unsigned char cmd;
{
	//	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = cmd;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 0;							//L
	sendbuf[4] = sendbuf[0] ^ sendbuf[1] ^ 0xff;

	byte_to_send = 5;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nPulseCommand %d: %u byte transmitted\n",cmd,send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	SetDataBand(databand)
unsigned short databand;
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = SDB;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 2;							//L

	//memcpy(&sendbuf[4],&databand,2);
	sendbuf[4] = (unsigned char)(databand>>8);	//High databand
	sendbuf[5] = (unsigned char)databand;	//Low databand
	
	byte_to_send = 7;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nSetDataBand %d: %u byte transmitted\n",databand,send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	ReportDataBand()
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = RDB;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 2;							//L

	sendbuf[4] = 0x10;		//Slot 1 (AI)
	sendbuf[5] = 0x20;		//Slot 2 (AI)
	
	byte_to_send = 7;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nReportDataBand %d: %u byte transmitted\n",databand,send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	CtrlOutputARM(timeout,slot,point)
unsigned char timeout,slot,point;
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = COA;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 2;							//L

	sendbuf[4] = timeout;		//Time out value
	sendbuf[5] = slot << 4;		//Slot
	sendbuf[5] = sendbuf[5] | point;	//point

	byte_to_send = 7;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nCtrlOutputARM: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	CtrlOutputExecute(slot,point)
unsigned char slot,point;
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = COE;

	/*
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 1;							//L

	sendbuf[4] = slot << 4;		//Slot
	sendbuf[4] = sendbuf[4] | point;	//point

	byte_to_send = 6;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nCtrlOutputExecute: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	AOCtrlDirect(slot,point,value)
unsigned char slot,point;
float value;
{
	unsigned char	chksum,n;
	unsigned short anaorgcode;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = COD;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 3;							//L

	sendbuf[4] = slot << 4;		//Slot
	sendbuf[4] = sendbuf[4] | point;	//point

	anaorgcode=(unsigned short)((value-lorange[slot][point])*RESOLUTE/(hirange[slot][point]-lorange[slot][point])+0.5);
	anaorgcode=(anaorgcode&0x0FFF);

	printf("\nAOCtrlDirect: %d:%d %f %d\n",slot,point,value,anaorgcode);

	sendbuf[5] = anaorgcode >> 8;		//value high 4 bits
	sendbuf[5] = sendbuf[5] | 0x40;	//AO

	sendbuf[6] = anaorgcode & 0xff;	//value low 8 bits

	byte_to_send = 8;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nAOCtrlDirect: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	DOCtrlDirect(slot,point,duration,resolution)
unsigned char slot,point;
unsigned short duration;
unsigned char resolution;
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = COD;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 4;							//L

	sendbuf[4] = slot << 4;		//Slot
	sendbuf[4] = sendbuf[4] | point;	//point

	sendbuf[5] = duration >> 8;		// duration high 6 bits
	sendbuf[5] = sendbuf[5] | 0xC0;	//DO

	sendbuf[6] = duration & 0xFF;	//duration low 8 bits

	sendbuf[7] = resolution;

	byte_to_send = 9;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nDOCtrlDirect: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	CtrlOutputLatch(slot,dvalue)
unsigned char slot;
DWORD dvalue;
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = COL;

	/*	
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 5;							//L

	sendbuf[4] = slot << 4;		//Slot

	memcpy(&sendbuf[5],&dvalue,4);	//32bits digital output
	/*
	sendbuf[5] = (unsigned char)(dvalue>>24);	//High dvalue 24-31
	sendbuf[6] = (unsigned char)(dvalue>>16);	//High dvalue 16-23
	sendbuf[7] = (unsigned char)(dvalue>>8);	//low dvalue 8-15
	sendbuf[8] = (unsigned char)dvalue;			//Low dvalue 0-8
	*/
	byte_to_send = 10;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nCtrlOutputLatch: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	DataReqLatch(slot)
unsigned char slot;
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = DRL;

	/*
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 1;							//L

	sendbuf[4] = slot << 4;		//Slot

	byte_to_send = 6;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nDataReqLatch: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}

BOOL	ReportSysTime(void)
{
	unsigned char	chksum,n;

	sendbuf[0] = STATION_ID;
	send_comd_id = sendbuf[1] = RST;

	/*
	if(retry)
		sendbuf[1] = sendbuf[1] | 0x80;
	*/
	sendbuf[2] = 0;
	sendbuf[3] = 0;							//L

	byte_to_send = 5;

	chksum = 0xff;
	for(n=0;n<byte_to_send-1;n++)
		chksum = chksum ^ sendbuf[n];

	sendbuf[byte_to_send-1] = chksum;							 //checksum char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//Sleep(10);
	
#ifdef DEBUG
	printf("\nReportSysTime: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
	}

	return(TRUE);
}


BOOL	SimuCommandSend(comdpnt)
unsigned int	comdpnt;
{
	Sleep(3000);

	switch( comdpnt )
	{
	case  0:
		ReportSysTime();
		break;

	case  1:
		AOCtrlDirect(4,0,432.0);
		break;

	case  2:
		//DoublePointCtrl(se,address,stat);
		SetDataBand(20);
		break;

	case  3:
		//RequestFullData();
		AOCtrlDirect(4,0,321.0);
		break;

	case  4:
		//CtrlOutputARM(timeout,slot,point);
		//CtrlOutputARM(55,8,2);
		ReportDataBand();
		break;

	case  5:
		//CtrlOutputExecute(slot,point);
		//CtrlOutputExecute(8,2);
		//RequestExceptData();
		AOCtrlDirect(4,0,210.0);
		break;

	case  6:
		//AOCtrlDirect(slot,point,value);
		ReportSysTime();
		break;

	case  7:
		//DOCtrlDirect(slot,point,duration,resolution);
		//DOCtrlDirect(4,1,20,1);
		//RequestExceptData();
		AOCtrlDirect(4,0,123.0);
		break;

	case  8:
		//CtrlOutputLatch(slot,dvalue);
		//CtrlOutputLatch(11,0x12345678);
		//RequestExceptData();
		ReportDataBand();
		break;

	case  9:
		//DataReqLatch(slot);
		//DataReqLatch(11);
		//RequestExceptData();
		AOCtrlDirect(4,0,234.0);
		break;

	case  10:
		//RequestExceptData(void);
		//RequestExceptData();
		ReportDataBand();
		break;

	case  11:
		//PulseCommand(cmd);
		//PulseCommand(PAR);
		AOCtrlDirect(4,0,345.0);
		break;

	case  12:
		//AOCtrlDirect(slot,point,value);
		//AOCtrlDirect(4,0,508.0);
		//RequestExceptData();
		AOCtrlDirect(4,0,456.0);
		break;

	default :
#ifdef DEBUG
		printf("No this command type %d\n",comdpnt);
#endif
		break;			
	}	//switch()

	return(TRUE);
}

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

void	ClearSendCmdFault()
{}