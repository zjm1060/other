
#include "/home/ems/system/iec/v4.0a/inc/iec104_com.h"

#define	DEBUG

extern	unsigned char	sendbuf[1024];
extern  char	infobuf[1024];
extern	SHM_RECORD *shm_rec_addr;
/*extern	SHM_RECV_DATA	*shm_recv_addr;*/
ASDUCOMDDATA	send_data;
ASDUDATA	recv_data;
extern	short	cmd_end;
extern	int	cmd_ack;
extern	unsigned char	comdflag;
extern	unsigned short	nr_recv,nr_send,nr_ack,ns_ack,nr_rs;

extern	unsigned short	nASDUId;
extern	unsigned char	nSrcId;

extern	int	Gps;		/* 0: Without GPS; 1: With GPS */
extern	int	GiType;		/* General interrogation type> 1: M_SP_NA; 20: M_PS_NA */
extern	int	MeASDUType;	/* Me ASDU type> 0: M_ME_NA; 1: M_ME_ND; 3: M_ME_NC */
extern	int	MeType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
extern	int	SEType;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
/* extern	int		TimeTag;	/* Time tag> 0: no; 1: short; 2: long */
extern	int	sp_cos;		/* sp_cos> 0: sp/dp transmit once; 1: twice */
extern	int	Cmd_Type;	/*  0: single point ctrl C_SC_NA; 1: double point ctrl C_DC_NA */
extern	int	Cmd_Sel;	

extern	time_t	scan_period;
extern	time_t	cmd_period;
extern	time_t	test_period_t3;

extern	int		ProcNum;
extern	PROC_REC	sProcRec[MAX_PROC_NUM+1];
extern	unsigned char cur_stn_id;		//added by yyp 2011.03.09
SPDEF		spdef;
DPDEF		dpdef;
MEDEF		medef;
DCDEF		dcdef;
SEDEF		sedef;
ITDEF		itdef;
STDEF		stdef;
SCDEF		scdef;
RCDEF		rcdef;

CMDBUF		cmdbuf;
SEBUF		sebuf;

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

extern char logfilename[128];
extern struct tm  *ntime;
extern int oldday;

long GetTimefromCmd(struct net_cmd_data *p_rec);			//added by yyp 2011.03.09

void InitData(char *NameID)
{
	nASDUId = CMM_ADRESS;
	nSrcId = SRC_ADRESS;
	Gps = 1;		/* With GPS */
	GiType = 1;		/* 1:M_SP_NA; 20: M_PS_NA */
	MeASDUType = 3;		/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 3: M_ME_NC */
	MeType = 0;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	SEType =0;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
/* 	TimeTag = 2; */		/* time tag 0: no; 1: short; 2: long */
	sp_cos = 0;		/* sp_cos 0: sp/dp transmit once; 1: twice */
	Cmd_Type = 1;	/*  0: single point ctrl C_SC_NA; 1: double point ctrl C_DC_NA */
	Cmd_Sel = 1;	/*  0: cmd without select; 1: cmd with select */

	scan_period = SCAN_PERIOD;
	cmd_period = CMD_TIMEOUT;
	test_period_t3 = t3;

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

/* 	m_cRespGenInterroGrpNum=m_cRespCountGrpNum=m_cCyclicGrpNum=m_cScanGrpNum=0; */
	send_data.pnt = 0;
	recv_data.pnt = 0;

	cmd_ack = 0;
	cmdbuf.tpyeId = 0;
	cmdbuf.infoaddr = 0;
	cmdbuf.ack_flag = 0;
	cmdbuf.selected = 0;
	cmdbuf.term_flag = 0;
	cmdbuf.sel_tim = 0;

	sebuf.ack_flag = 0;

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

	GenMETable(NameID,MEDEF_FILE);
/*
	GenSTTable(NameID,STDEF_FILE);
	GenITTable(NameID,ITDEF_FILE);
*/
	GenSETable(NameID,SEDEF_FILE);

	GenDCTable(NameID,(char *)DCDEF_FILE);
/*
	GenSCTable(NameID,(char *)SCDEF_FILE);
	GenRCTable(NameID,(char *)RCDEF_FILE);

	SaveMETable(NameID,MEDEF_DATA);
*/
	nr_recv = nr_send = nr_ack = ns_ack = nr_rs = 0;
}

float Datcvt(short status,unsigned short data)
{
	float	result;

	switch(status&0x060)
		{
		case 0x20:
			result=(float)(data*0.1);
			break;
		case 0x40:
			result=(float)(data*0.01);
			break;
		case 0x60:
			result=(float)(data*0.001);
			break;
		default:
			result=(float)data;
		}

	if(status&0x80)
		result=-result;

	return(result);
}

int ReadStnDef(char *pStnDefFile)
{
	FILE	*fp;
	char	filenamestr[200];
	char	NameID[20], Ip1[40], Ip2[40], Ip3[40], Ip4[40];
	int		id;

	sprintf(filenamestr,"%s%s",HOMEPATH,pStnDefFile);
#ifdef DEBUG
	printf("\n\nRead config file %s!!!\n",filenamestr);
#endif
	ProcNum = 0;
	while( (fp=fopen(filenamestr,"r"))==NULL )
	{
		printf("\n### ALARM !!! IEC104: Can't open Station config file %s !!! \n",pStnDefFile);
		sleep(4);
	}

	while( fscanf(fp,"%s %d  %s  %s  %s  %s",NameID,&id,Ip1,Ip2,Ip3,Ip4)!=EOF )
	{	
		strcpy( sProcRec[ProcNum].NameID, NameID );
		sProcRec[ProcNum].id=id;
		strcpy( sProcRec[ProcNum].Ip1, Ip1 );
		strcpy( sProcRec[ProcNum].Ip2, Ip2 );
		strcpy( sProcRec[ProcNum].Ip3, Ip3 );
		strcpy( sProcRec[ProcNum].Ip4, Ip4 );
		sProcRec[ProcNum].Pid = -1;
		if(sProcRec[ProcNum].NameID[0] == '!')
		{
			printf("\nDisable Station name %s: IP1 = %s IP2 = %s  IP3 = %s IP4 = %s \n",sProcRec[ProcNum].NameID,
				sProcRec[ProcNum].Ip1,sProcRec[ProcNum].Ip2,sProcRec[ProcNum].Ip3,sProcRec[ProcNum].Ip4);
			continue;
		}
/* #ifdef DEBUG */
		printf("\nStation %d name %s: IP1 = %s IP2 = %s  IP3 = %s IP4 = %s \n",ProcNum,sProcRec[ProcNum].NameID,
			sProcRec[ProcNum].Ip1,sProcRec[ProcNum].Ip2,sProcRec[ProcNum].Ip3,sProcRec[ProcNum].Ip4);
/* #endif */
		ProcNum++;
	}
	fclose( fp );
/* #ifdef DEBUG */
	printf("\nRead config file %s: proc num = %d!!!\n\n",pStnDefFile,ProcNum);
/* #endif */
	return( true );
}

int GenConfigTable(char *pName, char *pDefFile)
{
	FILE	*fp;
	int	iTemp,iRet;
	char	strFlag[80];
	char	filenamestr[200];
	char	strType[80];

	sprintf(filenamestr,"%s%s_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead config file %s!!!\n",filenamestr);
#endif
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("\n####Can't open config file %s!!!\n",filenamestr);
		nASDUId = CMM_ADRESS;
		sleep(3);
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
			return(true);
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
			iRet = fscanf(fp,"%s",strType);
			if(iRet<=0)
			{
				printf("Config file %s Error! ME_ASDUTYPE = %d\n",pDefFile,iTemp);
				MeASDUType = 0;
				continue;
			}
			if(strcmp(strType,"M_ME_NA")== 0)
			{
#ifdef DEBUG
				printf("ME_ASDUTYPE = %s\n",strType);
#endif
				MeASDUType = 0;
			}
			else if(strcmp(strType,"M_ME_ND")== 0)
			{
#ifdef DEBUG
				printf("ME_ASDUTYPE = %s\n",strType);
#endif
				MeASDUType = 1;
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
		else if(strcmp(strFlag,"CMD_TYPE:")== 0)
		{
			iRet = fscanf(fp,"%s",strType);
			if(iRet<=0)
			{
				printf("Config file %s Error! CMD_TYPE = %s\n",pDefFile,strType);
				Cmd_Type = 1;
				continue;
			}
			if(strcmp(strType,"C_SC_NA")== 0)
			{
#ifdef DEBUG
				printf("CMD_TYPE = %s\n",strType);
#endif
				Cmd_Type = 0;
			}
			else if(strcmp(strType,"C_DC_NA")== 0)
			{
#ifdef DEBUG
				printf("CMD_TYPE = %s\n",strType);
#endif
				Cmd_Type = 1;
			}
			else
			{
				printf("Config file %s Error! CMD_TYPE = %s unknown!\n",pDefFile,strType);
				Cmd_Type = 1;
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
/*	fclose(fp);
	return(true);
*/
}

int GenSPTable(char *pName, char *pDefFile)		/* single point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	/*int		data_type2;*/
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead SPTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open SP file %s\n",filenamestr);
 		spdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SP File %s Define Error! SP Number = %d\n",filenamestr,iTemp);
		spdef.number = 0;
		fclose(fp);
		return(false);
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
		fscanf(fp,"%d %s %d %s",&iTemp,spdef.name[j],&spdef.asdutype[j],lgname);
		if(strcmp(spdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("SP %d pnt: %s %d %s\n",j,spdef.name[j],spdef.asdutype[j],lgname);
#endif
			strcpy(name_buf2,spdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				spdef.ptr[j].dev_id = 0;
				printf("#### SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				sprintf(infobuf,"#### SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				WriteDspInfo(infobuf);
	    			WriteLogFile(infobuf);
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
/* #ifdef DEBUG */
	printf("---%s GenSPTable: point nNumber = %d\n\n",pName,spdef.number);
/* #endif */
	return(true);
}

int GenDPTable(char *pName, char *pDefFile)		/* double point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	/*int		data_type2;*/
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp/*,iTemp1*/,iRet;
	int	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead DPTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open DP file %s\n",filenamestr);
 		dpdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####DP File %s Define Error! DP Number = %d\n",filenamestr,iTemp);
		dpdef.number = 0;
		fclose(fp);
		return(false);
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
		fscanf(fp,"%d %s %d %s",&iTemp,dpdef.name[j],&dpdef.asdutype[j],lgname);
		if(strcmp(dpdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
		//	printf("DP %d pnt: %s %d %s\n",j,dpdef.name[j],dpdef.asdutype[j],lgname);
#endif
			strcpy(name_buf2,dpdef.name[j]);

			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				dpdef.ptr[j].dev_id = 0;
				printf("#### DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				sprintf(infobuf,"#### DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				WriteDspInfo(infobuf);
	    			WriteLogFile(infobuf);
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
/* #ifdef DEBUG */
	printf("---%s GenDPTable: point nNumber = %d\n\n",pName,dpdef.number);
/* #endif */
	return(true);
}

int GenMETable(char *pName, char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
/*	int				data_type2;*/
	char			name_buf2[NAME_SIZE],lgname[36];

	int		iTemp,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead METable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("####can't open ME file %s\n",filenamestr);
 		medef.number = 0;
		sleep(3);
/* 		return(false); */
	}
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####ME File %s Define Error! ME Number = %d\n",filenamestr,iTemp);
		medef.number = 0;
		fclose(fp);
		return(false);
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
		fscanf(fp,"%d %s %f %f %f %f %d %d %s",&iTemp,medef.name[j],&medef.lorange[j],&medef.hirange[j],&medef.scale[j],&medef.databand[j],&medef.asdutype[j],&medef.timetag[j],lgname);
		if(strcmp(medef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
		//	printf("ME %d pnt: %s %f %f %f %f %d %d %s\n",j,medef.name[j],medef.lorange[j],medef.hirange[j],medef.scale[j],medef.databand[j],medef.asdutype[j],medef.timetag[j],lgname);
#endif
			strcpy(name_buf2,medef.name[j]);

			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				sprintf(infobuf,"#### ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				WriteDspInfo(infobuf);
	    			WriteLogFile(infobuf);
			}
		}
	}
/* #ifdef DEBUG */
	printf("---%s GenMETable: point nNumber = %d\n\n",pName,medef.number);
/* #endif */
	fclose(fp);
	return(true);
}

int GenITTable(char *pName, char *pDefFile)		/* integrated total point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	/*int		data_type2;*/
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead ITTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open IT file %s\n",filenamestr);
 		itdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####IT File %s Define Error! IT Number = %d\n",filenamestr,iTemp);
		itdef.number = 0;
		fclose(fp);
		return(false);
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
		fscanf(fp,"%d %s %s",&iTemp,itdef.name[j],lgname);
		if(strcmp(itdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("IT %d pnt: %s %s\n",j,itdef.name[j],lgname);
#endif
			strcpy(name_buf2,itdef.name[j]);

			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### IT %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
/* #ifdef DEBUG */
	printf("---%s GenITTable: point nNumber = %d\n\n",pName,itdef.number);
/* #endif */
	return(true);
}

int GenSCTable(char *pName, char *pDefFile)		/* single point control command define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
/*	int		data_type2;*/
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1,iRet,numb;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead SCTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open SC file %s\n",filenamestr);
 		scdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SC File %s Define Error! SC Number = %d\n",filenamestr,iTemp);
		scdef.number = 0;
		fclose(fp);
		return(false);
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
		scdef.cmdtype[j] = iTemp;
		scdef.optype[j] = iTemp1;
		if(strcmp(scdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("SC %d pnt: %s %s\n",j,scdef.name[j],lgname);
#endif
			strcpy(name_buf2,scdef.name[j]);

			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### SC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
/* #ifdef DEBUG */
	printf("---%s GenSCTable: point nNumber = %d\n\n",pName,scdef.number);
/* #endif */
	return(true);
}

int GenDCTable(char *pName, char *pDefFile)		/* double point control command define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
/*	int		data_type2;*/
	char		name_buf2[NAME_SIZE],lgname[36];

	int		iTemp,iTemp1,iRet,numb;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead DCTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open DC file %s\n",filenamestr);
 		dcdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####DC File %s Define Error! DC Number = %d\n",filenamestr,iTemp);
		dcdef.number = 0;
		fclose(fp);
		return(false);
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
		dcdef.cmdtype[j] = iTemp;
		dcdef.optype[j] = iTemp1;
		if(j!=numb)
		{
			printf("DC %d pnt number=%d wrong!!!  %s %d %d %s\n",j,numb,dcdef.name[j],iTemp,iTemp1,lgname);
		}
		if(strcmp(dcdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("DC %d pnt: %s %d %d %s\n",j,dcdef.name[j],iTemp,iTemp1,lgname);
#endif
			strcpy(name_buf2,dcdef.name[j]);

			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				sprintf(infobuf,"#### DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				WriteDspInfo(infobuf);
	    			WriteLogFile(infobuf);
			}
			dcdef.ptr[j] = dbentry2.point;
		}
	}
	fclose(fp);
/* #ifdef DEBUG */
	printf("---%s GenDCTable: point nNumber = %d\n\n",pName,dcdef.number);
/* #endif */
	return(true);
}

int GenSETable(char *pName, char *pDefFile)		/* set point control point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[36];

	int	iTemp,iTemp1,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead SETable file %s!!!\n",filenamestr);
#endif
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		printf("####can't open SE file %s\n",filenamestr);
 		sedef.number = 0;
		sleep(3);
/* 		return(false); */
	}
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####SE File %s Define Error! SE Number = %d\n",filenamestr,iTemp);
		sedef.number = 0;
		fclose(fp);
		return(false);
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
		fscanf(fp,"%d %s %d %s",&iTemp,sedef.name[j],&iTemp1,lgname);
		sedef.cmdtype[j] = iTemp1;
		if(j!=iTemp)
		{
			printf("SE %d pnt number=%d wrong!!!  %s %f %f %s\n",j,iTemp,sedef.name[j],lgname);
		}
		if(strcmp(sedef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("SE %d pnt: %s %s\n",j,sedef.name[j],lgname);
#endif
			strcpy(name_buf2,sedef.name[j]);

			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### SE %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				sprintf(infobuf,"#### SE %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				WriteDspInfo(infobuf);
	    			WriteLogFile(infobuf);
				
			}
			sedef.ptr[j] = dbentry2.point;
		}
	}
/* #ifdef DEBUG */
	printf("---%s GenSETable: point nNumber = %d\n\n",pName,sedef.number);
/* #endif */
	fclose(fp);
	return(true);
}

int GenRCTable(char *pName, char *pDefFile)		/* step point control command define file */
{
	FILE			*fp;
	DMS_COMMON		dbentry2;
	/*int			data_type2;*/
	char			name_buf2[NAME_SIZE],lgname[36];

	int		iTemp,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	printf("\n\nRead RCTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		printf("####can't open RC file %s\n",filenamestr);
 		rcdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		printf("\n#####RC File %s Define Error! RC Number = %d\n",filenamestr,iTemp);
		rcdef.number = 0;
		fclose(fp);
		return(false);
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
		fscanf(fp,"%d %s %s",&iTemp,rcdef.name[j],lgname);
		if(strcmp(rcdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			printf("RC %d pnt: %s %s\n",j,rcdef.name[j],lgname);
#endif
			strcpy(name_buf2,rcdef.name[j]);

			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				printf("#### RC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
/* #ifdef DEBUG */
	printf("---%s GenRCTable: point nNumber = %d\n\n",pName,rcdef.number);
/* #endif */
	return(true);
}

void	LanBcCmdError(POINTER ptr,IND_DATA state)
{
	LAN_HEAD	lanhead;
	SW_MSG		bj_buf;
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
	lanhead.length=sizeof(SW_MSG);

#ifdef SUN
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

}

int	IEC870_5_Recv(char *pName,unsigned int recvdatapt,int sockfd)
{
	unsigned int	dwWaitRecvASDU;
	USHORT	infoaddr,datalength,lengthfactor;
	int	infoptr;
	short	ana16data;
	USHORT	ms,year;
	int 	i,datapoint/*,data_type*/;
	char	dbname[36];
	unsigned char	/*stnd,*/type_id,varstru,infonum,qoc,lanpt;
	unsigned short	asduaddr,causetra;

	SW_MSG		sw_buf[70];
	ALL_SW_MSG	all_sw_buf[70];
	unsigned char	me_buf[1440];
	LAN_HEAD	lanhead;
	IND_DATA	sw_status;
	/*ANLG_STATUS	me_status;*/
	float		value;
	int		count;
	int     ana_alm_stat;/**zwj for ANA ALM**/

	DMS_COMMON	dbcommpt;

	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	ANLG_DATA	anadata;
	struct	tm	tmpt;
	time_t		tim;

	char	name_buf[100];


	strcpy(name_buf,pName);
	type_id = recv_data.data[recvdatapt][0];
	varstru = recv_data.data[recvdatapt][1];
#ifdef SUN
	CrossCopy(&causetra, &recv_data.data[recvdatapt][2],2);
	CrossCopy(&asduaddr, &recv_data.data[recvdatapt][4],2);
#else
	memcpy(&causetra, &recv_data.data[recvdatapt][2],2);
	memcpy(&asduaddr, &recv_data.data[recvdatapt][4],2);
#endif
	causetra = causetra & 0xFF;
/*printf("\n============type_id=%d===========\n",type_id);*/
	switch(type_id)
	{
	case M_SP_NA:	/* single point without time tag: 1 */
	case M_DP_NA:	/* double point without time tag: 3 */

		if(varstru & 0x80)				/* SQ=1 */
		{
 			sprintf(infobuf,"\n状态量全送\n");
 			WriteDspInfo(infobuf);
 			WriteLogFile(infobuf);
			infonum = varstru & 0x7F;
			datalength = infonum+9;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
			}
			tim=time(NULL);
			tmpt=*localtime(&tim);

			lanpt = 0;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#endif
			if(type_id ==M_SP_NA)
			{
				infoptr = infoaddr - sp_base_addr;
			}
			else
			{
				infoptr = infoaddr - dp_base_addr;
			}
			for(i=0;i<infonum;i++)
			{
				datapoint = i+9;

				if(type_id ==M_SP_NA)
				{
					if((infoptr < 0) || (infoptr >= spdef.number))
					{
						printf("M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,spdef.number);
						break;
					}
					strcpy(dbname,spdef.name[infoptr]);
				}
				else
				{
					if((infoptr < 0) || (infoptr >= dpdef.number))
					{
						printf("M_DP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dpdef.number);
						break;
					}
					strcpy(dbname,dpdef.name[infoptr]);
				}
				infoptr ++;

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint];
				*(USHORT *)&sw_status = 0;
				if(type_id ==M_SP_NA)
				{
					if(qoc & 1)
						sw_status.rt_state = 1;
					else
						sw_status.rt_state = 0;
				}
				else
				{
					switch(qoc & 0x03)
					{
					case 0:	sw_status.rt_state = 2; /* transit */
						break;
					case 1:	sw_status.rt_state = 0; /* off */
						break;
					case 2:	sw_status.rt_state = 1; /* on */
						break;
					case 3:	sw_status.rt_state = 3; /* error */
						break;
					}
				}

				if(qoc & 0x20)	  			/* SIQ:= IV NT SB BL 0 0 0 SPI */
					sw_status.man_set = 1;	/* manual set */
				else
					sw_status.man_set = 0;
				if(qoc & 0xC0)
					sw_status.chan_fail = 1;	/* channel fault */
				else
					sw_status.chan_fail = 0;

				sw_status.fault_tag = 0;

				if(causetra == SPONT)
				{
					sw_buf[lanpt].type_id =DPS_IND_ALM;
					sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
					sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
					sw_buf[lanpt].data_type=dbcommpt.point.data_type;
					sw_buf[lanpt].number = 1;
					sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

					sw_status.alm_type = IND_ALM_TYPE_CHANGE;
					sw_buf[lanpt].status=sw_status;
					sw_buf[lanpt].ms=0;
					GetLocalHostId(&sw_buf[lanpt].host_id);

					tim=time(NULL);
					tmpt=*localtime(&tim);
					sw_buf[lanpt].sec=tmpt.tm_sec;
					sw_buf[lanpt].min=tmpt.tm_min;
					sw_buf[lanpt].hour=tmpt.tm_hour;
					sw_buf[lanpt].day=tmpt.tm_mday;
					sw_buf[lanpt].month=tmpt.tm_mon+1;
					sw_buf[lanpt].year=tmpt.tm_year+1900;

#ifdef SUN
					Swap2Byte(&sw_buf[lanpt].number);
					Swap2Byte(&sw_buf[lanpt].point_id);
					Swap2Byte(&sw_buf[lanpt].status);
					Swap2Byte(&sw_buf[lanpt].ms);
					Swap2Byte(&sw_buf[lanpt].year);
#endif
					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(SW_MSG);
#ifdef SUN
						Swap2Byte(&lanhead.length);
#endif
						lanhead.dp_type=BJ_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
						lanpt = 0;

					}
				}
				else
				{/* write realtime database */

					all_sw_buf[lanpt].type_id =DPS_STATUS_ALL;
					all_sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
					all_sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
					all_sw_buf[lanpt].data_type=dbcommpt.point.data_type;
					all_sw_buf[lanpt].number = 1;
					all_sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

					sw_status.alm_type = IND_ALM_TYPE_CHANGE;
					all_sw_buf[lanpt].status=sw_status;
					GetLocalHostId(&all_sw_buf[lanpt].host_id);

					tim=time(NULL);
					tmpt=*localtime(&tim);
					all_sw_buf[lanpt].sec=tmpt.tm_sec;
					all_sw_buf[lanpt].min=tmpt.tm_min;
					all_sw_buf[lanpt].hour=tmpt.tm_hour;
					all_sw_buf[lanpt].day=tmpt.tm_mday;
					all_sw_buf[lanpt].month=tmpt.tm_mon+1;
					all_sw_buf[lanpt].year=tmpt.tm_year+1900;

#ifdef SUN
					Swap2Byte(&all_sw_buf[lanpt].number);
					Swap2Byte(&all_sw_buf[lanpt].point_id);
					Swap2Byte(&all_sw_buf[lanpt].status);
					Swap2Byte(&all_sw_buf[lanpt].year);
#endif
					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(ALL_SW_MSG);
#ifdef SUN
						Swap2Byte(&lanhead.length);
#endif
						lanhead.dp_type=ALL_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =all_sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&all_sw_buf,lanhead,(unsigned char)0);
						lanpt = 0;
/*						sleep(1); */
					}
				}
			}
		}/* if(SQ) */
		else		/* SQ=0 */
		{
			infonum = varstru & 0x7F;
			datalength = infonum*4+6;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
			}
			tim=time(NULL);
			tmpt=*localtime(&tim);

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = i*4+6;
#ifdef SUN
				CrossCopy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#endif

				if(type_id ==M_SP_NA)
				{
					infoptr = infoaddr - sp_base_addr;
					if((infoptr < 0) || (infoptr >= spdef.number))
					{
#ifdef DEBUG
						printf("M_SP_NA SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,spdef.number);
#endif				
						break;
					}
					strcpy(dbname,spdef.name[infoptr]);
				}
				else
				{
					infoptr = infoaddr - dp_base_addr;
					if((infoptr < 0) || (infoptr >= dpdef.number))
					{
#ifdef DEBUG
						printf("M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dpdef.number);
#endif				
						break;
					}
					strcpy(dbname,dpdef.name[infoptr]);
				}

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+3];
				*(USHORT *)&sw_status = 0;
				if(type_id ==M_SP_NA)
				{
					if(qoc & 1)
						sw_status.rt_state = 1;
					else
						sw_status.rt_state = 0;
				}
				else
				{
					switch(qoc & 0x03)
					{
					case 0:	sw_status.rt_state = 2; /* transit */
							break;
					case 1:	sw_status.rt_state = 0; /* off */
							break;
					case 2:	sw_status.rt_state = 1; /* on */
							break;
					case 3:	sw_status.rt_state = 3; /* error */
							break;
					}
				}

				if(qoc & 0x20)	  			/* SIQ:= IV NT SB BL 0 0 0 SPI */
					sw_status.man_set = 1;	/* manual set */
				else
					sw_status.man_set = 0;
				if(qoc & 0xC0)
					sw_status.chan_fail = 1;	/* channel fault */
				else
					sw_status.chan_fail = 0;

				sw_status.fault_tag = 0;

				if(causetra == SPONT)
				{
					sw_buf[lanpt].type_id =DPS_IND_ALM;
					sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
					sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
					sw_buf[lanpt].data_type=dbcommpt.point.data_type;
					sw_buf[lanpt].number = 1;
					sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

					sw_status.alm_type = IND_ALM_TYPE_CHANGE;
					sw_buf[lanpt].status=sw_status;
					sw_buf[lanpt].ms=0;
					GetLocalHostId(&sw_buf[lanpt].host_id);

					tim=time(NULL);
					tmpt=*localtime(&tim);
					sw_buf[lanpt].sec=tmpt.tm_sec;
					sw_buf[lanpt].min=tmpt.tm_min;
					sw_buf[lanpt].hour=tmpt.tm_hour;
					sw_buf[lanpt].day=tmpt.tm_mday;
					sw_buf[lanpt].month=tmpt.tm_mon+1;
					sw_buf[lanpt].year=tmpt.tm_year+1900;

#ifdef SUN
					Swap2Byte(&sw_buf[lanpt].number);
					Swap2Byte(&sw_buf[lanpt].point_id);
					Swap2Byte(&sw_buf[lanpt].status);
					Swap2Byte(&sw_buf[lanpt].ms);
					Swap2Byte(&sw_buf[lanpt].year);
#endif
					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(SW_MSG);
#ifdef SUN
						Swap2Byte(&lanhead.length);
#endif
						lanhead.dp_type=BJ_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
						lanpt = 0;
/*						sleep(1); */
					}
				}
				else
				{/* write realtime database */

					all_sw_buf[lanpt].type_id =DPS_STATUS_ALL;
					all_sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
					all_sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
					all_sw_buf[lanpt].data_type=dbcommpt.point.data_type;
					all_sw_buf[lanpt].number = 1;
					all_sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

					sw_status.alm_type = IND_ALM_TYPE_CHANGE;
					all_sw_buf[lanpt].status=sw_status;
					GetLocalHostId(&all_sw_buf[lanpt].host_id);

					tim=time(NULL);
					tmpt=*localtime(&tim);
					all_sw_buf[lanpt].sec=tmpt.tm_sec;
					all_sw_buf[lanpt].min=tmpt.tm_min;
					all_sw_buf[lanpt].hour=tmpt.tm_hour;
					all_sw_buf[lanpt].day=tmpt.tm_mday;
					all_sw_buf[lanpt].month=tmpt.tm_mon+1;
					all_sw_buf[lanpt].year=tmpt.tm_year+1900;

#ifdef SUN
					Swap2Byte(&all_sw_buf[lanpt].number);
					Swap2Byte(&all_sw_buf[lanpt].point_id);
					Swap2Byte(&all_sw_buf[lanpt].status);
					Swap2Byte(&all_sw_buf[lanpt].year);
#endif
					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(ALL_SW_MSG);
#ifdef SUN
						Swap2Byte(&lanhead.length);
#endif
						lanhead.dp_type=ALL_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =all_sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&all_sw_buf,lanhead,(unsigned char)0);
						lanpt = 0;
/*						sleep(1); */
					}
				}
			}
		}/* if(SQ) */
		if( lanpt > 0 )
		{
			if(causetra == SPONT)
			{
				lanhead.length = lanpt*sizeof(SW_MSG);
				lanhead.dp_type=BJ_DB_TIME;
				lanhead.dest_stn =sw_buf[0].stn_id;
#ifdef SUN
				Swap2Byte(&lanhead.length);
#endif
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.packet_num =0;
				lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
			}
			else
			{
				lanhead.length = lanpt*sizeof(ALL_SW_MSG);
				lanhead.dp_type=ALL_DB_TIME;
				lanhead.dest_stn =all_sw_buf[0].stn_id;
#ifdef SUN
				Swap2Byte(&lanhead.length);
#endif
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.packet_num =0;
				lan_out ((char *)&all_sw_buf,lanhead,(unsigned char)0);
			}
		}		
		break;

	case M_SP_TA:	/* single point with time tag {data class 2 } */
	case M_DP_TA:	/* double point with time tag {data class 4 } */

		infonum = varstru & 0x7F;
		datalength = 7*infonum+6;
		if(datalength != recv_data.length[recvdatapt])
		{
			printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
					type_id,infonum,recv_data.length[recvdatapt]);
			break;
		}

		lanpt = 0;
		for(i=0;i<infonum;i++)
		{
			datapoint = 7*i+6;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#endif
			if(type_id ==M_SP_TA)
			{
				infoptr = infoaddr - sp_base_addr;
				if((infoptr < 0) || (infoptr > spdef.number))
				{
#ifdef DEBUG
					printf("M_SP_TA SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,spdef.number);
#endif
					break;
				}
				strcpy(dbname,spdef.name[infoptr]);
			}
			else
			{
				infoptr = infoaddr - dp_base_addr;
				if((infoptr < 0) || (infoptr > dpdef.number))
				{
#ifdef DEBUG
					printf("M_DP_TA SQ=0: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dpdef.number);
#endif
					break;
				}
				strcpy(dbname,dpdef.name[infoptr]);
			}
			if(strcmp(dbname,"NULL")==0)
				continue;
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("\nIEC No this point name=%s in db\n",dbname);
				continue;
			}
			if (!dbcommpt.fixed.iost.scanEnable)
			{
				continue;
			}
			qoc = recv_data.data[recvdatapt][datapoint+3];
			*(USHORT *)&sw_status = 0;
			if(type_id ==M_SP_TA)
			{
				if(qoc & 1)				   /* SIQ:= IV NT SB BL 0 0 0 SPI */
					sw_status.rt_state = 1;
				else
					sw_status.rt_state = 0;
			}
			else
			{
				switch(qoc & 0x03)
				{
				case 0:	sw_status.rt_state = 2; /* transit */
						break;
				case 1:	sw_status.rt_state = 0; /* off */
						break;
				case 2:	sw_status.rt_state = 1; /* on */
						break;
				case 3:	sw_status.rt_state = 3; /* error */
						break;
				}
			}

			if(qoc & 0x20)	  			/* SIQ:= IV NT SB BL 0 0 0 SPI */
				sw_status.man_set = 1;	/* manual set */
			else
				sw_status.man_set = 0;
			if(qoc & 0xC0)
				sw_status.chan_fail = 1;	/* channel fault */
			else
				sw_status.chan_fail = 0;

			sw_status.fault_tag = 0;

			sw_buf[lanpt].type_id =DPS_IND_ALM;
			sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
			sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
			sw_buf[lanpt].data_type=dbcommpt.point.data_type;
			sw_buf[lanpt].number = 1;
			sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

			sw_status.alm_type = IND_ALM_TYPE_CHANGE;
			sw_buf[lanpt].status=sw_status;
			GetLocalHostId(&sw_buf[lanpt].host_id);
#ifdef SUN
			CrossCopy(&ms,&recv_data.data[recvdatapt][datapoint+4],2);
#else
			memcpy(&ms,&recv_data.data[recvdatapt][datapoint+4],2);
#endif
			sw_buf[lanpt].ms = ms%1000;
			sw_buf[lanpt].sec=(char)(ms/1000);
			sw_buf[lanpt].min=recv_data.data[recvdatapt][datapoint+6] & 0x3F;
			sw_buf[lanpt].hour=tmpt.tm_hour;
			sw_buf[lanpt].day=tmpt.tm_mday;
			sw_buf[lanpt].month=tmpt.tm_mon+1;
			sw_buf[lanpt].year=tmpt.tm_year+1900;

#ifdef SUN
			Swap2Byte(&sw_buf[lanpt].number);
			Swap2Byte(&sw_buf[lanpt].point_id);
			Swap2Byte(&sw_buf[lanpt].status);
			Swap2Byte(&sw_buf[lanpt].ms);
			Swap2Byte(&sw_buf[lanpt].year);
#endif
			lanpt ++;
			if( lanpt>=70 )
			{
				lanhead.length = lanpt*sizeof(SW_MSG);
#ifdef SUN
				Swap2Byte(&lanhead.length);
#endif
				lanhead.dp_type=BJ_DB_TIME;
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.dest_stn =sw_buf[0].stn_id;
				lanhead.packet_num =0;
				lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
				lanpt = 0;
/*				sleep(1); */
			}
		}
		if( lanpt>0 )
		{
			lanhead.length = lanpt*sizeof(SW_MSG);
#ifdef SUN
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dp_type=BJ_DB_TIME;
			GetLocalHostId(&lanhead.src_id);
			lanhead.dest_id =0;
			lanhead.dest_stn =sw_buf[0].stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
		}		
		break;

	case M_SP_TB:	/* single point with time tag {data class 30 } */
	case M_DP_TB:	/* double point with time tag {data class 31 } */

		infonum = varstru & 0x7F;
		datalength = 11*infonum+6;
		if(datalength != recv_data.length[recvdatapt])
		{
			printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
					type_id,infonum,recv_data.length[recvdatapt]);
			break;
		}
		sprintf(infobuf,"\n状态量变化送\n");
		WriteDspInfo(infobuf);
		WriteLogFile(infobuf);
		lanpt = 0;
		for(i=0;i<infonum;i++)
		{
			datapoint = 11*i+6;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#endif
			if(type_id ==M_SP_TB)
			{
				infoptr = infoaddr - sp_base_addr;
				if((infoptr < 0) || (infoptr > spdef.number))
				{
#ifdef DEBUG
					printf("M_SP_TB SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,spdef.number);
#endif
					break;
				}
				strcpy(dbname,spdef.name[infoptr]);
			}
			else
			{
				infoptr = infoaddr - dp_base_addr;
				if((infoptr < 0) || (infoptr > dpdef.number))
				{
#ifdef DEBUG
					printf("M_DP_TB SQ=0: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,dpdef.number);
#endif
					break;
				}
				strcpy(dbname,dpdef.name[infoptr]);
			}
			if(strcmp(dbname,"NULL")==0)
				continue;
			printf("\n^^dbname=%s^^\n",dbname);
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("\nIEC No this point name=%s in db\n",dbname);
				continue;
			}
			if (!dbcommpt.fixed.iost.scanEnable)
			{
				continue;
			}
			qoc = recv_data.data[recvdatapt][datapoint+3];
			*(USHORT *)&sw_status = 0;
			if(type_id ==M_SP_TB)
			{
				if(qoc & 1)				   /* SIQ:= IV NT SB BL 0 0 0 SPI */
				sw_status.rt_state=1;
				else
				sw_status.rt_state=0;
				
			}
			else
			{
				switch(qoc & 0x03)
				{
				case 0:	sw_status.rt_state = 2; /* transit */
						break;
				case 1:	sw_status.rt_state = 0; /* off */
						break;
				case 2:	sw_status.rt_state = 1; /* on */
						break;
				case 3:	sw_status.rt_state = 3; /* error */
						break;
				}
				
			}
			sprintf(infobuf,"\n%s status:%d\n",dpdef.name[infoptr],sw_status.rt_state);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			if(qoc & 0x20)	  			/* SIQ:= IV NT SB BL 0 0 0 SPI */
			{	sw_status.man_set = 1;	/* manual set */
				printf("\nmanual set\n");
			}
			else
				sw_status.man_set = 0;

			if(qoc & 0xC0)
			{	sw_status.chan_fail = 1;	/* channel fault */
				printf("\nchannel fault\n");
			}
			else
				sw_status.chan_fail = 0;
		

			sw_status.fault_tag = 0;

			sw_buf[lanpt].type_id =DPS_IND_ALM;
			sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
			sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
			sw_buf[lanpt].data_type=dbcommpt.point.data_type;
			sw_buf[lanpt].number = 1;
			sw_buf[lanpt].point_id =dbcommpt.point.pt_id;
			
			sw_status.alm_type = IND_ALM_TYPE_CHANGE;
			sw_buf[lanpt].status=sw_status;
			GetLocalHostId(&sw_buf[lanpt].host_id);
#ifdef SUN
			CrossCopy(&ms,&recv_data.data[recvdatapt][datapoint+4],2);
#else
			memcpy(&ms,&recv_data.data[recvdatapt][datapoint+4],2);
#endif
			sw_buf[lanpt].ms = ms%1000;
			sw_buf[lanpt].sec=(char)(ms/1000);
			sw_buf[lanpt].min=recv_data.data[recvdatapt][datapoint+6] & 0x3F;
			sw_buf[lanpt].hour=recv_data.data[recvdatapt][datapoint+7] & 0x1F;
			sw_buf[lanpt].day=recv_data.data[recvdatapt][datapoint+8] & 0x1F;
			sw_buf[lanpt].month=recv_data.data[recvdatapt][datapoint+9] & 0x0F;
			sw_buf[lanpt].year=(recv_data.data[recvdatapt][datapoint+10] & 0x7F)+2000;

#ifdef SUN
			Swap2Byte(&sw_buf[lanpt].number);
			Swap2Byte(&sw_buf[lanpt].point_id);
			Swap2Byte(&sw_buf[lanpt].status);
			Swap2Byte(&sw_buf[lanpt].ms);
			Swap2Byte(&sw_buf[lanpt].year);
#endif
		
			lanpt ++;
			if( lanpt>=70 )
			{
				lanhead.length = lanpt*sizeof(SW_MSG);
#ifdef SUN
				Swap2Byte(&lanhead.length);
#endif
				lanhead.dp_type=BJ_DB_TIME;
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.dest_stn =sw_buf[0].stn_id;
				lanhead.packet_num =0;
				lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
				lanpt = 0;
/*				sleep(1); */
			}
		}
		if( lanpt>0 )
		{
			lanhead.length = lanpt*sizeof(SW_MSG);
#ifdef SUN
			Swap2Byte(&lanhead.length);
#endif
			lanhead.dp_type=BJ_DB_TIME;
			GetLocalHostId(&lanhead.src_id);
			lanhead.dest_id =0;
			lanhead.dest_stn =sw_buf[0].stn_id;
			lanhead.packet_num =0;

			lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
		}	
		break;

	case M_ME_NA:	/* measure value without time tag:9 */
	case M_ME_TA:	/* measure value with time tag:10 */
	case M_ME_TD:	/* measure value with time tag CP56Time2a normal:34 */
		infonum = varstru & 0x7F;			/* number of information object */
		if(varstru & 0x80)	/* SQ=1 */
		{
			if(type_id == M_ME_NA)
			{
				lengthfactor = 3;
			}
			else if(type_id == M_ME_TA)
				lengthfactor = 6;
			else
				lengthfactor = 10;
			datalength = lengthfactor*infonum+9;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif

			infoptr = infoaddr - me_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr < 0) || (infoptr >= medef.number))
				{
#ifdef DEBUG
					printf("M_ME_NA SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,medef.number);
#endif			
					break;
				}
				strcpy(dbname,medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+2];

#ifdef SUN
				CrossCopy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);
#endif
				if(MeType == 0)
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/32767.0
						 + medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/65535.0
						 + (medef.hirange[infoptr]+medef.lorange[infoptr])/2.0);
				}

				infoptr ++;
				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

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
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				if(type_id ==M_ME_NA)
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
					CrossCopy(&ms,&recv_data.data[recvdatapt][datapoint+3],2);
#else
					memcpy(&ms,&recv_data.data[recvdatapt][datapoint+3],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+5] & 0x3F;	/* minute */
					if(type_id ==M_ME_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TD */
					{
						me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+6] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+7] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+8] & 0x0F;	/* month */
						year=(recv_data.data[recvdatapt][datapoint+9] & 0x7F)+2000;		/* year */
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
			if(type_id == M_ME_NA)
			{
				lengthfactor = 6;
			}
			else if(type_id == M_ME_TA)
				lengthfactor = 9;
			else
				lengthfactor = 13;
			datalength = lengthfactor*infonum+6;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
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
				infoptr = infoaddr - me_base_addr;
				if((infoptr < 0) || (infoptr > medef.number))
				{
#ifdef DEBUG
					printf("M_ME_NA SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,medef.number);
#endif
					break;
				}
				strcpy(dbname,medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+5];

#ifdef SUN
				CrossCopy(&ana16data,&recv_data.data[recvdatapt][datapoint+3],2);
#else
				memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint+3],2);
#endif

				if(MeType == 0)
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/32767.0
						 + medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/65535.0
						 + (medef.hirange[infoptr]+medef.lorange[infoptr])/2.0);
				}

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

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
#ifdef DEBUG
		printf("\n%s::Receive=%f \nHI_ALARM=%f HI_WARN=%f HI_RECOVER=%f \nLO_ALARM=%f LO_WARN=%f LO_RECOVER=%f\n",dbname,value,
					dbcommpt.var.anlg.lmt_para.HI_ALARM,dbcommpt.var.anlg.lmt_para.HI_WARN,dbcommpt.var.anlg.lmt_para.HI_RECOVER,
					dbcommpt.var.anlg.lmt_para.LO_ALARM,dbcommpt.var.anlg.lmt_para.LO_WARN,dbcommpt.var.anlg.lmt_para.LO_RECOVER);
#endif


							if(value>=dbcommpt.var.anlg.lmt_para.HI_ALARM)
							{
								ana_alm_stat=ANA_ALM_HI_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>=dbcommpt.var.anlg.lmt_para.HI_WARN))
							{
								ana_alm_stat=ANA_ALM_HI_WARN;
							}
							if((value>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<=dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_LO_WARN;
							}
							if(value<=dbcommpt.var.anlg.lmt_para.LO_ALARM)
							{
								ana_alm_stat=ANA_ALM_LO_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_NORMAL;
							}

							if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_NORMAL)  /*原值正常*/
							{
								if(ana_alm_stat==ANA_ALM_NORMAL)                        /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_NORMAL;            /*正常数据广播*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}

								else if(ana_alm_stat==ANA_ALM_HI_ALARM)                   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;            /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;			  /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)					  /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;			  /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_ALARM)    /*原值越上上限*/
							{
								if(value>=(dbcommpt.var.anlg.lmt_para.HI_ALARM-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值未复归上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*无报警，状态保持越上上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_WARN)    /*原值越上限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER)) /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*无报警，状态保持越上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_WARN)    /*原值越下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_ALARM)                   /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*无报警，状态保持越下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_ALARM)    /*原值越下下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;          /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value<=(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER))                   /*新值未复归下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*无报警，状态保持越下下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
							}
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
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				if(type_id ==M_ME_NA)
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
					CrossCopy(&ms,&recv_data.data[recvdatapt][datapoint+6],2);
#else
					memcpy(&ms,&recv_data.data[recvdatapt][datapoint+6],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=recv_data.data[recvdatapt][datapoint+8] & 0x3F;	/* minute */
					if(type_id ==M_ME_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TD */
					{
						me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+10] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+11] & 0x0F;	/* month */
						year=(recv_data.data[recvdatapt][datapoint+12] & 0x7F)+2000;		/* year */
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
#ifdef SUN
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
		break;

	case M_ME_NC:	/* measure value without time tag:13 */
	case M_ME_TC:	/* measure value with time tag:14 */
	case M_ME_TF:	/* 36:measure value with time CP56Time2a,float value*/

		infonum = varstru & 0x7F;			/* number of information object */
		if(varstru & 0x80)	/* SQ=1 */
		{
 			sprintf(infobuf,"\n模拟量全送\n");
 			WriteDspInfo(infobuf);
 			WriteLogFile(infobuf);
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
				break;
			}

			lanpt = 0;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			infoptr = infoaddr - me_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr < 0) || (infoptr >= medef.number))
				{
#ifdef DEBUG
					printf("M_ME_NC SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,medef.number);
#endif			
					break;
				}
				strcpy(dbname,medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					infoptr ++;  //GCL 20110108
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
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

				if(dbcommpt.point.data_type == IMP_TYPE)
				{
					anadata.data = (GLONG)value;
						me_buf[lanpt*22] =DPS_IMPULSE;	/* type_id */
				}
				else
				{
					anadata.data = (GLONG)(value*1000);
					me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */


							if(value>dbcommpt.var.anlg.lmt_para.HI_ALARM)
							{
								ana_alm_stat=ANA_ALM_HI_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>dbcommpt.var.anlg.lmt_para.HI_WARN))
							{
								ana_alm_stat=ANA_ALM_HI_WARN;
							}
							if((value>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_LO_WARN;
							}
							if(value<=dbcommpt.var.anlg.lmt_para.LO_ALARM)
							{
								ana_alm_stat=ANA_ALM_LO_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_NORMAL;
							}

							if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_NORMAL)  /*原值正常*/
							{
								if(ana_alm_stat==ANA_ALM_NORMAL)                        /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_NORMAL;            /*正常数据广播*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}

								else if(ana_alm_stat==ANA_ALM_HI_ALARM)                   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;            /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;			  /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)					  /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;			  /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_ALARM)    /*原值越上上限*/
							{
								if(value>=(dbcommpt.var.anlg.lmt_para.HI_ALARM-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值未复归上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*无报警，状态保持越上上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_WARN)    /*原值越上限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER)) /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*无报警，状态保持越上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_WARN)    /*原值越下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_ALARM)                   /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*无报警，状态保持越下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_ALARM)    /*原值越下下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;          /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value<=(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER))                   /*新值未复归下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*无报警，状态保持越下下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
							}	
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
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				
				tim=time(NULL);
				tmpt=*localtime(&tim);
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
						year=(recv_data.data[recvdatapt][datapoint+11] & 0x7F)+2000;		/* year */
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
					lanhead.dp_type=BJ_DB_TIME;

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
				tim=time(NULL);
				tmpt=*localtime(&tim);
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
				break;
			}
			sprintf(infobuf,"\n模拟量变化送\n");
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;
#ifdef SUN
				CrossCopy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
				infoptr = infoaddr - me_base_addr;
				if((infoptr < 0) || (infoptr > medef.number))
				{
#ifdef DEBUG
					printf("M_ME_NC SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,medef.number);
#endif
					break;
				}
				strcpy(dbname,medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
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
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

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
#ifdef DEBUG
		printf("\n%s::Receive=%f \nHI_ALARM=%f HI_WARN=%f HI_RECOVER=%f \nLO_ALARM=%f LO_WARN=%f LO_RECOVER=%f\n",dbname,value,
					dbcommpt.var.anlg.lmt_para.HI_ALARM,dbcommpt.var.anlg.lmt_para.HI_WARN,dbcommpt.var.anlg.lmt_para.HI_RECOVER,
					dbcommpt.var.anlg.lmt_para.LO_ALARM,dbcommpt.var.anlg.lmt_para.LO_WARN,dbcommpt.var.anlg.lmt_para.LO_RECOVER);
#endif
		

							if(value>=dbcommpt.var.anlg.lmt_para.HI_ALARM)
							{
								ana_alm_stat=ANA_ALM_HI_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>=dbcommpt.var.anlg.lmt_para.HI_WARN))
							{
								ana_alm_stat=ANA_ALM_HI_WARN;
							}
							if((value>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<=dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_LO_WARN;
							}
							if(value<=dbcommpt.var.anlg.lmt_para.LO_ALARM)
							{
								ana_alm_stat=ANA_ALM_LO_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_NORMAL;
							}

							if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_NORMAL)  /*原值正常*/
							{
								if(ana_alm_stat==ANA_ALM_NORMAL)                        /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_NORMAL;            /*正常数据广播*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}

								else if(ana_alm_stat==ANA_ALM_HI_ALARM)                   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;            /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;			  /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)					  /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;			  /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_ALARM)    /*原值越上上限*/
							{
								if(value>=(dbcommpt.var.anlg.lmt_para.HI_ALARM-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值未复归上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*无报警，状态保持越上上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_WARN)    /*原值越上限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER)) /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*无报警，状态保持越上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_WARN)    /*原值越下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_ALARM)                   /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*无报警，状态保持越下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_ALARM)    /*原值越下下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;          /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value<=(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER))                   /*新值未复归下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*无报警，状态保持越下下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
							}
				}
				sprintf(infobuf,"\n%s value:%f\n",medef.name[infoptr],value);
				WriteDspInfo(infobuf);
				WriteLogFile(infobuf);
				me_buf[lanpt*22+1] =dbcommpt.point.stn_id;
				me_buf[lanpt*22+2] =dbcommpt.point.dev_id;
				me_buf[lanpt*22+3] =dbcommpt.point.data_type;
				me_buf[lanpt*22+4] = 1;		/* number low byte */
				me_buf[lanpt*22+5] = 0;		/* number high byte */
#ifdef SUN
				CrossCopy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				if(dbcommpt.point.data_type == IMP_TYPE)
					*(USHORT *)&anadata.status = 0;
				CrossCopy(&me_buf[lanpt*22+8], &anadata.status,2);
				CrossCopy(&me_buf[lanpt*22+10], &anadata.data,4);
#else
				memcpy(&me_buf[lanpt*22+6], &dbcommpt.point.pt_id,2);
				memcpy(&me_buf[lanpt*22+8], &anadata.status,2);
				memcpy(&me_buf[lanpt*22+10], &anadata.data,4);
#endif
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				tim=time(NULL);
				tmpt=*localtime(&tim);
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
						year=(recv_data.data[recvdatapt][datapoint+14] & 0x7F)+2000;		/* year */
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
#ifdef SUN
			Swap2Byte(&lanhead.length);
#endif

			lanhead.dp_type=BJ_DB_TIME;

			GetLocalHostId(&lanhead.src_id);
			lanhead.dest_id =0;
			lanhead.dest_stn =me_buf[1];
			lanhead.packet_num =0;
			lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
		}		
		break;

	case M_ME_ND:	/* measure value without quality:21 */

		infonum = varstru & 0x7F;			/* number of information object */

		if(varstru & 0x80)	/* SQ=1 */
		{
			lengthfactor = 2;
			datalength = lengthfactor*infonum+9;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
			}
			lanpt = 0;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			infoptr = infoaddr - me_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;
				if((infoptr < 0) || (infoptr >= medef.number))
				{
#ifdef DEBUG
					printf("M_ME_ND SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,medef.number);
#endif			
					break;
				}

				strcpy(dbname,medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
#ifdef SUN
				CrossCopy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint],2);
#endif
				if(MeType == 0)
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/32767.0
						 + medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/65535.0
						 + (medef.hirange[infoptr]+medef.lorange[infoptr])/2.0);
				}

				infoptr ++;

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

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
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
				me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
				me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
				me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
				me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
				year=tmpt.tm_year+1900;	/* year */
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
		else	/* SQ=0 */
		{
			lengthfactor = 5;
			datalength = lengthfactor*infonum+6;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
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
				infoptr = infoaddr - me_base_addr;
#ifdef DEBUG
				printf("M_ME_ND SQ=0: infoaddr=%x  infoptr=%d infonum=%d\n",infoaddr,infoptr,infonum);
#endif
				if((infoptr < 0) || (infoptr > medef.number))
				{
#ifdef DEBUG
					printf("M_ME_ND SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,medef.number);
#endif			
					break;
				}

				strcpy(dbname,medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
#ifdef SUN
				CrossCopy(&ana16data,&recv_data.data[recvdatapt][datapoint+3],2);
#else
				memcpy(&ana16data,&recv_data.data[recvdatapt][datapoint+3],2);
#endif
				if(MeType == 0)
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/32767.0
						 + medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(medef.hirange[infoptr]-medef.lorange[infoptr]))/65535.0
						 + (medef.hirange[infoptr]+medef.lorange[infoptr])/2.0);
				}

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

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
#ifdef DEBUG
		printf("\n%s::Receive=%f \nHI_ALARM=%f HI_WARN=%f HI_RECOVER=%f \nLO_ALARM=%f LO_WARN=%f LO_RECOVER=%f\n",dbname,value,
					dbcommpt.var.anlg.lmt_para.HI_ALARM,dbcommpt.var.anlg.lmt_para.HI_WARN,dbcommpt.var.anlg.lmt_para.HI_RECOVER,
					dbcommpt.var.anlg.lmt_para.LO_ALARM,dbcommpt.var.anlg.lmt_para.LO_WARN,dbcommpt.var.anlg.lmt_para.LO_RECOVER);
#endif

							if(value>=dbcommpt.var.anlg.lmt_para.HI_ALARM)
							{
								ana_alm_stat=ANA_ALM_HI_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>=dbcommpt.var.anlg.lmt_para.HI_WARN))
							{
								ana_alm_stat=ANA_ALM_HI_WARN;
							}
							if((value>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<=dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_LO_WARN;
							}
							if(value<=dbcommpt.var.anlg.lmt_para.LO_ALARM)
							{
								ana_alm_stat=ANA_ALM_LO_ALARM;
							}
							if((value<dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_WARN))
							{
								ana_alm_stat=ANA_ALM_NORMAL;
							}

							if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_NORMAL)  /*原值正常*/
							{
								if(ana_alm_stat==ANA_ALM_NORMAL)                        /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_NORMAL;            /*正常数据广播*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}

								else if(ana_alm_stat==ANA_ALM_HI_ALARM)                        /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;            /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;			  /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)					  /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;			  /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_ALARM)    /*原值越上上限*/
							{
								if(value>=(dbcommpt.var.anlg.lmt_para.HI_ALARM-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值未复归上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*无报警，状态保持越上上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER))   /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_HI_WARN)    /*原值越上限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>=(dbcommpt.var.anlg.lmt_para.HI_WARN-dbcommpt.var.anlg.lmt_para.HI_RECOVER)) /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;          /*无报警，状态保持越上限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_WARN) /*新值正常*/
								{	anadata.status.lmt_state=ANA_ALM_HI_RECOVER;          /*越上限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_WARN)					  /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_WARN)    /*原值越下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>dbcommpt.var.anlg.lmt_para.LO_ALARM)                   /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;             /*无报警，状态保持越下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
								else if(ana_alm_stat==ANA_ALM_LO_ALARM)                   /*新值越下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*越下下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
							}
							else if(dbcommpt.var.anlg.value.status.lmt_state==ANA_ALM_LO_ALARM)    /*原值越下下限*/
							{
								if(ana_alm_stat==ANA_ALM_HI_ALARM)   /*新值越上上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_ALARM;          /*越上上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(ana_alm_stat==ANA_ALM_HI_WARN)                    /*新值越上限*/
								{	anadata.status.lmt_state=ANA_ALM_HI_WARN;			  /*越上限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_WARN+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值正常且越下复限死区*/
								{	anadata.status.lmt_state=ANA_ALM_LO_RECOVER;          /*越下限复归报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value>(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER)) /*新值越下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_WARN;          /*越下限报警*/
									me_buf[lanpt*22] =DPS_ANA_ALM;
									anadata.status.alm_type=ANA_ALM_TYPE_LMT;
								}
								else if(value<=(dbcommpt.var.anlg.lmt_para.LO_ALARM+dbcommpt.var.anlg.lmt_para.LO_RECOVER))                   /*新值未复归下下限*/
								{	anadata.status.lmt_state=ANA_ALM_LO_ALARM;            /*无报警，状态保持越下下限*/
									me_buf[lanpt*22] =DPS_ANALOG;
									anadata.status.alm_type=ANA_ALM_TYPE_NORM;
								}
							}
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
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
				me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
				me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
				me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
				me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
				year=tmpt.tm_year+1900;	/* year */
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
#ifdef SUN
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
		break;

	case M_ST_NA:	/* step point without time tag:5 */
	case M_ST_TA:	/* step point with time tag:6 */
	case M_ST_TB:	/* step point with time tag CP56Time2a:32 */

		infonum = varstru & 0x7F;			/* number of information object */
		if(type_id == M_ST_NA)
		{
			tim=time(NULL);
			tmpt=*localtime(&tim);

			lengthfactor = 5;
		}
		else if(type_id == M_ST_TA)
			lengthfactor = 8;
		else
			lengthfactor = 12;
		datalength = lengthfactor*infonum+6;
		if(datalength != recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
		{
			printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
					type_id,infonum,recv_data.length[recvdatapt]);
			break;
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
			infoptr = infoaddr - st_base_addr;
			if((infoptr < 0) || (infoptr > stdef.number))
			{
#ifdef DEBUG
				printf("M_ST_NA SQ=0: infoaddr=%x  infoptr=%d > stnum=%d\n",infoaddr,infoptr,stdef.number);
#endif
				break;
			}
			strcpy(dbname,stdef.name[infoptr]);

			if(strcmp(dbname,"NULL")==0)
				continue;
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				printf("\nIEC No this point name=%s in db\n",dbname);
				continue;
			}
			if (!dbcommpt.fixed.iost.scanEnable)
			{
				continue;
			}
			qoc = recv_data.data[recvdatapt][datapoint+4];

			anadata.data = (recv_data.data[recvdatapt][datapoint+3] & 0x3F)*1000;

			if(recv_data.data[recvdatapt][datapoint+3] & 0x40)	/* VTI = T VALUE:=I7[1..7]<-64..63> */
				anadata.data = -anadata.data;	/* negative */

			if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
				anadata.status.ctrl_block = 1;	/* manual set */
			else
				anadata.status.ctrl_block = 0;
			if(qoc & 0xC0)
				anadata.status.chan_fail = 1;	/* channel fault */
			else
				anadata.status.chan_fail = 0;

			if(causetra == SPONT)
				me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */
			else
				me_buf[lanpt*22] =DPS_ANA_ALL;	/* type_id */

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
			GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

			if(type_id ==M_ST_NA)
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
				if(type_id ==M_ST_TA)
				{
					me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
					me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
					me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
					year=tmpt.tm_year+1900;		/* year */
				}
				else		/* M_ST_TB */
				{
					me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
					me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
					me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
					year=(recv_data.data[recvdatapt][datapoint+11] & 0x7F)+2000;		/* year */
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
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.dest_stn =me_buf[1];
				lanhead.packet_num =0;
				lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
				lanpt = 0;
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
			GetLocalHostId(&lanhead.src_id);
			lanhead.dest_id =0;
			lanhead.dest_stn =me_buf[1];
			lanhead.packet_num =0;
			lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
		}		
		break;

	case M_IT_NA:	/* integrated totals without time tag:15 */
	case M_IT_TA:	/* integrated totals with time tag:16 */
	case M_IT_TB:	/* integrated totals with time tag CP56Time2a:37 */

		infonum = varstru & 0x7F;			/* number of information object */
		if(varstru & 0x80)	/* SQ=1 */
		{
			if(type_id == M_IT_NA)
			{
				lengthfactor = 5;
			}
			else if(type_id == M_IT_TA)
				lengthfactor = 8;
			else
				lengthfactor = 12;
			datalength = lengthfactor*infonum+9;
			if(datalength != recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
#ifdef SUN
			CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			infoptr = infoaddr - it_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr < 0) || (infoptr >= itdef.number))
				{
#ifdef DEBUG
					printf("M_IT_NA SQ=1: infoaddr=%x  infoptr=%d > itnum=%d\n",infoaddr,infoptr,itdef.number);
#endif			
					break;
				}
				strcpy(dbname,medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+4];

#ifdef SUN
				CrossCopy(&count,&recv_data.data[recvdatapt][datapoint],4);
#else
				memcpy(&count,&recv_data.data[recvdatapt][datapoint],4);
#endif
				infoptr ++;

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

				if(dbcommpt.point.data_type == IMP_TYPE)
				{
					anadata.data = (GLONG)count & 0x7FFF;
					if(causetra == SPONT)
						me_buf[lanpt*22] =DPS_IMPULSE;	/* type_id */
					else
						me_buf[lanpt*22] =DPS_IMP_ALL;	/* type_id */
				}
				else
				{
					anadata.data = (GLONG)((count & 0x7FFF)*1000);
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
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				if(type_id ==M_IT_NA)
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
					if(type_id ==M_IT_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_IT_TB */
					{
						me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
						year=(recv_data.data[recvdatapt][datapoint+11] & 0x7F)+2000;		/* year */
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
			if(type_id == M_IT_NA)
			{
				lengthfactor = 8;
			}
			else if(type_id == M_IT_TA)
				lengthfactor = 11;
			else
				lengthfactor = 15;
			datalength = lengthfactor*infonum+6;
			if(datalength != recv_data.length[recvdatapt])
			{
				printf("received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,recv_data.length[recvdatapt]);
				break;
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
				infoptr = infoaddr - it_base_addr;
				if((infoptr < 0) || (infoptr > itdef.number))
				{
#ifdef DEBUG
					printf("M_IT_NA SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,itdef.number);
#endif
					break;
				}
				strcpy(dbname,itdef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					printf("\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				if (!dbcommpt.fixed.iost.scanEnable)
				{
					continue;
				}
				qoc = recv_data.data[recvdatapt][datapoint+7];
#ifdef SUN
				CrossCopy(&count,&recv_data.data[recvdatapt][datapoint+3],4);
#else
				memcpy(&count,&recv_data.data[recvdatapt][datapoint+3],4);
#endif
				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				else
					anadata.status.ctrl_block = 0;
				if(qoc & 0xC0)
					anadata.status.chan_fail = 1;	/* channel fault */
				else
					anadata.status.chan_fail = 0;

				if(dbcommpt.point.data_type == IMP_TYPE)
				{
					anadata.data = (GLONG)(count & 0x7FFF);
					if(causetra == SPONT)
						me_buf[lanpt*22] =DPS_IMPULSE;	/* type_id */
					else
						me_buf[lanpt*22] =DPS_IMP_ALL;	/* type_id */
				}
				else
				{
					anadata.data = (GLONG)((count & 0x7FFF)*1000);
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
				GetLocalHostId(&me_buf[lanpt*22+14]);	/* host_id */

				if(type_id ==M_IT_NA)
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
					if(type_id ==M_IT_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_IT_TB */
					{
						me_buf[lanpt*22+17]=recv_data.data[recvdatapt][datapoint+11] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=recv_data.data[recvdatapt][datapoint+12] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=recv_data.data[recvdatapt][datapoint+13] & 0x0F;	/* month */
						year=(recv_data.data[recvdatapt][datapoint+14] & 0x7F)+2000;		/* year */
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
#ifdef SUN
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
		break;

	case C_SC_NA:	/* confirm of single point control command */
	case C_DC_NA:	/* confirm of double point control command */
	case C_RC_NA:	/* confirm of regulating step command */
			
#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
		qoc = recv_data.data[recvdatapt][9];

		if(10 != recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
		{
			printf("%d C_DC_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			cmd_ack = -1;		/* negative confirm */
			comdflag = 0;
			cmdbuf.ack_flag = 0;
			cmdbuf.selected = 0;
			cmdbuf.term_flag = 0;
			cmdbuf.sel_tim = 0;
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
				comdflag = 0;
/* 				cmdbuf.infoaddr = pntnum+dc_cmd_base_addr; */
				cmdbuf.ack_flag = 0;
				cmdbuf.selected = 0;
				cmdbuf.term_flag = 0;
				cmdbuf.sel_tim = 0;
#ifdef DEBUG
				printf("IEC DC/RC %u command is NACK INF=%d com=%x\n",type_id,infoaddr,qoc);
#endif
				/*BdSms(COM_CTRL_FAIL);*/
				strcat(name_buf,COM_CTRL_FAIL);
				BroadCastSms(name_buf,true);
				break;
			}
			
			if(qoc & 0x80)
			{
#ifdef DEBUG
				printf("IEC SC/DC/RC %u select command ACK INF=%d com=%x\n",type_id,infoaddr,qoc);
#endif
				cmdbuf.ack_flag = 0;
				cmd_ack = 2;		/* select command ACK */
				comdflag = 2;
				if((infoaddr == cmdbuf.infoaddr) && (cmdbuf.selected == 1))
				{
					tim=time(NULL);
					if((tim - cmdbuf.sel_tim)<cmd_period)
					{
						if((Cmd_Type == 0)&&(cmdbuf.tpyeId == C_SC_NA))
						{
							if(!SinglePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sockfd))
								return(FALSE);
						}
						else if((Cmd_Type == 1)&&(cmdbuf.tpyeId == C_DC_NA))
						{
							if(!DoublePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sockfd))
								return(FALSE);
						}
						cmdbuf.ack_flag = 1;
						comdflag = 2;
					}
					else
					{
						printf("IEC SC/DC/RC %u select command ACK time out INF=%d com=%x \n",type_id,infoaddr,qoc);
						strcat(name_buf,COM_CTRL_TIMEOUT);
						BroadCastSms(name_buf,true);
						cmd_ack = -1;		/* negative confirm */
						comdflag = 0;
						cmdbuf.ack_flag = 0;
						cmdbuf.sel_tim = 0;
					}
				}
				else
				{
					comdflag = 0;
					cmd_ack = -1;
					cmdbuf.ack_flag = 0;
					cmdbuf.sel_tim = 0;
					printf("IEC SC/DC/RC %u select command ACK INF=%d or select flag wrong! com=%x \n",type_id,infoaddr,qoc);
					strcat(name_buf,COM_CTRL_FAIL);
					BroadCastSms(name_buf,true);
				}
				cmdbuf.selected = 0;
				cmdbuf.term_flag = 0;
			}
			else
			{
				cmd_ack = 1;		/* execute command ACK */
				cmdbuf.ack_flag = 0;
				cmdbuf.selected = 0;
				cmdbuf.term_flag = 1;
				cmdbuf.sel_tim = 0;
#ifdef DEBUG
				printf("IEC SC/DC/RC %u execute command ACK INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
			}
		}
		else if((causetra & 0x3F) == ACTTERM)
		{
			cmd_ack = 0;		/* execute command Term */
			cmdbuf.term_flag = 0;
			comdflag = 0;
#ifdef DEBUG
			printf("IEC SC/DC/RC %u execute command terminal INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
			strcat(name_buf,COM_CTRL_SUCCESS);
			BdSms(name_buf);
		}
		else
		{
			cmd_ack = 0;
#ifdef DEBUG
			printf("IEC SC/DC/RC %u  INF=%d com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,qoc,causetra);
#endif
			strcat(name_buf,COM_CTRL_FAIL);
			BroadCastSms(name_buf,true);
		}
		break;

	case C_SE_NA:	/* confirm of set-point command */
			
#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
		CrossCopy(&ana16data, &recv_data.data[recvdatapt][9],2);
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&ana16data, &recv_data.data[recvdatapt][9],2);
#endif
		qoc = recv_data.data[recvdatapt][11];

		if(12 != recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
		{
			printf("%d C_SE_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			cmd_ack = -1;		/* negative confirm */
			comdflag = 0;
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
				comdflag = 0;
#ifdef DEBUG
				printf("set-point %u NACK INF=%d stval=%x cause of transmission=%2x received!\n",type_id,infoaddr,ana16data,causetra);
#endif
				strcat(name_buf,COM_CTRL_FAIL);
				BroadCastSms(name_buf,true);
				break;
			}
			if(qoc & 0x80)
			{
				cmd_ack = 2;		/* select command ACK */
				comdflag = 2;
#ifdef DEBUG
				printf("IEC SE %u select command ACK INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
			}
			else
			{
				cmd_ack = 1;		/* execute command ACK */
#ifdef DEBUG
				printf("IEC SE %u execute command ACK INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
				strcat(name_buf,COM_CTRL_SUCCESS);
				BdSms(name_buf);
			}
		}
		else if((causetra & 0x3F) == ACTTERM)
		{
			cmd_ack = 0;		/* execute command Term */
			comdflag = 0;
#ifdef DEBUG
			printf("IEC SE %u execute command terminal INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
			
		}
		else
		{
			cmd_ack = 0;		/* execute command confirm */
#ifdef DEBUG
			printf("set-point %u INF=%d com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,qoc,causetra);
#endif
			strcat(name_buf,COM_CTRL_FAIL);
			BroadCastSms(name_buf,true);
		}
		break;

	case C_SE_NC:	/* confirm of set-point command */
			
#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
		CrossCopy(&value, &recv_data.data[recvdatapt][9],4);
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&value, &recv_data.data[recvdatapt][9],4);
#endif
/* 		value = (float)*(&recv_data.data[recvdatapt][9]); */
		qoc = recv_data.data[recvdatapt][13];

		if(14 != recv_data.length[recvdatapt])
		{
			printf("%d C_SE_NC: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			cmd_ack = -1;		/* negative confirm */
			comdflag = 0;
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
				comdflag = 0;
#ifdef DEBUG
				printf("set-point %u NACK INF=%d fval=%f cause of transmission=%2x received!\n",type_id,infoaddr,value,causetra);
#endif
				strcat(name_buf,COM_CTRL_FAIL);
				BroadCastSms(name_buf,true);
				break;
			}
			if(qoc & 0x80)
			{
				cmd_ack = 2;		/* select command ACK */
				comdflag = 2;
#ifdef DEBUG
				printf("IEC SE %u select command ACK INF=%d %f com=%x \n",type_id,infoaddr,value,qoc);
#endif
			}
			else
			{
				cmd_ack = 1;		/* execute command ACK */
#ifdef DEBUG
				printf("IEC SE %u execute command ACK INF=%d %f com=%x \n",type_id,infoaddr,value,qoc);
#endif
				strcat(name_buf,COM_CTRL_SUCCESS);
				BdSms(name_buf);
			}
		}
		else if((causetra & 0x3F) == ACTTERM)
		{
			cmd_ack = 0;		/* execute command Term */
			comdflag = 0;
#ifdef DEBUG
			printf("IEC SE %u execute command terminal INF=%d %f com=%x \n",type_id,infoaddr,value,qoc);
#endif
			
		}
		else
		{
			cmd_ack = 0;		/* execute command confirm */
#ifdef DEBUG
			printf("set-point %u INF=%d %f com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,value,qoc,causetra);
#endif
			strcat(name_buf,COM_CTRL_FAIL);
			BroadCastSms(name_buf,true);
		}
		break;

	case M_EI_NA:	/* Slave station End of Initialization */

		if(10 != recv_data.length[recvdatapt])
		{
			printf("%d M_EI_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			break;
		}
		switch(causetra & 0x3F)
		{
		case INIT:		
#ifdef DEBUG
			printf("End of Initialization in remote slave system!\n");
#endif
			break;
		default :
			printf("M_EI_NA: Received cause of transmission=%2x wrong!\n",causetra);
			break;
		}/* switch(causetra & 0x3F) */
 		break;

	case C_IC_NA:	/* confirm of general interrogation command */
			
		if(10 != recv_data.length[recvdatapt])
		{
			printf("%d C_IC_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			cmd_ack = -1;		/* negative confirm */
			break;
		}
		switch(causetra & 0x3F)
		{
		case ACTCON:		/* general interrogation command act confirm */

			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
/* 				interrogationflag[stn_id] = 0; */
#ifdef DEBUG
				printf("IEC general interrogation command is NACK by remote system!\n");
#endif
			}
			else
			{
				cmd_ack = 1;		/* command confirm */
/* 				interrogationflag[stn_id] = 2; */
#ifdef DEBUG
				printf("IEC general interrogation command is ACK by remote system!\n");
#endif
			}
			break;

		case ACTTERM:				/* general interrogation command act terminal */


#ifdef DEBUG
			printf("IEC general interrogation command is terminal in remote system!\n");
#endif
/*
			DBSentAll(pName);
*/
			break;

		case DEACTCON:

			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				printf("IEC general interrogation deact command is NACK by remote system!\n");
#endif
			}
			else
			{
				cmd_ack = 1;		/* positive confirm */
/* 				interrogationflag[stn_id] = 0; */
#ifdef DEBUG
				printf("IEC general interrogation deact command is ACK by remote system!\n");
#endif
			}
			break;
		default :
			printf("C_IC_NA: Received cause of transmission=%2x wrong!\n",causetra);
			break;
		}/* switch(causetra & 0x3F) */
 			
		break;

	case C_CI_NA:	/* confirm of counter interrogation command */

		if(10 != recv_data.length[recvdatapt])
		{
			printf("%d C_CI_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			cmd_ack = -1;		/* negative confirm */
			break;
		}
		switch(causetra & 0x3F)
		{
		case ACTCON:		/* counter interrogation command act confirm */
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
/* 				counterflag[stn_id] = 0; */
				cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				printf("IEC counter interrogation command is NACK by remote system!\n");
#endif
			}
			else
			{
				cmd_ack = 1;		/* execute command confirm */
/* 				counterflag[stn_id] = 2; */
#ifdef DEBUG
				printf("IEC counter interrogation command is ACK by remote system!\n");
#endif
			}
			break;

		case ACTTERM:				/* counter interrogation command act terminal */
/*
			counterflag[stn_id] = 0;
			initflag[stn_id] =0;
*/
#ifdef DEBUG
			printf("IEC counter interrogation command is terminal in remote system!\n");
#endif
			break;

		case DEACTCON:

			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				printf("IEC counter interrogation deact command is NACK by remote system!\n");
#endif
			}
			else
			{
				cmd_ack = 1;		/* positive confirm */
				printf("IEC counter interrogation deact command is ACK by remote system!\n");
			}
			break;

		default :
#ifdef DEBUG
			printf("C_CI_NA: Received cause of transmission=%2x wrong!\n",causetra);
#endif
			break;
		}/* switch(causetra & 0x3F) */
		break;

	case C_CS_NA:	/* confirm of clock synchronization command */
			
		if(20 != recv_data.length[recvdatapt])
		{
			printf("%d C_CS_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			cmd_ack = -1;		/* negative confirm */
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
/* 				ClockSyncflag[stn_id] = 0; */
#ifdef DEBUG
				printf("clock synchronization command is NACK by remote system!\n");
#endif
			}
			else
			{
				cmd_ack = 1;		/* execute command confirm */
/* 				ClockSyncflag[stn_id] = 0; */
#ifdef DEBUG
				printf("clock synchronization command is ACK by remote system!\n");
#endif
			}
		}
		else
			printf("clock synchronization command: Received cause of transmission=%2x wrong!\n",causetra);
		break;

	case P_ME_NA:	/* parameter for measurements,normalized */
			
#ifdef SUN
		CrossCopy(&infoaddr,&recv_data.data[recvdatapt][6],2);
		CrossCopy(&ana16data, &recv_data.data[recvdatapt][9],2);
#else
		memcpy(&infoaddr,&recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&ana16data, &recv_data.data[recvdatapt][9],2);
#endif
		qoc = recv_data.data[recvdatapt][11];

		if(12 != recv_data.length[recvdatapt])
		{
			printf("%d P_ME_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,recv_data.length[recvdatapt]);
			cmd_ack = -1;		/* negative confirm */
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				printf("parameter set %u NACK INF=%d stval=%x kind of parameter=%2x received!\n",
					type_id,infoaddr,ana16data,qoc);
#endif
			}
			else
			{
				cmd_ack = 0;		/* execute command confirm */
				comdflag = 0;
#ifdef DEBUG
				printf("parameter set %u ACK INF=%d stval=%x kind of parameter=%2x received!\n",
					type_id,infoaddr,ana16data,qoc);
#endif
			}
		}
		else
		{
			cmd_ack = 0;
#ifdef DEBUG
			printf("parameter set %u INF=%d com=%x cause of transmission=%2x wrong!\n",
				type_id,infoaddr,qoc,causetra);
#endif
		}
		break;

	default :
		printf("IEC type_id = %u is unavailable at this time!\n",type_id);
		break;
	}/* switch(type_id) */

	recvdatapt++;
	if( recvdatapt >= ASDU_ENTRY_NUMBER )
		recvdatapt = 0;
}	

int	IEC870_5_Send(int sock)
{
	short	sd_pnt;
	//unsigned char cmd_type,stn_id,op_type; 
	unsigned char cmd_type,op_type; 
	unsigned char stnid;		//added by yyp 2010.03.29
	struct net_cmd_data	cmd_rec;

	COMMAND_STATUS cmd_stat;
	short	rcv_pnt;
	POINTER	rcv_ptr;
	POINTER	ptr;

	int	cmd_data/*,normv*/;
 	char	host_id;
	short	/*ob_addr,base_addr,i,*/pntnum;
	float	ftemp,setpv;
	time_t	rec_time,curr_time_tt; /*zwj for cmd overtime*/
	struct	tm	timeptr;

	sd_pnt = 0;
	comdflag = 0;
	cmd_type = 0;
	stnid= cur_stn_id;
	printf("@@cmd_end=%d@@ stnid=%d, head=%d@@\n",cmd_end,stnid,shm_comm_cmd->stn_data[stnid].head);
	//modified by yyp 2011.03.09
	while( cmd_end != shm_comm_cmd->stn_data[stnid].head )
	{
		rcv_pnt = cmd_end;
		cmd_end++;
		if(cmd_end >= MAX_NETCMD_SEG_NUM)
			cmd_end = 0;
		//pCmd_alm = &shm_rec_addr->rec[rcv_pnt].cmd_rec;
		cmd_rec =shm_comm_cmd->stn_data[stnid].seg[rcv_pnt].net_cmd_buf.netdata;
		ClearCommShm(stnid,rcv_pnt,TYPE_ONE_SEG);	
#ifdef _BIG_ENDIAN_
		Swap2Byte( &(cmd_rec.point_id));
		Swap2Byte( &(cmd_rec.status));
		Swap4Byte( &(cmd_rec.data));
		Swap2Byte( &(cmd_rec.ctrl_addr));
		Swap2Byte( &(cmd_rec.year));
#endif
		if(cmd_rec.type_id < C_LCU_ONOFF)
			continue;

		cmd_type = cmd_rec.type_id;
		cmd_stat = cmd_rec.status;
		cmd_data = cmd_rec.data;//?

		op_type = cmd_stat.opr;

		rcv_ptr.stn_id = cmd_rec.stn_id;
		rcv_ptr.dev_id = cmd_rec.dev_id;
		rcv_ptr.data_type = cmd_rec.data_type;
		rcv_ptr.pt_id = cmd_rec.point_id;
		host_id = cmd_rec.host_id;

#ifdef DEBUG
		printf("\nNew Cmd:cmd_type=%d stn_id=%d dev_id=%d data_type=%d point_id=%d cmd_stat=%d cmd_data=%f host_id=%d,op_type=%d\n",
			cmd_type, rcv_ptr.stn_id, rcv_ptr.dev_id, rcv_ptr.data_type,rcv_ptr.pt_id,cmd_stat.state,cmd_data/1000,host_id,op_type);
		
#endif
		rec_time=GetTimefromCmd(&cmd_rec);
		if(-1==rec_time)
		{
/*			sprintf(infobuf,"\n记录时间错误：stn_id=%d dev_id=%d data_type=%d point_id=%d cmd_stat=%d cmd_data=%f host_id=%d\n",
			cmd_rec.stn_id, cmd_rec.type_id, cmd_rec.data_type,cmd_rec.pt_id,cmd_rec.cmd_status,cmd_rec.data.iValue/1000,cmd_rec.host_id);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);*/
			continue;
		}

		curr_time_tt=time(NULL);
		timeptr=*localtime(&curr_time_tt);

		if((rec_time-curr_time_tt)>(5*60))
		{
			sprintf(infobuf,"超前5分钟:记录时间%04d-%02d-%02d %d:%d:%d  本机时间%04d-%02d-%02d %d:%d:%d\n",cmd_rec.year,cmd_rec.month,cmd_rec.day,
				cmd_rec.hour,cmd_rec.minute,cmd_rec.second,timeptr.tm_year+1900,timeptr.tm_mon+1,timeptr.tm_mday,timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			continue;
		}
		if((curr_time_tt-rec_time)>(5*60))
		{
			sprintf(infobuf,"5分钟以前命令，命令失效:记录时间%04d-%02d-%02d %d:%d:%d  本机时间%04d-%02d-%02d %d:%d:%d\n",cmd_rec.year,cmd_rec.month,cmd_rec.day,
				cmd_rec.hour,cmd_rec.minute,cmd_rec.second,timeptr.tm_year+1900,timeptr.tm_mon+1,timeptr.tm_mday,timeptr.tm_hour,timeptr.tm_min,timeptr.tm_sec);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			continue;
		}

		switch	( cmd_type ) 
		{
		case C_SWITCH:
		case C_BREAKER:
		case C_DEVICE:
		case C_GATE:
		case C_TRANS:
		case C_SYS_MODE_SET:
		case C_SIGNAL_RESET:
		case DPS_MAN_STAT_SET:
		case C_ONOFF_FORCE:

			for(pntnum=0;pntnum<dcdef.number;pntnum++)		/* Check for DC Command */
			{
				ptr = dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(dcdef.cmdtype[pntnum] == cmd_type))
				{
					if(dcdef.optype[pntnum] == op_type)
					{
						if(Cmd_Type == 0)
							cmdbuf.stat = 0x1;
						else
							cmdbuf.stat = 0x2;
					}
/*					else if((dcdef.optype[pntnum] +1) == op_type)
					cmdbuf.stat = 0x1;
*/
					else
						continue;
					cmdbuf.cot = ACT;
					cmdbuf.infoaddr = pntnum+dc_cmd_base_addr;
					cmdbuf.ack_flag = 0;
					cmdbuf.selected = 1;
					cmdbuf.term_flag = 0;
					cmdbuf.sel_tim = time(&cmdbuf.sel_tim);
					if(Cmd_Type == 0)
					{
						cmdbuf.tpyeId = C_SC_NA;
						if(Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					else
					{
						cmdbuf.tpyeId = C_DC_NA;
						if(Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					comdflag = 1;
					sprintf(infobuf,"\n遥控令:%s,cmd_type=%d,op_type=%d\n",dcdef.name[pntnum],cmd_type,op_type);
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
					return(true);
				}
			}
			break;

		case C_UNIT:

		
			for(pntnum=0;pntnum<dcdef.number;pntnum++)		/* Check for DC Command */
			{
				ptr = dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(dcdef.cmdtype[pntnum] == cmd_type))
				{
					if(dcdef.optype[pntnum] == op_type)
					{
						if(Cmd_Type == 0)
							cmdbuf.stat = 0x1;
						else
							cmdbuf.stat = 0x2;
					}
/*
					else if((dcdef.optype[pntnum] +1) == op_type)
						cmdbuf.stat = 0x1;
*/
					else
						continue;
					cmdbuf.cot = ACT;
					cmdbuf.infoaddr = pntnum+dc_cmd_base_addr;
					cmdbuf.ack_flag = 0;
					cmdbuf.selected = 1;
					cmdbuf.term_flag = 0;
					cmdbuf.sel_tim = time(&cmdbuf.sel_tim);
					if(Cmd_Type == 0)
					{
						cmdbuf.tpyeId = C_SC_NA;
						cmdbuf.stat = 0x1;
						if(Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					else
					{
						cmdbuf.tpyeId = C_DC_NA;
						cmdbuf.stat = 0x2;
						if(Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					comdflag = 1;
					sprintf(infobuf,"\n遥控令:%s,cmd_type=%d,op_type=%d\n",dcdef.name[pntnum],cmd_type,op_type);
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
					return(true);
				}
			}
			break;

		case C_SPEED_STEP:
		case C_VOLT_STEP:


			for(pntnum=0;pntnum<dcdef.number;pntnum++)		/* Check for DC Command */
			{

				ptr = dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(dcdef.cmdtype[pntnum] == cmd_type)&&(dcdef.optype[pntnum] == op_type))
				{
					cmdbuf.cot = ACT;
					cmdbuf.infoaddr = pntnum+dc_cmd_base_addr;
					cmdbuf.ack_flag = 0;
					cmdbuf.selected = 1;
					cmdbuf.term_flag = 0;
					cmdbuf.sel_tim = time(&cmdbuf.sel_tim);
					if(Cmd_Type == 0)
					{
						cmdbuf.tpyeId = C_SC_NA;
						cmdbuf.stat = 0x1;
						if(Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					else
					{
						cmdbuf.tpyeId = C_DC_NA;
						cmdbuf.stat = 0x2;
						if(Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					comdflag = 1;
					sprintf(infobuf,"\n遥控令:%s,cmd_type=%d,op_type=%d\n",dcdef.name[pntnum],cmd_type,op_type);
					WriteDspInfo(infobuf);
					WriteLogFile(infobuf);
					return(true);
				}
			}
			break;

		case C_APOWER:
		case C_RPOWER:

			for(pntnum=0;pntnum<sedef.number;pntnum++)
			{
				ptr = sedef.ptr[pntnum];

				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(sedef.cmdtype[pntnum] == cmd_type))
					break;
			}
			if(pntnum>=sedef.number)
			{
				printf("No this C_APOWER/C_RPOWER:%d stn:%d dev:%d in set point table!\n",cmd_type,rcv_ptr.stn_id,rcv_ptr.dev_id);
				break;
			}
			sebuf.tpyeId = C_SE_NC;				/* cmd type of ASDU */
			sebuf.cot = ACT;
			sebuf.infoaddr = pntnum+se_cmd_float_addr;
			sebuf.qoc = 0;

			setpv = (float)(cmd_data/1000.0);
			sebuf.fvalue = setpv;
			printf("cmd_data= %d; value=%f\n",cmd_data,setpv);

			if(!SetPointCmdNC((char)0,sebuf.infoaddr,(float)setpv,sock))
				return(false);
			comdflag = 1;
			sprintf(infobuf,"\n遥调令:%s,cmd_type=%d\n",sedef.name[pntnum],cmd_type);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			break;

		case DPS_MAN_VALUE_SET:
		case C_OP_VALUE_SET:

			for(pntnum=0;pntnum<sedef.number;pntnum++)		/* Check for SE Command */
			{
				ptr = sedef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(sedef.cmdtype[pntnum] == cmd_type))
				{
					break;
				}
			}
			if(pntnum>=sedef.number)
			{
				continue;
			}
			sebuf.tpyeId = C_SE_NC;				/* cmd type of ASDU */
			sebuf.cot = ACT;
			sebuf.infoaddr = pntnum+se_cmd_float_addr;
			sebuf.qoc = 0;

			setpv = (float)(cmd_data/1000.0);
			sebuf.fvalue = setpv;

			if(!SetPointCmdNC(0,sebuf.infoaddr,sebuf.fvalue,sock))
				return(false);
			comdflag = 1;
			sprintf(infobuf,"\n遥调令:%s,cmd_type=%d\n",sedef.name[pntnum],cmd_type);
			WriteDspInfo(infobuf);
			WriteLogFile(infobuf);
			break;

		default:
			for(pntnum=0;pntnum<dcdef.number;pntnum++)		/* Check for DC Command */
			{
				ptr = dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(dcdef.cmdtype[pntnum] == cmd_type))
				{
					if(dcdef.optype[pntnum] == op_type)
					{
						if(Cmd_Type == 0)
							cmdbuf.stat = 0x1;
						else
							cmdbuf.stat = 0x2;
					}

					else
						continue;
					cmdbuf.cot = ACT;
					cmdbuf.infoaddr = pntnum+dc_cmd_base_addr;
					cmdbuf.ack_flag = 0;
					cmdbuf.selected = 1;
					cmdbuf.term_flag = 0;
					cmdbuf.sel_tim = time(&cmdbuf.sel_tim);
					if(Cmd_Type == 0)
					{
						cmdbuf.tpyeId = C_SC_NA;
						if(Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					else
					{
						cmdbuf.tpyeId = C_DC_NA;
						if(Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,cmdbuf.infoaddr,cmdbuf.stat,sock))
								return(FALSE);
						}
					}
					comdflag = 1;
#ifdef DEBUG
					printf("cmd_type=%d stn_id=%d dev_id=%d data_type=%d point_id=%d cmd_stat=%d cmd_data=%f host_id=%d\n",
						cmd_type, rcv_ptr.stn_id, rcv_ptr.dev_id, rcv_ptr.data_type,rcv_ptr.pt_id,cmd_stat.state,cmd_data/1000,host_id);
#endif
					return(true);
				}
			}
			break;
		}/* switch(cmd_type) */
	}/* while(shm_send_addr->head!= cmd_end) */
	return(true);
}/* IEC870_5_Send() */

//added by yyp 2011.03.09
long GetTimefromCmd(struct net_cmd_data *p_rec)
{
	struct tm tm_time;
	tm_time.tm_year=p_rec->year-1900;
	tm_time.tm_mon=p_rec->month-1;

	tm_time.tm_mday=p_rec->day;
	tm_time.tm_hour=p_rec->hour;
	tm_time.tm_min=p_rec->minute;
	tm_time.tm_sec=p_rec->second;
	tm_time.tm_isdst= 0;
	return mktime( &tm_time );
}