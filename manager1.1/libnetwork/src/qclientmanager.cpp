#include "qclientmanager.h"

#include "qmessageparser.h"

QClientManager::QClientManager(QSharedControllerPtr controller, QObject *parent/* =0 */)
	: QObject(parent)
	,m_pController(controller)
{
}

QClientManager::~QClientManager()
{
	removeAll();
}

QClient* QClientManager::addClient(const ServerInfo& info)
{
	if (info.count == 0)
	{
		return NULL;
	}

	QMutexLocker locker(&m_mutex);

	QMutableMapIterator<QClient*, ClientInfo> it(m_mapClient);
	while (it.hasNext())
	{
		it.next();
		ClientInfo& clientInfo=it.value();
		if (clientInfo.server == info)
		{
			clientInfo.server=info;

			return it.key();
		}
	}

	ClientInfo client;
	client.connected=false;
	client.reconnect=0;

	QClient* pClient=new QClient(info);
	client.server=info;

	connect(pClient, SIGNAL(quit()), this, SLOT(clientQuit()));
	connect(pClient, SIGNAL(recvMessage(QConnection*, QByteArray)), this, SLOT(clientRecvMessage(QConnection*, QByteArray)));
	connect(pClient, SIGNAL(clientStateChanged(QAbstractSocket::SocketState)), 
		this, SLOT(clientStateChanged(QAbstractSocket::SocketState)));
	connect(pClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientSocketError(QAbstractSocket::SocketError)));

	m_mapClient.insert(pClient, client);

	return pClient;
}

void QClientManager::remove(QClient* client)
{
	if (client != NULL && m_mapClient.contains(client))
	{
		m_mapClient.remove(client);

		client->closeSocket();
		client->deleteLater();
	}
}

void QClientManager::removeAll()
{
	QClient* pClient;

	QMapIterator<QClient*, ClientInfo> it(m_mapClient);
	while (it.hasNext())
	{
		it.next();

		pClient=it.key();

		pClient->closeSocket();
		pClient->deleteLater();
	}

	m_mapClient.clear();
}

void QClientManager::connectToHost(QClient* client)
{
	m_mutex.lock();
	if (client != NULL && m_mapClient.contains(client))
	{
		if (!m_mapClient[client].connected)
		{
			client->connectToHost();

			m_mutex.unlock();
		}
		else
		{
			m_mutex.unlock();

			emit connected(client);
		}
	}
	else
	{
		m_mutex.unlock();
	}
}

void QClientManager::reconnect(QClient* client)
{
	QMutexLocker locker(&m_mutex);

	if (client != NULL && m_mapClient.contains(client))
	{
		m_mapClient[client].reconnect=0;
		client->connectToHost();
	}
}

bool QClientManager::isConnected(QClient* client)
{
	QMutexLocker locker(&m_mutex);

	if (!m_mapClient.contains(client))
	{
		return false;
	}

	return m_mapClient[client].connected;
}

bool QClientManager::contains(QClient* client)
{
	return m_mapClient.contains(client);
}

void QClientManager::clientQuit()
{
	QClient* pClient=dynamic_cast<QClient*>(sender());
	if (pClient != NULL && m_mapClient.contains(pClient))
	{
		m_mapClient[pClient].connected=false;
	}

	emit disconnected(pClient);
}

void QClientManager::parseMessage(QClient* sender, int msgType, const QByteArray& buffer)
{
	QMessageParser::instance()->messageInstance(msgType)->parse(sender->connection(), buffer, m_pController.get());
}

void QClientManager::clientRecvMessage(QConnection* sender, QByteArray buffer)
{
	int nMsgType;

	Q_READ_ONLY_DATA_STREAM(out, buffer);

	out>>nMsgType;

	QMessageParser::instance()->messageInstance(nMsgType)->parse(sender, buffer.right(buffer.size()-sizeof(int)), m_pController.get());
}

void QClientManager::clientStateChanged(QAbstractSocket::SocketState socketState)
{
	QClient* pClient=dynamic_cast<QClient*>(sender());
	if (pClient == NULL || !m_mapClient.contains(pClient))
	{
		return;
	}

	if (socketState == QAbstractSocket::ConnectedState)
	{
		m_mapClient[pClient].reconnect=0;
		m_mapClient[pClient].connected=true;

		emit connected(pClient);
	}
	else if (socketState == QAbstractSocket::UnconnectedState)
	{
		m_mapClient[pClient].connected=false;

		emit connectFailed(pClient);
	}
}

void QClientManager::clientSocketError(QAbstractSocket::SocketError socketError)
{
	Q_UNUSED(socketError);

// 	QClient* pClient=dynamic_cast<QClient*>(sender());
// 	if (pClient == NULL || !m_mapClient.contains(pClient))
// 	{
// 		return;
// 	}
// 
// 	m_mapClient[pClient].reconnect++;
// 	m_mapClient[pClient].connected=false;
// 	if (m_mapClient[pClient].reconnect < MAX_RECONNECT_COUNT)
// 	{
// 		connectToHost(pClient);
// 	}
// 	else
// 	{
// 		emit connectFailed(pClient);
// 	}
}
