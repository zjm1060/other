#include "qmulticastreceiver.h"

#include "common.h"
#include "qlogger.h"
#include "qmulticastmsgparser.h"
#include "qdeafactory.h"

#ifdef Q_OS_WIN
#include <WS2tcpip.h>
#endif

static const int g_nMulticastMaxBufferSize=1024*1024*10;

QMulticastReceiver::QMulticastReceiver(const QString& uid, quint16 port, QObject* view, QObject* parent/* =0 */)
:QObject(parent)
,m_uid(uid)
,m_nPort(port)
,m_pView(view)
{
	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(readDatagram()));

	m_socket.bind(QHostAddress::Any, m_nPort, QUdpSocket::DefaultForPlatform);

	logplusInfo(LIBNETWORK_LOGGER_STR, "Start to receive privileges informations packets...");
}

QMulticastReceiver::~QMulticastReceiver()
{
	SAFE_DELETE_CLASS(m_pView);
}

void QMulticastReceiver::readDatagram()
{
	int nSize;
	int nEncryptType, nBlockSize;
	QHostAddress address;
	quint16 nPort;

	while (m_socket.hasPendingDatagrams())
	{
		m_buffer.resize((int)m_socket.pendingDatagramSize());
		m_socket.readDatagram(m_buffer.data(), m_buffer.size(), &address, &nPort);

		Q_READ_ONLY_DATA_STREAM(stream, m_buffer);

		stream>>nEncryptType>>nBlockSize;

		if (nBlockSize > g_nMulticastMaxBufferSize)
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

		QByteArray datagram;
		datagram.resize(nBlockSize);
		stream.readRawData(datagram.data(), nBlockSize);
		datagram=ptr->decrypt(datagram);		//解密

		if (datagram.size() < (int)(strlen(PROTOCAL_HEADER)+2*sizeof(int)))
		{
			logplusWarning(LIBNETWORK_LOGGER_STR, "Receive a too short multicast message!");
			
			continue;
		}

		Q_READ_ONLY_DATA_STREAM(out, datagram);

		char* strHeader;
		out>>strHeader;
		if (strHeader == NULL)
		{
			logplusWarning(LIBNETWORK_LOGGER_STR, "Receive an invalid multicast message!");

			continue;
		}

		if (strcmp(strHeader, PROTOCAL_HEADER) != 0)
		{
			logplusWarning(LIBNETWORK_LOGGER_STR, "Receive an invalid multicast message!");

			SAFE_DELETE_ARRAY(strHeader);
			continue;
		}

		SAFE_DELETE_ARRAY(strHeader);
		
		QString strFrom, strTo;
		out>>strFrom>>strTo;

		if (!strTo.isEmpty() && strTo != m_uid)
		{
			continue;		//不是发往本地的消息，退出
		}

		//解析消息
		int nType;

		out>>nSize>>nType;

		nSize-=sizeof(nType);

		if (nSize < 0)
		{
			continue;
		}

		QByteArray buffer;
		buffer.resize(nSize);
		int nLen=out.readRawData(buffer.data(), nSize);
		if (nLen == -1 || buffer.size() != nSize)
		{
			continue;
		}

		QIMulticastMsgPtr pMsg=QMulticastMsgParser::instance()->messageInstance(nType);
		if (pMsg != NULL)
		{
			pMsg->parse(strFrom, buffer, m_pView);
		}
	}
}
