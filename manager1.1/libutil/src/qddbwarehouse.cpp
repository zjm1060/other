#include "qddbwarehouse.h"

#include "qlogger.h"
#include "qconnectionmanager.h"

QDdbWarehouse::QDdbWarehouse(QObject *parent)
	: QObject(parent)
{

}

QDdbWarehouse::~QDdbWarehouse()
{

}

QDdbWarehouse* QDdbWarehouse::instance()
{
	static QDdbWarehouse* pInstance=new QDdbWarehouse;

	return pInstance;
}

void QDdbWarehouse::clear()
{
	m_mutex.lock();

	QList<QObject*> keys=m_mapConnection.keys();
	m_mapConnection.clear();

	m_mutex.unlock();


	QListIterator<QObject*> it(keys);
	while (it.hasNext())
	{
		QObject* pSender=it.next();

		QObjectPtr ptr=QConnectionManager::instance()->connection(pSender);
		if (ptr != NULL)
		{
			QMetaObject::invokeMethod(pSender, "disconnected", Qt::QueuedConnection);
		}
	}
}

void QDdbWarehouse::insert(QObject* conn, quint32 usrID, const QString& uid)
{
	logplusDebug(LIBUTIL_LOGGER_STR, QString("Accept the connection from %1").arg(uid));

	QMutexLocker locker(&m_mutex);

	QPair<quint32, QString>& peer=m_mapConnection[conn];

	peer.first=usrID;
	peer.second=uid;
}

void QDdbWarehouse::remove(QObject* conn)
{
	QMutexLocker locker(&m_mutex);

	if (!m_mapConnection.contains(conn))
	{
		return;
	}

	m_mapConnection.remove(conn);
}

bool QDdbWarehouse::contains(QObject* conn) const
{
	return m_mapConnection.contains(conn);
}

QPair<quint32, QString>	QDdbWarehouse::connection(QObject* conn)
{
	QMutexLocker locker(&m_mutex);

	if (!m_mapConnection.contains(conn))
	{
		return QPair<quint32, QString>();
	}

	return m_mapConnection[conn];
}

QMap<QObject*, QPair<quint32, QString> > QDdbWarehouse::connections()
{
	QMutexLocker locker(&m_mutex);

	return m_mapConnection;
}

bool QDdbWarehouse::updatePrivilegeData(const PrivilegeGroups& data)
{
	QMutexLocker locker(&m_mutex);

	QMap<QString, PrivilegeGroup> groups=data.groups;

	bool bRet=false;

	QMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		if (!groups.contains(it.key()))
		{
			bRet=true;

			break;
		}

		const PrivilegeGroup& group=it.value();
		QMapIterator<QString, PrivilegeUser> iter(group.usrs);
		while (iter.hasNext())
		{
			iter.next();
			if (!groups[it.key()].usrs.contains(iter.key()))
			{
				bRet=true;
				break;
			}
		}

		if (bRet)
		{
			break;
		}
	}

	bRet|=updatePrivilegeOperations(groups, m_groups.operations);
	bRet|=updatePrivilegeOperations(m_groups.groups, data.operations);
	bRet|=updatePrivilegeGroups(data.groups);

	mergePrivilegeOperations(m_groups.operations, data.operations);

	return bRet;
}

const PrivilegeGroups& QDdbWarehouse::privilegeData() const
{
	return m_groups;
}

bool QDdbWarehouse::updatePrivilegeGroups(const QMap<QString, PrivilegeGroup>& groups)
{
	bool bRet=false;

	QMapIterator<QString, PrivilegeGroup> it(groups);
	while (it.hasNext())
	{
		it.next();

		const PrivilegeGroup& group=it.value();

		if (!m_groups.groups.contains(it.key()))
		{
			m_groups.groups.insert(it.key(), group);
			bRet=true;
		}
		else
		{
			PrivilegeGroup& g=m_groups.groups[it.key()];

			if (QDateTime::fromString(group.datetime, STANDARD_DATETIME_FORMAT) > QDateTime::fromString(g.datetime, STANDARD_DATETIME_FORMAT))
			{
				g.datetime=group.datetime;
				bRet=true;
			}

			QMapIterator<QString, PrivilegeUser> iter(group.usrs);
			while (iter.hasNext())
			{
				iter.next();

				const PrivilegeUser& usr=iter.value();

				if (!g.usrs.contains(iter.key()))
				{
					g.usrs.insert(iter.key(), usr);
					bRet=true;
				}
				else
				{
					PrivilegeUser& user=g.usrs[iter.key()];
					if (user.datetime.isNull() || 
						QDateTime::fromString(usr.datetime, STANDARD_DATETIME_FORMAT) > QDateTime::fromString(user.datetime, STANDARD_DATETIME_FORMAT))
					{
						user.passwd=usr.passwd;
						user.datetime=usr.datetime;
						user.privileges=usr.privileges;
						bRet=true;
					}
				}
			}
		}
	}

	return bRet;
}

bool QDdbWarehouse::updatePrivilegeOperations(QMap<QString, PrivilegeGroup>& groups, const QList<PrivilegeOperation>& operations)
{
	bool bRet=false;

	QListIterator<PrivilegeOperation> it(operations);
	while (it.hasNext())
	{
		const PrivilegeOperation& operation=it.next();

		switch (operation.operation)
		{
		case PrivilegeDeleteGroup:
			{
				if (groups.contains(operation.uid) && 
					QDateTime::fromString(groups[operation.uid].datetime, STANDARD_DATETIME_FORMAT) < QDateTime::fromString(operation.datetime, STANDARD_DATETIME_FORMAT))
				{
					groups.remove(operation.uid);

					bRet=true;
				}
			}
			break;
		case PrivilegeDeleteUser:
			{
				QStringList keys=operation.uid.split("/", QString::SkipEmptyParts);

				if (keys.size() < 2)
				{
					break;
				}

				if (groups.contains(keys[0]))
				{
					PrivilegeGroup& group=groups[keys[0]];
					if (group.usrs.contains(keys[1]) && 
						QDateTime::fromString(group.usrs[keys[1]].datetime, STANDARD_DATETIME_FORMAT) < QDateTime::fromString(operation.datetime, STANDARD_DATETIME_FORMAT))
					{
						group.usrs.remove(keys[1]);

						bRet=true;
					}
				}
			}
			break;
		default:
			break;
		}
	}

	return bRet;
}

void QDdbWarehouse::mergePrivilegeOperations(QList<PrivilegeOperation>& operations, const QList<PrivilegeOperation>& src)
{
	QListIterator<PrivilegeOperation> it(src);
	while (it.hasNext())
	{
		const PrivilegeOperation& operation=it.next();

		bool bFind=false;
		QMutableListIterator<PrivilegeOperation> iter(operations);
		iter.toBack();
		while (iter.hasPrevious())
		{
			PrivilegeOperation opera=iter.previous();

			if (opera.uid == operation.uid)
			{
				if (QDateTime::fromString(operation.datetime, STANDARD_DATETIME_FORMAT) < QDateTime::fromString(opera.datetime, STANDARD_DATETIME_FORMAT))
				{
					bFind=true;
				}
				else
				{
					iter.remove();
				}

				break;
			}
		}

		if (!bFind)
		{
			bool bInsert=false;
			while (iter.hasNext())
			{
				PrivilegeOperation& opera=iter.next();
				if (QDateTime::fromString(operation.datetime, STANDARD_DATETIME_FORMAT) < QDateTime::fromString(opera.datetime, STANDARD_DATETIME_FORMAT))
				{
					iter.insert(operation);

					bInsert=true;
					break;
				}
			}

			if (!bInsert)
			{
				operations.append(operation);
			}
		}
	}
}
