#include "qprocesssocket.h"

#include "common.h"
#include "qinfomanager.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qlogger.h"

QProcessSocket::QProcessSocket(QObject* parent)
	: QObject(parent)
	,m_pView(NULL)
	,m_nUsrID(QReceiverParser::AllUser)
{
	m_timer.setSingleShot(true);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	quint32 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/serverPort", 2300).toInt();
	m_pClient=new QClient(QHostAddress(QHostAddress::LocalHost), nPort);

	connect(m_pClient.get(), SIGNAL(quit()), this, SLOT(clientQuit()));
	connect(m_pClient.get(), SIGNAL(recvMessage(QConnection*, QByteArray)), this, SLOT(clientRecvMessage(QConnection*, QByteArray)));
	connect(m_pClient.get(), SIGNAL(clientStateChanged(QAbstractSocket::SocketState)), 
		this, SLOT(clientStateChanged(QAbstractSocket::SocketState)));
	connect(m_pClient.get(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientError(QAbstractSocket::SocketError)));
}

QProcessSocket::~QProcessSocket()
{
	disconnect(this, SLOT(clientQuit()));
}

QSharedProcessSocketPtr	QProcessSocket::instance()
{
	static QSharedProcessSocketPtr pInstance(new QProcessSocket);

	return pInstance;
}

QString QProcessSocket::address() const
{
	return m_pClient != NULL? m_pClient->serverAddr().toString():QHostAddress(QHostAddress::LocalHost).toString();
}

quint16 QProcessSocket::port() const
{
	return m_pClient != NULL? m_pClient->port():(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/serverPort", 2300).toInt();
}

void QProcessSocket::connectToHost()
{
	if (m_pClient != NULL)
	{
		QInfoManager::instance()->append(QInfoManager::Information, tr("Connecting to the service process..."));
		m_pClient->connectToHost();
	}
}

void QProcessSocket::clientQuit()
{
	QInfoManager::instance()->append(QInfoManager::Error, tr("Disconnected from the service process!"));

	QMetaObject::invokeMethod(m_pView, "recvLocalServerQuit", Q_ARG(quint32, m_nUsrID));

	reconnect();
}

void QProcessSocket::clientRecvMessage(QConnection* sender, QByteArray buffer)
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

void QProcessSocket::clientStateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == QAbstractSocket::ConnectedState)
	{
		QInfoManager::instance()->append(QInfoManager::Information, tr("Successfully connected to the service process."));

		queryLocalServer();
	}
}

void QProcessSocket::clientError(QAbstractSocket::SocketError socketError)
{
	switch (socketError)
	{
	 case QAbstractSocket::RemoteHostClosedError:
		 QInfoManager::instance()->append(QInfoManager::Error, tr("The service process stopped running!Make sure the service process is running."));
		 break;
	 case QAbstractSocket::HostNotFoundError:
		 QInfoManager::instance()->append(QInfoManager::Error, tr("The service process was not running.Please check your network settings."));
		 break;
	 case QAbstractSocket::ConnectionRefusedError:
		 QInfoManager::instance()->append(QInfoManager::Error, tr("Connected the service process was refused.Make sure the service process is running."));
		 break;
	 default:
		 QInfoManager::instance()->append(QInfoManager::Error, tr("Connected the service process was failed.The following error occurred: %1.").arg(m_pClient->errorString()));
	}

	reconnect();
}

void QProcessSocket::timeout()
{
	connectToHost();
}

void QProcessSocket::reconnect()
{
	if (!m_timer.isActive())
	{
		int nInterval=3000;

		m_timer.start(nInterval);
		QInfoManager::instance()->append(QInfoManager::Information, tr("Reconnect to the service process after %1 seconds.").arg(nInterval/1000));
	}
}

void QProcessSocket::close()
{
	if (m_pClient != NULL)
	{
		m_pClient->closeSocket();
	}
}

void QProcessSocket::queryLocalServer()
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

void QProcessSocket::queryRemoteServerList()
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_REMOTE_SERVER);
	if (pInstance != NULL)
	{
		QByteArray buffer=pInstance->query(QueryRemoteServerList, m_nUsrID);

		if (m_pClient != NULL)
		{
			m_pClient->sendMessage(buffer, MSG_REMOTE_SERVER);
		}
	}
}

void QProcessSocket::queryTopology()
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_TOPOLOGY);
	if (pInstance != NULL)
	{
		QByteArray buffer=pInstance->query(QueryTopology, m_nUsrID);

		if (m_pClient != NULL)
		{
			m_pClient->sendMessage(buffer, MSG_TOPOLOGY);
		}
	}
}

void QProcessSocket::queryExecCommand(const QStringList& arguments)
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

bool QProcessSocket::sendMessage(const QByteArray& buffer, int type)
{
	bool bRet=false;

	if (m_pClient != NULL)
	{
		bRet=m_pClient->sendMessage(buffer, type);
	}

	return bRet;
}

void QProcessSocket::setView(QObject* view)
{
	m_pView=view;
}

void QProcessSocket::setUser(quint32 id)
{
	m_nUsrID=id;
}
