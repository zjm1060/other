/* interlockfn.c program lib
   cc -c interlockfn.c
*/

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

#ifndef MSWIN32
static	char	*genfile="/home/ems/h9000/dat/InterLockGen.dat";
#else
static	char	*genfile="/users/ems/project/pas/v4.0a/DATA/InterLockGen.dat";
#endif

int	BracketProc(int numb, int start_gnum, int end_gnum);

static	FILE		*fp;

static	short		pt_num;
static	short		gt_num;
static	short		et_num;

static	InterLock	*interlock;
static	I_Group		*group;
static	I_Entry		*entry;

int	InterLockFileLoad()
{
	static	FILE	*fp;
	char	verstr[VERLEN];

	short	num;

	if ((fp=fopen(genfile,"rb" ))==NULL)
	{
		printf ( "interlockfn.c:can't open(wb) the file %s!\n",genfile);
		return -1;
	}
	if (fread(&verstr[0],VERLEN,1,fp)!=1)
	{
		printf ( "interlockfn.c:file %s read pt_num error\n",genfile);
		return -1;
	}
	if(strncmp(verstr,VERSTR,strlen(VERSTR))!=0)
	{
		printf ( "interlockfn.c:file %s VERSION %s error!\n",genfile,verstr);
		return -1;
	}
#ifdef DEBUG
	printf ( "interlockfn.c:file %s VERSION: %s!\n",genfile,verstr);
#endif
	if (fread(&pt_num,sizeof (short),1,fp)!=1)
	{
		printf ( "interlockfn.c:file InterLockGen.dat read error\n" );
		return -1;
	}
	if (fread(&gt_num,sizeof (short),1,fp)!=1)
	{
		printf ( "interlockfn.c:file InterLockGen.dat read error\n" );
		return -1;
	}
	if (fread(&et_num,sizeof (short),1,fp)!=1)
	{
		printf ( "interlockfn.c:file InterLockGen.dat read error\n" );
		return -1;
	}
	Swap2Byte(&pt_num);
	Swap2Byte(&gt_num);
	Swap2Byte(&et_num);

	interlock=(InterLock *)calloc(pt_num,sizeof (InterLock));
	if (interlock==NULL)
	{
		printf ( "interlockfn.c:interlock calloc(pt_num) error\n" );
		return -1;
	}
	if (fread(interlock,sizeof (InterLock),pt_num,fp)!=(unsigned int)pt_num)
	{
		printf ( "interlockfn.c:file InterLockGen.dat read error\n" );
		return -1;
	}

	group=(I_Group *)calloc(gt_num,sizeof (I_Group));
	if (group==NULL)
	{
		printf ( "interlockfn.c: group *calloc error\n" );
		return -1;
	}
	if (fread(group,sizeof (I_Group),gt_num,fp)!=(unsigned int)gt_num)
	{
		printf ( "interlockfn.c:file InterLockGen.dat read error\n" );
		return -1;
	}

	entry=(I_Entry *)calloc(et_num,sizeof (I_Entry));
	if (entry==NULL)
	{
		printf ( "interlockfn.c: entry *calloc error\n" );
		return -1;
	}
	if (fread(entry,sizeof (I_Entry),et_num,fp)!=(unsigned int)et_num)
	{
		printf ( "interlockfn.c:file InterLockGen.dat read error\n" );
		return -1;
	}

	fclose (fp);

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
		Swap2Byte(&entry[num].opr);
		Swap2Byte(&entry[num].layer);
		if(entry[num].opr == BRACKET_F)
		{
			Swap2Byte(&entry[num].data.group.g_num);
			Swap2Byte(&entry[num].data.group.offset);
		}
		else
			Swap4Byte(&entry[num].data.value);
	}

	return 0;
}

int	InterLock_Judge (char *pt)
{
	int	start_gnum,end_gnum;
	int	rets;
	int	numb;

	for(numb=0;numb<pt_num;numb++)
	{
		if (strcmp(pt,interlock[numb].o_name)==0)
		{
#ifdef DEBUG
			printf ( "the point %s num= %d\n",pt,numb);
#endif
			break;
		}
	}
	if (numb>=pt_num)
	{
		printf ( "interlockfn.c: the point %s not find in InterLockGen.dat\n",pt);
		return -1;
	}
	start_gnum = interlock[numb].offset;
	end_gnum=interlock[numb].offset+interlock[numb].g_num;

	rets = BracketProc(numb, start_gnum, end_gnum);
#ifdef DEBUG
	if(rets == 1)
		printf ( "pt %s meet!\n",pt);
	else
		printf ( "pt %s not meet!\n",pt);
#endif
	return(rets);
}

int	BracketProc(int numb, int start_gnum, int end_gnum)
{
	int	s_gnum,e_gnum,e_end;
	int	j,k,nt_g_num,ret;
	float	value;
	int	flg,dat_type,stint,rets;

	short	layer_num,layer_num1;
	unsigned short	stat;
	DMS_COMMON	dbptr;

	layer_num = group[start_gnum].layer;
#ifdef DEBUG
	printf (" === lay %d start_g num=%d end_g num=%d\n",layer_num,start_gnum,end_gnum);
#endif

	for (j=start_gnum;j<=end_gnum;j++ )
	{
		e_end=group[j].offset+group[j].e_num;
		if(group[j].layer != layer_num)
		{
#ifdef	DEBUG
			printf ( "   !!!group=%d g_num=%d g_lay=%d != %d\n",
				j,j-start_gnum,group[j].layer,layer_num);
#endif
			continue;
		}
#ifdef	DEBUG
		printf ( "  << group=%d offset e_num=%d to %d lay=%d\n",
				j,group[j].offset,e_end,group[j].layer);
#endif

		for(k=group[j].offset;k<e_end;k++)
		{
			if(entry[k].opr == BRACKET_F)
			{
				s_gnum = entry[k].data.group.offset;
				e_gnum = entry[k].data.group.offset + entry[k].data.group.g_num -1;
#ifdef	DEBUG
				nt_g_num = entry[k].data.group.offset;
				layer_num1 = group[nt_g_num].layer;
				printf ( "\t{Bracket et_num=%d groups %d to %d lay=%d\n",k,s_gnum,e_gnum,layer_num1);
#endif
				rets = BracketProc(numb, s_gnum, e_gnum);
				if(rets == MEET)
				{
					nt_g_num = entry[k].data.group.offset + entry[k].data.group.g_num -1;
#ifdef	DEBUG
					printf ( "\t}Bracket et_num=%d meet! groups %d to %d lay=%d new_g_num=%d\n",k,s_gnum,e_gnum,layer_num1,nt_g_num);
#endif
					k = group[nt_g_num].offset+group[nt_g_num].e_num -1;
					continue;
				}
				else
				{
#ifdef	DEBUG
					printf ( "\t}Bracket et_num=%d status not meet! groups %d to %d lay=%d\n",k,s_gnum,e_gnum,layer_num1);
#endif
					break;
				}

			}
#ifdef	DEBUG
			else
			{
				printf ( "\tgroup=%d entry=%d e_num=%d %s opr=%d lay=%d\n",j,k,
					k-group[j].offset,entry[k].name,entry[k].opr,entry[k].layer);
			}
#endif
			if(entry[k].layer != layer_num)
			{
#ifdef	DEBUG
				printf ( "!!! group=%d entry=%d e_num=%d %s opr=%d lay=%d != %d\n",
					j,k,k-group[j].offset,entry[k].name,entry[k].opr,entry[k].layer,layer_num);
#endif
				continue;
			}
			ret = ReadEntryByNameStr(entry[k].name,&dbptr);
			if (ret==-1)
			{
				printf ( "interlockfn.c: point=%d group=%d entry=%d e_num=%d %s not find in db!\n",
						numb,j,k,k-group[j].offset,entry[k].name);
				break;
			}
			dat_type = dbptr.point.data_type;
			if ((entry[k].opr==BIN_LG)||(entry[k].opr==CHANFAIL))
			{
				if (entry[k].opr==BIN_LG)
				{
					if (dat_type!=SOE_TYPE && dat_type!=POL_TYPE && dat_type!=SYSMSG_TYPE && dat_type!=-1)
					{
						printf ( "Err:The data_type of %s isn't a ON_OFF variable!\n",entry[k].name);
						break;
					}
					if(dbptr.var.ind.status.chan_fail)
					{
#ifdef	DEBUG
						printf ( "\tgroup=%d entry=%d %s lay=%d data quality fail in db!\n",
								j,k,entry[k].name,entry[k].layer);
#endif
						break;
					}
					stat=dbptr.var.ind.status.rt_state;
				}
				else
				{
					stat=dbptr.var.ind.status.chan_fail;
				}
				stint=entry[k].data.status&0xff;
				if ((entry[k].data.status&0xff00)==0)
				{
					if (stat!=stint)
					{
#ifdef	DEBUG
						printf ( "\tThe status is %d,is not %d;not meet! lay=%d\n",stat,stint,entry[k].layer);
#endif
						break;
					}
				}
				else
				{
					if (stat==stint)
					{
#ifdef	DEBUG
						printf ( "\tThe status is %d,is not ~%d;not meet! lay=%d\n",stat,stint,entry[k].layer);
#endif
						break;
					}
				}
			}
			else
			{
				if (dat_type!=ANA_TYPE && dat_type!=CALC_TYPE && dat_type!=PARAM_TYPE)
				{
					printf ( "Err:The data_type of %s isn't a ANALOG variable!\n",entry[k].name);
					break;
				}
				if(dbptr.var.anlg.value.status.chan_fail)
				{
#ifdef	DEBUG
					printf ( "\tgroup=%d entry=%d %s lay=%d data quality fail in db!\n",
							j,k,entry[k].name,entry[k].layer);
#endif
						break;
				}
				value = dbptr.var.anlg.fValue;
				flg=0;
				switch(entry[k].opr)
				{
				case LESS:
					if (value<entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not < %9.3f\n",value,entry[k].data.value);
#endif 
					break;
				case LESS_E:
					if (value<=entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not <= %9.3f\n",value,entry[k].data.value);
#endif
					break;
				case LARGE:
					if (value>entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not >%9.3f\n",value,entry[k].data.value);
#endif
					break;
				case LARGE_E:
					if (value>=entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not>=%9.3f\n",value,entry[k].data.value);
#endif
					break;
				case EQUAL:
					if (value==entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not==%9.3f\n",value,entry[k].data.value);
#endif
					break;
				case N_EQUAL:
					if (value!=entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
						printf ( "\tThe value %9.3f not !=%9.3f\n",value,entry[k].data.value);
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
			printf ("\tgroup %d num=%d meet! lay=%d\n",j,j-interlock[numb].offset,group[j].layer);
#endif
			return 1;
		}
	}
#ifdef DEBUG
	printf ("\tgroup %d num=%d not meet! lay=%d\n",j,j-interlock[numb].offset,group[j].layer);
#endif
	return 0;
}

void InterLockFileFree ()
{
	free (interlock);
	free (group);
	free (entry);
}
/*
main()
{
	char	name_input[POINT_NAME_SIZE];

	if (InterLockFileLoad()==-1)
	{
		printf ( "InterLockGen.dat error!\n" );
		exit(1);
	}
	MountDB();

	for(;;)
	{
		printf ( "\nPlease Input the interlock point name:" );
		if (gets(name_input)==NULL)
		{
			printf ( "Input null name,exit now!\n" );
			break;
		}

		if (strlen(name_input)==0)
		{
			printf ( "CR received,exit now!\n" );
			break;
		}
		printf ( "\t InterLock_Judge () return %d\n",InterLock_Judge (name_input));
	}

	UnMountDB();
	InterLockFileFree ();
}
*/

