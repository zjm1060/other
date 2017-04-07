#include "qnontopomainwnd.h"

#include <QResizeEvent>

#include "common.h"
#include "qwndmanager.h"

#include "qlogger.h"

#define GROUP_TITLE_WORKGROUP	tr("Workgroup")
#define GROUP_TITLE_LOCAL		tr("Local")
#define GROUP_TITLE_REMOTE		tr("Remote")

QNonTopoMainWnd::QNonTopoMainWnd()
	: QMainWindow(),
	m_pWndGroupManager(NULL)
{
	m_icon[IconWorkgroup]=QIcon(":/images/workgroup.png");
	m_icon[IconServer]=QIcon(":/images/server.png");
	m_icon[IconMultiServer]=QIcon(":/images/multiserver.png");
	m_icon[IconServerQuit]=QIcon(":/images/serverquit.png");

	m_pScrollArea=new QScrollArea(this);
	m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pScrollArea->installEventFilter(this);

	m_pListGroupWidget=new QListGroupWidget(m_pScrollArea);
	m_pListGroupWidget->setFrameShape(QFrame::NoFrame);

	m_pScrollArea->setWidget(m_pListGroupWidget);

	m_pListGroupWidget->addGroup(GROUP_TITLE_WORKGROUP);
	m_pListGroupWidget->addGroup(GROUP_TITLE_LOCAL);
	m_pListGroupWidget->addGroup(GROUP_TITLE_REMOTE);

	connect(m_pListGroupWidget, SIGNAL(itemDoubleClicked(const QString&, const QModelIndex&)),
		this, SLOT(itemDoubleClicked(const QString&, const QModelIndex&)));

	setCentralWidget(m_pScrollArea);

	int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "main/minWidth", 640).toInt();
	int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "main/minHeight", 480).toInt();
	setMinimumSize(QSize(nMinWidth, nMinHeight));

	connect(QServerManager::instance(), SIGNAL(remoteServerComing(const QServerManager::ServerInfo&)), this,
		SLOT(remoteServerComing(const QServerManager::ServerInfo&)));
	connect(QServerManager::instance(), SIGNAL(remoteServerQuit(const QServerManager::ServerInfo&)), this,
		SLOT(remoteServerQuit(const QServerManager::ServerInfo&)));

	startServers();
	startHeartBeat();

	showMaximized();
}

QNonTopoMainWnd::~QNonTopoMainWnd()
{
	if (m_pScrollArea != NULL)
	{
		m_pScrollArea->removeEventFilter(this);
	}
}

bool QNonTopoMainWnd::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == m_pScrollArea)
	{
		if (event->type() == QEvent::Resize)
		{
			QResizeEvent *resizeEvent = static_cast<QResizeEvent*>(event);

			QSize size=resizeEvent->size();
			m_pListGroupWidget->adjustWidget(size.width(), size.height());
		}
	}

	return QMainWindow::eventFilter(watched, event);
}

void QNonTopoMainWnd::itemDoubleClicked(const QString& title, const QModelIndex& index)
{
	if (title != GROUP_TITLE_WORKGROUP)
	{
		QStandardItemModel* pModel=m_pListGroupWidget->viewModel(title);
		if (pModel == NULL)
		{
			return;
		}

		if (m_mapWidget.contains(pModel->index(index.row(), 0)))
		{
			QWidget* pWidget=m_mapWidget.value(index);
			Q_ASSERT(pWidget != NULL);
			pWidget->raise();
			pWidget->activateWindow();

			return;
		}

		bool bConnected=pModel->data(pModel->index(index.row(), 0), Qt::UserRole+1).toBool();
		if (!bConnected)
		{
			return;
		}

		QServerManager::ServerInfo info=
			pModel->data(pModel->index(index.row(), 0), Qt::UserRole).value<QServerManager::ServerInfo>();

		QServerManager::Server server;
		if (!canConnected(info, server))
		{
			return;
		}

		QWndManager* pManager=new QWndManager(info.hostName, server.addr, server.port);
		int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
		int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
		pManager->setMinimumSize(nMinWidth, nMinHeight);

		connect(pManager, SIGNAL(onClosed(QWidget*)), this, SLOT(managerDestroy(QWidget*)));
		pManager->show();

		m_mapWidget.insert(pModel->index(index.row(), 0), pManager);
	}
	else
	{
		if (m_pWndGroupManager == NULL)
		{
			m_pWndGroupManager=new QWndGroupManager;

			int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
			int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
			m_pWndGroupManager->setMinimumSize(nMinWidth, nMinHeight);

			connect(m_pWndGroupManager, SIGNAL(onClosed(QWidget*)), this, SLOT(managerDestroy(QWidget*)));
			m_pWndGroupManager->show();
		}
		else
		{
			m_pWndGroupManager->raise();
			m_pWndGroupManager->activateWindow();
		}
	}
}

void QNonTopoMainWnd::managerDestroy(QWidget* widget)
{
	if (widget != 0)
	{
		if (widget == m_pWndGroupManager)
		{
			m_pWndGroupManager=NULL;
		}
		else
		{
			QModelIndex index=m_mapWidget.key(widget);

			if (index.isValid())
			{
				m_mapWidget.remove(index);
			}
		}
	}
}

void QNonTopoMainWnd::startServers()
{
	QStandardItemModel* pModel=m_pListGroupWidget->viewModel(GROUP_TITLE_LOCAL);
	Q_ASSERT_NO_NULL(pModel);

	QServerManager::instance()->unregisterAll();
	
	pModel->setRowCount(0);

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

	if (localServers.count > 0)
	{
		QStandardItem* pItem=NULL;

		QString strTip=localServers.hostName;

		keys=localServers.servers.keys();
		foreach (QString key, keys)
		{
			QVector<QServerManager::Server> servers=localServers.servers[key];
			foreach (QServerManager::Server server, servers)
			{
				strTip+=QString("\n%1:%2").arg(server.addr).arg(server.port);

				if (pItem == NULL)
				{
					pModel->insertRow(0);

					pItem=pModel->itemFromIndex(pModel->index(0, 0));
					pItem->setEditable(false);
					pModel->setData(pModel->index(0, 0), server.addr+":"+QString::number(server.port)+"\n"+localServers.hostName);
					pModel->setData(pModel->index(0, 0), localServers.count>1? m_icon[IconMultiServer]:m_icon[IconServer],
						Qt::DecorationRole);

					QVariant value;
					value.setValue(localServers);
					pModel->setData(pModel->index(0, 0), value, Qt::UserRole);

					pModel->setData(pModel->index(0, 0), true, Qt::UserRole+1);		//是否在线
				}
			}
		}

		if (pItem != NULL)
		{
			pItem->setToolTip(strTip);
		}
	}

	pModel=m_pListGroupWidget->viewModel(GROUP_TITLE_WORKGROUP);
	Q_ASSERT_NO_NULL(pModel);
	pModel->setRowCount(1);

	pModel->itemFromIndex(pModel->index(0, 0))->setEditable(false);
	pModel->setData(pModel->index(0, 0), tr("Workgroup"));
	pModel->setData(pModel->index(0, 0), m_icon[IconWorkgroup], Qt::DecorationRole);
}

void QNonTopoMainWnd::startHeartBeat()
{
	quint16 nPort=parseConfig(getAppParentDir()+INI_FILE, "network/broadcasePort", 2500).toInt();
	m_pHeartBeat=new QHeartBeat(nPort);

	m_pHeartBeat->run();

	connect(m_pHeartBeat, SIGNAL(localNetStatusChanged(bool)), this, SLOT(localNetStatusChanged(bool)));
}

void QNonTopoMainWnd::localNetStatusChanged(bool connecting)
{
	startServers();

	if (connecting)
	{
		QStandardItemModel* pModel=m_pListGroupWidget->viewModel(GROUP_TITLE_LOCAL);
		Q_ASSERT(pModel != NULL);

		int nCount=pModel->rowCount();
		for (int i=0; i<nCount; i++)
		{
			QModelIndex index=pModel->index(i, 0);
			if (index.isValid())
			{
				reconnect(GROUP_TITLE_LOCAL, index);
			}
		}
	}
}

void QNonTopoMainWnd::remoteServerComing(const QServerManager::ServerInfo& serverInfo)
{
	QStandardItemModel* pModel=m_pListGroupWidget->viewModel(GROUP_TITLE_REMOTE);
	Q_ASSERT(pModel != NULL);

	QList<QString> keys=serverInfo.servers.keys();

	foreach (QString key, keys)
	{
		if (QServerManager::instance()->localServer().servers.contains(key))
		{
			return;		//本机服务器
		}
	}

	if (keys.size() == 1)
	{
		QVector<QServerManager::Server> servers=serverInfo.servers[keys[0]];
		if (servers.size() == 1 && QHostAddress(servers[0].addr) == QHostAddress::LocalHost)
		{
			return;		//127.0.0.1
		}
	}

	QModelIndex index;

	bool bExisted=false;
	for (int i=0; i<pModel->rowCount(); i++)
	{
		QServerManager::ServerInfo info=
			pModel->data(pModel->index(i, 0), Qt::UserRole).value<QServerManager::ServerInfo>();

		foreach (QString key, keys)
		{
			if (info.servers.contains(key))
			{
				bExisted=true;

				index=pModel->index(i, 0);

				QVariant var;
				var.setValue(serverInfo);
				pModel->setData(index, var, Qt::UserRole);
				break;
			}
		}

		if (bExisted)
		{
			break;
		}
	}

	if (bExisted)
	{
		pModel->setData(index, Qt::UserRole+1, true);
		pModel->setData(index, serverInfo.count>1? m_icon[IconMultiServer]:m_icon[IconServer], Qt::DecorationRole);

		reconnect(GROUP_TITLE_REMOTE, index);
	}
	else
	{
		if (serverInfo.count > 0)
		{
			QStandardItem* pItem=NULL;

			QString strTip=serverInfo.hostName;

			foreach (QString key, keys)
			{
				QVector<QServerManager::Server> servers=serverInfo.servers[key];
				foreach (QServerManager::Server server, servers)
				{
					strTip+=QString("\n%1:%2").arg(server.addr).arg(server.port);

					if (pItem == NULL)
					{
						pModel->insertRow(0);

						pItem=pModel->itemFromIndex(pModel->index(0, 0));
						pItem->setEditable(false);
						pModel->setData(pModel->index(0, 0), server.addr+":"+QString::number(server.port)+"\n"+serverInfo.hostName);
						pModel->setData(pModel->index(0, 0), serverInfo.count>1? m_icon[IconMultiServer]:m_icon[IconServer], 
							Qt::DecorationRole);

						QVariant value;
						value.setValue(serverInfo);
						pModel->setData(pModel->index(0, 0), value, Qt::UserRole);

						pModel->setData(pModel->index(0, 0), true, Qt::UserRole+1);		//是否在线
					}
				}
			}

			if (pItem != NULL)
			{
				pItem->setToolTip(strTip);
			}
		}

		QSize size=m_pScrollArea->size();
		m_pListGroupWidget->adjustWidget(size.width(), size.height());
	}
}

void QNonTopoMainWnd::remoteServerQuit(const QServerManager::ServerInfo& serverInfo)
{
	QStandardItemModel* pModel=m_pListGroupWidget->viewModel(GROUP_TITLE_REMOTE);
	Q_ASSERT(pModel != NULL);

	QList<QString> keys=serverInfo.servers.keys();
	for (int i=0; i<pModel->rowCount(); i++)
	{
		QServerManager::ServerInfo info=
			pModel->data(pModel->index(i, 0), Qt::UserRole).value<QServerManager::ServerInfo>();

		foreach (QString key, keys)
		{
			if (info.servers.contains(key))
			{
				QModelIndex index=pModel->index(i, 0);

				serverQuit(pModel, index);

				return;
			}
		}
	}
}

void QNonTopoMainWnd::serverQuit(QStandardItemModel* model, const QModelIndex& index)
{
	if (model == NULL)
	{
		return;
	}

	model->setData(model->index(index.row(), 0), false, Qt::UserRole+1);
	model->setData(model->index(index.row(), 0), m_icon[IconServerQuit], Qt::DecorationRole);
}

void QNonTopoMainWnd::reconnect(const QString& title, const QModelIndex& index)
{
	QStandardItemModel* pModel=m_pListGroupWidget->viewModel(title);
	if (pModel == NULL)
	{
		return;
	}

	QWidget* pWidget=m_mapWidget.value(pModel->index(index.row(), 0), NULL);
	if (pWidget != NULL)
	{
		QWndManager* pManager=static_cast<QWndManager*> (pWidget);
		if (pManager != NULL)
		{
			if (!pManager->isConnected())
			{
				QServerManager::ServerInfo info=
					pModel->data(pModel->index(index.row(), 0), Qt::UserRole).value<QServerManager::ServerInfo>();

				QServerManager::Server server;
				if (canConnected(info, server))
				{
					pManager->reconnect(server.addr, server.port);
				}
			}
		}
	}
}

bool QNonTopoMainWnd::canConnected(const QServerManager::ServerInfo& info, QServerManager::Server& server) const
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

void QNonTopoMainWnd::closeEvent(QCloseEvent* event)
{
	foreach(QWidget* pWidget, m_mapWidget)
	{
		if (pWidget != NULL)
		{
			pWidget->close();
		}
	}

	if (m_pWndGroupManager != NULL)
	{
		m_pWndGroupManager->close();
	}

	QNDC::instance()->pop();

	LOGPLUS_INFO_STR(QLogger::root(), getHostName()+"."+getUserName()+" exited!\n");

	event->accept();
}
