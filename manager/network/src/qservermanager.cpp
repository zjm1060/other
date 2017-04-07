#include "qservermanager.h"

#include <QStringList>
#include <QProcess>
#include <QNetworkInterface>

#include "common.h"
#include "qlogger.h"

QMutex QServerManager::m_mutex;
QServerManager* QServerManager::m_pServerManager=0;

static const qint32 g_nCheckInterval=2000;

#define MAX_CHECK_TIMES 6

QServerManager::QServerManager(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<ServerInfo>("ServerInfo");

	m_localAddr.hostName=m_server.hostName=getHostName();

	checkLocalAddress();

	m_server.count=0;

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
		m_mutex.lock();
		if (m_pServerManager == 0)
		{
			m_pServerManager=new QServerManager;
		}
		m_mutex.unlock();
	}

	return m_pServerManager;
}

void QServerManager::destroy()
{
	m_mutex.lock();

	SAFE_DELETE_CLASS(m_pServerManager);

	m_mutex.unlock();
}

void QServerManager::checkLocalAddress()
{
	bool bLocalHost=true;

	m_localAddr.servers.clear();
	m_server.servers.clear();
	unregisterAll();

	QList<QNetworkInterface> interfaces=QNetworkInterface::allInterfaces();
	QString strHardware;
	QServerManager::Server server;

	foreach (QNetworkInterface face, interfaces)
	{
		QVector<QServerManager::Server> servers;
		strHardware=face.hardwareAddress();
		if (strHardware.isEmpty())
		{
			strHardware=getMacAddress();
		}

		LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Find MAC address=%s", strHardware.toLocal8Bit().data()));

		QList<QNetworkAddressEntry> entries=face.addressEntries();
		foreach (QNetworkAddressEntry entry, entries)
		{
			if (entry.ip().protocol() != QAbstractSocket::IPv4Protocol)
			{
				continue;
			}

			server.addr=entry.ip().toString();
			server.netmask=entry.netmask().toIPv4Address();
			if (server.netmask == 0)
			{
				server.netmask=QHostAddress("255.255.255.0").toIPv4Address();
			}

			LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Find IP address=%s, Netmask=%s", server.addr.toLocal8Bit().data(),
				QHostAddress(server.netmask).toString().toLocal8Bit().data()));

			if (entry.ip() != QHostAddress::LocalHost)
			{
				bLocalHost=false;
			}

			servers.append(server);
		}

		if (!servers.isEmpty())
		{
			if (!m_localAddr.servers.contains(strHardware))
			{
				m_localAddr.servers.insert(strHardware, servers);
			}
			else
			{
				m_localAddr.servers[strHardware]+=servers;
			}
		}
	}

	if (!bLocalHost)
	{
		QList<QString> keys=m_localAddr.servers.keys();
		foreach (QString key, keys)
		{
			QVector<Server>& servers=m_localAddr.servers[key];

			QMutableVectorIterator<Server> it(servers);
			while (it.hasNext())
			{
				Server& server=it.next();
				if (QHostAddress(server.addr) == QHostAddress::LocalHost)
				{
					it.remove();
				}
			}
		}

		foreach (QString key, keys)
		{
			if (m_localAddr.servers[key].isEmpty())
			{
				m_localAddr.servers.remove(key);
			}
		}
	}
}

QServerManager::RegisterStatus QServerManager::registerServer(const QString& hardwareAddr, const Server& server)
{
	QServer* pServer;
	foreach(pServer, m_listServer)
	{
		if (pServer->serverAddress().toString() == server.addr && pServer->serverPort() == server.port)
		{
			return ServerExisting;
		}
	}

	pServer=new QServer(server.addr, server.port, this);
	if (!pServer->onListen())
	{
		SAFE_DELETE_CLASS(pServer);
		return RegisterFail;
	}

	LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Server: %s:%d listening...", server.addr.toLocal8Bit().data(), server.port));

	m_listServer.append(pServer);

	if (!m_server.servers.contains(hardwareAddr))
	{
		m_server.servers.insert(hardwareAddr, QVector<Server>());
	}

	QVector<Server>& servers=m_server.servers[hardwareAddr];
	servers.append(server);

	m_server.count++;

	return RegisterSuccess;
}

void QServerManager::unregisterServer(const QString& hardwareAddr, const Server& server)
{
	foreach (QServer* pServer, m_listServer)
	{
		if (pServer->serverAddress().toString() == server.addr && pServer->serverPort() == server.port)
		{
			pServer->onClose();
			m_listServer.removeOne(pServer);

			SAFE_DELETE_CLASS(pServer);

			if (m_server.servers.contains(hardwareAddr))
			{
				QVector<Server>& servers=m_server.servers[hardwareAddr];
				for (int i=0; i<servers.size(); i++)
				{
					if (servers[i].addr == server.addr && servers[i].netmask == server.netmask && servers[i].port == server.port)
					{
						servers.remove(i);

						m_server.count--;
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
	LOGPLUS_DEBUG_STR(QLogger::root(), qFormatString("Stop %d servers!!!", m_listServer.size()));

	foreach (QServer* pServer, m_listServer)
	{
		pServer->onClose();

		SAFE_DELETE_CLASS(pServer);
	}

	m_listServer.clear();

	m_server.count=0;
	m_server.servers.clear();
}

const QServerManager::ServerInfo& QServerManager::localServer() const
{
	return m_server;
}

const QServerManager::ServerInfo& QServerManager::localAddr() const
{
	return m_localAddr;
}

void QServerManager::addRemoteServer(const QServerManager::ServerInfo& serverInfo)
{
	int nNumServer=m_multiHashRemoteServer.size();

	bool bExisted=false;

	QList<QString> keys=serverInfo.servers.keys();
	foreach (QString key, keys)
	{
		if (m_server.servers.contains(key))
		{
			bExisted=true;		//本机服务器
			break;
		}
	}

	if (!bExisted)
	{
		QMultiHash<QString, ServerInfo>::iterator it=m_multiHashRemoteServer.find(serverInfo.hostName);
		while (it != m_multiHashRemoteServer.end() && it.key() == serverInfo.hostName)
		{
			ServerInfo& info=it.value();
			foreach (QString key, keys)
			{
				if (info.servers.contains(key))
				{
					info=serverInfo;

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

	ServerInfo info=serverInfo;
	info.heartbeat=0;

	if (!bExisted)
	{
		m_multiHashRemoteServer.insert(info.hostName, info);

		QString strLog="Receive heart beat packet:\n"+serverInfo.hostName;
		QMapIterator<QString, QVector<Server> > it(info.servers);
		while (it.hasNext())
		{
			it.next();

			const QVector<Server>& servers=it.value();
			for (int i=0; i<servers.size(); i++)
			{
				strLog+=QString("\n%1:%2").arg(servers[i].addr).arg(servers[i].port);
			}
		}
		
		LOGPLUS_DEBUG_STR(QLogger::root(), strLog.toLocal8Bit().data());
	}

	emit remoteServerComing(info);

	if (nNumServer == 0 && m_multiHashRemoteServer.size() == 1)
	{
		if (!m_timer.isActive())
		{
			m_timer.start();
		}
	}
}

const QMultiHash<QString, QServerManager::ServerInfo>& QServerManager::remoteServer() const
{
	return m_multiHashRemoteServer;
}

void QServerManager::timeout()
{
	foreach(QString key, m_multiHashRemoteServer.keys())
	{
		QMultiHash<QString, ServerInfo>::iterator it=m_multiHashRemoteServer.find(key);
		while (it != m_multiHashRemoteServer.end() && it.key() == key)
		{
			if (it->heartbeat > MAX_CHECK_TIMES)
			{
				emit remoteServerQuit(it.value());

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

bool QServerManager::canConnected(const Server& server)
{
	foreach (QVector<Server> servers, m_localAddr.servers)
	{
		foreach (Server s, servers)
		{
			QHostAddress localAddr=QHostAddress(s.addr);
			QHostAddress serverAddr=QHostAddress(server.addr);
			if ((localAddr.toIPv4Address() & s.netmask) == (serverAddr.toIPv4Address() & server.netmask))
			{
				return true;
			}
		}
	}

	return false;
}

bool QServerManager::checkConnect(const QServerManager::ServerInfo& source, const QServerManager::ServerInfo& target,
				  QString& sourceAddr, QString& targetAddr)
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
						sourceAddr=server.addr;
						targetAddr=QString("%1:%2").arg(t.addr).arg(server.port);
						return true;
					}
				}
			}
		}
	}

	return false;
}

QString QServerManager::findFirstAddress(const QServerManager::ServerInfo& info, bool hasPort)
{
	foreach(QVector<QServerManager::Server> servers, info.servers)
	{
		foreach (QServerManager::Server server, servers)
		{
			if (hasPort)
			{
				return QString("%1:%2").arg(server.addr).arg(server.port);
			}
			else
			{
				return server.addr;
			}
		}
	}

	return QString();
}

bool operator==(const QServerManager::ServerInfo& source, const QServerManager::ServerInfo& target)
{
	QList<QString> keys=source.servers.keys();
	foreach (QString key, keys)
	{
		if (target.servers.contains(key))
		{
			return true;
		}
	}

	return false;
}

QDataStream& operator<<(QDataStream& out, const QServerManager::ServerInfo& info)
{
	out<<info.hostName;

	QList<QString> keys=info.servers.keys();
	out<<keys.size();
	foreach (QString key, keys)
	{
		out<<key;

		const QVector<QServerManager::Server>& servers=info.servers[key];
		out<<servers.size();
		foreach (QServerManager::Server server, servers)
		{
			out<<server.addr<<server.netmask<<server.port;
		}
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, QServerManager::ServerInfo& info)
{
	int nNumKeys, nNumServers;

	in>>info.hostName>>nNumKeys;

	info.count=0;

	QString key;
	QServerManager::Server server;
	for (int i=0; i<nNumKeys; i++)
	{
		in>>key>>nNumServers;
		QVector<QServerManager::Server> servers;
		for (int j=0; j<nNumServers; j++)
		{
			in>>server.addr>>server.netmask>>server.port;

			servers.append(server);
		}

		info.servers.insert(key, servers);

		info.count+=nNumServers;
	}

	return in;
}
