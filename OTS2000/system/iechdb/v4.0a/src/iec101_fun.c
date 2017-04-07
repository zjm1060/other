/************************************************************
*  FILE NAME:           iec_fun.c      						*
*  FUNCTION:            Controlling Station					*
*						Basic Communication Functions				*
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
extern	unsigned char	recvbuf[1024];
extern	unsigned char	cmdbuf[36];
extern	ASDUCOMDDATA	send_data;
extern	ASDUDATA		recv_data;
extern	HANDLE	hPort_com;
extern	DWORD	dwErrors;
extern	COMSTAT	cstat;
extern	BOOL	fcb[STN_NUM+1];
extern	BOOL	resetlinkf[STN_NUM+1],initflag[STN_NUM+1],pollflag,comdflag[STN_NUM+1];
extern	unsigned int	repeatnum[STN_NUM+1],timeoutnum[STN_NUM+1];
extern	unsigned char	stn_id;

extern	SSWCTRL		sswctrl;
extern	DSWCTRL		dswctrl;
extern	SETPTCTRL	setpt;
extern	STEPCTRL	stepctrl;

BOOL	WriteSuccessf;
DWORD	byte_to_send;	
DWORD	send_byte;

void	ClearSendCmdFault();
void	PrintSendData();
float	Datcvt();

BOOL	RequestLinkStatus(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x49;
	
	if(fcb[stn_id])
		sendbuf[1] = sendbuf[1] | 0x20;

	sendbuf[2] = stn_id;
	sendbuf[3] = sendbuf[1] + sendbuf[2];
	sendbuf[4] = 0x16;
	byte_to_send = FIXFRAMELEN;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);

	if(WriteSuccessf==FALSE)
		printf("RequestLinkStatus: WriteFile ERRNO=%d\n",GetLastError());
	
#ifdef DEBUG
	printf("\nRequestLinkStatus: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	ResetLinkStatus(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x40;
	sendbuf[2] = stn_id;
	sendbuf[3] = sendbuf[1] + sendbuf[2];
	sendbuf[4] = 0x16;
	byte_to_send = FIXFRAMELEN;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);

#ifdef DEBUG
	printf("\nResetLinkStatus: %u byte transmitted\n",send_byte);
	PrintSendData(byte_to_send);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	RepeatTransmit(void)
{
	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);

#ifdef DEBUG
	printf("\nIEC repeat transmitted data byte number %u :\n",send_byte);
	PrintSendData(send_byte);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	CommandExec()
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 10;		//L

	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	memcpy(&sendbuf[5],&cmdbuf[5],9);	//copy to cmdbuf for exec

	sendbuf[12] = sendbuf[12] & 0x3F;

	byte_to_send = 16;

	chksum = 0;
	for(n=4;n<14;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[byte_to_send-2] = chksum;							 //checksum char
	sendbuf[byte_to_send-1] = 0x16;								 //end char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\n%u byte command  exec transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}
	
BOOL	ClockSynchronisation(void)
{
	unsigned char	chksum,n;
//	struct	tm	*tmpt;
//	time_t		tim;
	SYSTEMTIME	sys_tm;
	WORD		millisec;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 15;		//L

#ifdef SYN_BROADCAST
	sendbuf[4] = 0x44;					//control field	//no ack
	sendbuf[5] = 0xFF;					//link broadcast address field
	sendbuf[9] = 0xFF;					//ASDU common address
#else
	sendbuf[4] = 0x53;					//control field
	sendbuf[5] = stn_id;				//link address field
	sendbuf[9] = stn_id;				//ASDU common address
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;
#endif


	sendbuf[6] = C_CS_NA;			//type identifier field
	sendbuf[7] = 1;					//variable structure specify
	sendbuf[8] = ACT;				// cause of transmit:active:6
	sendbuf[10] = 0;				//information object low address
	sendbuf[11] = 0;				//information object high address

	GetLocalTime(&sys_tm);

//	tim=time(NULL);
//	tmpt=localtime(&tim);

	millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;
	memcpy(&sendbuf[12],&millisec,2);	//Milliseconds
	sendbuf[14] = (unsigned char)sys_tm.wMinute;	//Minutes
	sendbuf[15] = (unsigned char)sys_tm.wHour;		//Hour
	n = sys_tm.wDayOfWeek << 5;
	n = n & 0xE0;
	sendbuf[16] = (unsigned char)sys_tm.wDay | n;	//DayofWeek[6..8] day[1..5]
	sendbuf[17] = (unsigned char)sys_tm.wMonth;		//Month
	sendbuf[18] = (unsigned char)(sys_tm.wYear%100);		//Year
	
	byte_to_send = 21;

	chksum = 0;
	for(n=4;n<19;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[19] = chksum;				//checksum char
	sendbuf[20] = 0x16;					//end char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nClockSynchronisation: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	GeneralInterrogation(void)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 9;		//L
	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = stn_id;			//link address field
	sendbuf[6] = C_IC_NA;			//type identifier field
	sendbuf[7] = 0x81;				//variable structure specify
	sendbuf[8] = ACT;				//6 cause of transmit:active
	sendbuf[9] = stn_id;			//ASDU common address
	sendbuf[10] = 0;				//information object low address
	sendbuf[11] = 0;				//information object high address
	sendbuf[12] = INTROGEN;			//QOI:20 0x14							
	
	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;				//checksum char
	sendbuf[14] = 0x16;					//end char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nGeneralInterrogation: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	CounterInterrogation(void)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 9;		//L
	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = stn_id;						//link address field
	sendbuf[6] = C_CI_NA;					//101 type identifier field
	sendbuf[7] = 1;						//variable structure specify
	sendbuf[8] = ACT;						//6 cause of transmit:active
	sendbuf[9] = stn_id;						//ASDU common address
	sendbuf[10] = 0;						//Function
	sendbuf[11] = 0;					//information address
	sendbuf[12] = 0x41;					//QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]}
										//FRZ=1:frezw not reset, FRZ 2:=frezw not reset
										//RQT 1:=general counter interrogation
										//RQT 2-5 6-63 :=  reserve interrogation
	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;				//checksum char
	sendbuf[14] = 0x16;					//end char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nCounterInterrogation: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	CircuitBreakerCtrl(se,address,stat,std_n)
char	se,stat,std_n;
USHORT	address;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 9;		//L
	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = std_n;			//link address field
	sendbuf[6] = C_DC_NA;		//46 CB control type identifier
	sendbuf[7] = 1;				//variable structure specify
	sendbuf[8] = ACT;			//6 cause of transmit:active
	sendbuf[9] = std_n;			//ASDU common address

	memcpy(&sendbuf[10],&address,2);	//information address

	sendbuf[12] = stat;	//QCC:=CP8{DCS,QU,S/E}
							//DCS 1:=OFF 2:=ON
/*
	if(stat == 0)
		sendbuf[12] = 1;
	else
		sendbuf[12] = 2;	//QCC:=CP8{DCS,QU,S/E}
							//DCS 1:=OFF 2:=ON
*/
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	//S/E := 0:=Execute 1:= Select

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[14] = chksum;				//checksum char
	sendbuf[15] = 0x16;					//end char

	memcpy(&cmdbuf[5],&sendbuf[5],9);	//copy to cmdbuf for exec

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nCircuitBreakerCtrl: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	RegulatStepCtrl(se,address,stat,std_n)
char	se,stat,std_n;
USHORT	address;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 9;		//L
	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = std_n;				//link address field
	sendbuf[6] = C_RC_NA;			//47 regulating step control type identifier
	sendbuf[7] = 0x01;				//variable structure specify
	sendbuf[8] = ACT;				//6 cause of transmit:active
	sendbuf[9] = std_n;				//ASDU common address

	memcpy(&sendbuf[10],&address,2);	//information address

	sendbuf[12] = stat;	//QCC:=CP8{RCS,QU,S/E}
							//RCS 1:=low 2:=raise 3:=stop
/*
	if(stat == 0)
		sendbuf[12] = 1;
	else
		sendbuf[12] = 2;	//QCC:=CP8{RCS,QU,S/E}
							//RCS 1:=low 2:=raise 3:=stop
*/
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	//S/E := 0:=Execute 1:= Select

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;				//checksum char
	sendbuf[14] = 0x16;					//end char

	memcpy(&cmdbuf[5],&sendbuf[5],9);	//copy to cmdbuf for exec

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nRegulatStepCtrl: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}
/*
BOOL	CtrlCommand(se,address,stat,std_n)
char	se,address,stat,std_n;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 9;		//L
	sendbuf[4] = 0x53;					//control field
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = std_n;						//link address field
	sendbuf[6] = C_CC_NA_3;				//transformer step type identifier field
	sendbuf[7] = 0x01;						//variable structure specify
	sendbuf[8] = RMT_OP;				//12 cause of transmit:active
	sendbuf[9] = std_n;						//ASDU common address
	sendbuf[10] = 1;						//function
	sendbuf[11] = address;					//information address
	
	sendbuf[12] = stat;	//QCC:=CP8{DCS,QU,S/E}
							//DCS 1:=OFF 2:=ON
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	//S/E := 0:=Execute 1:= Select

	sendbuf[13] = 10;			//RIN

	byte_to_send = 16;

	chksum = 0;
	for(n=4;n<14;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[14] = chksum;				//checksum char
	sendbuf[15] = 0x16;					//end char

	memcpy(&cmdbuf[5],&sendbuf[5],9);	//copy to cmdbuf for exec

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nCtrl Command: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}
*/
BOOL	SetPointCmdNA(se,address,value)
char	se;
USHORT	address,value;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 11;		//L
	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = stn_id;				//link address field
	sendbuf[6] = C_SE_NA;				//66 type identifier field
	sendbuf[7] = 1;						//variable structure specify
	sendbuf[8] = ACT;					//cause of transmit:active:6
	sendbuf[9] = stn_id;				//ASDU common address
	memcpy(&sendbuf[10],&address,2);	//information object  address
//	sendbuf[11] = 0;					//information object high address
	
	memcpy(&sendbuf[12],&value,2);		//Normalized Value

	sendbuf[14] = 0;

	if(se)
		sendbuf[14] = sendbuf[14] | 0x80;	//S/E := 0:=Execute 1:= Select

	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;				//checksum char
	sendbuf[16] = 0x16;					//end char

	memcpy(&cmdbuf[5],&sendbuf[5],9);	//copy to cmdbuf for exec
	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nSet Point Command NA: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	SetPointCmdNB(se,address,value)
char	se;
USHORT	address,value;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 11;		//L
	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = stn_id;						//link address field
	sendbuf[6] = C_SE_NB;				//49 type identifier field
	sendbuf[7] = 1;						//variable structure specify
	sendbuf[8] = ACT;						//cause of transmit:active:6
	sendbuf[9] = stn_id;						//ASDU common address
	memcpy(&sendbuf[10],&address,2);	//information object  address
//	sendbuf[11] = 0;					//information object high address
	
	memcpy(&sendbuf[12],&value,2);		//Scaled Value

	sendbuf[14] = 0;

	if(se)
		sendbuf[14] = sendbuf[14] | 0x80;	//S/E := 0:=Execute 1:= Select

	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;				//checksum char
	sendbuf[16] = 0x16;					//end char

	memcpy(&cmdbuf[5],&sendbuf[5],9);	//copy to cmdbuf for exec
	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nSet Point Command NB: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	ParamSetCmdNA(kpa,address,value)
char	kpa;
USHORT	address,value;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 11;		//L
	sendbuf[4] = 0x53;					//control field
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = stn_id;				//link address field
	sendbuf[6] = P_ME_NA;				//110 parameter for measurements
	sendbuf[7] = 1;						//variable structure specify
	sendbuf[8] = ACT;					//cause of transmit:active:6
	sendbuf[9] = stn_id;				//ASDU common address
	memcpy(&sendbuf[10],&address,2);	//information object  address
//	sendbuf[11] = 0;					//information object high address
	
	memcpy(&sendbuf[12],&value,2);		//Normalized Value

	sendbuf[14] = kpa;					//KPA[1-6] LPC POP
										//KPA 1:= threshold 2:= filter factor
										//3:= low limit  4:= high limit


	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;				//checksum char
	sendbuf[16] = 0x16;					//end char

	memcpy(&cmdbuf[5],&sendbuf[5],9);	//copy to cmdbuf for exec
	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nParameter Set Command NA: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif

	if(!WriteSuccessf || send_byte == 0)
		{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	CommandSend(comdpnt)

unsigned int	comdpnt;
{
unsigned short	ctrladdr,stat,setdata,inf_addr,i;
unsigned char	station_ctrl,name[36],dev_id;
float			setvalue;

	if(fcb[stn_id])
		SetPointCmdNA(0,0xB81,234);
	else
		ParamSetCmdNA(1,PE_BASE_ADDR,15);
	return TRUE;

	switch	( send_data.data[comdpnt][1] ) 
		{
		case C_SWITCH:
		case C_BREAKER:
			memcpy(&ctrladdr,&send_data.data[comdpnt][2],2);	
			memcpy(&stat,&send_data.data[comdpnt][4],2);	
			station_ctrl = ctrladdr / 10;
			if((station_ctrl < 5) || (station_ctrl > 8))
				break;
			inf_addr = ctrladdr % 10 + SC_CMD_BASE_ADDR;
			if(inf_addr > 53)
				break;
			if((stat & 0x00F0) == 0x20 )
				{
#ifdef DEBUG
				printf("++++++++%d station INF=%d CB close\n",station_ctrl,inf_addr);
#endif
				CircuitBreakerCtrl(1,inf_addr,2,station_ctrl);
				}
			else
				{
#ifdef DEBUG
				printf("++++++++%d station INF=%d CB open\n",station_ctrl,inf_addr);
#endif
				CircuitBreakerCtrl(1,inf_addr,1,station_ctrl);
				}
			break;

		case C_GATE:
			memcpy((char *)&ctrladdr,&send_data.data[comdpnt][2],2);	
			memcpy((char *)&stat,&send_data.data[comdpnt][4],2);	
			station_ctrl = ctrladdr / 10;
			if((station_ctrl < 5) || (station_ctrl > 8))
				break;
			inf_addr = ctrladdr % 10 + RC_CMD_BASE_ADDR;
			if(inf_addr > 53)
				break;
			if((stat & 0x00F0) == 0x10 )
				RegulatStepCtrl(1,inf_addr,2,station_ctrl);
			else if((stat & 0x00F0) == 0x20 )
				RegulatStepCtrl(1,inf_addr,1,station_ctrl);
			else if((stat & 0x00F0) == 0x30 )
				RegulatStepCtrl(1,inf_addr,3,station_ctrl);

			break;
		case C_APOWER:
		case C_RPOWER:
//			send_data.data[sd_pnt][0] = C_SE_NA;	//set-point command
			memcpy(&stat,&send_data.data[comdpnt][4],2);
			memcpy(&setdata,&send_data.data[comdpnt][6],2);
			memset(name,0,36);
			dev_id = send_data.data[comdpnt][0];
			if(GetGroupNameById( stn_id, dev_id, name )==-1)
				break;
			strcat(name,"ANA");

			if(send_data.data[comdpnt][1] == C_APOWER)
				{
				strcat(name,"YG_MW");
				}
			else
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
				break;;
				}
			setvalue = Datcvt(stat,setdata);
			setdata=(unsigned short)((setvalue-setpt.lorange[i])*RESOLUTE/
					(setpt.hirange[i]-setpt.lorange[i]) + 0.5);

			inf_addr = SETP_CMD_BASE_ADDR + i;
			SetPointCmdNA(0,inf_addr,setdata);
			break;

			default :
#ifdef DEBUG
			printf("No this command type %d\n",send_data.data[comdpnt][0]);
#endif
			break;			
		}//switch(send_data.data[comdpnt][0])
	return(TRUE);
/*
	switch	( send_data.data[comdpnt][0] ) 
		{
		case  0:
//			SinglePointCtrl(se,address,stat);
			CircuitBreakerCtrl(0,SC_CMD_BASE_ADDR,0,5);
			break;

		case  1:
//			SinglePointCtrl(se,address,stat);
			CircuitBreakerCtrl(0,SC_CMD_BASE_ADDR,1,5);
			break;

		case  2:
//			RegulatStepCtrl(se,address,stat);
			RegulatStepCtrl(0,RC_CMD_BASE_ADDR,1);
			break;

		case  3:
//			RegulatStepCtrl(se,address,stat);
			RegulatStepCtrl(0,RC_CMD_BASE_ADDR,2);
			break;

			default :
#ifdef DEBUG
			printf("No this command type %d\n",send_data.data[comdpnt][0]);
#endif
			break;			
		}//switch(send_data.data[comdpnt][0])
	return(TRUE);
*/
}
	
BOOL	RequestUserDataClass1(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x5A;
	
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[1] = sendbuf[1] | 0x20;
	
	sendbuf[2] = stn_id;
	sendbuf[3] = sendbuf[1] + sendbuf[2];
	sendbuf[4] = 0x16;
	byte_to_send = FIXFRAMELEN;
	
	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
	//	Sleep(10);
	
#ifdef DEBUG
	printf("\nRequestUserDataClass1: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
	{
		Sleep(100);
		ClearSendCmdFault();
		return(FALSE);
	}
	return(TRUE);
}
	
BOOL	RequestUserDataClass2(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x5B;
	
	fcb[stn_id] = !fcb[stn_id];
	if(fcb[stn_id])
		sendbuf[1] = sendbuf[1] | 0x20;

	sendbuf[2] = stn_id;
	sendbuf[3] = sendbuf[1] + sendbuf[2];
	sendbuf[4] = 0x16;
	byte_to_send = FIXFRAMELEN;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);

#ifdef DEBUG
	printf("\nRequestUserDataClass2: %u byte transmitted\n",send_byte);
	PrintSendData(send_byte);
#endif
	
	if(!WriteSuccessf || send_byte == 0)
		{
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

void	ClearRecvFault(void)
{
	if(!ClearCommError(hPort_com,&dwErrors,&cstat))
		printf(">>>>>>ReadFile: ClearCommError ERRNO= %d \n",GetLastError());
	printf(">>>>>>ReadFile ClearCommError dwErrors = %xH cbInQue=%d\n",
			dwErrors,cstat.cbInQue);
	if((dwErrors !=0)||(cstat.cbInQue !=0))
		if(PurgeComm(hPort_com,PURGE_RXCLEAR)==FALSE)
			printf(">>>>>>ClearRecvFault: PurgeComm ERRNO= %d \n",GetLastError());	
	timeoutnum[stn_id]++;
	if(comdflag[stn_id])
		repeatnum[stn_id]++;
}


void ClearSendCmdFault(void)
{
	if(ClearCommError(hPort_com,&dwErrors,&cstat)==FALSE)
		printf("<<<<<<WriteFile: ClearCommError ERRNO= %d \n",GetLastError());	
	printf("<<<<<<WriteFile ClearCommError dwErrors = %xH cbOutQue=%d\n",
			dwErrors,cstat.cbOutQue);
	if((dwErrors !=0)||(cstat.cbOutQue !=0))
		if(PurgeComm(hPort_com,PURGE_TXCLEAR)==FALSE)
			printf("<<<<<<ClearSendCmdFault: PurgeComm ERRNO= %d \n",GetLastError());	
}

void	PrintSendData(nbsend)
DWORD	nbsend;
{
	DWORD d;

	for(d=1;d<=nbsend;d++)
		{
		printf("=%2x",sendbuf[d-1]);
		if((d%10)==0)
			printf("\n");
		}
	printf("\n");
}

void	PrintRecvData(nbrecv)
DWORD	nbrecv;
{
	DWORD	d;

	for(d=1;d<=nbrecv;d++)
		{
		printf("=%2x",recvbuf[d-1]);
		if((d%10)==0)
			printf("\n");
		}
	printf("\n");
}

