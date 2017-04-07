#include "qmessageddb.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"
#include "qddbwarehouse.h"
#include "qprivilegemanager.h"
#include "qglobalinfo.h"

QMessageDdb::QMessageDdb(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QMap<int, quint32> >("QMap<int, quint32>");
	qRegisterMetaType<QMap<QString, PrivilegeGroup> >("QMap<QString, PrivilegeGroup>");
	qRegisterMetaType<PrivilegeGroups>("PrivilegeGroups");
	qRegisterMetaType<PrivilegeUser>("PrivilegeUser");
}

QMessageDdb::~QMessageDdb()
{

}

QByteArray QMessageDdb::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<msgType<<usrID;

	if (!contents.isEmpty())
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageDdb::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageDdbRunnable* pRunnable=new QMessageDdbRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageDdb::finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (msgType)
	{
	case QueryInformDdbConnection:
		in<<AnswerInformDdbConnection;
		break;
	case QueryUploadPrivilegeData:
		in<<AnswerUploadPrivilegeData;
		break;
	case QueryCheckPrivilege:
		in<<AnswerCheckPrivilege;
		break;
	case QueryPrivileges:
		in<<AnswerPrivileges;
		break;
	case QueryCreateGroup:
		in<<AnswerCreateGroup;
		break;
	case QueryDeleteGroup:
		in<<AnswerDeleteGroup;
		break;
	case QueryCreateUser:
		in<<AnswerCreateUser;
		break;
	case QueryDeleteUser:
		in<<AnswerDeleteUser;
		break;
	case QueryModifyUserPrivilege:
		in<<AnswerModifyUserPrivilege;
		break;
	case QueryModifyUserPassword:
		in<<AnswerModifyUserPassword;
		break;
	case UpdateUserPrivilege:
		in<<UpdateUserPrivilege;
		break;
	default:
		break;
	}

	in<<usrID;

	if (!buffer.isEmpty())
	{
		in.writeRawData(buffer.data(), buffer.size());
	}

	QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
	if (ptr != NULL)
	{
		QMetaObject::invokeMethod(sender, "sendMessage", Q_ARG(QByteArray, buf), Q_ARG(int, Type));
	}
}

QMessageDdbRunnable::QMessageDdbRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageDdbRunnable::~QMessageDdbRunnable()
{

}

void QMessageDdbRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageDdbRunnable::run()
{
	int nMsgType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case QueryInformDdbConnection:
		{
			QString uid;
			out>>uid;

			QDdbWarehouse::instance()->insert(m_pSender, nUsrID, uid);

			emit finished(m_pSender, QueryInformDdbConnection, nUsrID, "");
		}
		break;
	case AnswerInformDdbConnection:
		{
			QMetaObject::invokeMethod(m_pView, "recvInformDdbConnection", Q_ARG(quint32, nUsrID));
		}
		break;
	case QueryUploadPrivilegeData:
		{
			PrivilegeGroups groups;
			bool isRetData;

			out>>groups>>isRetData;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

			bool bChanged=QDdbWarehouse::instance()->updatePrivilegeData(groups);
			if (bChanged)
			{
				PrivilegeGroups grps=QDdbWarehouse::instance()->privilegeData();
				stream<<PrivilegeDataModified<<grps.groups;

				if (!isRetData)
				{
					isRetData=QDdbWarehouse::instance()->updatePrivilegeData(groups);
				}

				QMap<QObject*, QPair<quint32, QString> > senders=QDdbWarehouse::instance()->connections();
				QMapIterator<QObject*, QPair<quint32, QString> > it(senders);
				while (it.hasNext())
				{
					it.next();

					if (isRetData || it.key() != m_pSender || nUsrID != it.value().first)
					{
						emit finished(it.key(), QueryUploadPrivilegeData, it.value().first, buffer);
					}
				}
			}
			else
			{
				stream<<PrivilegeDataUnmodified;
				emit finished(m_pSender, QueryUploadPrivilegeData, nUsrID, buffer);
			}
		}
		break;
	case AnswerUploadPrivilegeData:
		{
			int nFlag;
			out>>nFlag;

			QMap<QString, PrivilegeGroup> groups;
			if (nFlag == PrivilegeDataModified)
			{
				out>>groups;
			}

			QMetaObject::invokeMethod(m_pView, "recvPrivilegeData", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag),
				Q_ARG_PAIR(QMap, QString, PrivilegeGroup, groups));

			if (nFlag == PrivilegeDataModified)
			{
				//向所有修改过权限的用户发送修改信息
				QMap<QObject*, QString> users=QConnectionManager::instance()->loginUsers();
				QMapIterator<QObject*, QString> it(users);
				while (it.hasNext())
				{
					it.next();

					const QString& userName=it.value();

					bool bContains=false;

					QMapIterator<QString, PrivilegeGroup> iter(groups);
					while (iter.hasNext())
					{
						iter.next();

						const PrivilegeGroup& group=iter.value();
						if (group.usrs.contains(userName))
						{
							const PrivilegeUser& user=group.usrs[userName];

							Q_WRITE_ONLY_DATA_STREAM(s, buf);
							s<<UserPrivilegesChanged<<user;

							emit finished(it.key(), UpdateUserPrivilege, -1, buf);

							bContains=true;

							break;
						}
					}

					if (!bContains)
					{
						Q_WRITE_ONLY_DATA_STREAM(s, buf);
						s<<UserDeleted<<userName;

						emit finished(it.key(), UpdateUserPrivilege, -1, buf);
					}
				}
			}
		}
		break;
	case QueryCheckPrivilege:
		{
			QString strName, strPasswd;
			out>>strName>>strPasswd;

			PrivilegeUser user;
			int nRet=QPrivilegeManager::instance()->checkPrivilege(strName, strPasswd, user);
			
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<nRet;
			if (nRet == PrivilegeCheckPass)
			{
				QConnectionManager::instance()->insertLoginUser(m_pSender, strName);		//记录当前登录用户

				stream<<user;
			}

			emit finished(m_pSender, QueryCheckPrivilege, nUsrID, buffer);
		}
		break;
	case AnswerCheckPrivilege:
		{
			int nFlag;
			out>>nFlag;

			PrivilegeUser user;

			if (nFlag == PrivilegeCheckPass)
			{
				out>>user;
			}

			QMetaObject::invokeMethod(m_pView, "recvCheckPrivilege", Q_ARG(quint32, nUsrID), Q_ARG(int, nFlag), Q_ARG(PrivilegeUser, user));
		}
		break;
	case QueryPrivileges:
		{
			PrivilegeGroups groups=QPrivilegeManager::instance()->privilegeData();

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<groups;

			emit finished(m_pSender, QueryPrivileges, nUsrID, buffer);
		}
		break;
	case AnswerPrivileges:
		{
			PrivilegeGroups groups;

			out>>groups;

			QMetaObject::invokeMethod(m_pView, "recvPrivileges", Q_ARG(quint32, nUsrID), Q_ARG(PrivilegeGroups, groups));
		}
		break;
	case QueryCreateGroup:
		{
			QString strGroupName;

			out>>strGroupName;

			int flag=QPrivilegeManager::instance()->createGroup(strGroupName);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<flag<<strGroupName<<QPrivilegeManager::instance()->privilegeData();

			emit finished(m_pSender, QueryCreateGroup, nUsrID, buffer);
		}
		break;
	case AnswerCreateGroup:
		{
			int flag;
			QString strGroupName;
			PrivilegeGroups groups;

			out>>flag>>strGroupName>>groups;

			QMetaObject::invokeMethod(m_pView, "recvCreateGroup", Q_ARG(quint32, nUsrID), Q_ARG(int, flag), Q_ARG(QString, strGroupName),
				Q_ARG(PrivilegeGroups, groups));
		}
		break;
	case QueryDeleteGroup:
		{
			QString strGroupName;
			out>>strGroupName;

			QPrivilegeManager::instance()->deleteGroup(strGroupName);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<strGroupName<<QPrivilegeManager::instance()->privilegeData();

			emit finished(m_pSender, QueryDeleteGroup, nUsrID, buffer);

			//向所有属于删除组的登录用户，发送用户删除消息
			QMap<QObject*, QString> users=QConnectionManager::instance()->loginUsers();
			QMapIterator<QObject*, QString> it(users);
			while (it.hasNext())
			{
				it.next();

				const QString& userName=it.value();

				if (!QPrivilegeManager::instance()->containsUser(userName))
				{
					Q_WRITE_ONLY_DATA_STREAM(s, buf);
					s<<UserDeleted<<userName;

					emit finished(it.key(), UpdateUserPrivilege, -1, buf);
				}
			}
		}
		break;
	case AnswerDeleteGroup:
		{
			QString strGroupName;
			PrivilegeGroups groups;
			out>>strGroupName>>groups;

			QMetaObject::invokeMethod(m_pView, "recvDeleteGroup", Q_ARG(quint32, nUsrID), Q_ARG(QString, strGroupName), Q_ARG(PrivilegeGroups, groups));
		}
		break;
	case QueryCreateUser:
		{
			QString strGroupName;
			PrivilegeUser user;
			out>>strGroupName>>user;

			int flag=QPrivilegeManager::instance()->createUser(strGroupName, user);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<flag;

			if (flag == CreateUserExisted)
			{
				stream<<QPrivilegeManager::instance()->belongToGroup(user.name);
			}
			else
			{
				stream<<strGroupName;
			}

			stream<<user.name<<QPrivilegeManager::instance()->privilegeData();

			emit finished(m_pSender, QueryCreateUser, nUsrID, buffer);
		}
		break;
	case AnswerCreateUser:
		{
			int flag;
			QString group, user;
			PrivilegeGroups groups;

			out>>flag>>group>>user>>groups;

			QMetaObject::invokeMethod(m_pView, "recvCreateUser", Q_ARG(quint32, nUsrID), Q_ARG(int, flag), Q_ARG(QString, group),
				Q_ARG(QString, user), Q_ARG(PrivilegeGroups, groups));
		}
		break;
	case QueryDeleteUser:
		{
			QString strUserName;
			out>>strUserName;

			QPrivilegeManager::instance()->deleteUser(strUserName);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<strUserName<<QPrivilegeManager::instance()->privilegeData();

			emit finished(m_pSender, QueryDeleteUser, nUsrID, buffer);

			//向所有登录用户为删除用户，发送用户删除消息
			Q_WRITE_ONLY_DATA_STREAM(s, buf);
			s<<UserDeleted<<strUserName;

			QMap<QObject*, QString> users=QConnectionManager::instance()->loginUsers();
			QMapIterator<QObject*, QString> it(users);
			while (it.hasNext())
			{
				it.next();

				const QString& userName=it.value();
				if (userName == strUserName)
				{
					emit finished(it.key(), UpdateUserPrivilege, -1, buf);
				}
			}
		}
		break;
	case AnswerDeleteUser:
		{
			QString strUserName;
			PrivilegeGroups groups;
			out>>strUserName>>groups;

			QMetaObject::invokeMethod(m_pView, "recvDeleteUser", Q_ARG(quint32, nUsrID), Q_ARG(QString, strUserName), Q_ARG(PrivilegeGroups, groups));
		}
		break;
	case QueryModifyUserPrivilege:
		{
			PrivilegeUser user;
			out>>user;

			int flag=QPrivilegeManager::instance()->modifyUser(user);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<flag<<QPrivilegeManager::instance()->belongToGroup(user.name)<<user.name<<QPrivilegeManager::instance()->privilegeData();

			emit finished(m_pSender, QueryModifyUserPrivilege, nUsrID, buffer);

			if (flag == ModifyPrivilegeSuccess)
			{
				Q_WRITE_ONLY_DATA_STREAM(s, buf);
				s<<UserPrivilegesChanged<<QPrivilegeManager::instance()->user(user.name);

				QMap<QObject*, QString> users=QConnectionManager::instance()->loginUsers();
				QMapIterator<QObject*, QString> it(users);
				while (it.hasNext())
				{
					it.next();

					const QString& userName=it.value();
					if (userName == user.name)
					{
						emit finished(it.key(), UpdateUserPrivilege, -1, buf);
					}
				}
			}
		}
		break;
	case AnswerModifyUserPrivilege:
		{
			int flag;
			QString strGroupName, strUserName;
			PrivilegeGroups groups;
			out>>flag>>strGroupName>>strUserName>>groups;

			QMetaObject::invokeMethod(m_pView, "recvModifyUser", Q_ARG(quint32, nUsrID), Q_ARG(int, flag), Q_ARG(QString, strGroupName),
				Q_ARG(QString, strUserName), Q_ARG(PrivilegeGroups, groups));
		}
		break;
	case QueryModifyUserPassword:
		{
			QString strUserName, strPassword;
			out>>strUserName>>strPassword;

			int flag=QPrivilegeManager::instance()->modifyPassword(strUserName, strPassword);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);
			stream<<flag<<strUserName;

			if (flag == ModifyPasswordSuccess)
			{
				stream<<QPrivilegeManager::instance()->user(strUserName);
			}

			emit finished(m_pSender, QueryModifyUserPassword, nUsrID, buffer);
		}
		break;
	case AnswerModifyUserPassword:
		{
			int flag;
			QString strUserName;
			PrivilegeUser user;

			out>>flag>>strUserName;

			if (flag == ModifyPasswordSuccess)
			{
				out>>user;
			}

			QMetaObject::invokeMethod(m_pView, "recvModifyPassword", Q_ARG(quint32, nUsrID), Q_ARG(int, flag), Q_ARG(QString, strUserName),
				Q_ARG(PrivilegeUser, user));
		}
		break;
	case UpdateUserPrivilege:
		{
			int flag;
			
			out>>flag;

			switch (flag)
			{
			case UserPrivilegesChanged:
				{
					PrivilegeUser user;
					out>>user;

					PrivilegeUser oldUser=QGlobalInfo::instance()->user();
					if (oldUser.name == user.name)
					{
						QGlobalInfo::instance()->setUser(user);
					}
				}
				break;
			case UserDeleted:
				{
					QString strUserName;
					out>>strUserName;

					PrivilegeUser user=QGlobalInfo::instance()->user();
					if (user.name == strUserName)
					{
						//当前登录用户被删除
						QMetaObject::invokeMethod(QGlobalInfo::instance(), "recvUserDeleted", Q_ARG(QString, strUserName));
					}
				}
				break;
			case UserPasswordChanged:
				break;
			default:
				break;
			}
			
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageDdb) 
