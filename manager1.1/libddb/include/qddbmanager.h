#ifndef QDDBMANAGER_H
#define QDDBMANAGER_H

#include <QObject>
#include <QTimer>

#include "global.h"
#include "qutildef.h"
#include "qmulticastreceiver.h"
#include "qmulticastsender.h"
#include "qmulticastview.h"
#include "qleaderelection.h"
#include "qclientmanager.h"
#include "qabstractreceiver.h"

class DDB_EXPORT QDdbManager : public QObject, public QAbstractReceiver
{
	Q_OBJECT

public:
	~QDdbManager();

protected:
	QDdbManager(QObject *parent=0);

public:
	static QDdbManager* instance();

public:
	void start();

public:
	QLeaderElection::ElectionRole electionRole() const;
	void setElectionRole(QLeaderElection::ElectionRole role);

public:
	void setLeader(const ServerInfo& info);

	int leaderLapse(const QDateTime& datetime);

public:
	void electLeader();

public:
	virtual void recvInformDdbConnection();
	virtual void recvPrivilegeData(int flag, const QMap<QString, PrivilegeGroup>& groups);

public slots:
	void privilegesChanged();

private:
	void queryUploadPrivilegeData(bool isRetData);

private slots:
	void timeout();

	void connected(QClient* client);
	void connectFailed(QClient* client);
	void disconnected(QClient* client);

private:
	QString		m_uid;

	QMulticastReceiver*	m_pReceiver;
	QMulticastView*		m_pView;

	QMulticastSenderPtr	m_pSender;
	QLeaderElection*	m_pElect;

	quint16		m_nPortStep;	//当网络中存在不可连接的leader，递增本地的端口号，在新端口重新选举一个leader

	QTimer	m_timer;
	int		m_nCount;

	QDateTime	m_time;		//被选举为leader的起始时间

	QClientManager*		m_pClientManager;
	QClient*		m_pClient;
	quint32			m_nUsrID;
};

#endif // QDDBMANAGER_H
