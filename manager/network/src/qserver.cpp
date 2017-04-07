#include "qserver.h"

#include "define.h"
#include "qmessageparser.h"

#include "qlogger.h"

QServer::QServer(const QHostAddress& serverAddr, quint16 port, QObject *parent)
	: QTcpServer(parent)
{
	m_serverAddr=serverAddr;
	m_nPort=port;
}

QServer::QServer(const QString& addr, quint16 port, QObject *parent)
: QTcpServer(parent)
{
	m_serverAddr=QHostAddress(addr);
	m_nPort=port;
}

QServer::~QServer()
{
}

void QServer::setServerAddress(const QHostAddress& serverAddr)
{
	m_serverAddr=serverAddr;
}

QHostAddress QServer::serverAddress() const
{
	return m_serverAddr;
}

void QServer::setServerPort(quint16 port)
{
	m_nPort=port;
}

quint16 QServer::serverPort() const
{
	return m_nPort;
}

bool QServer::onListen()
{
	onClose();

	bool bRtn=listen(m_serverAddr, m_nPort);

	if (!bRtn)
	{
		LOGPLUS_ERROR_STR(QLogger::root(), 
			qFormatString("%s:%d listening  failed!", m_serverAddr.toString().toLocal8Bit().data(), m_nPort));
	}
	else
	{
		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("%s:%d begining to listen.", m_serverAddr.toString().toLocal8Bit().data(), m_nPort));
	}

	return bRtn;
}

void QServer::onClose()
{
	if (isListening())
	{
		close();

		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("%s:%d server closed.", m_serverAddr.toString().toLocal8Bit().data(), m_nPort));
	}
}

void QServer::incomingConnection(int socketDescriptor)
{
	QTcpSocket* pSocket=new QTcpSocket();
	if (pSocket != NULL)
	{
		pSocket->setSocketDescriptor(socketDescriptor);

		LOGPLUS_DEBUG_STR(QLogger::root(), 
			qFormatString("%s:%d client coming.", pSocket->peerAddress().toString().toLocal8Bit().data(), pSocket->peerPort()));

		QConnection* pConnection=new QConnection(pSocket);
		connect(pConnection, SIGNAL(recvMessage(QConnection*, QByteArray)), this, SLOT(recvMessage(QConnection*, QByteArray)));
		connect(pConnection, SIGNAL(deleteConnection(QTcpSocket*, QHostAddress, quint16)), 
			this, SLOT(deteteConnection(QTcpSocket*, QHostAddress, quint16)));
	}
}

void QServer::recvMessage(QConnection* sender, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	LOGPLUS_DEBUG_STR(QLogger::root(),
		qFormatString("Receive: %s:%d %s", sender->peerAddress().toString().toLocal8Bit().data(), sender->peerPort(), buffer.data()));

	QDataStream out(buffer);
	out.setVersion(DATA_STREAM_VERSION);
	int nType;

	out>>nType;

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(nType);
	if (pMsg != NULL)
	{
		pMsg->parse(sender, buffer.right(buffer.size()-sizeof(int)));
	}
}

void QServer::deteteConnection(QTcpSocket* socket, QHostAddress addr, quint16 port)
{
	LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("%s:%d client quit!", addr.toString().toLocal8Bit().data(), port));

	socket->deleteLater();
}
