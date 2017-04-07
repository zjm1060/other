#ifndef QMESSAGEDDB_H
#define QMESSAGEDDB_H

#include <QObject>
#include <QRunnable>

#include "qmessageextern.h"

/**
*	@Desc 分布式数据库信息传输格式
*	@Author zlb
*	@Time 2011-4-24
*
*	QueryInformDdbConnection:		QueryInformDdbConnection|用户ID|用户UID
*	AnswerInformDdbConnection		AnswerInformDdbConnection|用户ID
*	QueryUploadPrivilegeData		QueryUploadPrivilegeData|用户ID|组数据|是否需要返回数据
*	AnswerUploadPrivilegeData		AnswerUploadPrivilegeData|用户ID|组数据
*	QueryCheckPrivilege				QueryCheckPrivilege|用户ID|用户名|用户密码
*	AnswerCheckPrivilege			AnswerCheckPrivilege|用户ID|用户名|查询状态（|用户信息）
*	QueryPrivileges					QueryPrivileges|用户ID
*	AnswerPrivileges				AnswerPrivileges|用户ID|组数据
*	QueryCreateGroup				QueryCreateGroup|用户ID|用户组名
*	AnswerCreateGroup				AnswerCreateGroup|用户ID|创建状态|用户组名|用户组信息
*	QueryDeleteGroup				QueryDeleteGroup|用户ID|组名
*	AnswerDeleteGroup				AnswerDeleteGroup|用户ID|组名|用户组信息
*	QueryCreateUser					QueryCreateUser|用户ID|组名|用户信息
*	AnswerCreateUser				AnswerCreateUser|用户ID|创建状态|组名|用户名|用户组信息
*	QueryDeleteUser					QueryDeleteUser|用户ID|用户名
*	AnswerDeleteUser				AnswerDeleteUser|用户ID|用户名|用户组信息
*	QueryModifyUserPrivilege		QueryModifyUserPrivilege|用户ID|用户信息
*	AnswerModifyUserPrivilege		AnswerModifyUserPrivilege|用户ID|状态|组名|用户名|用户组信息
*	QueryModifyUserPassword			QueryModifyUserPassword|用户ID|用户名|用户更改密码
*	AnswerModifyUserPassword		AnswerModifyUserPassword|用户ID|状态|用户名(|用户信息)
*	UpdateUserPrivilege				UpdateUserPrivilege|用户ID(-1)|标志位(用户名|用户信息)
**/

class MSG_EXPORT QMessageDdb : public QObject, public QIMessage
{
	Q_OBJECT

public:
	QMessageDdb(QObject *parent=0);
	~QMessageDdb();

	enum {Type=MSG_DDB};

public:
	virtual int type() const	{return Type;}

public:
	virtual QByteArray query(int msgType, quint32 usrID=-1, const QByteArray& contents="");
	virtual bool parse(QObject* sender, QByteArray buffer, QObject* view=0);

private slots:
	void finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer);
};

class MSG_EXPORT QMessageDdbRunnable : public QObject, public QRunnable
{
	Q_OBJECT

public:
	QMessageDdbRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent=0);
	virtual ~QMessageDdbRunnable();

public:
	void setArg(QObject* sender, const QByteArray& buffer, QObject* view);
	virtual void run();

signals:
	void finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer);

private:
	QObject*		m_pSender;
	QByteArray		m_buffer;
	QObject*		m_pView;
};

#endif // QMESSAGEDDB_H
