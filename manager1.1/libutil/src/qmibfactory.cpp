#include "qmibfactory.h"

#include "define.h"
#include "qlogger.h"

#include <QLibrary>

QMibFactory::QMibFactory()
{
	clear();

	m_pSignalMapper=new QSignalMapper(this);

	connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(changed(int)));
}

QMibFactory::~QMibFactory()
{
	m_mapProducts.clear();

	SAFE_DELETE_CLASS(m_pSignalMapper);
}

QMibFactory* QMibFactory::instance()
{
	static QMibFactory* pInstance=new QMibFactory;

	return pInstance;
}

void QMibFactory::registerProduct(const QAbstractMibPtr& ptr)
{
	Q_ASSERT_NO_NULL(ptr);

	if (!m_mapProducts.contains(ptr->type()))
	{
		connect(ptr.get(), SIGNAL(changed()), m_pSignalMapper, SLOT(map()));
		m_pSignalMapper->setMapping(ptr.get(), ptr->type());

		m_mapProducts.insert(ptr->type(), ptr);
	}
}

QAbstractMibPtr QMibFactory::product(int type)
{
	return m_mapProducts.value(type, QAbstractMibPtr(NULL));
}

const QMap<int, QAbstractMibPtr>& QMibFactory::products() const
{
	return m_mapProducts;
}

bool QMibFactory::loadLibrary(const QString& filename, const char* symbol)
{
	CreateMibInstanceFunc func;

	QLibrary library(filename);
	func=(CreateMibInstanceFunc)library.resolve(symbol);

	if (func != 0)
	{
		logplusDebug(LIBUTIL_LOGGER_STR, QString("Resolve \"%1\" in library \"%2\" succeed.").arg(symbol).arg(filename));

		QVector<QAbstractMibPtr> instances;
		func(instances);

		foreach(QAbstractMibPtr pMib, instances)
		{
			if (pMib != 0)
			{
				instance()->registerProduct(pMib);
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

void QMibFactory::changed(int type)
{
	Q_ASSERT(type < UTIL_TYPE_END);

	QMutexLocker locker(&m_mutex);
	m_bChanged[type]=true;
}

void QMibFactory::clear()
{
	QMutexLocker locker(&m_mutex);

	memset(m_bChanged, 0, _countof(m_bChanged)*sizeof(bool));
}

quint64 QMibFactory::signs()
{
	QMutexLocker locker(&m_mutex);

	quint64 nSign=0;

	for (int i=0; i<UTIL_TYPE_END; i++)
	{
		if (m_bChanged[i])
		{
			nSign|=((quint64)1<<i);
		}
	}

	return nSign;
}
