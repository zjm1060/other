#include "qreceiverprivilege.h"

#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qprocesssocket.h"
#include "qinfomanager.h"

QReceiverPrivilege::QReceiverPrivilege(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<PrivilegeGroups>("PrivilegeGroups");

	m_nUsrID=QReceiverParser::instance()->registerInterface(this);
}

QReceiverPrivilege::~QReceiverPrivilege()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
}

void QReceiverPrivilege::queryPrivileges()
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		QByteArray buffer=pInstance->query(QueryPrivileges, m_nUsrID);

		if (!QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to query privilege data! Make sure the service process is running!"));
		}
	}
}

void QReceiverPrivilege::queryCreateGroup(const QString& name)
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<name;
		buffer=pInstance->query(QueryCreateGroup, m_nUsrID, buffer);

		if (!QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to send query! Make sure the service process is running!"));
		}
	}
}

void QReceiverPrivilege::queryDeleteGroup(const QString& name)
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<name;
		buffer=pInstance->query(QueryDeleteGroup, m_nUsrID, buffer);

		if (!QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to send query! Make sure the service process is running!"));
		}
	}
}

void QReceiverPrivilege::queryCreateUser(const QString& group, const PrivilegeUser& user)
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<group<<user;
		buffer=pInstance->query(QueryCreateUser, m_nUsrID, buffer);

		if (!QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to send query! Make sure the service process is running!"));
		}
	}
}

void QReceiverPrivilege::queryDeleteUser(const QString& user)
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<user;
		buffer=pInstance->query(QueryDeleteUser, m_nUsrID, buffer);

		if (!QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to send query! Make sure the service process is running!"));
		}
	}
}

void QReceiverPrivilege::queryModifyUserPrivileges(const PrivilegeUser& user)
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<user;
		buffer=pInstance->query(QueryModifyUserPrivilege, m_nUsrID, buffer);

		if (!QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to send query! Make sure the service process is running!"));
		}
	}
}

void QReceiverPrivilege::queryModifyUserPassword(const QString& user, const QString& password)
{
	QIMessagePtr pInstance=QMessageParser::instance()->messageInstance(MSG_DDB);
	if (pInstance != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<user<<password;
		buffer=pInstance->query(QueryModifyUserPassword, m_nUsrID, buffer);

		if (!QProcessSocket::instance()->sendMessage(buffer, MSG_DDB))
		{
			QInfoManager::instance()->append(QInfoManager::Error, tr("Failed to send query! Make sure the service process is running!"));
		}
	}
}

void QReceiverPrivilege::recvPrivileges(const PrivilegeGroups& groups)
{
	emit receivePrivileges(groups);
}

void QReceiverPrivilege::recvCreateGroup(int flag, const QString& name, const PrivilegeGroups& groups)
{
	switch (flag)
	{
	case CreateGroupExisted:
		{
			emit createExistedGroup(name, groups);
		}
		break;
	case CreateGroupSuccess:
		{
			emit createGroupSuccess(name, groups);
		}
		break;
	default:
		break;
	}
}

void QReceiverPrivilege::recvDeleteGroup(const QString& groupName, const PrivilegeGroups& groups)
{
	emit deleteGroupSuccess(groupName, groups);
}

void QReceiverPrivilege::recvCreateUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups)
{
	switch (flag)
	{
	case CreateUserSuccess:
		emit createUserSuccess(groupName, userName, groups);
		break;
	case CreateUserExisted:
		emit createExistedUser(groupName, userName, groups);
		break;
	case CreateUserGroupNonexistent:
		emit createUserGroupNonexistent(groupName, userName, groups);
		break;
	default:
		break;
	}
}

void QReceiverPrivilege::recvDeleteUser(const QString& userName, const PrivilegeGroups& groups)
{
	emit deleteUserSuccess(userName, groups);
}

void QReceiverPrivilege::recvModifyUser(int flag, const QString& groupName, const QString& userName, const PrivilegeGroups& groups)
{
	switch (flag)
	{
	case ModifyPrivilegeUserNonexistent:
		emit modifyPrivilegeUserNonexistent(userName, groups);
		break;
	case ModifyPrivilegeSuccess:
		emit modifyPrivilegeSuccess(groupName, userName, groups);
		break;
	default:
		break;
	}
}

void QReceiverPrivilege::recvModifyPassword(int flag, const QString& userName, const PrivilegeUser& user)
{
	switch (flag)
	{
	case ModifyPasswordUserNonexistent:
		emit modifyPasswordUserNonexistent(userName);
		break;
	case ModifyPasswordSuccess:
		emit modifyPasswordSuccess(userName, user);
		break;
	default:
		break;
	}
}
