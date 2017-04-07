
#include "/home/ems/system/iec/v4.0a/inc/iec104_com.h"

extern	unsigned char	sendbuf[1024];
extern	unsigned char	recvbuf[1024];
extern	unsigned char	cmdsavbuf[36];
extern  char	infobuf[1024];
extern	ASDUCOMDDATA	send_data;
extern	ASDUDATA	recv_data;
extern  unsigned short	nASDUId;
extern	unsigned char	nSrcId;
extern	unsigned short	nr_recv,nr_send,nr_ack,ns_ack,nr_rs;

extern	SPDEF		spdef;
extern	DPDEF		dpdef;
extern	MEDEF		medef;
extern	DCDEF		dcdef;
extern	SEDEF		sedef;
extern	ITDEF		itdef;
extern	SCDEF		scdef;
extern	RCDEF		rcdef;
extern	CMDBUF		cmdbuf;
extern	SEBUF		sebuf;

unsigned int	byte_to_send;	
unsigned int	send_byte;
extern	int	selectcmd;
extern char logfilename[128];
extern struct tm  *ntime;
extern int oldday;

int	StartDT(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;			/* APDU Length */
	
	sendbuf[2] = STARTDT_ACT | U_APCI;	/* 0x07 STARTDT */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;
#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: StartDT send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: StartDT send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nStartDT: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nStartDT: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	ACKStartDT(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;			/*APDU Length */
	
	sendbuf[2] = STARTDT_ACK | U_APCI;	/*0x0B STARTDT */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: ACKStartDT send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: ACKStartDT send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nACKStartDT: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nACKStartDT: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	StopDT(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;			/*APDU Length */
	
	sendbuf[2] = STOPDT_ACT | U_APCI;	/*0x13 STOPDT */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: StopDT send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: StopDT send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nStopDT: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nStopDT: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	ACKStopDT(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;			/* APDU Length */
	
	sendbuf[2] = STOPDT_ACK | U_APCI;	/* 0x23 STOPDT */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: StopDT send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: StopDT send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nStopDT: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nStopDT: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	TestFR(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;			/* APDU Length */
	
	sendbuf[2] = TESTFR_ACT | U_APCI;	/* 0x43 TESTFR */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif
/*	Sleep(10); */

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: TestFR send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: TestFR send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nTestFR: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nTestFR: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	ACKTestFR(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;			/* APDU Length */
	
	sendbuf[2] = TESTFR_ACK | U_APCI;	/* 0x83 TESTFR */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif
/*	Sleep(10); */

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: ACKTestFR send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: ACKTestFR send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nTestFR: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nTestFR: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	SFormatFR(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;	/* APDU Length */
	
	sendbuf[2] = S_APCI;	/* 0x83 TESTFR */
	sendbuf[3] = 0;
#ifdef SUN
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;

	byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif
/*	Sleep(10); */

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: S format FR send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: S format FR send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\niec104_com: S format FR: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\niec104_com: S format FR: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	CommandExec(int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	memcpy(&sendbuf[6],&cmdsavbuf[6],10);	/* copy to cmdbuf for exec */

	sendbuf[15] = sendbuf[15] & 0x3F;

	byte_to_send = 16;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: command exec send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: command exec send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\n%u byte command exec transmitted!",send_byte);
	sprintf(infobuf,"\n%u byte command exec transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}
	
int	ClockSynchronise(int sock)
{
/*	struct timespec sTimeSpec; */
#ifndef SUN_OS
	SYSTEMTIME sys_tm;/**zwj***/
#else
	struct timeval	 sTimeVal;
#endif
	
	struct tm sTimetm;
	USHORT		nMilliSec;
	unsigned char	n;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 20;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = C_CS_NA;			/* 103 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* 6 cause of transmit:actcon */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
/*
	if(getclock(1, &sTimeSpec) == -1)
	{
		printf("\nClockSynchronisation: getclock() error: %s\n",strerror(errno));
		return(false);
	}
*/
#ifdef SUN_OS
	if(gettimeofday(&sTimeVal,NULL) == -1)
	{
		printf("\nClockSynchronisation: gettimeofday() error: %s\n",strerror(errno));
		return(false);
	}
	sTimetm = *localtime(&sTimeVal.tv_sec);
	nMilliSec = sTimetm.tm_sec * 1000 + sTimeVal.tv_usec/1000;
#else
	GetLocalTime(&sys_tm);
	nMilliSec = sys_tm.wMilliseconds;
#endif
/*****zwj wait for process**/
	ns_ack = nr_recv;
	nr_send +=2;
#ifdef SUN
	CrossCopy(&sendbuf[15],&nMilliSec,2);
#else
	memcpy(&sendbuf[15],&nMilliSec,2);			/* Milliseconds */
#endif
	sendbuf[17] = (unsigned char)sTimetm.tm_min;		/* Minutes */
	sendbuf[18] = (unsigned char)sTimetm.tm_hour;		/* Hour */
	if(sTimetm.tm_wday == 0)
		sTimetm.tm_wday = 7;
	n = sTimetm.tm_wday << 5;
	n = n & 0xE0;
	sendbuf[19] = (unsigned char)sTimetm.tm_mday | n;	/* DayofWeek[6..8] day[1..5] */
	sendbuf[20] = (unsigned char)sTimetm.tm_mon + 1;	/* Month */
	sendbuf[21] = (unsigned char)(sTimetm.tm_year%100);	/* Year */
	
	byte_to_send = 22;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS	
		printf("iec104s_com: GeneralInterrogation send() Error %d! do close socket!\n",errno);
#else
		printf("iec104s_com: GeneralInterrogation send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nClockSynchronisation: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nClockSynchronisation: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	GeneralInterrogation(int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_IC_NA;			/* type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = INTROGEN;			/* QOI:20 0x14 */			
	
	byte_to_send = 16;
#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: GeneralInterrogation send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: GeneralInterrogation send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nGeneralInterrogation: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nGeneralInterrogation: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	GeneralInterrogationGroup(unsigned char gi_qoi,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_IC_NA;			/* 100 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = gi_qoi;			/* QOI:20 0x14 INTROGEN	:34 group 14 */						
	
	byte_to_send = 16;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS	
		printf("iec104_com: GeneralInterrogationGroup send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: GeneralInterrogationGroup send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nGeneralInterrogationGroup qoi=%d: %u byte transmitted!",gi_qoi,send_byte);
	sprintf(infobuf,"\nGeneralInterrogationGroup qoi=%d: %u byte transmitted!",gi_qoi,send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	CounterInterrogation(unsigned char qcc,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_CI_NA;			/* 101 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = qcc;			/* QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]} */
						/* FRZ=1:frezw not reset, FRZ 2:=frezw not reset */
						/* RQT 1:=general counter interrogation */
						/* RQT 2-5 6-63 :=  reserve interrogation */
	byte_to_send = 16;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: CounterInterrogation send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: CounterInterrogation send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nCounterInterrogation: qcc=%d %u byte transmitted!",qcc,send_byte);
	sprintf(infobuf,"\nCounterInterrogation: qcc=%d %u byte transmitted!",qcc,send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	DoublePointCtrl(char se,USHORT address,char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_DC_NA;			/* 46 CB control type identifier */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit:source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif

	sendbuf[14] = 0;			/* information object high address */

	sendbuf[15] = stat;			/* QCC:=CP8{DCS,QU,S/E} */
						/* DCS 1:=OFF 2:=ON */
/*
	if(stat == 0)
		sendbuf[12] = 1;
	else
		sendbuf[12] = 2;
*/
	if(se)
		sendbuf[15] = sendbuf[15] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	byte_to_send = 16;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: DoublePointCtrl send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: DoublePointCtrl send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
	memcpy(&cmdsavbuf[6],&sendbuf[6],10);	/* copy to cmdbuf for exec */
#ifdef DEBUG
	printf("\nDoublePointCtrl: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nDoublePointCtrl: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	SinglePointCtrl(char se,USHORT address,char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_DC_NA;			/* 45 CB control type identifier */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

	sendbuf[15] = stat;			/* QCC:=CP8{SCS,QU,S/E} */
						/* SCS 0:=OFF 1:=ON */
/*
	if(stat == 0)
		sendbuf[12] = 0;
	else
		sendbuf[12] = 1;
*/
	if(se)
		sendbuf[15] = sendbuf[15] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	byte_to_send = 16;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: SinglePointCtrl send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: SinglePointCtrl send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
	memcpy(&cmdsavbuf[6],&sendbuf[6],10);	/* copy to cmdbuf for exec */
#ifdef DEBUG
	printf("\nSinglePointCtrl: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nSinglePointCtrl: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	RegulatStepCtrl(char se,USHORT address,char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_RC_NA;			/* 47 regulating step control type identifier */
	sendbuf[7] = 0x01;			/* variable structure specify */
	sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

	sendbuf[15] = stat;		/* QCC:=CP8{RCS,QU,S/E} */
					/* RCS 1:=low 2:=raise 3:=stop */
/*
	if(stat == 0)
		sendbuf[15] = 1;
	else
		sendbuf[15] = 2;	/* QCC:=CP8{RCS,QU,S/E}
					/* RCS 1:=low 2:=raise 3:=stop
*/
	if(se)
		sendbuf[15] = sendbuf[15] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	byte_to_send = 16;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: RegulatStepCtrl send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: RegulatStepCtrl send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
	memcpy(&cmdsavbuf[6],&sendbuf[6],7);	/* copy to cmdbuf for exec */
#ifdef DEBUG
	printf("\nRegulatStepCtrl: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nRegulatStepCtrl: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	SetPointCmdNA(char se,USHORT address,USHORT value,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_SE_NA;			/* 48 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
	sendbuf[17] = 0;

	if(se)
		sendbuf[17] = sendbuf[17] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	byte_to_send = 18;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif
	memcpy(&cmdsavbuf[6],&sendbuf[6],12);	/* copy to cmdbuf for exec */

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: Set Point Command NA send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Set Point Command NA send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSet Point Command NA: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nSet Point Command NA: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	SetPointCmdNB(char se,USHORT address,USHORT value,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_SE_NB;			/* 49 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Scaled Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Scaled Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
	sendbuf[17] = 0;

	if(se)
		sendbuf[17] = sendbuf[17] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	byte_to_send = 18;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif
	memcpy(&cmdsavbuf[6],&sendbuf[6],12);	/* copy to cmdbuf for exec */

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: Set Point Command NB send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Set Point Command NB send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSet Point Command NB: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nSet Point Command NB: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	SetPointCmdNC(char se,USHORT address,float value,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 18;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_SE_NC;			/* 66 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,4);	/* float Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,4);		/* float Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
	sendbuf[19] = 0;

	if(se)
		sendbuf[19] = sendbuf[19] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	byte_to_send = 20;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif
	memcpy(&cmdsavbuf[6],&sendbuf[6],11);	/* copy to cmdbuf for exec */

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: Set Point Command NC send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Set Point Command NC send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSet Point Command NC: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nSet Point Command NC: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	TestCmd(int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 15;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_TS_NA;			/* 104 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
/* 	sendbuf[8] = ACTCON | 0x80;	*/	/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[8] = ACT;			/* 6 Act cause of transmit: T=1 P/N=0 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	sendbuf[12] = 0;			/* information address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = 0xAA;			/* test low byte AA */
	sendbuf[16] = 0x55;			/* test high byte 55 */

	byte_to_send = 17;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: Test channel send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Test channel send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nTest channel: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nTest channel: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	ResetProcessAck(unsigned char cot,unsigned char qrp,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_RP_NA;			/* 106 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cot;			/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	sendbuf[12] = 0;			/* information address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = qrp;			/* QRP[1..8] 1:reset process 2:reset soe buf */

	byte_to_send = 16;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: Reset Process send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Reset Process send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nReset Process: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nReset Process: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	ReadCmd(unsigned char qoc,unsigned short infoaddr,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 13;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_RD_NA;			/* 102 type identifier */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = qoc;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&infoaddr,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&infoaddr,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

	byte_to_send = 15;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: Read C_RD_NA send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Read C_RD_NA send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead C_RD_NA: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nRead C_RD_NA: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	ParamSetCmdNA(char kpa,USHORT address,USHORT value,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = P_ME_NA;			/* 110 parameter for measurements */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	

	sendbuf[17] = kpa;			/* KPA[1-6] LPC POP */
						/* KPA 1:= threshold 2:= filter factor */
						/* 3:= low limit  4:= high limit */


	byte_to_send = 18;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS	
		printf("iec104_com: Parameter Set Command NA send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Parameter Set Command NA send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nParameter Set Command NA: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nParameter Set Command NA: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	TimeDelayCmd(unsigned char cot,unsigned short cp16time,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 15;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif
	ns_ack = nr_recv;
	nr_send +=2;

	sendbuf[6] = C_CD_NA;			/* 106 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cot;			/* 6 cause of transmit:act 3: spont */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[15],&cp16time,2);	/* Milliseconds to second */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[15],&cp16time,2);	/* Milliseconds to second */
#endif

	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */

	
	byte_to_send = 17;

#ifdef SUN_OS
	send_byte = write(sock,sendbuf,byte_to_send);
#else
	send_byte = send(sock,sendbuf,byte_to_send,(int)0);
#endif

	if(send_byte != byte_to_send)
	{
#ifdef SUN_OS
		printf("iec104_com: Time Delay send() Error %d! do close socket!\n",errno);
#else
		printf("iec104_com: Time Delay send() Error %d! do close socket!\n",GetLastError());
#endif
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nTime Delay: %u byte transmitted!",send_byte);
	sprintf(infobuf,"\nTime Delay: %u byte transmitted!",send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
	PrintSendData(send_byte);
	WriteDspInfo(infobuf);
	WriteLogFile(infobuf);
#endif
	return(true);
}

int	CommandSend(unsigned int comdpnt,int sock)
{
unsigned short	ctrladdr,stat,setdata,inf_addr,i,sd_pnt;
unsigned char	station_ctrl,stn_id,dev_id;
char name[36];
float			setvalue;

	sd_pnt = send_data.pnt;
	comdpnt = sd_pnt;
/*
	if((comdpnt%4)==0)
/* 		ParamSetCmdNA(0x81,PE_BASE_ADDR+1,0x800F,sock);
/* 		TestCmd(sock);
/* 		ReadCmd(REQ,ME_BASE_ADDR+1,sock);	/* infoaddr
		ReadCmd(REQ,33,sock);	/* infoaddr
	else if((comdpnt%4)==1)
		GeneralInterrogationGroup(34);	/* qoi=34 :14 group
/* 		SetPointCmdNA(0,SE_CMD_BASE_ADDR+1,4321,sock);	/* se=1
/* 		TimeDelayCmd(ACT,1234,sock);	/* cot,cp16time
/* 		ResetProcessAck(6,2,sock);
	else if((comdpnt%4)==2)
/* 		TimeDelayCmd(SPONT,1234,sock);	/* cot,cp16time
/* 		DoublePointCtrl(1,DC_CMD_BASE_ADDR+1,1,sock);
		RegulatStepCtrl(1,RC_CMD_BASE_ADDR+1,2,sock);
/* 		ResetProcessAck(6,2,sock);
/* 		CounterInterrogation(0x45,sock);		/* 0x45 FRZ; 0x05 Read
/* 		CounterInterrogation(0x45,sock);		/* 0x45 FRZ; 0x05 Read
	else
		GeneralInterrogationGroup(21,sock);	/* qoi=34 :14 group
/* 		CounterInterrogation(0x05,sock);		/* 0x45 FRZ; 0x05 Read
	/* ReadCmd(6,ME_BASE_ADDR+1,sock);	/* infoaddr
/* ResetProcessAck(6,1,sock);		/* qrp 1:reset rtu  2: soe
/* TestCmd(sock);
/* TimeDelayCmd(ACT,1234,sock);	/* cot,cp16time
*/
	sd_pnt ++;
	if(sd_pnt>=CMD_ENTRY_NUMBER)
		sd_pnt = 0;
	send_data.pnt = sd_pnt;
	selectcmd=0;
	selectcmd=1;

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
			DoublePointCtrl(1,inf_addr,2,sock);
		}
		else
		{
#ifdef DEBUG
			printf("++++++++%d station INF=%d CB open\n",station_ctrl,inf_addr);
#endif
			DoublePointCtrl(1,inf_addr,1,sock);
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
			RegulatStepCtrl(1,inf_addr,2,sock);
		else if((stat & 0x00F0) == 0x20 )
			RegulatStepCtrl(1,inf_addr,1,sock);
		else if((stat & 0x00F0) == 0x30 )
			RegulatStepCtrl(1,inf_addr,3,sock);

		break;
	case C_APOWER:
	case C_RPOWER:
/* 		send_data.data[sd_pnt][0] = C_SE_NA; */	/* set-point command */
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

		for(i=0;i<sedef.number;i++)
		{
			if(strcmp(name,sedef.name[i])==0)
				break;
		}
		if(i>=sedef.number)
		{
			printf("No this point %s in set point table!\n",name);
			break;;
		}
		setvalue = Datcvt(stat,setdata);
		setdata=(unsigned short)((setvalue-sedef.lorange[i])*RESOLUTE/
				(sedef.hirange[i]-sedef.lorange[i]) + 0.5);

		inf_addr = SE_CMD_BASE_ADDR + i;
		SetPointCmdNA(0,inf_addr,setdata,sock);
		break;

	default :
#ifdef DEBUG
		printf("No this command type %d\n",send_data.data[comdpnt][0]);
#endif
		break;			
	}/* switch(send_data.data[comdpnt][0]) */
	return(true);
/*
	switch	( send_data.data[comdpnt][0] ) 
		{
		case  0:
			DoublePointCtrl(0,SC_CMD_BASE_ADDR,0,5,sock);
			break;

		case  1:
			DoublePointCtrl(0,SC_CMD_BASE_ADDR,1,5,sock);
			break;

		case  2:
			RegulatStepCtrl(0,RC_CMD_BASE_ADDR,1,sock);
			break;

		case  3:
			RegulatStepCtrl(0,RC_CMD_BASE_ADDR,2,sock);
			break;

			default :
#ifdef DEBUG
			printf("No this command type %d\n",send_data.data[comdpnt][0]);
#endif
			break;			
		}/* switch(send_data.data[comdpnt][0])
	return(true);
*/
}
	
void	PrintSendData(unsigned int nbsend)
{
	unsigned int d;
	char tmp[12];
	for(d=0;d<nbsend;d++)
	{
		if((d%20)==0)
		{
			printf("\n<< ");
			strcpy(tmp,"\n<< ");
			strcat(infobuf,tmp);
		}
		else if((d%10)==0)
		{
			printf(" ");
			strcpy(tmp," ");
			strcat(infobuf,tmp);
		}
		printf(" %02X",sendbuf[d]);
		strcpy(tmp,"");
		sprintf(tmp," %02X",sendbuf[d]);
		strcat(infobuf,tmp);
	}
	strcpy(tmp,"");
	printf("\n");
	strcpy(tmp,"\n");
	strcat(infobuf,tmp);
}

void	PrintRecvData(unsigned int nbrecv)
{
	unsigned int	d;
	char tmp[12];
    strcpy(infobuf,"");
	for(d=0;d<nbrecv;d++)
	{
		if((d%20)==0)
		{
			printf("\n>> ");
			strcpy(tmp,"\n>> ");
			strcat(infobuf,tmp);
		}
		else if((d%10)==0)
		{
			printf(" ");
			strcpy(tmp," ");
			strcat(infobuf,tmp);
		}		
		printf(" %02X",recvbuf[d]);
		strcpy(tmp,"");
		sprintf(tmp," %02X",recvbuf[d]);
		strcat(infobuf,tmp);
	}
	strcpy(tmp,"");
	printf("\n");
	strcpy(tmp,"\n");
	strcat(infobuf,tmp);
}

