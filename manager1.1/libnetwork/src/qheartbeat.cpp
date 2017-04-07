#include <QtNetwork>

#include "qheartbeat.h"
#include "qservermanager.h"

#include "qlogger.h"
#include "qdeafactory.h"
#include "qglobalinfo.h"

static const int g_nBeatInterval=2000;
static const int g_nHeartBeatMaxBufferSize=1024*1024;

QHeartBeat::QHeartBeat(quint16 beatPort)
{
	setPort(beatPort);

	m_bConnecting=true;

    connect(&m_socketBeat, SIGNAL(readyRead()), this, SLOT(readBeatDatagram()));

	m_socketBeat.bind(QHostAddress::Any, m_nPort, QUdpSocket::DefaultForPlatform);

	logplusInfo(LIBNETWORK_LOGGER_STR, "Start to receive heart beat packets...");

    connect(&m_timerBeat, SIGNAL(timeout()), this, SLOT(sendBeatDatagram()));
	m_timerBeat.setInterval(g_nBeatInterval);
}

QHeartBeat::~QHeartBeat()
{

}

void QHeartBeat::setPort(quint16 nPort)
{
    m_nPort= nPort;
}

void QHeartBeat::run()
{
    m_timerBeat.start();

	logplusInfo(LIBNETWORK_LOGGER_STR, "Start to send heart beat packets...");
}

void QHeartBeat::pause()
{
	m_timerBeat.stop();

	logplusWarning(LIBNETWORK_LOGGER_STR, "Pause to send heart beat packets!");
}

void QHeartBeat::sendBeatDatagram()
{
	const ServerInfo& serverInfo=QServerManager::instance()->localServer();
	if (serverInfo.count <= 0)
	{
		return;
	}

	int nDeaType=QGlobalInfo::instance()->deaType(QGlobalInfo::HeartBeatsDea);

	QIDEAPtr ptr=QDEAFactory::instance()->dea(nDeaType);
	if (ptr == NULL)
	{
		logplusError(LIBNETWORK_LOGGER_STR, QString("Failed to encrypt message! Please to check your libdea library!"));

		return;
	}

	Q_WRITE_ONLY_DATA_STREAM(out, buffer);

	out<<PROTOCAL_HEADER<<PROTOCAL_HEAT_BEAT_VER;
	out<<serverInfo;

	QByteArray buf=ptr->encrypt(buffer);	//数据加密

	Q_WRITE_ONLY_DATA_STREAM(stream, datagram);
	stream<<nDeaType<<buf.size();
	stream.writeRawData(buf.data(), buf.size());

	int nSendSize=0;

// #ifndef Q_OS_AIX
// 	nSendSize=m_socketBeat.writeDatagram(datagram, QHostAddress::Broadcast, m_nPort);
// #else
	QMapIterator<QString, QVector<Server> > it(serverInfo.servers);
	while (it.hasNext())
	{
		it.next();

		QVector<Server> servers=it.value();
		for (int i=0; i<servers.size(); i++)
		{
			int nSize=m_socketBeat.writeDatagram(datagram, QHostAddress(QHostAddress(servers[i].addr).toIPv4Address()| 0xff), m_nPort);
			if (nSendSize != -1)
			{
				nSendSize=nSize;
			}
		}
	}
//#endif

	if ((nSendSize != -1 && !m_bConnecting) || (nSendSize == -1 && m_bConnecting))
	{
		m_bConnecting=!m_bConnecting;

		QServerManager::instance()->checkLocalAddress();

		emit localNetStatusChanged(m_bConnecting);
	}
}

void QHeartBeat::readBeatDatagram()
{
	int nEncryptType, nBlockSize;
	QHostAddress address;
	quint16 nPort;

    while (m_socketBeat.hasPendingDatagrams())
	{
        QByteArray datagram;
        datagram.resize(m_socketBeat.pendingDatagramSize());
        if (m_socketBeat.readDatagram(datagram.data(), datagram.size(), &address, &nPort) == -1)
            continue;

		Q_READ_ONLY_DATA_STREAM(stream, datagram);

		stream>>nEncryptType>>nBlockSize;

		if (nBlockSize > g_nHeartBeatMaxBufferSize)
		{
			logplusWarning(LIBNETWORK_LOGGER_STR, QString("Receive a too long encryption message from %1:%2!")
				.arg(address.toString()).arg(nPort));

			continue;
		}

		QIDEAPtr ptr=QDEAFactory::instance()->dea(nEncryptType);
		if (ptr == NULL)
		{
			logplusError(LIBNETWORK_LOGGER_STR, "Receive an invalid encryption message!");
			continue;
		}

		QByteArray buffer;
		buffer.resize(nBlockSize);
		stream.readRawData(buffer.data(), nBlockSize);
		buffer=ptr->decrypt(buffer);		//解密

		Q_READ_ONLY_DATA_STREAM(in, buffer);

		char *strHeader;
		in>>strHeader;
		if (strcmp(strHeader, PROTOCAL_HEADER) != 0)
		{
			SAFE_DELETE_ARRAY(strHeader);
			continue;
		}

		SAFE_DELETE_ARRAY(strHeader);

		quint32 nVersion;
		in>>nVersion;

		switch (nVersion)
		{
		case PROTOCAL_HEAT_BEAT_V1:
			{
				ServerInfo serverInfo;
				in>>serverInfo;

				QServerManager::instance()->addRemoteServer(serverInfo);
			}
			break;
		default:
			break;
		}
    }
}
