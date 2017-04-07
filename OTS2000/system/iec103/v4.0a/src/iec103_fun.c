/*******************************************************************************************************************************
FILENAME:	
FUNCTION:		

VERSION:	v4.0a
OPERATE SYSTEM:	window 2000, XP

HISTORY:
2008.4.10	start to modify the code for H9000 V4.0		zyf
********************************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#include "\home\ems\system\iec103\v4.0a\inc\iec103_com.h"

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
extern	BOOL	resetlinkf[STN_NUM+1],resetframe[STN_NUM+1],initflag[STN_NUM+1],pollflag;
extern	unsigned int	repeatnum[STN_NUM+1],timeoutnum[STN_NUM+1];
extern	unsigned char	stn_id;

BOOL	WriteSuccessf;
DWORD	byte_to_send;	
DWORD	send_byte;

void	ClearSendCmdFault();
void	PrintSendData();

BOOL	RequestLinkStatus(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x49;
	
	if(fcb[stn_id])
		sendbuf[1] = sendbuf[1] | 0x20;

	//zm sendbuf[2] = stn_id;
	sendbuf[2] = GetStnAddr( stn_id ); //zm
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
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	ResetLinkStatus(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x40;
	//zm sendbuf[2] = stn_id;
	sendbuf[2] = GetStnAddr( stn_id ); //zm
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
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	ResetFrameStatus(void)    //C_RFB_NA_3 复位帧计数位
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x47;
	sendbuf[2] = stn_id;
	sendbuf[3] = sendbuf[1] + sendbuf[2];
	sendbuf[4] = 0x16;
	byte_to_send = FIXFRAMELEN;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);

#ifdef DEBUG
	printf("\nResetFrameStatus: %u byte transmitted\n",send_byte);
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
	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);

#ifdef DEBUG
	printf("\nIEC repeat transmitted data byte number %u :\n",send_byte);
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

BOOL	CommandExec()
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 10;		//L

	sendbuf[4] = 0x53;					//control field
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
		Sleep(1000);
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
	sendbuf[4] = 0x44;					//control field transmit/no ack
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

//	sendbuf[5] = stn_id;						//link address field
	sendbuf[5] = 0xFF;						//link broadcast address field
	sendbuf[6] = C_SYN_TA_3;					//type identifier field
	sendbuf[7] = 0x81;						//variable structure specify
	sendbuf[8] = SYN_CLK;						//8 cause of transmit:active
//	sendbuf[9] = stn_id;						//ASDU common address
	sendbuf[9] = 0xFF;						//ASDU common address
	sendbuf[10] = 255;					//function type
	sendbuf[11] = 0;					//information number

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
		Sleep(1000);
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
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	//zm sendbuf[5] = stn_id;						//link address field
	sendbuf[5] = GetStnAddr( stn_id );//zm						//link address field
	sendbuf[6] = C_IGI_NA_3;					//type identifier field
	sendbuf[7] = 0x81;						//variable structure specify
	sendbuf[8] = GI_ACK;						//9 cause of transmit:active
	//zm sendbuf[9] = stn_id;						//ASDU common address
	sendbuf[9] = GetStnAddr( stn_id );//zm						//ASDU common address
	sendbuf[10] = 255;					//function type
	sendbuf[11] = 0;					//information number
	sendbuf[12] = 20;					//SCN							
	
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
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}

BOOL	CounterInterrogation(void)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 10;		//L
	sendbuf[4] = 0x53;					//control field
	if(fcb[stn_id])
		sendbuf[4] = sendbuf[4] | 0x20;

	//zm sendbuf[5] = stn_id;						//link address field
	sendbuf[5] = GetStnAddr( stn_id );//zm						//link address field
	sendbuf[6] = C_CI_NA_3;					//101 type identifier field
	sendbuf[7] = 0x81;						//variable structure specify
	sendbuf[8] = CYC;						//2 cause of transmit:active
	//zm sendbuf[9] = stn_id;						//ASDU common address
	sendbuf[9] = GetStnAddr( stn_id );//zm						//ASDU common address
	sendbuf[10] = 1;						//Function
	sendbuf[11] = 0;					//information address
	sendbuf[12] = 0x45;					//QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]}
										//FRZ=1:frezw not reset, FRZ 2:=frezw not reset
										//RQT 5:=general counter interrogation
										//RQT 1-4 6-63 :=  reserve interrogation
	sendbuf[13] = 11;					//RII
	byte_to_send = 16;

	chksum = 0;
	for(n=4;n<14;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[14] = chksum;				//checksum char
	sendbuf[15] = 0x16;					//end char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nCounterInterrogation: %u byte transmitted\n",send_byte);
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

BOOL	CircuitBreakerCtrl(se,address,stat,std_n)
char	se,address,stat,std_n;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 10;		//L
	sendbuf[4] = 0x53;					//control field
	if(fcb[std_n])
		sendbuf[4] = sendbuf[4] | 0x20;

	//zm sendbuf[5] = std_n;						//link address field
	sendbuf[5] = GetStnAddr( std_n );//zm						//link address field
	sendbuf[6] = C_DC_NA_3;					//64 CB control type identifier
	sendbuf[7] = 0x01;					//variable structure specify
	sendbuf[8] = RMT_OP;				//12 cause of transmit:active
	//zm sendbuf[9] = std_n;				//ASDU common address
	sendbuf[9] = GetStnAddr( std_n );//zm				//ASDU common address
	sendbuf[10] = 1;				//function
	sendbuf[11] = address;				//information address
	
	sendbuf[12] = stat;		//QCC:=CP8{DCS,QU,S/E}
							//DCS 1:=OFF 2:=ON
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	//S/E := 0:=Execute 1:= Select

	sendbuf[13] = 13;			//RIN

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
	printf("\nCircuitBreakerCtrl: %u byte transmitted\n",send_byte);
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

BOOL	RegulatStepCtrl(se,address,stat,std_n)
char	se,address,stat,std_n;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 10;		//L
	sendbuf[4] = 0x53;					//control field
	if(fcb[std_n])
		sendbuf[4] = sendbuf[4] | 0x20;

	//zm sendbuf[5] = std_n;						//link address field
	sendbuf[5] = GetStnAddr( std_n );//zm						//link address field
	sendbuf[6] = C_RC_NA_3;				//65 regulating step control type identifier
	sendbuf[7] = 0x01;					//variable structure specify
	sendbuf[8] = RMT_OP;				//12 cause of transmit:active
	//zm sendbuf[9] = std_n;				//ASDU common address
	sendbuf[9] = GetStnAddr( std_n );//zm				//ASDU common address
	sendbuf[10] = 1;					//function
	sendbuf[11] = address;				//information address

	sendbuf[12] = stat;		//RCC:=CP8{RCS,QU,S/E}
							//RCS 1:=low 2:=raise 3:=stop
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	//S/E := 0:=Execute 1:= Select

	sendbuf[13] = 12;			//RIN

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
	printf("\nRegulatStepCtrl: %u byte transmitted\n",send_byte);
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

BOOL	CtrlCommand(se,address,stat,std_n)
char	se,address,stat,std_n;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 10;		//L
	sendbuf[4] = 0x53;					//control field
	if(fcb[std_n])
		sendbuf[4] = sendbuf[4] | 0x20;

	//zm sendbuf[5] = std_n;						//link address field
	sendbuf[5] = GetStnAddr( std_n );//zm						//link address field
	sendbuf[6] = C_CC_NA_3;				//transformer step type identifier field
	sendbuf[7] = 0x81;						//variable structure specify
	sendbuf[8] = RMT_OP;				//12 cause of transmit:active
	//zm sendbuf[9] = std_n;						//ASDU common address
	sendbuf[9] = GetStnAddr( std_n );//zm						//ASDU common address
	sendbuf[10] = 1;						//function
	sendbuf[11] = address;					//information address
	
	if(stat == 0)
		sendbuf[12] = 1;
	else
		sendbuf[12] = 2;	//QCC:=CP8{RCS,QU,S/E}
							//SCS 2:=Higher 1:=Lower
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
	printf("\nDouble Point Ctrl: %u byte transmitted\n",send_byte);
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

BOOL	SetPointCmdNA(se,address,value)
char	se;
int address;
int value;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 11;		//L
	sendbuf[4] = 0x53;					//control field
	if(fcb[stn_id])		//std_n
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = 0;						//link address field
	sendbuf[6] = C_SE_NA_3;				//66 type identifier field
	sendbuf[7] = 1;						//variable structure specify
	sendbuf[8] = 6;						//cause of transmit:active
	sendbuf[9] = 0;						//ASDU common address
	memcpy(&sendbuf[10],&address,2);	//information object  address
//	sendbuf[11] = 0;					//information object high address
	
	memcpy(&sendbuf[12],&value,2);	//Normalized Value

	sendbuf[14] = 0;

	if(se)
		sendbuf[14] = sendbuf[14] | 0x80;	//S/E := 0:=Execute 1:= Select

	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;				//checksum char
	sendbuf[16] = 0x16;					//end char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nSet Point Command NA: %u byte transmitted\n",send_byte);
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

BOOL	SetPointCmdNB(se,address,value)
char	se;
int address;
int value;
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		//start flag
	sendbuf[1] = sendbuf[2] = 11;		//L
	sendbuf[4] = 0x53;					//control field
	if(fcb[stn_id])		//std_n
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = 0;						//link address field
	sendbuf[6] = C_SE_NA_3;				//49 type identifier field
	sendbuf[7] = 1;						//variable structure specify
	sendbuf[8] = 6;						//cause of transmit:active
	sendbuf[9] = 0;						//ASDU common address
	memcpy(&sendbuf[10],&address,2);	//information object  address
//	sendbuf[11] = 0;					//information object high address
	
	memcpy(&sendbuf[12],&value,2);	//Scaled Value

	sendbuf[14] = 0;

	if(se)
		sendbuf[14] = sendbuf[14] | 0x80;	//S/E := 0:=Execute 1:= Select

	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;				//checksum char
	sendbuf[16] = 0x16;					//end char

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);
//	Sleep(10);
	
#ifdef DEBUG
	printf("\nSet Point Command NB: %u byte transmitted\n",send_byte);
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
unsigned short	ctrladdr,stat;
unsigned char	station_ctrl,inf_addr;

	switch	( send_data.data[comdpnt][1] ) 
		{
		case C_SWITCH:
		case C_BREAKER:
			memcpy((char *)&ctrladdr,&send_data.data[comdpnt][2],2);	
			memcpy((char *)&stat,&send_data.data[comdpnt][4],2);	
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

			default :
#ifdef DEBUG
			printf("No this command type %d\n",send_data.data[comdpnt][0]);
#endif
			break;			
		}//switch(send_data.data[comdpnt][0])
	return(TRUE);
}
	
BOOL	RequestUserDataClass1(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x5A;
	
	if(fcb[stn_id])
		sendbuf[1] = sendbuf[1] | 0x20;

	//zm sendbuf[2] = stn_id;
	sendbuf[2] = GetStnAddr( stn_id );//zm
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
		Sleep(1000);
		ClearSendCmdFault();
		return(FALSE);
		}
	return(TRUE);
}
	
BOOL	RequestUserDataClass2(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x5B;
	
	if(fcb[stn_id])
		sendbuf[1] = sendbuf[1] | 0x20;

	//zm sendbuf[2] = stn_id;
	sendbuf[2] = GetStnAddr( stn_id );//zm
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
		printf(">>>>>>>>ReadFile: ClearCommError ERRNO= %d \n",GetLastError());
	printf(">>>>>>>>>>ReadFile ClearCommError dwErrors = %xH cbInQue=%d\n",
			dwErrors,cstat.cbInQue);
	if((dwErrors !=0)||(cstat.cbInQue !=0))
		if(PurgeComm(hPort_com,PURGE_RXCLEAR)==FALSE)
			printf(">>>>>>>>>>>ClearRecvFault: PurgeComm ERRNO= %d \n",GetLastError());	
	timeoutnum[stn_id]++;
	repeatnum[stn_id]++;
}


void ClearSendCmdFault(void)
{
	if(ClearCommError(hPort_com,&dwErrors,&cstat)==FALSE)
		printf("<<<<<<<<<<<WriteFile: ClearCommError ERRNO= %d \n",GetLastError());	
	printf("<<<<<<<<<<<WriteFile ClearCommError dwErrors = %xH cbOutQue=%d\n",
			dwErrors,cstat.cbOutQue);
	if((dwErrors !=0)||(cstat.cbOutQue !=0))
		if(PurgeComm(hPort_com,PURGE_TXCLEAR)==FALSE)
			printf("<<<<<<<<<<<<ClearSendCmdFault: PurgeComm ERRNO= %d \n",GetLastError());	
}

void	PrintSendData(nbsend)
DWORD	nbsend;
{
	DWORD d;

	for(d=1;d<=nbsend;d++)
		{
		printf(" %02X",sendbuf[d-1]);
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
		printf(" %02X",recvbuf[d-1]);
		if((d%10)==0)
			printf("\n");
		}
	printf("\n");
}

