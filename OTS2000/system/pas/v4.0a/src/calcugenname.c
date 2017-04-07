/* calcugenname.c program
   cc -o calcugenname.dxe calcugenname.c /DBLIB/dblib.dlb
*/

#define DEBUG

#ifdef SUN_OS
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include <time.h>
#else
#include<fcntl.h>
#include<stdio.h>
#include <time.h>
#endif

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../pas/v4.0a/inc/calcugen.h"

static	int		n,r,line_num,f;

static	FILE		*fp,*fpdef,*fp1,*fpb,*ferr,*ftxt;

static	char		*filehead=	"BEGIN:";
static	char		*pt_flag=	"POINT:";
static	char		*fileflag =	"INCLUDE";
static	char		*filetail=	"END";
static	char		*d_clear_flag=	"D_CLEAR";
static	char		*m_clear_flag=	"M_CLEAR";
static	char		*y_clear_flag=	"Y_CLEAR";

static	char		*tmp_float=	"TMP_FLOAT";
static	char		*tmp_status= "TMP_STATUS";

static	char		def_filename[LINE_BUF_CHAR];
static	char		filename[LINE_BUF_CHAR];
static	char		genfilebuf[LINE_BUF_CHAR];
static	char		gentxtfilebuf[LINE_BUF_CHAR];
static	char		bakfilebuf[LINE_BUF_CHAR];
static	char		errfilebuf[LINE_BUF_CHAR];
static	char		oper[DATA_NAME_LONG];

static	char		filebuf[LINE_BUF_CHAR];

static  char		*gentxtfile="CalcuGenName.txt";
static	char		*bakfile="CalcuGen.bak";
static	char		*errfile="CalcuErr.rec";
static  char		*gentxtfile1="Calcu1GenName.txt";
static	char		*bakfile1="Calcu1Gen.bak";
static	char		*errfile1="Calcu1Err.rec";


static	char 		*dat_file_a = "Gen.dat";
static	char 		*txt_file_a = "GenName.txt";
static	char 		*bak_file_a = "Gen.bak";
static	char 		*err_file_a = "Err.rec";

static	short		pt_num;
static	short		gt_num;
static	short		et_num;
static	short		gt_num_rec;
static	short		et_num_rec;
static	short		layer_num;
static	short		gt_buf[LAYER_NUM];
static	short		et_buf[LAYER_NUM];

static	Calcu		calcu[CALCU_NUM];
static	C_Group		group[OR_NUM];
static	C_Entry		entry[AND_NUM];

static void	Dele_Space()
{
	while(filebuf[n]==' '||filebuf[n]==TAB)
	{
		n++;
	}
}

static int	Fgets_Line()
{
	while(1)
	{
		r++;

		if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if(feof(fp))
			{
				f++;
				printf("file %s exit without END\n",filename);
				return -1;
			}
			else
			{
				f++;
				fprintf(ferr,"Error!%3d line: is a null line.\n",r);
				continue;
			}
		}
		n=0;
/*
		Dele_Space();

		if(filebuf[n]==LF||filebuf[n]==0||(filebuf[n]==CR))
		{
			fprintf(ftxt,"%3d:\n",r);
			continue;
		}
*/
		return n;
	}
}

static	int	DefFgets_Line()
{
	while(1)
	{
		line_num++;

		if (fgets(filebuf,LINE_BUF_CHAR,fpdef)==NULL)
		{
			if (feof (fpdef))
			{
				f++;
				printf ("file %s exit without END\n",def_filename);
				fprintf (ferr,"file %s exit without END\n",def_filename);
				return -1;
			}
			else
			{
				f++;
				fprintf (ferr,"Error! %3d line: is a null line.\n",line_num);
				continue;
			}
		}
		n=0;
		return n;
	}
}

void Read_Entry()
{
	char	data_name[DATA_NAME_LONG];

	int	i,len,stint,iTmp;
	int	dat_type,dspflg;
	int	ret;
	DMS_COMMON	dbptr;
	POINTER	point;
	UINT	chan_fail;
	float	fValue;

	i=0;

	if(et_num>=AND_NUM)
	{
		f++;
		fprintf(ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if(group[gt_num].e_num)
			group[gt_num].e_num--;
	}
	while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
	{
		entry[et_num].name[i]=filebuf[n];
		i++;n++;
	}
	if(i==0)
	{
		return;
	}
	entry[et_num].name[i]=0;
	entry[et_num].layer=layer_num;

	Dele_Space();
	switch(filebuf[n])
	{
	case '?':
		n++;
		if((filebuf[n]=='C')||(filebuf[n]=='c'))
			entry[et_num].opr=CHANFAIL;
		else
			entry[et_num].opr=BIN_LG;
		break;

	case '<':
		n++;
		if(filebuf[n]!='=')
			entry[et_num].opr=LESS;
		else
			entry[et_num].opr=LESS_E;
		break;

	case '>':
		n++;
		if(filebuf[n]!='=')
			entry[et_num].opr=LARGE;
		else
			entry[et_num].opr=LARGE_E;
		break;

	case '=':
		n++;
		if(filebuf[n]!='=')
		{
			f++;
			fprintf(ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
		}
		entry[et_num].opr=EQUAL;
		break;

	case '!':
		n++;
		if(filebuf[n]!='=')
		{
			f++;
			fprintf(ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
		}
		entry[et_num].opr=N_EQUAL;
		break;

	case LF:
	case CR:
	case 0:
		entry[et_num].opr=BIN_LG;
		f++;
		fprintf(ferr,"Error!%3d line:%s Miss operator\n",r,filebuf);
		break;

	default :
		n++;
		f++;
		fprintf(ferr,"Error!%3d line:Error operator! %s\n",r,filebuf);
		entry[et_num].opr=BIN_LG;
	}

	n++;
	Dele_Space();

	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<DATA_NAME_LONG-1)
	{
		data_name[i]=filebuf[n];
		i++;n++;
	}
	data_name[i]=0;

	if(strncmp(tmp_float,entry[et_num].name,strlen(tmp_float))==0)
	{
		len=strlen(tmp_float);
		dat_type=ANA_TYPE;
		if(sscanf(&entry[et_num].name[len],"%d",&iTmp)==0)
		{
			f++;
			fprintf(ferr,"Error!%3d line: TMP_FLOAT=%s is Wrong!\n",r,entry[et_num].name);
		}
		dspflg=1;
		if((iTmp<0)||(iTmp>999))
		{
			f++;
			fprintf(ferr,"Error!%3d line: %s wrong TMP_FLOAT number(0-999)!\n",r,entry[et_num].name);
		}
	}
	else if(strncmp(tmp_status,entry[et_num].name,strlen(tmp_status))==0)
	{
		len=strlen(tmp_status);
		dat_type=POL_TYPE;
		if(sscanf(&entry[et_num].name[len],"%d",&iTmp)==0)
		{
			f++;
			fprintf(ferr,"Error!%3d line: TMP_STATUS=%s is Wrong!\n",r,entry[et_num].name);
		}
		dspflg=1;
		if((iTmp<0)||(iTmp>999))
		{
			f++;
			fprintf(ferr,"Error!%3d line: %s wrong TMP_STATUS number(0-999)!\n",r,entry[et_num].name);
		}
	}
	else
	{
//		ret = ReadEntryByNameStr(entry[et_num].name, &dbptr);
		ret= GetPtDataByStr(entry[et_num].name, &fValue, &chan_fail, &point);

		if(ret==-1)
		{
			f++;
			fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
		}
		else
			ReadEntryById(&point, &dbptr);
		dat_type = point.data_type;
	}
	if((entry[et_num].opr==BIN_LG)||(entry[et_num].opr==CHANFAIL))
	{
/*
		if (dat_type!=SOE_TYPE && dat_type!=POL_TYPE && dat_type!=SYSMSG_TYPE && dat_type!=-1)
		{
			f++;
			fprintf(ferr,"Error!%3d line:The data_type of %s isn't a ON_OFF variable!\n",
			        r,entry[et_num].name);
		}
*/
		dspflg=0;
		len=0;
		if(data_name[len]!='~')
			len=0;
		else
			len=1;

		if(!strcmp(&data_name[len],"CLOSE"))
		{
			entry[et_num].data.status=CLOSE;
		}
		else
		{
			if(!strcmp(&data_name[len],"OPEN"))
			{
				entry[et_num].data.status=OPEN;
			}
			else
			{
				if(sscanf(&data_name[len],"%d",&entry[et_num].data.status)==0)
				{
					f++;
					fprintf(ferr,"Error!%3d line: value=%s is not a short\n",r,data_name);
				}
				dspflg=1;
				if(entry[et_num].data.status<0 || entry[et_num].data.status>15)
				{
					entry[et_num].data.status=OPEN;
					f++;
					fprintf(ferr,"Error!%3d line:%s The status is out of 0--15\n",r,filebuf);
				}
			}
		}
		if(len==1)
			entry[et_num].data.status=entry[et_num].data.status+0xff00;
	}
	else
	{
		if (dat_type!=ANA_TYPE && dat_type!=CALC_TYPE && dat_type!=PARAM_TYPE && dat_type!=-1)
		{
			f++;
			fprintf(ferr,"Error!%3d line:The data_type of %s isn't a ANALOG/CALCU variable!\n",
			        r,entry[et_num].name);
		}
		if(sscanf(data_name,"%f",&entry[et_num].data.value)==0)
		{
			f++;
			fprintf(ferr,"Error!%3d line: value=%s is not a float\n",r,data_name);
		}
	}
	if (ret==-1)
	{
		fprintf (ftxt,"Error!%3d:\t%s%-24s ",r,oper,entry[et_num].name);
	}
	else
		fprintf (ftxt,"%3d:\t%s%-36s%-24s ",r,oper,dbptr.fixed.entryLongname,entry[et_num].name);

	switch(entry[et_num].opr)
	{
	case LESS:
				fprintf(ftxt,"<   ");
				break;
	case LESS_E:
				fprintf(ftxt,"<=  ");
				break;
	case LARGE:
				fprintf(ftxt,">   ");
				break;
	case LARGE_E:
				fprintf(ftxt,">=  ");
				break;
	case EQUAL:
				fprintf(ftxt,"==  ");
				break;
	case N_EQUAL:
				fprintf(ftxt,"!=  ");
				break;
	case CHANFAIL:
				fprintf(ftxt,"?c  ");
				break;
	default:
				fprintf(ftxt,"?   ");
	}
	if((entry[et_num].opr==BIN_LG)||(entry[et_num].opr==CHANFAIL))
	{
		stint=entry[et_num].data.status&0xff;
		if(dspflg==0)
		{
			if((entry[et_num].data.status&0xff00)==0)
			{
				if(stint==CLOSE)
					fprintf(ftxt,"CLOSE    ");
				else
					fprintf(ftxt,"OPEN     ");
			}
			else
			{
				if(stint==CLOSE)
					fprintf(ftxt,"~CLOSE   ");
				else
					fprintf(ftxt,"~OPEN    ");
			}
		}
		else if((entry[et_num].data.status&0xff00)==0)
			fprintf(ftxt,"%-5d    ",stint);
		else
		{
			fprintf(ftxt,"~%-5d   ",stint);
		}
	}
	else
		fprintf(ftxt,"%9.3f",entry[et_num].data.value);
	fprintf(ftxt,"\n");
	group[gt_num].e_num++;
#ifdef DEBUG
	fprintf(ftxt,"\t\t<group:%d total_et:%d g_offset:%d e_num:%d layer:%d/%d pt_gt:%d >\n",gt_num,et_num,
				group[gt_num].offset,group[gt_num].e_num,group[gt_num].layer,entry[et_num].layer,calcu[pt_num].g_num);
#endif
	et_num++;
}

static void Read_Cal_Entry()
{
	int	i,dat_type,len,iTmp;
	int	ret;
	DMS_COMMON	dbptr;
	POINTER	point;
	UINT	chan_fail;
	float	fValue;

	if(et_num>=AND_NUM)
	{
		f++;
		fprintf(ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if(group[gt_num].e_num)
			group[gt_num].e_num--;
	}
	if(entry[et_num].opr==SQRT)
	{
		fprintf(ftxt,"%3d:\t%s\n",r,oper);
#ifdef DEBUG
		fprintf(ftxt,"%48<group:%d entry:%d gt:%d g_offset:%d et:%d>\n",calcu[pt_num].g_num,
				group[gt_num].e_num,gt_num,group[gt_num].offset,et_num);
#endif
		et_num++;
		group[gt_num].e_num++;
		return;
	}
	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
	{
		entry[et_num].name[i]=filebuf[n];
		i++;n++;
	}
	entry[et_num].name[i]=0;

	if(entry[et_num].opr&RESET_C)
	{
		if(sscanf(entry[et_num].name,"%f",&entry[et_num].data.value)==0)
		{
			f++;
			fprintf(ferr,"Error!%3d line: value=%s is not a float\n",r,entry[et_num].name);
		}
		fprintf(ftxt,"%3d:\t%s\t%-9.3f\t\t",r,oper,entry[et_num].data.value);
	}
	else
	{
		if(strncmp(tmp_float,entry[et_num].name,strlen(tmp_float))==0)
		{
			len=strlen(tmp_float);
			dat_type=ANA_TYPE;
			if(sscanf(&entry[et_num].name[len],"%d",&iTmp)==0)
			{
				f++;
				fprintf(ferr,"Error!%3d line: TMP_FLOAT=%s is Wrong!\n",r,entry[et_num].name);
			}
			if((iTmp<0)||(iTmp>999))
			{
				f++;
				fprintf(ferr,"Error!%3d line: %s wrong TMP_FLOAT number(0-999)!\n",r,entry[et_num].name);
			}
			fprintf(ftxt,"%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
		}
	 	else if(strncmp(tmp_status,entry[et_num].name,strlen(tmp_status))==0)
		{
			len=strlen(tmp_status);
			dat_type=POL_TYPE;
			if(sscanf(&entry[et_num].name[len],"%d",&iTmp)==0)
			{
				f++;
				fprintf(ferr,"Error!%3d line: TMP_STATUS=%s is Wrong!\n",r,entry[et_num].name);
			}
			if((iTmp<0)||(iTmp>999))
			{
				f++;
				fprintf(ferr,"Error!%3d line: %s wrong TMP_STATUS number(0-999)!\n",r,entry[et_num].name);
			}
			fprintf(ftxt,"%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
		}
		else
		{
//			ret = ReadEntryByNameStr(entry[et_num].name,&dbptr);
			ret= GetPtDataByStr(entry[et_num].name, &fValue, &chan_fail, &point);

			if(ret==-1)
			{
				f++;
				fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
				fprintf(ftxt,"Error!%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
			}
			else
			{
				ReadEntryById(&point, &dbptr);
				fprintf (ftxt,"%3d:\t%s%-36s%-24s ",r,oper,dbptr.fixed.entryLongname,entry[et_num].name);
			}
			dat_type = point.data_type;
		}
	}
	fprintf(ftxt,"\n");
	group[gt_num].e_num++;
#ifdef DEBUG
	fprintf(ftxt,"\t\t<group:%d entry:%d g_offset:%d layer:%d/%d pt_gt:%d total_et:%d>\n",gt_num,group[gt_num].e_num,
				group[gt_num].offset,group[gt_num].layer,entry[et_num].layer,calcu[pt_num].g_num,et_num);
#endif
	et_num++;
}

static void Read_Diff_Entry()
{
	int	i,dat_type,ret;
	DMS_COMMON	dbptr;
	POINTER	point;
	UINT	chan_fail;
	float	fValue;

	if(et_num>=AND_NUM)
	{
		f++;
		fprintf(ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if(group[gt_num].e_num)
			group[gt_num].e_num--;
	}
	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
	{
		entry[et_num].name[i]=filebuf[n];
		i++;n++;
	}
	entry[et_num].name[i]=0;

	if(entry[et_num].opr==DIFF_C)
	{
		if(sscanf(entry[et_num].name,"%d",&entry[et_num].data.status)==0)
		{
			f++;
			fprintf(ferr,"Error!%3d line: value=%s is not an int\n",r,entry[et_num].name);
			fprintf(ftxt,"Error!%3d:\t%s",r,entry[et_num].name);
		}
		else
		{
			fprintf(ftxt,"%3d:\t%s\t%d",r,oper,entry[et_num].data.status);
		}
	}
	if(entry[et_num].opr==DIFF_S)
	{
		fprintf(ftxt,"%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
	}
	if((entry[et_num].opr!=DIFF_C)&&(entry[et_num].opr!=DIFF_S))
	{
		if((entry[et_num].opr==3)||(entry[et_num].opr==4))
		{
//			ret = ReadEntryByNameStr(entry[et_num].name, &dbptr);
			ret= GetPtDataByStr(entry[et_num].name, &fValue, &chan_fail, &point);

			if(ret==-1)
			{
				f++;
				fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
				fprintf(ftxt,"Error!%3d:\t%s",r,entry[et_num].name);
			}
			else
			{
				ReadEntryById(&point, &dbptr);
				fprintf (ftxt,"%3d:\t%s%-36s%-24s ",r,oper,dbptr.fixed.entryLongname,entry[et_num].name);
			}
			dat_type = dbptr.point.data_type;
		}
		else
			fprintf(ftxt,"%3d:\t\t%-24s ",r,entry[et_num].name);
	}
	fprintf(ftxt,"\n");
	group[gt_num].e_num++;
#ifdef DEBUG
	fprintf(ftxt,"\t\t\t<group:%d entry:%d g_offset:%d layer:%d/%d pt_gt:%d total_et:%d>\n",gt_num,group[gt_num].e_num,
				group[gt_num].offset,group[gt_num].layer,entry[et_num].layer,calcu[pt_num].g_num,et_num);
#endif
	et_num++;
}

static void Read_Clear_Entry()
{
	int	i,dat_type,ret;
	DMS_COMMON	dbptr;
	POINTER	point;
	UINT	chan_fail;
	float	fValue;

	if(et_num>=AND_NUM)
	{
		f++;
		fprintf(ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if(group[gt_num].e_num)
			group[gt_num].e_num--;
	}
	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
	{
		entry[et_num].name[i]=filebuf[n];
		i++;n++;
	}
	entry[et_num].name[i]=0;

//	ret = ReadEntryByNameStr(entry[et_num].name, &dbptr);
	ret= GetPtDataByStr(entry[et_num].name, &fValue, &chan_fail, &point);
  	if(ret==-1)
	{
		f++;
		fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
		fprintf(ftxt,"Error!%3d:\t\t%-24s No this point in DB",r,entry[et_num].name);
	}
	else
	{
		ReadEntryById(&point, &dbptr);
		fprintf(ftxt,"%3d:\t\t%-36s%-24s",r,dbptr.fixed.entryLongname,entry[et_num].name);
	}
	dat_type = point.data_type;
	fprintf(ftxt,"\n");
	group[gt_num].e_num++;
#ifdef DEBUG
	fprintf(ftxt,"%22<group:%d entry:%d gt:%d g_offset:%d et:%d>\n",calcu[pt_num].g_num,
			group[gt_num].e_num,gt_num,group[gt_num].offset,et_num);
#endif
	et_num++;
}


void Write_Data_File()
{
	char	temp;
	char	verstr[VERLEN];

	struct	tm	tmpt;
	time_t		tim;

	short	num;

	if((fp1=fopen(genfilebuf,"rb"))==NULL)
	{
		printf("No bak file generated ! can't open(rb) the old %s\n",genfilebuf);
	}
	else
	{
		if((fpb=fopen(bakfilebuf,"wb"))==NULL)
		{
			printf("No bak file generated ! can't open(wb) the file %s!\n",bakfilebuf);
		}
		else
		{
			temp=fgetc(fp1);
			while(!feof(fp1))
			{
				fputc(temp,fpb);
				temp=fgetc(fp1);
			}
			fclose(fpb);
		}
		fclose(fp1);
	}

	tim=time(NULL);
	tmpt=*localtime(&tim);

	sprintf(verstr,"%s %04d/%02d/%02d %02d:%02d:%02d",VERSTR,tmpt.tm_year+1900,
		tmpt.tm_mon+1,tmpt.tm_mday,tmpt.tm_hour,tmpt.tm_min,tmpt.tm_sec);

	if((fp1=fopen(genfilebuf,"wb"))==NULL)
	{
		printf("No date file generated ! can't open(wb) the file %s!\n",genfilebuf);
		return;
	}
	fwrite(&verstr[0],VERLEN,1,fp1);

	gt_num = gt_num_rec;
	pt_num++;
	gt_num++;

	for( num=0; num < pt_num; num++ )
	{
		Swap2Byte(&calcu[num].type);
		Swap2Byte(&calcu[num].g_num);
		Swap2Byte(&calcu[num].offset);
	}
	for( num=0; num < gt_num; num++ )
	{
		Swap2Byte(&group[num].layer);
		Swap2Byte(&group[num].e_num);
		Swap2Byte(&group[num].offset);
	}
	for( num=0; num < et_num; num++ )
	{
		if(entry[num].opr == BRACKET_F)
		{
			Swap2Byte(&entry[num].data.group.g_num);
			Swap2Byte(&entry[num].data.group.offset);
		}
		else
			Swap4Byte(&entry[num].data.value);
		Swap2Byte(&entry[num].opr);
		Swap2Byte(&entry[num].layer);
	}
	num = pt_num;
	Swap2Byte(&num);
	fwrite(&num,sizeof(short),1,fp1);
	num = gt_num;
	Swap2Byte(&num);
	fwrite(&num,sizeof(short),1,fp1);
	num = et_num;
	Swap2Byte(&num);
	fwrite(&num,sizeof(short),1,fp1);

	fwrite(&calcu[0],sizeof(Calcu),pt_num,fp1);
	fwrite(&group[0],sizeof(C_Group),gt_num,fp1);
	fwrite(&entry[0],sizeof(C_Entry),et_num,fp1);
	fclose(fp1);
	return;
}

int	Load_Calcu_File()
{
	int		i,flg,len,dat_type,ret,iTmp;
	char	data_name[DATA_NAME_LONG];
	DMS_COMMON	dbptr;
	short	pt_num_sav;
	short	gt_num_sav;
	short	et_num_sav;
	short	f_sav;
	POINTER	point;
	UINT	chan_fail;
	float	fValue;

/*
	pt_num=-1;
	gt_num=gt_num_rec=-1;
	et_num=et_num_rec=0;
	for(i=0;i<CALCU_NUM;i++)
	{
		calcu[i].g_num=0;
		for(l=0;l<OR_NUM;l++)
		{
			calcu[i].group[l].e_num=0;
		}
	}
*/
	r=n=0;
	layer_num=0;

	pt_num_sav = pt_num;
	gt_num_sav = gt_num;
	et_num_sav = et_num;
	f_sav = f;

	if((fp=fopen(filename,"rb"))==NULL)
	{
		printf("calcu.c:can't open the file %s\n",filename);
		return -1;
	}

	if(Fgets_Line()==-1)
	{
		printf("the file is empty!exit now!\n");
		return -1;
	}
	if(strncmp(filehead,&filebuf[n],strlen(filehead)))
	{
		printf("The file miss BEGIN: the first line is %s Exit now!\n",filebuf);
		return -1;
	}
	fprintf(ftxt,"%3d:%s",r,filebuf);

	while(pt_num<CALCU_NUM)
	{
		if(Fgets_Line()==-1)
		{
			fclose(fp);
			printf ("\n\tError! EOF without END! Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
			fprintf (ftxt,"\n\tError! EOF without END! Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
			fprintf (ferr,"\nError! EOF without END! Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
			if (f != f_sav)
			{
				printf ("  :: %d ERROR in the file %s!\n",f-f_sav,filename);
			}
			else
				printf ("  :: %s ----------OK!-----------\n",filename);
			return -1;
		}
		if(strncmp(pt_flag,filebuf,strlen(pt_flag))==0)
		{
			if(layer_num)
			{
				f++;
				fprintf(ferr,"Error!%3d line: POINT:%s The bracket num=%d in calcu point not full! \n",r-1,calcu[pt_num].o_name,layer_num);
				layer_num = 0;
			/*	break; */
			}
			n=strlen(pt_flag);
			if(pt_num>=CALCU_NUM-1)
			{
				f++;
				fprintf(ferr,"Error!%3d line:The calcu point has reached the MAX %d exit now!\n",CALCU_NUM);
				break;
			}
			pt_num++;
			Dele_Space();
			i=0;
			while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<POINT_NAME_SIZE-1)
			{
				calcu[pt_num].o_name[i]=filebuf[n];
				i++;n++;
			}
			calcu[pt_num].o_name[i]=0;
			calcu[pt_num].g_num=0;
			calcu[pt_num].type= 0;	/* modified to 0 for D_CLEAR M_CLEAR and Y_CLEAR */

			if(strncmp(d_clear_flag,calcu[pt_num].o_name,strlen(d_clear_flag))==0)
				calcu[pt_num].type=D_CLEAR;
			else if(strncmp(m_clear_flag,calcu[pt_num].o_name,strlen(m_clear_flag))==0)
				calcu[pt_num].type=M_CLEAR;
			else if(strncmp(y_clear_flag,calcu[pt_num].o_name,strlen(y_clear_flag))==0)
				calcu[pt_num].type=Y_CLEAR;
			else if(strncmp(tmp_float,calcu[pt_num].o_name,strlen(tmp_float))==0)
			{
				len=strlen(tmp_float);
				if(sscanf(&calcu[pt_num].o_name[len],"%d",&iTmp)==0)
				{
					f++;
					fprintf(ferr,"Error!%3d line: TMP_FLOAT=%s is Wrong!\n",r,calcu[pt_num].o_name);
				}
				else if((iTmp<0)||(iTmp>999))
				{
					f++;
					fprintf(ferr,"Error!%3d line: %s wrong TMP_FLOAT number(0-999)!\n",r,calcu[pt_num].o_name);
					fprintf(ftxt,"Error!%3d:\tPOINT:  %s ",r,calcu[pt_num].o_name);
				}
				else
					fprintf(ftxt,"%3d:\tPOINT:  %s ",r,calcu[pt_num].o_name);
			}
			else if(strncmp(tmp_status,calcu[pt_num].o_name,strlen(tmp_status))==0)
			{
				len=strlen(tmp_status);
				if(sscanf(&calcu[pt_num].o_name[len],"%d",&iTmp)==0)
				{
					f++;
					fprintf(ferr,"Error!%3d line: TMP_STATUS=%s is Wrong!\n",r,calcu[pt_num].o_name);
				}
				else if((iTmp<0)||(iTmp>999))
				{
					f++;
					fprintf(ferr,"Error!%3d line: %s wrong TMP_STATUS number(0-9)!\n",r,calcu[pt_num].o_name);
					fprintf(ftxt,"Error!%3d:\tPOINT:  %s ",r,calcu[pt_num].o_name);
				}
				else
					fprintf(ftxt,"%3d:\tPOINT:  %s ",r,calcu[pt_num].o_name);
			}
			else
			{
//				ret = ReadEntryByNameStr(calcu[pt_num].o_name, &dbptr);
				ret= GetPtDataByStr(calcu[pt_num].o_name, &fValue, &chan_fail, &point);
				dat_type = point.data_type;

				if(ret==-1)
				{
					f++;
					fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,calcu[pt_num].o_name);
					fprintf(ftxt,"Error!%3d:\tPOINT:  %s ",r,calcu[pt_num].o_name);
				}
				else
				{
					ReadEntryById(&point, &dbptr);
					fprintf(ftxt,"%3d:\tPOINT:  %s \t%-24s ",r,dbptr.fixed.entryLongname,calcu[pt_num].o_name);
				}
			}
			Dele_Space();
			switch(filebuf[n])
			{
			case 'C':
				calcu[pt_num].type=CALCU;	/*for calculator type*/
				break;
			case 'L':
				calcu[pt_num].type=LOGIC;	/*for logic type*/
				break;
			case 'D':
				calcu[pt_num].type=DIFF;	/*for diff type*/
				break;
			default:
				i=0;				/*for multi logic type*/
				while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<DATA_NAME_LONG-1)
				{
					data_name[i]=filebuf[n];
					i++;n++;
				}
				data_name[i]=0;
				if((calcu[pt_num].type==D_CLEAR)||(calcu[pt_num].type==M_CLEAR)||(calcu[pt_num].type==Y_CLEAR))
				{
					if(i==0)
					{
						calcu[pt_num].g_num=0;
						f++;
						fprintf(ferr,"Error!%3d line: no hour number\n",r);
					}
					else
					{
						if(sscanf(data_name,"%hd",&calcu[pt_num].g_num)==0)
						{
							f++;
							fprintf(ferr,"Error!%3d line: hour=%s is not a hour\n",r,data_name);
							calcu[pt_num].g_num=0;
						}
					}
					if((calcu[pt_num].g_num<0)||(calcu[pt_num].g_num>23))
					{
						f++;
						fprintf(ferr,"Error!%3d line: hour=%s is not a hour number\n",r,data_name);
						calcu[pt_num].g_num=0;
					}
					break;
				}
				if(i==0)
					calcu[pt_num].type=LOGIC;
				else
				{
					if(sscanf(data_name,"%hd",&calcu[pt_num].type)==0)
					{
						f++;
						fprintf(ferr,"Error!%3d line: value=%s is not a short\n",r,data_name);
					}
				}
			}

			if(gt_num>=OR_NUM-1)
			{
				f++;
				fprintf(ferr,"Error!%3d line:The calcu group has reached the MAX %d overlapped!\n",r,OR_NUM);
			}
			else
			{
				gt_num_rec++;
				gt_num = gt_num_rec;
			}
			calcu[pt_num].offset=gt_num;

			group[gt_num].layer=layer_num;
			group[gt_num].e_num=0;
			group[gt_num].offset=et_num;

			flg=1;
		/*	fprintf(ftxt,"%3d:\tPOINT:  %-24s ",r,calcu[pt_num].o_name); */
			if(calcu[pt_num].type==LOGIC)
				fprintf(ftxt,"LOGIC  \t");
			else if(calcu[pt_num].type==CALCU)
				fprintf(ftxt,"CALCU  \t");
			else if(calcu[pt_num].type==DIFF)
				fprintf(ftxt,"DIFF   \t");
			else if(calcu[pt_num].type==D_CLEAR)
				fprintf(ftxt,"%3d:\tPOINT:\tD_CLEAR\t%d",r,calcu[pt_num].g_num);
			else if(calcu[pt_num].type==M_CLEAR)
				fprintf(ftxt,"%3d:\tPOINT:\tM_CLEAR\t%d",r,calcu[pt_num].g_num);
			else if(calcu[pt_num].type==Y_CLEAR)
				fprintf(ftxt,"%3d:\tPOINT:\tY_CLEAR\t%d",r,calcu[pt_num].g_num);
			else
				fprintf(ftxt,"0x%4x \t",calcu[pt_num].type);
			fprintf(ftxt,"\n");
#ifdef DEBUG
			fprintf(ftxt,"\t<<No: %d offset:%d>>\n",pt_num,calcu[pt_num].offset);
#endif
			continue;
		}
		else
		{
			if(strncmp(filetail,filebuf,strlen(filetail))==0)
			{
				fclose(fp);
			/*	Write_Data_File(); */

				printf ("\n\tEND reacked,Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
				fprintf (ftxt,"\n\tEND reacked,Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
				if (f != f_sav)
				{
					printf ("  :: %d ERROR in the file %s!\n",f-f_sav,filename);
				}
				else
					printf ("  :: %s ----------OK!-----------\n",filename);
				return 0;
			}
		}

		Dele_Space();
	/*	if(calcu[pt_num].type==LOGIC) */
		if((calcu[pt_num].type!=DIFF)&&(calcu[pt_num].type!=CALCU)&&(calcu[pt_num].type!=D_CLEAR)&&
				(calcu[pt_num].type!=M_CLEAR)&&(calcu[pt_num].type!=Y_CLEAR))
		{
			strcpy(oper," ");
			while(filebuf[n]==')')
			{
				if(flg==1)
				{
					strcat(oper,"=");
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				group[gt_num].e_num =et_num - group[gt_num].offset;
				layer_num --;
				if(layer_num<0)
				{
					layer_num = 0;
					f++;
					fprintf(ferr,"Error!%3d line:%s The bracket now %d has below 0!\n",r,filebuf,layer_num);
				}
				gt_num = gt_buf[layer_num];
				group[gt_num].e_num = et_num - group[gt_num].offset;
				et_num_rec = et_buf[layer_num];
				entry[et_num_rec].data.group.g_num = gt_num_rec - entry[et_num_rec].data.group.offset + 1;
				n ++;
				strcat(oper,")");
				flg = 3;
#ifdef DEBUG
				fprintf(ftxt,"\t\t}bracket et_num:%d g_offset:%d g_num:%d lay=%d\n",et_num_rec,entry[et_num_rec].data.group.offset,
					entry[et_num_rec].data.group.g_num,layer_num);
				fprintf(ftxt,"\t}<group:%d g_offset:%d g_num:%d layer:%d/%d\n",gt_num,group[gt_num].offset,
					group[gt_num].e_num,group[gt_num].layer,entry[et_num_rec].layer);
#endif
			}
			Dele_Space();
			switch(filebuf[n])
			{
			case '=':
				if(flg==1)
				{
					strcat(oper,"=");
				}
				else
				{
		          	strcat(oper,"&");
					f++;
					fprintf(ferr,"Error!%3d line:%s More than one '=' in a point!\n",r,filebuf);
				}
				flg=2;
				n++;
				break;
			case '(':
				if(flg==1)
				{
					strcat(oper,"=");
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				while(filebuf[n]=='(')
				{
					entry[et_num].opr=(short)BRACKET_F;
					entry[et_num].layer=layer_num;
					group[gt_num].e_num ++;
#ifdef DEBUG
					fprintf(ftxt,"\t{<group:%d include bracket et_num:%d g_offset:%d layer:%d/%d\n",gt_num,et_num,
						group[gt_num].offset,group[gt_num].layer,entry[et_num].layer);
#endif
					gt_buf[layer_num] = gt_num;
					gt_num_rec++;
					gt_num = gt_num_rec;
					entry[et_num].data.group.offset=gt_num;
					entry[et_num].data.group.g_num=0;
					et_buf[layer_num] = et_num;
#ifdef DEBUG
					fprintf(ftxt,"\t\t{bracket et_num:%d e_g_offset:%d>\n",et_num,entry[et_num].data.group.offset);
#endif
					et_num ++;
					layer_num ++;
					if(layer_num>=LAYER_NUM)
					{
						f++;
						fprintf(ferr,"Error!%3d line:%s The bracket now %d has exceed the MAX lay %d!\n",r,filebuf,layer_num,LAYER_NUM);
					}

					calcu[pt_num].g_num++;
					group[gt_num].e_num =0;
					group[gt_num].layer = layer_num;
					group[gt_num].offset = et_num;
					n ++;
					strcat(oper,"(");
				}
				flg=2;
				break;
			case '&':
				if(flg!=1)
					strcat(oper,"&");
				else
				{
					strcat(oper,"=");
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				n++;
				flg=2;
				break;
			case '+':
				if(flg!=1)
				{
					if(gt_num>=OR_NUM -1)
					{
						f++;
						fprintf(ferr,"Error!%3d line:The group has reached the Max=%d Overlapped!\n",r,OR_NUM);
					}
					else
					{
						gt_num_rec++;
						gt_num = gt_num_rec;
						calcu[pt_num].g_num++;
						group[gt_num].e_num = 0;
						group[gt_num].layer = layer_num;
						group[gt_num].offset = et_num;
					}
					strcat(oper,"+");
				}
				else
				{
					strcat(oper,"=");
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				n++;
				flg=2;
				break;
			case 0  :
			case LF:
			case CR:
				if(flg == 3)
					fprintf(ftxt,"%3d:\t%s\n",r,oper);
				else
					fprintf(ftxt,"%3d:\n",r);
				continue;
			case ';':
				fprintf(ftxt,"%3d:Descript line: %s\n",r,filebuf);
				continue;
			default :
				if(flg != 3)
				{
					f++;
					fprintf(ferr,"Error!%3d line:bad operator! %s\n",r,filebuf); 
					continue;
				}
			}
			Dele_Space();
			while(filebuf[n]=='(')
			{
				entry[et_num].opr=(short)BRACKET_F;
				entry[et_num].layer=layer_num;
				group[gt_num].e_num ++;
#ifdef DEBUG
				fprintf(ftxt,"\t{<group:%d include bracket et_num:%d g_offset:%d layer:%d/%d\n",gt_num,et_num,
						group[gt_num].offset,group[gt_num].layer,entry[et_num].layer);
#endif
				gt_buf[layer_num] = gt_num;
				gt_num_rec++;
				gt_num = gt_num_rec;
				entry[et_num].data.group.offset=gt_num;
				entry[et_num].data.group.g_num=0;
				et_buf[layer_num] = et_num;
#ifdef DEBUG
				fprintf(ftxt,"\t\t{bracket et_num:%d e_g_offset:%d>\n",et_num,entry[et_num].data.group.offset);
#endif
				et_num ++;
				layer_num ++;
				if(layer_num>=LAYER_NUM)
				{
					f++;
					fprintf(ferr,"Error!%3d line:%s The bracket now %d has exceed the MAX lay %d!\n",r,filebuf,layer_num,LAYER_NUM);
				}

				calcu[pt_num].g_num++;
				group[gt_num].e_num =0;
				group[gt_num].layer = layer_num;
				group[gt_num].offset = et_num;
				n ++;
				strcat(oper,"(");
			}
			strcat(oper,"  ");
			Dele_Space();
			Read_Entry();
		}

		if(calcu[pt_num].type==CALCU)
		{
			switch(filebuf[n])
			{
			case 'I':
				n++;
				if(filebuf[n]!='F')
				{
					f++;
					fprintf(ferr,"Error!%3d line:bad operator! %s\n",r,filebuf);
					continue;
				}
				n++;
				strcpy(oper," IF ");
				calcu[pt_num].g_num=1;
				Dele_Space();
				Read_Entry();
				continue;
			case '=':
				n++;
				if(filebuf[n]!='C')
				{
					strcpy(oper," =  ");
					entry[et_num].opr=RESET;
				}
				else
				{
					strcpy(oper," =C ");
					entry[et_num].opr=RESET_C;
				}
				if(flg==1)
					flg=2;
				else
				{
					f++;
					fprintf(ferr,"Error!%3d line:%s More than one '=' in a point!\n",r,filebuf);
				}
				break;
			case '+':
				n++;
				if(filebuf[n]!='C')
				{
					strcpy(oper," +  ");
					entry[et_num].opr=ADD;
				}
				else
				{
					strcpy(oper," +C ");
					entry[et_num].opr=ADD_C;
				}
				if(flg!=2)
				{
					flg=2;
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				break;
			case '-':
				n++;
				if(filebuf[n]!='C')
				{
					strcpy(oper," -  ");
				 	entry[et_num].opr=MIN;
				}
				else
				{
					strcpy(oper," -C ");
					entry[et_num].opr=MIN_C;
				}
				if(flg!=2)
				{
					flg=2;
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				break;
			case '*':
				n++;
				if(filebuf[n]!='C')
				{
					strcpy(oper," *  ");
				  	entry[et_num].opr=MUL;
				}
				else
				{
					strcpy(oper," *C ");
					entry[et_num].opr=MUL_C;
				}
				if(flg!=2)
				{
					flg=2;
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				break;
			case '/':
				n++;
				if(filebuf[n]!='C')
				{
					strcpy(oper," /  ");
				  	entry[et_num].opr=DIV;
				}
				else
				{
					strcpy(oper," /C ");
					entry[et_num].opr=DIV_C;
				}
				if(flg!=2)
				{
					flg=2;
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				break;
			case 'S':
				n++;
				if((filebuf[n]=='Q')&&(filebuf[n+1]=='R')&&(filebuf[n+2]=='T'))
				{
					strcpy(oper," SQRT ");
					entry[et_num].opr=SQRT;
				}
				else
				{
					f++;
					fprintf(ferr,"Error!%3d line:bad operator! %s\n",r,filebuf);
					continue;
				}
				if(flg!=2)
				{
					flg=2;
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				break;
			case 0  :
			case LF :
			case CR :
				fprintf(ftxt,"%3d:\n",r);
				continue;
			case ';':
				fprintf(ftxt,"%3d:Descript line: %s\n",r,filebuf);
				continue;
			default : 
				f++;
				fprintf(ferr,"Error!%3d line:bad operator! %s\n",r,filebuf);
				continue;
			}
			n++;
			Dele_Space();
			Read_Cal_Entry();
		}

		if(calcu[pt_num].type==DIFF)
		{
			switch(filebuf[n])
			{
			case '=':
				n++;
				if(filebuf[n]!='C')
				{
					strcpy(oper," =  ");
					entry[et_num].opr=DIFF_S;
				}
				else
				{
					strcpy(oper," =C ");
					entry[et_num].opr=DIFF_C;
				}
				if(flg==1)
					flg=2;
				else
				{
					flg=2;
					f++;
					fprintf(ferr,"Error!%3d line:%s More than one '=' in a point!\n",r,filebuf);
				}
				n++;
				break;
			case 0  :
			case LF :
			case CR :
				fprintf(ftxt,"%3d:\n",r);
				continue;
			case ';':
				fprintf(ftxt,"%3d:Descript line: %s\n",r,filebuf);
				continue;
			default :
				if(flg==1)
				{
					flg=2;
					f++;
					fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
				entry[et_num].opr=flg;
				flg++;
				break;
			}
			Dele_Space();
			Read_Diff_Entry();
		}
		if((calcu[pt_num].type==D_CLEAR)||(calcu[pt_num].type==M_CLEAR)||(calcu[pt_num].type==Y_CLEAR))
		{
			if((filebuf[n]==0)||(filebuf[n]==LF)||(filebuf[n]==CR))
			{
				fprintf(ftxt,"%3d:\n",r);
				continue;
			}
			if(filebuf[n]==';')
			{
				fprintf(ftxt,"%3d:Descript line: %s\n",r,filebuf);
				continue;
			}

			Read_Clear_Entry();
		}
	}
	fclose(fp);
/*
	Write_Data_File();
*/
	printf("\nTotal pt=%d gt=%d et=%d exit now!\n",pt_num,gt_num,et_num);
	printf ("\n\tTotal pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
	fprintf (ftxt,"\n\tTotal pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
	if (f != f_sav)
	{
		printf ("  :: %d ERROR in the file %s!\n",f-f_sav,filename);
	}
	else
		printf ("  :: %s ----------OK!-----------\n",filename);
	return 0;
}


void main(int argc, char *argv[])
{
	char	read_buf[LINE_BUF_CHAR];
	int	i,select;

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	for(;;)
	{
		select=0;
		f = 0;
		while( (select < 1) | (select > 3) )
		{
			printf("Please Select Following Function:\n");
			printf("\n 1) Calcu.def\n");
			printf("\n 2) Calcu1.def\n");
			printf("\n 3) Exit\n");
			printf("Please Select:( 1, 2, 3 ): ");

			scanf("%d", &select);
		}

		if( select == 1 )
		{
		/*	sprintf(filename, "%s%s", SYS_DEFINE_PATH, CALCU_DEF_FILE); */
			sprintf(def_filename, "%s%s", SYS_DEFINE_PATH, CALCU_FILE_DEF);
			sprintf(errfilebuf, "%s%s", SYS_DEFINE_PATH, errfile);
			sprintf(genfilebuf, "%s%s", SYS_DATA_PATH, CALCU_DATA_FILE);
			sprintf(gentxtfilebuf, "%s%s", SYS_DEFINE_PATH, gentxtfile);
			sprintf(bakfilebuf, "%s%s", SYS_DATA_PATH, bakfile);
		}
		else if( select == 2 )
		{
		/*	sprintf(filename, "%s%s", SYS_DEFINE_PATH, CALCU1_DEF_FILE); */
			sprintf(def_filename, "%s%s", SYS_DEFINE_PATH, CALCU1_FILE_DEF);
			sprintf(errfilebuf, "%s%s", SYS_DEFINE_PATH, errfile1);
			sprintf(genfilebuf, "%s%s", SYS_DATA_PATH, CALCU1_DATA_FILE);
			sprintf(gentxtfilebuf, "%s%s", SYS_DEFINE_PATH, gentxtfile1);
			sprintf(bakfilebuf, "%s%s", SYS_DATA_PATH, bakfile1);
		}
		else
		{
			UnMountDB();
			exit(1);
		}

		pt_num = -1;
		gt_num = gt_num_rec = -1;
		et_num = et_num_rec = 0;
		layer_num = 0;
		f = 0;

		if ((fpdef=fopen(def_filename,"rb"))==NULL)
		{
			printf ("calcugenname.c: can't open the calcu def file %s\n",def_filename);
			continue;
		}

		line_num = 0;

		if (DefFgets_Line()==-1)
		{
			printf ("the file is empty! Abort now!\n");
			fclose(fpdef);
			continue;
		}
		if (strncmp(filehead,&filebuf[n],strlen(filehead)))
		{
			printf ("The file %s miss BEGIN: the first line is %s Abort now!\n",def_filename,filebuf);
			fclose(fpdef);
			continue;
		}
		if((ferr=fopen(errfilebuf,"w"))==NULL)
		{
			printf("calcugen.c:can't open(w) the error record file %s!\n",errfilebuf);
			fclose(fpdef);
			continue;
		}
		if((ftxt=fopen(gentxtfilebuf,"w"))==NULL)
		{
			printf("calcugen.c:can't open(w) the text record file %s!\n",gentxtfilebuf);
			fclose(ferr);
			fclose(fpdef);
			continue;
		}
		fprintf (ftxt,":: Reading the calcu def file %s ...\n",def_filename);
		fprintf (ftxt,"\n\t%02d: %s",line_num,filebuf);

		for(;;)
		{
			if (DefFgets_Line()==-1)
			{
				fclose(fpdef);
				Write_Data_File();
				printf ("The calcu def file %s now EOF without END! Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
				fprintf (ftxt,"\n\tThe calcu def file %s now EOF without END! Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
				fprintf (ferr,"\n\tThe calcu def file %s now EOF without END! Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
				break;
			}
			if (filebuf[0]=='#')
			{
				printf ("\n  :: The calcu def file %s is disabled ...\n",filebuf);
				fprintf (ftxt,"\n\t %02d: The calcu def file %s disabled line %s\n",line_num,def_filename,filebuf);
				continue;
			}
			Dele_Space();

			if (filebuf[n]==LF||filebuf[n]==CR||filebuf[n]==0)
			{
				fprintf (ftxt,"\n\t%02d: Space\n",line_num);
				continue;
			}
			if (strncmp(filetail,filebuf,strlen(filetail))==0)
			{
				fclose(fpdef);
				Write_Data_File();

				printf ("\n%s END reacked, Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
				fprintf (ftxt,"\n%s END reacked, Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
				break;
			}
			if (strncmp(fileflag,&filebuf[n],strlen(fileflag)))
			{
				fprintf (ftxt,"\n\tThe calcu def file %s line %d miss INCLUDE, %s Aborted!\n",def_filename,line_num,filebuf);
				fprintf (ferr,"\n\tThe calcu def file %s line %d miss INCLUDE, %s Aborted!\n",def_filename,line_num,filebuf);
				continue;
			}
			n = n + strlen(fileflag);

			Dele_Space();
			i=0;
			while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<POINT_NAME_SIZE-1)
			{
				read_buf[i]=filebuf[n];
				i++;n++;
			}
			read_buf[i]=0;
			fprintf (ftxt,"\n\t%02d: %s",line_num,filebuf);
			if (i==0)
			{
				fprintf (ftxt,"\n\tThe calcu def file %s line %d No filename defined, %s Aborted!\n",def_filename,line_num,filebuf);
				fprintf (ferr,"\n\tThe calcu def file %s line %d No filename defined, %s Aborted!\n",def_filename,line_num,filebuf);
				continue;
			}

			sprintf(filename, "%s%s", SYS_DEFINE_PATH, read_buf);
			printf ("\n  :: Now Loading %s ...\n",filename);
			fprintf (ftxt,"\n  :: Now Loading %s ...\n",filename);
			fprintf (ferr,"\n  :: Now Loading %s ...\n",filename);
			if (Load_Calcu_File()==-1)
			{
				printf ("  :: Check file: %s for errors !!!\n",filename);
				continue;
			}
		}
		fclose(ferr);
		fclose(ftxt);

		if(f)
		{
			printf("%d ERROR in the file %s!\n",f,def_filename);
			printf("Check file : %s for more error messages\n",errfilebuf);
		}
		else
			printf("----------OK!-----------\n");
	}
}
