#include "qabstractalarm.h"

#include "qutildef.h"
#include "qlogger.h"

QAbstractAlarm::QAbstractAlarm()
:m_bInit(false)
,m_bOpened(true)
,m_nIntervalL1(30)
,m_nIntervalL2(60*5)
{
	
}

QAbstractAlarm::~QAbstractAlarm()
{

}

void QAbstractAlarm::initialize()
{
	QMutexLocker locker(&m_mutex);

	m_bInit=true;
}

bool QAbstractAlarm::initialized()
{
	QMutexLocker locker(&m_mutex);

	return m_bInit;
}

void QAbstractAlarm::destroy()
{
	QMutexLocker locker(&m_mutex);

	m_bInit=false;
}

void QAbstractAlarm::open()
{
	QMutexLocker locker(&m_mutex);

	m_bOpened=true;
}

void QAbstractAlarm::close()
{
	QMutexLocker locker(&m_mutex);

	m_bOpened=false;
}

void QAbstractAlarm::alarm(const QVariant& var)
{
	if (!initialized())
	{
		return;
	}

	QMutexLocker locker(&m_mutex);
	if (!m_bOpened)
	{
		return;
	}

	bool bL1=false, bL2=false;

	QDateTime curr=QDateTime::currentDateTime();
	if (!m_lastDateTimeL1.isValid() || m_lastDateTimeL1.addSecs(m_nIntervalL1) <= curr)
	{
		bL1=true;
	}

	if (!m_lastDateTimeL2.isValid() || m_lastDateTimeL2.addSecs(m_nIntervalL2) <= curr)
	{
		bL2=true;
	}

	if (bL1 || bL2)
	{
		int flags=trigger(var, bL1, bL2);

		if ((flags & AlarmUpdateIntervalL1) !=0 || !m_lastDateTimeL1.isValid())
		{
			m_lastDateTimeL1=curr;
		}

		if ((flags & AlarmUpdateIntervalL2) !=0 || !m_lastDateTimeL2.isValid())
		{
			m_lastDateTimeL2=curr;
		}
	}
}
