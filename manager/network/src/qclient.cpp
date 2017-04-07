#include "qclient.h"

#include "qlogger.h"

QClient::QClient(const QHostAddress& serverAddr, quint16 port, QObject *parent)
	: QTcpSocket(parent)
{
	m_serverAddr=serverAddr;
	m_nPort=port;

	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
}

QClient::QClient(const QServerManager::ServerInfo& serverInfo, QObject *parent)
: QTcpSocket(parent)
{
	bool bFind=false;
	if (serverInfo.count > 0)
	{
		foreach (QVector<QServerManager::Server> servers, serverInfo.servers)
		{
			foreach (QServerManager::Server server, servers)
			{
				if (QServerManager::instance()->canConnected(server))
				{
					m_serverAddr=QHostAddress(server.addr);
					m_nPort=server.port;

					bFind=true;
					break;
				}
			}

			if (bFind)
			{
				break;
			}
		}
	}
	
	if (!bFind)
	{
		if (serverInfo.count > 0)
		{
			foreach (QVector<QServerManager::Server> servers, serverInfo.servers)
			{
				foreach (QServerManager::Server server, servers)
				{
					m_serverAddr=QHostAddress(server.addr);
					m_nPort=server.port;

					break;
				}
			}
		}
		else
		{
			m_serverAddr=QHostAddress();
			m_nPort=0;
		}
	}

	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
}

QClient::~QClient()
{
	close();
}

void QClient::connectToHost()
{
	close();

	QTcpSocket::connectToHost(m_serverAddr, m_nPort);

	LOGPLUS_DEBUG_STR(QLogger::root(), 
		qFormatString("Connecting to server %s:%d...", m_serverAddr.toString().toLocal8Bit().data(), m_nPort));
}

void QClient::socketStateChanged(QAbstractSocket::SocketState state)
{
	if (state == QAbstractSocket::ConnectedState)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("Connect to server %s:%d succeed!", m_serverAddr.toString().toLocal8Bit().data(), m_nPort));

		m_pConnection=new QConnection(this);

		connect(m_pConnection, SIGNAL(recvMessage(QConnection*, QByteArray)), 
			this, SIGNAL(recvMessage(QConnection*, QByteArray)));

		connect(m_pConnection, SIGNAL(deleteConnection(QTcpSocket*, QHostAddress, quint16)), 
			this, SLOT(deleteConnection(QTcpSocket*, QHostAddress, quint16)));

		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("%s:%d Begining to send messages...", m_serverAddr.toString().toLocal8Bit().data(), m_nPort));
	}

	emit clientStateChanged(state);
}

void QClient::deleteConnection(QTcpSocket* socket, QHostAddress addr, quint16 port)
{
	Q_UNUSED(socket);

	LOGPLUS_DEBUG_STR(QLogger::root(), 
		qFormatString("Disconnected to %s:%d Server!", addr.toString().toLocal8Bit().data(), port));

	emit quit();
}

bool QClient::sendMessage(const QByteArray& buffer, int msgType)
{
	if (m_pConnection == NULL)
	{
		return false;
	}

	if (state() != QAbstractSocket::ConnectedState)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("Send message failed! Connection has not been established!"));

		return false;
	}

	bool bRtn=m_pConnection->sendMessage(buffer, msgType);

	if (bRtn)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("%s:%d Client send message: %d bytes", localAddress().toString().toLocal8Bit().data(), localPort(),
			buffer.size()));
	}
	else
	{
		LOGPLUS_ERROR_STR(QLogger::root(), 
			qFormatString("%s:%d Client send message failed!", localAddress().toString().toLocal8Bit().data(), localPort()));
	}

	return bRtn;
}

void QClient::close()
{
	if (state() != QAbstractSocket::UnconnectedState)
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("%s:%d Client quit!", localAddress().toString().toLocal8Bit().data(), localPort()));

		QTcpSocket::close();
	}
}

const QHostAddress& QClient::serverAddr() const
{
	return m_serverAddr;
}

quint16 QClient::port() const
{
	return m_nPort;
}

void QClient::setServerAddr(const QString& addr)
{
	Q_ASSERT(!addr.isEmpty());

	if (m_serverAddr.toString() != addr)
	{
		m_serverAddr=QHostAddress(addr);
	}
}

void QClient::setPort(quint16 port)
{
	m_nPort=port;
}
