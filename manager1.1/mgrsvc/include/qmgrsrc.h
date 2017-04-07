#ifndef QMGRSRC_H
#define QMGRSRC_H

#include <QObject>

#include "qheartbeat.h"
#include "qutildef.h"
#include "qinterprocessserver.h"

class QMgrSrc : public QObject
{
	Q_OBJECT

public:
	QMgrSrc(bool superUser, QObject *parent=0);
	~QMgrSrc();

public:
	void startServers();
	void startHeartBeat();
	void startDdb();
	void startInterProcess();
	
private:
	void init();

private slots:
	void remoteServerComing(const ServerInfo& info);
	void remoteServerQuit(const ServerInfo& info);

	void statusChanged(quint64 sign);

	void topologyChanged(GraphTopo graph);

	void deleteConnection(const QString& name);

private:
	QHeartBeatPtr	m_pHeartBeat;
	bool	m_bSuperUser;

	QInterProcessServerPtr	m_pMibServer;		//mib服务进程

	bool	m_bLock;		//本地状态改变修改上锁
};

#endif // QMGRSRC_H
