#include "qfactory.h"

#include "common.h"

QFactoryRegistryBase::QFactoryRegistryBase()
{

}

QFactoryRegistryBase::~QFactoryRegistryBase()
{
	
}

bool QFactoryRegistryBase::exists(const QString& name)
{
	QMutexLocker locker(&m_mutex);

	return m_data.contains(name);
}

QList<QString> QFactoryRegistryBase::allProductNames()
{
	QMutexLocker locker(&m_mutex);

	return m_data.keys();
}

void QFactoryRegistryBase::registry(const QString& name, QFactoryBasePtr item)
{
	QMutexLocker locker(&m_mutex);

	m_data.insert(name, item);
}

QFactoryBasePtr QFactoryRegistryBase::item(const QString& name)
{
	QMutexLocker locker(&m_mutex);
	if (m_data.contains(name))
	{
		return m_data[name];
	}

	return QFactoryBasePtr();
}

void QFactoryRegistryBase::clear()
{
	QMutexLocker locker(&m_mutex);

	m_data.clear();
}
