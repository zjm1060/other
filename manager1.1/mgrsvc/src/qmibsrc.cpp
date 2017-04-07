#include "qmibsrc.h"

#include "qlogger.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif
#include "qmibfactory.h"
#include "qmibmanager.h"

QMibSrc::QMibSrc(QObject *parent/* =0 */)
	: QObject(parent)
{
	init();

	m_pClient=new QInterProcessClient("hman.mib");
	connect(m_pClient, SIGNAL(clientConnected()), this, SLOT(connected()));
	connect(m_pClient, SIGNAL(clientQuit()), this, SLOT(clientQuit()));

	m_pClient->connectToHost();
}

QMibSrc::~QMibSrc()
{
}

void QMibSrc::init()
{
	//加载统计模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading statistics module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QWmiFactory::instance()->loadLibrary("libmibd1", "CreateWmiInstance"))
#else
	if (QWmiFactory::instance()->loadLibrary("libmib1", "CreateWmiInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QStatFactory::instance()->loadLibrary("libmibd", "CreateStatInstance"))
#else
	if (QStatFactory::instance()->loadLibrary("libmib", "CreateStatInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded statistics module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "Statistics module loading failed!");
	}

	//加载管理信息库模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading the management information base module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QMibFactory::instance()->loadLibrary("libmibd1", "CreateMibInstance"))
#else
	if (QMibFactory::instance()->loadLibrary("libmib1", "CreateMibInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QMibFactory::instance()->loadLibrary("libmibd", "CreateMibInstance"))
#else
	if (QMibFactory::instance()->loadLibrary("libmib", "CreateMibInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded the management information base module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "The management information base loading failed!");
	}


#ifdef Q_OS_WIN
	logplusInfo(MGRSVC_LOGGER_STR, "Connecting to WMI service...");
	QWmiFactory::instance()->initialize();
#endif
}

void QMibSrc::startMib()
{
	logplusInfo(MGRSVC_LOGGER_STR, "Start the managerment informantion base...");
	QMibManager::instance()->start(true);
}

void QMibSrc::connected()
{
	connect(QMibManager::instance(), SIGNAL(collected(int)), this, SLOT(collected(int)));

	startMib();
}

void QMibSrc::clientQuit()
{
	logplusInfo(MGRSVC_LOGGER_STR, "Stop the managerment informantion base !");
	QMibManager::instance()->stop();

	SAFE_DELETE_CLASS(m_pClient);

	logplusInfo(MGRSVC_LOGGER_STR, "Exit ...");

	exit(0);
}

void QMibSrc::collected(int type)
{
	if (type >= UTIL_TYPE_PROCESSOR)
	{
		logplusDebug(MGRSVC_LOGGER_STR, QString("Collected MIB of type %1.").arg(type));

		QAbstractMibPtr ptr=QMibFactory::instance()->product(type);
		if (ptr != NULL)
		{
			m_pClient->sendMessage(ptr->writeStream(), type);
		}
	}
}

