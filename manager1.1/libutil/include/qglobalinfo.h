#ifndef QGLOBALINFO_H
#define QGLOBALINFO_H

#include <QMutex>
#include <QThreadPool>
#include <QTimer>

#include "global.h"
#include "qutildef.h"

class UTIL_EXPORT QGlobalInfo : public QObject
{
	Q_OBJECT

protected:
	QGlobalInfo(QObject* parent=0);

public:
	~QGlobalInfo();

	enum RunningFlags
	{
		EnableDebugFlag=1,
		EnableTopologyFlag=(1<<1)
	};

	enum DeaSettingsType
	{
		MessagesDea,		//报文加密
		HeartBeatsDea,		//心跳报文加密
		AccountsDea,		//账户文件加密
		DeaSettingsEnd
	};

public:
	static QGlobalInfo* instance();
	static QThreadPool* globalThreadPool();

public:
	void setRunningFlag(RunningFlags flag, bool enable);
	bool runningFlag(RunningFlags flag) const;

	int deaType(int type);

	void setLocal(const ServerInfo& info);
	ServerInfo	local();

	void setLogin(bool login);
	bool isLogin();

	void setUser(const PrivilegeUser& user);
	const PrivilegeUser user();

	QMap<int, quint32> privileges();

	bool hasPrivileges(int type, int flag);

public:
	void setServerList(const QList<QPair<ServerInfo, int> >& servers);
	const QList<QPair<ServerInfo, int> >	serverList();

public:
	Q_INVOKABLE void recvUserDeleted(const QString& user);

signals:
	void logout(const QString user);

private slots:
	void timeout();

private:
	void loadDea(const QString& key, const QString& defaultValue, int type);

private:
	QMutex		m_mutex;

	QTimer		m_timer;

	ServerInfo	m_local;	//本地服务器信息
	QList<QPair<ServerInfo, int> >	m_servers;	//远程服务器列表

	bool	m_bLogin;		//是否已经登入
	PrivilegeUser m_user;	//用户

	int		m_nDeaType[DeaSettingsEnd];		//报文加密标识

	quint64		m_nFlagRunning;		//运行标志
};

#endif // QGLOBALINFO_H
