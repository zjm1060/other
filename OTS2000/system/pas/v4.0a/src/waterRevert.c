

#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../lan/v4.0a/inc/lan_common.h"
#include	"../../../plc/v4.0a/inc/lcu_data.h"
#include	"../../../dpp/v4.0a/inc/dppdef.h"
#include	"../../../dpp/v4.0a/inc/dpp_functions.h"
#include	"../../../pas/v4.0a/inc/pdc_def.h"
#include	"../../../dps/v4.0a/inc/dps_type_def.h"
#include	"../../../dps/v4.0a/inc/dps_rec_def.h"
#include	"../../../dps/v4.0a/inc/dps_functions_def.h"
#include	"../../../dps/v4.0a/inc/ipc_common.h"


#define	MAX_REVERT_TIME_NUM			4
#define	REVERT_CHECK_PERIOD			60 /*60 second*/
#define	REVERT_CMD_TIMEOUT			300 /*300 second*/

#define	MAX_DEV_NUM_IN_UNIT			10
#define	MAX_UNIT_NUM				20

enum REVERT_RUN_MODE {EXIT_RUN_MODE=0, REQUEST_RUN_MODE, AUTO_RUN_MODE};
enum REVERT_TIME_MODE {DATE_MODE=0, PERIOD_MODE};

typedef struct
{
	POINTER		enable_pt; /*参加倒换*/
	POINTER		request_pt; /*请求倒换*/
	POINTER		run_hours_pt; /*机组连续运行小时*/
	POINTER		cond_pt; /*允许倒换*/
	POINTER		success_pt; /*倒换成功*/
	POINTER		fail_pt; /*倒换失败*/
	POINTER		cmd_pt; /*倒换命令*/
	POINTER		unit_online_pt; /*机组并网*/
} WATER_REVERT_DEV;

typedef struct
{
	WATER_REVERT_DEV	dev[MAX_DEV_NUM_IN_UNIT];
	int			dev_num;
	FLOAT		last_revert_hours; 
} UNIT_DEF;

struct
{
	UNIT_DEF	unit[MAX_UNIT_NUM];
	int			unit_num;
} unit_table;

struct
{
	USHORT		run_mode;
	USHORT		time_mode;
	USHORT		manual_revert;
	POINTER		run_mode_pt; /*运行模式*/
	POINTER		time_mode_pt; /*时间模式*/
	POINTER		ack_pt; /*确认倒换*/
	POINTER		deny_pt; /*否定倒换*/
	POINTER		manual_pt; /*强制倒换*/
	POINTER		run_hours_pt; /*启动倒换连续运行小时数*/
	struct
	{
		POINTER		day;
		POINTER		hour;
	} revert_time_pt[MAX_REVERT_TIME_NUM]; /*倒换时间*/
} revert_def;



int GetPointFromFile(FILE *fp, POINTER *pt)
{
	char	string[256];

	if(fscanf(fp, "%s", string)!=1)
		return 1;
	if(-1==GetPtIdByNameStr(string, pt))
		return -1;
	return 0;
}


int GetRevertDevFromFile(FILE *fp, WATER_REVERT_DEV *dev)
{
	int		ret;

	ret= GetPointFromFile(fp, &dev->enable_pt);/*参加倒换*/
	if(1==ret)
		return 1;
	if(-1==ret)
		return -1;
	if(!IsIndTypePoint(&dev->enable_pt))
		return -1;

	if(GetPointFromFile(fp, &dev->request_pt))/*请求倒换*/
		return -1;
	if(!IsIndTypePoint(&dev->request_pt))
		return -1;

	if(GetPointFromFile(fp, &dev->run_hours_pt))/*机组连续运行小时*/
		return -1;
	if(!IsAnaTypePoint(&dev->run_hours_pt))
		return -1;

	if(GetPointFromFile(fp, &dev->cond_pt))/*允许倒换*/
		return -1;
	if(!IsIndTypePoint(&dev->cond_pt))
		return -1;

	if(GetPointFromFile(fp, &dev->success_pt))/*倒换成功*/
		return -1;
	if(!IsIndTypePoint(&dev->success_pt))
		return -1;

	if(GetPointFromFile(fp, &dev->fail_pt))/*倒换失败*/
		return -1;
	if(!IsIndTypePoint(&dev->fail_pt))
		return -1;

	if(GetPointFromFile(fp, &dev->cmd_pt))/*倒换命令*/
		return -1;
	if(!IsIndTypePoint(&dev->cmd_pt))
		return -1;

	if(GetPointFromFile(fp, &dev->unit_online_pt))/*机组并网状态*/
		return -1;
	if(!IsIndTypePoint(&dev->unit_online_pt))
		return -1;

	return 0;
}


int	ReadRevertDefineFile(void)
{
	char	string[256];
	char	err_str[256];
	char	filename[256];
	FILE	*fp, *fp1;
	int		i, ret;
	WATER_REVERT_DEV dev;
	
	sprintf(filename, "%s%s", SYS_DEFINE_PATH, WATER_REVERT_DEFINE_FILE);
	if((fp=fopen(filename, "r"))==NULL)
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		return -1;
	}
	sprintf(filename, "%swater_revert.def", SYS_TMP_PATH);
	if((fp1=fopen(filename, "w"))==NULL)
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		fclose(fp);
		return -1;
	}
	while(fgets(string, sizeof(string), fp)!=NULL)
	{
		if(string[0]!='#' && string[0]!='!')
			fputs(string, fp1);
	}
	fclose(fp);
	fclose(fp1);

	fp=fopen(filename, "r");
	
	/*运行模式*/
	if(GetPointFromFile(fp, &revert_def.run_mode_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}
	if(!IsIndTypePoint(&revert_def.run_mode_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}

	/*时间模式*/
	if(GetPointFromFile(fp, &revert_def.time_mode_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}
	if(!IsIndTypePoint(&revert_def.time_mode_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}

	/*确认倒换*/
	if(GetPointFromFile(fp, &revert_def.ack_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}
	if(!IsIndTypePoint(&revert_def.ack_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}

	/*否定倒换*/
	if(GetPointFromFile(fp, &revert_def.deny_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}
	if(!IsIndTypePoint(&revert_def.deny_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}

	/*强制倒换*/
	if(GetPointFromFile(fp, &revert_def.manual_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}
	if(!IsIndTypePoint(&revert_def.manual_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}

	/*启动倒换连续运行小时数*/
	if(GetPointFromFile(fp, &revert_def.run_hours_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}
	if(!IsAnaTypePoint(&revert_def.run_hours_pt))
	{
		fclose(fp);		
		unlink(filename);
		return -1;
	}

	
	/*倒换时间*/
	for(i=0; i<MAX_REVERT_TIME_NUM; i++)
	{
		if(GetPointFromFile(fp, &revert_def.revert_time_pt[i].day))
		{
			fclose(fp);		
			unlink(filename);
			return -1;
		}
		if(!IsAnaTypePoint(&revert_def.revert_time_pt[i].day))
		{
			fclose(fp);		
			unlink(filename);
			return -1;
		}

		if(GetPointFromFile(fp, &revert_def.revert_time_pt[i].hour))
		{
			fclose(fp);		
			unlink(filename);
			return -1;
		}
		if(!IsAnaTypePoint(&revert_def.revert_time_pt[i].hour))
		{
			fclose(fp);		
			unlink(filename);
			return -1;
		}
	}

	unit_table.unit_num= 0;
	for(;;)
	{
		ret= GetRevertDevFromFile(fp, &dev);
		if(-1==ret)
		{
			fclose(fp);		
			unlink(filename);
			return -1;
		}
		if(1==ret)
		{
			fclose(fp);		
			unlink(filename);
			return 0;
		}

		for(i=0; i<unit_table.unit_num; i++)
		{
			if(dev.cmd_pt.stn_id==unit_table.unit[i].dev[0].cmd_pt.stn_id
				&& dev.cmd_pt.dev_id==unit_table.unit[i].dev[0].cmd_pt.dev_id)
				break;
		}
		if(i==unit_table.unit_num) /*new unit*/
		{
			unit_table.unit[i].dev[0]= dev;
			unit_table.unit[i].dev_num= 1;
			unit_table.unit_num++;
		}
		else /*old unit*/
		{
			unit_table.unit[i].dev[unit_table.unit[i].dev_num]= dev;
			unit_table.unit[i].dev_num++;
		}
	}
}


int	ClearRevertRespond(void)
{
	USHORT		ack_revert, deny_revert;
	
	ack_revert= deny_revert= 0;
	GetIndPtStateById(&revert_def.ack_pt, &ack_revert);
	if(ack_revert)
		BroadCastIndPtState(revert_def.ack_pt, 0, 0);
	GetIndPtStateById(&revert_def.deny_pt, &deny_revert);		
	if(deny_revert)
		BroadCastIndPtState(revert_def.deny_pt, 0, 0);
	return 0;
}


int	ClearRevertRequest(WATER_REVERT_DEV *dev)
{
	BroadCastIndPtState(dev->request_pt, 0, 0);
	return 0;
}


int	SendRevertRequest(WATER_REVERT_DEV *dev)
{
	BroadCastIndPtState(dev->request_pt, 0, 1);
	return 0;
}

int	SendRevertCmd(WATER_REVERT_DEV *dev)
{
	LAN_HEAD		head;
	COMMAND_MSG_PKG	cmd;
	UCHAR			hostId;

	head.length= sizeof(COMMAND_MSG_PKG);
	Swap2Byte(&head.length);
	head.dp_type= COMM_TIME;
	head.dest_id= dev->cmd_pt.dev_id;
	head.dest_stn= dev->cmd_pt.stn_id;
	if(-1==GetLocalHostId(&head.src_id))
		return -1;

	cmd.type_id= C_DEVICE;
	cmd.stn_id= dev->cmd_pt.stn_id;
	cmd.dev_id= dev->cmd_pt.dev_id;
	cmd.data_type= dev->cmd_pt.data_type;
	cmd.pt_id= dev->cmd_pt.pt_id;
	Swap2Byte(&cmd.pt_id);
	memset(&cmd.cmd_status, 0, 2);
	cmd.cmd_status.opr= 2;
	cmd.cmd_status.state= 1;
	Swap2Byte(&cmd.cmd_status);
	cmd.data.iValue= 0;
	Swap4Byte(&cmd.data);
	
	GetPtCtrlAddrById(&dev->cmd_pt, &cmd.ctrl_addr);
	Swap2Byte(&cmd.ctrl_addr);
	GetLocalHostId(&hostId);
	cmd.host_id= (char)hostId;
	cmd.usr_id= 0;
	cmd.res= 0;
	FillTimeMsg(&cmd.second);
	strcpy(cmd.usr_name, "REVERT");

	lan_out((char*)&cmd, head, 0);
	return 0;
}

int	RevertAllDevs(void)
{
	int			i, j, revert_fail_flag;
	USHORT		enable_revert, cond_revert, ack_revert, deny_revert;
	USHORT		success_revert, fail_revert, unit_online;
	time_t		cmd_time, curr_time;
	FLOAT		run_hours, max_run_hours;

	GetAnaPtfValueById(&revert_def.run_hours_pt, &max_run_hours);		
	if(revert_def.time_mode!=PERIOD_MODE) /*不是周期模式*/
		BdSms("本次倒换开始");

	for(i=0; i<unit_table.unit_num; i++)
	{
		GetIndPtStateById(&revert_def.run_mode_pt, &revert_def.run_mode);		
		if(revert_def.run_mode!=AUTO_RUN_MODE && revert_def.run_mode!=REQUEST_RUN_MODE) /*强制中止倒换*/
		{
			BdSms("水系统倒换被强制中止"); 
			break;
		}

		GetAnaPtfValueById(&unit_table.unit[i].dev[0].run_hours_pt, &run_hours);
		if(run_hours < unit_table.unit[i].last_revert_hours)
			unit_table.unit[i].last_revert_hours= 0;

		if(!revert_def.manual_revert && revert_def.time_mode==PERIOD_MODE) /*周期模式*/
		{
			if((run_hours-unit_table.unit[i].last_revert_hours) < max_run_hours)
				continue;
		}		
		unit_table.unit[i].last_revert_hours= run_hours;

		for(j=0; j<unit_table.unit[i].dev_num; j++)
		{
			GetIndPtStateById(&unit_table.unit[i].dev[j].enable_pt, &enable_revert);		
			if(!enable_revert)	
				continue;
			GetIndPtStateById(&unit_table.unit[i].dev[j].cond_pt, &cond_revert);		
			if(!cond_revert)	
				continue;
			GetIndPtStateById(&unit_table.unit[i].dev[j].unit_online_pt, &unit_online);		
			if(!unit_online)	
				continue;

			if(revert_def.run_mode==REQUEST_RUN_MODE) /*半自动*/
			{
				ClearRevertRespond();
				SendRevertRequest(&unit_table.unit[i].dev[j]);
				for(;;)
				{
					sleep(1);
					ack_revert= deny_revert= 0;
					GetIndPtStateById(&revert_def.ack_pt, &ack_revert);		
					GetIndPtStateById(&revert_def.deny_pt, &deny_revert);		
					if(ack_revert || deny_revert)
						break;
				}
				ClearRevertRequest(&unit_table.unit[i].dev[j]);
				ClearRevertRespond();

				if(deny_revert)
					continue;
			}
				
			SendRevertCmd(&unit_table.unit[i].dev[j]);
			cmd_time= time(NULL);
			for(;;)
			{
				sleep(1);
				GetIndPtStateById(&revert_def.run_mode_pt, &revert_def.run_mode);		
				if(revert_def.run_mode!=AUTO_RUN_MODE && revert_def.run_mode!=REQUEST_RUN_MODE) /*强制中止倒换*/
				{
					BdSms("水系统倒换被强制中止"); 
					BdSms("本次倒换结束");
					return 0;
				}
				
				success_revert= fail_revert= 0;
				GetIndPtStateById(&unit_table.unit[i].dev[j].success_pt, &success_revert);		
				GetIndPtStateById(&unit_table.unit[i].dev[j].fail_pt, &fail_revert);		
				if(success_revert)
				{
					revert_fail_flag= 0;
					break;
				}
				if(fail_revert)
				{
					revert_fail_flag= 1;
					break;
				}
				curr_time= time(NULL);
				if((curr_time-cmd_time)>=REVERT_CMD_TIMEOUT)
				{
					revert_fail_flag= 1;
					BdSms("倒换超时");
					break;
				}
			} /*for(;;)*/
			if(revert_fail_flag)
				break;
			sleep(2);
		} /*for(dev)*/
	} /*for(unit)*/

	if(revert_def.time_mode!=PERIOD_MODE) /*周期模式*/
		BdSms("本次倒换结束");
	return 0;
}


BOOL	IsRevertTime(void)
{
	time_t		tt_now;
	struct tm	tm_time;
	float		day, hour;
	int			i;

	tt_now= time(NULL);
	tm_time= *localtime(&tt_now);
	
	for(i=0; i<MAX_REVERT_TIME_NUM; i++)
	{
		GetAnaPtfValueById(&revert_def.revert_time_pt[i].day, &day);		
		GetAnaPtfValueById(&revert_def.revert_time_pt[i].hour, &hour);		
		if(day==tm_time.tm_mday && hour==tm_time.tm_hour 
			&& 0==tm_time.tm_min && tm_time.tm_sec<REVERT_CHECK_PERIOD)
			return TRUE;	
	}
	return FALSE;
}



int main(int argc, char *argv[])
{
	time_t		tt_now;
	struct tm	tm_time;
	BOOL		app_priv;
	USHORT		host_state;

#ifdef	WINDOWS_OS
	WSADATA		WSAData;
#endif
	
	SetModuleName("WATER_REVERT");
	SetErrLogMode(argc, argv);
	CreateLockFile(".water_revert.lock");

#ifdef	WINDOWS_OS
	if(WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
	{
		Err_log("Fail to initialize net!\n");
		exit(-1);
	}
#endif

	if(-1==MountDB(NULL))
	{
		Err_log("Fail to Mount RDB");
		exit(-1);
	}

	if(GetLocalHostPrvg("APP_SERVER", &app_priv) != 0)
	{
		Err_log("Fail to find APP_SERVER define list");
		exit(-1);
	}
	if(!app_priv)
	{
		for(;;)
			sleep(10000);
	}

	if(!lan_out_init())
	{
		Err_log("Fail to init lan_out");
		exit(-1);
	}

   	if(-1==ReadRevertDefineFile())
	{
		Err_log("water revert: define file error");
		return -1;
	}

	for(;;)
	{
		if(-1==GetLocalHostState(&host_state))
		{
			sleep(5);
			continue;
		}
		if(host_state!=1) /*主控*/
		{
			sleep(5);
			continue;
		}

		GetIndPtStateById(&revert_def.run_mode_pt, &revert_def.run_mode);		
		GetIndPtStateById(&revert_def.time_mode_pt, &revert_def.time_mode);		
		GetIndPtStateById(&revert_def.manual_pt, &revert_def.manual_revert);
		if(revert_def.run_mode!=AUTO_RUN_MODE && revert_def.run_mode!=REQUEST_RUN_MODE) /*倒换退出*/
		{
			sleep(REVERT_CHECK_PERIOD);
			continue;
		}

		if(revert_def.manual_revert)
		{
			RevertAllDevs();
			BroadCastIndPtState(revert_def.manual_pt, 0, 0);
			sleep(REVERT_CHECK_PERIOD);
			continue;
		}

		if(revert_def.time_mode==DATE_MODE) /*日期模式*/
		{
			if(IsRevertTime())
				RevertAllDevs();
			sleep(REVERT_CHECK_PERIOD);
			continue;
		}
		else if(revert_def.time_mode==PERIOD_MODE) /*周期模式*/
		{
			tt_now= time(NULL);
			tm_time= *localtime(&tt_now);
			if(tm_time.tm_hour<9 || tm_time.tm_hour>=22) 
			{
				sleep(REVERT_CHECK_PERIOD);
				continue;
			}
			RevertAllDevs();
		}
	}	
}


