#include "qprivilegemanager.h"

#include "common.h"
#include "qfileex.h"
#include "qlogger.h"
#include "qdeafactory.h"
#include "qimessage.h"
#include "qglobalinfo.h"

QPrivilegeManager::QPrivilegeManager()
:m_bSynchronized(false)
{

}

QPrivilegeManager::~QPrivilegeManager()
{

}

QPrivilegeManager* QPrivilegeManager::instance()
{
	static QPrivilegeManager* pInstance=new QPrivilegeManager;

	return pInstance;
}

bool QPrivilegeManager::write()
{
	int nDeaType=QGlobalInfo::instance()->deaType(QGlobalInfo::AccountsDea);

	QIDEAPtr ptr=QDEAFactory::instance()->dea(nDeaType);

	if (ptr == NULL)
	{
		ptr=QDEAFactory::instance()->dea(TripleDES);
	}
	
	if (ptr == NULL)
	{
		logplusWarning(LIBUTIL_LOGGER_STR, "Failed to find DEA! Please to check your libdea library!");

		return false;
	}

	QFileEx file(appendPath(getAppParentDir(), HMAN_PRIVILEGE_FILE));
	if (!file.open(QIODevice::WriteOnly))
	{
		return false;
	}

	Q_IO_DEVICE_DATA_STREAM(out, file);
	out<<ptr->type();

	Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
	stream<<HMAN_PRIVILEGE_FILE_HEADER<<m_groups;

	QByteArray buf=ptr->encrypt(buffer);
	out<<buf.size();
	out.writeRawData(buf.data(), buf.size());

	return true;
}

bool QPrivilegeManager::read()
{
	QString strFileName=appendPath(getAppParentDir(), HMAN_PRIVILEGE_FILE);
	QFile file(strFileName);
	if (!file.exists() || !file.open(QIODevice::ReadOnly))
	{
		setDefaultPrivilege();
		return false;
	}

	Q_IO_DEVICE_DATA_STREAM(out, file);

	int nType, nSize;
	out>>nType;

	QIDEAPtr ptr=QDEAFactory::instance()->dea(nType);
	if (ptr == NULL)
	{
		logplusWarning(LIBUTIL_LOGGER_STR, "Failed to find DEA! Please to check your libdea library!");

		setDefaultPrivilege();
		return false;
	}

	out>>nSize;
	if (nSize < 0)
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("The file named %1 is an invalid privilege file!").arg(strFileName));
		
		setDefaultPrivilege();
		return false;
	}

	QByteArray buffer;
	buffer.resize(nSize);
	if (out.readRawData(buffer.data(), nSize) != nSize)
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("The file named %1 is an invalid privilege file!").arg(strFileName));
		
		setDefaultPrivilege();
		return false;
	}

	buffer=ptr->decrypt(buffer);

	Q_READ_ONLY_DATA_STREAM(stream, buffer);
	char* strHeader;
	stream>>strHeader;
	if (strHeader == NULL)
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("The file named %1 is an invalid privilege file!").arg(strFileName));

		setDefaultPrivilege();
		return false;
	}

	if (strcmp(strHeader, HMAN_PRIVILEGE_FILE_HEADER) != 0)
	{
		logplusError(LIBUTIL_LOGGER_STR, QString("The file named %1 is an invalid privilege file!").arg(strFileName));

		SAFE_DELETE_ARRAY(strHeader);
		
		setDefaultPrivilege();
		return false;
	}

	SAFE_DELETE_ARRAY(strHeader);

	stream>>m_groups;

	return true;
}

void QPrivilegeManager::setDefaultPrivilege()
{
	m_groups.groups.clear();

	PrivilegeGroup group;

	group.name=HMAN_PRIVALEGE_DEFAULT_GROUP_NAME;
	group.datetime=QDateTime().toString(STANDARD_DATETIME_FORMAT);

	PrivilegeUser usr;
	usr.datetime=QDateTime().toString(STANDARD_DATETIME_FORMAT);
	usr.name=HMAN_PRIVALEGE_DEFAULT_USER_NAME;
	usr.passwd=HMAN_PRIVALEGE_DEFAULT_USER_PASSWD;
	usr.privileges.insert(PrivilegeRole, AdministratorRole);

	group.usrs.insert(usr.name, usr);
	m_groups.groups.insert(group.name, group);
}

const PrivilegeGroups& QPrivilegeManager::privilegeData()
{
	QMutexLocker locker(&m_mutex);

	return m_groups;
}

void QPrivilegeManager::updatePrivilegeGroups(const QMap<QString, PrivilegeGroup>& groups)
{
	if (!isSynchronized())
	{
		return;
	}

	QMutexLocker locker(&m_mutex);

	m_groups.groups=groups;

	write();	//写文件
}

void QPrivilegeManager::setSynchronized(bool sync)
{
	QMutexLocker locker(&m_mutex);
	m_bSynchronized=sync;
}

bool QPrivilegeManager::isSynchronized()
{
	QMutexLocker locker(&m_mutex);
	return m_bSynchronized;
}

int QPrivilegeManager::createGroup(const QString& name)
{
	if (m_groups.groups.contains(name))
	{
		return CreateGroupExisted;
	}

	m_mutex.lock();

	PrivilegeGroup group;
	group.name=name;
	group.datetime=QDateTime::currentDateTime().toString(STANDARD_DATETIME_FORMAT);

	m_groups.groups.insert(name, group);

	write();	//写文件

	m_mutex.unlock();

	emit changed();

	return CreateGroupSuccess;
}

void QPrivilegeManager::deleteGroup(const QString& name)
{
	if (!m_groups.groups.contains(name))
	{
		return;
	}

	m_mutex.lock();

	m_groups.groups.remove(name);

	QMutableListIterator<PrivilegeOperation> it(m_groups.operations);
	while (it.hasNext())
	{
		PrivilegeOperation& opera=it.next();
		if (opera.uid == name && opera.operation == PrivilegeDeleteGroup)
		{
			it.remove();

			break;
		}
	}

	PrivilegeOperation opera;
	opera.uid=name;
	opera.operation=PrivilegeDeleteGroup;
	opera.datetime=QDateTime::currentDateTime().toString(STANDARD_DATETIME_FORMAT);
	m_groups.operations.append(opera);

	write();	//写文件

	m_mutex.unlock();

	emit changed();
}

int QPrivilegeManager::createUser(const QString& groupName, const PrivilegeUser& user)
{
	if (!m_groups.groups.contains(groupName))
	{
		return CreateUserGroupNonexistent;
	}

	m_mutex.lock();

	QMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		const PrivilegeGroup& group=it.value();
		if (group.usrs.contains(user.name))
		{
			m_mutex.lock();
			return CreateUserExisted;
		}
	}

	PrivilegeGroup& group=m_groups.groups[groupName];
	PrivilegeUser usr=user;
	usr.datetime=QDateTime::currentDateTime().toString(STANDARD_DATETIME_FORMAT);

	group.usrs.insert(usr.name, usr);

	write();	//写文件

	m_mutex.unlock();

	emit changed();

	return CreateUserSuccess;
}

void QPrivilegeManager::deleteUser(const QString& name)
{
	m_mutex.lock();

	QMutableMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		PrivilegeGroup& group=it.value();
		if (group.usrs.contains(name))
		{
			PrivilegeOperation opera;
			opera.uid=QString("%1/%2").arg(group.name).arg(name);
			opera.operation=PrivilegeDeleteUser;
			opera.datetime=QDateTime::currentDateTime().toString(STANDARD_DATETIME_FORMAT);
			m_groups.operations.append(opera);

			group.usrs.remove(name);

			break;
		}
	}

	write();	//写文件

	m_mutex.unlock();

	emit changed();
}

int QPrivilegeManager::modifyUser(const PrivilegeUser& user)
{
	m_mutex.lock();

	int nRet=ModifyPrivilegeUserNonexistent;

	QMutableMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		PrivilegeGroup& group=it.value();
		if (group.usrs.contains(user.name))
		{
			nRet=ModifyPrivilegeSuccess;

			PrivilegeUser& usr=group.usrs[user.name];
			usr=user;
			usr.datetime=QDateTime::currentDateTime().toString(STANDARD_DATETIME_FORMAT);

			break;
		}
	}

	m_mutex.unlock();

	if (nRet == ModifyPrivilegeSuccess)
	{
		write();	//写文件

		emit changed();
	}

	return nRet;
}

int QPrivilegeManager::modifyPassword(const QString& userName, const QString& password)
{
	m_mutex.lock();

	int nRet=ModifyPasswordUserNonexistent;

	QMutableMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		PrivilegeGroup& group=it.value();
		if (group.usrs.contains(userName))
		{
			nRet=ModifyPasswordSuccess;

			group.usrs[userName].passwd=password;
			group.usrs[userName].datetime=QDateTime::currentDateTime().toString(STANDARD_DATETIME_FORMAT);

			break;
		}
	}

	m_mutex.unlock();

	if (nRet == ModifyPasswordSuccess)
	{
		write();	//写文件

		emit changed();
	}

	return nRet;
}

const PrivilegeUser QPrivilegeManager::user(const QString& userName)
{
	QMutexLocker locker(&m_mutex);

	PrivilegeUser usr;

	QMutableMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		PrivilegeGroup& group=it.value();
		if (group.usrs.contains(userName))
		{
			usr=group.usrs[userName];

			break;
		}
	}

	return usr;
}

bool QPrivilegeManager::containsUser(const QString& name)
{
	QMutexLocker locker(&m_mutex);

	QMutableMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		PrivilegeGroup& group=it.value();
		if (group.usrs.contains(name))
		{
			return true;
		}
	}

	return false;
}

QString QPrivilegeManager::belongToGroup(const QString& user)
{
	QMutexLocker locker(&m_mutex);

	QMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		const PrivilegeGroup& group=it.value();
		if (group.usrs.contains(user))
		{
			return group.name;
		}
	}

	return "";
}

int QPrivilegeManager::checkPrivilege(const QString& userName, const QString& passwd, PrivilegeUser& user)
{
	if (!isSynchronized())
	{
		return PrivilegeUncheckable;
	}

	QMutexLocker locker(&m_mutex);

	int nRet=PrivilegeUserNoExisted;

	QMapIterator<QString, PrivilegeGroup> it(m_groups.groups);
	while (it.hasNext())
	{
		it.next();

		const PrivilegeGroup& group=it.value();
		QMapIterator<QString, PrivilegeUser> iter(group.usrs);
		while (iter.hasNext())
		{
			iter.next();

			const PrivilegeUser& usr=iter.value();
			if (usr.name == userName)
			{
				nRet=(usr.passwd == passwd? (user=usr, PrivilegeCheckPass):PrivilegePasswdError);
			}
		}
	}

	return nRet;
}
