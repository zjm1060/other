#include "qabstractsender.h"

#include <QByteArray>
#include <QVector>
#include <QDataStream>

#include "qmessageparser.h"
#include "qservermanager.h"
#include "qconnectionmanager.h"

QAbstractSender::QAbstractSender(int interval, QObject *parent)
	: QObject(parent)
{
	m_timer.setInterval(interval);		//每过一秒钟向注册的客户端发送处理器信息

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QAbstractSender::~QAbstractSender()
{

}

void QAbstractSender::addClient(QConnection* client, quint32 usrID)
{
	if (client == NULL)
	{
		return;
	}

	QPair<QConnection*, quint32> pClient(client, usrID);

	QMutexLocker locker(&m_mutex);

	if (m_listClient.contains(pClient))
	{
		return;
	}

	m_listClient.append(pClient);

	if (m_listClient.size() == 1)
	{
		m_timer.start();
	}
}

void QAbstractSender::removeClient(QConnection* client, quint32 usrID)
{
	if (client == NULL)
	{
		return;
	}

	m_listClient.removeOne(QPair<QConnection*, quint32>(client, usrID));

	if (m_listClient.size() == 0)
	{
		m_timer.stop();
	}
}

void QAbstractSender::timeout()
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(message());
	if (pInstance == NULL)
	{
		return;
	}

	QList<QPair<QConnection*, quint32> > clients;

	m_mutex.lock();
	clients=m_listClient;
	m_mutex.unlock();

	QListIterator<QPair<QConnection*, quint32> > it(m_listClient);
	while (it.hasNext())
	{
		QPair<QConnection*, quint32> pClient=it.next();
		QObjectPtr ptr=QConnectionManager::instance()->connection(pClient.first);
		if (ptr != NULL)
		{
			if (!pClient.first->isQuit())
			{
				QByteArray buffer=pInstance->query(ack(), pClient.second);

				pInstance->parse(pClient.first, buffer);
			}
			else
			{
				removeClient(pClient.first, pClient.second);
			}
		}
		else
		{
			removeClient(pClient.first, pClient.second);
		}
	}
}
