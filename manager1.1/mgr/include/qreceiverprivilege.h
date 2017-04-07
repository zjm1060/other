#ifndef QRECEIVERPRIVILEGE_H
#define QRECEIVERPRIVILEGE_H

#include <QObject>

#include "qabstractreceiver.h"

class QReceiverPrivilege : public QObject, public QAbstractReceiver
{
	Q_OBJECT

public:
	QReceiverPrivilege(QObject *parent=0);
	~QReceiverPrivilege();

public:
	void queryPrivileges();

	void queryCreateGroup(const QString& name);
	void queryDeleteGroup(const QString& name);
	void queryCreateUser(const QString& group, const PrivilegeUser& user);
	void queryDeleteUser(const QString& user);
	void queryModifyUserPrivileges(const PrivilegeUser& user);
	void queryModifyUserPassword(const QString& user, const QString& password);

public:
	virtual void recvPrivileges(const PrivilegeGroups& groups);
	virtual void recvCreateGroup(int flag, const QString& name, const PrivilegeGroups& groups);
	virtual void recvDeleteGroup(const QString& groupName, const PrivilegeGroups& groups);
	virtual void recvCreateUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups);
	virtual void recvDeleteUser(const QString& userName, const PrivilegeGroups& groups);
	virtual void recvModifyUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups);
	virtual void recvModifyPassword(int flag, const QString& userName, const PrivilegeUser& user);

signals:
	void receivePrivileges(const PrivilegeGroups groups);
	void createExistedGroup(const QString name, const PrivilegeGroups groups);
	void createGroupSuccess(const QString name, const PrivilegeGroups groups);
	void deleteGroupSuccess(const QString groupName, const PrivilegeGroups groups);
	void createUserSuccess(const QString groupName, const QString userName, const PrivilegeGroups groups);
	void createExistedUser(const QString groupName, const QString userName, const PrivilegeGroups groups);
	void createUserGroupNonexistent(const QString groupName, const QString usrName, const PrivilegeGroups groups);
	void deleteUserSuccess(const QString userName, const PrivilegeGroups groups);
	void modifyPrivilegeSuccess(const QString groupName, const QString usrName, const PrivilegeGroups groups);
	void modifyPrivilegeUserNonexistent(const QString usrName, const PrivilegeGroups groups);
	void modifyPasswordSuccess(const QString usrName, const PrivilegeUser user);
	void modifyPasswordUserNonexistent(const QString usrName);

private:
	quint32		m_nUsrID;
};

#endif // QRECEIVERPRIVILEGE_H
