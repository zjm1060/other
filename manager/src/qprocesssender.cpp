#include "qprocesssender.h"

#include <QByteArray>
#include <QVector>
#include <QDataStream>
#include <QThreadPool>

#include "common.h"
#include "define.h"
#include "extern.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

#ifdef Q_OS_AIX
#include "qstatprocessor.h"
#endif

#include "qmessageprocess.h"

QMutex QProcessSender::m_mutex;
QProcessSender* QProcessSender::m_pInstance=0;

#define PROCESS_SENDER_TIME_OUT		1000

QProcessSender::QProcessSender(QObject *parent)
	: QObject(parent),
	m_nTotalTimeProcess(0)
{
	m_timer.setInterval(PROCESS_SENDER_TIME_OUT);		//每过一秒钟向注册的客户端发送进程消息

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

#ifdef Q_OS_AIX
	QVector<ProcessorInfo> processor=QStatFactory::instance()->exec(WMI_TYPE_PROCESSOR).value< QVector<ProcessorInfo> >();
	QStatProcessor::processors=processor.size();
#endif
}

QProcessSender::~QProcessSender()
{

}

QProcessSender* QProcessSender::instance()
{
	if (m_pInstance == 0)
	{
		m_mutex.lock();
		if (m_pInstance == 0)
		{
			m_pInstance=new QProcessSender;
		}
		m_mutex.unlock();
	}

	return m_pInstance;
}

void QProcessSender::addClient(QConnection* client)
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

void QProcessSender::removeClient(QConnection* client)
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
		QAbstractWmi* pInstance=QWmiFactory::instance()->wmiInstance(WMI_TYPE_PROCESS);
#else
		QAbstractStat* pInstance=QStatFactory::instance()->statInstance(WMI_TYPE_PROCESS);
#endif
		
		if (pInstance != NULL)
		{
			pInstance->clear();
		}
	}
}

void QProcessSender::timeout()
{
	QProcessSenderRunnable* pRunnable=new QProcessSenderRunnable(m_mapTimeProcess, m_nTotalTimeProcess);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QMap<quint32, quint64>, quint64, QByteArray)),
		this, SLOT(finished(QMap<quint32, quint64>, quint64, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);
}

void QProcessSender::finished(QMap<quint32, quint64> mapTimeProcess, quint64 totalTimeProcess, QByteArray buffer)
{
	m_mapTimeProcess=mapTimeProcess;
	m_nTotalTimeProcess=totalTimeProcess;

	foreach (QConnection* pClient, m_listClient)
	{
		if (pClient != NULL)
		{
			if (!pClient->isQuit())
			{
				pClient->sendMessage(buffer, QMessageProcess::Type);
			}
			else
			{
				removeClient(pClient);
			}
		}
	}
}

QProcessSenderRunnable::QProcessSenderRunnable(QMap<quint32, quint64> mapTimeProcess, quint64 totalTimeProcess, QObject* parent)
:QObject(parent)
{
	m_mapTimeProcess=mapTimeProcess;
	m_nTotalTimeProcess=totalTimeProcess;
}

QProcessSenderRunnable::~QProcessSenderRunnable()
{

}

void QProcessSenderRunnable::run()
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
	stream.setVersion(DATA_STREAM_VERSION);

	stream<<QMessageProcess::AnswerProcess;

	QVariant var;
#ifdef Q_OS_WIN
	var=QWmiFactory::instance()->exec(WMI_TYPE_PROCESS);
#else
	var=QStatFactory::instance()->exec(WMI_TYPE_PROCESS);
#endif

	QVector<SystemProcess> processes=var.value< QVector<SystemProcess> >();
	
	if (processes.isEmpty())
	{
		stream<<QMessageProcess::QueryFailed;
	}
	else
	{
		quint64 nOld, nNew;

		quint64 nTotalTimeProcess=0;
		QMap<quint32, quint64> mapTimeProcess;

		QMutableVectorIterator<SystemProcess> it(processes);
		while (it.hasNext())
		{
			it.next();

			const SystemProcess& process=it.value();

#ifdef Q_OS_WIN
			if (process.processorTime != -1)
			{
				nTotalTimeProcess+=process.processorTime;
			}

#elif defined(Q_OS_AIX)
			if (process.name == "wait" && process.executablePath.trimmed().isEmpty())
			{
				it.remove();
			}
#endif
		}

		qDebug("QProcessSenderRunnable: %llu", nTotalTimeProcess);

		stream<<QMessageProcess::QuerySucceed;
		stream<<processes.size();

		QString strH9000Path=getEnvPath("h9000");
		for (int i=0; i<processes.size(); i++)
		{
			SystemProcess& process=processes[i];

#if defined(Q_OS_WIN) || defined(Q_OS_AIX)
			if (process.processorTime == -1)
			{
				process.processorTime=0;
			}

#ifdef Q_OS_WIN
			if (m_nTotalTimeProcess > 0 && nTotalTimeProcess > m_nTotalTimeProcess)
			{
#endif
				mapTimeProcess.insert(process.processId, process.processorTime);

				if (m_mapTimeProcess.contains(process.processId))
				{
					nOld=m_mapTimeProcess[process.processId];
					nNew=process.processorTime;
					if (nOld == -1 || nNew == -1)
					{
						process.percentProcessorTime=0.0;
					}
					else
					{
						if (nNew>nOld)
						{
#ifdef Q_OS_WIN
							process.percentProcessorTime=(nNew-nOld)*100.0/(nTotalTimeProcess-m_nTotalTimeProcess);
#elif defined(Q_OS_AIX)
							int count=QStatProcessor::processors;
							count=count>0? count:1;
							process.percentProcessorTime=(nNew-nOld)*100.0/(1000000000.0*count);
#endif
							
						}
						else
						{
							process.percentProcessorTime=0.0;
						}
					}
				}
				else
				{
					process.percentProcessorTime=0.0;
				}
#ifdef Q_OS_WIN
			}
			else
			{
				process.percentProcessorTime=0.0;
			}
#endif
			
#endif

			if (!strH9000Path.isEmpty() && !process.executablePath.contains('/'))
			{
				QStringList list=process.executablePath.split(QRegExp("\\W+"), QString::SkipEmptyParts);
				if (list.size() > 0)
				{
					if (QFile::exists(appendPath(strH9000Path, list[0])))
					{
						process.executablePath=appendPath(strH9000Path, list[0]);

						QStringListIterator it(list);
						it.next();

						while (it.hasNext())
						{
							process.executablePath=" "+it.next();
						}
					}
				}
			}
			
			stream<<process.usrName<<process.executablePath<<process.name<<process.processId
				<<process.sessionId<<process.workingSetSize<<process.percentProcessorTime;
		}

 		emit finished(mapTimeProcess, nTotalTimeProcess, buffer);
	}
}
