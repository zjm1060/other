/****************************************************************
*  FILE NAME:           iec104s_app_fun.c      			*
*  FUNCTION:            Controlled Station			*
*			Application Layer Function		*
*  's NAME : 	              					*
*  DATE:                March,2006                		*
*								*
****************************************************************/

#include <time.h>
#include "../inc/iec104s_com.h"

#define	DEBUG
#define BY_RECORD		/* use shared memory record area to catch SOE */
#define CMD_TO_LCU
/* #define ME_TEST	*/	/* used for test only, Should be canceled in normal running */
/* #define V30_OLD	*/	/* used for early V3.0 version */


extern	unsigned char	sendbuf[1024],cmdsendbuf[24];
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

extern	int	ClockSyncflag,comdflag,cmdterm;
extern	int	Readflag,Testflag,Resetflag;
extern	int	Paramflag,MirrorNAckflag;
extern	int	interrogationflag,counterflag,icgroupflag,cigroupflag;
extern	int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
extern	int	cyclicflag,scanflag;
extern	int	cmd_ack;
extern	char	send_ic_data_type,send_ci_data_type;
extern	unsigned short	delayalarm;
extern	unsigned char	cot_interrogation,cot_counter;
extern	unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
extern	unsigned short	m_cRespParamNum,m_cRespICGrpNum,m_cRespCIGrpNum;
extern	unsigned char	count_sq,ITGrpType;

extern	unsigned short	seladdr,databand;

extern	unsigned short	nASDUId;
extern	unsigned char	nSrcId;
extern	int		Gps;		/* 0: Without GPS; 1: With GPS */
extern	int		GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
extern	int		MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_ND; 3: M_ME_NC */
extern	int		MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int		SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int		TimeTag;	/* Time tag> 0: no; 1: short; 2: long */
extern	int		sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
extern	int		Cmd_Sel;	/*  0: cmd without select; 1: cmd with select */

extern	time_t	scan_period;
extern	time_t	cmd_period;

extern	time_t	scan_tim;
/*extern	SYSTEMTIME	localsys_tm;
*/
extern	int		ProcNum;
extern	PROC_REC	sProcRec[MAX_PROC_NUM+1];

unsigned char	orgspsoebuf[MAX_SP_NUM],orgdpsoebuf[MAX_DP_NUM];
unsigned char	orgstsoebuf[MAX_ST_NUM],orgstvsoebuf[MAX_ST_NUM];
ASDUCOMDDATA	recv_data;
ASDUDATA	send_data,send_data2;
unsigned short	cd_delay_time;

#ifdef REC_YF_AGC_CMD
	unsigned short	daysaved;
	char		agcrecfile[80];
	FILE		*fagc;
#endif

SPDEF		spdef;
DPDEF		dpdef;
MEDEF		medef;
ME_SCALE_DEF	mescaledef;
ME_FLOAT_DEF	mefloatdef;
CYCDEF		cycdef;
CYC_SCALE_DEF	cycscaledef;
CYC_FLOAT_DEF	cycfloatdef;
STDEF		stdef;
ITDEF		itdef;
DCDEF		dcdef;
SEDEF		sedef;
SE_SCALE_DEF	sescaledef;
SE_FLOAT_DEF	sefloatdef;
SCDEF		scdef;
RCDEF		rcdef;

unsigned short	sp_base_addr;
unsigned short	dp_base_addr;
unsigned short	me_base_addr;
unsigned short	me_scale_addr;
unsigned short	me_float_addr;
unsigned short	cyc_base_addr;
unsigned short	cyc_scale_addr;
unsigned short	cyc_float_addr;
unsigned short	pm_base_addr;
unsigned short	it_base_addr;
unsigned short	st_base_addr;
unsigned short	bo_base_addr;

unsigned short	sc_cmd_base_addr;
unsigned short	dc_cmd_base_addr;
unsigned short	rc_cmd_base_addr;
unsigned short	se_cmd_base_addr;
unsigned short	se_cmd_scale_addr;
unsigned short	se_cmd_float_addr;
unsigned short	bc_cmd_base_addr;

char    src_id;

void InitData(char *NameID)
{
	int	i;
	nASDUId = CMM_ADRESS;
	nSrcId = SRC_ADRESS;
	Gps = 1;		/* With GPS */
	GiType = 1;		/* 1:M_SP_NA; 20: M_PS_NA */
	MeASDUType = 3;		/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 2: M_ME_ND; 3: M_ME_NC */
	MeType = 0;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	SEType =0;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	TimeTag =2;		/* time tag 0: no; 1: short; 2: long */
	sp_cos = 0;		/* sp_cos 0: sp/dp transmit once; 1: twice */
	Cmd_Sel = 1;		/* 0: cmd without select; 1: cmd with select */
	ITGrpType = 0;

	scan_period = SCAN_PERIOD;
	cmd_period = CMD_TIMEOUT;
	delayalarm = DIFF_TIME;
	cd_delay_time = 0;

	sp_base_addr = SP_BASE_ADDR;
	dp_base_addr = DP_BASE_ADDR;
	me_base_addr = ME_BASE_ADDR;
	me_scale_addr = ME_SCALE_ADDR;
	me_float_addr = ME_FLOAT_ADDR;
	cyc_base_addr = CYC_BASE_ADDR;
	cyc_scale_addr = CYC_SCALE_ADDR;
	cyc_float_addr = CYC_FLOAT_ADDR;
	pm_base_addr = PM_BASE_ADDR;
	it_base_addr = IT_BASE_ADDR;
	st_base_addr = ST_BASE_ADDR;
	bo_base_addr = BO_BASE_ADDR;

	sc_cmd_base_addr = SC_CMD_BASE_ADDR;
	dc_cmd_base_addr = DC_CMD_BASE_ADDR;
	rc_cmd_base_addr = RC_CMD_BASE_ADDR;

	se_cmd_base_addr = SE_CMD_BASE_ADDR;
	se_cmd_scale_addr = SE_CMD_SCALE_ADDR;
	se_cmd_float_addr = SE_CMD_FLOAT_ADDR;
	bc_cmd_base_addr = BC_CMD_BASE_ADDR;

	m_cRespGenInterroGrpNum=m_cRespCountGrpNum=m_cCyclicGrpNum=m_cScanGrpNum=0;
	send_data.pnt = 0;
	send_data2.pnt = 0;
	recv_data.pnt = 0;

	cmd_ack = 0;
	cmdbuf.tpyeId = 0;
	cmdbuf.infoaddr = 0;
	cmdbuf.ack_flag = 0;
	cmdbuf.selected = 0;
	cmdbuf.term_flag = 0;
	cmdbuf.sel_tim = 0;
	scan_tim = 0;

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

	databand = DATABAND;
	
	spdef.number = 0;
	dpdef.number = 0;
	stdef.number = 0;
	itdef.number = 0;

	medef.number = 0;
	mescaledef.number = 0;
	mefloatdef.number = 0;

	cycdef.number = 0;
	cycscaledef.number = 0;
	cycfloatdef.number = 0;

	sedef.number = 0;
	sescaledef.number = 0;
	sefloatdef.number = 0;

	scdef.number = 0;
	dcdef.number = 0;
	rcdef.number = 0;

	GenConfigTable(NameID,(char *)CONFIG_FILE);

	GenSPTable(NameID,(char *)SPDEF_FILE);
	GenDPTable(NameID,(char *)DPDEF_FILE);

	if((MeASDUType==0)||(MeASDUType==1)||(MeASDUType==2))
		GenMETable(NameID,(char *)MEDEF_NORM);
/*
	else if(MeASDUType==1)
		GenMEScaleTable(NameID,(char *)MEDEF_SCALE);
*/
	else
		GenMEFloatTable(NameID,(char *)MEDEF_FLOAT);
/*
	GenCYCTable(NameID,(char *)CYCDEF_NORM);
	GenCYCScaleTable(NameID,(char *)CYCDEF_SCALE);
	GenCYCFloatTable(NameID,(char *)CYCDEF_FLOAT);

	GenSTTable(NameID,(char *)STDEF_FILE);
	GenITTable(NameID,(char *)ITDEF_FILE);

	GenSEScaleTable(SEDEF_SCALE);
*/
	GenSETable(NameID,(char *)SEDEF_NORM);
	GenSEScaleTable(NameID,(char *)SEDEF_SCALE);
	GenSEFloatTable(NameID,(char *)SEDEF_FLOAT);

	GenSCTable(NameID,(char *)SCDEF_FILE);
	GenDCTable(NameID,(char *)DCDEF_FILE);
/*
	GenSCTable(NameID,(char *)SCDEF_FILE);
	GenRCTable(NameID,(char *)RCDEF_FILE);

	SaveMETable(NameID,(char *)MEDEF_NORM);
	SaveMEScaleTable(NameID,(char *)MEDEF_SCALE);
	SaveMEFloatTable(NameID,(char *)MEDEF_FLOAT);
	sendbufheadpt = 0;
	sendbufptr1 = 0;
	sendbufptr2 = 0;
*/

	counterflag=ClockSyncflag=interrogationflag=0;
	icgroupflag = cigroupflag = 0;
	Resetflag = Testflag = MirrorNAckflag = 0;
	Paramflag = Readflag = Testflag = 0;
	cyclicflag = scanflag = 0;
	soe_overflow_flag = soe_overflow_Sendflag = ClkDiffflag = 0;
	count_sq = 0;

	soebuf.bufpnt = soebuf.sendpnt = 0;
	nr_recv = nr_send = nr_ack = nr_rs = 0;

	src_id =0;
/*
#ifdef V30_OLD
	for(i=MIN_WS_NUM;i<=MAX_WS_NUM;i++)
	{
		if( strcmp( NameID , station[i-MIN_WS_NUM]) == 0)
		{
			src_id =i;
			break;
		}
	}
#else
	for(i=0;i<MAX_HOSTNUM_IN_EMS;i++)
	{
		if( strcmp( NameID , station[i]) == 0)
		{
			src_id =i;
			break;
		}
	}
#endif V30_OLD
*/
}

int ReadStnDef(char * pStnDefFile)
{	FILE	*fp;
	char	NameID[20], Ip[40], CtrlName[40];

	ProcNum = 0;
	while( (fp=fopen(pStnDefFile,"r"))==NULL )
	{
		printf("\n### ALARM !!! IEC104: Can't open Station config file %s !!! \n",pStnDefFile);
		sleep(5);
	}

	while( fscanf(fp,"%s %s %s",NameID,Ip,CtrlName)!=EOF )
	{
		strcpy( sProcRec[ProcNum].NameID, NameID );
		strcpy( sProcRec[ProcNum].Ip, Ip );
		strcpy( sProcRec[ProcNum].ctrl_priv, CtrlName );
		sProcRec[ProcNum].Pid = -1;
		if(sProcRec[ProcNum].NameID[0] == '!')
		{
			printf("\nDisable Station name %s: IP = %s Ctrl_Name = %s\n",sProcRec[ProcNum].NameID,sProcRec[ProcNum].Ip,sProcRec[ProcNum].ctrl_priv);
			continue;
		}
/* #ifdef DEBUG */
		printf("\nStation %d name %s: IP = %s Ctrl_Name = %s\n",ProcNum,sProcRec[ProcNum].NameID,sProcRec[ProcNum].Ip,sProcRec[ProcNum].ctrl_priv);
/* #endif */
		ProcNum++;
		if(ProcNum>=MAX_PROC_NUM)
			break;
	}
	fclose( fp );
/* #ifdef DEBUG */
	printf("\nRead config file %s: proc num = %d!!!\n\n",pStnDefFile,ProcNum);
/* #endif */
	return( TRUE );
}

int GenConfigTable(char * pName,char * pDefFile)
{
	FILE	*fp;
	int	iTemp,iRet;
	char	strFlag[80];
	char	strType[80];
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####Can't open config file %s!!!\n",filenamestr);
		nASDUId = CMM_ADRESS;
		sleep(5);
	}
	rewind(fp);	
#ifdef DEBUG
	printf("\n\nRead config file %s!!!\n",filenamestr);
#endif
	for(;;)
	{
		fscanf(fp,"%s",strFlag);
		if(feof(fp))
		{
			printf("Config file %s End!\n",filenamestr);
			fclose(fp);
			return(TRUE);
		}
		if(strcmp(strFlag,"CMM_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ASDU CMM_ADDR=%d\n",pDefFile,iTemp);
				nASDUId = CMM_ADRESS;
				continue;
			}
			if((iTemp<=0) || (iTemp>254))
			{
				printf("Config file %s Error! ASDU CMM_ADDR=%d\n",pDefFile,iTemp);
				nASDUId = CMM_ADRESS;
			}
			else
			{
#ifdef DEBUG
				printf("ASDU CMM_ADDR = %d\n",iTemp);
#endif
				nASDUId = iTemp;
			}
		}
		else if(strcmp(strFlag,"SRC_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ASDU SRC_ADDR=%d\n",pDefFile,iTemp);
				nSrcId = SRC_ADRESS;
				continue;
			}
			if((iTemp<=0) || (iTemp>255))
			{
				printf("Config file %s Error! ASDU SRC_ADDR=%d\n",pDefFile,iTemp);
				nSrcId = SRC_ADRESS;
			}
			else
			{
#ifdef DEBUG
				printf("ASDU SRC_ADDR = %d\n",iTemp);
#endif
				nSrcId = iTemp;
			}
		}
		else if(strcmp(strFlag,"GPS:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! GPS = %d\n",pDefFile,iTemp);
				Gps = 1;
				continue;
			}
			if((iTemp!=0) && (iTemp!=1))
			{
				printf("Config file %s Error! Gps = %d\n",pDefFile,iTemp);
				Gps = 1;
			}
			else
			{
#ifdef DEBUG
				printf("GPS = %d\n",iTemp);
#endif
				Gps = iTemp;
			}
		}
		else if(strcmp(strFlag,"GI_TYPE:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! GI_TYPE = %d\n",pDefFile,iTemp);
				GiType = 1;
				continue;
			}
			if((iTemp!=1) && (iTemp!=20))
			{
				printf("Config file %s Error! GI_TYPE = %d\n",pDefFile,iTemp);
				GiType = 1;
			}
			else
			{
#ifdef DEBUG
				printf("GI_TYPE = %d\n",iTemp);
#endif
				GiType = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_ASDUTYPE:")== 0)
		{
/*
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				MeASDUType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				printf("Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				MeASDUType = 0;
			}
			else
			{
#ifdef DEBUG
				printf("ME_TYPE = %d\n",iTemp);
#endif
				MeASDUType = iTemp;
			}
*/
			iRet = fscanf(fp,"%s",strType);
			if(iRet<=0)
			{
				printf("Config file %s Error! ME_ASDUTYPE = %d\n",pDefFile,iTemp);
				MeASDUType = 3;
				continue;
			}
			if(strcmp(strType,"M_ME_NA")== 0)
			{
#ifdef DEBUG
				printf("ME_ASDUTYPE = %s\n",strType);
#endif
				MeASDUType = 0;
			}
			else if(strcmp(strType,"M_ME_NB")== 0)
			{
#ifdef DEBUG
				printf("ME_ASDUTYPE = %s\n",strType);
#endif
				MeASDUType = 1;
			}
			else if(strcmp(strType,"M_ME_ND")== 0)
			{
#ifdef DEBUG
				printf("ME_ASDUTYPE = %s\n",strType);
#endif
				MeASDUType = 2;
			}
			else if(strcmp(strType,"M_ME_NC")== 0)
			{
#ifdef DEBUG
				printf("ME_ASDUTYPE = %s\n",strType);
#endif
				MeASDUType = 3;
			}
			else
			{
				printf("Config file %s Error! ME_ASDUTYPE = %s\n",pDefFile,strType);
				MeASDUType = 3;
			}
		}
		else if(strcmp(strFlag,"ME_TYPE:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				MeType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				printf("Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				MeType = 0;
			}
			else
			{
#ifdef DEBUG
				printf("ME_TYPE = %d\n",iTemp);
#endif
				MeType = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_TYPE:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SE_TYPE = %d\n",pDefFile,iTemp);
				SEType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				printf("Config file %s Error! SE_TYPE = %d\n",pDefFile,iTemp);
				SEType = 0;
			}
			else
			{
#ifdef DEBUG
				printf("SE_TYPE = %d\n",iTemp);
#endif
				SEType = iTemp;
			}
		}

		else if(strcmp(strFlag,"TIMETAG:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! TIMETAG = %d\n",pDefFile,iTemp);
				TimeTag = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				printf("Config file %s Error! TIMETAG = %d\n",pDefFile,iTemp);
				TimeTag = 1;
			}
			else
			{
#ifdef DEBUG
				printf("TIMETAG = %d\n",iTemp);
#endif
				TimeTag = iTemp;
			}
		}

		else if(strcmp(strFlag,"SP_COS:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SP_COS = %d\n",pDefFile,iTemp);
				sp_cos = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>1))
			{
				printf("Config file %s Error! SP_COS = %d\n",pDefFile,iTemp);
				sp_cos = 1;
			}
			else
			{
#ifdef DEBUG
				printf("SP_COS = %d\n",iTemp);
#endif
				sp_cos = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMD_SELECT:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! CMD_SELECT = %d\n",pDefFile,iTemp);
				Cmd_Sel = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				printf("Config file %s Error! CMD_SELECT = %d\n",pDefFile,iTemp);
				Cmd_Sel = 1;
			}
			else
			{
#ifdef DEBUG
				printf("CMD_SELECT = %d\n",iTemp);
#endif
				Cmd_Sel = iTemp;
			}
		}
		else if(strcmp(strFlag,"SCAN_PERIOD:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SCAN_PERIOD = %d\n",pDefFile,iTemp);
				scan_period = SCAN_PERIOD;
				continue;
			}
			if(iTemp<0)
			{
				printf("Config file %s Error! SCAN_PERIOD = %d\n",pDefFile,iTemp);
				scan_period = SCAN_PERIOD;
			}
			else
			{
#ifdef DEBUG
				printf("SCAN_PERIOD = %d\n",iTemp);
#endif
				scan_period = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMD_TIMEOUT:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! CMD_TIMEOUT = %d\n",pDefFile,iTemp);
				cmd_period = CMD_TIMEOUT;
				continue;
			}
			if(iTemp<0)
			{
				printf("Config file %s Error! CMD_TIMEOUT = %d\n",pDefFile,iTemp);
				cmd_period = CMD_TIMEOUT;
			}
			else
			{
#ifdef DEBUG
				printf("CMD_TIMEOUT = %d\n",iTemp);
#endif
				cmd_period = iTemp;
			}
		}
		else if(strcmp(strFlag,"SP_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SP_BASE_ADDR=%X\n",pDefFile,iTemp);
				sp_base_addr = SP_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! SP_BASE_ADDR=%X\n",pDefFile,iTemp);
				sp_base_addr = SP_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("SP_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				sp_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"DP_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! DP_BASE_ADDR=%X\n",pDefFile,iTemp);
				dp_base_addr = DP_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! DP_BASE_ADDR=%X\n",pDefFile,iTemp);
				dp_base_addr = DP_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("DP_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				dp_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ME_BASE_ADDR=%X\n",pDefFile,iTemp);
				me_base_addr = ME_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! ME_BASE_ADDR=%X\n",pDefFile,iTemp);
				me_base_addr = ME_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("ME_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				me_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_SCALE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ME_SCALE_ADDR=%X\n",pDefFile,iTemp);
				me_scale_addr = ME_SCALE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! ME_SCALE_ADDR=%X\n",pDefFile,iTemp);
				me_scale_addr = ME_SCALE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("ME_SCALE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				me_scale_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_FLOAT_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ME_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				me_float_addr = ME_FLOAT_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! ME_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				me_float_addr = ME_FLOAT_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("ME_FLOAT_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				me_float_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYC_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! CYC_BASE_ADDR=%X\n",pDefFile,iTemp);
				cyc_base_addr = CYC_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! CYC_BASE_ADDR=%X\n",pDefFile,iTemp);
				cyc_base_addr = CYC_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("CYC_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				cyc_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYC_SCALE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! CYC_SCALE_ADDR=%X\n",pDefFile,iTemp);
				cyc_scale_addr = CYC_SCALE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! CYC_SCALE_ADDR=%X\n",pDefFile,iTemp);
				cyc_scale_addr = CYC_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("CYC_SCALE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				cyc_scale_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYC_FLOAT_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! CYC_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				cyc_float_addr = CYC_FLOAT_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! CYC_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				cyc_float_addr = CYC_FLOAT_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("CYC_FLOAT_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				cyc_float_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"PM_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! PM_BASE_ADDR=%X\n",pDefFile,iTemp);
				pm_base_addr = PM_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! PM_BASE_ADDR=%X\n",pDefFile,iTemp);
				pm_base_addr = PM_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("PM_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				pm_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"IT_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! IT_BASE_ADDR=%X\n",pDefFile,iTemp);
				it_base_addr = IT_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! IT_BASE_ADDR=%X\n",pDefFile,iTemp);
				it_base_addr = IT_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("IT_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				it_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ST_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! ST_BASE_ADDR=%X\n",pDefFile,iTemp);
				st_base_addr = ST_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! ST_BASE_ADDR=%X\n",pDefFile,iTemp);
				st_base_addr = ST_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("ST_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				st_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"BO_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! BO_BASE_ADDR=%X\n",pDefFile,iTemp);
				bo_base_addr = BO_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! BO_BASE_ADDR=%X\n",pDefFile,iTemp);
				bo_base_addr = BO_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("BO_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				bo_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				sc_cmd_base_addr = SC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! SC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				sc_cmd_base_addr = SC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("SC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				sc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"DC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! DC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				dc_cmd_base_addr = DC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! DC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				dc_cmd_base_addr = DC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("DC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				dc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"RC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! RC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				rc_cmd_base_addr = RC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! RC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				rc_cmd_base_addr = RC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("RC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				rc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SE_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_base_addr = SE_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! SE_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_base_addr = SE_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("SE_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				se_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_CMD_SCALE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SE_CMD_SCALE_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_scale_addr = SE_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! SE_CMD_SCALE_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_scale_addr = SE_CMD_SCALE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("SE_CMD_SCALE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				se_cmd_scale_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_CMD_FLOAT_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! SE_CMD_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_float_addr = SE_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! SE_CMD_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				se_cmd_float_addr = SE_CMD_FLOAT_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("SE_CMD_FLOAT_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				se_cmd_float_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"BC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				printf("Config file %s Error! BC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				bc_cmd_base_addr = BC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				printf("Config file %s Error! BC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				bc_cmd_base_addr = BC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				printf("BC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				bc_cmd_base_addr = iTemp;
			}
		}
		else
		{
			printf("##### %s  ",strFlag);
			fscanf(fp,"%s",strFlag);
			printf("%s ##### ERROR!\n",strFlag);
		}
	}
}

int GenSPTable(char * pName,char *pDefFile)		/* single point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("\n####can't open SP file %s\n",filenamestr);
 		spdef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead SP define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SP File %s Define Error! SP Number = %d\n",pDefFile,iTemp);
		spdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_SP_NUM)
	{
		printf("\n!!!GenSPTable: Num=%d above the MAX_SP_NUM %d\n",iTemp,MAX_SP_NUM);
		spdef.number = MAX_SP_NUM;
	}
	else
		spdef.number = iTemp;

	for(j=0;j<spdef.number;j++)
	{
 		fscanf(fp,"%d %s %d %s",&iTemp,spdef.name[j],&spdef.TimeTag[j],lgname);
		if(strcmp(spdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
 			printf("SP %d pnt: %s\t%d\t%s\n",j,spdef.name[j],spdef.TimeTag[j],lgname);
#endif
			strcpy(name_buf2,spdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				spdef.ptr[j].dev_id = 0;
				printf("#### SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				spdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			spdef.ptr[j].dev_id = 0;
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("---GenSPTable: point nNumber = %d\n",spdef.number);
#endif
	return(TRUE);
}

int GenDPTable(char * pName,char *pDefFile)		/* double point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open DP file %s\n",filenamestr);
 		dpdef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead DP define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####DP File %s Define Error! DP Number = %d\n",pDefFile,iTemp);
		dpdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_DP_NUM)
	{
		printf("\n!!!GenDPTable: Num=%d above the MAX_DP_NUM %d\n",iTemp,MAX_DP_NUM);
		dpdef.number = MAX_DP_NUM;
	}
	else
		dpdef.number = iTemp;

	for(j=0;j<dpdef.number;j++)
	{
 		fscanf(fp,"%d %s %d %s",&iTemp,dpdef.name[j],&dpdef.TimeTag[j],lgname);
		if(strcmp(dpdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
 			printf("DP %d pnt: %s\t%d\t%s\n",j,dpdef.name[j],dpdef.TimeTag[j],lgname);
#endif
			strcpy(name_buf2,dpdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				dpdef.ptr[j].dev_id = 0;
				printf("#### DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				dpdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			dpdef.ptr[j].dev_id = 0;
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("---GenDPTable: point nNumber = %d\n",dpdef.number);
#endif
	return(TRUE);
}

int GenMETable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int		iTemp,iRet;
	short		j;
	char		filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####can't open ME file %s\n",filenamestr);
 		medef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead ME define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####ME File %s Define Error! ME Number = %d\n",pDefFile,iTemp);
		medef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_ME_NUM)
	{
		printf("\n!!!GenMETable: Num=%d above the MAX_ME_NUM %d\n",iTemp,MAX_ME_NUM);
		medef.number = MAX_ME_NUM;
	}
	else
		medef.number = iTemp;

	for(j=0;j<medef.number;j++)
	{
		fscanf(fp,"%d %s %f %f %f %s",&iTemp,medef.name[j],&medef.lorange[j],&medef.hirange[j],&medef.databand[j],lgname);
		if(strcmp(medef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("ME %d pnt: %s\t%.3f %.3f %.3f %s\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.databand[j],lgname);
#endif
			strcpy(name_buf2,medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenMETable: point nNumber = %d\n\n",medef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenMEScaleTable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####can't open ME Scale file %s\n",filenamestr);
 		mescaledef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead ME Scale define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####ME Scale File %s Define Error! ME Number = %d\n",pDefFile,iTemp);
		mescaledef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_ME_NUM)
	{
		printf("\n!!!GenMEScaleTable: Num=%d above the MAX_ME_NUM %d\n",iTemp,MAX_ME_NUM);
		mescaledef.number = MAX_ME_NUM;
	}
	else
		mescaledef.number = iTemp;

	for(j=0;j<mescaledef.number;j++)
	{
		fscanf(fp,"%s %f %f %f %f %d %s",mescaledef.name[j],&mescaledef.lorange[j],&mescaledef.hirange[j],
			&mescaledef.databand[j],&mescaledef.scale[j],&mescaledef.TimeTag[j],lgname);
		if(strcmp(mescaledef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("Scale ME %d pnt: %s\t%.3f %.3f %.3f %.3f %d %s\n",j,mescaledef.name[j],mescaledef.lorange[j],
				mescaledef.hirange[j],mescaledef.databand[j],mescaledef.scale[j],mescaledef.TimeTag[j],lgname);
#endif
			strcpy(name_buf2,mescaledef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("####Scale ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenMEScaleTable: point nNumber = %d\n\n",mescaledef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenMEFloatTable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####can't open Float ME file %s\n",filenamestr);
 		mefloatdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead Float ME define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####Float ME File %s Define Error! ME Number = %d\n",pDefFile,iTemp);
		mefloatdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_ME_NUM)
	{
		printf("\n!!!GenMEFloatTable: Num=%d above the MAX_ME_NUM %d\n",iTemp,MAX_ME_NUM);
		mefloatdef.number = MAX_ME_NUM;
	}
	else
		mefloatdef.number = iTemp;

	for(j=0;j<mefloatdef.number;j++)
	{
/* 		fscanf(fp,"%d %s %f %f %f %d %s",&iTemp,mefloatdef.name[j],&mefloatdef.lorange[j],&mefloatdef.hirange[j],&mefloatdef.databand[j],&mefloatdef.TimeTag[j],lgname); */
		fscanf(fp,"%d %s %f %f %f %s",&iTemp,mefloatdef.name[j],&mefloatdef.lorange[j],&mefloatdef.hirange[j],&mefloatdef.databand[j],lgname);
		if(strcmp(mefloatdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
/* 			printf("ME %d pnt: %s\t%.3f %.3f %.3f %d %s\n",j,mefloatdef.name[j],mefloatdef.lorange[j],mefloatdef.hirange[j],mefloatdef.databand[j],mefloatdef.TimeTag[j],lgname); */
			printf("ME %d pnt: %s\t%.3f %.3f %.3f %s\n",j,mefloatdef.name[j],mefloatdef.lorange[j],mefloatdef.hirange[j],mefloatdef.databand[j],lgname);
#endif
			strcpy(name_buf2,mefloatdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("####Float ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenMEFloatTable: point nNumber = %d\n\n",mefloatdef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenCYCTable(char * pName,char *pDefFile)		/* Cyclic point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####can't open Cyclic file %s\n",filenamestr);
 		cycdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead Cyclic define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####Cyclic File %s Define Error! Cyclic Number = %d\n",pDefFile,iTemp);
		cycdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_CYC_NUM)
	{
		printf("\n!!!GenCYCTable: Num=%d above the MAX_CYC_NUM %d\n",iTemp,MAX_CYC_NUM);
		cycdef.number = MAX_CYC_NUM;
	}
	else
		cycdef.number = iTemp;

	for(j=0;j<cycdef.number;j++)
	{
		fscanf(fp,"%d %s %f %f %f %s",&iTemp,cycdef.name[j],&cycdef.lorange[j],&cycdef.hirange[j],&cycdef.databand[j],lgname);
		if(strcmp(cycdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("CYC %d pnt: %s %.3f %.3f %.3f %s\n",j,cycdef.name[j],cycdef.lorange[j],cycdef.hirange[j],cycdef.databand[j],lgname);
#endif
			strcpy(name_buf2,cycdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### CYC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenCYCTable: point nNumber = %d\n",cycdef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenCYCScaleTable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####can't open CYC ME Scale file %s\n",filenamestr);
 		cycscaledef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead CYC ME Scale define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####ME Scale File %s Define Error! ME Number = %d\n",pDefFile,iTemp);
		cycscaledef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_ME_NUM)
	{
		printf("\n!!!GenCYCScaleTable: Num=%d above the MAX_ME_NUM %d\n",iTemp,MAX_ME_NUM);
		cycscaledef.number = MAX_ME_NUM;
	}
	else
		cycscaledef.number = iTemp;

	for(j=0;j<cycscaledef.number;j++)
	{
		fscanf(fp,"%d %s %f %f %f %f %s",&iTemp,cycscaledef.name[j],&cycscaledef.lorange[j],
			&cycscaledef.hirange[j],&cycscaledef.databand[j],&cycscaledef.scale[j],lgname);
		if(strcmp(cycscaledef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("CYC Scale ME %d pnt: %s %.3f %.3f %.3f %.3f %s\n",j,cycscaledef.name[j],
				cycscaledef.lorange[j],cycscaledef.hirange[j],cycscaledef.databand[j],cycscaledef.scale[j],lgname);
#endif
			strcpy(name_buf2,cycscaledef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("####CYC Scale ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenMEScaleTable: point nNumber = %d\n\n",cycscaledef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenCYCFloatTable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	int				data_type2;
	char			name_buf2[NAME_SIZE],lgname[36];

	int		iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####can't open CYC Float ME file %s\n",filenamestr);
 		cycfloatdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead CYC Float ME define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####CYC Float ME File %s Define Error! ME Number = %d\n",pDefFile,iTemp);
		cycfloatdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_ME_NUM)
	{
		printf("\n!!!GenCYCFloatTable: Num=%d above the MAX_ME_NUM %d\n",iTemp,MAX_ME_NUM);
		cycfloatdef.number = MAX_ME_NUM;
	}
	else
		cycfloatdef.number = iTemp;

	for(j=0;j<cycfloatdef.number;j++)
	{
		fscanf(fp,"%d %s %f %f %f %s",&iTemp,cycfloatdef.name[j],&cycfloatdef.lorange[j],&cycfloatdef.hirange[j],&cycfloatdef.databand[j],lgname);
		if(strcmp(cycfloatdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("ME %d pnt: %s %.3f %.3f %.3f %s\n",j,cycfloatdef.name[j],cycfloatdef.lorange[j],cycfloatdef.hirange[j],cycfloatdef.databand[j],lgname);
#endif
			strcpy(name_buf2,cycfloatdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("####CYC Float ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenCYCFloatTable: point nNumber = %d\n\n",cycfloatdef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int SaveMETable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE];
	char		longname_buf[36];

	int	iRet;
	short	j,k;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"w"))==NULL)
	{
 		printf("\n####can't open(w) ME file %s\n",filenamestr);
 		medef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	iRet = fprintf(fp,"%d\n",medef.number);
	if(iRet<=0)
	{
		printf("\n#####ME File %s Save Error! ME Number = %d\n",pDefFile,medef.number);
	}
#ifdef DEBUG
	printf("---SaveMETable: File %s, point number = %d\n",filenamestr,medef.number);
#endif

	for(j=0;j<medef.number;j++)
	{
		if(strcmp(medef.name[j],"NULL")!=0)
		{
			strcpy(name_buf2,medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				fprintf(fp,"%d\t%s\t%f\t%f\t%f\t%d\t数据库无此点%d\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.databand[j],medef.TimeTag[j],j);
#ifdef DEBUG
				printf("#### ME %d pnt: %s %.3f %.3f %.3f %d\t数据库无此点%d\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.databand[j],medef.TimeTag[j],j);
#endif
			}
			else
			{
				strcpy(longname_buf,dbentry2.fixed.entryLongname);
				for(k=0;k<36;k++)
				{
					if((longname_buf[k] == ' ')||(longname_buf[k] == 9))
						longname_buf[k] = '_';
				}
				fprintf(fp,"%d\t%s\t%.3f\t%.3f\t%.3f\t%d\t%s\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.databand[j],medef.TimeTag[j],
					longname_buf);
#ifdef DEBUG
				printf("ME %d pnt: %s %.3f %.3f %.3f\t%d\t%s\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.databand[j],medef.TimeTag[j],
					longname_buf);
#endif
			}
		}
		else
		{
			fprintf(fp,"%d\t%s\t%.3f\t%.3f\t%.3f\t%d\t测量值备用点%d\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.databand[j],medef.TimeTag[j],j);
#ifdef DEBUG
			printf("ME %d pnt: %s %.3f %.3f %.3f\t%d\t测量值备用点%d\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.databand[j],medef.TimeTag[j],j);
#endif
		}
	}
	fclose(fp);
	return(TRUE);
}

int SaveMEScaleTable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	int				data_type2;
	char			name_buf2[NAME_SIZE];
	char			longname_buf[36];

	int		iRet;
	short	j,k;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"w"))==NULL)
	{
 		printf("\n####can't open(w) ME Scale file %s\n",filenamestr);
 		mescaledef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	iRet = fprintf(fp,"%d\n",mescaledef.number);
	if(iRet<=0)
	{
		printf("\n#####ME Scale File %s Save Error! ME Number = %d\n",pDefFile,mescaledef.number);
	}
#ifdef DEBUG
	printf("---SaveMEScaleTable: File %s, point number = %d\n",filenamestr,mescaledef.number);
#endif

	for(j=0;j<mescaledef.number;j++)
	{
		if(strcmp(mescaledef.name[j],"NULL")!=0)
		{
			strcpy(name_buf2,mescaledef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				fprintf(fp,"%d\t%s\t%f\t%f\t%f\t%f\t%d\t数据库无此点%d\n",j,mescaledef.name[j],mescaledef.lorange[j],
					mescaledef.hirange[j],mescaledef.databand[j],mescaledef.scale[j],mescaledef.TimeTag[j],j);
#ifdef DEBUG
				printf("####Scale ME %d pnt: %s %.3f %.3f %.3f %.3f %d\t数据库无此点%d\n",j,mescaledef.name[j],
					mescaledef.lorange[j],mescaledef.hirange[j],mescaledef.databand[j],mescaledef.scale[j],
					mescaledef.TimeTag[j],j);
#endif
			}
			else
			{
				strcpy(longname_buf,dbentry2.fixed.entryLongname);
				for(k=0;k<36;k++)
				{
					if((longname_buf[k] == ' ')||(longname_buf[k] == 9))
						longname_buf[k] = '_';
				}
				fprintf(fp,"%d\t%s\t%.3f\t%.3f\t%.3f\t%.3f\t%d\t%s\n",j,mescaledef.name[j],
					mescaledef.lorange[j],mescaledef.hirange[j],mescaledef.databand[j],
					mescaledef.scale[j],mescaledef.TimeTag[j],longname_buf);
#ifdef DEBUG
				printf("Scale ME %d pnt: %s %.3f %.3f %.3f %.3f %d\t%s\n",j,mescaledef.name[j],
					mescaledef.lorange[j],mescaledef.hirange[j],mescaledef.databand[j],
					mescaledef.scale[j],mescaledef.TimeTag[j],longname_buf);
#endif
			}
		}
		else
		{
			fprintf(fp,"%d\t%s\t%.3f\t%.3f\t%.3f\t%.3f\t%d\t测量值备用点%d\n",j,mescaledef.name[j],
				mescaledef.lorange[j],mescaledef.hirange[j],mescaledef.databand[j],mescaledef.scale[j],
				mescaledef.TimeTag[j],j);
#ifdef DEBUG
			printf("Scale ME %d pnt: %s %.3f %.3f %.3f %.3f %d\t测量值备用点%d\n",j,mescaledef.name[j],
				mescaledef.lorange[j],mescaledef.hirange[j],mescaledef.databand[j],mescaledef.scale[j],
				mescaledef.TimeTag[j],j);
#endif
		}
	}
	fclose(fp);
	return(TRUE);
}

int SaveMEFloatTable(char * pName,char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE];
	char		longname_buf[36];

	int	iRet;
	short	j,k;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"w"))==NULL)
	{
 		printf("\n####can't open(w) ME Float file %s\n",filenamestr);
 		mefloatdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	iRet = fprintf(fp,"%d\n",mefloatdef.number);
	if(iRet<=0)
	{
		printf("\n#####ME Float File %s Save Error! ME Number = %d\n",filenamestr,mefloatdef.number);
	}
#ifdef DEBUG
	printf("---SaveMEFloatTable: File %s, point number = %d\n",pDefFile,mefloatdef.number);
#endif

	for(j=0;j<mefloatdef.number;j++)
	{
		if(strcmp(mefloatdef.name[j],"NULL")!=0)
		{
			strcpy(name_buf2,mefloatdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				fprintf(fp,"%d\t%s\t%f\t%f\t%f\t%d\t数据库无此点%d\n",j,mefloatdef.name[j],mefloatdef.lorange[j],
					mefloatdef.hirange[j],mefloatdef.databand[j],mefloatdef.TimeTag[j],j);
#ifdef DEBUG
				printf("####Float ME %d pnt: %s %.3f %.3f %.3f %d\t数据库无此点%d\n",j,mefloatdef.name[j],
					mefloatdef.lorange[j],mefloatdef.hirange[j],mefloatdef.databand[j],mefloatdef.TimeTag[j],j);
#endif
			}
			else
			{
				strcpy(longname_buf,dbentry2.fixed.entryLongname);
				for(k=0;k<36;k++)
				{
					if((longname_buf[k] == ' ')||(longname_buf[k] == 9))
						longname_buf[k] = '_';
				}
				fprintf(fp,"%d\t%s\t%.3f\t%.3f\t%.3f\t%d\t%s\n",j,mefloatdef.name[j],mefloatdef.lorange[j],
					mefloatdef.hirange[j],mefloatdef.databand[j],mefloatdef.TimeTag[j],longname_buf);
#ifdef DEBUG
				printf("Float ME %d pnt: %s %.3f %.3f %.3f %d\t%s\n",j,mefloatdef.name[j],mefloatdef.lorange[j],
					mefloatdef.hirange[j],mefloatdef.databand[j],mefloatdef.TimeTag[j],longname_buf);
#endif
			}
		}
		else
		{
			fprintf(fp,"%d\t%s\t%.3f\t%.3f\t%.3f\t%d\t测量值备用点%d\n",j,mefloatdef.name[j],mefloatdef.lorange[j],
				mefloatdef.hirange[j],mefloatdef.databand[j],mefloatdef.TimeTag[j],j);
#ifdef DEBUG
			printf("Float ME %d pnt: %s %.3f %.3f %.3f %d\t测量值备用点%d\n",j,mefloatdef.name[j],
				mefloatdef.lorange[j],mefloatdef.hirange[j],mefloatdef.databand[j],mefloatdef.TimeTag[j],j);
#endif
		}
	}
	fclose(fp);
	return(TRUE);
}

int GenSTTable(char * pName,char *pDefFile)		/* integrated total point define file */
{
	FILE			*fp;
	DMS_COMMON		dbentry2;
	int				data_type2;
	char			name_buf2[NAME_SIZE],lgname[36];

	int		iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open ST file %s\n",filenamestr);
 		stdef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead ST define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####ST File %s Define Error! ST Number = %d\n",pDefFile,iTemp);
		stdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_ST_NUM)
	{
		printf("\n!!!GenSTTable: Num=%d above the MAX_ST_NUM %d\n",iTemp,MAX_ST_NUM);
		stdef.number = MAX_ST_NUM;
	}
	else
		stdef.number = iTemp;

	for(j=0;j<stdef.number;j++)
	{
		fscanf(fp,"%d %s %d %s",&iTemp,stdef.name[j],&stdef.TimeTag[j],lgname);
		if(strcmp(stdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("ST %d pnt: %s %d %s\n",j,stdef.name[j],stdef.TimeTag[j],lgname);
#endif
			strcpy(name_buf2,stdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### ST %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			else
			{
				stdef.ptr[j] = dbentry2.point;
			}
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("---GenSTTable: point nNumber = %d\n",stdef.number);
#endif
	return(TRUE);
}

int GenITTable(char * pName,char *pDefFile)		/* integrated total point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open IT file %s\n",filenamestr);
 		itdef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead IT define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####IT File %s Define Error! IT Number = %d\n",pDefFile,iTemp);
		itdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_IT_NUM)
	{
		printf("\n!!!GenITTable: Num=%d above the MAX_IT_NUM %d\n",iTemp,MAX_IT_NUM);
		itdef.number = MAX_IT_NUM;
	}
	else
		itdef.number = iTemp;

	for(j=0;j<itdef.number;j++)
	{
		fscanf(fp,"%d %s %d %s",&iTemp,itdef.name[j],&itdef.TimeTag[j],lgname);
		if(strcmp(itdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("IT %d pnt: %s %d %s\n",j,itdef.name[j],itdef.TimeTag[j],lgname);
#endif
			strcpy(name_buf2,itdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### IT %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("---GenITTable: point nNumber = %d\n",itdef.number);
#endif
	return(TRUE);
}

int GenSCTable(char * pName,char *pDefFile)		/* single point control command define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1,iRet,numb;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open SC file %s\n",filenamestr);
 		scdef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead SC define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SC File %s Define Error! SC Number = %d\n",pDefFile,iTemp);
		scdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_SC_NUM)
	{
		printf("\n!!!GenSCTable: Num=%d above the MAX_SC_NUM %d\n",iTemp,MAX_SC_NUM);
		scdef.number = MAX_SC_NUM;
	}
	else
		scdef.number = iTemp;

	for(j=0;j<scdef.number;j++)
	{
		fscanf(fp,"%d %s %d %d %s",&numb,scdef.name[j],&iTemp,&iTemp1,lgname);
		scdef.cmdtype[j] = (unsigned char)iTemp;
		scdef.optype[j] = (unsigned char)iTemp1;
		if(j!=numb)
		{
			printf("SC %d pnt number=%d wrong!!! %s %d %d %s\n",j,numb,scdef.name[j],scdef.cmdtype[j],scdef.optype[j],lgname);
		}
		if(strcmp(scdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("SC %d pnt: %s %d %d %s\n",j,scdef.name[j],scdef.cmdtype[j],scdef.optype[j],lgname);
#endif
			strcpy(name_buf2,scdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### SC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("---GenSCTable: point nNumber = %d\n",scdef.number);
#endif
	return(TRUE);
}

int GenDCTable(char * pName,char *pDefFile)		/* double point control command define file */
{
	FILE			*fp;
	DMS_COMMON		dbentry2;
	int				data_type2;
	char			name_buf2[NAME_SIZE],lgname[36];

	int		iTemp,iTemp1,iRet,numb;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open DC file %s\n",filenamestr);
 		dcdef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead DC define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####DC File %s Define Error! DC Number = %d\n",pDefFile,iTemp);
		dcdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_DC_NUM)
	{
		printf("\n!!!GenDCTable: Num=%d above the MAX_DC_NUM %d\n",iTemp,MAX_DC_NUM);
		dcdef.number = MAX_DC_NUM;
	}
	else
		dcdef.number = iTemp;

	for(j=0;j<dcdef.number;j++)
	{
		fscanf(fp,"%d %s %d %d %s",&numb,dcdef.name[j],&iTemp,&iTemp1,lgname);
		dcdef.cmdtype[j] = (unsigned char)iTemp;
		dcdef.optype[j] = (unsigned char)iTemp1;
		if(j!=numb)
		{
			printf("DC %d pnt number=%d wrong!!! %s %d %d %s\n",j,numb,dcdef.name[j],dcdef.cmdtype[j],dcdef.optype[j],lgname);
		}
		if(strcmp(dcdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("DC %d pnt: %s %d %d %s\n",j,dcdef.name[j],dcdef.cmdtype[j],dcdef.optype[j],lgname);
#endif
			strcpy(name_buf2,dcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("---GenDCTable: point nNumber = %d\n",dcdef.number);
#endif
	return(TRUE);
}

int GenSETable(char * pName,char *pDefFile)		/* set point control point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("####can't open SE file %s\n",filenamestr);
 		sedef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead SE define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SE File %s Define Error! SE Number = %d\n",pDefFile,iTemp);
		sedef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_SE_NUM)
	{
		printf("\n!!!GenSETable: Num=%d above the MAX_SE_NUM %d\n",iTemp,MAX_SE_NUM);
		sedef.number = MAX_SE_NUM;
	}
	else
		sedef.number = iTemp;

	for(j=0;j<sedef.number;j++)
	{
		fscanf(fp," %d %s %f %f %s",&iTemp,sedef.name[j],&sedef.lorange[j],&sedef.hirange[j],lgname);
/* 		fscanf(fp,"%s %f %s",sedef.name[j],&sedef.lorange[j],lgname); */
		if(j!=iTemp)
		{
			printf("SE %d pnt number=%d wrong!!! %s %f %f %s\n",j,iTemp,sedef.name[j],sedef.lorange[j],sedef.hirange[j],lgname);
		}
		if(strcmp(sedef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("SE %d pnt: %s %f %f %s\n",j,sedef.name[j],sedef.lorange[j],sedef.hirange[j],lgname);
#endif
			strcpy(name_buf2,sedef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### SE %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenSETable: point nNumber = %d\n",sedef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenSEScaleTable(char * pName,char *pDefFile)		/* set point control point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("####can't open SE Scale file %s\n",filenamestr);
 		sescaledef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead SE Scale define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SE Scale File %s Define Error! SE Number = %d\n",pDefFile,iTemp);
		sescaledef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_SE_NUM)
	{
		printf("\n!!!GenSEScaleTable: Num=%d above the MAX_SE_NUM %d\n",iTemp,MAX_SE_NUM);
		sescaledef.number = MAX_SE_NUM;
	}
	else
		sescaledef.number = iTemp;

	for(j=0;j<sescaledef.number;j++)
	{
		fscanf(fp,"%d %s %f %f %f %s",&iTemp,sescaledef.name[j],&sescaledef.lorange[j],&sescaledef.hirange[j],&sescaledef.scale[j],lgname);
/* 		fscanf(fp,"%s %f %s",sescaledef.name[j],&sescaledef.lorange[j],lgname); */
		if(j!=iTemp)
		{
			printf("SE_Scale %d pnt number=%d wrong!!! %s %f %f %s\n",j,iTemp,sescaledef.name[j],sescaledef.lorange[j],sescaledef.hirange[j],lgname);
		}
		if(strcmp(sescaledef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("SE Scale %d pnt: %s %f %f %f %s\n",j,sescaledef.name[j],sescaledef.lorange[j],sescaledef.hirange[j],sescaledef.scale[j],lgname);
#endif
			strcpy(name_buf2,sescaledef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### SE Scale %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenSEScaleTable: point nNumber = %d\n",sescaledef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenSEFloatTable(char * pName,char *pDefFile)		/* set point control point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("####can't open SE float file %s\n",filenamestr);
 		sefloatdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead SE float define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SE float File %s Define Error! SE Number = %d\n",pDefFile,iTemp);
		sefloatdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_SE_NUM)
	{
		printf("\n!!!GenSEFloatTable: Num=%d above the MAX_SE_NUM %d\n",iTemp,MAX_SE_NUM);
		sefloatdef.number = MAX_SE_NUM;
	}
	else
		sefloatdef.number = iTemp;

	for(j=0;j<sefloatdef.number;j++)
	{
		fscanf(fp,"%d %s %f %f %s",&iTemp,sefloatdef.name[j],&sefloatdef.lorange[j],&sefloatdef.hirange[j],lgname);
/* 		fscanf(fp,"%s %f %s",sefloatdef.name[j],&sefloatdef.lorange[j],lgname); */
		if(j!=iTemp)
		{
			printf("SE_Float %d pnt number=%d wrong!!! %s %f %f %s\n",j,iTemp,sefloatdef.name[j],sefloatdef.lorange[j],sefloatdef.hirange[j],lgname);
		}
		if(strcmp(sefloatdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("SE float %d pnt: %s %f %f %s\n",j,sefloatdef.name[j],sefloatdef.lorange[j],sefloatdef.hirange[j],lgname);
#endif
			strcpy(name_buf2,sefloatdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### SE float %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
#ifdef DEBUG
	printf("---GenSEFloatTable: point nNumber = %d\n",sefloatdef.number);
#endif
	fclose(fp);
	return(TRUE);
}

int GenRCTable(char * pName,char *pDefFile)		/* step point control command define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet,numb;
	short	j;
	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("\n####can't open RC file %s\n",filenamestr);
 		rcdef.number = 0;
		return(FALSE);          
	}
		  
	rewind(fp);	
	
#ifdef DEBUG
	printf("\nRead RC define file %s!!!\n",filenamestr);
#endif
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####RC File %s Define Error! RC Number = %d\n",pDefFile,iTemp);
		rcdef.number = 0;
		fclose(fp);
		return(FALSE);
	}
	if(iTemp >= MAX_RC_NUM)
	{
		printf("\n!!!GenRCTable: Num=%d above the MAX_RC_NUM %d\n",iTemp,MAX_RC_NUM);
		rcdef.number = MAX_RC_NUM;
	}
	else
		rcdef.number = iTemp;

	for(j=0;j<rcdef.number;j++)
	{
		fscanf(fp,"%d %s %d %s",&numb,rcdef.name[j],&iTemp,lgname);
		rcdef.cmdtype[j] = (unsigned char)iTemp;
		if(strcmp(rcdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("RC %d pnt: %s %d %s\n",j,rcdef.name[j],rcdef.cmdtype[j],lgname);
#endif
			strcpy(name_buf2,rcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### RC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
#ifdef DEBUG
	printf("---GenRCTable: point nNumber = %d\n",rcdef.number);
#endif
	return(TRUE);
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
	bj_buf.host_id=GetHost_Cmd();


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
	lanhead.src_id =GetHost_Cmd();
*/
	lanhead.dest_id =0;
	lanhead.src_id =bj_buf.host_id;
	lanhead.dest_stn =bj_buf.stn_id;
	lanhead.packet_num =0;
	lan_out ((char *)&bj_buf,lanhead,(unsigned char)0);

}

int	IEC870_5_Recv(int pnum,unsigned int recvdatapt,int sockfd)
{
	int		ret;
	int		sock;
	LAN_HEAD	lanhead;
#ifdef CMD_TO_LCU
	COMMAND_MSG	cmd_buf;
#else
	SYS_MSG		bj_buf;
#endif
/*	ANA_FMT		ana_buf,lanana[40]; */
	char		me_buf[1440];
	unsigned short	infoaddr,ms,year,datalength,lengthfactor;
	int		infoptr;
	char		dbname[36];
	unsigned char	type_id,varstru,qoc,frz,lanpt;
	unsigned char	infonum,objpnt;
	float		value;
	short		setvalue;
	unsigned short	asduaddr,causetra;
	short		cmd_stat;

	DMS_COMMON	dbcommpt;
	int		i,datapoint,data_type;
	ANLG_DATA	anadata;

	struct timeval	 sTimeVal;
	struct	tm	tmpt;
	time_t		tim;
	time_t		stTim,sltTim;
	struct tm	sTimeStart,slTimeStart;
	int		diffclk;

	sock = sockfd;
	type_id = recv_data.data[recvdatapt][0];
	varstru = recv_data.data[recvdatapt][1];
#ifdef SUN_OS
	CrossCopy(&causetra, &recv_data.data[recvdatapt][2],2);	/* cause of transmit */
	CrossCopy(&asduaddr, &recv_data.data[recvdatapt][4],2);	/* ASDU address */
#else
	memcpy(&causetra, &recv_data.data[recvdatapt][2],2);	/* cause of transmit */
	memcpy(&asduaddr, &recv_data.data[recvdatapt][4],2);	/* ASDU address */
#endif
	if(asduaddr!=nASDUId)
	{
		printf("ASDU addr %d != %d wrong!\n",asduaddr,nASDUId);
		mirbuf.cot=(unsigned char)(0x40|UKASDUAD);
		mirbuf.pnt=recvdatapt;
		ret = MirrorNAckflag = 1;
		return(ret);
	}
	tim=time(&tim);

	switch(type_id)
	{
	case C_SC_NA:	/* 45 Single point control command */

		qoc = recv_data.data[recvdatapt][9];/* QOC:= S/E[8] QU[3..7] SCS[1] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
		cmdbuf.tpyeId = type_id;
		cmdbuf.infoaddr = infoaddr;
		cmdbuf.stat = qoc;
/* 		cmdbuf.ack_flag = 1;	 */	/* Set to 1 when ACK by CmdRespSend(int sock) */
		cmdbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(10 != recv_data.length[recvdatapt])
		{
			printf("received C_SC_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			cmdbuf.cot=(unsigned char)NACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
			ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-sc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=scdef.number))
			{
				printf("%04x info address wrong! No this point in SC Control Table!\n",infoaddr);
				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = SinglePointCtrlAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* SC NActCon */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				printf("The control privilege not in %s center! NACTCON SC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}

			strcpy(dbname,scdef.name[infoptr]);
#ifdef DEBUG
			printf("recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,qoc,causetra);
#endif
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("%s No this SC control point in the database file!\n",dbname);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
				return(ret);
			}
			if(qoc & 0x80)		/* Select Command */
			{
				if(((qoc & 0x3)==2)||((qoc & 0x3)==3))
				{
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
				}
				else
				{
					cmdbuf.cot=(unsigned char)ACTCON;
					cmdbuf.selected=1;
					cmdbuf.term_flag=0;
					cmdbuf.sel_tim=tim;
				}
#ifdef DEBUG
				printf("=====Select SC command %02x addr= %04x received\n",qoc,infoaddr);
#endif
				ret = SinglePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* SC ActCon */
				return(ret);
			}
/* 	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period))
				{
#ifdef DEBUG
					printf("NACK Execute SC command infoaddr:%d != Select infoaddr:%d or Timeout %d\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));
#endif
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
/* 					SinglePointCtrlAck(cmdbuf.cot,cmdbuf.infoaddr,cmdbuf.stat,sockfd); */
					ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
					return(ret);
				}
			}
			ret = SinglePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* SC ActCon:7 */
			cmd_stat = scdef.optype[infoptr];
#ifdef DEBUG
			printf("SC command addr=%04X QOC=%02X cmd_stat=%d received\n",infoaddr,qoc,cmd_stat);
#endif
			cmd_buf.type_id = scdef.cmdtype[infoptr];
			cmd_buf.stn_id = dbcommpt.point.stn_id;
			cmd_buf.dev_id = dbcommpt.point.dev_id;
			cmd_buf.data_type = dbcommpt.point.data_type;
			cmd_buf.point_id = dbcommpt.point.pt_id;

			cmd_buf.status.opr=cmd_stat;
			cmd_buf.status.state=0;
			cmd_buf.status.src=CMD_SOURCE;
			cmd_buf.data.iValue=0;
			cmd_buf.ctrl_addr=dbcommpt.fixed.ctrl_addr;
			cmd_buf.host_id=GetHost_Cmd();
			cmd_buf.usr_id=0;

			tim=time (NULL);
			tmpt=*localtime (&tim);
			cmd_buf.second=tmpt.tm_sec;
			cmd_buf.minute=tmpt.tm_min;
			cmd_buf.hour=tmpt.tm_hour;
			cmd_buf.day=tmpt.tm_mday;
			cmd_buf.month=tmpt.tm_mon+1;
			cmd_buf.year=tmpt.tm_year+1900;
			strcpy(cmd_buf.usr_login_name,"IEC104S");
			lanhead.length=sizeof(COMMAND_MSG);

#ifdef CMD_TO_LCU
			lanhead.dp_type=COMM_TIME;
#ifdef DEBUG
			printf("SC Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#else
			lanhead.dp_type=COMM_RECORD_TIME;
#ifdef DEBUG
			printf("SC Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#endif
#ifdef SUN_OS
			Swap2Byte(&cmd_buf.point_id);
			Swap2Byte(&cmd_buf.status);
			Swap4Byte(&cmd_buf.data.iValue);
			Swap2Byte(&cmd_buf.ctrl_addr);
			Swap2Byte(&cmd_buf.year);
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dest_id =0;
			lanhead.src_id =cmd_buf.host_id;
			lanhead.dest_stn =cmd_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
/*
#ifdef DEBUG
			printf("SC lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#else
			bj_buf.ptr = dbcommpt.entry.ind.variable.ptr;
			tmpt=*localtime(&tim);
			bj_buf.data = 0;
			bj_buf.minute  =tmpt.tm_min;
			bj_buf.second  =tmpt.tm_sec;

			bj_buf.type_id =DPS_IND;
			bj_buf.stn_id =ptr.stn_id;
			bj_buf.dev_id =ptr.dev_id;
			bj_buf.data_type=ptr.data_type;
			bj_buf.number = 1;
			bj_buf.point_id = ptr.pt_id;

			bj_buf.status=state;
			bj_buf.ms=0;
			bj_buf.host_id=GetHost_Cmd();

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
			lanhead.dest_id =0;
			lanhead.src_id =bj_buf.host_id;
			lanhead.dest_stn =bj_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&bj_buf,lanhead,(unsigned char)0);
#ifdef DEBUG
			printf("SC lanout type_id:%d %d=%d=%d=%d %d\n",bj_buf.type_id,bj_buf.stn_id,bj_buf.ptr.dev_id,
					bj_buf.data_type,bj_buf.point_id,bj_buf.status.opr);
#endif
#endif
*/
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;

			ret = SinglePointCtrlAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* SC Actterm:10 */
/* 			cmdterm = 1; */
#ifdef REC_YF_AGC_CMD
			tmpt=*localtime(&tim);
			sprintf(agcrecfile,"/usr/ems/history/iec/v4.0a/log/AGC%02d.REC",tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
				fagc = fopen(agcrecfile,"w");
				daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(agcrecfile,"a");
			if (fagc==NULL)
			{
				printf ("iec104s:can't open(wb) the file %s!\n",agcrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d infoaddr=%04x %d %d %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,infoaddr,scdef.cmdtype[infoptr],cmd_stat,scdef.name[infoptr]);
				fclose(fagc);
			}
#endif
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
			cmdbuf.cot=(unsigned char)DEACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
/* 			seladdr=-1; */
#ifdef DEBUG
			printf("=====DEACT SC command addr=%04x received\n",infoaddr);
#endif
			ret = SinglePointCtrlAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* SC DeActCon:9 */
			return(ret);
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
/* 			seladdr=-1; */
#ifdef DEBUG
			printf("Unknown cause %d of SC command addr=%04x received\n",causetra,infoaddr);
#endif
			ret = SinglePointCtrlAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			return(ret);
		}
		break;

	case C_DC_NA:	/* 46 double point control command */

		qoc = recv_data.data[recvdatapt][9];	/* QOC:= S/E[8] QU[3..7] DCS[1..2] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
		cmdbuf.tpyeId = type_id;
		cmdbuf.infoaddr = infoaddr;
		cmdbuf.stat = qoc;
/* 		cmdbuf.ack_flag = 1;	 */	/* Set to 1 when ACK by CmdRespSend(int sock) */
		cmdbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(10 != recv_data.length[recvdatapt])
		{
			printf("received C_DC_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			cmdbuf.cot=(unsigned char)NACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
			ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-dc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=dcdef.number))
			{
				printf("%04x info address wrong! No this point in DC Control Table!\n",infoaddr);
				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = DoublePointCtrlAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				printf("The control privilege not in %s center! NACTCON DC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}

			strcpy(dbname,dcdef.name[infoptr]);
#ifdef DEBUG
			printf("recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,qoc,causetra);
#endif
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("%s No this DC control point in the database file!\n",dbname);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}
			if(qoc & 0x80)		/* Select Command */
			{
				if(((qoc & 0x3)==0)||((qoc & 0x3)==3))
				{
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
				}
				else
				{
					cmdbuf.cot=(unsigned char)ACTCON;
					cmdbuf.selected=1;
					cmdbuf.term_flag=0;
					cmdbuf.sel_tim=tim;
				}
#ifdef DEBUG
				printf("=====Select DC command %02x addr= %04x received\n",qoc,infoaddr);
#endif
				ret = DoublePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* DC ActCon */
				return(ret);
			}
/* 	Execute Command */
/* 			stat=(dbentry.entry.ind.variable.value.status&0x3c)>>2; */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period))
				{
#ifdef DEBUG
					printf("NACK Execute DC command infoaddr:%d != Select infoaddr:%d or Timeout %d\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));
#endif
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
					return(ret);
				}
			}
			if(((qoc & 0x3)==0)||((qoc & 0x3)==3))
			{
				printf("=====DC addr=%04x qoc=%d this operate is not allowed!\n",infoaddr,qoc);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;
			ret = DoublePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* DC ActCon:7 */
			cmd_stat = dcdef.optype[infoptr];
#ifdef DEBUG
			printf("DC command addr=%04X QOC=%02X cmd_stat=%d received\n",infoaddr,qoc,cmd_stat);
#endif
			cmd_buf.type_id = dcdef.cmdtype[infoptr];
			cmd_buf.stn_id = dbcommpt.point.stn_id;
			cmd_buf.dev_id = dbcommpt.point.dev_id;
			cmd_buf.data_type = dbcommpt.point.data_type;
			cmd_buf.point_id = dbcommpt.point.pt_id;

			cmd_buf.status.opr=cmd_stat;
			cmd_buf.status.state=0;
			cmd_buf.status.src=CMD_SOURCE;
			cmd_buf.data.iValue=0;
			cmd_buf.ctrl_addr=dbcommpt.fixed.ctrl_addr;
			cmd_buf.host_id=GetHost_Cmd();
			cmd_buf.usr_id=0;

			tim=time (NULL);
			tmpt=*localtime (&tim);
			cmd_buf.second=tmpt.tm_sec;
			cmd_buf.minute=tmpt.tm_min;
			cmd_buf.hour=tmpt.tm_hour;
			cmd_buf.day=tmpt.tm_mday;
			cmd_buf.month=tmpt.tm_mon+1;
			cmd_buf.year=tmpt.tm_year+1900;
			strcpy(cmd_buf.usr_login_name,"IEC104S");
			lanhead.length=sizeof(COMMAND_MSG);

#ifdef CMD_TO_LCU
			lanhead.dp_type=COMM_TIME;
#ifdef DEBUG
			printf("DC Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#else
			lanhead.dp_type=COMM_RECORD_TIME;
#ifdef DEBUG
			printf("DC Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#endif
#ifdef SUN_OS
			Swap2Byte(&cmd_buf.point_id);
			Swap2Byte(&cmd_buf.status);
			Swap4Byte(&cmd_buf.data.iValue);
			Swap2Byte(&cmd_buf.ctrl_addr);
			Swap2Byte(&cmd_buf.year);
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dest_id =0;
			lanhead.src_id =cmd_buf.host_id;
			lanhead.dest_stn =cmd_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
/*
#ifdef DEBUG
			printf("DC lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#else
			bj_buf.ptr = dbcommpt.entry.ind.variable.ptr;
			tmpt=*localtime(&tim);
			bj_buf.data = 0;
			bj_buf.minute  =tmpt.tm_min;
			bj_buf.second  =tmpt.tm_sec;

			bj_buf.type_id =DPS_IND;
			bj_buf.stn_id =ptr.stn_id;
			bj_buf.dev_id =ptr.dev_id;
			bj_buf.data_type=ptr.data_type;
			bj_buf.number = 1;
			bj_buf.point_id = ptr.pt_id;

			bj_buf.status=state;
			bj_buf.ms=0;
			bj_buf.host_id=GetHost_Cmd();

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
			lanhead.dest_id =0;
			lanhead.src_id =bj_buf.host_id;
			lanhead.dest_stn =bj_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&bj_buf,lanhead,(unsigned char)0);
#ifdef DEBUG
			printf("DC lanout type_id:%d %d=%d=%d=%d %d\n",bj_buf.type_id,bj_buf.stn_id,bj_buf.ptr.dev_id,
					bj_buf.data_type,bj_buf.point_id,bj_buf.status.opr);
#endif
#endif
*/
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;
			ret = DoublePointCtrlAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* DC ACTTERM:10 */
/* 			cmdterm = 1; */
#ifdef REC_YF_AGC_CMD
			tmpt=*localtime(&tim);
			sprintf(agcrecfile,"/usr/ems/history/iec/v4.0a/log/AGC%02d.REC",tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
				fagc = fopen(agcrecfile,"w");
				daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(agcrecfile,"a");
			if (fagc==NULL)
			{
				printf ("iec104s:can't open(wb) the file %s!\n",agcrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d infoaddr=%04x %d %d %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,infoaddr,dcdef.cmdtype[infoptr],cmd_stat,dcdef.name[infoptr]);
				fclose(fagc);
			}
#endif
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
			cmdbuf.cot=(unsigned char)DEACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
/* 			seladdr=-1; */
#ifdef DEBUG
			printf("=====DEACT DC command addr=%04x received\n",infoaddr);
#endif
			ret = DoublePointCtrlAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* DC DeActCon:9 */
			return(ret);
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
/* 			seladdr=-1; */
#ifdef DEBUG
			printf("Unknown cause %d of DC command addr=%04x received\n",causetra,infoaddr);
#endif
			ret = DoublePointCtrlAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			return(ret);
		}
		break;

	case C_RC_NA:	/* 47 Regulating Step Control Command */

		qoc = recv_data.data[recvdatapt][9];	/* QOC:= S/E[8] QU[3..7] RCS[1..2] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
		cmdbuf.tpyeId = type_id;
		cmdbuf.infoaddr = infoaddr;
		cmdbuf.stat = qoc;
/* 		cmdbuf.ack_flag = 1;	 */	/* Set to 1 when ACK by CmdRespSend(int sock) */
		cmdbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(10 != recv_data.length[recvdatapt])
		{
			printf("received C_RC_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			cmdbuf.cot=(unsigned char)NACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
			ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-rc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=rcdef.number))
			{
				printf("%04x info address wrong! base=%04x No this point in RC Control Table num=%d!\n",infoaddr,rc_cmd_base_addr,rcdef.number);
				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = RegulatStepCtrlAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* RC NActCon */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				printf("The control privilege not in %s center! NACTCON RC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}

			strcpy(dbname,rcdef.name[infoptr]);
#ifdef DEBUG
			printf("recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,qoc,causetra);
#endif
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("%s No this RC control point in the database file!\n",dbname);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
				return(ret);
			}
			if(qoc & 0x80)		/* Select Command */
			{
				if(((qoc & 0x3)==0)||((qoc & 0x3)==3))
				{
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
				}
				else
				{
					cmdbuf.cot=(unsigned char)ACTCON;
					cmdbuf.selected=1;
					cmdbuf.term_flag=0;
					cmdbuf.sel_tim=tim;
				}
#ifdef DEBUG
				printf("=====Select RC command %02x addr= %04x received\n",qoc,infoaddr);
#endif
				ret = RegulatStepCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
				return(ret);
			}
/* 	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period))
				{
#ifdef DEBUG
					printf("NACK Execute RC command infoaddr:%d != Select infoaddr:%d or Timeout %d\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));
#endif
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
					return(ret);
				}
			}
			if(((qoc & 0x3)==0)||((qoc & 0x3)==3))
			{
				printf("=====RC addr=%04x qoc=%d this operate is not allowed!\n",infoaddr,qoc);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
				return(ret);
			}
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;
			ret = RegulatStepCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* RC ActCon:7 */
			cmd_stat = rcdef.optype[infoptr];
#ifdef DEBUG
			printf("RC command addr=%04X QOC=%02X cmd_stat=%d received\n",infoaddr,qoc,cmd_stat);
#endif
			cmd_buf.type_id = rcdef.cmdtype[infoptr];
			cmd_buf.stn_id = dbcommpt.point.stn_id;
			cmd_buf.dev_id = dbcommpt.point.dev_id;
			cmd_buf.data_type = dbcommpt.point.data_type;
			cmd_buf.point_id = dbcommpt.point.pt_id;

			cmd_buf.status.opr=cmd_stat;
			cmd_buf.status.state=0;
			cmd_buf.status.src=CMD_SOURCE;
			cmd_buf.data.iValue=0;
			cmd_buf.ctrl_addr=dbcommpt.fixed.ctrl_addr;
			cmd_buf.host_id=GetHost_Cmd();
			cmd_buf.usr_id=0;

			tim=time (NULL);
			tmpt=*localtime (&tim);
			cmd_buf.second=tmpt.tm_sec;
			cmd_buf.minute=tmpt.tm_min;
			cmd_buf.hour=tmpt.tm_hour;
			cmd_buf.day=tmpt.tm_mday;
			cmd_buf.month=tmpt.tm_mon+1;
			cmd_buf.year=tmpt.tm_year+1900;
			strcpy(cmd_buf.usr_login_name,"IEC104S");
			lanhead.length=sizeof(COMMAND_MSG);

#ifdef CMD_TO_LCU
			lanhead.dp_type=COMM_TIME;
#ifdef DEBUG
			printf("RC Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#else
			lanhead.dp_type=COMM_RECORD_TIME;
#ifdef DEBUG
			printf("RC Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#endif
#ifdef SUN_OS
			Swap2Byte(&cmd_buf.point_id);
			Swap2Byte(&cmd_buf.status);
			Swap4Byte(&cmd_buf.data.iValue);
			Swap2Byte(&cmd_buf.ctrl_addr);
			Swap2Byte(&cmd_buf.year);
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dest_id =0;
			lanhead.src_id =cmd_buf.host_id;
			lanhead.dest_stn =cmd_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
/*
#ifdef DEBUG
			printf("RC lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#endif
#else
			bj_buf.ptr = dbcommpt.entry.ind.variable.ptr;
			tmpt=*localtime(&tim);
			bj_buf.data = 0;
			bj_buf.minute  =tmpt.tm_min;
			bj_buf.second  =tmpt.tm_sec;

			bj_buf.type_id =DPS_IND;
			bj_buf.stn_id =ptr.stn_id;
			bj_buf.dev_id =ptr.dev_id;
			bj_buf.data_type=ptr.data_type;
			bj_buf.number = 1;
			bj_buf.point_id = ptr.pt_id;

			bj_buf.status=state;
			bj_buf.ms=0;
			bj_buf.host_id=GetHost_Cmd();

#ifdef DEBUG
			printf("RC lanout type_id:%d %d=%d=%d=%d %d\n",bj_buf.type_id,bj_buf.stn_id,bj_buf.ptr.dev_id,
					bj_buf.data_type,bj_buf.point_id,bj_buf.status.opr);
#endif
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
			lanhead.dest_id =0;
			lanhead.src_id =bj_buf.host_id;
			lanhead.dest_stn =bj_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&bj_buf,lanhead,(unsigned char)0);
#endif
*/
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;
			ret = RegulatStepCtrlAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* RC ACTTERM:10 */
/* 			cmdterm = 1; */
#ifdef REC_YF_AGC_CMD
			tmpt=*localtime(&tim);
			sprintf(agcrecfile,"/usr/ems/history/iec/v4.0a/log/AGC%02d.REC",tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
				fagc = fopen(agcrecfile,"w");
				daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(agcrecfile,"a");
			if (fagc==NULL)
			{
				printf ("iec104s:can't open(wb) the file %s!\n",agcrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d infoaddr=%04x %d %d %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,infoaddr,dcdef.cmdtype[infoptr],cmd_stat,rcdef.name[infoptr]);
				fclose(fagc);
			}
#endif
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
			cmdbuf.cot=(unsigned char)DEACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
/* 			seladdr=-1; */
#ifdef DEBUG
			printf("=====DEACT RC command addr=%04x received\n",infoaddr);
#endif
			ret = RegulatStepCtrlAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* RC DeActCon:9 */
			return(ret);
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
/* 			seladdr=-1; */
#ifdef DEBUG
			printf("Unknown cause %d of RC command addr=%04x received\n",causetra,infoaddr);
#endif
			ret = RegulatStepCtrlAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			return(ret);
		}
		break;

	case C_SE_NA:	/* 48 confirm of set-point command normalized value */

		infonum = varstru & 0x7F;
		qoc = recv_data.data[recvdatapt][11];	/* QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#endif
		sebuf.tpyeId = type_id;
/* 		sebuf.infonum = infonum; */
/* 		sebuf.rcvpnt = recvdatapt; */
		sebuf.infoaddr = infoaddr;
		sebuf.setvalue = setvalue;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	 *//* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;	/* Set to 0 when ACK directly. */

		if(12 != recv_data.length[recvdatapt])
		{
			printf("received C_SE_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)NACTCON;
			ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_base_addr;
			if((infoptr<0)||(infoptr>=sedef.number))
			{
				printf("!!!!!C_SE_NA:%04x info address wrong! No this point in SE Table!\n",infoaddr);
/* 				pn = 0x40;	/* NACK
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = SetPointCmdNAAck((unsigned char)(0x40|UKINFOAD),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				printf("The control privilege not in %s center! NACTCON C_SE_NA cmd %d\n",sProcRec[pnum].NameID,infoptr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}

			strcpy(dbname,sedef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("C_SE_NA:%s No this set point in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				printf("C_SE_NA:%s this point is not ANA in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
			if(qoc & 0x80)
			{
#ifdef DEBUG
				printf("?????Select C_SE_NA: addr=%04x value = %d Received\n",infoaddr,setvalue);
#endif
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				ret = SetPointCmdNAAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
#ifdef DEBUG
			printf("#####Set-Point C_SE_NA: addr=%04x value = %d Received\n",infoaddr,setvalue);
#endif
			if(SEType == 0)
			{
				value=(float)((setvalue*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/32767.0
					 + sedef.lorange[infoptr]);
			}
			else
			{
/*
				value=(float)((setvalue*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
					 + (sedef.hirange[infoptr]+sedef.lorange[infoptr])/2.0);
*/
				value=(float)(((setvalue+32768)*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
					 + sedef.lorange[infoptr]);
			}
			cmd_buf.type_id = sedef.cmdtype[infoptr];
			cmd_buf.stn_id = dbcommpt.point.stn_id;
			cmd_buf.dev_id = dbcommpt.point.dev_id;
			cmd_buf.data_type = dbcommpt.point.data_type;
			cmd_buf.point_id = dbcommpt.point.pt_id;

			cmd_buf.status.opr=0;
			cmd_buf.status.state=0;
			cmd_buf.status.src=CMD_SOURCE;
			cmd_buf.data.iValue=(int)(value*1000);
			cmd_buf.ctrl_addr=dbcommpt.fixed.ctrl_addr;
			cmd_buf.host_id=GetHost_Cmd();
			cmd_buf.usr_id=0;

			tim=time (NULL);
			tmpt=*localtime (&tim);
			cmd_buf.second=tmpt.tm_sec;
			cmd_buf.minute=tmpt.tm_min;
			cmd_buf.hour=tmpt.tm_hour;
			cmd_buf.day=tmpt.tm_mday;
			cmd_buf.month=tmpt.tm_mon+1;
			cmd_buf.year=tmpt.tm_year+1900;
			strcpy(cmd_buf.usr_login_name,"IEC104S");
			lanhead.length=sizeof(COMMAND_MSG);

#ifdef CMD_TO_LCU
			lanhead.dp_type=COMM_TIME;
#ifdef DEBUG
			printf("Set Point C_SE_NA Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
				cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#else
			lanhead.dp_type=COMM_RECORD_TIME;
#ifdef DEBUG
			printf("Set Point C_SE_NA Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
				cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#endif
#ifdef SUN_OS
			Swap2Byte(&cmd_buf.point_id);
			Swap2Byte(&cmd_buf.status);
			Swap4Byte(&cmd_buf.data.iValue);
			Swap2Byte(&cmd_buf.ctrl_addr);
			Swap2Byte(&cmd_buf.year);
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dest_id =0;
			lanhead.src_id =cmd_buf.host_id;
			lanhead.dest_stn =cmd_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);

			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)ACTCON;
			ret = SetPointCmdNAAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
#ifdef REC_YF_AGC_CMD
			sprintf(agcrecfile,"/usr/ems/history/iec/v4.0a/log/AGC%02d.REC",tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
					fagc = fopen(agcrecfile,"w");
					daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(agcrecfile,"a");
			if (fagc==NULL)
			{
				printf ("iec104s:can't open(wb) the file %s!\n",agcrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %04x %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value,infoaddr,setvalue,sedef.name[infoptr]);
				fclose(fagc);
			}
#endif
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			ret = SetPointCmdNAAck((unsigned char)DEACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point UKCOT qoc:0x00 */
			return(ret);
		}
		else
		{
#ifdef DEBUG
			printf("Cause of C_SE_NA: received %d wrong!!!\n",causetra);
#endif
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			ret = SetPointCmdNAAck((unsigned char)(0x40|UKCOT),infoaddr,setvalue,qoc,sockfd);	/* SET Point UKCOT qoc:0x00 */
			return(ret);
		}
		break;

	case C_SE_NB:	/* 49 confirm of set-point command scaled value */

		qoc = recv_data.data[recvdatapt][11];	/* QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#endif
		sebuf.tpyeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.setvalue = setvalue;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	 *//* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;	/* Set to 0 when ACK directly. */

		if(12 != recv_data.length[recvdatapt])
		{
			printf("received C_SE_NB:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)NACTCON;
			ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_scale_addr;
			if((infoptr<0)||(infoptr>=sescaledef.number))
			{
				printf("!!!!!C_SE_NB:%04x info address wrong! No this point in SE Table!\n",infoaddr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = SetPointCmdNBAck((unsigned char)(0x40|UKINFOAD),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				printf("The control privilege not in %s center! NACTCON C_SE_NB cmd %d\n",sProcRec[pnum].NameID,infoptr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}

			strcpy(dbname,sescaledef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("C_SE_NB:%s No this set point in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				printf("C_SE_NB:%s this point is not ANA in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if(qoc & 0x80)
			{
#ifdef DEBUG
				printf("?????Select C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
#endif
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				ret = SetPointCmdNBAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

#ifdef DEBUG
			printf("#####Set-Point C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
#endif
			value= setvalue * sescaledef.scale[infoptr];
			cmd_buf.type_id = sescaledef.cmdtype[infoptr];
			cmd_buf.stn_id = dbcommpt.point.stn_id;
			cmd_buf.dev_id = dbcommpt.point.dev_id;
			cmd_buf.data_type = dbcommpt.point.data_type;
			cmd_buf.point_id = dbcommpt.point.pt_id;

			cmd_buf.status.opr=0;
			cmd_buf.status.state=0;
			cmd_buf.status.src=CMD_SOURCE;
			cmd_buf.data.iValue=(int)(value*1000);
			cmd_buf.ctrl_addr=dbcommpt.fixed.ctrl_addr;
			cmd_buf.host_id=GetHost_Cmd();
			cmd_buf.usr_id=0;

			tim=time (NULL);
			tmpt=*localtime (&tim);
			cmd_buf.second=tmpt.tm_sec;
			cmd_buf.minute=tmpt.tm_min;
			cmd_buf.hour=tmpt.tm_hour;
			cmd_buf.day=tmpt.tm_mday;
			cmd_buf.month=tmpt.tm_mon+1;
			cmd_buf.year=tmpt.tm_year+1900;
			strcpy(cmd_buf.usr_login_name,"IEC104S");
			lanhead.length=sizeof(COMMAND_MSG);

#ifdef CMD_TO_LCU
			lanhead.dp_type=COMM_TIME;
#ifdef DEBUG
			printf("Set Point C_SE_NB Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
				cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#else
			lanhead.dp_type=COMM_RECORD_TIME;
#ifdef DEBUG
			printf("Set Point C_SE_NB Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
				cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#endif
#ifdef SUN_OS
			Swap2Byte(&cmd_buf.point_id);
			Swap2Byte(&cmd_buf.status);
			Swap4Byte(&cmd_buf.data.iValue);
			Swap2Byte(&cmd_buf.ctrl_addr);
			Swap2Byte(&cmd_buf.year);
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dest_id =0;
			lanhead.src_id =cmd_buf.host_id;
			lanhead.dest_stn =cmd_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);

			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)ACTCON;
			ret = SetPointCmdNBAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00
#ifdef REC_YF_AGC_CMD
			sprintf(agcrecfile,"/usr/ems/history/iec/v4.0a/log/AGC%02d.REC",tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
				fagc = fopen(agcrecfile,"w");
				daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(agcrecfile,"a");
			if (fagc==NULL)
			{
				printf ("iec104s:can't open(wb) the file %s!\n",agcrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %04x\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value,infoaddr,setvalue);
				fclose(fagc);
			}
#endif
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
#ifdef DEBUG
			printf("=====DEACT Cause of C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
#endif
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			ret = SetPointCmdNBAck((unsigned char)DEACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			return(ret);
		}
		else
		{
#ifdef DEBUG
			printf("Cause of C_SE_NB: received %d wrong!!!\n",causetra);
#endif
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			ret = SetPointCmdNBAck((unsigned char)(0x40|UKCOT),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			return(ret);
		}
		break;

	case C_SE_NC:	/* 50 confirm of set-point command float value

		qoc = recv_data.data[recvdatapt][13];			/* QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&value,&recv_data.data[recvdatapt][9],4);	/* value of float */
#endif
		sebuf.tpyeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.fvalue = value;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	*/	/* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(14 != recv_data.length[recvdatapt])
		{
			printf("received C_SE_NC:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)NACTCON;
			ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_float_addr;
			if((infoptr<0)||(infoptr>=sefloatdef.number))
			{
				printf("!!!!!C_SE_NC:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = SetPointCmdNCAck((unsigned char)(0x40|UKINFOAD),infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				printf("The control privilege not in %s center! NACTCON C_SE_NC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

			strcpy(dbname,sefloatdef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("C_SE_NC:%s No this set point in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				printf("C_SE_NC:%s this point is not ANA in the database file!\n",dbname);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if(qoc & 0x80)
			{
#ifdef DEBUG
				printf("?????Select C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);
#endif
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				ret = SetPointCmdNCAck((unsigned char)ACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

#ifdef DEBUG
			printf("#####Set-Point C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);
#endif
			cmd_buf.type_id = sefloatdef.cmdtype[infoptr];
			cmd_buf.stn_id = dbcommpt.point.stn_id;
			cmd_buf.dev_id = dbcommpt.point.dev_id;
			cmd_buf.data_type = dbcommpt.point.data_type;
			cmd_buf.point_id = dbcommpt.point.pt_id;

			cmd_buf.status.opr=0;
			cmd_buf.status.state=0;
			cmd_buf.status.src=CMD_SOURCE;
			cmd_buf.data.iValue=(int)(value*1000);
			cmd_buf.ctrl_addr=dbcommpt.fixed.ctrl_addr;
			cmd_buf.host_id=GetHost_Cmd();
			cmd_buf.usr_id=0;

			tim=time (NULL);
			tmpt=*localtime (&tim);
			cmd_buf.second=tmpt.tm_sec;
			cmd_buf.minute=tmpt.tm_min;
			cmd_buf.hour=tmpt.tm_hour;
			cmd_buf.day=tmpt.tm_mday;
			cmd_buf.month=tmpt.tm_mon+1;
			cmd_buf.year=tmpt.tm_year+1900;
			strcpy(cmd_buf.usr_login_name,"IEC104S");
			lanhead.length=sizeof(COMMAND_MSG);

#ifdef CMD_TO_LCU
			lanhead.dp_type=COMM_TIME;
#ifdef DEBUG
			printf("Set Point C_SE_NC Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
				cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#else
			lanhead.dp_type=COMM_RECORD_TIME;
#ifdef DEBUG
			printf("Set Point C_SE_NC Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
				cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#endif
#ifdef SUN_OS
			Swap2Byte(&cmd_buf.point_id);
			Swap2Byte(&cmd_buf.status);
			Swap4Byte(&cmd_buf.data.iValue);
			Swap2Byte(&cmd_buf.ctrl_addr);
			Swap2Byte(&cmd_buf.year);
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dest_id =0;
			lanhead.src_id =cmd_buf.host_id;
			lanhead.dest_stn =cmd_buf.stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);


			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)ACTCON;
			ret = SetPointCmdNCAck((unsigned char)ACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
#ifdef REC_YF_AGC_CMD
			tmpt=*localtime(&tim);
			sprintf(agcrecfile,"/usr/ems/history/iec/v4.0a/log/AGC%02d.REC",tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
				fagc = fopen(agcrecfile,"w");
				daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(agcrecfile,"a");
			if (fagc==NULL)
			{
				printf ("iec104s:can't open(wb) the file %s!\n",agcrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value,infoaddr,sefloatdef.name[infoptr]);
				fclose(fagc);
			}
#endif
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
#ifdef DEBUG
			printf("=====DEACT Cause of C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);
#endif
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			ret = SetPointCmdNCAck((unsigned char)DEACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		else
		{
#ifdef DEBUG
			printf("Cause of C_SE_NC: received %d wrong!!!\n",causetra);
#endif
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			ret = SetPointCmdNCAck((unsigned char)(0x40|UKCOT),infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		break;

	case C_SE_ND:	/* 136 confirm of multi set-point command normalized value

		infonum = varstru & 0x7F;
		qoc = recv_data.data[recvdatapt][6+infonum*5];	/* QOC:= S/E[8] QL[1..7] */

		sebuf.tpyeId = type_id;
		sebuf.infonum = infonum;
		sebuf.rcvpnt = recvdatapt;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	 */	/* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		infonum = varstru & 0x7F;			/* number of information object */
		datalength = 5*infonum+7;
		if(datalength != recv_data.length[recvdatapt])
		{
			printf("received C_SE_ND:ASDU length error! type_id=%d,length:%u != %u\n",
					type_id,recv_data.length[recvdatapt],datalength);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)NACTCON;
			ret = SetPointCmdNDAck((unsigned char)NACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			if(qoc & 0x80)
			{
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				ret = SetPointCmdNDAck((unsigned char)ACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
				return(ret);
			}

			for(objpnt=0;objpnt<infonum;objpnt++)
			{
#ifdef SUN_OS
				CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6+objpnt*5],2);	/* address of object */
				CrossCopy(&setvalue,&recv_data.data[recvdatapt][9+objpnt*5],2);	 /* value of set */
#else
				memcpy(&infoaddr,&recv_data.data[recvdatapt][6+objpnt*5],2);	 /* address of object */
				memcpy(&setvalue,&recv_data.data[recvdatapt][9+objpnt*5],2);	 /* value of set */
#endif
				infoptr = infoaddr - se_cmd_base_addr;
				if((infoptr<0)||(infoptr>=sedef.number))
				{
					printf("!!!!!C_SE_ND:%04x info address wrong! No this point in SET Point Control Table!\n",infoaddr);
/* 					pn = 0x40;	/* NACK */
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)(0x40|UKINFOAD);
					ret = SetPointCmdNDAck((unsigned char)(0x40|UKINFOAD),infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:6F(111) qoc:0x00 */
					return(ret);
				}

#ifdef DEBUG
				printf("#####Multi Set-Point C_SE_ND: addr=%04x value= %d Received\n",infoaddr,setvalue);
#endif

				if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
				{
					printf("The control privilege not in %s center! NACTCON C_SE_ND cmd %d\n",sProcRec[pnum].NameID,infoptr);
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)NACTCON;
					ret = SetPointCmdNDAck((unsigned char)NACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:0x47 qoc:0x00 */
					return(ret);
				}

				if(SEType == 0)
				{
					value=(float)((setvalue*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/32767.0
						 + sedef.lorange[infoptr]);
				}
				else
				{
					value=(float)((setvalue*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
						 + (sedef.hirange[infoptr]+sedef.lorange[infoptr])/2.0);
				}
				strcpy(dbname,sedef.name[infoptr]);
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("C_SE_ND:%s No this set point in the database file!\n",dbname);
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)NACTCON;
					ret = SetPointCmdNDAck((unsigned char)NACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:0x47 qoc:0x00 */
					continue;
				}
				if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
				{
					printf("C_SE_ND:%s this point is not ANA in the database file!\n",dbname);
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)NACTCON;
					ret = SetPointCmdNDAck((unsigned char)NACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:0x47 qoc:0x00 */
					continue;
				}

				cmd_buf.type_id = sescaledef.cmdtype[infoptr];
				cmd_buf.stn_id = dbcommpt.point.stn_id;
				cmd_buf.dev_id = dbcommpt.point.dev_id;
				cmd_buf.data_type = dbcommpt.point.data_type;
				cmd_buf.point_id = dbcommpt.point.pt_id;

				cmd_buf.status.opr=0;
				cmd_buf.status.state=0;
				cmd_buf.status.src=CMD_SOURCE;
				cmd_buf.data.iValue=(int)(value*1000);
				cmd_buf.ctrl_addr=dbcommpt.fixed.ctrl_addr;
				cmd_buf.host_id=GetHost_Cmd();
				cmd_buf.usr_id=0;

				tim=time (NULL);
				tmpt=*localtime (&tim);
				cmd_buf.second=tmpt.tm_sec;
				cmd_buf.minute=tmpt.tm_min;
				cmd_buf.hour=tmpt.tm_hour;
				cmd_buf.day=tmpt.tm_mday;
				cmd_buf.month=tmpt.tm_mon+1;
				cmd_buf.year=tmpt.tm_year+1900;
				strcpy(cmd_buf.usr_login_name,"IEC104S");
				lanhead.length=sizeof(COMMAND_MSG);

#ifdef CMD_TO_LCU
				lanhead.dp_type=COMM_TIME;
#ifdef DEBUG
				printf("Multi Set Point C_SE_ND Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#else
				lanhead.dp_type=COMM_RECORD_TIME;
#ifdef DEBUG
				printf("Multi Set Point C_SE_ND Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
					cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#endif
#endif
#ifdef SUN_OS
				Swap2Byte(&cmd_buf.point_id);
				Swap2Byte(&cmd_buf.status);
				Swap4Byte(&cmd_buf.data);
				Swap2Byte(&cmd_buf.ctrl_addr);
				Swap2Byte(&cmd_buf.year);
				Swap2Byte(&lanhead.length);
#endif
				lanhead.dest_id =0;
				lanhead.src_id =cmd_buf.host_id;
				lanhead.dest_stn =cmd_buf.stn_id;
				lanhead.packet_num =0;
				lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);

				if(!(objpnt%10))
					sleep(1);
				lan_out((char *)&cmd_buf,lanhead,(unsigned char)0);
#ifdef REC_YF_AGC_CMD
				sprintf(agcrecfile,"/usr/ems/history/iec/v4.0a/log/AGC%02d.REC",tmpt.tm_mday);
				if(daysaved != tmpt.tm_mday)
				{
					fagc = fopen(agcrecfile,"w");
					daysaved = tmpt.tm_mday;
				}
				else
					fagc = fopen(agcrecfile,"a");
				if (fagc==NULL)
				{
					printf ("iec104s:can't open(wb) the file %s!\n",agcrecfile);
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
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)ACTCON;
			ret = SetPointCmdNDAck((unsigned char)ACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
#ifdef DEBUG
#ifdef SUN_OS
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			printf("=====DEACT Cause of C_SE_ND: infoaddr= %04x infonum=%d Received\n",infoaddr,infonum);
#endif
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			ret = SetPointCmdNDAck((unsigned char)DEACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		else
		{
#ifdef DEBUG
			printf("Cause of C_SE_ND command received %d wrong!!!\n",causetra);
#endif
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			ret = SetPointCmdNDAck((unsigned char)(0x40|UKCOT),infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		break;

	case M_ME_NC:	/* measure value without time tag:13
	case M_ME_TC:	/* measure value with time tag:14
	case M_ME_TF:	/* 36:measure value with time CP56Time2a,float value*/

		infonum = varstru & 0x7F;			/* number of information object */
		if(varstru & 0x80)	/* SQ=1 */
		{
			if(type_id == M_ME_NC)
			{
				lengthfactor = 5;
			}
			else if(type_id == M_ME_TC)
				lengthfactor = 8;
			else
				lengthfactor = 12;
			datalength = lengthfactor*infonum+9;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				return(FALSE);
			}

			lanpt = 0;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif

			infoptr = infoaddr - se_cmd_float_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr<0)||(infoptr >= sefloatdef.number))
				{
#ifdef DEBUG
					printf("M_ME_NC SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,sefloatdef.number);
#endif			
					return(FALSE);
				}
				strcpy(dbname,sefloatdef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+4];

#ifdef SUN
				CrossCopy(&value,&recv_data.data[recvdatapt][datapoint],4);
#else
				memcpy(&value,&recv_data.data[recvdatapt][datapoint],4);
#endif

				infoptr ++;

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.man_set = 1;	/* manual set */
				if(qoc & 0xD0)
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
#ifdef SUN
				CrossCopy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				CrossCopy(&me_buf[lanpt*22+8], &anadata.status,2);
				CrossCopy(&me_buf[lanpt*22+10], &anadata.data,4);
#else
				memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
				memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
#endif
				me_buf[lanpt*22+14]=GetHost_Cmd();	/* host_id */

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
#ifdef SUN
					CrossCopy(&ms,&recv_data.data[recvdatapt][datapoint+5],2);
#else
					memcpy(&ms,&recv_data.data[recvdatapt][datapoint+5],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+7] & 0x3F;	/* minute */
					if(type_id ==M_ME_TC)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TF */
					{
						me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
						year=recv_data.data[recvdatapt][datapoint+11] & 0x7F+2000;		/* year */
					}
				}
#ifdef SUN
				CrossCopy(&me_buf[lanpt*22+20],&year,2);
#else
				memcpy(&me_buf[lanpt*22+20],&year,2);
#endif
				lanpt ++;
				if(lanpt>=64)
				{
					lanhead.length = lanpt*22;
#ifdef SUN
					Swap2Byte(&lanhead.length);
#endif
					if(causetra == SPONT)
						lanhead.dp_type=BJ_DB_TIME;
					else
						lanhead.dp_type=ALL_DB_TIME;
					lanhead.src_id =GetHost_Cmd();
					lanhead.dest_id =0;
					lanhead.dest_stn =me_buf[1];
					lanhead.packet_num =0;
					lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
					lanpt = 0;
					sleep(1);
				}
			}
		}
		else		/* SQ=0 */
		{
			if(type_id == M_ME_NC)
			{
/*
 				tim=time(NULL);
 				tmpt=*localtime(&tim);
*/
				lengthfactor = 8;
			}
			else if(type_id == M_ME_TC)
				lengthfactor = 11;
			else
				lengthfactor = 15;
			datalength = lengthfactor*infonum+6;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				return(FALSE);
			}

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;

#ifdef SUN
				CrossCopy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
				infoptr = infoaddr - se_cmd_float_addr;
				if((infoptr<0)||(infoptr > sefloatdef.number))
				{
#ifdef DEBUG
					printf("M_ME_NC SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,sefloatdef.number);
#endif
					return(FALSE);
				}
				strcpy(dbname,sefloatdef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+7];
#ifdef SUN
				CrossCopy(&value,&recv_data.data[recvdatapt][datapoint+3],4);
#else
				memcpy(&value,&recv_data.data[recvdatapt][datapoint+3],4);
#endif
				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.man_set = 1;	/* manual set */
				if(qoc & 0xD0)
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
#ifdef SUN
				CrossCopy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				CrossCopy(&me_buf[lanpt*22+8], &anadata.status,2);
				CrossCopy(&me_buf[lanpt*22+10], &anadata.data,4);
#else
				memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
				memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
#endif
				me_buf[lanpt*22+14]=GetHost_Cmd();	/* host_id */

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
#ifdef SUN
					CrossCopy(&ms,&recv_data.data[recvdatapt][datapoint+8],2);
#else
					memcpy(&ms,&recv_data.data[recvdatapt][datapoint+8],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+10] & 0x3F;	/* minute */
					if(type_id ==M_ME_TC)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TF */
					{
						me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+11] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+12] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+13] & 0x0F;	/* month */
						year=recv_data.data[recvdatapt][datapoint+14] & 0x7F+2000;		/* year */
					}
				}
#ifdef SUN
				CrossCopy(&me_buf[lanpt*22+20],&year,2);
#else
				memcpy(&me_buf[lanpt*22+20],&year,2);
#endif
				lanpt ++;
				if(lanpt>=64)
				{
					lanhead.length = lanpt*22;
#ifdef SUN
					Swap2Byte(&lanhead.length);
#endif
					if(causetra == SPONT)
						lanhead.dp_type=BJ_DB_TIME;
					else
						lanhead.dp_type=ALL_DB_TIME;
					lanhead.src_id =GetHost_Cmd();
					lanhead.dest_id =0;
					lanhead.dest_stn =me_buf[1];
					lanhead.packet_num =0;
					lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
					lanpt = 0;
					sleep(1);
				}
			}
		}
		if(lanpt>0)
		{
			lanhead.length = lanpt*22;
#ifdef SUN
			Swap2Byte(&lanhead.length);
#endif
			if(causetra == SPONT)
				lanhead.dp_type=BJ_DB_TIME;
			else
				lanhead.dp_type=ALL_DB_TIME;
			lanhead.src_id =GetHost_Cmd();
			lanhead.dest_id =0;
			lanhead.dest_stn =me_buf[1];
			lanhead.packet_num =0;
			lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
		}		
		return(TRUE);
		break;

	case P_ME_NA:	/* 110 confirm of Parameter-value command
					
/* 		infonum = varstru & 0x7F;
		qoc = recv_data.data[recvdatapt][11];			/* QPM:= POP[8]:1 LPC[7] KPA[1..6] */
#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#endif
		pmbuf.tpyeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.setvalue = setvalue;
		pmbuf.qpm = qoc;		/* C dir POP=1 M dir LPC=1 POP=0 */
/* 		pmbuf.ack_flag = 1;		/* Set to 1 when ACK by PMRespSend(int sock) */
		pmbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(12 != recv_data.length[recvdatapt])
		{
			printf("received P_ME_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			pmbuf.cot=(unsigned char)NACTCON;
			ret = ParamValueNAAck((unsigned char)NACTCON,qoc,infoaddr,setvalue,sockfd);
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr;
			if((infoptr<0)||(infoptr>=medef.number))
			{
				printf("!!!!!P_ME_NA:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = ParamValueNAAck((unsigned char)(0x40|UKINFOAD),qoc,infoaddr,setvalue,sockfd);
				return(ret);
			}

			if(SEType == 0)
			{
				value=(float)((setvalue*(medef.hirange[infoptr]-medef.lorange[infoptr]))/32767.0
					 + medef.lorange[infoptr]);
			}
			else
			{
				value=(float)((setvalue*(medef.hirange[infoptr]-medef.lorange[infoptr]))/65535.0
					 + (medef.hirange[infoptr]+medef.lorange[infoptr])/2.0);
			}
#ifdef DEBUG
			printf("##### P_ME_NA: hi=%f lo=%f ptr=%d \n",medef.hirange[infoptr],medef.lorange[infoptr],infoptr);
			printf("##### P_ME_NA: addr=%04X setvalue=%04X value=%f qpm=%02X Received\n",infoaddr,setvalue,value,qoc);
#endif
			switch(qoc & 0x3F)
			{
			case	1:

				databand = setvalue;
				medef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMETable(MEDEF_NORM); */
#ifdef DEBUG
				printf("Parameter threshold command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
#ifdef DEBUG
				printf("Parameter smothing factor command P_ME_NA: addr=%04x value = %04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			case	3:

/* 				medef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMETable(MEDEF_NORM); */
#ifdef DEBUG
				printf("Parameter low limit command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			case	4:

/* 				medef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMETable(MEDEF_NORM); */
#ifdef DEBUG
				printf("Parameter high limit command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				ret = ParamValueNAAck((unsigned char)NACTCON,qoc,infoaddr,setvalue,sockfd);
#ifdef DEBUG
				printf("Parameter reserved command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;
			}/* switch(qoc & 0x3F) */
			return(ret);
		}
		else
		{
#ifdef DEBUG
			printf("Cause of P_ME_NA command received %d wrong!!!\n",causetra);
#endif
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			ret = ParamValueNAAck((unsigned char)(0x40|UKCOT),qoc,infoaddr,setvalue,sockfd);
			return(ret);
		}
		break;

	case P_ME_NB:	/* 111 confirm of Parameter-value command */
					
/* 		infonum = varstru & 0x7F; */
		qoc = recv_data.data[recvdatapt][11];			/* QPM:= POP[8] LPC[7] KPA[1..6] */

#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#endif

		pmbuf.tpyeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.setvalue = setvalue;
		pmbuf.qpm = qoc;			/* C dir POP=1 M dir LPC=1 POP=0 */
/* 		pmbuf.ack_flag = 1;	 */		/* Set to 1 when ACK by PMRespSend(int sock) */
		pmbuf.ack_flag = 0;			/* Set to 0 when ACK directly. */

		if(12 != recv_data.length[recvdatapt])
		{
			printf("received P_ME_NB:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			pmbuf.cot=(unsigned char)NACTCON;
			ret = ParamValueNBAck((unsigned char)NACTCON,qoc,infoaddr,setvalue,sockfd);
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr - (me_scale_addr - me_base_addr);
			if((infoptr<0)||(infoptr>=mescaledef.number))
			{
				printf("!!!!!P_ME_NB:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = ParamValueNBAck((unsigned char)(0x40|UKINFOAD),qoc,infoaddr,setvalue,sockfd);
				return(ret);
			}
			value= setvalue * sescaledef.scale[infoptr];
#ifdef DEBUG
			printf("##### P_ME_NB: addr=%04X setvalue=%04X value=%f qpm=%02X Received\n",infoaddr,setvalue,value,qoc);
#endif
			switch(qoc & 0x3F)
			{
			case	1:

				databand = setvalue;
				mescaledef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMEScaleTable(MEDEF_SCALE); */
#ifdef DEBUG
				printf("Parameter threshold command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
#ifdef DEBUG
				printf("Parameter smothing factor command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			case	3:

/* 				mescaledef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMEScaleTable(MEDEF_SCALE); */
#ifdef DEBUG
				printf("Parameter low limit command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			case	4:

/* 				mescaledef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMEScaleTable(MEDEF_SCALE); */
#ifdef DEBUG
				printf("Parameter high limit command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				ret = ParamValueNBAck((unsigned char)NACTCON,qoc,infoaddr,setvalue,sockfd);
#ifdef DEBUG
				printf("Parameter reserved command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);
#endif
				break;
			}/* switch(qoc & 0x3F) */
			return(ret);
		}
		else
		{
#ifdef DEBUG
			printf("Cause of P_ME_NB command received %d wrong!!!\n",causetra);
#endif
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			ret = ParamValueNBAck((unsigned char)(0x40|UKCOT),qoc,infoaddr,setvalue,sockfd);
			return(ret);
		}
		break;

	case P_ME_NC:	/* 112 confirm of Parameter-value command */
					
/* 		infonum = varstru & 0x7F; */
		qoc = recv_data.data[recvdatapt][13];	/* QPM:= POP[8] LPC[7] KPA[1..6] */

#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&value,&recv_data.data[recvdatapt][9],4);	/* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&value,&recv_data.data[recvdatapt][9],4);	 /* value of set */
#endif

		pmbuf.tpyeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.fvalue = value;
		pmbuf.qpm = qoc;			/* C dir POP=1 M dir LPC=1 POP=0 */
/* 		pmbuf.ack_flag = 1;	 */		/* Set to 1 when ACK by PMRespSend(int sock) */
		pmbuf.ack_flag = 0;			/* Set to 0 when ACK directly. */

		if(14 != recv_data.length[recvdatapt])
		{
			printf("received P_ME_NC:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			pmbuf.cot=(unsigned char)NACTCON;
			ret = ParamValueNCAck((unsigned char)NACTCON,qoc,infoaddr,value,sockfd);
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr - (me_float_addr - me_base_addr);
			if((infoptr<0)||(infoptr>=mefloatdef.number))
			{
				printf("!!!!!P_ME_NC:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = ParamValueNCAck((unsigned char)(0x40|UKINFOAD),qoc,infoaddr,value,sockfd);
				return(ret);
			}
#ifdef DEBUG
			printf("##### P_ME_NC: addr=%04x value=%f qpm=%02X Received\n",infoaddr,value,qoc);
#endif
			switch(qoc & 0x3F)
			{
			case	1:

				mefloatdef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
/* 				SaveMEFloatTable(MEDEF_FLOAT); */
#ifdef DEBUG
				printf("Parameter threshold command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
#endif
				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
#ifdef DEBUG
				printf("Parameter smothing factor command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
#endif
				break;

			case	3:

/* 				mefloatdef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
/* 				SaveMEFloatTable(MEDEF_FLOAT); */
#ifdef DEBUG
				printf("Parameter low limit command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
#endif
				break;

			case	4:

/* 				mefloatdef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
/* 				SaveMEFloatTable(MEDEF_FLOAT); */
#ifdef DEBUG
				printf("Parameter high limit command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
#endif
				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				ret = ParamValueNCAck((unsigned char)NACTCON,qoc,infoaddr,value,sockfd);
#ifdef DEBUG
				printf("Parameter reserved command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);
#endif
				break;
			}/* switch(qoc & 0x3F) */
			return(ret);
		}
		else
		{
#ifdef DEBUG
			printf("Cause of P_ME_NC command received %d wrong!!!\n",causetra);
#endif
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			ret = ParamValueNCAck((unsigned char)(0x40|UKCOT),qoc,infoaddr,value,sockfd);
			return(ret);
		}
		break;

	case C_IC_NA:	/* 100 confirm of general interrogation command */

#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		qoc = recv_data.data[recvdatapt][9];			/* QOI[1..8]:= 20 general interrogation */
									/* 21: group1... 34:group14...36:group16 */
		icbuf.tpyeId = type_id;
		icbuf.qoi = qoc;
		icbuf.ack_flag = 0;

		if(10 != recv_data.length[recvdatapt])
		{
			printf("received C_IC_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			icbuf.cot = (unsigned char)NACTCON;
			ret = GeneralInterrogationAck((unsigned char)NACTCON,qoc,sockfd);
			return(ret);
		}
		if(infoaddr!=0)
		{
			icbuf.cot = (unsigned char)(0x40|UKINFOAD);
			ret = GeneralInterrogationAck((unsigned char)(0x40|UKINFOAD),qoc,sockfd);
			return(ret);
		}
		switch(causetra & 0x3F)
		{
		case ACT:		/* general interrogation command act confirm */

			if(qoc==20)
			{
				interrogationflag = 1;
#ifdef	DEBUG
				printf("IEC general interrogation %x command is received!\n",qoc);
#endif
				scanflag = 0;
				scan_tim = tim;

				send_ic_data_type=0;
				m_cRespGenInterroGrpNum = 0;
				icbuf.cot = (unsigned char)ACTCON;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,INTROGEN,sockfd);
				if(ret == FALSE)
					return(FALSE);
				ret = RespondGeneralInterrogation(sockfd);
				return(ret);
			}
			if(qoc==34) /* interrogation measure parameter */
			{
				Paramflag = 1;
				m_cRespParamNum = 0;
#ifdef	DEBUG
				printf("IEC interrogation measure parameter %x command is received!\n",qoc);
#endif
				icbuf.cot = (unsigned char)ACTCON;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,qoc,sockfd);
				return(ret);
			}
/* interrogation group */
			m_cRespICGrpNum = 0;
#ifdef	DEBUG
			printf("IEC interrogation group %d command is received!\n",qoc);
#endif
/*
			if((qoc>=21)&&(qoc<=28))
			{
				grp1 = spdef.number/IC_SP__GROUP_NUM;
				if(spdef.number%IC_SP__GROUP_NUM)
					grp1 ++;
				if((qoc-20) <= grp1)
				{
				}
			}
*/
/* 			if((qoc==21)&&((spdef.number !=0)||(dpdef.number !=0))) */
			if(qoc==21)
			{
#ifdef	DEBUG
				printf("interrogation group %d SP/DP group\n",qoc-20);
#endif
				icgroupflag = 1;
				icbuf.cot = (unsigned char)ACTCON;
				m_cRespICGrpNum = 0;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,qoc,sockfd);
				return(ret);
			}
			if(qoc==22)
			{
#ifdef	DEBUG
				printf("interrogation group %d DP group\n",qoc-20);
#endif
				icgroupflag = 1;
				icbuf.cot = (unsigned char)ACTCON;
				m_cRespICGrpNum = 0;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,qoc,sockfd);
				return(ret);
			}
/* 			if((qoc==29)&&((medef.number !=0)||(mescaledef.number !=0)||(mefloatdef.number !=0))) */
			if(qoc==29)
			{
#ifdef	DEBUG
				printf("interrogation group %d ME group\n",qoc-20);
#endif
				icgroupflag = 1;
				icbuf.cot = (unsigned char)ACTCON;
				m_cRespICGrpNum = 0;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,qoc,sockfd);
				return(ret);
			}
			if((qoc==30)&&(mescaledef.number !=0))
			{
#ifdef	DEBUG
				printf("interrogation group %d ME group\n",qoc-20);
#endif
				icgroupflag = 1;
				icbuf.cot = (unsigned char)ACTCON;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,qoc,sockfd);
				m_cRespICGrpNum = 0;
				return(ret);
			}
			if((qoc==31)&&(mefloatdef.number !=0))
			{
#ifdef	DEBUG
				printf("interrogation group %d ME Float group\n",qoc-20);
#endif
				icgroupflag = 1;
				icbuf.cot = (unsigned char)ACTCON;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,qoc,sockfd);
				m_cRespICGrpNum = 0;
				return(ret);
			}
			icgroupflag = 0;
			icbuf.cot = (unsigned char)(0x40|UKINFOAD);
			m_cRespICGrpNum = 0;
			ret = GeneralInterrogationAck((unsigned char)(0x40|UKINFOAD),qoc,sockfd);
			return(ret);
			break;

		case DEACT:		/* general interrogation command deact confirm */

			if(qoc==20)
			{
				interrogationflag = 0;
#ifdef	DEBUG
				printf("IEC general interrogation %x DEACT command is received!\n",qoc);
#endif
				send_ic_data_type=0;
				icbuf.cot = (unsigned char)DEACTCON;
				ret = GeneralInterrogationAck((unsigned char)DEACTCON,qoc,sockfd);
				return(ret);
			}
			else if(qoc==34) /* interrogation measure parameter */
			{
				Paramflag = 0;
#ifdef	DEBUG
				printf("IEC interrogation measure parameter %x DEACT command is received!\n",qoc);
#endif
				icbuf.cot = (unsigned char)DEACTCON;
				ret = GeneralInterrogationAck((unsigned char)DEACTCON,qoc,sockfd);
				return(ret);
			}
			else	/* interrogation group */
			{
#ifdef	DEBUG
				printf("IEC interrogation group %X DEACT command is received!\n",qoc);
#endif
				icbuf.cot = (unsigned char)DEACTCON;
				icgroupflag = 0;
				ret = GeneralInterrogationAck((unsigned char)DEACTCON,qoc,sockfd);
				return(ret);
			}
			break;

		default:
			printf("C_IC_NA: Received cause of transmission=%2X qoi=%dwrong!\n",causetra,qoc);
			icbuf.cot = (unsigned char)(0x40|UKCOT);
			ret = GeneralInterrogationAck((unsigned char)(0x40|UKCOT),qoc,sockfd);
			return(ret);
			break;
		}/* switch(causetra & 0x3F) */
 					
		break;

	case C_CI_NA:	/* 101 confirm of counter interrogation command */

#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		qoc = recv_data.data[recvdatapt][9];			/* QCC[1..6]:= 5: general 1: group1... 4:group4 */
									/* FRZ[7..8]:= 0:read 1:freeze 2:freeze and reset 3:reset */
		cibuf.tpyeId = type_id;
		cibuf.qcc = qoc;
		cibuf.ack_flag = 1;

		if(10 != recv_data.length[recvdatapt])
		{
			printf("received C_CI_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			cibuf.cot = (unsigned char)NACTCON;
			ret = CounterInterrogationAck((unsigned char)NACTCON,qoc,sockfd);
			return(ret);
		}
		if(infoaddr!=0)
		{
			cibuf.cot = (unsigned char)(0x40|UKINFOAD);
			ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
			return(ret);
		}
		switch(causetra & 0x3F)
		{
		case ACT:		/* counter interrogation command act confirm */

			frz = (qoc >> 6) &0x3;

			if(frz==0)
			{
				if(qoc==5)
				{
					counterflag = 1;
					m_cRespCountGrpNum=0;
#ifdef DEBUG
					printf("Counter interrogation %02X Read command is received!\n",qoc);
#endif
					cibuf.cot = (unsigned char)ACTCON;
					ret = CounterInterrogationAck((unsigned char)ACTCON,cibuf.qcc,sockfd);
					return(ret);
				}
				else	/* counter interrogation group */
				{
					if((qoc<1)||(qoc>4))
					{
#ifdef	DEBUG
						printf("Counter group %02X > itnum %d Read command is received!\n",qoc,itdef.number);
#endif
						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
					if(((qoc-1)*CI_GROUP_NUM) >= itdef.number)
					{
#ifdef	DEBUG
						printf("Counter group %02X > itnum %d Read command is received!\n",qoc,itdef.number);
#endif
						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
#ifdef	DEBUG
					printf("Counter group %02X Read command is received!\n",qoc);
#endif
					cibuf.cot = (unsigned char)ACTCON;
					cigroupflag = 1;
					m_cRespCIGrpNum = (qoc-1)*CI_GROUP_NUM/IT_GROUP_NUM;
					ret = CounterInterrogationAck((unsigned char)ACTCON,cibuf.qcc,sockfd);
					return(ret);
				}
			}
			else
			{
				if((qoc&0x3F)==5)
				{
#ifdef DEBUG
					printf("Counter interrogation %02X FRZ command is received!\n",qoc);
#endif
					cibuf.cot = (unsigned char)ACTCON;
					ret = CounterInterrogationAck((unsigned char)ACTCON,cibuf.qcc,sockfd);
					return(ret);
				}
				else	/* counter interrogation group */
				{
					if(((qoc&0x3F)<1)||((qoc&0x3F)>4))
					{
#ifdef	DEBUG
						printf("Counter group %02X > itnum %d FRZ command is received!\n",(qoc&0x3F),itdef.number);
#endif
						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
					if((((qoc&0x3F)-1)*CI_GROUP_NUM) >= itdef.number)
					{
#ifdef	DEBUG
						printf("Counter group %02X > itnum %d FRZ command is received!\n",(qoc&0x3F),itdef.number);
#endif
						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
#ifdef	DEBUG
					printf("Counter group %02X FRZ command is received!\n",qoc);
#endif
					cibuf.cot = (unsigned char)ACTCON;
					ret = CounterInterrogationAck((unsigned char)ACTCON,cibuf.qcc,sockfd);
					return(ret);
				}
			}
			break;

		case DEACT:		/* counter interrogation command deact */

			if((qoc&0x3F)==5)
			{
				counterflag = 0;
#ifdef DEBUG
				printf("IEC counter interrogation %x DEACT command is received!\n",qoc);
#endif
				cibuf.cot = (unsigned char)DEACTCON;
				ret = CounterInterrogationAck((unsigned char)DEACTCON,cibuf.qcc,sockfd);
				return(ret);
			}
			else	/* counter interrogation group */
			{
#ifdef	DEBUG
				printf("IEC interrogation group %x DEACT command is received!\n",qoc);
#endif
				cibuf.cot = (unsigned char)DEACTCON;
				cigroupflag = 0;
				ret = CounterInterrogationAck((unsigned char)DEACTCON,cibuf.qcc,sockfd);
				return(ret);
			}
			break;

		default :
			printf("C_CI_NA: Received cause of transmission=%2x qcc=%x wrong!\n",causetra,qoc);
			cibuf.cot = (unsigned char)(0x40|UKCOT);
			ret = CounterInterrogationAck((unsigned char)(0x40|UKCOT),cibuf.qcc,sockfd);									
			return(ret);
			break;
		}/* switch(causetra & 0x3F) */
					
		break;

	case C_RD_NA:	/* 102 confirm of read command */
				
#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		rdbuf.tpyeId = type_id;
		rdbuf.infoaddr = infoaddr;
		rdbuf.ack_flag = 0;

		if(9 != recv_data.length[recvdatapt])
		{
			printf("received C_RD_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			rdbuf.cot = (unsigned char)NACTCON;
			ret = ReadCmdAck((unsigned char)NACTCON,infoaddr,sockfd);
			return(ret);
		}
		if((causetra & 0x3F)==REQ)	/* read command req confirm */
		{
#ifdef DEBUG
			printf("IEC read command C_RD_NA infoaddr=%d is received!\n",infoaddr);
#endif
			rdbuf.cot = REQ;
			ret = ReadCmdResp(infoaddr,sockfd);									
			return(ret);
		}
		else
		{
			printf("C_RD_NA: Received cause of transmission=%2x wrong!\n",causetra);
			rdbuf.cot = (unsigned char)(0x40|UKCOT);
			ret = ReadCmdAck((unsigned char)(0x40|UKCOT),infoaddr,sockfd);
			return(ret);
		}
		break;

	case C_CS_NA:	/* 103 confirm of clock synchronization command */
					
#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif

		if(16 != recv_data.length[recvdatapt])
		{
			printf("received C_CS_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			return(FALSE);
		}
		if((causetra & 0x3F) == ACT)
		{
			ms = recv_data.data[recvdatapt][9] + recv_data.data[recvdatapt][10]*256;
/*
			sys_tm.wSecond = ms/1000;
			sys_tm.wMilliseconds = ms%1000;
			sys_tm.wMinute = recv_data.data[recvdatapt][8] & 0x3f;
			sys_tm.wHour = recv_data.data[recvdatapt][9] & 0x1f;
			sys_tm.wDayOfWeek = (recv_data.data[recvdatapt][10] & 0xE0 ) >> 5;
			sys_tm.wDay = recv_data.data[recvdatapt][10] & 0x1F;
			sys_tm.wMonth = recv_data.data[recvdatapt][11] & 0x0F;
			sys_tm.wYear = recv_data.data[recvdatapt][12];
			if ( sys_tm.wYear <= 60 )
				sys_tm.wYear = sys_tm.wYear + 2000;
			else
				sys_tm.wYear = sys_tm.wYear + 1900;
*/
			sTimeStart.tm_sec = ms/1000;
/*			sys_tm.wMilliseconds = ms%1000; */
			sTimeStart.tm_min = recv_data.data[recvdatapt][11] & 0x3f;
			sTimeStart.tm_hour = recv_data.data[recvdatapt][12] & 0x1f;
			sTimeStart.tm_mday = recv_data.data[recvdatapt][13] & 0x1F;
			sTimeStart.tm_mon = recv_data.data[recvdatapt][14] & 0x0F - 1;
			sTimeStart.tm_year =  recv_data.data[recvdatapt][15];
			if ( sTimeStart.tm_year <= 80 )
				sTimeStart.tm_year += 100;
			sTimeStart.tm_isdst = 0;
#ifdef DEBUG
			printf("clock synchronization %d command is received! Delay=%d\n",recv_data.data[recvdatapt][11],cd_delay_time);
			printf("Remote clock %04d/%02d/%02d %02d:%02d:%02d %03d!\n",sTimeStart.tm_year+1900,sTimeStart.tm_mon+1,
				sTimeStart.tm_mday,sTimeStart.tm_hour,sTimeStart.tm_min,sTimeStart.tm_sec,ms%1000);
#endif
			if((stTim = mktime(&sTimeStart)) != (time_t)-1 )
			{
				ms = ms%1000 + cd_delay_time;
				stTim = stTim + ms/1000;
				sTimeStart = *localtime(&stTim);
/*
				sys_tm.wMilliseconds = ms%1000;
				sys_tm.wSecond = sTimeStart.tm_sec;
				sys_tm.wMinute = sTimeStart.tm_min;
				sys_tm.wHour = sTimeStart.tm_hour;
				sys_tm.wDay = sTimeStart.tm_mday;
				sys_tm.wMonth = sTimeStart.tm_mon + 1;
				sys_tm.wYear = sTimeStart.tm_year + 1900;
				sys_tm.wDayOfWeek = sTimeStart.tm_wday + 1;
				if(Gps == 0)
					SetLocalTime(&sys_tm);
*/
			}
			else
			{
				printf("clock synchronization mktime() error!\n");
			}
/* 			GetLocalTime(&localsys_tm); */
			if(gettimeofday(&sTimeVal,NULL) == -1)
			{
				printf("\nClockSynchronisation: gettimeofday() error: %s\n",strerror(errno));
				return(-1);
			}
			slTimeStart = *localtime(&sTimeVal.tv_sec);
			ms = slTimeStart.tm_sec * 1000 + sTimeVal.tv_usec/1000;
			ClockSyncflag = 0;
/*
#ifdef DEBUG
			printf("Local clock %04d/%02d/%02d %02d:%02d:%02d %03d!\n",localsys_tm.wYear,localsys_tm.wMonth,
				localsys_tm.wDay,localsys_tm.wHour,localsys_tm.wMinute,localsys_tm.wSecond,localsys_tm.wMilliseconds);
			printf("Set clock %04d/%02d/%02d %02d:%02d:%02d %03d!\n",sys_tm.wYear,sys_tm.wMonth,
				sys_tm.wDay,sys_tm.wHour,sys_tm.wMinute,sys_tm.wSecond,sys_tm.wMilliseconds);
#endif
			slTimeStart.tm_sec = localsys_tm.wSecond;
			slTimeStart.tm_min = localsys_tm.wMinute;
			slTimeStart.tm_hour = localsys_tm.wHour;
			slTimeStart.tm_mday = localsys_tm.wDay;
			slTimeStart.tm_mon = localsys_tm.wMonth - 1;
			slTimeStart.tm_year =  localsys_tm.wYear - 1900;
			slTimeStart.tm_isdst = 0;
			sltTim = mktime(&slTimeStart);

			diffclk = stTim - sltTim;
			diffclk = diffclk * 1000 + sys_tm.wMilliseconds - localsys_tm.wMilliseconds;
			if(diffclk < 0)
				diffclk = -diffclk;
			if(diffclk > delayalarm)
			{
				ClkDiffflag = 1;
#ifdef DEBUG
				printf("###ALARM!!! clock different %d > %d\n",diffclk,delayalarm);
#endif
			}
			sltTim = sltTim - cd_delay_time/1000;
			if(localsys_tm.wMilliseconds >= (cd_delay_time%1000))
			{
				localsys_tm.wMilliseconds -= (cd_delay_time%1000);
			}
			else
			{
				localsys_tm.wMilliseconds = localsys_tm.wMilliseconds + 1000 - (cd_delay_time%1000);
				sltTim--;
			}
			sTimeStart = *localtime(&sltTim);

			localsys_tm.wSecond = sTimeStart.tm_sec;
			localsys_tm.wMinute = sTimeStart.tm_min;
			localsys_tm.wHour = sTimeStart.tm_hour;
			localsys_tm.wDay = sTimeStart.tm_mday;
			localsys_tm.wMonth = sTimeStart.tm_mon + 1;
			localsys_tm.wYear = sTimeStart.tm_year + 1900;
			localsys_tm.wDayOfWeek = sTimeStart.tm_wday + 1;
*/
			ret = ClockSynchroniseAck(sockfd);		/* ACK clock synchronize */
/*
#ifdef DEBUG
			printf("ACK Local clock %04d/%02d/%02d %02d:%02d:%02d %03d!\n",localsys_tm.wYear,localsys_tm.wMonth,
				localsys_tm.wDay,localsys_tm.wHour,localsys_tm.wMinute,localsys_tm.wSecond,localsys_tm.wMilliseconds);
#endif
*/
		}					
		return(ret);

		break;

	case C_TS_NA:	/* test command 104 */

#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* FBP:AA55 Fix test word */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* FBP:AA55 Fix test word */
#endif
#ifdef DEBUG
		printf("test request frame C_TS_NA received!\n");
#endif
		Testflag = 1;

		testbuf.tpyeId = type_id;
/* 		testbuf.ack_flag = 1; */

		if((causetra & 0x3F) == ACT)
		{
			if((setvalue==0x55AA)&&(infoaddr==0))
			{
#ifdef DEBUG
				printf("test command C_TS_NA frame received!\n");
#endif
				testbuf.cot = ACTCON | 0x80;
			}
			else
			{
#ifdef DEBUG
				printf("test command C_TS_NA frame received!\n");
#endif
				if(infoaddr!=0)
					testbuf.cot = (unsigned char)(0xC0|UKINFOAD);
				else
					testbuf.cot = NACTCON | 0x80;
			}
		}
		else
		{
#ifdef DEBUG
			printf("test command cause of transmit %x wrong!\n",causetra);
#endif
			testbuf.cot = (unsigned char)(0xC0|UKCOT);;
		}
		ret = RespondTestAck(sockfd);
		return(ret);
		break;

	case C_RP_NA:	/* Reset process command 105 */

#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		qoc = recv_data.data[recvdatapt][9];	/* QRP[1..8]:= 1: general reset 2: SOE buffer reset */

		rpbuf.tpyeId = type_id;
		rpbuf.qrp = qoc;
/* 		rpbuf.ack_flag = 1; */

		if(10 != recv_data.length[recvdatapt])
		{
			printf("received C_RP_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			rpbuf.cot = (unsigned char)NACTCON;
			ret = ResetProcessAck(sockfd);
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			if(infoaddr!=0)
			{
				rpbuf.cot = (unsigned char)(0x40|UKINFOAD);
				ret = ResetProcessAck(sockfd);
				return(ret);
			}
			if(qoc==1)
			{
#ifdef DEBUG
				printf("reset process command QRP=1 general process reset frame!\n");
#endif
/* 				Resetflag = 1; */
				rpbuf.cot = ACTCON;
				soebuf.sendpnt = soebuf.bufpnt =0;
			}
			else if(qoc==2)
			{
#ifdef DEBUG
				printf("reset process command QRP=2 SOE buffer reset frame!\n");
#endif
/* 				Resetflag = 2; */
				rpbuf.cot = ACTCON;
				soebuf.sendpnt = soebuf.bufpnt =0;
			}
			else
			{
#ifdef DEBUG
				printf("reset process command QRP=%d frame?\n");
#endif
				rpbuf.cot = ACTCON;
			}
		}
		else
		{
#ifdef DEBUG
			printf("reset process command QRP=%d cause of transmit %x wrong!\n",qoc,causetra);
#endif
			rpbuf.cot = (unsigned char)(0x40|UKCOT);;
		}
		ret = ResetProcessAck(sockfd);
		return(ret);
		break;

	case C_CD_NA:	/* Delay Acquire command 106 */

		cdbuf.tpyeId = type_id;
		cdbuf.ack_flag = 0;

#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&cdbuf.cp16time,&recv_data.data[recvdatapt][9],2);	 /* CP16time2a:ms to second */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&cdbuf.cp16time,&recv_data.data[recvdatapt][9],2);	 /* CP16time2a:ms to second */
#endif
#ifdef DEBUG
		printf("Dealy Acquire command frame C_CD_NA received!\n");
#endif
		if(11 != recv_data.length[recvdatapt])
		{
			printf("received C_CD_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			cdbuf.cot = (unsigned char)NACTCON;
			ret = TimeDelayAck(sockfd);
			return(ret);
		}
		if((causetra & 0x3F) == ACT)	/* Delay acquisition cmd */
		{
			if(infoaddr!=0)
			{
				cdbuf.cot = (unsigned char)(0x40|UKINFOAD);
				ret = TimeDelayAck(sockfd);
				return(ret);
			}
/*
 			GetLocalTime(&localsys_tm);
			cdbuf.locp16time = localsys_tm.wSecond * 1000 + localsys_tm.wMilliseconds;
 */
 			cdbuf.cot = (unsigned char)ACTCON;
			cdbuf.ack_flag = 1;
#ifdef DEBUG
			printf("Delay acquisition cmd cp16time=%d\n",cdbuf.cp16time);
#endif
		}
		else if((causetra & 0x3F) == SPONT)	/* Delay Transmit */
		{
			if(infoaddr!=0)
			{
				cdbuf.cot = (unsigned char)(0x40|UKINFOAD);
				ret = TimeDelayAck(sockfd);
				return(ret);
			}
			cdbuf.cot = (unsigned char)ACTCON;
			cdbuf.ack_flag = 0;
			cd_delay_time = cdbuf.cp16time;
#ifdef DEBUG
			printf("Delay Transmit cd_delay_time=%d\n",cd_delay_time);
#endif
		}
		ret = TimeDelayAck(sockfd);
		return(ret);
		break;

	default :
		mirbuf.cot=(unsigned char)(0x40|UKTYPE);
		mirbuf.pnt=recvdatapt;
/* 		MirrorNAckflag = 1; */
		ret = MirrorNAck(mirbuf.cot,mirbuf.pnt,sockfd);

		printf("IEC type_id = %u is unavailable at this time!\n",type_id);
		return(ret);
		break;
	}/* switch(type_id) */
}

void	IEC104_SOE(void)
{
#ifdef BY_RECORD
/*
	struct shm_rec *shm_rec_addr;
*/
	SHM_RECORD *shm_rec_addr;
	int shm_id_rec;
	IND_REC	*pInd_alm;
	unsigned char	type_id;
	short	rcv_end,rcv_pnt;
	POINTER	rcv_ptr;
	POINTER	ptr;
	int	cmd_type;
#else
	int		data_type;
	unsigned char	name_buf[NAME_SIZE];
	DMS_COMMON	dbentry;
#endif
	unsigned char	stat;
/* 	unsigned char	vti; */
	unsigned short	pntnum,soe_head;
	IND_DATA	dbstat;



#ifndef BY_RECORD
	for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Update orgbuf for SP status Change */
	{
		if(strcmp(spdef.name[pntnum],"NULL")==0)
		{
			orgspsoebuf[pntnum] = 0;
			continue;
		}
		strcpy(name_buf,spdef.name[pntnum]);
		if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
		{
			printf("%s No this SP point in the database file!\n",name_buf);
			orgspsoebuf[pntnum] = 0;
			continue;
		}
		else
		{
			dbstat=dbentry.var.ind.status;
			if(dbstat.state==1)
				stat = 1;
			else
				stat = 0;
			if(dbstat.man_set)
				stat = stat | 0x20;			/* SB=1 */
			if(dbstat.chan_fail)
				stat = stat | 0xC0;			/* IV=1; NT=1 */
			if(dbentry.fixed.iost.scanEnable==0)
				stat = stat | 0x10;			/* BL=1 */
			orgspsoebuf[pntnum] = stat;
		}
	}
	for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* update orgbuf for DP status */
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
			orgdpsoebuf[pntnum] = stat;
		}
	}
/* Check for SOE buffer */
	for(;;)
	{
		sleep(1);
#ifdef ME_TEST
		wt_num++;
		if(wt_num>6)
		{
			for(i=0;i<10;i++)
			{
				if((MeASDUType==0)||(MeASDUType==1)||(MeASDUType==2))
					strcpy(name_buf,medef.name[i]);
				else
					strcpy(name_buf,mefloatdef.name[i]);

				dbentry.var.anlg.value.status=64;
				dbentry.var.anlg.value.data=vlf;
				WriteEntryByName(&dbentry,RW_VAR);
			}
			wt_num=0;
		}
		vlf = vlf + 1;
		if(vlf > 40000)
			vlf = 0;
#endif
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
				dbstat=dbentry.var.ind.status;
				if(dbstat.state==1)
					stat = 1;
				else
					stat = 0;
				if(dbstat.man_set)
					stat = stat | 0x20;			/* SB=1 */
				if(dbstat.chan_fail)
					stat = stat | 0xC0;			/* IV=1; NT=1 */
				if(dbentry.fixed.iost.scanEnable==0)
					stat = stat | 0x10;			/* BL=1 */

				if(stat != orgspsoebuf[pntnum])
				{
					soe_head = soebuf.bufpnt;
 					if(spdef.TimeTag[pntnum] == 1)
/*					if(TimeTag == 1) */
						soebuf.soerec[soe_head].tpyeId = M_SP_TA;
					else
						soebuf.soerec[soe_head].tpyeId = M_SP_TB;

					soebuf.soerec[soe_head].infoaddr = sp_base_addr + pntnum;
					soebuf.soerec[soe_head].stat = orgspsoebuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 SPI */
					soebuf.soerec[soe_head].time = dbentry.entry.ind.variable.date_time;
					soebuf.soerec[soe_head].time.year = soebuf.soerec[soe_head].time.year-1900;
#ifdef DEBUG
					printf("soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						name_buf,soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);
#endif
					
					soe_head ++;
					if(soe_head >= SOE_NUM)
						soe_head = 0;
					soebuf.bufpnt = soe_head;
				}
			}
		}
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
				if(stat != orgdpsoebuf[pntnum])
				{
#ifdef DEBUG
					printf("DP soe: %s stat=%02X old=%02X infoaddr=%d\n",name_buf,stat,orgdpsoebuf[pntnum],dp_base_addr + pntnum);
#endif
					soe_head = soebuf.bufpnt;
 					if(dpdef.TimeTag[pntnum] == 1)
/*					if(TimeTag == 1) */
						soebuf.soerec[soe_head].tpyeId = M_DP_TA;
					else
						soebuf.soerec[soe_head].tpyeId = M_DP_TB;

					soebuf.soerec[soe_head].infoaddr = dp_base_addr + pntnum;
					soebuf.soerec[soe_head].stat = stat;				/* QDS IV NT SB BL RES3 SPI */
					orgdpsoebuf[pntnum] = stat;
					soebuf.soerec[soe_head].time = dbentry.var.ind.update_time;
					soebuf.soerec[soe_head].time.year = soebuf.soerec[soe_head].time.year-1900;
#ifdef DEBUG
					printf("DP soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						name_buf,soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);
#endif
					
					soe_head ++;
					if(soe_head >= SOE_NUM)
						soe_head = 0;
					soebuf.bufpnt = soe_head;
				}
			}
		}
	}
#else		
		/*      打开记录区共享内存      */

	if ( ( shm_id_rec=shmget( ftok (FILE_KEY,(char )SHM_REC_KEY_CHAR),SHM_REC_SIZE,0))==-1)
	{
		printf("\niec104s:Fail to create shm_rec:shmget!\n");
		exit(-1);
	}
 #ifdef DEBUG
        printf ( "iec104s:the shm_id_rec is:%d\n",shm_id_rec);
#endif
		/*      获取记录区共享内存地址  */
	shm_rec_addr=( SHM_RECORD *)shmat( shm_id_rec,0,SHM_RND);
#ifdef DEBUG
	printf ( "the shm_rec_addr is:0x%x\n",shm_rec_addr );
#endif
        if ( (long *)shm_rec_addr==(long *)-1)
	{
		printf("\niec104s: Fail to shmat shm_rec_addr: errno=%d\n",errno);
		exit(-1);
	}
	printf("\niec104s:: shmat shm_rec_addr: OK!\n");

	rcv_end = shm_rec_addr->head.header_ptr;

	for(;;)
	{
		sleep(1);
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
			rcv_ptr.dev_id = pInd_alm->dev_id;
			rcv_ptr.data_type = pInd_alm->data_type;
			rcv_ptr.pt_id = pInd_alm->pt_id;
/*
#ifdef DEBUG
			printf("\niec104s::Receive a record! %d=%d=%d=%d\n",rcv_ptr.stn_id,rcv_ptr.dev_id,rcv_ptr.data_type,rcv_ptr.pt_id);
#endif
*/
			for(pntnum=0;pntnum<spdef.number;pntnum++)		/* Check for SP status Change */
			{
				ptr = spdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(ptr.data_type == rcv_ptr.data_type)&&(ptr.pt_id == rcv_ptr.pt_id))
				{
					dbstat = pInd_alm->ind_status;
					if(dbstat.state==1)
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
/*					if(TimeTag == 1) */
						soebuf.soerec[soe_head].tpyeId = M_SP_TA;
					else
						soebuf.soerec[soe_head].tpyeId = M_SP_TB;

					soebuf.soerec[soe_head].infoaddr = sp_base_addr + pntnum;
					soebuf.soerec[soe_head].stat = orgspsoebuf[pntnum] = stat;			/* QDS IV NT SB BL RES3 SPI */
					soebuf.soerec[soe_head].time.ms = pInd_alm->ms;
					soebuf.soerec[soe_head].time.second = pInd_alm->second;
					soebuf.soerec[soe_head].time.minute = pInd_alm->minute;
					soebuf.soerec[soe_head].time.hour = pInd_alm->hour;
					soebuf.soerec[soe_head].time.day = pInd_alm->day;
					soebuf.soerec[soe_head].time.month = pInd_alm->month;
					soebuf.soerec[soe_head].time.year = pInd_alm->year;
/*
#ifdef DEBUG
					printf("soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						spdef.name[pntnum],soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);
#endif
*/						
					soe_head ++;
					if(soe_head >= SOE_NUM)
						soe_head = 0;
					if(soe_head == soebuf.sendpnt)
					{
						soe_overflow_flag = 1;
						soe_overflow_Sendflag = 1;
						break;
					}
					soebuf.bufpnt = soe_head;
					break;
				}
			}
			if(pntnum<spdef.number)
				continue;

			for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* Check for DP status Change */
			{
				ptr = dpdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(ptr.data_type == rcv_ptr.data_type)&&(ptr.pt_id == rcv_ptr.pt_id))
				{
					dbstat = pInd_alm->ind_status;
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
/*					if(pInd_alm->ind_status.iost.scanEnable==0)
						stat = stat | 0x10;	*/	/* BL=1 */

					soe_head = soebuf.bufpnt;
					if(dpdef.TimeTag[pntnum] == 1)
/*					if(TimeTag == 1) */
 						soebuf.soerec[soe_head].tpyeId = M_DP_TA;
					else
						soebuf.soerec[soe_head].tpyeId = M_DP_TB;

					soebuf.soerec[soe_head].infoaddr = dp_base_addr + pntnum;
					soebuf.soerec[soe_head].stat = orgdpsoebuf[pntnum] = stat;					/* QDS IV NT SB BL RES3 SPI
					soebuf.soerec[soe_head].time.ms = pInd_alm->ms;
					soebuf.soerec[soe_head].time.second = pInd_alm->second;
					soebuf.soerec[soe_head].time.minute = pInd_alm->minute;
					soebuf.soerec[soe_head].time.hour = pInd_alm->hour;
					soebuf.soerec[soe_head].time.day = pInd_alm->day;
					soebuf.soerec[soe_head].time.month = pInd_alm->month;
					soebuf.soerec[soe_head].time.year = pInd_alm->year;
/*
#ifdef DEBUG
					printf("soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						dpdef.name[pntnum],soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);
#endif
*/						
					soe_head ++;
					if(soe_head >= SOE_NUM)
						soe_head = 0;
					soebuf.bufpnt = soe_head;
					break;
				}
			}
			if(pntnum<dpdef.number)
				continue;
		}
	}
#endif
}	

int Get_Cas_Ctrl_Priv(char * ctrl_priv)
{
	int		data_type;
	char		name_buf[NAME_SIZE];
	DMS_COMMON	dbentry;
	unsigned short	dbstat;

	strcpy(name_buf,ctrl_priv);

	if(ReadEntryByNameStr(name_buf,&dbentry)==-1)
	{
		printf("%s No this ctrl privi point in the database file!\n",name_buf);
		return(FALSE);
	}
	else
	{
		dbstat=dbentry.var.ind.status.state;
		if(dbstat == 1)
			return(TRUE);
		else
			return(FALSE);
	}

}
