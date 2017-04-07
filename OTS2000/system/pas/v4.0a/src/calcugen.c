/* calcugen.c program
   cc -o calcugen.dxe calcugen.c /DBLIB/dblib.dlb
*/

#define DEBUG

#include<unistd.h>
#include "/DBINC/os_type_def.h"

#ifndef	 WINDOWS_OS
#include "/DBINC/dms_util_def.h"
#include "/DBINC/dms_functions.h"
#include "/DBINC/dms_com.h"
#include "/DBINC/dms_data_def.h"
#include "/DBINC/dms_structure_def.h"
#include "/PASINC/calcugen.h"
#else
#include "/users/ems/system/dms/v4.0a/inc/dms_util_def.h"
#include "/users/ems/system/dms/v4.0a/inc/dms_functions.h"
#include "/users/ems/system/dms/v4.0a/inc/dms_com.h"
#include "/users/ems/system/dms/v4.0a/inc/dms_data_def.h"
#include "/users/ems/system/dms/v4.0a/inc/dms_structure_def.h"
#include "/users/ems/system/pAs/v4.0a/inc/calcugen.h"
#endif

#define INTVAL		13

static	int		n,r,f;

static	FILE		*fp,*fp1,*fpb,*ferr;

static	char		*filehead=	"BEGIN:";
static	char		*pt_flag=	"POINT:";
static	char		*filetail=	"END";
static	char		*d_clear_flag=	"D_CLEAR";
static	char		*m_clear_flag=	"M_CLEAR";
static	char		*y_clear_flag=	"Y_CLEAR";

static	char		*tmp_float=	"TMP_FLOAT";
static	char		*tmp_status=	"TMP_STATUS";

static	char		filename[LINE_BUF_CHAR];
static	char		genfilebuf[LINE_BUF_CHAR];
static	char		bakfilebuf[LINE_BUF_CHAR];
static	char		errfilebuf[LINE_BUF_CHAR];
static	char		oper[DATA_NAME_LONG];

static	char		filebuf[LINE_BUF_CHAR];

#ifndef	 WINDOWS_OS
static  char            *deffile="/PASDATA/Calcu.txt";
static  char            *gentxtfile="/PASDATA/CalcuGenName.txt";
static  char            *genfile="/PASDATA/CalcuGen.dat";
static  char            *bakfile="/PASDATA/CalcuGen.bak";
static  char            *errfile="/PASDATA/CalcuErr.rec";
static  char            *deffile1="/PASDATA/Calcu1.txt";
static  char            *gentxtfile1="/PASDATA/Calcu1GenName.txt";
static  char            *genfile1="/PASDATA/Calcu1Gen.dat";
static  char            *bakfile1="/PASDATA/Calcu1Gen.bak";
static  char            *errfile1="/PASDATA/Calcu1Err.rec";
#else
static  char            *deffile="/users/ems/project/pas/v4.0a/DATA/Calcu.txt";
static  char            *gentxtfile="/users/ems/project/pas/v4.0a/DATA/CalcuGenName.txt";
static  char            *genfile="/users/ems/project/pas/v4.0a/DATA/CalcuGen.dat";
static  char            *bakfile="/users/ems/project/pas/v4.0a/DATA/CalcuGen.bak";
static  char            *errfile="/users/ems/project/pas/v4.0a/DATA/CalcuErr.rec";
static  char            *deffile1="/users/ems/project/pas/v4.0a/DATA/Calcu1.txt";
static  char            *gentxtfile1="/users/ems/project/pas/v4.0a/DATA/Calcu1GenName.txt";
static  char            *genfile1="/users/ems/project/pas/v4.0a/DATA/Calcu1Gen.dat";
static  char            *bakfile1="/users/ems/project/pas/v4.0a/DATA/Calcu1Gen.bak";
static  char            *errfile1="/users/ems/project/pas/v4.0a/DATA/Calcu1Err.rec";
#endif

static	char 		*dat_file_a = "Gen.dat";
static	char 		*bak_file_a = "Gen.bak";
static	char 		*err_file_a = "Err.rec";


static	short		pt_num;
static	short		gt_num;
static	short		et_num;

static	Calcu		calcu[CALCU_NUM];
static	C_Group		group[OR_NUM];
static	C_Entry		entry[AND_NUM];

static	void Dele_Space()
{
 while(filebuf[n]==' '||filebuf[n]==TAB)
	{
	n++;
	}
}

static	int	Fgets_Line()
{
 while(1)
	{
	r++;

	if(fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
		if(feof(fp))
			{
			f++;
			printf("\ncalcugen.c:: %s exit without END\n",filename);
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
	return n;
	}
}

static	void Read_Entry()
{
	char	data_name[DATA_NAME_LONG];

	int	i,len,stint;
	int	dat_type,dspflg;
	DMS_COMMON	dbptr;

	i=0;

	if(et_num>=AND_NUM)
		{
		f++;
                fprintf(ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if(group[gt_num].e_num)
			group[gt_num].e_num--;
		}
	while(filebuf[n]!=LF&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
		{
		entry[et_num].name[i]=filebuf[n];
		i++;n++;
		}
	entry[et_num].name[i]=0;

	Dele_Space();
	switch(filebuf[n])
		{
		case '?': entry[et_num].opr=BIN_LG;
		          break;
		case '<': n++;
		          if(filebuf[n]!='=')
		          	entry[et_num].opr=LESS;
		          else
		          	entry[et_num].opr=LESS_E;
		          break;
		case '>': n++;
		          if(filebuf[n]!='=')
		          	entry[et_num].opr=LARGE;
		          else
		          	entry[et_num].opr=LARGE_E;
		          break;
		case '=': n++;
		          if(filebuf[n]!='=')
				{
				f++;
		          	fprintf(ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
				}
		          entry[et_num].opr=EQUAL;
		          break;
		case '!': n++;
		          if(filebuf[n]!='=')
				{
				f++;
		          	fprintf(ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
				}
		          entry[et_num].opr=N_EQUAL;
		          break;
		case LF:
		case 0:   entry[et_num].opr=BIN_LG;
			  f++;
			  fprintf(ferr,"Error!%3d line:%s Miss operator\n",r,filebuf);
			  break;
		default : n++;
			  f++;
		          fprintf(ferr,"Error!%3d line:Error operator! %s\n",r,filebuf);
		          entry[et_num].opr=BIN_LG;
		}

	n++;
	Dele_Space();

	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<DATA_NAME_LONG-1)
		{
		data_name[i]=filebuf[n];
		i++;n++;
		}
	data_name[i]=0;

	if(strncmp(tmp_float,entry[et_num].name,strlen(tmp_float))==0)
		{
		len=strlen(tmp_float);
		dat_type=ANA_TYPE;
		if((entry[et_num].name[len]<'0')||(entry[et_num].name[len]>'9')||
			(entry[et_num].name[len+1]!=0))
			{
			f++;
			fprintf(ferr,"Error!%3d line: %s wrong TMP_FLOAT number(0-9)!\n",r,entry[et_num].name);
			}
		}
	 else if(strncmp(tmp_status,entry[et_num].name,strlen(tmp_status))==0)
		{
		len=strlen(tmp_status);
		dat_type=POL_TYPE;
		if((entry[et_num].name[len]<'0')||(entry[et_num].name[len]>'9')||
			(entry[et_num].name[len+1]!=0))
			{
			f++;
			fprintf(ferr,"Error!%3d line: %s wrong TMP_STATUS number(0-9)!\n",r,entry[et_num].name);
			}
		}
	else
		{
		if ( -1 == ReadEntryByNameStr ( entry[et_num].name, &dbptr) )
			{
			f++;
			fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
			}
		dat_type = dbptr.point.data_type;
		}
	if(entry[et_num].opr==0)
		{
		if(dat_type!=SOE_TYPE &&dat_type!=POL_TYPE &&dat_type!=SYSMSG_TYPE &&dat_type!=-1)
			{
			f++;
			fprintf(ferr,"Error!%3d line:The data_type of %s isn't a ON_OFF variable!\n",
			        r,entry[et_num].name);
			}
		dspflg=0;
		len=0;
		if(data_name[len]!='~')
			len=0;
		else	len=1;

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
					fprintf(ferr,"Error!%3d line:%s The status is %d out of 0--15\n",r,filebuf,entry[et_num].data.status);
					}
				}
			}
		if(len==1)
			entry[et_num].data.status=entry[et_num].data.status+0xff00;
		}
	else
		{
		if(dat_type!=ANA_TYPE &&dat_type!=-1&&dat_type!=CALC_TYPE)
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
#ifdef DEBUG
	printf("%3d:\t%s\t%-24s ",r,oper,entry[et_num].name);

	switch(entry[et_num].opr)
		{
		case LESS:	printf("<   ");
				break;
		case LESS_E:	printf("<=  ");
				break;
		case LARGE:	printf(">   ");
				break;
		case LARGE_E:	printf(">=  ");
				break;
		case EQUAL:	printf("==  ");
				break;
		case N_EQUAL:	printf("!=  ");
				break;
		default:	printf("?   ");
		}
	if(entry[et_num].opr==0)
		{
		stint=entry[et_num].data.status&0xff;
		if(dspflg==0)
			{
			if((entry[et_num].data.status&0xff00)==0)
				{
				if(stint==CLOSE)
					printf("CLOSE    ");
				else
					printf("OPEN     ");
				}
			else
				{
				if(stint==CLOSE)
					printf("~CLOSE   ");
				else
					printf("~OPEN    ");
				}
			}
		else
			if((entry[et_num].data.status&0xff00)==0)
				printf("%-5d    ",stint);
			else
				{
				printf("~%-5d   ",stint);
				}
		}
	else
		printf("%9.3f",entry[et_num].data.value);

	printf("%8<group:%d entry:%d gt:%d g_offset:%d et:%d>\n",calcu[pt_num].g_num,
			group[gt_num].e_num,gt_num,group[gt_num].offset,et_num);
#endif
	et_num++;
	group[gt_num].e_num++;
}

static void Read_Cal_Entry()
{
	int		i,dat_type,len;
	DMS_COMMON	dbptr;

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
#ifdef DEBUG
		printf("%3d:\t%s",r,oper);
		printf("%48<group:%d entry:%d gt:%d g_offset:%d et:%d>\n",calcu[pt_num].g_num,
				group[gt_num].e_num,gt_num,group[gt_num].offset,et_num);
#endif
		et_num++;
		group[gt_num].e_num++;
		return;
		}
	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
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
#ifdef DEBUG
		printf("%3d:\t%s\t%-9.3f\t\t",r,oper,entry[et_num].data.value);
#endif
		}
	else
		{
		if(strncmp(tmp_float,entry[et_num].name,strlen(tmp_float))==0)
			{
			len=strlen(tmp_float);
			dat_type=ANA_TYPE;
			if((entry[et_num].name[len]<'0')||(entry[et_num].name[len]>'9')||
				(entry[et_num].name[len+1]!=0))
				{
				f++;
				fprintf(ferr,"Error!%3d line: %s wrong TMP_FLOAT number(0-9)!\n",r,entry[et_num].name);
				}
			printf("%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
			}
	 	else if(strncmp(tmp_status,entry[et_num].name,strlen(tmp_status))==0)
			{
			len=strlen(tmp_status);
			dat_type=POL_TYPE;
			if((entry[et_num].name[len]<'0')||(entry[et_num].name[len]>'9')||
				(entry[et_num].name[len+1]!=0))
				{
				f++;
				fprintf(ferr,"Error!%3d line: %s wrong TMP_STATUS number(0-9)!\n",r,entry[et_num].name);
				}
			printf("%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
			}
		else
			{
			if ( -1 == ReadEntryByNameStr ( entry[et_num].name, &dbptr) )
				{
				f++;
				fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
				}
#ifdef DEBUG
			printf("%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
#endif
			}
		}
#ifdef DEBUG
	printf("%22<group:%d entry:%d gt:%d g_offset:%d et:%d>\n",calcu[pt_num].g_num,
			group[gt_num].e_num,gt_num,group[gt_num].offset,et_num);
#endif
	et_num++;
	group[gt_num].e_num++;
}

static void Read_Diff_Entry()
{
	int		i,dat_type;
	DMS_COMMON	dbptr;

	if(et_num>=AND_NUM)
		{
		f++;
		fprintf(ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if(group[gt_num].e_num)
			group[gt_num].e_num--;
		}
	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
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
			}
#ifdef DEBUG
		printf("%3d:\t%s\t%d\t\t\t",r,oper,entry[et_num].data.status);
#endif
		}
#ifdef DEBUG
	if(entry[et_num].opr==DIFF_S)
		{
		printf("%3d:\t%s\t%-24s",r,oper,entry[et_num].name);
		}
#endif
	if((entry[et_num].opr!=DIFF_C)&&(entry[et_num].opr!=DIFF_S))
		{
		if((entry[et_num].opr==3)||(entry[et_num].opr==4))
			{
			if ( -1 == ReadEntryByNameStr ( entry[et_num].name, &dbptr) )
				{
				f++;
				fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
				}
			}
#ifdef DEBUG
		printf("%3d:\t\t%-24s ",r,entry[et_num].name);
#endif
		}
#ifdef DEBUG
	printf("\t\t\t<group:%d entry:%d gt:%d g_offset:%d et:%d>\n",calcu[pt_num].g_num,
				group[gt_num].e_num,gt_num,group[gt_num].offset,et_num);
#endif
	et_num++;
	group[gt_num].e_num++;
}

static void Read_Clear_Entry()
{
	int		i,dat_type;
	DMS_COMMON	dbptr;

	if(et_num>=AND_NUM)
		{
		f++;
		fprintf(ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if(group[gt_num].e_num)
			group[gt_num].e_num--;
		}
	i=0;
	while(filebuf[n]!=LF&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
		{
		entry[et_num].name[i]=filebuf[n];
		i++;n++;
		}
	entry[et_num].name[i]=0;

	if ( -1 == ReadEntryByNameStr ( entry[et_num].name, &dbptr) )
		{
		f++;
		fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
		}
#ifdef DEBUG
	printf("%3d:\t\t%-24s",r,entry[et_num].name);
	printf("%22<group:%d entry:%d gt:%d g_offset:%d et:%d>\n",calcu[pt_num].g_num,
			group[gt_num].e_num,gt_num,group[gt_num].offset,et_num);
#endif
	et_num++;
	group[gt_num].e_num++;
}


void Write_Data_File()
{
	char	temp;
#ifdef	SUN_OS
	short	num;
#endif

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

	if((fp1=fopen(genfilebuf,"wb"))==NULL)
	{
		printf("No date file generated ! can't open(wb) the file %s!\n",genfilebuf);
		return;
	}
	pt_num++;
	gt_num++;
#ifdef	SUN_OS
	for( num=0; num < pt_num; num++ )
	{
		Swap2Byte(&calcu[num].type);
		Swap2Byte(&calcu[num].g_num);
		Swap2Byte(&calcu[num].offset);
	}
	for( num=0; num < gt_num; num++ )
	{
		Swap2Byte(&group[num].e_num);
		Swap2Byte(&group[num].offset);
	}
	for( num=0; num < et_num; num++ )
	{
		Swap2Byte(&entry[num].opr);
		Swap4Byte(&entry[num].data.value);
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
#else
	fwrite(&pt_num,sizeof(short),1,fp1);
	fwrite(&gt_num,sizeof(short),1,fp1);
	fwrite(&et_num,sizeof(short),1,fp1);
#endif
	fwrite(&calcu[0],sizeof(Calcu),pt_num,fp1);
	fwrite(&group[0],sizeof(C_Group),gt_num,fp1);
	fwrite(&entry[0],sizeof(C_Entry),et_num,fp1);
	fclose(fp1);
	return;
}

int	Load_Calcu_File()
{
	int		i,flg,len,dat_type;
	char		data_name[DATA_NAME_LONG];
	DMS_COMMON	dbptr;

	pt_num=-1;
	gt_num=-1;
	et_num=0;

	r=n=0;

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
	printf("%3d:%s",r,filebuf);

while(pt_num<CALCU_NUM)
	{
	if(Fgets_Line()==-1)
		{
		printf("The file now EOF without END!\n");
		return -1;
		}
	if(strncmp(pt_flag,filebuf,strlen(pt_flag))==0)
		{
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
		while(filebuf[n]!=LF&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<POINT_NAME_SIZE-1)
			{
			calcu[pt_num].o_name[i]=filebuf[n];
			i++;n++;
			}
		calcu[pt_num].o_name[i]=0;
		calcu[pt_num].g_num=0;
		calcu[pt_num].type= 0;  /*modified to 0 for D_CLEAR M_CLEAR and Y_CLEAR */

		if(strncmp(d_clear_flag,calcu[pt_num].o_name,strlen(d_clear_flag))==0)
			calcu[pt_num].type=D_CLEAR;
		else if(strncmp(m_clear_flag,calcu[pt_num].o_name,strlen(m_clear_flag))==0)
			calcu[pt_num].type=M_CLEAR;
		else if(strncmp(y_clear_flag,calcu[pt_num].o_name,strlen(y_clear_flag))==0)
			calcu[pt_num].type=Y_CLEAR;
		else if(strncmp(tmp_float,calcu[pt_num].o_name,strlen(tmp_float))==0)
			{
			len=strlen(tmp_float);
			if((calcu[pt_num].o_name[len]<'0')||(calcu[pt_num].o_name[len]>'9')||
				(calcu[pt_num].o_name[len+1]!=0))
				{
				f++;
				fprintf(ferr,"Error!%3d line: %s wrong TMP_FLOAT number(0-9)!\n",r,calcu[pt_num].o_name);
				}
			}
		 else if(strncmp(tmp_status,calcu[pt_num].o_name,strlen(tmp_status))==0)
			{
			len=strlen(tmp_status);
			if((calcu[pt_num].o_name[len]<'0')||(calcu[pt_num].o_name[len]>'9')||
				(calcu[pt_num].o_name[len+1]!=0))
				{
				f++;
				fprintf(ferr,"Error!%3d line: %s wrong TMP_STATUS number(0-9)!\n",r,calcu[pt_num].o_name);
				}
			}
		else
			{
			if ( -1 == ReadEntryByNameStr ( calcu[pt_num].o_name, &dbptr) )
				{
				f++;
				fprintf(ferr,"Error!%3d line: %s not find in db!\n",r,calcu[pt_num].o_name);
				}
			}
		Dele_Space();
		switch(filebuf[n])
			{
			case 'C': calcu[pt_num].type=CALCU;	/*for calculator type*/
				  break;
			case 'L': calcu[pt_num].type=LOGIC;	/*for logic type*/
				  break;
			case 'D': calcu[pt_num].type=DIFF;	/*for diff type*/
				  break;
			default:
				i=0;				/*for multi logic type*/
				while(filebuf[n]!=LF&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<DATA_NAME_LONG-1)
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
/*					if((calcu[pt_num].g_num<0)||(calcu[pt_num].g_num>23))
*/
					if(calcu[pt_num].g_num<0)
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
			gt_num++;
			}
		calcu[pt_num].offset=gt_num;

		group[gt_num].e_num=0;
		group[gt_num].offset=et_num;

		flg=1;
#ifdef DEBUG
		printf("%3d:\tPOINT:  %-24s ",r,calcu[pt_num].o_name);
		if(calcu[pt_num].type==LOGIC)
			printf("LOGIC  \t");
		else	if(calcu[pt_num].type==CALCU)
				printf("CALCU  \t");
		else	if(calcu[pt_num].type==DIFF)
				printf("DIFF   \t");
		else	if(calcu[pt_num].type==D_CLEAR)
				printf("D_CLEAR\t%d",calcu[pt_num].g_num);
		else	if(calcu[pt_num].type==M_CLEAR)
				printf("M_CLEAR\t%d",calcu[pt_num].g_num);
		else	if(calcu[pt_num].type==Y_CLEAR)
				printf("Y_CLEAR\t%d",calcu[pt_num].g_num);
			else
				printf("0x%4x \t",calcu[pt_num].type);
		printf("\t<<No: %d offset:%d>>\n",pt_num,calcu[pt_num].offset);
#endif
		continue;
		}
	else
		{
		if(strncmp(filetail,filebuf,strlen(filetail))==0)
			{
			fclose(fp);
			Write_Data_File();

			printf("\nEND reacked,Total pt=%d gt=%d et=%d exit now!\n",pt_num,gt_num,et_num);
			return 0;
			}
		}

	Dele_Space();
/*	if(calcu[pt_num].type==LOGIC)
*/
	if((calcu[pt_num].type!=DIFF)&&(calcu[pt_num].type!=CALCU)&&(calcu[pt_num].type!=D_CLEAR)&&
			(calcu[pt_num].type!=M_CLEAR)&&(calcu[pt_num].type!=Y_CLEAR))
	{
	switch(filebuf[n])
		{
		case '=': if(flg==1)
				{
		          	strcpy(oper," = ");
				flg=2;
				}
		          else
		          	{
		          	strcpy(oper," & ");
				f++;
		          	fprintf(ferr,"Error!%3d line:%s More than one '=' in a point!\n",r,filebuf);
		          	}
		          break;
		case '&': if(flg==2)
		          	strcpy(oper," & ");
		          else
		          	{
		          	strcpy(oper," = ");
				f++;
		          	fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
		          	}
		          break;
		case '+': if(flg==2)
				{
				if(gt_num>=OR_NUM -1)
					{
					f++;
					fprintf(ferr,"Error!%3d line:The group has reached the Max=%d Overlapped!\n",r,OR_NUM);
					}
		          	else
					{
					gt_num++;
		         		calcu[pt_num].g_num++;
					group[gt_num].e_num=0;

				group[gt_num].offset=et_num;
					}
				strcpy(oper," + ");
				}
			  else
				{
				strcpy(oper," = ");
				f++;
				fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
				}
		          break;
		case 0  :
		case LF:
#ifdef DEBUG
		          printf("%3d:Space\n",r);
#endif
		          continue;
		case ';':
#ifdef DEBUG
		          printf("%3d:Descript line: %s\n",r,filebuf);
#endif
		          continue;
		default : f++;
			  fprintf(ferr,"Error!%3d line:bad operator! %s\n",r,filebuf); 
		          continue;
		}
	n++;n++;
	Dele_Space();
	Read_Entry();
	}

	if(calcu[pt_num].type==CALCU)
		{
		switch(filebuf[n])
			{
			case 'I': n++;
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
			case '=': n++;
				  if(filebuf[n]!='C')
					{
					strcpy(oper," = ");
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
			case '+': n++;
				  if(filebuf[n]!='C')
					{
					strcpy(oper," + ");
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
			case '-': n++;
				  if(filebuf[n]!='C')
					{
					strcpy(oper," - ");
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
			case '*': n++;
				  if(filebuf[n]!='C')
					{
					strcpy(oper," * ");
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
			case '/': n++;
				  if(filebuf[n]!='C')
					{
					strcpy(oper," / ");
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
			case 'S': n++;
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
#ifdef DEBUG
				printf("%3d:Space\n",r);
#endif
				continue;
			case ';':
#ifdef DEBUG
				printf("%3d:Descript line: %s\n",r,filebuf);
#endif
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
			case '=': n++;
				 if(filebuf[n]!='C')
					{
					strcpy(oper," = ");
					entry[et_num].opr=DIFF_S;
					}
				  else
					{
					strcpy(oper," =C");
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
#ifdef DEBUG
				printf("%3d:Space\n",r);
#endif
				continue;
			case ';':
#ifdef DEBUG
				printf("%3d:Descript line: %s\n",r,filebuf);
#endif
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
		if((filebuf[n]==0)||(filebuf[n]==LF))
			{
#ifdef DEBUG
			printf("%3d:Space\n",r);
#endif
			continue;
			}
		if(filebuf[n]==';')
			{
#ifdef DEBUG
			printf("%3d:Descript line: %s\n",r,filebuf);
#endif
			continue;
			}

		Read_Clear_Entry();
		}
	}
	fclose(fp);

	Write_Data_File();

	printf("\nTotal pt=%d gt=%d et=%d exit now!\n",pt_num,gt_num,et_num);
	return 0;
}


void main(argc,argv)

	int	argc;
	char	*argv[];
{
	int x,y;
	char file_prf[LINE_BUF_CHAR];
	int	select;

	MountDB (DATABASE_NAME);

	if(argc!=2)
	{
	for(;;)
		{
		select=0;
		while( (select < 1) | (select > 3) )
			{
/*			printf("Loading file:%s\n",deffile);
*/
			printf("Please Select Following Function:\n");
			printf("\n 1) Calcu.txt\n");
			printf("\n 2) Calcu1.txt\n");
			printf("\n 3) Exit\n");
			printf("Please Select:( 1, 2, 3 ): ");
			
			scanf("%d", &select);
		
/*			printf("\n\nInvalid Input, Try again !!");
*/
			}

		if( select == 1 )
			{
			strcpy(filename,deffile);
			strcpy(errfilebuf,errfile);
			strcpy(genfilebuf,genfile);
			strcpy(bakfilebuf,bakfile);
			}
		else	if( select == 2 )
			{
			strcpy(filename,deffile1);
			strcpy(errfilebuf,errfile1);
			strcpy(genfilebuf,genfile1);
			strcpy(bakfilebuf,bakfile1);
			}
		else
			exit(1);
		if((ferr=fopen(errfilebuf,"w"))==NULL)
            {
             printf("calcugen.c:can't open(wb) the file %s!\n",errfilebuf);
             continue;
             }
		if(Load_Calcu_File()==-1)
			{
			printf("file:%s error! No file generated!\n",filename);
			fclose(ferr);
            continue;
			}
		fclose(ferr);

		if(f)
			{
			printf("%d ERROR in the file %s!\n",f,filename);
			printf("vi file : %s for more error messages\n",errfilebuf);
			}
		else
			printf("----------OK!-----------\n");

		}
	}
	else
	{
		printf("Loading file:%s\n",argv[1]);
		strcpy(filename,argv[1]);
		y = strlen(filename);
		for(x=y-1;x>0;x--)
			if(filename[x]=='.')
				break;
/*		if(x>0)
			strncpy(file_prf,filename,x);
*/
		if(x>0)
		{
			strncpy(file_prf,filename,x);
			file_prf[x] = 0;				/*	加结束码	*/
		}
		else
		strcpy(file_prf,filename);
		strcpy(errfilebuf,file_prf);
		strcat(errfilebuf,err_file_a);
		strcpy(genfilebuf,file_prf);
		strcat(genfilebuf,dat_file_a);
		strcpy(bakfilebuf,file_prf);
		strcat(bakfilebuf,bak_file_a);
	}

	if((ferr=fopen(errfilebuf,"wb"))==NULL)
                {
                printf("calcugen.c:can't open(wb) the file %s!\n",errfilebuf);
                exit( -1);
                }
	if(Load_Calcu_File()==-1)
		{
		printf("file:%s error! No file generated!\n",filename);
		fclose(ferr);
		exit(1);
		}
	fclose(ferr);

	if(f)
		{
		printf("%d ERROR in the file %s!\n",f,filename);
		printf("vi file : %s for more error messages\n",errfilebuf);
		}
	else
		printf("----------OK!-----------\n");

	UnMountDB();
}
