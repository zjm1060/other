/********************************************************
*  FILE NAME:           iec_fun.c      				    *
*  FUNCTION:            Controlling Station			    *
*			Basic Communication Functions		        *
*  's NAME :  			              			        *
*  Version :            Win3.0a-->Win4.0a  by  zwj   	*	     
*  DATE:                2008.11                			*
*********************************************************/

#include "../inc/gis_iec104_com.h"



int	StartDT(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;
	info->sendbuf[1] = 0x04;			/* APDU Length */
	
	info->sendbuf[2] = STARTDT_ACT | U_APCI;	/* 0x07 STARTDT */
	info->sendbuf[3] = 0;
	info->sendbuf[4] = 0;
	info->sendbuf[5] = 0;

	info->byte_to_send = FIXFRAMELEN;
#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: StartDT send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: StartDT send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nStartDT: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	ACKStartDT(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;
	info->sendbuf[1] = 0x04;			/*APDU Length */
	
	info->sendbuf[2] = STARTDT_ACK | U_APCI;	/*0x0B STARTDT */
	info->sendbuf[3] = 0;
	info->sendbuf[4] = 0;
	info->sendbuf[5] = 0;

	info->byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: ACKStartDT send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: ACKStartDT send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nACKStartDT: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	StopDT(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;
	info->sendbuf[1] = 0x04;			/*APDU Length */
	
	info->sendbuf[2] = STOPDT_ACT | U_APCI;	/*0x13 STOPDT */
	info->sendbuf[3] = 0;
	info->sendbuf[4] = 0;
	info->sendbuf[5] = 0;

	info->byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: StopDT send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: StopDT send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nStopDT: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	ACKStopDT(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;
	info->sendbuf[1] = 0x04;			/* APDU Length */
	
	info->sendbuf[2] = STOPDT_ACK | U_APCI;	/* 0x23 STOPDT */
	info->sendbuf[3] = 0;
	info->sendbuf[4] = 0;
	info->sendbuf[5] = 0;

	info->byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: StopDT send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: StopDT send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nStopDT: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	TestFR(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;
	info->sendbuf[1] = 0x04;			/* APDU Length */
	
	info->sendbuf[2] = TESTFR_ACT | U_APCI;	/* 0x43 TESTFR */
	info->sendbuf[3] = 0;
	info->sendbuf[4] = 0;
	info->sendbuf[5] = 0;

	info->byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif
/*	Sleep(10); */

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: TestFR send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: TestFR send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nTestFR: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	ACKTestFR(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;
	info->sendbuf[1] = 0x04;			/* APDU Length */
	
	info->sendbuf[2] = TESTFR_ACK | U_APCI;	/* 0x83 TESTFR */
	info->sendbuf[3] = 0;
	info->sendbuf[4] = 0;
	info->sendbuf[5] = 0;

	info->byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif
/*	Sleep(10); */

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: ACKTestFR send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: ACKTestFR send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nTestFR: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	SFormatFR(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;
	info->sendbuf[1] = 0x04;	/* APDU Length */
	
	info->sendbuf[2] = S_APCI;	/* 0x83 TESTFR */
	info->sendbuf[3] = 0;
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;

	info->byte_to_send = FIXFRAMELEN;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif
/*	Sleep(10); */

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: S format FR send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: S format FR send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\niec104_com: S format FR: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	CommandExec(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	memcpy(&info->sendbuf[6],&info->cmdsavbuf[6],10);	/* copy to info->cmdbuf for exec */

	info->sendbuf[15] = info->sendbuf[15] & 0x3F;

	info->byte_to_send = 16;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: command exec send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: command exec send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\n%u byte command exec transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}
	
int	ClockSynchronise(IEC104_CMM_INFO *info)
{
/*	struct timespec sTimeSpec; */
#ifndef SUN_OS
	time_t	time_tt;
	SYSTEMTIME sys_tm;/**zwj***/
#else
	struct timeval	 sTimeVal;
#endif
	
	struct tm sTimetm;
	USHORT		nMilliSec;
	unsigned char	n;

	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 20;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif

	info->sendbuf[6] = C_CS_NA;			/* 103 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* 6 cause of transmit:actcon */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#endif
	info->sendbuf[12] = 0;			/* information object low address */
	info->sendbuf[13] = 0;			/* information object high address */
	info->sendbuf[14] = 0;			/* information object high address */
/*
	if(getclock(1, &sTimeSpec) == -1)
	{
		OutputDspInfo(0, "\nClockSynchronisation: getclock() error: %s\n",strerror(errno));
		return(false);
	}
*/
#ifdef SUN_OS
	if(gettimeofday(&sTimeVal,NULL) == -1)
	{
		OutputDspInfo(0, "\nClockSynchronisation: gettimeofday() error: %s\n",strerror(errno));
		return(false);
	}
	sTimetm = *localtime(&sTimeVal.tv_sec);
	nMilliSec = sTimetm.tm_sec * 1000 + sTimeVal.tv_usec/1000;
#else
	GetLocalTime(&sys_tm);
	nMilliSec = sys_tm.wMilliseconds;
	time_tt= time(NULL);
	sTimetm = *localtime(&time_tt);
#endif
/*****zwj wait for process**/
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[15],&nMilliSec,2);
#else
	memcpy(&info->sendbuf[15],&nMilliSec,2);			/* Milliseconds */
#endif
	info->sendbuf[17] = (unsigned char)sTimetm.tm_min;		/* Minutes */
	info->sendbuf[18] = (unsigned char)sTimetm.tm_hour;		/* Hour */
	if(sTimetm.tm_wday == 0)
		sTimetm.tm_wday = 7;
	n = sTimetm.tm_wday << 5;
	n = n & 0xE0;
	info->sendbuf[19] = (unsigned char)sTimetm.tm_mday | n;	/* DayofWeek[6..8] day[1..5] */
	info->sendbuf[20] = (unsigned char)sTimetm.tm_mon + 1;	/* Month */
	info->sendbuf[21] = (unsigned char)(sTimetm.tm_year%100);	/* Year */
	
	info->byte_to_send = 22;

	info->send_byte = send(info->sock,(char *)info->sendbuf,info->byte_to_send,(int)0);

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS	
		OutputDspInfo(0, "iec104s_com: GeneralInterrogation send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104s_com: GeneralInterrogation send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nClockSynchronisation: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	GeneralInterrogation(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_IC_NA;			/* type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#endif
	info->sendbuf[12] = 0;			/* information object low address */
	info->sendbuf[13] = 0;			/* information object high address */
	info->sendbuf[14] = 0;			/* information object high address */
	info->sendbuf[15] = INTROGEN;			/* QOI:20 0x14 */			
	
	info->byte_to_send = 16;
#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: GeneralInterrogation send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: GeneralInterrogation send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nGeneralInterrogation: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	GeneralInterrogationGroup(unsigned char gi_qoi,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_IC_NA;			/* 100 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#endif
	info->sendbuf[12] = 0;			/* information object low address */
	info->sendbuf[13] = 0;			/* information object high address */
	info->sendbuf[14] = 0;			/* information object high address */
	info->sendbuf[15] = gi_qoi;			/* QOI:20 0x14 INTROGEN	:34 group 14 */						
	
	info->byte_to_send = 16;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS	
		OutputDspInfo(0, "iec104_com: GeneralInterrogationGroup send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: GeneralInterrogationGroup send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nGeneralInterrogationGroup qoi=%d: %u byte transmitted!",gi_qoi,info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	CounterInterrogation(unsigned char qcc,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_CI_NA;			/* 101 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#endif
	info->sendbuf[12] = 0;			/* information object low address */
	info->sendbuf[13] = 0;			/* information object high address */
	info->sendbuf[14] = 0;			/* information object high address */
	info->sendbuf[15] = qcc;			/* QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]} */
						/* FRZ=1:frezw not reset, FRZ 2:=frezw not reset */
						/* RQT 1:=general counter interrogation */
						/* RQT 2-5 6-63 :=  reserve interrogation */
	info->byte_to_send = 16;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: CounterInterrogation send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: CounterInterrogation send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nCounterInterrogation: qcc=%d %u byte transmitted!",qcc,info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	DoublePointCtrl(char se,USHORT address,char stat,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_DC_NA;			/* 46 CB control type identifier */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit:source address */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&address,2);	/* information address */
#endif

	info->sendbuf[14] = 0;			/* information object high address */

	info->sendbuf[15] = stat;			/* QCC:=CP8{DCS,QU,S/E} */
						/* DCS 1:=OFF 2:=ON */
/*
	if(stat == 0)
		info->sendbuf[12] = 1;
	else
		info->sendbuf[12] = 2;
*/
	if(se)
		info->sendbuf[15] = info->sendbuf[15] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	info->byte_to_send = 16;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: DoublePointCtrl send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: DoublePointCtrl send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
	memcpy(&info->cmdsavbuf[6],&info->sendbuf[6],10);	/* copy to info->cmdbuf for exec */
#ifdef DEBUG
	OutputDspInfo(0, "\nDoublePointCtrl: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	SinglePointCtrl(char se,USHORT address,char stat,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_SC_NA;			/* 45 CB control type identifier */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&address,2);	/* information address */
#endif
	info->sendbuf[14] = 0;			/* information object high address */

	info->sendbuf[15] = stat;			/* QCC:=CP8{SCS,QU,S/E} */
						/* SCS 0:=OFF 1:=ON */
/*
	if(stat == 0)
		info->sendbuf[12] = 0;
	else
		info->sendbuf[12] = 1;
*/
	if(se)
		info->sendbuf[15] = info->sendbuf[15] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	info->byte_to_send = 16;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: SinglePointCtrl send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: SinglePointCtrl send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
	memcpy(&info->cmdsavbuf[6],&info->sendbuf[6],10);	/* copy to info->cmdbuf for exec */
#ifdef DEBUG
	OutputDspInfo(0, "\nSinglePointCtrl: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	RegulatStepCtrl(char se,USHORT address,char stat,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_RC_NA;			/* 47 regulating step control type identifier */
	info->sendbuf[7] = 0x01;			/* variable structure specify */
	info->sendbuf[8] = ACT;			/* 6 cause of transmit:active */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&address,2);	/* information address */
#endif
	info->sendbuf[14] = 0;			/* information object high address */

	info->sendbuf[15] = stat;		/* QCC:=CP8{RCS,QU,S/E} */
					/* RCS 1:=low 2:=raise 3:=stop */
/*
	if(stat == 0)
		info->sendbuf[15] = 1;
	else
		info->sendbuf[15] = 2;	/* QCC:=CP8{RCS,QU,S/E}
					/* RCS 1:=low 2:=raise 3:=stop
*/
	if(se)
		info->sendbuf[15] = info->sendbuf[15] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	info->byte_to_send = 16;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: RegulatStepCtrl send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: RegulatStepCtrl send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
	memcpy(&info->cmdsavbuf[6],&info->sendbuf[6],7);	/* copy to info->cmdbuf for exec */
#ifdef DEBUG
	OutputDspInfo(0, "\nRegulatStepCtrl: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	SetPointCmdNA(char se,USHORT address,USHORT value,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 16;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_SE_NA;			/* 48 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&address,2);	/* information address */
	CrossCopy(&info->sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&address,2);	/* information address */
	memcpy(&info->sendbuf[15],&value,2);		/* Normalized Value */
#endif
	info->sendbuf[14] = 0;			/* information object high address */
	
	info->sendbuf[17] = 0;

	if(se)
		info->sendbuf[17] = info->sendbuf[17] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	info->byte_to_send = 18;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif
	memcpy(&info->cmdsavbuf[6],&info->sendbuf[6],12);	/* copy to info->cmdbuf for exec */

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: Set Point Command NA send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Set Point Command NA send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nSet Point Command NA: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	SetPointCmdNB(char se,USHORT address,USHORT value,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 16;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_SE_NB;			/* 49 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&address,2);	/* information address */
	CrossCopy(&info->sendbuf[15],&value,2);	/* Scaled Value */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&address,2);	/* information address */
	memcpy(&info->sendbuf[15],&value,2);		/* Scaled Value */
#endif
	info->sendbuf[14] = 0;			/* information object high address */
	
	info->sendbuf[17] = 0;

	if(se)
		info->sendbuf[17] = info->sendbuf[17] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	info->byte_to_send = 18;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif
	memcpy(&info->cmdsavbuf[6],&info->sendbuf[6],12);	/* copy to info->cmdbuf for exec */

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: Set Point Command NB send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Set Point Command NB send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nSet Point Command NB: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	SetPointCmdNC(char se,USHORT address,float value,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 18;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_SE_NC;			/* 66 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&address,2);	/* information address */
	CrossCopy(&info->sendbuf[15],&value,4);	/* float Value */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&address,2);	/* information address */
	memcpy(&info->sendbuf[15],&value,4);		/* float Value */
#endif
	info->sendbuf[14] = 0;			/* information object high address */
	
	info->sendbuf[19] = 0;

	if(se)
		info->sendbuf[19] = info->sendbuf[19] | 0x80;	/* S/E := 0:=Execute 1:= Select */

	info->byte_to_send = 20;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif
	memcpy(&info->cmdsavbuf[6],&info->sendbuf[6],11);	/* copy to info->cmdbuf for exec */

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: Set Point Command NC send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Set Point Command NC send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nSet Point Command NC: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	TestCmd(IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 15;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_TS_NA;			/* 104 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
/* 	info->sendbuf[8] = ACTCON | 0x80;	*/	/* 7 Actcon cause of transmit: T=1 P/N=0 */
	info->sendbuf[8] = ACT;			/* 6 Act cause of transmit: T=1 P/N=0 */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#endif

	info->sendbuf[12] = 0;			/* information address */
	info->sendbuf[13] = 0;			/* information object high address */
	info->sendbuf[14] = 0;			/* information object high address */
	info->sendbuf[15] = 0xAA;			/* test low byte AA */
	info->sendbuf[16] = 0x55;			/* test high byte 55 */

	info->byte_to_send = 17;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: Test channel send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Test channel send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nTest channel: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	ResetProcessAck(unsigned char cot,unsigned char qrp,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 14;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_RP_NA;			/* 106 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = cot;			/* 7 Actcon cause of transmit: T=1 P/N=0 */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
#endif

	info->sendbuf[12] = 0;			/* information address */
	info->sendbuf[13] = 0;			/* information object high address */
	info->sendbuf[14] = 0;			/* information object high address */
	info->sendbuf[15] = qrp;			/* QRP[1..8] 1:reset process 2:reset soe buf */

	info->byte_to_send = 16;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: Reset Process send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Reset Process send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nReset Process: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	ReadCmd(unsigned char qoc,unsigned short infoaddr,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 13;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_RD_NA;			/* 102 type identifier */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = qoc;			/* cause of transmit:active */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&infoaddr,2);	/* information address */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&infoaddr,2);	/* information address */
#endif
	info->sendbuf[14] = 0;			/* information object high address */

	info->byte_to_send = 15;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: Read C_RD_NA send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Read C_RD_NA send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nRead C_RD_NA: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	ParamSetCmdNA(char kpa,USHORT address,USHORT value,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 16;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = P_ME_NA;			/* 110 parameter for measurements */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = ACT;			/* cause of transmit:active:6 */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[12],&address,2);	/* information address */
	CrossCopy(&info->sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[12],&address,2);	/* information address */
	memcpy(&info->sendbuf[15],&value,2);		/* Normalized Value */
#endif
	info->sendbuf[14] = 0;			/* information object high address */
	

	info->sendbuf[17] = kpa;			/* KPA[1-6] LPC POP */
						/* KPA 1:= threshold 2:= filter factor */
						/* 3:= low limit  4:= high limit */


	info->byte_to_send = 18;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS	
		OutputDspInfo(0, "iec104_com: Parameter Set Command NA send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Parameter Set Command NA send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nParameter Set Command NA: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

int	TimeDelayCmd(unsigned char cot,unsigned short cp16time,IEC104_CMM_INFO *info)
{
	info->sendbuf[0] = 0x68;			/* start flag */
	info->sendbuf[1] = 15;			/* L */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[2],&info->nr_send,2);	/* V(S) */
	CrossCopy(&info->sendbuf[4],&info->nr_recv,2);	/* V(R) */
#else
	memcpy(&info->sendbuf[2],&info->nr_send,2);		/* V(S) */
	memcpy(&info->sendbuf[4],&info->nr_recv,2);		/* V(R) */
#endif
	info->ns_ack = info->nr_recv;
	info->nr_send +=2;

	info->sendbuf[6] = C_CD_NA;			/* 106 type identifier field */
	info->sendbuf[7] = 1;				/* variable structure specify */
	info->sendbuf[8] = cot;			/* 6 cause of transmit:act 3: spont */
	info->sendbuf[9] = info->nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	CrossCopy(&info->sendbuf[15],&cp16time,2);	/* Milliseconds to second */
#else
	memcpy(&info->sendbuf[10],&info->nASDUId,2);	/* ASDU common address */
	memcpy(&info->sendbuf[15],&cp16time,2);	/* Milliseconds to second */
#endif

	info->sendbuf[12] = 0;			/* information object low address */
	info->sendbuf[13] = 0;			/* information object high address */
	info->sendbuf[14] = 0;			/* information object high address */

	
	info->byte_to_send = 17;

#ifdef SUN_OS
	info->send_byte = write(info->sock,info->sendbuf,info->byte_to_send);
#else
	info->send_byte = send(info->sock,info->sendbuf,info->byte_to_send,(int)0);
#endif

	if(info->send_byte != info->byte_to_send)
	{
#ifdef SUN_OS
		OutputDspInfo(0, "iec104_com: Time Delay send() Error %d! do close info->socket!\n",errno);
#else
		OutputDspInfo(0, "iec104_com: Time Delay send() Error %d! do close info->socket!\n",GetLastError());
#endif
		close(info->sock);
		return(false);
	}
#ifdef DEBUG
	OutputDspInfo(0, "\nTime Delay: %u byte transmitted!",info->send_byte);
	PrintSendData(info->sendbuf, info->send_byte);
#endif
	return(true);
}

