
#define IFLAGS	(IPC_CREAT|IPC_EXCL)
#define ERR2	((CURVE_SHM_DATA *) -1 )


#include    "../../../lan/v4.0a/inc/lan.h"
#include    "../../../pas/v4.0a/inc/trend_data_rec.h"


DMS_COMMON	common_aa;
CURVE_DEF	data,newdy,sgfile;
CURVE_SHM_DATA	*pCurve;
int		shmid_curve;

struct tm	tmpt;
time_t		tim;

void Fatal (char * s)
{
	perror ( s);
	exit (1 );
}

#ifdef  SUN_OS

void Wakup(int sn)
{
	alarm (CYCLE);
	signal (SIGALRM,Wakup);
}

#endif

void Quit (int sn)
{
	UnMountDB();
	printf ( "simurec program exit normally!(Ctrl_c or kill -2 pid)\n" );
	exit (0);
}

void Getdat (int n)
{
	DATE_TIME time_cur;
	int	i,j,m,pt;
	
	i= pCurve->nextpt;

	tim = time (NULL);
	tmpt =*localtime (&tim);
	pt = tmpt.tm_hour * 3600 + tmpt.tm_min * 60 + tmpt.tm_sec;
	//yyp debug
	pt = i;
	GetSysTime(&time_cur);
#ifdef DEBUG
	sprintf(log_str, "%05d %05d %2d  %5d\n ",i,pt,tmpt.tm_sec, time_cur.ms);
	err_write(log_str);

#endif

	for (j=1;j<data.curve_num;j++ )
	{
		if ( ReadEntryById (&data.buffer[j], &common_aa ) == -1 )
		{
			printf ( "simurec:j=%d Point %d-%d-%d-%d not in db!!!\n",j,data.buffer[j].stn_id,
				data.buffer[j].dev_id,data.buffer[j].data_type,data.buffer[j].pt_id);
			pCurve->dat.fvalue[j][i]=(float)0.0;
			continue;
		}
#ifdef DEBUG
		printf ( "\t%d:%s=%f ", data.buffer[j].dev_id,common_aa.fixed.entryName,common_aa.var.anlg.fValue);
#endif
		if((data.buffer[j].data_type==SOE_TYPE)||(data.buffer[j].data_type==POL_TYPE)||
			(data.buffer[j].data_type==OUTS_TYPE)||(data.buffer[j].data_type==SYSMSG_TYPE))
		{
			pCurve->dat.status[j][i] =common_aa.var.ind.status.state;
			if(pt != i)
			{
				if(pt > i)
				{
					for (m=i+1;m<=pt;m++ )
					{
						pCurve->dat.status[j][m] = common_aa.var.ind.status.state;
					}
				}
				else
				{
					if((i>(DATA_NUM-10)) && (pt < 10))
					{
						for (m=i+1;m<DATA_NUM;m++ )
						{
							pCurve->dat.status[j][m] = common_aa.var.ind.status.state;
						}
						for (m=0;m<=pt;m++ )
						{
							pCurve->dat.status[j][m] = common_aa.var.ind.status.state;
						}
					}
					else
					{
						pCurve->dat.status[j][pt] = common_aa.var.ind.status.state;
					}
				}
			}
		}
		else
		{
			pCurve->dat.fvalue[j][i] =common_aa.var.anlg.fValue;
			if(pt != i)
			{
				if(pt > i)
				{
					for (m=i+1;m<=pt;m++ )
					{
						pCurve->dat.fvalue[j][m] = common_aa.var.anlg.fValue;
					}
				}
				else
				{
					if((i>(DATA_NUM-10)) && (pt < 10))
					{
						for (m=i+1;m<DATA_NUM;m++ )
						{
							pCurve->dat.fvalue[j][m] = common_aa.var.anlg.fValue;
						}
						for (m=0;m<=pt;m++ )
						{
							pCurve->dat.fvalue[j][m] = common_aa.var.anlg.fValue;
						}
						pCurve->overlapflag=1;
					}
					else
					{
						pCurve->dat.fvalue[j][pt] = common_aa.var.anlg.fValue;
					}
				}
			}
		}
	}
	pCurve->nextpt = pt + 1;

	if ( pCurve->nextpt>=DATA_NUM)
	{
		pCurve->nextpt =0;
		pCurve->overlapflag=1;
	}
#ifdef DEBUG
	printf ( "\n" );
#endif
}

void	ReLoadDefFile ()
{
	int 	m,k,i;
	FILE	*fp;
	char	curvefilename[256];
	
	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
	if (( fp= fopen ( curvefilename,"rb" ))==NULL)
		printf ( "simurec:can't open the curvefilename =%s\n",curvefilename );
	else
	{
		fread (&newdy,sizeof (CURVE_DEF),1, fp);
		fclose ( fp);
#ifdef SUN_OS
		Swap2Byte(&newdy.curve_num);
		for ( m =0; m<newdy.curve_num; m++ )
			Swap2Byte(&newdy.buffer[m].pt_id);
#endif
		if ( newdy.curve_num>MAX_CURVE_NUM)
			newdy.curve_num=MAX_CURVE_NUM;
		if ( newdy.curve_num<data.curve_num)
		{
			for ( m =0; m<newdy.curve_num; m++ )
			{
				if ( newdy.buffer[m].stn_id!= data.buffer[m].stn_id ||
					newdy.buffer[m].dev_id!= data.buffer[m].dev_id ||
					newdy.buffer[m].data_type!= data.buffer[m].data_type ||
					newdy.buffer[m].pt_id!= data.buffer[m].pt_id)
				{
					for (k=m;k<newdy.curve_num;k++ )
					{
						for (i=0;i<DATA_NUM;i++ )
						{
							pCurve->dat.status[k][i] = pCurve->dat.status[k+1][i];
						}
					}
					break;
				}
			}
		}
		else
		{
			for ( m =0; m<data.curve_num; m++ )
			{
				if (newdy.buffer[m].stn_id!= data.buffer[m].stn_id ||
					newdy.buffer[m].dev_id!= data.buffer[m].dev_id ||
					newdy.buffer[m].data_type!= data.buffer[m].data_type ||
					newdy.buffer[m].pt_id!= data.buffer[m].pt_id)
				{
					for (k=newdy.curve_num-1;k>m;k--)
					{
						for (i=0;i<DATA_NUM;i++ )
						{
							pCurve->dat.status[k][i] = pCurve->dat.status[k-1][i];
						}
					}
					break;
				}
			}
			for (i=0;i<DATA_NUM;i++ )
			{
				pCurve->dat.fvalue[m][i]=(float)0.0;
			}
		}
		data=newdy;
	}
#ifdef DEBUG
	printf ( "simurec: ReLoadDefFile\n" );
#endif
	return;
}

void main (int argc, char *argv[])
{
	FILE	*fp;
	char	keyfile[128];
	int		shm_flag;
	char	curvefilename[256];
	int		ms;
	int		i=0;
	wsa_startup();
//	SetLogLevel(argc, argv);
	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
#ifdef  SUN_OS
	signal (SIGCHLD,SIG_IGN);
	signal (SIGUSR1,SIG_IGN);
	signal (SIGUSR2,SIG_IGN);
	signal (SIGINT,Quit);
	signal (SIGALRM,Wakup);
#endif

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	if (( fp= fopen ( curvefilename,"rb" ))==NULL)
	{
		printf ( "simurec:can't open the file curvefilename =%s\n",curvefilename );
		data.curve_num=0;
	}
	else
	{
		fread (&data,sizeof (CURVE_DEF),1, fp);
	}
	fclose ( fp);
#ifdef SUN_OS
	Swap2Byte(&data.curve_num);
	for ( m =0; m<data.curve_num; m++ )
		Swap2Byte(&data.buffer[m].pt_id);
#endif
	if (data.curve_num>MAX_CURVE_NUM)
		data.curve_num=MAX_CURVE_NUM;
	
	printf("\nsimurec is running, num=%d...\n",data.curve_num);
#ifdef DEBUG
	for(i=1;i<data.curve_num;i++)
	{
		printf("i=%d\t %d.%d.%d.%d\n", i, data.buffer[i].stn_id,
			data.buffer[i].dev_id,data.buffer[i].data_type,data.buffer[i].pt_id);
	}
#endif
	sprintf(keyfile, "%s%s", SYS_TMP_PATH, FILE_KEY);

	shm_flag = 1;

	pCurve = (CURVE_SHM_DATA*) LinktoShareMem( SHM_TREND_KEY_CHAR );
	if( pCurve == (CURVE_SHM_DATA*)-1 )
	{
		Err_log(  "Fail to  Real curve shm");
		printf("simurec: RealCurve::  LinktoShareMem Error !!!!\n");
		exit(0) ;
	}
	if(shm_flag)
	{
		pCurve->nextpt = 0;
		pCurve->reloadflag = 0;
		pCurve->overlapflag = 0;
		pCurve->isstop=2;
	}
#ifdef SUN_OS
	alarm (CYCLE);
#endif
	for (;;)
	{
		if(pCurve->isstop==1)
		{
			pCurve->nextpt = 0;
			MySleep(50);
			continue; 
		}
		else if(pCurve->isstop==2)
		{
			MySleep(50);
			continue;
		}
		Getdat (0);

		if ( pCurve->reloadflag==1 )
		{
			pCurve->reloadflag=0;
			ReLoadDefFile ();
			pCurve->nextpt = 0;
		}
#ifdef SUN_OS
	    pause (); 
#else
        //yyp debug
		MySleep(CYCLE *50) ;
		//MySleep(CYCLE *1);
		ms=GetTickCount();
		i++;
#endif
	}
}

