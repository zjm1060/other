/****************************************************************
*  FILE NAME:           iec104s_fun.c      			*
*  FUNCTION:            Controlled Station			*
*			Basic Communication Function		*
*  NAME :  			              			*
*  DATE:                April,2006                		*
*								*
****************************************************************/

#include "../inc/iec104s_com.h"

#define	DEBUG

extern	unsigned char	sendbuf[1024];
extern	unsigned char	recvbuf[1024];
extern	unsigned short nr_recv,nr_send,nr_ack,nr_rs;
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
extern	unsigned short	me_scale_addr;
extern	unsigned short	me_float_addr;
extern	unsigned short	cyc_base_addr;
extern	unsigned short	cyc_scale_addr;
extern	unsigned short	cyc_float_addr;
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
extern	ASDUDATA		send_data,send_data2;
extern	ASDUCOMDDATA	recv_data;
extern	int	ClockSyncflag;
extern	int	Readflag,Testflag,Resetflag;
extern	int	Paramflag,MirrorNAckflag;
extern	int	interrogationflag,counterflag,icgroupflag,cigroupflag;
extern	int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
extern	int	scanflag;
extern	unsigned short	delayalarm;
extern	unsigned char	cot_interrogation,cot_counter;
extern	unsigned short	seladdr;
extern	short		databand;
/*extern	SYSTEMTIME	localsys_tm; */

extern	time_t	scan_tim;
extern	time_t	cycle_tim;

extern	SPDEF		spdef;
extern	DPDEF		dpdef;
extern	MEDEF		medef;
extern	ME_SCALE_DEF	mescaledef;
extern	ME_FLOAT_DEF	mefloatdef;
extern	MEDEF		medef;
extern	CYCDEF		cycdef;
extern	CYC_SCALE_DEF	cycscaledef;
extern	CYC_FLOAT_DEF	cycfloatdef;
extern	STDEF		stdef;
extern	ITDEF		itdef;
/*
extern	SCDEF		scdef;
extern	DCDEF		dcdef;
extern	SEDEF		sedef;
extern	SE_SCALE_DEF	sescaledef;
extern	SE_FLOAT_DEF	sefloatdef;
extern	RCDEF		rcdef;
*/
extern	int	GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
extern	int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 2: M_ME_ND; 3: M_ME_NC */
extern	int	MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int	SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
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

short	orgscalebuf[MAX_ME_NUM];
float	orgscalefbuf[MAX_ME_NUM];
unsigned char	orgscalestbuf[MAX_ME_NUM];

float	orgfloatbuf[MAX_ME_NUM];
unsigned char	orgfloatstbuf[MAX_ME_NUM];

unsigned char	orgspbuf[MAX_SP_NUM],orgdpbuf[MAX_DP_NUM];
unsigned char	orgstbuf[MAX_ST_NUM],orgstvbuf[MAX_ST_NUM];

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
		printf("iec104s_com: RespondStartDT send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRespondStartDT: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
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
		printf("iec104s_com: RespondStopDT send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRespondStopDT: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
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
		printf("iec104s_com: RespondTestFR send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRespondTestFR: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
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
	type_id = cmdbuf.tpyeId;
	if(cmdbuf.cot==ACTTERM)
	{
		cmdbuf.ack_flag = 0;
		CheckACD();
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
		printf("unknown cmd type id %d\n",type_id);
		NoChangeData = 1;
		cmdbuf.ack_flag = 0;
		break;
	}

	return(true);
}

int	SERespSend(int sock)
{
	unsigned char	type_id;

	NoChangeData = 0;
/* 	sebuf.ack_flag = 0; */
	type_id = sebuf.tpyeId;
	switch(type_id)
	{
	case C_SE_NA:

		SetPointCmdNAAck(sebuf.cot,sebuf.infoaddr,sebuf.setvalue,sebuf.qoc,sock);
/* 		MultiSetPointCmdNAAck(sebuf.cot,sebuf.infonum,sebuf.rcvpnt,sebuf.qoc,sock); */
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			sebuf.cot = ACTTERM;		/* COT:10  activation terminal */
			break;
		}
		sebuf.ack_flag = 0;
		break;

	case C_SE_NB:

		SetPointCmdNBAck(sebuf.cot,sebuf.infoaddr,sebuf.setvalue,sebuf.qoc,sock);
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			sebuf.cot = ACTTERM;		/* COT:10  activation terminal */
			break;
		}
		sebuf.ack_flag = 0;
		break;

	case C_SE_NC:

		SetPointCmdNCAck(sebuf.cot,sebuf.infoaddr,sebuf.fvalue,sebuf.qoc,sock);
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			sebuf.cot = ACTTERM;		/* COT:10  activation terminal */
			break;
		}
		sebuf.ack_flag = 0;
		break;

	case C_SE_ND:

/* 		SetPointCmdNDAck((unsigned char)ACTCON,infonum,recvdatapt,qoc,sock); */	/* Multi Set Point ActCon:7 qoc:0x80 */
		SetPointCmdNDAck(sebuf.cot,sebuf.infonum,sebuf.rcvpnt,sebuf.qoc,sock);
		if(sebuf.term_flag ==1)
		{
			sebuf.term_flag = 0;
			sebuf.cot = ACTTERM;		/* COT:10  activation terminal */
			break;
		}
		sebuf.ack_flag = 0;
		break;

	default:
		printf("unknown setpoint type id %d\n",type_id);
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
	type_id = pmbuf.tpyeId;

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
		printf("unknown setpoint type id %d\n",type_id);
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
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	memcpy(&sendbuf[6],&recv_data.data[pnt][0],infobyte);	/* Milliseconds */

	sendbuf[8] = cause;			/* ACTCON:7 cause of transmit:active */

	byte_to_send = 6 + infobyte;

	send_byte = send(sock,(char*)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104_com: Mirror Command NAck send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nMirror Command NAck: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	SinglePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
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

	sendbuf[6] = C_SC_NA;			/* 45 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 ActCon cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		printf("iec104s_com: Single Point Ctrl Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
/* 	memcpy(cmdsendbuf,sendbuf,byte_to_send); */	/* copy cmd for actterm */
#ifdef DEBUG
	printf("\nSingle Point Ctrl Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	DoublePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
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

	sendbuf[6] = C_DC_NA;			/* 46 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		printf("iec104s_com: Double Point Ctrl Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
/* 	memcpy(cmdsendbuf,sendbuf,byte_to_send); */	/* copy cmd for actterm */

#ifdef DEBUG
	printf("\nDouble Point Ctrl Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	RegulatStepCtrlAck(unsigned char cause,unsigned short address,unsigned char stat,int sock)
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

	sendbuf[6] = C_RC_NA;			/* 47 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* 7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		printf("iec104s_com: Regulating Step Ctrl Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRegulating Step Ctrl Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	SetPointCmdNAAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock)
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

	sendbuf[6] = C_SE_NA;			/* 48 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 9 cause of transmit:active */
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
	
	sendbuf[17] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Set Point Command NA Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSet Point Command NA Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	SetPointCmdNBAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se,int sock)
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

	sendbuf[6] = C_SE_NB;			/* 49 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 9 cause of transmit:active */
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

	sendbuf[17] = se;	/* QOC:= S/E[8] QL[1..7] S/E := 0:=Execute 1:= Select */

	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Set Point Command NB Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSet Point Command NB Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	SetPointCmdNCAck(unsigned char cause,unsigned short address,float value,unsigned char se,int sock)
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

	sendbuf[6] = C_SE_NC;			/* 50 type identifier field
	sendbuf[7] = 1;				/* variable structure specify
	sendbuf[8] = cause;			/* ACTCON:7 cause of transmit:active
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
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

	byte_to_send = 20;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Set Point Command NC Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSet Point Command NC Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	SetPointCmdNDAck(unsigned char cause,unsigned char num,unsigned int pnt,unsigned char se,int sock)
{
	unsigned char	infobyte;

	sendbuf[0] = 0x68;			/* start flag */
/*	sendbuf[1] = 16;	*/		/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = C_SE_ND;			/* Multi setpoint:136 type identifier field */
	sendbuf[7] = num;			/* variable structure specify */
	sendbuf[8] = cause;			/* ACTCON:7 cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
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
		printf("iec104s_com: Multi Set Point Command ND Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nMulti Set Point Command ND Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	ParamValueNAAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value,int sock)
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

	sendbuf[6] = P_ME_NA;			/* 110 type identifier field,normialized */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* cause of transmit:active */
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

	sendbuf[17] = qpm;			/* KPA[1-6] LPC=0(Same)/1(Changed) POP=0 */
						/* KPA 1:= threshold 2:= filter factor */
						/* 3:= low limit  4:= high limit */
	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Parameter value NA Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nParameter value NA Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	ParamValueNBAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value,int sock)
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

	sendbuf[6] = P_ME_NB;			/* 111 type identifier field,scaled */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
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
		printf("iec104s_com: Parameter value NB Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nParameter value NB Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	ParamValueNCAck(unsigned char cause,unsigned char qpm,unsigned short address,float value,int sock)
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

	sendbuf[6] = P_ME_NC;			/* 112 type identifier field,normialized */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cause;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
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
		printf("iec104s_com: Parameter value NC Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nParameter value NC Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	ClockSynchroniseAck(int sock)
{
/*	struct timespec sTimeSpec; */
	struct timeval	 sTimeVal;
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
	sendbuf[8] = ACTCON;			/* 7 cause of transmit:actcon */
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
	sTimetm = *localtime(&sTimeSpec.tv_sec);
	nMilliSec = sTimetm.tm_sec * 1000 + sTimeSpec.tv_nsec/1000000;
*/
	if(gettimeofday(&sTimeVal,NULL) == -1)
	{
		printf("\nClockSynchronisation: gettimeofday() error: %s\n",strerror(errno));
		return(false);
	}
	sTimetm = *localtime(&sTimeVal.tv_sec);
	nMilliSec = sTimetm.tm_sec * 1000 + sTimeVal.tv_usec/1000;

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
		printf("iec104s_com: GeneralInterrogation ACK send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nACKCON ClockSynchronisation: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	TimeDelayAck(int sock)
{
	unsigned short locp16time;
/*	struct timespec sTimeSpec; */
	struct timeval	 sTimeVal;
	struct tm sTimetm;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 15;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = C_CD_NA;			/* 106 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = cdbuf.cot;			/* 7 cause of transmit:actcon */
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
	sTimetm = *localtime(&sTimeSpec.tv_sec);
	locp16time = sTimetm.tm_sec * 1000 + sTimeSpec.tv_nsec/1000000;
*/
	if(gettimeofday(&sTimeVal,NULL) == -1)
	{
		printf("\nClockSynchronisation: gettimeofday() error: %s\n",strerror(errno));
		return(false);
	}
	sTimetm = *localtime(&sTimeVal.tv_sec);
	locp16time = sTimetm.tm_sec * 1000 + sTimeVal.tv_usec/1000;

	if(locp16time<cdbuf.locp16time)
		locp16time = locp16time + 1000 - cdbuf.locp16time;
	else
		locp16time = locp16time - cdbuf.locp16time;
	cdbuf.cp16time = cdbuf.cp16time + locp16time;
	if(cdbuf.cp16time > 59999)
		cdbuf.cp16time -= 60000;
#ifdef SUN
	CrossCopy(&sendbuf[15],&cdbuf.cp16time,2);
#else
	memcpy(&sendbuf[15],&cdbuf.cp16time,2);	/* Milliseconds to second */
#endif
	
	byte_to_send = 17;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Time Delay Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}

#ifdef DEBUG
	printf("\nACKCON Time Delay: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	GeneralInterrogationAck(unsigned char gi_cause,unsigned char gi_qoi,int sock)
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

	sendbuf[6] = C_IC_NA;			/* 100 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = gi_cause;			/* 7 ActCon 10 Actterm cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
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
		printf("iec104s_com: GeneralInterrogationAck send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}

#ifdef DEBUG
	printf("\nGeneralInterrogationAck: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	CounterInterrogationAck(unsigned char gi_cause,unsigned char gi_qcc,int sock)
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

	sendbuf[6] = C_CI_NA;			/* 101 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = gi_cause;			/* 7 Actcon 10 Actterm cause of transmit:active */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
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
		printf("iec104s_com: CounterInterrogationAck send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nCounterInterrogationAck: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	RespondTestAck(int sock)
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

	sendbuf[6] = C_TS_NA;			/* 104 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
/* 	sendbuf[8] = ACTCON | 0x80;		/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[8] = testbuf.cot;		/* 7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = 0xAA;			/* test low byte AA */
	sendbuf[16] = 0x55;			/* test high byte 55 */

	byte_to_send = 17;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Test channel Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nTest channel Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	ResetProcessAck(int sock)
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

	sendbuf[6] = C_RP_NA;			/* 106 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = rpbuf.cot;			/* 7 Actcon cause of transmit: T=1 P/N=0
	sendbuf[9] = nSrcId;			/* high byte of cause of transmit */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = rpbuf.qrp;		/* QRP[1..8] 1:reset process 2:reset soe buf */

	if(rpbuf.qrp==1)
		Resetflag = 1;

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Reset Process Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nReset Process Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	ReadCmdAck(unsigned char qoc,unsigned short infoaddr,int sock)
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

	sendbuf[6] = C_RD_NA;			/* 102 type identifier */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = qoc;			/* cause of transmit:active */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		printf("iec104s_com: Read C_RD_NA Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead C_RD_NA Ack: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
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
		ReadMeasureValueNA(infoaddr,sock);	/* M_ME_NA */
		return(true);
	}
	/* M_ME_NB */
/*
	infopnt = infoaddr - me_scale_addr;
	if((infoaddr>=me_scale_addr)&&(infopnt<mescaledef.number))
	{
		ReadMeasureValueNB(infoaddr,sock);
		return(true);
	}
*/
	infopnt = infoaddr - me_float_addr;
	if((infoaddr>=me_float_addr)&&(infopnt<mefloatdef.number))
	{
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
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = SPONT;			/* cause of transmit:spont */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		printf("iec104s_com: System information Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSystem information: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	SinglePointInform(unsigned char gi_cause,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	int		data_type,pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this SP point in the database file!\n",name_buf);
			continue;
		}
		else
		{
			if(dbentry.var.ind.status.state==1)
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
						printf("iec104s_com: Single Point information send() Error %d! do close socket!\n",errno);
						close(sock);
						return(false);
					}
#ifdef DEBUG
					printf("\nSingle Point information: %u byte transmitted!",send_byte);
					PrintSendData(send_byte);
#endif
					nr_send +=2;
#ifdef SUN
					CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
#else
					memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
#endif
					inf_n = 0;
				}
			}
		}
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("SinglePointInform: NoRequestData\n");
#endif
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
		printf("iec104s_com: Single Point information send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSingle Point information: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	CheckSinglePointChange(void)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
	int		data_type,pntnum;
	char	name_buf[NAME_SIZE];


	for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Check for SP status Change */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this SP point in the database file!\n",name_buf);
			continue;
		}
		else
		{
			if(dbentry.var.ind.status.state==1)
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
	unsigned char	stat;
	DMS_COMMON	dbentry;
	int		data_type,pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum=infoaddr - sp_base_addr;		/* Check for SP status Change */
	strcpy(name_buf,spdef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this SP point in the database file!\n",name_buf);
		stat = 0;
	}
	else
	{
		if(dbentry.var.ind.status.state==1)
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
		printf("iec104s_com: Read Single Point information Ack send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead Single Point information: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	SinglePointInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_SP_NA;			/* 1 type single point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this SP point in the database file!\n",name_buf);
			sendbuf[15+inf_n] = orgspbuf[pntnum] =0;	/* QDS IV NT SB BL RES3 SPI */
			inf_n++;
			continue;
		}
		if(dbentry.var.ind.status.state==1)
			stat = 1;
		else
			stat = 0;
		if(dbentry.var.ind.status.man_set)
			stat = stat | 0x20;			/* SB=1 */
		if(dbentry.var.ind.status.chan_fail)
			stat = stat | 0xC0;			/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;			/* BL=1 */

		sendbuf[15+inf_n] = orgspbuf[pntnum] =stat;	/* QDS IV NT SB BL RES3 SPI */
		inf_n++;
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("SinglePointInformCyc: NoRequestData\n");
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
		printf("iec104s_com: Single Point information Cyc send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}

#ifdef DEBUG
	printf("\nSingle Point information Cyc: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	SinglePointInformTA(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_SP_TA;			/* 2 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].tpyeId != M_SP_TA) || (nInfoNum >= TA_SP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*7] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*7] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES3 SPI */						

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN
		CrossCopy(&sendbuf[16+nInfoNum*7],&millisec,2);	/* Milliseconds */
#else
		memcpy(&sendbuf[16+nInfoNum*7],&millisec,2);		/* Milliseconds */
#endif
		sendbuf[18+nInfoNum*7] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Single Point information M_SP_TA: NoRequestData\n");
#endif
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
		printf("iec104s_com: Single Point information TA send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}

#ifdef DEBUG
	printf("\nSingle Point information TA: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	SinglePointInformTB(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_SP_TB;			/* 30 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].tpyeId != M_SP_TB) || (nInfoNum >= TB_SP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;		/* information object low address */
		sendbuf[13+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)>>8;			/* information object high address */
		sendbuf[14+nInfoNum*11] = 0;							/* information object high address */						
		sendbuf[15+nInfoNum*11] = soebuf.soerec[soepnt].stat;				/* QDS IV NT SB BL RES3 SPI */							

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
		memcpy(&sendbuf[16+nInfoNum*11],&millisec,2);					/* Milliseconds */
		sendbuf[18+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		sendbuf[19+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/* Hours(0-23) */
		sendbuf[20+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.day;
												/* Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[21+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.month;	/* Months(1-12) */
		sendbuf[22+nInfoNum*11] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/* Years(0-99) */

		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Single Point information M_SP_TB: NoRequestData\n");
#endif
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
		printf("iec104s_com: Single Point information TB send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSingle Point information TB: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	SinglePointGroupCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	unsigned short	stat16,scd16,bitmap;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_PS_NA;			/* 20 type single point group information) */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28) */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
				printf("%s No this SP point in the database file!\n",name_buf);
				orgspbuf[pntnum] =0;		/* QDS IV NT SB BL RES3 SPI) */
			}
			else
			{
				stat=(unsigned char)(dbentry.var.ind.status.state);
				if(stat==1)
				{
					stat16 = stat16 | bitmap;	
					orgspbuf[pntnum] =1;		/* QDS IV NT SB BL RES3 SPI */
				}
				else
					orgspbuf[pntnum] =0;		/* QDS IV NT SB BL RES3 SPI */
			}
		}
		bitmap = bitmap<<1;
		if(bitmap == 0)
		{
#ifdef SUN
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
#ifdef SUN
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
#ifdef DEBUG
		printf("SinglePointGroupCyc: NoRequestData\n");
#endif
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
		printf("iec104s_com: Single Point information Cyc send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nSingle Point information Cyc: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	DoublePointInform(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	unsigned short	dbstat;
	DMS_COMMON	dbentry;
	int		data_type,pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_DP_NA;			/* 3 type double point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this DP point in the database file!\n",name_buf);
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
				sendbuf[12+inf_n*4] = (dp_base_addr + pntnum*2)& 0xFF;	/* i-addr low address */
				sendbuf[13+inf_n*4] = (dp_base_addr + pntnum*2)>>8;	/* i-addr high address */
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
						printf("iec104s_com: Double Point information send() Error %d! do close socket!\n",errno);
						close(sock);
						return(false);
					}
#ifdef DEBUG
					printf("\nDouble Point information: %u byte transmitted!",send_byte);
					PrintSendData(send_byte);
#endif
					nr_send +=2;
#ifdef SUN
					CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
#else
					memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
#endif
					inf_n = 0;
				}
			}
		}
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("DoublePointInform: NoRequestData\n");
#endif
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
		printf("iec104s_com: Double Point information send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nDouble Point information: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	CheckDoublePointChange(void)
{
	unsigned char	stat;
	unsigned short	dbstat;
	DMS_COMMON	dbentry;
	int		data_type,pntnum;
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
			printf("%s No this DP point in the database file!\n",name_buf);
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
	int		data_type,pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* L */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_DP_NA;			/* 3 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum=infoaddr - dp_base_addr;		/* Check for DP status Change */
	strcpy(name_buf,dpdef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this DP point in the database file!\n",name_buf);
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
		printf("iec104s_com: Read Double Point information send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead Double Point information: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	return(true);
}

int	DoublePointInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned short	dbstat;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_DP_NA;			/* 3 type double point information */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this DP point in the database file!\n",name_buf);
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

		sendbuf[15+inf_n] = orgdpbuf[pntnum] = stat;	/* OFF:QDS IV NT SB BL RES2 DPI */

		inf_n++;
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("DoublePointInformCyc: NoRequestData\n");
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
		printf("iec104s_com: Double Point information Cyc send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nDouble Point information Cyc: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	DoublePointInformTA(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_DP_TA;			/* 4 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].tpyeId != M_DP_TA) || (nInfoNum >= TA_DP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*7] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*7] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES2 DPI */							

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
		memcpy(&sendbuf[16+nInfoNum*7],&millisec,2);				/* Milliseconds */
		sendbuf[18+nInfoNum*7] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Double Point information M_DP_TA: NoRequestData\n");
#endif
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
		printf("iec104s_com: Single Point information TA send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nDouble Point information TA: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	DoublePointInformTB(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_DP_TB;			/* 31 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].tpyeId != M_DP_TB) || (nInfoNum >= TB_DP_NUM))
		{
			break;
		}
		sendbuf[12+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;		/* information object low address */
		sendbuf[13+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)>>8;			/* information object high address */
		sendbuf[14+nInfoNum*11] = 0;							/* information object high address */
		sendbuf[15+nInfoNum*11] = soebuf.soerec[soepnt].stat;				/* QDS IV NT SB BL RES2 DPI */						

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
		memcpy(&sendbuf[16+nInfoNum*11],&millisec,2);					/* Milliseconds */
		sendbuf[18+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		sendbuf[19+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/* Hours(0-23) */
		sendbuf[20+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.day;
												/* Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[21+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.month;	/* Months(1-12) */
		sendbuf[22+nInfoNum*11] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/* Years(0-99) */

		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("Double Point information M_DP_TB: NoRequestData\n");
#endif
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
		printf("iec104s_com: Double Point information TB send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nDouble Point information TB: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	StepPositionInform(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat,vti;
	DMS_COMMON	dbentry;
	int		data_type,pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ST_NA;			/* 5 type step position information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this ST point in the database file!\n",name_buf);
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
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		if((stat != orgstbuf[pntnum])||(vti!= orgstvbuf[pntnum])) 
		{
			sendbuf[12+inf_n*5] = (st_base_addr + pntnum*2)& 0xFF;	/* i-addr low address */
			sendbuf[13+inf_n*5] = (st_base_addr + pntnum*2)>>8;	/* i-addr high address */
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
					printf("iec104s_com: StepPositionInform send() Error %d! do close socket!\n",errno);
					close(sock);
					return(false);
				}
#ifdef DEBUG
				printf("\nStepPositionInform: %u byte transmitted!",send_byte);
				PrintSendData(send_byte);
#endif
				nr_send +=2;
#ifdef SUN
				CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
#else
				memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
#endif
				inf_n = 0;
			}
		}
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("StepPositionInform: NoRequestData\n");
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
		printf("iec104s_com: Step Position information send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nStep Position information: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	CheckStepPositionChange(void)
{
	unsigned char	stat,vti;
	DMS_COMMON	dbentry;
	int		data_type,pntnum;
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
			printf("%s No this ST point in the database file!\n",name_buf);
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
			if(dbentry.var.anlg.value.status.man_set)
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
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ST_NA;			/* 5 type single point information */
	sendbuf[7] = 1;				/* variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum=infoaddr - st_base_addr;
	strcpy(name_buf,stdef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this Step Position point in the database file!\n",name_buf);
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
		if(dbentry.var.anlg.value.status.man_set)
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
		printf("iec104s_com: Read StepPosition information send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead StepPosition information: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	StepPositionInformCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	stat,inf_n;
	unsigned char	nLength,vti;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;


	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ST_NA;			/* 5 type step position information */
	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this ST point in the database file!\n",name_buf);
			sendbuf[15+inf_n*2] = orgstvbuf[pntnum] =0;	/* VTI */
			sendbuf[16+inf_n*2] = orgstbuf[pntnum] =0;	/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ST point in the database file!\n",name_buf);
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
			if(dbentry.var.anlg.value.status.man_set)
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
#ifdef DEBUG
		printf("StepPositionInformCyc: NoRequestData\n");
#endif
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
		printf("iec104s_com: StepPosition information Cyc send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nStepPosition information Cyc: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	StepPositionInformTA(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ST_TA;			/* 5 type step position information */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].tpyeId != M_ST_TA) || (nInfoNum >= 30))
		{
			break;
		}
		sendbuf[12+nInfoNum*8] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*8] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*8] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*8] = soebuf.soerec[soepnt].vti;			/* VTI */				
		sendbuf[16+nInfoNum*8] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES3 OV */							

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
		memcpy(&sendbuf[17+nInfoNum*8],&millisec,2);					/* Milliseconds */
		sendbuf[18+nInfoNum*8] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("StepPosition information M_ST_TA: NoRequestData\n");
#endif
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
		printf("iec104s_com: StepPosition information TA send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nStepPosition information TA: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	StepPositionInformTB(unsigned char gi_cause,int sock)
{
	unsigned short	millisec;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ST_TB;			/* 31 type single point information with time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].tpyeId != M_ST_TB) || (nInfoNum >= 20))
		{
			break;
		}
		sendbuf[12+nInfoNum*12] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/* information object low address */
		sendbuf[13+nInfoNum*12] = (soebuf.soerec[soepnt].infoaddr)>>8;		/* information object high address */
		sendbuf[14+nInfoNum*12] = 0;						/* information object high address */
		sendbuf[15+nInfoNum*12] = soebuf.soerec[soepnt].vti;			/* VTI  */		
		sendbuf[16+nInfoNum*12] = soebuf.soerec[soepnt].stat;			/* QDS IV NT SB BL RES3 OV */					

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
		memcpy(&sendbuf[17+nInfoNum*12],&millisec,2);					/* Milliseconds */
		sendbuf[19+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/* Minutes */
		sendbuf[20+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/* Hours(0-23) */
		sendbuf[21+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.day;
						/* Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[22+nInfoNum*12] = (unsigned char)soebuf.soerec[soepnt].time.month;	/* Months(1-12) */
		sendbuf[23+nInfoNum*12] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/* Years(0-99) */

		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		printf("StepPosition information M_ST_TB: NoRequestData\n");
#endif
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
		printf("iec104s_com: StepPosition information TB send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nStepPosition information TB: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNA(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NA;			/* 9 type normalized without time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			anaorgcode = (short)value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			if(dbentry.point.data_type != IMP_TYPE)
			{
				if(value <= medef.lorange[pntnum])
				{
					if(MeType == 0)
						anaorgcode = 0;
					else
						anaorgcode = (short)0x8000;
				}
				else	if(value >= medef.hirange[pntnum])
					anaorgcode = 0x7FFF;
				else
				{
					if(MeType == 0)
					{
						fdata=(value-medef.lorange[pntnum])*32767.0/
							(medef.hirange[pntnum]-medef.lorange[pntnum]);
						anaorgcode=(short)fdata;
						if((fdata-anaorgcode)>=0.5)
							anaorgcode++;
						else if((fdata-anaorgcode)<=-0.5)
							anaorgcode--;
					}
					else
					{
						fdata=(value-(medef.hirange[pntnum]+medef.lorange[pntnum])/2.0)*65535.0/
							(medef.hirange[pntnum]-medef.lorange[pntnum]);
						anaorgcode=(short)fdata;
						if((fdata-anaorgcode)>=0.5)
							anaorgcode++;
						else if((fdata-anaorgcode)<=-0.5)
							anaorgcode--;
					}
				}
			}

#ifdef DEBUG
			printf("ME %d: real %f save %f databand %f\n",pntnum,value,organafbuf[pntnum],medef.databand[pntnum]);
#endif
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
					printf("iec104s_com: Measure Value M_ME_NA() Error %d! do close socket!\n",errno);
					close(sock);
					return(false);
				}
#ifdef DEBUG
				printf("\nMeasure Value M_ME_NA: %u byte transmitted!",send_byte);
				PrintSendData(send_byte);
#endif
				nr_send +=2;
#ifdef SUN
				CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
#else
				memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
#endif
				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNA: NoRequestData\n");
#endif
		NoChangeData = 1;
		return(true);
	}

	nLength = inf_n * 6 + 10;
	sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Measure Value M_ME_NA send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nMeasure Value M_ME_NA: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	CheckMeasureValueNA(void)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
	int		data_type;
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
#ifdef DEBUG
			printf("Check ME %d: real %f save %f databand %f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
#endif
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
	DMS_COMMON		dbentry;
	int				data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short			anaorgcode;
	float			value;
	double			fdata;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NA;			/* 9 type normalized without time tag */

	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			sendbuf[15+inf_n*3] = (unsigned char)0;		/* N Value */						
			sendbuf[16+inf_n*3] = (unsigned char)0;		/* N Value */
			sendbuf[17+inf_n*3] = organastbuf[pntnum] = 0;	/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			anaorgcode = (short)value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */

			if(value <= medef.lorange[pntnum])
			{
				if(MeType == 0)
					anaorgcode = 0;
				else
					anaorgcode = (short)0x8000;
			}
			else	if(value >= medef.hirange[pntnum])
				anaorgcode = 0x7FFF;
			else
			{
				if(MeType == 0)
				{
					fdata=(value-medef.lorange[pntnum])*32767.0/
						(medef.hirange[pntnum]-medef.lorange[pntnum]);
					anaorgcode=(short)fdata;
					if((fdata-anaorgcode)>=0.5)
						anaorgcode++;
					else if((fdata-anaorgcode)<=-0.5)
						anaorgcode--;
				}
				else
				{
					fdata=(value-(medef.hirange[pntnum]+medef.lorange[pntnum])/2.0)*65535.0/
						(medef.hirange[pntnum]-medef.lorange[pntnum]);
					anaorgcode=(short)fdata;
					if((fdata-anaorgcode)>=0.5)
						anaorgcode++;
					else if((fdata-anaorgcode)<=-0.5)
						anaorgcode--;
				}
			}
		}
		sendbuf[15+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */						
		sendbuf[16+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/* N Value */
		sendbuf[17+inf_n*3] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNACyc: NoRequestData\n");
#endif
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
		printf("iec104s_com: Cyc Measure Value M_ME_NA send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nCyc Measure Value M_ME_NA: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	ReadMeasureValueNA(unsigned short infoaddr,int sock)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L; */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NA;			/* 9 type normalized without time tag */

	sendbuf[7] = 1;				/* SQ=0 */

	sendbuf[8] = REQ;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum = infoaddr - me_base_addr;
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;			/* i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this ANA point in the database file!\n",name_buf);
		stat = 0;				/* QDS IV NT SB BL RES3 OV */
		anaorgcode = (short)0;			/* N Value */
		value=(float)0;
	}
	else if(dbentry.point.data_type == IMP_TYPE)
	{
		value=dbentry.var.imp.rt_value;
		anaorgcode = (short)value;
		stat = 0;
		if(dbentry.var.imp.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.imp.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		value=dbentry.var.anlg.value.data/1000;
		stat = 0;
		if(dbentry.var.anlg.value.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */

		if(value <= medef.lorange[pntnum])
		{
			if(MeType == 0)
				anaorgcode = 0;
			else
				anaorgcode = (short)0x8000;
		}
		else	if(value >= medef.hirange[pntnum])
			anaorgcode = 0x7FFF;
		else
		{
			if(MeType == 0)
			{
				fdata=(value-medef.lorange[pntnum])*32767.0/
					(medef.hirange[pntnum]-medef.lorange[pntnum]);
				anaorgcode=(short)fdata;
				if((fdata-anaorgcode)>=0.5)
					anaorgcode++;
				else if((fdata-anaorgcode)<=-0.5)
					anaorgcode--;
			}
			else
			{
				fdata=(value-(medef.hirange[pntnum]+medef.lorange[pntnum])/2.0)*65535.0/
					(medef.hirange[pntnum]-medef.lorange[pntnum]);
				anaorgcode=(short)fdata;
				if((fdata-anaorgcode)>=0.5)
					anaorgcode++;
				else if((fdata-anaorgcode)<=-0.5)
					anaorgcode--;
			}
		}
	}

	sendbuf[15] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */							
	sendbuf[16] = (unsigned char)(anaorgcode >> 8);		/* N Value */
	sendbuf[17] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
	organabuf[pntnum] = anaorgcode;
	organafbuf[pntnum] = value;

	byte_to_send = 18;

	send_byte = send(sock,(char*)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Read Measure Value M_ME_NA send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead Measure Value M_ME_NA: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNB(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NB;			/* 11 type scaled without time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			anaorgcode = (short)value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			if(dbentry.point.data_type != IMP_TYPE)
			{
				fdata=value*medef.lorange[pntnum]; /*Note: Here lorange is used as scale factor */
				anaorgcode=(short)fdata;
				if((fdata-anaorgcode)>=0.5)
					anaorgcode++;
				else if((fdata-anaorgcode)<=-0.5)
					anaorgcode--;
#ifdef DEBUG
				printf("ME %d: real %f save %f databand %f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
#endif
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
					printf("iec104s_com: Measure Value M_ME_NB() Error %d! do close socket!\n",errno);
					close(sock);
					return(false);
				}
#ifdef DEBUG
				printf("\nMeasure Value M_ME_NB: %u byte transmitted!",send_byte);
				PrintSendData(send_byte);
#endif
				nr_send +=2;
#ifdef SUN
				CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
#else
				memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
#endif
				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNB: NoRequestData\n");
#endif
		NoChangeData = 1;
		return(true);
	}

	nLength = inf_n * 6 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Measure Value M_ME_NB send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nMeasure Value M_ME_NB: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	CheckMeasureValueNB(void)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
#ifdef DEBUG
			printf("Check ME %d: real %f save %f databand %f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
#endif
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
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NB;			/* 11 type normalized without time tag */

	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			sendbuf[15+inf_n*3] = (unsigned char)0;			/* N Value */							
			sendbuf[16+inf_n*3] = (unsigned char)0;			/* N Value */
			sendbuf[17+inf_n*3] = organastbuf[pntnum] = 0;		/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			anaorgcode = (short)value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */

			fdata=value*medef.lorange[pntnum]; 	/*Note: Here lorange is used as scale factor */
			anaorgcode=(short)fdata;
			if((fdata-anaorgcode)>=0.5)
				anaorgcode++;
			else if((fdata-anaorgcode)<=-0.5)
				anaorgcode--;
		}

		sendbuf[15+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */						
		sendbuf[16+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/* N Value */
		sendbuf[17+inf_n*3] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNBCyc: NoRequestData\n");
#endif
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
		printf("iec104s_com: Cyc Measure Value M_ME_NB send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nCyc Measure Value M_ME_NB: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	ReadMeasureValueNB(unsigned short infoaddr,int sock)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 16;			/* L; */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NB;			/* 11 type scaled without time tag */

	sendbuf[7] = 1;				/* SQ=0 */

	sendbuf[8] = REQ;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum = infoaddr - me_base_addr;
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);		/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);		/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this ANA point in the database file!\n",name_buf);
		stat = 0;				/* QDS IV NT SB BL RES3 OV */
		anaorgcode = (short)0;			/* N Value */
		value = (float)0;
	}
	else if(dbentry.point.data_type == IMP_TYPE)
	{
		value=dbentry.var.imp.rt_value;
		anaorgcode = (short)value;
		stat = 0;
		if(dbentry.var.imp.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.imp.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		value=dbentry.var.anlg.value.data/1000;
		stat = 0;
		if(dbentry.var.anlg.value.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */

		fdata=value*medef.lorange[pntnum]; 	/*Note: Here lorange is used as scale factor */
		anaorgcode=(short)fdata;
		if((fdata-anaorgcode)>=0.5)
			anaorgcode++;
		else if((fdata-anaorgcode)<=-0.5)
			anaorgcode--;
	}

	sendbuf[15] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */						
	sendbuf[16] = (unsigned char)(anaorgcode >> 8);		/* N Value */
	sendbuf[17] = organastbuf[pntnum] = stat;		/* QDS IV NT SB BL RES3 OV */
	organabuf[pntnum] = anaorgcode;
	organafbuf[pntnum] = value;

	byte_to_send = 18;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Read Measure Value M_ME_NB send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead Measure Value M_ME_NB: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNC(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength,stat;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NC;			/* 13 type float without time tag */
	sendbuf[8] = gi_cause;			/* cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	inf_n = 0;

	for(pntnum=0;pntnum<mefloatdef.number;pntnum++)
	{
/*		if((strcmp(mefloatdef.name[pntnum],"NULL")==0)||(mefloatdef.TimeTag[pntnum] !=0)) */
		if(strcmp(mefloatdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,mefloatdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		if(((value - orgfloatbuf[pntnum]) > mefloatdef.databand[pntnum]) || ((orgfloatbuf[pntnum] - value) > mefloatdef.databand[pntnum])
			||(orgfloatstbuf[pntnum] != stat))
		{
#ifdef DEBUG
 			printf("ME %d: real %f save %f databand %f\n",pntnum,
 					value,orgfloatbuf[pntnum],mefloatdef.databand[pntnum]);
#endif
			sendbuf[12+inf_n*8] = (unsigned char)((me_float_addr + pntnum)& 0xFF);	/* i-addr low address */
			sendbuf[13+inf_n*8] = (unsigned char)((me_float_addr + pntnum)>>8);	/* i-addr high address */
			sendbuf[14+inf_n*8] = (unsigned char)0;					/* i-addr high address */
			memcpy(&sendbuf[15+inf_n*8],&value,4);					/* N Value */			
			sendbuf[19+inf_n*8] = orgfloatstbuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 OV */

			inf_n++;
			orgfloatbuf[pntnum] = value;
			if(inf_n >= SPONT_ME_FLOAT_NUM)
			{
				sendbuf[7] = inf_n;			/* SQ=0 */
				nLength = inf_n * 8 + 10;
				sendbuf[1] = nLength;			/* L	byte_to_send = L + 6; */
				byte_to_send = nLength + 2;

				send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

				if(send_byte != byte_to_send)
				{
					printf("iec104s_com: Measure Value M_ME_NC() Error %d! do close socket!\n",errno);
					close(sock);
					return(false);
				}
#ifdef DEBUG
				printf("\nMeasure Value M_ME_NC: %u byte transmitted!",send_byte);
				PrintSendData(send_byte);
#endif
				nr_send +=2;
#ifdef SUN
				CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
#else
				memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
#endif
				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNC: NoRequestData\n");
#endif
		NoChangeData = 1;
		return(true);
	}
	nLength = inf_n * 8 + 10;
	sendbuf[1] = nLength;		/* L	byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Measure Value M_ME_NC send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nMeasure Value M_ME_NC: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	CheckMeasureValueNC(void)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	for(pntnum=0;pntnum<mefloatdef.number;pntnum++)
	{
		if(strcmp(mefloatdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,mefloatdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}

		if(((value - orgfloatbuf[pntnum]) > mefloatdef.databand[pntnum]) || ((orgfloatbuf[pntnum] - value) > mefloatdef.databand[pntnum])
			||(orgfloatstbuf[pntnum] != stat))
		{
#ifdef DEBUG
			printf("ME %d: real %f save %f databand %f\n",pntnum,
					value,orgfloatbuf[pntnum],mefloatdef.databand[pntnum]);
#endif
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
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NC;			/* 13 type float without time tag */

	sendbuf[8] = gi_cause;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	startnum = ME_FLOAT_GROUP_NUM*infogroup;	/* infogroup: 0,1,2,3 */
	if(mefloatdef.number <= startnum)
	{
		NoChangeData = 1;
		return(true);
	}
	firstinfoaddr = startnum + me_float_addr;	/* infogroup: 0,1,2,3 */
	endnum = startnum + ME_FLOAT_GROUP_NUM;
	if(mefloatdef.number < endnum)
		endnum = mefloatdef.number;

	sendbuf[12] = (unsigned char)(firstinfoaddr& 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(firstinfoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;				/* i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(mefloatdef.name[pntnum],"NULL")==0)
		{
			value = 0;
#ifdef SUN
			CrossCopy(&sendbuf[15+inf_n*5],&value,4);	/* N Value */
#else
			memcpy(&sendbuf[15+inf_n*5],&value,4);		/* N Value */						
#endif
			sendbuf[19+inf_n*5] = 0;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,mefloatdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			value = 0;
#ifdef SUN
			CrossCopy(&sendbuf[15+inf_n*5],&value,4);	/* N Value */
#else
			memcpy(&sendbuf[15+inf_n*5],&value,4);		/* N Value */						
#endif
			sendbuf[19+inf_n*5] = 0;			/* QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			stat = 0;
			if(dbentry.var.imp.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.imp.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			stat = 0;
			if(dbentry.var.anlg.value.status.man_set)
				stat = stat | 0x20;		/* SB=1 */
			if(dbentry.var.anlg.value.status.chan_fail)
				stat = stat | 0xC0;		/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;		/* BL=1 */
		}
#ifdef SUN
		CrossCopy(&sendbuf[15+inf_n*5],&value,4);		/* N Value */
#else
		memcpy(&sendbuf[15+inf_n*5],&value,4);			/* N Value */						
#endif
		sendbuf[19+inf_n*5] = orgfloatstbuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 OV */
		inf_n++;
		orgfloatbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNCCyc: NoRequestData\n");
#endif
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
		printf("iec104s_com: Cyc Measure Value M_ME_NC send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nCyc Measure Value M_ME_NC: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	ReadMeasureValueNC(unsigned short infoaddr,int sock)
{
	unsigned char	stat;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 18;			/* L; */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_NC;			/* 13 type float without time tag */

	sendbuf[7] = 1;				/* SQ=0 */

	sendbuf[8] = REQ;			/* cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
	CrossCopy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#else
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
#endif

	pntnum = infoaddr - me_float_addr;
	sendbuf[12] = (unsigned char)(infoaddr & 0xFF);	/* i-addr low address */
	sendbuf[13] = (unsigned char)(infoaddr>>8);	/* i-addr high address */
	sendbuf[14] = (unsigned char)0;			/* i-addr high address */
	strcpy(name_buf,mefloatdef.name[pntnum]);
	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this ANA point in the database file!\n",name_buf);
		value = 0.0f;			/* N Value */						
		stat = 0;			/* QDS IV NT SB BL RES3 OV */
	}
	else if(dbentry.point.data_type == IMP_TYPE)
	{
		value=dbentry.var.imp.rt_value;
		stat = 0;
		if(dbentry.var.imp.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.imp.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}
	else
	{
		value=dbentry.var.anlg.value.data/1000;
		stat = 0;
		if(dbentry.var.anlg.value.status.man_set)
			stat = stat | 0x20;		/* SB=1 */
		if(dbentry.var.anlg.value.status.chan_fail)
			stat = stat | 0xC0;		/* IV=1; NT=1 */
		if(dbentry.fixed.iost.scanEnable==0)
			stat = stat | 0x10;		/* BL=1 */
	}

#ifdef SUN
	CrossCopy(&sendbuf[15],&value,4);	/* N Value */
#else
	memcpy(&sendbuf[15],&value,4);		/* N Value */						
#endif
	sendbuf[19] = orgfloatstbuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 OV */
	orgfloatbuf[pntnum] = value;

	byte_to_send = 20;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Read Measure Value M_ME_NC send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nRead Measure Value M_ME_NC: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	MeasureValueND(unsigned char gi_cause,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_ND;		/* 21 type normalized without quality descriptor */
	sendbuf[8] = gi_cause;		/* cause of transmit:SPONT:3 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			anaorgcode = (short)value;
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum]))
		{
			if(dbentry.point.data_type != IMP_TYPE)
			{
				if(value <= medef.lorange[pntnum])
				{
					if(MeType == 0)
						anaorgcode = 0;
					else
						anaorgcode = (short)0x8000;
				}
				else	if(value >= medef.hirange[pntnum])
					anaorgcode = 0x7FFF;
				else
				{
					if(MeType == 0)
					{
						fdata=(value-medef.lorange[pntnum])*32767.0/
							(medef.hirange[pntnum]-medef.lorange[pntnum]);
						anaorgcode=(short)fdata;
						if((fdata-anaorgcode)>=0.5)
							anaorgcode++;
						else if((fdata-anaorgcode)<=-0.5)
							anaorgcode--;
					}
					else
					{
						fdata=(value-(medef.hirange[pntnum]+medef.lorange[pntnum])/2.0)*65535.0/
							(medef.hirange[pntnum]-medef.lorange[pntnum]);
						anaorgcode=(short)fdata;
						if((fdata-anaorgcode)>=0.5)
							anaorgcode++;
						else if((fdata-anaorgcode)<=-0.5)
							anaorgcode--;
					}
				}
			}
#ifdef DEBUG
			printf("ME %d: real %f save %f databand %f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
#endif
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
					printf("iec104s_com: Measure Value M_ME_ND() Error %d! do close socket!\n",errno);
					close(sock);
					return(false);
				}
#ifdef DEBUG
				printf("\nMeasure Value M_ME_ND: %u byte transmitted!",send_byte);
				PrintSendData(send_byte);
#endif
				nr_send +=2;
#ifdef SUN
				CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
#else
				memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
#endif
				inf_n = 0;
			}
		}
	}
	sendbuf[7] = inf_n;	/* SQ=0 */

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueND: NoRequestData\n");
#endif
		NoChangeData = 1;
		return(true);
	}
	nLength = inf_n * 5 + 10;
	sendbuf[1] = nLength;		/* L byte_to_send = L + 6; */
	byte_to_send = nLength + 2;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: Measure Value M_ME_ND send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nMeasure Value M_ME_ND: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	MeasureValueNDCyc(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;
	double		fdata;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_ME_ND;			/* 21 type normalized without quality descriptor */

	sendbuf[8] = gi_cause;			/* INTRO9:29 INTRO10:30 */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this ANA point in the database file!\n",name_buf);
			sendbuf[15+inf_n*2] = (unsigned char)0;		/* N Value */						
			sendbuf[16+inf_n*2] = (unsigned char)0;		/* N Value */
			inf_n++;
			continue;
		}
		if(dbentry.point.data_type == IMP_TYPE)
		{
			value=dbentry.var.imp.rt_value;
			anaorgcode = (short)value;
		}
		else
		{
			value=dbentry.var.anlg.value.data/1000;
			if(value <= medef.lorange[pntnum])
			{
				if(MeType == 0)
					anaorgcode = 0;
				else
					anaorgcode = (short)0x8000;
			}
			else	if(value >= medef.hirange[pntnum])
				anaorgcode = 0x7FFF;
			else
			{
				if(MeType == 0)
				{
					fdata=(value-medef.lorange[pntnum])*32767.0/
						(medef.hirange[pntnum]-medef.lorange[pntnum]);
					anaorgcode=(short)fdata;
					if((fdata-anaorgcode)>=0.5)
						anaorgcode++;
					else if((fdata-anaorgcode)<=-0.5)
						anaorgcode--;
				}
				else
				{
					fdata=(value-(medef.hirange[pntnum]+medef.lorange[pntnum])/2.0)*65535.0/
						(medef.hirange[pntnum]-medef.lorange[pntnum]);
					anaorgcode=(short)fdata;
					if((fdata-anaorgcode)>=0.5)
						anaorgcode++;
					else if((fdata-anaorgcode)<=-0.5)
						anaorgcode--;
				}
			}
		}
		sendbuf[15+inf_n*2] = (unsigned char)(anaorgcode & 0xFF);	/* N Value */							
		sendbuf[16+inf_n*2] = (unsigned char)(anaorgcode >> 8);		/* N Value */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("MeasureValueNDCyc: NoRequestData\n");
#endif
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
		printf("iec104s_com: Cyc Measure Value M_ME_ND send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nCyc Measure Value M_ME_ND: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	IntegratedTotals(unsigned char gi_cause,unsigned char infogroup,int sock)
{
	unsigned char	inf_n;
	unsigned char	nLength;
	unsigned int	count;
	DMS_COMMON	dbentry;
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_IT_NA;			/* 15 type Integrated Totals information */
	sendbuf[8] = gi_cause;			/* 37 cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this IMP point in the database file!\n",name_buf);
			continue;
		}
		sendbuf[12+inf_n*8] = (it_base_addr + pntnum)& 0xFF;	/* information object low address */
		sendbuf[13+inf_n*8] = (it_base_addr + pntnum)>>8;	/* information object high address */
		sendbuf[14+inf_n*8] = 0;				/* information object high address */
		count = dbentry.var.imp.rt_value;		/* count value */
#ifdef SUN
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
#ifdef DEBUG
		printf("IntegratedTotals: NoRequestData\n");
#endif
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
		printf("iec104s_com: Integrated Totals send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nIntegrated Totals: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
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
	int		data_type;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_IT_TA;			/* 16 type Integrated Totals information */
	sendbuf[8] = gi_cause;			/* 37 cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this IMP point in the database file!\n",name_buf);
			continue;
		}
		sendbuf[12+inf_n*8] = (it_base_addr + pntnum)& 0xFF;	/* information object low address */
		sendbuf[13+inf_n*8] = (it_base_addr + pntnum)>>8;	/* information object high address */
		sendbuf[14+inf_n*8] = 0;				/* information object high address */
		count = dbentry.var.imp.rt_value;		/* count value */
#ifdef SUN
		CrossCopy(&sendbuf[15+inf_n*8],&count,4);
#else
		memcpy(&sendbuf[15+inf_n*8],&count,4);
#endif
		sendbuf[19+inf_n*8] = count_sq;			/* QDS IV CA CY SQ */						
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		millisec = dbentry.var.imp.update_time.second * 1000 + dbentry.var.imp.update_time.ms;
#ifdef SUN
		CrossCopy(&sendbuf[20+inf_n*11],&millisec,2);	/* ASDU common address */
#else
		memcpy(&sendbuf[20+inf_n*11],&millisec,2);	/* Milliseconds */
#endif
		sendbuf[22+inf_n*11] = dbentry.var.imp.update_time.minute;	/* Minutes */
		inf_n ++;
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("IntegratedTotalsTA: NoRequestData\n");
#endif
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
		printf("iec104s_com: Integrated Totals TA send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nIntegrated Totals TA: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	IntegratedTotalsTB(unsigned char gi_cause,unsigned short infogroup,int sock)
{
	unsigned short	millisec;
	unsigned char	n,inf_n;
	unsigned char	nLength;
	unsigned int	count;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = 0x68;			/* start flag */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_IT_TB;			/* 37 type Integrated Totals information */
	sendbuf[8] = gi_cause;			/* 37 cause of transmit:requested */
	sendbuf[9] = nSrcId;			/* source address */
#ifdef SUN
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
			printf("%s No this IMP point in the database file!\n",name_buf);
			continue;
		}
		sendbuf[12+inf_n*15] = (it_base_addr + pntnum)& 0xFF;	/* i-addr information object low address */
		sendbuf[13+inf_n*15] = (it_base_addr + pntnum)>>8;	/* i-addr information object high address */
		sendbuf[14+inf_n*15] = 0;				/* i-addr information object high address */
		count = dbentry.var.imp.rt_value;		/* count value */
#ifdef SUN
		CrossCopy(&sendbuf[15+inf_n*8],&count,4);
#else
		memcpy(&sendbuf[15+inf_n*8],&count,4);
#endif
		sendbuf[19+inf_n*15] = count_sq;		/* QDS IV CA CY SQ */							
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		millisec = dbentry.var.imp.update_time.second * 1000 + dbentry.var.imp.update_time.ms;
#ifdef SUN
		CrossCopy(&sendbuf[20+inf_n*11],&millisec,2);	/* ASDU common address */
#else
		memcpy(&sendbuf[20+inf_n*11],&millisec,2);	/* Milliseconds */
#endif
		sendbuf[22+inf_n*11] = dbentry.var.imp.update_time.minute;	/* Minutes */
		sendbuf[23+inf_n*15] = dbentry.var.imp.update_time.hour;	/* Hour */
/*		n = sys_tm.wDayOfWeek << 5;
		n = n & 0xE0;
*/
		sendbuf[24+inf_n*15] = dbentry.var.imp.update_time.day;		/* DayofWeek[6..8] day[1..5] */
		sendbuf[25+inf_n*15] = dbentry.var.imp.update_time.month;	/* Month */
		sendbuf[26+inf_n*15] = (unsigned char)(dbentry.var.imp.update_time.year%100);	/* Year */
		inf_n ++;
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		printf("IntegratedTotalsTA: NoRequestData\n");
#endif
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
		printf("iec104s_com: Integrated Totals TB send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nIntegrated Totals TB: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
	NoChangeData = 0;
	return(true);
}

int	EndofInitialization(int sock)
{
	sendbuf[0] = 0x68;			/* start flag */
	sendbuf[1] = 14;			/* Length */
#ifdef SUN
	CrossCopy(&sendbuf[2],&nr_send,2);	/* V(S) */
	CrossCopy(&sendbuf[4],&nr_recv,2);	/* V(R) */
#else
	memcpy(&sendbuf[2],&nr_send,2);		/* V(S) */
	memcpy(&sendbuf[4],&nr_recv,2);		/* V(R) */
#endif

	sendbuf[6] = M_EI_NA;			/* 70 type identifier field */
	sendbuf[7] = 1;				/* variable structure specify */
	sendbuf[8] = 4;				/* 4 Actterm cause of transmit:end */
	sendbuf[9] = 0;				/* cause of transmit:SPONT:3 */
	memcpy(&sendbuf[10],&nASDUId,2);	/* ASDU common address */
	sendbuf[12] = 0;			/* information object low address */
	sendbuf[13] = 0;			/* information object high address */
	sendbuf[14] = 0;			/* information object high address */
	sendbuf[15] = 5;			/* COI 0 local power on	 */					

	byte_to_send = 16;

	send_byte = send(sock,(char *)sendbuf,byte_to_send,(int)0);

	if(send_byte != byte_to_send)
	{
		printf("iec104s_com: End of Initialization send() Error %d! do close socket!\n",errno);
		close(sock);
		return(false);
	}
#ifdef DEBUG
	printf("\nEnd of Initialization: %u byte transmitted!",send_byte);
	PrintSendData(send_byte);
#endif
	nr_send +=2;
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

		ret = DoublePointInformCyc(INTRO1,m_cRespICGrpNum,sock);	/* 22 respond to Group 1 (2) general Interrogation */
		break;

	case  22:

		ret = DoublePointInformCyc(INTRO2,m_cRespICGrpNum,sock);	/* 22 respond to Group 1 (2) general Interrogation */
		break;

	case  23:

		if(GiType==1)
			ret = SinglePointInformCyc(INTRO3,m_cRespICGrpNum,sock);	/* 23 respond to Group 2 (3) general Interrogation */
		else
			ret = SinglePointGroupCyc(INTRO3, m_cRespICGrpNum,sock);	/* 23 respond to Group 2 (3) Interrogation M_PS_NA */
		ret = DoublePointInformCyc(INTRO3,m_cRespICGrpNum,sock);	/* 22 respond to Group 1 (2) general Interrogation */
		break;

	case  24:
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
			ret = MeasureValueNACyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 0 (9) general Interrogation M_ME_NA */
		else if(MeASDUType==1)
			ret = MeasureValueNBCyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 0 (9) general Interrogation M_ME_NB */
		else if(MeASDUType==2)
			ret = MeasureValueNDCyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 0 (9) general Interrogation M_ME_ND */
		else
			ret = MeasureValueNCCyc(INTRO10,m_cRespICGrpNum,sock);	/* 30 respond to Group 0 (9) general Interrogation M_ME_NC */
		break;

	case  31:

		ret = MeasureValueNCCyc(INTRO11,m_cRespICGrpNum,sock);	/* 31 respond to Group 2 (11) general Interrogation M_ME_NC */
		break;

	default :
#ifdef DEBUG
		printf("No this group data qoi=%d\n",gi_qoi);
#endif
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

int RespondGeneralInterrogation(int sock)
{
	int	ret;
	int		unack_num;
/*
	if(soe_overflow_Sendflag ==1)
	{
		ret = SequenceofEvent();
		if(NoChangeData == 0)
			return(ret);
		else
			soe_overflow_Sendflag = 0;
	}
*/
	if(nr_send >= nr_ack)
		unack_num = nr_send - nr_ack;
	else
		unack_num = 0x10000 + nr_send - nr_ack;
	if(unack_num >= K*2)
		return(true);

	NoChangeData = 0;
	while(m_cRespGenInterroGrpNum < 2000)
	{
		if(GiType==1)
		{
			ret = SinglePointInformCyc(INTROGEN, m_cRespGenInterroGrpNum,sock);	/* 1 respond to general Interrogation M_SP_NA */
			if(ret == false)
				return(false);
		}
		else
		{
			ret = SinglePointGroupCyc(INTROGEN, m_cRespGenInterroGrpNum,sock);	/* 20 respond to general Interrogation M_PS_NA */
			if(ret == false)
				return(false);
		}
#ifdef DEBUG
		printf("++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
#endif
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
		ret = DoublePointInformCyc((unsigned char)INTROGEN, (unsigned short)(m_cRespGenInterroGrpNum-2000),sock);	/* 20 respond to general Interrogation DP */
		if(ret == false)
			return(false);
#ifdef DEBUG
		printf("++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
#endif
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
		ret = StepPositionInformCyc((unsigned char)INTROGEN, (unsigned short)(m_cRespGenInterroGrpNum-4000),sock);	/* 20 respond to general Interrogation ST */
		if(ret == false)
			return(false);
#ifdef DEBUG
		printf("++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
#endif
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
#ifdef DEBUG
		printf("++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
#endif
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
#ifdef DEBUG
	printf("++++General Interrogation End! m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
#endif
	NoChangeData = 0;
	return(ret);
}

int ScanDataTransmit(int sock)
{
	int	ret;
	
#ifdef DEBUG
	printf("++++Scan m_cScanGrpNum =%d\n",m_cScanGrpNum);
#endif
	while(m_cScanGrpNum < 96)
	{
		if(GiType==1)
			ret = SinglePointInformCyc(BACK, m_cScanGrpNum,sock);	/* 1 respond to back scan M_SP_NA */
		else
			ret = SinglePointGroupCyc(BACK, m_cScanGrpNum,sock);	/* 20 respond to back scan M_PS_NA */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 96;
		}
		else
		{
			m_cScanGrpNum++;
		}
	}
	while(m_cScanGrpNum < 128)
	{
		ret = DoublePointInformCyc(BACK, (unsigned short)(m_cScanGrpNum-96),sock);	/* 20 respond to back scan DP */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 128;
		}
		else
		{
			m_cScanGrpNum++;
		}
	}
	while(m_cScanGrpNum < 160)
	{
		ret = StepPositionInformCyc((unsigned char)BACK, (unsigned short)(m_cScanGrpNum-128),sock);	/* 20 respond to general Interrogation DP */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 160;
		}
		else
		{
			m_cScanGrpNum++;
		}
	}
	while(m_cScanGrpNum < 224)
	{
		ret = MeasureValueNACyc((unsigned char)BACK,(unsigned short)(m_cScanGrpNum-160),sock);	/* 9 respond to back scan M_ME_NA */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 224;
		}
		else
		{
			m_cScanGrpNum++;
		}
	}
	while(m_cScanGrpNum < 255)
	{
		ret = MeasureValueNCCyc((unsigned char)BACK,(unsigned short)(m_cScanGrpNum-224),sock);	/* 9 respond to back scan M_ME_NA */
		if(NoChangeData != 1)
		{
			m_cScanGrpNum++;
		}
	}
#ifdef DEBUG
	printf("++++Scan End! m_cScanGrpNum =%d\n",m_cScanGrpNum);
#endif
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
#ifdef DEBUG
			printf("++++m_cRespParamNum =%d\n",m_cRespParamNum);
#endif
			return(true);
		}
		m_cRespParamNum = 1000;
	}
	if(m_cRespParamNum < 2000)
	{
		pntnum = m_cRespParamNum - 1000;
		while(pntnum<mescaledef.number)
		{
			if(strcmp(mescaledef.name[pntnum],"NULL")==0)
			{
				m_cRespParamNum++;
				continue;
			}
			setvalue=(short)(mescaledef.databand[pntnum] * mescaledef.scale[pntnum]);
			pntaddr = pntnum + me_scale_addr - me_base_addr;
			ParamValueNBAck((unsigned char)INTRO14,(unsigned char)1,(unsigned short)(pntaddr+pm_base_addr),setvalue,sockfd);
			m_cRespParamNum++;
#ifdef DEBUG
			printf("++++m_cRespParamNum =%d pntnum=%d\n",m_cRespParamNum,pntnum);
#endif
			return(true);
		}
		m_cRespParamNum = 2000;
	}
	if(m_cRespParamNum < 3000)
	{
		pntnum = m_cRespParamNum - 2000;
		while(pntnum<mefloatdef.number)
		{
			if(strcmp(mefloatdef.name[pntnum],"NULL")==0)
			{
				m_cRespParamNum++;
				continue;
			}
			pntaddr = pntnum + me_float_addr - me_base_addr;
			ParamValueNCAck((unsigned char)INTRO14,(unsigned char)1,(unsigned short)(pntaddr+pm_base_addr),mefloatdef.databand[m_cRespParamNum],sockfd);
			m_cRespParamNum++;
#ifdef DEBUG
			printf("++++m_cRespParamNum =%d pntnum=%d\n",m_cRespParamNum,pntnum);
#endif
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
	CheckACD();
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
	  
int	RespondUserDataClass1(int sock)
{
	int ret;

	NoChangeData = 0;
/*
	if(initflag ==1)
	{
		ret = EndofInitialization(sock);
		initflag = 0;
		return(ret);
	}
	if(cmdbuf.ack_flag ==1)
	{
		ret = CmdRespSend(sock);
		if(!NoChangeData)
			return(ret);
	}

	if(sebuf.ack_flag ==1)
	{
		sebuf.ack_flag = 0;
		ret = SERespSend(sock);
		if(!NoChangeData)
			return(ret);
	}
*/

	if(MirrorNAckflag)
	{
		MirrorNAckflag = 0;
		ret = MirrorNAck(mirbuf.cot,mirbuf.pnt,sock);	/* Mirror NACK */
		return(ret);
	}
/*
	if(icbuf.ack_flag)
	{
		icbuf.ack_flag = 0;
		ret = GeneralInterrogationAck(icbuf.cot,icbuf.qoi,sock);
		return(ret);
	}

	if(rdbuf.ack_flag ==1)
	{
		rdbuf.ack_flag = 0;
		if(rdbuf.cot == REQ)
		{
			ret = ReadCmdResp(rdbuf.infoaddr,sock);									
		}
		else
		{
			ret = ReadCmdAck(rdbuf.cot,rdbuf.infoaddr,sock);
		}
		return(ret);
	}

	if(testbuf.ack_flag ==1)
	{
		testbuf.ack_flag = 0;
		ret = RespondTestAck(sock);
		return(ret);
	}

	if(rpbuf.ack_flag)
	{
		rpbuf.ack_flag = 0;
		ret = ResetProcessAck(sock);
		return(ret);
	}
*/
	if(pmbuf.ack_flag ==1)
	{
		pmbuf.ack_flag = 0;
		ret = PMRespSend(sock);
		if(!NoChangeData)
			return(ret);
	}
/*
	if(cibuf.ack_flag)
	{
		cibuf.ack_flag = 0;
		ret = CounterInterrogationAck(cibuf.cot,cibuf.qcc,sock);
		return(ret);
	}
	if(soe_overflow_flag)
	{
		soe_overflow_flag = 0;
		ret = SysInform((unsigned short)1,sock);
		return(ret);
	}

	if(ClkDiffflag)
	{
		ClkDiffflag = 0;
		ret = SysInform((unsigned short)8,sock);
		return(ret);
	}

*/
	NoChangeData = 0;
	if(sp_cos == 1)
	{
		ret = SinglePointInform(SPONT,sock);
		if(ret == false)
			return(false);

		NoChangeData = 0;
		ret = DoublePointInform(SPONT,sock);
		if(ret == false)
			return(false);
/*
		NoChangeData = 0;
		ret = StepPositionInform(SPONT,sock);
		if(!NoChangeData)
			return(ret);
*/
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
		ret = MeasureValueNA(SPONT,sock);	/* M_ME_NA */
	else if(MeASDUType==1)
		ret = MeasureValueNB(SPONT,sock);	/* M_ME_NB */
	else if(MeASDUType==2)
		ret = MeasureValueND(SPONT,sock);	/* M_ME_ND */
	else
		ret = MeasureValueNC(SPONT,sock);	/* M_ME_NC */
	if(ret == false)
		return(false);
/*
	ret = MeasureValueNA(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueNB(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueNC(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueTA(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueTB(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueTC(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueTD(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueTE(SPONT,sock);
	if(!NoChangeData)
		return(ret);

	ret = MeasureValueTF(SPONT,sock);
	if(!NoChangeData)
		return(ret);

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
*/

	if(sp_cos == 1)
	{
		ret = SequenceofEvent(sock);
		if(NoChangeData == 0)
			return(ret);
	}
	return(ret);
}

int	CheckACD(void)
{
/* Check for command soe response data */
	if((cmdbuf.ack_flag ==1)||(sebuf.ack_flag ==1)||interrogationflag||counterflag
		||ClockSyncflag ||(rdbuf.ack_flag ==1)||(testbuf.ack_flag ==1)
		||(rpbuf.ack_flag ==1)||(pmbuf.ack_flag ==1)||(icbuf.ack_flag ==1)
		||(cibuf.ack_flag ==1)||(icgroupflag ==1)||(cigroupflag ==1)
		||(ClkDiffflag ==1)||(MirrorNAckflag == 1)||(Paramflag ==1)
		||(soe_overflow_flag ==1)||(soebuf.sendpnt != soebuf.bufpnt))
	{
#ifdef DEBUG
		printf("!!!Command or soe response data exist!\n");
#endif
		return(true);
	}

	if(sp_cos == 1)
	{
/* Check for SP status Change */
		CheckSinglePointChange();
		if(NoChangeData)
		{
#ifdef DEBUG
			printf("!!!CheckSinglePointChange data exist!\n");
#endif
			return(true);
		}

/* Check for DP status Change */
		CheckDoublePointChange();
		if(NoChangeData)
		{
#ifdef DEBUG
			printf("!!!CheckDoublePointChange data exist!\n");
#endif
			return(true);
		}

/* Check for ST status Change */
		CheckStepPositionChange();
		if(NoChangeData)
		{
#ifdef DEBUG
			printf("!!!CheckStepPositionChange data exist!\n");
#endif
			return(true);
		}
	}

/* No Indication Class 1 data, Check for Analog Change class 1 data */
	CheckMeasureValueNA();	/* M_ME_NA */
	if(NoChangeData)
	{
#ifdef DEBUG
		printf("!!!CheckMeasureValueNA data exist!\n");
#endif
		return(true);
	}

	CheckMeasureValueNC();	/* M_ME_NC */
	if(NoChangeData)
	{
#ifdef DEBUG
		printf("!!!CheckMeasureValueNC data exist!\n");
#endif
		return(true);
	}
/* Check for SOE data */
/*
	if(soebuf.sendpnt != soebuf.bufpnt)
	{
#ifdef DEBUG
		printf("!!!Check for SOE data exist!\n");
#endif
		acd = 1;
		return(true);
	}
*/
	return(false);
}

int	CheckMEACD(void)
{
/* Check for command soe response data */
	if(soebuf.sendpnt != soebuf.bufpnt)
	{
#ifdef DEBUG
		printf("!!!soe data exist!\n");
#endif
		return(true);
	}

/* No Indication Class 1 data, Check for Analog Change class 1 data */
	CheckMeasureValueNA();	/* M_ME_NA */
	if(NoChangeData)
	{
#ifdef DEBUG
		printf("!!!CheckMeasureValueNA data exist!\n");
#endif
		return(true);
	}

	CheckMeasureValueNC();	/* M_ME_NC */
	if(NoChangeData)
	{
#ifdef DEBUG
		printf("!!!CheckMeasureValueNC data exist!\n");
#endif
		return(true);
	}
	return(false);
}

int	RespondUserDataClass2(int sock)
{
	int	ret;

/* Check if there are class 2 data */
	if(scanflag==0)
	{
		ret = RespondUserDataClass1(sock);
		return(ret);
	}

/* Scan data */
	if(scanflag)
	{
#ifdef DEBUG
		printf("!!!ScanDataTransmit\n");
#endif
		ret = ScanDataTransmit(sock);
		return(ret);
	}
	NoChangeData = 1;
	return(true);
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
		nTypeID = soebuf.soerec[soepnt].tpyeId;

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

		default :

#ifdef DEBUG
			printf("SOE No this TypeID = %d\n",nTypeID);
#endif
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

void	PrintSendData(unsigned short nbsend)
{
	unsigned short d;

	for(d=0;d<nbsend;d++)
	{
		if((d%20)==0)
			printf("\n<< ");
		else if((d%10)==0)
			printf(" ");
		printf(" %02X",sendbuf[d]);
	}
	printf("\n");
}

void	PrintRecvData(unsigned short nbrecv)
{
	unsigned short	d;

	for(d=0;d<nbrecv;d++)
	{
		if((d%20)==0)
			printf("\n>> ");
		else if((d%10)==0)
			printf(" ");
		printf(" %02X",recvbuf[d]);
	}
	printf("\n");
}

