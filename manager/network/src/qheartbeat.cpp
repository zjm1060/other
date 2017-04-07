#include <QtNetwork>

#include "qheartbeat.h"
#include "qservermanager.h"

#include "qlogger.h"

static const qint32 g_nBeatInterval=2000;

QHeartBeat::QHeartBeat(quint16 beatPort)
{
	m_nPort=beatPort;

	m_bConnecting=true;

    connect(&m_socketBeat, SIGNAL(readyRead()), this, SLOT(readBeatDatagram()));

	m_socketBeat.bind(QHostAddress::Any, m_nPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

	LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Start to receive heart beat packets."));

    connect(&m_timerBeat, SIGNAL(timeout()), this, SLOT(sendBeatDatagram()));
	m_timerBeat.setInterval(g_nBeatInterval);
}

QHeartBeat::~QHeartBeat()
{

}

void QHeartBeat::setServerPort(quint16 nPort)
{
    m_nPort= nPort;
}

void QHeartBeat::run()
{
    m_timerBeat.start();

	LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Start to send heart beat packets."));
}

void QHeartBeat::pause()
{
	m_timerBeat.stop();

	LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Pause to send heart beat packets."));
}

void QHeartBeat::sendBeatDatagram()
{
	const QServerManager::ServerInfo& serverInfo=QServerManager::instance()->localServer();
	if (serverInfo.count <= 0)
	{
		return;
	}

	QByteArray datagram;
	QDataStream out(&datagram, QIODevice::WriteOnly);
	out.setVersion(DATA_STREAM_VERSION);

	out<<serverInfo.hostName;

	QList<QString> keys=serverInfo.servers.keys();
	out<<keys.size();
	foreach (QString key, keys)
	{
		out<<key;

		QVector<QServerManager::Server> servers=serverInfo.servers[key];
		out<<servers.size();
		foreach (QServerManager::Server server, servers)
		{
			out<<server.addr<<server.netmask<<server.port;
		}
	}

	int nSendSize=m_socketBeat.writeDatagram(datagram, QHostAddress::Broadcast, m_nPort);
	if ((nSendSize != -1 && !m_bConnecting) || (nSendSize == -1 && m_bConnecting))
	{
		m_bConnecting=!m_bConnecting;

		QServerManager::instance()->checkLocalAddress();

		emit localNetStatusChanged(m_bConnecting);
	}
}

void QHeartBeat::readBeatDatagram()
{
    while (m_socketBeat.hasPendingDatagrams())
	{
        QByteArray datagram;
        datagram.resize(m_socketBeat.pendingDatagramSize());
        if (m_socketBeat.readDatagram(datagram.data(), datagram.size()) == -1)
            continue;

		QDataStream out(datagram);
		out.setVersion(DATA_STREAM_VERSION);

		QServerManager::ServerInfo serverInfo;
		serverInfo.count=0;

		int nPair;

		out>>serverInfo.hostName>>nPair;

		QString key;
		int nCount;

		QServerManager::Server	server;

		for (int i=0; i<nPair; i++)
		{
			if (out.atEnd())
			{
				return;
			}

			QVector<QServerManager::Server> servers;

			out>>key>>nCount;
			for (int index=0; index<nCount; index++)
			{
				if (out.atEnd())
				{
					return;
				}

				out>>server.addr>>server.netmask>>server.port;

				servers.append(server);
			}

			serverInfo.count+=nCount;
			serverInfo.servers.insert(key, servers);
		}

		QServerManager::instance()->addRemoteServer(serverInfo);
    }
}
