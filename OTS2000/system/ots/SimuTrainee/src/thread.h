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
	HeartBeatThread(USER_INFO user_info,QObject * parent):m_user_info(user_info){		stop_flag=false;};
	void run();
	void stop(){stop_flag=true;};
	bool stop_flag;
private:
	QUdpSocket *udpSocket;
	USER_INFO m_user_info;

};

class DisplayThread : public QThread
{
	Q_OBJECT
public:
	DisplayThread(QObject *parent){stop_flag=false;};
	void run();
	void stop(){stop_flag=true;};
	bool stop_flag;
Q_SIGNALS:
	void sigForceLogout();
	void sigNewTask();
	void sigUpdateSimuProgress(int);
	void sigUpdateCurTaskName(const QString &);
	void sigUpdateStatusBar(const QString &);
	void sigTrainmodelchanged(int);
	void sigSimustatechanged(unsigned char);
	void sigGroupinfoChanged();
private:

};

class NetstateThread : public QThread
{
	Q_OBJECT
public:
	NetstateThread(){ stop_flag=false;cout_connected = 0;	};
	void run();
	void stop(){stop_flag=true;cout_connected = 0;	CloseHDB(nettest);};
	int cout_connected;
	bool stop_flag;
Q_SIGNALS:
	void exitapp();
	void showtryconeectfail();
	void showtryconeect(int i,bool j);
private:
	bool tryconeectdb();
	void tryagain();
	bool connected;
	bool connectok;
	bool reconnect;
	QSqlDatabase nettest;
};

#endif