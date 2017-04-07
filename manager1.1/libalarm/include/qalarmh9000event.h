#ifndef QALARMH9000EVENT_H
#define QALARMH9000EVENT_H

#include "global.h"
#include "qabstractalarm.h"
#include "qreadwritealarmfromxmlfile.h"

class ALARM_EXPORT QAlarmH9000Event : public QAbstractAlarm
{
public:
	QAlarmH9000Event();
	virtual ~QAlarmH9000Event();

public:
	enum {Type=H9000AlarmEvent};

public:
	virtual int type() const	{return Type;}

	virtual void initialize();

protected:
	virtual int trigger(const QVariant& var, bool L1, bool L2);

private:
	bool sendAlarm(const QString& path, int level, double value);

private:
	QReadH9000FileSystemAlarmFormXmlFile::AlarmConfigure	m_configure;
};

#endif // QABSTRACTALARM_H
