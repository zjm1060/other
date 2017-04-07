#include<stdio.h>
#include<malloc.h>

#include "../inc/dms_com.h"
#include "../../../dps/v4.0a/inc/dps_type_def.h"
#include "../../../pas/v4.0a/inc/interlockgen.h"


#define	DEBUG



unsigned short		interlock_pt_num;
unsigned short		interlock_gt_num;
unsigned short		interlock_et_num;

InterLock			*interlock;
I_Group				*group;
I_Entry				*entry;


void InterLockFileFree(void);


int	InterLockFileLoad(void)
{
	char	err_str[128];
	FILE	*fp;
	short	num;
	char	filename[256];

	if(interlock)
	{
		Err_log("InterLockFile loaded already");
		return -1;
	}

	sprintf(filename, "%s%s", DATABASE_OUT_PATH, INTERLOCK_DATA_FILE);
	if ((fp=fopen(filename,"rb" ))==NULL)
	{
		sprintf(err_str, "interlockfn.c:can't open(wb) the file %s!\n",filename);
		Err_log(err_str);
		return -1;
	}
	if (fread(&interlock_pt_num,sizeof (short),1,fp)!=1)
	{
		sprintf(err_str, "interlockfn.c:file InterLockGen.dat read error\n" );
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	if (fread(&interlock_gt_num,sizeof (short),1,fp)!=1)
	{
		sprintf(err_str, "interlockfn.c:file InterLockGen.dat read error\n" );
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	if (fread(&interlock_et_num,sizeof (short),1,fp)!=1)
	{
		sprintf(err_str, "interlockfn.c:file InterLockGen.dat read error\n" );
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	Swap2Byte(&interlock_pt_num);
	Swap2Byte(&interlock_gt_num);
	Swap2Byte(&interlock_et_num);

	interlock=(InterLock *)calloc(interlock_pt_num,sizeof (InterLock));
	if (interlock==NULL)
	{
		sprintf(err_str, "interlockfn.c:interlock calloc(interlock_pt_num) error\n" );
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	if (fread(interlock,sizeof (InterLock),interlock_pt_num,fp)!=interlock_pt_num)
	{
		sprintf(err_str, "interlockfn.c:file InterLockGen.dat read error\n" );
		Err_log(err_str);
		fclose(fp);
		InterLockFileFree();
		return -1;
	}

	group=(I_Group *)calloc(interlock_gt_num,sizeof (I_Group));
	if (group==NULL)
	{
		sprintf(err_str, "interlockfn.c: group *calloc error\n" );
		Err_log(err_str);
		fclose(fp);
		InterLockFileFree();
		return -1;
	}
	if (fread(group,sizeof (I_Group),interlock_gt_num,fp)!=interlock_gt_num)
	{
		sprintf(err_str, "interlockfn.c:file InterLockGen.dat read error\n" );
		Err_log(err_str);
		fclose(fp);
		InterLockFileFree();
		return -1;
	}

	entry=(I_Entry *)calloc(interlock_et_num,sizeof (I_Entry));
	if (entry==NULL)
	{
		sprintf(err_str, "interlockfn.c: entry *calloc error\n" );
		Err_log(err_str);
		fclose(fp);
		InterLockFileFree();
		return -1;
	}
	if (fread(entry,sizeof (I_Entry),interlock_et_num,fp)!=interlock_et_num)
	{
		sprintf(err_str, "interlockfn.c:file InterLockGen.dat read error\n" );
		Err_log(err_str);
		fclose(fp);
		InterLockFileFree();
		return -1;
	}

	fclose (fp);

	for( num=0; num < interlock_pt_num; num++ )
	{
		Swap2Byte(&interlock[num].g_num);
		Swap2Byte(&interlock[num].offset);
	}
	for( num=0; num < interlock_gt_num; num++ )
	{
		Swap2Byte(&group[num].e_num);
		Swap2Byte(&group[num].offset);
	}
	for( num=0; num < interlock_et_num; num++ )
	{
		Swap2Byte(&entry[num].opr);
		Swap4Byte(&entry[num].data.value);
	}

	return 0;
}





BOOL	InterLock_Judge(char *ptNameStr)
{
	int			numb;
	int			g_end,e_end;
	int			j,k;
	float		value;
	int			flg,dat_type;
	char		err_str[128];
	DMS_COMMON	dbptr;
	unsigned int	stat;

	if(!interlock)
	{
		Err_log("Need load interlock file first");
		return 0;
	}

	for(numb=0;numb<interlock_pt_num;numb++)
	{
		if (strcmp(ptNameStr,interlock[numb].o_name)==0)
		{
#ifdef DEBUG
			sprintf(err_str, "the point %s num= %d\n",ptNameStr,numb);
			Err_log(err_str);
#endif
			break;
		}
	}
	if (numb>=interlock_pt_num)
	{
		sprintf(err_str, "the point %s not find in InterLockGen.dat\n",ptNameStr);
		Err_log(err_str);
		return 1;
	}
	g_end=interlock[numb].offset+interlock[numb].g_num;

	for(j=interlock[numb].offset;j<=g_end;j++)
	{
		e_end=group[j].offset+group[j].e_num;

		for(k=group[j].offset;k<e_end;k++)
		{
#ifdef DEBUG
			sprintf (err_str, "group=%d entry=%d %s\n",j-interlock[numb].offset,k-group[j].offset,entry[k].name);
			Err_log(err_str);
#endif
			
			if (ReadEntryByNameStr(entry[k].name,&dbptr) ==-1)
			{
				sprintf (err_str, "group=%d entry=%d %s not find in db!\n",j-interlock[numb].offset,k-group[j].offset,entry[k].name);
				Err_log(err_str);
				break;
			}
			dat_type = dbptr.point.data_type;
			if (entry[k].opr==0)
			{
				if (dat_type!=SOE_TYPE &&dat_type!=POL_TYPE &&dat_type!=SYSMSG_TYPE)
				{
					sprintf (err_str, "Err:The data_type of %s isn't a ON_OFF variable!\n",entry[k].name);
					Err_log(err_str);
					break;
				}
				stat=dbptr.var.ind.status.state;
				if (stat!=entry[k].data.status)
				{
#ifdef DEBUG
					sprintf (err_str, "\tThe status is %d,is not %d;not meet!\n",stat,entry[k].data.status);
					Err_log(err_str);
#endif
					break;
				}
			}
			else
			{
				if (dat_type!=ANA_TYPE  &&dat_type!=CALC_TYPE)
				{
					sprintf (err_str, "Err:The data_type of %s isn't a ANALOG variable!\n",entry[k].name);
					Err_log(err_str);
					break;
				}
				if(dbptr.var.anlg.value.status.chan_fail)
				{
					sprintf (err_str, "Err:The point data quality fail!\n",entry[k].name);
					Err_log(err_str);
					break;
				}
				
				value= dbptr.var.anlg.fValue ;

				flg=0;
				switch(entry[k].opr)
				{
				case 1:
					if (value<entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
					{
						sprintf (err_str, "\tThe value %9.3f not < %9.3f\n",value,entry[k].data.value);
						Err_log(err_str);
					}
#endif 
					break;
				case 2:
					if (value<=entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
					{
						sprintf (err_str, "\tThe value %9.3f not <= %9.3f\n",value,entry[k].data.value);
						Err_log(err_str);
					}
#endif
					break;
				case 3:
					if (value>entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
					{
						sprintf (err_str, "\tThe value %9.3f not >%9.3f\n",value,entry[k].data.value);
						Err_log(err_str);
					}
#endif
					break;
				case 4:
					if (value>=entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
					{
						sprintf (err_str, "\tThe value %9.3f not>=%9.3f\n",value,entry[k].data.value);
						Err_log(err_str);
					}
#endif
					break;
				case 5:
					if (value==entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
					{
						sprintf (err_str, "\tThe value %9.3f not==%9.3f\n",value,entry[k].data.value);
						Err_log(err_str);
					}
#endif
					break;
				case 6:
					if (value!=entry[k].data.value)
						flg=1;
#ifdef DEBUG
					else
					{
						sprintf (err_str, "\tThe value %9.3f not !=%9.3f\n",value,entry[k].data.value);
						Err_log(err_str);
					}
#endif
					break;
				}   /*switch*/
				if (flg!=1)
					break;
			}  /*else*/
		}  /*for*/
		
		if (k>=e_end)
		{
#ifdef DEBUG
			sprintf (err_str, "%d group meet!\n",j-interlock[numb].offset);
			Err_log(err_str);
#endif
			return 1;
		}
	}
	
#ifdef DEBUG
	sprintf (err_str, "pt %s not meet!\n",ptNameStr);
	Err_log(err_str);
#endif
	return 0;
}



void InterLockFileFree(void)
{
	if(interlock)
	{
		free (interlock);
		interlock= 0;
	}
	if(group)
	{
		free (group);
		group= 0;
	}
	if(entry)
	{
		free (entry);
		entry= 0;
	}
}
