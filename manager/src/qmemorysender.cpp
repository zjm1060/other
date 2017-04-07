#include "qmemorysender.h"

#include <QByteArray>
#include <QVector>
#include <QDataStream>
#include <QThreadPool>

#include "define.h"
#include "extern.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

#include "qmessagememory.h"

QMutex QMemorySender::m_mutex;
QMemorySender* QMemorySender::m_pInstance=0;

QMemorySender::QMemorySender(QObject *parent)
	: QObject(parent)
{
	m_timer.setInterval(1000);		//每过一秒钟向注册的客户端发送进程消息

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QMemorySender::~QMemorySender()
{

}

QMemorySender* QMemorySender::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QMemorySender;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void QMemorySender::addClient(QConnection* client)
{
	if (client == NULL)
	{
		return;
	}

	foreach (QConnection* pConnection, m_listClient)
	{
		if (pConnection == client)
		{
			return;
		}
	}

	m_listClient.append(client);
	client->ref();

	if (m_listClient.size() == 1)
	{
		timeout();
		m_timer.start();
	}
}

void QMemorySender::removeClient(QConnection* client)
{
	if (client == NULL)
	{
		return;
	}

	QMutableListIterator<QConnection*>	it(m_listClient);
	QConnection* pConnection;
	while (it.hasNext())
	{
		pConnection=it.next();
		if (pConnection == client)
		{
			pConnection->unref();
			it.remove();
			break;
		}
	}

	if (m_listClient.size() == 0)
	{
		m_timer.stop();

#ifdef Q_OS_WIN
		QAbstractWmi* pInstance=QWmiFactory::instance()->wmiInstance(WMI_TYPE_SYSTEM_MEMORY);
		if (pInstance != NULL)
		{
			pInstance->clear();
		}
#endif

	}
}

void QMemorySender::timeout()
{
	QMemorySenderRunnable* pRunnable=new QMemorySenderRunnable();
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QByteArray)), this, SLOT(finished(QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);
}

void QMemorySender::finished(QByteArray buffer)
{
	foreach (QConnection* pClient, m_listClient)
	{
		if (pClient != NULL)
		{
			if (!pClient->isQuit())
			{
				pClient->sendMessage(buffer, QMessageMemory::Type);
			}
			else
			{
				removeClient(pClient);
			}
		}
	}
}

QMemorySenderRunnable::QMemorySenderRunnable(QObject* parent)
:QObject(parent)
{
}

QMemorySenderRunnable::~QMemorySenderRunnable()
{

}

void QMemorySenderRunnable::run()
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
	stream.setVersion(DATA_STREAM_VERSION);

	stream<<QMessageMemory::AnswerSystemMemory;

	QVariant var;
#ifdef Q_OS_WIN
	var=QWmiFactory::instance()->exec(WMI_TYPE_SYSTEM_MEMORY);
#else
	var=QStatFactory::instance()->exec(WMI_TYPE_SYSTEM_MEMORY);
#endif

	SystemMemory memory=var.value<SystemMemory>();
	if (memory.totalVisibleMemorySize == 0)
	{
		stream<<QMessageMemory::QueryFailed;
	}
	else
	{
		stream<<QMessageMemory::QuerySucceed;

		stream<<memory.datetime<<memory.freePhysicalMemory<<memory.freeVirtualMemory<<memory.totalVisibleMemorySize
			<<memory.totalVirtualMemorySize<<memory.freeSpaceInPagingFiles<<memory.sizeStoredInPagingFiles
			<<memory.sizeUsageInPagingFiles<<memory.sizeUsageInPagingFilesPeak
			<<memory.systemCache<<memory.poolNonpaged<<memory.poolPaged;
	}

	emit finished(buffer);
}
