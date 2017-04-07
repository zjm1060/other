/* trend_data_rec.c program 
   cc -o trend_data_rec.dxe sgzy.c /DBLIB/dblib.dlb /usr/ccs/lib/librt.a
*/


#define IFLAGS	(IPC_CREAT|IPC_EXCL)
#define ERR2	((CURVE_SHM_DATA *) -1 )
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>


#ifdef SUN_OS
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/time.h>
#endif


#include    "../../../dms/v4.0a/inc/dms_com.h"
#include    "../../../pas/v4.0a/inc/trend_data_rec.h"
#include    "../../../dps/v4.0a/inc/ipc_common.h"
#include    "../../../lan/v4.0a/inc/lan_common.h"

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
/*
	if ( shmctl ( shmid_curve,IPC_RMID,( struct shmid_ds*)0)<0)
		perror ( "shmctl1" );
*/
	SendTraceMsg(0,0, "trend_data_rec program exit normally!(Ctrl_c or kill -2 pid)\n" );
	exit (0);
}

void Getdat (int n)
{
	int	i,j /*,m,pt*/;

	pCurve->nTime=time(NULL);
	
	i= pCurve->nextpt;

// 	tim = time (NULL);
// 	tmpt =*localtime (&tim);
// 	pt = tmpt.tm_hour * 3600 + tmpt.tm_min * 60 + tmpt.tm_sec;

	for (j=0;j<data.curve_num;j++ )
	{
		if ( ReadEntryById (&data.buffer[j], &common_aa ) == -1 )
		{
			SendTraceMsg(0,0, "trend_data_rec:Point %d-%d-%d-%d not in db!!!\n",data.buffer[j].stn_id,
				data.buffer[j].dev_id,data.buffer[j].data_type,data.buffer[j].pt_id);
			pCurve->dat.fvalue[j][i]=(float)0.0;
			continue;
		}

		if((data.buffer[j].data_type==SOE_TYPE)||(data.buffer[j].data_type==POL_TYPE)||
			(data.buffer[j].data_type==OUTS_TYPE)||(data.buffer[j].data_type==SYSMSG_TYPE))
		{
//			common_aa.var.ind.status.state =(int)((i%20)/10);
			SendTraceMsg(0,0, "\t%d:%s=%d ", data.buffer[j].dev_id,common_aa.fixed.entryName,
				common_aa.var.ind.status.state);

			pCurve->dat.status[j][i] =common_aa.var.ind.status.state;

// 			if(pt != i)
// 			{
// 				if(pt > i)
// 				{
// 					for (m=i+1;m<=pt;m++ )
// 					{
// 						pCurve->dat.status[j][m] = common_aa.var.ind.status.state;
// 					}
// 				}
// 				else
// 				{
// 					if((i>(DATA_NUM-10)) && (pt < 10))
// 					{
// 						for (m=i+1;m<DATA_NUM;m++ )
// 						{
// 							pCurve->dat.status[j][m] = common_aa.var.ind.status.state;
// 						}
// 						for (m=0;m<=pt;m++ )
// 						{
// 							pCurve->dat.status[j][m] = common_aa.var.ind.status.state;
// 						}
// 					}
// 					else
// 					{
// 						pCurve->dat.status[j][pt] = common_aa.var.ind.status.state;
// 					}
// 				}
// 			}
		}
		else
		{
//			common_aa.var.anlg.fValue =((int)((i%20)/10))*100.0f;
			SendTraceMsg(0,0, "\t%d:%s=%f ", data.buffer[j].dev_id,common_aa.fixed.entryName,common_aa.var.anlg.fValue);

			pCurve->dat.fvalue[j][i] =common_aa.var.anlg.fValue;

// 			if(pt != i)
// 			{
// 				if(pt > i)
// 				{
// 					for (m=i+1;m<=pt;m++ )
// 					{
// 						pCurve->dat.fvalue[j][m] = common_aa.var.anlg.fValue;
// 					}
// 				}
// 				else
// 				{
// 					if((i>(DATA_NUM-10)) && (pt < 10))
// 					{
// 						for (m=i+1;m<DATA_NUM;m++ )
// 						{
// 							pCurve->dat.fvalue[j][m] = common_aa.var.anlg.fValue;
// 						}
// 						for (m=0;m<=pt;m++ )
// 						{
// 							pCurve->dat.fvalue[j][m] = common_aa.var.anlg.fValue;
// 						}
// 						pCurve->overlapflag=1;
// 					}
// 					else
// 					{
// 						pCurve->dat.fvalue[j][pt] = common_aa.var.anlg.fValue;
// 					}
// 				}
// 			}
 		}
	}

// 	if ( (pt+1)>=DATA_NUM)
// 	{
// 		pCurve->nextpt =0;
// 		pCurve->overlapflag=1;
// 	}
// 	else
// 		pCurve->nextpt = pt + 1;

	i++;
	if (pCurve->overlapflag == 0)
	{
		pCurve->overlapflag=i>DATA_NUM? 1:0;		//为1时，标识共享内存区数值都有效
	}

	pCurve->nextpt=i%(DATA_NUM);
}

void	ReLoadDefFile ()
{
	int 	m,k,i;
	FILE	*fp;
	char	curvefilename[256];
	
	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
	if (( fp= fopen ( curvefilename,"rb" ))==NULL)
		SendTraceMsg(0,0, "Fail to open %s\n",curvefilename );
	else
	{
		fread (&newdy,sizeof (CURVE_DEF),1, fp);
		fclose ( fp);

		Swap2Byte(&newdy.curve_num);
		for ( m =0; m<newdy.curve_num; m++ )
			Swap2Byte(&newdy.buffer[m].pt_id);

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
/*
		for (k=0;k<newdy.curve_num;k++ )
			data.buffer[k] =newdy.buffer[k];
		data.curve_num=newdy.curve_num;
*/
		data=newdy;
	}

	SendTraceMsg(0,0, "ReLoadDefFile: curve num= %d\n", data.curve_num );

	return;
}

int main (int argc, char *argv[])
{
	FILE	*fp;
	short	m;
	int		shm_flag;
	char	curvefilename[256];
	
	CreateLockFile(".trend_data_rec.lock");
	/*
	if(-1==InitSysLog(H9000_MODULE_TRENDDATAREC, "TrendDataRec"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	*/

	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
#ifdef  SUN_OS

	signal (SIGCHLD,SIG_IGN);
	signal (SIGUSR1,SIG_IGN);
	signal (SIGUSR2,SIG_IGN);
	signal (SIGINT,Quit);
	signal (SIGALRM,Wakup);
#endif

	if(-1==InitSysLog(H9000_MODULE_TRENDDATAREC, "TrendDataRec"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	if(-1==MountDB(NULL))
	{
		SendTraceMsg(0,0,"\nFail to MountDB!");
		exit(-1);
	}

	if (( fp= fopen ( curvefilename,"rb" ))==NULL)
	{
		SendTraceMsg(0,0, "Fail to open %s\n",curvefilename );
		
		memset(&data, 0, sizeof(data));

		fp= fopen(curvefilename, "wb");
		if(fp)
		{
			SendTraceMsg(0,0, "Success to create %s\n",curvefilename );
			fwrite(&data, 1, sizeof(data), fp);
			fclose(fp);
		}
		else
		{
			SendTraceMsg(0,0, "Fail to create %s\n",curvefilename );
			exit(0);
		}
	}
	else
	{
		fread (&data,sizeof (CURVE_DEF),1, fp);
		fclose ( fp);
	}

	Swap2Byte(&data.curve_num);
	for ( m =0; m<data.curve_num; m++ )
		Swap2Byte(&data.buffer[m].pt_id);

	if (data.curve_num>MAX_CURVE_NUM)
		data.curve_num=MAX_CURVE_NUM;

	shm_flag = 1;

	pCurve = (CURVE_SHM_DATA*) LinktoShareMem( SHM_TREND_KEY_CHAR );
	if( pCurve == (CURVE_SHM_DATA*)-1 )
	{
		Err_log(  "Fail to  Real curve shm");
		SendTraceMsg(0,0,"trend_data_rec: RealCurve::  LinktoShareMem Error !!!!\n");
		exit(0) ;
	}

//////////////////////////////////////////////

	if(shm_flag)
	{
		pCurve->nextpt = 0;
		pCurve->reloadflag = 0;
		pCurve->overlapflag = 0;
	}
#ifdef SUN_OS
	alarm (CYCLE);
#endif
	for (;;)
	{
		Getdat (0);

		if ( pCurve->reloadflag==1 )
		{
			pCurve->reloadflag=0;
			ReLoadDefFile ();
		}
		
#ifdef SUN_OS
	    pause (); 
#else
        MySleep(CYCLE *1000) ;
#endif
		
	}
}

