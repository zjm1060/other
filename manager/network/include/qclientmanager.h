#ifndef QCLIENTMANAGER_H
#define QCLIENTMANAGER_H

#include <QObject>
#include <QMap>

#include "qclient.h"

class NETWORK_EXPORT QClientManager : public QObject
{
	Q_OBJECT

public:
	QClientManager(QObject* controller, QObject *parent=0);
	~QClientManager();

public:
	typedef struct _clientInfo 
	{
		QServerManager::ServerInfo server;

		bool	connected;
		int		reconnect;
	}ClientInfo;

public:
	QClient* addClient(const QServerManager::ServerInfo& info);

	void remove(QClient* client);
	void removeAll();

	bool isConnected(QClient* client);

public:
	void connectToHost(QClient* client);
	void reconnect(QClient* client);

signals:
	void connected(QClient* client);
	void disconnected(QClient* client);

private slots:
	void clientQuit();
	void clientRecvMessage(QConnection* sender, QByteArray buffer);
	void clientStateChanged(QAbstractSocket::SocketState socketState);
	void clientSocketError(QAbstractSocket::SocketError socketError);

private:
	QObject*	m_pController;
	QMap<QClient*, ClientInfo>	m_mapClient;
};

#endif // QCLIENTMANAGER_H
