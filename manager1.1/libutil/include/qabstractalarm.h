#ifndef QABSTRACTALARM_H
#define QABSTRACTALARM_H

#include <QVector>
#include <QMutex>
#include <QVariant>
#include <QDateTime>

#include "global.h"
#include "qsharedobject.h"

enum Alarm
{
	H9000AlarmEvent=0
};

class UTIL_EXPORT QAbstractAlarm : public QSharedObject
{
public:
	QAbstractAlarm();
	virtual ~QAbstractAlarm();

	enum AlarmUpdateIntervalFlags
	{
		AlarmUpdateIntervalL1=1,
		AlarmUpdateIntervalL2=(1<<1)
	};

public:
	void open();
	void close();

public:
	virtual int type() const=0;

	virtual void initialize();
	bool initialized();

	virtual void destroy();

public:
	void alarm(const QVariant& var);

protected:
	virtual int trigger(const QVariant& var, bool L1, bool L2)=0;

protected:
	QMutex	m_mutex;
	bool	m_bInit;

	int		m_nIntervalL1;	//严重警告播报间隔
	int		m_nIntervalL2;	//普通警告播报间隔

	QDateTime	m_lastDateTimeL1;		//上一次严重播报的时间
	QDateTime	m_lastDateTimeL2;		//上一次普通播报的时间

private:
	bool	m_bOpened;
};

typedef QSharedObjectPtr<QAbstractAlarm> QAbstractAlarmPtr;

typedef void (*CreateAlarmInstanceFunc)(QVector<QAbstractAlarmPtr>&);		//预警系统动态库必须提供的接口

#endif // QABSTRACTALARM_H
