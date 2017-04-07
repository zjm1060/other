#include "qmulticastsender.h"

#include "qdeafactory.h"
#include "qlogger.h"
#include "qutildef.h"
#include "qglobalinfo.h"
#include "qservermanager.h"

QMulticastSender::QMulticastSender(const QString& uid, quint16 port, QObject* parent/* =0 */)
:QObject(parent)
,m_uid(uid)
,m_nPort(port)
{
	
}

QMulticastSender::~QMulticastSender()
{
	
}

void QMulticastSender::sendDatagram(int msgType, const QString& uid, const QByteArray& datagram)
{
	const ServerInfo& serverInfo=QServerManager::instance()->localServer();
	if (serverInfo.count <= 0)
	{
		return;
	}

	QIDEAPtr ptr=QDEAFactory::instance()->dea(DES);
	if (ptr == NULL)
	{
		logplusError(LIBNETWORK_LOGGER_STR, QString("Failed to encrypt message! Please to check your libdea library!"));

		return;
	}

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<PROTOCAL_HEADER<<m_uid<<uid<<datagram.size()+(int)sizeof(msgType)<<msgType;
	in.writeRawData(datagram.data(), datagram.size());

	QByteArray buf=ptr->encrypt(buffer);	//Êý¾Ý¼ÓÃÜ
	
	Q_WRITE_ONLY_DATA_STREAM(stream, data);
	stream<<DES<<buf.size();
	stream.writeRawData(buf.data(), buf.size());

	QMapIterator<QString, QVector<Server> > it(serverInfo.servers);
	while (it.hasNext())
	{
		it.next();

		QVector<Server> servers=it.value();
		for (int i=0; i<servers.size(); i++)
		{
			m_socket.writeDatagram(data, QHostAddress(QHostAddress(servers[i].addr).toIPv4Address()| 0xff), m_nPort);
		}
	}
}
