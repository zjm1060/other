#include "qclientmanager.h"

#include "qmessageparser.h"

QClientManager::QClientManager(QObject* controller, QObject *parent)
	: QObject(parent)
{
	m_pController=controller;
}

QClientManager::~QClientManager()
{
	removeAll();
}

QClient* QClientManager::addClient(const QServerManager::ServerInfo& info)
{
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

		SAFE_DELETE_CLASS(client);
	}
}

void QClientManager::removeAll()
{
	QClient* pClient;

	QMutableMapIterator<QClient*, ClientInfo> it(m_mapClient);
	while (it.hasNext())
	{
		it.next();

		pClient=it.key();
		it.remove();

		SAFE_DELETE_CLASS(pClient);
	}
}

void QClientManager::connectToHost(QClient* client)
{
	if (client != NULL && m_mapClient.contains(client))
	{
		if (!m_mapClient[client].connected)
		{
			client->connectToHost();
		}
		else
		{
			emit connected(client);
		}
	}
}

void QClientManager::reconnect(QClient* client)
{
	if (client != NULL && m_mapClient.contains(client))
	{
		m_mapClient[client].reconnect=0;
		client->connectToHost();
	}
}

bool QClientManager::isConnected(QClient* client)
{
	if (!m_mapClient.contains(client))
	{
		return false;
	}

	return m_mapClient[client].connected;
}

void QClientManager::clientQuit()
{
	QClient* pClient=dynamic_cast<QClient*>(sender());
	if (pClient != NULL && m_mapClient.contains(pClient))
	{
		m_mapClient[pClient].connected=false;
	}
}

void QClientManager::clientRecvMessage(QConnection* sender, QByteArray buffer)
{
	int nMsgType;

	QDataStream out(buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	QMessageParser::instance()->messageInstance(nMsgType)->parse(sender, buffer.right(buffer.size()-sizeof(int)), m_pController);
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
}

void QClientManager::clientSocketError(QAbstractSocket::SocketError socketError)
{
	Q_UNUSED(socketError);

	QClient* pClient=dynamic_cast<QClient*>(sender());
	if (pClient == NULL || !m_mapClient.contains(pClient))
	{
		return;
	}

	m_mapClient[pClient].reconnect++;
	if (m_mapClient[pClient].reconnect < MAX_RECONNECT_COUNT)
	{
		connectToHost(pClient);
	}
	else
	{
		emit disconnected(pClient);
	}
}
