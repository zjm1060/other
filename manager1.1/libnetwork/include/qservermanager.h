#ifndef QSERVERMANAGER_H
#define QSERVERMANAGER_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QMutex>
#include <QTimer>
#include <QMultiHash>
#include <QMetaType>

#include "qutildef.h"
#include "qserver.h"

class NETWORK_EXPORT QServerManager : public QObject
{
	Q_OBJECT

public:
	~QServerManager();

	enum RegisterStatus
	{
		RegisterSuccess=0,
		RegisterFail,
		ServerExisting
	};

	typedef struct _localServer
	{
		ServerInfo	info;	//本地网段信息
		quint16		port;	//本地端口号

		ServerInfo	server;	//本地服务器信息
		QList<QServerPtr>	servers;		//本地服务器列表
	}LocalServer;

	typedef struct _remoteServer 
	{
		ServerInfo	server;

		int	heartbeat;		//心跳超时次数，当超时次数大于设定次数后，表示其服务器已退出
	}RemoteServer;

protected:
	QServerManager(QObject *parent=0);

public:
	static QServerManager*	instance();
	static void destroy();

	static bool checkConnect(const ServerInfo& source, const ServerInfo& target, Server* sourceServer, Server* targetServer);
	static bool findFirstServer(const ServerInfo& info, Server* server);
	static bool canConnected(const ServerInfo& info, const Server& server);

public:
	QServerManager::RegisterStatus registerServer(const QString& hardwareAddr, const Server& server);
	QServerManager::RegisterStatus registerLocalServer();	//用于进程间通信
	void unregisterServer(const QString& hardwareAddr, const Server& server);

	void unregisterAll();

	void addRemoteServer(const ServerInfo& serverInfo);

public:
	const ServerInfo localServer() const;
	const ServerInfo& localAddr() const;
	const QList<QServerPtr>& localServerList() const;

	const QMultiHash<QString, RemoteServer>& remoteServer() const;

	void checkLocalAddress();	//检测本地网络
	void setPort(quint16 port);

public:
	void informStatusChanged(quint64 sign);
	
signals:
	void remoteServerComing(const ServerInfo& server);
	void remoteServerQuit(const ServerInfo& server);

private slots:
	void timeout();

private:
	LocalServer		m_localServer;
	
	QServerPtr		m_pLocalServer;			//回环地址服务器,用于进程间通讯

	QTimer			m_timer;		//超时定时器
	QMultiHash<QString, RemoteServer>	m_multiHashRemoteServer;	//远程可用服务器列表

private:
	QMutex	m_mutex;

private:
	static QMutex				m_smutex;
	static QServerManager*		m_pServerManager;
};

#endif // QSERVERMANAGER_H
