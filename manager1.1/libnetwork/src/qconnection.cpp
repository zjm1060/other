#include "qconnection.h"

#include <QMutexLocker>

#include "qlogger.h"
#include "qimessage.h"
#include "qprocesssender.h"
#include "qprocessorsender.h"
#include "qmemorysender.h"
#include "qdeafactory.h"
#include "qglobalinfo.h"

static const int MaxBufferSize= 1024*1024*200;		//通信消息缓冲区最大值

QConnection::QConnection(QTcpSocket* socket, QObject *parent)
	: QObject(parent)
	,m_pSocket(socket)
	,m_eStatus(WaitingForMessage)
	,m_nNextBlockSize(0)
	,m_bLock(false)
{
	if (m_pSocket != NULL)
	{
		connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	}
	else
	{
		m_eStatus=Quit;
	}
}

QConnection::~QConnection()
{
}

QTcpSocket*	QConnection::connectionSocket()
{
	return m_pSocket;
}

void QConnection::disconnected()
{
	if (m_eStatus == Quit)
	{
		return;
	}

	m_eStatus=Quit;

	if (m_pSocket != NULL)
	{
		m_pSocket->abort();

		emit deleteConnection(this);
	}
}

bool QConnection::isQuit()
{
	return (m_eStatus==Quit? true:false);
}

void QConnection::readyRead()
{
	if (m_eStatus==Quit || m_bLock)
	{
		return;
	}

	m_bLock=true;

	Q_READ_ONLY_DATA_STREAM(out, m_pSocket);

	while (m_pSocket->bytesAvailable() > 0)
	{
		if (m_eStatus == WaitingForMessage)
		{
			if (m_pSocket->bytesAvailable() < 2*sizeof(int))
			{
				return;
			}

			out>>m_nEncryptType>>m_nNextBlockSize;

			if (m_nNextBlockSize > MaxBufferSize)
			{
				logplusWarning(LIBNETWORK_LOGGER_STR, QString("Receive a too long encryption message from %1:%2!")
					.arg(m_pSocket->peerAddress().toString()).arg(m_pSocket->peerPort()));

				m_eStatus=Quit;
				m_pSocket->abort();

				m_bLock=false;
				return;
			}

			m_buffer.clear();

			m_eStatus=ReadingMessage;
		}

		if (m_eStatus == ReadingMessage)
		{
			if (m_nNextBlockSize-m_buffer.size() > 0)
			{
				char* buf=new char[m_nNextBlockSize-m_buffer.size()];
				int nLen=out.readRawData(buf, m_nNextBlockSize-m_buffer.size());

				if (nLen != -1)
				{
					m_buffer.append(buf, nLen);

					if (m_buffer.size() == m_nNextBlockSize)
					{
						m_eStatus = ReadyForUse;
					}

					SAFE_DELETE_ARRAY(buf);
				}
				else
				{
					m_eStatus=Quit;
					m_pSocket->abort();

					SAFE_DELETE_ARRAY(buf);

					m_bLock=false;
					return;
				}
				
			}
			else
			{
				m_eStatus=WaitingForMessage;
			}
		}

		if (m_eStatus == ReadyForUse)
		{
			QIDEAPtr ptr=QDEAFactory::instance()->dea(m_nEncryptType);
			if (ptr == NULL)
			{
				logplusError(LIBNETWORK_LOGGER_STR, QString("Receive an invalid encryption message from %1:%2, encryption type = %3!")
					.arg(m_pSocket->peerAddress().toString()).arg(m_pSocket->peerPort()).arg(m_nEncryptType));

				m_eStatus=Quit;
				m_pSocket->abort();

				m_bLock=false;
				return;
			}

			QByteArray datagram=ptr->decrypt(m_buffer);

			if (datagram.size() < (int)(strlen(PROTOCAL_HEADER)+2*sizeof(int)))
			{
				logplusWarning(LIBNETWORK_LOGGER_STR, QString("Receive a too short message from %1:%2!")
					.arg(m_pSocket->peerAddress().toString()).arg(m_pSocket->peerPort()));

				m_bLock=false;
				return;
			}

			Q_READ_ONLY_DATA_STREAM(stream, datagram);

			char* strHeader;
			stream>>strHeader;
			if (strHeader == NULL)
			{
				logplusWarning(LIBNETWORK_LOGGER_STR, QString("Receive an invalid message from %1:%2!")
					.arg(m_pSocket->peerAddress().toString()).arg(m_pSocket->peerPort()));

				m_eStatus=Quit;

				m_bLock=false;
				return;
			}

			if (strcmp(strHeader, PROTOCAL_HEADER) != 0)
			{
				logplusWarning(LIBNETWORK_LOGGER_STR, QString("Receive an invalid message from %1:%2!")
					.arg(m_pSocket->peerAddress().toString()).arg(m_pSocket->peerPort()));

				m_eStatus=Quit;

				m_pSocket->abort();
				SAFE_DELETE_ARRAY(strHeader);

				m_bLock=false;

				return;
			}

			SAFE_DELETE_ARRAY(strHeader);
			
			stream>>m_nNextBlockSize;
			if (m_nNextBlockSize > MaxBufferSize)
			{
				logplusWarning(LIBNETWORK_LOGGER_STR, QString("Receive a too long message from %1:%2!")
					.arg(m_pSocket->peerAddress().toString()).arg(m_pSocket->peerPort()));

				m_pSocket->abort();

				m_eStatus=Quit;
				break;
			}

			QByteArray data;
			data.resize(m_nNextBlockSize);
			stream.readRawData(data.data(), m_nNextBlockSize);

			emit recvMessage(this, data);

			m_eStatus=WaitingForMessage;
		}
	}

	m_bLock=false;
}

QByteArray QConnection::addHeaderToPacket(const QByteArray& buffer, int msgType)
{
	Q_WRITE_ONLY_DATA_STREAM(in, buf);

	in<<PROTOCAL_HEADER<<buffer.size()+(int)sizeof(int)<<msgType;
	in.writeRawData(buffer.data(), buffer.size());

	return buf;
}

bool QConnection::sendMessage(const QByteArray& buffer, int msgType)
{
	if (isQuit())
	{
		return false;
	}

	int nDeaType=QGlobalInfo::instance()->deaType(QGlobalInfo::MessagesDea);

	QIDEAPtr ptr=QDEAFactory::instance()->dea(nDeaType);
	if (ptr == NULL)
	{
		logplusError(LIBNETWORK_LOGGER_STR, QString("Failed to encrypt message! Please to check your libdea library!"));

		return false;
	}

	QByteArray buf=addHeaderToPacket(buffer, msgType);
	buf=ptr->encrypt(buf);

	Q_WRITE_ONLY_DATA_STREAM(in, datagram);
	in<<nDeaType<<buf.size();
	in.writeRawData(buf.data(), buf.size());

	QMutexLocker locker(&m_mutex);

	if (m_pSocket == NULL || m_pSocket->state() != QAbstractSocket::ConnectedState)
	{
		logplusWarning(LIBNETWORK_LOGGER_STR, QString("Send message failed! Socket is unconnected!"));

		return false;
	}

	bool bRtn=(m_pSocket->write(datagram) == datagram.size());

	if (!bRtn)
	{
		logplusWarning(LIBNETWORK_LOGGER_STR, QString("Send message failed! Socket write failed!"));
	}

	return bRtn;
}

bool QConnection::addReceiver(quint32 usrID, int msgType)
{
	if (isQuit())
	{
		return false;
	}

	switch (msgType)
	{
	case MSG_SYSTEM_PROCESS:
		QProcessSender::instance()->addClient(this, usrID);
		break;
	case MSG_PROCESSOR:
		QProcessorSender::instance()->addClient(this, usrID);
		break;
	case MSG_SYSTEM_MEMORY:
		QMemorySender::instance()->addClient(this, usrID);
		break;
	default:
		break;
	}

	return true;
}

bool QConnection::removeReceiver(quint32 usrID, int msgType)
{
	switch (msgType)
	{
	case MSG_SYSTEM_PROCESS:
		QProcessSender::instance()->removeClient(this, usrID);
		break;
	case MSG_PROCESSOR:
		QProcessorSender::instance()->removeClient(this, usrID);
		break;
	case MSG_SYSTEM_MEMORY:
		QMemorySender::instance()->removeClient(this, usrID);
		break;
	default:
		break;
	}

	return true;
}

QHostAddress QConnection::peerAddress()
{
	QHostAddress addr;

	if (m_pSocket != NULL)
	{
		addr=m_pSocket->peerAddress();
	}
	
	return addr;
}

quint16 QConnection::peerPort()
{
	if (m_pSocket != NULL)
	{
		return m_pSocket->peerPort();
	}
	
	return 0;
}
