#include "qinterprocessserver.h"

#include <QLocalSocket>

#include "define.h"
#include "qlogger.h"
#include "qmibfactory.h"

QInterProcessServer::QInterProcessServer(const QString& name, QObject *parent/* =NULL */)
	: QObject(parent)
	,m_strName(name)
	,m_pServer(NULL)
	,m_eStatus(WaitingForMessage)
	,m_nNextBlockSize(0)
	,m_bLock(false)
{
	m_pServer=new QLocalServer;
}

QInterProcessServer::~QInterProcessServer()
{
	SAFE_DELETE_CLASS(m_pServer);
}

bool QInterProcessServer::onListen()
{
	if (m_pServer == NULL)
	{
		return false;
	}

	onClose();

	QLocalServer::removeServer(m_strName);
	bool bRtn=m_pServer->listen(m_strName);

	if (!bRtn)
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("Failed to init interprocess communication! error=%3").arg(m_pServer->errorString()));
	}
	else
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("Finished to init interprocess communication successfully. name=%1").arg(m_strName));

		connect(m_pServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}

	return bRtn;
}

void QInterProcessServer::disconnected()
{
	if (m_eStatus == Quit)
	{
		return;
	}

	m_eStatus=Quit;

	QLocalSocket* pSocket=dynamic_cast<QLocalSocket*>(sender()); 

	if (pSocket != NULL)
	{
		pSocket->abort();

		emit deleteConnection(m_strName);
	}
}

void QInterProcessServer::onClose()
{
	if (m_pServer != NULL && m_pServer->isListening())
	{
		m_pServer->close();

		logplusInfo(LIBNETWORK_LOGGER_STR, QString("Interprocess communication closed."));
	}
}

void QInterProcessServer::newConnection()
{
	QLocalSocket* pSocket= m_pServer->nextPendingConnection(); 

	connect(pSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(pSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

static const int MaxBufferSize= 1024*1024*30;

void QInterProcessServer::readyRead()
{
	if (m_eStatus==Quit || m_bLock)
	{
		return;
	}

	QLocalSocket* pSocket=static_cast<QLocalSocket *>(sender()); 
	if (pSocket == NULL)
	{
		return;
	}

	m_bLock=true;

	Q_READ_ONLY_DATA_STREAM(out, pSocket);

	while (pSocket->bytesAvailable() > 0)
	{
		if (m_eStatus == WaitingForMessage)
		{
			if (pSocket->bytesAvailable() < 2*sizeof(int))
			{
				return;
			}

			out>>m_nNextBlockSize;

			if (m_nNextBlockSize > MaxBufferSize)
			{
				logplusWarning(LIBNETWORK_LOGGER_STR, QString("Receive a too long interprocess message! lenth=%1").arg(m_nNextBlockSize));

				m_eStatus=Quit;
				pSocket->abort();

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
					pSocket->abort();

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
			recvMessage(m_buffer);

			m_eStatus=WaitingForMessage;
		}
	}

	m_bLock=false;
}

void QInterProcessServer::recvMessage(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer)

	int nType;

	out>>nType;
	Q_ASSERT(nType>=0 && nType<UTIL_TYPE_END);

	QMibFactory::instance()->product(nType)->setResult(buffer.right(buffer.size()-sizeof(int)));
	QMibFactory::instance()->product(nType)->finished();
}

