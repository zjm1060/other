/************************************************************************
*  FILE NAME:           iccp_lib.c      								*
*  FUNCTION:															*
*			Common Application Layer Function							*
*  's NAME : 	              											*
*  DATE:                Oct,2011                						*
*																		*
* MODIFICATION LOG :													*
*  Date     Who   Rev                     Comments						*
* --------  ---  ------   ----------------------------------------------*
*      *
* 	*
* 10/06/2011  yyp    1       Created for SUN ICCP TASE.2 application	*
************************************************************************/

#include <time.h>
#include "../inc/mit_main.h"
#include "str_util.h"

#include "cppinc.h"

#include "../inc/iccp_main.h"
#include "../../../plc/v4.0a/inc/lcu_data.h"
#include "../../../dpp/v4.0a/inc/dppdef.h"
#include "../../../dpp/v4.0a/inc/dpp_functions.h"

/*
#define	DEBUG
*/
//#define BY_RECORD		/* use shared memory record area to catch SOE */
#define CMD_TO_LCU

#define GW_DEVID	20

SHM_RECORD *shm_rec_addr;

extern	char	infobuf[1024];
extern	SHM_BUF *shm_buf_addr;

extern	int	soe_overflow_flag,soe_overflow_Sendflag;
extern	unsigned char	cmd_src;

extern	SOEBUF	soebuf;
short	rcv_end;
extern	ST_INT	nAssocNum[MAX_DC_NUM];		/* Added for save association number for remotes */
extern	sGWAssoc  sAssocDat[MAX_DC_NUM][2];	/* Changed to two dimension for 2 association in one remote 2003.7.30 zy */

extern unsigned char cur_remote_id;
unsigned char	orgdpsoebuf[MAX_DP_NUM];


#ifdef REC_YF_AGC_CMD
	unsigned short	daysaved;
	char		cmdrecfile[80];
	FILE		*fagc;
#endif

SPDEF		spdef;
DPDEF		dpdef;
MEDEF		medef;
DPDEF		dpdef;
MEDEF		medef;
SEDEF		sedef;
CMDDEF		scdef;
extern	short	cmd_end;
extern	unsigned char cur_stn_id;		//added by yyp 2011.03.09
int  chann_priv_old, chann_priv=0;		//added by yyp 2011.10.09
struct iccp_data_seg iccp_data, *p_iccp_data;	
struct shm_iccp_data	data_iccp,*shm_iccp_data_addr;
static	unsigned char	me_buf[1440];
static	SW_MSG		sw_buf[70];
static	short		me_num = 0;
static	short		sw_num = 0;
static	int		ana_all_flag = 0;

char    src_id;
long GetTimefromCmd(struct net_cmd_data *p_rec);			//added by yyp 2011.03.09
extern  ST_VOID *findDevice (MIT_DEVICE_REF *pstDeviceSet, ST_CHAR *devName);
extern ST_VOID    DisplayDeviceYYP (ST_VOID);

void InitData(void)
{
	spdef.number = 0;
	dpdef.number = 0;
	medef.number = 0;
	dpdef.number = 0;
	medef.number = 0;

	sedef.number = 0;

	scdef.number = 0;
/*
	GenSPTable((char *)SPDEF_FILE);
*/
	GenDPTable((char *)DPDEF_FILE);

	GenMETable((char *)MEDEF_FILE);
/*
	GenClientDPTable((char *)C_DPDEF_FILE);

	GenClientMETable((char *)C_MEDEF_FILE);
*/
	GenSETable((char *)SEDEF_FILE);

	GenSCTable((char *)SCDEF_FILE);
/*
	GenDCTable((char *)DCDEF_FILE);
*/
	soe_overflow_flag = soe_overflow_Sendflag = 0;

	soebuf.bufpnt = soebuf.sendpnt = 0;

	ICCP_SOE_Init();

	src_id =0;
}

void InitData_S(void)
{
	spdef.number = 0;
	dpdef.number = 0;
	medef.number = 0;
	dpdef.number = 0;
	medef.number = 0;

	sedef.number = 0;

	scdef.number = 0;
/*
	GenSPTable((char *)SPDEF_FILE_S);
*/
	GenDPTable((char *)DPDEF_FILE_S);

	GenMETable((char *)MEDEF_FILE_S);
/*
	GenClientDPTable((char *)C_DPDEF_FILE);

	GenClientMETable((char *)C_MEDEF_FILE);
*/
	GenSETable((char *)SEDEF_FILE_S);

	GenSCTable((char *)SCDEF_FILE_S);
/*
	GenDCTable((char *)DCDEF_FILE_S);
*/
	soe_overflow_flag = soe_overflow_Sendflag = 0;

	soebuf.bufpnt = soebuf.sendpnt = 0;

	ICCP_SOE_Init();

	src_id =0;
}

//added by yyp 2011.10.12
void InitICCPShm()
{
	unsigned char	stat;
	POINTER 	ptr;
	unsigned int pntnum;
	DATE_TIME	tm;
	float value;
	GetSysTime(&tm);
	iccp_data.stn_id = cur_stn_id;
	iccp_data.dp_num = dpdef.number;
	iccp_data.me_num = medef.number;

	for(pntnum=0;pntnum<dpdef.number;pntnum++)
	{
		ptr = dpdef.ptr[pntnum];
		strcpy(iccp_data.dp[pntnum].name,dpdef.name[pntnum]);
		iccp_data.dp[pntnum].time = tm;
		if(dpdef.doublept[pntnum] == 1)
		{
			if(GetDPStatById(&ptr,&stat)==-1)
			{
				iccp_data.dp[pntnum].valid=0;
				continue;
			}
			else
			{
				iccp_data.dp[pntnum].valid = 1;
				iccp_data.dp[pntnum].stat = stat;
			}
		}
		else
		{
			if(GetSPStatById(&ptr,&stat)==-1)
			{
				iccp_data.dp[pntnum].valid=0;
				continue;
			}
			else
			{
				iccp_data.dp[pntnum].valid = 1;
				iccp_data.dp[pntnum].stat = stat;
			}
		}
	}

	for(pntnum=0;pntnum<medef.number;pntnum++)	
	{
		strcpy(iccp_data.dp[pntnum].name,dpdef.name[pntnum]);
		iccp_data.dp[pntnum].time = tm;
		ptr = medef.ptr[pntnum];
		if(GetMEValueById(&ptr,&stat,&value)==-1)
		{
			iccp_data.me[pntnum].valid=0;
			continue;
		}
		iccp_data.me[pntnum].valid = 1;
		iccp_data.me[pntnum].stat = stat;
		iccp_data.me[pntnum].fvalue = value;
	}
	//needed to be modified by yyp 2011.10.12
	shm_iccp_data_addr->iccp_data[cur_remote_id] = iccp_data;

	return;
}


int GenSPTable(char *pDefFile)		/* single point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		sprintf(infobuf,"\n####can't open SP file %s\n",filenamestr);
 		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		spdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	

	sprintf(infobuf,"\nRead SP define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				spdef.number = j;
				sprintf(infobuf,"file: %s END --- GenSPTable: point nNumber = %d\n",pDefFile,spdef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %d %s",&iTemp,spdef.name[j],spdef.iccp_name[j],&iTemp1,lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		spdef.TimeTag[j] = iTemp1;

		if(j != iTemp)
		{
			sprintf(infobuf,"+++Warning! %d line: num %d != %d\n",r,iTemp,j);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
		}
		if(strcmp(spdef.name[j],"NULL")!=0)
		{
 			sprintf(infobuf,"SP %d pnt: %s\t%s\t%d\t%s\n",j,spdef.name[j],spdef.iccp_name[j],spdef.TimeTag[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			strcpy(name_buf2,spdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				spdef.ptr[j].dev_id = 0;
				sprintf(infobuf,"#### SP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				spdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			spdef.ptr[j].dev_id = 0;
 			sprintf(infobuf,"SP %d pnt: NULL\t\t%s\t%d\t%s\n",j,spdef.iccp_name[j],spdef.TimeTag[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(iTemp >= MAX_SP_NUM)
		{
			sprintf(infobuf,"\n!!!GenSPTable: Num=%d above the MAX_SP_NUM %d\n",iTemp,MAX_SP_NUM);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			spdef.number = MAX_SP_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}

int GenDPTable(char *pDefFile)		/* double point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		sprintf(infobuf,"####can't open DP file %s\n",filenamestr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
 		dpdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	sprintf(infobuf,"\nRead DP define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				dpdef.number = j;
				sprintf(infobuf,"file: %s END ---  GenDPTable: point nNumber = %d\n",pDefFile,dpdef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %d %d %s",&iTemp,dpdef.name[j],dpdef.iccp_name[j],&iTemp1,&iTemp2,lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		dpdef.TimeTag[j] = iTemp1;
		dpdef.doublept[j] = iTemp2;

		if(j != iTemp)
		{
			sprintf(infobuf,"+++Warning! %d line: num %d != %d\n",r,iTemp,j);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
		}
		if(strcmp(dpdef.name[j],"NULL")!=0)
		{
 			sprintf(infobuf,"DP %d pnt: %s\t%s\t%d\t%d\t%s\n",j,dpdef.name[j],dpdef.iccp_name[j],dpdef.TimeTag[j],dpdef.doublept[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			strcpy(name_buf2,dpdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				dpdef.ptr[j].dev_id = 0;
				sprintf(infobuf,"#### DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				dpdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			dpdef.ptr[j].dev_id = 0;
 			sprintf(infobuf,"DP %d pnt: NULL\t\t%s\t%d\t%d\t%s\n",j,dpdef.iccp_name[j],dpdef.TimeTag[j],dpdef.doublept[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(iTemp >= MAX_DP_NUM)
		{
			sprintf(infobuf,"\n!!!GenDPTable: Num=%d above the MAX_DP_NUM %d\n",iTemp,MAX_DP_NUM);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			dpdef.number = MAX_DP_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}

int GenMETable(char *pDefFile)		/* measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		sprintf(infobuf,"\n####can't open ME file %s\n",filenamestr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
 		medef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	

	sprintf(infobuf,"\nRead ME define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				medef.number = j;
				sprintf(infobuf,"file: %s END  ---  GenMETable: point nNumber = %d\n",pDefFile,medef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %f %f %f %f %d %d %s",&iTemp,medef.name[j],medef.iccp_name[j],
					&medef.lorange[j],&medef.hirange[j],&medef.scale[j],
					&medef.databand[j],&iTemp1,&iTemp2,lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		medef.ASDUType[j] = iTemp1;
		medef.TimeTag[j] = iTemp2;

		if(j != iTemp)
		{
			sprintf(infobuf,"+++Warning! %d line: num %d != %d\n",r,iTemp,j);
		}
		if(strcmp(medef.name[j],"NULL")!=0)
		{
			sprintf(infobuf,"ME %d %s %s %f %f %f %f %d %d %s\n",j,medef.name[j],medef.iccp_name[j],
				medef.lorange[j],medef.hirange[j],medef.scale[j],
				medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			strcpy(name_buf2,medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				medef.ptr[j].dev_id = 0;
				sprintf(infobuf,"#### ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				medef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			medef.ptr[j].dev_id = 0;
 			sprintf(infobuf,"ME %d pnt: NULL\t %s %f %f %f %f %d %d %s\n",j,medef.iccp_name[j],
				medef.lorange[j],medef.hirange[j],medef.scale[j],
				medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(j >= MAX_ME_NUM)
		{
			sprintf(infobuf,"\n!!!GenMETable: Num=%d above the MAX_ME_NUM %d\n",j,MAX_ME_NUM);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			medef.number = MAX_ME_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}

int GenClientDPTable(char *pDefFile)		/* Client double point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		sprintf(infobuf,"####can't open client DP file %s\n",filenamestr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
 		dpdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	
	sprintf(infobuf,"\nRead client DP define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				dpdef.number = j;
				sprintf(infobuf,"file: %s END ---  GenClientDPTable: point nNumber = %d\n",pDefFile,dpdef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %d %d %s",&iTemp,dpdef.name[j],dpdef.iccp_name[j],&iTemp1,&iTemp2,lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		dpdef.TimeTag[j] = iTemp1;
		dpdef.doublept[j] = iTemp2;

		if(j != iTemp)
		{
			sprintf(infobuf,"+++Warning! %d line: num %d != %d\n",r,iTemp,j);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
		}
		if(strcmp(dpdef.name[j],"NULL")!=0)
		{
 			sprintf(infobuf,"Client DP %d pnt: %s\t%s\t%d\t%d\t%s\n",j,dpdef.name[j],dpdef.iccp_name[j],
					dpdef.TimeTag[j],dpdef.doublept[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			strcpy(name_buf2,dpdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				dpdef.ptr[j].dev_id = 0;
				sprintf(infobuf,"####Client DP %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				dpdef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			dpdef.ptr[j].dev_id = 0;
 			sprintf(infobuf,"Client DP %d pnt: NULL\t\t%s\t%d\t%d\t%s\n",j,dpdef.iccp_name[j],
					dpdef.TimeTag[j],dpdef.doublept[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(iTemp >= MAX_CLI_DP_NUM)
		{
			sprintf(infobuf,"\n!!!GenClientDPTable: Num=%d above the MAX_CLI_DP_NUM %d\n",iTemp,MAX_CLI_DP_NUM);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			dpdef.number = MAX_CLI_DP_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}

int GenClientMETable(char *pDefFile)		/* Client measurement point define file */
{
	FILE		*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char		name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1,iTemp2;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	r = j = 0;
	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		sprintf(infobuf,"\n####can't open Client ME file %s\n",filenamestr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
 		medef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	

	sprintf(infobuf,"\nRead Client ME define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);
	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				medef.number = j;
				sprintf(infobuf,"file: %s END  ---  GenClientMETable: point nNumber = %d\n",pDefFile,medef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %f %f %f %f %d %d %s",&iTemp,medef.name[j],medef.iccp_name[j],
					&medef.lorange[j],&medef.hirange[j],&medef.scale[j],
					&medef.databand[j],&iTemp1,&iTemp2,lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		medef.ASDUType[j] = iTemp1;
		medef.TimeTag[j] = iTemp2;

		if(j != iTemp)
		{
			sprintf(infobuf,"+++Warning! %d line: num %d != %d\n",r,iTemp,j);
		}
		if(strcmp(medef.name[j],"NULL")!=0)
		{
			sprintf(infobuf,"Client ME %d %s %s %f %f %f %f %d %d %s\n",j,medef.name[j],medef.iccp_name[j],
				medef.lorange[j],medef.hirange[j],medef.scale[j],
				medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			strcpy(name_buf2,medef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				medef.ptr[j].dev_id = 0;
				sprintf(infobuf,"####Client ME %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
				printf("%s",infobuf);
				WriteDspInfo(infobuf);
			}
			else
			{
				medef.ptr[j] = dbentry2.point;
			}
		}
		else
		{
			medef.ptr[j].dev_id = 0;
 			sprintf(infobuf,"Client ME %d pnt: NULL\t %s %f %f %f %f %d %d %s\n",j,medef.iccp_name[j],
				medef.lorange[j],medef.hirange[j],medef.scale[j],
				medef.databand[j],medef.ASDUType[j],medef.TimeTag[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(j >= MAX_CLI_ME_NUM)
		{
			sprintf(infobuf,"\n!!!GenClientMETable: Num=%d above the MAX_CLI_ME_NUM %d\n",j,MAX_CLI_ME_NUM);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			medef.number = MAX_CLI_ME_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}

int GenSCTable(char *pDefFile)		/* single point control command define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char	name_buf2[NAME_SIZE],lgname[60];

	int		iTemp,iTemp1,iTemp2,numb;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		sprintf(infobuf,"####can't open SC file %s\n",filenamestr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		scdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	r=j=0;

	sprintf(infobuf,"\nRead SC define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				scdef.number = j;
				sprintf(infobuf,"file: %s END  ---  GenSCTable: point nNumber = %d\n",pDefFile,scdef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %d %d %d %s %s",&numb,scdef.name[j],scdef.iccp_name[j],
					&iTemp,&iTemp1,&iTemp2,scdef.interlockname[j],lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		scdef.cmdtype[j] = (unsigned char)iTemp;
		scdef.optype1[j] = (unsigned char)iTemp1;
		scdef.optype2[j] = (unsigned char)iTemp2;

		if(j!=numb)
		{
			sprintf(infobuf,"SC %d pnt number=%d wrong!!! %s  %s %d %d %d %s %s\n",j,numb,scdef.name[j],
				scdef.iccp_name[j],scdef.cmdtype[j],scdef.optype1[j],scdef.optype2[j],scdef.interlockname[j],lgname);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
		}
		if(strcmp(scdef.name[j],"NULL")!=0)
		{
			sprintf(infobuf,"SC %d pnt: %s %s %d %d %d %s %s\n",j,scdef.name[j],scdef.iccp_name[j],
					scdef.cmdtype[j],scdef.optype1[j],scdef.optype2[j],scdef.interlockname[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			strcpy(name_buf2,scdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				sprintf(infobuf,"#### SC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			sprintf(infobuf,"SC %d pnt: NULL\t%s %d %d %d %s %s\n",j,scdef.iccp_name[j],
				scdef.cmdtype[j],scdef.optype1[j],scdef.optype2[j],scdef.interlockname[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(j >= MAX_SC_NUM)
		{
			sprintf(infobuf,"\n!!!GenSCTable: Num=%d above the MAX_SC_NUM %d\n",j,MAX_SC_NUM);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			scdef.number = MAX_SC_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}

/* double point control command define file */
/*
int GenDCTable(char *pDefFile)
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	int		data_type2;
	char	name_buf2[NAME_SIZE],lgname[60];

	int		iTemp,iTemp1,iTemp2,numb;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
 	{
 		sprintf(infobuf,"####can't open DC file %s\n",filenamestr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
 		dcdef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	r=j=0;

	sprintf(infobuf,"\nRead DC define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				dcdef.number = j;
				sprintf(infobuf,"file: %s END  ---  GenDCTable: point nNumber = %d\n",pDefFile,dcdef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %d %d %d %s %s",&numb,dcdef.name[j],dcdef.iccp_name[j],&iTemp,&iTemp1,&iTemp2,dcdef.interlockname[j],lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		dcdef.cmdtype[j] = (unsigned char)iTemp;
		dcdef.optype1[j] = (unsigned char)iTemp1;
		dcdef.optype2[j] = (unsigned char)iTemp2;

		if(j!=numb)
		{
			sprintf(infobuf,"DC %d pnt number=%d wrong!!! %s %s %d %d %d %s %s\n",j,numb,dcdef.name[j],
				dcdef.iccp_name[j],dcdef.cmdtype[j],dcdef.optype1[j],dcdef.optype2[j],scdef.interlockname[j],lgname);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
		}
		if(strcmp(dcdef.name[j],"NULL")!=0)
		{
			sprintf(infobuf,"DC %d pnt: %s %s %d %d %d %s %s\n",j,dcdef.name[j],dcdef.iccp_name[j],
					dcdef.cmdtype[j],dcdef.optype1[j],dcdef.optype2[j],dcdef.interlockname[j],lgname);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			strcpy(name_buf2,dcdef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				sprintf(infobuf,"#### DC %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
 			sprintf(infobuf,"DC %d pnt: NULL\t%s %s %d %d %d %s %s\n",j,dcdef.iccp_name[j],
					dcdef.cmdtype[j],dcdef.optype1[j],dcdef.optype2[j],dcdef.interlockname[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(j >= MAX_DC_NUM)
		{
			sprintf(infobuf,"\n!!!GenDCTable: Num=%d above the MAX_DC_NUM %d\n",j,MAX_DC_NUM);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			dcdef.number = MAX_DC_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}
*/
int GenSETable(char *pDefFile)		/* set point control point define file */
{
	FILE	*fp;
	DMS_COMMON	dbentry2;
	char	name_buf2[NAME_SIZE],lgname[60];

	int	iTemp,iTemp1;
	short	j,r;
	char	filenamestr[200];
	char	filebuf[LINE_BUF_CHAR];

	sprintf(filenamestr,"%s%s",HOMEPATH,pDefFile);

 	if((fp=fopen(filenamestr,"r"))==NULL)
	{
 		sprintf(infobuf,"####can't open SE file %s\n",filenamestr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		sedef.number = 0;
		return(FALSE);          
	}
	rewind(fp);	
	r=j=0;

	sprintf(infobuf,"\nRead SE define file %s!!!\n",filenamestr);
	printf("%s",infobuf);
	WriteDspInfo(infobuf);

	for(;;)
	{
		r++;
		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				sedef.number = j;
				sprintf(infobuf,"file: %s END  ---  GenSETable: point nNumber = %d\n",pDefFile,sedef.number);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				fclose(fp);
				return(TRUE);
			}
			else
			{
				sprintf(infobuf,"Error!%3d line: is a null line.\n",r);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);
				continue;
			}
		}
		if((filebuf[0] < '0')||(filebuf[0] > '9'))
		{
			sprintf(infobuf,"%s",filebuf);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			continue;
		}
		if(sscanf(filebuf,"%d %s %s %f %f %f %d %s %s",&iTemp,sedef.name[j],sedef.iccp_name[j],
				&sedef.lorange[j],&sedef.hirange[j],&sedef.scale[j],&iTemp1,sedef.interlockname[j],lgname)<=0)
		{
			sprintf(infobuf,"Error!%3d line: %s\n",r,filebuf);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
			j ++;
			continue;
		}
		sedef.cmdtype[j] = iTemp1;

		if(j!=iTemp)
		{
			sprintf(infobuf,"SE %d pnt number=%d wrong!!! %s %s %5.3f %5.3f %5.3f %d %s %s\n",j,iTemp,sedef.name[j],
					sedef.iccp_name[j],sedef.lorange[j],sedef.hirange[j],sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
			printf("%s",infobuf);
			WriteDspInfo(infobuf);
		}

		if(strcmp(sedef.name[j],"NULL")!=0)
		{
			sprintf(infobuf,"SE %d pnt: %s %s %5.3f %5.3f %5.3f %d %s %s\n",j,sedef.name[j],sedef.iccp_name[j],
					sedef.lorange[j],sedef.hirange[j],sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
			strcpy(name_buf2,sedef.name[j]);
			if(ReadEntryByNameStr(name_buf2,&dbentry2)==-1)
			{
				sprintf(infobuf,"#### SE %d pnt: %s No this point in the database file!!!\n",j,name_buf2);
			}
		}
		else
		{
			sprintf(infobuf,"SE %d pnt: NULL %s %5.3f %5.3f %5.3f %d %s %s\n",j,sedef.iccp_name[j],sedef.lorange[j],
					sedef.hirange[j],sedef.scale[j],iTemp1,sedef.interlockname[j],lgname);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
		}
		j ++;
		if(iTemp >= MAX_SE_NUM)
		{
			sprintf(infobuf,"\n!!!GenSETable: Num=%d above the MAX_SE_NUM %d\n",iTemp,MAX_SE_NUM);
			sedef.number = MAX_SE_NUM;
			fclose(fp);
			return(FALSE);
		}
	}
}

void	LanBcCmdError(POINTER ptr,short state)
{
	SW_MSG		bj_buf;
	LAN_HEAD	lanhead;
	struct	tm	tmpt;
	time_t		tim;

	bj_buf.type_id =DPS_IND_ALM;
	bj_buf.stn_id =ptr.stn_id;
	bj_buf.dev_id =ptr.dev_id;
	bj_buf.data_type=ptr.data_type;
	bj_buf.number = 1;
	bj_buf.point_id = ptr.pt_id;

	bj_buf.status.rt_state=state;	/*status: 0:normal; 1:Fail */
	bj_buf.status.chan_fail = 0;
	bj_buf.status.fault_tag = state;
	bj_buf.status.alm_type = 1;
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

#ifdef _BIG_ENDIAN_
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
	strcpy(cmd_buf.usr_login_name,"ICCP");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef _BIG_ENDIAN_
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

	sprintf(infobuf,"Broadcast Cmd Interlock Fail -- type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

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
	strcpy(cmd_buf.usr_login_name,"ICCP");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef _BIG_ENDIAN_
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

	sprintf(infobuf,"Set Point Broadcast Cmd Interlock Fail -- type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

int Get_Cmd_Ctrl_Priv(char * interlockname,unsigned char stat,DMS_COMMON *pdbentry)
{
	char	name_buf[O_NAME_SIZE];

	strcpy(name_buf,interlockname);

	if(InterLock_Judge( name_buf )!=1)
	{
		sprintf(infobuf,"Ctrl condition %s not fulfiled!\n",name_buf);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
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
	char	name_buf[O_NAME_SIZE];

	strcpy(name_buf,interlockname);

	if(InterLock_Judge( name_buf )==0)
	{
		sprintf(infobuf,"Ctrl condition %s not fulfiled!\n",name_buf);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(FALSE);
	}
	return(TRUE);
}

int	ClientMEDataProc (char * iccp_name, float Value, ST_UCHAR Flags)
{
	POINTER		ptr;
	WR_VAR_UNION	value;
	int			d_accum;
	IMP_STATUS	imp_status;
	LAN_HEAD	lanhead;

	unsigned short	infoptr;
	struct tm	tmpt;
	DATE_TIME tm;
	time_t		tim;
	USHORT		year;
#ifdef _BIG_ENDIAN_
	Swap4Byte(&Value);	
#endif	
	printf("########### yyp iccp_name=%s , value=%f, flag=%d\n",iccp_name,Value,Flags );	
	for(infoptr=0;infoptr<medef.number;infoptr++)		/* Check for ME iccp_name */
	{
		//printf("########### yyp iccp_name=%s , medef=%s, infoptr=%d\n",iccp_name,medef.iccp_name[infoptr],infoptr );
		if(strcmp(iccp_name, medef.iccp_name[infoptr])==0)
			break;
	}
	if(infoptr >= medef.number)
	{
		sprintf(infobuf,"Fail to find iccp: client ME '%s' Value = %5.3f stat %02X\n",
			iccp_name, Value, Flags);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	if(medef.ptr[infoptr].dev_id == 0)
	{
		sprintf(infobuf,"+++ No this Client ME point %s %s in the database!\n",medef.name[infoptr],medef.iccp_name[infoptr]);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	tim=time(&tim);
	tmpt = *localtime(&tim);
	GetSysTime(&tm);
	shm_iccp_data_addr->iccp_data[cur_remote_id].me[infoptr].fvalue = Value;
	shm_iccp_data_addr->iccp_data[cur_remote_id].me[infoptr].stat = Flags;
	shm_iccp_data_addr->iccp_data[cur_remote_id].me[infoptr].time = tm;
	ptr = medef.ptr[infoptr];
	
	if(ptr.data_type == IMP_TYPE)
	{
		me_buf[me_num*22] =DPS_IMPULSE;	/* type_id */
		d_accum = Value;	/* d_accum */
		imp_status.option = 0;
		if(Flags & 0x30)
			imp_status.chan_fail = 1;
		else
			imp_status.chan_fail = 0;
	}
	else
	{
		me_buf[me_num*22] =DPS_ANALOG;	/* type_id */
		value.anlg.data = Value*1000.0;
		if(Flags & 0x30)
			value.anlg.status.chan_fail = 1;
		else
			value.anlg.status.chan_fail = 0;
	}
	me_buf[me_num*22+1] =ptr.stn_id;
	me_buf[me_num*22+2] =ptr.dev_id;
	me_buf[me_num*22+3] =ptr.data_type;
	me_buf[me_num*22+4] = 1;		/* number low byte */
	me_buf[me_num*22+5] = 0;		/* number high byte */

#ifdef _BIG_ENDIAN_
	CrossCopy(&me_buf[me_num*22+6], &ptr.pt_id,2);
	if(ptr.data_type == IMP_TYPE)
	{
		CrossCopy(&me_buf[me_num*22+8], &imp_status,2);
		CrossCopy(&me_buf[me_num*22+10], &d_accum,4);
	}
	else
	{
		CrossCopy(&me_buf[me_num*22+8], &value.anlg.status,2);
		CrossCopy(&me_buf[me_num*22+10], &value.anlg.data,4);
	}
#else
	memcpy(&me_buf[me_num*22+6], &ptr.pt_id,2);
	if(ptr.data_type == IMP_TYPE)
	{
		memcpy(&me_buf[me_num*22+8], &imp_status,2);
		memcpy(&me_buf[me_num*22+10], &d_accum,4);
	}
	else
	{
		memcpy(&me_buf[me_num*22+8], &value.anlg.status,2);
		memcpy(&me_buf[me_num*22+10], &value.anlg.data,4);
	}
#endif
	GetLocalHostId(&me_buf[me_num*22+14]);	/* host_id */
	me_buf[me_num*22+15]=tmpt.tm_sec;	/* second */
	me_buf[me_num*22+16]=tmpt.tm_min;	/* minute */
	me_buf[me_num*22+17]=tmpt.tm_hour;	/* hour */
	me_buf[me_num*22+18]=tmpt.tm_mday;	/* day */
	me_buf[me_num*22+19]=tmpt.tm_mon+1;	/* month */
	year=tmpt.tm_year+1900;	/* year */
#ifdef _BIG_ENDIAN_
	CrossCopy(&me_buf[me_num*22+20],&year,2);
#else
	memcpy(&me_buf[me_num*22+20],&year,2);
#endif
	me_num ++;
	if(me_num>=64)
	{
		lanhead.length = me_num*22;
#ifdef _BIG_ENDIAN_
		Swap2Byte(&lanhead.length);
#endif
		lanhead.dp_type=BJ_DB_TIME;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.dest_stn =me_buf[1];
		lanhead.packet_num =0;
		//added by yyp 2011.10.09
		if( chann_priv == CHANPRIV_ONDUTY)
		{
			lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
		}	
		me_num = 0;
	}
	return(TRUE);
}

int	ClientSWDataProc (char * iccp_name, ST_UCHAR Flags)
{
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	IND_DATA	dbStat,status;
	LAN_HEAD	lanhead;

	unsigned short	infoptr;
	struct tm	tmpt;
	time_t		tim;
	USHORT		year;
	//added by yyp 2011.11.08
	return TRUE;
	for(infoptr=0;infoptr<dpdef.number;infoptr++)		/* Check for SW iccp_name */
	{
		if(strcmp(iccp_name, dpdef.iccp_name[infoptr])==0)
			break;
	}
	if(infoptr >= dpdef.number)
	{
		sprintf(infobuf,"Fail to find iccp: client SW '%s' stat %02X\n",iccp_name, Flags);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	if(dpdef.ptr[infoptr].dev_id == 0)
	{
		sprintf(infobuf,"+++ No this Client SW point %s %s in the database!\n",dpdef.name[infoptr],dpdef.iccp_name[infoptr]);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	ptr = dpdef.ptr[infoptr];
	if(ReadEntryById(&ptr,&dbentry)==-1)
	{
		sprintf(infobuf,"+++ No this Client SW point %s %s in the database!\n",dpdef.name[infoptr],dpdef.iccp_name[infoptr]);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	dbStat = dbentry.var.ind.status;
	if(Flags & 0x32)
		status.chan_fail = 1;
	else
		status.chan_fail = 0;
	if(dpdef.doublept[infoptr] == 1)
	{
		if((Flags & 0xC0) == 0x80)
		{
			status.state = status.rt_state = 1;
		}
		else if((Flags & 0xC0) == 0x40)
		{
			status.state = status.rt_state = 0;
		}
		else if((Flags & 0xC0) == 0x00)
		{
			status.state = 2;
			status.rt_state = 0;
		}
		else if((Flags & 0xC0) == 0xC0)
		{
			status.state = 3;
			status.rt_state = 1;
		}
		if((status.state == dbStat.state) && (status.chan_fail == dbStat.chan_fail))
			return(0);
	}
	else
	{
		if((Flags & 0xC0) == 0x80)
			status.rt_state = 1;
		else
			status.rt_state = 0;
		if((status.rt_state == dbStat.rt_state) && (status.chan_fail == dbStat.chan_fail))
			return(0);
	}
	tim=time(&tim);
	tmpt = *localtime(&tim);

	status.fault_tag = 0;
	status.alm_type = 1;

	sw_buf[sw_num].type_id =DPS_IND_ALM;
	sw_buf[sw_num].stn_id =dbentry.point.stn_id;
	sw_buf[sw_num].dev_id =dbentry.point.dev_id;
	sw_buf[sw_num].data_type=dbentry.point.data_type;
	sw_buf[sw_num].number = 1;
	sw_buf[sw_num].point_id =dbentry.point.pt_id;

	sw_buf[sw_num].status=status;
	sw_buf[sw_num].ms=0;
	GetLocalHostId(&sw_buf[sw_num].host_id);

	sw_buf[sw_num].sec=tmpt.tm_sec;
	sw_buf[sw_num].min=tmpt.tm_min;
	sw_buf[sw_num].hour=tmpt.tm_hour;
	sw_buf[sw_num].day=tmpt.tm_mday;
	sw_buf[sw_num].month=tmpt.tm_mon+1;
	sw_buf[sw_num].year=tmpt.tm_year+1900;

#ifdef _BIG_ENDIAN_
	Swap2Byte(&sw_buf[sw_num].number);
	Swap2Byte(&sw_buf[sw_num].point_id);
	Swap2Byte(&sw_buf[sw_num].status);
	Swap2Byte(&sw_buf[sw_num].ms);
	Swap2Byte(&sw_buf[sw_num].year);
#endif
	sw_num ++;
	if( sw_num >= 70 )
	{
		lanhead.length = sw_num*sizeof(SW_MSG);
#ifdef _BIG_ENDIAN_
		Swap2Byte(&lanhead.length);
#endif
		lanhead.dp_type=BJ_DB_TIME;
		lanhead.dest_stn =sw_buf[0].stn_id;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.packet_num =0;
		lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
		sw_num = 0;
	}
	return(TRUE);
}

//added by yyp 2011.10.07
int	ClientStateFromSOEProc (int buf_Size, ST_UCHAR *info_buf)
{
	DMS_COMMON	dbentry;
	POINTER 	ptr;
	IND_DATA	dbStat,status;
	LAN_HEAD	lanhead;
	ST_UCHAR	Flags;
	unsigned short	infoptr;
	struct tm	tmpt;
	time_t		tim;
	USHORT		year;
	SOEREC		rec;

	printf("#############yyp  ClientStateFromSOEProc() \n");
	//added by yyp 2011.10.09

	if( buf_Size != sizeof(SOEREC) )
	{
		sprintf(infobuf,"ClientStateFromSOEProc(): buf_size =%d is not equal to %d error\n",buf_Size,sizeof(SOEREC));
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}

	memcpy(&rec,info_buf,sizeof(SOEREC));
#ifdef _BIG_ENDIAN_
	Swap2Byte(&rec.infoaddr);
	Swap2Byte(&rec.stvalue);
	Swap4Byte(&rec.fvalue);
	Swap2Byte(&rec.time.ms);
	Swap2Byte(&rec.time.year);
#endif	
	infoptr = rec.infoaddr ;
	Flags = rec.stat;

	printf("########ClientStateFromSOEProc(): Flags=%d, infoaddr=%d\n",Flags,infoptr);

	if(infoptr >= dpdef.number)
	{
		sprintf(infobuf,"Fail to find iccp: client SW point infoptr = %d \n",infoptr);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	if(dpdef.ptr[infoptr].dev_id == 0)
	{
		sprintf(infobuf,"+++ No this Client SW point %s in the database!\n",dpdef.name[infoptr]);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	shm_iccp_data_addr->iccp_data[cur_remote_id].dp[infoptr].stat = Flags;
	shm_iccp_data_addr->iccp_data[cur_remote_id].dp[infoptr].time = rec.time;
	ptr = dpdef.ptr[infoptr];
	if(ReadEntryById(&ptr,&dbentry)==-1)
	{
		sprintf(infobuf,"+++ No this Client SW point %s %s in the database!\n",dpdef.name[infoptr],dpdef.iccp_name[infoptr]);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	dbStat = dbentry.var.ind.status;
	if(Flags & 0x32)
		status.chan_fail = 1;
	else
		status.chan_fail = 0;
	if(dpdef.doublept[infoptr] == 1)
	{
		
		
		if((Flags & 0xC0) == 0x80)
		{
			status.state = status.rt_state = 1;
		}
		else if((Flags & 0xC0) == 0x40)
		{
			status.state = status.rt_state = 0;
		}
		else if((Flags & 0xC0) == 0x00)
		{
			status.state = 2;
			status.rt_state = 0;
		}
		else if((Flags & 0xC0) == 0xC0)
		{
			status.state = 3;
			status.rt_state = 1;
		}
		
		//status.state = rec.stat;
		if((status.state == dbStat.state) && (status.chan_fail == dbStat.chan_fail))
			return(0);
	}
	else
	{
		/*
		if((Flags & 0xC0) == 0x80)
			status.rt_state = 1;
		else
			status.rt_state = 0;
		*/
		status.state = rec.stat;
		if((status.rt_state == dbStat.rt_state) && (status.chan_fail == dbStat.chan_fail))
			return(0);
	}
	tim=time(&tim);
	tmpt = *localtime(&tim);

	status.fault_tag = 0;
	status.alm_type = 1;

	sw_buf[sw_num].type_id =DPS_IND_ALM;
	sw_buf[sw_num].stn_id =dbentry.point.stn_id;
	sw_buf[sw_num].dev_id =dbentry.point.dev_id;
	sw_buf[sw_num].data_type=dbentry.point.data_type;
	sw_buf[sw_num].number = 1;
	sw_buf[sw_num].point_id =dbentry.point.pt_id;

	sw_buf[sw_num].status=status;
	sw_buf[sw_num].ms=rec.time.ms;
	GetLocalHostId(&sw_buf[sw_num].host_id);

	sw_buf[sw_num].sec=rec.time.second;
	sw_buf[sw_num].min=rec.time.minute;
	sw_buf[sw_num].hour=rec.time.hour;
	sw_buf[sw_num].day=rec.time.day;
	sw_buf[sw_num].month=rec.time.month;
	sw_buf[sw_num].year=rec.time.year;



#ifdef _BIG_ENDIAN_
	Swap2Byte(&sw_buf[sw_num].number);
	Swap2Byte(&sw_buf[sw_num].point_id);
	Swap2Byte(&sw_buf[sw_num].status);
	Swap2Byte(&sw_buf[sw_num].ms);
	Swap2Byte(&sw_buf[sw_num].year);
#endif
	sw_num ++;
	//needed to be modified by yyp 2011.10.9
	if( sw_num >= 1 )
	//if( sw_num >= 70 )
	{
		lanhead.length = sw_num*sizeof(SW_MSG);
#ifdef _BIG_ENDIAN_
		Swap2Byte(&lanhead.length);
#endif
		lanhead.dp_type=BJ_DB_TIME;
		lanhead.dest_stn =sw_buf[0].stn_id;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.packet_num =0;
		if( chann_priv != CHANPRIV_ONDUTY)
		{
			lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
		}

		sw_num = 0;
	}
	return(TRUE);
}

int	ClientDataBD (void)
{
	LAN_HEAD	lanhead;
	
	if( sw_num > 0 )
	{
		lanhead.length = sw_num*sizeof(SW_MSG);
#ifdef _BIG_ENDIAN_
		Swap2Byte(&lanhead.length);
#endif
		lanhead.dp_type=BJ_DB_TIME;
		lanhead.dest_stn =sw_buf[0].stn_id;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.packet_num =0;
		//removed by yyp 2011.10.09
		if( chann_priv == CHANPRIV_ONDUTY)
		{
			lan_out ((char *)&sw_buf,lanhead,(unsigned char)0);
		}
		sw_num = 0;
	}
	if(me_num > 0)
	{
		lanhead.length = me_num*22;
#ifdef _BIG_ENDIAN_
		Swap2Byte(&lanhead.length);
#endif
		lanhead.dp_type=BJ_DB_TIME;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.dest_stn =me_buf[1];
		lanhead.packet_num =0;
		if( chann_priv == CHANPRIV_ONDUTY)
		{		
			lan_out ((char *)&me_buf,lanhead,(unsigned char)0);
		}
		me_num = 0;
	}
	return(TRUE);
}

int	GWRtuAllSend(void)
{
	unsigned short  j;
	LCUID           lcuno;
	unsigned int 	point_num;
	IND_STRUCT 	*msg_db;
	IND_DATA	*msg_all;
	ANA_STRUCT	*ana_db;
	AIALLSTRUCT	*ai_all;
	DATE_TIME	tm;

   	if(-1==GetLocalHostStnId(&lcuno.stnid))
		return (-1);

	lcuno.stnid = GW_DEVID;

	GetSysTime(&tm);
        
	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, ANA_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(ana_db=malloc(point_num*sizeof(ANA_STRUCT))))
			return (-1);
		if(NULL==(ai_all=malloc(point_num*sizeof(AIALLSTRUCT))))
		{
			free(ana_db);
			return (-1);
		}

		ReadDb_ANA(lcuno, ANA_TYPE, 0, (USHORT)point_num, ana_db);

		for(j=0;j<point_num;j++)
		{
			ai_all[j].data = ana_db[j].data;
			ai_all[j].status = ana_db[j].status;
		}
		
		AllSend_ANA(lcuno, DPS_ANA_ALL, ANA_TYPE, 0, (USHORT)point_num, ai_all, tm);

		free(ana_db);
		free(ai_all);
	}	

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, POL_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(msg_db=malloc(point_num*sizeof(IND_STRUCT))))
			return (-1);
		if(NULL==(msg_all=malloc(point_num*sizeof(IND_DATA))))
		{
			free(msg_db);
			return (-1);
		}

		ReadDb_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_db);

		for(j=0;j<point_num;j++)
			msg_all[j]= msg_db[j].status;
		
		AllSend_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_all, tm);

		free(msg_db);
		free(msg_all);
	}
	return(TRUE);
}
	
time_t CP56CmdTimeProc(unsigned char *recv_buf)
{
	time_t cmd_time;
	unsigned short ms;
	struct tm	sTimeCmd;

	ms = *recv_buf + (*(recv_buf+1))*256;
	sTimeCmd.tm_sec = ms/1000;
	sTimeCmd.tm_min = (*(recv_buf+2)) & 0x3f;
	sTimeCmd.tm_hour = (*(recv_buf+3)) & 0x1f;
	sTimeCmd.tm_mday = (*(recv_buf+4)) & 0x1F;
	sTimeCmd.tm_mon = (*(recv_buf+5)) & 0x0F - 1;
	sTimeCmd.tm_year =  *(recv_buf+6);
	if ( sTimeCmd.tm_year <= 80 )
		sTimeCmd.tm_year += 100;
	sTimeCmd.tm_isdst = 0;

	sprintf(infobuf,"Remote clock %04d/%02d/%02d %02d:%02d:%02d %03d!\n",sTimeCmd.tm_year+1900,sTimeCmd.tm_mon+1,
			sTimeCmd.tm_mday,sTimeCmd.tm_hour,sTimeCmd.tm_min,sTimeCmd.tm_sec,ms%1000);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

	cmd_time = mktime(&sTimeCmd);
	return(cmd_time);
}


void WriteDspInfo(char *buf)
{
	int	bufptr,len,charlen;
	//modified by yyp 2011.10.09
	//printf("%s\n",buf);
	return;
	if ((long *)shm_buf_addr==(long *)-1)
		return;
	bufptr = shm_buf_addr->header_ptr;
	len = strlen(buf);
	charlen = DSP_INFO_SIZE - bufptr;
	if(charlen < len)
	{
		memcpy(&shm_buf_addr->buf[bufptr],buf,charlen);
		bufptr = len - charlen;
		memcpy(&shm_buf_addr->buf[0],&buf[charlen],bufptr);
		shm_buf_addr->header_ptr = bufptr;
	}
	else
	{
		memcpy(&shm_buf_addr->buf[bufptr],buf,len);
		charlen  = bufptr + len;
		if(charlen >= DSP_INFO_SIZE)
			shm_buf_addr->header_ptr = 0;
		else
			shm_buf_addr->header_ptr = charlen;
	}
}




//added by yyp 2011.03.09
long GetTimefromCmd(struct net_cmd_data *p_rec)
{
	struct tm tm_time;
	tm_time.tm_year=p_rec->year-1900;
	tm_time.tm_mon=p_rec->month-1;
	tm_time.tm_mday=p_rec->day;
	tm_time.tm_hour=p_rec->hour;
	//added by yyp 2011.10.09 for aix
#ifdef _AIX
	tm_time.tm_hour=tm_time.tm_hour-1;
#endif
	tm_time.tm_min=p_rec->minute;
	tm_time.tm_sec=p_rec->second;
	tm_time.tm_isdst= 0;
	return mktime( &tm_time );
}




int ICCPDeviceOperateCmd(int nDst,char *iccp_s_devName, ST_INT16 cmdstat)
{
	int		infoptr;
	char		dbname[36];
	short		cmd_stat;
	char		cmdtype;
	unsigned char	opstat;

	DMS_COMMON	dbcommpt;
	struct	tm	tmpt;
	time_t		tim;

	for(infoptr = 0; infoptr < scdef.number; infoptr++)
	{
		if(strcmp(iccp_s_devName, scdef.iccp_name[infoptr])==0)
			break;
	}
	if(infoptr >= scdef.number)
	{
		sprintf(infobuf,"Fail to find iccp:-TASE2 command '%s' OperateValue = %d in %s file\n",
			iccp_s_devName, cmdstat,SCDEF_FILE);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}

	strcpy(dbname,scdef.name[infoptr]);
	sprintf(infobuf,"\n######iccp: - TASE2 received command '%s' %s OperateValue = %d\n",
			iccp_s_devName, dbname, cmdstat);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

	if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
	{
		sprintf(infobuf,"%s No this SC control point in the database file!\n",dbname);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(false);
	}

	if((cmdstat & 0x1)==1)
		opstat = 1;
	else
		opstat = 0;

	if(cmdstat & 0x1)
		cmd_stat = scdef.optype2[infoptr];
	else
		cmd_stat = scdef.optype1[infoptr];

	cmdtype = scdef.cmdtype[infoptr];
	if(Get_Cmd_Ctrl_Priv(scdef.interlockname[infoptr],opstat,&dbcommpt) == FALSE)
	{
		CmdInterlockFailBroadcast(cmdtype,cmd_stat,&dbcommpt);
		sprintf(infobuf,"!!!!!!iccp: -- The Control Condition %s for cmd %s not fulfiled!\n",scdef.interlockname[infoptr],scdef.name[infoptr]);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
/* 	Execute Command */
	ComdSendToLCU(cmdtype,cmd_stat,&dbcommpt);

	sprintf(infobuf,"iccp command %s dbname=%s cmd=%d stat=%d has send out\n",
			iccp_s_devName,dbname,cmdtype,cmd_stat);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

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
		sprintf(infobuf,"iec104s:can't open(wb) the file %s!\n",cmdrecfile);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	else
	{
		fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d iccp command %s dbname=%s cmd=%d stat=%d received\n",
			tmpt.tm_year+1900,tmpt.tm_mon+1,tmpt.tm_mday,tmpt.tm_hour,tmpt.tm_min,tmpt.tm_sec,
			iccp_s_devName,dbname,cmdtype,cmd_stat);
		fclose(fagc);
	}
#endif
	return(true);
}

int ICCPSetPointCmd(int nDst,char *iccp_s_devName, float setValue)
{

	unsigned short	infoaddr,ms,year,datalength,lengthfactor;
	int		infoptr;
	char		dbname[36];
	float		value;
	short		cmd_stat;
	char		cmdtype;

	DMS_COMMON	dbcommpt;
	int		i,datapoint,data_type;

	struct timeval	sTimeVal;
	struct	tm	tmpt;
	time_t		tim;

	for(infoptr = 0; infoptr < sedef.number; infoptr++)
	{
		if(strcmp(iccp_s_devName, sedef.iccp_name[infoptr])==0)
			break;
	}
	if(infoptr >= sedef.number)
	{
		sprintf(infobuf,"Fail to find iccp:-TASE2 command '%s' SetValue = %5.3f in %s file\n",
			iccp_s_devName, setValue,SEDEF_FILE);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(-1);
	}
	strcpy(dbname,sedef.name[infoptr]);
	if(ReadEntryByNameStr(dbname,&dbcommpt)==-1)
	{
		sprintf(infobuf,"ICCPSetPointCmd:%s No this set point in the database file!\n",dbname);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);

		return(false);
	}

	value = setValue;
	cmdtype = sedef.cmdtype[infoptr];

	if(Get_Setpt_Ctrl_Priv(sedef.interlockname[infoptr]) == FALSE)
	{
		SECmdInterlockFailBroadcast(cmdtype, value, &dbcommpt);
		sprintf(infobuf,"!!!!!!iccp: -- The Control Condition %s for Setpnt %s not fulfiled!\n",sedef.interlockname[infoptr],sedef.name[infoptr]);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(false);
	}

	if((dbcommpt.point.data_type != ANA_TYPE)&&(dbcommpt.point.data_type != PARAM_TYPE))
	{
		sprintf(infobuf,"ICCPSetPointCmd:%s this point is not ANA in the database file!\n",dbname);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		return(false);
	}

	SEComdSendToLCU(cmdtype, value, &dbcommpt);

	sprintf(infobuf,"#####ICCP Set-Point: %s dbname=%s cmd=%d  fvalue= %5.3f Has Send Out\n",iccp_s_devName,dbname,cmdtype,value);
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

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
		sprintf(infobuf,"iec104s:can't open(wb) the file %s!\n",cmdrecfile);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
	}
	else
	{
		fprintf (fagc,"%4d/%02d/%02d %02d:%02d:%02d ICCP Set-Point: %s dbname=%s cmd=%d  fvalue= %5.3f\n",tmpt.tm_year+1900,
			tmpt.tm_mon+1,tmpt.tm_mday,	tmpt.tm_hour,tmpt.tm_min,
			tmpt.tm_sec,iccp_s_devName,dbname,cmdtype,value);
		fclose(fagc);
	}
#endif
	return(true);
}

void ICCP_SOE_Init()
{
	char	name_buf[NAME_SIZE];
	unsigned char	stat;
	unsigned short	pntnum;
	POINTER 	ptr;

	/*	获取记录区共享内存地址  */
	shm_rec_addr= (SHM_RECORD*) LinktoShareMem( SHM_REC_KEY_CHAR );
	if( shm_rec_addr == (SHM_RECORD*)-1 )
	{
		sprintf(infobuf,"\niccp: Fail to shmat shm_rec_addr: errno=%d\n",errno);
		printf("%s",infobuf);
		WriteDspInfo(infobuf);
		exit(-1);
	}
	rcv_end = shm_rec_addr->head.header_ptr;

	sprintf(infobuf,"\niccp:: shmat shm_rec_addr: OK!\n");
#ifdef DEBUG
	printf("%s",infobuf);
#endif
	WriteDspInfo(infobuf);

	/* Update orgbuf for DP status Change */
	for(pntnum=0;pntnum<dpdef.number;pntnum++)
	{
		if(dpdef.ptr[pntnum].dev_id == 0)
		{
			orgdpsoebuf[pntnum] = 0;
			continue;
		}
		ptr = dpdef.ptr[pntnum];
		if(dpdef.doublept[pntnum] == 1)
		{
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
		else
		{
			if(GetSPStatById(&ptr,&stat)==-1)
			{
				orgdpsoebuf[pntnum] = 0;
				continue;
			}
			else
			{
				orgdpsoebuf[pntnum] = stat;
			}
		}
	}
}

void ICCP_SOE_Rec(void)
{
//#ifdef BY_RECORD
/*
	SHM_RECORD *shm_rec_addr;
	int shm_id_rec;
*/
	IND_REC	*pInd_alm;
	unsigned char	type_id;
	short	rcv_pnt;
	POINTER	rcv_ptr;
	POINTER	ptr;
	int	cmd_type;
//#endif
	int		data_type;
	char		name_buf[NAME_SIZE];
	DMS_COMMON	dbentry;

	unsigned char	stat;
	unsigned short	pntnum,soe_head;
	IND_DATA	dbstat;

	/* Check for SOE buffer */
#ifndef BY_RECORD
/*	for(;;) */
	{
		for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* Check for DP status Change */
		{
			if(dpdef.TimeTag[pntnum] == 0)
				continue;
			if(dpdef.ptr[pntnum].dev_id == 0)
			{
				continue;
			}
			ptr = dpdef.ptr[pntnum];
			if(dpdef.doublept[pntnum] == 1)
			{
				if(GetDPStatById(&ptr,&stat)==-1)
				{
					continue;
				}
			}
			else
			{
				if(GetSPStatById(&ptr,&stat)==-1)
				{
					continue;
				}
			}
			if(stat != orgdpsoebuf[pntnum])
			{
				soe_head = soebuf.bufpnt;
				soebuf.soerec[soe_head].nDst = 0;

				soebuf.soerec[soe_head].infoaddr = pntnum;
				soebuf.soerec[soe_head].stat = stat;				/* DPI RES1 IV RES4 */
				orgdpsoebuf[pntnum] = stat;
				soebuf.soerec[soe_head].time = dbentry.var.ind.change_time;
				soebuf.soerec[soe_head].time.year = soebuf.soerec[soe_head].time.year-1900;

				sprintf(infobuf,"DP soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
					name_buf,soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
					soebuf.soerec[soe_head].infoaddr,
					soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
					soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
					soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
					soebuf.soerec[soe_head].time.ms);
#ifdef DEBUG
				printf("%s",infobuf);
#endif
				WriteDspInfo(infobuf);

				soe_head ++;
				if(soe_head >= SOE_NUM)
					soe_head = 0;
				soebuf.bufpnt = soe_head;
			}
		}
	}
#else		
/*	for(;;) */
	{
		while( rcv_end!=shm_rec_addr->head.header_ptr )
		{
			rcv_pnt = rcv_end;
			rcv_end++;
			if(rcv_end >= MAX_SHM_RECORD_NUM)
				rcv_end = 0;

			pInd_alm = &shm_rec_addr->rec[rcv_pnt].ind_rec;
			type_id = pInd_alm->type_id;
/*
			if((type_id != DPS_IND_ALM) && (type_id != DPS_MAN_STAT_SET) && (rcv_ptr.data_type != C_SYS_MODE_SET))
*/
			if(type_id != DPS_IND_ALM)
			{
				continue;
			}
			rcv_ptr.stn_id = pInd_alm->stn_id;
			rcv_ptr.dev_id = pInd_alm->dev_id;
			rcv_ptr.data_type = pInd_alm->data_type;
			rcv_ptr.pt_id = pInd_alm->pt_id;
/*
			sprintf(infobuf,"\niec104s::Receive a record! %d=%d=%d=%d\n",rcv_ptr.stn_id,rcv_ptr.dev_id,rcv_ptr.data_type,rcv_ptr.pt_id);
#ifdef DEBUG
			printf("%s",infobuf);
#endif
			WriteDspInfo(infobuf);
*/
			for(pntnum=0;pntnum<dpdef.number;pntnum++)		/* Check for DP status Change */
			{
				if(dpdef.TimeTag[pntnum] == 0)
					continue;
				ptr = dpdef.ptr[pntnum];
				if((ptr.stn_id == rcv_ptr.stn_id)&&(ptr.dev_id == rcv_ptr.dev_id)&&
					(ptr.data_type == rcv_ptr.data_type)&&(ptr.pt_id == rcv_ptr.pt_id))
				{
					dbstat = pInd_alm->ind_status;
					if(dpdef.doublept[pntnum] == 1)
					{
						if(dbstat.rt_state == 0)
							stat = 0x40;	/* OFF:DPI RES1 IV RES4 */
						else if(dbstat.rt_state == 1)
							stat = 0x80;	/* ON: DPI RES1 IV RES4 */
						else if(dbstat.rt_state == 2)
							stat = 0;	/* Transit:DPI RES1 IV RES4 */
						else
							stat = 0xC0;	/* Error:  DPI RES1 IV RES4 */
					}
					else
					{
						if(dbstat.rt_state == 1)
							stat = 0x80;	/* OFF:DPI RES1 IV RES4 */
						else
							stat = 0x40;	/* ON: DPI RES1 IV RES4 */
					}
/*					if(dbstat.man_set)
						stat = stat | 0x20;	*/		/* SB=1 */
					if(dbstat.chan_fail)
						stat = stat | 0x10;			/* IV=1;*/

					soe_head = soebuf.bufpnt;
					soebuf.soerec[soe_head].nDst = 0;

					soebuf.soerec[soe_head].infoaddr = pntnum;
					soebuf.soerec[soe_head].stat = orgdpsoebuf[pntnum] = stat;	/* DPI RES1 IV RES4 */
					soebuf.soerec[soe_head].time.ms = pInd_alm->ms;
					soebuf.soerec[soe_head].time.second = pInd_alm->second;
					soebuf.soerec[soe_head].time.minute = pInd_alm->minute;
					soebuf.soerec[soe_head].time.hour = pInd_alm->hour;
					soebuf.soerec[soe_head].time.day = pInd_alm->day;
					soebuf.soerec[soe_head].time.month = pInd_alm->month;
					soebuf.soerec[soe_head].time.year = pInd_alm->year;

					sprintf(infobuf,"soe: %s stat=%02x infoaddr=%d %04x %d/%d/%d %d:%d:%d %d\n",
						dpdef.name[pntnum],soebuf.soerec[soe_head].stat,soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].infoaddr,
						soebuf.soerec[soe_head].time.year,soebuf.soerec[soe_head].time.month,
						soebuf.soerec[soe_head].time.day,soebuf.soerec[soe_head].time.hour,
						soebuf.soerec[soe_head].time.minute,soebuf.soerec[soe_head].time.second,
						soebuf.soerec[soe_head].time.ms);
#ifdef DEBUG
					printf("%s",infobuf);
#endif
					WriteDspInfo(infobuf);
						
					soe_head ++;
					if(soe_head >= SOE_NUM)
						soe_head = 0;
					soebuf.bufpnt = soe_head;
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
	strcpy(cmd_buf.usr_login_name,"ICCP");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef _BIG_ENDIAN_
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

		sprintf(infobuf,"Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		sprintf(infobuf,"Cmd lanout type_id:%d %d=%d=%d=%d %d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
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
	strcpy(cmd_buf.usr_login_name,"ICCP");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef _BIG_ENDIAN_
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

		sprintf(infobuf,"Set Point Broadcast Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	else
	{
		lanhead.dest_id = cmd_buf.dev_id;
		lanhead.dest_stn = cmd_buf.stn_id;
		lanhead.dp_type=COMM_TIME;

		sprintf(infobuf,"Set Point Cmd lanout type_id:%d %d=%d=%d=%d %d data=%d ctrl_addr:%d\n",cmd_buf.type_id,cmd_buf.stn_id,cmd_buf.dev_id,
			cmd_buf.data_type,cmd_buf.point_id,cmd_buf.status.opr,cmd_buf.data.iValue,cmd_buf.ctrl_addr);
#ifdef DEBUG
		printf("%s",infobuf);
#endif
		WriteDspInfo(infobuf);
	}
	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}

/****** map iccp shared memory added by yyp on 2011.10.12 ******/
int get_iccp_data_shm()
{
	int	shm_id_iccp;
	char	keyfile[128];
	
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);
	shm_iccp_data_addr=&data_iccp;
	if((shm_id_iccp=shmget (ftok(keyfile,(char )SHM_DATA_KEY_CHAR_ICCP),SHM_ICCP_DATA_SIZE,0))<0)
	{
		printf("can't get iccp data shared memory ,size=%d\n",SHM_ICCP_DATA_SIZE);
		return(FALSE);
	}
	shm_iccp_data_addr=(struct shm_iccp_data *)shmat (shm_id_iccp,0,SHM_RND);
	if((long *)shm_iccp_data_addr==(long *)-1)
	{
		printf("can't attach iccp data shared memory\n");
		return(FALSE);
	}
	return(TRUE);
}

