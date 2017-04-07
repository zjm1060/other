#include "qipcsocket.h"

#include "common.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qlogger.h"
#include "qviewcontroller.h"
#include "qglobalinfo.h"

QIpcSocket::QIpcSocket(QObject* parent)
	: QObject(parent)
{
	m_timer.setSingleShot(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_pView=new QViewController;

	m_nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/serverPort", 2300).toInt();
	m_pClient=new QClient(QHostAddress(QHostAddress::LocalHost), m_nPort);

	connect(m_pClient.get(), SIGNAL(quit()), this, SLOT(clientQuit()));
	connect(m_pClient.get(), SIGNAL(recvMessage(QConnection*, QByteArray)), this, SLOT(clientRecvMessage(QConnection*, QByteArray)));
	connect(m_pClient.get(), SIGNAL(clientStateChanged(QAbstractSocket::SocketState)), 
		this, SLOT(clientStateChanged(QAbstractSocket::SocketState)));
	connect(m_pClient.get(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError(QAbstractSocket::SocketError)));
}

QIpcSocket::~QIpcSocket()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);

	disconnect(this, SLOT(clientQuit()));
}

QSharedIpcSocketPtr	QIpcSocket::instance()
{
	static QSharedIpcSocketPtr pInstance(new QIpcSocket);

	return pInstance;
}

QSharedClientPtr QIpcSocket::clientSocket()
{
	return m_pClient;
}

QString QIpcSocket::address() const
{
	return m_pClient != NULL? m_pClient->serverAddr().toString():QHostAddress(QHostAddress::LocalHost).toString();
}

quint16 QIpcSocket::port() const
{
	return m_pClient != NULL? m_pClient->port():m_nPort;
}

void QIpcSocket::connectToHost()
{
	if (m_pClient != NULL)
	{
		logplusInfo(HSHOW_LOGGER_STR, tr("Connecting to the service process..."));
		m_pClient->connectToHost();
	}
}

void QIpcSocket::clientQuit()
{
	logplusInfo(HSHOW_LOGGER_STR, tr("Disconnected from the service process!"));

	QGlobalInfo::instance()->setLogin(false);

	reconnect();
}

void QIpcSocket::clientRecvMessage(QConnection* sender, QByteArray buffer)
{
	int nMsgType;

	Q_READ_ONLY_DATA_STREAM(out, buffer);

	out>>nMsgType;

	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(nMsgType);
	if (pInstance != NULL)
	{
		pInstance->parse(sender, buffer.right(buffer.size()-sizeof(int)), m_pView);
	}
}

void QIpcSocket::clientStateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == QAbstractSocket::ConnectedState)
	{
		logplusInfo(HSHOW_LOGGER_STR, tr("Successfully connected to the service process."));

		QGlobalInfo::instance()->setLogin(true);

		queryLocalServer();
	}
}

void QIpcSocket::clientError(QAbstractSocket::SocketError socketError)
{
	switch (socketError)
	{
	 case QAbstractSocket::RemoteHostClosedError:
		 logplusInfo(HSHOW_LOGGER_STR, tr("The service process stopped running!Make sure the service process is running."));
		 break;
	 case QAbstractSocket::HostNotFoundError:
		 logplusInfo(HSHOW_LOGGER_STR, tr("The service process was not running.Please check your network settings."));
		 break;
	 case QAbstractSocket::ConnectionRefusedError:
		 logplusInfo(HSHOW_LOGGER_STR, tr("Connected the service process was refused.Make sure the service process is running."));
		 break;
	 default:
		 logplusInfo(HSHOW_LOGGER_STR, tr("Connected the service process was failed.The following error occurred: %1.").arg(m_pClient->errorString()));
	}

	reconnect();
}

void QIpcSocket::timeout()
{
	connectToHost();
}

void QIpcSocket::reconnect()
{
	if (!m_timer.isActive())
	{
		int nInterval=3000;

		m_timer.start(nInterval);
		logplusInfo(HSHOW_LOGGER_STR, tr("Reconnect to the service process after %1 seconds.").arg(nInterval/1000));
	}
}

void QIpcSocket::close()
{
	if (m_pClient != NULL)
	{
		m_pClient->closeSocket();
	}
}

void QIpcSocket::queryLocalServer()
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_LOCAL_SERVER);
	if (pInstance != NULL)
	{
		QByteArray buffer=pInstance->query(QueryLocalServer, m_nUsrID);

		if (m_pClient != NULL)
		{
			m_pClient->sendMessage(buffer, MSG_LOCAL_SERVER);
		}
	}
}

void QIpcSocket::queryExecCommand(const QStringList& arguments)
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		stream<<arguments;

		buffer=pInstance->query(QueryCreateProcess, -1, buffer);

		if (m_pClient != NULL)
		{
			m_pClient->sendMessage(buffer, MSG_SYSTEM_PROCESS);
		}
	}
}

bool QIpcSocket::sendMessage(const QByteArray& buffer, int type)
{
	bool bRet=false;

	if (m_pClient != NULL)
	{
		bRet=m_pClient->sendMessage(buffer, type);
	}

	return bRet;
}

void QIpcSocket::recvLocalServer(int flag, const ServerInfo& info)
{
	if (flag == QuerySucceed)
	{
		QGlobalInfo::instance()->setLocal(info);
	}
}
