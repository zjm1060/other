#include "qglobalinfo.h"

#include "common.h"
#include "qdeafactory.h"

QGlobalInfo::QGlobalInfo(QObject* parent/* =0 */)
:QObject(parent)
,m_bLogin(false)
,m_nFlagRunning((quint64)-1)
{
#ifndef QT_DEBUG
	m_nFlagRunning&=~EnableDebugFlag;
#endif

	m_nDeaType[MessagesDea]=PlainText;
	m_nDeaType[HeartBeatsDea]=DES;
	m_nDeaType[AccountsDea]=TripleDES;

	timeout();

	m_timer.setInterval(5000);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));

	m_timer.start();
}

QGlobalInfo::~QGlobalInfo()
{

}

QGlobalInfo* QGlobalInfo::instance()
{
	static QGlobalInfo* pInstance=new QGlobalInfo;

	return pInstance;
}

int QGlobalInfo::deaType(int type)
{
	Q_ASSERT(type >=0 && type < DeaSettingsEnd);

	return m_nDeaType[type];
}

void QGlobalInfo::timeout()
{
	loadDea("dea/messages", "PlainText", MessagesDea);
	loadDea("dea/heartBeats", "DES", HeartBeatsDea);
	loadDea("dea/accounts", "TripleDES", AccountsDea);
}

void QGlobalInfo::loadDea(const QString& key, const QString& defaultValue, int type)
{
	QString strDea=parseConfig(getAppParentDir()+INI_FILE, key, defaultValue).toString();
	QIDEAPtr ptr=QDEAFactory::instance()->dea(strDea);
	if (ptr == NULL)
	{
		ptr=QDEAFactory::instance()->dea(defaultValue);
	}

	if (ptr != NULL)
	{
		m_nDeaType[type]=ptr->type();
	}
}

void QGlobalInfo::setRunningFlag(RunningFlags flag, bool enable)
{
	enable? m_nFlagRunning|=flag:m_nFlagRunning&=~flag;
}

bool QGlobalInfo::runningFlag(RunningFlags flag) const
{
	return (m_nFlagRunning&flag) != 0;
}

bool QGlobalInfo::hasPrivileges(int type, int flag)
{
	if (!isLogin())
	{
		return false;
	}

	QMutexLocker locker(&m_mutex);
	return checkPrivileges(m_user.privileges, type, flag);
}

void QGlobalInfo::recvUserDeleted(const QString& user)
{
	emit logout(user);
}

void QGlobalInfo::setLocal(const ServerInfo& info)
{
	QMutexLocker locker(&m_mutex);
	m_local=info;
}

ServerInfo QGlobalInfo::local()
{
	QMutexLocker locker(&m_mutex);
	return m_local;
}

void QGlobalInfo::setLogin(bool login)
{
	QMutexLocker locker(&m_mutex);
	m_bLogin=login;
}

bool QGlobalInfo::isLogin()
{
	QMutexLocker locker(&m_mutex);
	return m_bLogin;
}

void QGlobalInfo::setUser(const PrivilegeUser& user)
{
	if (!isLogin())
	{
		return;
	}

	QMutexLocker locker(&m_mutex);
	m_user=user;
}

const PrivilegeUser QGlobalInfo::user()
{
	QMutexLocker locker(&m_mutex);
	return m_user;
}

QMap<int, quint32> QGlobalInfo::privileges()
{
	if (!isLogin())
	{
		return QMap<int, quint32>();
	}

	QMutexLocker locker(&m_mutex);
	return m_user.privileges;
}

void QGlobalInfo::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (!isLogin())
	{
		return ;
	}

	QMutexLocker locker(&m_mutex);
	m_servers=servers;
}

const QList<QPair<ServerInfo, int> >	QGlobalInfo::serverList()
{
	if (!isLogin())
	{
		return QList<QPair<ServerInfo, int> >();
	}

	QMutexLocker locker(&m_mutex);
	return m_servers;
}

QThreadPool* QGlobalInfo::globalThreadPool()
{
	return QThreadPool::globalInstance();
}
