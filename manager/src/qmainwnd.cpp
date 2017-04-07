#include "qmainwnd.h"

#include <QResizeEvent>

#include "common.h"
#include "qwndmanager.h"

#include "qlogger.h"

QMainWnd::QMainWnd()
	: QMainWindow(),
	m_pWndManager(NULL),
	m_pWndGroupManager(NULL)
{
#ifdef Q_OS_WIN
	WSADATA wsaData;  
	if(WSAStartup(MAKEWORD(2,2),&wsaData)==SOCKET_ERROR)  
	{  
		WSACleanup();  
	}  
#endif

	setWindowTitle(QString("Manager")+ MANAGER_VERSION_STR);

	m_pWndGraphics=new QWndGraphics(this);
	setCentralWidget(m_pWndGraphics);

	m_pLogicalTopo=new QLogicalTopo();
	connect(m_pLogicalTopo, SIGNAL(routesProbed(QVector<QPair<quint32, int> >)), 
		m_pWndGraphics, SLOT(setRouters(QVector<QPair<quint32, int> >)));
	connect(m_pLogicalTopo, SIGNAL(alivesProbed(quint32, quint32, QVector<quint32>)),
		this, SLOT(setAlives(quint32, quint32, QVector<quint32>)));
	connect(m_pLogicalTopo, SIGNAL(checkAlives()), m_pWndGraphics, SLOT(checkAlives()));
	connect(m_pLogicalTopo, SIGNAL(disconnected()), m_pWndGraphics, SLOT(disconnected()));

	int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "main/minWidth", 640).toInt();
	int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "main/minHeight", 480).toInt();
	setMinimumSize(QSize(nMinWidth, nMinHeight));

	connect(QServerManager::instance(), SIGNAL(remoteServerComing(const QServerManager::ServerInfo&)), this,
		SLOT(remoteServerComing(const QServerManager::ServerInfo&)));
	connect(QServerManager::instance(), SIGNAL(remoteServerQuit(const QServerManager::ServerInfo&)), this,
		SLOT(remoteServerQuit(const QServerManager::ServerInfo&)));

	m_pBar=new QToolBar(tr("Tools"), this);
	addToolBar(Qt::RightToolBarArea, m_pBar);
	m_pBar->setFloatable(false);
	m_pBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	int nSize=parseConfig(getAppParentDir()+INI_FILE, "main/iconSize", 64).toInt();
	m_pBar->setIconSize(QSize(nSize, nSize));

	m_pAction[ActionLocalHost]=new QAction(QIcon(":/images/server.png"), tr("Local Host"), m_pBar);
	m_pAction[ActionWorkgroup]=new QAction(QIcon(":/images/workgroup.png"), tr("Workgroup"), m_pBar);

	m_pGroup=new QActionGroup(m_pBar);
	for (int i=0; i<ActionEnd; i++)
	{
		m_pGroup->addAction(m_pAction[i]);
	}
	connect(m_pGroup, SIGNAL(triggered(QAction*)), this, SLOT(onActionToggled(QAction*)));

	for (int i=0; i<ActionEnd; i++)
	{
		m_pBar->addAction(m_pAction[i]);
	}

	m_pLogicalTopo->probe();

	startServers();
	startHeartBeat();

	showMaximized();
}

QMainWnd::~QMainWnd()
{
	SAFE_DELETE_CLASS(m_pLogicalTopo);

#ifdef Q_OS_WIN
	WSACleanup();
#endif
}

void QMainWnd::wndClosed(QWidget* widget)
{
	if (widget == NULL)
	{
		return;
	}

	if (widget == m_pWndManager)
	{
		SAFE_DELETE_CLASS(m_pWndManager);
	}
	else if (widget == m_pWndGroupManager)
	{
		SAFE_DELETE_CLASS(m_pWndGroupManager);
	}
}

void QMainWnd::onActionToggled(QAction* action)
{
	if (action == m_pAction[ActionWorkgroup])
	{
		if (m_pWndGroupManager == NULL)
		{
			m_pWndGroupManager=new QWndGroupManager;

			int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
			int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
			m_pWndGroupManager->setMinimumSize(nMinWidth, nMinHeight);

			connect(m_pWndGroupManager, SIGNAL(onClosed(QWidget*)), this, SLOT(wndClosed(QWidget*)));
			m_pWndGroupManager->show();
		}
		else
		{
			m_pWndGroupManager->raise();
			m_pWndGroupManager->activateWindow();
		}
	}
	else if (action == m_pAction[ActionLocalHost])
	{
		if (m_pWndManager == NULL)
		{
			const QServerManager::ServerInfo& serverInfo=QServerManager::instance()->localAddr();

			QServerManager::ServerInfo info=m_pAction[ActionLocalHost]->data().value<QServerManager::ServerInfo>();
			QServerManager::Server server;
			if (!canConnected(info, server))
			{
				qDebug("QMainWnd: Cann\'t connect to local host !!!");
				return;
			}

			m_pWndManager=new QWndManager(serverInfo.hostName, server.addr, server.port);
			int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
			int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
			m_pWndManager->setMinimumSize(nMinWidth, nMinHeight);

			connect(m_pWndManager, SIGNAL(onClosed(QWidget*)), this, SLOT(wndClosed(QWidget*)));

			m_pWndManager->show();
		}
		else
		{
			m_pWndManager->raise();
			m_pWndManager->activateWindow();
		}
	}
}

void QMainWnd::startServers()
{
	QServerManager::instance()->unregisterAll();

	quint16 nPort=(quint16)parseConfig(getAppParentDir()+INI_FILE, "network/serverPort", 2300).toInt();

	const QServerManager::ServerInfo& serverInfo=QServerManager::instance()->localAddr();

	QList<QString> keys=serverInfo.servers.keys();
	foreach (QString key, keys)
	{
		QVector<QServerManager::Server> servers=serverInfo.servers[key];
		foreach (QServerManager::Server server, servers)
		{
			server.port=nPort;
			QServerManager::instance()->registerServer(key, server);
		}
	}

	const QServerManager::ServerInfo& localServers=QServerManager::instance()->localServer();
	m_pAction[ActionLocalHost]->setText(localServers.hostName);

	if (localServers.count > 0)
	{
		QString strTip=localServers.hostName;

		keys=localServers.servers.keys();
		foreach (QString key, keys)
		{
			QVector<QServerManager::Server> servers=localServers.servers[key];
			foreach (QServerManager::Server server, servers)
			{
				strTip+=QString("\n%1:%2").arg(server.addr).arg(server.port);
			}
		}

		if (localServers.count == 0)
		{
			m_pAction[ActionLocalHost]->setIcon(QIcon(":/images/serverquit.png"));
		}
		else if (localServers.count == 1)
		{
			m_pAction[ActionLocalHost]->setIcon(QIcon(":/images/server.png"));
		}
		else
		{
			m_pAction[ActionLocalHost]->setIcon(QIcon(":/images/multiserver.png"));
		}

		m_pAction[ActionLocalHost]->setToolTip(strTip);

		QVariant value;
		value.setValue(localServers);

		m_pAction[ActionLocalHost]->setData(value);
	}
}

void QMainWnd::startHeartBeat()
{
	quint16 nPort=parseConfig(getAppParentDir()+INI_FILE, "network/broadcasePort", 2500).toInt();
	m_pHeartBeat=new QHeartBeat(nPort);

	m_pHeartBeat->run();

	connect(m_pHeartBeat, SIGNAL(localNetStatusChanged(bool)), this, SLOT(localNetStatusChanged(bool)));
}

void QMainWnd::localNetStatusChanged(bool connecting)
{
	if (!connecting)
	{
		const QServerManager::ServerInfo& info=QServerManager::instance()->localServer();
		remoteServerQuit(info);
	}

	startServers();

	if (m_pWndManager != NULL)
	{
		QServerManager::ServerInfo info=m_pAction[ActionLocalHost]->data().value<QServerManager::ServerInfo>();
		reconnect(m_pWndManager, info);
	}

	if (connecting)
	{
		if (m_pWndGraphics != NULL)
		{
			m_pWndGraphics->reconnect();
		}

		m_pLogicalTopo->reprobe();
	}
}

void QMainWnd::remoteServerComing(const QServerManager::ServerInfo& info)
{
	if (!m_pWndGraphics->addServer(info))
	{
		m_pLogicalTopo->reprobe();	//ÖØÐÂÌ½²âÍøÂç
	}
}

void QMainWnd::remoteServerQuit(const QServerManager::ServerInfo& info)
{
	m_pWndGraphics->removeServer(info);
}

void QMainWnd::reconnect(QWndManager* manager, const QServerManager::ServerInfo& info)
{
	if (manager == NULL)
	{
		return;
	}

	QServerManager::Server server;
	if (canConnected(info, server))
	{
		manager->reconnect(server.addr, server.port);
	}
}

bool QMainWnd::canConnected(const QServerManager::ServerInfo& info, QServerManager::Server& server) const
{
	bool bCanConnected=false;
	foreach (QVector<QServerManager::Server> servers, info.servers)
	{
		foreach (QServerManager::Server s, servers)
		{
			if (QServerManager::instance()->canConnected(s))
			{
				server=s;

				bCanConnected=true;
				break;
			}
		}

		if (bCanConnected)
		{
			break;
		}
	}

	return bCanConnected;
}

void QMainWnd::closeEvent(QCloseEvent* event)
{
	if (m_pWndManager != NULL)
	{
		m_pWndManager->close();
	}

	if (m_pWndGroupManager != NULL)
	{
		m_pWndGroupManager->close();
	}

	m_pWndGraphics->closeEvent();

	QNDC::instance()->pop();

	LOGPLUS_INFO_STR(QLogger::root(), getHostName()+"."+getUserName()+" exited!\n");

	event->accept();
}

void QMainWnd::setAlives(quint32 route, quint32 gateway, QVector<quint32> alives)
{
	m_pWndGraphics->setAlives(route, gateway, alives);

	const QServerManager::ServerInfo& info=QServerManager::instance()->localServer();
	m_pWndGraphics->addServer(info);
}
