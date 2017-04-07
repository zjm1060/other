#include<stdio.h>
#include<malloc.h>

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../inc/interlockgen.h"


#define	DEBUG



static	int			n,r,f;

static	FILE		*fp,*fp1,*fpb,*ferr,*ftxt;

static	char		*filehead="BEGIN:";
static	char		*pt_flag="POINT:";
static	char		*filetail="END";
static	char		filename[LINE_BUF_CHAR];
static	char		oper[DATA_NAME_LONG];

static	char		filebuf[LINE_BUF_CHAR];

static	char		*deffile="/home/ems/h9000/def/InterLock.txt";
static	char		*gentxtfile="/home/ems/h9000/def/InterLockGenName.txt";
static	char		*bakfile="/home/ems/h9000/def/InterLockGen.bak";
static	char		*err_file="/home/ems/h9000/def/InterLockErr.rec";


static	DMS_COMMON	dbptr;
int			ret;

static	unsigned short		pt_num;
static	unsigned short		gt_num;
static	unsigned short		et_num;

static	InterLock	inter_lock[INTERLOCK_NUM];
static	I_Group		interlock_group[OR_NUM];
static	I_Entry		interlock_entry[AND_NUM];

static  char genfile[256];

void Dele_Space(void)
{
 	while(filebuf[n]==' '||filebuf[n]==TAB)
	{
		n++;
	}
}

int	Fgets_Line(void)
{
 	while(1)
	{
		r++;

		if (fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if (feof (fp))
			{
				f++;
				printf ("file %s exit without END\n",filename);
				return -1;
			}
			else
			{
			f++;
			fprintf (ferr,"Error!%3d line: is a null line.\n",r);
			continue;
			}
		}
		n=0;
/*
		Dele_Space();

		if (filebuf[n]==LF||filebuf[n]==0)
		{
			printf ("%3d:Space\n",r);
			continue;
		}
*/
		return n;
	}
}

void Read_Entry(void)
{
	char	data_name[DATA_NAME_LONG];

	int	i;
	int	dat_type,dspflg;

	i=0;

	if (et_num>=AND_NUM)
	{
		f++;
                fprintf (ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if (interlock_group[gt_num].e_num)
			interlock_group[gt_num].e_num--;
	}
	while(filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
	{
		interlock_entry[et_num].name[i]=filebuf[n];
		i++;n++;
	}
	interlock_entry[et_num].name[i]=0;

	Dele_Space();
	switch(filebuf[n])
	{
	case '?':
		interlock_entry[et_num].opr=0;
		break;
	case '<':
		n++;
		if (filebuf[n]!='=')
			interlock_entry[et_num].opr=1;
		else
			interlock_entry[et_num].opr=2;
		break;
	case '>':
		n++;
		if (filebuf[n]!='=')
			interlock_entry[et_num].opr=3;
		else
			interlock_entry[et_num].opr=4;
		break;
	case '=':
		n++;
		if (filebuf[n]!='=')
		{
			f++;
			fprintf (ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
		}
		interlock_entry[et_num].opr=5;
		break;
	case '!':
		n++;
		if (filebuf[n]!='=')
		{
			f++;
			fprintf (ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
		}
		interlock_entry[et_num].opr=6;
		break;
	default :
		n++;
		f++;
		fprintf (ferr,"Error!%3d line:Error operator! %s",r,filebuf);
		interlock_entry[et_num].opr=0;
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
/*
	ReadEntryByName(interlock_entry[et_num].name,&dat_type,&dbptr);
*/
	ret = ReadEntryByNameStr(interlock_entry[et_num].name, &dbptr);

	if (ret == -1)
	{
		f++;
		fprintf (ferr,"Error!%3d line: %s not find in db!\n",r,interlock_entry[et_num].name);
	}
	dat_type = dbptr.point.data_type;
	if (interlock_entry[et_num].opr==0)
	{
		if (dat_type!=SOE_TYPE &&dat_type!=POL_TYPE &&dat_type!=SYSMSG_TYPE &&dat_type!=-1)
		{
			f++;
			fprintf (ferr,"Error!%3d line:The data_type of %s isn't a ON_OFF variable!\n",
			        r,interlock_entry[et_num].name);
		}
		dspflg=0;
		if (!strcmp(data_name,"CLOSE"))
		{
			interlock_entry[et_num].data.status=CLOSE;
		}
		else
		{
			if (!strcmp(data_name,"OPEN"))
			{
				interlock_entry[et_num].data.status=OPEN;
			}
			else
			{
				interlock_entry[et_num].data.status=atoi(data_name);
				dspflg=1;
				if (interlock_entry[et_num].data.status<0 || interlock_entry[et_num].data.status>15)
				{
					interlock_entry[et_num].data.status=OPEN;
					f++;
					fprintf (ferr,"Error!%3d line:%s The status is out of 0--15\n",r,filebuf);
				}
			}
		}
	}
	else
	{
		if (dat_type!=ANA_TYPE &&dat_type!=-1&&dat_type!=CALC_TYPE)
		{
			f++;
			fprintf (ferr,"Error!%3d line:The data_type of %s isn't a ANALOG variable!\n",
			        r,interlock_entry[et_num].name);
		}
/*
		interlock_entry[et_num].data.value=(float)atof (data_name);
*/
		if (sscanf (data_name,"%f",&interlock_entry[et_num].data.value)==-1)
		{
			f++;
			fprintf (ferr,"Error!%3d line: value=%s\n",r,data_name);
		}
	}
#ifdef DEBUG
	printf ("%3d:\t%s%-24s ",r,oper,interlock_entry[et_num].name);

	switch(interlock_entry[et_num].opr)
	{
		case 1: printf ("<   ");
			break;
		case 2: printf ("<=  ");
			break;
		case 3: printf (">   ");
			break;
		case 4: printf (">=  ");
			break;
		case 5: printf ("==  ");
			break;
		case 6: printf ("!=  ");
			break;
		default:printf ("?   ");
	}
	if (interlock_entry[et_num].opr==0)
	{
		if (dspflg==0)
		{
			if (interlock_entry[et_num].data.status==CLOSE)
				printf ("CLOSE\t");
			else
				printf ("OPEN \t");
		}
		else
			printf ("%5d\t",interlock_entry[et_num].data.status);
	}
	else
		printf ("%9.3f",interlock_entry[et_num].data.value);

	printf ("\t\t<interlock_group:%d interlock_entry:%d gt:%d g_offset:%d et:%d>\n",inter_lock[pt_num].g_num,interlock_group[gt_num].e_num,gt_num,interlock_group[gt_num].offset,et_num);
#endif
	et_num++;
	interlock_group[gt_num].e_num++;
}

void Write_Data_File(void)
{
	char	temp;
	short	num;

	if ((fp1=fopen(genfile,"rb"))==NULL)
	{
		printf ("No bak file generated ! can't open(rb) the old %s\n",genfile);
	}
	else
	{
		if ((fpb=fopen(bakfile,"wb"))==NULL)
		{
			printf ("No bak file generated ! can't open(wb) the file %s!\n",bakfile);
		}
		else
		{
			temp=fgetc(fp1);
			while(!feof (fp1))
			{
				fputc(temp,fpb);
				temp=fgetc(fp1);
			}
			fclose(fpb);
		}
		fclose(fp1);
	}

	if ((fp1=fopen(genfile,"wb"))==NULL)
	{
		printf ("No date file generated ! can't open(wb) the file %s!\n",genfile);
		return;
	}
	pt_num++;
	gt_num++;

	for( num=0; num < pt_num; num++ )
	{
		Swap2Byte(&inter_lock[num].g_num);
		Swap2Byte(&inter_lock[num].offset);
	}
	for( num=0; num < gt_num; num++ )
	{
		Swap2Byte(&interlock_group[num].e_num);
		Swap2Byte(&interlock_group[num].offset);
	}
	for( num=0; num < et_num; num++ )
	{
		Swap2Byte(&interlock_entry[num].opr);
		Swap4Byte(&interlock_entry[num].data.value);
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

	fwrite(&inter_lock[0],sizeof (InterLock),pt_num,fp1);
	fwrite(&interlock_group[0],sizeof (I_Group),gt_num,fp1);
	fwrite(&interlock_entry[0],sizeof (I_Entry),et_num,fp1);
	fclose(fp1);
	return;
}

int	Load_InterLock_File()
{
	int	i,flg;

	pt_num=-1;
	gt_num=-1;
	et_num=0;
/*
	for(i=0;i<INTERLOCK_NUM;i++)
	{
		inter_lock[i].g_num=0;
		for(l=0;l<OR_NUM;l++)
		{
			inter_lock[i].interlock_group[l].e_num=0;
		}
	}
*/
	r=n=0;

	if ((fp=fopen(filename,"rb"))==NULL)
	{
		printf ("inter_lock.c:can't open the file %s\n",filename);
		return -1;
	}

	if (Fgets_Line()==-1)
	{
		printf ("the file is empty!exit now!\n");
		return -1;
	}
	if (strncmp(filehead,&filebuf[n],strlen(filehead)))
	{
		printf ("The file miss BEGIN: the first line is %s Exit now!\n",filebuf);
		return -1;
	}
	printf ("%3d:%s",r,filebuf);

	while(pt_num<INTERLOCK_NUM)
	{
		if (Fgets_Line()==-1)
		{
			printf ("The file now EOF without END!\n");
			return -1;
		}
		if (strncmp(pt_flag,filebuf,strlen(pt_flag))==0)
		{
			n=n+strlen(pt_flag);
			if (pt_num>=INTERLOCK_NUM-1)
			{
				f++;
				fprintf (ferr,"Error!%3d line:The inter_lock point has reached the MAX %d exit now!\n",r,INTERLOCK_NUM);
				break;
			}
			pt_num++;
			Dele_Space();
			i=0;
			printf("O_NAME_SIZE=%d\n",O_NAME_SIZE);
			while(filebuf[n]!=LF&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<O_NAME_SIZE-1)
			{
				inter_lock[pt_num].o_name[i]=filebuf[n];
				i++;n++;
			}
			inter_lock[pt_num].o_name[i]=0;
			inter_lock[pt_num].g_num=0;

			if (gt_num>=OR_NUM-1)
			{
				f++;
				fprintf (ferr,"Error!%3d line:The inter_lock interlock_group has reached the MAX %d overlapped!\n",r,OR_NUM);
			}
			else
			{
				gt_num++;
			}
			inter_lock[pt_num].offset=gt_num;

			interlock_group[gt_num].e_num=0;
			interlock_group[gt_num].offset=et_num;

/* Following added for inter_lock display 2003.6.25 zy */
			Dele_Space();
			i=0;
			while(filebuf[n]!=LF&&filebuf[n]!=0&&filebuf[n]!=TAB&&i<LONG_NAME_SIZE-1)
			{
				inter_lock[pt_num].long_name[i]=filebuf[n];
				i++;n++;
			}
			inter_lock[pt_num].long_name[i]=0;

			flg=1;
#ifdef DEBUG
			printf ("%3d:\tPOINT:  %-24s\t%s\t<<No: %d offset:%d>>\n",r,inter_lock[pt_num].o_name,inter_lock[pt_num].long_name,pt_num,inter_lock[pt_num].offset);
#endif
			continue;
		}
		else
		{
			if (strncmp(filetail,filebuf,strlen(filetail))==0)
			{
				fclose(fp);
				Write_Data_File();
				printf ("\nEND reacked,Total pt=%d gt=%d et=%d exit now!\n",pt_num,gt_num,et_num);
				return 0;
			}
		}

		Dele_Space();
		switch(filebuf[n])
		{
		case '=':
			if (flg==1)
			{
				strcpy(oper," = ");
				flg=2;
			}
			else
			{
				strcpy(oper," & ");
				f++;
				fprintf (ferr,"Error!%3d line:%s More than one '=' in a point!\n",r,filebuf);
			}
			break;
		case '&':
			if (flg==2)
				strcpy(oper," & ");
			else
			{
				strcpy(oper," = ");
				f++;
				fprintf (ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
			}
			break;
		case '+':
			if (flg==2)
			{
				if (gt_num>=OR_NUM -1)
				{
					f++;
					fprintf (ferr,"Error!%3d line:The interlock_group has reached the Max=%d Overlapped!\n",r,OR_NUM);
				}
		          	else
				{
					gt_num++;
		         		inter_lock[pt_num].g_num++;
					interlock_group[gt_num].e_num=0;
					interlock_group[gt_num].offset=et_num;
				}
				strcpy(oper," + ");
			}
			else
			{
				strcpy(oper," = ");
				f++;
				fprintf (ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
			}
			break;
		case 0 :
		case LF:
#ifdef DEBUG
			printf ("%3d:Space\n",r);
#endif
			continue;
		case ';':
#ifdef DEBUG
			printf ("%3d:Descript line: %s\n",r,filebuf);
#endif
			continue;
		default :
			f++;
			fprintf (ferr,"Error!%3d line:bad operator! %s\n",r,filebuf); 
			continue;
		}
		n++;
		Dele_Space();
		Read_Entry();
	}
	fclose(fp);

	Write_Data_File();
/*
	if ((fp1=fopen(genfile,"wb"))==NULL)
	{
		printf ("inter_lockgen.c:can't open(wb) the file %s!\n",genfile);
		return -1;
	}
	fwrite(&pt_num,sizeof (short),1,fp1);
	fwrite(&gt_num,sizeof (short),1,fp1);
	fwrite(&et_num,sizeof (short),1,fp1);
	fwrite(&inter_lock[0],sizeof (InterLock),pt_num+1,fp1);
	fwrite(&interlock_group[0],sizeof (I_Group),gt_num+1,fp1);
	fwrite(&interlock_entry[0],sizeof (I_Entry),et_num,fp1);
	fclose(fp1);
*/
	printf ("\nTotal pt=%d gt=%d et=%d exit now!\n",pt_num,gt_num,et_num);
	return 0;
}


int main(int argc,char *argv[])
{
	if(-1==MountDB(DATABASE_NAME))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	if (argc!=2)
	{
		printf ("Loading file:%s\n",deffile);
		strcpy(filename,deffile);
	}
	else
	{
		printf ("Loading file:%s\n",argv[1]);
		strcpy(filename,argv[1]);
	}

	if ((ferr=fopen(err_file,"w"))==NULL)
	{
		printf ("inter_lockgen.c:can't open(wb) the file %s!\n",err_file);
		exit( -1);
	}

	sprintf(genfile, "%s%s", DATABASE_OUT_PATH, INTERLOCK_DATA_FILE);

	if (Load_InterLock_File()==-1)
	{
		printf ("file:%s error! No file generated!\n",filename);
		fclose(ferr);
		exit(1);
	}
	fclose(ferr);

	if (f)
	{
		printf ("%d ERROR in the file %s!\n",f,filename);
		printf ("vi file : %s for more error messages\n",err_file);
	}
	else
		printf ("----------OK!-----------\n");

	UnMountDB();
}
