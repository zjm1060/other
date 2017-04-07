/********************************************************
*  FILE NAME:           iec104_app_fun.c    			*
*  FUNCTION:            Controlling Station			    *
*			Application Layer Functions		            *
*  's NAME :  			              			        *
*  Version :            Win3.0a-->Win4.0a  by  zwj   	*	     
*  DATE:                2008.11                			*
*********************************************************/

#include "../inc/gis_iec104_com.h"

//#define	DEBUG


extern	int			ProcNum;
extern	PROC_REC	sProcRec[MAX_PROC_NUM+1];
extern struct	net_cmd		*shm_net_cmd_addr;
extern char chan9698CommPtName[ALL_CHANNEL_NUM][256];
extern char local_hostname[36], pair_hostname[36];

int Gen9698CommPtTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* single point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	short		j;
	char		filenamestr[200];
	char		linebuf[256], temp[256];
	
	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);
	OutputDspInfo(OUTPUT_ERROR, "Read 9698CommPtTable file %s",filenamestr);
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
		OutputDspInfo(OUTPUT_ERROR, "####can't open 9698 comm point file %s\n",filenamestr);
		return false;
	}
		  
	rewind(fp);	
	
	j=0;
	while(fgets(linebuf,256,fp))
	{
		if(j>=ALL_CHANNEL_NUM)
			break;
		if((linebuf[0] == '#')||(linebuf[0] == '!'))
		{
			OutputDspInfo(0,"%s",linebuf);
			continue;
		}
		if(1!=sscanf(linebuf,"%s", temp))
			continue;

		OutputDspInfo(OUTPUT_ERROR, "9698 state pt %s",temp);
		if(ReadEntryByNameStr(temp,&dbentry2)==-1)
		{
			OutputDspInfo(OUTPUT_ERROR, "#### 9698 state %d pnt: %s No this point in the database file!!!\n",j,temp);
			fclose(fp);
			return false;
		}
		else
		{
			strcpy(chan9698CommPtName[j], temp);
			j++;
		}
	}
	fclose(fp);

	if(j!=ALL_CHANNEL_NUM)
	{
		OutputDspInfo(OUTPUT_ERROR, "#### 9698 state point num error: %d, must be %d",j, ALL_CHANNEL_NUM);
		return false;
	}

	return(true);
}

void InitData(IEC104_CMM_INFO *info, char *NameID, int ip_no)
{
	info->nASDUId = CMM_ADRESS;
	info->nSrcId = SRC_ADRESS;
	info->Gps = 1;		/* With GPS */
	info->GiType = 1;		/* 1:M_SP_NA; 20: M_PS_NA */
	info->MeASDUType = 3;		/* Me ASDU type> 0: M_ME_NA; 1: M_ME_NB; 3: M_ME_NC */
	info->MeType = 0;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
	info->SEType =0;		/* 0: Positive(0 ~ 32767); 1: Full (-32768 ~ 32767) */
/* 	TimeTag = 2; */		/* time tag 0: no; 1: short; 2: long */
	info->sp_cos = 0;		/* info->sp_cos 0: sp/dp transmit once; 1: twice */
	info->Cmd_Type = 1;	/*  0: single point ctrl C_SC_NA; 1: double point ctrl C_DC_NA */
	info->Cmd_Sel = 1;	/*  0: cmd without select; 1: cmd with select */

	info->scan_period = SCAN_PERIOD;
	info->cmd_period = CMD_TIMEOUT;

	info->sp_base_addr = SP_BASE_ADDR;
	info->dp_base_addr = DP_BASE_ADDR;
	info->me_base_addr = ME_BASE_ADDR;
	info->me_scale_addr = ME_SCALE_ADDR;
	info->me_float_addr = ME_FLOAT_ADDR;
	info->cyc_base_addr = CYC_BASE_ADDR;
	info->cyc_scale_addr = CYC_SCALE_ADDR;
	info->cyc_float_addr = CYC_FLOAT_ADDR;
	info->pm_base_addr = PM_BASE_ADDR;
	info->it_base_addr = IT_BASE_ADDR;
	info->st_base_addr = ST_BASE_ADDR;
	info->bo_base_addr = BO_BASE_ADDR;

	info->sc_cmd_base_addr = SC_CMD_BASE_ADDR;
	info->dc_cmd_base_addr = DC_CMD_BASE_ADDR;
	info->rc_cmd_base_addr = RC_CMD_BASE_ADDR;

	info->se_cmd_base_addr = SE_CMD_BASE_ADDR;
	info->se_cmd_scale_addr = SE_CMD_SCALE_ADDR;
	info->se_cmd_float_addr = SE_CMD_FLOAT_ADDR;
	info->bc_cmd_base_addr = BC_CMD_BASE_ADDR;

/* 	m_cRespGenInterroGrpNum=m_cRespCountGrpNum=m_cCyclicGrpNum=m_cScanGrpNum=0; */
	info->send_data.pnt = 0;
	info->recv_data.pnt = 0;

	info->cmd_ack = 0;
	info->cmdbuf.tpyeId = 0;
	info->cmdbuf.infoaddr = 0;
	info->cmdbuf.ack_flag = 0;
	info->cmdbuf.selected = 0;
	info->cmdbuf.term_flag = 0;
	info->cmdbuf.sel_tim = 0;

	info->sebuf.ack_flag = 0;

	info->spdef.number = 0;
	info->dpdef.number = 0;
	info->stdef.number = 0;
	info->itdef.number = 0;

	info->medef.number = 0;

	info->sedef.number = 0;

	info->scdef.number = 0;
	info->dcdef.number = 0;
	info->rcdef.number = 0;

	GenConfigTable(info, NameID,(char *)CONFIG_FILE);

	GenSPTable(info, NameID,(char *)SPDEF_FILE);
	GenDPTable(info, NameID,(char *)DPDEF_FILE, ip_no);

	GenMETable(info, NameID,(char *)MEDEF_FILE);
/*
	GenSTTable(NameID,STDEF_FILE);
	GenITTable(NameID,ITDEF_FILE);
*/
//	GenSETable(NameID,SEDEF_FILE);

	GenDCTable(info, NameID,(char *)DCDEF_FILE);
/*
	GenSCTable(NameID,(char *)SCDEF_FILE);
	GenRCTable(NameID,(char *)RCDEF_FILE);

	SaveMETable(NameID,MEDEF_DATA);
*/

	Gen9698CommPtTable(info, NameID, (char *)NR9698_COMM_PT_FILE);

	info->nr_recv = info->nr_send = info->nr_ack = info->ns_ack = info->nr_rs = 0;
	info->cmd_end= shm_net_cmd_addr->head;
}


int ReadStnDef(char *pStnDefFile)
{
	FILE	*fp;
	char	NameID[256], ip[4][256];
	char	filebuf[256];
	int		r, ret, i;
	char	filenamestr[256];
	
	r=0;
	ProcNum = 0;
	sprintf(filenamestr,"%s%s_%s",IEC_HOMEPATH,local_hostname,pStnDefFile);
	while( (fp=fopen(filenamestr,"r"))==NULL )
	{
		OutputDspInfo(OUTPUT_ERROR,"\nFail to open Station config file %s !!! \n",pStnDefFile);
		sleep(5);
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
		if(fgets(filebuf,256,fp)==NULL)
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
		if((ret=sscanf(filebuf,"%s %s %s %s %s",NameID,ip[0],ip[1],ip[2],ip[3]))<2)
		{
			OutputDspInfo(OUTPUT_ERROR,"Error!%3d line: %s\n",r,filebuf);
			continue;
		}

		strcpy( sProcRec[ProcNum].NameID, NameID );
		for(i=0; i<(ret-1); i++)
			strcpy( sProcRec[ProcNum].ip[i], ip[i]);
		sProcRec[ProcNum].ip_num= ret-1;

		OutputDspInfo(0,"\nStation %d name %s: IPADDR = %s %s %s %s\n",ProcNum,sProcRec[ProcNum].NameID,
			sProcRec[ProcNum].ip[0],sProcRec[ProcNum].ip[1],sProcRec[ProcNum].ip[2],sProcRec[ProcNum].ip[3]);

		if(sProcRec[ProcNum].NameID[0] != '!')
			ProcNum++;
	}
}

int GenConfigTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)
{
	FILE	*fp;
	int	iTemp,iRet;
	char	strFlag[80];
	char	filenamestr[200];
	char	strType[80];

	sprintf(filenamestr,"%s%s_%s",IEC_HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	OutputDspInfo(0, "\n\nRead config file %s!!!\n",filenamestr);
#endif
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(0, "\n####Can't open config file %s!!!\n",filenamestr);
		info->nASDUId = CMM_ADRESS;
		sleep(3);
	}
	rewind(fp);	
#ifdef DEBUG
	OutputDspInfo(0, "\n\nRead config file %s!!!\n",filenamestr);
#endif
	for(;;)
	{
		fscanf(fp,"%s",strFlag);
		if(feof(fp))
		{
			OutputDspInfo(0, "Config file %s End!\n",filenamestr);
			fclose(fp);
			return(true);
		}
		if(strcmp(strFlag,"CMM_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ASDU CMM_ADDR=%d\n",pDefFile,iTemp);
				info->nASDUId = CMM_ADRESS;
				continue;
			}
			if((iTemp<=0) || (iTemp>254))
			{
				OutputDspInfo(0, "Config file %s Error! ASDU CMM_ADDR=%d\n",pDefFile,iTemp);
				info->nASDUId = CMM_ADRESS;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ASDU CMM_ADDR = %d\n",iTemp);
#endif
				info->nASDUId = iTemp;
			}
		}
		else if(strcmp(strFlag,"SRC_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ASDU SRC_ADDR=%d\n",pDefFile,iTemp);
				info->nSrcId = SRC_ADRESS;
				continue;
			}
			if((iTemp<=0) || (iTemp>255))
			{
				OutputDspInfo(0, "Config file %s Error! ASDU SRC_ADDR=%d\n",pDefFile,iTemp);
				info->nSrcId = SRC_ADRESS;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ASDU SRC_ADDR = %d\n",iTemp);
#endif
				info->nSrcId = iTemp;
			}
		}
		else if(strcmp(strFlag,"GPS:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! GPS = %d\n",pDefFile,iTemp);
				info->Gps = 1;
				continue;
			}
			if((iTemp!=0) && (iTemp!=1))
			{
				OutputDspInfo(0, "Config file %s Error! info->Gps = %d\n",pDefFile,iTemp);
				info->Gps = 1;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "GPS = %d\n",iTemp);
#endif
				info->Gps = iTemp;
			}
		}
		else if(strcmp(strFlag,"GI_TYPE:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! GI_TYPE = %d\n",pDefFile,iTemp);
				info->GiType = 1;
				continue;
			}
			if((iTemp!=1) && (iTemp!=20))
			{
				OutputDspInfo(0, "Config file %s Error! GI_TYPE = %d\n",pDefFile,iTemp);
				info->GiType = 1;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "GI_TYPE = %d\n",iTemp);
#endif
				info->GiType = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_ASDUTYPE:")== 0)
		{
/*			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				info->MeASDUType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(0, "Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				info->MeASDUType = 0;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_TYPE = %d\n",iTemp);
#endif
				info->MeASDUType = iTemp;
			}
*/
			iRet = fscanf(fp,"%s",strType);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ME_ASDUTYPE = %d\n",pDefFile,iTemp);
				info->MeASDUType = 0;
				continue;
			}
			if(strcmp(strType,"M_ME_NA")== 0)
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_ASDUTYPE = %s\n",strType);
#endif
				info->MeASDUType = 0;
			}
			else if(strcmp(strType,"M_ME_ND")== 0)
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_ASDUTYPE = %s\n",strType);
#endif
				info->MeASDUType = 1;
			}
			else if(strcmp(strType,"M_ME_NC")== 0)
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_ASDUTYPE = %s\n",strType);
#endif
				info->MeASDUType = 3;
			}
			else
			{
				OutputDspInfo(0, "Config file %s Error! ME_ASDUTYPE = %s\n",pDefFile,strType);
				info->MeASDUType = 3;
			}
		}
		else if(strcmp(strFlag,"ME_TYPE:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				info->MeType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(0, "Config file %s Error! ME_TYPE = %d\n",pDefFile,iTemp);
				info->MeType = 0;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_TYPE = %d\n",iTemp);
#endif
				info->MeType = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_TYPE:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! SE_TYPE = %d\n",pDefFile,iTemp);
				info->SEType = 0;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(0, "Config file %s Error! SE_TYPE = %d\n",pDefFile,iTemp);
				info->SEType = 0;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "SE_TYPE = %d\n",iTemp);
#endif
				info->SEType = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMD_TYPE:")== 0)
		{
/*
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! CMD_TYPE = %d\n",pDefFile,iTemp);
				info->Cmd_Type = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(0, "Config file %s Error! CMD_TYPE = %d\n",pDefFile,iTemp);
				info->Cmd_Type = 1;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "CMD_TYPE = %d\n",iTemp);
#endif
				info->Cmd_Type = iTemp;
			}
*/
			iRet = fscanf(fp,"%s",strType);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! CMD_TYPE = %s\n",pDefFile,strType);
				info->Cmd_Type = 1;
				continue;
			}
			if(strcmp(strType,"C_SC_NA")== 0)
			{
#ifdef DEBUG
				OutputDspInfo(0, "CMD_TYPE = %s\n",strType);
#endif
				info->Cmd_Type = 0;
			}
			else if(strcmp(strType,"C_DC_NA")== 0)
			{
#ifdef DEBUG
				OutputDspInfo(0, "CMD_TYPE = %s\n",strType);
#endif
				info->Cmd_Type = 1;
			}
			else
			{
				OutputDspInfo(0, "Config file %s Error! CMD_TYPE = %s unknown!\n",pDefFile,strType);
				info->Cmd_Type = 1;
			}
		}
		else if(strcmp(strFlag,"CMD_SELECT:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! CMD_SELECT = %d\n",pDefFile,iTemp);
				info->Cmd_Sel = 1;
				continue;
			}
			if((iTemp<0) || (iTemp>2))
			{
				OutputDspInfo(0, "Config file %s Error! CMD_SELECT = %d\n",pDefFile,iTemp);
				info->Cmd_Sel = 1;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "CMD_SELECT = %d\n",iTemp);
#endif
				info->Cmd_Sel = iTemp;
			}
		}
		else if(strcmp(strFlag,"SCAN_PERIOD:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! SCAN_PERIOD = %d\n",pDefFile,iTemp);
				info->scan_period = SCAN_PERIOD;
				continue;
			}
			if(iTemp<0)
			{
				OutputDspInfo(0, "Config file %s Error! SCAN_PERIOD = %d\n",pDefFile,iTemp);
				info->scan_period = SCAN_PERIOD;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "SCAN_PERIOD = %d\n",iTemp);
#endif
				info->scan_period = iTemp;
			}
		}
		else if(strcmp(strFlag,"CMD_TIMEOUT:")== 0)
		{
			iRet = fscanf(fp,"%d",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! CMD_TIMEOUT = %d\n",pDefFile,iTemp);
				info->cmd_period = CMD_TIMEOUT;
				continue;
			}
			if(iTemp<0)
			{
				OutputDspInfo(0, "Config file %s Error! CMD_TIMEOUT = %d\n",pDefFile,iTemp);
				info->cmd_period = CMD_TIMEOUT;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "CMD_TIMEOUT = %d\n",iTemp);
#endif
				info->cmd_period = iTemp;
			}
		}
		else if(strcmp(strFlag,"SP_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! SP_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->sp_base_addr = SP_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! SP_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->sp_base_addr = SP_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "SP_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->sp_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"DP_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! DP_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->dp_base_addr = DP_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! DP_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->dp_base_addr = DP_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "DP_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->dp_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ME_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->me_base_addr = ME_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! ME_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->me_base_addr = ME_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->me_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_SCALE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ME_SCALE_ADDR=%X\n",pDefFile,iTemp);
				info->me_scale_addr = ME_SCALE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! ME_SCALE_ADDR=%X\n",pDefFile,iTemp);
				info->me_scale_addr = ME_SCALE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_SCALE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->me_scale_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ME_FLOAT_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ME_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				info->me_float_addr = ME_FLOAT_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! ME_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				info->me_float_addr = ME_FLOAT_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ME_FLOAT_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->me_float_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYC_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! CYC_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->cyc_base_addr = CYC_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! CYC_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->cyc_base_addr = CYC_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "CYC_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->cyc_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYC_SCALE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! CYC_SCALE_ADDR=%X\n",pDefFile,iTemp);
				info->cyc_scale_addr = CYC_SCALE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! CYC_SCALE_ADDR=%X\n",pDefFile,iTemp);
				info->cyc_scale_addr = CYC_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "CYC_SCALE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->cyc_scale_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"CYC_FLOAT_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! CYC_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				info->cyc_float_addr = CYC_FLOAT_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! CYC_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				info->cyc_float_addr = CYC_FLOAT_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "CYC_FLOAT_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->cyc_float_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"PM_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! PM_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->pm_base_addr = PM_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! PM_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->pm_base_addr = PM_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "PM_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->pm_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"IT_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! IT_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->it_base_addr = IT_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! IT_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->it_base_addr = IT_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "IT_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->it_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"ST_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! ST_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->st_base_addr = ST_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! ST_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->st_base_addr = ST_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "ST_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->st_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"BO_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! BO_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->bo_base_addr = BO_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! BO_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->bo_base_addr = BO_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "BO_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->bo_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! SC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->sc_cmd_base_addr = SC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! SC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->sc_cmd_base_addr = SC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "SC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->sc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"DC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! DC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->dc_cmd_base_addr = DC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! DC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->dc_cmd_base_addr = DC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "DC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->dc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"RC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! RC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->rc_cmd_base_addr = RC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! RC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->rc_cmd_base_addr = RC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "RC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->rc_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! SE_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->se_cmd_base_addr = SE_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! SE_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->se_cmd_base_addr = SE_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "SE_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->se_cmd_base_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_CMD_SCALE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! SE_CMD_SCALE_ADDR=%X\n",pDefFile,iTemp);
				info->se_cmd_scale_addr = SE_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! SE_CMD_SCALE_ADDR=%X\n",pDefFile,iTemp);
				info->se_cmd_scale_addr = SE_CMD_SCALE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "SE_CMD_SCALE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->se_cmd_scale_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"SE_CMD_FLOAT_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! SE_CMD_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				info->se_cmd_float_addr = SE_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! SE_CMD_FLOAT_ADDR=%X\n",pDefFile,iTemp);
				info->se_cmd_float_addr = SE_CMD_FLOAT_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "SE_CMD_FLOAT_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->se_cmd_float_addr = iTemp;
			}
		}
		else if(strcmp(strFlag,"BC_CMD_BASE_ADDR:")== 0)
		{
			iRet = fscanf(fp,"%x",&iTemp);
			if(iRet<=0)
			{
				OutputDspInfo(0, "Config file %s Error! BC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->bc_cmd_base_addr = BC_CMD_BASE_ADDR;
				continue;
			}
			if((iTemp<=0) || (iTemp>65535))
			{
				OutputDspInfo(0, "Config file %s Error! BC_CMD_BASE_ADDR=%X\n",pDefFile,iTemp);
				info->bc_cmd_base_addr = BC_CMD_BASE_ADDR;
			}
			else
			{
#ifdef DEBUG
				OutputDspInfo(0, "BC_CMD_BASE_ADDR = %d %04X\n",iTemp,iTemp);
#endif
				info->bc_cmd_base_addr = iTemp;
			}
		}
		else
		{
			OutputDspInfo(0, "unsupported item: %s",strFlag);
			fscanf(fp,"%s",strFlag);
		}
	}
/*	fclose(fp);
	return(true);
*/
}

int GenSPTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* single point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[128],lgname[128];
	int		iTemp;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);
	OutputDspInfo(OUTPUT_WARN, "\n\nRead SPTable file %s!!!\n",filenamestr);

	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_WARN, "Fail to open SP file %s\n",filenamestr);
 		info->spdef.number = 0;
		sleep(3);
	}
		  
	rewind(fp);	
	
	r = j = 0;

	for(;;)
	{
		if(j >= MAX_SP_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenSPTable: Num=%d above the MAX_SP_NUM %d\n",j,MAX_SP_NUM);
			info->spdef.number = MAX_SP_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				info->spdef.number = j;
				OutputDspInfo(OUTPUT_WARN, "file: %s END --- GenSPTable: point nNumber = %d\n",pDefFile,info->spdef.number);
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
		if(sscanf(filebuf,"%d %s %d %s",&iTemp,info->spdef.name[j],&info->spdef.asdutype[j],lgname)!=4)
		{
			OutputDspInfo(OUTPUT_ERROR, "Error! SP %3d line: %s\n",r,filebuf);
			j++;
			continue;
		}
		
		if(j != iTemp)
			OutputDspInfo(OUTPUT_WARN, "+++Warning! SP %d line: num %d != %d\n",r,iTemp,j);
		
		if(strcmp(info->spdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "SP %d pnt: %s\t%d\t%s\n",j,info->spdef.name[j],info->spdef.asdutype[j],lgname);
			strcpy(name_buf2,info->spdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				OutputDspInfo(OUTPUT_ERROR, "SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
		}
		else
			OutputDspInfo(OUTPUT_WARN, "SP %d pnt: NULL\n",j);

		j++;
	}

}

int GenDPTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile, int ip_no)		/* double point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[1128],lgname[128];
	int		iTemp;
	short	r, j;
	char	filenamestr[200];
	char	defFile[256], *ptr;
	char	filebuf[LINE_BUF_CHAR];

	strcpy(defFile, pDefFile);
	ptr= strstr(defFile, ".DEF");
	if(ptr)
	{
		*ptr= 0;
		sprintf(filenamestr,"%s%s_IEC104_%s%d.DEF",IEC_HOMEPATH,pName,defFile,ip_no+1);
		OutputDspInfo(OUTPUT_INFO, "\n\nRead DPTable file %s!!!\n",filenamestr);
	}
	else
	{
		OutputDspInfo(OUTPUT_ERROR, "\n\nDPTable filename error: %s\n",pDefFile);
		return 0;		
	}

 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(OUTPUT_ERROR, "####can't open DP file %s\n",filenamestr);
 		info->dpdef.number = 0;
		sleep(3);
	}
		  
	rewind(fp);	
	
	r = j = 0;

	for(;;)
	{
		if(j >= MAX_DP_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenDPTable: Num=%d above the MAX_DP_NUM %d\n",j,MAX_DP_NUM);
			info->dpdef.number = MAX_DP_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				info->dpdef.number = j;
				OutputDspInfo(OUTPUT_WARN, "file: %s END --- GenDPTable: point nNumber = %d\n",pDefFile,info->dpdef.number);
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
		if(sscanf(filebuf,"%d %s %d %s",&iTemp,info->dpdef.name[j],&info->dpdef.asdutype[j],lgname)!=4)
		{
			OutputDspInfo(OUTPUT_ERROR, "Error! DP %3d line: %s\n",r,filebuf);
			j++;
			continue;
		}
		
		if(j != iTemp)
			OutputDspInfo(OUTPUT_WARN, "+++Warning! DP %d line: num %d != %d\n",r,iTemp,j);
		
		if(strcmp(info->dpdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "DP %d pnt: %s\t%d\t%s\n",j,info->dpdef.name[j],info->dpdef.asdutype[j],lgname);
			strcpy(name_buf2,info->dpdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				OutputDspInfo(OUTPUT_ERROR, "DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
		}
		else
			OutputDspInfo(OUTPUT_WARN, "DP %d pnt: NULL\n",j);

		j++;
	}
}

int GenMETable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON		dbentry2;
	char			name_buf2[128],lgname[128];
	int		iTemp;
	short	r, j;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);

	OutputDspInfo(0, "\n\nRead METable file %s!!!\n",filenamestr);

	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(0, "####can't open ME file %s\n",filenamestr);
 		info->medef.number = 0;
		sleep(3);
	}
	rewind(fp);	
	
	r= j= 0;

	for(;;)
	{
		if(j >= MAX_ME_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenMETable: Num=%d above the MAX_ME_NUM %d\n",j,MAX_ME_NUM);
			
			info->medef.number = MAX_ME_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				info->medef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenMETable: point nNumber = %d\n",pDefFile,info->medef.number);
				
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
		if(sscanf(filebuf,"%d %s %f %f %f %f %d %d %s",&iTemp,info->medef.name[j],&info->medef.lorange[j],
			&info->medef.hirange[j],&info->medef.scale[j],&info->medef.databand[j],
			&info->medef.asdutype[j],&info->medef.timetag[j],lgname)!=9)
		{
			OutputDspInfo(OUTPUT_ERROR, "ME Error!%3d line: %s\n",r,filebuf);
			j ++;
			continue;
		}
		
		if(j != iTemp)
			OutputDspInfo(OUTPUT_WARN, "+++Warning! ME %d line: num %d != %d\n",r,iTemp,j);

		if(strcmp(info->medef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "ME %d %s %5.3f %5.3f %5.3f %5.3f %d %d %s\n",j,info->medef.name[j],info->medef.lorange[j],info->medef.hirange[j],
				info->medef.scale[j],info->medef.databand[j],info->medef.asdutype[j],info->medef.timetag[j],lgname);
			strcpy(name_buf2,info->medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				OutputDspInfo(OUTPUT_ERROR, "ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
		}
		else
			OutputDspInfo(OUTPUT_WARN, "ME %d pnt: NULL\n",j);

		j ++;
	}
}


int GenITTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* integrated total point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	/*int		data_type2;*/
	char		name_buf2[1128],lgname[128];

	int	iTemp,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	OutputDspInfo(0, "\n\nRead ITTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(0, "####can't open IT file %s\n",filenamestr);
 		info->itdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		OutputDspInfo(0, "\n#####IT File %s Define Error! IT Number = %d\n",filenamestr,iTemp);
		info->itdef.number = 0;
		fclose(fp);
		return(false);
	}
	if(iTemp >= MAX_IT_NUM)
	{
		OutputDspInfo(0, "\n!!!GenITTable: Num=%d above the MAX_IT_NUM %d\n",iTemp,MAX_IT_NUM);
		info->itdef.number = MAX_IT_NUM;
	}
	else
		info->itdef.number = iTemp;

	for(j=0;j<info->itdef.number;j++)
	{
		fscanf(fp,"%d %s %s",&iTemp,info->itdef.name[j],lgname);
		if(strcmp(info->itdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			OutputDspInfo(0, "IT %d pnt: %s %s\n",j,info->itdef.name[j],lgname);
#endif
			strcpy(name_buf2,info->itdef.name[j]);
/*			ReadEntryByNameStr(name_buf2,&data_type2,&dbentry2);
*/
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(0, "#### IT %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
/* #ifdef DEBUG */
	OutputDspInfo(0, "---%s GenITTable: point nNumber = %d\n\n",pName,info->itdef.number);
/* #endif */
	return(true);
}

int GenSCTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* single point control command define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
/*	int		data_type2;*/
	char		name_buf2[1128],lgname[128];

	int	iTemp,iTemp1,iTemp2,iTemp3,iTemp4,iRet,numb;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	OutputDspInfo(0, "\n\nRead SCTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(0, "####can't open SC file %s\n",filenamestr);
 		info->scdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		OutputDspInfo(0, "\n#####SC File %s Define Error! SC Number = %d\n",filenamestr,iTemp);
		info->scdef.number = 0;
		fclose(fp);
		return(false);
	}
	if(iTemp >= MAX_SC_NUM)
	{
		OutputDspInfo(0, "\n!!!GenSCTable: Num=%d above the MAX_SC_NUM %d\n",iTemp,MAX_SC_NUM);
		info->scdef.number = MAX_SC_NUM;
	}
	else
		info->scdef.number = iTemp;

	for(j=0;j<info->scdef.number;j++)
	{
		fscanf(fp,"%d %s %d %d %d %d %d %s",&numb,info->scdef.name[j],&iTemp,&iTemp1,&iTemp2,&iTemp3,&iTemp4,lgname);
		info->scdef.cmdtype[j] = iTemp;
		info->scdef.optype[j][0] = iTemp1;
		info->scdef.optype[j][1] = iTemp2;
		info->scdef.optype[j][2] = iTemp3;
		info->scdef.optype[j][3] = iTemp4;
		if(strcmp(info->scdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			OutputDspInfo(0, "SC %d pnt: %s %s\n",j,info->scdef.name[j],lgname);
#endif
			strcpy(name_buf2,info->scdef.name[j]);
/*			ReadEntryByNameStr(name_buf2,&data_type2,&dbentry2);
*/
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(0, "#### SC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
/* #ifdef DEBUG */
	OutputDspInfo(0, "---%s GenSCTable: point nNumber = %d\n\n",pName,info->scdef.number);
/* #endif */
	return(true);
}

int GenDCTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* double point control command define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[128],lgname[128];
	int		iTemp,iTemp1,iTemp2,iTemp3,iTemp4,numb;
	short	r, j;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);

	OutputDspInfo(0, "\n\nRead DCTable file %s!!!\n",filenamestr);

	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(0, "####can't open DC file %s\n",filenamestr);
 		info->dcdef.number = 0;
		sleep(3);
	}
		  
	rewind(fp);	
	
	r=j=0;
	
	OutputDspInfo(0, "\nRead DC define file %s!!!\n",filenamestr);
	
	for(;;)
	{
		if(j >= MAX_DC_NUM)
		{
			OutputDspInfo(OUTPUT_WARN, "\n!!!GenDCTable: Num=%d above the MAX_DC_NUM %d\n",j,MAX_DC_NUM);
			info->dcdef.number = MAX_DC_NUM;
			fclose(fp);
			return(FALSE);
		}
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				info->dcdef.number = j;
				OutputDspInfo(0, "file: %s END  ---  GenDCTable: point nNumber = %d\n",pDefFile,info->dcdef.number);
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
		if(sscanf(filebuf,"%d %s %d %d %d %d %d %s",&numb,info->dcdef.name[j],
			&iTemp,&iTemp1,&iTemp2,&iTemp3,&iTemp4,lgname)!=8)
		{
			OutputDspInfo(OUTPUT_ERROR, "Error! %3d line: %s\n",r,filebuf);
			j ++;
			continue;
		}
		info->dcdef.cmdtype[j] = iTemp;
		info->dcdef.optype[j][0] = iTemp1;
		info->dcdef.optype[j][1] = iTemp2;
		info->dcdef.optype[j][2] = iTemp3;
		info->dcdef.optype[j][3] = iTemp4;
		
		if(j!=numb)
		{
			OutputDspInfo(OUTPUT_WARN, "DC %d pnt number=%d wrong!!! %s %d %d %d %d %d %s\n",
				j, numb, info->dcdef.name[j],
				info->dcdef.cmdtype[j],info->dcdef.optype[j][0],info->dcdef.optype[j][1],
				info->dcdef.optype[j][2],info->dcdef.optype[j][3],lgname);
		}
		if(strcmp(info->dcdef.name[j],"NULL")!=0)
		{
			OutputDspInfo(0, "DC %d pnt: %s %d %d %d %d %d %s\n",
				j, info->dcdef.name[j],
				info->dcdef.cmdtype[j],
				info->dcdef.optype[j][0],info->dcdef.optype[j][1],
				info->dcdef.optype[j][2],info->dcdef.optype[j][3],lgname);
			
			strcpy(name_buf2,info->dcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
				OutputDspInfo(OUTPUT_ERROR, "DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			else
				info->dcdef.ptr[j]= dbentry2.point;

		}
		else
			OutputDspInfo(OUTPUT_WARN, "DC %d pnt: NULL\n",j);

		j ++;
	}
}

int GenSETable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* set point control point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
/*	int		data_type2;*/
	char		name_buf2[1128],lgname[128];

	int	iTemp,iTemp1,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	OutputDspInfo(0, "\n\nRead SETable file %s!!!\n",filenamestr);
#endif
	while((fp=fopen(filenamestr,"r"))==NULL)
	{
 		OutputDspInfo(0, "####can't open SE file %s\n",filenamestr);
 		info->sedef.number = 0;
		sleep(3);
/* 		return(false); */
	}
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		OutputDspInfo(0, "\n#####SE File %s Define Error! SE Number = %d\n",filenamestr,iTemp);
		info->sedef.number = 0;
		fclose(fp);
		return(false);
	}
	if(iTemp >= MAX_SE_NUM)
	{
		OutputDspInfo(0, "\n!!!GenSETable: Num=%d above the MAX_SE_NUM %d\n",iTemp,MAX_SE_NUM);
		info->sedef.number = MAX_SE_NUM;
	}
	else
		info->sedef.number = iTemp;

	for(j=0;j<info->sedef.number;j++)
	{
		fscanf(fp,"%d %s %d %f %f %s",&iTemp,info->sedef.name[j],&iTemp1,&info->sedef.lorange[j],&info->sedef.hirange[j],lgname);
/* 		fscanf(fp,"%s %f %s",info->sedef.name[j],&info->sedef.lorange[j],lgname); */
		info->sedef.cmdtype[j] = iTemp1;
		if(j!=iTemp)
		{
			OutputDspInfo(0, "SE %d pnt number=%d wrong!!!  %s %f %f %s\n",j,iTemp,info->sedef.name[j],info->sedef.lorange[j],info->sedef.hirange[j],lgname);
		}
		if(strcmp(info->sedef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			OutputDspInfo(0, "SE %d pnt: %s %f %f %s\n",j,info->sedef.name[j],info->sedef.lorange[j],info->sedef.hirange[j],lgname);
#endif
			strcpy(name_buf2,info->sedef.name[j]);
/*			ReadEntryByNameStr(name_buf2,&data_type2,&dbentry2);
*/
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(0, "#### SE %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
			info->sedef.ptr[j] = dbentry2.point;

		}
	}
/* #ifdef DEBUG */
	OutputDspInfo(0, "---%s GenSETable: point nNumber = %d\n\n",pName,info->sedef.number);
/* #endif */
	fclose(fp);
	return(true);
}

int GenRCTable(IEC104_CMM_INFO *info, char *pName, char *pDefFile)		/* step point control command define file */
{
	FILE			*fp;
	DMS_COMMON		dbentry2;
	/*int			data_type2;*/
	char			name_buf2[1128],lgname[128];

	int		iTemp,iRet;
	short	j;

	char	filenamestr[200];

	sprintf(filenamestr,"%s%s_IEC104_%s",IEC_HOMEPATH,pName,pDefFile);
#ifdef DEBUG
	OutputDspInfo(0, "\n\nRead RCTable file %s!!!\n",filenamestr);
#endif
 	while((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		OutputDspInfo(0, "####can't open RC file %s\n",filenamestr);
 		info->rcdef.number = 0;
		sleep(3);
/* 		return(false); */
	}
		  
	rewind(fp);	
	
	iRet = fscanf(fp,"%d",&iTemp);
	if(iRet<=0)
	{
		OutputDspInfo(0, "\n#####RC File %s Define Error! RC Number = %d\n",filenamestr,iTemp);
		info->rcdef.number = 0;
		fclose(fp);
		return(false);
	}
	if(iTemp >= MAX_RC_NUM)
	{
		OutputDspInfo(0, "\n!!!GenRCTable: Num=%d above the MAX_RC_NUM %d\n",iTemp,MAX_RC_NUM);
		info->rcdef.number = MAX_RC_NUM;
	}
	else
		info->rcdef.number = iTemp;

	for(j=0;j<info->rcdef.number;j++)
	{
		fscanf(fp,"%d %s %s",&iTemp,info->rcdef.name[j],lgname);
		if(strcmp(info->rcdef.name[j],"NULL")!=0)
		{
#ifdef DEBUG
			OutputDspInfo(0, "RC %d pnt: %s %s\n",j,info->rcdef.name[j],lgname);
#endif
			strcpy(name_buf2,info->rcdef.name[j]);
/*			ReadEntryByNameStr(name_buf2,&data_type2,&dbentry2);
*/
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				OutputDspInfo(0, "#### RC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
	}
	fclose(fp);
/* #ifdef DEBUG */
	OutputDspInfo(0, "---%s GenRCTable: point nNumber = %d\n\n",pName,info->rcdef.number);
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

}

BOOL GetCosBroadcastPrvg(char *stn_name, int ip_no);

int	IEC870_5_Recv(IEC104_CMM_INFO *info, COMM_INFO *comm_info ,char *pName,unsigned int recvdatapt)
{
	USHORT	infoaddr,datalength,lengthfactor;
	int	infoptr;
	short	ana16data;
	USHORT	ms,year;
	int 	i,datapoint;
	char	dbname[128];
	unsigned char	/*stnd,*/type_id,varstru,infonum,qoc,lanpt;
	unsigned short	asduaddr,causetra;

	SW_MSG		sw_buf[70];
	ALL_SW_MSG	all_sw_buf[70];
	unsigned char	me_buf[1440];
	LAN_HEAD	lanhead;
	IND_DATA	sw_status;
	float		value;
	int		count;

	DMS_COMMON	dbcommpt;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	ANLG_DATA	anadata;
	struct	tm	tmpt;
	time_t		tim;

	type_id = info->recv_data.data[recvdatapt][0];
	varstru = info->recv_data.data[recvdatapt][1];
	memcpy(&causetra, &info->recv_data.data[recvdatapt][2],2);
	memcpy(&asduaddr, &info->recv_data.data[recvdatapt][4],2);
	Swap2Byte(&causetra);
	Swap2Byte(&asduaddr);

	causetra = causetra & 0xFF;

	switch(type_id)
	{
	case M_SP_NA:	/* single point without time tag: 1 */
	case M_DP_NA:	/* double point without time tag: 3 */
		if(!GetCosBroadcastPrvg(comm_info->stn_name, comm_info->ip_no))
			break;

		if(varstru & 0x80)				/* SQ=1 */
		{
			infonum = varstru & 0x7F;
			datalength = infonum+9;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}
			tim=time(NULL);
			tmpt=*localtime(&tim);

			lanpt = 0;
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);
			Swap2Byte(&infoaddr);
			if(type_id ==M_SP_NA)
			{
				infoptr = infoaddr - info->sp_base_addr;
			}
			else
			{
				infoptr = infoaddr - info->dp_base_addr;
			}
			for(i=0;i<infonum;i++)
			{
				datapoint = i+9;

				if(type_id ==M_SP_NA)
				{
					if((infoptr < 0) || (infoptr >= info->spdef.number))
					{
						OutputDspInfo(0, "M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,info->spdef.number);
						break;
					}
					strcpy(dbname,info->spdef.name[infoptr]);
				}
				else
				{
					if((infoptr < 0) || (infoptr >= info->dpdef.number))
					{
						OutputDspInfo(0, "M_DP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,info->dpdef.number);
						break;
					}
					strcpy(dbname,info->dpdef.name[infoptr]);
				}
				infoptr ++;

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint];
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
				if(qoc & 0xD0)
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

					sw_buf[lanpt].sec=tmpt.tm_sec;
					sw_buf[lanpt].min=tmpt.tm_min;
					sw_buf[lanpt].hour=tmpt.tm_hour;
					sw_buf[lanpt].day=tmpt.tm_mday;
					sw_buf[lanpt].month=tmpt.tm_mon+1;
					sw_buf[lanpt].year=tmpt.tm_year+1900;

					Swap2Byte(&sw_buf[lanpt].number);
					Swap2Byte(&sw_buf[lanpt].point_id);
					Swap2Byte(&sw_buf[lanpt].status);
					Swap2Byte(&sw_buf[lanpt].ms);
					Swap2Byte(&sw_buf[lanpt].year);

					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(SW_MSG);
						Swap2Byte(&lanhead.length);
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
				{
					all_sw_buf[lanpt].type_id =DPS_STATUS_ALL;
					all_sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
					all_sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
					all_sw_buf[lanpt].data_type=dbcommpt.point.data_type;
					all_sw_buf[lanpt].number = 1;
					all_sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

					sw_status.alm_type = IND_ALM_TYPE_CHANGE;
					all_sw_buf[lanpt].status=sw_status;
					GetLocalHostId(&all_sw_buf[lanpt].host_id);

					all_sw_buf[lanpt].sec=tmpt.tm_sec;
					all_sw_buf[lanpt].min=tmpt.tm_min;
					all_sw_buf[lanpt].hour=tmpt.tm_hour;
					all_sw_buf[lanpt].day=tmpt.tm_mday;
					all_sw_buf[lanpt].month=tmpt.tm_mon+1;
					all_sw_buf[lanpt].year=tmpt.tm_year+1900;

					Swap2Byte(&all_sw_buf[lanpt].number);
					Swap2Byte(&all_sw_buf[lanpt].point_id);
					Swap2Byte(&all_sw_buf[lanpt].status);
					Swap2Byte(&all_sw_buf[lanpt].year);

					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(ALL_SW_MSG);
						Swap2Byte(&lanhead.length);
						lanhead.dp_type=ALL_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =all_sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&all_sw_buf,lanhead,(unsigned char)0);
						lanpt = 0;
					}
				}
			}
		}/* if(SQ) */
		else		/* SQ=0 */
		{
			infonum = varstru & 0x7F;
			datalength = infonum*4+6;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}
			tim=time(NULL);
			tmpt=*localtime(&tim);

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = i*4+6;
				memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
				Swap2Byte(&infoaddr);

				if(type_id ==M_SP_NA)
				{
					infoptr = infoaddr - info->sp_base_addr;
					if((infoptr < 0) || (infoptr >= info->spdef.number))
					{
						OutputDspInfo(0, "M_SP_NA SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,info->spdef.number);
						break;
					}
					strcpy(dbname,info->spdef.name[infoptr]);
				}
				else
				{
					infoptr = infoaddr - info->dp_base_addr;
					if((infoptr < 0) || (infoptr >= info->dpdef.number))
					{
						OutputDspInfo(0, "M_SP_NA SQ=1: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,info->dpdef.number);
						break;
					}
					strcpy(dbname,info->dpdef.name[infoptr]);
				}

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint+3];
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
				if(qoc & 0xD0)
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

					sw_buf[lanpt].sec=tmpt.tm_sec;
					sw_buf[lanpt].min=tmpt.tm_min;
					sw_buf[lanpt].hour=tmpt.tm_hour;
					sw_buf[lanpt].day=tmpt.tm_mday;
					sw_buf[lanpt].month=tmpt.tm_mon+1;
					sw_buf[lanpt].year=tmpt.tm_year+1900;

					Swap2Byte(&sw_buf[lanpt].number);
					Swap2Byte(&sw_buf[lanpt].point_id);
					Swap2Byte(&sw_buf[lanpt].status);
					Swap2Byte(&sw_buf[lanpt].ms);
					Swap2Byte(&sw_buf[lanpt].year);

					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(SW_MSG);
						Swap2Byte(&lanhead.length);
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
				{
					all_sw_buf[lanpt].type_id =DPS_STATUS_ALL;
					all_sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
					all_sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
					all_sw_buf[lanpt].data_type=dbcommpt.point.data_type;
					all_sw_buf[lanpt].number = 1;
					all_sw_buf[lanpt].point_id =dbcommpt.point.pt_id;

					sw_status.alm_type = IND_ALM_TYPE_CHANGE;
					all_sw_buf[lanpt].status=sw_status;
					GetLocalHostId(&all_sw_buf[lanpt].host_id);

					all_sw_buf[lanpt].sec=tmpt.tm_sec;
					all_sw_buf[lanpt].min=tmpt.tm_min;
					all_sw_buf[lanpt].hour=tmpt.tm_hour;
					all_sw_buf[lanpt].day=tmpt.tm_mday;
					all_sw_buf[lanpt].month=tmpt.tm_mon+1;
					all_sw_buf[lanpt].year=tmpt.tm_year+1900;

					Swap2Byte(&all_sw_buf[lanpt].number);
					Swap2Byte(&all_sw_buf[lanpt].point_id);
					Swap2Byte(&all_sw_buf[lanpt].status);
					Swap2Byte(&all_sw_buf[lanpt].year);

					lanpt ++;
					if( lanpt >= 70 )
					{
						lanhead.length = lanpt*sizeof(ALL_SW_MSG);
						Swap2Byte(&lanhead.length);
						lanhead.dp_type=ALL_DB_TIME;
						GetLocalHostId(&lanhead.src_id);
						lanhead.dest_id =0;
						lanhead.dest_stn =all_sw_buf[0].stn_id;
						lanhead.packet_num =0;
						lan_out ((char *)&all_sw_buf,lanhead,(unsigned char)0);
						lanpt = 0;
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
				Swap2Byte(&lanhead.length);
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
				Swap2Byte(&lanhead.length);
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.packet_num =0;
				lan_out ((char *)&all_sw_buf,lanhead,(unsigned char)0);
			}
		}		
		break;

	case M_SP_TA:	/* single point with time tag {data class 2 } */
	case M_DP_TA:	/* double point with time tag {data class 4 } */
		tim=time(NULL);
		tmpt=*localtime(&tim);
		infonum = varstru & 0x7F;
		datalength = 7*infonum+6;
		if(datalength != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
					type_id,infonum,info->recv_data.length[recvdatapt]);
			break;
		}

		lanpt = 0;
		for(i=0;i<infonum;i++)
		{
			datapoint = 7*i+6;
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
			Swap2Byte(&infoaddr);
			if(type_id ==M_SP_TA)
			{
				infoptr = infoaddr - info->sp_base_addr;
				if((infoptr < 0) || (infoptr > info->spdef.number))
				{
					OutputDspInfo(0, "M_SP_TA SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,info->spdef.number);
					break;
				}
				strcpy(dbname,info->spdef.name[infoptr]);
			}
			else
			{
				infoptr = infoaddr - info->dp_base_addr;
				if((infoptr < 0) || (infoptr > info->dpdef.number))
				{
					OutputDspInfo(0, "M_DP_TA SQ=0: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,info->dpdef.number);
					break;
				}
				strcpy(dbname,info->dpdef.name[infoptr]);
			}
			if(strcmp(dbname,"NULL")==0)
				continue;
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
				continue;
			}
			qoc = info->recv_data.data[recvdatapt][datapoint+3];
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
			if(qoc & 0xD0)
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
			memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+4],2);
			Swap2Byte(&ms);

			sw_buf[lanpt].ms = ms%1000;             
			sw_buf[lanpt].sec=(char)(ms/1000);
			sw_buf[lanpt].min=info->recv_data.data[recvdatapt][datapoint+6] & 0x3F;
			sw_buf[lanpt].hour=tmpt.tm_hour;
			sw_buf[lanpt].day=tmpt.tm_mday;
			sw_buf[lanpt].month=tmpt.tm_mon+1;
			sw_buf[lanpt].year=tmpt.tm_year+1900;

			
			if(!comm_info->ctrl_priv)
			{
				OutputDspInfo(0, "insert to soebuf: %d.%d.%d.%d", dbcommpt.point.stn_id,
					dbcommpt.point.dev_id, dbcommpt.point.data_type, dbcommpt.point.pt_id);
				InsertSoeRecBuff(info, &sw_buf[lanpt], tim);
			}
			else
			{
				OutputDspInfo(0, "broadcast soe: %d.%d.%d.%d", dbcommpt.point.stn_id,
					dbcommpt.point.dev_id, dbcommpt.point.data_type, dbcommpt.point.pt_id);
				Swap2Byte(&sw_buf[lanpt].number);
				Swap2Byte(&sw_buf[lanpt].point_id);
				Swap2Byte(&sw_buf[lanpt].status);
				Swap2Byte(&sw_buf[lanpt].ms);
				Swap2Byte(&sw_buf[lanpt].year);
				
				lanpt ++;
				if( lanpt>=70 )
				{
					lanhead.length = lanpt*sizeof(SW_MSG);
					Swap2Byte(&lanhead.length);
					lanhead.dp_type=BJ_DB_TIME;
					GetLocalHostId(&lanhead.src_id);
					lanhead.dest_id =0;
					lanhead.dest_stn =sw_buf[0].stn_id;
					lanhead.packet_num =0;
					lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
					lanpt = 0;
				}

			}
		}
		if( lanpt>0 )
		{
			lanhead.length = lanpt*sizeof(SW_MSG);
			Swap2Byte(&lanhead.length);
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
		tim=time(NULL);
		tmpt=*localtime(&tim);
		infonum = varstru & 0x7F;
		datalength = 11*infonum+6;
		if(datalength != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
					type_id,infonum,info->recv_data.length[recvdatapt]);
			break;
		}

		lanpt = 0;
		for(i=0;i<infonum;i++)
		{
			datapoint = 11*i+6;
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
			Swap2Byte(&infoaddr);
			if(type_id ==M_SP_TB)
			{
				infoptr = infoaddr - info->sp_base_addr;
				if((infoptr < 0) || (infoptr > info->spdef.number))
				{
					OutputDspInfo(0, "M_SP_TB SQ=0: infoaddr=%x  infoptr=%d > sswnum=%d\n",infoaddr,infoptr,info->spdef.number);
					break;
				}
				strcpy(dbname,info->spdef.name[infoptr]);
			}
			else
			{
				infoptr = infoaddr - info->dp_base_addr;
				if((infoptr < 0) || (infoptr > info->dpdef.number))
				{
					OutputDspInfo(0, "M_DP_TB SQ=0: infoaddr=%x  infoptr=%d > dswnum=%d\n",infoaddr,infoptr,info->dpdef.number);
					break;
				}
				strcpy(dbname,info->dpdef.name[infoptr]);
			}
			if(strcmp(dbname,"NULL")==0)
				continue;

			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
				continue;
			}
			qoc = info->recv_data.data[recvdatapt][datapoint+3];
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
			sw_status.alm_type = IND_ALM_TYPE_CHANGE;
			
			if(qoc & 0x20)	  			/* SIQ:= IV NT SB BL 0 0 0 SPI */
			{	
				sw_status.man_set = 1;	/* manual set SB*/
				sw_status.alm_type = IND_ALM_TYPE_CHANFAIL;
			}

			else
				sw_status.man_set = 0;

			if(qoc & 0xD0)
				sw_status.chan_fail = 1;	/* channel fault BL*/
			else
				sw_status.chan_fail = 0;
		

			sw_status.fault_tag = 0;

			sw_buf[lanpt].type_id =DPS_IND_ALM;
			sw_buf[lanpt].stn_id =dbcommpt.point.stn_id;
			sw_buf[lanpt].dev_id =dbcommpt.point.dev_id;
			sw_buf[lanpt].data_type=dbcommpt.point.data_type;
			sw_buf[lanpt].number = 1;
			sw_buf[lanpt].point_id =dbcommpt.point.pt_id;
			
			sw_buf[lanpt].status=sw_status;
			GetLocalHostId(&sw_buf[lanpt].host_id);
			memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+4],2);
			Swap2Byte(&ms);
			sw_buf[lanpt].ms = ms%1000;
			sw_buf[lanpt].sec=(char)(ms/1000);
			sw_buf[lanpt].min=info->recv_data.data[recvdatapt][datapoint+6] & 0x3F;
			sw_buf[lanpt].hour=info->recv_data.data[recvdatapt][datapoint+7] & 0x1F;
			sw_buf[lanpt].day=info->recv_data.data[recvdatapt][datapoint+8] & 0x1F;
			sw_buf[lanpt].month=info->recv_data.data[recvdatapt][datapoint+9] & 0x0F;
			sw_buf[lanpt].year=(info->recv_data.data[recvdatapt][datapoint+10] & 0x7F)+2000;

			if(!comm_info->ctrl_priv)
			{
				OutputDspInfo(0, "insert to soebuf: %d.%d.%d.%d", dbcommpt.point.stn_id,
					dbcommpt.point.dev_id, dbcommpt.point.data_type, dbcommpt.point.pt_id);
				InsertSoeRecBuff(info, &sw_buf[lanpt], tim);
			}
			else
			{
				OutputDspInfo(0, "broadcast soe: %d.%d.%d.%d", dbcommpt.point.stn_id,
					dbcommpt.point.dev_id, dbcommpt.point.data_type, dbcommpt.point.pt_id);
				Swap2Byte(&sw_buf[lanpt].number);
				Swap2Byte(&sw_buf[lanpt].point_id);
				Swap2Byte(&sw_buf[lanpt].status);
				Swap2Byte(&sw_buf[lanpt].ms);
				Swap2Byte(&sw_buf[lanpt].year);
				
				lanpt ++;
				if( lanpt>=70 )
				{
					lanhead.length = lanpt*sizeof(SW_MSG);
					Swap2Byte(&lanhead.length);
					lanhead.dp_type=BJ_DB_TIME;
					GetLocalHostId(&lanhead.src_id);
					lanhead.dest_id =0;
					lanhead.dest_stn =sw_buf[0].stn_id;
					lanhead.packet_num =0;
					lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
					lanpt = 0;
				}
			}
		}
		if( lanpt>0 )
		{
			lanhead.length = lanpt*sizeof(SW_MSG);
			Swap2Byte(&lanhead.length);
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
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");

		tim=time(NULL);
		tmpt=*localtime(&tim);
		infonum = varstru & 0x7F;			/* number of information object */
		if(varstru & 0x80)	/* SQ=1 */
		{
			if(type_id == M_ME_NA)
				lengthfactor = 3;
			else if(type_id == M_ME_TA)
				lengthfactor = 6;
			else
				lengthfactor = 10;
			datalength = lengthfactor*infonum+9;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
#ifdef SUN_OS
			CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif

			infoptr = infoaddr - info->me_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr < 0) || (infoptr >= info->medef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_ME_NA SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,info->medef.number);
#endif			
					break;
				}
				strcpy(dbname,info->medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint+2];

#ifdef SUN_OS
				CrossCopy(&ana16data,&info->recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&ana16data,&info->recv_data.data[recvdatapt][datapoint],2);
#endif
				if(info->MeType == 0)
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/32767.0
						 + info->medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/65535.0
						 + (info->medef.hirange[infoptr]+info->medef.lorange[infoptr])/2.0);
				}

				infoptr ++;
				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
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
#ifdef SUN_OS
					CrossCopy(&ms,&info->recv_data.data[recvdatapt][datapoint+3],2);
#else
					memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+3],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=info->recv_data.data[recvdatapt][datapoint+5] & 0x3F;	/* minute */
					if(type_id ==M_ME_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TD */
					{
						me_buf[lanpt*22+17]=info->recv_data.data[recvdatapt][datapoint+6] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=info->recv_data.data[recvdatapt][datapoint+7] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=info->recv_data.data[recvdatapt][datapoint+8] & 0x0F;	/* month */
						year=(info->recv_data.data[recvdatapt][datapoint+9] & 0x7F)+2000;		/* year */
					}
				}
#ifdef SUN_OS
				CrossCopy(&me_buf[lanpt*22+20],&year,2);
#else
				memcpy(&me_buf[lanpt*22+20],&year,2);
#endif
/*
				if(dbcommpt.entry.anlg.variable.ptr.data_type == IMP_TYPE)
				{
					dbcommpt.entry.imp.variable.rt_value = value;
					if (WriteEntryByID (&dbcommpt, RW_VAR_RT ) == -1)
						printf ( "\nM_ME_NA: %s No this IMP point in the database when Wdb!\n",dbname);
				}
*/
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
			if(type_id == M_ME_NA)
			{
/*
				tim=time(NULL);
				tmpt=*localtime(&tim);
*/
				lengthfactor = 6;
			}
			else if(type_id == M_ME_TA)
				lengthfactor = 9;
			else
				lengthfactor = 13;
			datalength = lengthfactor*infonum+6;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;
#ifdef SUN_OS
				CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
				infoptr = infoaddr - info->me_base_addr;
				if((infoptr < 0) || (infoptr > info->medef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_ME_NA SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,info->medef.number);
#endif
					break;
				}
				strcpy(dbname,info->medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint+5];

#ifdef SUN_OS
				CrossCopy(&ana16data,&info->recv_data.data[recvdatapt][datapoint+3],2);
#else
				memcpy(&ana16data,&info->recv_data.data[recvdatapt][datapoint+3],2);
#endif

				if(info->MeType == 0)
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/32767.0
						 + info->medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/65535.0
						 + (info->medef.hirange[infoptr]+info->medef.lorange[infoptr])/2.0);
				}

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
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
#ifdef DEBUG
		OutputDspInfo(0, "\n%s::Receive=%f \nHI_ALARM=%f HI_WARN=%f HI_RECOVER=%f \nLO_ALARM=%f LO_WARN=%f LO_RECOVER=%f\n",dbname,value,
					dbcommpt.var.anlg.lmt_para.HI_ALARM,dbcommpt.var.anlg.lmt_para.HI_WARN,dbcommpt.var.anlg.lmt_para.HI_RECOVER,
					dbcommpt.var.anlg.lmt_para.LO_ALARM,dbcommpt.var.anlg.lmt_para.LO_WARN,dbcommpt.var.anlg.lmt_para.LO_RECOVER);
#endif
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)>dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value<dbcommpt.var.anlg.lmt_para.HI_RECOVER)
					&&(value>dbcommpt.var.anlg.lmt_para.LO_RECOVER))
				{
				     OutputDspInfo(0, "复上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_RECOVER;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;

				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.LO_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_RECOVER)
					&&(value<dbcommpt.var.anlg.lmt_para.HI_RECOVER))
				{
					OutputDspInfo(0, "复下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_RECOVER;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>dbcommpt.var.anlg.lmt_para.HI_ALARM))
				{
					OutputDspInfo(0, "越上上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_ALARM;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&
					(value>dbcommpt.var.anlg.lmt_para.HI_WARN))
				{
					OutputDspInfo(0, "越上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_WARN;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if((dbcommpt.var.anlg.value.data>dbcommpt.var.anlg.lmt_para.LO_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&
				(value<dbcommpt.var.anlg.lmt_para.LO_WARN))
				{
					OutputDspInfo(0, "越下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_WARN;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<dbcommpt.var.anlg.lmt_para.LO_ALARM))
				{
					OutputDspInfo(0, "越下下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_ALARM;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
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
#ifdef SUN_OS
					CrossCopy(&ms,&info->recv_data.data[recvdatapt][datapoint+6],2);
#else
					memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+6],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=info->recv_data.data[recvdatapt][datapoint+8] & 0x3F;	/* minute */
					if(type_id ==M_ME_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TD */
					{
						me_buf[lanpt*22+17]=info->recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=info->recv_data.data[recvdatapt][datapoint+10] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=info->recv_data.data[recvdatapt][datapoint+11] & 0x0F;	/* month */
						year=(info->recv_data.data[recvdatapt][datapoint+12] & 0x7F)+2000;		/* year */
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
		break;

	case M_ME_NC:	/* measure value without time tag:13 */
	case M_ME_TC:	/* measure value with time tag:14 */
	case M_ME_TF:	/* 36:measure value with time CP56Time2a,float value*/
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");

		tim=time(NULL);
		tmpt=*localtime(&tim);
		infonum = varstru & 0x7F;			/* number of information object */
		if(varstru & 0x80)	/* SQ=1 */
		{
			if(type_id == M_ME_NC)
				lengthfactor = 5;
			else if(type_id == M_ME_TC)
				lengthfactor = 8;
			else
				lengthfactor = 12;
			datalength = lengthfactor*infonum+9;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
#ifdef SUN_OS
			CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			infoptr = infoaddr - info->me_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr < 0) || (infoptr >= info->medef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_ME_NC SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,info->medef.number);
#endif			
					break;
				}
				strcpy(dbname,info->medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint+4];

#ifdef SUN_OS
				CrossCopy(&value,&info->recv_data.data[recvdatapt][datapoint],4);
#else
				memcpy(&value,&info->recv_data.data[recvdatapt][datapoint],4);
#endif
/*
				value = *((float *)&info->recv_data.data[recvdatapt][datapoint]);
				memcpy(tmp4,&info->recv_data.data[recvdatapt][datapoint],4);
				value = *(float*)tmp4;
*/
				infoptr ++;

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
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
					CrossCopy(&ms,&info->recv_data.data[recvdatapt][datapoint+5],2);
#else
					memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+5],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=info->recv_data.data[recvdatapt][datapoint+7] & 0x3F;	/* minute */
					if(type_id ==M_ME_TC)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TF */
					{
						me_buf[lanpt*22+17]=info->recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=info->recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=info->recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
						year=(info->recv_data.data[recvdatapt][datapoint+11] & 0x7F)+2000;		/* year */
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
				lengthfactor = 8;
			}
			else if(type_id == M_ME_TC)
				lengthfactor = 11;
			else
				lengthfactor = 15;
			datalength = lengthfactor*infonum+6;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;
#ifdef SUN_OS
				CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
				infoptr = infoaddr - info->me_base_addr;
				if((infoptr < 0) || (infoptr > info->medef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_ME_NC SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,info->medef.number);
#endif
					break;
				}
				strcpy(dbname,info->medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint+7];
#ifdef SUN_OS
				CrossCopy(&value,&info->recv_data.data[recvdatapt][datapoint+3],4);
#else
				memcpy(&value,&info->recv_data.data[recvdatapt][datapoint+3],4);
#endif
				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
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
#ifdef DEBUG
		OutputDspInfo(0, "\n%s::Receive=%f \nHI_ALARM=%f HI_WARN=%f HI_RECOVER=%f \nLO_ALARM=%f LO_WARN=%f LO_RECOVER=%f\n",dbname,value,
					dbcommpt.var.anlg.lmt_para.HI_ALARM,dbcommpt.var.anlg.lmt_para.HI_WARN,dbcommpt.var.anlg.lmt_para.HI_RECOVER,
					dbcommpt.var.anlg.lmt_para.LO_ALARM,dbcommpt.var.anlg.lmt_para.LO_WARN,dbcommpt.var.anlg.lmt_para.LO_RECOVER);
#endif
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)>dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value<dbcommpt.var.anlg.lmt_para.HI_RECOVER)
					&&(value>dbcommpt.var.anlg.lmt_para.LO_RECOVER))
				{
				     OutputDspInfo(0, "复上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_RECOVER;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;

				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.LO_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_RECOVER)
					&&(value<dbcommpt.var.anlg.lmt_para.HI_RECOVER))
				{
					OutputDspInfo(0, "复下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_RECOVER;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>dbcommpt.var.anlg.lmt_para.HI_ALARM))
				{
					OutputDspInfo(0, "越上上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_ALARM;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&
					(value>dbcommpt.var.anlg.lmt_para.HI_WARN))
				{
					OutputDspInfo(0, "越上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_WARN;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if((dbcommpt.var.anlg.value.data>dbcommpt.var.anlg.lmt_para.LO_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&
				(value<dbcommpt.var.anlg.lmt_para.LO_WARN))
				{
					OutputDspInfo(0, "越下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_WARN;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<dbcommpt.var.anlg.lmt_para.LO_ALARM))
				{
					OutputDspInfo(0, "越下下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_ALARM;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
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
					CrossCopy(&ms,&info->recv_data.data[recvdatapt][datapoint+8],2);
#else
					memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+8],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=info->recv_data.data[recvdatapt][datapoint+10] & 0x3F;	/* minute */
					if(type_id ==M_ME_TC)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_ME_TF */
					{
						me_buf[lanpt*22+17]=info->recv_data.data[recvdatapt][datapoint+11] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=info->recv_data.data[recvdatapt][datapoint+12] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=info->recv_data.data[recvdatapt][datapoint+13] & 0x0F;	/* month */
						year=(info->recv_data.data[recvdatapt][datapoint+14] & 0x7F)+2000;		/* year */
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
		break;

	case M_ME_ND:	/* measure value without quality:21 */
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");

		infonum = varstru & 0x7F;			/* number of information object */

		tim=time(NULL);
		tmpt=*localtime(&tim);

		if(varstru & 0x80)	/* SQ=1 */
		{
			lengthfactor = 2;
			datalength = lengthfactor*infonum+9;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}
			lanpt = 0;
#ifdef SUN_OS
			CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			infoptr = infoaddr - info->me_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;
				if((infoptr < 0) || (infoptr >= info->medef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_ME_ND SQ=1: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,info->medef.number);
#endif			
					break;
				}

				strcpy(dbname,info->medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
#ifdef SUN_OS
				CrossCopy(&ana16data,&info->recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&ana16data,&info->recv_data.data[recvdatapt][datapoint],2);
#endif
				if(info->MeType == 0)
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/32767.0
						 + info->medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/65535.0
						 + (info->medef.hirange[infoptr]+info->medef.lorange[infoptr])/2.0);
				}

				infoptr ++;

				*(USHORT *)&anadata.status = 0;

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

				me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
				me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
				me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
				me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
				me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
				year=tmpt.tm_year+1900;	/* year */
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
		else	/* SQ=0 */
		{
			lengthfactor = 5;
			datalength = lengthfactor*infonum+6;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;
#ifdef SUN_OS
				CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
				infoptr = infoaddr - info->me_base_addr;
#ifdef DEBUG
				OutputDspInfo(0, "M_ME_ND SQ=0: infoaddr=%x  infoptr=%d infonum=%d\n",infoaddr,infoptr,infonum);
#endif
				if((infoptr < 0) || (infoptr > info->medef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_ME_ND SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,info->medef.number);
#endif			
					break;
				}

				strcpy(dbname,info->medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
#ifdef SUN_OS
				CrossCopy(&ana16data,&info->recv_data.data[recvdatapt][datapoint+3],2);
#else
				memcpy(&ana16data,&info->recv_data.data[recvdatapt][datapoint+3],2);
#endif
				if(info->MeType == 0)
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/32767.0
						 + info->medef.lorange[infoptr]);
				}
				else
				{
					value=(float)((ana16data*(info->medef.hirange[infoptr]-info->medef.lorange[infoptr]))/65535.0
						 + (info->medef.hirange[infoptr]+info->medef.lorange[infoptr])/2.0);
				}

				*(USHORT *)&anadata.status = 0;

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
		OutputDspInfo(0, "\n%s::Receive=%f \nHI_ALARM=%f HI_WARN=%f HI_RECOVER=%f \nLO_ALARM=%f LO_WARN=%f LO_RECOVER=%f\n",dbname,value,
					dbcommpt.var.anlg.lmt_para.HI_ALARM,dbcommpt.var.anlg.lmt_para.HI_WARN,dbcommpt.var.anlg.lmt_para.HI_RECOVER,
					dbcommpt.var.anlg.lmt_para.LO_ALARM,dbcommpt.var.anlg.lmt_para.LO_WARN,dbcommpt.var.anlg.lmt_para.LO_RECOVER);
#endif
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)>dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value<dbcommpt.var.anlg.lmt_para.HI_RECOVER)
					&&(value>dbcommpt.var.anlg.lmt_para.LO_RECOVER))
				{
				     OutputDspInfo(0, "复上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_RECOVER;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;

				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.LO_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_RECOVER)
					&&(value<dbcommpt.var.anlg.lmt_para.HI_RECOVER))
				{
					OutputDspInfo(0, "复下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_RECOVER;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&(value>dbcommpt.var.anlg.lmt_para.HI_ALARM))
				{
					OutputDspInfo(0, "越上上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_ALARM;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)<dbcommpt.var.anlg.lmt_para.HI_WARN)&&(value<dbcommpt.var.anlg.lmt_para.HI_ALARM)&&
					(value>dbcommpt.var.anlg.lmt_para.HI_WARN))
				{
					OutputDspInfo(0, "越上限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_HI_WARN;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if((dbcommpt.var.anlg.value.data>dbcommpt.var.anlg.lmt_para.LO_WARN)&&(value>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&
				(value<dbcommpt.var.anlg.lmt_para.LO_WARN))
				{
					OutputDspInfo(0, "越下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_WARN;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
				if(((float)(dbcommpt.var.anlg.value.data/1000.0)>dbcommpt.var.anlg.lmt_para.LO_ALARM)&&(value<dbcommpt.var.anlg.lmt_para.LO_ALARM))
				{
					OutputDspInfo(0, "越下下限\n");
					me_buf[lanpt*22] =DPS_ANA_ALM;
					anadata.status.lmt_state=ANA_ALM_LO_ALARM;
					anadata.status.alm_type=ANA_ALM_TYPE_LMT;
				}
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

				me_buf[lanpt*22+15]=tmpt.tm_sec;	/* second */
				me_buf[lanpt*22+16]=tmpt.tm_min;	/* minute */
				me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
				me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
				me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
				year=tmpt.tm_year+1900;	/* year */
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
		break;

	case M_ST_NA:	/* step point without time tag:5 */
	case M_ST_TA:	/* step point with time tag:6 */
	case M_ST_TB:	/* step point with time tag CP56Time2a:32 */
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");

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
		if(datalength != info->recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
		{
			OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
					type_id,infonum,info->recv_data.length[recvdatapt]);
			break;
		}

		lanpt = 0;
		for(i=0;i<infonum;i++)
		{
			datapoint = lengthfactor*i+6;
#ifdef SUN_OS
			CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
#else
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
			infoptr = infoaddr - info->st_base_addr;
			if((infoptr < 0) || (infoptr > info->stdef.number))
			{
#ifdef DEBUG
				OutputDspInfo(0, "M_ST_NA SQ=0: infoaddr=%x  infoptr=%d > stnum=%d\n",infoaddr,infoptr,info->stdef.number);
#endif
				break;
			}
			strcpy(dbname,info->stdef.name[infoptr]);

			if(strcmp(dbname,"NULL")==0)
				continue;
			if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
			{
				OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
				continue;
			}
			qoc = info->recv_data.data[recvdatapt][datapoint+4];

			anadata.data = (info->recv_data.data[recvdatapt][datapoint+3] & 0x3F)*1000;

			if(info->recv_data.data[recvdatapt][datapoint+3] & 0x40)	/* VTI = T VALUE:=I7[1..7]<-64..63> */
				anadata.data = -anadata.data;	/* negative */

			if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
				anadata.status.ctrl_block = 1;	/* manual set */
			if(qoc & 0xD0)
				anadata.status.chan_fail = 1;	/* channel fault */

			if(causetra == SPONT)
				me_buf[lanpt*22] =DPS_ANALOG;	/* type_id */
			else
				me_buf[lanpt*22] =DPS_ANA_ALL;	/* type_id */

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
#ifdef SUN_OS
				CrossCopy(&ms,&info->recv_data.data[recvdatapt][datapoint+5],2);
#else
				memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+5],2);
#endif
				me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
				me_buf[lanpt*22+16]=info->recv_data.data[recvdatapt][datapoint+7] & 0x3F;	/* minute */
				if(type_id ==M_ST_TA)
				{
					me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
					me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
					me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
					year=tmpt.tm_year+1900;		/* year */
				}
				else		/* M_ST_TB */
				{
					me_buf[lanpt*22+17]=info->recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
					me_buf[lanpt*22+18]=info->recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
					me_buf[lanpt*22+19]=info->recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
					year=(info->recv_data.data[recvdatapt][datapoint+11] & 0x7F)+2000;		/* year */
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
		break;

	case M_IT_NA:	/* integrated totals without time tag:15 */
	case M_IT_TA:	/* integrated totals with time tag:16 */
	case M_IT_TB:	/* integrated totals with time tag CP56Time2a:37 */
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");

		tim=time(NULL);
		tmpt=*localtime(&tim);

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
			if(datalength != info->recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
#ifdef SUN_OS
			CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);
#else
			memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
#endif
			infoptr = infoaddr - info->it_base_addr;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+9;

				if((infoptr < 0) || (infoptr >= info->itdef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_IT_NA SQ=1: infoaddr=%x  infoptr=%d > itnum=%d\n",infoaddr,infoptr,info->itdef.number);
#endif			
					break;
				}
				strcpy(dbname,info->medef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
				{
					infoptr ++;
					continue;
				}
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					infoptr ++;
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint+4];

#ifdef SUN_OS
				CrossCopy(&count,&info->recv_data.data[recvdatapt][datapoint],4);
#else
				memcpy(&count,&info->recv_data.data[recvdatapt][datapoint],4);
#endif
				infoptr ++;

				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				if(qoc & 0xD0)
					anadata.status.chan_fail = 1;	/* channel fault */

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
#ifdef SUN_OS
					CrossCopy(&ms,&info->recv_data.data[recvdatapt][datapoint+5],2);
#else
					memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+5],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=info->recv_data.data[recvdatapt][datapoint+7] & 0x3F;	/* minute */
					if(type_id ==M_IT_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_IT_TB */
					{
						me_buf[lanpt*22+17]=info->recv_data.data[recvdatapt][datapoint+8] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=info->recv_data.data[recvdatapt][datapoint+9] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=info->recv_data.data[recvdatapt][datapoint+10] & 0x0F;	/* month */
						year=(info->recv_data.data[recvdatapt][datapoint+11] & 0x7F)+2000;		/* year */
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
			if(type_id == M_IT_NA)
			{
				lengthfactor = 8;
			}
			else if(type_id == M_IT_TA)
				lengthfactor = 11;
			else
				lengthfactor = 15;
			datalength = lengthfactor*infonum+6;
			if(datalength != info->recv_data.length[recvdatapt])
			{
				OutputDspInfo(0, "received ASDU length error! type_id=%d,infonum=%u,length=%u\n",
						type_id,infonum,info->recv_data.length[recvdatapt]);
				break;
			}

			lanpt = 0;
			for(i=0;i<infonum;i++)
			{
				datapoint = lengthfactor*i+6;
#ifdef SUN_OS
				CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);
#else
				memcpy(&infoaddr,&info->recv_data.data[recvdatapt][datapoint],2);	 /* address of object */
#endif
				infoptr = infoaddr - info->it_base_addr;
				if((infoptr < 0) || (infoptr > info->itdef.number))
				{
#ifdef DEBUG
					OutputDspInfo(0, "M_IT_NA SQ=0: infoaddr=%x  infoptr=%d > ananum=%d\n",infoaddr,infoptr,info->itdef.number);
#endif
					break;
				}
				strcpy(dbname,info->itdef.name[infoptr]);

				if(strcmp(dbname,"NULL")==0)
					continue;
				if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
				{
					OutputDspInfo(0, "\nIEC No this point name=%s in db\n",dbname);
					continue;
				}
				qoc = info->recv_data.data[recvdatapt][datapoint+7];
#ifdef SUN_OS
				CrossCopy(&count,&info->recv_data.data[recvdatapt][datapoint+3],4);
#else
				memcpy(&count,&info->recv_data.data[recvdatapt][datapoint+3],4);
#endif
				*(USHORT *)&anadata.status = 0;
				if(qoc & 0x20)			/* QDS:= IV NT SB BL 0 0 0 OV */
					anadata.status.ctrl_block = 1;	/* manual set */
				if(qoc & 0xD0)
					anadata.status.chan_fail = 1;	/* channel fault */

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
#ifdef SUN_OS
					CrossCopy(&ms,&info->recv_data.data[recvdatapt][datapoint+8],2);
#else
					memcpy(&ms,&info->recv_data.data[recvdatapt][datapoint+8],2);
#endif
					me_buf[lanpt*22+15] = (char)(ms/1000);	/* second */
					me_buf[lanpt*22+16]=info->recv_data.data[recvdatapt][datapoint+10] & 0x3F;	/* minute */
					if(type_id ==M_IT_TA)
					{
						me_buf[lanpt*22+17]=tmpt.tm_hour;	/* hour */
						me_buf[lanpt*22+18]=tmpt.tm_mday;	/* day */
						me_buf[lanpt*22+19]=tmpt.tm_mon+1;	/* month */
						year=tmpt.tm_year+1900;		/* year */
					}
					else		/* M_IT_TB */
					{
						me_buf[lanpt*22+17]=info->recv_data.data[recvdatapt][datapoint+11] & 0x1F;	/* hour */
						me_buf[lanpt*22+18]=info->recv_data.data[recvdatapt][datapoint+12] & 0x1F;	/* day */
						me_buf[lanpt*22+19]=info->recv_data.data[recvdatapt][datapoint+13] & 0x0F;	/* month */
						year=(info->recv_data.data[recvdatapt][datapoint+14] & 0x7F)+2000;		/* year */
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
		break;

	case C_SC_NA:	/* confirm of single point control command */
	case C_DC_NA:	/* confirm of double point control command */
	case C_RC_NA:	/* confirm of regulating step command */
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");

		memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
		Swap2Byte(&infoaddr);

		qoc = info->recv_data.data[recvdatapt][9];

		if(10 != info->recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
		{
			OutputDspInfo(0, "%d C_DC_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			info->cmd_ack = -1;		/* negative confirm */
			info->comdflag = 0;
			info->cmdbuf.ack_flag = 0;
			info->cmdbuf.selected = 0;
			info->cmdbuf.term_flag = 0;
			info->cmdbuf.sel_tim = 0;
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
				info->comdflag = 0;
/* 				info->cmdbuf.infoaddr = pntnum+info->dc_cmd_base_addr; */
				info->cmdbuf.ack_flag = 0;
				info->cmdbuf.selected = 0;
				info->cmdbuf.term_flag = 0;
				info->cmdbuf.sel_tim = 0;
#ifdef DEBUG
				OutputDspInfo(0, "IEC SC/DC/RC %u command is NACK INF=%d com=%x\n",type_id,infoaddr,qoc);
#endif
				/*BdSms(COM_CTRL_FAIL);*/
				BroadCastSms(COM_CTRL_FAIL,true);
				break;
			}
			
			if(qoc & 0x80)
			{
#ifdef DEBUG
				OutputDspInfo(0, "IEC SC/DC/RC %u select command ACK INF=%d com=%x\n",type_id,infoaddr,qoc);
#endif
				info->cmdbuf.ack_flag = 0;
				info->cmd_ack = 2;		/* select command ACK */
				info->comdflag = 2;
				if((infoaddr == info->cmdbuf.infoaddr) && (info->cmdbuf.selected == 1))
				{
					tim=time(NULL);
					if((tim - info->cmdbuf.sel_tim)<info->cmd_period)
					{
						if((info->Cmd_Type == 0)&&(info->cmdbuf.tpyeId == C_SC_NA))
						{
							if(!SinglePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else if((info->Cmd_Type == 1)&&(info->cmdbuf.tpyeId == C_DC_NA))
						{
							if(!DoublePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						info->cmdbuf.ack_flag = 1;
						info->comdflag = 2;
					}
					else
					{
						OutputDspInfo(0, "IEC SC/DC/RC %u select command ACK time out INF=%d com=%x \n",type_id,infoaddr,qoc);
						BroadCastSms(COM_CTRL_TIMEOUT,true);
						info->cmd_ack = -1;		/* negative confirm */
						info->comdflag = 0;
						info->cmdbuf.ack_flag = 0;
						info->cmdbuf.sel_tim = 0;
					}
				}
				else
				{
					info->comdflag = 0;
					info->cmd_ack = -1;
					info->cmdbuf.ack_flag = 0;
					info->cmdbuf.sel_tim = 0;
					OutputDspInfo(0, "IEC SC/DC/RC %u select command ACK INF=%d or select flag wrong! com=%x \n",type_id,infoaddr,qoc);
					BroadCastSms(COM_CTRL_FAIL,true);
				}
				info->cmdbuf.selected = 0;
				info->cmdbuf.term_flag = 0;
			}
			else
			{
				info->cmd_ack = 1;		/* execute command ACK */
				info->cmdbuf.ack_flag = 0;
				info->cmdbuf.selected = 0;
				info->cmdbuf.term_flag = 1;
#ifdef DEBUG
				OutputDspInfo(0, "IEC SC/DC/RC %u execute command ACK INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
				BroadCastSms(COM_SEND_SUCCESS,true);
			}
		}
		else if((causetra & 0x3F) == ACTTERM)
		{
			info->cmd_ack = 0;		/* execute command Term */
			info->cmdbuf.term_flag = 0;
			info->comdflag = 0;
			info->cmdbuf.sel_tim = 0;
#ifdef DEBUG
			OutputDspInfo(0, "IEC SC/DC/RC %u execute command terminal INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
			BroadCastSms(COM_CTRL_TERM,true);
		}
		else
		{
			info->cmd_ack = 0;
#ifdef DEBUG
			OutputDspInfo(0, "IEC SC/DC/RC %u  INF=%d com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,qoc,causetra);
#endif
			BroadCastSms(COM_CTRL_FAIL,true);
		}
		break;

	case C_SE_NA:	/* confirm of set-point command */
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");
			
		memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&ana16data, &info->recv_data.data[recvdatapt][9],2);
		Swap2Byte(&infoaddr);
		Swap2Byte(&ana16data);

		qoc = info->recv_data.data[recvdatapt][11];

		if(12 != info->recv_data.length[recvdatapt])	/* pkt_len - 6; buf[1]-4 */
		{
			OutputDspInfo(0, "%d C_SE_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			info->cmd_ack = -1;		/* negative confirm */
			info->comdflag = 0;
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
				info->comdflag = 0;
#ifdef DEBUG
				OutputDspInfo(0, "set-point %u NACK INF=%d stval=%x cause of transmission=%2x received!\n",type_id,infoaddr,ana16data,causetra);
#endif
				BroadCastSms(COM_CTRL_FAIL,true);
				break;
			}
			if(qoc & 0x80)
			{
				info->cmd_ack = 2;		/* select command ACK */
				info->comdflag = 2;
#ifdef DEBUG
				OutputDspInfo(0, "IEC SE %u select command ACK INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
			}
			else
			{
				info->cmd_ack = 1;		/* execute command ACK */
#ifdef DEBUG
				OutputDspInfo(0, "IEC SE %u execute command ACK INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
				BroadCastSms(COM_SEND_SUCCESS,true);
			}
		}
		else if((causetra & 0x3F) == ACTTERM)
		{
			info->cmd_ack = 0;		/* execute command Term */
			info->comdflag = 0;
#ifdef DEBUG
			OutputDspInfo(0, "IEC SE %u execute command terminal INF=%d com=%x \n",type_id,infoaddr,qoc);
#endif
			BroadCastSms(COM_CTRL_TERM,true);
			
		}
		else
		{
			info->cmd_ack = 0;		/* execute command confirm */
#ifdef DEBUG
			OutputDspInfo(0, "set-point %u INF=%d com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,qoc,causetra);
#endif
			BroadCastSms(COM_CTRL_FAIL,true);
		}
		break;

	case C_SE_NC:	/* confirm of set-point command */
		if(!comm_info->ctrl_priv)
		{
			OutputDspInfo(0, "no data broadcast priv");
			return 0;
		}
		OutputDspInfo(0, "have data broadcast priv");
			
		memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&value, &info->recv_data.data[recvdatapt][9],4);
		Swap2Byte(&infoaddr);
		Swap2Byte(&value);

		qoc = info->recv_data.data[recvdatapt][13];

		if(14 != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "%d C_SE_NC: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			info->cmd_ack = -1;		/* negative confirm */
			info->comdflag = 0;
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
				info->comdflag = 0;
#ifdef DEBUG
				OutputDspInfo(0, "set-point %u NACK INF=%d fval=%f cause of transmission=%2x received!\n",type_id,infoaddr,value,causetra);
#endif
				BroadCastSms(COM_CTRL_FAIL,true);
				break;
			}
			if(qoc & 0x80)
			{
				info->cmd_ack = 2;		/* select command ACK */
				info->comdflag = 2;
#ifdef DEBUG
				OutputDspInfo(0, "IEC SE %u select command ACK INF=%d %f com=%x \n",type_id,infoaddr,value,qoc);
#endif
			}
			else
			{
				info->cmd_ack = 1;		/* execute command ACK */
#ifdef DEBUG
				OutputDspInfo(0, "IEC SE %u execute command ACK INF=%d %f com=%x \n",type_id,infoaddr,value,qoc);
#endif
				BroadCastSms(COM_SEND_SUCCESS,true);
			}
		}
		else if((causetra & 0x3F) == ACTTERM)
		{
			info->cmd_ack = 0;		/* execute command Term */
			info->comdflag = 0;
#ifdef DEBUG
			OutputDspInfo(0, "IEC SE %u execute command terminal INF=%d %f com=%x \n",type_id,infoaddr,value,qoc);
#endif
			BroadCastSms(COM_CTRL_TERM,true);
			
		}
		else
		{
			info->cmd_ack = 0;		/* execute command confirm */
#ifdef DEBUG
			OutputDspInfo(0, "set-point %u INF=%d %f com=%x cause of transmission=%2x wrong!\n",type_id,infoaddr,value,qoc,causetra);
#endif
			BroadCastSms(COM_CTRL_FAIL,true);
		}
		break;

	case M_EI_NA:	/* Slave station End of Initialization */

		if(10 != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "%d M_EI_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			break;
		}
		switch(causetra & 0x3F)
		{
		case INIT:		/* Slave station init
/*
			initflag[stn_id] = 2;
			if(stn_id == 1)
				ClockSyncflag[stn_id] = 1;
			interrogationflag[stn_id] = 1;
			if(info->itdef.number != 0)
				counterflag[stn_id] = 1;
*/
#ifdef DEBUG
			OutputDspInfo(0, "End of Initialization in remote slave system!\n");
#endif
			break;
		default :
			OutputDspInfo(0, "M_EI_NA: Received cause of transmission=%2x wrong!\n",causetra);
			break;
		}/* switch(causetra & 0x3F) */
 		break;

	case C_IC_NA:	/* confirm of general interrogation command */
			
		if(10 != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "%d C_IC_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			info->cmd_ack = -1;		/* negative confirm */
			break;
		}
		switch(causetra & 0x3F)
		{
		case ACTCON:		/* general interrogation command act confirm */

			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
/* 				interrogationflag[stn_id] = 0; */
#ifdef DEBUG
				OutputDspInfo(0, "IEC general interrogation command is NACK by remote system!\n");
#endif
			}
			else
			{
				info->cmd_ack = 1;		/* command confirm */
/* 				interrogationflag[stn_id] = 2; */
#ifdef DEBUG
				OutputDspInfo(0, "IEC general interrogation command is ACK by remote system!\n");
#endif
			}
			break;

		case ACTTERM:				/* general interrogation command act terminal */

/*
			interrogationflag[stn_id] = 0;
			if(initflag[stn_id] ==2)
			{
				if(info->itdef.number != 0)
				{
					counterflag[stn_id] = 1;
					initflag[stn_id] =3;
				}
				else
					initflag[stn_id] =0;
			}
*/
#ifdef DEBUG
			OutputDspInfo(0, "IEC general interrogation command is terminal in remote system!\n");
#endif
/*
			DBSentAll(pName);
*/
			break;

		case DEACTCON:

			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				OutputDspInfo(0, "IEC general interrogation deact command is NACK by remote system!\n");
#endif
			}
			else
			{
				info->cmd_ack = 1;		/* positive confirm */
/* 				interrogationflag[stn_id] = 0; */
#ifdef DEBUG
				OutputDspInfo(0, "IEC general interrogation deact command is ACK by remote system!\n");
#endif
			}
			break;
		default :
			OutputDspInfo(0, "C_IC_NA: Received cause of transmission=%2x wrong!\n",causetra);
			break;
		}/* switch(causetra & 0x3F) */
 			
		break;

	case C_CI_NA:	/* confirm of counter interrogation command */

		if(10 != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "%d C_CI_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			info->cmd_ack = -1;		/* negative confirm */
			break;
		}
		switch(causetra & 0x3F)
		{
		case ACTCON:		/* counter interrogation command act confirm */
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
/* 				counterflag[stn_id] = 0; */
				info->cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				OutputDspInfo(0, "IEC counter interrogation command is NACK by remote system!\n");
#endif
			}
			else
			{
				info->cmd_ack = 1;		/* execute command confirm */
/* 				counterflag[stn_id] = 2; */
#ifdef DEBUG
				OutputDspInfo(0, "IEC counter interrogation command is ACK by remote system!\n");
#endif
			}
			break;

		case ACTTERM:				/* counter interrogation command act terminal */
/*
			counterflag[stn_id] = 0;
			initflag[stn_id] =0;
*/
#ifdef DEBUG
			OutputDspInfo(0, "IEC counter interrogation command is terminal in remote system!\n");
#endif
			break;

		case DEACTCON:

			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				OutputDspInfo(0, "IEC counter interrogation deact command is NACK by remote system!\n");
#endif
			}
			else
			{
				info->cmd_ack = 1;		/* positive confirm */
				OutputDspInfo(0, "IEC counter interrogation deact command is ACK by remote system!\n");
			}
			break;

		default :
#ifdef DEBUG
			OutputDspInfo(0, "C_CI_NA: Received cause of transmission=%2x wrong!\n",causetra);
#endif
			break;
		}/* switch(causetra & 0x3F) */
		break;

	case C_CS_NA:	/* confirm of clock synchronization command */
			
		if(20 != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "%d C_CS_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			info->cmd_ack = -1;		/* negative confirm */
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
/* 				ClockSyncflag[stn_id] = 0; */
#ifdef DEBUG
				OutputDspInfo(0, "clock synchronization command is NACK by remote system!\n");
#endif
			}
			else
			{
				info->cmd_ack = 1;		/* execute command confirm */
/* 				ClockSyncflag[stn_id] = 0; */
#ifdef DEBUG
				OutputDspInfo(0, "clock synchronization command is ACK by remote system!\n");
#endif
			}
		}
		else
			OutputDspInfo(0, "clock synchronization command: Received cause of transmission=%2x wrong!\n",causetra);
		break;

	case P_ME_NA:	/* parameter for measurements,normalized */
			
#ifdef SUN_OS
		CrossCopy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);
		CrossCopy(&ana16data, &info->recv_data.data[recvdatapt][9],2);
#else
		memcpy(&infoaddr,&info->recv_data.data[recvdatapt][6],2);	 /* address of object */
		memcpy(&ana16data, &info->recv_data.data[recvdatapt][9],2);
#endif
		qoc = info->recv_data.data[recvdatapt][11];

		if(12 != info->recv_data.length[recvdatapt])
		{
			OutputDspInfo(0, "%d P_ME_NA: ACK ASDU length error! type_id=%d,length=%u\n",
				pName,type_id,info->recv_data.length[recvdatapt]);
			info->cmd_ack = -1;		/* negative confirm */
			break;
		}
		if((causetra & 0x3F) == ACTCON)
		{
			if(causetra & 0x40)		/* 0 positive 1 negative confirm */
			{
				info->cmd_ack = -1;		/* negative confirm */
#ifdef DEBUG
				OutputDspInfo(0, "parameter set %u NACK INF=%d stval=%x kind of parameter=%2x received!\n",
					type_id,infoaddr,ana16data,qoc);
#endif
			}
			else
			{
				info->cmd_ack = 0;		/* execute command confirm */
				info->comdflag = 0;
#ifdef DEBUG
				OutputDspInfo(0, "parameter set %u ACK INF=%d stval=%x kind of parameter=%2x received!\n",
					type_id,infoaddr,ana16data,qoc);
#endif
			}
		}
		else
		{
			info->cmd_ack = 0;
#ifdef DEBUG
			OutputDspInfo(0, "parameter set %u INF=%d com=%x cause of transmission=%2x wrong!\n",
				type_id,infoaddr,qoc,causetra);
#endif
		}
		break;

	default :
		OutputDspInfo(0, "IEC type_id = %u is unavailable at this time!\n",type_id);
		break;
	}/* switch(type_id) */

	return 0;
}	

UCHAR sc_opr_stat[MAX_OPR_NUM]= {0x00,0x11,0x21,0x41};
UCHAR dc_opr_stat[MAX_OPR_NUM]= {0x01,0x12,0x22,0x42};

int	IEC870_5_Send(IEC104_CMM_INFO *info, COMM_INFO *comm_info)
{
	short	sd_pnt;
	unsigned char cmd_type,op_type; 

	struct	net_cmd_seg		netCmdSeg;
	struct	net_cmd_data	netCmdData;	
	COMMAND_STATUS cmd_stat;
	short	rcv_pnt;
	POINTER	rcv_ptr;
	POINTER	ptr;

	int		cmd_data, i;
 	char	host_id;
	short	pntnum;
	float	setpv;
	int		find_flag;

	sd_pnt = 0;
	info->comdflag = 0;
	cmd_type = 0;


	while( info->cmd_end!=shm_net_cmd_addr->head )
	{
		rcv_pnt = info->cmd_end;
		info->cmd_end++;
		if(info->cmd_end >= MAX_NETCMD_SEG_NUM)			
			info->cmd_end = 0;

		if(!comm_info->ctrl_priv)
			continue;

		netCmdSeg= shm_net_cmd_addr->seg[rcv_pnt];
		Swap2Byte(&netCmdSeg.lan_head.length);
		if(netCmdSeg.lan_head.length!=sizeof(COMMAND_MSG))
		{
			OutputDspInfo(OUTPUT_ERROR, "net_cmd length error: %d in shm_net_cmd buf ptr= %d", 
				netCmdSeg.lan_head.length, rcv_pnt);
			continue;
		}

		if(netCmdSeg.lan_head.dp_type!=COMM_TIME && netCmdSeg.lan_head.dp_type!=COMMB_TIME
			&& netCmdSeg.lan_head.dp_type!=COMM_RECORD_TIME)
		{
			OutputDspInfo(OUTPUT_ERROR, "net_cmd dp_type error: %d in shm_net_cmd buf ptr= %d", 
				netCmdSeg.lan_head.dp_type, rcv_pnt);
			continue;
		}
		
		netCmdData= netCmdSeg.net_cmd_buf.netdata;
		Swap2Byte( &(netCmdData.point_id));
		Swap2Byte( &(netCmdData.status));
		Swap4Byte( &(netCmdData.data));
		Swap2Byte( &(netCmdData.ctrl_addr));
		Swap2Byte( &(netCmdData.year));
		

		cmd_type = netCmdData.type_id;
		cmd_stat = netCmdData.status;
		cmd_data = netCmdData.data;

		op_type = (unsigned char)(cmd_stat.opr);

		rcv_ptr.stn_id = netCmdData.stn_id;
		rcv_ptr.dev_id = netCmdData.dev_id;
		rcv_ptr.data_type = netCmdData.data_type;
		rcv_ptr.pt_id = netCmdData.point_id;
		host_id = netCmdData.host_id;

		find_flag= 0;
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
			OutputDspInfo(0, "cmd_type=%d stn_id=%d dev_id=%d data_type=%d point_id=%d cmd_stat=%d cmd_data=%f host_id=%d\n",
				cmd_type, rcv_ptr.stn_id, rcv_ptr.dev_id, rcv_ptr.data_type,rcv_ptr.pt_id,cmd_stat.opr,cmd_data/1000.,host_id);

			for(pntnum=0;pntnum<info->dcdef.number;pntnum++)		/* Check for DC Command */
			{
				ptr = info->dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(info->dcdef.cmdtype[pntnum] == cmd_type))
				{
					for(i=0;i<MAX_OPR_NUM;i++)
					{
						if(info->dcdef.optype[pntnum][i] == op_type)
						{
							if(info->Cmd_Type == 0)
								info->cmdbuf.stat = sc_opr_stat[i];
							else
								info->cmdbuf.stat = dc_opr_stat[i];
							find_flag= 1;
							break;
						}						
					}

					if(!find_flag)
						continue;
					
					memset(&shm_net_cmd_addr->seg[rcv_pnt], 0, sizeof(struct net_cmd_seg));

					info->cmdbuf.cot = ACT;
					info->cmdbuf.infoaddr = pntnum+info->dc_cmd_base_addr;
					info->cmdbuf.ack_flag = 0;
					info->cmdbuf.selected = 1;
					info->cmdbuf.term_flag = 0;
					info->cmdbuf.sel_tim = time(&info->cmdbuf.sel_tim);
					if(info->Cmd_Type == 0)
					{
						info->cmdbuf.tpyeId = C_SC_NA;
						if(info->Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					else
					{
						info->cmdbuf.tpyeId = C_DC_NA;
						if(info->Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					info->comdflag = 1;
					return(true);
				}
			}
			break;

		case C_UNIT:
			OutputDspInfo(0, "cmd_type=%d stn_id=%d dev_id=%d data_type=%d point_id=%d cmd_stat=%d cmd_data=%f host_id=%d\n",
			cmd_type, rcv_ptr.stn_id, rcv_ptr.dev_id, rcv_ptr.data_type,rcv_ptr.pt_id,cmd_stat.opr,cmd_data/1000.,host_id);

			for(pntnum=0;pntnum<info->dcdef.number;pntnum++)		/* Check for DC Command */
			{
				ptr = info->dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(info->dcdef.cmdtype[pntnum] == cmd_type))
				{
					for(i=0;i<MAX_OPR_NUM;i++)
					{
						if(info->dcdef.optype[pntnum][i] == op_type)
						{
							if(info->Cmd_Type == 0)
								info->cmdbuf.stat = sc_opr_stat[i];
							else
								info->cmdbuf.stat = dc_opr_stat[i];
							find_flag= 1;
							break;
						}						
					}
					
					if(!find_flag)
						continue;

					memset(&shm_net_cmd_addr->seg[rcv_pnt], 0, sizeof(struct net_cmd_seg));
					
					info->cmdbuf.cot = ACT;
					info->cmdbuf.infoaddr = pntnum+info->dc_cmd_base_addr;
					info->cmdbuf.ack_flag = 0;
					info->cmdbuf.selected = 1;
					info->cmdbuf.term_flag = 0;
					info->cmdbuf.sel_tim = time(&info->cmdbuf.sel_tim);
					if(info->Cmd_Type == 0)
					{
						info->cmdbuf.tpyeId = C_SC_NA;
						//info->cmdbuf.stat = 0x1;
						if(info->Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					else
					{
						info->cmdbuf.tpyeId = C_DC_NA;
						//info->cmdbuf.stat = 0x2;
						if(info->Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					info->comdflag = 1;
					return(true);
				}
			}
			break;

		case C_SPEED_STEP:
		case C_VOLT_STEP:
			OutputDspInfo(0, "cmd_type=%d stn_id=%d dev_id=%d data_type=%d point_id=%d cmd_stat=%d cmd_data=%f host_id=%d\n",
			cmd_type, rcv_ptr.stn_id, rcv_ptr.dev_id, rcv_ptr.data_type,rcv_ptr.pt_id,cmd_stat.state,cmd_data/1000.,host_id);

			for(pntnum=0;pntnum<info->dcdef.number;pntnum++)		/* Check for DC Command */
			{
				ptr = info->dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(info->dcdef.cmdtype[pntnum] == cmd_type))
				{
					for(i=0;i<MAX_OPR_NUM;i++)
					{
						if(info->dcdef.optype[pntnum][i] == op_type)
						{
							if(info->Cmd_Type == 0)
								info->cmdbuf.stat = sc_opr_stat[i];
							else
								info->cmdbuf.stat = dc_opr_stat[i];
							find_flag= 1;
							break;
						}						
					}
					
					if(!find_flag)
						continue;

					memset(&shm_net_cmd_addr->seg[rcv_pnt], 0, sizeof(struct net_cmd_seg));
					
					info->cmdbuf.cot = ACT;
					info->cmdbuf.infoaddr = pntnum+info->dc_cmd_base_addr;
					info->cmdbuf.ack_flag = 0;
					info->cmdbuf.selected = 1;
					info->cmdbuf.term_flag = 0;
					info->cmdbuf.sel_tim = time(&info->cmdbuf.sel_tim);
					if(info->Cmd_Type == 0)
					{
						info->cmdbuf.tpyeId = C_SC_NA;
						info->cmdbuf.stat = 0x1;
						if(info->Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					else
					{
						info->cmdbuf.tpyeId = C_DC_NA;
						info->cmdbuf.stat = 0x2;
						if(info->Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					info->comdflag = 1;
					return(true);
				}
			}
			break;

		case C_APOWER:
		case C_RPOWER:
			for(pntnum=0;pntnum<info->sedef.number;pntnum++)
			{
				ptr = info->sedef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(info->sedef.cmdtype[pntnum] == cmd_type))
					break;
			}
			if(pntnum>=info->sedef.number)
			{
				OutputDspInfo(0, "No this C_APOWER/C_RPOWER:%d stn:%d dev:%d in set point table!\n",cmd_type,rcv_ptr.stn_id,rcv_ptr.dev_id);
				break;
			}

			memset(&shm_net_cmd_addr->seg[rcv_pnt], 0, sizeof(struct net_cmd_seg));

			info->sebuf.tpyeId = C_SE_NC;				/* cmd type of ASDU */
			info->sebuf.cot = ACT;
			info->sebuf.infoaddr = pntnum+info->se_cmd_float_addr;
			info->sebuf.qoc = 0;

			setpv = (float)(cmd_data/1000.);
			info->sebuf.fvalue = setpv;
			OutputDspInfo(0, "cmd_data= %d; value=%f\n",cmd_data,setpv);
/*
			setpv = GetAnaValue ( &anadata );
			OutputDspInfo(0, "stat %d data %d; value=%f\n",cmd_stat,cmd_data,setpv);
*/
/*
			if(setpv <= info->sedef.lorange[pntnum])
			{
				if(info->SEType == 0)
					normv = 0;
				else
					normv = (short)0x8000;
			}
			else	if(setpv >= info->sedef.hirange[pntnum])
				normv = 0x7FFF;
			else
			{
				if(info->SEType == 0)
				{
					ftemp=(setpv-info->sedef.lorange[pntnum])*32767.0/
						(info->sedef.hirange[pntnum]-info->sedef.lorange[pntnum]);
					normv=(short)ftemp;
					if((ftemp-normv)>=0.5)
						normv++;
					else if((ftemp-normv)<=-0.5)
						normv--;
				}
				else
				{
					ftemp=(setpv-(info->sedef.hirange[pntnum]+info->sedef.lorange[pntnum])/2.0)*65535.0/
						(info->sedef.hirange[pntnum]-info->sedef.lorange[pntnum]);
					normv=(short)ftemp;
					if((ftemp-normv)>=0.5)
						normv++;
					else if((ftemp-normv)<=-0.5)
						normv--;
				}
			}
*/
/*			if(!SetPointCmdNC((char)0,info->sebuf.infoaddr,info->sebuf.fvalue,sock)) */
			if(!SetPointCmdNC((char)0,info->sebuf.infoaddr,(float)setpv,info))
				return(false);
			info->comdflag = 1;
			break;

		case DPS_MAN_VALUE_SET:
		case C_OP_VALUE_SET:

			for(pntnum=0;pntnum<info->sedef.number;pntnum++)		/* Check for SE Command */
			{
				ptr = info->sedef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(info->sedef.cmdtype[pntnum] == cmd_type))
				{
					break;
				}
			}
			if(pntnum>=info->sedef.number)
				continue;

			memset(&shm_net_cmd_addr->seg[rcv_pnt], 0, sizeof(struct net_cmd_seg));

			info->sebuf.tpyeId = C_SE_NC;				/* cmd type of ASDU */
			info->sebuf.cot = ACT;
			info->sebuf.infoaddr = pntnum+info->se_cmd_float_addr;
			info->sebuf.qoc = 0;

			setpv = (float)(cmd_data/1000.);
			info->sebuf.fvalue = setpv;
/*
			if(setpv <= info->sedef.lorange[i])
			{
				if(info->SEType == 0)
					normv = 0;
				else
					normv = (short)0x8000;
			}
			else	if(setpv >= info->sedef.hirange[i])
				normv = 0x7FFF;
			else
			{
				if(info->SEType == 0)
				{
					ftemp=(setpv-info->sedef.lorange[i])*32767.0/
						(info->sedef.hirange[i]-info->sedef.lorange[i]);
					normv=(short)ftemp;
					if((ftemp-normv)>=0.5)
						normv++;
					else if((ftemp-normv)<=-0.5)
						normv--;
				}
				else
				{
					ftemp=(setpv-(info->sedef.hirange[i]+info->sedef.lorange[i])/2.0)*65535.0/
						(info->sedef.hirange[i]-info->sedef.lorange[i]);
					normv=(short)ftemp;
					if((ftemp-normv)>=0.5)
						normv++;
					else if((ftemp-normv)<=-0.5)
						normv--;
				}
			}
*/
			if(!SetPointCmdNC(0,info->sebuf.infoaddr,info->sebuf.fvalue,info))
				return(false);
			info->comdflag = 1;
			break;

		default:
			for(pntnum=0;pntnum<info->dcdef.number;pntnum++)		/* Check for DC Command */
			{
				ptr = info->dcdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&(ptr.data_type == rcv_ptr.data_type)&&
					(ptr.pt_id == rcv_ptr.pt_id)&&(info->dcdef.cmdtype[pntnum] == cmd_type))
				{
					for(i=0;i<MAX_OPR_NUM;i++)
					{
						if(info->dcdef.optype[pntnum][i] == op_type)
						{
							if(info->Cmd_Type == 0)
								info->cmdbuf.stat = sc_opr_stat[i];
							else
								info->cmdbuf.stat = dc_opr_stat[i];
							find_flag= 1;
							break;
						}						
					}
					
					if(!find_flag)
						continue;

					memset(&shm_net_cmd_addr->seg[rcv_pnt], 0, sizeof(struct net_cmd_seg));
					
					info->cmdbuf.cot = ACT;
					info->cmdbuf.infoaddr = pntnum+info->dc_cmd_base_addr;
					info->cmdbuf.ack_flag = 0;
					info->cmdbuf.selected = 1;
					info->cmdbuf.term_flag = 0;
					info->cmdbuf.sel_tim = time(&info->cmdbuf.sel_tim);
					if(info->Cmd_Type == 0)
					{
						info->cmdbuf.tpyeId = C_SC_NA;
						if(info->Cmd_Sel == 0)
						{
							if(!SinglePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!SinglePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					else
					{
						info->cmdbuf.tpyeId = C_DC_NA;
						if(info->Cmd_Sel == 0)
						{
							if(!DoublePointCtrl((char)0,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
						else
						{
							if(!DoublePointCtrl((char)1,info->cmdbuf.infoaddr,info->cmdbuf.stat,info))
								return(FALSE);
						}
					}
					info->comdflag = 1;
#ifdef DEBUG
					OutputDspInfo(0, "cmd_type=%d stn_id=%d dev_id=%d data_type=%d point_id=%d cmd_stat=%d cmd_data=%f host_id=%d\n",
						cmd_type, rcv_ptr.stn_id, rcv_ptr.dev_id, rcv_ptr.data_type,rcv_ptr.pt_id,cmd_stat.state,cmd_data/1000.,host_id);
#endif
					return(true);
				}
			}
			break;
		}/* switch(cmd_type) */
	}/* while(shm_send_addr->head!= info->cmd_end) */
	return(true);
}/* IEC870_5_Send() */

BOOL FindSameSoeRecInEventTable(SW_MSG *ptr)
{
	USHORT	rcv_pnt, rec_num;
	int		i;
	IND_REC *soe_rec;
	extern	SHM_RECORD	*shm_rec_addr;	

	rcv_pnt= shm_rec_addr->head.header_ptr;
	rec_num= shm_rec_addr->head.count;
	for(i=0; i<rec_num; i++)
	{
		rcv_pnt= (rcv_pnt+MAX_SHM_RECORD_NUM-1)%MAX_SHM_RECORD_NUM;
		soe_rec= &shm_rec_addr->rec[rcv_pnt].ind_rec;
		if(soe_rec->type_id!=DPS_IND_ALM)
			continue;
		
		if(ptr->point_id==soe_rec->pt_id && ptr->data_type==soe_rec->data_type
			&& ptr->dev_id==soe_rec->dev_id && ptr->stn_id==soe_rec->stn_id			
			&& ptr->type_id==soe_rec->type_id && ptr->sec==soe_rec->second
			&& ptr->ms==soe_rec->ms && ptr->min==soe_rec->minute
			&& ptr->hour==soe_rec->hour && ptr->day==soe_rec->day
			&& ptr->month==soe_rec->month && ptr->year==soe_rec->year
			&& ptr->status.rt_state==soe_rec->ind_status.rt_state
			&& ptr->status.alm_type==soe_rec->ind_status.alm_type
			&& ptr->status.chan_fail==soe_rec->ind_status.chan_fail
			&& ptr->status.man_set==soe_rec->ind_status.man_set)
			return TRUE;
	}
	return FALSE;
}


BOOL InsertSoeRecBuff(IEC104_CMM_INFO *info, SW_MSG *soe_msg, time_t recv_time)
{
	int	i;
	time_t	soe_time_s;
	struct tm	time_tm;
	POINTER	pt;
	USHORT	almEnable;
	
	if(info->soe_rec_buff.num>=MAX_SOE_REC_NUM)
		return FALSE;

	pt.stn_id= soe_msg->stn_id;
	pt.dev_id= soe_msg->dev_id;
	pt.data_type= soe_msg->data_type;
	pt.pt_id= soe_msg->point_id;
	if(-1==IsPtAlmEnableById(&pt, &almEnable))
		return FALSE;
	if(!almEnable)
		return FALSE;

	//ON OFF ALM

	time_tm.tm_isdst= 0;
	time_tm.tm_sec= soe_msg->sec;
	time_tm.tm_min= soe_msg->min;
	time_tm.tm_hour= soe_msg->hour;
	time_tm.tm_mday= soe_msg->day;
	time_tm.tm_mon= soe_msg->month-1;
	time_tm.tm_year= soe_msg->year-1900;
	soe_time_s= mktime(&time_tm);
	if((recv_time-soe_time_s) > (60*5)) //seconds
	{
		OutputDspInfo(0, "discard soe before 5 minute: %d.%d.%d.%d %02d:%02d:%02d:%03d", 
			soe_msg->stn_id, soe_msg->dev_id, soe_msg->data_type, soe_msg->point_id,
			soe_msg->hour, soe_msg->min, soe_msg->sec, soe_msg->ms);
		return FALSE;
	}
	
	if(FindSameSoeRecInEventTable(soe_msg))
		return FALSE;

	for(i=0; i<MAX_SOE_REC_NUM; i++)
	{
		if(info->soe_rec_buff.soe[i].type_id)
			continue;
		memcpy(&info->soe_rec_buff.soe[i], soe_msg, sizeof(SW_MSG));
		info->soe_rec_buff.recv_time[i]= recv_time;
		info->soe_rec_buff.num++;
		return TRUE;
	}
	return FALSE;
}


void	CheckRecvSoeRec(IEC104_CMM_INFO *info, COMM_INFO *comm_info)
{
	time_t	curr_time_s;
	int		i,num,total_soe_num;
	extern int mainChanAbnormalFlag;

 	OutputDspInfo(0, "Check recv soe buf: count=%d", info->soe_rec_buff.num);

	num=0;
	total_soe_num= info->soe_rec_buff.num;
	curr_time_s= time(NULL);
	for(i=0; i<MAX_SOE_REC_NUM; i++)
	{
		SW_MSG *ptr;
		
		if(num>=total_soe_num)
			break;

		ptr= &info->soe_rec_buff.soe[i];
		if(ptr->type_id==0)
			continue;
		
		num++;
		
		if(FindSameSoeRecInEventTable(ptr))
		{
			memset(ptr, 0, sizeof(SW_MSG));
			info->soe_rec_buff.num--;
			continue;
		}

// 		if((curr_time_s-info->soe_rec_buff.recv_time[i]) > 20) //seconds
// 		{
// 			OutputDspInfo(OUTPUT_ERROR, "Additional soe exist in standby channel: %d.%d.%d.%d %02d:%02d:%02d:%03d, \nset chan switch flag",
// 				ptr->stn_id, ptr->dev_id, ptr->data_type, ptr->point_id,
// 				ptr->hour, ptr->min, ptr->sec, ptr->ms);
// 			mainChanAbnormalFlag= 1;
// 			return;
// 		}
	}
}

void	BroadcastSoeRec(IEC104_CMM_INFO *info, COMM_INFO *comm_info)
{
	int		i,num,lanpt;
	SW_MSG		sw_buf[70];
	LAN_HEAD	lanhead;
	
	num=0;
	lanpt= 0;

	OutputDspInfo(0, "start to BroadcastSoeRec");
	
	for(i=0; i<MAX_SOE_REC_NUM; i++)
	{
		SW_MSG *ptr;
		
		if(num>=info->soe_rec_buff.num)
			break;

		ptr= &info->soe_rec_buff.soe[i];
		if(ptr->type_id==0)
			continue;
		
		if(FindSameSoeRecInEventTable(ptr))
		{
			memset(ptr, 0, sizeof(SW_MSG));
			continue;
		}

		memcpy(&sw_buf[lanpt], ptr, sizeof(SW_MSG));
		memset(ptr, 0, sizeof(SW_MSG));

		Swap2Byte(&sw_buf[lanpt].number);
		Swap2Byte(&sw_buf[lanpt].point_id);
		Swap2Byte(&sw_buf[lanpt].status);
		Swap2Byte(&sw_buf[lanpt].ms);
		Swap2Byte(&sw_buf[lanpt].year);
		
		lanpt ++;
		if( lanpt>=70 )
		{
			OutputDspInfo(0, "BroadcastSoeRec: num= %d", lanpt);
			lanhead.length = lanpt*sizeof(SW_MSG);
			Swap2Byte(&lanhead.length);
			lanhead.dp_type=BJ_DB_TIME;
			GetLocalHostId(&lanhead.src_id);
			lanhead.dest_id =0;
			lanhead.dest_stn =sw_buf[0].stn_id;
			lanhead.packet_num =0;
			lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
			lanpt = 0;
		}

		num++;
		
	}

	if( lanpt>0 )
	{
		OutputDspInfo(0, "BroadcastSoeRec: num= %d", lanpt);
		lanhead.length = lanpt*sizeof(SW_MSG);
		Swap2Byte(&lanhead.length);
		lanhead.dp_type=BJ_DB_TIME;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.dest_stn =sw_buf[0].stn_id;
		lanhead.packet_num =0;
		lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
	}	
	info->soe_rec_buff.num= 0;
}

