#include "qstatfactory.h"

#include "define.h"
#include "qlogger.h"

#include <QLibrary>

QStatFactory::QStatFactory()
{
}

QStatFactory::~QStatFactory()
{
}

QStatFactory* QStatFactory::instance()
{
	static QStatFactory* pInstance=new QStatFactory;

	return pInstance;
}

void QStatFactory::registerStat(QAbstractStatPtr& statistics)
{
	Q_ASSERT_NO_NULL(statistics);

	if (!m_mapStatistics.contains(statistics->type()))
	{
		m_mapStatistics.insert(statistics->type(), statistics);
	}
}

QAbstractStatPtr QStatFactory::statInstance(int type)
{
	return m_mapStatistics.value(type, QAbstractStatPtr(NULL));
}

QVariant QStatFactory::exec(int type)
{
	QAbstractStatPtr pStatistics=m_mapStatistics.value(type);
	if (pStatistics != NULL)
	{
		return pStatistics->exec();
	}

	return QVariant();
}

int QStatFactory::execMethod(int type, int method, const QStringList& arguments)
{
	QAbstractStatPtr pStatistics=m_mapStatistics.value(type);
	if (pStatistics != NULL)
	{
		return pStatistics->execMethod(method, arguments);
	}

	return QAbstractStat::ExecMethodInvalid;
}

bool QStatFactory::loadLibrary(const QString& filename, const char* symbol)
{
	CreateStatInstanceFunc func;

	func=(CreateStatInstanceFunc)QLibrary::resolve(filename, symbol);

	if (func != 0)
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QAbstractStatPtr> instances;
		func(instances);

		foreach(QAbstractStatPtr pWmi, instances)
		{
			if (pWmi != 0)
			{
				instance()->registerStat(pWmi);

				logplusDebug(LIBUTIL_LOGGER_STR, QString("Resolve QStatFactory type=%1 instances").arg(pWmi->type()));
			}
		}
	}
	else
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" failed!").arg(symbol).arg(filename));
		return false;
	}

	return true;
}
