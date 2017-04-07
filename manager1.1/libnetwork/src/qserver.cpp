#include "qserver.h"

#include "define.h"
#include "qmessageparser.h"
#include "qconnectionmanager.h"

#include "qlogger.h"
#include "qddbwarehouse.h"
#include "qglobalinfo.h"
#include "qservermanager.h"

QServer::QServer(const QHostAddress& serverAddr, quint16 port, QObject *parent)
	: QTcpServer(parent)
	,m_serverAddr(serverAddr)
	,m_nPort(port)
	,m_pView(NULL)
{
	m_pView=new QControlledView(this);
}

QServer::QServer(const QString& addr, quint16 port, QObject *parent)
: QTcpServer(parent)
,m_serverAddr(addr)
,m_nPort(port)
,m_pView(NULL)
{
	m_pView=new QControlledView(this);
}

QServer::~QServer()
{
	SAFE_DELETE_CLASS(m_pView);
}

void QServer::setAddress(const QHostAddress& serverAddr)
{
	m_serverAddr=serverAddr;
}

const QHostAddress& QServer::address() const
{
	return m_serverAddr;
}

void QServer::setPort(quint16 port)
{
	m_nPort=port;
}

quint16 QServer::port() const
{
	return m_nPort;
}

bool QServer::onListen()
{
	onClose();

	bool bRtn=listen(m_serverAddr, m_nPort);

	if (!bRtn)
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("%1:%2 listening  failed! error=%3").arg(m_serverAddr.toString()).arg(m_nPort).arg(errorString()));
	}
	else
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("%1:%2 begining to listen.").arg(m_serverAddr.toString()).arg(m_nPort));
	}

	return bRtn;
}

void QServer::onClose()
{
	if (isListening())
	{
		close();

		logplusInfo(LIBNETWORK_LOGGER_STR, QString("%1:%2 server closed.").arg(m_serverAddr.toString()).arg(m_nPort));
	}
}

void QServer::incomingConnection(int socketDescriptor)
{
	QTcpSocket* pSocket=new QTcpSocket();
	if (pSocket != NULL)
	{
		pSocket->setSocketDescriptor(socketDescriptor);

		logplusInfo(LIBNETWORK_LOGGER_STR, QString("%1:%2 client coming.").arg(pSocket->peerAddress().toString()).arg(pSocket->peerPort()));

		QConnection* pConnection=new QConnection(pSocket);

		connect(pConnection, SIGNAL(recvMessage(QConnection*, QByteArray)), this, SLOT(recvMessage(QConnection*, QByteArray)));
		connect(pConnection, SIGNAL(deleteConnection(QConnection*)), this, SLOT(deteteConnection(QConnection*)));

		Server server;
		server.addr=pSocket->peerAddress().toString();
		server.netmask=QHostAddress("255.255.255.0").toIPv4Address();
		server.port=pSocket->peerPort();

		bool bHighRate=QServerManager::canConnected(QGlobalInfo::instance()->local(), server);

		QConnectionManager::instance()->insert(this, pConnection, pConnection, bHighRate);
	}
}

void QServer::recvMessage(QConnection* sender, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	logplusDebug(LIBNETWORK_LOGGER_STR, QString("Receive: %1:%2 length=%3")
		.arg(sender->peerAddress().toString()).arg(sender->peerPort()).arg(buffer.size()));

	Q_READ_ONLY_DATA_STREAM(out, buffer)

	int nType;

	out>>nType;

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(nType);
	if (pMsg != NULL)
	{
		pMsg->parse(sender, buffer.right(buffer.size()-sizeof(int)), m_pView);
	}
}

void QServer::deteteConnection(QConnection* connection)
{
	logplusInfo(LIBNETWORK_LOGGER_STR, QString("%1:%2 client quit!").arg(connection->peerAddress().toString()).arg(connection->peerPort()));

	QTcpSocket* pSocket=connection->connectionSocket();
	if (pSocket != NULL)
	{
		pSocket->deleteLater();
	}

	QConnectionManager::instance()->remove(this, connection);
	QDdbWarehouse::instance()->remove(connection);
}

void QServer::informStatusChanged(quint64 sign)
{
	if (QConnectionManager::instance()->isEmpty(this))
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_STATUS_CHANGED);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<sign;
		
		buffer=pMsg->query(InformStatusChanged, -1, buffer);

		QList<QConnectionManager::ConnectionParam> connections=QConnectionManager::instance()->connections(this);
		foreach (QConnectionManager::ConnectionParam pConnection, connections)
		{
			if (pConnection.connection != NULL)
			{
				((QConnection*)pConnection.connection.get())->sendMessage(buffer, MSG_STATUS_CHANGED);
			}
		}
	}
}
