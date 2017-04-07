#include "qalarmfactory.h"

#include <QLibrary>

#include "qlogger.h"

QAlarmFactory::QAlarmFactory()
{
}

QAlarmFactory::~QAlarmFactory()
{
	m_mapInstances.clear();
}

QAlarmFactory* QAlarmFactory::instance()
{
	static QAlarmFactory* pInstance=new QAlarmFactory;

	return pInstance;
}

bool QAlarmFactory::registerInstance(const QAbstractAlarmPtr& alarm)
{
	if (alarm == 0)
	{
		return false;
	}

	if (!m_mapInstances.contains(alarm->type()))
	{
		m_mapInstances.insert(alarm->type(), alarm);
	}

	return true;
}

QAbstractAlarmPtr QAlarmFactory::alarmInstance(int type)
{
	return m_mapInstances.value(type, QAbstractAlarmPtr(NULL));
}

void QAlarmFactory::alarm(const QVariant& var)
{
	QMapIterator<int, QAbstractAlarmPtr > it(m_mapInstances);
	while (it.hasNext())
	{
		it.next();

		QAbstractAlarmPtr ptr=it.value();
		if (ptr != NULL)
		{
			ptr->alarm(var);
		}
	}
}

bool QAlarmFactory::loadLibrary(const QString& filename, const char* symbol)
{
	CreateAlarmInstanceFunc func;

	QLibrary library(filename);

	if (!library.load())
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Load library \"%1\" failed!").arg(filename));
	}

	func=(CreateAlarmInstanceFunc)library.resolve(symbol);
	
	if (func != 0)
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QAbstractAlarmPtr> instances;
		func(instances);

		foreach(QAbstractAlarmPtr ptr, instances)
		{
			if (ptr != 0)
			{
				instance()->registerInstance(ptr);
			}
		}
	}
	else
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" failed! %3").arg(symbol).arg(filename)
			.arg(library.errorString()));
		return false;
	}

	return true;
}
