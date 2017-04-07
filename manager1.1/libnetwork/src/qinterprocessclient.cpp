#include "qinterprocessclient.h"

#include <QCoreApplication>
#include "qlogger.h"

QInterProcessClient::QInterProcessClient(const QString& name, QObject *parent/* =0 */)
	: QLocalSocket(parent)
	,m_strName(name)
	,m_bClosed(true)
{
	connect(this, SIGNAL(connected()), this, SLOT(connected()));
	connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(this, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)), this, SLOT(stateChanged(QLocalSocket::LocalSocketState)));
}

QInterProcessClient::~QInterProcessClient()
{
//	close();
}

void QInterProcessClient::connectToHost()
{
	closeSocket();

	logplusInfo(LIBNETWORK_LOGGER_STR, QString("Connecting to InterProcess named=%1...").arg(m_strName));
	QLocalSocket::connectToServer(m_strName);
}

void QInterProcessClient::connected()
{
	m_bClosed=false;

	logplusInfo(LIBNETWORK_LOGGER_STR, QString("Connected to InterProcess named=%1...").arg(m_strName));

	emit clientConnected();
}

void QInterProcessClient::stateChanged(QLocalSocket::LocalSocketState socketState)
{
	if (socketState == QLocalSocket::ClosingState)
	{
		m_bClosed=true;

		emit clientQuit();
	}
}

void QInterProcessClient::disconnected()
{
	logplusInfo(LIBNETWORK_LOGGER_STR, QString("Disconnected to InterProcess named=%1! Quit...").arg(m_strName));
}

bool QInterProcessClient::sendMessage(const QByteArray& buffer, int msgType)
{
	if (m_bClosed)
	{
		return false;
	}

	Q_WRITE_ONLY_DATA_STREAM(in, datagram);
	in<<(int)sizeof(int)+buffer.size()<<msgType;
	in.writeRawData(buffer.data(), buffer.size());

	bool bRtn=(write(datagram) == datagram.size());

	if (!bRtn)
	{
		logplusWarning(LIBNETWORK_LOGGER_STR, QString("Send interprocess message failed!"));
	}

	return bRtn;
}

void QInterProcessClient::closeSocket()
{
	if (state() == QLocalSocket::ConnectedState)
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("Unconnected to interprocess! name=%1").arg(m_strName));

		m_bClosed=true;
	}
}

