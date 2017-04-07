// client_service.cpp : 定义控制台应用程序的入口点。
//

#define DEBUG
#include "stdafx.h"
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>
#include <QSqlDatabase>   
#include <QSqlQuery>
#include <QVariant>
//DZY ADDED 140715 
#include <QtGlobal>
#include <QSystemSemaphore>
#include <QSharedMemory>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "/home/ems/system/dpp/v4.0a/inc/dppdef.h"
#include "/home/ems/system/dpp/v4.0a/inc/dpp_functions.h"
#include "/home/ems/system/ots/SimuService/inc/SimuService.h"

Q_DECLARE_METATYPE(HANDLE);
#define MAX_PROC_NUM	5;
typedef LONG ( NTAPI *_NtSuspendProcess )( IN HANDLE ProcessHandle );
typedef LONG ( NTAPI *_NtResumeProcess )( IN HANDLE ProcessHandle );
QMap <QString,HANDLE> proc_map;
_NtSuspendProcess NtSuspendProcess = 0;
_NtResumeProcess NtResumeProcess = 0;
QStringList proc_list;
bool flag_settingscene;//设置工况中标志
QSqlDatabase db_scene,db_service;
QMutex mutex;
struct scene_pt_data
{	
	char		LogicalName[38];
	float		value;
	unsigned short	status;
}SCENE_PT_DATA;

typedef struct groups_info
{
	int		groups_id;
	short	groups_role;
}GROUPS_INFO;

DMS_COMMON	dms_rec;

#ifdef MSWIN32
 DWORD		thread_train,thread_scene,thread_record,thread_chat,wait_thrid;
 WSADATA	wsaData;
#endif
#ifndef MSWIN32
 thread_t	thread_train,thread_scene,thread_record,thread_chat,wait_thrid;
#endif
int InitOTSShm();
void * ThreadTrain( void * arg );
void * ThreadScene( void * arg );
void * ThreadRecord( void * arg );
void * ThreadChat( void * arg );
int SetFaultData(int fault_id);
int SetFaultLine(DATA_FAULT_DATA *p_data);
int SetSceneData(int scene_id);
int SetPtValue(char *logicalname,float data,short status,int mode);
int GetLocalGroupsInfo(GROUPS_INFO *p_groups_info);
int GetLocalTrainInfo(DATA_TRAIN_INFO *p_train);
int GetPtFromDev(unsigned char stn_id,unsigned char dev_id);
int GetPtFromDbin();
int	InsertSceneData(int scene_id);

HANDLE GetProcessHandleById(int nID);//通过进程ID获取进程句柄
HANDLE GetProcessHandleByName(LPCTSTR pName);//通过进程名获取进程句柄

int ChildThread();
//dzy added retry to connct to HDB
void reconnectHDB();
struct scene_pt_data scn_data[MAX_PT_NUM];
unsigned int pt_num;

void * RtuAllSend(LCUID  lcuno); 
//dzy 20150120 for fault
int BDINDFAULT(char* logicalname,short status);
int BDANAFAULT(char* logicalname,float value);

int	ReadCMD( short *ptr, struct net_cmd_seg* p_netcmdseg );	
//DZY ADDED 140715 
QSystemSemaphore sema("OTS_SIMUSERVICE",1,QSystemSemaphore::Open); 
QSharedMemory mem("SimuService");// 全局对象名 
//END
int _tmain(int argc, char * argv[])
{
	QApplication app(argc, argv);
 	char hostname[20];
	 flag_settingscene = false;//设置工况中标志
	/********dzy ADDED 140715***********/	
	// To mark simuservice is running	
	sema.acquire();
	if (!mem.create(1))// 如果全局对象以存在则退出 
	{ 
		
		sema.release();// 如果是 Unix 系统，会自动释放。 
		exit(0);
	} 
	sema.release();// 临界区
	/******************************/
	unsigned char	stnid,devid;
	struct net_cmd_seg netcmdseg;
	short	ots_netcmd_ptr;
	LCUID lcuno;
//	app_id = HDB_APP;
	wsa_startup();
	//SetLogLevel(argc,argv);
	gethostname(hostname,20); 
	if(-1==InitSysLog(H9000_MODULE_SIMUSERVICE, "SimuService"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== SimuService.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}

	if ( SimulogDataLoad () == -1 )
	{
		printf ( "simulog.dat error!\n" );
		exit (1);
	}

	if(lan_init_all() == FALSE)
	{
		printf("==== SimuService: call Lan_init_all() error\n");		
		exit(0);
	}

	ReadOtsDefFile();

	if(get_hdb_shm()==FALSE)
	{
		exit(0);
	}

	if(get_ots_shm()==FALSE)
	{
		exit(0);
	}
	

	/******** clear the shm  **********/
	
	if(ConnectHDB(db_service)!=(int)TRUE)
	{
		printf("连接数据库失败\n");
		return FALSE;
	}
	else
	{
		printf("连接数据库成功\n");
	}
	if(ConnectHDB(db_scene,"scene")!=(int)TRUE)
	{
		printf("连接数据库失败\n");
		return FALSE;
	}
	else
	{
		printf("连接数据库成功\n");
	}

	if( InitOTSHdb()!=TRUE )
	{
		printf("InitOTSHdb 失败\n");
		return FALSE;
	}
	else
	{
		printf("InitOTSHdb 成功\n");
	}

	InitOTSShm();
	printf("hostname=%s, host=%d\n",g_host_name,g_host_id);

	proc_list << "agc.exe" << "SimuEngine.exe";
	for (int i = 0; i < proc_list.size(); ++i)
	{
		HANDLE ProcessHandle = 0;
		proc_map.insert(proc_list.at(i),ProcessHandle);
	}
	
	NtSuspendProcess = (_NtSuspendProcess) 
		GetProcAddress( GetModuleHandle( _T("ntdll") ), "NtSuspendProcess" );
	NtResumeProcess = (_NtResumeProcess) 
		GetProcAddress( GetModuleHandle( _T("ntdll") ), "NtResumeProcess" );
	ChildThread();
/**************allsend function***************/
	g_all_send = FALSE;
	ots_netcmd_ptr = shm_net_cmd->head;
	printf("\nwaiting for new polling cmd...\n");

	for(;;)
	{
		//MySleep(1000);
		//探测trainee 是否在执行 退出则跟着退
		bool traineeruning,tutorrunning;
		QSystemSemaphore sema("OTS_SIMUTRAINEE",1,QSystemSemaphore::Open); 
		QSharedMemory mem("SimuTrainee");// 全局对象名 
		sema.acquire();// 在临界区操作共享内存   SharedMemory
		if (mem.create(1))// 如果全局对象不存在则退出 
		{ 
			traineeruning = false;
			mem.deleteLater();
			sema.release();// 如果是 Unix 系统，会自动释放。 
		} 
		else
		{
			traineeruning = true;
			mem.deleteLater();
			sema.release();
		}
		QSystemSemaphore sema2("OTS_TEACHER",1,QSystemSemaphore::Open); 
		QSharedMemory mem2("TEACHER_MANAGER");
		sema2.acquire();// 在临界区操作共享内存   SharedMemory
		if (mem2.create(1))// 如果全局对象不存在则退出 
		{ 
			tutorrunning = false;
			mem2.deleteLater();
			sema2.release();// 如果是 Unix 系统，会自动释放。 
		} 
		else
		{
			tutorrunning = true;
			mem2.deleteLater();
			sema2.release();
		}
		if (!tutorrunning&&!traineeruning)
		{
			exit(0);
		}
		//探测
		MySleep(100);
		if( g_all_send != TRUE && shm_ots_addr->login_status == 1 && shm_ots_addr->groups_role == 0)
		{
			continue;
		}
		if( ReadCMD( &ots_netcmd_ptr, &netcmdseg ) != TRUE) 
		{
			continue;
		}
		printf("get new polling cmd\n");	
		lcuno.stnid = netcmdseg.net_cmd_buf.netdata.stn_id;
		lcuno.devid = netcmdseg.net_cmd_buf.netdata.dev_id;
		RtuAllSend(lcuno);
		BdSms("全送召唤已经完成");
		printf("send all data completed\n");

	}
	UnMountDB ();

	return 0;
}

int InitOTSShm()
{
	shm_ots_addr->user_id=g_user_id;
	shm_ots_addr->host_id=g_host_id;
	shm_ots_addr->groups_id=g_groups_id;
	shm_ots_addr->groups_role=g_groups_role;
	shm_ots_addr->hdb_write_flag=g_hdb_write;
	shm_ots_addr->runflag = 0;
	shm_ots_addr->speed = 8;
	shm_ots_addr->login_status = FALSE;
	shm_ots_addr->dbinflag  = FALSE;
	shm_ots_addr->data_scene.requestflag=0;
	shm_ots_addr->data_scene.type=0;
	shm_ots_addr->data_scene.lockflag=0;
	shm_ots_addr->data_scene.pt_num=0;
	shm_ots_addr->data_task.flag_new_send_task=0;
	shm_ots_addr->data_task.flag_new_scene=0;
	shm_ots_addr->data_task.flag_new_fault=0;
	shm_ots_addr->data_task.task_status=1;
	shm_ots_addr->data_task.scene_status = 0;
	shm_ots_addr->data_task.fault_status = 0;
	shm_hdb_addr->write_alm_flag = FALSE;
	shm_hdb_addr->write_sec_flag = FALSE;
	shm_ots_addr->all_points_readout = -1;
	return TRUE;
}

int ChildThread()
{
	char	log_str[256];
#ifdef	MSWIN32
	HANDLE		res;
#else
	int 		res;
#endif

#ifndef 	MULTITHREAD	
	ThreadTrain(NULL);
#endif

#ifndef MSWIN32
	res =   thr_create(NULL, 0, ThreadTrain, NULL, 0, &thread_train);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== SimuService: Create train thread thr_id=%d OK\n",
			(int)thread_train);	
		log_printf(INF,1,PLC_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== SimuService: Create train thread failed\n");
		log_printf(ERR,1,PLC_APP,log_str);
		return FALSE;
	}
	MySleep(1000);
	res =   thr_create(NULL, 0, ThreadScene, NULL, 0, &thread_scene);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== SimuService: Create scene thread thr_id=%d OK\n",
			(int)thread_scene);	
		log_printf(INF,1,PLC_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== SimuService: Create scene thread failed\n");
		log_printf(ERR,1,PLC_APP,log_str);
		return FALSE;
	}
#else
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) ThreadTrain, 
		 		(LPVOID)NULL, 
				(DWORD)0,
				(LPDWORD) &thread_train );

	if(res!=(int)NULL)
	{
		sprintf(log_str,"==== SimuService: Create train thread thr_id=%d OK\n",
			(int)thread_train);	
		log_printf(H9000_LOG_INFO,1,app_code,log_str);
	}
	else
	{
		sprintf(log_str,"==== SimuService: Create thread failed\n");
		log_printf(H9000_LOG_ERROR,1,app_code,log_str);
		return FALSE;	    
	}
	MySleep(1000);
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) &ThreadScene, 
		 		(LPVOID)NULL,	0,	(LPDWORD)&thread_scene);

	if(res!=(int)NULL)
	{
		sprintf(log_str,"==== SimuService: Create scene thread thr_id=%d OK\n",
			(int)thread_scene);	
		log_printf(H9000_LOG_INFO,1,app_code,log_str);
	}
	else
	{
		sprintf(log_str,"==== SimuService: Create scene thread failed\n");
		log_printf(H9000_LOG_ERROR,1,app_code,log_str);
		return FALSE;	    
	}
#endif

	MySleep(100);	
	return TRUE;
}


void * ThreadTrain( void * arg )
{
	//QMutexLocker locker(&mutex);
	GROUPS_INFO old_groups_info,groups_info;
	DATA_TRAIN_INFO train,old_train,data_train;
	FLAG_TRAIN_INFO flag_train;
	DB_FMT data;
	char time_str[22];
	char sms_str[26];
	unsigned char	stnid,devid;
	data.table_id=TABLE_TRAIN_INFO;

	InitDbData(&data,data.table_id);
	old_groups_info.groups_id = -1;
	old_groups_info.groups_role = -1;
	GetLocalTrainInfo(&old_train);
	printf("\n==== waitting for user login ...... \n");
	for(;;)
	{
		MySleep(QUERY_INTERVAL * 1000);
		flag_train.task_status = FALSE;
		flag_train.scene_status = FALSE;
		flag_train.fault_status = FALSE;
		flag_train.status = FALSE;
		if( g_all_send == TRUE)
		{
			shm_ots_addr->groups_id = 0;
			continue;
		}
		if( GetLocalTrainInfo(&train)==TRUE )
		{
			shm_ots_addr->user_id = train.user_id;
			if (train.login_status == TRUE && GetLocalGroupsInfo(&groups_info)==TRUE)
			{
				shm_ots_addr->groups_id = groups_info.groups_id;
				shm_ots_addr->groups_role = groups_info.groups_role;
				if (groups_info.groups_id != old_groups_info.groups_id)
				{
					data_train.task_status = 0;
					flag_train.task_status = TRUE;
					data_train.scene_status = 0;
					flag_train.scene_status = TRUE;
					data_train.fault_status = 0;
					flag_train.fault_status = TRUE;
					data_train.status = 0;
					flag_train.status = TRUE;
					shm_ots_addr->runflag = 0;
					if (groups_info.groups_id != -1 && groups_info.groups_role == 0)
					{
						printf("\n分组变更，同步该组数据！\n");
						for(int i=1;i<=g_stn_num;i++)
						{
							if(-1==GetStnIdByIndex(i, &stnid))
								continue;
							for(devid=1;devid<=data_station[stnid].dev_num;devid++)
							{
								BdPollingCmd(stnid,devid);
							}
						}
					}
					old_groups_info = groups_info;
				}
				shm_ots_addr->login_status = train.login_status;
			}
			else
			{
				shm_ots_addr->groups_id = -1;
				shm_ots_addr->groups_role = -1;
				shm_ots_addr->login_status = train.login_status;
				continue;
			}
			shm_ots_addr->inter_mode = train.inter_mode;/*zwj*/
			shm_ots_addr->simu_mode = train.simu_mode;
			shm_ots_addr->data_task.task_startable = train.task_startable;//dzy
			//新任务处理：
			if( train.task_status == 1 && shm_ots_addr->data_task.task_status != 0&&shm_ots_addr->data_task.task_startable == 1)
			{
				if (/*groups_info.groups_id == -1  ||groups_info.groups_role == 1 ||*/shm_ots_addr->simu_mode == 0||shm_ots_addr->simu_mode == 1) //DZY new ADDED .groups_id ==0 ||groups_info.groups_id ==0
				{
					GetCurTimeStr(time_str);
					printf("Get new task ,name =%s time=%s\n", train.task_name,time_str);
					sprintf(sms_str,"培训仿真新任务下达！");
					BdSms(sms_str);
					//shm_ots_addr->data_task.flag_new_send_task=1;//DZY 挪到下面
					shm_ots_addr->data_task.send_task_id = train.send_task_id; 
					shm_ots_addr->data_task.task_id = train.task_id;
					shm_ots_addr->data_task.scene_id = train.scene_id;
					shm_ots_addr->data_task.fault_id = train.fault_id;
					shm_ots_addr->data_task.scene_status = train.scene_status;
					shm_ots_addr->data_task.fault_status = train.fault_status;
					shm_ots_addr->data_task.task_status = 0;
					shm_ots_addr->data_task.is_record = train.is_record;
					data_train.task_status = 2;
					flag_train.task_status = TRUE;
					shm_hdb_addr->write_alm_flag = TRUE;
					shm_hdb_addr->write_sec_flag = TRUE;
					shm_ots_addr->data_task.flag_new_send_task=1;//dzy  挪到这 
				}
				else if (shm_ots_addr->simu_mode == 2 && groups_info.groups_id >0 && groups_info.groups_role != 1 )
				{
					sprintf(sms_str,"此学员非组长，不可接任务");
					BdSms(sms_str);
					data_train.task_status = 0;
					flag_train.task_status = TRUE;
				}
				else   if (shm_ots_addr->simu_mode == 2 && groups_info.groups_id >0 && groups_info.groups_role == 1 )//dzy new added  for  task test
				{
					GetCurTimeStr(time_str);
					printf("Get new task ,name =%s time=%s\n", train.task_name,time_str);
					sprintf(sms_str,"培训仿真新任务下达！");
					BdSms(sms_str);
					//shm_ots_addr->data_task.flag_new_send_task=1;//DZY 挪到下面
					shm_ots_addr->data_task.send_task_id = train.send_task_id; 
					shm_ots_addr->data_task.task_id = train.task_id;
					shm_ots_addr->data_task.scene_id = train.scene_id;
					shm_ots_addr->data_task.fault_id = train.fault_id;
					shm_ots_addr->data_task.scene_status = train.scene_status;
					shm_ots_addr->data_task.fault_status = train.fault_status;
					shm_ots_addr->data_task.task_status = 0;
					shm_ots_addr->data_task.is_record = train.is_record;
					data_train.task_status = 2;
					flag_train.task_status = TRUE;
					shm_hdb_addr->write_alm_flag = TRUE;
					shm_hdb_addr->write_sec_flag = TRUE;
					shm_ots_addr->data_task.flag_new_send_task=1;//dzy  挪到这 
				}
			}
			//任务完成处理：	//0:无新任务；1:新的任务；2: 任务已处理
			else if( train.task_status >= 3 && old_train.task_status != train.task_status)
			{
				GetCurTimeStr(time_str);
				printf("Get new task ,name =%s time=%s\n", train.task_name,time_str);
				sprintf(sms_str,"培训仿真任务结束！");
				BdSms(sms_str);
				shm_hdb_addr->write_sec_flag = FALSE;
			}
			//工况处理：
			if( train.scene_status == 1)
			{
				QMapIterator<QString, HANDLE> i(proc_map);
				while (i.hasNext()) 
				{
					i.next();
					HANDLE ProcessHandle = GetProcessHandleByName(LPCTSTR(i.key().utf16()));
					proc_map[i.key()] = ProcessHandle;
					if (NtSuspendProcess && ProcessHandle)
						NtSuspendProcess( proc_map[i.key()] );
				}
				shm_ots_addr->data_task.scene_status  = data_train.scene_status;
				printf("Get new scene ,name =%s \n", train.scene_name);			
				sprintf(sms_str,"\n培训仿真新工况已设置！");		
				BdSms(sms_str);	
				printf("%s\n",sms_str);
				data_train.scene_status = 2;
				SetSceneData(train.scene_id);
				flag_train.scene_status = TRUE;
				shm_ots_addr->data_task.scene_status  = data_train.scene_status;
				MySleep(500);
				while (i.hasPrevious()) 
				{
					i.previous();
					HANDLE ProcessHandle = proc_map[i.key()] ; 
					if (NtResumeProcess && ProcessHandle)
						NtResumeProcess( proc_map[i.key()]);
					CloseHandle(proc_map[i.key()] );
				}
			}
 			if( train.fault_status == 1&&train.scene_status != 1)//dzy added 先下工况 后下故障
			{
				printf("Get new fault ,name =%s \n", train.fault_name);
			
				SetFaultData(train.fault_id);
				shm_ots_addr->all_points_readout = 0;
// 				sprintf(sms_str,"培训仿真新故障已设置！");
// 				data_train.fault_status = 2;
// 				flag_train.fault_status = TRUE;
// 				BdSms(sms_str);	
// 				printf("%s\n",sms_str);
			}
			if( train.fault_status == 1&&train.scene_status == 1&&shm_ots_addr->data_task.scene_status == 2)//dzy added 先下工况 后下故障
			{
				printf("Get new fault ,name =%s \n", train.fault_name);
				
				SetFaultData(train.fault_id);
				shm_ots_addr->all_points_readout = 0;
			}
			if (train.fault_status == 1&&shm_ots_addr->all_points_readout == 2)
			{
				sprintf(sms_str,"培训仿真新故障已设置！");
				data_train.fault_status = 2;
				flag_train.fault_status = TRUE;
				BdSms(sms_str);	
				printf("%s\n",sms_str);
			}
			if( train.speed !=shm_ots_addr->speed )
			{
				if( train.speed<1 || train.speed>16*8)
				{
					printf("err: train speed  in hdb =%d \n", train.speed);	
				}
				else
				{
					printf("Get new speed ,speed =%d \n", train.speed);	
					sprintf(sms_str,"培训仿真速度已变更为%d！",train.speed);
					printf("%s\n",sms_str);
					shm_ots_addr->speed = train.speed;
				}
			}
			//学员仿真控制：
			if( train.status !=shm_ots_addr->runflag && flag_train.status == FALSE)
			{
				if (groups_info.groups_id != -1 && groups_info.groups_role == 0/*&&shm_ots_addr.simu_mode>0*/)
				{
					shm_ots_addr->runflag = 0;
					strcpy(sms_str,"组员不能独立开始仿真!");
					BdSms(sms_str);	
					data_train.status = 0;
					flag_train.status = TRUE;
				}
				else
				{
					printf("Get new run status ,status =%d \n", train.status);	
					sprintf(sms_str,"仿真运行方式已变更为%d！",train.status);
					printf("%s\n",sms_str);

					if(train.status == 1)
					{
						strcpy(sms_str,"仿真开始,请在oix操作!");
						BdSms(sms_str);	
					}
					else if(train.status == 0)
					{
						strcpy(sms_str,"仿真停止!");
						BdSms(sms_str);	
					}
					else if(train.status ==2)
					{
						strcpy(sms_str,"仿真暂停!");
						BdSms(sms_str);	
					}

					shm_ots_addr->runflag = train.status;
					data_train.status = train.status;
					flag_train.status = TRUE;
				}
			}
			if(flag_train.task_status == TRUE || flag_train.scene_status==TRUE || flag_train.fault_status==TRUE || flag_train.status==TRUE)
			{
				data.flag.flag_train_info = flag_train;
				data.seg.seg_train_info = data_train;
				data.cond_flag.flag_train_info.host_id=TRUE;
				data.cond_seg.seg_train_info.host_id=g_host_id;
				if( UpdateOneRec(&data) != TRUE)
				{
					printf("更新数据失败\n");	
					reconnectHDB();
					continue;
				}
				else
				{
					printf("更新数据成功\n");	
				}
			}
			old_train = train;
		}//end	if( GetLocalTrainInfo(&train)==TRUE )

	}//end	for(;;)
	return NULL;
}

void * ThreadScene( void * arg )
{
	DATA_SCENE data_scene;
	FLAG_SCENE flag_scene;
	char time_str[22];
	DB_FMT data;
	printf("\n==== waitting for new scene request ...... \n");
	for(;;)
	{
		if( shm_ots_addr->data_scene.requestflag == 1 )
		{
			printf("\n get new scene request\n");
			shm_ots_addr->data_scene.pt_num = 0;
			shm_ots_addr->data_scene.requestflag = 0;
			shm_ots_addr->data_scene.lockflag = 1;
			GetCurTimeStr(time_str)	;
			data.table_id=TABLE_SCENE;
			InitDbData(&data,data.table_id);
			flag_scene.scene_id = TRUE;
			flag_scene.name = TRUE;
			flag_scene.src = TRUE;
			flag_scene.user_id = TRUE;
			flag_scene.station_id = TRUE;
			flag_scene.dev_id = TRUE;
			flag_scene.datetime = TRUE;
			flag_scene.name = TRUE;
			flag_scene.filename = TRUE;
			flag_scene.remark = TRUE;
			data_scene.scene_id = GetMaxIdInHdb(TABLE_SCENE)+1;
			strcpy(data_scene.filename,"");
			strcpy(data_scene.datetime,time_str);
			data_scene.station_id = shm_ots_addr->data_scene.stn_id;
			data_scene.dev_id = shm_ots_addr->data_scene.dev_id;
			data_scene.user_id = shm_ots_addr->user_id;
			data_scene.src= shm_ots_addr->data_scene.type;
			strcpy(data_scene.name,shm_ots_addr->data_scene.name);
			strcpy(data_scene.remark,shm_ots_addr->data_scene.remark);
			switch ( shm_ots_addr->data_scene.type )
			{
			case 1:
				strcpy(data_scene.filename,shm_ots_addr->data_scene.filename);		
				GetPtFromDbin();
				break;
			case 2:
				GetPtFromDev(shm_ots_addr->data_scene.stn_id,shm_ots_addr->data_scene.dev_id);
				break;
			case 3:

				break;
			case 4:

				break;
			default:
				printf("\n err: scene request type =%d \n",shm_ots_addr->data_scene.type);
				break;

			}

			data.flag.flag_scene = flag_scene;
			data.seg.seg_scene = data_scene;

			//调用接口函数，参数为DB_FMT结构体
			if( InsertOneRec(&data,db_scene) != TRUE)
			{
				HdbLogWrite(data.sql_str,data.sql_error);
				printf("插入Scene数据失败\n");	
				shm_ots_addr->data_scene.lockflag = 0;
				reconnectHDB();
				continue;
			}
			else
			{
				printf("插入Scene数据成功\n");		
			}
			if(InsertSceneData(data_scene.scene_id) != TRUE)
			{
				printf("插入SceneData数据失败\n");	
				shm_ots_addr->data_scene.lockflag = 0;
				continue;
			}
			else
			{
				printf("插入SceneData数据成功\n");
			}
			shm_ots_addr->data_scene.lockflag = 0;
		}
		MySleep(1000);
	}
	return NULL;
}
void * ThreadRecord( void * arg )
{
	INFO_RECORD		*shm_info_rec_addr; 
	if(get_rec_shm() == FALSE )
	{
		printf("\nhdbwrite.c: call get_rec_shm() error\n");		
		exit(0);
	}
	shm_info_rec_addr = (INFO_RECORD*) LinktoShareMem( SHM_INFO_RECORD_KEY_CHAR );
	if( shm_info_rec_addr == (INFO_RECORD*)-1 )
	{
		err_write("Fail to link shm_info_rec_addr shm");
		exit(0);
	}

	for(;;)
	{

		MySleep(1000);
	}
	return NULL;
}
void * ThreadChat( void * arg )
{
	for(;;)
	{
		MySleep(1000);
	}
	return NULL;
}
int GetLocalGroupsInfo(GROUPS_INFO *p_groups_info)
{
	if (shm_ots_addr->data_scene.lockflag)
		return FALSE;
	DB_FMT data;
	data.table_id=TABLE_USERINFO;
	InitDbData(&data,data.table_id);
	
	data.cond_flag.flag_userinfo.user_id=TRUE;
	data.cond_seg.seg_userinfo.user_id=shm_ots_addr->user_id;

	if( SelectRecs(&data) != TRUE)
	{
		printf("查询数据失败\n");	
		reconnectHDB();
		return FALSE;
	}
	else
	{
		if( data.rownum != 1 )
		{
			printf("返回分组信息查询记录 %d 条 error\n",data.rownum);
			return FALSE;
		}
		else
		{
			p_groups_info->groups_id = data_userinfo[0].groups_id;
			p_groups_info->groups_role = data_userinfo[0].groups_role;
			printf("当前分组信息：分组号为 %d  分组角色为 %d\n",data_userinfo[0].groups_id,data_userinfo[0].groups_role);
			return TRUE;
		}
	}
}

int GetLocalTrainInfo(DATA_TRAIN_INFO *p_train)
{
	if (shm_ots_addr->data_scene.lockflag)
		return FALSE;
	DB_FMT data;
	data.table_id=TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);

	
	//设置查询条件
	data.cond_flag.flag_train_info.host_id=TRUE;
	data.cond_seg.seg_train_info.host_id=g_host_id;
	//调用接口函数，参数为DB_FMT结构体
	if( SelectRecs(&data) != TRUE)
	{
		printf("查询数据失败\n");	
		reconnectHDB();
		return FALSE;
	}
	else
	{
		printf("查询数据成功\n");
		if( data.rownum != 1 )
		{
			printf("返回查询记录 %d 条 error\n",data.rownum);
			return FALSE;
		}
		else
		{
			printf("返回查询记录 %d 条 success\n",data.rownum);
			*p_train = data_train_info[0];
		}
		return TRUE;
	}
}

int SetFaultData(int fault_id)
{
	DB_FMT data;
	int i;
	data.table_id=TABLE_FAULT_DATA;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_fault_data.fault_id = 1;
	data.cond_seg.seg_fault_data.fault_id = fault_id;

	if( SelectRecs(&data) != TRUE)
	{
		printf("查询数据失败\n");	
		reconnectHDB();
		return FALSE;
	}
	else
	{
		while(flag_settingscene)////设置工况中标志
		{
			MySleep(100);
			printf("等待工况设置中……\n");
		}
		printf("查询数据成功\n");
		printf("返回查询记录 %d 条\n",data.rownum);		
	}
	//yyp needed to be modified，//dzy new mofied for faultdata
	if (data.rownum > 50)
	{
		printf("查询数据失败\n");	
		return FALSE;
	}
	else
		shm_ots_addr->ptnum = data.rownum;
	for(i=0;i<data.rownum;i++)
	{
		memcpy(&(shm_ots_addr->data_fault_points[i].type), &(data_fault_data[i].type), 2);
		strcpy(shm_ots_addr->data_fault_points[i].logicalname,data_fault_data[i].logicalname);
		shm_ots_addr->data_fault_points[i].ivalue =data_fault_data[i].init_value;
		shm_ots_addr->data_fault_points[i].dvalue =data_fault_data[i].dst_value;
		shm_ots_addr->data_fault_points[i].norm_value =data_fault_data[i].norm_value;		

// 		MySleep(data_fault_data[i].delay*1000);
// 		if(data_fault_data[i].chg_mode == 0)
// 		{
// 			SetPtValue(data_fault_data[i].logicalname,data_fault_data[i].norm_value,(short)data_fault_data[i].norm_value,1);
// 		}
// 		else
// 		{
// 			SetFaultLine(&data_fault_data[i]);
// 		}
	}
	return TRUE;
}


int SetFaultLine(DATA_FAULT_DATA *p_data)
{
	int type;
	int count,i;
	float fstep;
	POINTER		point;
	float cur_value;
	if(GetPtIdByNameStr( p_data->logicalname, &point )==-1)
	{
		return FALSE;
	}
	type = point.data_type;

	if(type != ANA_TYPE  && type != PARAM_TYPE  )
	{
		return FALSE;
	}


	count = p_data->chg_time / 0.2;
	fstep = (p_data->dst_value-p_data->init_value)/count;
	cur_value = p_data->init_value;

	for(i=0;i<count;i++)
	{
		cur_value = cur_value + fstep;
		WrANA ( p_data->logicalname, cur_value, 0 );
		MySleep(200);
	}
	return TRUE;
}

int SetSceneData(int scene_id)
{
	DB_FMT data;
	int i;
	char str[26];
	data.table_id=TABLE_SCENE_DATA;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_scene_data.scene_id = 1;
	data.cond_seg.seg_scene_data.scene_id = scene_id;

	if( SelectRecs(&data) != TRUE)
	{
		printf("查询数据失败\n");
		sprintf(str,"\n查询数据失败！");
		reconnectHDB();
		return FALSE;
	}
	else
	{
		flag_settingscene = true;//设置工况中标志
		printf("查询数据成功\n");
		printf("返回查询记录 %d 条\n",data.rownum);		
	}
	for(i=0;i<data.rownum;i++)
	{
		SetPtValue(data_scene_data[i].logicalname,data_scene_data[i].data,(short)data_scene_data[i].status,0 );
	}
	//printf("工况设置完成\n",data.rownum);
	sprintf(str,"\n工况设置完成！");
	BdSms(str);	
	MySleep(500);
	flag_settingscene = false;//设置工况中标志
	return TRUE;
}


int SetPtValue(char *logicalname,float data,short status,int mode)//mode =1 故障 mode=0 工况
{
	time_t now;
	struct tm *datetime;
	char TimeStr[30];
	int type;
	POINTER		point;
	DATE_TIME	updatetime;
	GetSysTime(&updatetime);
	time(&now);
	datetime=localtime(&now);
	tmtostring(*datetime,TimeStr);
	if(GetPtIdByNameStr( logicalname, &point )==-1)
	{
		return FALSE;
	}
	type = point.data_type;
	switch	( type )
	{
	case	ANA_TYPE:
	case	PARAM_TYPE:
			if(mode ==0)
			{
				WrANA ( logicalname, data, status );
			}
			else
			{				
				BDANAFAULT ( logicalname, data );
				//BdANAByName ( logicalname, data );
			}
			break;

	case	POL_TYPE:
	case	SOE_TYPE:
	case	SYSMSG_TYPE:
	case	DEVOBJ_TYPE:
	case	OUTS_TYPE:
			if(mode ==0)
			{		
				WrIND ( logicalname,status );
			}
			else
			{				
				BDINDFAULT( logicalname,status );
				BdINDByName ( logicalname,status );
			}
			break;

	default:
		break;
	}
	return TRUE;
}

int GetPtFromDbin()
{
	int yx_num,yc_num;
	pt_num = 0;
	for (yx_num=0;yx_num<shm_ots_addr->data_point.yxnum;yx_num++)
	{
		strcpy(scn_data[pt_num].LogicalName,shm_ots_addr->data_point.yxname[yx_num]);
		pt_num++;
		if(pt_num<MAX_PT_NUM ) continue; 
		else return FALSE;
	}
	for (yc_num=0;yc_num<shm_ots_addr->data_point.ycnum;yc_num++)
	{
		strcpy(scn_data[pt_num].LogicalName,shm_ots_addr->data_point.ycname[yc_num]);
		pt_num++;
		if(pt_num<MAX_PT_NUM ) continue; 
		else return FALSE;
	}
	return TRUE;
}

int GetPtFromDev(unsigned char stn_id,unsigned char dev_id)
{
	int i=0;
	int		DataType,	PointId,	Number;
	char		lname[100];
	POINTER		point;
	POINT_NAME	pt_name;	
 	int 		Start,End;
	pt_num = 0;
 	if( dev_id == 0 )
	{
		Start = 1;
 		End = g_dev_num[stn_id];
	}
	else
	{
		Start = dev_id;
 		End = dev_id;
	}
	
	for(dev_id=Start;dev_id<=End;dev_id++)
	{
		for(DataType=1;DataType<=MAX_DATATYPE_NUM;DataType++)
		{								
			if( DataType==ANA_TYPE || DataType==PARAM_TYPE || DataType==IMP_TYPE || DataType==SOE_TYPE || 
				DataType==POL_TYPE || DataType==SYSMSG_TYPE || DataType==DEVOBJ_TYPE || DataType==OUTS_TYPE )
			{	
				point.stn_id	  =	stn_id;	 		
				point.dev_id      =	dev_id;
				point.data_type   =	DataType;
				point.pt_id    =	0;
				Number = GetPointNumber ( &point );
				for(PointId=0;PointId<Number;PointId++)
				{
					point.stn_id	  =	stn_id;
					point.dev_id      =	dev_id;
					point.data_type   =	DataType;
					point.pt_id    =	PointId;

					if(GetPtNameById( &point, lname, &pt_name ) ==-1)
					{
						continue;
					}
					
					strcpy(scn_data[i].LogicalName,lname); 
										
					if( (++i)<MAX_PT_NUM ) continue; 
						else return FALSE;
				}
			}
		}
	}
	pt_num = i;
	return TRUE;
}


int	InsertSceneData(int scene_id)
{
	unsigned int i;
  	DB_DATA		db_data;
	//统一函数参数数据体
	DB_FMT data;
	char	log_str[256];
	for(i=0;i<pt_num;i++)
	{
		if(ReadDBByName( scn_data[i].LogicalName, &db_data )==-1)
		{	
			sprintf(log_str,"%s invalid!",scn_data[i].LogicalName);
			err_write(log_str);
			return FALSE;
		}
		//memcpy(&scn_data[i].status,&db_data.status,2);
		scn_data[i].status = db_data.status;
		scn_data[i].value = db_data.fvalue;
	}
	data.table_id=TABLE_SCENE_DATA;
	InitDbData(&data,data.table_id);
	//将需要访问的字段flag置为1
	data.flag.flag_scene_data.scene_id =TRUE;
	data.flag.flag_scene_data.logicalname=TRUE;
	data.flag.flag_scene_data.status=TRUE;
	data.flag.flag_scene_data.data=TRUE;
	data.seg.seg_scene_data.scene_id = scene_id;
	for(i=0;i<pt_num;i++)
	{
		//填充表中每个字段的值
		data.seg.seg_scene_data.status =scn_data[i].status;
		data.seg.seg_scene_data.data =scn_data[i].value;
		strcpy(data.seg.seg_scene_data.logicalname ,scn_data[i].LogicalName);

		//调用接口函数，参数为DB_FMT结构体
		if( InsertOneRec(&data,db_scene) != TRUE)
		{
			printf("插入数据失败\n");
			reconnectHDB();
			continue;
		}
		else
		{
			printf("插入数据成功\n");	
		}
	}
	shm_ots_addr->data_scene.pt_num = pt_num;
	printf("InsertSceneData successfully, num = %d\n",pt_num);
	return TRUE;
}


int	ReadCMD( short *ptr, struct net_cmd_seg* p_netcmdseg )
{
	short	shm_end;

	shm_end	= *ptr ;

	while ( shm_end != shm_net_cmd->head )
	{
		if(shm_net_cmd->seg[shm_end].lan_head.dp_type != COMM_SIM || 
			shm_net_cmd->seg[shm_end].net_cmd_buf.netdata.type_id != C_DATA_POLLING )
		{
			shm_end ++ ;
			if ( shm_end >= MAX_NETCMD_SEG_NUM )
				shm_end = 0;
			continue;
		}
		*p_netcmdseg = shm_net_cmd->seg[shm_end];
		shm_end++;
		if ( shm_end >= MAX_NETCMD_SEG_NUM )
			shm_end = 0;
		*ptr	= shm_end;
		return ( TRUE );
	}
	*ptr	= shm_end;
	return ( FALSE );
} 


void * RtuAllSend(LCUID  lcuno) 	
{
	unsigned short  j;
	unsigned int 	point_num;
	IND_STRUCT 		*msg_db;
	IND_DATA		*msg_all;
	ANA_STRUCT		*ana_db;
	AIALLSTRUCT		*ai_all;
	DEVOBJ_STRUCT 	*obj_db;
	OBJALLSTRUCT	*obj_all;
	DATE_TIME		tm;
	printf("\n------SYS_RTU_ALL_SEND (%d.%d) ------%d\n", lcuno.stnid,lcuno.devid,time(NULL));

	GetSysTime(&tm);
	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, POL_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(msg_db=(IND_STRUCT *)malloc(point_num*sizeof(IND_STRUCT))))
			return ((void*)-1);
		if(NULL==(msg_all=(IND_DATA *)malloc(point_num*sizeof(IND_DATA))))
		{
			free(msg_db);
			return ((void*)-1);
		}

		ReadDb_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_db);

		for(j=0;j<point_num;j++)
			msg_all[j]= msg_db[j].status;
	
		AllSend_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_all, tm);

		free(msg_db);
		free(msg_all);
	}
//	return ((void*)-1);

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, ANA_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(ana_db=(ANA_STRUCT *)malloc(point_num*sizeof(ANA_STRUCT))))
			return ((void*)-1);
		if(NULL==(ai_all=(AIALLSTRUCT *)malloc(point_num*sizeof(AIALLSTRUCT))))
		{
			free(ana_db);
			return ((void*)-1);
		}

		ReadDb_ANA(lcuno, ANA_TYPE, 0, (USHORT)point_num, ana_db);

		for(j=0;j<point_num;j++)
		{
			ai_all[j].data = ana_db[j].data;
			ai_all[j].status = ana_db[j].status;
		}
	
		AllSend_ANA(lcuno, DPS_ANA_ALL, ANA_TYPE, 0, (USHORT)point_num, ai_all, tm);

		free(ana_db);
		free(ai_all);
	}	

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, PARAM_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(ana_db=(ANA_STRUCT *)malloc(point_num*sizeof(ANA_STRUCT))))
			return ((void*)-1);
		if(NULL==(ai_all=(AIALLSTRUCT *)malloc(point_num*sizeof(AIALLSTRUCT))))
		{
			free(ana_db);
			return ((void*)-1);
		}

		ReadDb_ANA(lcuno, PARAM_TYPE, 0, (USHORT)point_num, ana_db);

		for(j=0;j<point_num;j++)
		{
			ai_all[j].data = ana_db[j].data;
			ai_all[j].status = ana_db[j].status;
		}
	
		AllSend_ANA(lcuno, DPS_ANA_ALL, PARAM_TYPE, 0, (USHORT)point_num, ai_all, tm);

		free(ana_db);
		free(ai_all);
	}	

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, SOE_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(msg_db=(IND_STRUCT *)malloc(point_num*sizeof(IND_STRUCT))))
			return ((void*)-1);
		if(NULL==(msg_all=(IND_DATA *)malloc(point_num*sizeof(IND_DATA))))
		{
			free(msg_db);
			return ((void*)-1);
		}

		ReadDb_IND(lcuno, SOE_TYPE, 0, (USHORT)point_num, msg_db);

		for(j=0;j<point_num;j++)
			msg_all[j]= msg_db[j].status;
	
		AllSend_IND(lcuno, SOE_TYPE, 0, (USHORT)point_num, msg_all, tm);

		free(msg_db);
		free(msg_all);
	}

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, POL_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(msg_db=(IND_STRUCT *)malloc(point_num*sizeof(IND_STRUCT))))
			return ((void*)-1);
		if(NULL==(msg_all=(IND_DATA *)malloc(point_num*sizeof(IND_DATA))))
		{
			free(msg_db);
			return ((void*)-1);
		}

		ReadDb_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_db);

		for(j=0;j<point_num;j++)
			msg_all[j]= msg_db[j].status;
	
		AllSend_IND(lcuno, POL_TYPE, 0, (USHORT)point_num, msg_all, tm);

		free(msg_db);
		free(msg_all);
	}

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, OUTS_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(msg_db=(IND_STRUCT *)malloc(point_num*sizeof(IND_STRUCT))))
			return ((void*)-1);
		if(NULL==(msg_all=(IND_DATA *)malloc(point_num*sizeof(IND_DATA))))
		{
			free(msg_db);
			return ((void*)-1);
		}

		ReadDb_IND(lcuno, OUTS_TYPE, 0, (USHORT)point_num, msg_db);

		for(j=0;j<point_num;j++)
			msg_all[j]= msg_db[j].status;
	
		AllSend_IND(lcuno, OUTS_TYPE, 0, (USHORT)point_num, msg_all, tm);

		free(msg_db);
		free(msg_all);
	}

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, SYSMSG_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(msg_db=(IND_STRUCT *)malloc(point_num*sizeof(IND_STRUCT))))
			return ((void*)-1);
		if(NULL==(msg_all=(IND_DATA *)malloc(point_num*sizeof(IND_DATA))))
		{
			free(msg_db);
			return ((void*)-1);
		}

		ReadDb_IND(lcuno, SYSMSG_TYPE, 0, (USHORT)point_num, msg_db);

		for(j=0;j<point_num;j++)
			msg_all[j]= msg_db[j].status;
	
		AllSend_IND(lcuno, SYSMSG_TYPE, 0, (USHORT)point_num, msg_all, tm);

		free(msg_db);
		free(msg_all);
	}

	if(-1!=GetEntryNumById(lcuno.stnid, lcuno.devid, DEVOBJ_TYPE, &point_num) && point_num>0) 
	{
		if(NULL==(obj_db=(DEVOBJ_STRUCT *)malloc(point_num*sizeof(DEVOBJ_STRUCT))))
			return ((void*)-1);
		if(NULL==(obj_all=(OBJALLSTRUCT *)malloc(point_num*sizeof(OBJALLSTRUCT))))
		{
			free(msg_db);
			return ((void*)-1);
		}
		ReadDb_OBJ(lcuno, DEVOBJ_TYPE, 0, (USHORT)point_num, obj_db);

		for(j=0;j<point_num;j++)
		{
			obj_all[j].fb_status1= obj_db[j].fb_status1;
			obj_all[j].fb_status2= obj_db[j].fb_status2;
			obj_all[j].param= obj_db[j].param;
		}
	  	AllSend_OBJ(lcuno,DPS_DEV_FB_ALL,DEVOBJ_TYPE, 0, (USHORT)point_num, obj_all,tm);
		free(obj_db);
		free(obj_all);
	}
	return ((void*)1);
}
HANDLE GetProcessHandleById(int nID)//通过进程ID获取进程句柄
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, nID);
}

HANDLE GetProcessHandleByName(LPCTSTR pName)//通过进程名获取进程句柄
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe)) {
		if (!_tcscmp(pe.szExeFile, pName)) {
			CloseHandle(hSnapshot);
			return GetProcessHandleById(pe.th32ProcessID);
		}
	}
	return NULL;
}

void reconnectHDB()
{
	if(ConnectHDB(db_service)!=(int)TRUE)
	{
		printf("重新连接数据库失败\n");
	}
	else
	{
		printf("重新连接数据库成功\n");
	}
	if(ConnectHDB(db_scene,"scene")!=(int)TRUE)
	{
		printf("重新连接数据库失败\n");
	}
	else
	{
		printf("重新连接数据库成功\n");
	}

}

int BDANAFAULT(char* logicalname,float value)
{
	POINTER point;
	char	log_str[256];

	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		sprintf(log_str,"BdANAByName: pt %s not found\n",logicalname); 
		err_write(log_str);	
		return FALSE;
	}
/////////
	struct 	tm	 tmpt;
	time_t		tim;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;

	cmd_buf.type_id = C_ANA_FORCE;
	cmd_buf.stn_id = point.stn_id;
	cmd_buf.dev_id = point.dev_id;
	cmd_buf.data_type =point.data_type;
	cmd_buf.point_id = point.pt_id;

	cmd_buf.status.state=0; 
	cmd_buf.status.result=0;
	cmd_buf.status.opr=0;
	cmd_buf.status.src=0;  
	cmd_buf.data.iValue=(int)(value*1000);
	//cmd_buf.ctrl_addr=pdbcomm->fixed.ctrl_addr; /////////////////控制地址是多少？
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"IEC104S");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif

	lanhead.dest_id = 0;
	lanhead.dest_stn = 0;
	lanhead.dp_type=/*COMM_RECORD_TIME*/ALLB;

	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
}
int BDINDFAULT(char* logicalname,short status)
{
	POINTER point;
	char	log_str[256];
	if(-1==GetPtIdByNameStr(logicalname, &point))
	{
		sprintf(log_str,"BdINDByName: pt %s not found\n",logicalname); 
		err_write(log_str);	
		return FALSE;
	}
	struct 	tm 	tmpt;
	time_t		tim;
	LAN_HEAD	lanhead;
	COMMAND_MSG	cmd_buf;

	cmd_buf.type_id = C_ONOFF_FORCE;
	cmd_buf.stn_id = point.stn_id;
	cmd_buf.dev_id = point.dev_id;
	cmd_buf.data_type = point.data_type;
	cmd_buf.point_id = point.pt_id;
	status = status +256; /* added by yyp 06.11.10 */	
	memcpy(&cmd_buf.status, &status, 2);
// 	cmd_buf.status.opr=status;
// 	cmd_buf.status.state=1;
// 	cmd_buf.status.result=0;
	//cmd_buf.status.src=cmd_src;
	cmd_buf.data.iValue=0;
	cmd_buf.ctrl_addr=0;
	GetLocalHostId(&cmd_buf.host_id);
	cmd_buf.usr_id=0;

	tim=time (NULL);
	tmpt=*localtime (&tim);
	cmd_buf.second=tmpt.tm_sec;
	cmd_buf.minute=tmpt.tm_min;
	cmd_buf.hour=tmpt.tm_hour;
	cmd_buf.day=tmpt.tm_mday;
	cmd_buf.month=tmpt.tm_mon+1;
	cmd_buf.year=tmpt.tm_year+1900;
	strcpy(cmd_buf.usr_login_name,"IEC104S");
	lanhead.length=sizeof(COMMAND_MSG);

#ifdef SUN_OS
	Swap2Byte(&cmd_buf.point_id);
	Swap2Byte(&cmd_buf.status);
	Swap4Byte(&cmd_buf.data.iValue);
	Swap2Byte(&cmd_buf.ctrl_addr);
	Swap2Byte(&cmd_buf.year);
	Swap2Byte(&lanhead.length);
#endif

	lanhead.dest_id = 0;
	lanhead.dest_stn = 0;
	lanhead.dp_type=/*COMM_RECORD_TIME*/COMM_SIM;

	lanhead.src_id =cmd_buf.host_id;
	lanhead.packet_num =0;
	lan_out ((char *)&cmd_buf,lanhead,(unsigned char)0);
	return TRUE;

}