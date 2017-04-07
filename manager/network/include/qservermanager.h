#ifndef QSERVERMANAGER_H
#define QSERVERMANAGER_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QMutex>
#include <QTimer>
#include <QMultiHash>
#include <QMetaType>

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

	typedef struct _server 
	{
		QString		addr;			//服务器ip地址
		quint32		netmask;		//子网掩码
		quint16		port;			//服务器端口号,使用同一端口
	}Server;

	typedef struct _serverInfo
	{
		QString		hostName;
		QMap<QString, QVector<Server> > servers;	//以物理地址作为key

		int	heartbeat;		//心跳超时次数，当超时次数大于十次后，表示其服务器已退出
		
		int	count;
	}ServerInfo;

protected:
	QServerManager(QObject *parent=0);

public:
	static QServerManager*	instance();
	static void destroy();

	static bool checkConnect(const QServerManager::ServerInfo& source, const QServerManager::ServerInfo& target,
		QString& sourceAddr, QString& targetAddr);

	static QString findFirstAddress(const QServerManager::ServerInfo& info, bool hasPort=false);

public:
	QServerManager::RegisterStatus registerServer(const QString& hardwareAddr, const Server& server);
	void unregisterServer(const QString& hardwareAddr, const Server& server);

	void unregisterAll();

	void addRemoteServer(const QServerManager::ServerInfo& serverInfo);

	bool canConnected(const Server& server);

public:
	const ServerInfo& localServer() const;
	const ServerInfo& localAddr() const;

	const QMultiHash<QString, QServerManager::ServerInfo>& remoteServer() const;

	void checkLocalAddress();	//检测本地网络
	
signals:
	void remoteServerComing(const QServerManager::ServerInfo& server);
	void remoteServerQuit(const QServerManager::ServerInfo& server);

private slots:
	void timeout();

private:
	static QMutex				m_mutex;
	static QServerManager*		m_pServerManager;

	ServerInfo		m_localAddr;		//本地网段信息

	ServerInfo		m_server;
	QList<QServer*>	m_listServer;		//本地服务器列表

	QTimer				m_timer;		//超时定时器
	QMultiHash<QString, ServerInfo>	m_multiHashRemoteServer;	//远程可用服务器列表
};

NETWORK_EXPORT bool operator==(const QServerManager::ServerInfo& source, const QServerManager::ServerInfo& target);
NETWORK_EXPORT QDataStream& operator<<(QDataStream& out, const QServerManager::ServerInfo& info);
NETWORK_EXPORT QDataStream& operator>>(QDataStream& in, QServerManager::ServerInfo& info);

Q_DECLARE_METATYPE(QServerManager::Server)
Q_DECLARE_METATYPE(QServerManager::ServerInfo)

#endif // QSERVERMANAGER_H
