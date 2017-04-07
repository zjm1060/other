#ifndef QCLIENTMANAGER_H
#define QCLIENTMANAGER_H

#include <QObject>
#include <QMap>
#include <QMutex>

#include "qclient.h"
#include "qabstractcontroller.h"

class NETWORK_EXPORT QClientManager : public QObject
{
	Q_OBJECT

public:
	QClientManager(QSharedControllerPtr controller, QObject *parent=0);
	~QClientManager();

public:
	typedef struct _clientInfo 
	{
		ServerInfo server;

		bool	connected;
		int		reconnect;
	}ClientInfo;

public:
	QClient* addClient(const ServerInfo& info);

	void remove(QClient* client);
	void removeAll();

	bool isConnected(QClient* client);
	bool contains(QClient* client);

public:
	void connectToHost(QClient* client);
	void reconnect(QClient* client);

public:
	void parseMessage(QClient* sender, int msgType, const QByteArray& buffer);		//解析信息包，用于解析非网络传输后的消息

signals:
	void connected(QClient* client);
	void connectFailed(QClient* client);
	void disconnected(QClient* client);

private slots:
	void clientQuit();
	void clientRecvMessage(QConnection* sender, QByteArray buffer);
	void clientStateChanged(QAbstractSocket::SocketState socketState);
	void clientSocketError(QAbstractSocket::SocketError socketError);

private:
	QMutex	m_mutex;

	QSharedControllerPtr	m_pController;
	QMap<QClient*, ClientInfo>	m_mapClient;
};

#endif // QCLIENTMANAGER_H
