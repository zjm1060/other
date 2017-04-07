#include "qconnection.h"

#include <QMutexLocker>

#include "qlogger.h"

#define PROTOCAL_HEADER "IWHR"

static const int MaxBufferSize= 1024000*3;		//通信消息缓冲区最大值

QConnection::QConnection(QTcpSocket* socket, QObject *parent)
	: QObject(parent),
	m_nRef(0)
{
	m_pSocket=socket;

	m_eStatus=WaitingForMessage;
	m_nNextBlockSize=0;

	connect(m_pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

QConnection::~QConnection()
{
}

void QConnection::disconnected()
{
	m_eStatus=Quit;

	if (m_pSocket != NULL)
	{
		m_pSocket->abort();

		emit deleteConnection(m_pSocket, m_pSocket->peerAddress(), m_pSocket->peerPort());
		m_pSocket=NULL;
	}

	unref();
}

void QConnection::ref()
{
	QMutexLocker locker(&m_mutex);
	m_nRef++;
}

void QConnection::unref()
{
	QMutexLocker locker(&m_mutex);
	m_nRef--;
	if (m_nRef < 0)
	{
		deleteLater();
	}
}

bool QConnection::isQuit()
{
	if (m_eStatus == Quit)
	{
		return true;
	}

	return false;
}

void QConnection::readyRead()
{
	if (m_eStatus==Quit)
	{
		return;
	}

	while (m_pSocket->bytesAvailable() > 0)
	{
		QDataStream out(m_pSocket);
		out.setVersion(DATA_STREAM_VERSION);

		if (m_eStatus == WaitingForMessage)
		{
			if (m_pSocket->bytesAvailable() < strlen(PROTOCAL_HEADER)+2*sizeof(int))
			{
				return;
			}

			m_buffer.clear();

			char* strHeader;
			out>>strHeader;

			if (strcmp(strHeader, PROTOCAL_HEADER) != 0)
			{
				LOGPLUS_ERROR_STR(QLogger::root(), qFormatString("Invalid Message!!"));

				m_pSocket->abort();
				SAFE_DELETE_ARRAY(strHeader);
				return;
			}

			SAFE_DELETE_ARRAY(strHeader);
;
			out>>m_nNextBlockSize;
			if (m_nNextBlockSize > MaxBufferSize)
			{
				LOGPLUS_ERROR_STR(QLogger::root(), qFormatString("The Message is too long!!"));

				m_pSocket->abort();
				return;
			}

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
					m_pSocket->abort();

					SAFE_DELETE_ARRAY(buf);

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
			emit recvMessage(this, m_buffer);

			m_eStatus=WaitingForMessage;
		}
	}
}

bool QConnection::sendMessage(const QByteArray& buffer, int msgType)
{
	if (m_eStatus == Quit)
	{
		return false;
	}

	if (m_pSocket == NULL || m_pSocket->state() != QAbstractSocket::ConnectedState)
	{
		LOGPLUS_ERROR_STR(QLogger::root(), qFormatString("Send message failed! Socket is unconnected!"));

		return false;
	}

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<PROTOCAL_HEADER<<buffer.size()+(int)sizeof(int)<<msgType;
	in.writeRawData(buffer.data(), buffer.size());

	bool bRtn=(m_pSocket->write(buf) == buf.size());

	if (!bRtn)
	{
		LOGPLUS_ERROR_STR(QLogger::root(), qFormatString("Send message failed! Socket write failed!!"));
	}

	return bRtn;
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
