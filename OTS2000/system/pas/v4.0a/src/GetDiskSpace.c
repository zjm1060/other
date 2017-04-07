
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include    "../../../pas/v4.0a/inc/pdc_def.h"
#include    "../../../dps/v4.0a/inc/dps_functions_def.h"


void *GetDiskSpace(PDC_TASK *pTask)
{ 
	char	tmp[256], hostname[MAX_HOST_NAME_SIZE]; 
	long	avail,	total;
	int		drive;
	int	rate;
	int		run_flag;
	USHORT	old_state;
	POINTER		pt;
	

	for(;;)
	{
		sleep(pTask->check_period);
		if((run_flag = PdcCheck(pTask))!=0) 
			continue;

#ifdef WINDOWS_OS
		{
			struct	diskfree_t free;
			drive = _getdrive ();		/*	获取被检测盘名（3为C盘)	*/
			_getdiskfree (drive, &free );	/*	获取检测盘自由空间	*/
			
			if( free.sectors_per_cluster == 0xFFFF)
			{
				SendTraceMsg(0, 0, "Fail to call getdiskfree");	
				continue;
			}

			total =(long)free.total_clusters;	/*	获取检测盘总簇数	*/	
					//* (long)free.bytes_per_sector*(long)free.sectors_per_cluster; 
					/*	每个扇区字节数*每个簇的扇区数=512字节	*/
			avail = (long)free.avail_clusters;	/*	获取检测盘可用簇数	*/
				//	* (long)free.bytes_per_sector* (long)free.sectors_per_cluster;

			rate = avail*100/total;
		}
#else

		{
			char	cmd_str[128], filename[128];	
			char	buff[256], tmp[256];
			FILE	*fp;
			
			sprintf(filename, "%sgetdiskspace_tmp", SYS_TMP_PATH);
			sprintf(cmd_str, "df -h|grep /home/ems > %s", filename);
			system(cmd_str);
			if(NULL==(fp=fopen(filename, "r")))
			{
				SendTraceMsg(0,0, "Fail to open %s", filename);
				continue;
			}

			rate= 0;
			while(fgets(buff, sizeof(buff), fp)!=NULL)
			{
				if(sscanf(buff, "%s %s %s %s %d %s", tmp,tmp,tmp,tmp,&rate, tmp)!=6)
				{
					SendTraceMsg(0,0, "Fail to get disk infomation: %s", buff);
					continue;
				}
				break;
			}
			rate= 100-rate;
			fclose(fp);
			unlink(filename);
		}		

#endif

		SendTraceMsg(0, 0, "Available disk percent is %d%", rate);

		if(-1==GetLocalHostStnId(&pt.stn_id))
			continue;
		if(-1==GetLocalSysLcuId(&pt.dev_id))
			continue;
		
		pt.data_type= ANA_TYPE;
		gethostname(hostname, sizeof(hostname));
		sprintf(tmp, "%s_%s", hostname, "FREE_DISK");
		if(-1!=GetEntryIdByName(tmp, &pt))
			BroadCastAnaPtValue(pt, 0, (float)rate);
		
		pt.data_type= SYSMSG_TYPE;
		sprintf(tmp, "%s_%s", hostname, "DISK_ALM");
		if(-1!=GetEntryIdByName(tmp, &pt))
		{
			GetIndPtStateById(&pt, &old_state);
			if(rate < 10 && old_state==0)
			{
				SendTraceMsg(0,0,"Disk free space alarm: %d%", rate);
				BroadCastIndPtState(pt, 0, 1);
			}
			else if(rate >=10. && old_state!=0)
				BroadCastIndPtState(pt, 0, 0);
		}
	}
}


