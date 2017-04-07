/****************************************************************
*  FILE NAME:           iec101s_fun.c      			*
*  FUNCTION:            Controlled Station			*
*			Basic Communication Function		*
*  NAME :  							*
*  DATE:                May,2006               			*
*								*
****************************************************************/

#include "../inc/iec101s_com.h"

#define	DEBUG

extern	unsigned char	sendbuf[1024];/*,cmdsendbuf[24] */
extern	unsigned char	recvbuf[1024];
extern	char	infobuf[1024];

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
extern	CSBUF	csbuf;

/*
extern	ICGROUPBUF	icgroupbuf;
extern	CIGROUPBUF	cigroupbuf;
*/
extern	unsigned short	sp_base_addr;
extern	unsigned short	dp_base_addr;
extern	unsigned short	me_base_addr;
extern	unsigned short	cyc_base_addr;
extern	unsigned short	pm_base_addr;
extern	unsigned short	it_base_addr;
extern	unsigned short	st_base_addr;
extern	unsigned short	bo_base_addr;

extern	unsigned short	sc_cmd_base_addr;
extern	unsigned short	dc_cmd_base_addr;
extern	unsigned short	rc_cmd_base_addr;
extern	unsigned short	se_cmd_base_addr;
extern	unsigned short	bc_cmd_base_addr;

extern	unsigned short	nStnId;
extern	unsigned short	nASDUId;
extern	ASDUDATA		send_data,send_data2;
extern	ASDUCOMDDATA	recv_data;

/*
extern	unsigned int	dwErrors;
extern	COMSTAT	cstat;
*/
extern	int	acd,dfc,coi;
extern	int	pollflag,comdflag,cmdterm;
extern	int	Readflag,ResetProcessflag,localinitflag;
extern	int	Paramflag,MirrorNAckflag;
extern	int	interrogationflag,counterflag,icgroupflag,cigroupflag;
extern	int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
extern	int	cyclicflag,scanflag;
extern	unsigned short	delayalarm;
extern	unsigned char	cot_interrogation,cot_counter;
extern	unsigned int	repeatnum,timeoutnum;
extern	unsigned short	seladdr;

extern	time_t	scan_tim;
extern	time_t	cycle_tim;

extern	SPDEF	spdef;
extern	DPDEF	dpdef;
extern	MEDEF	medef;
extern	MEDEF	medef;
extern	CYCDEF	cycdef;
extern	STDEF	stdef;
extern	ITDEF	itdef;
/*
extern	SCDEF	scdef;
extern	DCDEF	dcdef;
extern	SEDEF	sedef;
extern	RCDEF	rcdef;
*/
extern	int	GiType;		/*General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
extern	int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 2: M_ME_ND; 3: M_ME_NC */
extern	int	MeType;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int	SEType;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	short	me_code;	/* 0 or -32768 */
extern	short	se_code;	/* 0 or -32768 */
/*extern	int		TimeTag; */	/*Time tag> 0: no; 1: short; 2: long */
extern	int	sp_cos;		/*sp_cos> 0: sp/dp transmit once; 1: twice */
extern	int	Cmd_Sel;	/*  0: cmd without select; 1: cmd with select */
extern	SYSTEMTIME		old_local_tm;

int	NoChangeData;
int retn;

extern int		fixframe_len;
extern	int linkaddr_length;
extern	int cmmaddr_length;

BOOL	WriteSuccessf;
int	byte_to_send;	

char	send_ic_data_type,send_ci_data_type,class1_data_type,class2_data_type;
unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
unsigned short	m_cRespParamNum,m_cRespICGrpNum,m_cRespCIGrpNum;
unsigned char	count_sq,ITGrpType;

float	lo_code,hi_code;

short	organabuf[MAX_ME_NUM];
float	organafbuf[MAX_ME_NUM];
unsigned char	organastbuf[MAX_ME_NUM];

unsigned char	orgspbuf[MAX_SP_NUM],orgdpbuf[MAX_DP_NUM];
unsigned char	orgstbuf[MAX_ST_NUM],orgstvbuf[MAX_ST_NUM];

int SendFixFrame(void)
{
	if(linkaddr_length==1)
	{
		sendbuf[2]= (unsigned char)nStnId;
		sendbuf[3] = sendbuf[1] + sendbuf[2];
		sendbuf[4] = 0x16;
	}
	else if(linkaddr_length==2)
	{
		memcpy(&sendbuf[2], &nStnId, 2);
		Swap2Byte(&sendbuf[2]);
		sendbuf[4] = sendbuf[1] + sendbuf[2] + sendbuf[3];
		sendbuf[5] = 0x16;
	}
	else
	{
		OutputDspInfo(OUTPUT_ERROR, "linkaddr_length error: %d", linkaddr_length);
		return -1;
	}
	byte_to_send = fixframe_len;
	return SendData();
}


int SendVarFrame(void)
{
	char	buff[1024];
	int		asdu_length;
	int		check_length;
	unsigned char	chksum,n;

	if(linkaddr_length==1 && cmmaddr_length==1)
		return SendData();
	
	asdu_length= byte_to_send-8;
	
	memcpy(buff, sendbuf, byte_to_send);

	if(linkaddr_length==2 && cmmaddr_length==1)
	{
		memcpy(&sendbuf[5], &nStnId, 2);		/*link address field */
		Swap2Byte(&sendbuf[5]);
		memcpy(&sendbuf[7], &buff[6], asdu_length);	
		sendbuf[10] = (UCHAR)nASDUId;			/*ASDU common address */
		byte_to_send= byte_to_send+1;
	}
	else if(linkaddr_length==1 && cmmaddr_length==2)
	{
		sendbuf[5]= (UCHAR)nStnId;			/*link address field */
		memcpy(&sendbuf[9], &nASDUId, 2);   /*ASDU common address */
		Swap2Byte(&sendbuf[9]);
		memcpy(&sendbuf[11], &buff[10], asdu_length-4);
		byte_to_send= byte_to_send+1;
	}
	else if(linkaddr_length==2 && cmmaddr_length==2)
	{
		memcpy(&sendbuf[5], &nStnId, 2);		/*link address field */
		Swap2Byte(&sendbuf[5]);

		memcpy(&sendbuf[7], &buff[6], 3);	

		memcpy(&sendbuf[10], &nASDUId, 2);   /*ASDU common address */
		Swap2Byte(&sendbuf[10]);

		memcpy(&sendbuf[12], &buff[10], asdu_length-4);

		byte_to_send= byte_to_send+2;
	}

	check_length= byte_to_send-6;
	chksum = 0;
	for(n=0;n<check_length;n++)
		chksum = chksum + sendbuf[n+4];
	
	sendbuf[byte_to_send-2] = chksum;	/*checksum char */
	sendbuf[byte_to_send-1] = 0x16;		/*end char */
	sendbuf[1] = sendbuf[2]= byte_to_send-6; /*L byte_to_send = L + 6; */

	return SendData();
}



int	RespondLinkStatus(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x0B;	/*Link OK FC=11 */
	
	if(acd)
		sendbuf[1] = sendbuf[1] | 0x20;

	OutputDspInfo(0, "\nRespondLinkStatus");
	return SendFixFrame();
}


int	AckCmd(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x20;		/*ACK FC=0 ACD=1*/

	OutputDspInfo(0, "\nCommand ACK");
	return SendFixFrame();
}


int	NAckCmd(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x0F;	/*NACK: unsupport ctrlfunc */
	if(acd)
		sendbuf[1] = sendbuf[1] | 0x20;

	OutputDspInfo(0, "\nCommand NACK");
	return SendFixFrame();
}

int	NoRequestData(void)
{
	sendbuf[0] = 0x10;
	sendbuf[1] = 0x09;	/*NACK:	No Request Data	FC=9 */
	if( acd || cmdbuf.ack_flag)
		sendbuf[1] = sendbuf[1] | 0x20;

	OutputDspInfo(0, "\nNoRequestData");
	return SendFixFrame();
}

int	SingleRespondByte(void)
{
	sendbuf[0] = 0xE5;
	byte_to_send = 1;

	OutputDspInfo(0, "\nSingleRespondByte");
	return SendData();
}

int	RepeatTransmit(void)
{
	OutputDspInfo(0, "\nIEC101s repeat transmitted data");
	return SendData();
}

int	CmdRespSend(void)
{
	unsigned char	type_id;

	type_id = cmdbuf.typeId;

	if(cmdbuf.cot==ACTTERM)
	{
		cmdbuf.ack_flag = 0;
		CheckACD();
	}
	NoChangeData = 0;
	switch(type_id)
	{
	case C_SC_NA:

		SinglePointCtrlAck(cmdbuf.cot,cmdbuf.infoaddr,cmdbuf.stat);
		if(cmdbuf.term_flag ==1)
		{
			cmdbuf.term_flag = 0;
			if(cmdbuf.cot==ACTCON)
			{
				cmdbuf.cot = ACTTERM;		/*COT:10  activation terminal */
				return(0);
			}
		}
		cmdbuf.ack_flag = 0;
		break;

	case C_DC_NA:

		DoublePointCtrlAck(cmdbuf.cot,cmdbuf.infoaddr,cmdbuf.stat);
		if(cmdbuf.term_flag ==1)
		{
			cmdbuf.term_flag = 0;
			if(cmdbuf.cot==ACTCON)
			{
				cmdbuf.cot = ACTTERM;		/*COT:10  activation terminal */
				return(0);
			}
		}
		cmdbuf.ack_flag = 0;
		break;

	case C_RC_NA:

		RegulatStepCtrlAck(cmdbuf.cot,cmdbuf.infoaddr,cmdbuf.stat);
		if(cmdbuf.term_flag ==1)
		{
			cmdbuf.term_flag = 0;
			if(cmdbuf.cot==ACTCON)
			{
				cmdbuf.cot = ACTTERM;		/*COT:10  activation terminal */
				return(0);
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

	return(0);
}


int	SERespSend(void)
{
	unsigned char	type_id;

	if(sebuf.cot==ACTTERM)
	{
		sebuf.ack_flag = 0;
		CheckACD();
	}

	NoChangeData = 0;
	
	type_id = sebuf.typeId;
	switch(type_id)
	{
	case C_SE_NA:

		SetPointCmdNAAck(sebuf.cot,sebuf.infoaddr,sebuf.setvalue,sebuf.qoc);
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

		SetPointCmdNBAck(sebuf.cot,sebuf.infoaddr,sebuf.setvalue,sebuf.qoc);
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

		SetPointCmdNCAck(sebuf.cot,sebuf.infoaddr,sebuf.fvalue,sebuf.qoc);
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

		SetPointCmdNDAck(sebuf.cot,sebuf.infonum,sebuf.rcvpnt,sebuf.qoc);
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
	return(0);
}

int	PMRespSend(void)
{
	unsigned char	type_id;

	NoChangeData = 0;
	pmbuf.ack_flag = 0;
	type_id = pmbuf.typeId;

	switch(type_id)
	{
	case P_ME_NA:

		ParamValueNAAck(pmbuf.cot,pmbuf.qpm,pmbuf.infoaddr,pmbuf.setvalue);
		break;

	case P_ME_NB:

		ParamValueNBAck(pmbuf.cot,pmbuf.qpm,pmbuf.infoaddr,pmbuf.setvalue);
		break;

	case P_ME_NC:

		ParamValueNCAck(pmbuf.cot,pmbuf.qpm,pmbuf.infoaddr,pmbuf.fvalue);
		break;

	default:
		OutputDspInfo(OUTPUT_WARN, "unknown setpoint type id %d\n",type_id);
			

		NoChangeData = 1;
		pmbuf.ack_flag = 0;
		break;
	}
	return(0);
}

int	MirrorNAck(unsigned char cause,unsigned int pnt)
{
	unsigned char	chksum,n;
	unsigned char	infobyte;

	sendbuf[0] = sendbuf[3] = 0x68;			/*start flag */
	infobyte = (unsigned char)recv_data.length[pnt];
	sendbuf[1] = sendbuf[2] = infobyte;		/*L */
	sendbuf[4] = 0x08;				/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	memcpy(&sendbuf[5],&recv_data.data[pnt][1],infobyte-1);	/*Milliseconds */
/*
	sendbuf[5] = (UCHAR)nStnId;			/*link address field
	sendbuf[6] = C_SE_ND;			/*Multi setpoint:136 type identifier field
	sendbuf[7] = num;			/*variable structure specify
	sendbuf[8] = cause;			/*ACTCON:7 cause of transmit:active
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address
*/
	sendbuf[8] = cause;			/*ACTCON:7 cause of transmit:active */

	byte_to_send = 6 + infobyte;

	infobyte = infobyte + 4;
	chksum = 0;
	for(n=4;n<infobyte;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[infobyte] = chksum;		/*checksum char */
	sendbuf[infobyte+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nMirror Command NAck");
	
	retn = SendVarFrame();
	return(retn);
}

int	SinglePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */

	if(acd || cmdbuf.term_flag)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_SC_NA;			/*45 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*7 ActCon cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
#else
	memcpy(&sendbuf[10],&address,2);	/* information object address */
#endif
/*	
	if(stat == 0)
		sendbuf[12] = 0;
	else
		sendbuf[12] = 1;		/*QCC:=CP8{SCS,QU,S/E}
						/*SCS 1:=ON 0:=OFF
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	/*S/E := 0:=Execute 1:= Select
*/
	sendbuf[12] = stat;

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;			/*checksum char */
	sendbuf[14] = 0x16;			/*end char */

	OutputDspInfo(0, "\nSingle Point Ctrl Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	DoublePointCtrlAck(unsigned char cause,unsigned short address,unsigned char stat)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */

	if(acd || cmdbuf.term_flag)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_DC_NA;			/*46 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*7 cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
#else
	memcpy(&sendbuf[10],&address,2);	/* information object address */
#endif
	
/*
	if(stat == 0)
		sendbuf[12] = 1;
	else
		sendbuf[12] = 2;		/*QCC:=CP8{DCS,QU,S/E}
						/*DCS 2:=ON 1:=OFF
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	/*S/E := 0:=Execute 1:= Select
*/
	sendbuf[12] = stat;

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;			/*checksum char */
	sendbuf[14] = 0x16;			/*end char */

/*	memcpy(cmdsendbuf,sendbuf,byte_to_send); */	/*copy cmd for actterm */

	OutputDspInfo(0, "\nDouble Point Ctrl Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	RegulatStepCtrlAck(unsigned char cause,unsigned short address,unsigned char stat)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd || cmdbuf.term_flag)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_RC_NA;			/*47 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*7 cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
#else
	memcpy(&sendbuf[10],&address,2);	/* information object address */
#endif
	
/*	if(stat == 0)
		sendbuf[12] = 1;
	else
		sendbuf[12] = 2;		/*QCC:=CP8{RCS,QU,S/E}
						/*RCS 2:=Higher 1:=Lower
	if(se)
		sendbuf[12] = sendbuf[12] | 0x80;	/*S/E := 0:=Execute 1:= Select
*/
	sendbuf[12] = stat;

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;			/*checksum char */
	sendbuf[14] = 0x16;			/*end char */

	OutputDspInfo(0, "\nRegulating Step Ctrl Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	SetPointCmdNAAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 11;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd || sebuf.term_flag)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_SE_NA;			/*48 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*ACTCON:7 9 cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
	CrossCopy(&sendbuf[12],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&address,2);	/* information object address */
	memcpy(&sendbuf[12],&value,2);		/* Normalized Value */
#endif
	

	sendbuf[14] = se;			/*QOC:= S/E[8] QL[1..7] */

/*
	if(se)
		sendbuf[14] = sendbuf[14] | 0x80;	/*S/E := 0:=Execute 1:= Select
*/
	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;			/*checksum char */
	sendbuf[16] = 0x16;			/*end char */

	OutputDspInfo(0, "\nSet Point Command NA Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	SetPointCmdNBAck(unsigned char cause,unsigned short address,unsigned short value,unsigned char se)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 11;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_SE_NB;			/*49 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*ACTCON:7 9 cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
	CrossCopy(&sendbuf[12],&value,2);	/* Scaled Value */
#else
	memcpy(&sendbuf[10],&address,2);	/* information object address */
	memcpy(&sendbuf[12],&value,2);		/* Scaled Value */
#endif

	sendbuf[14] = se;			/*QOC:= S/E[8] QL[1..7]
/*
	if(se)
		sendbuf[14] = sendbuf[14] | 0x80;	/*S/E := 0:=Execute 1:= Select
*/
	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;			/*checksum char */
	sendbuf[16] = 0x16;			/*end char */

	OutputDspInfo(0, "\nSet Point Command NB Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	SetPointCmdNCAck(unsigned char cause,unsigned short address,float value,unsigned char se)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 13;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=1 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_SE_NC;			/*50 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*ACTCON:7 cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
	CrossCopy(&sendbuf[12],&value,4);	/* float Value */
#else
	memcpy(&sendbuf[10],&address,2);	/* information object address */
	memcpy(&sendbuf[12],&value,4);	/* float Value */
#endif

	sendbuf[16] = se;		/*QOC:= S/E[8] QL[1..7] */
/*
	if(se)
		sendbuf[16] = sendbuf[16] | 0x80;	/*S/E := 0:=Execute 1:= Select
*/
	byte_to_send = 19;

	chksum = 0;
	for(n=4;n<17;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[17] = chksum;			/*checksum char */
	sendbuf[18] = 0x16;			/*end char */

	OutputDspInfo(0, "\nSet Point Command NC Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	SetPointCmdNDAck(unsigned char cause,unsigned char num,unsigned int pnt,unsigned char se)
{
	unsigned char	chksum,n;
	unsigned char	infobyte;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
/*	sendbuf[1] = sendbuf[2] = 11; */	/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_SE_ND;			/*Multi setpoint:136 type identifier field */
	sendbuf[7] = num;			/*variable structure specify */
	sendbuf[8] = cause;			/*ACTCON:7 cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	infobyte = num*4 + 1;
	memcpy(&sendbuf[10],&recv_data.data[pnt][6],infobyte);	/*Infoaddr,value,QOC */

	sendbuf[1] = sendbuf[2] = (unsigned char)(6 + infobyte);	/*L */
	byte_to_send = 12 + infobyte;

	infobyte = infobyte + 10;
	chksum = 0;
	for(n=4;n<infobyte;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[infobyte] = chksum;		/*checksum char */
	sendbuf[infobyte+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nMulti Set Point Command ND Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	ParamValueNAAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 11;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = P_ME_NA;			/*110 type identifier field,normialized */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
	CrossCopy(&sendbuf[12],&value,2);	/* Normalized Value */
#else
	memcpy(&sendbuf[10],&address,2);	/*information object  address */
	memcpy(&sendbuf[12],&value,2);		/*Normalized Value */
#endif
	sendbuf[14] = qpm;			/*KPA[1-6] LPC=0(Same)/1(Changed) POP=0 */
						/*KPA 1:= threshold 2:= filter factor */
						/*3:= low limit  4:= high limit */
	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;			/*checksum char */
	sendbuf[16] = 0x16;			/*end char */

	OutputDspInfo(0, "\nParameter value NA Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	ParamValueNBAck(unsigned char cause,unsigned char qpm,unsigned short address,unsigned short value)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 11;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = P_ME_NB;			/*111 type identifier field,scaled */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
	CrossCopy(&sendbuf[12],&value,2);	/* Scaled Value */
#else
	memcpy(&sendbuf[10],&address,2);	/*information object  address */
	memcpy(&sendbuf[12],&value,2);		/*Scaled Value */
#endif
	sendbuf[14] = qpm;			/*KPA[1-6] LPC POP */
						/*KPA 1:= threshold 2:= filter factor */
						/*3:= low limit  4:= high limit */
	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;			/*checksum char */
	sendbuf[16] = 0x16;			/*end char */

	OutputDspInfo(0, "\nParameter value NB Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	ParamValueNCAck(unsigned char cause,unsigned char qpm,unsigned short address,float value)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 13;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = P_ME_NC;			/*112 type identifier field,normialized */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cause;			/*cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&address,2);	/* information object address */
	CrossCopy(&sendbuf[12],&value,4);	/* float Value */
#else
	memcpy(&sendbuf[10],&address,2);	/*information object  address */
	memcpy(&sendbuf[12],&value,4);		/*float Value */
#endif
	sendbuf[14] = qpm;			/*KPA[1-6] LPC POP */
						/*KPA 1:= threshold 2:= filter factor */
						/*3:= low limit  4:= high limit */
	byte_to_send = 19;

	chksum = 0;
	for(n=4;n<17;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[17] = chksum;			/*checksum char */
	sendbuf[18] = 0x16;			/*end char */

	OutputDspInfo(0, "\nParameter value NC Ack");
	
	retn = SendVarFrame();
	return(retn);
}

int	ClockSynchroniseAck(void)
{
	unsigned char	chksum,n;
	WORD		millisec;
	extern int Gps;

	sendbuf[0] = sendbuf[3] = 0x68;	//start flag
	sendbuf[1] = sendbuf[2] = 15;		//L
	sendbuf[4] = 0x08;						//control field FC=8
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			//link address field
	sendbuf[6] = C_CS_NA;		//103 type identifier field
	sendbuf[7] = 1;				//variable structure specify
	sendbuf[8] = csbuf.cot;			//7 cause of transmit:actcon
	sendbuf[9] = (UCHAR)nASDUId;		//ASDU common address
	sendbuf[10] = 0;				//information object low address
	sendbuf[11] = 0;				//information object high address

	millisec = old_local_tm.wSecond * 1000 + old_local_tm.wMilliseconds;
	memcpy(&sendbuf[12],&millisec,2);				//Milliseconds
	sendbuf[14] = (unsigned char)old_local_tm.wMinute;		//Minutes
	sendbuf[15] = (unsigned char)old_local_tm.wHour;		//Hour
	n = old_local_tm.wDayOfWeek << 5;
	n = n & 0xE0;
	sendbuf[16] = (unsigned char)old_local_tm.wDay | n;	//DayofWeek[6..8] day[1..5]
	sendbuf[17] = (unsigned char)old_local_tm.wMonth;		//Month
	sendbuf[18] = (unsigned char)(old_local_tm.wYear%100);	//Year
	
	byte_to_send = 21;

	chksum = 0;
	for(n=4;n<19;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[19] = chksum;				//checksum char
	sendbuf[20] = 0x16;					//end char

	OutputDspInfo(0, "\nACKCON ClockSynchronisation");
	retn = SendVarFrame();
	return(retn);
}

int	TimeDelayAck(void)
{
	unsigned char	chksum,n;
	unsigned short	sdt;
	DATE_TIME	d_time; 
	time_t		sec;
	
	GetSysTimeEx(&d_time, &sec);

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 10;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_CD_NA;			/*106 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = cdbuf.cot;			/*7 cause of transmit:actcon */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
	sendbuf[10] = 0;			/*information object low address */
	sendbuf[11] = 0;			/*information object high address */

	sdt= cdbuf.cp16time+(sec-cdbuf.recv_time_sec)*1000+(d_time.ms-cdbuf.recv_time_ms);
	if(sdt > 59999)
		sdt -= 60000;
	memcpy(&sendbuf[12],&sdt,2);		/*Milliseconds*/
	Swap2Byte(&sendbuf[12]);

	byte_to_send = 16;

	chksum = 0;
	for(n=4;n<14;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[14] = chksum;			/*checksum char */
	sendbuf[15] = 0x16;			/*end char */

	OutputDspInfo(0, "\nACKCON Time Delay");
	retn = SendVarFrame();
	return(retn);
}

int	GeneralInterrogationAck(unsigned char gi_cause,unsigned char gi_qoi)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_IC_NA;			/*100 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = gi_cause;			/*7 ActCon 10 Actterm cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
	sendbuf[10] = 0;			/*information object low address */
	sendbuf[11] = 0;			/*information object high address */
	sendbuf[12] = gi_qoi;			/*QOI:20 general 34:param group	(icbuf.qoi) */
	
	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;			/*checksum char */
	sendbuf[14] = 0x16;			/*end char */

	OutputDspInfo(0, "\nGeneralInterrogationAck");
	retn = SendVarFrame();
	return(retn);
}

int	CounterInterrogationAck(unsigned char gi_cause,unsigned char gi_qcc)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_CI_NA;			/*101 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = gi_cause;			/*7 Actcon 10 Actterm cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
	sendbuf[10] = 0;			/*information object low address */
	sendbuf[11] = 0;			/*information object high address */
	sendbuf[12] = gi_qcc;			/*QCC 5 general request counter */					
						/*QCC:=CP8{RQT:=UI6[1..6],FRZ:=UI2[7..8]} */
						/*RQT 5:=general counter interrogation */
						/*RQT 1-4 := 1-4 group counter interrogation */
						/*FRZ 0:=request counter value */					
	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;		/*checksum char */
	sendbuf[14] = 0x16;		/*end char */

	OutputDspInfo(0, "\nCounterInterrogationAck");
	retn = SendVarFrame();
	return(retn);
}

int	RespondTestAck(void)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 10;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_TS_NA;			/*104 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = testbuf.cot;		/*7 Actcon cause of transmit: T=0 P/N=0 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
	sendbuf[10] = 0;			/*information object low address */
	sendbuf[11] = 0;			/*information object high address */
	sendbuf[12] = 0xAA;			/*test low byte AA */
	sendbuf[13] = 0x55;			/*test high byte 55 */

	byte_to_send = 16;

	chksum = 0;
	for(n=4;n<14;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[14] = chksum;			/*checksum char */
	sendbuf[15] = 0x16;			/*end char */

	OutputDspInfo(0, "\nTest channel Ack");
	retn = SendVarFrame();
	return(retn);
}

int	ResetProcessAck(void)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L */
	sendbuf[4] = 0x08;			/*control field FC=8 */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_RP_NA;			/*106 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = rpbuf.cot;			/*7 Actcon cause of transmit: T=1 P/N=0 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
	sendbuf[10] = 0;			/*information object low address */
	sendbuf[11] = 0;			/*information object high address */
	sendbuf[12] = rpbuf.qrp;		/*QRP[1..8] 1:reset process 2:reset soe buf */

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;			/*checksum char */
	sendbuf[14] = 0x16;			/*end char */

	OutputDspInfo(0, "\nReset Process Ack");
	retn = SendVarFrame();
	return(retn);
}

int	ReadCmdAck(unsigned char qoc,unsigned short infoaddr)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 8;		/*L */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = C_RD_NA;			/*102 type identifier */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = qoc;			/*cause of transmit:active */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
#ifdef SUN_OS
	CrossCopy(&sendbuf[10],&infoaddr,2);	/*information object  address */
#else
	memcpy(&sendbuf[10],&infoaddr,2);	/*information object  address */
#endif
	byte_to_send = 14;

	chksum = 0;
	for(n=4;n<12;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[12] = chksum;			/*checksum char */
	sendbuf[13] = 0x16;			/*end char */

	OutputDspInfo(0, "\nRead C_RD_NA Ack");
	retn = SendVarFrame();
	return(retn);
}

int	ReadCmdResp(unsigned short infoaddr)
{
	unsigned short	infopnt;

	infopnt = infoaddr - sp_base_addr;
	if((infoaddr>sp_base_addr)&&(infopnt<spdef.number))
	{
		ReadSingleInform(infoaddr);
		return(0);
	}

	infopnt = infoaddr - dp_base_addr;
	if((infoaddr>dp_base_addr)&&(infopnt<dpdef.number))
	{
		ReadDoubleInform(infoaddr);
		return(0);
	}

	infopnt = infoaddr - st_base_addr;
	if((infoaddr>st_base_addr)&&(infopnt<stdef.number))
	{
		ReadStepPositionInform(infoaddr);
		return(0);
	}

	infopnt = infoaddr - me_base_addr;
	if((infoaddr>me_base_addr)&&(infopnt<medef.number))
	{
		ReadMeasureValueNA(infoaddr);	/*M_ME_NA */
		return(0);
	}

	infopnt = infoaddr - me_base_addr;
	if((infoaddr>me_base_addr)&&(infopnt<medef.number))
	{
		ReadMeasureValueNB(infoaddr);	/*M_ME_NB */
		return(0);
	}

	infopnt = infoaddr - me_base_addr;
	if((infoaddr>me_base_addr)&&(infopnt<medef.number))
	{
		ReadMeasureValueNC(infoaddr);	/*M_ME_NB */
		return(0);
	}
	else
	{
		ReadCmdAck((unsigned char)(0x40|UKINFOAD),infoaddr);
	}
	return(0);
}

int	SysInform(unsigned short info_addr)
{
	unsigned char	chksum,n;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L	byte_to_send = L + 6; */
	sendbuf[4] = 0x08;			/*control field */

	CheckACD();
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_SP_NA;		/*1 type single point information */
	sendbuf[7] = 1;			/*variable structure specify (number of i-object) */
	sendbuf[8] = SPONT;		/*cause of transmit:spont */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	sendbuf[10] = info_addr& 0xFF;	/*i-addr low address */
	sendbuf[11] = info_addr>>8;	/*i-addr high address */
	sendbuf[12] = 1;		/*QDS IV NT SB BL RES3 SPI */

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;		/*checksum char */
	sendbuf[14] = 0x16;		/*end char */

	OutputDspInfo(0, "\nSystem information");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	SinglePointInform(unsigned char gi_cause)
{
	unsigned char	chksum,n,stat,inf_n;
	unsigned char	nLength;
	int	pntnum;
	char	name_buf[NAME_SIZE];


	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_SP_NA;		/*1 type single point information */
	sendbuf[8] = gi_cause;		/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	for(pntnum=0;pntnum<spdef.number;pntnum++)	/*Check for SP status Change */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		retn = GetSPStatByName(name_buf,&stat);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
			
			continue;
		}
		if(stat != orgspbuf[pntnum])
		{
			sendbuf[10+inf_n*3] = (sp_base_addr + pntnum)& 0xFF;	/*i-addr low address */
			sendbuf[11+inf_n*3] = (sp_base_addr + pntnum)>>8;	/*i-addr high address */
			sendbuf[12+inf_n*3] = orgspbuf[pntnum] =stat;		/*QDS IV NT SB BL RES3 SPI */
			inf_n++;
			if(inf_n >= 81)
			{
				acd = 1;
				sendbuf[4] = sendbuf[4] | 0x20;
				break;
			}
		}
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "SinglePointInform: NoRequestData\n");
			
		NoChangeData = 1;
		return(0);
	}
	if((sendbuf[4] & 0x20)==0)
	{
		CheckACD();
		if(acd)
			sendbuf[4] = sendbuf[4] | 0x20;
	}
	sendbuf[7] = inf_n;			/*variable structure specify (number of i-object) */
	nLength = sendbuf[7] * 3 + 6;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nSingle Point information");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CheckSinglePointChange(void)
{
	unsigned char	stat;
	int	pntnum;
	char	name_buf[NAME_SIZE];


	for(pntnum=0;pntnum<spdef.number;pntnum++)	/*Check for SP status Change */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		retn = GetSPStatByName(name_buf,&stat);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
			
			continue;
		}
		if(stat != orgspbuf[pntnum])
		{
			NoChangeData = 1;
			return(0);
		}
	}
	NoChangeData = 0;
	return(-1);
}

int	ReadSingleInform(unsigned short infoaddr)
{
	unsigned char	chksum,n,stat;
	int	pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L byte_to_send = L + 6; */
	sendbuf[4] = 0x08;			/*control field */
/*
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;
*/
	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_SP_NA;			/*1 type single point information */
	sendbuf[7] = 1;				/*variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	pntnum=infoaddr - sp_base_addr;		/*Check for SP status Change */
	strcpy(name_buf,spdef.name[pntnum]);
	retn = GetSPStatByName(name_buf,&stat);
	if(retn==-1)
	{
		stat = 0;
	}
	sendbuf[10] = (unsigned char)(infoaddr & 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(infoaddr>>8);	/*i-addr high address */
	sendbuf[12] = orgspbuf[pntnum] =stat;		/*QDS IV NT SB BL RES3 SPI */

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;		/*checksum char */
	sendbuf[14] = 0x16;		/*end char */

	OutputDspInfo(0, "\nRESP Read Single Point information");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	SinglePointInformCyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,stat,inf_n;
	unsigned char	nLength;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_SP_NA;			/*1 type single point information */
	sendbuf[8] = gi_cause;			/*cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = SP_GROUP_NUM*infogroup;	/*infogroup: 0,1,2...8 */
	if(spdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + sp_base_addr;
	endnum = startnum + SP_GROUP_NUM;
	if(spdef.number < endnum)
		endnum = spdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)	/*Cycle for SP status */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n] = orgspbuf[pntnum] =0;/*QDS IV NT SB BL RES3 SPI */
			inf_n++;
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		retn = GetSPStatByName(name_buf,&stat);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
			

			sendbuf[12+inf_n] = orgspbuf[pntnum] =0;	/*QDS IV NT SB BL RES3 SPI */
			inf_n++;
			continue;
		}

		sendbuf[12+inf_n] = orgspbuf[pntnum] =stat;		/*QDS IV NT SB BL RES3 SPI */
		inf_n++;
	}
	if(inf_n == 0)
	{
#ifdef DEBUG
		OutputDspInfo(0, "SinglePointInformCyc: NoRequestData\n");
			
#endif
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;		/*SQ=1 variable structure specify (number of i-object) */
	nLength = inf_n + 8;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nSingle Point information Cyc");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	SinglePointInformTA(unsigned char gi_cause)
{
	unsigned short	millisec;
	unsigned char	chksum,n;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_SP_TA;			/*2 type single point information with time tag */
	sendbuf[8] = gi_cause;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	acd = 0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_SP_TA) || (nInfoNum >= 40))
		{
			acd = 1;
			break;
		}
		sendbuf[10+nInfoNum*6] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/*information object low address */
		sendbuf[11+nInfoNum*6] = (soebuf.soerec[soepnt].infoaddr)>>8;		/*information object high address */
		sendbuf[12+nInfoNum*6] = soebuf.soerec[soepnt].stat;			/*QDS IV NT SB BL RES3 SPI */						

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[13+nInfoNum*6],&millisec,2);		/*Milliseconds */
#else
		memcpy(&sendbuf[13+nInfoNum*6],&millisec,2);		/*Milliseconds */
#endif
		sendbuf[15+nInfoNum*6] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/*Minutes */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		OutputDspInfo(0, "Single Point information M_SP_TA: NoRequestData\n");
			
#endif
		NoChangeData = 1;
		return(0);
	}
	if(acd==0)
		CheckACD();
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}
	sendbuf[7] = nInfoNum;			/*variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 6 + 6;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nSingle Point information TA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	SinglePointInformTB(unsigned char gi_cause)
{
	unsigned short	millisec;
	unsigned char	chksum,n;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_SP_TB;			/*30 type single point information with time tag */
	sendbuf[8] = gi_cause;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	acd = 0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_SP_TB) || (nInfoNum >= 24))
		{
			acd = 1;
			break;
		}
		sendbuf[10+nInfoNum*10] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/*information object low address */
		sendbuf[11+nInfoNum*10] = (soebuf.soerec[soepnt].infoaddr)>>8;		/*information object high address */
		sendbuf[12+nInfoNum*10] = soebuf.soerec[soepnt].stat;			/*QDS IV NT SB BL RES3 SPI */						

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[13+nInfoNum*10],&millisec,2);	/*Milliseconds */
#else
		memcpy(&sendbuf[13+nInfoNum*10],&millisec,2);		/*Milliseconds */
#endif
		sendbuf[15+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/*Minutes */
		sendbuf[16+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/*Hours(0-23) */
		sendbuf[17+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.day;
												/*Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[18+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.month;	/*Months(1-12) */
		sendbuf[19+nInfoNum*10] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/*Years(0-99) */

		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
#ifdef DEBUG
		OutputDspInfo(0, "Single Point information M_SP_TB: NoRequestData\n");
			
#endif
		NoChangeData = 1;
		return(0);
	}
	if(acd==0)
		CheckACD();
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}
	sendbuf[7] = nInfoNum;			/*variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 10 + 6;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nSingle Point information TB");
	
	NoChangeData = 0;
	retn = SendVarFrame();
	return(retn);
}

int	SinglePointGroupCyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,stat,inf_n;
	unsigned char	nLength;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	unsigned short	stat16,scd16,bitmap;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_PS_NA;			/*20 type single point group information */
	sendbuf[8] = gi_cause;			/*cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = PS_GROUP_NUM*infogroup;	/*infogroup: 0,1,2...8 */
	if(spdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + sp_base_addr;
	endnum = startnum + PS_GROUP_NUM;
	if(spdef.number < endnum)
		endnum = spdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	bitmap = 1;
	stat16 = 0;
	scd16 = 0xFFFF;
	for(pntnum=startnum;pntnum<endnum;pntnum++)		/*Cycle for SP status */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			orgspbuf[pntnum] =0;			/*QDS IV NT SB BL RES3 SPI */
		}
		else
		{
			strcpy(name_buf,spdef.name[pntnum]);
			retn = GetSPStatByName(name_buf,&stat);
			if(retn==-1)
			{
				OutputDspInfo(OUTPUT_WARN, "%s No this SP point in the database file!\n",name_buf);
					
				orgspbuf[pntnum] =0;		/*QDS IV NT SB BL RES3 SPI */
			}
			else
			{
				orgspbuf[pntnum] =stat;		/*QDS IV NT SB BL RES3 SPI */
				if(stat & 1)
				{
					stat16 = stat16 | bitmap;	
				}
			}
		}
		bitmap = bitmap<<1;
		if(bitmap == 0)
		{
#ifdef SUN_OS
			CrossCopy(&sendbuf[12+inf_n*5],&stat16,2);	/*SCD:stat16 */
			CrossCopy(&sendbuf[14+inf_n*5],&scd16,2);	/*SCD:scd16 */
#else
			memcpy(&sendbuf[12+inf_n*5],&stat16,2);		/*SCD:stat16 */
			memcpy(&sendbuf[14+inf_n*5],&scd16,2);		/*SCD:scd16 */
#endif
			sendbuf[16+inf_n*5] = 0;
			inf_n++;
			bitmap = 1;
			stat16 = 0;
		}
	}
	if(bitmap != 1)
	{
#ifdef SUN_OS
		CrossCopy(&sendbuf[12+inf_n*5],&stat16,2);	/*SCD:stat16 */
		CrossCopy(&sendbuf[14+inf_n*5],&scd16,2);	/*SCD:scd16 */
#else
		memcpy(&sendbuf[12+inf_n*5],&stat16,2);		/*SCD:stat16 */
		memcpy(&sendbuf[14+inf_n*5],&scd16,2);		/*SCD:scd16 */
#endif
		sendbuf[16+inf_n*5] = 0;
		inf_n++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "SinglePointGroupCyc: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;		/*SQ=1 variable structure specify (number of i-object) */
	nLength = 8 + inf_n*5;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nSingle Point information Group Cyc");
	
	NoChangeData = 0;
	retn = SendVarFrame();
	return(retn);
}

int	DoublePointInform(unsigned char gi_cause)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_DP_NA;			/*3 type double point information */
	sendbuf[8] = gi_cause;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	inf_n = 0;
	for(pntnum=0;pntnum<dpdef.number;pntnum++)	/*Check for DP status Change */
	{
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,dpdef.name[pntnum]);
		retn = GetDPStatByName(name_buf,&stat);
		if(retn==-1)
		{
			orgdpbuf[pntnum] =0;		/*QDS IV NT SB BL RES2 DPI */
			continue;
		}
		if(stat != orgdpbuf[pntnum])
		{
			OutputDspInfo(0, "Check DP %s changed: %u old %u", dpdef.name[pntnum], stat, orgdpbuf[pntnum]);
			sendbuf[10+inf_n*3] = (dp_base_addr + pntnum)& 0xFF;	/*i-addr low address */
			sendbuf[11+inf_n*3] = (dp_base_addr + pntnum)>>8;	/*i-addr high address */
			sendbuf[12+inf_n*3] = orgdpbuf[pntnum] = stat;		/*OFF:QDS IV NT SB BL RES2 DPI */
			inf_n++;
			if(inf_n >= 81)
			{
				acd = 1;
				sendbuf[4] = sendbuf[4] | 0x20;
				break;
			}
		}
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "DoublePointInform: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	if((sendbuf[4] & 0x20)==0)
	{
		CheckACD();
		if(acd)
			sendbuf[4] = sendbuf[4] | 0x20;
	}
	sendbuf[7] = inf_n;			/*variable structure specify (number of i-object) */
	nLength = sendbuf[7] * 3 + 6;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nDouble Point information");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CheckDoublePointChange(void)
{
	unsigned char	stat;
	int		pntnum;

	for(pntnum=0;pntnum<dpdef.number;pntnum++)		/*Check for DP status Change */
	{
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		retn = GetDPStatByName(dpdef.name[pntnum],&stat);
		if(retn==-1)
		{
			continue;
		}
		if(stat != orgdpbuf[pntnum])
		{
			OutputDspInfo(0, "Check DP %s changed: %u old %u", dpdef.name[pntnum], stat, orgdpbuf[pntnum]);
			NoChangeData = 1;
			return(0);
		}
	}
	NoChangeData = 0;
	return(-1);
}

int	ReadDoubleInform(unsigned short infoaddr)
{
	unsigned char	chksum,n,stat;
	int		pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L	byte_to_send = L + 6; */
	sendbuf[4] = 0x08;			/*control field */
/*
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;
*/
	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_DP_NA;		/*3 type single point information */
	sendbuf[7] = 1;			/*variable structure specify (number of i-object) */
	sendbuf[8] = REQ;		/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	pntnum=infoaddr - dp_base_addr;		/*Check for DSP status Change */
	strcpy(name_buf,dpdef.name[pntnum]);
	retn = GetDPStatByName(name_buf,&stat);
	if(retn==-1)
	{
		stat = 0;
	}
	sendbuf[10] = (unsigned char)(infoaddr & 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(infoaddr>>8);	/*i-addr high address */
	sendbuf[12] = orgdpbuf[pntnum] =stat;		/*QDS IV NT SB BL RES2 DPI */

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;	/*checksum char */
	sendbuf[14] = 0x16;	/*end char */

	OutputDspInfo(0, "\nRead Double Point information");
	
	retn = SendVarFrame();
	return(retn);
}

int	DoublePointInformCyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,stat,inf_n;
	unsigned char	nLength;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;


	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_DP_NA;		/*3 type double point information */
	sendbuf[8] = gi_cause;		/*cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	startnum = DP_GROUP_NUM*infogroup;	/*infogroup: 0,1,2...8 */
	if(dpdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + dp_base_addr;	
	endnum = startnum + DP_GROUP_NUM;
	if(dpdef.number < endnum)
		endnum = dpdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)		/*Cycle for DP status */
	{
		if(strcmp(dpdef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n] = orgdpbuf[pntnum] =0;	/*QDS IV NT SB BL RES2 DPI */
			inf_n++;
			continue;
		}
		strcpy(name_buf,dpdef.name[pntnum]);
		retn = GetDPStatByName(name_buf,&stat);
		if(retn==-1)
		{
			sendbuf[12+inf_n] = orgdpbuf[pntnum] =0;	/*QDS IV NT SB BL RES2 DPI */
			inf_n++;
			continue;
		}
		sendbuf[12+inf_n] = orgdpbuf[pntnum] = stat;	/*OFF:QDS IV NT SB BL RES2 DPI */

		inf_n++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "DoublePointInformCyc: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;		/*SQ=1 variable structure specify (number of i-object) */
	nLength = inf_n + 8;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nDouble Point information Cyc");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	DoublePointInformTA(unsigned char gi_cause)
{
	unsigned short	millisec;
	unsigned char	chksum,n;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_DP_TA;			/*4 type single point information with time tag */
	sendbuf[8] = gi_cause;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	acd = 0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_DP_TA) || (nInfoNum >= 40))
		{
			acd = 1;
			break;
		}
		sendbuf[10+nInfoNum*6] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/*information object low address */
		sendbuf[11+nInfoNum*6] = (soebuf.soerec[soepnt].infoaddr)>>8;		/*information object high address */
		sendbuf[12+nInfoNum*6] = soebuf.soerec[soepnt].stat;	/*QDS IV NT SB BL RES2 DPI */			

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[13+nInfoNum*6],&millisec,2);		/*Milliseconds */
#else
		memcpy(&sendbuf[13+nInfoNum*6],&millisec,2);		/*Milliseconds */
#endif
		sendbuf[15+nInfoNum*6] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/*Minutes */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
		OutputDspInfo(0, "Double Point information M_DP_TA: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	if(acd==0)
		CheckACD();
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}
	sendbuf[7] = nInfoNum;			/*variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 6 + 6;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */
	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nDouble Point information TA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	DoublePointInformTB(unsigned char gi_cause)
{
	unsigned short	millisec;
	unsigned char	chksum,n;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_DP_TB;			/*31 type single point information with time tag */
	sendbuf[8] = gi_cause;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	acd = 0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_DP_TB) || (nInfoNum >= 24))
		{
			acd = 1;
			break;
		}
		sendbuf[10+nInfoNum*10] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/*information object low address */
		sendbuf[11+nInfoNum*10] = (soebuf.soerec[soepnt].infoaddr)>>8;		/*information object high address */
		sendbuf[12+nInfoNum*10] = soebuf.soerec[soepnt].stat;				/*QDS IV NT SB BL RES2 DPI */					

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[13+nInfoNum*10],&millisec,2);	/*Milliseconds */
#else
		memcpy(&sendbuf[13+nInfoNum*10],&millisec,2);		/*Milliseconds */
#endif
		sendbuf[15+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/*Minutes */
		sendbuf[16+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/*Hours(0-23) */
		sendbuf[17+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.day;
												/*Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[18+nInfoNum*10] = (unsigned char)soebuf.soerec[soepnt].time.month;	/*Months(1-12) */
		sendbuf[19+nInfoNum*10] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/*Years(0-99) */

		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
		OutputDspInfo(0, "Double Point information M_DP_TB: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	if(acd==0)
		CheckACD();
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;		/*control field */
	}
	sendbuf[7] = nInfoNum;				/*variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 10 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */
	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nDouble Point information TB");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	StepPositionInform(unsigned char gi_cause)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat,vti;
	int		pntnum;
	char		name_buf[NAME_SIZE];

	sendbuf[0] = sendbuf[3] = 0x68;			/*start flag */
	sendbuf[4] = 0x08;				/*control field */

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ST_NA;		/*5 type step position information */
	sendbuf[8] = gi_cause;		/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	for(pntnum=0;pntnum<stdef.number;pntnum++)		/*Check for ST status Change */
	{
		if(strcmp(stdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,stdef.name[pntnum]);
		retn = GetSTStatByName(name_buf,&vti,&stat);
		if(retn==-1)
		{
			continue;
		}
		if((stat != orgstbuf[pntnum])||(vti!= orgstvbuf[pntnum])) 
		{
			sendbuf[10+inf_n*4] = (st_base_addr + pntnum)& 0xFF;	/*i-addr low address */
			sendbuf[11+inf_n*4] = (st_base_addr + pntnum)>>8;	/*i-addr high address */
			sendbuf[12+inf_n*4] = orgstvbuf[pntnum] = vti;		/*VTI[1..7]:-64 63 */
			sendbuf[13+inf_n*4] = orgstbuf[pntnum] = stat;		/*OFF:QDS IV NT SB BL RES3 OV */
			inf_n++;
			if(inf_n >= 40)
			{
				acd = 1;
				sendbuf[4] = sendbuf[4] | 0x20;
				break;
			}
		}
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "StepPositionInform: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	if((sendbuf[4] & 0x20)==0)
	{
		CheckACD();
		if(acd)
			sendbuf[4] = sendbuf[4] | 0x20;
	}
	sendbuf[7] = inf_n;			/*variable structure specify (number of i-object) */
	nLength = sendbuf[7] * 4 + 6;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nStep Position information");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CheckStepPositionChange(void)
{
	unsigned char	stat,vti;
	int		pntnum;
	char		name_buf[NAME_SIZE];

	for(pntnum=0;pntnum<stdef.number;pntnum++)	/*Check for ST status Change */
	{
		if(strcmp(stdef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,stdef.name[pntnum]);
		retn = GetSTStatByName(name_buf,&vti,&stat);
		if(retn==-1)
		{
			continue;
		}
		if((stat != orgstbuf[pntnum])||(vti != orgstvbuf[pntnum]))
		{
			NoChangeData = 1;
			return(0);
		}
	}
	NoChangeData = 0;
	return(-1);
}

int	ReadStepPositionInform(unsigned short infoaddr)
{
	unsigned char	chksum,n,stat,vti;
	int				pntnum;
	char	name_buf[NAME_SIZE];

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L byte_to_send = L + 6; */
	sendbuf[4] = 0x08;			/*control field */
/*
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;
*/
	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ST_NA;			/*5 type single point information */
	sendbuf[7] = 1;				/*variable structure specify (number of i-object) */
	sendbuf[8] = REQ;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	pntnum=infoaddr - st_base_addr;		/*Check for DSP status Change */
	strcpy(name_buf,stdef.name[pntnum]);
	retn = GetSTStatByName(name_buf,&vti,&stat);
	if(retn==-1)
	{
			vti = stat = 0;
	}
	sendbuf[10] = (unsigned char)(infoaddr & 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(infoaddr>>8);	/*i-addr high address */
	sendbuf[12] = orgstvbuf[pntnum] =vti;		/*VTI */
	sendbuf[12] = orgstbuf[pntnum] =stat;		/*QDS IV NT SB BL RES3 OV */

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;		/*checksum char */
	sendbuf[14] = 0x16;		/*end char */

	OutputDspInfo(0, "\nRead StepPosition information");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	StepPositionInformCyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,stat,inf_n;
	unsigned char	nLength,vti;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;


	sendbuf[0] = sendbuf[3] = 0x68;			/*start flag */
	sendbuf[4] = 0x08;				/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ST_NA;		/*5 type step position information */
	sendbuf[8] = gi_cause;		/*cause of transmit:INTROGEN:20 INTRO1:21 INTRO2:22 ... INTRO8:28 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	startnum = ST_GROUP_NUM*infogroup;	/*infogroup: 0,1,2...8 */
	if(stdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + st_base_addr;	
	endnum = startnum + ST_GROUP_NUM;
	if(stdef.number < endnum)
		endnum = stdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)		/*Cycle for DP status */
	{
		if(strcmp(stdef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n*2] = orgstvbuf[pntnum] =0;	/*VTI */
			sendbuf[13+inf_n*2] = orgstbuf[pntnum] =0;	/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,stdef.name[pntnum]);
		retn = GetSTStatByName(name_buf,&vti,&stat);
		if(retn==-1)
		{
			sendbuf[12+inf_n*2] = orgstvbuf[pntnum] =0;	/*VTI */
			sendbuf[13+inf_n*2] = orgstbuf[pntnum] =0;	/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		sendbuf[12+inf_n*2] = orgstvbuf[pntnum] = vti;		/*VTI */
		sendbuf[13+inf_n*2] = orgstbuf[pntnum] = stat;		/*OFF:QDS IV NT SB BL RES3 OV */

		inf_n++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "StepPositionInformCyc: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;		/*SQ=1 variable structure specify (number of i-object) */
	nLength = inf_n*2 + 8;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */
	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nStepPosition information Cyc");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	StepPositionInformTA(unsigned char gi_cause)
{
	unsigned short	millisec;
	unsigned char	chksum,n;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ST_TA;			/*5 type step position information */
	sendbuf[8] = gi_cause;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	acd = 0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_ST_TA) || (nInfoNum >= 20))
		{
			acd = 1;
			break;
		}
		sendbuf[10+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/*information object low address */
		sendbuf[11+nInfoNum*7] = (soebuf.soerec[soepnt].infoaddr)>>8;		/*information object high address */
		sendbuf[12+nInfoNum*7] = soebuf.soerec[soepnt].vti;			/*VTI */
		sendbuf[13+nInfoNum*7] = soebuf.soerec[soepnt].stat;			/*QDS IV NT SB BL RES3 OV */						

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[14+nInfoNum*7],&millisec,2);		/*Milliseconds */
#else
		memcpy(&sendbuf[14+nInfoNum*7],&millisec,2);		/*Milliseconds */
#endif
		sendbuf[16+nInfoNum*7] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/*Minutes */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
		OutputDspInfo(0, "StepPosition information M_ST_TA: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	if(acd==0)
		CheckACD();
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}
	sendbuf[7] = nInfoNum;				/*variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 7 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */
	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nStepPosition information TA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	StepPositionInformTB(unsigned char gi_cause)
{
	unsigned short	millisec;
	unsigned char	chksum,n;
	unsigned char	nLength,nInfoNum;
	unsigned short	soepnt;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ST_TB;			/*31 type single point information with time tag */
	sendbuf[8] = gi_cause;			/*cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	soepnt = soebuf.sendpnt;

	nInfoNum=0;
	acd = 0;
	while(soepnt != soebuf.bufpnt)
	{
		if((soebuf.soerec[soepnt].typeId != M_ST_TB) || (nInfoNum >= 20))
		{
			acd = 1;
			break;
		}
		sendbuf[10+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)& 0xFF;	/*information object low address */
		sendbuf[11+nInfoNum*11] = (soebuf.soerec[soepnt].infoaddr)>>8;		/*information object high address */
		sendbuf[12+nInfoNum*11] = soebuf.soerec[soepnt].vti;			/*VTI */			
		sendbuf[13+nInfoNum*11] = soebuf.soerec[soepnt].stat;			/*QDS IV NT SB BL RES3 OV */					

		millisec = soebuf.soerec[soepnt].time.second * 1000 + soebuf.soerec[soepnt].time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[14+nInfoNum*11],&millisec,2);		/*Milliseconds */
#else
		memcpy(&sendbuf[14+nInfoNum*11],&millisec,2);			/*Milliseconds */
#endif
		sendbuf[16+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.minute;	/*Minutes */
		sendbuf[17+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.hour;	/*Hours(0-23) */
		sendbuf[18+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.day;
												/*Days of week(1-7) Days(1-31) + (unsigned char)(m_sEvent.nDayOfWeek << 5); */
		sendbuf[19+nInfoNum*11] = (unsigned char)soebuf.soerec[soepnt].time.month;	/*Months(1-12) */
		sendbuf[20+nInfoNum*11] = (unsigned char)(soebuf.soerec[soepnt].time.year % 100);	/*Years(0-99) */

		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		nInfoNum ++;
	}
	soebuf.sendpnt = soepnt;

	if(nInfoNum == 0)
	{
		OutputDspInfo(0, "StepPosition information M_ST_TB: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	if(acd==0)
		CheckACD();
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}
	sendbuf[7] = nInfoNum;				/*variable structure specify (nNumber of i-object) */
	nLength = sendbuf[7] * 11 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */
	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nStepPosition information TB");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueNA(unsigned char gi_cause)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_NA;		/*9 type normalized without time tag */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{

			OutputDspInfo(0, "ME %d: real %5.3f old_value %5.3f stat %u old_stat %u\n",pntnum,
					value,organafbuf[pntnum], stat, organastbuf[pntnum]);
			
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum],medef.scale[pntnum]);
			sendbuf[10+inf_n*5] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*5] = (unsigned char)((me_base_addr + pntnum)>>8);		/*i-addr high address */
			sendbuf[12+inf_n*5] = (unsigned char)(anaorgcode & 0xFF);		/*N Value */						
			sendbuf[13+inf_n*5] = (unsigned char)(anaorgcode >> 8);			/*N Value */
			sendbuf[14+inf_n*5] = organastbuf[pntnum] = stat;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 24)
			{
				break;
			}
		}
	}
	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNA: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 5 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_NA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CheckMeasureValueNA(void)
{
	unsigned char	stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
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
			return(0);
		}
	}
	NoChangeData = 0;
	return(-1);
}

int	MeasureValueNACyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_NA;			/*9 type normalized without time tag */

	sendbuf[8] = gi_cause;			/*cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(medef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + me_base_addr;	/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */						
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			

			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = organastbuf[pntnum] = 0;	/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum],medef.scale[pntnum]);
		sendbuf[12+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */						
		sendbuf[13+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		sendbuf[14+inf_n*3] = organastbuf[pntnum] = stat;		/*QDS IV NT SB BL RES3 OV */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNACyc: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;		/*SQ=1 */

	nLength = inf_n * 3 + 8;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */

	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nCyc Measure Value M_ME_NA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CyclicSendNA(unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	char	name_buf[NAME_SIZE];
	short	anaorgcode;
	float	value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_NA;			/*9 type normalized without time tag */

	sendbuf[8] = PER_CYC;			/*cause of transmit:PER_CYC:1 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(cycdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + cyc_base_addr;		/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(cycdef.number < endnum)
		endnum = cycdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(cycdef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */						
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,cycdef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ME point in the database file!\n",name_buf);
			

			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = organastbuf[pntnum] = 0;	/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		anaorgcode = GetNormalizedValue(value,cycdef.lorange[pntnum],cycdef.hirange[pntnum],cycdef.scale[pntnum]);

		sendbuf[12+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */						
		sendbuf[13+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		sendbuf[14+inf_n*3] = stat;					/*QDS IV NT SB BL RES3 OV */
		inf_n++;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "CyclicSendNA: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;			/*SQ=1 */

	nLength = inf_n * 3 + 8;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nCyclic send M_ME_NA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	ReadMeasureValueNA(unsigned short infoaddr)
{
	unsigned char	chksum,n,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 11;		/*L	byte_to_send = L + 6; */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_NA;		/*9 type normalized without time tag */

	sendbuf[7] = 1;			/*SQ=0 */

	sendbuf[8] = REQ;		/*cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	pntnum = infoaddr - me_base_addr;
	sendbuf[10] = (unsigned char)(infoaddr & 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(infoaddr>>8);	/*i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	retn = GetMEValueByName(name_buf,&stat,&value);
	if(retn==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			

		sendbuf[12] = (unsigned char)0;		/*N Value */
		sendbuf[13] = (unsigned char)0;		/*N Value */
		sendbuf[14] = 0;			/*QDS IV NT SB BL RES3 OV */
	}
	else
	{
		anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum],medef.scale[pntnum]);

		sendbuf[12] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */						
		sendbuf[13] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		sendbuf[14] = organastbuf[pntnum] = stat;		/*QDS IV NT SB BL RES3 OV */
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;				/*checksum char */

	sendbuf[16] = 0x16;				/*end char */

	OutputDspInfo(0, "\nRead Measure Value M_ME_NA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueNB(unsigned char gi_cause)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_NB;		/*11 type scaled without time tag */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			
			continue;
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum])||((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			anaorgcode=(short)(value * medef.scale[pntnum]);

			OutputDspInfo(0, "Scale ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			
			sendbuf[10+inf_n*5] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*5] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
			sendbuf[12+inf_n*5] = (unsigned char)(anaorgcode & 0xFF);		/*N Value */
			sendbuf[13+inf_n*5] = (unsigned char)(anaorgcode >> 8);			/*N Value */
			sendbuf[14+inf_n*5] = organastbuf[pntnum] = stat;				/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 24)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNB: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 5 + 6;
	sendbuf[1] = sendbuf[2] = nLength;	/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;		/*checksum char */

	sendbuf[nLength+1] = 0x16;		/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_NB");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CheckMeasureValueNB(void)
{
	unsigned char	stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			
			continue;
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			NoChangeData = 1;
			return(0);
		}
	}
	NoChangeData = 0;
	return(-1);
}


int	MeasureValueNBCyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_NB;			/*11 type scaled without time tag */

	sendbuf[8] = gi_cause;			/*cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(medef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + me_base_addr;	/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			

			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}

		anaorgcode=(short)(value * medef.scale[pntnum]);
		sendbuf[12+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */
		sendbuf[13+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		sendbuf[14+inf_n*3] = organastbuf[pntnum] = stat;		/*QDS IV NT SB BL RES3 OV */
		inf_n++;
		organafbuf[pntnum] = value;
		organabuf[pntnum] = anaorgcode;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNBCyc: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;			/*SQ=1 */

	nLength = inf_n * 3 + 8;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nCyc Measure Value M_ME_NB");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CyclicSendNB(unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_NB;			/*11 type scaled without time tag */

	sendbuf[8] = PER_CYC;			/*cause of transmit:PER_CYC:1 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(cycdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + cyc_base_addr;	/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(cycdef.number < endnum)
		endnum = cycdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(cycdef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,cycdef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			

			sendbuf[12+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*3] = (unsigned char)0;		/*N Value */
			sendbuf[14+inf_n*3] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		anaorgcode=(short)(value * cycdef.scale[pntnum]);
		sendbuf[12+inf_n*3] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */
		sendbuf[13+inf_n*3] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		sendbuf[14+inf_n*3] = stat;					/*QDS IV NT SB BL RES3 OV */
		inf_n++;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "CyclicSendNB: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;			/*SQ=1 */

	nLength = inf_n * 3 + 8;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nCyclic send M_ME_NB");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	ReadMeasureValueNB(unsigned short infoaddr)
{
	unsigned char	chksum,n,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short			anaorgcode;
	float			value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 11;		/*L byte_to_send = L + 6; */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_NB;		/*11 type scaled without time tag */

	sendbuf[7] = 1;			/*SQ=0 */

	sendbuf[8] = REQ;		/*cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	pntnum = infoaddr - me_base_addr;
	sendbuf[10] = (unsigned char)(infoaddr & 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(infoaddr>>8);	/*i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	retn = GetMEValueByName(name_buf,&stat,&value);
	if(retn==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			

		sendbuf[12] = (unsigned char)0;		/*N Value */
		sendbuf[13] = (unsigned char)0;		/*N Value */
		sendbuf[14] = 0;			/*QDS IV NT SB BL RES3 OV */
	}
	else
	{

		anaorgcode=(unsigned short)(value*medef.scale[pntnum]);

		sendbuf[12] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */
		sendbuf[13] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		sendbuf[14] = organastbuf[pntnum] = stat;		/*QDS IV NT SB BL RES3 OV */
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	byte_to_send = 17;

	chksum = 0;
	for(n=4;n<15;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[15] = chksum;			/*checksum char */

	sendbuf[16] = 0x16;			/*end char */

	OutputDspInfo(0, "\nRead Measure Value M_ME_NB");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueNC(unsigned char gi_cause)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_NC;		/*13 type float without time tag */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			
			continue;
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			
			sendbuf[10+inf_n*7] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*7] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
#ifdef SUN_OS
			CrossCopy(&sendbuf[12+inf_n*7],&value,4);			/*N Value */
#else
			memcpy(&sendbuf[12+inf_n*7],&value,4);				/*N Value */
#endif
			sendbuf[16+inf_n*7] = organastbuf[pntnum] = stat;		/*QDS IV NT SB BL RES3 OV */

			inf_n++;
			organafbuf[pntnum] = value;
			if(inf_n >= 16)
			{
				sendbuf[4] = sendbuf[4] | 0x20;
				break;
			}
		}
	}
	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNC: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 7 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_NC");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CheckMeasureValueNC(void)
{
	unsigned char	stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
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
			return(0);
		}
	}
	NoChangeData = 0;
	return(-1);
}

int	MeasureValueNCCyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	float			value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_NC;			/*13 type float without time tag */

	sendbuf[8] = gi_cause;			/*cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(medef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + me_base_addr;	/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			value = 0;
#ifdef SUN_OS
			CrossCopy(&sendbuf[12+inf_n*5],&value,4);	/*N Value */
#else
			memcpy(&sendbuf[12+inf_n*5],&value,4);		/*N Value */
#endif
			sendbuf[16+inf_n*5] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			

			value = 0;
#ifdef SUN_OS
			CrossCopy(&sendbuf[12+inf_n*5],&value,4);	/*N Value */
#else
			memcpy(&sendbuf[12+inf_n*5],&value,4);		/*N Value */
#endif
			sendbuf[16+inf_n*5] = 0;			/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}

#ifdef SUN_OS
		CrossCopy(&sendbuf[12+inf_n*5],&value,4);	/*N Value */
#else
		memcpy(&sendbuf[12+inf_n*5],&value,4);		/*N Value */
#endif
		sendbuf[16+inf_n*5] = organastbuf[pntnum] = stat;	/*QDS IV NT SB BL RES3 OV */
		inf_n++;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNCCyc: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;	/*SQ=1 */

	nLength = inf_n * 5 + 8;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nCyc Measure Value M_ME_NC");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CyclicSendNC(unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_NC;			/*13 type float without time tag */

	sendbuf[8] = PER_CYC;			/*cause of transmit:PER_CYC:1 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(cycdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + cyc_base_addr;	/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(cycdef.number < endnum)
		endnum = cycdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(cycdef.name[pntnum],"NULL")==0)
		{
			value = 0;
			memcpy(&sendbuf[12+inf_n*5],&value,4);	/*N Value */
			sendbuf[16+inf_n*5] = 0;		/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}
		strcpy(name_buf,cycdef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			

			memcpy(&sendbuf[12+inf_n*5],&value,4);	/*N Value */
			sendbuf[16+inf_n*5] = 0;		/*QDS IV NT SB BL RES3 OV */
			inf_n++;
			continue;
		}

#ifdef SUN_OS
		CrossCopy(&sendbuf[12+inf_n*5],&value,4);	/*N Value */
#else
		memcpy(&sendbuf[12+inf_n*5],&value,4);		/*N Value */
#endif
		sendbuf[16+inf_n*5] = stat;			/*QDS IV NT SB BL RES3 OV */
		inf_n++;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "CyclicSendNC: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;	/*SQ=1 */

	nLength = inf_n * 5 + 8;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nCyclic send M_ME_NC");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	ReadMeasureValueNC(unsigned short infoaddr)
{
	unsigned char	chksum,n,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float	value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 13;		/*L byte_to_send = L + 6; */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_NC;		/*13 type float without time tag */

	sendbuf[7] = 1;			/*SQ=0 */

	sendbuf[8] = REQ;		/*cause of transmit:INTROGEN:20 INTRO9:29 INTRO10:30 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	pntnum = infoaddr - me_base_addr;
	sendbuf[10] = (unsigned char)(infoaddr & 0xFF);		/*i-addr low address */
	sendbuf[11] = (unsigned char)(infoaddr>>8);		/*i-addr high address */
	strcpy(name_buf,medef.name[pntnum]);
	retn = GetMEValueByName(name_buf,&stat,&value);
	if(retn==-1)
	{
		OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
		sendbuf[12] = (unsigned char)0;		/*N Value */
		sendbuf[13] = (unsigned char)0;		/*N Value */
		sendbuf[14] = (unsigned char)0;		/*N Value */
		sendbuf[15] = (unsigned char)0;		/*N Value */
		sendbuf[16] = 0;			/*QDS IV NT SB BL RES3 OV */
	}
	else
	{
#ifdef SUN_OS
		CrossCopy(&sendbuf[12],&value,4);	/*N Value */
#else
		memcpy(&sendbuf[12],&value,4);		/*N Value */
#endif
		sendbuf[16] = organastbuf[pntnum] = stat;	/*QDS IV NT SB BL RES3 OV */
		organafbuf[pntnum] = value;
	}

	byte_to_send = 19;

	chksum = 0;
	for(n=4;n<17;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[17] = chksum;			/*checksum char */

	sendbuf[18] = 0x16;			/*end char */

	OutputDspInfo(0, "\nRead Measure Value M_ME_NC");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueND(unsigned char gi_cause)
{
	unsigned char	chksum,n,inf_n,stat;
	unsigned char	nLength;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short	anaorgcode;
	float	value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_ND;		/*21 type normalized without quality descriptor */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum]))
		{
			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum],medef.scale[pntnum]);
			sendbuf[10+inf_n*4] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*4] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
			sendbuf[12+inf_n*4] = (unsigned char)(anaorgcode & 0xFF);		/*N Value */
			sendbuf[13+inf_n*4] = (unsigned char)(anaorgcode >> 8);			/*N Value */
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 24)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueND: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 4 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_ND");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueNDCyc(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n,stat;
	unsigned char	nLength;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short	anaorgcode;
	float	value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_ND;			/*21 type normalized without quality descriptor */

	sendbuf[8] = gi_cause;			/* INTRO9:29 INTRO10:30 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(medef.number < startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + me_base_addr;	/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(medef.number < endnum)
		endnum = medef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(medef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n*2] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*2] = (unsigned char)0;		/*N Value */
			inf_n++;
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			sendbuf[12+inf_n*2] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*2] = (unsigned char)0;		/*N Value */
			inf_n++;
			continue;
		}
		anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum],medef.scale[pntnum]);

		sendbuf[12+inf_n*2] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */
		sendbuf[13+inf_n*2] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		inf_n++;
		organabuf[pntnum] = anaorgcode;
		organafbuf[pntnum] = value;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueNDCyc: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;	/*SQ=1 */

	nLength = inf_n * 2 + 8;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nCyc Measure Value M_ME_ND");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	CyclicSendND(unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n,stat;
	unsigned char	nLength;

	char		name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_ND;		/*21 type normalized without quality descriptor */

	sendbuf[8] = PER_CYC;		/*cause of transmit:PER_CYC:1 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	startnum = ME_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(cycdef.number <= startnum)
	{
/*		NoRequestData(); */
		NoChangeData = 1;
		return(0);
	}
	firstinfoaddr = startnum + cyc_base_addr;	/*infogroup: 0,1,2,3 */
	endnum = startnum + ME_GROUP_NUM;
	if(cycdef.number < endnum)
		endnum = cycdef.number;

	sendbuf[10] = (unsigned char)(firstinfoaddr& 0xFF);	/*i-addr low address */
	sendbuf[11] = (unsigned char)(firstinfoaddr>>8);	/*i-addr high address */
	inf_n = 0;
	for(pntnum=startnum;pntnum<endnum;pntnum++)
	{
		if(strcmp(cycdef.name[pntnum],"NULL")==0)
		{
			sendbuf[12+inf_n*2] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*2] = (unsigned char)0;		/*N Value */
			inf_n++;
			continue;
		}
		strcpy(name_buf,cycdef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			sendbuf[12+inf_n*2] = (unsigned char)0;		/*N Value */
			sendbuf[13+inf_n*2] = (unsigned char)0;		/*N Value */
			inf_n++;
			continue;
		}
		anaorgcode = GetNormalizedValue(value,cycdef.lorange[pntnum],cycdef.hirange[pntnum],cycdef.scale[pntnum]);

		sendbuf[12+inf_n*2] = (unsigned char)(anaorgcode & 0xFF);	/*N Value */
		sendbuf[13+inf_n*2] = (unsigned char)(anaorgcode >> 8);		/*N Value */
		inf_n++;
	}

	if(inf_n == 0)
	{
		OutputDspInfo(0, "CyclicSendND: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	sendbuf[7] = inf_n | 0x80;	/*SQ=1 */

	nLength = inf_n * 2 + 8;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nCyclicSendND M_ME_ND");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueTA(unsigned char gi_cause)
{
	SYSTEMTIME	sys_tm;
	USHORT		millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_TA;		/*10 type normalized with time tag */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	nLength = sendbuf[7];

	GetLocalTime(&sys_tm);
	millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=1))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum],medef.scale[pntnum]);

			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			
			sendbuf[10+inf_n*8] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*8] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
			sendbuf[12+inf_n*8] = (unsigned char)(anaorgcode & 0xFF);		/*N Value */
			sendbuf[13+inf_n*8] = (unsigned char)(anaorgcode >> 8);			/*N Value */
			sendbuf[14+inf_n*8] = organastbuf[pntnum] = stat;			/*QDS IV NT SB BL RES3 OV */

			memcpy(&sendbuf[15+inf_n*8],&millisec,2);	//Milliseconds
			sendbuf[17+inf_n*8] = (unsigned char)sys_tm.wMinute;	//Minutes
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 16)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTA: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 8 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_TA");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueTD(unsigned char gi_cause)
{
	SYSTEMTIME	sys_tm;
	USHORT		millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short		anaorgcode;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_TD;		/*34 type normalized with time tag CP56Time2a */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	nLength = sendbuf[7];
	GetLocalTime(&sys_tm);
	millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=2))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			continue;
		}
		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			anaorgcode = GetNormalizedValue(value,medef.lorange[pntnum],medef.hirange[pntnum],medef.scale[pntnum]);

			OutputDspInfo(0, "ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			
			sendbuf[10+inf_n*12] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*12] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
			sendbuf[12+inf_n*12] = (unsigned char)(anaorgcode & 0xFF);		/*N Value */
			sendbuf[13+inf_n*12] = (unsigned char)(anaorgcode >> 8);			/*N Value */
			sendbuf[14+inf_n*12] = organastbuf[pntnum] = stat;			/*QDS IV NT SB BL RES3 OV */

			memcpy(&sendbuf[15+inf_n*12],&millisec,2);					//Milliseconds
			sendbuf[17+inf_n*12] = (unsigned char)sys_tm.wMinute;		//Minutes
			sendbuf[18+inf_n*12] = (unsigned char)sys_tm.wHour;	//Hour
			n = sys_tm.wDayOfWeek << 5;
			n = n & 0xE0;
//			sendbuf[19+inf_n*12] = (unsigned char)sys_tm.wDay | n;		//DayofWeek[6..8] day[1..5]
			sendbuf[19+inf_n*12] = (unsigned char)sys_tm.wDay;		//DayofWeek[6..8] day[1..5]
			sendbuf[20+inf_n*12] = (unsigned char)sys_tm.wMonth;		//Month
			sendbuf[21+inf_n*12] = (unsigned char)(sys_tm.wYear%100);	//Year

			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 12)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTD: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 12 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_TD");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueTB(unsigned char gi_cause)
{
	SYSTEMTIME	sys_tm;
	WORD		millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short			anaorgcode;
	float			value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_TB;		/*12 type scaled with time tag */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	GetLocalTime(&sys_tm);
	millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=1))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			continue;
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum])||((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			anaorgcode=(short)(value * medef.scale[pntnum]);

			OutputDspInfo(0, "Scale ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			
			sendbuf[10+inf_n*8] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*8] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
			sendbuf[12+inf_n*8] = (unsigned char)(anaorgcode & 0xFF);		/*N Value */
			sendbuf[13+inf_n*8] = (unsigned char)(anaorgcode >> 8);			/*N Value */
			sendbuf[14+inf_n*8] = organastbuf[pntnum] = stat;				/*QDS IV NT SB BL RES3 OV */

			memcpy(&sendbuf[15+inf_n*8],&millisec,2);	//Milliseconds
			sendbuf[17+inf_n*8] = (unsigned char)sys_tm.wMinute;	//Minutes
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 16)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTB: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 8 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_TB");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueTE(unsigned char gi_cause)
{
	SYSTEMTIME	sys_tm;
	WORD		millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	short			anaorgcode;
	float			value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_ME_TE;			/*35 type scaled with time tag */
	sendbuf[8] = gi_cause;			/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	inf_n = 0;
	GetLocalTime(&sys_tm);
	millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=2))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			continue;
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum])||((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			anaorgcode=(short)(value * medef.scale[pntnum]);

			OutputDspInfo(0, "Scale ME %d: real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);
			
			sendbuf[10+inf_n*12] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*12] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
			sendbuf[12+inf_n*12] = (unsigned char)(anaorgcode & 0xFF);		/*N Value */
			sendbuf[13+inf_n*12] = (unsigned char)(anaorgcode >> 8);			/*N Value */
			sendbuf[14+inf_n*12] = organastbuf[pntnum] = stat;				/*QDS IV NT SB BL RES3 OV */

			memcpy(&sendbuf[15+inf_n*12],&millisec,2);	//Milliseconds
			sendbuf[17+inf_n*12] = (unsigned char)sys_tm.wMinute;	//Minutes
			sendbuf[18+inf_n*12] = (unsigned char)sys_tm.wHour;	//Hour
			n = sys_tm.wDayOfWeek << 5;
			n = n & 0xE0;
//			sendbuf[19+inf_n*12] = (unsigned char)sys_tm.wDay | n;		//DayofWeek[6..8] day[1..5]
			sendbuf[19+inf_n*12] = (unsigned char)sys_tm.wDay;		//DayofWeek[6..8] day[1..5]
			sendbuf[20+inf_n*12] = (unsigned char)sys_tm.wMonth;		//Month
			sendbuf[21+inf_n*12] = (unsigned char)(sys_tm.wYear%100);	//Year
			inf_n++;
			organabuf[pntnum] = anaorgcode;
			organafbuf[pntnum] = value;
			if(inf_n >= 12)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTE: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 12 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_TE");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueTC(unsigned char gi_cause)
{
	SYSTEMTIME	sys_tm;
	WORD		millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char		name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float		value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
/*
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;
*/
	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_TC;		/*14 type float with time tag */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	GetLocalTime(&sys_tm);
	millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=1))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			continue;
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			OutputDspInfo(0, "ANA %d real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);

			sendbuf[10+inf_n*10] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*10] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
#ifdef SUN_OS
			CrossCopy(&sendbuf[12+inf_n*10],&value,4);		/*N Value */
#else
			memcpy(&sendbuf[12+inf_n*10],&value,4);			/*N Value */
#endif
			sendbuf[16+inf_n*10] = organastbuf[pntnum] = stat;	/*QDS IV NT SB BL RES3 OV */

			memcpy(&sendbuf[17+inf_n*10],&millisec,2);	//Milliseconds
			sendbuf[19+inf_n*10] = (unsigned char)sys_tm.wMinute;	//Minutes
			inf_n++;
			organafbuf[pntnum] = value;
			if(inf_n >= 16)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTC: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 10 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_TC");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	MeasureValueTF(unsigned char gi_cause)
{
	SYSTEMTIME	sys_tm;
	WORD		millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength,stat;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum;
	float			value;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
/*
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;
*/
	sendbuf[5] = (UCHAR)nStnId;		/*link address field */
	sendbuf[6] = M_ME_TF;		/*36 type float with time tag */
	sendbuf[8] = gi_cause;		/*cause of transmit:SPONT:3 INTROGEN:20 */
	sendbuf[9] = (UCHAR)nASDUId;		/*ASDU common address */

	inf_n = 0;
	GetLocalTime(&sys_tm);
	millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;

	for(pntnum=0;pntnum<medef.number;pntnum++)
	{
		if((strcmp(medef.name[pntnum],"NULL")==0)||(medef.TimeTag[pntnum] !=2))
		{
			continue;
		}
		strcpy(name_buf,medef.name[pntnum]);
		retn = GetMEValueByName(name_buf,&stat,&value);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this ANA point in the database file!\n",name_buf);
			continue;
		}

		if(((value - organafbuf[pntnum]) > medef.databand[pntnum]) || ((organafbuf[pntnum] - value) > medef.databand[pntnum])
			||(organastbuf[pntnum] != stat))
		{
			OutputDspInfo(0, "ANA %d real %5.3f save %5.3f databand %5.3f\n",pntnum,
					value,organafbuf[pntnum],medef.databand[pntnum]);

			sendbuf[10+inf_n*14] = (unsigned char)((me_base_addr + pntnum)& 0xFF);	/*i-addr low address */
			sendbuf[11+inf_n*14] = (unsigned char)((me_base_addr + pntnum)>>8);	/*i-addr high address */
#ifdef SUN_OS
			CrossCopy(&sendbuf[12+inf_n*14],&value,4);		/*N Value */
#else
			memcpy(&sendbuf[12+inf_n*14],&value,4);			/*N Value */
#endif
			sendbuf[16+inf_n*14] = organastbuf[pntnum] = stat;	/*QDS IV NT SB BL RES3 OV */

			millisec = sys_tm.wSecond * 1000 + sys_tm.wMilliseconds;
			memcpy(&sendbuf[17+inf_n*14],&millisec,2);	//Milliseconds
			sendbuf[19+inf_n*14] = (unsigned char)sys_tm.wMinute;	//Minutes
			sendbuf[20+inf_n*14] = (unsigned char)sys_tm.wHour;	//Hour
			n = sys_tm.wDayOfWeek << 5;
			n = n & 0xE0;
//			sendbuf[21+inf_n*14] = (unsigned char)sys_tm.wDay | n;		//DayofWeek[6..8] day[1..5]
			sendbuf[21+inf_n*14] = (unsigned char)sys_tm.wDay;		//DayofWeek[6..8] day[1..5]
			sendbuf[22+inf_n*14] = (unsigned char)sys_tm.wMonth;		//Month
			sendbuf[23+inf_n*14] = (unsigned char)(sys_tm.wYear%100);	//Year
			inf_n++;
			organafbuf[pntnum] = value;
			if(inf_n >= 12)
			{
				break;
			}
		}
	}

	sendbuf[7] = inf_n;	/*SQ=0 */

	if(inf_n == 0)
	{
		OutputDspInfo(0, "MeasureValueTF: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}
	
	if(acd)
	{
		sendbuf[4] = sendbuf[4] | 0x20;	/*control field */
	}

	nLength = inf_n * 14 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */

	sendbuf[nLength+1] = 0x16;			/*end char */

	OutputDspInfo(0, "\nMeasure Value M_ME_TF");
	
	retn = SendVarFrame();
	NoChangeData = 0;
	return(retn);
}

int	IntegratedTotals(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned char	chksum,n,inf_n,stat;
	unsigned char	nLength;
	unsigned int	count;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_IT_NA;			/*15 type Integrated Totals information */
/*	sendbuf[7] = 2;				/*variable structure specify (number of i-object) */
	sendbuf[8] = gi_cause;			/*37 cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = IT_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(itdef.number < startnum)
		{
		NoRequestData();
		return(0);
		}
	firstinfoaddr = startnum + it_base_addr;	/*infogroup: 0,1,2,3 */
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
		retn = GetIMPValueByName(name_buf,&stat,&count);
		if(retn==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this IMP point in the database file!\n",name_buf);
			continue;
		}
		sendbuf[10+inf_n*7] = (it_base_addr + pntnum)& 0xFF;	/*information object low address */
		sendbuf[11+inf_n*7] = (it_base_addr + pntnum)>>8;	/*information object high address */

#ifdef SUN_OS
		CrossCopy(&sendbuf[12+inf_n*7],&count,4);
#else
		memcpy(&sendbuf[12+inf_n*7],&count,4);
#endif
		sendbuf[16+inf_n*7] = count_sq;								/*QDS IV CA CY SQ */
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		inf_n++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "IntegratedTotals: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}

	sendbuf[7] = inf_n;
	nLength = inf_n * 7 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */
	sendbuf[nLength+1] = 0x16;			/*end char */

	NoChangeData = 0;

	OutputDspInfo(0, "\nIntegrated Totals");
	
	retn = SendVarFrame();
	return(retn);
}

int	IntegratedTotalsTA(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned short	millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength;
	unsigned int	count;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
/*	sendbuf[1] = sendbuf[2] = 9; */		/*L */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_IT_TA;			/*16 type Integrated Totals information */
/*	sendbuf[7] = 2;	*/			/*variable structure specify (number of i-object) */
	sendbuf[8] = gi_cause;			/*37 cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = IT_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(itdef.number < startnum)
		{
		NoRequestData();
		return(0);
		}
	firstinfoaddr = startnum + it_base_addr;	/*infogroup: 0,1,2,3 */
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
		sendbuf[10+inf_n*10] = (it_base_addr + pntnum)& 0xFF;	/*information object low address */
		sendbuf[11+inf_n*10] = (it_base_addr + pntnum)>>8;	/*information object high address */

		count = dbentry.var.imp.entry.rt_value;		/* count value */

#ifdef SUN_OS
		CrossCopy(&sendbuf[12+inf_n*10],&count,4);
#else
		memcpy(&sendbuf[12+inf_n*10],&count,4);
#endif
		sendbuf[16+inf_n*10] = count_sq;								/*QDS IV CA CY SQ */
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		millisec = dbentry.var.imp.entry.update_time.second * 1000 + dbentry.var.imp.entry.update_time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[17+inf_n*10],&millisec,2);	/*Milliseconds */
#else
		memcpy(&sendbuf[17+inf_n*10],&millisec,2);	/*Milliseconds */
#endif
		sendbuf[19+inf_n*10] = dbentry.var.imp.entry.update_time.minute;	/*Minutes */
		inf_n ++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "IntegratedTotalsTA: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}

	sendbuf[7] = inf_n;
	nLength = sendbuf[7] * 10 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */
	sendbuf[nLength+1] = 0x16;			/*end char */

	NoChangeData = 0;

	OutputDspInfo(0, "\nIntegrated Totals TA");
	
	retn = SendVarFrame();
	return(retn);
}

int	IntegratedTotalsTB(unsigned char gi_cause,unsigned short infogroup)
{
	unsigned short	millisec;
	unsigned char	chksum,n,inf_n;
	unsigned char	nLength;
	unsigned int	count;
	DMS_COMMON	dbentry;
	char	name_buf[NAME_SIZE];
	unsigned short	pntnum,firstinfoaddr,startnum,endnum;

	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
/*	sendbuf[1] = sendbuf[2] = 9; */		/*L */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_IT_TB;			/*37 type Integrated Totals information */
/*	sendbuf[7] = 2;	 */			/*variable structure specify (number of i-object) */
	sendbuf[8] = gi_cause;			/*37 cause of transmit:requested */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */

	startnum = IT_GROUP_NUM*infogroup;	/*infogroup: 0,1,2,3 */
	if(itdef.number < startnum)
		{
		NoRequestData();
		return(0);
		}
	firstinfoaddr = startnum + it_base_addr;	/*infogroup: 0,1,2,3 */
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
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			OutputDspInfo(OUTPUT_WARN, "%s No this IMP point in the database file!\n",name_buf);
			continue;
		}
		sendbuf[10+inf_n*14] = (it_base_addr + pntnum)& 0xFF;	/*information object low address */
		sendbuf[11+inf_n*14] = (it_base_addr + pntnum)>>8;	/*information object high address */

#ifdef SUN_OS
		CrossCopy(&sendbuf[12+inf_n*14],&count,4);
#else
		memcpy(&sendbuf[12+inf_n*14],&count,4);
#endif
		sendbuf[16+inf_n*14] = count_sq;			/*QDS IV CA CY SQ */
		count_sq ++;
		if(count_sq>=32)
			count_sq = 0;
		millisec = dbentry.var.imp.entry.update_time.second * 1000 + dbentry.var.imp.entry.update_time.ms;
#ifdef SUN_OS
		CrossCopy(&sendbuf[17+inf_n*14],&millisec,2);		/*Milliseconds */
#else
		memcpy(&sendbuf[17+inf_n*14],&millisec,2);		/*Milliseconds */
#endif
		sendbuf[19+inf_n*14] = dbentry.var.imp.entry.update_time.minute;	/*Minutes */
		sendbuf[20+inf_n*14] = dbentry.var.imp.entry.update_time.hour;		/*Hour */
		sendbuf[21+inf_n*14] = dbentry.var.imp.entry.update_time.day;		/*DayofWeek[6..8] day[1..5] */
		sendbuf[22+inf_n*14] = dbentry.var.imp.entry.update_time.month;		/*Month */
		sendbuf[23+inf_n*14] = (unsigned char)(dbentry.var.imp.entry.update_time.year%100);	/*Year */
		inf_n ++;
	}
	if(inf_n == 0)
	{
		OutputDspInfo(0, "IntegratedTotalsTA: NoRequestData\n");
		NoChangeData = 1;
		return(0);
	}

	sendbuf[7] = inf_n;
	nLength = sendbuf[7] * 14 + 6;
	sendbuf[1] = sendbuf[2] = nLength;		/*L	byte_to_send = L + 6; */
	byte_to_send = nLength + 6;
	nLength = nLength + 4;

	chksum = 0;
	for(n=4;n<nLength;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[nLength] = chksum;			/*checksum char */
	sendbuf[nLength+1] = 0x16;			/*end char */

	NoChangeData = 0;

	OutputDspInfo(0, "\nIntegrated Totals TB");
	
	retn = SendVarFrame();
	return(retn);
}

int	EndofInitialization(void)
{
	unsigned char	chksum,n;
	acd=0;
	sendbuf[0] = sendbuf[3] = 0x68;		/*start flag */
	sendbuf[1] = sendbuf[2] = 9;		/*L */
	sendbuf[4] = 0x08;			/*control field */
	if(acd)
		sendbuf[4] = sendbuf[4] | 0x20;

	sendbuf[5] = (UCHAR)nStnId;			/*link address field */
	sendbuf[6] = M_EI_NA;			/*70 type identifier field */
	sendbuf[7] = 1;				/*variable structure specify */
	sendbuf[8] = 4;				/*4 Actterm cause of transmit:end */
	sendbuf[9] = (UCHAR)nASDUId;			/*ASDU common address */
	sendbuf[10] = 0;			/*information object low address */
	sendbuf[11] = 0;			/*information object high address */
	sendbuf[12] = coi;			/*COI 0 local power on */

	byte_to_send = 15;

	chksum = 0;
	for(n=4;n<13;n++)
		chksum = chksum + sendbuf[n];

	sendbuf[13] = chksum;			/*checksum char */
	sendbuf[14] = 0x16;			/*end char */

	OutputDspInfo(0, "\nEnd of Initialization");
	retn = SendVarFrame();
	return(retn);
}

int	RespondGroupInterrogation(unsigned char gi_qoi)
{
	switch	( gi_qoi ) 
	{
	case  21:

		if(GiType==1)
			SinglePointInformCyc(INTRO1,m_cRespICGrpNum);	/*21 respond to Group 0 (1) Interrogation */
		else
			SinglePointGroupCyc(INTRO1, m_cRespICGrpNum);	/*21 respond to Group 0 (1) Interrogation M_PS_NA */
			break;

	case  22:

		DoublePointInformCyc(INTRO2,m_cRespICGrpNum);	/*22 respond to Group 1 (2) general Interrogation */
		break;

	case  23:

		if(GiType==1)
			SinglePointInformCyc(INTRO3,m_cRespICGrpNum);	/*23 respond to Group 2 (3) general Interrogation */
		else
			SinglePointGroupCyc(INTRO3, m_cRespICGrpNum);	/*23 respond to Group 2 (3) Interrogation M_PS_NA */
		break;

	case  24:
		DoublePointInformCyc(INTRO4,m_cRespICGrpNum);  /*24 respond to Group 3 (4) general Interrogation */
		break;

	case  29:

		if(MeASDUType==0)
			MeasureValueNACyc(INTRO9,m_cRespICGrpNum);	/*29 respond to Group 0 (9) general Interrogation M_ME_NA */
		else if(MeASDUType==1)
			MeasureValueNBCyc(INTRO9,m_cRespICGrpNum);	/*29 respond to Group 0 (9) general Interrogation M_ME_NA */
		else if(MeASDUType==2)
			MeasureValueNDCyc(INTRO9,m_cRespICGrpNum);	/*29 respond to Group 0 (9) general Interrogation M_ME_NA */
		else
			MeasureValueNCCyc(INTRO9,m_cRespICGrpNum);	/*29 respond to Group 0 (9) general Interrogation M_ME_NA */
		break;

	case  30:

		if(MeASDUType==0)
			MeasureValueNACyc(INTRO10,m_cRespICGrpNum);	/*30 respond to Group 1 (10) general Interrogation M_ME_NA */
		else if(MeASDUType==1)
			MeasureValueNBCyc(INTRO10,m_cRespICGrpNum);	/*30 respond to Group 1 (10) general Interrogation M_ME_NA */
		else if(MeASDUType==2)
			MeasureValueNDCyc(INTRO10,m_cRespICGrpNum);	/*30 respond to Group 1 (10) general Interrogation M_ME_NA */
		else
			MeasureValueNCCyc(INTRO10,m_cRespICGrpNum);	/*30 respond to Group 1 (10) general Interrogation M_ME_NA */
		break;

	case  31:

		if(MeASDUType==0)
			MeasureValueNACyc(INTRO11,m_cRespICGrpNum);	/*31 respond to Group 2 (11) general Interrogation M_ME_NA */
		else if(MeASDUType==1)
			MeasureValueNBCyc(INTRO11,m_cRespICGrpNum);	/*31 respond to Group 2 (11) general Interrogation M_ME_NA */
		else if(MeASDUType==2)
			MeasureValueNDCyc(INTRO11,m_cRespICGrpNum);	/*31 respond to Group 2 (11) general Interrogation M_ME_NA */
		else
			MeasureValueNCCyc(INTRO11,m_cRespICGrpNum);	/*31 respond to Group 2 (11) general Interrogation M_ME_NC */
		break;

	default :
		OutputDspInfo(OUTPUT_WARN, "No this group data qoi=%d\n",gi_qoi);
		
		NoChangeData = 1;
		break;			
	}/*switch(gi_cause) */
	if(NoChangeData == 1)
	{
		icgroupflag = 0;
		m_cRespICGrpNum = 0;
		CheckACD();
		GeneralInterrogationAck(ACTTERM,gi_qoi);	/*10 ActTrem to general Interrogation */
	}
	m_cRespICGrpNum++;
	return(0);
}

void RespondGeneralInterrogation(void)
{
	OutputDspInfo(0, "++++m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);

	NoChangeData = 0;
	if(m_cRespGenInterroGrpNum < 2000)
	{
		acd=1;
		if(GiType==1)
			SinglePointInformCyc(INTROGEN, m_cRespGenInterroGrpNum);	/*20 respond to general Interrogation M_SP_NA */
		else
			SinglePointGroupCyc(INTROGEN, m_cRespGenInterroGrpNum);		/*20 respond to general Interrogation M_PS_NA */
		if(NoChangeData == 1)
		{
			m_cRespGenInterroGrpNum = 2000;
		}
		else
		{
			m_cRespGenInterroGrpNum++;
			return;
		}
	}
	if(m_cRespGenInterroGrpNum < 4000)
	{
		acd=1;
		DoublePointInformCyc(INTROGEN, (unsigned short)(m_cRespGenInterroGrpNum-2000));	/*20 respond to general Interrogation DP */
		if(NoChangeData == 1)
		{
			m_cRespGenInterroGrpNum = 4000;
		}
		else
		{
			m_cRespGenInterroGrpNum++;
			return;
		}
	}
	if(m_cRespGenInterroGrpNum < 6000)
	{
		acd=1;
		StepPositionInformCyc(INTROGEN, (unsigned short)(m_cRespGenInterroGrpNum-4000));	/*20 respond to general Interrogation DP */
		if(NoChangeData == 1)
		{
			m_cRespGenInterroGrpNum = 6000;
		}
		else
		{
			m_cRespGenInterroGrpNum++;
			return;
		}
	}
	if(m_cRespGenInterroGrpNum < 8000)
	{
		acd=1;
		if(MeASDUType==0)
		{
			MeasureValueNACyc(INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000));	/*20 respond to general Interrogation M_ME_NA */
		}
		else if(MeASDUType==1)
		{
			MeasureValueNBCyc(INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000));	/*20 respond to general Interrogation M_ME_NA */
		}
		else if(MeASDUType==2)
		{
			MeasureValueNDCyc(INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000));	/*20 respond to general Interrogation M_ME_NA */
		}
		else
		{
			MeasureValueNCCyc(INTROGEN,(unsigned short)(m_cRespGenInterroGrpNum-6000));	/*20 respond to general Interrogation M_ME_NA */
		}
		if(NoChangeData != 1)
		{
			m_cRespGenInterroGrpNum++;
			return;
		}
	}

	m_cRespGenInterroGrpNum = 0;
	interrogationflag = 0;
	CheckACD();
	GeneralInterrogationAck(ACTTERM,INTROGEN);	/*10 ActTrem to general Interrogation */

	OutputDspInfo(0, "++++General Interrogation End! m_cRespGenInterroGrpNum =%d\n",m_cRespGenInterroGrpNum);
}

void ScanDataTransmit(void)
{
/*	short	grpno; */
	
	OutputDspInfo(0, "++++Scan m_cScanGrpNum =%d\n",m_cScanGrpNum);
	
	if(m_cScanGrpNum < 2000)
	{
		if(GiType==1)
			SinglePointInformCyc(BACK, m_cScanGrpNum);	/*1 respond to back scan M_SP_NA */
		else
			SinglePointGroupCyc(BACK, m_cScanGrpNum);	/*1 respond to back scan M_PS_NA */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 2000;
		}
		else
		{
			m_cScanGrpNum++;
			return;
		}
	}
	if(m_cScanGrpNum < 4000)
	{
		DoublePointInformCyc(BACK, (unsigned short)(m_cScanGrpNum-2000));	/*1 respond to back scan DP */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 4000;
		}
		else
		{
			m_cScanGrpNum++;
			return;
		}
	}
	if(m_cScanGrpNum < 6000)
	{
		StepPositionInformCyc(BACK, (unsigned short)(m_cScanGrpNum-4000));	/*1 respond to general Interrogation DP */
		if(NoChangeData == 1)
		{
			m_cScanGrpNum = 6000;
		}
		else
		{
			m_cScanGrpNum++;
			return;
		}
	}
	if(m_cScanGrpNum < 8000)
	{
		if(MeASDUType==0)
		{
			MeasureValueNACyc(BACK,(unsigned short)(m_cScanGrpNum-6000));	/*1 respond to back scan M_ME_NA */
		}
		else if(MeASDUType==1)
		{
			MeasureValueNBCyc(BACK,(unsigned short)(m_cScanGrpNum-6000));	/*1 respond to back scan M_ME_NA */
		}
		else if(MeASDUType==2)
		{
			MeasureValueNDCyc(BACK,(unsigned short)(m_cScanGrpNum-6000));	/*1 respond to back scan M_ME_NA */
		}
		else
		{
			MeasureValueNCCyc(BACK,(unsigned short)(m_cScanGrpNum-6000));	/*1 respond to back scan M_ME_NA */
		}
		if(NoChangeData != 1)
		{
			m_cScanGrpNum++;
			return;
		}
	}
	OutputDspInfo(0, "++++Scan End! m_cScanGrpNum =%d\n",m_cScanGrpNum);
	
	m_cScanGrpNum = 0;
	scanflag = 0;
/*	NoRequestData(); */
	SingleRespondByte();
}

int	RespondParamInterrogation(void)
{
	short	setvalue;
	float	fvalue;
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
			acd = 1;
			fvalue=medef.databand[m_cRespParamNum];

			setvalue = GetNormalizedValue(fvalue,medef.lorange[m_cRespParamNum],medef.hirange[m_cRespParamNum],medef.scale[m_cRespParamNum]);
			ParamValueNAAck(INTRO14,1,(unsigned short)(m_cRespParamNum+pm_base_addr),setvalue);
			m_cRespParamNum++;
			OutputDspInfo(0, "++++m_cRespParamNum =%d\n",m_cRespParamNum);
			
			return(0);
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
			acd = 1;
			setvalue=(short)(medef.databand[pntnum] * medef.scale[pntnum]);
			pntaddr = pntnum;
			ParamValueNBAck(INTRO14,1,(unsigned short)(pntaddr+pm_base_addr),setvalue);
			m_cRespParamNum++;

			OutputDspInfo(0, "++++m_cRespParamNum =%d pntnum=%d\n",m_cRespParamNum,pntnum);
			
			return(0);
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
			acd = 1;
			pntaddr = pntnum;
			ParamValueNCAck(INTRO14,1,(unsigned short)(pntaddr+pm_base_addr),medef.databand[m_cRespParamNum]);
			m_cRespParamNum++;

			OutputDspInfo(0, "++++m_cRespParamNum =%d pntnum=%d\n",m_cRespParamNum,pntnum);
			
			return(0);
		}
		Paramflag = 0;
		m_cRespParamNum = 0;
		CheckACD();
		GeneralInterrogationAck(ACTTERM,INTRO14);	/*10 ActTrem to general Interrogation */
	}
	return(0);
}

int	RespondCountGroupInterrogation(unsigned char gi_qcc)
{
	unsigned char		grp_num,end_num;

	end_num = (gi_qcc & 0x3F) * CI_GROUP_NUM;
	if(end_num > itdef.number)
		end_num = (unsigned char)itdef.number;

	grp_num = (unsigned char)end_num / IT_GROUP_NUM;
	if(end_num % IT_GROUP_NUM)
		grp_num++;

	while(m_cRespCIGrpNum < grp_num)
	{
		acd = 1;
		switch	(gi_qcc & 0x3F) 
		{
		case  1:
			if(ITGrpType==0)
			{
				ITGrpType = 1;
				IntegratedTotals(REQCO1,m_cRespCIGrpNum);	/*cause:38 respond to group 1 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				IntegratedTotalsTA(REQCO1,m_cRespCIGrpNum);	/*cause:38 respond to group 1 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			else
			{
				ITGrpType = 0;
				IntegratedTotalsTB(REQCO1,m_cRespCIGrpNum);	/*cause:38 respond to group 1 Counter Interrogation */
				m_cRespCIGrpNum++;
				if(NoChangeData==0)
					return(0);
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
				IntegratedTotals(REQCO2,m_cRespCIGrpNum);	/*cause:39 respond to group 2 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				IntegratedTotalsTA(REQCO2,m_cRespCIGrpNum);	/*cause:39 respond to group 2 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			else
			{
				ITGrpType = 0;
				IntegratedTotalsTB(REQCO2,m_cRespCIGrpNum);	/*cause:39 respond to group 2 Counter Interrogation */
				m_cRespCountGrpNum++;
				if(NoChangeData==0)
					return(0);
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
				IntegratedTotals(REQCO3,m_cRespCIGrpNum);	/*cause:40 respond to group 3 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				IntegratedTotalsTA(REQCO3,m_cRespCIGrpNum);	/*cause:40 respond to group 3 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			else
			{
				ITGrpType = 0;
				IntegratedTotalsTB(REQCO3,m_cRespCIGrpNum);	/*cause:40 respond to group 3 Counter Interrogation */
				m_cRespCIGrpNum++;
				if(NoChangeData==0)
					return(0);
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
				IntegratedTotals(REQCO4,m_cRespCIGrpNum);	/*cause:41 respond to group 4 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			if(ITGrpType==1)
			{
				ITGrpType = 2;
				IntegratedTotalsTA(REQCO4,m_cRespCIGrpNum);	/*cause:41 respond to group 4 Counter Interrogation */
				if(NoChangeData==0)
					return(0);
			}
			else
			{
				ITGrpType = 0;
				IntegratedTotalsTB(REQCO4,m_cRespCIGrpNum);	/*cause:41 respond to group 4 Counter Interrogation */
				m_cRespCIGrpNum++;
				if(NoChangeData==0)
					return(0);
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
	CounterInterrogationAck(ACTTERM,gi_qcc);	/*10 ActTrem to general Interrogation */
	return(0);
}
						  
void RespondCountInterrogation(void)
{
	short	grpno;

	grpno = itdef.number/IT_GROUP_NUM;
	if(itdef.number%IT_GROUP_NUM)
		grpno ++;
	while(m_cRespCountGrpNum < grpno)
	{
		if(ITGrpType==0)
		{
			ITGrpType = 1;
			IntegratedTotals(REQCOGCN,m_cRespCountGrpNum);	/*cause:37 respond to Counter Interrogation */
			if(NoChangeData==0)
				return;
		}
		if(ITGrpType==1)
		{
			ITGrpType = 2;
			IntegratedTotalsTA(REQCOGCN,m_cRespCountGrpNum);	/*cause:37 respond to Counter Interrogation */
			if(NoChangeData==0)
				return;
		}
		else
		{
			ITGrpType = 0;
			IntegratedTotalsTB(REQCOGCN,m_cRespCountGrpNum);	/*cause:37 respond to Counter Interrogation */
			m_cRespCountGrpNum++;
			if(NoChangeData==0)
				return;
			else
			{
				continue;
			}
		}
	}
	m_cRespCountGrpNum = 0;
	counterflag = 0;
	CheckACD();
	CounterInterrogationAck(ACTTERM,cibuf.qcc);		/*10 ActTrem to Counter Interrogation qcc=5 */
}
	  
void	RespondUserDataClass1(void)
{
	NoChangeData = 0;
	if(cmdbuf.ack_flag ==1) //SC DC
	{
		CmdRespSend();
		if(!NoChangeData)
			return;
	}

	if(sebuf.ack_flag ==1) //set point
	{
		SERespSend();
		if(!NoChangeData)
			return;
	}

	acd = 0;
	if(sp_cos == 1) 
	{
		SinglePointInform(SPONT);
		if(!NoChangeData)
			return;
		
		NoChangeData = 0;
		DoublePointInform(SPONT);
		if(!NoChangeData)
			return;
		
		NoChangeData = 0;
		StepPositionInform(SPONT);
		if(!NoChangeData)
			return;
		
	}
	
	if(csbuf.ack_flag==1) //clock sync
	{
		csbuf.ack_flag = 0;
		CheckACD();
		ClockSynchroniseAck();
		return;
	}

	if(cdbuf.ack_flag) //clock delay
	{
		cdbuf.ack_flag = 0;
		TimeDelayAck();
		return;
	}
	
	if(rpbuf.ack_flag) //reset process
	{
		rpbuf.ack_flag = 0;
		ResetProcessAck();
		return;
	}
	
			
	if(testbuf.ack_flag ==1) //test
	{
		testbuf.ack_flag = 0;
		CheckACD();
		RespondTestAck();
		return;
	}
	
	if(MirrorNAckflag)
	{
		MirrorNAckflag = 0;
		CheckACD();
		MirrorNAck(mirbuf.cot,mirbuf.pnt);	/*Mirror NACK */
		return;
	}

	if(icbuf.ack_flag)
	{
		icbuf.ack_flag = 0;
		acd=1;
		GeneralInterrogationAck(icbuf.cot,icbuf.qoi);
		return;
	}

	if(interrogationflag)
	{
		acd=1;
		RespondGeneralInterrogation();	/*general */
		return;
	}
	
	if(icgroupflag)
	{
		acd=1;
		RespondGroupInterrogation(icbuf.qoi);	/*group??? */
		return;
	}
	
	if(rdbuf.ack_flag ==1)
	{
		rdbuf.ack_flag = 0;
		CheckACD();
		if(rdbuf.cot == REQ)
		{
			ReadCmdResp(rdbuf.infoaddr);									
		}
		else
		{
			ReadCmdAck(rdbuf.cot,rdbuf.infoaddr);
		}
		return;
	}

	if(pmbuf.ack_flag ==1)
	{
		pmbuf.ack_flag = 0;
		CheckACD();
		PMRespSend();
		if(!NoChangeData)
			return;
	}

	if(cibuf.ack_flag)
	{
		cibuf.ack_flag = 0;
		acd=1;
		CounterInterrogationAck(cibuf.cot,cibuf.qcc);
		return;
	}

	if(cigroupflag)
	{
		acd=1;
		RespondCountGroupInterrogation(cibuf.qcc);	/*group??? */
		return;
	}

	acd = 0;

	if(Paramflag)
	{
		acd=1;
		RespondParamInterrogation();	
		return;
	}

	if(counterflag)
	{
		acd=1;
		RespondCountInterrogation();
		return;
	}

// 	NoChangeData = 0;
// 	if(soe_overflow_flag)
// 	{
// 		soe_overflow_flag = 0;
// 		CheckACD();
// 		SysInform((unsigned short)1);
// 		return;
// 	}
// 	
// 	if(ClkDiffflag)
// 	{
// 		ClkDiffflag = 0;
// 		CheckACD();
// 		SysInform((unsigned short)8);
// 		return;
// 	}
	
	SingleRespondByte();
}

int	CheckACD(void)
{
	/*Check for command soe response data */
	if((cmdbuf.ack_flag ==1)||(sebuf.ack_flag ==1)||interrogationflag||counterflag
		||(csbuf.ack_flag==1) ||(rdbuf.ack_flag ==1)||(testbuf.ack_flag ==1)
		||(rpbuf.ack_flag ==1)||(pmbuf.ack_flag ==1)||(icbuf.ack_flag ==1)
		||(cibuf.ack_flag ==1)||(icgroupflag ==1)||(cigroupflag ==1)
		||(ClkDiffflag ==1)||(MirrorNAckflag == 1)||(Paramflag ==1)
		||(localinitflag==1) ||(ResetProcessflag==1))
	{
		OutputDspInfo(0, "!!!new class 1 data exist!\n");
		acd = 1;
		return(1);
	}

	if(sp_cos == 1)
	{
		/*Check for SP status Change */
		CheckSinglePointChange();
		if(NoChangeData)
		{
			OutputDspInfo(0, "!!!CheckSinglePointChange data exist!\n");
			acd = 1;
			return(1);
		}

		/*Check for DP status Change */
		CheckDoublePointChange();
		if(NoChangeData)
		{
			OutputDspInfo(0, "!!!CheckDoublePointChange data exist!\n");
			acd = 1;
			return(1);
		}

		/*Check for ST status Change */
		CheckStepPositionChange();
		if(NoChangeData)
		{
			OutputDspInfo(0, "!!!CheckStepPositionChange data exist!\n");
			acd = 1;
			return(1);
		}
	}

	acd = 0;
	return(0);
}




void	RespondUserDataClass2(void)
{

/*Cyclic data */
// 	if((cyclicflag)&&(cycdef.number !=0))
// 	{
// 		if(MeASDUType==0)
// 			CyclicSendNA(m_cCyclicGrpNum);	/*M_ME_NA */
// 		else if(MeASDUType==1)
// 			CyclicSendNB(m_cCyclicGrpNum);	/*M_ME_NB */
// 		else if(MeASDUType==2)
// 			CyclicSendND(m_cCyclicGrpNum);	/*M_ME_ND */
// 		else
// 			CyclicSendNC(m_cCyclicGrpNum);	/*M_ME_NC */
// 		if(NoChangeData != 1)
// 		{
// 			OutputDspInfo(0, "++++m_cCyclicGrpNum =%d\n",m_cCyclicGrpNum);
// 			m_cCyclicGrpNum++;
// 			return;
// 		}
// 		OutputDspInfo(0, "++++Cyclic End! m_cCyclicGrpNum =%d\n",m_cCyclicGrpNum);
// 	// 
// 		m_cCyclicGrpNum = 0;
// 		cyclicflag = 0;
// 	}

/*Scan data */
// 	if(scanflag)
// 	{
// 		OutputDspInfo(0, "!!!ScanDataTransmit\n");
// 	//     	ScanDataTransmit();
//         scanflag=0; 
// 	}

	/*Check for ACD flag */
	if(CheckACD()==1)
		acd = 1;

	NoChangeData= 0;
	SequenceofEvent();
	if(!NoChangeData)
		return;

	if(MeASDUType==0)
		MeasureValueNA(SPONT);	/*M_ME_NA*/
	else if(MeASDUType==1)
		MeasureValueNB(SPONT);	/*M_ME_NB*/
	else if(MeASDUType==2)
		MeasureValueND(SPONT);	/*M_ME_ND*/
	else
		MeasureValueNC(SPONT);	/*M_ME_NC*/
	if(!NoChangeData)
		return;

	if(acd==1)
		RespondUserDataClass1();
	else
		SingleRespondByte();
}

int SequenceofEvent(void)
{
	unsigned char	nTypeID,nCause;
	unsigned short	soepnt;

	NoChangeData = 0;
	soepnt = soebuf.sendpnt;
	nCause = SPONT;
	while(soepnt != soebuf.bufpnt)
	{
		nTypeID = soebuf.soerec[soepnt].typeId;

		switch	( nTypeID ) 
		{
		case  M_SP_TA:
			retn = SinglePointInformTA(nCause);
			return(retn);

		case  M_SP_TB:
			retn = SinglePointInformTB(nCause);
			return(retn);

		case  M_DP_TA:
			retn = DoublePointInformTA(nCause);
			return(retn);

		case  M_DP_TB:
			retn = DoublePointInformTB(nCause);
			return(retn);

		case  M_ST_TA:
			retn = StepPositionInformTA(nCause);
			return(retn);

		case  M_ST_TB:
			retn = StepPositionInformTB(nCause);
			return(retn);

		default :
			OutputDspInfo(OUTPUT_WARN, "SOE No this TypeID = %d\n",nTypeID);
					break;			
		}/*switch(nTypeID) */
		soepnt ++;
		if(soepnt >= SOE_NUM)
			soepnt = 0;
		soebuf.sendpnt = soepnt;
	}

	NoChangeData = 1;
	return(0);
}

int	SendData(void)
{
	int send_byte=0;
	extern HANDLE	hPort_com;

	WriteSuccessf = WriteFile(hPort_com,(LPSTR)sendbuf,byte_to_send,&send_byte,NULL);

	if( send_byte != byte_to_send )
	{
		OutputDspInfo(OUTPUT_ERROR, "\nIEC101s: write %d byte error: %s !!!\n",byte_to_send,strerror(errno));
		PrintSendData(byte_to_send);
		sleep(1);
		return(-1);
	}
	PrintSendData(send_byte);
	return(0);
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
	stat=(unsigned char)dbentry.var.ind.status.rt_state;
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
	*pvti=(unsigned char)dbentry.var.ind.status.state;

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
		*pValue=(float)dbentry.var.imp.entry.rt_value;
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
		*pValue=(float)(dbentry.var.anlg.value.data/1000.0);
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
		*pValue=(unsigned int)(dbentry.var.anlg.value.data/1000.0);
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
	stat=(unsigned char)dbentry.var.ind.status.rt_state;
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

	if(ReadEntryByNameStr(pName,&dbentry)==-1)
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
	*pvti=(unsigned char)dbentry.var.ind.status.state;

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
		*pValue=(float)dbentry.var.imp.entry.rt_value;
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
		*pValue=(float)(dbentry.var.anlg.value.data/1000.0);
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

short	GetNormalizedValue(float value,float lorange,float hirange,float scale)
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
		anaorgcode=(short)(fdata*scale);

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
		*pValue=(unsigned int)(dbentry.var.anlg.value.data/1000.0);
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

