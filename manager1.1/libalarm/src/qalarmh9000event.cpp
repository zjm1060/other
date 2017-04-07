#include "qalarmh9000event.h"

#include <QCoreApplication>

#include "extern.h"
#include "qlogger.h"
#include "common.h"
#include "qcscsinitialize.h"

QAlarmH9000Event::QAlarmH9000Event()
{
	
}

QAlarmH9000Event::~QAlarmH9000Event()
{
	
}

void QAlarmH9000Event::initialize()
{
	if (!initialized())
	{
		m_mutex.lock();

		if (!QCscsInitialize::instance()->initialized())
		{
			m_mutex.unlock();

			return;
		}

		QReadH9000FileSystemAlarmFormXmlFile configure(appendPath(getAppParentDir(), HMAN_ALARM_CONFIGURE_FILE));
		if (configure.read())
		{
			m_configure=configure.result();
		}

		m_mutex.unlock();

		QAbstractAlarm::initialize();
	}
}

int QAlarmH9000Event::trigger(const QVariant& var, bool L1, bool L2)
{
	int flags=0;
	
	QVector<LogicalDisk> disks=var.value<QVector<LogicalDisk> >();
	for (int i=0; i<disks.size(); i++)
	{
		double dUsed=1.0-((double)disks[i].freeSpace)/disks[i].size;

		bool bRet=false;
		if (L1 && dUsed>m_configure.upperLevel)
		{
#ifdef Q_OS_WIN
			bRet=sendAlarm(disks[i].deviceID, 4, 1.0-dUsed);
#else
			bRet=sendAlarm(disks[i].description, 4, 1.0-dUsed);
#endif

			if (bRet)
			{
				flags|=AlarmUpdateIntervalL1;
			}
		}
		else if (L2 && dUsed > m_configure.lowLevel)
		{
#ifdef Q_OS_WIN
			bRet=sendAlarm(disks[i].deviceID, dUsed>m_configure.upperLevel? 4:3, 1.0-dUsed);
#else
			bRet=sendAlarm(disks[i].description, dUsed>m_configure.upperLevel? 4:3, 1.0-dUsed);
#endif

			if (bRet)
			{
				flags|=AlarmUpdateIntervalL2;
			}
		}
	}

	return flags;
}

bool QAlarmH9000Event::sendAlarm(const QString& path, int level, double value)
{
	if (!m_configure.partitions.contains(path))
	{
		return false;
	}

	QString strLongName=m_configure.partitions[path];

	return QCscsInitialize::instance()->sendAlarm(strLongName, level, value);
}

REGISTER_ALARM_INSTANCE(QAlarmH9000Event)
