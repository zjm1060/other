#include "qclient.h"

#include "qlogger.h"
#include "qservermanager.h"
#include "qconnectionmanager.h"
#include "qddbwarehouse.h"
#include "qglobalinfo.h"

QMap<QString, Server> QClient::m_sMapFirstServer;

QClient::QClient(const QHostAddress& serverAddr, quint16 port, QObject *parent)
	: QTcpSocket(parent)
	,m_pConnection(NULL)
	,m_bClosed(false)
{
	m_serverAddr=serverAddr;
	m_nPort=port;

	m_nIndex=0;
	m_server.count=0;

	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
}

QClient::QClient(const ServerInfo& serverInfo, QObject *parent)
: QTcpSocket(parent)
,m_pConnection(NULL)
,m_nIndex(0)
,m_bClosed(false)
{
	Q_ASSERT(serverInfo.count > 0);

	m_server=serverInfo;

	m_vecAddrs.clear();

	QList<Server> lstCheckedAddrs;		//有相同网段的ip地址
	QList<Server> lstUncheckedAddrs;	//没有相同网段的ip地址

	bool bFindFirst=false;

	foreach (QVector<Server> servers, serverInfo.servers)
	{
		foreach (Server server, servers)
		{
			if (m_sMapFirstServer.contains(serverInfo.hostName) && m_sMapFirstServer[serverInfo.hostName].addr == server.addr)
			{
				lstCheckedAddrs.prepend(server);

				bFindFirst=true;

				continue;
			}

			if (QServerManager::canConnected(QGlobalInfo::instance()->local(), server))
			{
				QListIterator<Server> it(lstCheckedAddrs);
				int nPos=0;

				if (bFindFirst)
				{
					nPos++;
					it.next();
				}

				while (it.hasNext())
				{
					Server srv=it.next();
					if (QHostAddress(srv.addr).toIPv4Address() > QHostAddress(server.addr).toIPv4Address())
					{
						break;
					}

					nPos++;
				}

				lstCheckedAddrs.insert(nPos, server);
			}
			else
			{
				lstUncheckedAddrs.append(server);
			}
		}
	}
	
	m_vecAddrs+=lstCheckedAddrs.toVector();
	m_vecAddrs+=lstUncheckedAddrs.toVector();

	m_serverAddr=QHostAddress(m_vecAddrs[m_nIndex].addr);
	m_nPort=m_vecAddrs[m_nIndex].port;

	connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
}

QClient::~QClient()
{
//	close();
}

void QClient::connectToHost()
{
	closeSocket();

	m_bClosed=false;

	logplusInfo(LIBNETWORK_LOGGER_STR, QString("Connecting to server %1:%2...").arg(m_serverAddr.toString()).arg(m_nPort));
	QTcpSocket::connectToHost(m_serverAddr, m_nPort);
}

void QClient::socketStateChanged(QAbstractSocket::SocketState state)
{
	if (state == QAbstractSocket::ConnectedState)
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("Connect to server %1:%2 succeed!").arg(m_serverAddr.toString()).arg(m_nPort));

		if (!m_server.hostName.isEmpty())
		{
			Server server;
			server.addr=m_serverAddr.toString();
			server.port=m_nPort;

			m_sMapFirstServer.insert(m_server.hostName, server);
		}
		
		m_pConnection=new QConnection(this);

		connect(m_pConnection, SIGNAL(recvMessage(QConnection*, QByteArray)), this, SIGNAL(recvMessage(QConnection*, QByteArray)));
		connect(m_pConnection, SIGNAL(deleteConnection(QConnection*)), this, SLOT(deleteConnection(QConnection*)));

		Server s;
		if (!m_vecAddrs.isEmpty())
		{
			s=m_vecAddrs[m_nIndex];
		}
		else
		{
			s.addr=m_serverAddr.toString();
			s.netmask=QHostAddress("255.255.255.0").toIPv4Address();
			s.port=m_nPort;
		}
		
		bool bHighRate=QServerManager::canConnected(QGlobalInfo::instance()->local(), s);

		QConnectionManager::instance()->insert(this, m_pConnection, m_pConnection, bHighRate);

		m_nIndex=0;
	}
	else if (state == QAbstractSocket::UnconnectedState)
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("Unconnected to server %1:%2!").arg(m_serverAddr.toString()).arg(m_nPort));

		if (!m_bClosed)
		{
			if (!m_vecAddrs.isEmpty() && (++m_nIndex) < m_vecAddrs.size())
			{
				m_serverAddr=QHostAddress(m_vecAddrs[m_nIndex].addr);
				m_nPort=m_vecAddrs[m_nIndex].port;

				connectToHost();
			}
		}
	}

	if (state != QAbstractSocket::UnconnectedState || (state == QAbstractSocket::UnconnectedState && m_nIndex >= m_vecAddrs.size()))
	{
		if (state == QAbstractSocket::UnconnectedState && m_nIndex >= m_vecAddrs.size())
		{
			m_nIndex=0;
			if (!m_vecAddrs.isEmpty())
			{
				m_serverAddr=QHostAddress(m_vecAddrs[m_nIndex].addr);
				m_nPort=m_vecAddrs[m_nIndex].port;
			}
		}

		emit clientStateChanged(state);
	}
}

void QClient::deleteConnection(QConnection* connection)
{
	logplusInfo(LIBNETWORK_LOGGER_STR, QString("Disconnected to server %1:%2!").arg(m_serverAddr.toString()).arg(m_nPort));

	m_pConnection=NULL;
	QConnectionManager::instance()->remove(this, connection);
	QDdbWarehouse::instance()->remove(connection);

	emit quit();
}

bool QClient::sendMessage(const QByteArray& buffer, int msgType)
{
	if (m_pConnection == NULL)
	{
		return false;
	}

	QObjectPtr ptr=QConnectionManager::instance()->connection(m_pConnection);
	if (ptr == NULL)
	{
		return false;
	}

	if (state() != QAbstractSocket::ConnectedState)
	{
		logplusWarning(LIBNETWORK_LOGGER_STR, QString("Send message failed! Connection has not been established!"));

		return false;
	}

	bool bRtn=((QConnection*)ptr.get())->sendMessage(buffer, msgType);

	if (bRtn)
	{
		logplusDebug(LIBNETWORK_LOGGER_STR, 
			QString("Client %1:%2 send message: %3 bytes").arg(localAddress().toString()).arg(localPort()).arg(buffer.size()));
	}

	return bRtn;
}

void QClient::closeSocket()
{
	if (state() == QAbstractSocket::ConnectedState)
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("%1:%2 Client quit!").arg(localAddress().toString()).arg(localPort()));

		m_bClosed=true;

		abort();
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

QConnection* QClient::connection()
{
	return m_pConnection;
}
