#ifndef SimuService_H
#define SimuService_H

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QThread>
#include <QtNetwork>
#include <QMetaType>
#include <QDebug>
#include <QMutex>
#include "/home/ems/system/ots/ots_hdb/inc/ots.h"

#include "TcpSocket.h"
//#include "SimuComm.h"

class ReadPointThread : public QThread
{
	Q_OBJECT
public:
	void run();
private:
	bool m_bInitValue;
	QStringList POINT_LIST;
	QMap<QString,POINT_INFO> POINT_MAP;
	bool initPoint();
public slots:
	void setInitValue(){m_bInitValue = true;};
};

class ReadMsgThread : public QThread
{
public:
	void run();
};

class SimuService : public QObject
{
	Q_OBJECT
public:	// construction
	SimuService(QObject *parent = 0);

public:	// overridables
	virtual void	Run();
	virtual void	Stop();
private:
	ReadPointThread *readPointThread;
	ReadMsgThread *readMsgThread;
};

#endif