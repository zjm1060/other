/* calcufn.c program lib
   cc -c calcufn.c
*/
/*
#define DEBUG
*/
#define MINUTE_CLEAR

#ifdef SUN_OS
#include<unistd.h>
#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include <sys/time.h>
#else
#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include <time.h>
#include <string.h>
#endif

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../pas/v4.0a/inc/calcugen.h"
#include "../../../pas/v4.0a/inc/pdc_def.h"
#include "../../../lan/v4.0a/inc/lan_common.h"

static	char	genfile[2][256];

int     CalcuFileLoad(short i);
int     Wdb(short i, char *name,IND_DATA status);
void    Calcu_Judge(short i, short numb);
void    WFdb(short i, ANLG_STATUS stat_flag, float value1, char *name);
void    Calcu_Dat(short i, short numb);
void    Calcu_Diff (short i, short numb);
void	Clear_Db(short i, short numb);
void	CalcuFileFree(short i);
void	CalcuFnts(short i);
void	*CalcuFn(PDC_TASK *pTask);
int	BracketLogic(short i, short numb, int start_gnum, int end_gnum);
BOOL	search(char str1[],char str2[]);
double	TransformToDouble(float value);

static	char		*tmp_float=	"TMP_FLOAT";
static	char		*tmp_status=	"TMP_STATUS";

static	float		float_tmp[2][1000];
static	ANLG_STATUS	fstat_tmp[2][1000];
static	IND_DATA	status_tmp[2][1000];

static	short		pt_num[2];
static	short		gt_num[2];
static	short		et_num[2];

static	Calcu		*calcu[2];
static	C_Group		*group[2];
static	C_Entry		*entry[2];

static	time_t		tim;
static	struct tm	tmpt;

static	unsigned char	me_buf[2][1440];
static	SW_MSG		sw_buf[2][70];
static	short		me_num[2],sw_num[2];
static	int		ana_all_flag[2];
static	USHORT		HostStat;

int	CalcuFileLoad(short fnum)
{
	FILE	*fp;
	int		i;
	char	verstr[VERLEN+1];
	short	num;

	i = fnum;
	if(i==0)
		sprintf(genfile[0], "%s%s", SYS_DATA_PATH, CALCU_DATA_FILE);
	else if(i==1)
		sprintf(genfile[1], "%s%s", SYS_DATA_PATH, CALCU1_DATA_FILE);
	else
		return -1;

	if ((fp=fopen(genfile[i],"rb" ))==NULL)
	{
		SendErrMsg(0,"calcufn.c:can't open(wb) the file %s!\n",genfile[i]);
		return -1;
	}
	if (fread(&verstr[0],VERLEN,1,fp)!=1)
	{
		SendErrMsg(0,"calcufn.c:file %s read verstr error\n",genfile[i]);
		return -1;
	}
	if(strncmp(verstr,VERSTR,strlen(VERSTR))!=0)
	{
		SendErrMsg(0,"calcufn.c:file %s VERSION %s error!\n",genfile[i],verstr);
		return -1;
	}
#ifdef DEBUG
	SendErrMsg(0,"calcufn.c:file %s VERSION: %s\n",genfile[i],verstr);
#endif
	if (fread(&pt_num[i],sizeof(short),1,fp)!=1)
	{
		SendErrMsg(0,"calcufn.c:file %s read pt_num error\n",genfile[i]);
		return -1;
	}
	if (fread(&gt_num[i],sizeof(short),1,fp)!=1)
	{
		SendErrMsg(0,"calcufn.c:file %s read gt_num %d error\n",genfile[i],gt_num[i]);
		return -1;
	}
	if (fread(&et_num[i],sizeof(short),1,fp)!=1)
	{
		SendErrMsg(0,"calcufn.c:file %s read et_num error\n",genfile[i]);
		return -1;
	}

	Swap2Byte(&pt_num[i]);
	Swap2Byte(&gt_num[i]);
	Swap2Byte(&et_num[i]);

	SendErrMsg(0,"calcufn.c: pt_num:%d gt_num:%d et_num:%d\n",pt_num[i],gt_num[i],et_num[i]);
	
	calcu[i]=(Calcu *)calloc( pt_num[i],sizeof (Calcu));
	if (calcu[i]==NULL && pt_num[i]!=0)//cxs add for aix
	{
		SendErrMsg(0,"calcufn.c:calcu calloc( pt_num[%d]) error\n",i);
		return -1;
	}
	if (fread(calcu[i],sizeof (Calcu),pt_num[i],fp)!=(unsigned int)pt_num[i])
	{
		SendErrMsg(0,"calcufn.c:file %s read calcu error\n",genfile[i]);
		return -1;
	}

	group[i]=(C_Group *)calloc(gt_num[i],sizeof (C_Group));
	if (group[i]==NULL && gt_num[i]!=0)//cxs add for aix
	{
		SendErrMsg(0,"calcufn.c: group *calloc error\n" );
		return -1;
	}
	if (fread(group[i],sizeof (C_Group),gt_num[i],fp)!=(unsigned int)gt_num[i])
	{
		SendErrMsg(0,"calcufn.c:file %s read group error\n",genfile[i]);
		return -1;
	}

	entry[i]=(C_Entry *)calloc(et_num[i],sizeof (C_Entry));
	if (entry[i]==NULL && et_num[i]!=0)//cxs add for aix
	{
		SendErrMsg(0,"calcufn.c: entry *calloc error\n" );
		return -1;
	}
	if (fread(entry[i],sizeof (C_Entry),et_num[i],fp)!=(unsigned int)et_num[i])
	{
		SendErrMsg(0,"calcufn.c:file %s read entry error\n",genfile[i]);
		return -1;
	}

	fclose(fp);
	for( num=0; num < pt_num[i]; num++ )
	{
		Swap2Byte(&calcu[i][num].type);
		Swap2Byte(&calcu[i][num].g_num);
		Swap2Byte(&calcu[i][num].offset);
	}
	for( num=0; num < gt_num[i]; num++ )
	{
		Swap2Byte(&group[i][num].layer);
		Swap2Byte(&group[i][num].e_num);
		Swap2Byte(&group[i][num].offset);
	}
	for( num=0; num < et_num[i]; num++ )
	{
		Swap2Byte(&entry[i][num].opr);
		Swap2Byte(&entry[i][num].layer);
		if(entry[i][num].opr == BRACKET_F)
		{
			Swap2Byte(&entry[i][num].data.group.g_num);
			Swap2Byte(&entry[i][num].data.group.offset);
		}
		else
			Swap4Byte(&entry[i][num].data.value);
	}

	memset(&float_tmp[0][0],0,sizeof(float_tmp));
	memset(&fstat_tmp[0][0],0,sizeof(fstat_tmp));
	memset(&status_tmp[0][0],0,sizeof(status_tmp));
	printf("sizeof(float_tmp):%d; sizeof(fstat_tmp):%d;sizeof(status_tmp):%d\n",
		sizeof(float_tmp), sizeof(fstat_tmp), sizeof(status_tmp));
	return 0;
}

int	Wdb(short i, char *name,IND_DATA status)
{
	char		name_buf[POINT_NAME_SIZE];
	DMS_COMMON	dbcommpt;
	IND_DATA	dbStat;
	LAN_HEAD	lanhead;

	WR_VAR_UNION	new_value;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	DATE_TIME	update_time;
	UCHAR		Stnid, SysLCUid;
	int		ret;

	strcpy (name_buf,name);
	if(ReadEntryByNameStr(name_buf, &dbcommpt)==-1)
	{
		SendErrMsg(0,"\ncalcufn: %s No this point in the database when Wdb!\n",name_buf);
		return(-1);
	}
	Stnid =dbcommpt.point.stn_id;
	GetStnSysLcuIdById( Stnid, &SysLCUid );
	if(dbcommpt.point.dev_id == SysLCUid)
	{
		status.chan_fail = 0;
	}
	status.fault_tag = 0;
	status.alm_type = 1;
	new_value.ind = status;

	if(HostStat == 1)
	{
		dbStat = dbcommpt.var.ind.status;
/*
		if((status.rt_state != dbStat.rt_state) || (status.chan_fail != dbStat.chan_fail))
*/
		if(status.rt_state != dbStat.rt_state)
		{
			sw_buf[i][sw_num[i]].type_id =DPS_IND_ALM;
			sw_buf[i][sw_num[i]].stn_id =dbcommpt.point.stn_id;
			sw_buf[i][sw_num[i]].dev_id =dbcommpt.point.dev_id;
			sw_buf[i][sw_num[i]].data_type=dbcommpt.point.data_type;
			sw_buf[i][sw_num[i]].number = 1;
			sw_buf[i][sw_num[i]].point_id =dbcommpt.point.pt_id;

			sw_buf[i][sw_num[i]].status=status;
			sw_buf[i][sw_num[i]].ms=0;
			GetLocalHostId(&sw_buf[i][sw_num[i]].host_id);

			sw_buf[i][sw_num[i]].sec=tmpt.tm_sec;
			sw_buf[i][sw_num[i]].min=tmpt.tm_min;
			sw_buf[i][sw_num[i]].hour=tmpt.tm_hour;
			sw_buf[i][sw_num[i]].day=tmpt.tm_mday;
			sw_buf[i][sw_num[i]].month=tmpt.tm_mon+1;
			sw_buf[i][sw_num[i]].year=tmpt.tm_year+1900;

			Swap2Byte(&sw_buf[i][sw_num[i]].number);
			Swap2Byte(&sw_buf[i][sw_num[i]].point_id);
			Swap2Byte(&sw_buf[i][sw_num[i]].status);
			Swap2Byte(&sw_buf[i][sw_num[i]].ms);
			Swap2Byte(&sw_buf[i][sw_num[i]].year);

			sw_num[i] ++;
			if( sw_num[i] >= 70 )
			{
				lanhead.length = sw_num[i]*sizeof(SW_MSG);
				Swap2Byte(&lanhead.length);
				lanhead.dp_type=BJ_DB_TIME;
				lanhead.dest_stn =sw_buf[i][0].stn_id;
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.packet_num =0;
				lan_out ((char *)&sw_buf[i],lanhead,(unsigned char)0);
				sw_num[i] = 0;
			}
		}
	}
#ifdef DEBUG
	printf ("\ncalcufn: %s state=%d!\n",name_buf,new_value.ind.rt_state);
#endif
	update_time.ms		= 0;
	update_time.second	= tmpt.tm_sec;
	update_time.minute	= tmpt.tm_min;
	update_time.hour	= tmpt.tm_hour;
	update_time.day		= tmpt.tm_mday;
	update_time.month	= tmpt.tm_mon+1;
	update_time.year	= tmpt.tm_year+1900;
	rw_flag = RW_UPDATE_TIME | RW_RT_VALUE | RW_CHAN_FAIL;

	ret = SetPtValueIostById(&dbcommpt.point, new_value, iost, update_time, rw_flag);
	if (ret == -1)
		SendErrMsg(0,"\ncalcufn: %s No this point in the database when Wdb!\n",name_buf);
	return(ret);
}

void	Calcu_Judge(short i, short numb)
{
	int	start_gnum,end_gnum;
	int	ret;
	int	len,t_num;

	IND_DATA	obj_stat={0};

#ifdef DEBUG
	SendErrMsg(0,"\n<<<point num= %d %s \n",numb,calcu[i][numb].o_name);
#endif

	start_gnum = calcu[i][numb].offset;
	end_gnum = calcu[i][numb].offset+calcu[i][numb].g_num;

	ret = BracketLogic(i, numb, start_gnum, end_gnum);
#ifdef DEBUG
	SendErrMsg(0,">>>point num= %d %s result=%d\n",numb,calcu[i][numb].o_name,ret);
#endif
	if((ret == NOT_MEET_FAIL)||(ret == MEETWITH_FAIL))
		obj_stat.chan_fail=1;
	else
		obj_stat.chan_fail=0;
	if((ret == MEET)||(ret == MEETWITH_FAIL))
		obj_stat.rt_state=1;
	else
		obj_stat.rt_state=0;

	if (strncmp(tmp_status,calcu[i][numb].o_name,(int)strlen(tmp_status))==0)
	{
		len=strlen(tmp_status);
		if(sscanf(&calcu[i][numb].o_name[len],"%d",&t_num)==0)
		{
			SendErrMsg(0,"calcufn: point=%d %s wrong TMP_STATUS!\n",
				numb,calcu[i][numb].o_name);
			return;
		}
		if (t_num<0||t_num>999)
		{
			SendErrMsg(0,"calcufn: point=%d %s wrong TMP_STATUS %d number (0-999)!\n",
				numb,calcu[i][numb].o_name,t_num);
			return;
		}
		if (calcu[i][numb].type==LOGIC)
		{
			status_tmp[i][t_num] = obj_stat;
		}
		else if(status_tmp[i][t_num].rt_state==1)
		{
			status_tmp[i][t_num].rt_state=status_tmp[i][t_num].state=calcu[i][numb].type;
			status_tmp[i][t_num].chan_fail=obj_stat.chan_fail;
		}
	}
	else
	{
#ifdef DEBUG
		SendErrMsg(0,">>>wdb point num= %d %s result=%d\n",numb,calcu[i][numb].o_name,obj_stat.rt_state);
#endif
		if (calcu[i][numb].type==LOGIC)
		{
			Wdb(i,calcu[i][numb].o_name, obj_stat);
		}
		else if(obj_stat.rt_state==1)
		{
			obj_stat.rt_state = calcu[i][numb].type;
			Wdb(i,calcu[i][numb].o_name, obj_stat);
		}
	}
}

int	BracketLogic(short i, short numb, int start_gnum, int end_gnum)
{
	int	s_gnum,e_gnum,e_end;
	int	j,k,nt_g_num;
	float	value;
	int	flg,dat_type,stint,ret;
	int	group_fail,chan_fail_flag,meet_with_fail;
	int	len,t_num;
	short	layer_num;
#ifdef DEBUG
	short	layer_num1;
#endif
	unsigned short	stat;
//	DMS_COMMON	dbptr;
	IND_DATA	obj_stat={0};
	POINTER	point;
	UINT	chan_fail;
	float	fValue;

	layer_num = group[i][start_gnum].layer;
#ifdef DEBUG
	printf (" === lay %d start_g num=%d end_g num=%d\n",layer_num,start_gnum,end_gnum);
#endif
	meet_with_fail = 0;
	chan_fail_flag = 0;

	for (j=start_gnum;j<=end_gnum;j++ )
	{
		e_end=group[i][j].offset+group[i][j].e_num;
		group_fail = 0;
		if(group[i][j].layer != layer_num)
		{
#ifdef	DEBUG
			SendErrMsg(0,"   !!!group=%d g_num=%d g_lay=%d != %d\n",
				j,j-start_gnum,group[i][j].layer,layer_num);
#endif
			continue;
		}
#ifdef	DEBUG
		SendErrMsg(0,"  << group=%d offset e_num=%d to %d lay=%d\n",
				j,group[i][j].offset,e_end,group[i][j].layer);
#endif
		for (k=group[i][j].offset;k<e_end;k++ )
		{
			if(entry[i][k].opr == BRACKET_F)
			{
				s_gnum = entry[i][k].data.group.offset;
				e_gnum = entry[i][k].data.group.offset + entry[i][k].data.group.g_num -1;
#ifdef	DEBUG
				nt_g_num = entry[i][k].data.group.offset;
				layer_num1 = group[i][nt_g_num].layer;
				SendErrMsg(0,"\t{Bracket et_num=%d groups %d to %d lay=%d\n",k,s_gnum,e_gnum,layer_num1);
#endif
				ret = BracketLogic(i, numb, s_gnum, e_gnum);
				if((ret == NOT_MEET_FAIL)||(ret == MEETWITH_FAIL))
				{
					chan_fail_flag = 1;
					group_fail = 1;
				}
				if((ret == MEET)||(ret == MEETWITH_FAIL))
				{
					nt_g_num = entry[i][k].data.group.offset + entry[i][k].data.group.g_num -1;
#ifdef	DEBUG
					SendErrMsg(0,"\t}Bracket  et_num=%d meet! groups %d to %d lay=%d new_g_num=%d\n",k,s_gnum,e_gnum,layer_num1,nt_g_num);
#endif
					k = group[i][nt_g_num].offset+group[i][nt_g_num].e_num -1;
					continue;
				}
				else
				{
#ifdef	DEBUG
					SendErrMsg(0,"\t}Bracket  et_num=%d status not meet! groups %d to %d lay=%d\n",k,s_gnum,e_gnum,layer_num1);
#endif
					break;
				}

			}
#ifdef	DEBUG
			else
			{
				SendErrMsg(0,"\tgroup=%d entry=%d e_num=%d %s opr=%d lay=%d\n",j,k,
					k-group[i][j].offset,entry[i][k].name,entry[i][k].opr,entry[i][k].layer);
			}
#endif
			if(entry[i][k].layer != layer_num)
			{
#ifdef	DEBUG
				SendErrMsg(0,"!!! group=%d entry=%d e_num=%d %s opr=%d lay=%d != %d\n",
					j,k,k-group[i][j].offset,entry[i][k].name,entry[i][k].opr,entry[i][k].layer,layer_num);
#endif
				continue;
			}
			if (strncmp(tmp_float,entry[i][k].name,(int)strlen(tmp_float))==0)
			{
				len=strlen(tmp_float);
				dat_type=ANA_TYPE;
				if(sscanf(&entry[i][k].name[len],"%d",&t_num)==0)
				{
					SendErrMsg(0,"calcufn: point=%d %s wrong TMP_FLOAT %s!\n",
							numb,calcu[i][numb].o_name,entry[i][k].name);
					break;
				}
				if (t_num<0||t_num>999)
				{
					SendErrMsg(0,"calcufn: point=%d %s wrong TMP_FLOAT %d number!\n",
							numb,calcu[i][numb].o_name,t_num);
					break;
				}
				if (entry[i][k].opr==CHANFAIL)
				{
					stat=fstat_tmp[i][t_num].chan_fail;
				}
				else if (entry[i][k].opr==BIN_LG)
				{
					SendErrMsg(0,"calcufn:The TMP_FLOAT %s isn't a ON_OFF variable!\n",
								entry[i][k].name);
					break;
				}
				else
				{
					if(fstat_tmp[i][t_num].chan_fail)
					{
						chan_fail_flag = 1;
						group_fail = 1;
					}
					value=float_tmp[i][t_num];
				}
			}
			else if (strncmp(tmp_status,entry[i][k].name,(int)strlen(tmp_status))==0)
			{
				len=strlen(tmp_status);
				dat_type=POL_TYPE;
				if(sscanf(&entry[i][k].name[len],"%d",&t_num)==0)
				{
					SendErrMsg(0,"calcufn: point=%d %s wrong TMP_STATUS %s!\n",
							numb,calcu[i][numb].o_name,entry[i][k].name);
					break;
				}
				if (t_num<0||t_num>999)
				{
					SendErrMsg(0,"calcufn: point=%d %s wrong TMP_STATUS %d number (0-999)!\n",
							numb,calcu[i][numb].o_name,t_num);
					break;
				}
				if (entry[i][k].opr==CHANFAIL)
				{
					stat=status_tmp[i][t_num].chan_fail;
				}
				else if (entry[i][k].opr==BIN_LG)
				{
					if(status_tmp[i][t_num].chan_fail)
					{
						chan_fail_flag = 1;
						group_fail = 1;
					}
					stat=status_tmp[i][t_num].rt_state;
				}
				else
				{
					SendErrMsg(0,"calcufn:The TMP_STATUS %s is a ON_OFF variable!\n",
								entry[i][k].name);
					break;
				}
			}
			else
			{
//				ret = ReadEntryByNameStr(entry[i][k].name,&dbptr);
				ret= GetPtDataByStr(entry[i][k].name, &fValue, &chan_fail, &point);
				if (ret==-1)
				{
					SendErrMsg(0,"calcufn: point=%d group=%d entry=%d e_num=%d %s not find in db!\n",
							numb,j,k,k-group[i][j].offset,entry[i][k].name);
					break;
				}
				dat_type = point.data_type;

				if (entry[i][k].opr==BIN_LG)
				{
					if (dat_type!=SOE_TYPE && dat_type!=POL_TYPE && dat_type!=SYSMSG_TYPE && dat_type!=-1)
					{
						SendErrMsg(0,"calcufn:The data_type of %s isn't a ON_OFF variable!\n",
								entry[i][k].name);
						break;
					}
					if(chan_fail)
					{
#ifdef	DEBUG
						SendErrMsg(0,"calcufn: group=%d entry=%d e_num=%d %s data quality fail in db!\n",
								j,k,k-group[i][j].offset,entry[i][k].name);
#endif
						chan_fail_flag = 1;
						group_fail = 1;
					}
//					stat=dbptr.var.ind.status.rt_state;
					stat=(unsigned int)fValue;
				}
				else if (entry[i][k].opr==CHANFAIL)
				{
					if (dat_type==SOE_TYPE || dat_type==POL_TYPE || dat_type==SYSMSG_TYPE)
					{
						stat=chan_fail;
					}
					else if (dat_type==ANA_TYPE || dat_type==CALC_TYPE || dat_type==PARAM_TYPE)
					{
						stat=chan_fail;
					}
					else
					{
						SendErrMsg(0,"calcufn:The data_type of %s isn't supported in chanfail calcu!\n",
								entry[i][k].name);
						break;
					}
				}
				else
				{
					if (dat_type!=ANA_TYPE && dat_type!=CALC_TYPE && dat_type!=PARAM_TYPE 
						&& dat_type!=CHARA_TYPE && dat_type!=CURVE_TYPE)
					{
						SendErrMsg(0,"Err:The data_type of %s isn't a ANA/CALC/CHARA/CURVE variable!\n",
								entry[i][k].name);
						break;
					}
					if(chan_fail)
					{
#ifdef	DEBUG
						SendErrMsg(0,"calcufn: group=%d entry=%d e_num=%d %s data quality fail in db!\n",
							j,k,k-group[i][j].offset,entry[i][k].name);
#endif
						chan_fail_flag = 1;
						group_fail = 1;
					}
					value = fValue;
				}
			}
			if ((entry[i][k].opr==BIN_LG)||(entry[i][k].opr==CHANFAIL))
			{
				stint=entry[i][k].data.status&0xff;
				if ((entry[i][k].data.status&0xff00)==0)
				{
					if (stat!=stint)
					{
#ifdef	DEBUG
						SendErrMsg(0,"\tThe status is %d,is not %d;not meet! lay=%d\n",stat,stint,entry[i][k].layer);
#endif
						break;
					}
				}
				else
				{
					if (stat==stint)
					{
#ifdef	DEBUG
						SendErrMsg(0,"\tThe status is %d,is not ~%d;not meet! lay=%d\n",stat,stint,entry[i][k].layer);
#endif
						break;
					}
				}
			}
			else
			{
				flg=0;
				switch(entry[i][k].opr)
				{
				case LESS:
					if (value<entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						SendErrMsg(0,"\tThe value %9.3f not < %9.3f lay=%d\n",value,entry[i][k].data.value,entry[i][k].layer);
#endif 
					break;
				case LESS_E:
					if (value<=entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						SendErrMsg(0,"\tThe value %9.3f not <= %9.3f lay=%d\n",value,entry[i][k].data.value,entry[i][k].layer);
#endif
					break;
				case LARGE:
					if (value>entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						SendErrMsg(0,"\tThe value %9.3f not >%9.3f lay=%d\n",value,entry[i][k].data.value,entry[i][k].layer);
#endif
					break;
				case LARGE_E:
					if (value>=entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						SendErrMsg(0,"\tThe value %9.3f not>=%9.3f lay=%d\n",value,entry[i][k].data.value,entry[i][k].layer);
#endif
					break;
				case EQUAL:
					if (value==entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						SendErrMsg(0,"\tThe value %9.3f not==%9.3f lay=%d\n",value,entry[i][k].data.value,entry[i][k].layer);
#endif
					break;
				case N_EQUAL:
					if (value!=entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						SendErrMsg(0,"\tThe value %9.3f not !=%9.3f lay=%d\n",value,entry[i][k].data.value,entry[i][k].layer);
#endif
					break;
				}
				if (flg!=1)
					break;
			}
		}
		if (k>=e_end)
		{
			if(group_fail)
			{
				meet_with_fail = 1;
#ifdef DEBUG
				SendErrMsg(0,"\tgroup %d num=%d meet with quality fail! lay=%d\n",j,j-calcu[i][numb].offset,group[i][j].layer);
#endif
				continue;
			}
#ifdef DEBUG
			SendErrMsg(0,"\tgroup %d num=%d meet! lay=%d\n",j,j-start_gnum,group[i][j].layer);
#endif
			return(MEET);
		}
	}
	if(meet_with_fail)
		return(MEETWITH_FAIL);
	else
		if(chan_fail_flag)
			return(NOT_MEET_FAIL);
		else
			return(NOT_MEET);
}

#ifndef	WINDOWS_OS
#define	INT64	long long 
#endif

double	TransformToDouble(float value)
{
	INT64	intValue;
	double	coefficient=1000.0f;

	intValue=(INT64)(value*coefficient);
	if(value*coefficient-intValue>=0.5f) 
		intValue++;
	if(value*coefficient-intValue<=-0.5f)
		intValue--;
	return intValue/coefficient;
}

void	WFdb(short i, ANLG_STATUS stat_flag, float value1, char *name)
{
	char		name_buf[POINT_NAME_SIZE];
	DMS_COMMON	dbcommpt;
	float		dbValue,dBand;
	ANLG_STATUS	me_stat;
	IMP_STATUS	imp_status;
	LAN_HEAD	lanhead;

	WR_VAR_UNION	value;
	ENTRY_IMP_VAL	imp_var;
	int		d_accum;
	int		stat;
	DATE_TIME	update_time;
	USHORT		year;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};

	strcpy (name_buf,name);
	if(ReadEntryByNameStr(name_buf, &dbcommpt)==-1)
	{
		SendErrMsg(0,"\ncalcufn: %s No this point in the database when WFdb!\n",name_buf);
		return;
	}
	if(dbcommpt.point.data_type == IMP_TYPE)
	{
		imp_status.option = 0;	/* d_accum */
		imp_status.chan_fail = stat_flag.chan_fail;
		imp_var = dbcommpt.var.imp.entry;
		d_accum = (int)value1;
	}
	else
	{	/*modified by wzg 20080821
		value.anlg.data = (int)(value1*1000.0f);		modify by gj,to round a number up/down
		if(value1*1000.0-value.anlg.data>=0.5) 
			value.anlg.data++;
		if(value1*1000.0-value.anlg.data<-0.5)
			value.anlg.data--;*/
		value.anlg.data= (int) (1000.f * TransformToDouble(value1));
		value.anlg.status = stat_flag;
	}
	if(HostStat == 1)
	{
		if(dbcommpt.point.data_type == IMP_TYPE)
		{
			dbValue = dbcommpt.var.imp.entry.d_accum;
			dBand = 0;
			me_stat.chan_fail = dbcommpt.var.imp.entry.status.chan_fail;
		}
		else
		{
			dbValue = (float)(dbcommpt.var.anlg.value.data/1000.0);
			dBand = (float)dbcommpt.var.anlg.eng_para.D_BAND;
			me_stat = dbcommpt.var.anlg.value.status;
		}

		if(ana_all_flag || ((value1 - dbValue) > dBand) || ((dbValue - value1) > dBand)
				|| (me_stat.chan_fail != stat_flag.chan_fail))
		{
			if(dbcommpt.point.data_type == IMP_TYPE)
				me_buf[i][me_num[i]*22] =DPS_IMPULSE;	/* type_id */
			else
				me_buf[i][me_num[i]*22] =DPS_ANALOG;	/* type_id */

			me_buf[i][me_num[i]*22+1] =dbcommpt.point.stn_id;
			me_buf[i][me_num[i]*22+2] =dbcommpt.point.dev_id;
			me_buf[i][me_num[i]*22+3] =dbcommpt.point.data_type;
			me_buf[i][me_num[i]*22+4] = 1;		/* number low byte */
			me_buf[i][me_num[i]*22+5] = 0;		/* number high byte */

			memcpy(&me_buf[i][me_num[i]*22+6], &dbcommpt.point.pt_id,2);
			Swap2Byte(&me_buf[i][me_num[i]*22+6]);
			if(dbcommpt.point.data_type == IMP_TYPE)
			{
				memcpy(&me_buf[i][me_num[i]*22+8], &imp_status,2);
				memcpy(&me_buf[i][me_num[i]*22+10], &d_accum,4);
			}
			else
			{
				memcpy(&me_buf[i][me_num[i]*22+8], &value.anlg.status,2);
				memcpy(&me_buf[i][me_num[i]*22+10], &value.anlg.data,4);
			}
			Swap2Byte(&me_buf[i][me_num[i]*22+8]);
			Swap4Byte(&me_buf[i][me_num[i]*22+10]);
			
			GetLocalHostId(&me_buf[i][me_num[i]*22+14]);	/* host_id */
			me_buf[i][me_num[i]*22+15]=tmpt.tm_sec;	/* second */
			me_buf[i][me_num[i]*22+16]=tmpt.tm_min;	/* minute */
			me_buf[i][me_num[i]*22+17]=tmpt.tm_hour;	/* hour */
			me_buf[i][me_num[i]*22+18]=tmpt.tm_mday;	/* day */
			me_buf[i][me_num[i]*22+19]=tmpt.tm_mon+1;	/* month */
			year=tmpt.tm_year+1900;	/* year */

			memcpy(&me_buf[i][me_num[i]*22+20],&year,2);
			Swap2Byte(&me_buf[i][me_num[i]*22+20]);
			
			me_num[i] ++;
			if(me_num[i]>=64)
			{
				lanhead.length = me_num[i]*22;
				Swap2Byte(&lanhead.length);
				lanhead.dp_type=BJ_DB_TIME;
				GetLocalHostId(&lanhead.src_id);
				lanhead.dest_id =0;
				lanhead.dest_stn =me_buf[i][1];
				lanhead.packet_num =0;
				lan_out ((char *)&me_buf[i],lanhead,(unsigned char)0);
				me_num[i] = 0;
			}
		}
	}
#ifdef DEBUG
	SendErrMsg(0,"\nCalcuFn: %s %f\n",name_buf,value1);
#endif
	if(dbcommpt.point.data_type == IMP_TYPE)
	{
		imp_var.d_accum = value1;
		imp_var.status = imp_status;
		stat = SetImpPtVarById(&dbcommpt.point, imp_var);
	}
	else
	{
		update_time.ms		= 0;
		update_time.second	= tmpt.tm_sec;
		update_time.minute	= tmpt.tm_min;
		update_time.hour	= tmpt.tm_hour;
		update_time.day		= tmpt.tm_mday;
		update_time.month	= tmpt.tm_mon+1;
		update_time.year	= tmpt.tm_year+1900;
		rw_flag = RW_UPDATE_TIME | RW_CHAN_FAIL;
		stat = SetPtValueIostById(&dbcommpt.point, value, iost, update_time, rw_flag );

		stat = SetAnaPtfValueById(&dbcommpt.point, value1, update_time);
	}
	if (stat==-1)
	{
		SendErrMsg(0,"\ncalcufn: %s No this point in the database when Wdb!\n",name_buf);
	}
}

void	Calcu_Dat(short i,short numd)
{
	int	dat_type,flg,ret;
	short	j,k,kk,e_end;
	double	result,temp;
	unsigned int	stat,stint;
	float		value, fValue;
//	DMS_COMMON	dbptr;
	POINTER	point;
	UINT	chan_fail;
	ANLG_STATUS	anlg_flag={0};
	int	len,t_num;

	j=calcu[i][numd].offset;
	e_end=group[i][j].offset+group[i][j].e_num;
	kk= group[i][j].offset;

	if (calcu[i][numd].g_num!=0)
	{
#ifdef DEBUG
		SendErrMsg(0,"o_name=%s,entry_name=%s\n",calcu[i][numd].o_name,entry[i][kk].name);
#endif
		if (strncmp(tmp_float,entry[i][kk].name,strlen(tmp_float))==0)
		{
			len=strlen(tmp_float);
			dat_type=ANA_TYPE;
			if(sscanf(&entry[i][kk].name[len],"%d",&t_num)==0)
			{
				SendErrMsg(0,"calcufn: point=%d %s wrong TMP_FLOAT %s !\n",
						numd,calcu[i][numd].o_name,entry[i][kk].name[len]);
				return;
			}
			if (t_num<0||t_num>999)
			{
				SendErrMsg(0,"calcufn: point=%d %s wrong TMP_FLOAT %d number (0-999)!\n",
						numd,calcu[i][numd].o_name,t_num);
				return;
			}
			if(fstat_tmp[i][t_num].chan_fail)
			{
#ifdef DEBUG
				SendErrMsg(0,"calcufn: point=%d %s data quality fail IF TMP_FLOAT %d !\n",
						numd,calcu[i][numd].o_name,t_num);
#endif
				return;
			}
			value=float_tmp[i][t_num];
		}
		else if (strncmp(tmp_status,entry[i][kk].name,strlen(tmp_status))==0)
		{
			len=strlen(tmp_status);
			dat_type=POL_TYPE;
			if(sscanf(&entry[i][kk].name[len],"%d",&t_num)==0)
			{
				SendErrMsg(0,"calcufn: point=%d %s wrong TMP_STATUS: %s !\n",
						numd,calcu[i][numd].o_name,entry[i][kk].name);
				return;
			}
			if (t_num<0||t_num>999)
			{
				SendErrMsg(0,"calcufn: point=%d %s wrong TMP_STATUS %s %d number (0-999)!\n",
						numd,calcu[i][numd].o_name,entry[i][kk].name,t_num);
				return;
			}
			if(status_tmp[i][t_num].chan_fail)
			{
#ifdef DEBUG
				SendErrMsg(0,"calcufn: point=%d %s data quality fail IF TMP_STATUS %d !\n",
						numd,calcu[i][numd].o_name,t_num);
#endif
				return;
			}
			stat=status_tmp[i][t_num].rt_state;
		}
		else
		{
			ret= GetPtDataByStr(entry[i][kk].name, &fValue, &chan_fail, &point);
//			ret = ReadEntryByNameStr(entry[i][kk].name,&dbptr);
			if (ret==-1)
			{
				SendErrMsg(0,"calcufn: group=%d IF %s not find in db!\n",j,entry[i][kk].name);
				return;
			}
			dat_type = point.data_type;
			if (entry[i][kk].opr==BIN_LG)
			{
				if (dat_type!=SOE_TYPE && dat_type!=POL_TYPE && dat_type!=SYSMSG_TYPE )
				{
					SendErrMsg(0,"calcufn:The data_type of %s isn't a ON_OFF variable!\n",
							entry[i][kk].name);
					return;
				}
				if(chan_fail)
				{
#ifdef DEBUG
					SendErrMsg(0,"calcufn: group=%d IF %s data quality fail in db!\n",j,entry[i][kk].name);
#endif
					return;
				}
				stat=(unsigned int)fValue;
			}
			else if (entry[i][kk].opr==CHANFAIL)
			{
				if (dat_type==SOE_TYPE || dat_type==POL_TYPE || dat_type==SYSMSG_TYPE)
				{
					stat=chan_fail;
				}
				else if (dat_type==ANA_TYPE || dat_type==CALC_TYPE || dat_type==PARAM_TYPE)
				{
					stat=chan_fail;
				}
				else
				{
					SendErrMsg(0,"calcufn:The data_type of %s isn't supported in chanfail calcu!\n",
							entry[i][kk].name);
					return;
				}
			}
			else
			{
				if (dat_type!=ANA_TYPE && dat_type!=CALC_TYPE && dat_type!=PARAM_TYPE 
					&& dat_type!=CHARA_TYPE && dat_type!=CURVE_TYPE)
				{
					SendErrMsg(0,"Err:The data_type of %s isn't a ANA/CALC/CHARA/CURVE variable!\n",
							entry[i][kk].name);
					return;
				}
				if(chan_fail)
				{
#ifdef DEBUG
					SendErrMsg(0,"calcufn: group=%d IF %s data quality fail in db!\n",j,entry[i][kk].name);
#endif
					return;
				}
				value= fValue;
			}
			
		}
		if ((entry[i][kk].opr==BIN_LG)||(entry[i][kk].opr==CHANFAIL))
		{
			stint=entry[i][kk].data.status&0xff;
			if ((entry[i][kk].data.status&0xff00)==0)
			{
				if (stat!=stint)
				{
#ifdef DEBUG
					SendErrMsg(0,"\tThe status is %d,is not %d;not meet!\n",stat,stint);
#endif
					return;
				}
			}
			else
			{
				if (stat==stint)
				{
#ifdef	DEBUG
					SendErrMsg(0,"\tThe status is %d,is not ~%d;not meet!\n",stat,stint);
#endif
					return;
				}
			}
		}
		else
		{
			if (dat_type!=ANA_TYPE && dat_type!=CALC_TYPE && dat_type!=PARAM_TYPE 
				&& dat_type!=CHARA_TYPE && dat_type!=CURVE_TYPE)
			{
				SendErrMsg(0,"Err:The data_type of %s isn't a ANA/CALC/CHARA/CURVE variable!\n",entry[i][kk].name);
				return;
			}
			flg=0;
			switch(entry[i][kk].opr)
			{
			case LESS:
				if (value<entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					SendErrMsg(0,"\tThe value %9.3f not < %9.3f\n",value,entry[i][kk].data.value);
#endif 
				break;
			case LESS_E:
				if (value<=entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					SendErrMsg(0,"\tThe value %9.3f not <= %9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case LARGE:
				if (value>entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					SendErrMsg(0,"\tThe value %9.3f not >%9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case LARGE_E:
				if (value>=entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					SendErrMsg(0,"\tThe value %9.3f not>=%9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case EQUAL:
				if (value==entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					SendErrMsg(0,"\tThe value %9.3f not==%9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case N_EQUAL:
				if (value!=entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					SendErrMsg(0,"\tThe value %9.3f not !=%9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			}
			if (flg!=1)
				return;	
		}
		kk++;
	}
	for (k=kk; k<e_end; k++ )
	{
		if ((entry[i][k].opr&RESET_C)||(entry[i][k].opr==SQRT))
			temp= TransformToDouble(entry[i][k].data.value);/*(double)entry[i][k].data.value;*/
		else
		{
			if (strncmp(tmp_float,entry[i][k].name,strlen(tmp_float))==0)
			{
				len=strlen(tmp_float);
				dat_type=ANA_TYPE;
				if(sscanf(&entry[i][k].name[len],"%d",&t_num)==0)
				{
					SendErrMsg(0,"calcufn: point=%d %s wrong TMP_FLOAT %s !\n",
						numd,calcu[i][numd].o_name,entry[i][k].name);
					return;
				}
				if (t_num<0||t_num>999)
				{
					SendErrMsg(0,"calcufn: point=%d %s wrong TMP_FLOAT %d number!\n",
						numd,calcu[i][numd].o_name,t_num);
					return;
				}
				temp=TransformToDouble(float_tmp[i][t_num]);
				if(fstat_tmp[i][t_num].chan_fail)
					anlg_flag.chan_fail=1;
			}
			else
			{
//				ret = ReadEntryByNameStr(entry[i][k].name,&dbptr);
				ret= GetPtDataByStr(entry[i][k].name, &fValue, &chan_fail, &point);
				if (ret==-1)
				{
					SendErrMsg(0,"calcufn: group=%d entry=%d %s not find in db!\n",j,
						k-group[i][j].offset,entry[i][k].name);
					return;
				}
				dat_type = point.data_type;
				/*if(dat_type == IMP_TYPE)
				{
					if(dbptr.var.imp.entry.status.chan_fail)
						anlg_flag.chan_fail=1;
					temp= (double)dbptr.var.imp.entry.d_accum;
				}
				else
				{
					if(dbptr.var.anlg.value.status.chan_fail)
						anlg_flag.chan_fail=1;
					temp= (double)dbptr.var.anlg.fValue;
				}*/
				anlg_flag.chan_fail= chan_fail;
				temp= TransformToDouble(fValue); /*(double)fValue;*/
			}
		}
#ifdef DEBUG
		printf("calcufn: group=%d entry=%d %s = %f\n",j,k-group[i][j].offset,entry[i][k].name,temp);
#endif
		switch(entry[i][k].opr&MASK_C)
		{
		case RESET:
				result=temp;
				break;
		case ADD:
				result+=temp;
				break;
		case MIN:
				result-=temp;
				break;
		case MUL:
				result*=temp;
				break;
		case DIV:
				if (temp==0.0)
					return;
				result/=temp;
				break;
		case SQRT:
				if (result>=0.0)
					result= sqrt(result);
				else
					result=0.0;
				break;
		}
	}
	value = (float)result;

	if (strncmp(calcu[i][numd].o_name,tmp_float,strlen(tmp_float))==0)
	{
		len=strlen(tmp_float);
		if(sscanf(&calcu[i][numd].o_name[len],"%d",&t_num)==0)
		{
			SendErrMsg(0,"calcufn: point=%d %s wrong TMP_FLOAT !\n",
					numd,calcu[i][numd].o_name);
			return;
		}
		if (t_num<0||t_num>999)
		{
			SendErrMsg(0,"calcufn: point=%d  wrong TMP_FLOAT %s number %d (0-9)!\n",
					numd,calcu[i][numd].o_name,t_num);
			return;
		}
		float_tmp[i][t_num]=value;
		fstat_tmp[i][t_num]=anlg_flag;
	}
	else
		WFdb(i,anlg_flag,value, calcu[i][numd].o_name);
}

void	Calcu_Diff (short i,short numd)
{
	int		dat_type,ret;
	short	j,k;
	float	p,wh;
/*
	float	v1,v2,value,temp;
	int		e_end,fn, x1, x2, y1, y2;
	DIFF_DATA	*diff_dat;
	char	dev_buf[POINT_NAME_SIZE];
*/
	char	name_buf[POINT_NAME_SIZE];

	DMS_COMMON	dbptr;

	j=calcu[i][numd].offset;
/*
	e_end=group[i][j].offset+group[i][j].e_num;
*/
	k=group[i][j].offset;

	ret = ReadEntryByNameStr(entry[i][k+2].name,&dbptr);
	if (ret==-1)
	{
		SendErrMsg(0,"calcufn: group=%d entry=%d %s not find in db!\n",j,k+2,entry[i][k+2].name);
		return;
	}
	dat_type = dbptr.point.data_type;
	p = dbptr.var.anlg.fValue;

	ret = ReadEntryByNameStr(entry[i][k+3].name,&dbptr);
	if (ret==-1)
	{
		SendErrMsg(0,"calcufn: group=%d entry=%d %s not find in db!\n",j,k+3,entry[i][k+3].name);
		return;
	}
	dat_type = dbptr.point.data_type;
	wh = dbptr.var.anlg.fValue;

	strcpy (name_buf,entry[i][k+1].name);
/*
	if (entry[i][k].opr==DIFF_C)
	{
		fn=entry[i][k].data.status;
		diff_dat = (DIFF_DATA*) get_diff_value(fn,name_buf,p,wh);
	}
	else
	{
		strcpy (dev_buf,entry[i][k].name);
		diff_dat = (DIFF_DATA*) get_diff_value(dev_buf,name_buf,p,wh);
	}
	x1=diff_dat->x_value[0];
	x2=diff_dat->x_value[2];
	y1=diff_dat->y_value[0];
	y2=diff_dat->y_value[1];

	if (x1==x2)
	{
		v1=diff_dat->data[0];
		v2=diff_dat->data[2];
	}
	else
	{
		v1=diff_dat->data[0]+(diff_dat->data[2]-diff_dat->data[0])*( p-x1)/(x2-x1);
		v2=diff_dat->data[1]+(diff_dat->data[3]-diff_dat->data[1])*( p-x1)/(x2-x1);
	}

	if (y1==y2)
		value=v1;
	else
		value=v1+(v2-v1)*(wh-y1)/(y2-y1);

	WFdb(i,value, calcu[i][numd].o_name);
*/
}

void Clear_Db(short i, short numd)
{
	int		e_end;
	short	j,k,minutes;
	ANLG_STATUS	anlg_flag={0};
	float	value;
	struct tm	tmpt;
	time_t		tim;
	int	len,t_num;

	tim=time(NULL);
	tmpt=*localtime(&tim);

#ifdef MINUTE_CLEAR
	minutes = tmpt.tm_hour * 60 + tmpt.tm_min;
	if(minutes !=calcu[i][numd].g_num)
		return;
#else
	if (tmpt.tm_hour!=calcu[i][numd].g_num)
		return;
#endif
	if ((calcu[i][numd].type==M_CLEAR)&&(tmpt.tm_mday!=1))
		return;
	else if (calcu[i][numd].type==Y_CLEAR)
		if ((tmpt.tm_mday!=1)||(tmpt.tm_mon!=0))
			return;

	j=calcu[i][numd].offset;
	e_end=group[i][j].offset+group[i][j].e_num;
	anlg_flag.chan_fail = 0;
	value = 0.0f;
	for (k=group[i][j].offset;k<e_end;k++ )
	{
		if (strncmp(tmp_float,calcu[i][numd].o_name,strlen(tmp_float))==0)
		{
			len=strlen(tmp_float);
			if(sscanf(&calcu[i][numd].o_name[len],"%d",&t_num)==0)
			{
				SendErrMsg(0,"calcufn: point=%d  wrong TMP_FLOAT %s!\n",
						numd,calcu[i][numd].o_name);
				continue;
			}
			if (t_num<0||t_num>999)
			{
				SendErrMsg(0,"calcufn: point=%d  wrong TMP_FLOAT %s number %d !\n",
						numd,calcu[i][numd].o_name,t_num);
				continue;
			}
			float_tmp[i][t_num]=value;
			fstat_tmp[i][t_num]=anlg_flag;
		}
		else
			WFdb(i,anlg_flag,value,entry[i][k].name);
	}
}

void CalcuFileFree(short i)
{
	free(calcu[i]);
	free(group[i]);
	free(entry[i]);
}

void *CalcuFn(PDC_TASK *pTask)
{
	short		pnum;
	int			run_num,run_flag;
	BOOL		host_priv;
	LAN_HEAD	lanhead;

	if(-1==CalcuFileLoad(pTask->param))
	{
		SendErrMsg(0, "Fail to load calcu %d file", pTask->param);
		exit(-1);
	}
	run_num = 0;
	for(;;)
	{                 
		sleep(pTask->check_period);
		if((run_flag = PdcCheck(pTask))!=0)
			continue;

/*		printf("\n------CALCU%d------\n",pTask->param);*/

		HostStat = 0;
		if(GetLocalHostPrvg("APP_SERVER", &host_priv) == 0)
		{
			if(host_priv == TRUE)
			{
				if(-1==GetLocalHostState(&HostStat))
				{
					SendErrMsg(0, "!!! calcufn: Host Stat not in database\n");
				}
			}
		}
/*
		if(-1==GetLocalHostState(&HostStat))
		{
			printf("!!! calcufn: Host Stat not in database\n");
		}
*/
		if( HostStat==1 )
		{
#ifdef DEBUG
			printf("!!! calcufn: Host Stat is Master! Start Net Broadcast!\n");
#endif
			run_num ++;
			if(run_num>120)
			{
				ana_all_flag[pTask->param] = 1;
				run_num = 0;
			}
			else
			{
				ana_all_flag[pTask->param] = 0;
			}
		}
		me_num[pTask->param] = 0;
		sw_num[pTask->param] = 0;
		tim=time(&tim);
		tmpt = *localtime(&tim);

		for ( pnum=0; pnum<pt_num[pTask->param]; pnum++ )
		{
			switch(calcu[pTask->param][pnum].type)
			{
			case LOGIC:
				Calcu_Judge(pTask->param,pnum);
				break;
			case CALCU:
				Calcu_Dat(pTask->param,pnum);
				break;
			case DIFF :
				Calcu_Diff (pTask->param,pnum);
				break;
			case D_CLEAR:
			case M_CLEAR:
			case Y_CLEAR:
				Clear_Db(pTask->param,pnum);
				break;
			default:
				Calcu_Judge(pTask->param,pnum);
			}
		}
/*		printf("\n------END CALCU%d------\n",pTask->param);*/
		if(me_num[pTask->param]>0)
		{
			lanhead.length = me_num[pTask->param]*22;
			Swap2Byte(&lanhead.length);
			lanhead.dp_type=BJ_DB_TIME;
			lanhead.dest_stn =me_buf[pTask->param][1];
			GetLocalHostId(&lanhead.src_id);
			lanhead.dest_id =0;
			lanhead.packet_num =0;
			lan_out ((char *)&me_buf[pTask->param],lanhead,(unsigned char)0);
		}		
		if( sw_num[pTask->param] > 0 )
		{
			lanhead.length = sw_num[pTask->param]*sizeof(SW_MSG);
			Swap2Byte(&lanhead.length);
			lanhead.dp_type=BJ_DB_TIME;
			lanhead.dest_stn =sw_buf[pTask->param][0].stn_id;
			GetLocalHostId(&lanhead.src_id);
			lanhead.dest_id =0;
			lanhead.packet_num =0;
			lan_out ((char *)&sw_buf[pTask->param],lanhead,(unsigned char)0);
		}
	}
}

void CalcuFnts(short i)
{
	short		pnum;	                 
	LAN_HEAD	lanhead;

	if(-1==GetLocalHostState(&HostStat))
	{
		SendErrMsg(0, "!!! calcufn: Host Stat not in database\n");
	}
	if( HostStat==1 )
	{
#ifdef DEBUG
		printf("!!! calcufn: Host Stat is Master! Start Net Broadcast!\n");
#endif
	}
	me_num[i] = 0;
	sw_num[i] = 0;
	tim=time(&tim);
	tmpt = *localtime(&tim);

	for ( pnum=0; pnum<pt_num[i]; pnum++ )
	{
		switch(calcu[i][pnum].type)
		{
		case LOGIC:
			Calcu_Judge(i,pnum);
			break;
		case CALCU:
			Calcu_Dat(i,pnum);
			break;
		case DIFF :
			Calcu_Diff (i,pnum);
			break;
		case D_CLEAR:
		case M_CLEAR:
		case Y_CLEAR:
			Clear_Db(i,pnum);
			break;
		default:
			Calcu_Judge(i,pnum);
		}
	}
	if(me_num[i]>0)
	{
		lanhead.length = me_num[i]*22;
		Swap2Byte(&lanhead.length);
		lanhead.dp_type=BJ_DB_TIME;
		lanhead.dest_stn =me_buf[i][1];
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.packet_num =0;
		lan_out ((char *)&me_buf[i],lanhead,(unsigned char)0);
	}		
	if( sw_num[i] > 0 )
	{
		lanhead.length = sw_num[i]*sizeof(SW_MSG);
		Swap2Byte(&lanhead.length);
		printf("!!! calcufn: Host Stat is Master! Start SW Broadcast!\n");
		lanhead.dp_type=BJ_DB_TIME;
		lanhead.dest_stn =sw_buf[i][0].stn_id;
		GetLocalHostId(&lanhead.src_id);
		lanhead.dest_id =0;
		lanhead.packet_num =0;
		lan_out ((char *)&sw_buf[i],lanhead,(unsigned char)0);
	}
}

void CalcuShowStruct(short i)
{
	short	pnum,j,k;
	int	start_gnum,end_gnum,e_end;
	int bs_gnum,be_gnum,nt_g_num,layer_num,layer_num1;

	printf ("\n<<<<<total point num= %d groups %d entry %d\n",pt_num[i],gt_num[i],et_num[i]);
	for ( pnum=0; pnum<pt_num[i]; pnum++ )
	{
		start_gnum = calcu[i][pnum].offset;
		if(calcu[i][pnum].type <= LOGIC)
			end_gnum = calcu[i][pnum].offset+calcu[i][pnum].g_num;
		else
			end_gnum = calcu[i][pnum].offset+calcu[i][pnum].g_num-1;
		printf ("\n<<<<<point num= %d %s type=%04X groups %d to %d\n",pnum,calcu[i][pnum].o_name,calcu[i][pnum].type,start_gnum,end_gnum);
		for (j=start_gnum;j<=end_gnum;j++ )
		{
			layer_num = group[i][j].layer;
			e_end=group[i][j].offset+group[i][j].e_num;
			SendErrMsg(0,"\t<<group=%d g_lay=%d entrys %d to %d\n",
				j,group[i][j].layer,group[i][j].offset,e_end);
			for (k=group[i][j].offset;k<e_end;k++ )
			{
				if(layer_num != entry[i][k].layer)
					continue;
				if(entry[i][k].opr == BRACKET_F)
				{
					bs_gnum = entry[i][k].data.group.offset;
					be_gnum = entry[i][k].data.group.offset + entry[i][k].data.group.g_num -1;
					nt_g_num = entry[i][k].data.group.offset;
					layer_num1 = group[i][nt_g_num].layer;
					SendErrMsg(0,"\t{Bracket et_num=%d groups %d to %d lay=%d\n",k,bs_gnum,be_gnum,layer_num1);
				}
				else
				{
					SendErrMsg(0,"\t\tgroup=%d entry=%d et=%d %s opr=%d lay=%d\n",
						j,k,k-group[i][j].offset,entry[i][k].name,entry[i][k].opr,entry[i][k].layer);
				}
			}
		}
	}
}

/*
main()
{
	char	name_input[POINT_NAME_SIZE];

	if (CalcuFileLoad(0)==-1)
	{
		SendErrMsg(0,"Calcugen.dat error!\n" );
		exit(1);
	}
	MountDB();

	for (;;)
	{
		SendErrMsg(0,"\nPlease Input the calcu point name:" );
		if (gets(name_input)==NULL)
		{
			SendErrMsg(0,"Input null name,exit now!\n" );
			break;
		}

		if (strlen(name_input)==0)
		{
			SendErrMsg(0,"CR received,exit now!\n" );
			break;
		}
		SendErrMsg(0,"\t CalcuFn(0) begin \n" );

		CalcuFn(0);
	}

	UnMountDB();
	CalcuFileFree(0);
}
*/

