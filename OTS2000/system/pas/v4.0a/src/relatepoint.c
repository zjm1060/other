/* RelatePointname.c program
   cc -o RelatePointname.dxe RelatePointname.c /DBLIB/dblib.dlb
*/
//#define UNIX_OS

#define DEBUG

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
#endif

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../pas/v4.0a/inc/relatepoint.h"

static	int		n,r,line_num,f;

static	FILE		*fp,*fpdef,*fp1,*fpb,*ferr,*ftxt;

static	char		*filehead = "BEGIN:";
static	char		*pt_flag = "POINT:";
static	char		*fileflag = "INCLUDE";
static	char		*filetail = "END";
static	char		*childbgn = "{";
static	char		*childend = "}";

static	char		oper[DATA_NAME_LONG];
static	char		filebuf[LINE_BUF_CHAR];

static	char		def_filename[LINE_BUF_CHAR];
static	char		intlk_filename[LINE_BUF_CHAR];
static	char		gen_filename[LINE_BUF_CHAR];
static	char		gentxt_filename[LINE_BUF_CHAR];
static	char		bak_filename[LINE_BUF_CHAR];
static	char		err_filename[LINE_BUF_CHAR];

static	char		*gentxtfile="RelatePointName.txt";
static	char		*bakfile="RelatePoint.bak";
static	char		*errfile="RelatePoint.rec";

static	DMS_COMMON	dbptr;

static	short		pt_num;
static	short		et_num;

static	RelatePoint	relatept[RELATEPOINT_NUM];


static void Dele_Space()
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

		if (fgets(filebuf,LINE_BUF_CHAR,fp)==NULL)
		{
			if (feof (fp))
			{
				f++;
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

		if (filebuf[n]==LF||filebuf[n]==CR||filebuf[n]==0)
		{
			printf ("%3d:Space\n",r);
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

static void	Read_Entry()
{
	char	data_name[DATA_NAME_LONG];

	int	i,len,stint,ret;
	int	dat_type,dspflg;
	char	name[POINT_NAME_SIZE];

	i=0;

	while(filebuf[n]!=CR&&filebuf[n]!=LF&&filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
	{
		name[i]=filebuf[n];
		relatept[pt_num].childname[et_num][i]=filebuf[n];
		i++;n++;
	}
	if(i==0)
		return;
	name[i]=0;
	relatept[pt_num].childname[et_num][i]=0;
	

	Dele_Space();
	n++;
	Dele_Space();

	i=0;

	ret = ReadEntryByNameStr(name,&dbptr);
	if (ret==-1)
	{// 
		f++;
		fprintf (ferr,"Error!%3d line: %s not find in db!\n",r,name);
	}else
	{
		et_num++;
		relatept[pt_num].child_num=et_num;
	}
	dat_type = dbptr.point.data_type;

	if (ret==-1)
	{
		fprintf (ftxt,"Error!%3d:\t%s%-24s ",r,oper,name);
	}
	else
		fprintf (ftxt,"%3d:\t%s%-36s%-24s ",r,oper,dbptr.fixed.entryLongname,name);


	fprintf (ftxt,"\n");

	//group[gt_num].e_num++;
#ifdef DEBUG
//	fprintf(ftxt,"\t\t<group:%d total_et:%d g_offset:%d e_num:%d layer:%d/%d pt_gt:%d >\n",gt_num,et_num,
//				group[gt_num].offset,group[gt_num].e_num,group[gt_num].layer,entry[et_num].layer,relatept[pt_num].g_num);
#endif
	
}

void Write_Data_File()
{
	char	temp;
	char	verstr[VERLEN];

	struct	tm	tmpt;
	time_t		tim;

	short	num;

	if ((fp1=fopen(gen_filename,"rb"))==NULL)
	{
		printf ("No bak file generated ! can't open(rb) the old %s\n",gen_filename);
	}
	else
	{
		if ((fpb=fopen(bak_filename,"wb"))==NULL)
		{
			printf ("No bak file generated ! can't open(wb) the file %s!\n",bak_filename);
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

	tim=time(NULL);
	tmpt=*localtime(&tim);

	sprintf(verstr,"%s %04d/%02d/%02d %02d:%02d:%02d",RPVERSTR,tmpt.tm_year+1900,
		tmpt.tm_mon+1,tmpt.tm_mday,tmpt.tm_hour,tmpt.tm_min,tmpt.tm_sec);

	if ((fp1=fopen(gen_filename,"wb"))==NULL)
	{
		printf ("No date file generated ! can't open(wb) the file %s!\n",gen_filename);
		return;
	}
	fwrite(&verstr[0],VERLEN,1,fp1);

//	gt_num = gt_num_rec;
	pt_num++;
//	gt_num++;

	for( num=0; num < pt_num; num++ )
	{
		Swap2Byte(&relatept[num].child_num);
		//Swap2Byte(&relatept[num].offset);
	}

	num = pt_num;
	Swap2Byte(&num);
	fwrite(&num,sizeof(short),1,fp1);


	fwrite(&relatept[0],sizeof (RelatePoint),pt_num,fp1);

	fclose(fp1);
	return;
}

int	Load_RelatePoint_File()
{
	int	i,j,flg,check_pt_err;
	char	name_buf[256], *ptr;
	POINTER	point;
	short	pt_num_sav;
	short	gt_num_sav;
	short	et_num_sav;
	short	f_sav;
	int duplicated;


	r=n=0;
//	layer_num=0;

	pt_num_sav = pt_num;
// 	gt_num_sav = gt_num;
// 	et_num_sav = et_num;
	f_sav = f;

	if ((fp=fopen(intlk_filename,"rb"))==NULL)
	{
		printf ("\tCan't open the file %s\n",intlk_filename);
		return -1;
	}

	if (Fgets_Line()==-1)
	{
		printf ("\tThe file is empty!exit now!\n");
		fclose(fp);
		return -1;
	}
	if (strncmp(filehead,&filebuf[n],strlen(filehead)))
	{
		printf ("\tThe file miss BEGIN: the first line is %s Exit now!\n",filebuf);
		fclose(fp);
		return -1;
	}

	fprintf (ftxt,"%3d: %s",r,filebuf);
	while(pt_num<RELATEPOINT_NUM)
	{
		if (Fgets_Line()==-1)
		{
			fclose(fp);
			printf ("\n\tError! EOF without END! Total pt=%d  close file now!\n",pt_num-pt_num_sav);
			fprintf (ftxt,"\n\tError! EOF without END! Total pt=%d  close file now!\n",pt_num-pt_num_sav);
			fprintf (ferr,"\nError! EOF without END! Total pt=%d  close file now!\n",pt_num-pt_num_sav);
			if (f != f_sav)
			{
				printf ("  :: %d ERROR in the file %s!\n",f-f_sav,intlk_filename);
			}
			else
				printf ("  :: %s ----------OK!-----------\n",intlk_filename);
			return -1;
		}
		if (strncmp(pt_flag,filebuf,strlen(pt_flag))==0)
		{
		
			n=n+strlen(pt_flag);
			if (pt_num>=RELATEPOINT_NUM-1)
			{
				f++;
				fprintf (ferr,"Error!%3d line:The relatept point has reached the MAX %d exit now!\n",r,RELATEPOINT_NUM);
				break;
			}
			pt_num++;
			Dele_Space();
			i=0;
			while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<O_NAME_SIZE-1)
			{
				name_buf[i]=filebuf[n];
				i++;n++;
			}
			name_buf[i]=0;

			duplicated=0;
			for(j=0; j<pt_num; j++)
			{
				if(strcmp(name_buf, relatept[j].o_name) == 0)
				{
					f++;
					printf ("Error!%3d line:The point %s is duplicated with point %d\n",r,relatept[pt_num].o_name,j);
					fprintf (ferr,"Error!%3d line:The point %s is duplicated with point %d\n",r,relatept[pt_num].o_name,j);
					duplicated=1;
					break;
				}
			}
			if (duplicated)
			{
				pt_num--;
				continue;
			}else{
				strcpy(relatept[pt_num].o_name,name_buf);

			}





		//	if((relatept[pt_num].o_name[i-1]!='.') || (relatept[pt_num].o_name[i-2]!='.'))
			{
			//	strcpy(name_buf, relatept[pt_num].o_name);
				check_pt_err= 1;
			
						
				if(-1!=GetPtIdByNameStr(name_buf, &point))
				check_pt_err= 0;
			
				if(check_pt_err)
				{
					f++;
					fprintf (ferr,"Error!%3d line:No such point %s in DB!\n",r,name_buf);
				}
			}

			


			Dele_Space();
			i=0;


			flg=1;
			fprintf (ftxt,"%3d:\tPOINT:  %-24s\t\t<<No: %d >>\n",r,relatept[pt_num].o_name,pt_num);
			continue;
		}
		else
		{
			if (strncmp(filetail,filebuf,strlen(filetail))==0)
			{
				fclose(fp);
			

				printf ("\n\tEND reacked,Total pt=%d close file now!\n",pt_num-pt_num_sav);
				fprintf (ftxt,"\n\tEND reacked,Total et=%d close file now!\n",pt_num-pt_num_sav);
				if (f != f_sav)
				{
					printf ("  :: %d ERROR in the file %s!\n",f-f_sav,intlk_filename);
				}
				else
					printf ("  :: %s ----------OK!-----------\n",intlk_filename);
				return 0;
			}
		}

		Dele_Space();
		strcpy(oper," ");
		while(filebuf[n]=='}')
		{
			if(flg==1)
			{
				strcat(oper,"}");
				f++;
				fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
			}
		
			n ++;
			strcat(oper,"}");
			flg = 3;
#ifdef DEBUG
// 			fprintf(ftxt,"\t\t}bracket et_num:%d g_offset:%d g_num:%d lay=%d\n",et_num_rec,entry[et_num_rec].data.group.offset,
// 					entry[et_num_rec].data.group.g_num,layer_num);
// 			fprintf(ftxt,"\t}<group:%d g_offset:%d g_num:%d layer:%d/%d\n",gt_num,group[gt_num].offset,
// 					group[gt_num].e_num,group[gt_num].layer,entry[et_num_rec].layer);
#endif
		}
		Dele_Space();
		switch(filebuf[n])
		{

		case '{':
// 			if(flg==1)
// 			{
// 				strcat(oper,"{");
// 				f++;
// 				fprintf(ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
// 			}
		//	while(filebuf[n]=='{')
			{
			
				n ++;
				strcat(oper,"{");
			}
			et_num=0;
			flg=3;
			break;
	

		default :
			
			if(flg != 3)
			{
				f++;
				fprintf(ferr,"Error!%3d line:bad operator! %s\n",r,filebuf); 
				continue;
			}
		}
		Dele_Space();
		
		strcat(oper,"  ");
		Dele_Space();
		Read_Entry();
	}
	fclose(fp);

	printf ("\n\tTotal pt=%d  close file now!\n",pt_num-pt_num_sav);
	fprintf (ftxt,"\n\tTotal pt=%d  close file now!\n",pt_num-pt_num_sav);
	if (f != f_sav)
	{
		printf ("  :: %d ERROR in the file %s!\n",f-f_sav,intlk_filename);
	}
	else
		printf ("  :: %s ----------OK!-----------\n",intlk_filename);
	return 0;
	
}


void main(int argc, char *argv[])
{
	int	i;
	char	read_buf[LINE_BUF_CHAR];

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}
	if (argc!=2)
	{
/*		sprintf(def_filename, "%s%s", SYS_DEFINE_PATH, INTERLOCK_DEF_FILE); */
		sprintf(def_filename, "%s%s", SYS_DEFINE_PATH, RELATEPOINT_FILE_DEF);
		printf ("\nReading file: %s ...\n",def_filename);
	}
	else
	{
		printf ("\nReading file:%s\n",argv[1]);
		strcpy(def_filename,argv[1]);
	}

	sprintf(gen_filename, "%s%s", SYS_DATA_PATH,RELATEPOINT_DATA_FILE);
	sprintf(gentxt_filename, "%s%s", SYS_DEFINE_PATH, gentxtfile);
	sprintf(bak_filename, "%s%s", SYS_DATA_PATH, bakfile);
	sprintf(err_filename, "%s%s", SYS_DEFINE_PATH, errfile);

	pt_num = -1;
	
	f = 0;

	if ((fpdef=fopen(def_filename,"rb"))==NULL)
	{
		printf ("RelatePointname.c: can't open the relatept def file %s\n",def_filename);
		exit(-1);
	}

	line_num = 0;

	if (DefFgets_Line()==-1)
	{
		printf ("the file is empty! exit now!\n");
		fclose(fpdef);
		UnMountDB();
		exit(-1);
	}
	if (strncmp(filehead,&filebuf[n],strlen(filehead)))
	{
		printf ("The file %s miss BEGIN: the first line is %s Exit now!\n",def_filename,filebuf);
		fclose(fpdef);
		UnMountDB();
		exit(-1);
	}
	if ((ferr=fopen(err_filename,"w"))==NULL)
	{
		printf ("RelatePointname.c:can't open(w) the file %s!\n",err_filename);
		fclose(fpdef);
		UnMountDB();
		exit(-1);
	}
	if ((ftxt=fopen(gentxt_filename,"w"))==NULL)
	{
		printf ("RelatePointname.c:can't open(w) the file %s!\n",gentxt_filename);
		fclose(fpdef);
		fclose(ferr);
		UnMountDB();
		exit(-1);
	}
	fprintf (ftxt,":: Reading the relatept def file %s ...\n",def_filename);
	fprintf (ftxt,"\n\t%02d: %s",line_num,filebuf);

	for(;;)
	{
		if (DefFgets_Line()==-1)
		{
			fclose(fpdef);
			Write_Data_File();
			printf ("The relatept def file %s now EOF without END! Total pt=%d  exit now!\n",def_filename,pt_num);
			fprintf (ftxt,"\n\tThe relatept def file %s now EOF without END! Total pt=%d  exit now!\n",def_filename,pt_num);
			fprintf (ferr,"\n\tThe relatept def file %s now EOF without END! Total pt=%d  exit now!\n",def_filename,pt_num);
			break;
		}
		if (filebuf[0]=='#')
		{
			printf ("\n  :: The relatept def file %s is disabled ...\n",filebuf);
			fprintf (ftxt,"\n\t %02d: The relatept def file %s disabled line %s\n",line_num,def_filename,filebuf);
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

			printf ("\n%s END reacked, Total pt=%d  exit now!\n",def_filename,pt_num);
			fprintf (ftxt,"\n%s END reacked, Total pt=%d  exit now!\n",def_filename,pt_num);
			break;
		}
		if (strncmp(fileflag,&filebuf[n],strlen(fileflag)))
		{
			fprintf (ftxt,"\n\tThe relatept def file %s line %d miss INCLUDE, %s Aborted!\n",def_filename,line_num,filebuf);
			fprintf (ferr,"\n\tThe relatept def file %s line %d miss INCLUDE, %s Aborted!\n",def_filename,line_num,filebuf);
			continue;
		}
		n = n + strlen(fileflag);

		Dele_Space();
		i=0;
		while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<O_NAME_SIZE-1)
		{
			read_buf[i]=filebuf[n];
			i++;n++;
		}
		read_buf[i]=0;
		fprintf (ftxt,"\n\t%02d: %s",line_num,filebuf);
		if (i==0)
		{
			fprintf (ftxt,"\n\tThe relatept def file %s line %d No filename defined, %s Aborted!\n",def_filename,line_num,filebuf);
			fprintf (ferr,"\n\tThe relatept def file %s line %d No filename defined, %s Aborted!\n",def_filename,line_num,filebuf);
			continue;
		}

		sprintf(intlk_filename, "%s%s", SYS_DEFINE_PATH, read_buf);
		printf ("\n  :: Now Loading %s ...\n",intlk_filename);
		fprintf (ftxt,"\n  :: Now Loading %s ...\n",intlk_filename);
		fprintf (ferr,"\n  :: Now Loading %s ...\n",intlk_filename);
		if (Load_RelatePoint_File()==-1)
		{
			printf ("  :: Check file: %s for errors !!!\n",intlk_filename);
			continue;
		}
	}
	fclose(ferr);
	fclose(ftxt);

	if (f)
	{
		printf ("%d ERROR in the file %s!\n",f,def_filename);
		printf ("Check the file : %s for more error messages\n",err_filename);
	}
	else
		printf ("----------OK!-----------\n");

	UnMountDB();
}
