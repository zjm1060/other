/****************************************************************
*  FILE NAME:           iec104s_fun.c      			*
*  FUNCTION:            Controlled Station			*
*			Basic Communication Function		*
*  NAME :  			              			*
*  DATE:                April,2006                		*
*								*
****************************************************************/

#include "../inc/iec104s_com.h"
#include <errno.h>
#define	DEBUG

extern	unsigned char	sendbuf[1024];
extern	unsigned char	recvbuf[1024];
extern	char	infobuf[1024];

extern	unsigned short	nr_recv,nr_send,nr_ack,ns_ack;
extern	SOEBUF	soebuf;
extern	CMDBUF	cmdbuf;
extern	SEBUF	sebuf;
extern	PMBUF	pmbuf;
extern	ICBUF	icbuf;
extern	CIBUF	cibuf;
extern	RDBUF	rdbuf;
extern	RPBUF	rpbuf;
extern	TESTBUF	testbuf;
extern	CDBUF	cdbuf;
extern	MIRBUF	mirbuf;

extern	unsigned short	sp_base_addr;
extern	unsigned short	dp_base_addr;
extern	unsigned short	me_base_addr;
extern	unsigned short	pm_base_addr;
extern	unsigned short	it_base_addr;
extern	unsigned short	st_base_addr;
extern	unsigned short	bo_base_addr;

extern	unsigned short	sc_cmd_base_addr;
extern	unsigned short	dc_cmd_base_addr;
extern	unsigned short	rc_cmd_base_addr;
extern	unsigned short	se_cmd_base_addr;
extern	unsigned short	bc_cmd_base_addr;

extern	unsigned short	nASDUId;
extern	unsigned char	nSrcId;
extern	ASDUDATA		send_data;
extern	ASDUCOMDDATA	recv_data;
extern	int	ClockSyncflag;
extern	int	Readflag,Testflag,ResetProcessflag;
extern	int	Paramflag,MirrorNAckflag;
extern	int	interrogationflag,counterflag,icgroupflag,cigroupflag;
extern	int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;

extern	int	scanflag;
extern	unsigned short	delayalarm;
extern	unsigned char	cot_interrogation,cot_counter;
extern	unsigned short	seladdr;

extern	time_t	scan_tim;

extern	SPDEF	spdef;
extern	DPDEF	dpdef;
extern	MEDEF	medef;

extern	STDEF	stdef;
extern	ITDEF	itdef;
/*
extern	SCDEF	scdef;
extern	DCDEF	dcdef;
extern	SEDEF	sedef;
extern	RCDEF	rcdef;
*/
extern	int	GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
extern	int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 2: M_ME_ND; 3: M_ME_NC */
extern	int	MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int	SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	short	me_code;	/* 0 or -32768 */
extern	short	se_code;	/* 0 or -32768 */
/* extern	int	TimeTag;	/* Time tag> 0: no; 1: short; 2: long */
extern	int	sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
extern	int	Cmd_Sel;	/*  0: cmd without select; 1: cmd with select */

int	NoChangeData;
int		byte_to_send;	
unsigned short	send_byte;
char	send_ic_data_type,send_ci_data_type,class1_data_type,class2_data_type;
unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
unsigned short	m_cRespParamNum,m_cRespICGrpNum,m_cRespCIGrpNum;
unsigned char	count_sq,ITGrpType;

short	organabuf[MAX_ME_NUM];
float	organafbuf[MAX_ME_NUM];
unsigned char	organastbuf[MAX_ME_NUM];

unsigned char	orgspbuf[MAX_SP_NUM],orgdpbuf[MAX_DP_NUM];
unsigned char	orgstbuf[MAX_ST_NUM],orgstvbuf[MAX_ST_NUM];


void FillCP56Time2aField(unsigned char *buf)
{
#ifdef WINDOWS_OS
	SYSTEMTIME	sys_tm;
	USHORT		nMilliSec;
	unsigned char n;
	
	GetLocalTime(&sys_tm);
	nMilliSec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;
	memcpy(&buf[0],&nMilliSec,2);	/*Milliseconds */
	Swap2Byte(&buf[0]);
	buf[2] = (unsigned char)sys_tm.wMinute;/*Minutes */
	buf[3] = (unsigned char)sys_tm.wHour;	/*Hour */
	n = sys_tm.wDayOfWeek << 5;
	n = n & 0xE0;
	buf[4] = (unsigned char)sys_tm.wDay | n;	/*DayofWeek[6..8] day[1..5] */
	buf[5] = (unsigned char)sys_tm.wMonth;		/*Month */
	buf[6] = (unsigned char)(sys_tm.wYear%100);	/*Year */
#else
	struct timeval	 sTimeVal;
	struct tm sTimetm;
	USHORT		nMilliSec;
	unsigned char	n;

	gettimeofday(&sTimeVal,NULL);
	sTimetm = *localtime(&sTimeVal.tv_sec);
	nMilliSec = sTimetm.tm_sec * 1000 + sTimeVal.tv_usec/1000;
	memcpy(&buf[0],&nMilliSec,2);			/* Milliseconds */
	Swap2Byte(&buf[0]);
	buf[2] = (unsigned char)sTimetm.tm_min;		/* Minutes */
	buf[3] = (unsigned char)sTimetm.tm_hour;		/* Hour */
	if(sTimetm.tm_wday == 0)
		sTimetm.tm_wday = 7;
	n = sTimetm.tm_wday << 5;
	n = n & 0xE0;
	buf[4] = (unsigned char)sTimetm.tm_mday | n;	/* DayofWeek[6..8] day[1..5] */
	buf[5] = (unsigned char)sTimetm.tm_mon + 1;		/* Month */
	buf[6] = (unsigned char)(sTimetm.tm_year%100);	/* Year */

#endif
}

void UpdateSN(void)
{
	extern time_t tim_waitack[];

	ns_ack = nr_recv;
	tim_waitack[nr_send]=time(NULL);	
	nr_send +=2;
}

void FillControlField(unsigned char *ctrl_field)
{
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
	Swap2Byte(&sendbuf[2]);
	Swap2Byte(&sendbuf[4]);	
}



int	RespondStartDT(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;	/* APDU Length */
	
	sendbuf[2] = 0x0B;	/* ACK STARTDT */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);
/* 	Sleep(10); */

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "RespondStartDT send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRespondStartDT: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	return(true);
}

int	RespondStopDT(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;	/* APDU Length */
	
	sendbuf[2] = 0x23;	/* ACK STOPDT */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);
/* 	Sleep(10); */

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "RespondStopDT send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRespondStopDT: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
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

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "TestFR send() Error %d  %d %d! do close socket!\n",errno,ENOSPC,EBADF);
		return(false);
	}
	OutputDspInfo(0, "\nTestFR: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	return(true);
}

int	RespondTestFR(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;	/* APDU Length */
	
	sendbuf[2] = 0x83;	/* ACK TESTFR */
	sendbuf[3] = 0;
	sendbuf[4] = 0;
	sendbuf[5] = 0;

	byte_to_send = FIXFRAMELEN;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);
/* 	Sleep(10); */

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "RespondTestFR send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRespondTestFR: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	return(true);
}

int	SFormatFR(int sock)
{
	sendbuf[0] = 0x68;
	sendbuf[1] = 0x04;	/* APDU Length */
	
	sendbuf[2] = S_APCI;	/* 0x83 TESTFR */
	sendbuf[3] = 0;

	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
	Swap2Byte(&sendbuf[4]);

	byte_to_send = FIXFRAMELEN;
/*********change by zyf ***********/
	//send_byte = write(sock,sendbuf,byte_to_send);
	  send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);
/*********end change by zyf ***********/
/*	Sleep(10); */
	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "S format FR send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "S format FR: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	return(true);
}

int	CmdRespSend(int sock)
{
	unsigned char	type_id;

/*	//control field
	sendbuf[4] = 0x00;
	if(acd || cmdbuf.ack_flag)
		sendbuf[4] = sendbuf[4] | 0x20;
*/
	type_id = cmdbuf.typeId;
	if(cmdbuf.cot==ACTTERM)
	{
		cmdbuf.ack_flag = 0;
	}
	NoChangeData = 0;
	switch(type_id)
	{
	case C_SC_NA:

		SinglePointCtrlAck(cmdbuf.cot,cmdbuf.infoaddr,cmdbuf.stat,sock);
		if(cmdbuf.term_flag ==1)
		{
			cmdbuf.term_flag = 0;
			if(cmdbuf.cot==ACTCON)
			{
				cmdbuf.cot = ACTTERM;		/* COT:10  activation terminal */
				return(true);
			}
		}
		cmdbuf.ack_flag = 0;
		break;

	case C_DC_NA:

		DoublePointCtrlAck(cmdbuf.cot,cmdbuf.infoaddr,cmdbuf.stat,sock);
		if(cmdbuf.term_flag ==1)
		{
			cmdbuf.term_flag = 0;
			if(cmdbuf.cot==ACTCON)
			{
				cmdbuf.cot = ACTTERM;		/* COT:10  activation terminal */
				return(true);
			}
		}
		cmdbuf.ack_flag = 0;
		break;

	case C_RC_NA:

		RegulatStepCtrlAck(cmdbuf.cot,cmdbuf.infoaddr,cmdbuf.stat,sock);
		if(cmdbuf.term_flag ==1)
		{
			cmdbuf.term_flag = 0;
			if(cmdbuf.cot==ACTCON)
			{
				cmdbuf.cot = ACTTERM;		/* COT:10  activation terminal */
				return(true);
			}
		}
		cmdbuf.ack_flag = 0;
		break;

	default:
		OutputDspInfo(OUTPUT_WARN, "unknown cmd type id %d\n",type_id);
		NoChangeData = 1;
		cmdbuf.ack_flag = 0;
		break;
	}

	return(true);
}

int	SERespSend(int sock)
{
	unsigned char	type_id;

	if(sebuf.cot==ACTTERM)
		sebuf.ack_flag = 0;
	
	NoChangeData = 0;

	type_id = sebuf.typeId;
	switch(type_id)
	{
	case C_SE_NA:

		SetPointCmdNAAck(sebuf.cot,sebuf.infoaddr,sebuf.setvalue,sebuf.qoc,sock);
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			if(sebuf.cot==ACTCON)
			{
				sebuf.cot = ACTTERM;		/*COT:10  activation terminal */
				return(0);
			}
			break;
		}
		sebuf.ack_flag = 0;
		break;

	case C_SE_NB:

		SetPointCmdNBAck(sebuf.cot,sebuf.infoaddr,sebuf.setvalue,sebuf.qoc,sock);
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			if(sebuf.cot==ACTCON)
			{
				sebuf.cot = ACTTERM;		/*COT:10  activation terminal */
				return(0);
			}
			break;
		}
		sebuf.ack_flag = 0;
		break;

	case C_SE_NC:

		SetPointCmdNCAck(sebuf.cot,sebuf.infoaddr,sebuf.fvalue,sebuf.qoc,sock);
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			if(sebuf.cot==ACTCON)
			{
				sebuf.cot = ACTTERM;		/*COT:10  activation terminal */
				return(0);
			}
			break;
		}
		sebuf.ack_flag = 0;
		break;

	case C_SE_ND:

		SetPointCmdNDAck(sebuf.cot,sebuf.infonum,sebuf.rcvpnt,sebuf.qoc,sock);
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			if(sebuf.cot==ACTCON)
			{
				sebuf.cot = ACTTERM;		/*COT:10  activation terminal */
				return(0);
			}
			break;
		}
		sebuf.ack_flag = 0;
		break;

	default:
		OutputDspInfo(OUTPUT_WARN, "unknown setpoint type id %d\n",type_id);
		NoChangeData = 1;
		sebuf.ack_flag = 0;
		break;
	}
	return(true);
}

int	PMRespSend(int sock)
{
	unsigned char	type_id;
	int		ret;

	NoChangeData = 0;
	pmbuf.ack_flag = 0;
	type_id = pmbuf.typeId;

	switch(type_id)
	{
	case P_ME_NA:

		ret = ParamValueNAAck(pmbuf.cot,pmbuf.qpm,pmbuf.infoaddr,pmbuf.setvalue,sock);
		break;

	case P_ME_NB:

		ret = ParamValueNBAck(pmbuf.cot,pmbuf.qpm,pmbuf.infoaddr,pmbuf.setvalue,sock);
		break;

	case P_ME_NC:

		ret = ParamValueNCAck(pmbuf.cot,pmbuf.qpm,pmbuf.infoaddr,pmbuf.fvalue,sock);
		break;

	default:
		OutputDspInfo(OUTPUT_WARN, "unknown setpoint type id %d\n",type_id);
		NoChangeData = 1;
		pmbuf.ack_flag = 0;
		ret = true;
		break;
	}
	return(ret);
}

int	MirrorNAck(unsigned char cause,unsigned int pnt,int sock)
{
	unsigned char	infobyte;

	sendbuf[0] = 0x68;			/* start flag */
	infobyte = (unsigned char)recv_data.length[pnt];
	sendbuf[1] = infobyte+4;		/* L */

	FillControlField(sendbuf+2);

	memcpy(&sendbuf[6],&recv_data.data[pnt][0],infobyte);	/* Milliseconds */

	sendbuf[8] = cause;			/* ACTCON:7 cause of transmit:active */

	byte_to_send = 6 + infobyte;

	send_byte = send(sock,(char*)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Mirror Command NAck send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nMirror Command NAck: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SinglePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */

	FillControlField(sendbuf+2);

	sendbuf[6] = C_SC_NA;			/* 45 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 ActCon cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

/*	QCC:=CP8{SCS,QU,S/E} SCS 1:=ON 0:=OFF S/E := 0:=Execute 1:= Select */
	sendbuf[15] = stat;

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point Ctrl Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
/* 	memcpy(cmdsendbuf,sendbuf,byte_to_send); */	/* copy cmd for actterm */
	OutputDspInfo(0, "\nSingle Point Ctrl Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SinglePointCtrlTAAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 21;			/* L */
	
	FillControlField(sendbuf+2);
	
	sendbuf[6] = C_SC_TA;		/* 58 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 ActCon cause of transmit:active */
	sendbuf[9] = nSrcId;		/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

/*	QCC:=CP8{SCS,QU,S/E} SCS 1:=ON 0:=OFF S/E := 0:=Execute 1:= Select */
	sendbuf[15] = stat;

	FillCP56Time2aField(&sendbuf[16]);

	byte_to_send = 23;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point Ctrl TA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
/* 	memcpy(cmdsendbuf,sendbuf,byte_to_send); */	/* copy cmd for actterm */
	OutputDspInfo(0, "\nSingle Point Ctrl TA Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	DoublePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
{

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	
	FillControlField(sendbuf+2);
	
	sendbuf[6] = C_DC_NA;			/* 46 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
/*	QCC:=CP8{DCS,QU,S/E} DCS 2:=ON 1:=OFF S/E := 0:=Execute 1:= Select */
	sendbuf[15] = stat;

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Double Point Ctrl Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
/* 	memcpy(cmdsendbuf,sendbuf,byte_to_send); */	/* copy cmd for actterm */

	OutputDspInfo(0, "\nDouble Point Ctrl Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	DoublePointCtrlTAAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 21;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_DC_TA;			/* 59 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
/*	QCC:=CP8{DCS,QU,S/E} DCS 2:=ON 1:=OFF S/E := 0:=Execute 1:= Select */
	sendbuf[15] = stat;

	FillCP56Time2aField(&sendbuf[16]);

	byte_to_send = 23;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Double Point Ctrl TA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
/* 	memcpy(cmdsendbuf,sendbuf,byte_to_send); */	/* copy cmd for actterm */

	OutputDspInfo(0, "\nDouble Point Ctrl TA Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	RegulatStepCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_RC_NA;			/* 47 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

/*	QCC:=CP8{RCS,QU,S/E} RCS 2:=Higher 1:=Lower S/E := 0:=Execute 1:= Select */
	sendbuf[15] = stat;

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Regulating Step Ctrl Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRegulating Step Ctrl Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	RegulatStepCtrlTAAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 21;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_RC_TA;			/* 60 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

/*	QCC:=CP8{RCS,QU,S/E} RCS 2:=Higher 1:=Lower S/E := 0:=Execute 1:= Select */
	sendbuf[15] = stat;

	FillCP56Time2aField(&sendbuf[16]);

	byte_to_send = 23;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Regulating Step Ctrl TA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRegulating Step Ctrl TA Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SetPointCmdNAAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock)
{

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_SE_NA;			/* 48 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 9 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
	sendbuf[17] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Set Point Command NA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSet Point Command NA Ack: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SetPointCmdTAAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 23;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_SE_TA;			/* 61 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 9 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
	sendbuf[17] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	FillCP56Time2aField(&sendbuf[18]);

	byte_to_send = 25;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Set Point Command TA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSet Point Command TA Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SetPointCmdNBAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_SE_NB;			/* 49 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 9 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */

	sendbuf[17] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Set Point Command NB Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSet Point Command NB Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SetPointCmdTBAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 23;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_SE_TB;			/* 62 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 9 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
	sendbuf[17] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	FillCP56Time2aField(&sendbuf[18]);

	byte_to_send = 25;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Set Point Command TB Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSet Point Command TB Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SetPointCmdNCAck(unsigned char cause,unsigned short address,float value,unsigned char se,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 18;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_SE_NC;			/* 50 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,4);	/*  Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,4);		/*  Value */
#endif
	sendbuf[14] = 0;			/* information object high address */

	sendbuf[19] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	byte_to_send = 20;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Set Point Command NC Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSet Point Command NC Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SetPointCmdTCAck(unsigned char cause,unsigned short address,float value,unsigned char se,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 25;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_SE_TC;			/* 63 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 9 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,4);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,4);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */
	
	sendbuf[19] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	FillCP56Time2aField(&sendbuf[20]);

	byte_to_send = 27;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Set Point Command TC Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSet Point Command TC Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SetPointCmdNDAck(unsigned char cause,unsigned char num,unsigned int pnt,unsigned char se,int sock)
{
	unsigned char	infobyte;

	sendbuf[0] = 0x68;			/* start flag */
/*	sendbuf[1] = 16;	*/		/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_SE_ND;			/* Multi setpoint:136 type identifier field */
	sendbuf[7] = num;			/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	infobyte = num*5 + 1;
	memcpy(&sendbuf[12],&recv_data.data[pnt][6],infobyte);	/* Milliseconds */

	sendbuf[1] = (unsigned char)(10 + infobyte);	/* L */
	byte_to_send = 12 + infobyte;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Multi Set Point Command ND Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nMulti Set Point Command ND Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	ParamValueNAAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = P_ME_NA;			/* 110 type identifier field,normialized */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Normalized Value */
#endif
	sendbuf[14] = 0;			/* information object high address */

	sendbuf[17] = qpm;			/* KPA[1-6] LPC=0(Same)/1(Changed) POP=0 */
						/* KPA 1:= threshold 2:= filter factor */
						/* 3:= low limit  4:= high limit */
	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Parameter value NA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nParameter value NA Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	ParamValueNBAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = P_ME_NB;			/* 111 type identifier field,scaled */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* Scaled Value */
#endif
	sendbuf[14] = 0;			/* information object high address */

	sendbuf[17] = qpm;			/* KPA[1-6] LPC POP */
						/* KPA 1:= threshold 2:= filter factor */
						/* 3:= low limit  4:= high limit */
	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Parameter value NB Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nParameter value NB Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	ParamValueNCAck(unsigned char cause,unsigned char qpm,unsigned short address,float value,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 18;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = P_ME_NC;			/* 112 type identifier field,normialized */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&address,2);	/* information address */
	CrossCopy(&sendbuf[15],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&address,2);	/* information address */
	memcpy(&sendbuf[15],&value,2);		/* float Value */
#endif
	sendbuf[14] = 0;			/* information object high address */

	sendbuf[19] = qpm;		/* KPA[1-6] LPC POP */
					/* KPA 1:= threshold 2:= filter factor */
					/* 3:= low limit  4:= high limit */
	byte_to_send = 20;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Parameter value NC Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nParameter value NC Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	ClockSynchroniseAck(unsigned char cause, int sock)
{
	extern int Gps;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 20;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_CS_NA;			/* 103 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			//7 cause of transmit:actcon
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */

	FillCP56Time2aField(&sendbuf[15]);

	byte_to_send = 22;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "GeneralInterrogation ACK send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nACKCON ClockSynchronisation: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	TimeDelayAck(int sock)
{
	unsigned short locp16time;

#ifdef WINDOWS_OS
	SYSTEMTIME	sys_tm;
	GetLocalTime(&sys_tm);
	locp16time = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;
#else
	struct timeval	 sTimeVal;
	struct tm sTimetm;
	gettimeofday(&sTimeVal,NULL);
	sTimetm = *localtime(&sTimeVal.tv_sec);
	locp16time = sTimetm.tm_sec * 1000 + sTimeVal.tv_usec/1000;	
#endif

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 15;			/* L */
	
	FillControlField(sendbuf+2);

	sendbuf[6] = C_CD_NA;			/* 106 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cdbuf.cot;			/* 7 cause of transmit:actcon */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */

	if(locp16time<cdbuf.cp16time)
		locp16time = locp16time + 1000 - cdbuf.cp16time;
	else
		locp16time = locp16time - cdbuf.cp16time;
	cdbuf.cp16time = cdbuf.cp16time + locp16time;
	if(cdbuf.cp16time > 59999)
		cdbuf.cp16time -= 60000;
#ifdef SUN_OS
	CrossCopy(&sendbuf[15],&cdbuf.cp16time,2);
#else
	memcpy(&sendbuf[15],&cdbuf.cp16time,2);	/* Milliseconds to second */
#endif
	
	byte_to_send = 17;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Time Delay Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nACKCON Time Delay: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	GeneralInterrogationAck(unsigned char gi_cause,unsigned char gi_qoi,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = C_IC_NA;			/* 100 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = gi_cause;			/* 7 ActCon 10 Actterm cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = gi_qoi;			/* QOI:20 general 34:param group (icbuf.qoi) */
	
	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "GeneralInterrogationAck send() Error %d! do close socket!\n",errno);
		
		return(false);
	}

	OutputDspInfo(0, "\nGeneralInterrogationAck: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	CounterInterrogationAck(unsigned char gi_cause,unsigned char gi_qcc,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = C_CI_NA;			/* 101 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = gi_cause;			/* 7 Actcon 10 Actterm cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = gi_qcc;			/* QCC 5 general request counter */							
						/* QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]} */
					/* RQT 5:=general counter interrogation */
					/* RQT 1-4 := 1-4 group counter interrogation */
					/* FRZ 0:=request counter value */					
	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "CounterInterrogationAck send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nCounterInterrogationAck: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	RespondTestAck(unsigned short tsc,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 15;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = C_TS_NA;			/* 104 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
/* 	sendbuf[8] = ACTCON | 0x80;		/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[8] = testbuf.cot;		/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[15],&tsc,2);	/* TSC */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[15],&tsc,2);	/* TSC */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
/*	sendbuf[15] = 0xAA;	*/		/* test low byte AA */
/*	sendbuf[16] = 0x55;	*/		/* test high byte 55 */

	byte_to_send = 17;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Test channel Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nTest channel Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	RespondTestTAAck(unsigned short tsc,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 22;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = C_TS_TA;			/* 107 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
/* 	sendbuf[8] = ACTCON | 0x80;		/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[8] = testbuf.cot;		/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[15],&tsc,2);	/* TSC */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[15],&tsc,2);	/* TSC */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
/*	sendbuf[15] = 0xAA;	*/		/* test low byte AA */
/*	sendbuf[16] = 0x55;	*/		/* test high byte 55 */

	FillCP56Time2aField(&sendbuf[17]);

	byte_to_send = 24;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Test channel TA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nTest channel TA Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	ResetProcessAck(int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = C_RP_NA;			/* 106 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = rpbuf.cot;			/* 7 Actcon cause of transmit: T=1 P/N=0*/
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = rpbuf.qrp;		/* QRP[1..8] 1:reset process 2:reset soe buf */

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Reset Process Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nReset Process Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	ReadCmdAck(unsigned char qoc,unsigned short infoaddr,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 13;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = C_RD_NA;			/* 102 type identifier */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = qoc;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&infoaddr,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&infoaddr,2);	/* information address */
#endif
	sendbuf[14] = 0;			/* information object high address */

	byte_to_send = 15;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read C_RD_NA Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRead C_RD_NA Ack: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	ReadCmdResp(unsigned short infoaddr,int sock)
{
	unsigned short	infopnt;

	infopnt = infoaddr - sp_base_addr;
	if((infoaddr>=sp_base_addr)&&(infopnt<spdef.number))
	{
		ReadSingleInform(infoaddr,sock);
		return(true);
	}

	infopnt = infoaddr - dp_base_addr;
	if((infoaddr>=dp_base_addr)&&(infopnt<dpdef.number))
	{
		ReadDoubleInform(infoaddr,sock);
		return(true);
	}

	infopnt = infoaddr - st_base_addr;
	if((infoaddr>=st_base_addr)&&(infopnt<stdef.number))
	{
		ReadStepPositionInform(infoaddr,sock);
		return(true);
	}

	infopnt = infoaddr - me_base_addr;
	if((infoaddr>=me_base_addr)&&(infopnt<medef.number))
	{
		if(medef.ASDUType[infopnt] == 0)
			ReadMeasureValueNA(infoaddr,sock);	/* M_ME_NA */
		else if(medef.ASDUType[infopnt] == 1)
			ReadMeasureValueNB(infoaddr,sock);	/* M_ME_NB */
		else if(medef.ASDUType[infopnt] == 2)
			ReadMeasureValueND(infoaddr,sock);	/* M_ME_ND */
		else
			ReadMeasureValueNC(infoaddr,sock);	/* M_ME_NC */
		return(true);
	}
	else
	{
		ReadCmdAck((unsigned char)(0x40|UKINFOAD),infoaddr,sock);
	}
	return(true);
}

int	SysInform(unsigned short info_addr,int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = SPONT;			/* cause of transmit:spont */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	CrossCopy(&sendbuf[12],&info_addr,2);	/* information address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	memcpy(&sendbuf[12],&info_addr,2);	/* information address */
#endif

	sendbuf[12] = info_addr& 0xFF;		/* i-addr low address */
	sendbuf[13] = info_addr>>8;		/* i-addr high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = 1;			/* QDS IV NT SB BL RES3 SPI */

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "System information Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSystem information: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SinglePointInform(unsigned char gi_cause,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength;
	int pntnum;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Check for SP status Change */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
			
			continue;
		}
		else
		{
			if(dbentry.var.ind.status.rt_state==1)
				stat = 1;
			else
				stat = 0;
			if(dbentry.var.ind.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.ind.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
			if(stat != orgspbuf[pntnum])
			{
				sendbuf[12+inf_n*4] = (sp_base_addr + pntnum)& 0xFF;	/* i-addr low address */
				sendbuf[13+inf_n*4] = (sp_base_addr + pntnum)>>8;	/* i-addr high address */
				sendbuf[14+inf_n*4] = 0;				/* i-addr high address */
				sendbuf[15+inf_n*4] = orgspbuf[pntnum] =stat;		/* QDS IV NT SB BL RES3 SPI */
				inf_n++;
				if(inf_n >= SPONT_SP_NUM)
				{
					sendbuf[7] = inf_n;		/* variable structure specify (number of i-object) */
					nLength = sendbuf[7] * 4 + 10;
					sendbuf[1] = nLength;		/* L	byte_to_send = L + 2; */
					byte_to_send = nLength + 2;

					send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

					if(send_byte != byte_to_send)
					{
						OutputDspInfo(OUTPUT_ERROR, "Single Point information send() Error %d! do close socket!\n",errno);
						
						
						return(false);
					}
					OutputDspInfo(0, "\nSingle Point information: %u byte transmitted!",send_byte);
					
					PrintSendData(send_byte);
					
					UpdateSN();
					FillControlField(sendbuf+2);
					inf_n = 0;
				}
			}
		}
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "SinglePointInform: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n;		/* variable structure specify (number of i-object) */
	nLength = sendbuf[7] * 4 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point information send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nSingle Point information: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	CheckSinglePointChange(void)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
    int pntnum;

	for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Check for SP status Change */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
				continue;
		}
		else
		{
			if(dbentry.var.ind.status.rt_state==1)
				stat = 1;
			else
				stat = 0;
			if(dbentry.var.ind.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.ind.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
			if(stat != orgspbuf[pntnum])
			{
				NoChangeData = 1;
				return(true);
			}
		}
	}
	NoChangeData = 0;
	return(false);
}

int	ReadSingleInform(unsigned short infoaddr,int sock)
{
	int pntnum;
	unsigned char	stat;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum=infoaddr - sp_base_addr;		/* Check for SP status Change */
	strcpy(name_buf,spdef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
		stat = 0;
	}
	else
	{
		if(dbentry.var.ind.status.rt_state==1)
			stat = 1;
		else
			stat = 0;
		if(dbentry.var.ind.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.ind.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);	/* i-addr high address */
	sendbuf[14] = 0;				/* information object high address */
	sendbuf[15] = orgspbuf[pntnum] =stat;		/* QDS IV NT SB BL RES3 SPI */

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read Single Point information Ack send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRead Single Point information: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	SinglePointInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = SP_GROUP_NUM*infogroup;	/* infogroup: 0,1,2...8 */
	if(spdef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + sp_base_addr;
	endnum = startnum + SP_GROUP_NUM;
	if(spdef.number < endnum)
		endnum = spdef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)		/* Cycle for SP status */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			sendbuf[15+inf_n] = orgspbuf[pntnum] =0;	/* QDS IV NT SB BL RES3 SPI */
			inf_n++;
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
			
			sendbuf[15+inf_n] = orgspbuf[pntnum] =0;	/* QDS IV NT SB BL RES3 SPI */
			inf_n++;
			continue;
		}
		if(dbentry.var.ind.status.rt_state==1)
			stat = 1;
		else
			stat = 0;
		if(dbentry.var.ind.status.man_set)
			stat = stat | 0x20;			/* SB=1 */
		if(dbentry.var.ind.status.chan_fail)
			stat = stat | 0xC0;			/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;			/* BL=1 */

		sendbuf[15+inf_n]  =stat;	/* QDS IV NT SB BL RES3 SPI */
		inf_n++;
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		OutputDspInfo(0, "SinglePointInformCyc: NoRequestData\n");
		
#endif
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;	/* SQ=1 variable structure specify (number of i-object) */
	nLength = inf_n + 13;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point information Cyc send() Error %d! do close socket!\n",errno);
		return(false);
	}

	OutputDspInfo(0, "\nSingle Point information Cyc: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	SinglePointInformTA(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_SP_TA;			/* 2 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_SP_TA) || (nInfoNum >= TA_SP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*7] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*7] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES3 SPI */						

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[16+nInfoNum*7],&millisec,2);	/* Milliseconds */
#else
		memcpy(&sendbuf[16+nInfoNum*7],&millisec,2);		/* Milliseconds */
#endif
		sendbuf[18+nInfoNum*7] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */

		soebuf.nr_send[soepnt]= nr_send;

		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}

	if(nInfoNum == 0)
	{
		soebuf.sendpnt = soepnt;
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = nInfoNum;		/* variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 7 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point information TA send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSingle Point information TA: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	soebuf.sendpnt = soepnt;
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	SinglePointInformTB(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_SP_TB;			/* 30 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_SP_TB) || (nInfoNum >= TB_SP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;		/* information object low address */
		sendbuf[13+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)>>8;			/* information object high address */
		sendbuf[14+nInfoNum*11] = 0;							/* information object high address */						
		sendbuf[15+nInfoNum*11] = soebuf.soerec[soepnt].stat;				/* QDS IV NT SB BL RES3 SPI */							

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[16+nInfoNum*11],&millisec,2);	/* Milliseconds */
#else
		memcpy(&sendbuf[16+nInfoNum*11],&millisec,2);		/* Milliseconds */
#endif
		sendbuf[18+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		sendbuf[19+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/* Hours(0-23) */
		sendbuf[20+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.day;
												/* Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[21+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.month;	/* Months(1-12) */
		sendbuf[22+nInfoNum*11] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/* Years(0-99) */

		soebuf.nr_send[soepnt]= nr_send;
		
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}

	if(nInfoNum == 0)
	{
		soebuf.sendpnt = soepnt;
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = nInfoNum;			/* variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 11 + 10;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point information TB send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nSingle Point information TB: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	soebuf.sendpnt = soepnt;
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	SinglePointGroupCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	unsigned short	stat16,scd16,bitmap;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_PS_NA;			/* 20 type single point group information) */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28) */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = PS_GROUP_NUM*infogroup;	/* infogroup: 0,1,2...8) */
	if(spdef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + sp_base_addr;
	endnum = startnum + PS_GROUP_NUM;
	if(spdef.number < endnum)
		endnum = spdef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address) */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address) */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address) */
	inf_n = 0;
	bitmap = 1;
	stat16 = 0;
	scd16 = 0xFFFF;
	for(pntnum=startnum;pntnum<endnum;pntnum++)		/* Cycle for SP status) */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			orgspbuf[pntnum] =0;			/* QDS IV NT SB BL RES3 SPI) */
		}
		else
		{
			strcpy(name_buf,spdef.name[pntnum]);
			if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
			{
				OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
				
				orgspbuf[pntnum] =0;		/* QDS IV NT SB BL RES3 SPI) */
			}
			else
			{
				stat=(unsigned char)(dbentry.var.ind.status.rt_state);
				if(stat==1)
					stat16 = stat16 | bitmap;	
			}
		}
		bitmap = bitmap<<1;
		if(bitmap == 0)
		{
#ifdef SUN_OS
			CrossCopy(&sendbuf[15+inf_n*5],&stat16,2);	/* SCD:stat16 */
			CrossCopy(&sendbuf[17+inf_n*5],&scd16,2);	/* SCD:scd16 */
#else
			memcpy(&sendbuf[15+inf_n*5],&stat16,2);		/* SCD:stat16 */
			memcpy(&sendbuf[17+inf_n*5],&scd16,2);		/* SCD:scd16 */
#endif
			sendbuf[19+inf_n*5] = 0;
			inf_n++;
			bitmap = 1;
			stat16 = 0;
		}
	}
	if(bitmap != 1)
	{
#ifdef SUN_OS
		CrossCopy(&sendbuf[15+inf_n*5],&stat16,2);	/* SCD:stat16 */
		CrossCopy(&sendbuf[17+inf_n*5],&scd16,2);	/* SCD:scd16 */
#else
		memcpy(&sendbuf[15+inf_n*5],&stat16,2);		/* SCD:stat16 */
		memcpy(&sendbuf[17+inf_n*5],&scd16,2);		/* SCD:scd16 */
#endif
		sendbuf[19+inf_n*5] = 0;
		inf_n++;
	}
	if(inf_n == 0)
	{
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;	/* SQ=1 variable structure specify (number of i-object) */
	nLength = 13 + inf_n*5;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point information Cyc send() Error %d! do close socket!\n",errno);
		
		
		return(false);
	}
	OutputDspInfo(0, "\nSingle Point information Cyc: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	DoublePointInform(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	unsigned short	dbstat;
	DMS_COMMON	dbentry;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_DP_NA;			/* 3 type double point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<dpdef.number;pntnum++)	/* Check for DP status Change */
	{
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,dpdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this DP point in the database file!\n",name_buf);
			continue;
		}
		else
		{
			dbstat=(dbentry.var.ind.status.state);
			if(dbstat == 0)
				stat = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
			else if(dbstat == 1)
				stat = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
			else if(dbstat == 2)
				stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
			else
				stat = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

			if(dbentry.var.ind.status.man_set)
				stat = stat | 0x20;			/* SB=1 */
			if(dbentry.var.ind.status.chan_fail)
				stat = stat | 0xC0;			/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;			/* BL=1 */
			if(stat != orgdpbuf[pntnum])
			{
				sendbuf[12+inf_n*4] = (dp_base_addr + pntnum)& 0xFF;	/* i-addr low address */
				sendbuf[13+inf_n*4] = (dp_base_addr + pntnum)>>8;	/* i-addr high address */
				sendbuf[14+inf_n*4] = 0;				/* i-addr high address */
				sendbuf[15+inf_n*4] = orgdpbuf[pntnum] = stat;		/* OFF:QDS IV NT SB BL RES2 DPI */
				inf_n++;
				if(inf_n >= SPONT_DP_NUM)
				{
					sendbuf[7] = inf_n;		/* variable structure specify (number of i-object) */
					nLength = sendbuf[7] * 4 + 10;
					sendbuf[1] = nLength;		/* L	byte_to_send = L + 2; */
					byte_to_send = nLength + 2;

					send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

					if(send_byte != byte_to_send)
					{
						OutputDspInfo(OUTPUT_ERROR, "Double Point information send() Error %d! do close socket!\n",errno);
						return(false);
					}
					OutputDspInfo(0, "\nDouble Point information: %u byte transmitted!",send_byte);
					
					PrintSendData(send_byte);
					
					UpdateSN();
					FillControlField(sendbuf+2);
					inf_n = 0;
				}
			}
		}
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "DoublePointInform: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n;			/* variable structure specify (number of i-object) */
	nLength = sendbuf[7] * 4 + 10;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Double Point information send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nDouble Point information: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	CheckDoublePointChange(void)
{
	unsigned char	stat;
	unsigned short	dbstat;
	DMS_COMMON	dbentry;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* Check for DP status Change */
	{
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,dpdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this DP point in the database file!\n",name_buf);
			
			continue;
		}
		else
		{
			dbstat=dbentry.var.ind.status.state;
			if(dbstat == 0)
				stat = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
			else if(dbstat == 1)
				stat = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
			else if(dbstat == 2)
				stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
			else
				stat = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

			if(stat != orgdpbuf[pntnum])
			{
				NoChangeData = 1;
				return(true);
			}
		}
	}
	NoChangeData = 0;
	return(false);
}

int	ReadDoubleInform(unsigned short infoaddr,int sock)
{
	unsigned char	stat;
	unsigned short	dbstat;
	DMS_COMMON	dbentry;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_DP_NA;			/* 3 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum=infoaddr - dp_base_addr;		/* Check for DP status Change */
	strcpy(name_buf,dpdef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this DP point in the database file!\n",name_buf);
		
		stat = 0;
	}
	else
	{
		dbstat=dbentry.var.ind.status.state;
		if(dbstat == 0)
			stat = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
		else if(dbstat == 1)
			stat = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
		else if(dbstat == 2)
			stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
		else
			stat = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

		if(dbentry.var.ind.status.man_set)
			stat = stat | 0x20;			/* SB=1 */
		if(dbentry.var.ind.status.chan_fail)
			stat = stat | 0xC0;			/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;			/* BL=1 */

	}
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;			/* i-addr high address */
	sendbuf[15] = orgdpbuf[pntnum] =stat;		/* QDS IV NT SB BL RES2 DPI */

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read Double Point information send() Error %d! do close socket!\n",errno);
		
		
		return(false);
	}
	OutputDspInfo(0, "\nRead Double Point information: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	UpdateSN();
	return(true);
}

int	DoublePointInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned short	dbstat;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_DP_NA;			/* 3 type double point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = DP_GROUP_NUM*infogroup;	/* infogroup: 0,1,2...8 */
	if(dpdef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + dp_base_addr;	
	endnum = startnum + DP_GROUP_NUM;
	if(dpdef.number < endnum)
		endnum = dpdef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)		/* Cycle for DP status */
	{
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			sendbuf[15+inf_n] = orgdpbuf[pntnum] =0;	/* QDS IV NT SB BL RES2 DPI */
			inf_n++;
			continue;
		}
		strcpy(name_buf,dpdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this DP point in the database file!\n",name_buf);
			
			sendbuf[15+inf_n] = orgdpbuf[pntnum] =0;	/* QDS IV NT SB BL RES2 DPI */
			inf_n++;
			continue;
		}
		dbstat=dbentry.var.ind.status.state;
		if(dbstat == 0)
			stat = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
		else if(dbstat == 1)
			stat = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
		else if(dbstat == 2)
			stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
		else
			stat = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

		if(dbentry.var.ind.status.man_set)
			stat = stat | 0x20;			/* SB=1 */
		if(dbentry.var.ind.status.chan_fail)
			stat = stat | 0xC0;			/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;			/* BL=1 */

		sendbuf[15+inf_n] = stat;	/* OFF:QDS IV NT SB BL RES2 DPI */

		inf_n++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "DoublePointInformCyc: NoRequestData\n");
		
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;	/* SQ=1 variable structure specify (number of i-object) */
	nLength = inf_n + 13;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Double Point information Cyc send() Error %d! do close socket!\n",errno);
		
		
		return(false);
	}
	OutputDspInfo(0, "\nDouble Point information Cyc: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	DoublePointInformTA(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_DP_TA;			/* 4 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_DP_TA) || (nInfoNum >= TA_DP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*7] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*7] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES2 DPI */							

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[16+nInfoNum*7],&millisec,2);	/* Milliseconds */
#else
		memcpy(&sendbuf[16+nInfoNum*7],&millisec,2);		/* Milliseconds */
#endif
		sendbuf[18+nInfoNum*7] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */

		soebuf.nr_send[soepnt]= nr_send;
		
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}

	if(nInfoNum == 0)
	{
		soebuf.sendpnt = soepnt;
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = nInfoNum;			/* variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 7 + 10;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Single Point information TA send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nDouble Point information TA: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	soebuf.sendpnt = soepnt;
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	DoublePointInformTB(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_DP_TB;			/* 31 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	//OutputDspInfo(0, "\nZYF::$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");	
	//OutputDspInfo(0, "\nZYF::Double Point information TB: soebuf.sendpnt=%d soebuf.bufpnt=%d",soebuf.sendpnt,soebuf.bufpnt);
	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_DP_TB) || (nInfoNum >= TB_DP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;		/* information object low address */
		sendbuf[13+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)>>8;			/* information object high address */
		sendbuf[14+nInfoNum*11] = 0;							/* information object high address */
		sendbuf[15+nInfoNum*11] = soebuf.soerec[soepnt].stat;				/* QDS IV NT SB BL RES2 DPI */						

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[16+nInfoNum*11],&millisec,2);	/* Milliseconds */
#else
		memcpy(&sendbuf[16+nInfoNum*11],&millisec,2);		/* Milliseconds */
#endif
		sendbuf[18+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		sendbuf[19+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/* Hours(0-23) */
		sendbuf[20+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.day;
												/* Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[21+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.month;	/* Months(1-12) */
		sendbuf[22+nInfoNum*11] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/* Years(0-99) */

		soebuf.nr_send[soepnt]= nr_send;
		
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}

	if(nInfoNum == 0)
	{
		soebuf.sendpnt = soepnt;
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = nInfoNum;			/* variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 11 + 10;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Double Point information TB send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nDouble Point information TB: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	soebuf.sendpnt = soepnt;
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	StepPositionInform(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat,vti;
	DMS_COMMON	dbentry;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ST_NA;			/* 5 type step position information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<stdef.number;pntnum++)	/* Check for ST status Change */
	{
		if(strcmp(stdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,stdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ST point in the database file!\n",name_buf);
				continue;
		}
		if(dbentry.point.data_type != ANA_TYPE)
		{
			vti= (unsigned char)(dbentry.var.ind.status.state);

			stat = 0;
			if(dbentry.var.ind.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.ind.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			vti= (unsigned char)(dbentry.var.anlg.value.data/1000);

			stat = 0;
			if(dbentry.var.anlg.value.status.ctrl_block)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		if((stat != orgstbuf[pntnum])||(vti!= orgstvbuf[pntnum])) 
		{
			sendbuf[12+inf_n*5] = (st_base_addr + pntnum)& 0xFF;	/* i-addr low address */
			sendbuf[13+inf_n*5] = (st_base_addr + pntnum)>>8;	/* i-addr high address */
			sendbuf[14+inf_n*5] = 0;				/* i-addr high address */
			sendbuf[15+inf_n*5] = orgstvbuf[pntnum] = vti;		/* VTI[1..7]:-64 63 */
			sendbuf[16+inf_n*5] = orgstbuf[pntnum] = stat;		/* OFF:QDS IV NT SB BL RES3 OV */
			inf_n++;
			if(inf_n >= 60)
			{
				sendbuf[7] = inf_n;				/* variable structure specify (number of i-object) */
				nLength = sendbuf[7] * 4 + 10;
				sendbuf[1] = nLength;				/* L	byte_to_send = L + 2; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "StepPositionInform send() Error %d! do close socket!\n",errno);
					return(false);
				}
				OutputDspInfo(0, "\nStepPositionInform: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
			FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		OutputDspInfo(0, "StepPositionInform: NoRequestData\n");
#endif
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n;		/* variable structure specify (number of i-object) */
	nLength = sendbuf[7] * 5 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Step Position information send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nStep Position information: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	CheckStepPositionChange(void)
{
	unsigned char	stat,vti;
	DMS_COMMON	dbentry;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	for(pntnum=0;pntnum<stdef.number;pntnum++)		/* Check for ST status Change */
	{
		if(strcmp(stdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,stdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ST point in the database file!\n",name_buf);
				continue;
		}
		if(dbentry.point.data_type != ANA_TYPE)
		{
			vti= (unsigned char)(dbentry.var.ind.status.state);

			stat = 0;
			if(dbentry.var.ind.status.man_set)
				stat = stat | 0x20;			/* SB=1 */
			if(dbentry.var.ind.status.chan_fail)
				stat = stat | 0xC0;			/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;			/* BL=1 */
		}
		else
		{
			vti= (unsigned char)(dbentry.var.anlg.value.data/1000);

			stat = 0;
			if(dbentry.var.anlg.value.status.ctrl_block)
				stat = stat | 0x20;			/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;			/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;			/* BL=1 */
		}
		if((stat != orgstbuf[pntnum])||(vti != orgstvbuf[pntnum]))
		{
			NoChangeData = 1;
			return(true);
		}
	}
	NoChangeData = 0;
	return(false);
}

int	ReadStepPositionInform(unsigned short infoaddr,int sock)
{
	unsigned char	stat,vti;
	DMS_COMMON	dbentry;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 15;			/* L */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ST_NA;			/* 5 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum=infoaddr - st_base_addr;
	strcpy(name_buf,stdef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this Step Position point in the database file!\n",name_buf);
		stat = 0;
	}
	if(dbentry.point.data_type != ANA_TYPE)
	{
		vti= (unsigned char)(dbentry.var.ind.status.state);

		stat = 0;
		if(dbentry.var.ind.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.ind.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		vti= (unsigned char)(dbentry.var.anlg.value.data/1000);

		stat = 0;
		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);		/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);		/* i-addr high address */
	sendbuf[14] = 0;					/* i-addr high address */
	sendbuf[15] = orgstvbuf[pntnum] =vti;			/* VTI */
	sendbuf[16] = orgstbuf[pntnum] =stat;			/* QDS IV NT SB BL RES3 OV */

	byte_to_send = 17;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read StepPosition information send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRead StepPosition information: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	StepPositionInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength,vti;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;


	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ST_NA;			/* 5 type step position information */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = ST_GROUP_NUM*infogroup;	/* infogroup: 0,1,2...8 */
	if(stdef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + st_base_addr;	
	endnum = startnum + ST_GROUP_NUM;
	if(stdef.number < endnum)
		endnum = stdef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)		/* Cycle for DP status */
	{
		if(strcmp(stdef.name[pntnum],"NULL")==0)
		{
			sendbuf[15+inf_n*2] = orgstvbuf[pntnum] =0;	/* VTI */
			sendbuf[16+inf_n*2] = orgstbuf[pntnum] =0;	/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,stdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ST point in the database file!\n",name_buf);
				sendbuf[15+inf_n*2] = orgstvbuf[pntnum] =0;	/* VTI */
			sendbuf[16+inf_n*2] = orgstbuf[pntnum] =0;	/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ST point in the database file!\n",name_buf);
				continue;
		}
		if(dbentry.point.data_type != ANA_TYPE)
		{
			vti= (unsigned char)(dbentry.var.ind.status.state);

			stat = 0;
			if(dbentry.var.ind.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.ind.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			vti= (unsigned char)(dbentry.var.anlg.value.data/1000);

			stat = 0;
			if(dbentry.var.anlg.value.status.ctrl_block)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}

		sendbuf[15+inf_n*2] = orgstvbuf[pntnum] = vti;	/* VTI */
		sendbuf[16+inf_n*2] = orgstbuf[pntnum] = stat;	/* OFF:QDS IV NT SB BL RES3 OV */

		inf_n++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "StepPositionInformCyc: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;		/* SQ=1 variable structure specify (number of i-object) */
	nLength = inf_n*2 + 13;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "StepPosition information Cyc send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nStepPosition information Cyc: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	StepPositionInformTA(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ST_TA;			/* 5 type step position information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_ST_TA) || (nInfoNum >= 30))
		{
			break;
		}
		sendbuf[12+nInfoNum*8] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*8] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*8] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*8] = soebuf.soerec[soepnt].vti;			/* VTI */				
		sendbuf[16+nInfoNum*8] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES3 OV */							

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[17+nInfoNum*8],&millisec,2);	/* Milliseconds */
#else
		memcpy(&sendbuf[17+nInfoNum*8],&millisec,2);		/* Milliseconds */
#endif
		sendbuf[19+nInfoNum*8] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */

		soebuf.nr_send[soepnt]= nr_send;
		
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}

	if(nInfoNum == 0)
	{
		soebuf.sendpnt = soepnt;
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = nInfoNum;				/* variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 8 + 10;
	sendbuf[1] = nLength;				/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "StepPosition information TA send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nStepPosition information TA: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	soebuf.sendpnt = soepnt;
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	StepPositionInformTB(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ST_TB;			/* 31 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_ST_TB) || (nInfoNum >= 20))
		{
			break;
		}
		sendbuf[12+nInfoNum*12] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*12] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*12] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*12] = soebuf.soerec[soepnt].vti;			/* VTI  */		
		sendbuf[16+nInfoNum*12] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES3 OV */					

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[17+nInfoNum*12],&millisec,2);	/* Milliseconds */
#else
		memcpy(&sendbuf[17+nInfoNum*12],&millisec,2);		/* Milliseconds */
#endif
		sendbuf[19+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		sendbuf[20+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/* Hours(0-23) */
		sendbuf[21+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.day;
						/* Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[22+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.month;	/* Months(1-12) */
		sendbuf[23+nInfoNum*12] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/* Years(0-99) */

		soebuf.nr_send[soepnt]= nr_send;
		
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}

	if(nInfoNum == 0)
	{
		soebuf.sendpnt = soepnt;
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = nInfoNum;				/* variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 12 + 10;
	sendbuf[1] = nLength;				/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "StepPosition information TB send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nStepPosition information TB: %u byte transmitted!",send_byte);
	
	PrintSendData(send_byte);
	
	soebuf.sendpnt = soepnt;
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNA(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	POINTER ptr;
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NA;			/* 9 type normalized without time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
/*
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if((medef.ptr[pntnum].dev_id == 0)||(medef.ASDUType[pntnum] !=0)||(medef.TimeTag[pntnum] !=0))
*/
		if(medef.ptr[pntnum].dev_id == 0)
		{
			continue;
		}
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr,&stat,&value)==-1)
		{
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum]);

			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f code=%d\n",
				pntnum,value,organafbuf[pntnum],medef.databand[pntnum], anaorgcode);
			
			sendbuf[12+inf_n*6] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*6] = (unsigned char)((me_base_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*6] = (unsigned char)0;					/* i-addr high address */
			sendbuf[15+inf_n*6] = (unsigned char)(anaorgcode & 0xFF);		/* N Value */
			sendbuf[16+inf_n*6] = (unsigned char)(anaorgcode >> 8);			/* N Value */
			sendbuf[17+inf_n*6] = organastbuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= SPONT_ME_NUM)
			{
				sendbuf[7] = inf_n;			/* SQ=0 */
				nLength = inf_n * 6 + 10;
				sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_NA() Error %d! do close socket!\n",errno);
					return(false);
				}
				OutputDspInfo(0, "\nMeasure Value M_ME_NA: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
				FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNA: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}

	nLength = inf_n * 6 + 10;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_NA send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nMeasure Value M_ME_NA: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueTD(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;

	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_TD;			/* 34 type normalized with time tag CP56Time2a*/
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
/*		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.ASDUType[pntnum] !=0)||(medef.TimeTag[pntnum] !=2))
*/
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=2))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum]);

#ifdef DEBUG
			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,value,organafbuf[pntnum],medef.databand[pntnum]);
	#endif
			sendbuf[12+inf_n*13] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*13] = (unsigned char)((me_base_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*13] = (unsigned char)0;					/* i-addr high address */
			sendbuf[15+inf_n*13] = (unsigned char)(anaorgcode & 0xFF);		/* N Value */						
			sendbuf[16+inf_n*13] = (unsigned char)(anaorgcode >> 8);			/* N Value */
			sendbuf[17+inf_n*13] = organastbuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 OV */

			FillCP56Time2aField(&sendbuf[18+inf_n*13]);

			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= SPONT_ME_TD_NUM)
			{
				sendbuf[7] = inf_n;			/* SQ=0 */
				nLength = inf_n * 13 + 10;
				sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_TD() Error %d! do close socket!\n",errno);
					return(false);
				}
				OutputDspInfo(0, "\nMeasure Value M_ME_TD: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
			FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTD: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}

	nLength = inf_n * 13 + 10;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_TD send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nMeasure Value M_ME_TD: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	CheckMeasureValueNA(void)
{
	unsigned char	stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			OutputDspInfo(0, "Check ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			NoChangeData = 1;
			return(true);
		}
	}
	NoChangeData = 0;
	return(false);
}

int	MeasureValueNACyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short			anaorgcode;
	float			value;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NA;			/* 9 type normalized without time tag */

	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = ME_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(medef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + me_base_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			sendbuf[15+inf_n*3] = (unsigned char)0;		/* N Value */						
			sendbuf[16+inf_n*3] = (unsigned char)0;		/* N Value */
			sendbuf[17+inf_n*3] = 0;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				sendbuf[15+inf_n*3] = (unsigned char)0;		/* N Value */						
			sendbuf[16+inf_n*3] = (unsigned char)0;		/* N Value */
			sendbuf[17+inf_n*3] = organastbuf[pntnum] = 0;	/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum]);
		sendbuf[15+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */						
		sendbuf[16+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/* N Value */
		sendbuf[17+inf_n*3] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNACyc: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;	/* SQ=1 */

	nLength = inf_n * 3 + 13;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Cyc Measure Value M_ME_NA send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nCyc Measure Value M_ME_NA: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	ReadMeasureValueNA(unsigned short infoaddr,int sock)
{
	unsigned char	stat;

	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L; */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NA;			/* 9 type normalized without time tag */

	sendbuf[7] = 1;				/* SQ=0 */

	sendbuf[8] = REQ;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum = infoaddr - me_base_addr;
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;			/* i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	if(GetMEValueByName(name_buf,&stat,&value)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
		stat = 0;				/* QDS IV NT SB BL RES3 OV */
		anaorgcode = (short)0;			/* N Value */
		value=(float)0;
	}
	anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum]);

	sendbuf[15] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */							
	sendbuf[16] = (unsigned char)(anaorgcode >> 8);		/* N Value */
	sendbuf[17] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
	organabuf[pntnum] = anaorgcode;
	organafbuf[pntnum] = value;

	byte_to_send = 18;

	send_byte = send(sock,(char*)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read Measure Value M_ME_NA send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRead Measure Value M_ME_NA: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNB(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	POINTER	point;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NB;			/* 11 type scaled without time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
/*		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=0)) */
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			GetPtIdByNameStr(name_buf, &point);
			if(point.data_type != IMP_TYPE)
			{
				fdata=value*medef.scale[pntnum]; /*Note: Here use scale factor */
				anaorgcode=(short)fdata;
				if((fdata-anaorgcode)>=0.5)
					anaorgcode++;
				else if((fdata-anaorgcode)<=-0.5)
					anaorgcode--;

				OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);

			}
			sendbuf[12+inf_n*6] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*6] = (unsigned char)((me_base_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*6] = (unsigned char)0;					/* i-addr high address */
			sendbuf[15+inf_n*6] = (unsigned char)(anaorgcode & 0xFF);		/* N Value */							
			sendbuf[16+inf_n*6] = (unsigned char)(anaorgcode >> 8);			/* N Value */
			sendbuf[17+inf_n*6] = organastbuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= SPONT_ME_NUM)
			{
				sendbuf[7] = inf_n;		/* SQ=0 */
				nLength = inf_n * 6 + 10;
				sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_NB() Error %d! do close socket!\n",errno);
					return(false);
				}
				OutputDspInfo(0, "\nMeasure Value M_ME_NB: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
				FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNB: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}

	nLength = inf_n * 6 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_NB send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nMeasure Value M_ME_NB: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueTE(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_TE;			/* 35 type scaled with time tag CP56Time2a*/
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=2))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			fdata=value*medef.scale[pntnum]; /*Note: Here use scale factor */
			anaorgcode=(short)fdata;
			if((fdata-anaorgcode)>=0.5)
				anaorgcode++;
			else if((fdata-anaorgcode)<=-0.5)
				anaorgcode--;

			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
				value,organafbuf[pntnum],medef.databand[pntnum]);

			sendbuf[12+inf_n*13] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*13] = (unsigned char)((me_base_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*13] = (unsigned char)0;					/* i-addr high address */
			sendbuf[15+inf_n*13] = (unsigned char)(anaorgcode & 0xFF);		/* N Value */							
			sendbuf[16+inf_n*13] = (unsigned char)(anaorgcode >> 8);			/* N Value */
			sendbuf[17+inf_n*13] = organastbuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 OV */
			FillCP56Time2aField(&sendbuf[18+inf_n*13]);
			
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= SPONT_ME_TE_NUM)
			{
				sendbuf[7] = inf_n;		/* SQ=0 */
				nLength = inf_n * 13 + 10;
				sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_TE() Error %d! do close socket!\n",errno);
					return(false);
				}
				OutputDspInfo(0, "\nMeasure Value M_ME_TE: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
				FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTE: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}

	nLength = inf_n * 13 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_TE send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nMeasure Value M_ME_TE: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	CheckMeasureValueNB(void)
{
	unsigned char	stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			OutputDspInfo(0, "Check ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);

			NoChangeData = 1;
			return(true);
		}
	}
	NoChangeData = 0;
	return(false);
}

int	MeasureValueNBCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;

	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NB;			/* 11 type normalized without time tag */

	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = ME_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(medef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + me_base_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			sendbuf[15+inf_n*3] = (unsigned char)0;		/* N Value */							
			sendbuf[16+inf_n*3] = (unsigned char)0;		/* N Value */
			sendbuf[17+inf_n*3] = 0;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				sendbuf[15+inf_n*3] = (unsigned char)0;			/* N Value */							
			sendbuf[16+inf_n*3] = (unsigned char)0;			/* N Value */
			sendbuf[17+inf_n*3] = organastbuf[pntnum] = 0;		/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		fdata=value*medef.scale[pntnum]; 	/*Note: Here use scale factor */
		anaorgcode=(short)fdata;
		if((fdata-anaorgcode)>=0.5)
			anaorgcode++;
		else if((fdata-anaorgcode)<=-0.5)
			anaorgcode--;

		sendbuf[15+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */						
		sendbuf[16+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/* N Value */
		sendbuf[17+inf_n*3] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNBCyc: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;	/* SQ=1 */

	nLength = inf_n * 3 + 13;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Cyc Measure Value M_ME_NB send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nCyc Measure Value M_ME_NB: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	ReadMeasureValueNB(unsigned short infoaddr,int sock)
{
	unsigned char	stat;

	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L; */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NB;			/* 11 type scaled without time tag */

	sendbuf[7] = 1;				/* SQ=0 */

	sendbuf[8] = REQ;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum = infoaddr - me_base_addr;
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);		/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);		/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	if(GetMEValueByName(name_buf,&stat,&value)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
		stat = 0;				/* QDS IV NT SB BL RES3 OV */
		anaorgcode = (short)0;			/* N Value */
		value = (float)0;
	}
	fdata=value*medef.scale[pntnum]; 	/*Note: Here use scale factor */
	anaorgcode=(short)fdata;
	if((fdata-anaorgcode)>=0.5)
		anaorgcode++;
	else if((fdata-anaorgcode)<=-0.5)
		anaorgcode--;

	sendbuf[15] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */						
	sendbuf[16] = (unsigned char)(anaorgcode >> 8);		/* N Value */
	sendbuf[17] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
	organabuf[pntnum] = anaorgcode;
	organafbuf[pntnum] = value;

	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read Measure Value M_ME_NB send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nRead Measure Value M_ME_NB: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNC(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;

	POINTER ptr;
	unsigned short	pntnum;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NC;			/* 13 type float without time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
/*		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.ASDUType[pntnum] !=3)||(medef.TimeTag[pntnum] !=0))
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
 */
		if(medef.ptr[pntnum].dev_id == 0)
		{
			continue;
		}
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr,&stat,&value)==-1)
		{
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
 			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
 					value,organafbuf[pntnum],medef.databand[pntnum]);

			sendbuf[12+inf_n*8] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*8] = (unsigned char)((me_base_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*8] = (unsigned char)0;					/* i-addr high address */
#ifdef SUN_OS
			CrossCopy(&sendbuf[15+inf_n*8],&value,4);	/* N Value */
#else
			memcpy(&sendbuf[15+inf_n*8],&value,4);		/* N Value */						
#endif
			sendbuf[19+inf_n*8] = organastbuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 OV */

			inf_n++;
			organafbuf[pntnum] = value;
			if(inf_n >= SPONT_ME_FLOAT_NUM)
			{
				sendbuf[7] = inf_n;			/* SQ=0 */
				nLength = inf_n * 8 + 10;
				sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_NC() Error %d! do close socket!\n",errno);
					
					return(false);
				}
				OutputDspInfo(0, "\nMeasure Value M_ME_NC: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
				FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNC: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	nLength = inf_n * 8 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_NC send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nMeasure Value M_ME_NC: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueTF(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;

	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_TF;			/* 36 type float without time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=2))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
 			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
 					value,organafbuf[pntnum],medef.databand[pntnum]);

			sendbuf[12+inf_n*15] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*15] = (unsigned char)((me_base_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*15] = (unsigned char)0;					/* i-addr high address */
#ifdef SUN_OS
			CrossCopy(&sendbuf[15+inf_n*15],&value,4);	/* N Value */
#else
			memcpy(&sendbuf[15+inf_n*15],&value,4);		/* N Value */						
#endif
			sendbuf[19+inf_n*15] = organastbuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 OV */
			FillCP56Time2aField(&sendbuf[20+inf_n*15]);

			inf_n++;
			organafbuf[pntnum] = value;
			if(inf_n >= SPONT_ME_FLOAT_TF_NUM)
			{
				sendbuf[7] = inf_n;			/* SQ=0 */
				nLength = inf_n * 15 + 10;
				sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_TF() Error %d! do close socket!\n",errno);
					return(false);
				}
				OutputDspInfo(0, "\nMeasure Value M_ME_TF: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
				FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTF: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	nLength = inf_n * 15 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_TF send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nMeasure Value M_ME_TF: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	CheckMeasureValueNC(void)
{
	unsigned char	stat;

	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);

			NoChangeData = 1;
			return(true);
		}
	}
	NoChangeData = 0;
	return(false);
}


int	MeasureValueNCCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	POINTER ptr;

	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NC;			/* 13 type float without time tag */

	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = ME_FLOAT_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(medef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + me_base_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + ME_FLOAT_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
/*
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			value = 0;
#ifdef SUN_OS
			CrossCopy(&sendbuf[15+inf_n*5],&value,4);
#else
			memcpy(&sendbuf[15+inf_n*5],&value,4);						
#endif
			sendbuf[19+inf_n*5] = 0;
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				value = 0;
#ifdef SUN_OS
			CrossCopy(&sendbuf[15+inf_n*5],&value,4);
#else
			memcpy(&sendbuf[15+inf_n*5],&value,4);					
#endif
			sendbuf[19+inf_n*5] = 0;
			inf_n++;
			continue;
		}
 */
		if(medef.ptr[pntnum].dev_id == 0)
		{
			value = 0;
#ifdef SUN_OS
			CrossCopy(&sendbuf[15+inf_n*5],&value,4);	/* N Value */
#else
			memcpy(&sendbuf[15+inf_n*5],&value,4);		/* N Value */						
#endif
			sendbuf[19+inf_n*5] = 0;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr,&stat,&value)==-1)
		{
			value = 0;
#ifdef SUN_OS
			CrossCopy(&sendbuf[15+inf_n*5],&value,4);	/* N Value */
#else
			memcpy(&sendbuf[15+inf_n*5],&value,4);		/* N Value */						
#endif
			sendbuf[19+inf_n*5] = 0;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
#ifdef SUN_OS
		CrossCopy(&sendbuf[15+inf_n*5],&value,4);		/* N Value */
#else
		memcpy(&sendbuf[15+inf_n*5],&value,4);			/* N Value */						
#endif
		sendbuf[19+inf_n*5] = organastbuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 OV */
		inf_n++;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNCCyc: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;	/* SQ=1 */

	nLength = inf_n * 5 + 13;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Cyc Measure Value M_ME_NC send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nCyc Measure Value M_ME_NC: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	ReadMeasureValueNC(unsigned short infoaddr,int sock)
{
	unsigned char	stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 18;			/* L; */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_NC;			/* 13 type float without time tag */

	sendbuf[7] = 1;				/* SQ=0 */

	sendbuf[8] = REQ;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum = infoaddr - me_base_addr;
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;			/* i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	if(GetMEValueByName(name_buf,&stat,&value)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
		value = 0.0f;			/* N Value */						
		stat = 0;			/* QDS IV NT SB BL RES3 OV */
	}
#ifdef SUN_OS
	CrossCopy(&sendbuf[15],&value,4);	/* N Value */
#else
	memcpy(&sendbuf[15],&value,4);		/* N Value */						
#endif
	sendbuf[19] = organastbuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 OV */
	organafbuf[pntnum] = value;

	byte_to_send = 20;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read Measure Value M_ME_NC send() Error %d! do close socket!\n",errno);
		return(false);
	}
	OutputDspInfo(0, "\nRead Measure Value M_ME_NC: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueND(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	unsigned char	stat;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_ND;		/* 21 type normalized without quality descriptor */
	sendbuf[8] = gi_cause;		/* cause of transmit:SPONT:3 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum]))
		{
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum]);

			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);

			sendbuf[12+inf_n*5] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*5] = (unsigned char)((me_base_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*5] = (unsigned char)0;					/* i-addr high address */
			sendbuf[15+inf_n*5] = (unsigned char)(anaorgcode & 0xFF);		/* N Value */							
			sendbuf[16+inf_n*5] = (unsigned char)(anaorgcode >> 8);			/* N Value */
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 48)
			{
				sendbuf[7] = inf_n;			/* SQ=0 */
				nLength = inf_n * 5 + 10;
				sendbuf[1] = nLength;			/* L byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_ND() Error %d! do close socket!\n",errno);
					return(false);
				}
				OutputDspInfo(0, "\nMeasure Value M_ME_ND: %u byte transmitted!",send_byte);
				
				PrintSendData(send_byte);
				
				UpdateSN();
				FillControlField(sendbuf+2);

				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueND: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	nLength = inf_n * 5 + 10;
	sendbuf[1] = nLength;		/* L byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Measure Value M_ME_ND send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nMeasure Value M_ME_ND: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNDCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength;
	POINTER	point;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;
	unsigned char	stat;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_ND;			/* 21 type normalized without quality descriptor */

	sendbuf[8] = gi_cause;			/* INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = ME_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(medef.number < startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + me_base_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			sendbuf[15+inf_n*2] = (unsigned char)0;		/* N Value */							
			sendbuf[16+inf_n*2] = (unsigned char)0;		/* N Value */
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		if(GetMEValueByName(name_buf,&stat,&value)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
				sendbuf[15+inf_n*2] = (unsigned char)0;		/* N Value */						
			sendbuf[16+inf_n*2] = (unsigned char)0;		/* N Value */
			inf_n++;
			continue;
		}
		GetPtIdByNameStr(name_buf, &point);
		if(point.data_type == IMP_TYPE)
		{
			anaorgcode = (short)value;
		}
		else
		{
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum]);
		}
		sendbuf[15+inf_n*2] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */							
		sendbuf[16+inf_n*2] = (unsigned char)(anaorgcode >> 8);		/* N Value */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNDCyc: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n | 0x80;	/* SQ=1 */

	nLength = inf_n * 2 + 13;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Cyc Measure Value M_ME_ND send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nCyc Measure Value M_ME_ND: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	ReadMeasureValueND(unsigned short infoaddr,int sock)
{
	unsigned char	stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 15;			/* L; */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_ME_ND;			/* 9 type normalized without time tag */

	sendbuf[7] = 1;				/* SQ=0 */

	sendbuf[8] = REQ;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum = infoaddr - me_base_addr;
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;			/* i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	if(GetMEValueByName(name_buf,&stat,&value)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
		stat = 0;				/* QDS IV NT SB BL RES3 OV */
		anaorgcode = (short)0;			/* N Value */
		value=(float)0;
	}
	anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum]);

	sendbuf[15] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */							
	sendbuf[16] = (unsigned char)(anaorgcode >> 8);		/* N Value */
	organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
	organabuf[pntnum] = anaorgcode;
	organafbuf[pntnum] = value;

	byte_to_send = 17;

	send_byte = send(sock,(char*)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Read Measure Value M_ME_ND send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nRead Measure Value M_ME_ND: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	IntegratedTotals(unsigned char gi_cause,unsigned char infogroup,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength;
	unsigned int	count;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_IT_NA;			/* 15 type Integrated Totals information */
	sendbuf[8] = gi_cause;			/* 37 cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = IT_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(itdef.number < startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + it_base_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + IT_GROUP_NUM;
	if(itdef.number < endnum)
		endnum = itdef.number;

	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if((strcmp(itdef.name[pntnum],"NULL")==0)||(itdef.TimeTag[pntnum]!=0))
		{
			continue;
		}
		strcpy(name_buf,itdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this IMP point in the database file!\n",name_buf);
				continue;
		}
		sendbuf[12+inf_n*8] = (it_base_addr + pntnum)& 0xFF;	/* information object low address */
		sendbuf[13+inf_n*8] = (it_base_addr + pntnum)>>8;	/* information object high address */
		sendbuf[14+inf_n*8] = 0;				/* information object high address */
		count = dbentry.var.imp.entry.rt_value;		/* count value */
#ifdef SUN_OS
		CrossCopy(&sendbuf[15+inf_n*8],&count,4);
#else
		memcpy(&sendbuf[15+inf_n*8],&count,4);
#endif
		sendbuf[19+inf_n*8] = count_sq;			/* QDS IV CA CY SQ */						
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		inf_n++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "IntegratedTotals: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n;

	nLength = inf_n * 8 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Integrated Totals send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nIntegrated Totals: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	IntegratedTotalsTA(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned short	millisec;
	unsigned char	inf_n;
	unsigned char	nLength;
	unsigned int	count;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_IT_TA;			/* 16 type Integrated Totals information */
	sendbuf[8] = gi_cause;			/* 37 cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = IT_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(itdef.number < startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + it_base_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + IT_GROUP_NUM;
	if(itdef.number < endnum)
		endnum = itdef.number;

	inf_n = 0;

	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if((strcmp(itdef.name[pntnum],"NULL")==0)||(itdef.TimeTag[pntnum]!=1))
		{
			continue;
		}
		strcpy(name_buf,itdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this IMP point in the database file!\n",name_buf);
				continue;
		}
		sendbuf[12+inf_n*8] = (it_base_addr + pntnum)& 0xFF;	/* information object low address */
		sendbuf[13+inf_n*8] = (it_base_addr + pntnum)>>8;	/* information object high address */
		sendbuf[14+inf_n*8] = 0;				/* information object high address */
		count = dbentry.var.imp.entry.rt_value;		/* count value */
#ifdef SUN_OS
		CrossCopy(&sendbuf[15+inf_n*8],&count,4);
#else
		memcpy(&sendbuf[15+inf_n*8],&count,4);
#endif
		sendbuf[19+inf_n*8] = count_sq;			/* QDS IV CA CY SQ */						
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		millisec = dbentry.var.imp.entry.update_time.second * 1000 + dbentry.var.imp.entry.update_time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[20+inf_n*11],&millisec,2);	/* ASDU common address */
#else
		memcpy(&sendbuf[20+inf_n*11],&millisec,2);	/* Milliseconds */
#endif
		sendbuf[22+inf_n*11] = dbentry.var.imp.entry.update_time.minute;	/* Minutes */
		inf_n ++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "IntegratedTotalsTA: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n;

	nLength = inf_n * 11 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Integrated Totals TA send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nIntegrated Totals TA: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	IntegratedTotalsTB(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned short	millisec;
	unsigned char	inf_n;
	unsigned char	nLength;
	unsigned int	count;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_IT_TB;			/* 37 type Integrated Totals information */
	sendbuf[8] = gi_cause;			/* 37 cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = IT_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(itdef.number < startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + it_base_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + IT_GROUP_NUM;
	if(itdef.number < endnum)
		endnum = itdef.number;

	inf_n = 0;

	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if((strcmp(itdef.name[pntnum],"NULL")==0)||(itdef.TimeTag[pntnum]!=2))
		{
			continue;
		}
		strcpy(name_buf,itdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this IMP point in the database file!\n",name_buf);
				continue;
		}
		sendbuf[12+inf_n*15] = (it_base_addr + pntnum)& 0xFF;	/* i-addr information object low address */
		sendbuf[13+inf_n*15] = (it_base_addr + pntnum)>>8;	/* i-addr information object high address */
		sendbuf[14+inf_n*15] = 0;				/* i-addr information object high address */
		count = dbentry.var.imp.entry.rt_value;		/* count value */
#ifdef SUN_OS
		CrossCopy(&sendbuf[15+inf_n*8],&count,4);
#else
		memcpy(&sendbuf[15+inf_n*8],&count,4);
#endif
		sendbuf[19+inf_n*15] = count_sq;		/* QDS IV CA CY SQ */							
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		millisec = dbentry.var.imp.entry.update_time.second * 1000 + dbentry.var.imp.entry.update_time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[20+inf_n*15],&millisec,2);	/* ASDU common address */
#else
		memcpy(&sendbuf[20+inf_n*15],&millisec,2);	/* Milliseconds */
#endif
		sendbuf[22+inf_n*15] = dbentry.var.imp.entry.update_time.minute;	/* Minutes */
		sendbuf[23+inf_n*15] = dbentry.var.imp.entry.update_time.hour;	/* Hour */
/*		n = sys_tm.wDayOfWeek << 5;
		n = n & 0xE0;
*/
		sendbuf[24+inf_n*15] = dbentry.var.imp.entry.update_time.day;		/* DayofWeek[6..8] day[1..5] */
		sendbuf[25+inf_n*15] = dbentry.var.imp.entry.update_time.month;	/* Month */
		sendbuf[26+inf_n*15] = (unsigned char)(dbentry.var.imp.entry.update_time.year%100);	/* Year */
		inf_n ++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "IntegratedTotalsTA: NoRequestData\n");
		NoChangeData = 1;
		return(true);
	}
	sendbuf[7] = inf_n;

	nLength = inf_n * 15 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 2; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "Integrated Totals TB send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nIntegrated Totals TB: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);
}

int	EndofInitialization(int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* Length */
	FillControlField(sendbuf+2);

	sendbuf[6] = M_EI_NA;			/* 70 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = 4;				/* 4 Actterm cause of transmit:end */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = 0;			/* COI 0 local power on	 */					

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		OutputDspInfo(OUTPUT_ERROR, "End of Initialization send() Error %d! do close socket!\n",errno);
		
		return(false);
	}
	OutputDspInfo(0, "\nEnd of Initialization: %u byte transmitted!",send_byte);

	PrintSendData(send_byte);
	
	UpdateSN();
	NoChangeData = 0;
	return(true);	
}

int	RespondGroupInterrogation(unsigned char gi_qoi,int sock)
{
	int ret;
	switch	( gi_qoi ) 
	{
	case  21:

		if(GiType==1)
			ret = SinglePointInformCyc(INTRO1,m_cRespICGrpNum,sock);	/* 21 respond to Group 0 (1) Interrogation */
		else
			ret = SinglePointGroupCyc(INTRO1, m_cRespICGrpNum,sock);	/* 21 respond to Group 0 (1) Interrogation M_PS_NA */

		ret = DoublePointInformCyc(INTRO1,m_cRespICGrpNum,sock);	/* 21 respond to Group 0 (1) general Interrogation */
		break;

	case  22:

		if(GiType==1)
			ret = SinglePointInformCyc(INTRO2,m_cRespICGrpNum,sock);	/* 22 respond to Group 1 (2) Interrogation */
		else
			ret = SinglePointGroupCyc(INTRO2, m_cRespICGrpNum,sock);	/* 22 respond to Group 1 (2) Interrogation M_PS_NA */
		ret = DoublePointInformCyc(INTRO2,m_cRespICGrpNum,sock);	/* 22 respond to Group 1 (2) general Interrogation */
		break;

	case  23:

		if(GiType==1)
			ret = SinglePointInformCyc(INTRO3,m_cRespICGrpNum,sock);	/* 23 respond to Group 2 (3) general Interrogation */
		else
			ret = SinglePointGroupCyc(INTRO3, m_cRespICGrpNum,sock);	/* 23 respond to Group 2 (3) Interrogation M_PS_NA */
		ret = DoublePointInformCyc(INTRO3,m_cRespICGrpNum,sock);	/* 23 respond to Group 2 (3) general Interrogation */
		break;

	case  24:
		if(GiType==1)
			ret = SinglePointInformCyc(INTRO4,m_cRespICGrpNum,sock);	/* 24 respond to Group 3 (4) general Interrogation */
		else
			ret = SinglePointGroupCyc(INTRO4, m_cRespICGrpNum,sock);	/* 24 respond to Group 3 (4) Interrogation M_PS_NA */
		ret = DoublePointInformCyc(INTRO4,m_cRespICGrpNum,sock);  /* 24 respond to Group 3 (4) general Interrogation */
		break;

	case  29:

		if(MeASDUType==0)
			ret = MeasureValueNACyc(INTRO9,m_cRespICGrpNum,sock);	/* 29 respond to Group 0 (9) general Interrogation M_ME_NA */
		else if(MeASDUType==1)
			ret = MeasureValueNBCyc(INTRO9,m_cRespICGrpNum,sock);	/* 29 respond to Group 0 (9) general Interrogation M_ME_NB */
		else if(MeASDUType==2)
			ret = MeasureValueNDCyc(INTRO9,m_cRespICGrpNum,sock);	/* 29 respond to Group 0 (9) general Interrogation M_ME_ND */
		else
			ret = MeasureValueNCCyc(INTRO9,m_cRespICGrpNum,sock);	/* 29 respond to Group 0 (9) general Interrogation M_ME_NC */
		break;

	case  30:

		if(MeASDUType==0)
			ret = MeasureValueNACyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 1 (10) general Interrogation M_ME_NA */
		else if(MeASDUType==1)
			ret = MeasureValueNBCyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 1 (10) general Interrogation M_ME_NB */
		else if(MeASDUType==2)
			ret = MeasureValueNDCyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 1 (10) general Interrogation M_ME_ND */
		else
			ret = MeasureValueNCCyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 1 (10) general Interrogation M_ME_NC */
		break;

	case  31:

		if(MeASDUType==0)
			ret = MeasureValueNACyc(INTRO11,m_cRespICGrpNum,sock);	/* 31 respond to Group 2 (11) general Interrogation M_ME_NA */
		else if(MeASDUType==1)
			ret = MeasureValueNBCyc(INTRO11,m_cRespICGrpNum,sock);	/* 31 respond to Group 2 (11) general Interrogation M_ME_NB */
		else if(MeASDUType==2)
			ret = MeasureValueNDCyc(INTRO11,m_cRespICGrpNum,sock);	/* 31 respond to Group 2 (9) general Interrogation M_ME_ND */
		else
			ret = MeasureValueNCCyc(INTRO11,m_cRespICGrpNum,sock);	/* 31 respond to Group 2 (9) general Interrogation M_ME_NC */
		break;

	default :
		OutputDspInfo(0, "No this group data qoi=%d\n",gi_qoi);

		NoChangeData = 1;
		break;			
	}/* switch(gi_cause)  */
	if(NoChangeData == 1)
	{
		icgroupflag = 0;
		m_cRespICGrpNum = 0;
		ret = GeneralInterrogationAck(ACTTERM,gi_qoi,sock);	/* 10 ActTrem to general Interrogation */
	}
	m_cRespICGrpNum++;
	return(ret);
}

int CheckIndPtState(int sock)
{
	NoChangeData= 0;
	if(!SinglePointInform(SPONT,sock))
		return(false);
	
	NoChangeData= 0;
	if(!DoublePointInform(SPONT,sock))
		return(false);
	return true;
}


int RespondGeneralInterrogation(int sock)
{
	int	ret;
	int	unack_num;

	if(soe_overflow_Sendflag ==1)
	{
		ret = SequenceofEvent(sock);
		if(NoChangeData == 0)
			return(ret);
		else
			soe_overflow_Sendflag = 0;
	}

	if(nr_send >= nr_ack)
		unack_num = nr_send - nr_ack;
	else
		unack_num = 0x10000 + nr_send - nr_ack;
	if(unack_num >= K*2)
		return(true);

	NoChangeData = 0;
	while(m_cRespGenInterroGrpNum < 2000)
	{
		if(!(m_cRespGenInterroGrpNum%7))
		{
			if(!CheckIndPtState(sock))
			return false;
		}		

		if(GiType==1)
		{
			ret = SinglePointInformCyc(INTROGEN, m_cRespGenInterroGrpNum,sock);	/* 20 respond to general Interrogation M_SP_NA */
			if(ret == false)
				return(false);
		}
		else
		{
			ret = SinglePointGroupCyc(INTROGEN, m_cRespGenInterroGrpNum,sock);	/* 20 respond to general Interrogation M_PS_NA */
			if(ret == false)
				return(false);
		}
		OutputDspInfo(0, "++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
		

		if(NoChangeData == 1)
		{
			m_cRespGenInterroGrpNum = 2000;
		}
		else
		{
			m_cRespGenInterroGrpNum++;
		}
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	while(m_cRespGenInterroGrpNum < 4000)
	{
		if(!(m_cRespGenInterroGrpNum%7))
		{
			if(!CheckIndPtState(sock))
				return false;
		}

		ret = DoublePointInformCyc((unsigned char)INTROGEN, (unsigned short)(m_cRespGenInterroGrpNum-2000),sock);	/* 20 respond to general Interrogation DP */
		if(ret == false)
			return(false);
		OutputDspInfo(0, "++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
		
		if(NoChangeData == 1)
		{
			m_cRespGenInterroGrpNum = 4000;
		}
		else
		{
			m_cRespGenInterroGrpNum++;
		}
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	while(m_cRespGenInterroGrpNum < 6000)
	{
		if(!(m_cRespGenInterroGrpNum%7))
		{
			if(!CheckIndPtState(sock))
				return false;
		}
		
		ret = StepPositionInformCyc((unsigned char)INTROGEN, (unsigned short)(m_cRespGenInterroGrpNum-4000),sock);	/* 20 respond to general Interrogation ST */
		if(ret == false)
			return(false);
		OutputDspInfo(0, "++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
		
		if(NoChangeData == 1)
		{
			m_cRespGenInterroGrpNum = 6000;
		}
		else
		{
			m_cRespGenInterroGrpNum++;
		}
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	while(m_cRespGenInterroGrpNum < 8000)
	{
		if(!(m_cRespGenInterroGrpNum%7))
		{
			if(!CheckIndPtState(sock))
				return false;
		}
		
		if(MeASDUType==0)
		{
			ret = MeasureValueNACyc((unsigned char)INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000),sock);	/* respond to general Interrogation M_ME_NA */
			if(ret == false)
				return(false);
		}
		else if(MeASDUType==1)
		{
			ret = MeasureValueNBCyc((unsigned char)INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000),sock);	/* respond to general Interrogation M_ME_NB */
			if(ret == false)
				return(false);
		}
		else if(MeASDUType==2)
		{
			ret = MeasureValueNDCyc((unsigned char)INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000),sock);	/* respond to general Interrogation M_ME_ND */
			if(ret == false)
				return(false);
		}
		else
		{
			ret = MeasureValueNCCyc((unsigned char)INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000),sock);	/* respond to general Interrogation M_ME_NC */
			if(ret == false)
				return(false);
		}
		OutputDspInfo(0, "++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
		
		if(NoChangeData != 1)
		{
			m_cRespGenInterroGrpNum++;
		}
		else
			break;
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	m_cRespGenInterroGrpNum = 0;
	interrogationflag = 0;
	ret = GeneralInterrogationAck(ACTTERM,(unsigned char)INTROGEN,sock);	/* 10 ActTrem to general Interrogation */
	OutputDspInfo(0, "++++General Interrogation End! m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
	
	NoChangeData = 0;
	return(ret);
}

int ScanDataTransmit(int sock)
{
	int	ret;
	int	unack_num;
	OutputDspInfo(0, "++++Scan m_cScanGrpNum =%d\n",m_cScanGrpNum);


	if(nr_send >= nr_ack)
		unack_num = nr_send - nr_ack;
	else
		unack_num = 0x10000 + nr_send - nr_ack;
	if(unack_num >= K*2)
		return(true);

	NoChangeData = 0;
	while(m_cScanGrpNum < 2000)
	{
		if(GiType==1)
			ret = SinglePointInformCyc((unsigned char)BACK, m_cScanGrpNum,sock);	/* 1 respond to back scan M_SP_NA */
		else
			ret = SinglePointGroupCyc((unsigned char)BACK, m_cScanGrpNum,sock);	/* 1 respond to back scan M_PS_NA */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 2000;
		}
		else
		{
			m_cScanGrpNum++;
		}
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	while(m_cScanGrpNum < 4000)
	{
		ret = DoublePointInformCyc((unsigned char)BACK, (unsigned short)(m_cScanGrpNum-2000),sock);	/* 1 respond to back scan DP */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 4000;
		}
		else
		{
			m_cScanGrpNum++;
		}
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	while(m_cScanGrpNum < 6000)
	{
		ret = StepPositionInformCyc((unsigned char)BACK, (unsigned short)(m_cScanGrpNum-4000),sock);	/* 20 respond to general Interrogation DP */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 6000;
		}
		else
		{
			m_cScanGrpNum++;
		}
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	while(m_cScanGrpNum < 8000)
	{
		if(MeASDUType==0)
		{
			ret = MeasureValueNACyc((unsigned char)BACK,(unsigned short)(m_cScanGrpNum-6000),sock);	/* 9 respond to back scan M_ME_NA */
			if(ret == false)
				return(false);
		}
		else if(MeASDUType==1)
		{
			ret = MeasureValueNBCyc((unsigned char)BACK,(unsigned short)(m_cScanGrpNum-6000),sock);	/* 9 respond to back scan M_ME_NA */
			if(ret == false)
				return(false);
		}
		else if(MeASDUType==2)
		{
			ret = MeasureValueNDCyc((unsigned char)BACK,(unsigned short)(m_cScanGrpNum-6000),sock);	/* 9 respond to back scan M_ME_NA */
			if(ret == false)
				return(false);
		}
		else
		{
			ret = MeasureValueNCCyc((unsigned char)BACK,(unsigned short)(m_cScanGrpNum-6000),sock);	/* 9 respond to back scan M_ME_NA */
			if(ret == false)
				return(false);
		}
		if(NoChangeData != 1)
		{
			m_cScanGrpNum++;
		}
		else
			break;
		if(nr_send >= nr_ack)
			unack_num = nr_send - nr_ack;
		else
			unack_num = 0x10000 + nr_send - nr_ack;
		if(unack_num >= K*2)
			return(true);
	}
	OutputDspInfo(0, "++++Scan End! m_cScanGrpNum =%d\n",m_cScanGrpNum);

	m_cScanGrpNum = 0;
	scanflag = 0;
	return(ret);
}

int	RespondParamInterrogation(int sockfd)
{
	short	setvalue;
	float	fvalue;
	double	fdata;
	unsigned short	pntnum,pntaddr;

	if(m_cRespParamNum < 1000)
	{
		while(m_cRespParamNum<medef.number)
		{
			if(strcmp(medef.name[m_cRespParamNum],"NULL")==0)
			{
				m_cRespParamNum++;
				continue;
			}
			fvalue=medef.databand[m_cRespParamNum];

			if(fvalue <= medef.lorange[m_cRespParamNum])
			{
				if(MeType == 0)
					setvalue = 0;
				else
					setvalue = (short)0x8000;
			}
			else	if(fvalue >= medef.hirange[m_cRespParamNum])
				setvalue = 0x7FFF;
			else
			{
				if(MeType == 0)
				{
					fdata=(fvalue-medef.lorange[m_cRespParamNum])*32767.0/
						(medef.hirange[m_cRespParamNum]-medef.lorange[m_cRespParamNum]);
					setvalue=(short)fdata;
					if((fdata-setvalue)>=0.5)
						setvalue++;
					else if((fdata-setvalue)<=-0.5)
						setvalue--;
				}
				else
				{
					fdata=(fvalue-(medef.hirange[m_cRespParamNum]+medef.lorange[m_cRespParamNum])/2.0)*65535.0/
						(medef.hirange[m_cRespParamNum]-medef.lorange[m_cRespParamNum]);
					setvalue=(short)fdata;
					if((fdata-setvalue)>=0.5)
						setvalue++;
					else if((fdata-setvalue)<=-0.5)
						setvalue--;
				}
			}
			ParamValueNAAck((unsigned char)INTRO14,(unsigned char)1,(unsigned short)(m_cRespParamNum+pm_base_addr),setvalue,sockfd);
			m_cRespParamNum++;
			OutputDspInfo(0, "++++m_cRespParamNum =%d\n",m_cRespParamNum);

			return(true);
		}
		m_cRespParamNum = 1000;
	}
	if(m_cRespParamNum < 2000)
	{
		pntnum = m_cRespParamNum - 1000;
		while(pntnum<medef.number)
		{
			if(strcmp(medef.name[pntnum],"NULL")==0)
			{
				m_cRespParamNum++;
				continue;
			}
			setvalue=(short)(medef.databand[pntnum] * medef.scale[pntnum]);
			pntaddr = pntnum;
			ParamValueNBAck((unsigned char)INTRO14,(unsigned char)1,(unsigned short)(pntaddr+pm_base_addr),setvalue,sockfd);
			m_cRespParamNum++;
			OutputDspInfo(0, "++++m_cRespParamNum =%d pntnum=%d\n",m_cRespParamNum,pntnum);

			return(true);
		}
		m_cRespParamNum = 2000;
	}
	if(m_cRespParamNum < 3000)
	{
		pntnum = m_cRespParamNum - 2000;
		while(pntnum<medef.number)
		{
			if(strcmp(medef.name[pntnum],"NULL")==0)
			{
				m_cRespParamNum++;
				continue;
			}
			pntaddr = pntnum;
			ParamValueNCAck((unsigned char)INTRO14,(unsigned char)1,(unsigned short)(pntaddr+pm_base_addr),medef.databand[m_cRespParamNum],sockfd);
			m_cRespParamNum++;
			OutputDspInfo(0, "++++m_cRespParamNum =%d pntnum=%d\n",m_cRespParamNum,pntnum);

			return(true);
		}
		Paramflag = 0;
		m_cRespParamNum = 0;
		GeneralInterrogationAck(ACTTERM,INTRO14,sockfd);	/* 10 ActTrem to general Interrogation */
	}
	return(true);
}

int	RespondCountGroupInterrogation(unsigned char gi_qcc,int sock)
{
	unsigned char	grp_num,end_num;
	int	ret;

	end_num = (gi_qcc & 0x3F) * CI_GROUP_NUM;
	if(end_num > itdef.number)
		end_num = (unsigned char)itdef.number;

	grp_num = (unsigned char)end_num / IT_GROUP_NUM;
	if(end_num % IT_GROUP_NUM)
		grp_num++;

	while(m_cRespCIGrpNum < grp_num)
	{
		switch	(gi_qcc & 0x3F) 
		{
		case  1:
			if(ITGrpType==0)
			{
				ITGrpType = 1;
				ret = IntegratedTotals((unsigned char)REQCO1,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:38 respond to group 1 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				ret = IntegratedTotalsTA((unsigned char)REQCO1,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:38 respond to group 1 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			else
			{
				ITGrpType = 0;
				ret = IntegratedTotalsTB((unsigned char)REQCO1,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:38 respond to group 1 Counter Interrogation */
				if(ret == false)
					return(false);
				m_cRespCIGrpNum++;
				if(NoChangeData==1)
					break;
				else
				{
					continue;
				}
			}
			break;

		case  2:
			if(ITGrpType==0)
			{
				ITGrpType = 1;
				ret = IntegratedTotals((unsigned char)REQCO2,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:39 respond to group 2 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				ret = IntegratedTotalsTA((unsigned char)REQCO2,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:39 respond to group 2 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			else
			{
				ITGrpType = 0;
				ret = IntegratedTotalsTB((unsigned char)REQCO2,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:39 respond to group 2 Counter Interrogation */
				if(ret == false)
					return(false);
				m_cRespCountGrpNum++;
				if(NoChangeData==1)
					break;
				else
				{
					continue;
				}
			}
			break;

		case  3:
			if(ITGrpType==0)
			{
				ITGrpType = 1;
				ret = IntegratedTotals((unsigned char)REQCO3,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:40 respond to group 3 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				ret = IntegratedTotalsTA((unsigned char)REQCO3,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:40 respond to group 3 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			else
			{
				ITGrpType = 0;
				ret = IntegratedTotalsTB((unsigned char)REQCO3,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:40 respond to group 3 Counter Interrogation */
				if(ret == false)
					return(false);
				m_cRespCIGrpNum++;
				if(NoChangeData==1)
					break;
				else
				{
					continue;
				}
			}
			break;

		case  4:
			if(ITGrpType==0)
			{
				ITGrpType = 1;
				ret = IntegratedTotals((unsigned char)REQCO4,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:41 respond to group 4 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				ret = IntegratedTotalsTA((unsigned char)REQCO4,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:41 respond to group 4 Counter Interrogation */
				if(ret == false)
					return(false);
			}
			else
			{
				ITGrpType = 0;
				ret = IntegratedTotalsTB((unsigned char)REQCO4,(unsigned char)m_cRespCIGrpNum,sock);	/* cause:41 respond to group 4 Counter Interrogation */
				if(ret == false)
					return(false);
				m_cRespCIGrpNum++;
				if(NoChangeData==1)
					break;
				else
				{
					continue;
				}
			}
			break;

		}
	}
	cigroupflag = 0;
	m_cRespCIGrpNum = 0;
	CounterInterrogationAck(ACTTERM,gi_qcc,sock);	/* 10 ActTrem to general Interrogation */
	return(true);
}
						  
int RespondCountInterrogation(int sock)
{
	int	ret;
	short	grpno;

	grpno = itdef.number/IT_GROUP_NUM;
	if(itdef.number%IT_GROUP_NUM)
		grpno ++;
	while(m_cRespCountGrpNum < grpno)
	{
		if(ITGrpType==0)
		{
			ITGrpType = 1;
			ret = IntegratedTotals((unsigned char)REQCOGCN,(unsigned char)m_cRespCountGrpNum,sock);	/* cause:37 respond to Counter Interrogation */
			if(ret == false)
				return(false);
		}
		if(ITGrpType==1)
		{
			ITGrpType = 2;
			ret = IntegratedTotalsTA((unsigned char)REQCOGCN,(unsigned char)m_cRespCountGrpNum,sock);	/* cause:37 respond to Counter Interrogation */
			if(ret == false)
				return(false);
		}
		else
		{
			ITGrpType = 0;
			ret = IntegratedTotalsTB((unsigned char)REQCOGCN,(unsigned char)m_cRespCountGrpNum,sock);	/* cause:37 respond to Counter Interrogation */
			if(ret == false)
				return(false);
			m_cRespCountGrpNum++;
			if(NoChangeData==1)
				break;
			else
			{
				continue;
			}
		}
	}
	m_cRespCountGrpNum = 0;
	counterflag = 0;
	ret = CounterInterrogationAck(ACTTERM,cibuf.qcc,sock);		/* 10 ActTrem to Counter Interrogation qcc=5 */
	return(ret);
}

void ResetOrgData(void)
{
	int	pntnum;
	POINTER	ptr;
	unsigned char stat;
	
	for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Update orgbuf for SP status Change */
	{
		if(spdef.ptr[pntnum].dev_id == 0)
		{
			orgspbuf[pntnum] = 0;
			continue;
		}
		ptr = spdef.ptr[pntnum];
		if(GetSPStatById(&ptr,&stat)==-1)
		{
			orgspbuf[pntnum] = 0;
			continue;
		}
		else
		{
			orgspbuf[pntnum] = stat;
		}
	}
	for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* update orgbuf for DP status */
	{
		if(dpdef.ptr[pntnum].dev_id == 0)
		{
			continue;
		}
		ptr = dpdef.ptr[pntnum];
		if(GetDPStatById(&ptr,&stat)==-1)
		{
			orgdpbuf[pntnum] = 0;
			continue;
		}
		else
		{
			orgdpbuf[pntnum] = stat;
		}
	}
	for(pntnum=0;pntnum<medef.number;pntnum++)		/* update orgbuf for st status */
	{
		float	fValue;
		
		if(medef.ptr[pntnum].dev_id == 0)
		{
			continue;
		}
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr, &stat, &fValue)==-1)
		{
			organafbuf[pntnum] = 0;
			organastbuf[pntnum] = 0;
			continue;
		}
		else
		{
			organafbuf[pntnum] = fValue;
			organastbuf[pntnum] = stat;
		}
	}
	
}


	  
int	RespondUserDataClass1(int sock)
{
	int ret;

	NoChangeData = 0;

	if(MirrorNAckflag)
	{
		MirrorNAckflag = 0;
		ret = MirrorNAck(mirbuf.cot,mirbuf.pnt,sock);	/* Mirror NACK */
		return(ret);
	}

	NoChangeData = 0;
	if(sp_cos == 1)
	{
		ret = SinglePointInform(SPONT,sock);
		if(!NoChangeData)
			return(ret);

		NoChangeData = 0;
		ret = DoublePointInform(SPONT,sock);
		if(!NoChangeData)
			return(ret);

		NoChangeData = 0;
		ret = StepPositionInform(SPONT,sock);
		if(!NoChangeData)
			return(ret);

	}
	else
	{
		ret = SequenceofEvent(sock);
		if(ret == false)
			return(false);
	}

	if(interrogationflag)
	{
		ret = RespondGeneralInterrogation(sock);	/* general */
		return(ret);
	}

	if(icgroupflag)
	{
		ret = RespondGroupInterrogation(icbuf.qoi,sock);	/* group??? */
		return(ret);
	}

	NoChangeData = 0;

	if(MeASDUType==0)
	{
		ret = MeasureValueNA(SPONT,sock);
		if(ret == false)
			return(ret);
/*
		ret = MeasureValueTD(SPONT,sock);
		if(ret == false)
			return(ret);
*/
	}
	else if(MeASDUType==1)
	{
		ret = MeasureValueNB(SPONT,sock);
		if(ret == false)
			return(ret);
/*
		ret = MeasureValueTE(SPONT,sock);
		if(ret == false)
			return(ret);
*/
	}
	else if(MeASDUType==2)
	{
		ret = MeasureValueND(SPONT,sock);
		if(ret == false)
			return(ret);
	}
	else
	{
		ret = MeasureValueNC(SPONT,sock);
		if(ret == false)
			return(ret);
/*
		ret = MeasureValueTF(SPONT,sock);
		if(ret == false)
			return(ret);
*/
	}

	if(Paramflag)
	{
		ret = RespondParamInterrogation(sock);
		return(ret);
	}

	if(icgroupflag)
	{
		ret = RespondGroupInterrogation(icbuf.qoi,sock);
		return(ret);
	}

	if(counterflag)
	{
		ret = RespondCountInterrogation(sock);
		return(ret);
	}

	if(cigroupflag)
	{
		ret = RespondCountGroupInterrogation(cibuf.qcc,sock);
		return(ret);
	}

	if(sp_cos == 1)
	{
		ret = SequenceofEvent(sock);
		if(NoChangeData == 0)
			return(ret);
	}
/* Scan data */
// 	if(scanflag)
// 	{
// 		OutputDspInfo(0, "!!!ScanDataTransmit\n");
// 
// 		ret = ScanDataTransmit(sock);
// 		return(ret);
// 	}
	return(ret);
}



int SequenceofEvent(int sockfd)
{
	unsigned char	nTypeID,nCause;
	unsigned short	soepnt;
	int			ret;

	NoChangeData = 0;
	soepnt = soebuf.sendpnt;
	nCause = SPONT;
	while(soepnt != soebuf.bufpnt)
	{
		nTypeID = soebuf.soerec[soepnt].typeId;

		switch	( nTypeID ) 
		{
		case  M_SP_TA:

			ret = SinglePointInformTA(nCause,sockfd);
			return(ret);

		case  M_SP_TB:

			ret = SinglePointInformTB(nCause,sockfd);
			return(ret);

		case  M_DP_TA:

			ret = DoublePointInformTA(nCause,sockfd);
			return(ret);

		case  M_DP_TB:

			ret = DoublePointInformTB(nCause,sockfd);
			return(ret);

		case  M_ST_TA:

			ret = StepPositionInformTA(nCause,sockfd);
			return(ret);

		case  M_ST_TB:

			ret = StepPositionInformTB(nCause,sockfd);
			return(ret);

		default:
			OutputDspInfo(OUTPUT_WARN, "SOE No this TypeID = %d\n",nTypeID);
			
			break;			
		}/* switch(nTypeID)  */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		soebuf.sendpnt = soepnt;
	}
	NoChangeData = 1;
	return(true);
}

int	GetSPStatById(POINTER *ptr,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	unsigned char	stat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "No this SP point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		*pstat =0;	/* QDS IV NT SB BL RES3 SPI */
		return(-1);
	}
	stat=dbentry.var.ind.status.rt_state;
	if(stat != 1)
		stat = 0;	/* OFF:QDS IV NT SB BL RES3 SPI */

	if(dbentry.var.ind.status.man_set)
		stat = stat | 0x20;			/* SB=1 */
	if(dbentry.var.ind.status.chan_fail)
		stat = stat | 0xC0;			/* IV=1; NT=1 */
	if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;			/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES3 SPI */
	return(0);
}

int	GetDPStatById(POINTER *ptr,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	unsigned char	stat;
	IND_DATA	dbstat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "No this DP point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		*pstat =0;	/* QDS IV NT SB BL RES2 DPI */
		return(-1);
	}
	dbstat=dbentry.var.ind.status;
	if(dbstat.state == 0)
		stat = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 1)
		stat = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 2)
		stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
	else
		stat = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

	if(dbstat.man_set)
		stat = stat | 0x20;			/* SB=1 */
	if(dbstat.chan_fail)
		stat = stat | 0xC0;			/* IV=1; NT=1 */
	if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;			/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES2 DPI */
	return(0);
}

int	GetSTStatById(POINTER *ptr,unsigned char *pvti,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	unsigned char	stat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "No this ST point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		*pstat = *pvti = 0;	/* QDS IV NT SB BL RES3 OV ; VTI */
		return(-1);
	}
	*pvti=dbentry.var.ind.status.state;

	stat = 0;
	if(dbentry.var.ind.status.man_set)
		stat = stat | 0x20;			/* SB=1 */
	if(dbentry.var.ind.status.chan_fail)
		stat = stat | 0xC0;			/* IV=1; NT=1 */
	if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;			/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES3 OV */
	return(0);
}

int	GetMEValueById(POINTER *ptr,unsigned char *pstat,float *pValue)
{
	DMS_COMMON		dbentry;
	unsigned char	stat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "No this ME point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		*pstat = 0;
		*pValue = 0.0f;
		return(-1);
	}
	if(ptr->data_type == IMP_TYPE)
	{
//		*pValue=dbentry.var.imp.entry.rt_value;   //yujji091027
		*pValue = dbentry.var.imp.entry.d_accum;

		if(MeASDUType == 3)
//			*pValue=*pValue/100.0;		//如果是短浮点数上送，为防溢出将脉冲个数除100上送  
			*pValue=*pValue;		//瀑布沟与大渡河集控改为上送电度表读数 yujji091027
		stat = 0;
		if(dbentry.var.imp.entry.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.imp.entry.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		*pValue=(float)dbentry.var.anlg.value.data/1000.0;
		stat = 0;
		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
		if(dbentry.var.anlg.value.status.overflow)
			stat = stat | 0x01;		/* OV=1 */

	}
	*pstat = stat;
	return(0);
}

int	GetIMPValueById(POINTER *ptr,unsigned char *pstat,unsigned int *pValue)
{
	DMS_COMMON		dbentry;
	unsigned char	stat;

	if(ReadEntryById(ptr,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "No this IMP point %d.%d.%d.%d in the database file!\n",
			ptr->stn_id,ptr->dev_id,ptr->data_type,ptr->pt_id);
		*pstat = 0;
		*pValue = 0;
		return(-1);
	}
	if(ptr->data_type == IMP_TYPE)
	{
		*pValue=dbentry.var.imp.entry.rt_value;
		stat = 0;
		if(dbentry.var.imp.entry.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.imp.entry.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		*pValue=(float)dbentry.var.anlg.value.data/1000.0;
		stat = 0;
		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	*pstat = stat;
	return(0);
}

int	GetSPStatByName(char *pName,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
		*pstat =0;	/* QDS IV NT SB BL RES3 SPI */
		return(-1);
	}
	stat=dbentry.var.ind.status.rt_state;
	if(stat != 1)
		stat = 0;	/* OFF:QDS IV NT SB BL RES3 SPI */

	if(dbentry.var.ind.status.man_set)
		stat = stat | 0x20;			/* SB=1 */
	if(dbentry.var.ind.status.chan_fail)
		stat = stat | 0xC0;			/* IV=1; NT=1 */
	if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;			/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES3 SPI */
	return(0);
}

int	GetDPStatByName(char *pName,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;
	IND_DATA	dbstat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this DP point in the database file!\n",name_buf);
		*pstat =0;	/* QDS IV NT SB BL RES2 DPI */
		return(-1);
	}
	dbstat=dbentry.var.ind.status;
	if(dbstat.state == 0)
		stat = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 1)
		stat = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
	else if(dbstat.state == 2)
		stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
	else
		stat = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

	if(dbstat.man_set)
		stat = stat | 0x20;			/* SB=1 */
	if(dbstat.chan_fail)
		stat = stat | 0xC0;			/* IV=1; NT=1 */
	if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;			/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES2 DPI */
	return(0);
}

int	GetSTStatByName(char *pName,unsigned char *pvti,unsigned char *pstat)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ST point in the database file!\n",name_buf);
		*pstat = *pvti = 0;	/* QDS IV NT SB BL RES3 OV ; VTI */
		return(-1);
	}
	*pvti=dbentry.var.ind.status.state;

	stat = 0;
	if(dbentry.var.ind.status.man_set)
		stat = stat | 0x20;			/* SB=1 */
	if(dbentry.var.ind.status.chan_fail)
		stat = stat | 0xC0;			/* IV=1; NT=1 */
	if(dbentry.fixed.iost.scanEnable==0)
		stat = stat | 0x10;			/* BL=1 */
	*pstat =stat;	/* QDS IV NT SB BL RES3 OV */
	return(0);
}

int	GetMEValueByName(char *pName,unsigned char *pstat,float *pValue)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
		*pstat = 0;
		*pValue = 0.0f;
		return(-1);
	}
	if(dbentry.point.data_type == IMP_TYPE)
	{
		*pValue=dbentry.var.imp.entry.rt_value;
		stat = 0;
		if(dbentry.var.imp.entry.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.imp.entry.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		*pValue=(float)dbentry.var.anlg.value.data/1000.0;
		stat = 0;
		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
		if(dbentry.var.anlg.value.status.overflow)
			stat = stat | 0x01;		/* OV=1 */

	}
	*pstat = stat;
	return(0);
}

short	GetNormalizedValue(float value,float lorange,float hirange)
{
	short			anaorgcode;
	float			fdata;

	if(value <= lorange)
		anaorgcode = (short)me_code;
	else if(value >= hirange)
		anaorgcode = (short)HI_CODE;
	else
	{
		fdata=(value-lorange)*(HI_CODE - me_code)/(hirange-lorange) + me_code;
		anaorgcode=(short)fdata;
		if((fdata-anaorgcode)>=0.5)
			anaorgcode++;
		else if((fdata-anaorgcode)<=-0.5)
			anaorgcode--;
	}
	return(anaorgcode);
}

int	GetIMPValueByName(char *pName,unsigned char *pstat,unsigned int *pValue)
{
	DMS_COMMON		dbentry;
	char	name_buf[NAME_SIZE];
	unsigned char	stat;

	strcpy(name_buf,pName);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this IMP point in the database file!\n",name_buf);
		*pstat = 0;
		*pValue = 0;
		return(-1);
	}
	if(dbentry.point.data_type == IMP_TYPE)
	{
		*pValue=dbentry.var.imp.entry.rt_value;
		stat = 0;
		if(dbentry.var.imp.entry.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.imp.entry.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		*pValue=(float)dbentry.var.anlg.value.data/1000.0;
		stat = 0;
		if(dbentry.var.anlg.value.status.ctrl_block)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	*pstat = stat;
	return(0);
}

void	PrintSendData(unsigned int nbsend)
{
	unsigned int d;
	char	infobuf[1024], *ptr= infobuf;
	
	ptr += sprintf(ptr, "Send data byte num %u:",nbsend);
	for(d=0;d<nbsend;d++)
	{
		if((d==0) || (d==6) || ((d>6)&&(!((d-6)%20))))
			ptr += sprintf(ptr,"\n<< ");
		else if((d>6)&&(!((d-6)%10)))
			ptr += sprintf(ptr,"  ");
		
		ptr += sprintf(ptr,"%02X ",sendbuf[d]);
		
		if((ptr-infobuf)>=1000)
		{
			*ptr= 0;
			OutputDspInfo(0, "%s", infobuf);
			ptr= infobuf;
		}
	}
	ptr += sprintf(ptr,"\n");
	*ptr= 0;
	OutputDspInfo(0, "%s", infobuf);
}

void	PrintRecvData(unsigned int nbrecv)
{
	unsigned int d;
	char	infobuf[1024], *ptr= infobuf;
	
	ptr += sprintf(ptr, "Received data byte num %u:",nbrecv);
	for(d=0;d<nbrecv;d++)
	{
		if((d==0) || (d==6) || ((d>6)&&(!((d-6)%20))))
			ptr += sprintf(ptr,"\n>> ");
		else if((d>6)&&(!((d-6)%10)))
			ptr += sprintf(ptr,"  ");
		
		ptr += sprintf(ptr,"%02X ",recvbuf[d]);
		
		if((ptr-infobuf)>=1000)
		{
			*ptr= 0;
			OutputDspInfo(0, "%s", infobuf);
			ptr= infobuf;
		}
	}
	ptr += sprintf(ptr,"\n");
	*ptr= 0;
	OutputDspInfo(0, "%s", infobuf);
}

