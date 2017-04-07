/* sgzy.c program 
   cc -o sgzy.dxe sgzy.c /DBLIB/dblib.dlb /usr/ccs/lib/librt.a
   use "sgtest.dxe dev_id data_type point_id" to set event */

/*#define DEBUG*/

#define WD_TRED



#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../inc/sgzy.h"
#include "../../../dps/v4.0a/inc/ipc_common.h"


#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "wsock32.lib")


#ifdef	WINDOWS_OS
#define		Msleep(ms)				Sleep(ms)
#else
#define		Msleep(ms)				usleep(ms*1000)
#endif


DMS_COMMON	common_aa;
SGZY_DY		data,newdy,sgfile;
ZYHD		zyhd;
SGZY_SHM_DATA	*pSgzy;

#ifdef WD_TRED
WDQS_DY		wddy,newwd;
WDHD		wdhd;
WDQS_SHM_DATA	*pWdqs;
int			shmid_wd;
int			wd_cyc;
short		wd_save_fg;
#endif

int		shmid_sg,shmid_zy;
struct tm	tmpt;
time_t		tim;

int			WTFILE;
char		sgzyfilename[256];
char		wdqsfilename[256];


void Fatal (char * s)
{
	perror ( s);
	exit (1 );
}

void Quit (int sn)
{
	UnMountDB();
	printf ( "sgzy program exit normally!(Ctrl_c or kill -2 pid)\n" );
	exit (0);
}

void Getdat (int n)
{
	int	i,j;
	char stn_longname[STATION_LONGNAME_SIZE];
	char dev_longname[GROUP_LONGNAME_SIZE];
	
	i= pSgzy->nextpt;

	tim = time (NULL);
	pSgzy->dCurTime=tim;

#ifdef DEBUG
	tmpt =*localtime (&tim);
	printf ( "%2d %2d ",i,tmpt.tm_sec);
#endif

	for (j=0;j<data.next_pos;j++ )
	{
		zyhd.pointer[j]=data.buffer[j];
		Swap2Byte(&zyhd.pointer[j].pt_id);

		if ( ReadEntryById (&data.buffer[j], &common_aa ) == -1 )
		{
			printf ( "sgzy:Point %d-%d-%d-%d not in db!!!\n",data.buffer[j].stn_id,
				data.buffer[j].dev_id,data.buffer[j].data_type,data.buffer[j].pt_id);
			if ( n==1 )
			{
				strcpy ( zyhd.longname[j],"Wrong pointer!" );
				zyhd.data_type[j]=data.buffer[j].data_type;	/*v4.0 modified */
				zyhd.hi_alm[j]=(float)0.0;	/*v3.0 modified */
				zyhd.lo_alm[j]=(float)0.0;	/*v3.0 modified */
			}
			pSgzy->sg_dat.fvalue[j][i]=(float)0.0;
			continue;
		}
#ifdef DEBUG
		printf ( "%s=%f ", common_aa.fixed.entryName,common_aa.var.anlg.fValue);
#endif
		if ( n==1 )
		{
			if (GetStnLongnameById(common_aa.point.stn_id, stn_longname) == -1
				|| GetGroupLongnameById(common_aa.point.stn_id,common_aa.point.dev_id,dev_longname) == -1)
			{
				strcpy ( zyhd.longname[j],common_aa.fixed.entryLongname );
			}
			else
			{
				strcpy(zyhd.longname[j], stn_longname);
				strcat(zyhd.longname[j], dev_longname);
				strcat(zyhd.longname[j], common_aa.fixed.entryLongname);
			}
#ifdef DEBUG
			printf("zyhd.longname[%d]=%s\n",j,zyhd.longname[j]);
#endif

			zyhd.data_type[j]=data.buffer[j].data_type;	/*v4.0 modified */
			zyhd.hi_alm[j] =common_aa.var.anlg.eng_para.HI_RANGE;	/*v3.0 modified from HI_ALLIM to HI_RANGE*/
			zyhd.lo_alm[j] =common_aa.var.anlg.eng_para.LO_RANGE;	/*v3.0 modified from LO_ALLIM to LO_RANGE*/
			Swap4Byte(&zyhd.hi_alm[j]);
			Swap4Byte(&zyhd.lo_alm[j]);
		}
		if((data.buffer[j].data_type==SOE_TYPE)||(data.buffer[j].data_type==POL_TYPE)||
			(data.buffer[j].data_type==OUTS_TYPE)||(data.buffer[j].data_type==SYSMSG_TYPE))
		{
//			common_aa.var.ind.status.state =(int)((i%20)/10);	//Test
			pSgzy->sg_dat.status[j][i] =common_aa.var.ind.status.state;
		}
		else
		{
//			common_aa.var.anlg.fValue =((int)((i%20)/10))*100.0f;	//Test
			pSgzy->sg_dat.fvalue[j][i] =common_aa.var.anlg.fValue;
		}
	}

	tmpt =*localtime (&tim);
	pSgzy->sghd.alarm.time.year = tmpt.tm_year+1900;
	pSgzy->sghd.alarm.time.month = tmpt.tm_mon+1;
	pSgzy->sghd.alarm.time.day = tmpt.tm_mday;
	pSgzy->sghd.alarm.time.hour = tmpt.tm_hour;
	pSgzy->sghd.alarm.time.minute = tmpt.tm_min;
	pSgzy->sghd.alarm.time.second = tmpt.tm_sec;
	pSgzy->sghd.alarm.time.ms=0;
	Swap2Byte(&pSgzy->sghd.alarm.time.year);

	pSgzy->nextpt++;
	if ( pSgzy->nextpt>=SGZY_NUM)
		pSgzy->nextpt =0;
#ifdef DEBUG
	printf ( "\n" );
#endif
/*   Temperature trend data */
#ifdef WD_TRED
	if (i%wd_cyc) return;

	pWdqs->dCurTime=tim;

	i= pWdqs->nextpt;
#ifdef DEBUG
	printf ( "\tTemp:%d ",i);
#endif
	for (j=0;j<wddy.next_pos;j++ )
	{
		wdhd.pointer[j]=wddy.buffer[j];
		Swap2Byte(&wdhd.pointer[i].pt_id);

		if ( ReadEntryById (&wddy.buffer[j] ,&common_aa) == -1 )
		{
			printf ( "sgzy:TRED Point %d-%d-%d-%d not in db!!!\n",wddy.buffer[j].stn_id,
					wddy.buffer[j].dev_id,wddy.buffer[j].data_type,wddy.buffer[j].pt_id);
			if ( pWdqs->save_flg==1 )
			{
				strcpy (wdhd.longname[j],"Wrong pointer!" );
				wdhd.data_type[j]=wddy.buffer[j].data_type;	/*v4.0 modified */
				wdhd.hi_alm[j]=(float)0.0;	/*v3.0 modified */
				wdhd.lo_alm[j]=(float)0.0;	/*v3.0 modified */
			}
			pWdqs->wd_dat.fvalue[j][i]=(float)0.0;
			continue;
		}

		if ( pWdqs->save_flg==1 )
		{
			if (GetStnLongnameById(common_aa.point.stn_id, stn_longname) == -1
				|| GetGroupLongnameById(common_aa.point.stn_id,common_aa.point.dev_id,dev_longname) == -1)
			{
				strcpy ( wdhd.longname[j],common_aa.fixed.entryLongname );
			}
			else
			{
				strcpy(wdhd.longname[j], stn_longname);
				strcat(wdhd.longname[j], dev_longname);
				strcat(wdhd.longname[j], common_aa.fixed.entryLongname );
			}
#ifdef DEBUG
			printf("wdhd.longname[%d]=%s\n",j,wdhd.longname[j]);
#endif

			wdhd.data_type[j]=wddy.buffer[j].data_type;	/*v4.0 modified */
			if((wddy.buffer[j].data_type==SOE_TYPE)||(wddy.buffer[j].data_type==POL_TYPE)||
				(wddy.buffer[j].data_type==OUTS_TYPE)||(wddy.buffer[j].data_type==SYSMSG_TYPE))
			{
				wdhd.hi_alm[j] = 15.0f;	/*v4.0 added*/
				wdhd.lo_alm[j] = 0.0f;	/*v4.0 added*/
			}
			else
			{
				wdhd.hi_alm[j] =common_aa.var.anlg.eng_para.HI_RANGE;	/*v3.0 modified from HI_ALLIM to HI_RANGE*/
				wdhd.lo_alm[j] =common_aa.var.anlg.eng_para.LO_RANGE;	/*v3.0 modified from LO_ALLIM to LO_RANGE*/
			}
			Swap4Byte(&wdhd.hi_alm[j]);
			Swap4Byte(&wdhd.lo_alm[j]);
		}
		if((wddy.buffer[j].data_type==SOE_TYPE)||(wddy.buffer[j].data_type==POL_TYPE)||
			(wddy.buffer[j].data_type==OUTS_TYPE)||(wddy.buffer[j].data_type==SYSMSG_TYPE))
		{
//			common_aa.var.ind.status.state =(int)((i%20)/10);	//Test
			pWdqs->wd_dat.status[j][i] =common_aa.var.ind.status.state;
#ifdef DEBUG
		printf ( "%s=%d ", common_aa.fixed.entryName, pWdqs->wd_dat.status[j][i]);
#endif
		}
		else
		{
//			common_aa.var.anlg.fValue =((int)((i%20)/10))*100.0f;	//Test
			pWdqs->wd_dat.fvalue[j][i] =common_aa.var.anlg.fValue;
#ifdef DEBUG
		printf ( "%s=%f ", common_aa.fixed.entryName, pWdqs->wd_dat.fvalue[j][i]);
#endif
		}
	}
	if ( pWdqs->save_flg==1 )
	{
//		tim = time (NULL);
		tmpt =*localtime (&tim);

		wdhd.time.year = tmpt.tm_year+1900;
		wdhd.time.month = tmpt.tm_mon+1;
		wdhd.time.day = tmpt.tm_mday;
		wdhd.time.hour = tmpt.tm_hour;
		wdhd.time.minute = tmpt.tm_min;
		wdhd.time.second = tmpt.tm_sec;
		wdhd.time.ms=0;
		Swap2Byte(&wdhd.time.year);
		wd_save_fg=1;
		pWdqs->save_flg=0;
	}

	pWdqs->nextpt++;
	if ( pWdqs->nextpt>=WDQS_NUM)
		pWdqs->nextpt =0;

#ifdef DEBUG
	printf ( "\n" );
#endif
#endif
}

void	Rzyfile ()
{
	int 	m,k,i;
	FILE	*fp;

	if (( fp= fopen ( sgzyfilename,"rb" ))==NULL)
		printf ( "sgzy:can't open the sgzyilename =%s\n",sgzyfilename );
	else
	{
		fread (&newdy,sizeof (SGZY_DY),1, fp);
		fclose ( fp);
		Swap2Byte(&newdy.next_pos);
		for ( m =0; m<newdy.next_pos; m++ )
			Swap2Byte(&newdy.buffer[m].pt_id);
		if ( newdy.next_pos>MAX_SGZY_COUNT)
			newdy.next_pos=MAX_SGZY_COUNT;
		if ( newdy.next_pos<data.next_pos)
		{
			for ( m =0; m<newdy.next_pos; m++ )
			{
				if ( newdy.buffer[m].stn_id!= data.buffer[m].stn_id||
					newdy.buffer[m].dev_id!= data.buffer[m].dev_id||
					newdy.buffer[m].data_type!= data.buffer[m].data_type||
					newdy.buffer[m].pt_id!= data.buffer[m].pt_id)
				{
					for (k=m;k<newdy.next_pos;k++ )
					{
						for (i=0;i<SGZY_NUM;i++ )
						{
							pSgzy->sg_dat.status[k][i] = pSgzy->sg_dat.status[k+1][i];
						}
					}
					break;
				}
			}
		}
		else
		{
			for ( m =0; m<data.next_pos; m++ )
			{
				if ( newdy.buffer[m].dev_id!= data.buffer[m].dev_id||
					newdy.buffer[m].data_type!= data.buffer[m].data_type||
					newdy.buffer[m].pt_id!= data.buffer[m].pt_id)
				{
					for (k=newdy.next_pos-1;k>m;k--)
					{
						for (i=0;i<SGZY_NUM;i++ )
						{
							pSgzy->sg_dat.status[k][i] = pSgzy->sg_dat.status[k-1][i];
						}
					}
					break;
				}
			}
			for (i=0;i<SGZY_NUM;i++ )
			{
				pSgzy->sg_dat.fvalue[m][i]=(float)0.0;
			}
		}
		for (k=0;k<newdy.next_pos;k++ )
			data.buffer[k] =newdy.buffer[k];
		data.next_pos=newdy.next_pos;
	}
	printf ( "sgzy: in Rzyfile \n" );
	return;
}

#ifdef	WD_TRED
void	Rwdfile ()
{
	int 	m,k,i;
	FILE	*fp;

	if (( fp= fopen (wdqsfilename,"rb" ))==NULL)
		printf ( "sgzy:can't open the file wdqsfilename =%s\n",wdqsfilename );
	else
	{
		fread (&newwd,sizeof (WDQS_DY),1, fp);
		fclose ( fp);
		Swap2Byte(&newwd.next_pos);
		for ( m =0; m<newwd.next_pos; m++ )
			Swap2Byte(&newwd.buffer[m].pt_id);
		if ( newwd.next_pos>MAX_WDQS_COUNT)
			newwd.next_pos=MAX_WDQS_COUNT;
		if ( newwd.next_pos<wddy.next_pos)
		{
			for ( m =0; m<newwd.next_pos; m++ )
			{
				if ( newwd.buffer[m].stn_id!= wddy.buffer[m].stn_id||
					newwd.buffer[m].dev_id!= wddy.buffer[m].dev_id||
					newwd.buffer[m].data_type!= wddy.buffer[m].data_type||
					newwd.buffer[m].pt_id!= wddy.buffer[m].pt_id)
				{
					for (k=m;k<=newwd.next_pos;k++ )
					{
						for (i=0;i<WDQS_NUM;i++ )
						{
							pWdqs->wd_dat.status[k][i] = pWdqs->wd_dat.status[k+1][i];
						}
					}
					break;
				}
			}
		}
		else
		{
			for (i=0;i<WDQS_NUM;i++ )
			{
				pWdqs->wd_dat.status[newwd.next_pos-1][i]=0;
			}
		}
		for (k=0;k<newwd.next_pos;k++ )
			wddy.buffer[k] =newwd.buffer[k];
		wddy.next_pos=newwd.next_pos;
	}
	return;
}
#endif

int main (int argc, char *argv[])
{
	short	m,len,pnt,noadd,year;
	char	filename[SG_FILENAME_LONG]= SGZY_LOG_PATH;
	char	wdfile[SG_FILENAME_LONG] =SGZY_LOG_PATH;
	char	sgfilename[SG_FILENAME_LONG] =SYS_DATA_PATH;
	char	ptnamestr[SG_FILENAME_LONG];
	FILE	*fp,*fp1,*fpdy;
	short	sgdyptnum[MAX_SGZY_COUNT];
	char	stn_longname[STATION_LONGNAME_SIZE];
	char	dev_longname[GROUP_LONGNAME_SIZE];
	char	entry_name[36];
	INT_ALRM	alm_sg;	/*added 2003.4.21 zy */
	short	record_num;
	int		record_value;
	DATE_TIME	dateTime;
	
	CreateLockFile(".sgzy.lock");
	if(-1==InitSysLog(H9000_MODULE_SGZY, "SGZY"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	


#ifdef SUN_OS
	signal (SIGCHLD,SIG_IGN);
	signal (SIGUSR1,SIG_IGN);
	signal (SIGUSR2,SIG_IGN);
	signal (SIGINT,Quit);
#endif

	WTFILE = strlen ( SGZY_LOG_PATH);

	if(-1==MountDB(NULL))
	{
		printf("\nFail to MountDB!");
		exit(-1);
	}

	sprintf(sgzyfilename, "%s%s", SYS_DATA_PATH, SGZY_DATA_FILE);
	if (( fp= fopen ( sgzyfilename,"rb" ))==NULL)
	{
		printf ( "sgzy:can't open the file sgzyfilename =%s\n",sgzyfilename );
		data.next_pos=0;
	}
	else
	{
		fread (&data,sizeof (SGZY_DY),1, fp);
		fclose ( fp);
	}
	//fclose ( fp);

	Swap2Byte(&data.next_pos);
	for ( m =0; m<data.next_pos; m++ )
		Swap2Byte(&data.buffer[m].pt_id);

	if (data.next_pos>MAX_SGZY_COUNT)
		data.next_pos=MAX_SGZY_COUNT;

	zyhd.zynum = data.next_pos;

#ifdef WD_TRED
	wd_cyc = WD_CYCLE;
	sprintf(wdqsfilename, "%s%s", SYS_DATA_PATH, WDQS_DATA_FILE);
	if (( fp= fopen (wdqsfilename,"rb" ))==NULL)
	{
		printf ( "sgzy:can't open the file wdqsfilename =%s\n",wdqsfilename );
		wddy.next_pos=0;
	}
	else
	{
		fread (&wddy,sizeof (WDQS_DY),1, fp);
		fclose ( fp);
	}
	//fclose ( fp);
	Swap2Byte(&wddy.next_pos);
	for ( m =0; m<wddy.next_pos; m++ )
		Swap2Byte(&wddy.buffer[m].pt_id);

	if (wddy.next_pos>MAX_WDQS_COUNT)
		wddy.next_pos=MAX_WDQS_COUNT;
	wdhd.zynum = wddy.next_pos;

#endif

	/*      打开事故追忆共享内存    */
	pSgzy= (SGZY_SHM_DATA*) LinktoShareMem( SHM_SGZY_KEY_CHAR );
	if( pSgzy == (SGZY_SHM_DATA*)-1 )
	{
		Err_log("Fail to link sgzy shm");
		return -1;
	}

#ifdef WD_TRED
	/*      打开温度趋势共享内存    */
	pWdqs= (WDQS_SHM_DATA*) LinktoShareMem( SHM_WDQS_KEY_CHAR );
	if( pWdqs == (WDQS_SHM_DATA*)-1 )
	{
		Err_log("Fail to link sgzy shm");
		return -1;
	}
#endif

	pSgzy->sghd.sg_flg=0;
	pSgzy->nextpt =0;
	pSgzy->flg=0;
#ifdef WD_TRED
	pWdqs->nextpt =0;
	pWdqs->flg=0;
	pWdqs->save_flg=0;
	wd_save_fg=0;
#endif

	for (;;)
	{
		Msleep(CYCLE*1000);

#ifdef WD_TRED
		if ( pSgzy->sghd.sg_flg==0&&wd_save_fg==0)
#else
		if ( pSgzy->sghd.sg_flg==0)
#endif
		{
			Getdat (0);

			if ( pSgzy->flg==1 )
			{
				pSgzy->flg=0;
				Rzyfile ();
				zyhd.zynum = data.next_pos;
			}
#ifdef WD_TRED
			if ( pWdqs->flg==1 )
			{
				pWdqs->flg=0;
				Rwdfile ();
				wdhd.zynum = wddy.next_pos;
			}
#endif
		}
		else
		{
#ifdef WD_TRED
			if (wd_save_fg==1 )
			{
				wd_save_fg=0;
				Getdat (0);
				year= wdhd.time.year;
				Swap2Byte(&year);

#ifdef ENGLISH_VER
				sprintf(&wdfile[WTFILE],"%02d-%s-%04d.%02d_%02d_%02d", wdhd.time.day,
					mthstr[wdhd.time.month-1], year,
					wdhd.time.hour, wdhd.time.minute, wdhd.time.second);
#else
				sprintf(&wdfile[WTFILE],"%04d-%02d-%02d.%02d_%02d_%02d", year,
					wdhd.time.month, wdhd.time.day,
					wdhd.time.hour, wdhd.time.minute, wdhd.time.second);
#endif
				strcat (wdfile,".WDQS" );
#ifdef DEBUG
				printf ( "Temperature trend data save to file:%s\n",wdfile );
#endif
				fp1= fopen (wdfile,"wb" );

				if (( fp1==NULL)||wddy.next_pos==0)
				{
						printf ( "sgzy:can't open (wb) the file %s, next_pos=%d !\n",wdfile, wddy.next_pos);
				}
				else
				{
					fwrite (&wdhd.time,sizeof (DATE_TIME),1, fp1 );
					
					record_num = wddy.next_pos;
					Swap2Byte(&record_num);

					fwrite (&record_num,sizeof ( short),1, fp1 );
					fwrite (wdhd.longname,POINT_LONGNAME_SIZE,wddy.next_pos, fp1 );
					fwrite (wdhd.data_type,sizeof (CHAR),wddy.next_pos, fp1 );	/*v4.0 added */
					fwrite (wdhd.hi_alm,sizeof (float),wddy.next_pos, fp1 );	/*v3.0 ANLG_DATA -> float */
					fwrite (wdhd.lo_alm,sizeof (float),wddy.next_pos, fp1 );	/*v3.0 ANLG_DATA -> float */
					fwrite (wdhd.pointer,sizeof(POINTER),wddy.next_pos, fp1);
					for ( m =0; m<wddy.next_pos; m++ )
					{
						for ( record_num = pWdqs->nextpt; record_num < WDQS_NUM; record_num++ )
						{
							record_value = pWdqs->wd_dat.status[m][record_num];
							Swap4Byte(&record_value);
							fwrite (&record_value, sizeof(float), 1, fp1 );
						}
						for ( record_num = 0; record_num < pWdqs->nextpt; record_num++ )
						{
							record_value = pWdqs->wd_dat.status[m][record_num];
							Swap4Byte(&record_value);
							fwrite (&record_value, sizeof(float), 1, fp1 );
						}
					}
					fclose ( fp1 );
				}
				continue;
			}
#endif

			Getdat (1 );

			alm_sg = pSgzy->sghd.alarm;
			if ( ReadEntryById (&alm_sg.ptr, &common_aa) == -1 )
			{
				printf ( "\nsgzy:No this sg point in the database file !!!" );
				strcpy ( zyhd.sgname,"NO_DATABASE_NAME" );
				strcpy ( entry_name,"NO_POINT_NAME" );
				pSgzy->sghd.alarm.ptr.data_type = 0;
				sprintf(sgfilename, "%s%s", SYS_DATA_PATH, SGZY_DATA_FILE);
			}
			else
			{
				/* strcpy ( zyhd.sgname,common_aa.entry.ind.fixed.entryLongname ); by zm 040807 */
				if(GetStnLongnameById(common_aa.point.stn_id, stn_longname) == -1
					|| GetGroupLongnameById(common_aa.point.stn_id,common_aa.point.dev_id,dev_longname)==-1)
				{
					strcpy ( zyhd.sgname, common_aa.fixed.entryLongname);
				}
				else
				{
					strcpy(zyhd.sgname, stn_longname);
					strcat(zyhd.sgname, dev_longname);
					strcat(zyhd.sgname, common_aa.fixed.entryLongname);
				}
				
				strcpy ( entry_name,common_aa.fixed.entryName );
				strcpy ( sgfilename,SYS_DATA_PATH);
				if(GetPtNameById(&common_aa.point,ptnamestr,NULL)==-1)
				{
					strcat ( sgfilename,ptnamestr );
				}
				strcat ( sgfilename,ptnamestr );
				strcat ( sgfilename,".FAIL" );
			}

			strcpy ( filename, SGZY_LOG_PATH);

#ifdef FAIL_REC_ENTRY_NAME
			strcat ( filename,entry_name );
#else
			strcat ( filename,zyhd.sgname );
#endif

			len= strlen ( filename );
/*
 Replace the character less than '!' or '\*?/:' or great than '~' in the longname
   with character '_'.
*/
			for ( m =WTFILE; m<len; m++ )
			{
				if (( filename[m]>=0&&filename[m]<='!')||filename[m] ==0x5c||
					filename[m] ==0x2a||filename[m] ==0x3f||
					filename[m] ==0x2f||filename[m] ==0x3a||filename[m]>=0x7e )
				{
					filename[m] ='_';
				}
			}
			filename[len] ='.';

//			GetSysTime(&alm_sg.time);

			year = alm_sg.time.year;
#ifdef ENGLISH_VER
			sprintf(&filename[len+1],"%02d-%s-%04d.%02d_%02d_%02d",alm_sg.time.day,
				mthstr[alm_sg.time.month - 1], year,
				alm_sg.time.hour, alm_sg.time.minute, alm_sg.time.second);
#else
			sprintf(&filename[len+1],"%04d-%02d-%02d.%02d_%02d_%02d", year,
				alm_sg.time.month, alm_sg.time.day,
				alm_sg.time.hour, alm_sg.time.minute, alm_sg.time.second);
#endif

			strcat (filename,".SGZY" );
			if (( fp1= fopen ( filename,"wb" ))==NULL)
			{
				printf ( "sgzy:can't open (wb) the file %s\n", filename );
			}
			else
			{
				printf ( "sgzy: receive a failure event signal! start event recall file generating\n" );
				Swap2Byte(&alm_sg.ptr.pt_id);
				Swap2Byte(&alm_sg.status);
				Swap2Byte(&alm_sg.time.ms);
				Swap2Byte(&alm_sg.time.year);
				if (( alm_sg.ptr.data_type == 0)||(( fpdy = fopen ( sgfilename,"rb" ))==NULL))
				{
					fwrite (&alm_sg,sizeof (INT_ALRM),1, fp1 );
					record_num = data.next_pos;
					Swap2Byte(&record_num);
					fwrite (&record_num,sizeof( short),1, fp1 );
					fwrite ( zyhd.sgname,POINT_LONGNAME_SIZE,1, fp1 ); /*+10 added by zyp on 2004.8*/
					fwrite ( zyhd.longname,POINT_LONGNAME_SIZE,data.next_pos, fp1 );/*+10 added by zyp on 2004.8*/
					fwrite ( zyhd.data_type,sizeof (CHAR),data.next_pos, fp1 );	/*v4.0 added */
					fwrite ( zyhd.hi_alm,sizeof (float),data.next_pos, fp1 );	/*v3.0 ANLG_DATA -> float*/
					fwrite ( zyhd.lo_alm,sizeof (float),data.next_pos, fp1 );	/*v3.0 ANLG_DATA -> float*/
					fwrite ( zyhd.pointer,sizeof (POINTER),data.next_pos, fp1 );  /*zlb 2011-9-8*/
			
					for ( m =1; m<SGH_NUM; m++ )
					{
						Msleep(CYCLE*1000);
						Getdat (0);
					}

					dateTime=pSgzy->sghd.alarm.time;
					Swap2Byte(&dateTime.ms);
					Swap2Byte(&dateTime.year);
					fwrite(&dateTime, sizeof(DATE_TIME), 1, fp1);
				
					for ( m =0; m<data.next_pos; m++ )
					{
						for ( record_num = pSgzy->nextpt; record_num < SGZY_NUM; record_num++ )
						{
							record_value = pSgzy->sg_dat.status[m][record_num];
							Swap4Byte(&record_value);
							fwrite (&record_value, sizeof(float), 1, fp1 );
						}
						for ( record_num = 0; record_num < pSgzy->nextpt; record_num++ )
						{
							record_value = pSgzy->sg_dat.status[m][record_num];
							Swap4Byte(&record_value);
							fwrite (&record_value, sizeof(float), 1, fp1 );
						}
					}
				}
				else
				{
					fread (&sgfile,sizeof (SGZY_DY),1, fpdy );
					fclose ( fpdy );
					Swap2Byte(&sgfile.next_pos);
					for ( m =0; m<sgfile.next_pos; m++ )
						Swap2Byte(&sgfile.buffer[m].pt_id);
#ifdef DEBUG
					printf ( "sgzy:sgfilename =%s\n",sgfilename );
#endif

					fwrite (&alm_sg,sizeof (INT_ALRM),1, fp1 );

					noadd = sgfile.next_pos;
					for ( pnt =0; pnt<sgfile.next_pos; pnt++ )
					{
						for ( m =0; m<data.next_pos; m++ )
						{
							if (( sgfile.buffer[pnt].dev_id == data.buffer[m].dev_id) &&
							   ( sgfile.buffer[pnt].data_type == data.buffer[m].data_type ) &&
							   ( sgfile.buffer[pnt].pt_id == data.buffer[m].pt_id))
							{
								sgdyptnum[pnt] =m;
								break;
							}
						}
						if ( m>= data.next_pos)
						{
							sgdyptnum[pnt] =-1;
							noadd--;
						}
					}

					record_num = noadd;
					Swap2Byte(&record_num);
					fwrite (&record_num,sizeof( short),1, fp1 );
					fwrite ( zyhd.sgname,POINT_LONGNAME_SIZE,1, fp1 ); /* +10 added by zyp on 2004.8*/

					for ( pnt =0; pnt<sgfile.next_pos; pnt++ )
						if (( m = sgdyptnum[pnt] )!=-1 )
							fwrite ( zyhd.longname[m],POINT_LONGNAME_SIZE,1, fp1 );
					for ( pnt =0; pnt<sgfile.next_pos; pnt++ )
						if (( m = sgdyptnum[pnt] )!=-1 )
							fwrite (&zyhd.data_type[m],sizeof (CHAR),1, fp1 );  /*v4.0 added */
					for ( pnt =0; pnt<sgfile.next_pos; pnt++ )
						if (( m = sgdyptnum[pnt] )!=-1 )
							fwrite (&zyhd.hi_alm[m],sizeof (float),1, fp1 );  /*v3.0 ANLG_DATA-->float*/
					for ( pnt =0; pnt<sgfile.next_pos; pnt++ )
						if (( m = sgdyptnum[pnt] )!=-1 )
							fwrite (&zyhd.lo_alm[m],sizeof (float),1, fp1 );  /*v3.0 ANLG_DATA-->float*/
					for ( pnt =0; pnt<sgfile.next_pos; pnt++ )
						if (( m = sgdyptnum[pnt] )!=-1 )
							fwrite (&zyhd.pointer[m],sizeof (POINTER),1, fp1 );  /*zlb 2011-9-8*/
					for ( m =1; m<SGH_NUM; m++ )
					{
						Msleep(CYCLE*1000);
						Getdat (0);
					}
					dateTime=pSgzy->sghd.alarm.time;
					Swap2Byte(&dateTime.ms);
					Swap2Byte(&dateTime.year);
					fwrite(&dateTime, sizeof(DATE_TIME), 1, fp1);

					for ( pnt =0; pnt<sgfile.next_pos; pnt++ )
					{
						if (( m = sgdyptnum[pnt] )!=-1 )
						{
							for ( record_num = pSgzy->nextpt; record_num < SGZY_NUM; record_num++ )
							{
								record_value = pSgzy->sg_dat.status[m][record_num];
								Swap4Byte(&record_value);
								fwrite (&record_value, sizeof(float), 1, fp1 );
							}
							for ( record_num = 0; record_num < pSgzy->nextpt; record_num++ )
							{
								record_value = pSgzy->sg_dat.status[m][record_num];
								Swap4Byte(&record_value);
								fwrite (&record_value, sizeof(float), 1, fp1 );
							}
						}
					}
				}
				fclose ( fp1 );
			}
			pSgzy->sghd.sg_flg=0;
		}
	}
}

