#ifndef QALARMFACTORY_H
#define QALARMFACTORY_H

#include <QMap>

#include "global.h"
#include "qabstractalarm.h"

class UTIL_EXPORT QAlarmFactory
{
public:
	virtual ~QAlarmFactory();

protected:
	QAlarmFactory();

public:
	bool registerInstance(const QAbstractAlarmPtr& ptr);

public:
	void alarm(const QVariant& var);
	QAbstractAlarmPtr alarmInstance(int type);

public:
	bool loadLibrary(const QString& filename, const char* symbol);

public:
	static QAlarmFactory* instance();

private:
	QMap<int, QAbstractAlarmPtr > m_mapInstances;
};

#endif // QALARMFACTORY_H
