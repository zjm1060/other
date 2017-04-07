#include "qprocessorsender.h"

#include <QByteArray>
#include <QVector>
#include <QDataStream>
#include <QThreadPool>

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

#include "qmessageprocessor.h"

QMutex QProcessorSender::m_mutex;
QProcessorSender* QProcessorSender::m_pInstance=0;

QProcessorSender::QProcessorSender(QObject *parent)
	: QObject(parent),
	m_nTotalTimeProcess(0)
{
	m_timer.setInterval(1000);		//每过一秒钟向注册的客户端发送处理器信息

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QProcessorSender::~QProcessorSender()
{

}

QProcessorSender* QProcessorSender::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QProcessorSender;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void QProcessorSender::addClient(QConnection* client)
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
#ifndef Q_OS_WIN
		QAbstractStat* pInstance=QStatFactory::instance()->statInstance(WMI_TYPE_PROCESSOR);
		if (pInstance != NULL)
		{
			pInstance->init();
		}
#endif

		timeout();
		m_timer.start();
	}
}

void QProcessorSender::removeClient(QConnection* client)
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
		QAbstractWmi* pInstance=QWmiFactory::instance()->wmiInstance(WMI_TYPE_PROCESSOR);
		if (pInstance != NULL)
		{
			pInstance->clear();
		}
#else
		QAbstractStat* pInstance=QStatFactory::instance()->statInstance(WMI_TYPE_PROCESSOR);
		if (pInstance != NULL)
		{
			pInstance->clear();
		}
#endif
		
	}
}

void QProcessorSender::timeout()
{
	QProcessorSenderRunnable* pRunnable=new QProcessorSenderRunnable();
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QByteArray)), this, SLOT(finished(QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);
}

void QProcessorSender::finished(QByteArray buffer)
{
	foreach (QConnection* pClient, m_listClient)
	{
		if (pClient != NULL)
		{
			if (!pClient->isQuit())
			{
				pClient->sendMessage(buffer, QMessageProcessor::Type);
			}
			else
			{
				removeClient(pClient);
			}
		}
	}
}

QProcessorSenderRunnable::QProcessorSenderRunnable(QObject* parent)
:QObject(parent)
{
}

QProcessorSenderRunnable::~QProcessorSenderRunnable()
{

}

void QProcessorSenderRunnable::run()
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
	stream.setVersion(DATA_STREAM_VERSION);

	stream<<QMessageProcessor::AnswerRuntimeProcessor;

	QVariant var;
#ifdef Q_OS_WIN
	var=QWmiFactory::instance()->exec(WMI_TYPE_PERF_PROCESSOR);
#else
	var=QStatFactory::instance()->exec(WMI_TYPE_PERF_PROCESSOR);
#endif

	PerfProcessor processor=var.value<PerfProcessor>();
	if (processor.core.isEmpty())
	{
		stream<<QMessageProcessor::QueryFailed;
	}
	else
	{
		stream<<QMessageProcessor::QuerySucceed;

		stream<<processor.currentClockSpeed<<processor.currentVoltage<<processor.status;

		stream<<processor.core.size();
		for (int i=0; i<processor.core.size(); i++)
		{
			const PerfProcessorCore& core=processor.core[i];
			stream<<core.name<<core.percentProcessorTime;
		}

 		emit finished(buffer);
	}
}
