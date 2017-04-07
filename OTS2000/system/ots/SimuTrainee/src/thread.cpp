#include "thread.h"

void HeartBeatThread::run()
{
	udpSocket = new QUdpSocket;//dzy modified from constructor fun():HeartBeatThread()
	while(TRUE)
	{
		//udpSocket->writeDatagram((char *)&user_info,sizeof(user_info),QHostAddress::Broadcast,HEART_BEAT_PORT);
		udpSocket->writeDatagram((char *)&m_user_info,sizeof(m_user_info),QHostAddress(g_ots_hdb_ip),HEART_BEAT_PORT);
		MySleep(2000);
		if (stop_flag)
			break;
	}
}

/*******************************************************************************************************************/

void DisplayThread::run()
{
	QString strStatusBar;
	QStringList strTaskStatus,strSimuStatus,strSimuMode;
	TASK_INFO old_task_info,task_info;
	unsigned char old_runflag;
	unsigned char old_speed;
	short old_simu_mode,old_task_startable;
	int old_group_id,old_group_role;
	old_group_id = shm_ots_addr->groups_id;
	old_group_role = shm_ots_addr->groups_role;
	old_speed = shm_ots_addr->speed;
	old_runflag = shm_ots_addr->runflag;
	old_simu_mode = shm_ots_addr->simu_mode;
	old_task_startable = shm_ots_addr->data_task.task_startable;//dzy
 	taskManage->getCurTaskInfo(&old_task_info);
	strTaskStatus.append(QString::fromLocal8Bit("任务未接收"));
	strTaskStatus.append(QString::fromLocal8Bit("任务已接收"));
	strTaskStatus.append(QString::fromLocal8Bit("任务已启动"));
	strTaskStatus.append(QString::fromLocal8Bit("任务已完成"));
	strTaskStatus.append(QString::fromLocal8Bit("任务非正常结束"));
	strSimuStatus.append(QString::fromLocal8Bit("停止"));
	strSimuStatus.append(QString::fromLocal8Bit("运行"));
	strSimuStatus.append(QString::fromLocal8Bit("暂停"));
	strSimuMode.append(QString::fromLocal8Bit("培训模式"));
	strSimuMode.append(QString::fromLocal8Bit("考试模式"));
	strSimuMode.append(QString::fromLocal8Bit("竞赛模式"));
	while(!stop_flag)
	{
		if (shm_ots_addr->login_status)
		{
 			taskManage->getCurTaskInfo(&task_info);
			if (shm_ots_addr->data_task.flag_new_send_task == 1&&shm_ots_addr->data_task.task_startable==1)//dzy
			{
				shm_ots_addr->data_task.flag_new_send_task = 0;
				emit sigNewTask();
			}
			if (shm_ots_addr->runflag)
			{
				sigUpdateSimuProgress(shm_ots_addr->prgrate);
			}
			if (task_info.task_id != old_task_info.task_id)
			{
				emit sigUpdateCurTaskName(task_info.task_name);
			}
			if (task_info.task_status != old_task_info.task_status || old_speed != shm_ots_addr->speed 
				|| old_runflag != shm_ots_addr->runflag || old_simu_mode != shm_ots_addr->simu_mode)
			{
// 				strStatusBar = QString::fromLocal8Bit("   任务状态:%1   仿真状态:%2   仿真速度:%3   培训模式:%4")
// 								.arg(strTaskStatus.at(task_info.task_status))
// 								.arg(strSimuStatus.at(shm_ots_addr->runflag))
// 								.arg(QString::number(shm_ots_addr->speed/8.0))
// 								.arg(strSimuMode.at(shm_ots_addr->simu_mode));
// 				emit sigUpdateStatusBar(strStatusBar);
				if ( old_simu_mode != shm_ots_addr->simu_mode)
				{
						emit sigTrainmodelchanged(shm_ots_addr->simu_mode);//dzy
				}
				if (old_runflag != shm_ots_addr->runflag)
				{
					emit sigSimustatechanged(shm_ots_addr->runflag);
				}
				old_runflag = shm_ots_addr->runflag;

				old_speed = shm_ots_addr->speed;
				old_simu_mode = shm_ots_addr->simu_mode;
				old_task_info = task_info;
			}
			if (shm_ots_addr->groups_id != old_group_id || shm_ots_addr->groups_role != old_group_role)
			{
				emit sigGroupinfoChanged();
				old_group_id = shm_ots_addr->groups_id;
				old_group_role = shm_ots_addr->groups_role;
			}
		}
		else
		{
 			emit sigForceLogout();
			break;
		}
		MySleep(1000);
	}
}
/***************************************************************************************************************************************/
void NetstateThread::run()
{
	int server_port;
	if (strcmp(g_ots_hdb_type,"ORACLE")==0)
	{		
		server_port = 1521;
		nettest =  QSqlDatabase::addDatabase("QOCI","NETTEST"); 
	}
	else if (strcmp(g_ots_hdb_type,"MYSQL")==0)
	{
		server_port = 3306;
		nettest = QSqlDatabase::addDatabase("QMYSQL","NETTEST"); 
	}
	nettest.setHostName(g_ots_hdb_ip);   
	nettest.setDatabaseName(g_ots_hdb_sid);   
	nettest.setUserName("ots");   
	nettest.setPassword("ots");   
	nettest.setPort(server_port); 
	connectok = true;
	while(!stop_flag)
	{
		tryconeectdb();	
		if (!connected)
		{
			tryagain();
		}
		else
		{
			MySleep(5000);
		}
// 		if( InitOTSHdb()!=(int)TRUE )
// 		{
// 			QMessageBox().information(NULL,QString::fromLocal8Bit("错误!"),QString::fromLocal8Bit("InitOTSHdb失败!"),QMessageBox::Ok);		
// 			return FALSE;
// 		} 
	}
}
bool NetstateThread::tryconeectdb()
{	
	connected = nettest.open();

	return connected;
}
void NetstateThread::tryagain()
{
	cout_connected++;
	if(cout_connected<=15)
	{
		emit showtryconeect(cout_connected,false);
		MySleep(5000);	
		tryconeectdb();
		if (!connected)
		{
			tryagain();
		}
		else
		{
			emit showtryconeect(0,true);
			cout_connected = 0;
			return;
		}
	}
	else
	{
		emit showtryconeectfail();
		stop_flag = true;
		CloseHDB(nettest);
		//cout_connected = 0;
	}
}