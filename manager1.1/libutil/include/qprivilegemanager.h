#ifndef QPRIVILEGEMANAGER_H
#define QPRIVILEGEMANAGER_H

#include <QString>
#include <QMap>
#include <QList>
#include <QMutex>

#include "global.h"
#include "qsharedobject.h"
#include "qutildef.h"

class UTIL_EXPORT QPrivilegeManager : public QObject, public QSharedObject
{
	Q_OBJECT

public:
	~QPrivilegeManager();

protected:
	QPrivilegeManager();

public:
	static QPrivilegeManager* instance();

public:
	int checkPrivilege(const QString& userName, const QString& passwd, PrivilegeUser& user);

	int createGroup(const QString& name);
	void deleteGroup(const QString& name);

	int createUser(const QString& groupName, const PrivilegeUser& user);
	void deleteUser(const QString& name);

	int modifyUser(const PrivilegeUser& user);

	int modifyPassword(const QString& userName, const QString& password);

public:
	const PrivilegeGroups& privilegeData();
	const PrivilegeUser user(const QString& userName);
	bool containsUser(const QString& name);

	QString belongToGroup(const QString& user);

	void updatePrivilegeGroups(const QMap<QString, PrivilegeGroup>& groups);

	void setSynchronized(bool sync);
	bool isSynchronized();

public:
	bool write();
	bool read();

signals:
	void changed();

private:
	void setDefaultPrivilege();

private:
	QMutex	m_mutex;

	PrivilegeGroups m_groups;

	bool	m_bSynchronized;		//权限数据是否已经同步
};

typedef QSharedObjectPtr<QPrivilegeManager> QPrivilegeManagerPtr;

#endif // QPRIVILEGEMANAGER_H
