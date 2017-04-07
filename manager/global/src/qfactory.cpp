#include "qfactory.h"

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

void QFactoryRegistryBase::registry(const QString& name, void* item)
{
	QMutexLocker locker(&m_mutex);
	if (m_data.contains(name))
	{
		deleteObject(m_data[name]);
	}

	m_data.insert(name, item);
}

void* QFactoryRegistryBase::item(const QString& name)
{
	QMutexLocker locker(&m_mutex);
	if (m_data.contains(name))
	{
		return m_data[name];
	}

	return NULL;
}

void QFactoryRegistryBase::clear()
{
	QMutexLocker locker(&m_mutex);

	QMapIterator<QString, void*> it(m_data);
	while (it.hasNext())
	{
		it.next();

		deleteObject(it.value());
	}

	m_data.clear();
}
