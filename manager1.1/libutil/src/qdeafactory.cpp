#include "qdeafactory.h"

#include <QLibrary>

#include "qlogger.h"

QDEAFactory::QDEAFactory()
{
}

QDEAFactory::~QDEAFactory()
{
	m_mapInstances.clear();
}

QDEAFactory* QDEAFactory::instance()
{
	static QDEAFactory* pInstance=new QDEAFactory;

	return pInstance;
}

bool QDEAFactory::registerInstance(const QIDEAPtr& algorithm)
{
	if (algorithm == 0)
	{
		return false;
	}

	if (!m_mapInstances.contains(algorithm->type()))
	{
		m_mapInstances.insert(algorithm->type(), algorithm);
	}

	return true;
}

QIDEAPtr QDEAFactory::dea(int type)
{
	return m_mapInstances.value(type, QIDEAPtr(NULL));
}

QIDEAPtr QDEAFactory::dea(const QString& desc)
{
	QIDEAPtr ptr;

	QMapIterator<int, QIDEAPtr > it(m_mapInstances);
	while (it.hasNext())
	{
		it.next();

		if (it.value()->desc() == desc)
		{
			ptr=it.value();

			break;
		}
	}

	return ptr;
}

bool QDEAFactory::loadLibrary(const QString& filename, const char* symbol)
{
	CreateDEAInstanceFunc func;

	QLibrary library(filename);

	if (!library.load())
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Load library \"%1\" failed!").arg(filename));
	}

	func=(CreateDEAInstanceFunc)library.resolve(symbol);
	
	if (func != 0)
	{
		logplusInfo(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QIDEAPtr> instances;
		func(instances);

		foreach(QIDEAPtr ptr, instances)
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
