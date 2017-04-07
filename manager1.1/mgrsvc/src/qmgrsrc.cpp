#include "qmgrsrc.h"

#include "qservermanager.h"
#include "qlogger.h"

#include "qmessageparser.h"
#include "qmulticastmsgparser.h"
#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif
#include "qmibfactory.h"
#include "qmibmanager.h"

#ifdef TOPOLOGY_ENABLE
#include "qtopologygraph.h"
#endif

#include "qddbmanager.h"
#include "qdeafactory.h"
#include "qalarmfactory.h"
#include "qglobalinfo.h"
#include "qpluginmanager.h"

QMgrSrc::QMgrSrc(bool superUser, QObject *parent/* =0 */)
	: QObject(parent)
	,m_bSuperUser(superUser)
	,m_bLock(false)
{
	quint16 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/serverPort", 2300).toInt();

	QServerManager::instance()->setPort(nPort);
	init();

	connect(QMibManager::instance(), SIGNAL(changed(quint64)), this, SLOT(statusChanged(quint64)));

#ifdef TOPOLOGY_ENABLE
	connect(QTopologyGraph::instance(), SIGNAL(changed(GraphTopo)), SLOT(topologyChanged(GraphTopo)));
#endif
	
	connect(QServerManager::instance(), SIGNAL(remoteServerComing(const ServerInfo&)), this,
		SLOT(remoteServerComing(const ServerInfo&)), Qt::QueuedConnection);
	connect(QServerManager::instance(), SIGNAL(remoteServerQuit(const ServerInfo&)), this,
		SLOT(remoteServerQuit(const ServerInfo&)), Qt::QueuedConnection);

	startInterProcess();
	startServers();
	startHeartBeat();
}

QMgrSrc::~QMgrSrc()
{
#ifdef Q_OS_WIN
	WSACleanup();
#endif
}

void QMgrSrc::init()
{
#ifdef Q_OS_WIN
	WSADATA wsaData;  
	if(WSAStartup(MAKEWORD(2,2),&wsaData)==SOCKET_ERROR)  
	{
		logplusError(MGRSVC_LOGGER_STR, QString("WSAStartup failed!"));

		WSACleanup();
		exit(0);
	}  
#endif

	//加载数据加密模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading data encryption module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QDEAFactory::instance()->loadLibrary("libdead1", "CreateDEAInstance"))
#else
	if (QDEAFactory::instance()->loadLibrary("libdea1", "CreateDEAInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QDEAFactory::instance()->loadLibrary("libdead", "CreateDEAInstance"))
#else
	if (QDEAFactory::instance()->loadLibrary("libdea", "CreateDEAInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded data encryption module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "Data encryption module loading  failed!");
	}

	//加载消息模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading message module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QMessageParser::instance()->loadLibrary("libmsgd1", "CreateMessageInstance"))
#else
	if (QMessageParser::instance()->loadLibrary("libmsg1", "CreateMessageInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QMessageParser::instance()->loadLibrary("libmsgd", "CreateMessageInstance"))
#else
	if (QMessageParser::instance()->loadLibrary("libmsg", "CreateMessageInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded message module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "Message module loading failed!");
	}

	//加载多播消息模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading multicast message module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QMulticastMsgParser::instance()->loadLibrary("libmsgd1", "CreateMulticastMessageInstance"))
#else
	if (QMulticastMsgParser::instance()->loadLibrary("libmsg1", "CreateMulticastMessageInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QMulticastMsgParser::instance()->loadLibrary("libmsgd", "CreateMulticastMessageInstance"))
#else
	if (QMulticastMsgParser::instance()->loadLibrary("libmsg", "CreateMulticastMessageInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded multicast message module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "Multicast message module loading failed!");
	}

	//加载统计模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading statistics module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QWmiFactory::instance()->loadLibrary("libmibd1", "CreateWmiInstance"))
#else
	if (QWmiFactory::instance()->loadLibrary("libmib1", "CreateWmiInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QStatFactory::instance()->loadLibrary("libmibd", "CreateStatInstance"))
#else
	if (QStatFactory::instance()->loadLibrary("libmib", "CreateStatInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded statistics module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "Statistics module loading failed!");
	}

	//加载管理信息库模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading the management information base module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QMibFactory::instance()->loadLibrary("libmibd1", "CreateMibInstance"))
#else
	if (QMibFactory::instance()->loadLibrary("libmib1", "CreateMibInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QMibFactory::instance()->loadLibrary("libmibd", "CreateMibInstance"))
#else
	if (QMibFactory::instance()->loadLibrary("libmib", "CreateMibInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded the management information base module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "The management information base loading failed!");
	}

	//加载报警模块
	logplusInfo(MGRSVC_LOGGER_STR, "Loading the alarm module...");
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
	if (QAlarmFactory::instance()->loadLibrary("libalarmd1", "CreateAlarmInstance"))
#else
	if (QAlarmFactory::instance()->loadLibrary("libalarm1", "CreateAlarmInstance"))
#endif
#else
#ifdef QT_DEBUG
	if (QAlarmFactory::instance()->loadLibrary("libalarmd", "CreateAlarmInstance"))
#else
	if (QAlarmFactory::instance()->loadLibrary("libalarm", "CreateAlarmInstance"))
#endif
#endif
	{
		logplusInfo(MGRSVC_LOGGER_STR, "Loaded the alarm module successfully.");
	}
	else
	{
		logplusError(MGRSVC_LOGGER_STR, "The alarm loading failed!");
	}

#ifdef Q_OS_WIN
	logplusInfo(MGRSVC_LOGGER_STR, "Connecting to WMI service...");
	QWmiFactory::instance()->initialize();
#endif

	logplusInfo(MGRSVC_LOGGER_STR, "Start the managerment informantion base...");
	QMibManager::instance()->start(false);

	if (QServerManager::instance()->registerLocalServer() == QServerManager::RegisterFail)
	{
		logplusFatal(MGRSVC_LOGGER_STR, "Failed to start local services! Quit...");

		exit(0);
	}
}

void QMgrSrc::startServers()
{
	QServerManager::instance()->unregisterAll();

	const ServerInfo& serverInfo=QServerManager::instance()->localAddr();

	QList<QString> keys=serverInfo.servers.keys();
	foreach (QString key, keys)
	{
		QVector<Server> servers=serverInfo.servers[key];
		foreach (Server server, servers)
		{
			if (QServerManager::instance()->registerServer(key, server) == QServerManager::RegisterFail)
			{
				logplusError(MGRSVC_LOGGER_STR, QString("Register server %1:%2 failed! The port may have been used.").arg(server.addr).arg(server.port));

				logplusInfo(MGRSVC_LOGGER_STR, "The task has been running! Quit...");
				exit(0);
			}
		}
	}

	QGlobalInfo::instance()->setLocal(QServerManager::instance()->localServer());

	QAbstractMibPtr pMibInstance=QMibFactory::instance()->product(UTIL_TYPE_LOCAL_SERVER);
	if (pMibInstance != NULL)
	{
		QVariant var;
		var.setValue(QServerManager::instance()->localServer());
		pMibInstance->setManualResult(var);
	}

	pMibInstance=QMibFactory::instance()->product(UTIL_TYPE_REMOTE_SERVER);
	if (pMibInstance != NULL)
	{
		MibRemoteServer server;
		server.info=QServerManager::instance()->localServer();
		server.coming=true;

		QVariant var;
		var.setValue(server);
		pMibInstance->setManualResult(var);
	}
}

void QMgrSrc::startInterProcess()
{
	m_pMibServer=new QInterProcessServer("hman.mib");
	if (!m_pMibServer->onListen())
	{
		exit(0);
	}

	connect(m_pMibServer.get(), SIGNAL(deleteConnection(const QString&)), this, SLOT(deleteConnection(const QString&)));

	deleteConnection("hman.mib");
}

void QMgrSrc::deleteConnection(const QString& name)
{
	QPluginManager::instance()->activate(name);
}

void QMgrSrc::startHeartBeat()
{
	quint16 nPort=parseConfig(getAppParentDir()+INI_FILE, "network/broadcastPort", 2500).toInt();
	m_pHeartBeat=new QHeartBeat(nPort);

	m_pHeartBeat->run();

//	connect(m_pHeartBeat.get(), SIGNAL(localNetStatusChanged(bool)), this, SLOT(localNetStatusChanged(bool)));
}

void QMgrSrc::startDdb()
{
	//启动分布式数据库
	QDdbManager::instance()->start();
}

void QMgrSrc::remoteServerComing(const ServerInfo& info)
{
	QAbstractMibPtr pMibInstance=QMibFactory::instance()->product(UTIL_TYPE_REMOTE_SERVER);
	if (pMibInstance != NULL)
	{
		MibRemoteServer server;
		server.info=info;
		server.coming=true;

		QVariant var;
		var.setValue(server);
		pMibInstance->setManualResult(var);
	}
}

void QMgrSrc::remoteServerQuit(const ServerInfo& info)
{
	QAbstractMibPtr pMibInstance=QMibFactory::instance()->product(UTIL_TYPE_REMOTE_SERVER);
	if (pMibInstance != NULL)
	{
		MibRemoteServer server;
		server.info=info;
		server.coming=false;

		QVariant var;
		var.setValue(server);
		pMibInstance->setManualResult(var);
	}
}

void QMgrSrc::statusChanged(quint64 sign)
{
	if ((sign & ((quint64)1<<UTIL_TYPE_NETWORK_ADAPTER)) != 0 && !m_bLock)	//网卡信息改变，检测服务器信息
	{
		m_bLock=true;

		logplusInfo(MGRSVC_LOGGER_STR, QString("Network adapters has been changed. Recheck the servers!"));

		QServerManager::instance()->checkLocalAddress();

		const ServerInfo& localInfo=QServerManager::instance()->localAddr();
		const ServerInfo& serverInfo=QServerManager::instance()->localServer();
		if (localInfo != serverInfo)
		{
			QMapIterator<QString, QVector<Server> > it(serverInfo.servers);
			while (it.hasNext())
			{
				it.next();

				const QVector<Server>& servers=it.value();
				const QVector<Server> localAddrs=localInfo.servers.value(it.key());
				foreach (Server server, servers)
				{
					bool bExisted=false;
					foreach (Server localAddr, localAddrs)
					{
						if (localAddr == server)
						{
							bExisted=true;
							break;
						}
					}

					if (!bExisted)
					{
						QServerManager::instance()->unregisterServer(it.key(), server);
					}
				}
			}

			QMapIterator<QString, QVector<Server> > iter(localInfo.servers);
			while (iter.hasNext())
			{
				iter.next();

				const QVector<Server>& localAddrs=iter.value();
				foreach (Server localAddr, localAddrs)
				{
					if (QServerManager::instance()->registerServer(iter.key(), localAddr) == QServerManager::RegisterFail)
					{
						logplusError(MGRSVC_LOGGER_STR, QString("Register server %1:%2 failed! The port may have been used.")
							.arg(localAddr.addr).arg(localAddr.port));
					}
				}
			}

			QGlobalInfo::instance()->setLocal(QServerManager::instance()->localServer());
		}

		QAbstractMibPtr pMibInstance=QMibFactory::instance()->product(UTIL_TYPE_LOCAL_SERVER);
		if (pMibInstance != NULL)
		{
			QVariant var;
			var.setValue(QServerManager::instance()->localServer());
			pMibInstance->setManualResult(var);
		}

		pMibInstance=QMibFactory::instance()->product(UTIL_TYPE_REMOTE_SERVER);
		if (pMibInstance != NULL)
		{
			MibRemoteServer server;
			server.info=QServerManager::instance()->localServer();
			server.coming=true;

			QVariant var;
			var.setValue(server);
			pMibInstance->setManualResult(var);
		}

		logplusInfo(MGRSVC_LOGGER_STR, QString("Network adapters has been changed. Rebuild the ddb!"));
		startDdb();

		if (m_bSuperUser && QGlobalInfo::instance()->runningFlag(QGlobalInfo::EnableTopologyFlag))
		{
#ifdef TOPOLOGY_ENABLE
//			logplusInfo(MGRSVC_LOGGER_STR, QString("Network adapters has been changed. Reprobe network topology!"));
//			QTopologyGraph::instance()->probe(QServerManager::instance()->localAddr());
#endif
		}

		m_bLock=false;
	}

	QServerManager::instance()->informStatusChanged(sign);
}

void QMgrSrc::topologyChanged(GraphTopo graph)
{
	QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_TOPOLOGY);
	if (pInstance != NULL)
	{
		QVariant var;
		var.setValue(graph);
		pInstance->setManualResult(var);
	}
}
