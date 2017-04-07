

#include<stdio.h>
#include<malloc.h>

#include "../inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../pas/v4.0a/inc/relatepoint.h"


/*#define	DEBUG*/



unsigned short		relate_pt_num;
unsigned short		relate_gt_num;
unsigned short		relate_et_num;

//InterLock			*interlock;
RelatePoint			*relatepoint;
// I_Group				*group;
// I_Entry				*entry;


int	BracketProc(int numb, int start_gnum, int end_gnum);


void RelatePointFree(void)
{
	if(relatepoint)
	{
		free (relatepoint);
		relatepoint= 0;
	}

}

int	RelatePointFileLoad()
{
	char	verstr[VERLEN];
	char	err_str[256];
	FILE	*fp;
	char	filename[256];
	short	num;

	if(relatepoint)
	{
		Err_log("InterLockFile loaded already");
		return -1;
	}

	sprintf(filename, "%s%s", SYS_DATA_PATH, RELATEPOINT_DATA_FILE);
	if((fp=fopen(filename, "rb" ))==NULL)
	{
		sprintf(err_str, "RelatePoint: fail to open %s", filename);
		Err_log(err_str);
		return -1;
	}
	if(fread(verstr, VERLEN, 1, fp)!=1)
	{
		sprintf(err_str, "RelatePoint: fail to read %s", filename);
		Err_log(err_str);
		return -1;
	}
	if(strncmp(verstr, RPVERSTR, strlen(RPVERSTR))!=0)
	{
		sprintf(err_str, "RelatePoint: version error %s", verstr);
		Err_log(err_str);
		return -1;
	}

#ifdef DEBUG
	printf("RelatePoint: %s VERSION: %s!\n", filename, verstr);
#endif

	if (fread(&relate_pt_num, sizeof(short), 1, fp)!=1)
	{
		sprintf(err_str, "RelatePoint: fail to read %s", filename);
		Err_log(err_str);
		return -1;
	}
	
	Swap2Byte(&relate_pt_num);
	

	relatepoint=(RelatePoint *)calloc(relate_pt_num,sizeof (RelatePoint));
	if (relatepoint==NULL)
	{
		sprintf(err_str, "RelatePoint: relatepoint calloc error");
		Err_log(err_str);
		return -1;
	}
	if (fread(relatepoint,sizeof (RelatePoint),relate_pt_num,fp)!=(unsigned int)relate_pt_num)
	{
		sprintf(err_str, "RelatePoint: fail to read %s", filename);
		Err_log(err_str);
		InterLockFree();
		return -1;
	}

	

	fclose (fp);

	for( num=0; num < relate_pt_num; num++ )
	{
		Swap2Byte(&relatepoint[num].child_num);
	//	Swap2Byte(&relatepoint[num].offset);
	}

	return 0;
}

int	RelatePoint_Judge (char *pt)
{
	int		rets;
	int		numb;
	char	err_str[256];
	rets=-1;

	for(numb=0;numb<relate_pt_num;numb++)
	{
		if (strcmp(pt,relatepoint[numb].o_name)==0)
		{
#ifdef DEBUG
			printf ( "the point %s num= %d\n",pt,numb);
#endif
			break;
		}
	}
	if (numb>=relate_pt_num)
	{
		sprintf(err_str, "RelatePoint: fail to find %s", pt);
		Err_log(err_str);
		return -1;
	}
	rets=1;

//	start_gnum = relatepoint[numb].offset;
//	end_gnum=relatepoint[numb].offset+relatepoint[numb].g_num;

//	rets = BracketProc(numb, start_gnum, end_gnum);
#ifdef DEBUG
	if(rets == 1)
		printf ( "pt %s have relate point!\n",pt);
	else
		printf ( "pt %s not relate point!\n",pt);
#endif
	return(numb);
}
/*
int	BracketProc(int numb, int start_gnum, int end_gnum)
{
	int	s_gnum,e_gnum,e_end;
	int	j,k,nt_g_num,ret;
	float	value;
	int	flg,dat_type,stint,rets;

	short	layer_num;
#ifdef	DEBUG
	short	layer_num1;
#endif
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
			printf ("\tgroup %d num=%d meet! lay=%d\n",j,j-relatepoint[numb].offset,group[j].layer);
#endif
			return 1;
		}
	}
#ifdef DEBUG
	printf ("\tgroup %d num=%d not meet! lay=%d\n",j,j-relatepoint[numb].offset,group[j].layer);
#endif
	return 0;
}

*/
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
		printf ( "\nPlease Input the relatepoint point name:" );
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
	InterLockFree ();
}
*/




