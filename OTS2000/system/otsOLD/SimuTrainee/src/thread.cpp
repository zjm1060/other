#include "thread.h"

void HeartBeatThread::run()
{
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
	short old_simu_mode;
	old_speed = shm_ots_addr->speed;
	old_runflag = shm_ots_addr->runflag;
	old_simu_mode = shm_ots_addr->simu_mode;
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
	while(!stop_flag)
	{
		if (shm_ots_addr->login_status)
		{
 			taskManage->getCurTaskInfo(&task_info);
			if (shm_ots_addr->data_task.flag_new_send_task == 1)
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
				strStatusBar = QString::fromLocal8Bit("   任务状态:%1   仿真状态:%2   仿真速度:%3   培训模式:%4")
								.arg(strTaskStatus.at(task_info.task_status))
								.arg(strSimuStatus.at(shm_ots_addr->runflag))
								.arg(QString::number(shm_ots_addr->speed/8.0))
								.arg(strSimuMode.at(shm_ots_addr->simu_mode));
				emit sigUpdateStatusBar(strStatusBar);
				old_speed = shm_ots_addr->speed;
				old_runflag = shm_ots_addr->runflag;
				old_simu_mode = shm_ots_addr->simu_mode;
				old_task_info = task_info;
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
