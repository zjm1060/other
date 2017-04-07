/* calcufnts.c program lib
   cc -c calcufnts.c
*/

/*#define DEBUG
*/
#define MINUTE_CLEAR

#ifdef  SUN_OS
#include<unistd.h>
#include<stdio.h>
#include<malloc.h>
#include<math.h>
#include <sys/time.h>
#else
#include<stdio.h>
#include<math.h>
#endif

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../pas/v4.0a/inc/calcugen.h"
#include "../../../dps/v4.0a/inc/pdc_def.h"
#include "../../../pas/v4.0a/inc/rpt_def.h"

static	char	*genfile[]={"/home/ems/h9000/dat/CalcuGen.dat",
			                "/home/ems/h9000/dat/Calcu1Gen.dat"};


extern	float	GetAnaValue();
int     CalcuFileLoad(short);
int     Wdb(char *,short);
void    Calcu_Judge(short,short);
void    WFdb(short, float, char *);
void    Calcu_Dat(short,short);
void    Calcu_Diff (short,short);
void    Clear_Db(short,short);
void    CalcuFileFree(short);
void    CalcuFnts(short);

static	FILE		*fp;

static	char		*tmp_float=	"TMP_FLOAT";
static	char		*tmp_status=	"TMP_STATUS";

static	float		float_tmp[2][10];
static	unsigned short	status_t[2][10];

static	short		pt_num[2];
static	short		gt_num[2];
static	short		et_num[2];

static	Calcu		*calcu[2];
static	C_Group		*group[2];
static	C_Entry		*entry[2];

static	short	len,t_num;

int	CalcuFileLoad(short i)
{
static	FILE	*fp;
#ifdef	SUN_OS
	short	num;
#endif

	if ((fp=fopen(genfile[i],"rb" ))==NULL)
	{
		printf ( "calcufnts.c:can't open(rb) the file %s!\n",genfile[i]);
		return -1;
	}
	if (fread(&pt_num[i],sizeof (short),1,fp)!=1)
	{
		printf ( "calcufnts.c:file %s read pt_num error\n",genfile[i]);
		return -1;
	}
	if (fread(&gt_num[i],sizeof (short),1,fp)!=1)
	{
		printf ( "calcufnts.c:file %s read gt_num error\n",genfile[i]);
		return -1;
	}
	if (fread(&et_num[i],sizeof (short),1,fp)!=1)
	{
		printf ( "calcufnts.c:file %s read et_num error\n",genfile[i]);
		return -1;
	}

#ifdef	SUN_OS
	Swap2Byte(&pt_num[i]);
	Swap2Byte(&gt_num[i]);
	Swap2Byte(&et_num[i]);
#endif	
	calcu[i]=(Calcu *)calloc( pt_num[i],sizeof (Calcu));
	if (calcu[i]==NULL)
	{
		printf ( "calcufnts.c:calcu calloc( pt_num[%d]) error\n",i);
		return -1;
	}
	if (fread(calcu[i],sizeof (Calcu),pt_num[i],fp)!=pt_num[i])
	{
		printf ( "calcufnts.c:file %s read calcu error\n",genfile[i]);
		return -1;
	}

	group[i]=(C_Group *)calloc(gt_num[i],sizeof (C_Group));
	if (group[i]==NULL)
	{
		printf ( "calcufnts.c: group *calloc error\n" );
		return -1;
	}
	if (fread(group[i],sizeof (C_Group),gt_num[i],fp)!=gt_num[i])
	{
		printf ( "calcufnts.c:file %s read group error\n",genfile[i]);
		return -1;
	}

	entry[i]=(C_Entry *)calloc(et_num[i],sizeof (C_Entry));
	if (entry[i]==NULL)
	{
		printf ( "calcufnts.c: entry *calloc error\n" );
		return -1;
	}
	if (fread(entry[i],sizeof (C_Entry),et_num[i],fp)!=et_num[i])
	{
		printf ( "calcufnts.c:file %s read entry error\n",genfile[i]);
		return -1;
	}

	fclose(fp);
#ifdef	SUN_OS
	for( num=0; num < pt_num[i]; num++ )
	{
		Swap2Byte(&calcu[i][num].type);
		Swap2Byte(&calcu[i][num].g_num);
		Swap2Byte(&calcu[i][num].offset);
	}
	for( num=0; num < gt_num[i]; num++ )
	{
		Swap2Byte(&group[i][num].e_num);
		Swap2Byte(&group[i][num].offset);
	}
	for( num=0; num < et_num[i]; num++ )
	{
		Swap2Byte(&entry[i][num].opr);
		Swap4Byte(&entry[i][num].data.value);
	}
#endif
	return 0;
}

int	Wdb(char *name,short status)
{
	WR_VAR_UNION	new_value;
	char		name_buf[POINT_NAME_SIZE];
	int		ret;
	enum RW_FLAG	rw_flag=0;
	IOST		iost={0};
	time_t		tim;
	struct tm	tmpt;
	DATE_TIME	update_time;

	strcpy (name_buf,name);
	new_value.ind.rt_state = status;
	tim=time(&tim);
	tmpt = *localtime(&tim);
	update_time.ms		= 0;
	update_time.second	= tmpt.tm_sec;
	update_time.minute	= tmpt.tm_min;
	update_time.hour	= tmpt.tm_hour;
	update_time.day		= tmpt.tm_mday;
	update_time.month	= tmpt.tm_mon+1;
	update_time.year	= tmpt.tm_year+1900;
	rw_flag = RW_UPDATE_TIME | RW_RT_VALUE;


/*	common_bb.point_id=name_buf;

	common_bb.entry.ind.variable.value.status = status << BitS_STATUS;
	stat = WriteEntryByName(&common_bb, RW_VAR);
*/
	/***printf ( "\ncalcufnts: %s stat=%d!\n",name_buf,status);***/
	ret = SetPtValueIostByNameStr( name_buf, new_value, iost, update_time, rw_flag);
	if (ret == -1)
		printf ( "\ncalcufnts: %s No this point in the database when Wdb!\n",name_buf);
	return(ret);
}


void	Calcu_Judge(short i,short numb)
{
	int	g_end,e_end;
	int	j,k;
	float	value;
	int	flg,dat_type,stint;
	int	ret;

	unsigned short	stat;
	DMS_COMMON	dbptr;

#ifdef DEBUG
	printf ( "the point %s num= %d\n",calcu[i][numb].o_name,numb);
#endif

	g_end=calcu[i][numb].offset+calcu[i][numb].g_num;

	for (j=calcu[i][numb].offset;j<=g_end;j++ )
		{
		e_end=group[i][j].offset+group[i][j].e_num;

		for (k=group[i][j].offset;k<e_end;k++ )
			{
			if (strncmp(tmp_float,entry[i][k].name,strlen(tmp_float))==0)
				{
				len=strlen(tmp_float);
				dat_type=ANA_TYPE;
				t_num=(short)entry[i][k].name[len]-'0';
				if (t_num<0||t_num>9)
					printf ( "calcufnts: point=%d %s wrong TMP_FLOAT %d number (0-9)!\n",
							numb,calcu[i][numb].o_name,t_num);
				value=float_tmp[i][t_num];
			}
			else if (strncmp(tmp_status,entry[i][k].name,strlen(tmp_status))==0)
			{
				len=strlen(tmp_status);
				dat_type=POL_TYPE;
				t_num=(short)entry[i][k].name[len]-'0';
				if (t_num<0||t_num>9)
					printf ( "calcufnts: point=%d %s wrong TMP_STATUS %d number (0-9)!\n",
							numb,calcu[i][numb].o_name,t_num);
				stat=status_t[i][t_num];
			}
			else
				{
				ret = ReadEntryByNameStr(entry[i][k].name,&dbptr);
				if (ret==-1)
				{
					printf ( "calcufnts: point=%d entry=%d %s not find in db!\n",
							numb,j-group[i][j].offset,entry[i][k].name);
					break;
				}
				dat_type = dbptr.point.data_type;
				if (entry[i][k].opr==BIN_LG)
				{
					if (dat_type!=SOE_TYPE &&dat_type!=POL_TYPE &&dat_type!=SYSMSG_TYPE )
					{
						printf ( "calcufnts:The data_type of %s isn't a ON_OFF variable!\n",
								entry[i][k].name);     
						break;
					}
					stat=dbptr.var.ind.status.state;
				}
				else
				{
					if (dat_type!=ANA_TYPE &&dat_type!=CALC_TYPE)
					{
						printf ( "Err:The data_type of %s isn't a ANA/CALC variable!\n",
								entry[i][k].name);
						break;
					}
/*
					if (dat_type==CALC_TYPE)
						value=dbptr.entry.calc.variable.value.float_data.data;
					else
						value= GetAnaValue ( &dbptr.entry.anlg.variable.value );
*/
					value = dbptr.var.anlg.fValue;
				}
			}
			if (entry[i][k].opr==BIN_LG)
			{
				stint=entry[i][k].data.status&0xff;
				if ((entry[i][k].data.status&0xff00)==0)
				{
					if (stat!=stint)
					{
#ifdef	DEBUG
						printf ( "\tThe status is %d,is not %d;not meet!\n",stat,stint);
#endif
						break;
					}
				}
				else
				{
					if (stat==stint)
					{
#ifdef	DEBUG
						printf ( "\tThe status is %d,is not ~%d;not meet!\n",stat,stint);
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
						printf ( "\tThe value %9.3f not < %9.3f\n",value,entry[i][k].data.value);
#endif 
					break;
				case LESS_E:
					if (value<=entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not <= %9.3f\n",value,entry[i][k].data.value);
#endif
					break;
				case LARGE:
					if (value>entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not >%9.3f\n",value,entry[i][k].data.value);
#endif
					break;
				case LARGE_E:
					if (value>=entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not>=%9.3f\n",value,entry[i][k].data.value);
#endif
					break;
				case EQUAL:
					if (value==entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not==%9.3f\n",value,entry[i][k].data.value);
#endif
					break;
				case N_EQUAL:
					if (value!=entry[i][k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not !=%9.3f\n",value,entry[i][k].data.value);
#endif
					break;
				}
				if (flg!=1)
					break;
			}
		}
		if (k>=e_end)
		{
#ifdef DEBUG
			printf ( "%d group meet!\n",j-calcu[i][numb].offset);
#endif
			if (strncmp(tmp_status,calcu[i][numb].o_name,strlen(tmp_status))==0)
			{
				len=strlen(tmp_status);
				t_num=(short)calcu[i][numb].o_name[len]-'0';
				if (t_num<0||t_num>9)
					printf ( "calcufnts: point=%d %s wrong TMP_STATUS %d number (0-9)!\n",
							numb,calcu[i][numb].o_name,t_num);
				if (calcu[i][numb].type==LOGIC)
					status_t[i][t_num]=1;
				else
					status_t[i][t_num]=calcu[i][numb].type;
			}
			else
			{
				if (calcu[i][numb].type==LOGIC)
					Wdb(calcu[i][numb].o_name,1);
				else
					Wdb(calcu[i][numb].o_name,calcu[i][numb].type);
			}
			return;			/*2000.4.20 modified by zy */
		}
	}

	if (calcu[i][numb].type==LOGIC)
	{
		if (strncmp(tmp_status,calcu[i][numb].o_name,strlen(tmp_status))==0)
		{
			len=strlen(tmp_status);
			t_num=(short)calcu[i][numb].o_name[len]-'0';
			if (t_num<0||t_num>9)
				printf ( "calcufnts: point=%d %s wrong TMP_STATUS %d number (0-9)!\n",
						numb,calcu[i][numb].o_name,t_num);
			status_t[i][t_num]=0;
		}
		else
			Wdb(calcu[i][numb].o_name,0);
	}
}

void	WFdb(short i,float value1, char *name)
{
	short		j,k,l;
	short		data;
	char		name_buf[POINT_NAME_SIZE];
	DMS_COMMON	common_bb;
/*	DMS_CALCULATE	calc_bb;
*/
	float		value;
	int		stat;
	time_t		tim;
	struct tm	tmpt;
	DATE_TIME	update_time;

	strcpy (name_buf,name);
	value = value1;

	tim=time(&tim);
	tmpt = *localtime(&tim);
	update_time.ms		= 0;
	update_time.second	= tmpt.tm_sec;
	update_time.minute	= tmpt.tm_min;
	update_time.hour	= tmpt.tm_hour;
	update_time.day		= tmpt.tm_mday;
	update_time.month	= tmpt.tm_mon+1;
	update_time.year	= tmpt.tm_year+1900;
#ifdef DEBUG
	printf ( "\nCalcuFnts: %s %f\n",name_buf,value);
#endif
	stat = SetAnaPtfValueByNameStr(name_buf,value,update_time);
	if (stat==-1)
	{
		printf ( "\ncalcufnts: %s No this point in the database when Wdb!\n",name_buf);
	}
/*
	for (l=0;l<NAME_SIZE;l++ )
		if (name_buf[l]=='.')
			break;
	if (strncmp(&name_buf[l+1],"CALC",4)==0)
	{
		calc_bb.point_id = name_buf;
		calc_bb.calc.value.float_data.data = value;
		if (WriteCalculate(&calc_bb, RW_VAR)==-1)
		{
			printf ( "\nCalcuFnts: %s No this point in the database when Wdb!\n",name_buf);
		}
	}
	else
	{
		if ( value <0 )
		{
			value=-value;
			k=0x80;
		}
		else	k=0;

		if (value<65.535)
		{
			j=0x060;
			value*=1000;
		}
		else if ( value < 655.35 )
		{
			j=0x40;
			value*=100;
		}
		else if ( value < 6553.5 )
		{
			j=0x20;
			value*=10;
		}
		else	j=0;

		common_bb.point_id=name_buf;
		common_bb.entry.anlg.variable.value.status = j | k;
		common_bb.entry.anlg.variable.value.data = (unsigned short)value;
		if (WriteEntryByName(&common_bb, RW_VAR)==-1)
*/
}

void	Calcu_Dat(short i,short numd)
{
	int		dat_type,flg,stint;
	short		j,k,kk,e_end,xxx,ddd;
	double		result,temp;
	int		ret;
	unsigned int	stat;
	float		value;
	DMS_COMMON	dbptr;

	j=calcu[i][numd].offset;
	e_end=group[i][j].offset+group[i][j].e_num;
	kk= group[i][j].offset;

	if (calcu[i][numd].g_num!=0)
	{
#ifdef DEBUG
		printf ( "o_name=%s,entry_name=%s\n",calcu[i][numd].o_name,entry[i][kk].name);
#endif
		if (strncmp(tmp_float,entry[i][kk].name,strlen(tmp_float))==0)
		{
			len=strlen(tmp_float);
			dat_type=ANA_TYPE;
			t_num=(short)entry[i][kk].name[len]-'0';
			if (t_num<0||t_num>9)
				printf ( "calcufnts: point=%d %s wrong TMP_FLOAT %d number (0-9)!\n",
						numd,calcu[i][numd].o_name,t_num);
			value=float_tmp[i][t_num];
		}
		else if (strncmp(tmp_status,entry[i][kk].name,strlen(tmp_status))==0)
		{
			len=strlen(tmp_status);
			dat_type=POL_TYPE;
			t_num=(short)entry[i][kk].name[len]-'0';
			if (t_num<0||t_num>9)
				printf ( "calcufnts: point=%d %s wrong TMP_STATUS %d number (0-9)!\n",
						numd,calcu[i][numd].o_name,t_num);
			stat=status_t[i][t_num];
		}
		else
		{
/*
			ReadEntryByName(entry[i][kk].name,&dat_type,&dbptr);
			if (dbptr.err_code==-1)
			{
				printf ( "calcufnts: group=%d IF %s not find in db!\n",j,entry[i][kk].name);
				return;
			}
*/
			ret = ReadEntryByNameStr(entry[i][kk].name,&dbptr);
			if (ret==-1)
			{
				printf ( "calcufnts: group=%d IF %s not find in db!\n",j,entry[i][kk].name);
				return;
			}
			dat_type = dbptr.point.data_type;
			if (entry[i][kk].opr==BIN_LG)
			{
				if (dat_type!=SOE_TYPE &&dat_type!=POL_TYPE &&dat_type!=SYSMSG_TYPE )
				{
					printf ( "AAAAcalcufnts:The data_type of dat_type=%d,%s isn't a ON_OFF variable!\n",
							dbptr.point.data_type,entry[i][kk].name);
					return;
				}
				stat=dbptr.var.ind.status.state;
			}
			else
			{
				if (dat_type!=ANA_TYPE  &&dat_type!=CALC_TYPE)
				{
					printf ( "Err:The data_type of %s isn't a ANA variable!\n",
							entry[i][kk].name);
					return;
				}
/*
				if (dat_type==CALC_TYPE)
					value=dbptr.entry.calc.variable.value.float_data.data;
				else
					value= GetAnaValue ( &dbptr.entry.anlg.variable.value );
*/
				value= dbptr.var.anlg.fValue;
			}
			
		}
		if (entry[i][kk].opr==BIN_LG)
		{
			stint=entry[i][kk].data.status&0xff;
			if ((entry[i][kk].data.status&0xff00)==0)
			{
				if (stat!=stint)
				{
#ifdef DEBUG
					printf ( "\tThe status is %d,is not %d;not meet!\n",stat,stint);
#endif
					return;
				}
			}
			else
			{
				if (stat==stint)
				{
#ifdef	DEBUG
					printf ( "\tThe status is %d,is not ~%d;not meet!\n",stat,stint);
#endif
					return;
				}
			}
		}
		else
		{
			if (dat_type!=ANA_TYPE  &&dat_type!=CALC_TYPE)
			{
				printf ( "Err:The data_type of %s isn't a ANA/CALC variable!\n",entry[i][kk].name);
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
					printf ( "\tThe value %9.3f not < %9.3f\n",value,entry[i][kk].data.value);
#endif 
				break;
			case LESS_E:
				if (value<=entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					printf ( "\tThe value %9.3f not <= %9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case LARGE:
				if (value>entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					printf ( "\tThe value %9.3f not >%9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case LARGE_E:
				if (value>=entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					printf ( "\tThe value %9.3f not>=%9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case EQUAL:
				if (value==entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					printf ( "\tThe value %9.3f not==%9.3f\n",value,entry[i][kk].data.value);
#endif
				break;
			case N_EQUAL:
				if (value!=entry[i][kk].data.value)
					flg=1;
#ifdef DEBUG
				else
					printf ( "\tThe value %9.3f not !=%9.3f\n",value,entry[i][kk].data.value);
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
			temp= (double)entry[i][k].data.value;
		else
		{
			if (strncmp(tmp_float,entry[i][k].name,strlen(tmp_float))==0)
			{
				len=strlen(tmp_float);
				dat_type=ANA_TYPE;
				t_num=(short)entry[i][k].name[len]-'0';
				if (t_num<0||t_num>9)
					printf ( "calcufnts: point=%d %s wrong TMP_FLOAT %d number (0-9)!\n",
						numd,calcu[i][numd].o_name,t_num);
				temp=float_tmp[i][t_num];
			}
			else
			{
/*
				ReadEntryByName(entry[i][k].name,&dat_type,&dbptr);
				if (dbptr.err_code==-1)
				{
					printf ( "calcufnts: group=%d entry=%d %s not find in db!\n",j,
						k-group[i][j].offset,entry[i][k].name);
					return;
				}
*/
				ret = ReadEntryByNameStr(entry[i][k].name,&dbptr);
				if (ret==-1)
				{
					printf ( "calcufnts: group=%d entry=%d %s not find in db!\n",j,
						k-group[i][j].offset,entry[i][k].name);
					break;
				}
				dat_type = dbptr.point.data_type;
/*
				if (dat_type==CALC_TYPE)
					temp= (double)dbptr.entry.calc.variable.value.float_data.data;
				else
					temp= (double)GetAnaValue ( &dbptr.entry.anlg.variable.value );
*/
				temp= (double)dbptr.var.anlg.fValue;
			}
		}
#ifdef DEBUG
		printf("calcufnts: group=%d entry=%d %s = %f\n",j,k-group[i][j].offset,entry[i][k].name,temp);
#endif
		switch(entry[i][k].opr&MASK_C)
		{
		case RESET:	result=temp;
				break;
		case ADD:	result+=temp;
				break;
		case MIN:	result-=temp;
				break;
		case MUL:	result*=temp;
				break;
		case DIV:	if (temp==0.0)
					return;
				result/=temp;
				break;
		case SQRT:	if (result>=0.0)
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
		t_num=(short)calcu[i][numd].o_name[len]-'0';
		if (t_num<0||t_num>9)
			printf ( "calcufnts: point=%d  wrong TMP_FLOAT %s number (0-9)!\n",
					numd,calcu[i][numd].o_name);
		float_tmp[i][t_num]=value;
	}
	else
		WFdb(i,value, calcu[i][numd].o_name);
}

void	Calcu_Diff (short i,short numd)
{
        int             e_end,dat_type;
        short           j,k;
        float           v1,v2,value,temp;
	int		fn, x1, x2, y1, y2;
	int		ret;
	float		p,wh;
        char            dev_buf[POINT_NAME_SIZE],name_buf[POINT_NAME_SIZE];
/*	DIFF_DATA	*diff_dat;
*/
	DMS_COMMON	dbptr;

	j=calcu[i][numd].offset;
/*	e_end=group[i][j].offset+group[i][j].e_num;
*/
	k=group[i][j].offset;
/*
	ReadEntryByName(entry[i][k+2].name,&dat_type,&dbptr);
	if (dbptr.err_code==-1)
	{
		printf ( "calcufnts: group=%d entry=%d %s not find in db!\n",j,k+2,entry[i][k+2].name);
		return;
	}
*/
	ret = ReadEntryByNameStr(entry[i][k+2].name,&dbptr);
	if (ret==-1)
	{
		printf ( "calcufnts: group=%d entry=%d %s not find in db!\n",j,k+2,entry[i][k+2].name);
		return;
	}
	dat_type = dbptr.point.data_type;
	p = dbptr.var.anlg.fValue;

	ret = ReadEntryByNameStr(entry[i][k+3].name,&dbptr);
	if (ret==-1)
	{
		printf ( "calcufnts: group=%d entry=%d %s not find in db!\n",j,k+3,entry[i][k+3].name);
		return;
	}
	wh = dbptr.var.anlg.fValue;

	strcpy (name_buf,entry[i][k+1].name);
/*
printf ( "\nfn = %d", entry[i][k].data.status);
printf ( "\nname_buf = %s", name_buf);
printf ( "\np = %f", p);
printf ( "\nwh = %f", wh);


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

void Clear_Db(short i,short numd)
{
	int		e_end;
	short		j,k,minutes;
	struct tm	tmpt;
	time_t		tim;

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
	for (k=group[i][j].offset;k<e_end;k++ )
	{
		if (strncmp(tmp_float,calcu[i][numd].o_name,strlen(tmp_float))==0)
		{
			len=strlen(tmp_float);
			t_num=(short)calcu[i][numd].o_name[len]-'0';
			if (t_num<0||t_num>9)
				printf ( "calcufnts: point=%d  wrong TMP_FLOAT %s number (0-9)!\n",
						numd,calcu[i][numd].o_name);
			float_tmp[i][t_num]=0;
		}
		else
			WFdb(i,0.0f,entry[i][k].name);
	}
}

void CalcuFileFree(short i)
{
	free(calcu[i]);
	free(group[i]);
	free(entry[i]);
}

void  CalcuFnts(short i)
{
	short	    pnum;         
                 
	for ( pnum=0; pnum<pt_num[i]; pnum++ )
	{
		switch(calcu[i][pnum].type)
		{
		case LOGIC:	Calcu_Judge(i,pnum);
				break;
		case CALCU:	Calcu_Dat(i,pnum);
				break;
		case DIFF :	Calcu_Diff (i,pnum);
				break;
		case D_CLEAR:
		case M_CLEAR:
		case Y_CLEAR:	Clear_Db(i,pnum);
				break;
		default:	Calcu_Judge(i,pnum);
		}
	}	
}



