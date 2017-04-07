#ifndef _THREAD_H_
#define _THREAD_H_
#include <QThread>
#include <QRunnable>
#include <QMutex>
#include <QUdpSocket>
#include "/home/ems/system/ots/ots_hdb/inc/ots_hdb.h"
#include "qcommon.h"

typedef struct
{	
	char		LogicalName[38];
	float		value;
	unsigned short	status;
}SCENE_PT_DATA;

class UserLoginStateThread : public QThread
{
	Q_OBJECT
public:
	UserLoginStateThread(QObject *parent);
	void run();
signals:
	void sigUserStateChanged(int user_id,short state);
private:
	QUdpSocket *udpSocket;
	QMap<int,int> mapLoginUser;
private slots:
	void readPendingDatagrams();
};
class SaveSceneThread :public  QThread
{
	Q_OBJECT
public:
	SaveSceneThread(UCHAR stn_id,UCHAR dev_id,int scene_id,QObject * parent);
	QMutex mutex;
	void run();
	void addSceneData(char *point_name,short status,float fvalue);
signals:
	void failed();
	void finished();
private:
	UCHAR stn_id;
	UCHAR dev_id;
	int	  scene_id;
};

class SaveSceneRunnable : public QObject,public QRunnable 
{
	Q_OBJECT
public:
	SaveSceneRunnable(UCHAR stnId,UCHAR devId,int sceneId):stn_id(stnId),dev_id(devId),scene_id(sceneId){pt_num = 0;};
	//virtual ~SaveSceneRunnable();
public:
	void run();
	int insertSceneData();
signals:
	void finished();
private:
	UCHAR stn_id;
	UCHAR dev_id;
	int scene_id;
	int pt_num;
	QMutex mutex;
	QWaitCondition condition;
	SCENE_PT_DATA scn_data[MAX_PT_NUM];
};
#endif