/* interlockname.c program
   cc -o interlockname.dxe interlockname.c /DBLIB/dblib.dlb
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
#include "../../../pas/v4.0a/inc/interlockgen.h"

static	int		n,r,line_num,f;

static	FILE		*fp,*fpdef,*fp1,*fpb,*ferr,*ftxt;

static	char		*filehead = "BEGIN:";
static	char		*pt_flag = "POINT:";
static	char		*fileflag = "INCLUDE";
static	char		*filetail = "END";
static	char		oper[DATA_NAME_LONG];
static	char		filebuf[LINE_BUF_CHAR];

static	char		def_filename[LINE_BUF_CHAR];
static	char		intlk_filename[LINE_BUF_CHAR];
static	char		gen_filename[LINE_BUF_CHAR];
static	char		gentxt_filename[LINE_BUF_CHAR];
static	char		bak_filename[LINE_BUF_CHAR];
static	char		err_filename[LINE_BUF_CHAR];

static	char		*gentxtfile="InterLockGenName.txt";
static	char		*bakfile="InterLockGen.bak";
static	char		*errfile="InterLockErr.rec";

static	DMS_COMMON	dbptr;

static	short		pt_num;
static	short		gt_num;
static	short		et_num;

static	short		gt_num_rec;
static	short		et_num_rec;
static	short		layer_num;
static	short		gt_buf[LAYER_NUM];
static	short		et_buf[LAYER_NUM];

static	InterLock	interlock[INTERLOCK_NUM];
static	I_Group		group[OR_NUM];
static	I_Entry		entry[AND_NUM];

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

	i=0;

	if (et_num>=AND_NUM)
	{
		f++;
		fprintf (ferr,"Error!%3d 行:点已超过最大数 Max=%d,前点被覆盖!\n",r,AND_NUM);
		et_num--;
		if (group[gt_num].e_num)
			group[gt_num].e_num--;
	}
	while(filebuf[n]!=' '&&filebuf[n]!=TAB&&filebuf[n]!=0&&i<POINT_NAME_SIZE-1)
	{
		entry[et_num].name[i]=filebuf[n];
		i++;n++;
	}
	if(i==0)
		return;
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
		if (filebuf[n]!='=')
			entry[et_num].opr=LESS;
		else
			entry[et_num].opr=LESS_E;
		break;

	case '>':
		n++;
		if (filebuf[n]!='=')
			entry[et_num].opr=LARGE;
		else
			entry[et_num].opr=LARGE_E;
		break;

	case '=':
		n++;
		if (filebuf[n]!='=')
		{
			f++;
			fprintf (ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
		}
		entry[et_num].opr=EQUAL;
		break;

	case '!':
		n++;
		if (filebuf[n]!='=')
		{
			f++;
			fprintf (ferr,"Error!%3d line:%s Miss'='\n",r,filebuf);
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
		fprintf (ferr,"Error!%3d line:Error operator! %s",r,filebuf);
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

	ret = ReadEntryByNameStr(entry[et_num].name,&dbptr);
	if (ret==-1)
	{
		f++;
		fprintf (ferr,"Error!%3d line: %s not find in db!\n",r,entry[et_num].name);
	}
	dat_type = dbptr.point.data_type;
	if((entry[et_num].opr==BIN_LG)||(entry[et_num].opr==CHANFAIL))
	{
/*
		if (dat_type!=SOE_TYPE && dat_type!=POL_TYPE && dat_type!=SYSMSG_TYPE && dat_type!=-1)
		{
			f++;
			fprintf (ferr,"Error!%3d line:The data_type of %s isn't a ON_OFF variable!\n",
			        r,entry[et_num].name);
		}
*/
		dspflg=0;
		len=0;
		if(data_name[len]!='~')
			len=0;
		else
			len=1;
		if (!strcmp(data_name,"CLOSE"))
		{
			entry[et_num].data.status=CLOSE;
		}
		else
		{
			if (!strcmp(data_name,"OPEN"))
			{
				entry[et_num].data.status=OPEN;
			}
			else
			{
/*
				entry[et_num].data.status=atoi(&data_name[len]);
*/
				if(sscanf(&data_name[len],"%d",&entry[et_num].data.status)==0)
				{
					f++;
					fprintf(ferr,"Error!%3d line: status=%s is not a short\n",r,data_name);
				}
				dspflg=1;
				if (entry[et_num].data.status<0 || entry[et_num].data.status>15)
				{
					entry[et_num].data.status=OPEN;
					f++;
					fprintf (ferr,"Error!%3d line:%s The status:%d %s is out of 0--15\n",r,filebuf,
						entry[et_num].data.status,data_name);
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
			fprintf (ferr,"Error!%3d line:The data_type of %s isn't a ANALOG variable!\n",
			        r,entry[et_num].name);
		}
		if (sscanf (data_name,"%f",&entry[et_num].data.value)==-1)
		{
			f++;
			fprintf (ferr,"Error!%3d line: value=%s\n",r,data_name);
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
				fprintf (ftxt,"<   ");
				break;
	case LESS_E:
				fprintf (ftxt,"<=  ");
				break;
	case LARGE:
				fprintf (ftxt,">   ");
				break;
	case LARGE_E:
				fprintf (ftxt,">=  ");
				break;
	case EQUAL:
				fprintf (ftxt,"==  ");
				break;
	case N_EQUAL:
				fprintf (ftxt,"!=  ");
				break;
	case CHANFAIL:
				fprintf(ftxt,"?C  ");
				break;
	default:
				fprintf (ftxt,"?   ");
	}
	if((entry[et_num].opr==BIN_LG)||(entry[et_num].opr==CHANFAIL))
	{
		stint=entry[et_num].data.status&0xff;
		if (dspflg==0)
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
		fprintf (ftxt,"%9.3f",entry[et_num].data.value);
	fprintf (ftxt,"\n");

	group[gt_num].e_num++;
#ifdef DEBUG
	fprintf(ftxt,"\t\t<group:%d total_et:%d g_offset:%d e_num:%d layer:%d/%d pt_gt:%d >\n",gt_num,et_num,
				group[gt_num].offset,group[gt_num].e_num,group[gt_num].layer,entry[et_num].layer,interlock[pt_num].g_num);
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

	sprintf(verstr,"%s %04d/%02d/%02d %02d:%02d:%02d",VERSTR,tmpt.tm_year+1900,
		tmpt.tm_mon+1,tmpt.tm_mday,tmpt.tm_hour,tmpt.tm_min,tmpt.tm_sec);

	if ((fp1=fopen(gen_filename,"wb"))==NULL)
	{
		printf ("No date file generated ! can't open(wb) the file %s!\n",gen_filename);
		return;
	}
	fwrite(&verstr[0],VERLEN,1,fp1);

	gt_num = gt_num_rec;
	pt_num++;
	gt_num++;

	for( num=0; num < pt_num; num++ )
	{
		Swap2Byte(&interlock[num].g_num);
		Swap2Byte(&interlock[num].offset);
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

	fwrite(&interlock[0],sizeof (InterLock),pt_num,fp1);
	fwrite(&group[0],sizeof (I_Group),gt_num,fp1);
	fwrite(&entry[0],sizeof (I_Entry),et_num,fp1);
	fclose(fp1);
	return;
}

int	Load_InterLock_File()
{
	int	i,flg,check_pt_err;
	char	name_buf[256], *ptr;
	POINTER	point;
	short	pt_num_sav;
	short	gt_num_sav;
	short	et_num_sav;
	short	f_sav;

/*
	pt_num=-1;
	gt_num=gt_num_rec=-1;
	et_num=et_num_rec=0;
	layer_num=0;

	for(i=0;i<INTERLOCK_NUM;i++)
	{
		interlock[i].g_num=0;
		for(l=0;l<OR_NUM;l++)
		{
			interlock[i].group[l].e_num=0;
		}
	}
*/
	r=n=0;
	layer_num=0;

	pt_num_sav = pt_num;
	gt_num_sav = gt_num;
	et_num_sav = et_num;
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
	while(pt_num<INTERLOCK_NUM)
	{
		if (Fgets_Line()==-1)
		{
			fclose(fp);
			printf ("\n\tError! EOF without END! Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
			fprintf (ftxt,"\n\tError! EOF without END! Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
			fprintf (ferr,"\nError! EOF without END! Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
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
			if(layer_num)
			{
				f++;
				fprintf(ferr,"Error!%3d line: POINT:%s The bracket num=%d in interlock point not full! \n",r-1,interlock[pt_num].o_name,layer_num);
				layer_num = 0;
			/*	break; */
			}
			n=n+strlen(pt_flag);
			if (pt_num>=INTERLOCK_NUM-1)
			{
				f++;
				fprintf (ferr,"Error!%3d line:The interlock point has reached the MAX %d exit now!\n",r,INTERLOCK_NUM);
				break;
			}
			pt_num++;
			Dele_Space();
			i=0;
			while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&filebuf[n]!=' '&&i<O_NAME_SIZE-1)
			{
				interlock[pt_num].o_name[i]=filebuf[n];
				i++;n++;
			}
			interlock[pt_num].o_name[i]=0;
			interlock[pt_num].g_num=0;

			if((interlock[pt_num].o_name[i-1]!='.') || (interlock[pt_num].o_name[i-2]!='.'))
			{
				strcpy(name_buf, interlock[pt_num].o_name);
				check_pt_err= 1;
				if(ptr=strrchr(name_buf, '.'))
				{
					*ptr= 0;
					if(ptr=strrchr(name_buf, '.'))
					{
						*ptr= 0;
						if(-1!=GetPtIdByNameStr(name_buf, &point))
						check_pt_err= 0;
						else	//cxs add start
						{
							if(strlen(name_buf)>4)
							{
								if(name_buf[strlen(name_buf)-1]=='.' 
									&& name_buf[strlen(name_buf)-2]=='.')
									check_pt_err=0;
							}
						}//cxs add end
					}
				}
				
			
			 
				
				if(check_pt_err)
				{
					f++;
					fprintf (ferr,"Error!%3d line:No such point %s in DB!\n",r,name_buf);
				}
			}
			for(i=0; i<pt_num; i++)
			{
				if(strcmp(interlock[pt_num].o_name, interlock[i].o_name) == 0)
				{
					f++;
					printf ("Error!%3d line:The point %s is duplicated with point %d\n",r,interlock[pt_num].o_name,i);
					fprintf (ferr,"Error!%3d line:The point %s is duplicated with point %d\n",r,interlock[pt_num].o_name,i);
					break;
				}
			}
			if (gt_num>=OR_NUM-1)
			{
				f++;
				fprintf (ferr,"Error!%3d line:The interlock group has reached the MAX %d overlapped!\n",r,OR_NUM);
			}
			else
			{
				gt_num_rec++;
				gt_num = gt_num_rec;
			}
			interlock[pt_num].offset=gt_num;

			group[gt_num].layer=layer_num;
			group[gt_num].e_num=0;
			group[gt_num].offset=et_num;

/* Following added for interlock display 2003.6.25 zy */
			Dele_Space();
			i=0;
			while(filebuf[n]!=LF&&filebuf[n]!=CR&&filebuf[n]!=0&&filebuf[n]!=TAB&&i<LONG_NAME_SIZE-1)
			{
				interlock[pt_num].long_name[i]=filebuf[n];
				i++;n++;
			}
			interlock[pt_num].long_name[i]=0;

			flg=1;
			fprintf (ftxt,"%3d:\tPOINT:  %-24s\t%s\t<<No: %d offset:%d>>\n",r,interlock[pt_num].o_name,interlock[pt_num].long_name,pt_num,interlock[pt_num].offset);
			continue;
		}
		else
		{
			if (strncmp(filetail,filebuf,strlen(filetail))==0)
			{
				fclose(fp);
			/*	Write_Data_File(); */

				printf ("\n\tEND reacked,Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
				fprintf (ftxt,"\n\tEND reacked,Total pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
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
			if (flg==1)
			{
				strcat(oper," = ");
			}
			else
			{
				strcat(oper," & ");
				f++;
				fprintf (ferr,"Error!%3d line:%s More than one '=' in a point!\n",r,filebuf);
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

				interlock[pt_num].g_num++;
				group[gt_num].e_num =0;
				group[gt_num].layer = layer_num;
				group[gt_num].offset = et_num;
				n ++;
				strcat(oper,"(");
			}
			flg=2;
			break;
		case '&':
			if (flg!=1)
				strcat(oper," & ");
			else
			{
				strcat(oper," = ");
				f++;
				fprintf (ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
			}
			flg=2;
			n++;
			break;
		case '+':
			if (flg!=1)
			{
				if (gt_num>=OR_NUM -1)
				{
					f++;
					fprintf (ferr,"Error!%3d line:The group has reached the Max=%d Overlapped!\n",r,OR_NUM);
				}
				else
				{
					gt_num_rec++;
					gt_num = gt_num_rec;
					interlock[pt_num].g_num++;
					group[gt_num].e_num = 0;
					group[gt_num].layer = layer_num;
					group[gt_num].offset = et_num;
				}
				strcat(oper," + ");
			}
			else
			{
				strcat(oper," = ");
				f++;
				fprintf (ferr,"Error!%3d line:%s No '=' in a point!\n",r,filebuf);
			}
			n++;
			flg=2;
			break;
		case 0  :
		case LF :
		case CR :
			if(flg == 3)
				fprintf(ftxt,"%3d:\t%s\n",r,oper);
			else
				fprintf(ftxt,"%3d:\n",r);
			continue;
		case ';':
			fprintf (ftxt,"%3d:Descript line: %s",r,filebuf);
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

			interlock[pt_num].g_num++;
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
	fclose(fp);
/*
	Write_Data_File();
*/
	printf ("\n\tTotal pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
	fprintf (ftxt,"\n\tTotal pt=%d gt=%d et=%d close file now!\n",pt_num-pt_num_sav,gt_num-gt_num_sav,et_num-et_num_sav);
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
		sprintf(def_filename, "%s%s", SYS_DEFINE_PATH, INTERLOCK_FILE_DEF);
		printf ("\nReading file: %s ...\n",def_filename);
	}
	else
	{
		printf ("\nReading file:%s\n",argv[1]);
		strcpy(def_filename,argv[1]);
	}

	sprintf(gen_filename, "%s%s", SYS_DATA_PATH, INTERLOCK_DATA_FILE);
	sprintf(gentxt_filename, "%s%s", SYS_DEFINE_PATH, gentxtfile);
	sprintf(bak_filename, "%s%s", SYS_DATA_PATH, bakfile);
	sprintf(err_filename, "%s%s", SYS_DEFINE_PATH, errfile);

	pt_num = -1;
	gt_num = gt_num_rec = -1;
	et_num = et_num_rec = 0;
	layer_num = 0;
	f = 0;

	if ((fpdef=fopen(def_filename,"rb"))==NULL)
	{
		printf ("interlockname.c: can't open the interlock def file %s\n",def_filename);
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
		printf ("interlockname.c:can't open(w) the file %s!\n",err_filename);
		fclose(fpdef);
		UnMountDB();
		exit(-1);
	}
	if ((ftxt=fopen(gentxt_filename,"w"))==NULL)
	{
		printf ("interlockname.c:can't open(w) the file %s!\n",gentxt_filename);
		fclose(fpdef);
		fclose(ferr);
		UnMountDB();
		exit(-1);
	}
	fprintf (ftxt,":: Reading the interlock def file %s ...\n",def_filename);
	fprintf (ftxt,"\n\t%02d: %s",line_num,filebuf);

	for(;;)
	{
		if (DefFgets_Line()==-1)
		{
			fclose(fpdef);
			Write_Data_File();
			printf ("The interlock def file %s now EOF without END! Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
			fprintf (ftxt,"\n\tThe interlock def file %s now EOF without END! Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
			fprintf (ferr,"\n\tThe interlock def file %s now EOF without END! Total pt=%d gt=%d et=%d exit now!\n",def_filename,pt_num,gt_num,et_num);
			break;
		}
		if (filebuf[0]=='#')
		{
			printf ("\n  :: The interlock def file %s is disabled ...\n",filebuf);
			fprintf (ftxt,"\n\t %02d: The interlock def file %s disabled line %s\n",line_num,def_filename,filebuf);
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
			fprintf (ftxt,"\n\tThe interlock def file %s line %d miss INCLUDE, %s Aborted!\n",def_filename,line_num,filebuf);
			fprintf (ferr,"\n\tThe interlock def file %s line %d miss INCLUDE, %s Aborted!\n",def_filename,line_num,filebuf);
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
			fprintf (ftxt,"\n\tThe interlock def file %s line %d No filename defined, %s Aborted!\n",def_filename,line_num,filebuf);
			fprintf (ferr,"\n\tThe interlock def file %s line %d No filename defined, %s Aborted!\n",def_filename,line_num,filebuf);
			continue;
		}

		sprintf(intlk_filename, "%s%s", SYS_DEFINE_PATH, read_buf);
		printf ("\n  :: Now Loading %s ...\n",intlk_filename);
		fprintf (ftxt,"\n  :: Now Loading %s ...\n",intlk_filename);
		fprintf (ferr,"\n  :: Now Loading %s ...\n",intlk_filename);
		if (Load_InterLock_File()==-1)
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
