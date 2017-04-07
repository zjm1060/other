// login_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <QApplication>
#include <QtGui>
#include <QtCore/QCoreApplication>
#include <QSqlDatabase>   
#include <QSqlQuery>
#include <QVariant>

#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h" 
#include "../../SimuTutor/src/qcommon.h"
#include "../../../lan/v4.0a/inc/lan_common.h"
#include "../../../lan/v4.0a/inc/lan_private.h"
#define TIMEOUT	10
QMap<int,USER_INFO> map_host;
QSqlDatabase db_thread;
void * TimeOut( void * arg );
int KickOut(int host_id);
int IsUserLogin(int user_id,int host_id);
void HeartBeatRecv(SOCKET sock);
int _tmain(int argc, char * argv[])
{
	QApplication app(argc, argv);
	SOCKET sock;
	char	log_str[256];
	char hostname[20];
#ifdef	MSWIN32
	HANDLE		res;
	DWORD		thread_timeout;
#else
	int 		res;
	thread_t	thread_timeout;
#endif
	wsa_startup();
	map_host.clear();
	gethostname(hostname,20); 
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== client_service.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	ReadOtsDefFile();
	if(ConnectHDB(db_conn)!=(int)TRUE)
	{
		printf("连接数据库失败\n");
		return FALSE;
	}
	else
	{
		printf("连接数据库成功\n");
	}
	if(ConnectHDB(db_thread,"thread")!=(int)TRUE)
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

	printf("hostname=%s, host=%d\n",g_host_name,g_host_id);
#ifndef MSWIN32
	res =   thr_create(NULL, 0, TimeOut, NULL, 0, &thread_timeout);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== login_test: Create timeout thread thr_id=%d OK\n",
			(int)thread_timeout);	
		log_printf(H9000_LOG_INFO,1,app_code,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== login_test: Create train thread failed\n");
		log_printf(H9000_LOG_INFO,1,app_code,log_str);
		return FALSE;
	}
#else
	res=CreateThread(NULL,	0,	(LPTHREAD_START_ROUTINE) TimeOut, 
		(LPVOID)NULL, 
		(DWORD)0,
		(LPDWORD) &thread_timeout );

	if(res!=(int)NULL)
	{
		sprintf(log_str,"==== login_test: Create timeout thread thr_id=%d OK\n",
			(int)thread_timeout);	
		log_printf(H9000_LOG_INFO,1,app_code,log_str);
	}
	else
	{
		sprintf(log_str,"==== login_test: Create timeout thread failed\n");
		log_printf(H9000_LOG_ERROR,1,app_code,log_str);
		return FALSE;	    
	}
#endif
	for (;;)
	{
		sock=udp_init(HEART_BEAT_PORT);
		if(sock!=INVALID_SOCKET)	break;
		printf("udp init failed,continue......\n");
		MySleep(1000);
	}
	HeartBeatRecv(sock);
	udp_close();
	UnMountDB ();
	CloseHDB(db_conn);
	CloseHDB(db_thread);
	return 0;
}
void HeartBeatRecv(SOCKET sock)
{
	short dcount;
	USER_INFO user_info;
	for (;;)
	{
		dcount=udp_recv(sock,(char *)&user_info,sizeof(USER_INFO));
		if(dcount<0)
		{
			printf("netupd.c: recv dcount=%d,recv error\n",dcount);
			continue;
		}	
		if (map_host.contains(user_info.host_id))
		{
			map_host[user_info.host_id].timeout = 0;
			map_host[user_info.host_id].user_id = user_info.user_id;
			map_host[user_info.host_id].host_id = user_info.host_id;
			strcpy(map_host[user_info.host_id].login_time,user_info.login_time);
		}
		else
		{
			if (IsUserLogin(user_info.user_id,user_info.host_id)==TRUE)
			{
				map_host.insert(user_info.host_id,user_info);
				printf("new user login user_id = %d host_id = %d\n",user_info.user_id,user_info.host_id);
			}
		}
	}
}
void * TimeOut( void * arg )
{
	for (;;)
	{
		QMap<int,USER_INFO>::const_iterator i = map_host.constBegin();
		while (i != map_host.constEnd()) 
		{
			if (i.value().timeout > TIMEOUT)
			{
				KickOut(i.key());
				i = map_host.constBegin();
			}
			else
			{
				if (i.value().timeout > 2)
					printf("user user_id = %d host_id = %d timeout = %d\n",i.value().user_id,i.value().host_id,i.value().timeout);
				map_host[i.key()].timeout = i.value().timeout+1;	
			}
			++i;
		}
		MySleep(1000);
	}
}
int IsUserLogin(int user_id,int host_id)
{
	DB_FMT data;
	data.table_id=TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_train_info.login_status = TRUE;
	data.cond_seg.seg_train_info.login_status = 1;
	data.cond_flag.flag_train_info.user_id = TRUE;
	data.cond_seg.seg_train_info.user_id = user_id;
	data.cond_flag.flag_train_info.host_id = TRUE;
	data.cond_seg.seg_train_info.host_id = host_id;
	if (SelectRecs(&data) != TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		return FALSE;
	}
	if (data.rownum == 1)	return TRUE;
	else	return FALSE;
}
int KickOut(int host_id)
{
	QMap<int,USER_INFO>::const_iterator i = map_host.find(host_id);
	DB_FMT data;
	data.table_id=TABLE_TRAIN_INFO;
	InitDbData(&data,data.table_id);
	data.flag.flag_train_info.login_status=TRUE;	
	data.seg.seg_train_info.login_status=0;
	data.flag.flag_train_info.status = TRUE;
	data.seg.seg_train_info.status=0;
	data.flag.flag_train_info.task_status = TRUE;
	data.seg.seg_train_info.task_status=0;
	data.flag.flag_train_info.scene_status = TRUE;
	data.seg.seg_train_info.scene_status=0;
	data.flag.flag_train_info.fault_status = TRUE;
	data.seg.seg_train_info.fault_status=0;
	data.flag.flag_train_info.user_id=TRUE;	
	data.seg.seg_train_info.user_id=CONST_NEG;
	data.cond_flag.flag_train_info.host_id =TRUE;
	data.cond_seg.seg_train_info.host_id = i.value().host_id;
	if (UpdateOneRec(&data,db_thread)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		printf("kicked user user_id = %d host_id = %d fail\n",i.value().user_id,i.value().host_id);
		map_host.remove(i.value().host_id);
		return FALSE;
	}
	data.table_id = TABLE_LOGIN_INFO;
	InitDbData(&data,data.table_id);
	data.cond_flag.flag_login_info.user_id = TRUE;
	data.cond_seg.seg_login_info.user_id = i.value().user_id;
	data.cond_flag.flag_login_info.host_id = TRUE;
	data.cond_seg.seg_login_info.host_id = i.value().host_id;
	data.cond_flag.flag_login_info.login_time = TRUE;
	strcpy(data.cond_seg.seg_login_info.login_time,i.value().login_time);

	data.flag.flag_login_info.logout_time = TRUE;
	GetCurTimeStr(data.seg.seg_login_info.logout_time);
	if (UpdateOneRec(&data,db_thread)!=TRUE)
	{
		HdbLogWrite(data.sql_str,data.sql_error);
		printf("kicked user user_id = %d host_id = %d fail\n",i.value().user_id,i.value().host_id);
		map_host.remove(i.value().host_id);
		return FALSE;
	}
	printf("kicked user user_id = %d host_id = %d success\n",i.value().user_id,i.value().host_id);
	map_host.remove(i.value().host_id);
	return TRUE;
}
