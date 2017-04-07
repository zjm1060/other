#include "qservermanager.h"

#include <QStringList>
#include <QProcess>
#include <QNetworkInterface>

#include "common.h"
#include "qlogger.h"
#include "qutildef.h"

QMutex QServerManager::m_smutex;
QServerManager* QServerManager::m_pServerManager=0;

static const qint32 g_nCheckInterval=2000;

#define MAX_CHECK_TIMES 6

QServerManager::QServerManager(QObject *parent)
	: QObject(parent)
	,m_pLocalServer(NULL)
{
	m_localServer.info.hostName=m_localServer.server.hostName=getHostName();
	m_localServer.info.usrName=m_localServer.server.usrName=getUserName();
	m_localServer.info.home=m_localServer.server.home=getAppParentDir();

	checkLocalAddress();

	m_localServer.server.count=0;
	m_localServer.server.servers.clear();

	m_timer.setInterval(g_nCheckInterval);
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

QServerManager::~QServerManager()
{
	unregisterAll();
}

QServerManager* QServerManager::instance()
{
	if (m_pServerManager == 0)
	{
		m_smutex.lock();
		if (m_pServerManager == 0)
		{
			m_pServerManager=new QServerManager;
		}
		m_smutex.unlock();
	}

	return m_pServerManager;
}

void QServerManager::destroy()
{
	m_smutex.lock();

	SAFE_DELETE_CLASS(m_pServerManager);

	m_smutex.unlock();
}

void QServerManager::checkLocalAddress()
{
	QMutexLocker locker(&m_mutex);

	m_localServer.info.count=0;
	m_localServer.info.servers.clear();

#if defined(Q_OS_SOLARIS) || defined(Q_OS_AIX)
	QVector<NetworkAdapter> adapters=findNetworkAdapter();
	foreach (NetworkAdapter adapter, adapters)
	{
		if (adapter.IPAddress.isEmpty() || adapter.IPAddress.size() != adapter.IPSubnet.size())
		{
			continue;
		}

		Server server;
		QString strHardware;
		QVector<Server> servers;

		for (int i=0; i<adapter.IPAddress.size(); i++)
		{
			if (QHostAddress(adapter.IPAddress[i]) == QHostAddress::LocalHost)
			{
				continue;
			}

			server.addr=adapter.IPAddress[i];
			server.netmask=QHostAddress(adapter.IPSubnet[i]).toIPv4Address();
			server.port=m_localServer.port;
			if (server.netmask == 0)
			{
				server.netmask=QHostAddress("255.255.255.0").toIPv4Address();
			}

			logplusDebug(LIBNETWORK_LOGGER_STR, QString("Find IP address=%1, Netmask=%2").arg(server.addr).arg(QHostAddress(server.netmask).toString()));

			servers.append(server);
		}

		if (!servers.isEmpty())
		{
			strHardware=convertToStdMAC(adapter.MACAddress);

			logplusDebug(LIBNETWORK_LOGGER_STR, QString("Find MAC address=%1").arg(strHardware));

			if (!m_localServer.info.servers.contains(strHardware))
			{
				m_localServer.info.servers.insert(strHardware, servers);
			}
			else
			{
				m_localServer.info.servers[strHardware]+=servers;
			}

			m_localServer.info.count+=servers.size();
		}
	}

#else

	QList<QNetworkInterface> interfaces=QNetworkInterface::allInterfaces();
	QString strHardware;
	Server server;

	foreach (QNetworkInterface face, interfaces)
	{
		if ((face.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsLoopBack)) == 0)
		{
			continue;
		}

		QVector<Server> servers;

		QList<QNetworkAddressEntry> entries=face.addressEntries();
		foreach (QNetworkAddressEntry entry, entries)
		{
			if (entry.ip() == QHostAddress::LocalHost || entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
			{
				continue;
			}

			server.addr=entry.ip().toString();
			server.netmask=entry.netmask().toIPv4Address();
			server.port=m_localServer.port;
			if (server.netmask == 0)
			{
				server.netmask=QHostAddress("255.255.255.0").toIPv4Address();
			}

			logplusDebug(LIBNETWORK_LOGGER_STR, QString("Find IP address=%1, Netmask=%2").arg(server.addr).arg(QHostAddress(server.netmask).toString()));

			servers.append(server);
		}

		if (!servers.isEmpty())
		{
			strHardware=convertToStdMAC(face.hardwareAddress());
			if (strHardware.isEmpty())
			{
				strHardware=getMacAddress(face.name());
			}

			logplusDebug(LIBNETWORK_LOGGER_STR, QString("Find MAC address=%1").arg(strHardware));

			if (!m_localServer.info.servers.contains(strHardware))
			{
				m_localServer.info.servers.insert(strHardware, servers);
			}
			else
			{
				m_localServer.info.servers[strHardware]+=servers;
			}

			m_localServer.info.count+=servers.size();
		}
	}
#endif
}

QServerManager::RegisterStatus QServerManager::registerServer(const QString& hardwareAddr, const Server& server)
{
	QMutexLocker locker(&m_mutex);

	QServerPtr pServer;
	foreach(pServer, m_localServer.servers)
	{
		Q_ASSERT_NO_NULL(pServer);
		if (pServer->serverAddress().toString() == server.addr && pServer->serverPort() == server.port)
		{
			return ServerExisting;
		}
	}

	pServer=new QServer(server.addr, server.port, this);
	if (!pServer->onListen())
	{
		return RegisterFail;
	}

	logplusDebug(LIBNETWORK_LOGGER_STR, QString("Server: %1:%2 listening...").arg(server.addr).arg(server.port));

	m_localServer.servers.append(pServer);

	if (!m_localServer.server.servers.contains(hardwareAddr))
	{
		m_localServer.server.servers.insert(hardwareAddr, QVector<Server>());
	}

	QVector<Server>& servers=m_localServer.server.servers[hardwareAddr];
	servers.append(server);

	m_localServer.server.count++;

	return RegisterSuccess;
}

QServerManager::RegisterStatus QServerManager::registerLocalServer()
{
	QHostAddress localHostAddr=QHostAddress(QHostAddress::LocalHost);
	m_pLocalServer=QServerPtr(new QServer(localHostAddr, m_localServer.port, this));
	if (!m_pLocalServer->onListen())
	{
		logplusInfo(LIBNETWORK_LOGGER_STR, QString("Local Server:Register server %1:%2 failed! The port may have been used.")
			.arg(localHostAddr.toString()).arg(m_localServer.port));

		m_pLocalServer=NULL;
		return RegisterFail;
	}

	logplusInfo(LIBNETWORK_LOGGER_STR, QString("Local Server: %1:%2 listening...").arg(localHostAddr.toString()).arg(m_localServer.port));

	return RegisterSuccess;
}

void QServerManager::unregisterServer(const QString& hardwareAddr, const Server& server)
{
	QMutexLocker locker(&m_mutex);

	foreach (QServerPtr pServer, m_localServer.servers)
	{
		Q_ASSERT_NO_NULL(pServer);
		if (pServer->serverAddress().toString() == server.addr && pServer->serverPort() == server.port)
		{
			logplusInfo(LIBNETWORK_LOGGER_STR, QString("Server: %1:%2 closing...").arg(server.addr).arg(server.port));

			pServer->onClose();
			m_localServer.servers.removeOne(pServer);

			if (m_localServer.server.servers.contains(hardwareAddr))
			{
				QVector<Server>& servers=m_localServer.server.servers[hardwareAddr];
				for (int i=0; i<servers.size(); i++)
				{
					if (servers[i].addr == server.addr && servers[i].netmask == server.netmask && servers[i].port == server.port)
					{
						servers.remove(i);

						m_localServer.server.count--;
						break;
					}
				}
			}

			return;
		}
	}
}

void QServerManager::unregisterAll()
{
	QMutexLocker locker(&m_mutex);

	if (m_localServer.servers.isEmpty())
	{
		return;
	}

	logplusDebug(LIBNETWORK_LOGGER_STR, QString("All servers stop listening!!!"));

	foreach (QServerPtr pServer, m_localServer.servers)
	{
		pServer->onClose();
	}

	m_localServer.servers.clear();

	m_localServer.server.count=0;
	m_localServer.server.servers.clear();
}

const ServerInfo QServerManager::localServer() const
{
	if (m_localServer.server.count > 0)
	{
		return m_localServer.server;
	}
	
	ServerInfo info=m_localServer.server;

	Server server;
	server.addr=m_pLocalServer->serverAddress().toString();
	server.netmask=QHostAddress("255.255.255.0").toIPv4Address();
	server.port=m_pLocalServer->serverPort();

	info.servers["FF:FF:FF:FF:FF:FF"].append(server);

	info.count=1;

	return info;
}

const ServerInfo& QServerManager::localAddr() const
{
	return m_localServer.info;
}

const QList<QServerPtr>& QServerManager::localServerList() const
{
	return m_localServer.servers;
}

void QServerManager::addRemoteServer(const ServerInfo& serverInfo)
{
	QMutexLocker locker(&m_mutex);

	int nNumServer=m_multiHashRemoteServer.size();

	bool bExisted=false;

	QList<QString> keys=serverInfo.servers.keys();
	foreach (QString key, keys)
	{
		if (m_localServer.server.servers.contains(key))
		{
			bExisted=true;		//本机服务器
			break;
		}
	}

	if (!bExisted)
	{
		QMultiHash<QString, RemoteServer>::iterator it=m_multiHashRemoteServer.find(serverInfo.hostName);
		while (it != m_multiHashRemoteServer.end() && it.key() == serverInfo.hostName)
		{
			RemoteServer& info=it.value();
			foreach (QString key, keys)
			{
				if (info.server.servers.contains(key))
				{
					info.server=serverInfo;

					info.heartbeat=0;
					bExisted=true;

					break;
				}
			}
			
			if (bExisted)
			{
				break;
			}

			++it;
		}
	}

	RemoteServer info;
	info.server=serverInfo;
	info.heartbeat=0;

	if (!bExisted)
	{
		m_multiHashRemoteServer.insert(info.server.hostName, info);

		QString strLog="Receive heart beat packet:\n"+serverInfo.hostName+"-"+serverInfo.usrName;
		QMapIterator<QString, QVector<Server> > it(info.server.servers);
		while (it.hasNext())
		{
			it.next();

			const QVector<Server>& servers=it.value();
			for (int i=0; i<servers.size(); i++)
			{
				strLog+=QString("\n%1:%2").arg(servers[i].addr).arg(servers[i].port);
			}
		}
		
		logplusDebug(LIBNETWORK_LOGGER_STR, strLog);
	}

	emit remoteServerComing(info.server);

	if (nNumServer == 0 && m_multiHashRemoteServer.size() == 1)
	{
		if (!m_timer.isActive())
		{
			m_timer.start();
		}
	}
}

const QMultiHash<QString, QServerManager::RemoteServer>& QServerManager::remoteServer() const
{
	return m_multiHashRemoteServer;
}

void QServerManager::timeout()
{
	foreach(QString key, m_multiHashRemoteServer.keys())
	{
		QMultiHash<QString, RemoteServer>::iterator it=m_multiHashRemoteServer.find(key);
		while (it != m_multiHashRemoteServer.end() && it.key() == key)
		{
			if (it->heartbeat > MAX_CHECK_TIMES)
			{
				emit remoteServerQuit(it.value().server);

				it=m_multiHashRemoteServer.erase(it);
			}
			else
			{
				it->heartbeat++;
				it++;
			}
		}
	}

	if (m_multiHashRemoteServer.size() == 0)
	{
		m_timer.stop();
	}
}

bool QServerManager::canConnected(const ServerInfo& info, const Server& server)
{
	logplusDebug(LIBNETWORK_LOGGER_STR, QString("The client %1 try to connect to server!").arg(server.addr));

	foreach (QVector<Server> servers, info.servers)
	{
		foreach (Server s, servers)
		{
			QHostAddress localAddr=QHostAddress(s.addr);
			QHostAddress serverAddr=QHostAddress(server.addr);
			quint32 nMask=(s.netmask&server.netmask);
			
			if ((localAddr.toIPv4Address() & nMask) == (serverAddr.toIPv4Address() & nMask))
			{
				return true;
			}
		}
	}

	return false;
}

bool QServerManager::checkConnect(const ServerInfo& source, const ServerInfo& target, Server* sourceServer, Server* targetServer)
{
	foreach (QVector<Server> servers, source.servers)
	{
		foreach (Server server, servers)
		{
			foreach(QVector<Server> ts, target.servers)
			{
				foreach (Server t, ts)
				{
					QHostAddress souceHost=QHostAddress(server.addr);
					QHostAddress targetHost=QHostAddress(t.addr);
					if ((souceHost.toIPv4Address() & server.netmask) == (targetHost.toIPv4Address() & t.netmask))
					{
						if (sourceServer != NULL)
						{
							*sourceServer=server;
						}

						if (targetServer != NULL)
						{
							*targetServer=t;
						}

						return true;
					}
				}
			}
		}
	}

	findFirstServer(source, sourceServer);
	findFirstServer(target, targetServer);

	return false;
}

bool QServerManager::findFirstServer(const ServerInfo& info, Server* server)
{
	QMapIterator<QString, QVector<Server> > it(info.servers);
	while (it.hasNext())
	{
		it.next();

		const QVector<Server>& servers=it.value();
		if (!servers.isEmpty())
		{
			if (server != NULL)
			{
				*server=servers[0];
			}
			return true;
		}
	}

	return false;
}

void QServerManager::informStatusChanged(quint64 sign)
{
	QMutexLocker locker(&m_mutex);

	if (m_pLocalServer != NULL)
	{
		m_pLocalServer->informStatusChanged(sign);
	}

	foreach (QServerPtr pServer, m_localServer.servers)
	{
		if (pServer != NULL)
		{
			pServer->informStatusChanged(sign);
		}
	}
}

void QServerManager::setPort(quint16 port)
{
	m_localServer.port=port;

	checkLocalAddress();
}
