/****************************************************************
*  FILE NAME:           iec101s_app_fun.c      			*
*  FUNCTION:            Controlled Station			*
*			Application Layer Function		*
*  NAME :  			              			*
*  DATE:                August,2006                		*
*								*
****************************************************************/

#include "../inc/iec101s_com.h"

#define	DEBUG
#define BY_RECORD
#define REC_YF_AGC_CMD


extern	int		ProcNum;
extern	PROC_REC	sProcRec[MAX_PROC_NUM+1];

extern	unsigned char	sendbuf[1024],cmdsendbuf[24];

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

extern	int	fcb,acd,dfc;
extern	int	pollflag,comdflag,cmdterm;
extern	int	Readflag,ResetProcessflag;
extern	int	Paramflag,MirrorNAckflag;
extern	int	interrogationflag,counterflag,icgroupflag,cigroupflag;
extern	int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
extern	unsigned short	delayalarm;
extern	unsigned char	cot_interrogation,cot_counter;
extern	int	cyclicflag,scanflag;
extern	char	send_ic_data_type,send_ci_data_type;
extern	unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
extern	unsigned short	m_cRespParamNum,m_cRespICGrpNum,m_cRespCIGrpNum;
extern	unsigned char	count_sq,ITGrpType;

extern	unsigned short	seladdr;

extern	int linkaddr_length;
extern	int cmmaddr_length;
extern	unsigned short	nStnId;
extern	unsigned short	nASDUId;
extern	unsigned char	cmd_src;
extern	char	strDev[36];
extern	int	nBaudRate;
extern	int	Dualhost;	/* 0: Without Dual host switch; 1: With Dual host switch */
extern	int	Gps;		/* 0: Without GPS; 1: With GPS */
extern	int	GiType;		/*General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
extern	int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_ND; 3: M_ME_NC */
extern	int	MeType;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int	SEType;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	short	me_code;	/* 0 or -32768 */
extern	short	se_code;	/* 0 or -32768 */
extern	int	TimeTag;	/*Time tag> 0: no; 1: short; 2: long */
extern	int	sp_cos;		/*sp_cos> 0: sp/dp transmit once; 1: twice */
extern	int	Cmd_Sel;	/*  0: cmd without select; 1: cmd with select */

extern	time_t	scan_period;
extern	time_t	cycle_period;
extern	time_t	cmd_period;
extern	time_t	cmd_trans_delay;

extern	time_t	scan_tim;
extern	time_t	cycle_tim;

unsigned char	orgspsoebuf[MAX_SP_NUM],orgdpsoebuf[MAX_DP_NUM];
unsigned char	orgstsoebuf[MAX_ST_NUM],orgstvsoebuf[MAX_ST_NUM];
ASDUCOMDDATA	recv_data;
ASDUDATA	send_data,send_data2;

SYSTEMTIME		old_local_tm;

unsigned short	cd_delay_time;

#ifdef REC_YF_AGC_CMD
	unsigned short	daysaved;
	char	cmdrecfile[80];
	FILE	*fagc;
#endif

SPDEF		spdef;
DPDEF		dpdef;
MEDEF		medef;
CYCDEF		cycdef;
STDEF		stdef;
ITDEF		itdef;
DCDEF		dcdef;
SEDEF		sedef;
SCDEF		scdef;
RCDEF		rcdef;

unsigned short	sp_base_addr;
unsigned short	dp_base_addr;
unsigned short	me_base_addr;
unsigned short	cyc_base_addr;
unsigned short	pm_base_addr;
unsigned short	it_base_addr;
unsigned short	st_base_addr;
unsigned short	bo_base_addr;

unsigned short	sc_cmd_base_addr;
unsigned short	dc_cmd_base_addr;
unsigned short	rc_cmd_base_addr;
unsigned short	se_cmd_base_addr;
unsigned short	bc_cmd_base_addr;

char	infobuf[1024];

void InitSoeBuf(void);


void InitFlag(void)
{
	m_cRespGenInterroGrpNum=m_cRespCountGrpNum=m_cCyclicGrpNum=m_cScanGrpNum=0;
	send_data.pnt = 0;
	send_data2.pnt = 0;
	recv_data.pnt = 0;
	
	cmdbuf.typeId = 0;
	cmdbuf.infoaddr = 0;
	cmdbuf.ack_flag = 0;
	cmdbuf.selected = 0;
	cmdbuf.term_flag = 0;
	cmdbuf.sel_tim = 0;
	scan_tim = 0;
	cycle_tim = 0;
	
	sebuf.ack_flag = 0;
	pmbuf.ack_flag = 0;
	icbuf.ack_flag = 0;
	cibuf.ack_flag = 0;
	rdbuf.ack_flag = 0;
	rpbuf.ack_flag = 0;
	testbuf.ack_flag = 0;
	cdbuf.ack_flag = 0;
	cdbuf.cp16time = 0;
	icgroupflag = 0;
	cigroupflag = 0;

	acd = 0;
	fcb = 0;
	
	counterflag=interrogationflag=0;
	icgroupflag = cigroupflag = 0;
	ResetProcessflag = MirrorNAckflag = 0;
	Paramflag = Readflag = 0;
	cyclicflag = scanflag = 0;
	soe_overflow_flag = soe_overflow_Sendflag = ClkDiffflag = 0;
	count_sq = 0;

}

void InitConfig(char *NameID)
{
	nStnId = LINKADRESS;
	nASDUId = CMM_ADRESS;
	nBaudRate = Com_Baud_Rate;


	linkaddr_length= 1;
	cmmaddr_length= 1;
	Dualhost = 0;		/* 0: Without Dual host switch */
	Gps = 1;		/* With GPS */
	GiType = 1;		/* 1:M_SP_NA; 20: M_PS_NA */
	MeASDUType = 0;		/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 2: M_ME_ND; 3: M_ME_NC */
	MeType = 1;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	SEType =0;		/*0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
/*	TimeTag =2;	 */	/*time tag 0: no; 1: short; 2: long */
	sp_cos = 1;		/*sp_cos 0: sp/dp transmit once; 1: twice */
	Cmd_Sel = 1;		/* 0: cmd without select; 1: cmd with select */
	ITGrpType = 0;

	scan_period = SCAN_PERIOD;
	cycle_period = CYCLE_PERIOD;
	cmd_period = CMD_TIMEOUT;
	delayalarm = DIFF_TIME;
	cd_delay_time = 0;

	sp_base_addr = SP_BASE_ADDR;
	dp_base_addr = DP_BASE_ADDR;
	me_base_addr = ME_BASE_ADDR;
	cyc_base_addr = CYC_BASE_ADDR;
	pm_base_addr = PM_BASE_ADDR;
	it_base_addr = IT_BASE_ADDR;
	st_base_addr = ST_BASE_ADDR;
	bo_base_addr = BO_BASE_ADDR;

	sc_cmd_base_addr = SC_CMD_BASE_ADDR;
	dc_cmd_base_addr = DC_CMD_BASE_ADDR;
	rc_cmd_base_addr = RC_CMD_BASE_ADDR;

	se_cmd_base_addr = SE_CMD_BASE_ADDR;
	bc_cmd_base_addr = BC_CMD_BASE_ADDR;


	spdef.number = 0;
	dpdef.number = 0;
	stdef.number = 0;
	itdef.number = 0;

	medef.number = 0;

	cycdef.number = 0;

	sedef.number = 0;

	scdef.number = 0;
	dcdef.number = 0;
	rcdef.number = 0;

	GenConfigTable(NameID,(char *)CONFIG_FILE);

	GenSPTable(NameID,(char *)SPDEF_FILE);
	GenDPTable(NameID,(char *)DPDEF_FILE);

	GenMETable(NameID,(char *)MEDEF_NORM);
	GenCYCTable(NameID,(char *)CYCDEF_NORM);

	GenSTTable(NameID,(char *)STDEF_FILE);
	GenITTable(NameID,(char *)ITDEF_FILE);

	GenSETable(NameID,(char *)SEDEF_NORM);

	GenSCTable(NameID,(char *)SCDEF_FILE);
	GenDCTable(NameID,(char *)DCDEF_FILE);
	GenRCTable(NameID,(char *)RCDEF_FILE);


	InitSoeBuf();
	InitFlag();

	if(MeType)
		me_code = -32768;
	else
		me_code = 0;

	if(SEType)
		se_code = -32768;
	else
		se_code = 0;
}

int ReadStnDef(char * pStnDefFile)
{	FILE	*fp;
	char	NameID[256], Ip[256], CtrlName[256];
	int		c_src;
	char	filebuf[LINE_BUF_CHAR];
	int		r;
	char	filenamestr[256];
	
	r=0;
	ProcNum = 0;
	sprintf(filenamestr,"%s%s",IEC101_HOMEPATH,pStnDefFile);
	while( (fp=fopen(filenamestr,"r"))==NULL )
	{
		OutputDspInfo(OUTPUT_ERROR,"\n### ALARM !!! IEC101: Can't open Station config file %s !!! \n",filenamestr);
		Sleep(5000);
	}

	for(;;)
	{
		if(ProcNum>=MAX_PROC_NUM)
		{
			fclose( fp );
			OutputDspInfo(0, "\nRead Station define file %s: proc num = %d!!!\n\n",pStnDefFile,ProcNum);
			return( TRUE );
		}

		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				OutputDspInfo(0, "\nRead Station define file %s: proc num = %d!!!\n\n",pStnDefFile,ProcNum);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				printf("ReadStnDef Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			continue;
		}
		if(sscanf(filebuf,"%s %s %d %s",NameID,Ip,&c_src,CtrlName)!=4)
		{
			OutputDspInfo(OUTPUT_ERROR, "ReadStnDef Error!%3d line: %s\n",r,filebuf);
			continue;
		}
		strcpy( sProcRec[ProcNum].NameID, NameID );
		strcpy( sProcRec[ProcNum].Ip, Ip );
		strcpy( sProcRec[ProcNum].ctrl_priv, CtrlName );
		sProcRec[ProcNum].cmd_src = c_src;
		sProcRec[ProcNum].Pid = -1;
		OutputDspInfo(0, "\nStation %d name %s: ADDR = %s SRC=%d Ctrl_Name = %s\n",ProcNum,sProcRec[ProcNum].NameID,
			sProcRec[ProcNum].Ip,sProcRec[ProcNum].cmd_src,sProcRec[ProcNum].ctrl_priv);
		if(sProcRec[ProcNum].NameID[0] != '!')
			ProcNum++;
	}
}

int GenConfigTable(char * pName,char * pDefFile)
{
	FILE	*fp;
	int		iTemp;
	char	strFlag[80];
	char	strType[80];
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];
	int		n,r;

	r=0;
	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "\nCan't open config file %s!!!\n",filenamestr);
		nASDUId = CMM_ADRESS;
		sleep(5);
	}
	rewind(fp);	
	OutputDspInfo(0, "\n\nRead config file %s!!!\n",filenamestr);

	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				OutputDspInfo(0, "file: %s END\n",pDefFile);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			continue;
		}
		if(sscanf(filebuf,"%s",strFlag)<=0)
		{
			OutputDspInfo(OUTPUT_ERROR, "Config file Error!%3d line: %s\n",r,filebuf);
			continue;
		}
		n = strlen(strFlag);
		if(strcmp(strFlag,"LINKADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! LINKADDR=%d\n",pDefFile,iTemp);
				nStnId = LINKADRESS;
				continue;
			}
			if((iTemp<=0) || (iTemp>65534))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! LINKADDR=%d\n",pDefFile,iTemp);
				nStnId = LINKADRESS;
			}
			else
			{
				OutputDspInfo(0, "LINKADDR = %d\n",iTemp);
				nStnId = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMM_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file Error!%3d line: %s\n",r,filebuf);
				nASDUId = CMM_ADRESS;
				continue;
			}
			if((iTemp<=0) || (iTemp>65534))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ASDU CMM_ADDR=%d\n",pDefFile,iTemp);
				nASDUId = CMM_ADRESS;
			}
			else
			{
				OutputDspInfo(0, "ASDU CMM_ADDR = %d\n",iTemp);
				nASDUId = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMMADDR_LENGTH:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file Error!%3d line: %s\n",r,filebuf);
				cmmaddr_length = 1;
				continue;
			}
			if((iTemp<=0) || (iTemp>2))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ASDU CMMADDR_LENGTH=%d\n",pDefFile,iTemp);
				cmmaddr_length = 1;
			}
			else
			{
				OutputDspInfo(0, "ASDU CMMADDR_LENGTH = %d\n",iTemp);
				cmmaddr_length = iTemp;
			}
		}
		else if(strcmp(strFlag,"LINKADDR_LENGTH:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file Error!%3d line: %s\n",r,filebuf);
				linkaddr_length = 1;
				continue;
			}
			if((iTemp<=0) || (iTemp>2))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! LINKADDR_LENGTH=%d\n",pDefFile,iTemp);
				linkaddr_length = 1;
			}
			else
			{
				OutputDspInfo(0, "LINKADDR_LENGTH = %d\n",iTemp);
				linkaddr_length = iTemp;
			}
		}
		else if(strcmp(strFlag,"BAUDRATE:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! BAUDRATE = %d\n",pDefFile,iTemp);
				nBaudRate = Com_Baud_Rate;
				continue;
			}
			if((iTemp<=300) || (iTemp>19200) || (iTemp%100 != 0))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! BAUDRATE = %d\n",pDefFile,iTemp);
				nBaudRate = Com_Baud_Rate;
			}
			else
			{
				OutputDspInfo(0, "BAUDRATE = %d\n",iTemp);
				nBaudRate = iTemp;
			}
		}
		else if(strcmp(strFlag,"DUALHOST:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! DUALHOST = %d\n",pDefFile,iTemp);
				Dualhost = 0;
				continue;
			}
			if((iTemp!=0) && (iTemp!=1))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! DUALHOST = %d\n",pDefFile,iTemp);
				Gps = 1;
			}
			else
			{
				OutputDspInfo(0, "DUALHOST = %d\n",iTemp);
				Dualhost = iTemp;
			}
		}
		else if(strcmp(strFlag,"GPS:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! GPS = %d\n",pDefFile,iTemp);
				Gps = 1;
				continue;
			}
			if((iTemp!=0) && (iTemp!=1))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! Gps = %d\n",pDefFile,iTemp);
				Gps = 1;
			}
			else
			{
				OutputDspInfo(0, "GPS = %d\n",iTemp);
				Gps = iTemp;
			}
		}
		else if(strcmp(strFlag,"GI_TYPE:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! GI_TYPE = %d\n",pDefFile,iTemp);
				GiType = 1;
				continue;
			}
			if((iTemp!=1) && (iTemp!=20))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! GI_TYPE = %d\n",pDefFile,iTemp);
				GiType = 1;
			}
			else
			{
				OutputDspInfo(0, "GI_TYPE = %d\n",iTemp);
				GiType = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_ASDUTYPE:")== 0)
		{
			if(sscanf(&filebuf[n],"%s",strType)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ME_ASDUTYPE = %d\n",pDefFile,iTemp);
				MeASDUType = 3;
				continue;
			}
			if(strcmp(strType,"M_ME_NA")== 0)
			{
				OutputDspInfo(0, "ME_ASDUTYPE = %s\n",strType);
				MeASDUType = 0;
			}
			else if(strcmp(strType,"M_ME_NB")== 0)
			{
				OutputDspInfo(0, "ME_ASDUTYPE = %s\n",strType);
				MeASDUType = 1;
			}
			else if(strcmp(strType,"M_ME_ND")== 0)
			{
				OutputDspInfo(0, "ME_ASDUTYPE = %s\n",strType);
				MeASDUType = 2;
			}
			else if(strcmp(strType,"M_ME_NC")== 0)
			{
				OutputDspInfo(0, "ME_ASDUTYPE = %s\n",strType);
				MeASDUType = 3;
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ME_ASDUTYPE = %s\n",pDefFile,strType);
				MeASDUType = 3;
			}
		}
		else if(strcmp(strFlag,"ME_TYPE:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				MeType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				MeType = 0;
			}
			else
			{
				OutputDspInfo(0, "ME_TYPE = %d\n",iTemp);
				MeType = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_TYPE:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SE_TYPE = %d\n",pDefFile,iTemp);
				SEType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SE_TYPE = %d\n",pDefFile,iTemp);
				SEType = 0;
			}
			else
			{
				OutputDspInfo(0, "SE_TYPE = %d\n",iTemp);
				SEType = iTemp;
			}
		}

		else if(strcmp(strFlag,"TIMETAG:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! TIMETAG = %d\n",pDefFile,iTemp);
				TimeTag = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! TIMETAG = %d\n",pDefFile,iTemp);
				TimeTag = 1;
			}
			else
			{
				OutputDspInfo(0, "TIMETAG = %d\n",iTemp);
				TimeTag = iTemp;
			}
		}

		else if(strcmp(strFlag,"SP_COS:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SP_COS = %d\n",pDefFile,iTemp);
				sp_cos = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>1))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SP_COS = %d\n",pDefFile,iTemp);
				sp_cos = 1;
			}
			else
			{
				OutputDspInfo(0, "SP_COS = %d\n",iTemp);
				sp_cos = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMD_SELECT:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CMD_SELECT = %d\n",pDefFile,iTemp);
				Cmd_Sel = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CMD_SELECT = %d\n",pDefFile,iTemp);
				Cmd_Sel = 1;
			}
			else
			{
				OutputDspInfo(0, "CMD_SELECT = %d\n",iTemp);
				Cmd_Sel = iTemp;
			}
		}
		else if(strcmp(strFlag,"SCAN_PERIOD:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SCAN_PERIOD = %d\n",pDefFile,iTemp);
				scan_period = SCAN_PERIOD;
				continue;
			}
			if(iTemp<0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SCAN_PERIOD = %d\n",pDefFile,iTemp);
				scan_period = SCAN_PERIOD;
			}
			else
			{
				OutputDspInfo(0, "SCAN_PERIOD = %d\n",iTemp);
				scan_period = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYCLE_PERIOD:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SCAN_PERIOD = %d\n",pDefFile,iTemp);
				cycle_period = CYCLE_PERIOD;
				continue;
			}
			if(iTemp<0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CYCLE_PERIOD = %d\n",pDefFile,iTemp);
				cycle_period = CYCLE_PERIOD;
			}
			else
			{
				OutputDspInfo(0, "CYCLE_PERIOD = %d\n",iTemp);
				cycle_period = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMD_TIMEOUT:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CMD_TIMEOUT = %d\n",pDefFile,iTemp);
				cmd_period = CMD_TIMEOUT;
				continue;
			}
			if(iTemp<0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CMD_TIMEOUT = %d\n",pDefFile,iTemp);
				cmd_period = CMD_TIMEOUT;
			}
			else
			{
				OutputDspInfo(0, "CMD_TIMEOUT = %d\n",iTemp);
				cmd_period = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMD_DELAY:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CMD_TIMEOUT = %d\n",pDefFile,iTemp);
				cmd_trans_delay = CMD_DELAY;
				continue;
			}
			if(iTemp<0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CMD_DELAY = %d\n",pDefFile,iTemp);
				cmd_trans_delay = CMD_DELAY;
			}
			else
			{
				OutputDspInfo(0, "CMD_DELAY = %d\n",iTemp);
				cmd_trans_delay = iTemp;
			}
		}
		else if(strcmp(strFlag,"SP_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SP_BASE_ADDR=%X\n",pDefFile,iTemp);
				sp_base_addr = SP_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SP_BASE_ADDR=%X\n",pDefFile,iTemp);
				sp_base_addr = SP_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "SP_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				sp_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"DP_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! DP_BASE_ADDR=%X\n",pDefFile,iTemp);
				dp_base_addr = DP_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! DP_BASE_ADDR=%X\n",pDefFile,iTemp);
				dp_base_addr = DP_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "DP_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				dp_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ME_BASE_ADDR=%X\n",pDefFile,iTemp);
				me_base_addr = ME_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ME_BASE_ADDR=%X\n",pDefFile,iTemp);
				me_base_addr = ME_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "ME_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				me_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYC_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CYC_BASE_ADDR=%X\n",pDefFile,iTemp);
				cyc_base_addr = CYC_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! CYC_BASE_ADDR=%X\n",pDefFile,iTemp);
				cyc_base_addr = CYC_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "CYC_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				cyc_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"PM_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! PM_BASE_ADDR=%X\n",pDefFile,iTemp);
				pm_base_addr = PM_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! PM_BASE_ADDR=%X\n",pDefFile,iTemp);
				pm_base_addr = PM_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "PM_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				pm_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"IT_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! IT_BASE_ADDR=%X\n",pDefFile,iTemp);
				it_base_addr = IT_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! IT_BASE_ADDR=%X\n",pDefFile,iTemp);
				it_base_addr = IT_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "IT_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				it_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ST_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ST_BASE_ADDR=%X\n",pDefFile,iTemp);
				st_base_addr = ST_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ST_BASE_ADDR=%X\n",pDefFile,iTemp);
				st_base_addr = ST_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "ST_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				st_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"BO_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! BO_BASE_ADDR=%X\n",pDefFile,iTemp);
				bo_base_addr = BO_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! BO_BASE_ADDR=%X\n",pDefFile,iTemp);
				bo_base_addr = BO_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "BO_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				bo_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SC_CMD_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				sc_cmd_base_addr = SC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				sc_cmd_base_addr = SC_CMD_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "SC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				sc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"DC_CMD_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! DC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				dc_cmd_base_addr = DC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! DC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				dc_cmd_base_addr = DC_CMD_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "DC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				dc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"RC_CMD_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! RC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				rc_cmd_base_addr = RC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! RC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				rc_cmd_base_addr = RC_CMD_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "RC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				rc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_CMD_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SE_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_base_addr = SE_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! SE_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_base_addr = SE_CMD_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "SE_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				se_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"BC_CMD_BASE_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%x",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! BC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				bc_cmd_base_addr = BC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! BC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				bc_cmd_base_addr = BC_CMD_BASE_ADDR;
			}
			else
			{
				OutputDspInfo(0, "BC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
				bc_cmd_base_addr = iTemp;
			}
		}
		else
			OutputDspInfo(OUTPUT_WARN, "Config file Unsupported item: %s\n",strFlag);
	}
}

int GenSPTable(char * pName,char *pDefFile)		/* single point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[256];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "\n####can't open SP file %s\n",filenamestr);
		spdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	

	OutputDspInfo(0, "\nRead SP define file %s!!!\n",filenamestr);

	for(;;)
	{
		if(j >= MAX_SP_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenSPTable: Num=%d above the MAX_SP_NUM %d\n",j,MAX_SP_NUM);
			spdef.number = MAX_SP_NUM;
			fclose(fp);
			return(FALSE);
		}

		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				spdef.number = j;
				OutputDspInfo(0, "file: %s END --- GenSPTable: point nNumber = %d\n",pDefFile,spdef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "GenSPTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %s",&iTemp,spdef.name[j],&iTemp1,lgname)!=4)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenSPTable: Error!%3d line: %s\n",r,filebuf);
			j ++;
			continue;
		}
		spdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "GenSPTable: +++Warning! %d line: num %d != %d\n",r,iTemp,j);
			
		}
		if(strcmp(spdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "SP %d pnt: %s\t%d\t%s\n",j,spdef.name[j],spdef.TimeTag[j],lgname);
			
			strcpy(name_buf2,spdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				spdef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "GenSPTable: #### SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				spdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			spdef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "GenSPTable: SP %d pnt: NULL\n",j);
			
		}
		j ++;
		
	}
}

int GenDPTable(char * pName,char *pDefFile)		/* double point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[256];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "####can't open DP file %s\n",filenamestr);
			
 		dpdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	OutputDspInfo(0, "\nRead DP define file %s!!!\n",filenamestr);
	
 	for(;;)
	{
		if(j >= MAX_DP_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenDPTable: Num=%d above the MAX_DP_NUM %d\n",j,MAX_DP_NUM);
			
			dpdef.number = MAX_DP_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				dpdef.number = j;
				OutputDspInfo(0, "file: %s END ---  GenDPTable: point nNumber = %d\n",pDefFile,dpdef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "GenDPTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %s",&iTemp,dpdef.name[j],&iTemp1,lgname)!=4)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenDPTable: Error!%3d line: %s\n",r,filebuf);
			j ++;
			continue;
		}
		dpdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "GenDPTable: +++Warning! %d line: num %d != %d\n",r,iTemp,j);
			
		}
		if(strcmp(dpdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "DP %d pnt: %s\t%d\t%s\n",j,dpdef.name[j],dpdef.TimeTag[j],lgname);
			
			strcpy(name_buf2,dpdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				dpdef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "GenDPTable: #### DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				dpdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			dpdef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "GenDPTable: DP %d pnt: NULL\n",j);
			
		}
		j ++;
	}
}

int GenMETable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "\n####can't open ME file %s\n",filenamestr);
			
 		medef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	

	OutputDspInfo(0, "\nRead ME define file %s!!!\n",filenamestr);
	
	for(;;)
	{
		if(j >= MAX_ME_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenMETable: Num=%d above the MAX_ME_NUM %d\n",j,MAX_ME_NUM);
			
			medef.number = MAX_ME_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				medef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenMETable: point nNumber = %d\n",pDefFile,medef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "GenMETable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %f %f %f %f %d %d %s",&iTemp,medef.name[j],&medef.lorange[j],&medef.hirange[j],
					&medef.scale[j],&medef.databand[j],&iTemp1,&iTemp2,lgname)!=9)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenMETable: Error!%3d line: %s\n",r,filebuf);			
			j ++;
			continue;
		}
		medef.ASDUType[j] = iTemp1;
		medef.TimeTag[j] = iTemp2;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "GenMETable: +++Warning! %d line: num %d != %d\n",r,iTemp,j);
		}
		if(strcmp(medef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "ME %d %s %5.3f %5.3f %5.3f %5.3f %d %d %s\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],
				medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],lgname);
			
			strcpy(name_buf2,medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				medef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "GenMETable: #### ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				medef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			medef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "GenMETable: ME %d pnt: NULL\n",j);
			
		}
		j ++;
	}
}

int SaveMETable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE];
	char		longname_buf[36];

	short	j,k;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"w"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "\n####can't open(w) ME file %s\n",filenamestr);
			
 		medef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	

	OutputDspInfo(0, "---SaveMETable: File %s, point number = %d\n",filenamestr,medef.number);
	

	for(j=0;j<medef.number;j++)
	{
		if(strcmp(medef.name[j],"NULL")!=0)
		{
			strcpy(name_buf2,medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				fprintf(fp,"%04d %s\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%d\t%d\t数据库无此点%d",j,medef.name[j],medef.lorange[j],medef.hirange[j],
					medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],j);

				OutputDspInfo(0, "ME %04d pnt: %s\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%d\t%d\t数据库无此点%d",j,medef.name[j],medef.lorange[j],medef.hirange[j],
					medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],j);
			}
			else
			{
				strcpy(longname_buf,dbentry2.fixed.entryLongname);
				for(k=0;k<36;k++)
				{
					if((longname_buf[k] == ' ')||(longname_buf[k] == 9))
						longname_buf[k] = '_';
				}
				fprintf(fp,"%04d %s\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%d\t%d\t%s",j,medef.name[j],medef.lorange[j],medef.hirange[j],
					medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],longname_buf);

				OutputDspInfo(0, "ME %04d pnt: %s\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%d\t%d\t%s",j,medef.name[j],medef.lorange[j],medef.hirange[j],
					medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],longname_buf);
			}
		}
		else
		{
			fprintf(fp,"%04d %s\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%d\t%d\t测量值备用点%d",j,medef.name[j],medef.lorange[j],medef.hirange[j],
					medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],j);

			OutputDspInfo(0, "ME %04d pnt: %s\t%5.3f\t%5.3f\t%5.3f\t%5.3f\t%d\t%d\t测量值备用点%d",j,medef.name[j],medef.lorange[j],medef.hirange[j],
					medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],j);
			
		}
	}
	fclose(fp);
	return(TRUE);
}

int GenCYCTable(char * pName,char *pDefFile)		/* Cyclic point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "\n####can't open Cyclic file %s\n",filenamestr);
			
 		cycdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	

	OutputDspInfo(0, "\nRead Cyclic define file %s!!!\n",filenamestr);
	
	for(;;)
	{
		if(j >= MAX_CYC_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenCYCTable: Num=%d above the MAX_CYC_NUM %d\n",j,MAX_CYC_NUM);
			
			cycdef.number = MAX_CYC_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				cycdef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenCYCTable: point nNumber = %d\n",pDefFile,medef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "GenCYCTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %f %f %f %d %s",&iTemp,cycdef.name[j],&cycdef.lorange[j],&cycdef.hirange[j],
					&cycdef.scale[j],&iTemp1,lgname)!=7)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenCYCTable: Error!%3d line: %s\n",r,filebuf);
			j ++;
			continue;
		}
		cycdef.ASDUType[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "GenCYCTable: +++Warning! %d line: num %d != %d\n",r,iTemp,j);
		}
		if(strcmp(cycdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "CYC %d %s %f %f %f %d %s\n",j,cycdef.name[j],cycdef.lorange[j],cycdef.hirange[j],
				cycdef.scale[j],cycdef.ASDUType[j],lgname);
			
			strcpy(name_buf2,cycdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				cycdef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "GenCYCTable: #### CYC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				cycdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			cycdef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "GenCYCTable: CYC %d pnt: NULL\n",j);
			
		}
		j ++;
	}
}

int GenSTTable(char * pName,char *pDefFile)		/* integrated total point define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[256];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "\n####can't open ST file %s\n",filenamestr);
			
		stdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	OutputDspInfo(0, "\nRead ST define file %s!!!\n",filenamestr);
	

	for(;;)
	{
		if(j >= MAX_ST_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenSTTable: Num=%d above the MAX_ST_NUM %d\n",j,MAX_ST_NUM);
			
			stdef.number = MAX_ST_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				stdef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenSTTable: point nNumber = %d\n",pDefFile,stdef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "GenSTTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %s",&iTemp,stdef.name[j],&iTemp1,lgname)!=4)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenSTTable: Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		stdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "GenSTTable: +++Warning! %d line: num %d != %d\n",r,iTemp,j);
			
		}
		if(strcmp(stdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "ST %d pnt: %s\t%d\t%s\n",j,stdef.name[j],stdef.TimeTag[j],lgname);
			
			strcpy(name_buf2,stdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				stdef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "GenSTTable: #### ST %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				stdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			stdef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "GenSTTable: ST %d pnt: NULL\n",j);
			
		}
		j ++;
	}
}

int GenITTable(char * pName,char *pDefFile)		/* integrated total point define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[256];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "\n####can't open IT file %s\n",filenamestr);
			
		itdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	OutputDspInfo(0, "\nRead IT define file %s!!!\n",filenamestr);
	

	for(;;)
	{
		if(j >= MAX_IT_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenITTable: Num=%d above the MAX_IT_NUM %d\n",j,MAX_IT_NUM);
			
			itdef.number = MAX_IT_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				itdef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenITTable: point nNumber = %d\n",pDefFile,itdef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "GenITTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %s",&iTemp,itdef.name[j],&iTemp1,lgname)!=4)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenITTable: Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		itdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "GenITTable: +++Warning! %d line: num %d != %d\n",r,iTemp,j);
			
		}
		if(strcmp(itdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "IT %d pnt: %s\t%d\t%s\n",j,itdef.name[j],itdef.TimeTag[j],lgname);
			
			strcpy(name_buf2,itdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "GenITTable: #### IT %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "GenITTable: IT %d pnt: NULL\n",j);
			
		}
		j ++;
	}
}

int GenSCTable(char * pName,char *pDefFile)		/* single point control command define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2,numb;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "####can't open SC file %s\n",filenamestr);
			
		scdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	r=j=0;

	OutputDspInfo(0, "\nRead SC define file %s!!!\n",filenamestr);
	

	for(;;)
	{
		if(j >= MAX_SC_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenSCTable: Num=%d above the MAX_SC_NUM %d\n",j,MAX_SC_NUM);
			
			scdef.number = MAX_SC_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				scdef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenSCTable: point nNumber = %d\n",pDefFile,scdef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "GenSCTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %d %d %s %s",&numb,scdef.name[j],&iTemp,&iTemp1,&iTemp2,scdef.interlockname[j],lgname)!=7)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenSCTable: Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		scdef.cmdtype[j] = (unsigned char)iTemp;
		scdef.optype1[j] = (unsigned char)iTemp1;
		scdef.optype2[j] = (unsigned char)iTemp2;

		if(j!=numb)
		{
			OutputDspInfo(OUTPUT_WARN, "GenSCTable: SC %d pnt number=%d wrong!!! %s %d %d %d %s %s\n",j,numb,scdef.name[j],
				scdef.cmdtype[j],scdef.optype1[j],scdef.optype2[j],scdef.interlockname[j],lgname);
			
		}
		if(strcmp(scdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "SC %d pnt: %s %d %d %d %s %s\n",j,scdef.name[j],scdef.cmdtype[j],
				scdef.optype1[j],scdef.optype2[j],scdef.interlockname[j],lgname);
			
			strcpy(name_buf2,scdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "GenSCTable: #### SC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "GenSCTable: SC %d pnt: NULL\n",j);
			
		}
		j ++;
	}
}

int GenDCTable(char * pName,char *pDefFile)		/* double point control command define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2,numb;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "####can't open DC file %s\n",filenamestr);
			
 		dcdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	r=j=0;

	OutputDspInfo(0, "\nRead DC define file %s!!!\n",filenamestr);
	

	for(;;)
	{
		if(j >= MAX_DC_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenDCTable: Num=%d above the MAX_DC_NUM %d\n",j,MAX_DC_NUM);
			
			dcdef.number = MAX_DC_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				dcdef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenDCTable: point nNumber = %d\n",pDefFile,dcdef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "GenDCTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %d %d %s %s",&numb,dcdef.name[j],&iTemp,&iTemp1,&iTemp2,dcdef.interlockname[j],lgname)!=7)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenDCTable: Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		dcdef.cmdtype[j] = (unsigned char)iTemp;
		dcdef.optype1[j] = (unsigned char)iTemp1;
		dcdef.optype2[j] = (unsigned char)iTemp2;

		if(j!=numb)
		{
			OutputDspInfo(OUTPUT_WARN, "DC %d pnt number=%d wrong!!! %s %d %d %d %s %s\n",j,numb,dcdef.name[j],
				dcdef.cmdtype[j],dcdef.optype1[j],dcdef.optype2[j],dcdef.interlockname[j],lgname);
			
		}
		if(strcmp(dcdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "GenDCTable: DC %d pnt: %s %d %d %d %s %s\n",j,dcdef.name[j],dcdef.cmdtype[j],
				dcdef.optype1[j],dcdef.optype2[j],dcdef.interlockname[j],lgname);
			
			strcpy(name_buf2,dcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "GenDCTable: #### DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "GenDCTable: DC %d pnt: NULL\n",j);
			
		}
		j ++;
	}
}

int GenSETable(char * pName,char *pDefFile)		/* set point control point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[256];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "####can't open SE file %s\n",filenamestr);
			
		sedef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	r=j=0;

	OutputDspInfo(0, "\nRead SE define file %s!!!\n",filenamestr);
	

	for(;;)
	{
		if(j >= MAX_SE_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenSETable: Num=%d above the MAX_SE_NUM %d\n",j,MAX_SE_NUM);
			sedef.number = MAX_SE_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				sedef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenSETable: point nNumber = %d\n",pDefFile,sedef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "GenSETable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %f %f %f %d %s %s",&iTemp,sedef.name[j],&sedef.lorange[j],&sedef.hirange[j],
			&sedef.scale[j],&iTemp1,sedef.interlockname[j],lgname)!=8)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenSETable: Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		sedef.cmdtype[j] = iTemp1;

		if(j!=iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "SE %d pnt number=%d wrong!!! %s %f %f %f %d %s %s\n",j,iTemp,sedef.name[j],
				sedef.lorange[j],sedef.hirange[j],sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
			
		}

		if(strcmp(sedef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "GenSETable: SE %d pnt: %s %5.3f %5.3f %5.3f %d %s %s\n",j,sedef.name[j],sedef.lorange[j],sedef.hirange[j],
					sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
			
			strcpy(name_buf2,sedef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "GenSETable: #### SE %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "GenSETable: SE %d pnt: NULL %5.3f %5.3f %5.3f %d %s %s\n",j,sedef.lorange[j],sedef.hirange[j],
					sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
			
		}
		j ++;
	}
}

int GenRCTable(char * pName,char *pDefFile)		/* step point control command define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2,numb;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_%s",IEC101_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "####can't open RC file %s\n",filenamestr);
			
		rcdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	r=j=0;

	OutputDspInfo(0, "\nRead RC define file %s!!!\n",filenamestr);
	

	for(;;)
	{
		if(j >= MAX_RC_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenRCTable: Num=%d above the MAX_RC_NUM %d\n",j,MAX_RC_NUM);
			
			rcdef.number = MAX_RC_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				rcdef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenRCTable: point nNumber = %d\n",pDefFile,rcdef.number);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "GenRCTable: Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %d %d %s %s",&numb,rcdef.name[j],&iTemp,&iTemp1,&iTemp2,rcdef.interlockname[j],lgname)!=7)
		{
			OutputDspInfo(OUTPUT_ERROR, "GenRCTable: Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		rcdef.cmdtype[j] = (unsigned char)iTemp;
		rcdef.optype1[j] = (unsigned char)iTemp1;
		rcdef.optype2[j] = (unsigned char)iTemp2;

		if(j!=numb)
		{
			OutputDspInfo(OUTPUT_WARN, "GenRCTable: RC %d pnt number=%d wrong!!! %s %d %d %d %s %s\n",j,numb,rcdef.name[j],
				rcdef.cmdtype[j],rcdef.optype1[j],rcdef.optype2[j],rcdef.interlockname[j],lgname);
			
		}
		if(strcmp(rcdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "RC %d pnt: %s %d %d %d %s %s\n",j,rcdef.name[j],rcdef.cmdtype[j],
				rcdef.optype1[j],rcdef.optype2[j],rcdef.interlockname[j],lgname);
			
			strcpy(name_buf2,rcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "GenRCTable: #### RC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "GenRCTable: RC %d pnt: NULL\n",j);
			
		}
		j ++;
	}
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

void	IEC870_5_Recv(int pnum,unsigned int recvdatapt, USHORT recv_cmmaddr)
{
	LAN_HEAD	lanhead;
	unsigned char	me_buf[1440];
	unsigned short	infoaddr,ms,year,datalength,lengthfactor;
	short		infoptr;
	char		dbname[36];
	unsigned char	type_id,varstru,causetra,qoc,frz,lanpt;
	unsigned char	infonum,objpnt,opstat;
	float		value;
	short		setvalue;
	short		cmd_stat;
	char		cmdtype;

	DMS_COMMON	dbcommpt;
	int		i,datapoint;
	ANLG_DATA	anadata;

	struct	tm	tmpt;
	time_t		tim;
	int	diffclk;
	char *data_buff= recv_data.data[recvdatapt];  //start from recvbuf[4]: ctrlfunc linkaddr ASDU

	type_id = data_buff[2];
	varstru = data_buff[3];
	causetra = data_buff[4];

	if(cmmaddr_length==1)
	{
		if(recv_cmmaddr!=nASDUId && recv_cmmaddr!=BROADCAST_CMMADRESS1)
		{
			OutputDspInfo(OUTPUT_ERROR, "ASDU addr %d != %d wrong!\n",recv_cmmaddr,nASDUId);
			
			mirbuf.cot=(unsigned char)(0x40|UKASDUAD);
			mirbuf.pnt=recvdatapt;
			MirrorNAckflag = 1;
			return;
		}
	}
	else if(cmmaddr_length==2)
	{
		if(recv_cmmaddr!=nASDUId && recv_cmmaddr!=BROADCAST_CMMADRESS2)
		{
			OutputDspInfo(OUTPUT_ERROR, "ASDU addr %d != %d wrong!\n",recv_cmmaddr,nASDUId);
			
			mirbuf.cot=(unsigned char)(0x40|UKASDUAD);
			mirbuf.pnt=recvdatapt;
			MirrorNAckflag = 1;
			return;
		}
	}

	tim=time(&tim);
	tmpt=*localtime(&tim);

	switch(type_id)
	{
	case C_SC_NA:	/*45 Single point control command */

		qoc = data_buff[8];/*QOC:= S/E[8] QU[3..7] SCS[1] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif
		cmdbuf.typeId = type_id;
		cmdbuf.stat = qoc;
		cmdbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-sc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=scdef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "%04x info address wrong! No this point in SC Control Table!\n",infoaddr);
				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}
			strcpy(dbname,scdef.name[infoptr]);
			OutputDspInfo(0, "recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,data_buff[8],causetra);
	
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "%s No this SC control point in the database file!\n",dbname);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}

			if((qoc & 0x1)==1)
				opstat = 1;
			else
				opstat = 0;

			if(Get_Cmd_Ctrl_Priv(scdef.interlockname[infoptr],opstat,&dbcommpt) == FALSE)
			{
				cmdtype = scdef.cmdtype[infoptr];
				if(qoc & 0x1)
					cmd_stat = scdef.optype2[infoptr];
				else
					cmd_stat = scdef.optype1[infoptr];
				CmdInterlockFailBroadcast(cmdtype,cmd_stat,&dbcommpt);

				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON SC\n",sProcRec[pnum].NameID,scdef.name[infoptr]);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}

			if(qoc & 0x80)		/*Select Command */
			{
				if(cmdbuf.selected==1)
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute SC command error: command is already selected\n");
						
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					return;
				}

				cmdbuf.cot=(unsigned char)ACTCON;
				cmdbuf.selected=1;
				cmdbuf.term_flag=0;
				cmdbuf.sel_tim=tim;
				cmdbuf.opstat= opstat;
				cmdbuf.infoaddr= infoaddr;

				OutputDspInfo(0, "=====Select SC command %02x addr= %04x received\n",qoc,infoaddr);
				return;
			}

			/*	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period)
					|| (opstat!=cmdbuf.opstat))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute SC command infoaddr:%d != Select infoaddr:%d or Timeout %d or selected op diff\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));
	
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					return;
				}
			}
			if(qoc & 0x1)
				cmd_stat = scdef.optype2[infoptr];
			else
				cmd_stat = scdef.optype1[infoptr];

			OutputDspInfo(0, "SC command addr=%04X QOC=%02X cmd_stat=%d received\n",infoaddr,qoc,cmd_stat);
	
			cmdtype = scdef.cmdtype[infoptr];
			ComdSendToLCU(cmdtype,cmd_stat,&dbcommpt);

			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;
			return;
		}
		else if((causetra & 0x3F) == DEACT)
		{
			cmdbuf.cot=(unsigned char)DEACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(0, "=====DEACT SC command addr=%04x received\n",infoaddr);
				return;
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(OUTPUT_ERROR, "Unknown cause %d of SC command addr=%04x received\n",causetra,infoaddr);
				return;
		}
		break;

	case C_DC_NA:	/*46 double point control command */

		qoc = data_buff[8];/*QOC:= S/E[8] QU[3..7] DCS[1..2] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif

		cmdbuf.typeId = type_id;
		cmdbuf.stat = qoc;
		cmdbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-dc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=dcdef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "%04x info address wrong! No this point in DC Control Table!\n",infoaddr);

				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}
			strcpy(dbname,dcdef.name[infoptr]);
			OutputDspInfo(0, "recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,qoc,causetra);
	
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "%s No this DC control point in the database file!\n",dbname);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}

			if((qoc & 0x3)==2)
				opstat = 1;
			else if((qoc & 0x3)==1)
				opstat = 0;
			else
			{
				OutputDspInfo(OUTPUT_WARN, "=====DC addr=%04x qoc=%d this operate is not allowed!\n",infoaddr,qoc);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}

			if(Get_Cmd_Ctrl_Priv(dcdef.interlockname[infoptr],opstat,&dbcommpt) == FALSE)
			{
				cmdtype = dcdef.cmdtype[infoptr];
				if((qoc & 0x3)==2)
					cmd_stat = dcdef.optype2[infoptr];
				else
					cmd_stat = dcdef.optype1[infoptr];
				CmdInterlockFailBroadcast(cmdtype,cmd_stat,&dbcommpt);

				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON DC\n",sProcRec[pnum].NameID,dcdef.name[infoptr]);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}

			if(qoc & 0x80)		/*Select Command */
			{
				if(cmdbuf.selected==1)
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute DC command error: command is already selected\n");
						
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					return;
				}

				cmdbuf.cot=(unsigned char)ACTCON;
				cmdbuf.selected=1;
				cmdbuf.term_flag=0;
				cmdbuf.sel_tim=tim;
				cmdbuf.opstat= opstat;
				cmdbuf.infoaddr= infoaddr;

				OutputDspInfo(0, "=====Select DC command %02x addr= %04x received\n",qoc,infoaddr);
				return;
			}

			/*	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period)
					|| (opstat!=cmdbuf.opstat))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute DC command infoaddr:%d != Select infoaddr:%d or Timeout %d or selected op diff\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));
	
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					return;
				}
			}
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;

			if((qoc & 0x3)==2)
				cmd_stat = dcdef.optype2[infoptr];
			else
				cmd_stat = dcdef.optype1[infoptr];

			OutputDspInfo(0, "DC command addr=%04X QOC=%02X cmd_stat=%d received\n",infoaddr,qoc,cmd_stat);
	
			cmdtype = dcdef.cmdtype[infoptr];
			ComdSendToLCU(cmdtype,cmd_stat,&dbcommpt);

			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;
			return;
		}
		else if((causetra & 0x3F) == DEACT)
		{
			cmdbuf.cot=(unsigned char)DEACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(0, "=====DEACT DC command addr=%04x received\n",infoaddr);
				return;
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(OUTPUT_WARN, "Unknown cause %d of DC command addr=%04x received\n",causetra,infoaddr);
				return;
		}
		break;

	case C_RC_NA:	/*47 Regulating Step Control Command */

		qoc = data_buff[8];/*QOC:= S/E[8] QU[3..7] RCS[1..2] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif

		cmdbuf.typeId = type_id;
		cmdbuf.stat = qoc;
		cmdbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-rc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=rcdef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "%04x info address wrong! base=%04x No this point in RC Control Table num=%d!\n",infoaddr,rc_cmd_base_addr,rcdef.number);

				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}
			strcpy(dbname,rcdef.name[infoptr]);

			OutputDspInfo(0, "recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,data_buff[8],causetra);
	
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "%s No this RC control point in the database file!\n",dbname);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}
			if((qoc & 0x3)==2)
				opstat = 1;
			else if((qoc & 0x3)==1)
				opstat = 0;
			else
			{
				OutputDspInfo(OUTPUT_WARN, "=====RC addr=%04x qoc=%d this operate is not allowed!\n",infoaddr,qoc);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}

			if(Get_Cmd_Ctrl_Priv(rcdef.interlockname[infoptr],opstat,&dbcommpt) == FALSE)
			{
				cmdtype = rcdef.cmdtype[infoptr];
				if((qoc & 0x3)==2)
					cmd_stat = rcdef.optype2[infoptr];
				else
					cmd_stat = rcdef.optype1[infoptr];
				CmdInterlockFailBroadcast(cmdtype,cmd_stat,&dbcommpt);

				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON RC\n",sProcRec[pnum].NameID,rcdef.name[infoptr]);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				return;
			}

			if(qoc & 0x80)		/*Select Command */
			{
				if(cmdbuf.selected==1)
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute RC command error: command is already selected\n");
						
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					return;
				}
				cmdbuf.cot=(unsigned char)ACTCON;
				cmdbuf.selected=1;
				cmdbuf.term_flag=0;
				cmdbuf.sel_tim=tim;
				cmdbuf.opstat= opstat;
				cmdbuf.infoaddr= infoaddr;

				OutputDspInfo(0, "=====Select RC command %02x addr= %04x received\n",qoc,infoaddr);
				return;
			}

			/*	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period)
					|| (opstat!=cmdbuf.opstat))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute RC command infoaddr:%d != Select infoaddr:%d or Timeout %d or selected op diff\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));
						cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					return;
				}
			}
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			if((qoc & 0x3)==2)
				cmd_stat = rcdef.optype2[infoptr];
			else
				cmd_stat = rcdef.optype1[infoptr];

			OutputDspInfo(0, "RC command addr=%04X QOC=%02X cmd_stat=%d received\n",infoaddr,qoc,cmd_stat);
	
			cmdtype = rcdef.cmdtype[infoptr];
			ComdSendToLCU(cmdtype,cmd_stat,&dbcommpt);

			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;

		}
		else if((causetra & 0x3F) == DEACT)
		{
			cmdbuf.cot=(unsigned char)DEACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(0, "=====DEACT RC command addr=%04x received\n",infoaddr);
	
			return;
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(OUTPUT_WARN, "Unknown cause %d of RC command addr=%04x received\n",causetra,infoaddr);
				return;
		}
		break;

	case C_SE_NA:	/*48 confirm of set-point command normalized value */

		qoc = data_buff[10];	/*QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&setvalue,&data_buff[8],2);	 /* value of set */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
		memcpy(&setvalue,&data_buff[8],2);	 /*value of set */
#endif
		sebuf.typeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.setvalue = setvalue;
		sebuf.qoc = qoc;
		sebuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_base_addr;
			if((infoptr<0)||(infoptr>=sedef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE_NA:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				return;
			}

			strcpy(dbname,sedef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NA:%s No this set point in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}

			if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
			{
					
				if(SEType == 0)
				{
					value=(float)((setvalue*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/32767.0
						 + sedef.lorange[infoptr]);
				}
				else
				{
					value=(float)(((setvalue+32768)*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
						 + sedef.lorange[infoptr]);
				}
			
				cmdtype = sedef.cmdtype[infoptr];
				SECmdInterlockFailBroadcast(cmdtype, value, &dbcommpt);
		
				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_NA\n",sProcRec[pnum].NameID,sedef.name[infoptr]);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}

			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NA:%s this point is not ANA in the database file!\n",dbname);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}
			if(qoc & 0x80)
			{
				OutputDspInfo(0, "Set-Point C_SE_NA: addr=%04x value = %d Received\n",infoaddr,setvalue);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				return;
			}
			OutputDspInfo(0, "Set-Point C_SE_NA: addr=%04x value = %d Received\n",infoaddr,setvalue);
			
            
			if(SEType == 0)
			{
				value=(float)((setvalue*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/32767.0
					 + sedef.lorange[infoptr]);
			}
			else
			{
				value=(float)(((setvalue+32768)*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
					 + sedef.lorange[infoptr]);
			}
			
			OutputDspInfo(0, "\n#####Set-Point C_SE_NA: value = %f Received\n",value);
			cmdtype = sedef.cmdtype[infoptr];
			SEComdSendToLCU(cmdtype, value, &dbcommpt);

			sebuf.term_flag = 1;
			sebuf.cot=(unsigned char)ACTCON;
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
				OutputDspInfo(OUTPUT_WARN, "iec101s:can't open(wb) the file %s!\n",cmdrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %04x\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value,infoaddr,setvalue);
				fclose(fagc);
			}
#endif
			return;
		}
		else if((causetra & 0x3F) == DEACT)
		{
			OutputDspInfo(0, "=====DEACT Cause of C_SE_NA: addr=%04x value = %d Received\n",infoaddr,setvalue);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			return;
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_NA: received %d wrong!!!\n",causetra);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			return;
		}
		break;

	case C_SE_NB:	/*49 confirm of set-point command scaled value */

		qoc = data_buff[10];	/*QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&setvalue,&data_buff[8],2);	 /* value of set */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
		memcpy(&setvalue,&data_buff[8],2);	 /*value of set */
#endif

		sebuf.typeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.setvalue = setvalue;
		sebuf.qoc = qoc;
		sebuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_base_addr;
			if((infoptr<0)||(infoptr>=sedef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE_NB:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				return;
			}

			strcpy(dbname,sedef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NB:%s No this set point in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}

			if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
			{
				value= setvalue * sedef.scale[infoptr];
				cmdtype = sedef.cmdtype[infoptr];
				SECmdInterlockFailBroadcast(cmdtype, value, &dbcommpt);

				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_NB\n",sProcRec[pnum].NameID,sedef.name[infoptr]);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}

			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NB:%s this point is not ANA in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}
			if(qoc & 0x80)
			{
				OutputDspInfo(0, "?????Select C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				return;
			}

			OutputDspInfo(0, "#####Set-Point C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
			

			value= setvalue * sedef.scale[infoptr];

			cmdtype = sedef.cmdtype[infoptr];
			SEComdSendToLCU(cmdtype, value, &dbcommpt);

			sebuf.term_flag = 1;
			sebuf.cot=(unsigned char)ACTCON;
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
				OutputDspInfo(OUTPUT_WARN, "iec101s:can't open(wb) the file %s!\n",cmdrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %04x\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value,infoaddr,setvalue);
				fclose(fagc);
			}
#endif
			return;
		}
		else if((causetra & 0x3F) == DEACT)
		{
			OutputDspInfo(0, "=====DEACT Cause of C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			return;
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_NB: received %d wrong!!!\n",causetra);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			return;
		}
		break;

	case C_SE_NC:	/*50 confirm of set-point command float value */

		qoc = data_buff[12];	/*QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&value,&data_buff[8],4);	 /* value of set */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
		memcpy(&value,&data_buff[8],4);	 /*value of float */
#endif

		sebuf.typeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.fvalue = value;
		sebuf.qoc = qoc;
		sebuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_base_addr;
			if((infoptr<0)||(infoptr>=sedef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE_NC:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				return;
			}

			strcpy(dbname,sedef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NC:%s No this set point in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}

			if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
			{
				cmdtype = sedef.cmdtype[infoptr];
				SECmdInterlockFailBroadcast(cmdtype, value, &dbcommpt);

				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_NC\n",sProcRec[pnum].NameID,sedef.name[infoptr]);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}

			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				OutputDspInfo(OUTPUT_WARN, "C_SE_NC:%s this point is not ANA in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				return;
			}
			if(qoc & 0x80)
			{
				OutputDspInfo(0, "?????Select C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				return;
			}

			OutputDspInfo(0, "#####Set-Point C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);

			cmdtype = sedef.cmdtype[infoptr];
			SEComdSendToLCU(cmdtype, value, &dbcommpt);

			sebuf.term_flag = 1;
			sebuf.cot=(unsigned char)ACTCON;

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
				OutputDspInfo(OUTPUT_WARN, "iec101s:can't open(wb) the file %s!\n",cmdrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %04x\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value,infoaddr,value);
				fclose(fagc);
			}
#endif
			return;
		}
		else if((causetra & 0x3F) == DEACT)
		{
			OutputDspInfo(0, "=====DEACT Cause of C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			return;
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_NC: received %d wrong!!!\n",causetra);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			return;
		}
		break;

	case C_SE_ND:	/*136 confirm of multi set-point command normalized value */

		infonum = varstru & 0x7F;
		qoc = data_buff[6+infonum*4];	/*QOC:= S/E[8] QL[1..7] */

		sebuf.typeId = type_id;
		sebuf.infonum = infonum;
		sebuf.rcvpnt = recvdatapt;
		sebuf.qoc = qoc;
		sebuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			if(qoc & 0x80)
			{
#ifdef SUN_OS
				CrossCopy(&infoaddr,&data_buff[6],2);	 /*address of object */
#else
				memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif
				OutputDspInfo(0, "????Select? C_SE_ND: infoaddr= %04x infonum=%d Received\n",infoaddr,infonum);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				return;
			}

			for(objpnt=0;objpnt<infonum;objpnt++)
			{
#ifdef SUN_OS
				CrossCopy(&infoaddr,&data_buff[6+objpnt*4],2);	 /*address of object */
				CrossCopy(&setvalue,&data_buff[8+objpnt*4],2);	 /*value of set */
#else
				memcpy(&infoaddr,&data_buff[6+objpnt*4],2);	 /*address of object */
				memcpy(&setvalue,&data_buff[8+objpnt*4],2);	 /*value of set */
#endif

				infoptr = infoaddr - se_cmd_base_addr;
				if((infoptr<0)||(infoptr>=sedef.number))
				{
					OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE_ND:%04x info address wrong! No this point in SET Point Control Table!\n",infoaddr);
		
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)(0x40|UKINFOAD);
					return;
				}

				if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
				{
					OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_ND\n",sProcRec[pnum].NameID,sedef.name[infoptr]);
		
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)NACTCON;
					return;
				}

				OutputDspInfo(0, "#####Multi Set-Point C_SE_ND: addr=%04x value= %d Received\n",infoaddr,setvalue);

				if(SEType == 0)
				{
					value=(float)((setvalue*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/32767.0
						 + sedef.lorange[infoptr]);
				}
				else
				{
					value=(float)(((setvalue+32768)*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
						 + sedef.lorange[infoptr]);
				}
				strcpy(dbname,sedef.name[infoptr]);
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(OUTPUT_ERROR, "C_SE_ND:%s No this set point in the database file!\n",dbname);
					continue;
				}
				if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
				{
					OutputDspInfo(OUTPUT_WARN, "C_SE_ND:%s this point is not ANA in the database file!\n",dbname);
					continue;
				}

				cmdtype = sedef.cmdtype[infoptr];
				SEComdSendToLCU(cmdtype, value, &dbcommpt);
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
					OutputDspInfo(OUTPUT_WARN, "iec101s:can't open(wb) the file %s!\n",cmdrecfile);
				}
				else
				{
					fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %04x\n",tmpt.tm_year+1900,
						tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
						tmpt.tm_sec,value,infoaddr,setvalue);
					fclose(fagc);
				}
#endif
			}
			sebuf.term_flag = 1;
			sebuf.cot=(unsigned char)ACTCON;
			return;
		}
		else if((causetra & 0x3F) == DEACT)
		{
#ifdef SUN_OS
			CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
			memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif
			OutputDspInfo(0, "=====DEACT Cause of C_SE_ND: infoaddr= %04x infonum=%d Received\n",infoaddr,infonum);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			return;
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_ND command received %d wrong!!!\n",causetra);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			return;
		}
		break;

	case M_ME_NC:	/*13 float value (set-point command) */
	case M_ME_TC:	/*measure value with time tag:14 */
	case M_ME_TF:	/* 36:measure value with time CP56Time2a,float value*/

		infonum = varstru & 0x7F;		/*number of information object */
		if(varstru & 0x80)	/*SQ=1 */
		{
			if(type_id == M_ME_NC)
			{
				lengthfactor = 5;
			}
			else if(type_id == M_ME_TC)
				lengthfactor = 8;
			else
				lengthfactor = 12;
			datalength = lengthfactor*infonum+8;
			if(datalength != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				return;
			}

			lanpt = 0;
#ifdef SUN_OS
			CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
			memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif

			infoptr = infoaddr - se_cmd_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+8;

				if((infoptr<0)||(infoptr >= sedef.number))
				{
					OutputDspInfo(OUTPUT_ERROR, "M_ME_NC SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,sedef.number);
					return;
				}
				strcpy(dbname,sedef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(OUTPUT_ERROR, "\nIEC No this point name=%s in db\n",dbname);
		
					infoptr ++;
					continue;
				}
				qoc = data_buff[datapoint+4];

#ifdef SUN_OS
				CrossCopy(&value,&data_buff[datapoint],4);
#else
				memcpy(&value,&data_buff[datapoint],4);
#endif

				infoptr ++;

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x30)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				if(qoc & 0xC0)
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
#ifdef SUN_OS
				CrossCopy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				CrossCopy(&me_buf[lanpt*22+8], &anadata.status,2);
				CrossCopy(&me_buf[lanpt*22+10], &anadata.data,4);
#else
				memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
				memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
#endif
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				if(type_id ==M_ME_NC)
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
#ifdef SUN_OS
					CrossCopy(&ms,&data_buff[datapoint+5],2);
#else
					memcpy(&ms,&data_buff[datapoint+5],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=data_buff[datapoint+7] & 0x3F;	/* minute */
					if(type_id ==M_ME_TC)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TF */
					{
						me_buf[lanpt*22+17]=data_buff[datapoint+8] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=data_buff[datapoint+9] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=data_buff[datapoint+10] & 0x0F;	/* month */
						year=data_buff[datapoint+11] & 0x7F+2000;		/* year */
					}
				}
#ifdef SUN_OS
				CrossCopy(&me_buf[lanpt*22+20],&year,2);
#else
				memcpy(&me_buf[lanpt*22+20],&year,2);
#endif
				lanpt ++;
				if(lanpt>=64)
				{
					lanhead.length = lanpt*22;
#ifdef SUN_OS
					Swap2Byte(&lanhead.length);
#endif
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
/*					sleep(1); */
				}
			}
		}
		else		/*SQ=0 */
		{
			if(type_id == M_ME_NC)
			{
				lengthfactor = 7;
			}
			else if(type_id == M_ME_TC)
				lengthfactor = 10;
			else
				lengthfactor = 14;
			datalength = lengthfactor*infonum+6;
			if(datalength != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				return;
			}

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;

#ifdef SUN_OS
				CrossCopy(&infoaddr,&data_buff[datapoint],2);	 /*address of object */
#else
				memcpy(&infoaddr,&data_buff[datapoint],2);	 /*address of object */
#endif

				infoptr = infoaddr - se_cmd_base_addr;
				if((infoptr<0)||(infoptr > sedef.number))
				{
					OutputDspInfo(OUTPUT_ERROR, "M_ME_NC SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,sedef.number);
					return;
				}
				strcpy(dbname,sedef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(OUTPUT_ERROR, "\nIEC No this point name=%s in db\n",dbname);
							continue;
				}
				qoc = data_buff[datapoint+6];

#ifdef SUN_OS
				CrossCopy(&value,&data_buff[datapoint+2],4);
#else
				memcpy(&value,&data_buff[datapoint+2],4);
#endif

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x30)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				if(qoc & 0xC0)
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
#ifdef SUN_OS
				CrossCopy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				CrossCopy(&me_buf[lanpt*22+8], &anadata.status,2);
				CrossCopy(&me_buf[lanpt*22+10], &anadata.data,4);
#else
				memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
				memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
#endif
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				if(type_id ==M_ME_NC)
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
#ifdef SUN_OS
					CrossCopy(&ms,&data_buff[datapoint+8],2);
#else
					memcpy(&ms,&data_buff[datapoint+8],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=data_buff[datapoint+10] & 0x3F;	/* minute */
					if(type_id ==M_ME_TC)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TF */
					{
						me_buf[lanpt*22+17]=data_buff[datapoint+11] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=data_buff[datapoint+12] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=data_buff[datapoint+13] & 0x0F;	/* month */
						year=data_buff[datapoint+14] & 0x7F+2000;		/* year */
					}
				}
#ifdef SUN_OS
				CrossCopy(&me_buf[lanpt*22+20],&year,2);
#else
				memcpy(&me_buf[lanpt*22+20],&year,2);
#endif
				lanpt ++;
				if(lanpt>=64)
				{
					lanhead.length = lanpt*22;
#ifdef SUN_OS
					Swap2Byte(&lanhead.length);
#endif
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
/*					sleep(1); */
				}
			}
		}
		if(lanpt>0)
		{
			lanhead.length = lanpt*22;
#ifdef SUN_OS
			Swap2Byte(&lanhead.length);
#endif
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
		return;
		break;

	case P_ME_NA:	/*110 confirm of Parameter-value command */
					
/*		infonum = varstru & 0x7F; */
		qoc = data_buff[10];	/*QPM:= POP[8]:1 LPC[7] KPA[1..6] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&setvalue,&data_buff[8],2);	 /*value of set */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
		memcpy(&setvalue,&data_buff[8],2);	 /*value of set */
#endif

		pmbuf.typeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.setvalue = setvalue;
		pmbuf.qpm = qoc;	/*C dir POP=1 M dir LPC=1 POP=0 */
		pmbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr;
			if((infoptr<0)||(infoptr>=medef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!P_ME_NA:%04x info address wrong! No this point in ME Table!\n",infoaddr);

				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				return;
			}

			if(MeType == 0)
			{
				value=(float)((setvalue*(medef.hirange[infoptr]-medef.lorange[infoptr]))/32767.0
					 + medef.lorange[infoptr]);
			}
			else
			{
				value=(float)(((setvalue+32768)*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
					 + sedef.lorange[infoptr]);
			}
			OutputDspInfo(0, "##### P_ME_NA: addr=%04X setvalue=%04X value=%f qpm=%02X Received\n",infoaddr,setvalue,value,qoc);
			

			switch(qoc & 0x3F)
			{
			case	1:

				medef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */

				OutputDspInfo(0, "Parameter threshold command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */

				OutputDspInfo(0, "Parameter smothing factor command P_ME_NA: addr=%04x value = %04x %f Received\n",infoaddr,setvalue,value);
				break;

			case	3:

/*				medef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */

				OutputDspInfo(0, "Parameter low limit command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
				break;

			case	4:

/*				medef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */
				OutputDspInfo(0, "Parameter high limit command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				OutputDspInfo(0, "Parameter reserved command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
				break;
			}/*switch(qoc & 0x3F) */
			return;
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of P_ME_NA command received %d wrong!!!\n",causetra);
			
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			return;
		}
		break;

	case P_ME_NB:	/*111 confirm of Parameter-value command */
					
/*		infonum = varstru & 0x7F; */
		qoc = data_buff[10];	/*QPM:= POP[8] LPC[7] KPA[1..6] */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&setvalue,&data_buff[8],2);	 /*value of set */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
		memcpy(&setvalue,&data_buff[8],2);	 /*value of set */
#endif

		pmbuf.typeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.setvalue = setvalue;
		pmbuf.qpm = qoc;			/*C dir POP=1 M dir LPC=1 POP=0 */
		pmbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr;
			if((infoptr<0)||(infoptr>=medef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!P_ME_NB:%04x info address wrong! No this point in ME Table!\n",infoaddr);

				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				return;
			}
			value= setvalue * sedef.scale[infoptr];
#ifdef DEBUG
			OutputDspInfo(0, "##### P_ME_NB: addr=%04X setvalue=%04X value=%f qpm=%02X Received\n",infoaddr,setvalue,value,qoc);
#endif
			switch(qoc & 0x3F)
			{
			case	1:

				medef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */
				OutputDspInfo(0, "Parameter threshold command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
				OutputDspInfo(0, "Parameter smothing factor command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
				break;

			case	3:

/*				medef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */
				OutputDspInfo(0, "Parameter low limit command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
				break;

			case	4:

/*				medef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */
				OutputDspInfo(0, "Parameter high limit command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				OutputDspInfo(0, "Parameter reserved command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
				break;
			}/*switch(qoc & 0x3F) */
			return;
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of P_ME_NB command received %d wrong!!!\n",causetra);
			
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			return;
		}
		break;

	case P_ME_NC:	/*112 confirm of Parameter-value command */
					
/*		infonum = varstru & 0x7F; */
		qoc = data_buff[12];	/*QPM:= POP[8] LPC[7] KPA[1..6] */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&value,&data_buff[8],4);	 /*value of set */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
		memcpy(&value,&data_buff[8],4);	 /*value of set */
#endif

		pmbuf.typeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.fvalue = value;
		pmbuf.qpm = qoc;			/*C dir POP=1 M dir LPC=1 POP=0 */
		pmbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr;
			if((infoptr<0)||(infoptr>=medef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!P_ME_NC:%04x info address wrong! No this point in ME Table!\n",infoaddr);

				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				return;
			}
#ifdef DEBUG
			OutputDspInfo(0, "##### P_ME_NC: addr=%04x value=%f qpm=%02X Received\n",infoaddr,value,qoc);
#endif
			switch(qoc & 0x3F)
			{
			case	1:

				medef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */

				OutputDspInfo(0, "Parameter threshold command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */

				OutputDspInfo(0, "Parameter smothing factor command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
				break;

			case	3:

/*				medef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */

				OutputDspInfo(0, "Parameter low limit command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
				break;

			case	4:

/*				medef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/*C dir POP=1 M dir LPC=1 POP=0 */
/*				SaveMETable(MEDEF_NORM); */

				OutputDspInfo(0, "Parameter high limit command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;

				OutputDspInfo(0, "Parameter reserved command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
				break;
			}/*switch(qoc & 0x3F) */
			return;
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of P_ME_NC command received %d wrong!!!\n",causetra);
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			return;
		}
		break;

	case C_IC_NA:	/*100 confirm of general interrogation command */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif
		qoc = data_buff[8];	/*QOI[1..8]:= 20 general interrogation */
												/*21: group1... 34:group14...36:group16 */
		icbuf.typeId = type_id;
		icbuf.qoi = qoc;
		icbuf.ack_flag = 1;

		if(infoaddr!=0)
		{
			icbuf.cot = (unsigned char)(0x40|UKINFOAD);
			return;
		}
		switch(causetra & 0x3F)
		{
		case ACT:		/*general interrogation command act confirm */

			if(qoc==INTROGEN)
			{
				OutputDspInfo(0, "IEC general interrogation command is received!\n");
				
				interrogationflag = 1;
				acd = 1;
				scanflag = 0;
				scan_tim = tim;
				m_cRespGenInterroGrpNum = 0;
				icbuf.cot = (unsigned char)ACTCON;
				return;
			}
// 			else if(qoc>=INTRO1 || qoc<=INTRO16)
// 			{
// 				/*interrogation group */
// 				OutputDspInfo(0, "IEC interrogation group %d command is received!\n",qoc-INTROGEN);
// 				icgroupflag= 1;
// 				acd = 1;
// 				scanflag = 0;
// 				scan_tim = tim;
// 				m_cRespICGrpNum = 0;
// 				icbuf.cot = (unsigned char)ACTCON;
// 				return;
// 			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "IEC interrogation command qoi %d error\n",qoc);
				icbuf.cot = (unsigned char)NACTCON;
				return;
			}
			break;

		case DEACT:		/*general interrogation command deact confirm */

			OutputDspInfo(0, "IEC general interrogation %x DEACT command is received!\n",qoc);
			icbuf.cot = (unsigned char)DEACTCON;
			if(qoc==INTROGEN)
				interrogationflag = 0;
			else
				icgroupflag = 0;

			break;

		default:
			OutputDspInfo(OUTPUT_WARN, "C_IC_NA: Received cause of transmission=%2X qoi=%dwrong!\n",causetra,qoc);
	
			icbuf.cot = (unsigned char)(0x40|UKCOT);
			break;
		}/*switch(causetra & 0x3F) */
		break;

	case C_CI_NA:	/*101 confirm of counter interrogation command */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif
		qoc = data_buff[8];		/*QCC[1..6]:= 5: general 1: group1... 4:group4 */
													/*FRZ[7..8]:= 0:read 1:freeze 2:freeze and reset 3:reset */
		cibuf.typeId = type_id;
		cibuf.qcc = qoc;
		cibuf.ack_flag = 1;

		if(infoaddr!=0)
		{
			cibuf.cot = (unsigned char)(0x40|UKINFOAD);
			return;
		}
		switch(causetra & 0x3F)
		{
		case ACT:		/*counter interrogation command act confirm */

			frz = (qoc >> 6) &0x3;

			if(frz==0)
			{
				if(qoc==5)
				{
					counterflag = 1;
					acd = 1;
					m_cRespCountGrpNum=0;

					OutputDspInfo(0, "Counter interrogation %02X Read command is received!\n",qoc);
#ifdef	DEBUG
	#endif
	
					cibuf.cot = (unsigned char)ACTCON;
				}
				else	/*counter interrogation group */
				{
					if((qoc<1)||(qoc>4))
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d Read command is received!\n",qoc,itdef.number);
#ifdef	DEBUG
		#endif
								cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						return;
					}
					if(((qoc-1)*CI_GROUP_NUM) >= itdef.number)
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d Read command is received!\n",qoc,itdef.number);
#ifdef	DEBUG
		#endif
								cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						return;
					}
					OutputDspInfo(0, "Counter group %02X Read command is received!\n",qoc);
#ifdef	DEBUG
	#endif
						cibuf.cot = (unsigned char)ACTCON;
					acd = 1;
					cigroupflag = 1;
					m_cRespCIGrpNum = (qoc-1)*CI_GROUP_NUM/IT_GROUP_NUM;
/*					CounterInterrogationAck((unsigned char)ACTCON,cibuf.qcc); */
				}
			}
			else
			{
				if((qoc&0x3F)==5)
				{
					OutputDspInfo(0, "Counter interrogation %02X FRZ command is received!\n",qoc);
#ifdef	DEBUG
	#endif
						cibuf.cot = (unsigned char)ACTCON;
				}
				else	/*counter interrogation group */
				{
					if(((qoc&0x3F)<1)||((qoc&0x3F)>4))
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d FRZ command is received!\n",(qoc&0x3F),itdef.number);
#ifdef	DEBUG
		#endif
								cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						return;
					}
					if((((qoc&0x3F)-1)*CI_GROUP_NUM) >= itdef.number)
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d FRZ command is received!\n",(qoc&0x3F),itdef.number);
#ifdef	DEBUG
		#endif
								cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						return;
					}
					OutputDspInfo(0, "Counter group %02X FRZ command is received!\n",qoc);
#ifdef	DEBUG
	#endif
						cibuf.cot = (unsigned char)ACTCON;
				}
			}
			break;

		case DEACT:		/*counter interrogation command deact */

			if((qoc&0x3F)==5)
			{
				counterflag = 0;
				OutputDspInfo(0, "IEC counter interrogation %x DEACT command is received!\n",qoc);
#ifdef	DEBUG
#endif
				cibuf.cot = (unsigned char)DEACTCON;
			}
			else	/*counter interrogation group */
			{
				OutputDspInfo(0, "IEC interrogation group %x DEACT command is received!\n",qoc);
#ifdef	DEBUG
#endif
				cibuf.cot = (unsigned char)DEACTCON;
				cigroupflag = 0;
			}
			break;

		default :
			OutputDspInfo(OUTPUT_WARN, "C_CI_NA: Received cause of transmission=%2x qcc=%x wrong!\n",causetra,qoc);
			
			cibuf.cot = (unsigned char)(0x40|UKCOT);
			break;
		}/*switch(causetra & 0x3F) */
					
		break;

	case C_RD_NA:	/*102 confirm of read command */
				
#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
#endif
		rdbuf.typeId = type_id;
		rdbuf.infoaddr = infoaddr;
		rdbuf.ack_flag = 1;

		if((causetra & 0x3F)==REQ)	/*read command req confirm */
		{
			OutputDspInfo(0, "IEC read command C_RD_NA infoaddr=%d is received!\n",infoaddr);
#ifdef	DEBUG
#endif
			
			rdbuf.cot = REQ;
		}
		else
		{
			OutputDspInfo(OUTPUT_ERROR, "C_RD_NA: Received cause of transmission=%2x wrong!\n",causetra);
			
			rdbuf.cot = (unsigned char)(0x40|UKCOT);
		}
		break;

	case C_CS_NA:	/*103 confirm of clock synchronization command */
		OutputDspInfo(0, "clock synchronization command frame C_CS_NA received!\n");

		GetLocalTime(&old_local_tm);		
		csbuf.typeId = type_id;
		csbuf.ack_flag =1;			
		memcpy(&infoaddr,&data_buff[6],2);	 //address of object
		Swap2Byte(&infoaddr);
		if(infoaddr!=0)
		{
			csbuf.cot = (unsigned char)(0x40|UKINFOAD);
			return;
		}

		if((causetra & 0x3F) == ACT)
		{
			struct tm	remoteTime;
			time_t	stTim;
			
			memcpy(&ms, &data_buff[8],2);
			Swap2Byte(&ms);
			remoteTime.tm_sec = ms/1000;
			remoteTime.tm_min = data_buff[10] & 0x3f;
			remoteTime.tm_hour = data_buff[11] & 0x1f;
			remoteTime.tm_mday = data_buff[12] & 0x1f;
			remoteTime.tm_mon = (data_buff[13] & 0x0f) - 1;
			remoteTime.tm_year =  (data_buff[14] & 0x7f) + 100;
			remoteTime.tm_isdst = 0;
			OutputDspInfo(0, "clock synchronization: remote clock %04d/%02d/%02d %02d:%02d:%02d %03d!\n",
				remoteTime.tm_year+1900,remoteTime.tm_mon+1,
				remoteTime.tm_mday,remoteTime.tm_hour,remoteTime.tm_min,
				remoteTime.tm_sec,remoteTime.tm_sec);


			if(remoteTime.tm_sec>=60 || remoteTime.tm_min>=60
				|| remoteTime.tm_hour>=24 || remoteTime.tm_mday==0
				|| remoteTime.tm_mon>=12)
			{
				OutputDspInfo(OUTPUT_ERROR, "clock synchronization: remote time error!\n");
				csbuf.cot = (unsigned char)NACTCON;
				return;
			}

			if((stTim = mktime(&remoteTime)) != (time_t)-1 )
			{
				SYSTEMTIME		sys_tm;
				time_t			ltTim;
				struct tm		sTimeStart;
				
				GetLocalTime(&sys_tm);
				ms = sys_tm.wMilliseconds + cd_delay_time;
				stTim = stTim + ms/1000;
				remoteTime = *localtime(&stTim);
				
				sys_tm.wMilliseconds = ms%1000;
				sys_tm.wSecond = remoteTime.tm_sec;
				sys_tm.wMinute = remoteTime.tm_min;
				sys_tm.wHour = remoteTime.tm_hour;
				sys_tm.wDay = remoteTime.tm_mday;
				sys_tm.wMonth = remoteTime.tm_mon + 1;
				sys_tm.wYear = remoteTime.tm_year + 1900;
				sys_tm.wDayOfWeek = remoteTime.tm_wday + 1;
				if(!Gps)
				{
					SetLocalTime(&sys_tm);
					csbuf.cot = (unsigned char)ACTCON;
				}
				else
				{
					csbuf.cot = (unsigned char)NACTCON;
				}
				
				diffclk = (stTim - tim) * 1000 + sys_tm.wMilliseconds - old_local_tm.wMilliseconds + cd_delay_time;
				if(diffclk < 0)
					diffclk = -diffclk;
				if(diffclk > delayalarm)
				{
					ClkDiffflag = 1;
					OutputDspInfo(OUTPUT_WARN, "clock different between local and remote %d > %d\n",diffclk,delayalarm);
				}
				
				ltTim = tim - cd_delay_time/1000;
				if(old_local_tm.wMilliseconds >= (cd_delay_time%1000))
				{
					old_local_tm.wMilliseconds -= (cd_delay_time%1000);
				}
				else
				{
					old_local_tm.wMilliseconds = old_local_tm.wMilliseconds + 1000 - (cd_delay_time%1000);
					ltTim--;
				}
				sTimeStart = *localtime(&ltTim);
				
				old_local_tm.wSecond = sTimeStart.tm_sec;
				old_local_tm.wMinute = sTimeStart.tm_min;
				old_local_tm.wHour = sTimeStart.tm_hour;
				old_local_tm.wDay = sTimeStart.tm_mday;
				old_local_tm.wMonth = sTimeStart.tm_mon + 1;
				old_local_tm.wYear = sTimeStart.tm_year + 1900;
				old_local_tm.wDayOfWeek = sTimeStart.tm_wday + 1;
				return;
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "clock synchronization: remote time error!\n");
				csbuf.cot = (unsigned char)NACTCON;
				return;
			}
		}	
		else
			csbuf.cot = (unsigned char)(0x40|UKCOT);;

		break;

	case C_TS_NA:	/*test command 104 */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&setvalue,&data_buff[8],2);	 /* FBP:AA55 Fix test word */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /*address of object */
		memcpy(&setvalue,&data_buff[8],2);	 /*FBP:AA55 Fix test word */
#endif

		OutputDspInfo(0, "test request frame C_TS_NA received!\n");

		testbuf.typeId = type_id;
		testbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			if((setvalue==0x55AA)&&(infoaddr==0))
			{
				testbuf.cot = ACTCON;
			}
			else
			{
				if(infoaddr!=0)
					testbuf.cot = (unsigned char)(0x40|UKINFOAD);
				else
					testbuf.cot = NACTCON;
			}
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "test command cause of transmit %x wrong!\n",causetra);
			testbuf.cot = (unsigned char)(0x40|UKCOT);
		}
		break;

	case C_RP_NA:	/*Reset process command 105 */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /* address of object */
#endif
		qoc = data_buff[8];	/*QRP[1..8]:= 1: general reset 2: SOE buffer reset */

		rpbuf.typeId = type_id;
		rpbuf.qrp = qoc;
		rpbuf.ack_flag = 1;

		if((causetra & 0x3F) == ACT)
		{
			if(infoaddr!=0)
			{
				testbuf.cot = (unsigned char)(0x40|UKINFOAD);
				return;
			}
			if(qoc==1)
			{
				OutputDspInfo(0, "reset process command QRP=1 general process reset frame!\n");
				ResetProcessflag = 1; 
				rpbuf.cot = ACTCON;
			}
			else if(qoc==2)
			{
				OutputDspInfo(0, "reset process command QRP=2 SOE buffer reset frame!\n");
				rpbuf.cot = ACTCON;
				InitSoeBuf();
			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "reset process command QRP=%d frame?\n", qoc);
				rpbuf.cot = NACTCON;
			}
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "reset process command QRP=%d cause of transmit %x wrong!\n",qoc,causetra);
			rpbuf.cot = (unsigned char)(0x40|UKCOT);;
		}
		break;

	case C_CD_NA:	/*Delay Acquire command 106 */

		cdbuf.typeId = type_id;
		cdbuf.ack_flag = 1;

#ifdef SUN_OS
		CrossCopy(&infoaddr,&data_buff[6],2);	/* address of object */
		CrossCopy(&cdbuf.cp16time,&data_buff[8],2);	 /* CP16time2a:ms to second */
#else
		memcpy(&infoaddr,&data_buff[6],2);	 /* address of object */
		memcpy(&cdbuf.cp16time,&data_buff[8],2);	 /* CP16time2a:ms to second */
#endif

		OutputDspInfo(0, "Dealy Acquire command frame C_CD_NA received!\n");

		if((causetra & 0x3F) == ACT)	/*Delay acquisition cmd */
		{
			if(infoaddr!=0)
			{
				cdbuf.cot = (unsigned char)(0x40|UKINFOAD);
			}
			else
			{
				DATE_TIME	d_time; 
				time_t		sec;

				cdbuf.cot = (unsigned char)ACTCON;
				GetSysTimeEx(&d_time, &sec);
				cdbuf.recv_time_sec= sec;
				cdbuf.recv_time_ms= d_time.ms;
			}

			OutputDspInfo(0, "Delay acquisition cmd cp16time=%d\n",cdbuf.cp16time);
		}
		else if((causetra & 0x3F) == SPONT)	/*Delay Transmit */
		{
			if(infoaddr!=0)
			{
				cdbuf.cot = (unsigned char)(0x40|UKINFOAD);
			}
			else
			{
				cdbuf.ack_flag = 0;
				cdbuf.cot = (unsigned char)ACTCON;
				cd_delay_time = cdbuf.cp16time;
			}

			OutputDspInfo(0, "Delay Transmit cd_delay_time=%d\n", cd_delay_time);
		}
		else
			cdbuf.cot = (unsigned char)NACTCON;

		break;

	default :
		mirbuf.cot=(unsigned char)(0x40|UKTYPE);
		mirbuf.pnt=recvdatapt;
		MirrorNAckflag = 1;

		OutputDspInfo(OUTPUT_WARN, "IEC type_id = %u is unavailable at this time!\n",type_id);
		break;
	}/*switch(type_id) */
}


void InitSoeBuf(void)
{
	soebuf.sendpnt = soebuf.bufpnt = soebuf.count = soebuf.ackpnt= 0;
	memset(soebuf.soerec, 0, sizeof(soebuf.soerec));
}


void IncreaseSoeHead(unsigned short soe_head)
{
	soe_head++;
	if(soe_head >= SOE_NUM)
		soe_head = 0;
	soebuf.bufpnt = soe_head;
	
	if(soebuf.count<SOE_NUM)
		soebuf.count++;
}

void *	IEC101_SOE(void *arg)
{
#ifdef BY_RECORD

	SHM_RECORD	*shm_rec_addr;
	IND_REC		*pInd_alm;
	unsigned char	type_id;
	short		rcv_end,rcv_pnt;
	POINTER		rcv_ptr;

#else
	int		data_type;
	unsigned char	name_buf[NAME_SIZE];
	DMS_COMMON	dbentry;
#endif
	POINTER		ptr;
	IND_DATA	dbstat;
	unsigned char	stat;
 	unsigned char	vti;
	unsigned short	pntnum,soe_head;

	OutputDspInfo(0, "------------IEC101_SOE thread start!!!-----------\n");
	
	InitSoeBuf();

#ifndef BY_RECORD
	for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Update orgbuf for SP status Change */
	{
		if(spdef.ptr[pntnum].dev_id == 0)
		{
			orgspsoebuf[pntnum] = 0;
			continue;
		}
		ptr = spdef.ptr[pntnum];
		if(GetSPStatById(&ptr,&stat)==-1)
		{
			orgspsoebuf[pntnum] = 0;
			continue;
		}
		else
		{
			orgspsoebuf[pntnum] = stat;
		}
	}
	for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* update orgbuf for DP status */
	{
		if(dpdef.ptr[pntnum].dev_id == 0)
		{
			orgdpsoebuf[pntnum] = 0;
			continue;
		}
		ptr = dpdef.ptr[pntnum];
		if(GetDPStatById(&ptr,&stat)==-1)
		{
			orgdpsoebuf[pntnum] = 0;
			continue;
		}
		else
		{
			orgdpsoebuf[pntnum] = stat;
		}
	}
	for(pntnum=0;pntnum<stdef.number;pntnum++)		/* update orgbuf for st status */
	{
		if(stdef.ptr[pntnum].dev_id == 0)
		{
			orgstsoebuf[pntnum] = 0;
			orgstvsoebuf[pntnum] = 0;
			continue;
		}
		ptr = stdef.ptr[pntnum];
		if(GetSTStatById(&ptr, &vti, &stat)==-1)
		{
			orgstsoebuf[pntnum] = 0;
			orgstvsoebuf[pntnum] = 0;
			continue;
		}
		else
		{
			orgstsoebuf[pntnum] = stat;
			orgstvsoebuf[pntnum] = vti;
		}
	}
/* Check for SOE buffer */
	for(;;)
	{
		Sleep(1000);
		for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Check for SP status Change */
		{
			if((spdef.ptr[pntnum].dev_id == 0)||(spdef.TimeTag[pntnum] == 0))
			{
				continue;
			}
			ptr = spdef.ptr[pntnum];
			if(GetSPStatById(&ptr,&stat)==-1)
			{
				continue;
			}

			if(stat != orgspsoebuf[pntnum])
			{
				soe_head = soebuf.bufpnt;
 				if(spdef.TimeTag[pntnum] == 1)
					soebuf.soerec[soe_head].typeId = M_SP_TA;
				else if(spdef.TimeTag[pntnum] == 2)
					soebuf.soerec[soe_head].typeId = M_SP_TB;
				else
					continue;

				soebuf.soerec[soe_head].infoaddr = sp_base_addr + pntnum;
				soebuf.soerec[soe_head].stat = orgspsoebuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 SPI */
				soebuf.soerec[soe_head].time = dbentry.entry.ind.variable.date_time;
				soebuf.soerec[soe_head].time.year = soebuf.soerec[soe_head].time.year-1900;

				OutputDspInfo(0, "soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						name_buf,soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);

				IncreaseSoeHead(soe_head);
			}
		}
		for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* Check for DP status Change */
		{
			if((dpdef.ptr[pntnum].dev_id == 0)||(dpdef.TimeTag[pntnum] == 0))
			{
				continue;
			}
			ptr = dpdef.ptr[pntnum];
			if(GetDPStatById(&ptr,&stat)==-1)
			{
				continue;
			}
			if(stat != orgdpsoebuf[pntnum])
			{
				OutputDspInfo(0, "DP soe: %s stat=%02X old=%02X infoaddr=%d\n",name_buf,stat,orgdpsoebuf[pntnum],dp_base_addr + pntnum);

				soe_head = soebuf.bufpnt;
 				if(dpdef.TimeTag[pntnum] == 1)
					soebuf.soerec[soe_head].typeId = M_DP_TA;
				else if(dpdef.TimeTag[pntnum] == 2)
					soebuf.soerec[soe_head].typeId = M_DP_TB;
				else
					continue;

				soebuf.soerec[soe_head].infoaddr = dp_base_addr + pntnum;
				soebuf.soerec[soe_head].stat = stat;				/* QDS IV NT SB BL RES3 SPI */
				orgdpsoebuf[pntnum] = stat;
				soebuf.soerec[soe_head].time = dbentry.var.ind.update_time;
				soebuf.soerec[soe_head].time.year = soebuf.soerec[soe_head].time.year-1900;

				OutputDspInfo(0, "DP soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
					name_buf,soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
					soebuf.soerec[soe_head].infoaddr,
					soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
					soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
					soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
					soebuf.soerec[soe_head].time.ms);

				IncreaseSoeHead(soe_head);
			}
		}
	}
#else		
/*	获取记录区共享内存地址  */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		OutputDspInfo(0, "\niec101s: Fail to shmat shm_rec_addr: errno=%d\n",errno);
			
		exit(0);
	}
	OutputDspInfo(0, "\niec101s:: shmat shm_rec_addr: OK!\n");
	

	rcv_end = shm_rec_addr->head.header_ptr;

	for(;;)
	{
		Sleep(100);
		while( rcv_end!=shm_rec_addr->head.header_ptr )
		{
			rcv_pnt = rcv_end;
			rcv_end++;
			if(rcv_end >= MAX_SHM_RECORD_NUM)
				rcv_end = 0;

			pInd_alm = &shm_rec_addr->rec[rcv_pnt].ind_rec;
			type_id = pInd_alm->type_id;

			if((type_id != DPS_IND_ALM) && (type_id != DPS_MAN_STAT_SET) && (rcv_ptr.data_type != C_SYS_MODE_SET))
			{
				continue;
			}
			rcv_ptr.stn_id = pInd_alm->stn_id;
			rcv_ptr.dev_id = pInd_alm->dev_id;
			rcv_ptr.data_type = pInd_alm->data_type;
			rcv_ptr.pt_id = pInd_alm->pt_id;

			for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Check for SP status Change */
			{
				ptr = spdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(ptr.data_type == rcv_ptr.data_type)&&(ptr.pt_id == rcv_ptr.pt_id))
				{
					dbstat = pInd_alm->ind_status;
					if(dbstat.rt_state==1)
						stat = 1;
					else
						stat = 0;
					if(dbstat.man_set)
						stat = stat | 0x20;		/* SB=1 */
					if(dbstat.chan_fail)
						stat = stat | 0xC0;		/* IV=1; NT=1 */
/*					if(pInd_alm->ind_status.iost.scanEnable==0)
						stat = stat | 0x10;	*/	/* BL=1 */

					soe_head = soebuf.bufpnt;
 					if(spdef.TimeTag[pntnum] == 1)
						soebuf.soerec[soe_head].typeId = M_SP_TA;
					else if(spdef.TimeTag[pntnum] == 2)
						soebuf.soerec[soe_head].typeId = M_SP_TB;
					else
						 break;

					soebuf.soerec[soe_head].infoaddr = sp_base_addr + pntnum;
					soebuf.soerec[soe_head].stat = orgspsoebuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 SPI */
					soebuf.soerec[soe_head].time.ms = pInd_alm->ms;
					soebuf.soerec[soe_head].time.second = pInd_alm->second;
					soebuf.soerec[soe_head].time.minute = pInd_alm->minute;
					soebuf.soerec[soe_head].time.hour = pInd_alm->hour;
					soebuf.soerec[soe_head].time.day = pInd_alm->day;
					soebuf.soerec[soe_head].time.month = pInd_alm->month;
					soebuf.soerec[soe_head].time.year = pInd_alm->year;

					OutputDspInfo(0, "soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						spdef.name[pntnum],soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);
							
					IncreaseSoeHead(soe_head);
					break;
				}
			}

			for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* Check for DP status Change */
			{
				ptr = dpdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(ptr.data_type == rcv_ptr.data_type)&&(ptr.pt_id == rcv_ptr.pt_id))
				{
					dbstat = pInd_alm->ind_status;
					if(dbstat.rt_state == 0)
						stat = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
					else if(dbstat.rt_state == 1)
						stat = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
					else if(dbstat.rt_state == 2)
						stat = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
					else
						stat = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

					if(dbstat.man_set)
						stat = stat | 0x20;			/* SB=1 */
					if(dbstat.chan_fail)
						stat = stat | 0xC0;			/* IV=1; NT=1 */
/*					if(pInd_alm->ind_status.iost.scanEnable==0)
						stat = stat | 0x10;	*/	/* BL=1 */

					soe_head = soebuf.bufpnt;
					if(dpdef.TimeTag[pntnum] == 1)
 						soebuf.soerec[soe_head].typeId = M_DP_TA;
					else if(dpdef.TimeTag[pntnum] == 2)
						soebuf.soerec[soe_head].typeId = M_DP_TB;
					else
						break;

					soebuf.soerec[soe_head].infoaddr = dp_base_addr + pntnum;
					soebuf.soerec[soe_head].stat = orgdpsoebuf[pntnum] = stat;	/* QDS IV NT SB BL RES2 DPI*/
					soebuf.soerec[soe_head].time.ms = pInd_alm->ms;
					soebuf.soerec[soe_head].time.second = pInd_alm->second;
					soebuf.soerec[soe_head].time.minute = pInd_alm->minute;
					soebuf.soerec[soe_head].time.hour = pInd_alm->hour;
					soebuf.soerec[soe_head].time.day = pInd_alm->day;
					soebuf.soerec[soe_head].time.month = pInd_alm->month;
					soebuf.soerec[soe_head].time.year = pInd_alm->year;

					OutputDspInfo(0, "soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						dpdef.name[pntnum],soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);

					IncreaseSoeHead(soe_head);
					break;
				}
			}
			for(pntnum=0;pntnum<stdef.number;pntnum++)		/* Check for ST status Change */
			{
				ptr = stdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(ptr.data_type == rcv_ptr.data_type)&&(ptr.pt_id == rcv_ptr.pt_id))
				{
					dbstat = pInd_alm->ind_status;
					if(dbstat.rt_state == 0)
						vti = 1;	/* OFF:QDS IV NT SB BL RES2 DPI */
					else if(dbstat.rt_state == 1)
						vti = 2;	/* ON:QDS IV NT SB BL RES2 DPI */
					else if(dbstat.rt_state == 2)
						vti = 0;	/* Transit:QDS IV NT SB BL RES2 DPI */
					else
						vti = 3;	/* Error:QDS IV NT SB BL RES2 DPI */

					stat = 0;
					if(dbstat.man_set)
						stat = stat | 0x20;			/* SB=1 */
					if(dbstat.chan_fail)
						stat = stat | 0xC0;			/* IV=1; NT=1 */
/*					if(pInd_alm->ind_status.iost.scanEnable==0)
						stat = stat | 0x10;	*/	/* BL=1 */

					soe_head = soebuf.bufpnt;
					if(stdef.TimeTag[pntnum] == 1)
 						soebuf.soerec[soe_head].typeId = M_ST_TA;
					else if(stdef.TimeTag[pntnum] == 2)
						soebuf.soerec[soe_head].typeId = M_ST_TB;
					else
						break;

					soebuf.soerec[soe_head].infoaddr = st_base_addr + pntnum;
					soebuf.soerec[soe_head].stat = orgstsoebuf[pntnum] = stat;	/* QDS IV NT SB BL RES2 DPI*/
					soebuf.soerec[soe_head].vti = orgstvsoebuf[pntnum] = vti;	/* QDS IV NT SB BL RES2 DPI*/
					soebuf.soerec[soe_head].time.ms = pInd_alm->ms;
					soebuf.soerec[soe_head].time.second = pInd_alm->second;
					soebuf.soerec[soe_head].time.minute = pInd_alm->minute;
					soebuf.soerec[soe_head].time.hour = pInd_alm->hour;
					soebuf.soerec[soe_head].time.day = pInd_alm->day;
					soebuf.soerec[soe_head].time.month = pInd_alm->month;
					soebuf.soerec[soe_head].time.year = pInd_alm->year;

					OutputDspInfo(0, "soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						stdef.name[pntnum],soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);
							
					IncreaseSoeHead(soe_head);
					break;
				}
			}
		}
	}
#endif
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
	strcpy(cmd_buf.usr_login_name,"IEC101S");
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

		OutputDspInfo(0, "Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
		
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		OutputDspInfo(0, "Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
		
	}
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

void CmdInterlockFailBroadcast(char type_id,char cmd_stat,DMS_COMMON *pdbcomm)
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
	cmd_buf.status.state=2;
	cmd_buf.status.result=2;	/* Set to 3: Interlock Fail */
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
	strcpy(cmd_buf.usr_login_name,"IEC101S");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif
	lanhead.dest_id = 0;
	lanhead.dest_stn = 0;
	lanhead.dp_type=COMM_RECORD_TIME;

	OutputDspInfo(0, "Broadcast Cmd Interlock Fail -- type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
	

	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
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
	strcpy(cmd_buf.usr_login_name,"IEC101S");
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

		OutputDspInfo(0, "Set Point Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		OutputDspInfo(0, "Set Point Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
	}
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

void SECmdInterlockFailBroadcast(char type_id, float value, DMS_COMMON *pdbcomm)
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

	cmd_buf.status.state=2;
	cmd_buf.status.result=2;	/* Set to 3: Interlock Fail */
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
	strcpy(cmd_buf.usr_login_name,"IEC101S");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif
	lanhead.dest_id = 0;
	lanhead.dest_stn = 0;
	lanhead.dp_type=COMM_RECORD_TIME;

	OutputDspInfo(0, "Set Point Broadcast Cmd Interlock Fail -- type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);

	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

int Get_Cmd_Ctrl_Priv(char * interlockname,unsigned char stat,DMS_COMMON *pdbentry)
{
	if(strcmp(interlockname,"NULL")==0)	
		return(TRUE);
	if(InterLock_Judge( interlockname )!=1)
	{
		OutputDspInfo(0, "Ctrl condition %s not fulfilled!\n",interlockname);
		return(FALSE);
	}
	else
	{
		if(pdbentry->point.data_type == DEVOBJ_TYPE)
		{
			if(stat == 1)
			{
				if(pdbentry->var.devobj.fb_status1.condc1)
					return(TRUE);
			}
			else
			{
				if(pdbentry->var.devobj.fb_status1.condc0)
					return(TRUE);
			}
			return(FALSE);
		}
		return(TRUE);
	}
}

int Get_Setpt_Ctrl_Priv(char * interlockname)
{
	if(strcmp(interlockname,"NULL")==0)	
		return(TRUE);
	if(InterLock_Judge( interlockname )!=1)
	{
		OutputDspInfo(0, "Ctrl condition %s not fulfiled!\n",interlockname);
		return(FALSE);
	}
	return(TRUE);
}


