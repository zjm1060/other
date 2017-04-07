#include "qmibmanager.h"

#include "qmibfactory.h"
#include "qlogger.h"
#include "qglobalinfo.h"

#define MIB_CHECK_OUT_INTERVAL	100

QMibManager::QMibManager(QObject *parent)
	: QObject(parent)
	,m_bCollect(false)
{
	m_timer.setInterval(MIB_CHECK_OUT_INTERVAL);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QMibManager::~QMibManager()
{
	stop();
}

QMibManager* QMibManager::instance()
{
	static QMibManager* pInstance=new QMibManager;

	return pInstance;
}

void QMibManager::setInterval(int interval)
{
	if (interval < 0)
	{
		logplusDebug(LIBUTIL_LOGGER_STR, "Set invalid interval to the MIB manager thread!!!");
		interval=0;
	}

	m_timer.setInterval(interval);
}

void QMibManager::start(bool collect)
{
	if (m_timer.isActive())
	{
		logplusDebug(LIBUTIL_LOGGER_STR, "The MIB manager is active!!!");
		return;
	}

	m_bFirst=true;
	m_bCollect=collect;

	m_timer.start();
}

void QMibManager::stop()
{
	m_timer.stop();

#ifndef Q_OS_AIX
	QGlobalInfo::globalThreadPool()->waitForDone();
#endif
}

void QMibManager::timeout()
{
	if (!m_bCollect)
	{
		quint64 nSign=QMibFactory::instance()->signs();

		if (nSign != 0)
		{
			QMibFactory::instance()->clear();
			emit changed(nSign);
		}
	}
	else
	{
#ifdef Q_OS_AIX
		static bool g_bRunning=false;

		if (g_bRunning)
		{
			return;
		}

		g_bRunning=true;
#endif
		QDateTime dateTime=QDateTime::currentDateTime();

		QList<int> keys=QMibFactory::instance()->products().keys();

		foreach (int key, keys)
		{
			QAbstractMibPtr ptr=QMibFactory::instance()->product(key);
			if (ptr != NULL&& ptr->model() != QAbstractMib::ManualModel && (m_bFirst || ptr->isTurn(dateTime)))
			{
				logplusDebug(LIBUTIL_LOGGER_STR, QString("Add new task: type=%1 !").arg(key));

				QMibTask* pTask=new QMibTask(key, this);

#ifdef Q_OS_AIX
				pTask->run();
				SAFE_DELETE_CLASS(pTask);		//aix系统下收集进程信息可能会于搜集其他信息的功能产生死锁
#else
				pTask->setAutoDelete(true);
				QGlobalInfo::globalThreadPool()->start(pTask);
#endif
			}
		}

		m_bFirst=false;

#ifdef Q_OS_AIX
		g_bRunning=false;
#endif
	}
}

void QMibManager::finished(int type)
{
	QAbstractMibPtr ptr=QMibFactory::instance()->product(type);
	if (ptr != NULL)
	{
		ptr->finished();
	}

	if (m_bCollect)
	{
		emit collected(type);
	}
}


QMibTask::QMibTask(int type, QObject* view)
:m_nType(type)
,m_pView(view)
{

}

QMibTask::~QMibTask()
{
}

void QMibTask::run()
{
	QAbstractMibPtr ptr=QMibFactory::instance()->product(m_nType);
	if (ptr != NULL)
	{
		ptr->exec();
	}

	QMetaObject::invokeMethod(m_pView, "finished", Q_ARG(int, m_nType));
}
