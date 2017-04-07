#include "qconnectionmanager.h"

//#include "qddbwarehouse.h"

QConnectionManager::QConnectionManager(QObject *parent)
	: QObject(parent)
{

}

QConnectionManager::~QConnectionManager()
{

}

QConnectionManager* QConnectionManager::instance()
{
	static QConnectionManager* pInstance=new QConnectionManager;

	return pInstance;
}

bool QConnectionManager::isEmpty(QObject* server)
{
	QMutexLocker locker(&m_mutex);

	if (!m_mapConnections.contains(server))
	{
		return true;
	}

	return m_mapConnections[server].isEmpty();
}

void QConnectionManager::insert(QObject* server, QObject* key, QSharedObject* connection, bool highRate)
{
	QMutexLocker locker(&m_mutex);

	QHash<QObject*, ConnectionParam>& connections=m_mapConnections[server];

	if (!connections.contains(key))
	{
		ConnectionParam param;
		param.connection=QObjectPtr(connection);
		param.highRate=highRate;

		connections.insert(key, param);
	}
	else
	{
		ConnectionParam& param=connections[key];
		param.connection=QObjectPtr(connection);
		param.highRate=highRate;
	}
}

void QConnectionManager::remove(QObject* server, QObject* connection)
{
	m_mutex.lock();

	if (!m_mapConnections.contains(server))
	{
		m_mutex.unlock();
		return;
	}

	QHash<QObject*, ConnectionParam>& connections=m_mapConnections[server];

	if (connections.contains(connection))
	{
		connections.remove(connection);

		m_mutex.unlock();

		removeLoginUser(connection);

//		QDdbWarehouse::instance()->remove(connection);
	}
	else
	{
		m_mutex.unlock();
	}
}

bool QConnectionManager::contains(QObject* server, QObject* connection)
{
	QMutexLocker locker(&m_mutex);

	if (!m_mapConnections.contains(server))
	{
		return false;
	}

	return m_mapConnections[server].contains(connection);
}

QList<QConnectionManager::ConnectionParam>	QConnectionManager::connections(QObject* server)
{
	QMutexLocker locker(&m_mutex);

	if (!m_mapConnections.contains(server))
	{
		return QList<ConnectionParam>();
	}

	return m_mapConnections[server].values();
}

QObjectPtr QConnectionManager::connection(QObject* key)
{
	QMutexLocker locker(&m_mutex);

	QMapIterator<QObject*, QHash<QObject*, ConnectionParam> > it(m_mapConnections);
	while (it.hasNext())
	{
		it.next();

		const QHash<QObject*, ConnectionParam>& connections=it.value();
		if (connections.contains(key))
		{
			return connections[key].connection;
		}
	}

	return QObjectPtr(NULL);
}

bool QConnectionManager::canHighRate(QObject* key)
{
	QMutexLocker locker(&m_mutex);

	QMapIterator<QObject*, QHash<QObject*, ConnectionParam> > it(m_mapConnections);
	while (it.hasNext())
	{
		it.next();

		const QHash<QObject*, ConnectionParam>& connections=it.value();
		if (connections.contains(key))
		{
			return connections[key].highRate;
		}
	}

	return false;
}

void QConnectionManager::insertLoginUser(QObject* sender, const QString& loginName)
{
	QMutexLocker locker(&m_mutex);

	m_loginUsers.insert(sender, loginName);
}

void QConnectionManager::removeLoginUser(QObject* sender)
{
	QMutexLocker locker(&m_mutex);

	m_loginUsers.remove(sender);
}

QMap<QObject*, QString> QConnectionManager::loginUsers()
{
	QMutexLocker locker(&m_mutex);

	return m_loginUsers;
}
