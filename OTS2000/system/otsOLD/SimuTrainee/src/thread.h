#ifndef _SIMUTRAINEE_THREAD_H_
#define _SIMUTRAINEE_THREAD_H_

#include <QThread>
#include <QRunnable>
#include <QMutex>
#include <QUdpSocket>
#include "taskManage.h"
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "/home/ems/system/ots/SimuTutor/src/qcommon.h"

class HeartBeatThread : public QThread
{
	Q_OBJECT
public:
	HeartBeatThread(USER_INFO user_info,QObject * parent):m_user_info(user_info){udpSocket = new QUdpSocket;stop_flag=false;};
	void run();
	void stop(){stop_flag=true;};
private:
	QUdpSocket *udpSocket;
	USER_INFO m_user_info;
	bool stop_flag;
};

class DisplayThread : public QThread
{
	Q_OBJECT
public:
	DisplayThread(QObject *parent){stop_flag=false;};
	void run();
	void stop(){stop_flag=true;};
Q_SIGNALS:
	void sigForceLogout();
	void sigNewTask();
	void sigUpdateSimuProgress(int);
	void sigUpdateCurTaskName(const QString &);
	void sigUpdateStatusBar(const QString &);
private:
	bool stop_flag;
};


#endif