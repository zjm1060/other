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
#define REC_YF_AGC_CMD
/* #define ME_TEST	*/	/* used for test only, Should be canceled in normal running */

extern	unsigned char	sendbuf[1024],cmdsendbuf[24];
extern	char	infobuf[1024];

extern	unsigned short nr_recv,nr_send,nr_ack;
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

extern	unsigned short	m_cRespGenInterroGrpNum,m_cRespCountGrpNum,m_cCyclicGrpNum,m_cScanGrpNum;
extern	unsigned short	m_cRespParamNum,m_cRespICGrpNum,m_cRespCIGrpNum;
extern	int	ClockSyncflag,DT_Enable_Flag,comdflag,cmdterm;
extern	int	Readflag,Testflag,ResetProcessflag;
extern	int	Paramflag,MirrorNAckflag;
extern	int	interrogationflag,counterflag,icgroupflag,cigroupflag;
extern	int	ClkDiffflag,soe_overflow_flag,soe_overflow_Sendflag;
extern	int	cyclicflag,scanflag;
extern	int	cmd_ack;
extern	char	send_ic_data_type,send_ci_data_type;
extern	unsigned char	cot_interrogation,cot_counter;
extern	unsigned char	count_sq,ITGrpType;

extern	unsigned short	nASDUId;
extern	unsigned char	nSrcId;
extern	unsigned char	cmd_src;
extern	int		Gps;		/* 0: Without GPS; 1: With GPS */
extern	int		GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
extern	int		MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_ND; 3: M_ME_NC */
extern	int		MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int		SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	short		me_code;	/* 0 or -32768 */
extern	short		se_code;	/* 0 or -32768 */
extern	int		TimeTag;	/* Time tag> 0: no; 1: short; 2: long */
extern	int		sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
extern	int		Cmd_Sel;	/*  0: cmd without select; 1: cmd with select */

extern	time_t	scan_tim;
extern	time_t	scan_period;
extern	time_t	cmd_period;
extern	time_t	cmd_trans_delay;

extern	int		ProcNum;
extern	PROC_REC	sProcRec[MAX_PROC_NUM+1];

unsigned char	orgspsoebuf[MAX_SP_NUM],orgdpsoebuf[MAX_DP_NUM];
unsigned char	orgstsoebuf[MAX_ST_NUM],orgstvsoebuf[MAX_ST_NUM];
ASDUCOMDDATA	recv_data;
ASDUDATA	send_data;
unsigned short	cd_delay_time;

#ifdef REC_YF_AGC_CMD
	unsigned short	daysaved;
	char		cmdrecfile[80];
	FILE		*fagc;
#endif

SPDEF		spdef;
DPDEF		dpdef;
MEDEF		medef;
STDEF		stdef;
ITDEF		itdef;
DCDEF		dcdef;
SEDEF		sedef;
SCDEF		scdef;
RCDEF		rcdef;


unsigned short	sp_base_addr;
unsigned short	dp_base_addr;
unsigned short	me_base_addr;
unsigned short	pm_base_addr;
unsigned short	it_base_addr;
unsigned short	st_base_addr;
unsigned short	bo_base_addr;

unsigned short	sc_cmd_base_addr;
unsigned short	dc_cmd_base_addr;
unsigned short	rc_cmd_base_addr;
unsigned short	se_cmd_base_addr;
unsigned short	bc_cmd_base_addr;

char    src_id;


void InitFlag(void)
{
	send_data.pnt = 0;
	recv_data.pnt = 0;
	
	cmdbuf.typeId = 0;
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

	counterflag=ClockSyncflag=interrogationflag=0;
	icgroupflag = cigroupflag = 0;
	ResetProcessflag = Testflag = MirrorNAckflag = 0;
	Paramflag = Readflag = Testflag = 0;
	cyclicflag = scanflag = 0;
	soe_overflow_flag = soe_overflow_Sendflag = ClkDiffflag = 0;
	count_sq = 0;
}

void InitConfig(char *NameID)
{
	nASDUId = CMM_ADRESS;
	nSrcId = SRC_ADRESS;
	Gps = 1;		/* With GPS */
	GiType = 1;		/* 1:M_SP_NA; 20: M_PS_NA */
	MeASDUType = 3;		/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 2: M_ME_ND; 3: M_ME_NC */
	MeType = 1;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	SEType =1;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	TimeTag =2;		/* time tag 0: no; 1: short; 2: long */
	sp_cos = 0;		/* sp_cos 0: sp/dp transmit once; 1: twice */
	Cmd_Sel = 1;		/* 0: cmd without select; 1: cmd with select */
	ITGrpType = 0;

	scan_period = SCAN_PERIOD;
	cmd_period = CMD_TIMEOUT;
	cd_delay_time = 0;

	sp_base_addr = SP_BASE_ADDR;
	dp_base_addr = DP_BASE_ADDR;
	me_base_addr = ME_BASE_ADDR;
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

	sedef.number = 0;

	scdef.number = 0;
	dcdef.number = 0;
	rcdef.number = 0;

	GenConfigTable(NameID,(char *)CONFIG_FILE);

	GenSPTable(NameID,(char *)SPDEF_FILE);
	GenDPTable(NameID,(char *)DPDEF_FILE);

	GenMETable(NameID,(char *)MEDEF_NORM);

	GenSTTable(NameID,(char *)STDEF_FILE);
	GenITTable(NameID,(char *)ITDEF_FILE);

	GenSETable(NameID,(char *)SEDEF_NORM);

	GenSCTable(NameID,(char *)SCDEF_FILE);
	GenDCTable(NameID,(char *)DCDEF_FILE);
	GenRCTable(NameID,(char *)RCDEF_FILE);


	if(MeType)
		me_code = -32768;
	else
		me_code = 0;

	if(SEType)
		se_code = -32768;
	else
		se_code = 0;

	src_id =0;
}

int ReadStnDef(char * pStnDefFile)
{	FILE	*fp;
	char	NameID[256], Ip[256], CtrlName[256];
	int		c_src;
	char	filebuf[LINE_BUF_CHAR];
	int		r;
	char	procname[256];	
	char	filenamestr[256];
	
	r=0;
	ProcNum = 0;
	sprintf(filenamestr,"%s%s",IEC104_HOMEPATH,pStnDefFile);
	while( (fp=fopen(filenamestr,"r"))==NULL )
	{
		OutputDspInfo(OUTPUT_ERROR,"\n### ALARM !!! IEC104: Can't open Station config file %s !!! \n",filenamestr);
		Sleep(5000);
	}


	for(;;)
	{
		if(ProcNum>=MAX_PROC_NUM)
		{
			fclose( fp );
			OutputDspInfo(0,"\nRead Station define file %s: proc num = %d!!!\n\n",pStnDefFile,ProcNum);
			return( TRUE );
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				OutputDspInfo(0,"\nRead Station define file %s: proc num = %d!!!\n\n",pStnDefFile,ProcNum);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR,"Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0,"%s",filebuf);
			continue;
		}
		if(sscanf(filebuf,"%s %s %d %s %s",NameID,Ip,&c_src,CtrlName,procname)<=0)
		{
			OutputDspInfo(OUTPUT_ERROR,"Error!%3d line: %s\n",r,filebuf);
			continue;
		}
		strcpy( sProcRec[ProcNum].NameID, NameID );
		strcpy( sProcRec[ProcNum].Ip, Ip );
		strcpy( sProcRec[ProcNum].ctrl_priv, CtrlName );
		strcpy( sProcRec[ProcNum].procname, procname);
		sProcRec[ProcNum].cmd_src = c_src;
		sProcRec[ProcNum].Pid = -1;
		OutputDspInfo(0,"\nStation %d name %s: ADDR = %s SRC=%d Ctrl_Name = %s procname= %s\n",ProcNum,sProcRec[ProcNum].NameID,
			sProcRec[ProcNum].Ip,sProcRec[ProcNum].cmd_src,sProcRec[ProcNum].ctrl_priv,sProcRec[ProcNum].procname);
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
	int	n,r;

	r=0;
	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "\nCan't open config file %s!!!\n",filenamestr);
		nASDUId = CMM_ADRESS;
		Sleep(5000);
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);
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
			OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
			continue;
		}
		n = strlen(strFlag);
		if(strcmp(strFlag,"CMM_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
				
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
		else if(strcmp(strFlag,"SRC_ADDR:")== 0)
		{
			if(sscanf(&filebuf[n],"%d",&iTemp)<=0)
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ASDU SRC_ADDR=%d\n",pDefFile,iTemp);
				
				nSrcId = SRC_ADRESS;
				continue;
			}
			if((iTemp<0) || (iTemp>255))
			{
				OutputDspInfo(OUTPUT_ERROR, "Config file %s Error! ASDU SRC_ADDR=%d\n",pDefFile,iTemp);
				
				nSrcId = SRC_ADRESS;
			}
			else
			{
				OutputDspInfo(0, "ASDU SRC_ADDR = %d\n",iTemp);

				nSrcId = iTemp;
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
			OutputDspInfo(OUTPUT_WARN, "Unsupported item: %s\n",strFlag);
	}
}

int GenSPTable(char * pName,char *pDefFile)		/* single point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "\ncan't open SP file %s\n",filenamestr);
 		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

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
			OutputDspInfo(OUTPUT_ERROR, "Error! SP %3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		spdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "+++Warning! SP %d line: num %d != %d\n",r,iTemp,j);
			
		}

		if(strcmp(spdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "SP %d pnt: %s\t%d\t%s\n",j,spdef.name[j],spdef.TimeTag[j],lgname);
			strcpy(name_buf2,spdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				spdef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				
			}
			else
			{
				spdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			spdef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "SP %d pnt: NULL\n",j);
		}
		j ++;
	}
}

int GenDPTable(char * pName,char *pDefFile)		/* double point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "can't open DP file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

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
			OutputDspInfo(OUTPUT_ERROR, "DP Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		dpdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "+++Warning! DP %d line: num %d != %d\n",r,iTemp,j);
			
		}
		if(strcmp(dpdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "DP %d pnt: %s\t%d\t%s\n",j,dpdef.name[j],dpdef.TimeTag[j],lgname);
			strcpy(name_buf2,dpdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				dpdef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				
			}
			else
			{
				dpdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			dpdef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "DP %d pnt: NULL\n",j);
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
	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "\ncan't open ME file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

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
			OutputDspInfo(OUTPUT_ERROR, "ME Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		medef.ASDUType[j] = iTemp1;
		medef.TimeTag[j] = iTemp2;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "+++Warning! ME %d line: num %d != %d\n",r,iTemp,j);
		}
		if(strcmp(medef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "ME %d %s %5.3f %5.3f %5.3f %5.3f %d %d %s\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],
				medef.scale[j],medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],lgname);
			strcpy(name_buf2,medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				medef.ptr[j].dev_id = 0;
				OutputDspInfo(OUTPUT_ERROR, "ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				
			}
			else
			{
				medef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			medef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "ME %d pnt: NULL\n",j);
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

	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"w"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "\ncan't open(w) ME file %s\n",filenamestr);
		
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

int GenSTTable(char * pName,char *pDefFile)		/* integrated total point define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "\ncan't open ST file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

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
			OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		stdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "+++Warning! %d line: num %d != %d\n",r,iTemp,j);
			
		}
		if(strcmp(stdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "ST %d pnt: %s\t%d\t%s\n",j,stdef.name[j],stdef.TimeTag[j],lgname);
			strcpy(name_buf2,stdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				stdef.ptr[j].dev_id = 0;
				OutputDspInfo(0, "ST %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				
			}
			else
			{
				stdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			stdef.ptr[j].dev_id = 0;
 			OutputDspInfo(OUTPUT_WARN, "ST %d pnt: NULL\n",j);
		}
		j ++;
	}
}

int GenITTable(char * pName,char *pDefFile)		/* integrated total point define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "\ncan't open IT file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

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
			OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		itdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "+++Warning! %d line: num %d != %d\n",r,iTemp,j);
			
		}
		if(strcmp(itdef.name[j],"NULL")!=0)
		{
 			OutputDspInfo(0, "IT %d pnt: %s\t%d\t%s\n",j,itdef.name[j],itdef.TimeTag[j],lgname);
			strcpy(name_buf2,itdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "IT %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "IT %d pnt: NULL\n",j);
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

	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "can't open SC file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %d %d %s %s",&numb,scdef.name[j],
			&iTemp,&iTemp1,&iTemp2,scdef.interlockname[j],lgname)!=7)
		{
			OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		scdef.cmdtype[j] = (unsigned char)iTemp;
		scdef.optype1[j] = (unsigned char)iTemp1;
		scdef.optype2[j] = (unsigned char)iTemp2;

		if(j!=numb)
		{
			OutputDspInfo(OUTPUT_WARN, "SC %d pnt number=%d wrong!!! %s %d %d %d %s %s\n",j,numb,scdef.name[j],
				scdef.cmdtype[j],scdef.optype1[j],scdef.optype2[j],scdef.interlockname[j],lgname);
			
		}
		if(strcmp(scdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "SC %d pnt: %s %d %d %d %s %s\n",j,scdef.name[j],scdef.cmdtype[j],
				scdef.optype1[j],scdef.optype2[j],scdef.interlockname[j],lgname);
			strcpy(name_buf2,scdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "SC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "SC %d pnt: NULL\n",j);
		}
		j ++;
	}
}

int GenDCTable(char * pName,char *pDefFile)		/* double point control command define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2,numb;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "can't open DC file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %d %d %s %s",&numb,dcdef.name[j],
			&iTemp,&iTemp1,&iTemp2,dcdef.interlockname[j],lgname)!=7)
		{
			OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
			
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
			OutputDspInfo(0, "DC %d pnt: %s %d %d %d %s %s\n",j,dcdef.name[j],dcdef.cmdtype[j],
				dcdef.optype1[j],dcdef.optype2[j],dcdef.interlockname[j],lgname);
			
			strcpy(name_buf2,dcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "DC %d pnt: NULL\n",j);
		}
		j ++;
	}
}

int GenSETable(char * pName,char *pDefFile)		/* set point control point define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(OUTPUT_ERROR, "can't open SE file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

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
			OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		sedef.cmdtype[j] = (unsigned char)iTemp1;  //GCL

		if(j!=iTemp)
		{
			OutputDspInfo(OUTPUT_WARN, "SE %d pnt number=%d wrong!!! %s %5.3f %5.3f %5.3f %d %s %s\n",j,iTemp,sedef.name[j],
				sedef.lorange[j],sedef.hirange[j],sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
			
		}

		if(strcmp(sedef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "SE %d pnt: %s %5.3f %5.3f %5.3f %d %s %s\n",j,sedef.name[j],sedef.lorange[j],sedef.hirange[j],
					sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
			strcpy(name_buf2,sedef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "SE %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "SE %d pnt: NULL\n",j);
		}
		j ++;
	}
}

int GenRCTable(char * pName,char *pDefFile)		/* step point control command define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[60];

	int		iTemp,iTemp1,iTemp2,numb;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_%s",IEC104_HOMEPATH,pName,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "can't open RC file %s\n",filenamestr);
		
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
				OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: is a null line.\n",r);

				continue;
			}
		}
		if((filebuf[0] == '#')||(filebuf[0] == '!'))
		{
			OutputDspInfo(0, "%s",filebuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %d %d %d %s %s",&numb,rcdef.name[j],
			&iTemp,&iTemp1,&iTemp2,rcdef.interlockname[j],lgname)!=7)
		{
			OutputDspInfo(OUTPUT_ERROR, "Error!%3d line: %s\n",r,filebuf);
			
			j ++;
			continue;
		}
		rcdef.cmdtype[j] = (unsigned char)iTemp;
		rcdef.optype1[j] = (unsigned char)iTemp1;
		rcdef.optype2[j] = (unsigned char)iTemp2;

		if(j!=numb)
		{
			OutputDspInfo(OUTPUT_WARN, "RC %d pnt number=%d wrong!!! %s %d %d %d %s %s\n",j,numb,rcdef.name[j],
				rcdef.cmdtype[j],rcdef.optype1[j],rcdef.optype2[j],rcdef.interlockname[j],lgname);
			
		}
		if(strcmp(rcdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "RC %d pnt: %s %d %d %d %s %s\n",j,rcdef.name[j],rcdef.cmdtype[j],
				rcdef.optype1[j],rcdef.optype2[j],rcdef.interlockname[j],lgname);
			strcpy(name_buf2,rcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "RC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				
			}
		}
		else
		{
 			OutputDspInfo(OUTPUT_WARN, "RC %d pnt: NULL\n",j);
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

int	IEC870_5_Recv(int pnum,unsigned int recvdatapt,int sockfd)
{
	int		ret;
	int		sock;
	LAN_HEAD	lanhead;
	unsigned char	me_buf[1440];
	unsigned short	infoaddr,ms,year,datalength,lengthfactor;
	int		infoptr;
	char		dbname[36];
	unsigned char	type_id,varstru,qoc,frz,lanpt;
	unsigned char	infonum,objpnt,opstat;
	float		value;
	short		setvalue;
	unsigned short	asduaddr,causetra;
	short		cmd_stat;
	char		cmdtype;

	DMS_COMMON	dbcommpt;
	int		i,datapoint;
	ANLG_DATA	anadata;

	struct	tm	tmpt;
	time_t		tim;
	time_t		stTim;

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
	if(asduaddr!=nASDUId && asduaddr!=0xffff)
	{
		OutputDspInfo(OUTPUT_WARN, "ASDU addr %d != %d wrong!\n",asduaddr,nASDUId);
		mirbuf.cot=(unsigned char)(0x40|UKASDUAD);
		mirbuf.pnt=recvdatapt;
		ret = MirrorNAckflag = 1;
		return(ret);
	}
	tim=time(&tim);

	switch(type_id)
	{
	case C_SC_NA:	/* 45 Single point control command */
	case C_SC_TA:	/* 58 Single point control command */

		qoc = recv_data.data[recvdatapt][9];/* QOC:= S/E[8] QU[3..7] SCS[1] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
		cmdbuf.typeId = type_id;
		cmdbuf.stat = qoc;
/* 		cmdbuf.ack_flag = 1;	 */	/* Set to 1 when ACK by CmdRespSend(int sock) */
		cmdbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(type_id==C_SC_NA)
		{
			if(10 != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received C_SC_NA:ASDU length error! type_id=%d,length=%u\n",
						type_id,recv_data.length[recvdatapt]);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
				return(ret);
			}
		}
		else if(17 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_SC_TA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			cmdbuf.cot=(unsigned char)NACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
			ret = SinglePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-sc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=scdef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "%04x info address wrong! No this point in SC Control Table!\n",infoaddr);

				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_SC_NA)
					ret = SinglePointCtrlAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* SC NActCon */
				else
					ret = SinglePointCtrlTAAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* SC NActCon */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control privilege not in %s center! NACTCON SC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}

			strcpy(dbname,scdef.name[infoptr]);
			OutputDspInfo(0, "recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,qoc,causetra);

			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_WARN, "%s No this SC control point in the database file!\n",dbname);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_SC_NA)
					ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
				else
					ret = SinglePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
				return(ret);
			}
			if((qoc & 0x1)==1)
				opstat = 1;
			else
				opstat = 0;

			if(Get_Cmd_Ctrl_Priv(scdef.interlockname[infoptr],opstat,&dbcommpt) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON SC\n",sProcRec[pnum].NameID,scdef.name[infoptr]);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_SC_NA)
					ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
				else
					ret = SinglePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
				return(ret);
			}

			if(qoc & 0x80)		/* Select Command */
			{
				if(cmdbuf.selected==1)
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute SC command error: command is already selected\n");
					
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;

					if(type_id==C_SC_NA)
						ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
					else
						ret = SinglePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
					return(ret);
				}

				cmdbuf.cot=(unsigned char)ACTCON;
				cmdbuf.selected=1;
				cmdbuf.term_flag=0;
				cmdbuf.sel_tim=tim;
				cmdbuf.opstat= opstat;
				cmdbuf.infoaddr= infoaddr;

				OutputDspInfo(0, "Select SC command %02x addr= %04x received\n",qoc,infoaddr);

				if(type_id==C_SC_NA)
				{
					ret = SinglePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* SC ActCon */
					return(ret);
				}

				stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][10]);
				if((stTim == (time_t)-1 )||((tim-stTim)>cmd_trans_delay))
				{
					cmdbuf.selected=0;
					ret = SinglePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
					return(ret);
				}
				ret = SinglePointCtrlTAAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
				return(ret);
			}
/* 	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period)
					|| (opstat!=cmdbuf.opstat))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute SC command infoaddr:%d != Select infoaddr:%d or Timeout %d or selected op diff\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));

					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					if(type_id==C_SC_NA)
						ret = SinglePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
					else
						ret = SinglePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
					return(ret);
				}
			}
			if(type_id==C_SC_NA)
				ret = SinglePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* SC ActCon */
			else
			{
				stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][10]);
				if((stTim == (time_t)-1 )||((tim-stTim)>cmd_trans_delay))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute SC TA command infoaddr:%d delay %d\n",infoaddr,cmdbuf.infoaddr,(tim-stTim));

					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					ret = SinglePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
					return(ret);
				}
				ret = SinglePointCtrlTAAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* SC NActCon */
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

			if(type_id==C_SC_NA)
				ret = SinglePointCtrlAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* SC Actterm:10 */
			else
				ret = SinglePointCtrlTAAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* SC Actterm:10 */
/* 			cmdterm = 1; */
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
				OutputDspInfo(OUTPUT_ERROR, "can't open(wb) the file %s!\n",cmdrecfile);

			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d infoaddr=%04X qoc=%02X %d %d %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,infoaddr,qoc,scdef.cmdtype[infoptr],cmd_stat,scdef.name[infoptr]);
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

			OutputDspInfo(0, "DEACT SC command addr=%04x received\n",infoaddr);

			if(type_id==C_SC_NA)
				ret = SinglePointCtrlAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* SC DeActCon:9 */
			else
				ret = SinglePointCtrlTAAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* SC DeActCon:9 */
			return(ret);
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(OUTPUT_WARN, "Unknown cause %d of SC command addr=%04x received\n",causetra,infoaddr);

			if(type_id==C_SC_NA)
				ret = SinglePointCtrlAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			else
				ret = SinglePointCtrlTAAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			return(ret);
		}
		break;

	case C_DC_NA:	/* 46 double point control command */
	case C_DC_TA:	/* 59 double point control command */

		qoc = recv_data.data[recvdatapt][9];	/* QOC:= S/E[8] QU[3..7] DCS[1..2] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
		cmdbuf.typeId = type_id;
		cmdbuf.stat = qoc;
/* 		cmdbuf.ack_flag = 1;	 */	/* Set to 1 when ACK by CmdRespSend(int sock) */
		cmdbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(type_id==C_DC_NA)
		{
			if(10 != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received C_DC_NA:ASDU length error! type_id=%d,length=%u\n",
						type_id,recv_data.length[recvdatapt]);
				

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}
		}
		else if(17 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_DC_TA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			cmdbuf.cot=(unsigned char)NACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
			ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-dc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=dcdef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "%04x info address wrong! No this point in DC Control Table!\n",infoaddr);
				

				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_DC_NA)
				   ret = DoublePointCtrlAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* DC NActCon */
				else
				   ret = DoublePointCtrlTAAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control privilege not in %s center! NACTCON DC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
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
				if(type_id==C_DC_NA)
					ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				else
					ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
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
				if(type_id==C_DC_NA)
					ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				else
					ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}
			if(Get_Cmd_Ctrl_Priv(dcdef.interlockname[infoptr],opstat,&dbcommpt) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON DC\n",sProcRec[pnum].NameID,dcdef.name[infoptr]);


				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_DC_NA)
					ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				else
					ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}
			if(qoc & 0x80)		/* Select Command */
			{
				if(cmdbuf.selected==1)
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute DC command error: command is already selected\n");
					
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					if(type_id==C_DC_NA)
						ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
					else
						ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
					return(ret);
				}

				cmdbuf.cot=(unsigned char)ACTCON;
				cmdbuf.selected=1;
				cmdbuf.term_flag=0;
				cmdbuf.sel_tim=tim;
				cmdbuf.opstat= opstat;
				cmdbuf.infoaddr= infoaddr;

				OutputDspInfo(0, "Select DC command %02x addr= %04x received\n",qoc,infoaddr);

				if(type_id==C_DC_NA)
				{
					ret = DoublePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* DC ActCon */
					return(ret);
				}
				stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][10]);
				if((stTim == (time_t)-1 )||((tim-stTim)>cmd_trans_delay))
				{
					cmdbuf.selected=0;
					ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
					return(ret);
				}
				ret = DoublePointCtrlTAAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}
/* 	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period)
					|| (opstat!=cmdbuf.opstat))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute DC command infoaddr:%d != Select infoaddr:%d or Timeout %d or selected op diff\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));

					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					if(type_id==C_DC_NA)
						ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC ActCon */
					else
						ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC ActCon */
					return(ret);
				}
			}
			if(((qoc & 0x3)==0)||((qoc & 0x3)==3))
			{
				OutputDspInfo(OUTPUT_WARN, "=====DC addr=%04x qoc=%d this operate is not allowed!\n",infoaddr,qoc);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_DC_NA)
					ret = DoublePointCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC ActCon */
				else
					ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC ActCon */
				return(ret);
			}
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
			if(type_id==C_DC_NA)
				ret = DoublePointCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* DC ActCon:7 */
			else
			{
				stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][10]);
				if((stTim == (time_t)-1 )||((tim-stTim)>cmd_trans_delay))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute SC TA command infoaddr:%d delay %d\n",infoaddr,cmdbuf.infoaddr,(tim-stTim));

					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					ret = DoublePointCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
					return(ret);
				}
				ret = DoublePointCtrlTAAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* DC ActCon:7 */
			}
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
			if(type_id==C_DC_NA)
				ret = DoublePointCtrlAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* DC ACTTERM:10 */
			else
				ret = DoublePointCtrlTAAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* DC ACTTERM:10 */

#ifdef REC_YF_AGC_CMD
			tmpt=*localtime(&tim);
			sprintf(&cmdrecfile[0],"%s%02d.REC",cmdrecbuf,tmpt.tm_mday);
			if(daysaved != tmpt.tm_mday)
			{
				fagc = fopen(cmdrecfile,"w");
				daysaved = tmpt.tm_mday;
			}
			else
				fagc = fopen(cmdrecfile,"a");
			if (fagc==NULL)
			{
				OutputDspInfo(OUTPUT_ERROR, "can't open(wb) the file %s!\n",cmdrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d infoaddr=%04x qoc=%02d %d %d %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,infoaddr,qoc,dcdef.cmdtype[infoptr],cmd_stat,dcdef.name[infoptr]);
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

			OutputDspInfo(0, "=====DEACT DC command addr=%04x received\n",infoaddr);

			if(type_id==C_DC_NA)
				ret = DoublePointCtrlAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* DC DeActCon:9 */
			else
				ret = DoublePointCtrlTAAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* DC DeActCon:9 */
			return(ret);
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(OUTPUT_WARN, "Unknown cause %d of DC command addr=%04x received\n",causetra,infoaddr);

			if(type_id==C_DC_NA)
				ret = DoublePointCtrlAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			else
				ret = DoublePointCtrlTAAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			return(ret);
		}
		break;

	case C_RC_NA:	/* 47 Regulating Step Control Command */
	case C_RC_TA:	/* 60 Regulating Step Control Command */

		qoc = recv_data.data[recvdatapt][9];	/* QOC:= S/E[8] QU[3..7] RCS[1..2] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
		cmdbuf.typeId = type_id;
		cmdbuf.stat = qoc;
/* 		cmdbuf.ack_flag = 1;	 */	/* Set to 1 when ACK by CmdRespSend(int sock) */
		cmdbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(type_id==C_RC_NA)
		{
			if(10 != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received C_RC_NA:ASDU length error! type_id=%d,length=%u\n",
						type_id,recv_data.length[recvdatapt]);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
				return(ret);
			}
		}
		else if(17 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_RC_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			cmdbuf.cot=(unsigned char)NACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;
			ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr-rc_cmd_base_addr;
			if((infoptr<0)||(infoptr>=rcdef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "%04x info address wrong! base=%04x No this point in RC Control Table num=%d!\n",infoaddr,rc_cmd_base_addr,rcdef.number);

				cmdbuf.cot=(unsigned char)(0x40|UKINFOAD);
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_RC_NA)
					ret = RegulatStepCtrlAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* RC NActCon */
				else
					ret = RegulatStepCtrlTAAck((unsigned char)(0x40|UKINFOAD),infoaddr,qoc,sockfd);	/* RC NActCon */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control privilege not in %s center! NACTCON RC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* DC NActCon */
				return(ret);
			}
			strcpy(dbname,rcdef.name[infoptr]);

			OutputDspInfo(0, "recv cmd: type=%d point=%d name=%s qoc=%d cause=%d\n",type_id,
					infoaddr,dbname,qoc,causetra);

			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "%s No this RC control point in the database file!\n",dbname);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_RC_NA)
					ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
				else
					ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
				return(ret);
			}
			if((qoc & 0x3)==2)
				opstat = 1;
			else if((qoc & 0x3)==1)
				opstat = 0;
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "=====RC addr=%04x qoc=%d this operate is not allowed!\n",infoaddr,qoc);
				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_RC_NA)
					ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
				else
					ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
				return(ret);
			}
			if(Get_Cmd_Ctrl_Priv(rcdef.interlockname[infoptr],opstat,&dbcommpt) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON RC\n",sProcRec[pnum].NameID,rcdef.name[infoptr]);

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_RC_NA)
					ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
				else
					ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
				return(ret);
			}
			if(qoc & 0x80)		/* Select Command */
			{
				if(cmdbuf.selected==1)
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute RC command error: command is already selected\n");
					
					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					if(type_id==C_RC_NA)
						ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
					else
						ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
					return(ret);
				}

				cmdbuf.cot=(unsigned char)ACTCON;
				cmdbuf.term_flag=0;
				cmdbuf.opstat= opstat;
				cmdbuf.infoaddr= infoaddr;

				if(type_id==C_RC_NA)
				{
					OutputDspInfo(0, "=====Select RC command %02x addr= %04x received!\n",qoc,infoaddr);

					cmdbuf.selected=1;
					cmdbuf.sel_tim=tim;
					ret = RegulatStepCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
					return(ret);
				}
				stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][10]);
				if((stTim == (time_t)-1 )||((tim-stTim)>cmd_trans_delay))
				{
					OutputDspInfo(OUTPUT_ERROR, "=====Select RC command %02x addr= %04x received delay\n",qoc,infoaddr);

					cmdbuf.selected=0;
					cmdbuf.sel_tim=0;
					ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
					return(ret);
				}
				OutputDspInfo(0, "=====Select RC_TA command %02x addr= %04x received\n",qoc,infoaddr);

				cmdbuf.selected=1;
				cmdbuf.sel_tim=tim;
				ret = RegulatStepCtrlTAAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* RC ActCon */
				return(ret);
			}
/* 	Execute Command */
			if(Cmd_Sel == 1)
			{
				if((cmdbuf.selected != 1)||(infoaddr!=cmdbuf.infoaddr)||((tim-cmdbuf.sel_tim)>cmd_period)
					|| (opstat!=cmdbuf.opstat))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute RC command infoaddr:%d != Select infoaddr:%d or Timeout %d or selected op diff\n",infoaddr,cmdbuf.infoaddr,(tim-cmdbuf.sel_tim));

					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					if(type_id==C_RC_NA)
						ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
					else
						ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
					return(ret);
				}
			}
			if(((qoc & 0x3)==0)||((qoc & 0x3)==3))
			{
				OutputDspInfo(OUTPUT_ERROR, "=====RC addr=%04x qoc=%d this operate is not allowed!\n",infoaddr,qoc);
				

				cmdbuf.cot=(unsigned char)NACTCON;
				cmdbuf.selected=0;
				cmdbuf.term_flag=0;
				if(type_id==C_RC_NA)
					ret = RegulatStepCtrlAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
				else
					ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
				return(ret);
			}
			cmdbuf.cot=(unsigned char)ACTCON;
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			if(type_id==C_RC_NA)
				ret = RegulatStepCtrlAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* RC ActCon:7 */
			else
			{
				stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][10]);
				if((stTim == (time_t)-1 )||((tim-stTim)>cmd_trans_delay))
				{
					OutputDspInfo(OUTPUT_ERROR, "NACK Execute RC TA command infoaddr:%d delay %d\n",infoaddr,cmdbuf.infoaddr,(tim-stTim));

					cmdbuf.cot=(unsigned char)NACTCON;
					cmdbuf.selected=0;
					cmdbuf.term_flag=0;
					ret = RegulatStepCtrlTAAck((unsigned char)NACTCON,infoaddr,qoc,sockfd);	/* RC NActCon */
					return(ret);
				}
				ret = RegulatStepCtrlTAAck((unsigned char)ACTCON,infoaddr,qoc,sockfd);	/* RC ActCon:7 */
			}
			if((qoc & 0x3)==2)
				cmd_stat = rcdef.optype2[infoptr];
			else
				cmd_stat = rcdef.optype1[infoptr];

			OutputDspInfo(0, "RC command addr=%04X QOC=%02X cmd_stat=%d received\n",infoaddr,qoc,cmd_stat);

			cmdtype = rcdef.cmdtype[infoptr];
			ComdSendToLCU(cmdtype,cmd_stat,&dbcommpt);

			cmdbuf.cot=(unsigned char)ACTTERM;
			cmdbuf.selected=0;
			cmdbuf.term_flag=1;
			if(type_id==C_RC_NA)
				ret = RegulatStepCtrlAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* RC ACTTERM:10 */
			else
				ret = RegulatStepCtrlTAAck((unsigned char)ACTTERM,infoaddr,qoc,sockfd);	/* RC ACTTERM:10 */
/* 			cmdterm = 1; */
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
				OutputDspInfo(OUTPUT_ERROR, "iec104s:can't open(wb) the file %s!\n",cmdrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d infoaddr=%04X qoc=%02X %d %d %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,infoaddr,qoc,dcdef.cmdtype[infoptr],cmd_stat,rcdef.name[infoptr]);
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

			OutputDspInfo(0, "=====DEACT RC command addr=%04x received\n",infoaddr);

			if(type_id==C_RC_NA)
				ret = RegulatStepCtrlAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* RC ACTTERM:10 */
			else
				ret = RegulatStepCtrlTAAck((unsigned char)DEACTCON,infoaddr,qoc,sockfd);	/* RC DeActCon:9 */
			return(ret);
		}
		else
		{
			cmdbuf.cot=(unsigned char)(0x40|UKCOT);
			cmdbuf.selected=0;
			cmdbuf.term_flag=0;

			OutputDspInfo(OUTPUT_WARN, "Unknown cause %d of RC command addr=%04x received\n",causetra,infoaddr);

			if(type_id==C_RC_NA)
				ret = RegulatStepCtrlAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			else
				ret = RegulatStepCtrlTAAck((unsigned char)(0x40|UKCOT),infoaddr,qoc,sockfd);
			return(ret);
		}
		break;

	case C_SE_NA:	/* 48 confirm of set-point command normalized value */
	case C_SE_TA:	/* 61 confirm of set-point command normalized value */

		infonum = varstru & 0x7F;
		qoc = recv_data.data[recvdatapt][11];	/* QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#endif
		sebuf.typeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.setvalue = setvalue;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	 *//* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;	/* Set to 0 when ACK directly. */

		if(type_id==C_SE_NA)
		{
			if(12 != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received C_SE_NA:ASDU length error! type_id=%d,length=%u\n",
						type_id,recv_data.length[recvdatapt]);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
		}
		else if(19 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_SE_TA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)NACTCON;
			ret = SetPointCmdTAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_base_addr;
			if((infoptr<0)||(infoptr>=sedef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE:%04x info address wrong! No this point in SE Table!\n",infoaddr);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				if(type_id==C_SE_NA)
					ret = SetPointCmdNAAck((unsigned char)(0x40|UKINFOAD),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTAAck((unsigned char)(0x40|UKINFOAD),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control privilege not in %s center! NACTCON C_SE_NA cmd %d\n",sProcRec[pnum].NameID,infoptr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}

			if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_NA\n",sProcRec[pnum].NameID,sedef.name[infoptr]);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NA)
					ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}

			strcpy(dbname,sedef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NA:%s No this set point in the database file!\n",dbname);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NA)
					ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NA:%s this point is not ANA in the database file!\n",dbname);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NA)
					ret = SetPointCmdNAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
			if(qoc & 0x80)
			{
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				if(type_id==C_SE_NA)
				{
					OutputDspInfo(0, "?????Select C_SE_NA: addr=%04x value = %d Received\n",infoaddr,setvalue);

					ret = SetPointCmdNAAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
					return(ret);
				}
				else if((stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][12])) != (time_t)-1 )
				{
					if((tim-stTim)<cmd_trans_delay)
					{
						OutputDspInfo(OUTPUT_ERROR, "?????Select C_SE_TA: addr=%04x value = %d Received\n",infoaddr,setvalue);

						ret = SetPointCmdTAAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
						return(ret);
					}
				}
				OutputDspInfo(0, "?????Delayed Select C_SE_TA: addr=%04x value = %d Received\n",infoaddr,setvalue);

				ret = SetPointCmdTAAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
			OutputDspInfo(0, "#####Set-Point C_SE_NA: addr=%04x value = %d Received\n",infoaddr,setvalue);

			if(type_id==C_SE_NA)
				ret = SetPointCmdNAAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTAAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */

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
			sebuf.cot=(unsigned char)ACTTERM;
			if(type_id==C_SE_NA)
				ret = SetPointCmdNAAck((unsigned char)ACTTERM,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTAAck((unsigned char)ACTTERM,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
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
				OutputDspInfo(OUTPUT_ERROR, "iec104s:can't open(wb) the file %s!\n",cmdrecfile);
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04X setvalue=%04X %s\n",tmpt.tm_year+1900,
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
			if(type_id==C_SE_NA)
				ret = SetPointCmdNAAck((unsigned char)DEACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point UKCOT qoc:0x00 */
			else
				ret = SetPointCmdTAAck((unsigned char)DEACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point UKCOT qoc:0x00 */
			return(ret);
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_NA: received %d wrong!!!\n",causetra);

			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			if(type_id==C_SE_NA)
				ret = SetPointCmdNAAck((unsigned char)(0x40|UKCOT),infoaddr,setvalue,qoc,sockfd);	/* SET Point UKCOT qoc:0x00 */
			else
				ret = SetPointCmdTAAck((unsigned char)(0x40|UKCOT),infoaddr,setvalue,qoc,sockfd);	/* SET Point UKCOT qoc:0x00 */
			return(ret);
		}
		break;

	case C_SE_NB:	/* 49 confirm of set-point command scaled value */
	case C_SE_TB:	/* 62 confirm of set-point command scaled value */

		qoc = recv_data.data[recvdatapt][11];	/* QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* value of set */
#endif
		sebuf.typeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.setvalue = setvalue;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	 *//* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;	/* Set to 0 when ACK directly. */

		if(type_id==C_SE_NB)
		{
			if(12 != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received C_SE_NB:ASDU length error! type_id=%d,length=%u\n",
						type_id,recv_data.length[recvdatapt]);


				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
		}
		else if(19 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_SE_TB:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)NACTCON;
			ret = SetPointCmdTBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_base_addr;
			if((infoptr<0)||(infoptr>=sedef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE_NB:%04x info address wrong! No this point in SE Table!\n",infoaddr);
				
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = SetPointCmdNBAck((unsigned char)(0x40|UKINFOAD),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control privilege not in %s center! NACTCON C_SE_NB cmd %d\n",sProcRec[pnum].NameID,infoptr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}

			if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_NB\n",sProcRec[pnum].NameID,sedef.name[infoptr]);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NB)
					ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}

			strcpy(dbname,sedef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NB:%s No this set point in the database file!\n",dbname);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NB)
					ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NB:%s this point is not ANA in the database file!\n",dbname);

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NB)
					ret = SetPointCmdNBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if(qoc & 0x80)
			{
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				if(type_id==C_SE_NB)
				{
					OutputDspInfo(0, "?????Select C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);


					ret = SetPointCmdNBAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
					return(ret);
				}
				else
				if((stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][12])) != (time_t)-1 )
				{
					if((tim-stTim)<cmd_trans_delay)
					{
						OutputDspInfo(OUTPUT_ERROR, "?????Select C_SE_TB: addr=%04x value = %d Received\n",infoaddr,setvalue);
						ret = SetPointCmdTBAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
						return(ret);
					}
				}
				OutputDspInfo(0, "?????Delayed Select C_SE_TB: addr=%04x value = %d Received\n",infoaddr,setvalue);


				ret = SetPointCmdTBAck((unsigned char)NACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}

			OutputDspInfo(0, "#####Set-Point C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
			if(type_id==C_SE_NB)
				ret = SetPointCmdNBAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTBAck((unsigned char)ACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */

			value= setvalue * sedef.scale[infoptr];
			cmdtype = sedef.cmdtype[infoptr];
			SEComdSendToLCU(cmdtype, value, &dbcommpt);

			sebuf.term_flag = 1;
			sebuf.cot=(unsigned char)ACTTERM;
			if(type_id==C_SE_NB)
				ret = SetPointCmdNBAck((unsigned char)ACTTERM,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTBAck((unsigned char)ACTTERM,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
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
				OutputDspInfo(OUTPUT_ERROR, "can't open(wb) the file %s!\n",cmdrecfile);
				
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
			OutputDspInfo(0, "=====DEACT Cause of C_SE_NB: addr=%04x value = %d Received\n",infoaddr,setvalue);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			if(type_id==C_SE_NB)
				ret = SetPointCmdNBAck((unsigned char)DEACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTBAck((unsigned char)DEACTCON,infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			return(ret);
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_NB: received %d wrong!!!\n",causetra);
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			if(type_id==C_SE_NB)
				ret = SetPointCmdNBAck((unsigned char)(0x40|UKCOT),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTBAck((unsigned char)(0x40|UKCOT),infoaddr,setvalue,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			return(ret);
		}
		break;

	case C_SE_NC:	/* 50 confirm of set-point command float value */
	case C_SE_TC:	/* 63 confirm of set-point command float value */

		qoc = recv_data.data[recvdatapt][13];			/* QOC:= S/E[8] QL[1..7] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&value,&recv_data.data[recvdatapt][9],4);	 /* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&value,&recv_data.data[recvdatapt][9],4);	/* value of float */
#endif
		sebuf.typeId = type_id;
		sebuf.infoaddr = infoaddr;
		sebuf.fvalue = value;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	*/	/* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(type_id==C_SE_NC)
		{
			if(14 != recv_data.length[recvdatapt])
			{
				OutputDspInfo(OUTPUT_ERROR, "received C_SE_NC:ASDU length error! type_id=%d,length=%u\n",
						type_id,recv_data.length[recvdatapt]);
				

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
		}
		else if(21 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_SE_TC:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)NACTCON;
			ret = SetPointCmdTCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - se_cmd_base_addr;
			if((infoptr<0)||(infoptr>=sedef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE_NC:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)(0x40|UKINFOAD);
				if(type_id==C_SE_NC)
					ret = SetPointCmdNCAck((unsigned char)(0x40|UKINFOAD),infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTCAck((unsigned char)(0x40|UKINFOAD),infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control privilege not in %s center! NACTCON C_SE_NC cmd %d\n",sProcRec[pnum].NameID,infoptr);
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
			{
				OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_NC\n",sProcRec[pnum].NameID,sedef.name[infoptr]);
				

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NC)
					ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}

			strcpy(dbname,sedef.name[infoptr]);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NC:%s No this set point in the database file!\n",dbname);
				

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NC)
					ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
			{
				OutputDspInfo(OUTPUT_ERROR, "C_SE_NC:%s this point is not ANA in the database file!\n",dbname);
				

				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)NACTCON;
				if(type_id==C_SE_NC)
					ret = SetPointCmdNCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				else
					ret = SetPointCmdTCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
				return(ret);
			}
			if(qoc & 0x80)
			{
				sebuf.term_flag = 0;
				sebuf.cot=(unsigned char)ACTCON;
				if(type_id==C_SE_NC)
				{
					OutputDspInfo(0, "?????Select C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);
					ret = SetPointCmdNCAck((unsigned char)ACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
					return(ret);
				}
				else
				if((stTim = CP56CmdTimeProc(&recv_data.data[recvdatapt][14])) != (time_t)-1 )
				{
					if((tim-stTim)<cmd_trans_delay)
					{
						OutputDspInfo(OUTPUT_ERROR, "?????Select C_SE_TC: addr=%04x fvalue= %f Received\n",infoaddr,value);
						ret = SetPointCmdTCAck((unsigned char)ACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
						return(ret);
					}
				}
				OutputDspInfo(0, "?????Delayed Select C_SE_TC: addr=%04x fvalue= %f Received\n",infoaddr,value);

				ret = SetPointCmdTCAck((unsigned char)NACTCON,infoaddr,value,qoc,sockfd);	/* SET Point NActCon:7 qoc:0x00 */
				return(ret);
			}
			OutputDspInfo(0, "#####Set-Point C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);
			if(type_id==C_SE_NC)
				ret = SetPointCmdNCAck((unsigned char)ACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTCAck((unsigned char)ACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */

			cmdtype = sedef.cmdtype[infoptr];
			SEComdSendToLCU(cmdtype, value, &dbcommpt);  

			sebuf.term_flag = 1;
			sebuf.cot=(unsigned char)ACTTERM;
			if(type_id==C_SE_NC)
				ret = SetPointCmdNCAck((unsigned char)ACTTERM,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
			else
				ret = SetPointCmdTCAck((unsigned char)ACTTERM,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x00 */
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
				OutputDspInfo(OUTPUT_ERROR, "iec104s:can't open(wb) the file %s!\n",cmdrecfile);
				
			}
			else
			{
				fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d %5.2f infoaddr=%04x %s\n",tmpt.tm_year+1900,
					tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
					tmpt.tm_sec,value,infoaddr,sedef.name[infoptr]);
				fclose(fagc);
			}
#endif
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
			OutputDspInfo(0, "=====DEACT Cause of C_SE_NC: addr=%04x fvalue= %f Received\n",infoaddr,value);

			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			if(type_id==C_SE_NC)
				ret = SetPointCmdNCAck((unsigned char)DEACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			else
				ret = SetPointCmdTCAck((unsigned char)DEACTCON,infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_NC: received %d wrong!!!\n",causetra);

			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			if(type_id==C_SE_NC)
				ret = SetPointCmdNCAck((unsigned char)(0x40|UKCOT),infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			else
				ret = SetPointCmdTCAck((unsigned char)(0x40|UKCOT),infoaddr,value,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		break;

	case C_SE_ND:	/* 136 confirm of multi set-point command normalized value */

		infonum = varstru & 0x7F;
		qoc = recv_data.data[recvdatapt][6+infonum*5];	/* QOC:= S/E[8] QL[1..7] */

		sebuf.typeId = type_id;
		sebuf.infonum = infonum;
		sebuf.rcvpnt = recvdatapt;
		sebuf.qoc = qoc;
/* 		sebuf.ack_flag = 1;	 */	/* Set to 1 when ACK by SERespSend(int sock) */
		sebuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		infonum = varstru & 0x7F;			/* number of information object */
		datalength = 5*infonum+7;
		if(datalength != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_SE_ND:ASDU length error! type_id=%d,length:%u != %u\n",
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
					OutputDspInfo(OUTPUT_ERROR, "!!!!!C_SE_ND:%04x info address wrong! No this point in SET Point Control Table!\n",infoaddr);
						
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)(0x40|UKINFOAD);
					ret = SetPointCmdNDAck((unsigned char)(0x40|UKINFOAD),infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:6F(111) qoc:0x00 */
					return(ret);
				}

				if(Get_Cas_Ctrl_Priv(sProcRec[pnum].ctrl_priv) == FALSE)
				{
					OutputDspInfo(OUTPUT_WARN, "The control privilege not in %s center! NACTCON C_SE_ND cmd %d\n",sProcRec[pnum].NameID,infoptr);
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)NACTCON;
					ret = SetPointCmdNDAck((unsigned char)NACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:0x47 qoc:0x00 */
					return(ret);
				}

				OutputDspInfo(0, "#####Multi Set-Point C_SE_ND: addr=%04x value= %d Received\n",infoaddr,setvalue);

				if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
				{
					OutputDspInfo(OUTPUT_WARN, "The control condition for %s cmd %s not fulfiled! NACTCON C_SE_ND\n",sProcRec[pnum].NameID,sedef.name[infoptr]);
						
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
					value=(float)(((setvalue+32768)*(sedef.hirange[infoptr]-sedef.lorange[infoptr]))/65535.0
						 + sedef.lorange[infoptr]);
				}
				strcpy(dbname,sedef.name[infoptr]);
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(OUTPUT_ERROR, "C_SE_ND:%s No this set point in the database file!\n",dbname);
						
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)NACTCON;
					ret = SetPointCmdNDAck((unsigned char)NACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:0x47 qoc:0x00 */
					continue;
				}
				if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
				{
					OutputDspInfo(OUTPUT_ERROR, "C_SE_ND:%s this point is not ANA in the database file!\n",dbname);
						
					sebuf.term_flag = 0;
					sebuf.cot=(unsigned char)NACTCON;
					ret = SetPointCmdNDAck((unsigned char)NACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:0x47 qoc:0x00 */
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
					OutputDspInfo(OUTPUT_ERROR, "can't open(wb) the file %s!\n",cmdrecfile);
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
			ret = SetPointCmdNDAck((unsigned char)ACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			ret = SetPointCmdNDAck((unsigned char)ACTTERM,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		else if((causetra & 0x3F) == DEACT)
		{
#ifdef SUN_OS
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			OutputDspInfo(0, "=====DEACT Cause of C_SE_ND: infoaddr= %04x infonum=%d Received\n",infoaddr,infonum);

			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)DEACTCON;
			ret = SetPointCmdNDAck((unsigned char)DEACTCON,infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of C_SE_ND command received %d wrong!!!\n",causetra);

			sebuf.term_flag = 0;
			sebuf.cot=(unsigned char)(0x40|UKCOT);
			ret = SetPointCmdNDAck((unsigned char)(0x40|UKCOT),infonum,recvdatapt,qoc,sockfd);	/* SET Point ActCon:7 qoc:0x80 */
			return(ret);
		}
		break;

	case M_ME_NC:	/* measure value without time tag:13 */
	case M_ME_TC:	/* measure value with time tag:14 */
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
				OutputDspInfo(OUTPUT_ERROR, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				return(FALSE);
			}

			lanpt = 0;
#ifdef SUN_OS
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif

			infoptr = infoaddr - se_cmd_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr<0)||(infoptr >= sedef.number))
				{
					OutputDspInfo(OUTPUT_ERROR, "M_ME_NC SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,sedef.number);

					return(FALSE);
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
				qoc = recv_data.data[recvdatapt][datapoint+4];

#ifdef SUN_OS
				CrossCopy(&value,&recv_data.data[recvdatapt][datapoint],4);
#else
				memcpy(&value,&recv_data.data[recvdatapt][datapoint],4);
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
					tmpt=*localtime(&tim);
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
				OutputDspInfo(OUTPUT_ERROR, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				return(FALSE);
			}

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;

#ifdef SUN_OS
				CrossCopy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
				infoptr = infoaddr - se_cmd_base_addr;
				if((infoptr<0)||(infoptr > sedef.number))
				{
					OutputDspInfo(OUTPUT_ERROR, "M_ME_NC SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,sedef.number);

					return(FALSE);
				}
				strcpy(dbname,sedef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(OUTPUT_ERROR, "\nIEC No this point name=%s in db\n",dbname);
											continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+7];
#ifdef SUN_OS
				CrossCopy(&value,&recv_data.data[recvdatapt][datapoint+3],4);
#else
				memcpy(&value,&recv_data.data[recvdatapt][datapoint+3],4);
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
					tmpt=*localtime(&tim);
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
					Sleep(1000);
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
		return(TRUE);
		break;

	case P_ME_NA:	/* 110 confirm of Parameter-value command */
					
/* 		infonum = varstru & 0x7F; */
		qoc = recv_data.data[recvdatapt][11];			/* QPM:= POP[8]:1 LPC[7] KPA[1..6] */
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#endif
		pmbuf.typeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.setvalue = setvalue;
		pmbuf.qpm = qoc;		/* C dir POP=1 M dir LPC=1 POP=0 */
/* 		pmbuf.ack_flag = 1;		/* Set to 1 when ACK by PMRespSend(int sock) */
		pmbuf.ack_flag = 0;		/* Set to 0 when ACK directly. */

		if(12 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received P_ME_NA:ASDU length error! type_id=%d,length=%u\n",
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
				OutputDspInfo(OUTPUT_ERROR, "!!!!!P_ME_NA:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = ParamValueNAAck((unsigned char)(0x40|UKINFOAD),qoc,infoaddr,setvalue,sockfd);
				return(ret);
			}

			if(MeType == 0)
			{
				value=(float)((setvalue*(medef.hirange[infoptr]-medef.lorange[infoptr]))/32767.0
					 + medef.lorange[infoptr]);
			}
			else
			{
				value=(float)(((setvalue+32768)*(medef.hirange[infoptr]-medef.lorange[infoptr]))/65535.0
					 + medef.lorange[infoptr]);
			}
			OutputDspInfo(0, "##### P_ME_NA: addr=%04X setvalue=%04X value=%f qpm=%02X Received\n",infoaddr,setvalue,value,qoc);

			switch(qoc & 0x3F)
			{
			case	1:

				medef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMETable(MEDEF_NORM); */
				OutputDspInfo(0, "Parameter threshold command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);

				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
				OutputDspInfo(0, "Parameter smothing factor command P_ME_NA: addr=%04x value = %04x %f Received\n",infoaddr,setvalue,value);

				break;

			case	3:

/* 				medef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMETable(MEDEF_NORM); */
				OutputDspInfo(0, "Parameter low limit command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);

				break;

			case	4:

/* 				medef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNAAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMETable(MEDEF_NORM); */
				OutputDspInfo(0, "Parameter high limit command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);

				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				ret = ParamValueNAAck((unsigned char)NACTCON,qoc,infoaddr,setvalue,sockfd);
				OutputDspInfo(0, "Parameter reserved command P_ME_NA: addr=%04x value= %04x %f Received\n",infoaddr,setvalue,value);

				break;
			}/* switch(qoc & 0x3F) */
			return(ret);
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of P_ME_NA command received %d wrong!!!\n",causetra);
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			ret = ParamValueNAAck((unsigned char)(0x40|UKCOT),qoc,infoaddr,setvalue,sockfd);
			return(ret);
		}
		break;

	case P_ME_NB:	/* 111 confirm of Parameter-value command */
					
/* 		infonum = varstru & 0x7F; */
		qoc = recv_data.data[recvdatapt][11];			/* QPM:= POP[8] LPC[7] KPA[1..6] */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	/* value of set */
#endif

		pmbuf.typeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.setvalue = setvalue;
		pmbuf.qpm = qoc;			/* C dir POP=1 M dir LPC=1 POP=0 */
/* 		pmbuf.ack_flag = 1;	 */		/* Set to 1 when ACK by PMRespSend(int sock) */
		pmbuf.ack_flag = 0;			/* Set to 0 when ACK directly. */

		if(12 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received P_ME_NB:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			pmbuf.cot=(unsigned char)NACTCON;
			ret = ParamValueNBAck((unsigned char)NACTCON,qoc,infoaddr,setvalue,sockfd);
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr;
			if((infoptr<0)||(infoptr>=medef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!P_ME_NB:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = ParamValueNBAck((unsigned char)(0x40|UKINFOAD),qoc,infoaddr,setvalue,sockfd);
				return(ret);
			}
			value= setvalue * medef.scale[infoptr];
			OutputDspInfo(0, "##### P_ME_NB: addr=%04X setvalue=%04X value=%f qpm=%02X Received\n",infoaddr,setvalue,value,qoc);
//			
			switch(qoc & 0x3F)
			{
			case	1:

				medef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMEScaleTable(MEDEF_SCALE); */
				OutputDspInfo(0, "Parameter threshold command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);

				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
				OutputDspInfo(0, "Parameter smothing factor command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);

				break;

			case	3:

/* 				medef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMEScaleTable(MEDEF_SCALE); */
				OutputDspInfo(0, "Parameter low limit command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);

				break;

			case	4:

/* 				medef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNBAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,setvalue,sockfd);
/* 				SaveMEScaleTable(MEDEF_SCALE); */
				OutputDspInfo(0, "Parameter high limit command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);

				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				ret = ParamValueNBAck((unsigned char)NACTCON,qoc,infoaddr,setvalue,sockfd);
				OutputDspInfo(0, "Parameter reserved command P_ME_NB: addr=%04x value=%04x %f Received\n",infoaddr,setvalue,value);

				break;
			}/* switch(qoc & 0x3F) */
			return(ret);
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of P_ME_NB command received %d wrong!!!\n",causetra);
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			ret = ParamValueNBAck((unsigned char)(0x40|UKCOT),qoc,infoaddr,setvalue,sockfd);
			return(ret);
		}
		break;

	case P_ME_NC:	/* 112 confirm of Parameter-value command */
					
/* 		infonum = varstru & 0x7F; */
		qoc = recv_data.data[recvdatapt][13];	/* QPM:= POP[8] LPC[7] KPA[1..6] */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&value,&recv_data.data[recvdatapt][9],4);	/* value of set */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&value,&recv_data.data[recvdatapt][9],4);	 /* value of set */
#endif

		pmbuf.typeId = type_id;
		pmbuf.infoaddr = infoaddr;
		pmbuf.fvalue = value;
		pmbuf.qpm = qoc;			/* C dir POP=1 M dir LPC=1 POP=0 */
/* 		pmbuf.ack_flag = 1;	 */		/* Set to 1 when ACK by PMRespSend(int sock) */
		pmbuf.ack_flag = 0;			/* Set to 0 when ACK directly. */

		if(14 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received P_ME_NC:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			pmbuf.cot=(unsigned char)NACTCON;
			ret = ParamValueNCAck((unsigned char)NACTCON,qoc,infoaddr,value,sockfd);
			return(ret);
		}
		if((causetra & 0x3F) == ACT)
		{
			infoptr = infoaddr - pm_base_addr;
			if((infoptr<0)||(infoptr>=medef.number))
			{
				OutputDspInfo(OUTPUT_ERROR, "!!!!!P_ME_NC:%04x info address wrong! No this point in ME Table!\n",infoaddr);
				

				pmbuf.cot=(unsigned char)(0x40|UKINFOAD);
				ret = ParamValueNCAck((unsigned char)(0x40|UKINFOAD),qoc,infoaddr,value,sockfd);
				return(ret);
			}
			OutputDspInfo(0, "##### P_ME_NC: addr=%04x value=%f qpm=%02X Received\n",infoaddr,value,qoc);
//			
			switch(qoc & 0x3F)
			{
			case	1:

				medef.databand[infoptr] = value;
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
/* 				SaveMEFloatTable(MEDEF_FLOAT); */
				OutputDspInfo(0, "Parameter threshold command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);

				break;

			case	2:

				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
				OutputDspInfo(0, "Parameter smothing factor command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);

				break;

			case	3:

/* 				medef.lorange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
/* 				SaveMEFloatTable(MEDEF_FLOAT); */
				OutputDspInfo(0, "Parameter low limit command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);

				break;

			case	4:

/* 				medef.hirange[infoptr] = value; */
				pmbuf.cot=(unsigned char)ACTCON;
				pmbuf.qpm = (qoc & 0x7F) | 0x40;	/* C dir POP=1 M dir LPC=1 POP=0 */
				ret = ParamValueNCAck((unsigned char)ACTCON,pmbuf.qpm,infoaddr,value,sockfd);
/* 				SaveMEFloatTable(MEDEF_FLOAT); */
				OutputDspInfo(0, "Parameter high limit command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);

				break;

			default:
				pmbuf.cot=(unsigned char)ACTCON;
				ret = ParamValueNCAck((unsigned char)NACTCON,qoc,infoaddr,value,sockfd);
				OutputDspInfo(0, "Parameter reserved command P_ME_NC: addr=%04x value=%f Received\n",infoaddr,value);

				break;
			}/* switch(qoc & 0x3F) */
			return(ret);
		}
		else
		{
			OutputDspInfo(OUTPUT_WARN, "Cause of P_ME_NC command received %d wrong!!!\n",causetra);
			pmbuf.cot=(unsigned char)(0x40|UKCOT);
			ret = ParamValueNCAck((unsigned char)(0x40|UKCOT),qoc,infoaddr,value,sockfd);
			return(ret);
		}
		break;

	case C_IC_NA:	/* 100 confirm of general interrogation command */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		qoc = recv_data.data[recvdatapt][9];			/* QOI[1..8]:= 20 general interrogation */
									/* 21: group1... 34:group14...36:group16 */
		icbuf.typeId = type_id;
		icbuf.qoi = qoc;
		icbuf.ack_flag = 0;

		if(10 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_IC_NA:ASDU length error! type_id=%d,length=%u\n",
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

			if(qoc==INTROGEN)
			{
				interrogationflag = 1;
				OutputDspInfo(0, "IEC general interrogation %x command is received!\n",qoc);
				scanflag = 0;
				scan_tim = tim;
				
				m_cRespGenInterroGrpNum = 0;
				icbuf.cot = (unsigned char)ACTCON;
				ret = GeneralInterrogationAck((unsigned char)ACTCON,INTROGEN,sockfd);
				if(ret == FALSE)
					return(FALSE);
				ret = RespondGeneralInterrogation(sockfd);
				return(ret);
			}
// 			else if(qoc>=INTRO1 || qoc<=INTRO16)
// 			{
// 				icgroupflag = 1;
// 				OutputDspInfo(0, "IEC interrogation group %d command is received!\n",qoc-INTROGEN);
// 				scanflag = 0;
// 				scan_tim = tim;
// 
// 				m_cRespICGrpNum = 0;
// 				icbuf.cot = (unsigned char)ACTCON;
// 				ret = GeneralInterrogationAck((unsigned char)ACTCON,qoc,sockfd);
// 				if(ret == FALSE)
// 					return(ret);
// 				ret = RespondGroupInterrogation(qoc,sock);	/* group??? */
// 				return(ret);
// 			}
			else
			{
				OutputDspInfo(OUTPUT_WARN, "IEC interrogation command qoi %d error\n",qoc);
				icgroupflag = 0;
				icbuf.cot = (unsigned char)NACTCON;
				m_cRespICGrpNum = 0;
				ret = GeneralInterrogationAck((unsigned char)NACTCON,qoc,sockfd);
				return(ret);
			}
			break;

			
		case DEACT:		/* general interrogation command deact confirm */

			OutputDspInfo(0, "IEC general interrogation %x DEACT command is received!\n",qoc);
			icbuf.cot = (unsigned char)DEACTCON;
			if(qoc==INTROGEN)
				interrogationflag = 0;
			else
				icgroupflag = 0;

			ret = GeneralInterrogationAck((unsigned char)DEACTCON,qoc,sockfd);
			return(ret);
			break;

		default:
			OutputDspInfo(OUTPUT_WARN, "C_IC_NA: Received cause of transmission=%2X qoi=%dwrong!\n",causetra,qoc);
			icbuf.cot = (unsigned char)(0x40|UKCOT);
			ret = GeneralInterrogationAck((unsigned char)(0x40|UKCOT),qoc,sockfd);
			return(ret);
			break;
		}/* switch(causetra & 0x3F) */
 					
		break;

	case C_CI_NA:	/* 101 confirm of counter interrogation command */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		qoc = recv_data.data[recvdatapt][9];			/* QCC[1..6]:= 5: general 1: group1... 4:group4 */
									/* FRZ[7..8]:= 0:read 1:freeze 2:freeze and reset 3:reset */
		cibuf.typeId = type_id;
		cibuf.qcc = qoc;
		cibuf.ack_flag = 1;

		if(10 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_CI_NA:ASDU length error! type_id=%d,length=%u\n",
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
					OutputDspInfo(0, "Counter interrogation %02X Read command is received!\n",qoc);

					cibuf.cot = (unsigned char)ACTCON;
					ret = CounterInterrogationAck((unsigned char)ACTCON,cibuf.qcc,sockfd);
					return(ret);
				}
				else	/* counter interrogation group */
				{
					if((qoc<1)||(qoc>4))
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d Read command is received!\n",qoc,itdef.number);

						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
					if(((qoc-1)*CI_GROUP_NUM) >= itdef.number)
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d Read command is received!\n",qoc,itdef.number);

						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
					OutputDspInfo(0, "Counter group %02X Read command is received!\n",qoc);

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
					OutputDspInfo(0, "Counter interrogation %02X FRZ command is received!\n",qoc);

					cibuf.cot = (unsigned char)ACTCON;
					ret = CounterInterrogationAck((unsigned char)ACTCON,cibuf.qcc,sockfd);
					return(ret);
				}
				else	/* counter interrogation group */
				{
					if(((qoc&0x3F)<1)||((qoc&0x3F)>4))
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d FRZ command is received!\n",(qoc&0x3F),itdef.number);

						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
					if((((qoc&0x3F)-1)*CI_GROUP_NUM) >= itdef.number)
					{
						OutputDspInfo(0, "Counter group %02X > itnum %d FRZ command is received!\n",(qoc&0x3F),itdef.number);

						cibuf.cot = (unsigned char)(0x40|UKINFOAD);
						ret = CounterInterrogationAck((unsigned char)(0x40|UKINFOAD),cibuf.qcc,sockfd);
						return(ret);
					}
					OutputDspInfo(0, "Counter group %02X FRZ command is received!\n",qoc);

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
				OutputDspInfo(0, "IEC counter interrogation %x DEACT command is received!\n",qoc);

				cibuf.cot = (unsigned char)DEACTCON;
				ret = CounterInterrogationAck((unsigned char)DEACTCON,cibuf.qcc,sockfd);
				return(ret);
			}
			else	/* counter interrogation group */
			{
				OutputDspInfo(0, "IEC interrogation group %x DEACT command is received!\n",qoc);

				cibuf.cot = (unsigned char)DEACTCON;
				cigroupflag = 0;
				ret = CounterInterrogationAck((unsigned char)DEACTCON,cibuf.qcc,sockfd);
				return(ret);
			}
			break;

		default :
			OutputDspInfo(OUTPUT_WARN, "C_CI_NA: Received cause of transmission=%2x qcc=%x wrong!\n",causetra,qoc);
		
			cibuf.cot = (unsigned char)(0x40|UKCOT);
			ret = CounterInterrogationAck((unsigned char)(0x40|UKCOT),cibuf.qcc,sockfd);									
			return(ret);
			break;
		}/* switch(causetra & 0x3F) */
					
		break;

	case C_RD_NA:	/* 102 confirm of read command */
				
#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		rdbuf.typeId = type_id;
		rdbuf.infoaddr = infoaddr;
		rdbuf.ack_flag = 0;

		if(9 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_RD_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
		
			rdbuf.cot = (unsigned char)NACTCON;
			ret = ReadCmdAck((unsigned char)NACTCON,infoaddr,sockfd);
			return(ret);
		}
		if((causetra & 0x3F)==REQ)	/* read command req confirm */
		{
			OutputDspInfo(0, "IEC read command C_RD_NA infoaddr=%d is received!\n",infoaddr);

			rdbuf.cot = REQ;
			ret = ReadCmdResp(infoaddr,sockfd);									
			return(ret);
		}
		else
		{
			OutputDspInfo(0, "C_RD_NA: Received cause of transmission=%2x wrong!\n",causetra);
		
			rdbuf.cot = (unsigned char)(0x40|UKCOT);
			ret = ReadCmdAck((unsigned char)(0x40|UKCOT),infoaddr,sockfd);
			return(ret);
		}
		break;

	case C_CS_NA:	/* 103 confirm of clock synchronization command */
		OutputDspInfo(0, "clock synchronization command frame C_CS_NA received!\n");

		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		Swap2Byte(&infoaddr);

		if(16 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_CS_NA:ASDU length error! type_id=%d,length=%u\n",
					type_id,recv_data.length[recvdatapt]);
			return(FALSE);
		}

		if(infoaddr!=0)
			return ClockSynchroniseAck((unsigned char)(0x40|UKINFOAD), sockfd);

		if((causetra & 0x3F) == ACT)
		{
			struct tm	remoteTime;

			memcpy(&ms, &recv_data.data[recvdatapt][9],2);
			Swap2Byte(&ms);
			remoteTime.tm_sec = ms/1000;
			remoteTime.tm_min = recv_data.data[recvdatapt][11] & 0x3f;
			remoteTime.tm_hour = recv_data.data[recvdatapt][12] & 0x1f;
			remoteTime.tm_mday = recv_data.data[recvdatapt][13] & 0x1f;
			remoteTime.tm_mon = (recv_data.data[recvdatapt][14] & 0x0f) - 1;
			remoteTime.tm_year =  (recv_data.data[recvdatapt][15] & 0x7f) + 100;
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
				return ClockSynchroniseAck(NACTCON, sockfd);		/* ACK clock synchronize */
			}

			if((stTim = mktime(&remoteTime)) != (time_t)-1 )
			{
				if(!Gps)
				{
					int ret;
					
#ifdef WINDOWS_OS
					SYSTEMTIME		sys_tm;
					ms = ms + cd_delay_time;
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
					ret= ClockSynchroniseAck(ACTCON, sockfd);		/* ACK clock synchronize */
					SetLocalTime(&sys_tm);
#else
					struct timeval	sTimeVal;
					ms = ms + cd_delay_time;
					sTimeVal.tv_usec = (ms%1000)*1000;
					sTimeVal.tv_sec = stTim + ms/1000;
					ret= ClockSynchroniseAck(ACTCON, sockfd);		/* ACK clock synchronize */
					settimeofday(&sTimeVal,NULL);
#endif

					ClockSyncflag= 1;
					return ret;
				}
				else
					return ClockSynchroniseAck(NACTCON, sockfd);		/* ACK clock synchronize */
			}
			else
			{
				OutputDspInfo(OUTPUT_ERROR, "clock synchronization: remote time error!\n");
				return ClockSynchroniseAck(NACTCON, sockfd);		/* ACK clock synchronize */
			}
			return TRUE;
		}					
		else
			return ClockSynchroniseAck((unsigned char)(0x40|UKCOT), sockfd);

		break;

	case C_TS_NA:	/* test command 104 */
	case C_TS_TA:	/* test command 107 */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* FBP:AA55 Fix test word */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		memcpy(&setvalue,&recv_data.data[recvdatapt][9],2);	 /* FBP:AA55 Fix test word */
#endif
		OutputDspInfo(0, "test request frame C_TS_NA received!\n");

		Testflag = 1;

		testbuf.typeId = type_id;

		if((causetra & 0x3F) == ACT)
		{
			if(infoaddr==0)
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
			testbuf.cot = (unsigned char)(0x40|UKCOT);;
		}

		if(type_id==C_TS_NA)
			ret = RespondTestAck(setvalue,sockfd);
		else
			ret = RespondTestTAAck(setvalue,sockfd);
		return(ret);
		break;

	case C_RP_NA:	/* Reset process command 105 */

#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
#endif
		qoc = recv_data.data[recvdatapt][9];	/* QRP[1..8]:= 1: general reset 2: SOE buffer reset */

		rpbuf.typeId = type_id;
		rpbuf.qrp = qoc;

		if(10 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_RP_NA:ASDU length error! type_id=%d,length=%u\n",
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
			rpbuf.cot = (unsigned char)(0x40|UKCOT);
		}
		ret = ResetProcessAck(sockfd);
		return(ret);
		break;

	case C_CD_NA:	/* Delay Acquire command 106 */

		cdbuf.typeId = type_id;

#ifdef SUN_OS
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);	/* address of object */
		CrossCopy(&cdbuf.cp16time,&recv_data.data[recvdatapt][9],2);	 /* CP16time2a:ms to second */
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&cdbuf.cp16time,&recv_data.data[recvdatapt][9],2);	 /* CP16time2a:ms to second */
#endif
		OutputDspInfo(0, "Dealy Acquire command frame C_CD_NA received!\n");

		if(11 != recv_data.length[recvdatapt])
		{
			OutputDspInfo(OUTPUT_ERROR, "received C_CD_NA:ASDU length error! type_id=%d,length=%u\n",
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
			}
			else
			{
				cdbuf.cot = (unsigned char)ACTCON;
				OutputDspInfo(0, "Delay acquisition cmd cp16time=%d\n",cdbuf.cp16time);
			}
		}
		else if((causetra & 0x3F) == SPONT)	/* Delay Transmit */
		{
			if(infoaddr!=0)
			{
				cdbuf.cot = (unsigned char)(0x40|UKINFOAD);
			}
			else
			{
				cdbuf.cot = (unsigned char)ACTCON;
				cd_delay_time = cdbuf.cp16time;
				OutputDspInfo(0, "Delay Transmit cd_delay_time=%d\n",cd_delay_time);
			}
		}
		else
			cdbuf.cot = (unsigned char)NACTCON;

		ret = TimeDelayAck(sockfd);
		return(ret);
		break;

	default :
		mirbuf.cot=(unsigned char)(0x40|UKTYPE);
		mirbuf.pnt=recvdatapt;
/* 		MirrorNAckflag = 1; */
		ret = MirrorNAck(mirbuf.cot,mirbuf.pnt,sockfd);

		OutputDspInfo(OUTPUT_WARN, "IEC type_id = %u is unavailable at this time!\n",type_id);
		
		
		return(ret);
		break;
	}/* switch(type_id) */
}



void InitSoeBuf(void)
{
	int	i;

	soebuf.sendpnt = soebuf.bufpnt = soebuf.ackpnt = soebuf.count =0;
	for(i=0; i<SOE_NUM; i++)
		soebuf.nr_send[i]= -1;
	soebuf.nr_ack= 0;
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

void *	IEC104_SOE(void *arg)
{
#ifdef BY_RECORD

	SHM_RECORD *shm_rec_addr;
	IND_REC	*pInd_alm;
	unsigned char	type_id;
	short	rcv_end,rcv_pnt;
	POINTER	rcv_ptr;
#else
	unsigned char	name_buf[NAME_SIZE];
	DMS_COMMON	dbentry;
#endif
	POINTER	ptr;
	unsigned char	stat;
 	unsigned char	vti;
	unsigned short	pntnum,soe_head;
	extern int	localinitflag;


	OutputDspInfo(OUTPUT_WARN, "------------IEC104_SOE start running !!!-----------\n");

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
			if(spdef.ptr[pntnum].dev_id == 0)
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
 				if(spdef.TimeTag[pntnum] == 2)
					soebuf.soerec[soe_head].typeId = M_SP_TB;
				else if(spdef.TimeTag[pntnum] == 1)
					soebuf.soerec[soe_head].typeId = M_SP_TA;
				else
					continue;

				soebuf.soerec[soe_head].infoaddr = sp_base_addr + pntnum;
				soebuf.soerec[soe_head].stat = orgspsoebuf[pntnum] = stat;	/* QDS IV NT SB BL RES3 SPI */
				ReadEntryById(&ptr, &dbentry);
				soebuf.soerec[soe_head].time = dbentry.var.ind.change_time;
				soebuf.soerec[soe_head].time.year = soebuf.soerec[soe_head].time.year-1900;

				OutputDspInfo(0, "soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						spdef.name[pntnum],soebuf.soerec[soe_head].stat,
						soebuf.soerec[soe_head].infoaddr,
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
			if(dpdef.ptr[pntnum].dev_id == 0)
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
 				if(dpdef.TimeTag[pntnum] == 2)
					soebuf.soerec[soe_head].typeId = M_DP_TB;
				else if(dpdef.TimeTag[pntnum] == 1)
					soebuf.soerec[soe_head].typeId = M_DP_TA;
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

#else /*by record*/		
/*	获取记录区共享内存地址  */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		OutputDspInfo(0, "\niec104s: Fail to shmat shm_rec_addr: errno=%d\n",errno);
		exit(0);
	}
	
	OutputDspInfo(0, "\niec104s:: shmat shm_rec_addr: OK!\n");

	rcv_end = shm_rec_addr->head.header_ptr;
	for(;;)
	{
		IND_DATA		dbstat;
		
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
 					if(spdef.TimeTag[pntnum] == 2)
						soebuf.soerec[soe_head].typeId = M_SP_TB;
					else if(spdef.TimeTag[pntnum] == 1)
						soebuf.soerec[soe_head].typeId = M_SP_TA;
					else
						continue;

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
/*
			if(pntnum<spdef.number)
				continue;
*/
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
					if(dpdef.TimeTag[pntnum] == 2)
 						soebuf.soerec[soe_head].typeId = M_DP_TB;
					else if(dpdef.TimeTag[pntnum] == 1)
						soebuf.soerec[soe_head].typeId = M_DP_TA;
					else
						continue;

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
					if(stdef.TimeTag[pntnum] == 2)
 						soebuf.soerec[soe_head].typeId = M_ST_TB;
					else if(stdef.TimeTag[pntnum] == 1)
						soebuf.soerec[soe_head].typeId = M_ST_TA;
					else
						continue;

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
				} //if
			}//for(stdef)
		}//while(rec)
	} //for(;;)
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
	strcpy(cmd_buf.usr_login_name,"IEC104S");
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

	cmd_buf.status.state=0;  //GCL
	cmd_buf.status.result=0;
	if (cmd_buf.type_id == C_GATE)
		cmd_buf.status.opr=13;
	else
		cmd_buf.status.opr=0;
	cmd_buf.status.src=0;  //GCL
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
	strcpy(cmd_buf.usr_login_name,"IEC104S");
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

int Get_Cmd_Ctrl_Priv(char * interlockname,unsigned char stat,DMS_COMMON *pdbentry)
{
	if(strcmp(interlockname,"NULL")==0)	
		return(TRUE);
	if(InterLock_Judge( interlockname )!=1)
	{
		OutputDspInfo(0, "Ctrl condition %s not fulfiled!\n",interlockname);
		return(FALSE);
	}

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

time_t CP56CmdTimeProc(unsigned char *recv_buf)
{
	unsigned short ms;
	struct tm	sTimeCmd;

	memcpy(&ms, recv_buf, 2);
	Swap2Byte(&ms);
	sTimeCmd.tm_sec = ms/1000;
	sTimeCmd.tm_min = recv_buf[2] & 0x3f;
	sTimeCmd.tm_hour = recv_buf[3] & 0x1f;
	sTimeCmd.tm_mday = recv_buf[4] & 0x1F;
	sTimeCmd.tm_mon = (recv_buf[5] & 0x0F) - 1;
	sTimeCmd.tm_year = (recv_buf[6] & 0x7f) + 100;
	sTimeCmd.tm_isdst = 0;

	OutputDspInfo(0, "Remote clock %04d/%02d/%02d %02d:%02d:%02d %03d!\n",sTimeCmd.tm_year+1900,sTimeCmd.tm_mon+1,
			sTimeCmd.tm_mday,sTimeCmd.tm_hour,sTimeCmd.tm_min,sTimeCmd.tm_sec,ms%1000);
	
	return mktime(&sTimeCmd);
}

int Get_Cas_Ctrl_Priv(char * ctrl_priv)
{
	char		name_buf[NAME_SIZE];
	DMS_COMMON	dbentry;
	unsigned short	dbstat;

	if(strcmp(ctrl_priv,"NULL")==0)	
		return(TRUE);

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




