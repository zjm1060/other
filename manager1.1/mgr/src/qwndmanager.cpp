#include "qwndmanager.h"

#include <QToolBar>
#include <QCloseEvent>
#include <QFrame>

#include "common.h"
#include "qmessageparser.h"
#include "qfilecontroller.h"

QWndManager::QWndManager(const QString& hostname, const QString& ipAddr, quint16 port, QWidget *parent)
: QMainWindow(parent)
 ,m_pSystemManager(NULL)
 ,m_pProcessManager(NULL)
 ,m_pResourceManager(NULL)
 ,m_pFileTransferManager(NULL)
// ,m_pSyncManager(NULL)
{
	m_pClient=new QClient(QHostAddress(ipAddr), port);

	connect(m_pClient, SIGNAL(quit()), this, SLOT(clientQuit()));
	connect(m_pClient, SIGNAL(recvMessage(QConnection*, QByteArray)), this, SLOT(clientRecvMessage(QConnection*, QByteArray)));
	connect(m_pClient, SIGNAL(clientStateChanged(QAbstractSocket::SocketState)), 
		this, SLOT(clientStateChanged(QAbstractSocket::SocketState)));

	m_strPeerName=hostname;

	init();

	setTitleBar(false);

	m_pController=new QFileController(this);

	QFrame* pFrame=new QFrame(this);
	m_pLayout=new QVBoxLayout(pFrame);
	pFrame->setLayout(m_pLayout);

	setCentralWidget(pFrame);

	m_pAction[ActionSystemMgr]->trigger();

	connectToHost();

	int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
	int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
	setMinimumSize(nMinWidth, nMinHeight);

	showMaximized();
}

QWndManager::~QWndManager()
{
	SAFE_DELETE_CLASS(m_pClient);
}

void QWndManager::init()
{
	m_pBar=addToolBar(tr("Tool Bar"));
	m_pBar->setFloatable(false);
	m_pBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	int nSize=parseConfig(getAppParentDir()+INI_FILE, "window/iconSize", 64).toInt();
	m_pBar->setIconSize(QSize(nSize, nSize));

	m_pAction[ActionSystemMgr]=new QAction(QIcon(":/images/system.png"), tr("System Manager"), m_pBar);
	m_pAction[ActionSystemMgr]->setCheckable(true);
	m_pAction[ActionSystemMgr]->setData(ActionSystemMgr);

	m_pAction[ActionProcessMgr]=new QAction(QIcon(":/images/process.png"), tr("Process Manager"), m_pBar);
	m_pAction[ActionProcessMgr]->setCheckable(true);
	m_pAction[ActionProcessMgr]->setData(ActionProcessMgr);

	m_pAction[ActionFileTransfer]=new QAction(QIcon(":/images/ftp.png"), tr("File Transfer"), m_pBar);
	m_pAction[ActionFileTransfer]->setCheckable(true);
	m_pAction[ActionFileTransfer]->setData(ActionFileTransfer);

// 	m_pAction[ActionSynchronize]=new QAction(QIcon(":/images/sync.png"), tr("Synchronize"), m_pBar);
// 	m_pAction[ActionSynchronize]->setCheckable(true);
// 	m_pAction[ActionSynchronize]->setData(ActionSynchronize);

	m_pAction[ActionNetwork]=new QAction(QIcon(":/images/network.png"), tr("Network"), m_pBar);
	m_pAction[ActionNetwork]->setCheckable(true);
	m_pAction[ActionNetwork]->setData(ActionNetwork);

	m_pAction[ActionResourceMgr]=new QAction(QIcon(":/images/resource.png"), tr("Resource Manager"), m_pBar);
	m_pAction[ActionResourceMgr]->setCheckable(true);
	m_pAction[ActionResourceMgr]->setData(ActionResourceMgr);

	m_pAction[ActionDatabase]=new QAction(QIcon(":/images/database.png"), tr("Database"), m_pBar);
	m_pAction[ActionDatabase]->setCheckable(true);
	m_pAction[ActionDatabase]->setData(ActionDatabase);

	m_pGroup=new QActionGroup(m_pBar);
	for (int i=0; i<ActionEnd; i++)
	{
		m_pGroup->addAction(m_pAction[i]);
	}
	m_pGroup->setExclusive(true);
	connect(m_pGroup, SIGNAL(triggered(QAction*)), this, SLOT(onActionToggled(QAction*)));

	for (int i=0; i<ActionEnd; i++)
	{
		m_pBar->addAction(m_pAction[i]);
	}
}

void QWndManager::closeEvent(QCloseEvent* event)
{
	emit onClosed(this);

	event->accept();
}

void QWndManager::clientStateChanged(QAbstractSocket::SocketState socketState)
{
	if (socketState == QAbstractSocket::ConnectedState)
	{
		setTitleBar(true);

		QAction* pAction=m_pGroup->checkedAction();
		if (pAction == NULL)
		{
			return;
		}

		int nType=pAction->data().toInt();
		switch (nType)
		{
		case ActionSystemMgr:
			{
				if (m_pSystemManager != NULL)
				{
					m_pSystemManager->reconnected();
				}
			}
			break;
		case ActionProcessMgr:
			{
				if (m_pProcessManager != NULL)
				{
					m_pProcessManager->querySystemProcess();
				}
			}
			break;
		case ActionFileTransfer:
			break;
// 		case ActionSynchronize:
// 			break;
		case ActionNetwork:
			break;
		case ActionResourceMgr:
			{
				if (m_pResourceManager != NULL)
				{
					m_pResourceManager->reconnected(true);
				}
			}
			break;
		case ActionDatabase:
			break;
		default:
			break;
		}
	}
}

void QWndManager::connectToHost()
{
	if (m_pClient != NULL)
	{
		m_pClient->connectToHost();
	}
}

void QWndManager::reconnect()
{
	connectToHost();
}

void QWndManager::reconnect(const QString& addr, quint16 port)
{
	if (m_pClient != NULL)
	{
		m_pClient->setServerAddr(addr);
		m_pClient->setPort(port);
	}

	reconnect();
}

bool QWndManager::isConnected()
{
	if (m_pClient != NULL)
	{
		return m_pClient->state() == QAbstractSocket::ConnectedState;
	}

	return false;
}

void QWndManager::setTitleBar(bool bConnected)
{
	Q_ASSERT(m_pClient);
	QString strAddr=m_pClient->serverAddr().toString();
	quint16 nPort=m_pClient->port();

	if (bConnected)
	{
		setWindowIcon(QIcon(":/images/link.png"));
		setWindowTitle(tr("Connected to %1(%2:%3)").arg(m_strPeerName).arg(strAddr).arg(nPort));
	}
	else
	{
		setWindowIcon(QIcon(":/images/unlink.png"));
		setWindowTitle(tr("Disconnected with %1(%2:%3)").arg(m_strPeerName).arg(strAddr).arg(nPort));
	}
}

void QWndManager::clientQuit()
{
	setTitleBar(false);
}

void QWndManager::clientRecvMessage(QConnection* sender, QByteArray buffer)
{
	int nMsgType;

	Q_READ_ONLY_DATA_STREAM(out, buffer);

	out>>nMsgType;

	QIMessagePtr pMessage=QMessageParser::instance()->messageInstance(nMsgType);
	if (pMessage != NULL)
	{
		pMessage->parse(sender, buffer.right(buffer.size()-sizeof(int)), m_pController);
	}
}

void QWndManager::onActionToggled(QAction* action)
{
	int nType=action->data().toInt();

	switch (nType)
	{
	case ActionSystemMgr:
		{
			SAFE_HIDE_WIDGET(m_pProcessManager);
			SAFE_HIDE_WIDGET(m_pFileTransferManager);
// 			SAFE_HIDE_WIDGET(m_pSyncManager);
			SAFE_HIDE_WIDGET(m_pResourceManager);

			if (m_pSystemManager == NULL)
			{
				m_pSystemManager=new QSystemManager(m_pClient, this);
				m_pLayout->addWidget(m_pSystemManager);
			}
			m_pSystemManager->show();
		}
		break;
	case ActionProcessMgr:
		{
 			SAFE_HIDE_WIDGET(m_pSystemManager);
			SAFE_HIDE_WIDGET(m_pFileTransferManager);
// 			SAFE_HIDE_WIDGET(m_pSyncManager);
			SAFE_HIDE_WIDGET(m_pResourceManager);

			if (m_pProcessManager == NULL)
			{
				m_pProcessManager=new QProcessManager(m_pClient, this);
				m_pLayout->addWidget(m_pProcessManager);
			}
 			m_pProcessManager->show();
		}
		break;
	case ActionFileTransfer:
		{
			SAFE_HIDE_WIDGET(m_pSystemManager);
			SAFE_HIDE_WIDGET(m_pProcessManager);
//			SAFE_HIDE_WIDGET(m_pSyncManager);
			SAFE_HIDE_WIDGET(m_pResourceManager);

			if (m_pFileTransferManager == NULL)
			{
				m_pFileTransferManager=new QFileTransferManager(m_pClient, this);
				m_pLayout->addWidget(m_pFileTransferManager);
			}
			m_pFileTransferManager->show();
		}
		break;
// 	case ActionSynchronize:
// 		{
// 			SAFE_HIDE_WIDGET(m_pSystemManager);
// 			SAFE_HIDE_WIDGET(m_pProcessManager);
// 			SAFE_HIDE_WIDGET(m_pFileTransferManager);
// 			if (m_pSyncManager == NULL)
// 			{
// 				m_pSyncManager=new QSyncManager(m_pClient, this);
// 				m_pLayout->addWidget(m_pSyncManager);
// 			}
// 			m_pSyncManager->show();
// 			SAFE_HIDE_WIDGET(m_pResourceManager);	
// 		}
// 		break;
	case ActionNetwork:
		{
 			SAFE_HIDE_WIDGET(m_pSystemManager);
 			SAFE_HIDE_WIDGET(m_pProcessManager);
			SAFE_HIDE_WIDGET(m_pFileTransferManager);
// 			SAFE_HIDE_WIDGET(m_pSyncManager);
 			SAFE_HIDE_WIDGET(m_pResourceManager);
		}
		break;
	case ActionResourceMgr:
		{
 			SAFE_HIDE_WIDGET(m_pSystemManager);
 			SAFE_HIDE_WIDGET(m_pProcessManager);
			SAFE_HIDE_WIDGET(m_pFileTransferManager);
// 			SAFE_HIDE_WIDGET(m_pSyncManager);
			if (m_pResourceManager == NULL)
			{
				m_pResourceManager=new QResourceManager(m_pClient, this);
				m_pLayout->addWidget(m_pResourceManager);
			}
			m_pResourceManager->show();
		}
		break;
	case ActionDatabase:
		{
 			SAFE_HIDE_WIDGET(m_pSystemManager);
 			SAFE_HIDE_WIDGET(m_pProcessManager);
			SAFE_HIDE_WIDGET(m_pFileTransferManager);
// 			SAFE_HIDE_WIDGET(m_pSyncManager);
 			SAFE_HIDE_WIDGET(m_pResourceManager);
		}
		break;
	default:
		break;
	}

	if (m_pClient->state() == QAbstractSocket::ConnectedState)
	{
		if (m_pProcessManager != NULL)
		{
			if (nType == ActionProcessMgr)
			{
				m_pProcessManager->querySystemProcess();
			}
			else
			{
				m_pProcessManager->stopProcessInfo();
			}
		}

		if (m_pResourceManager != NULL)
		{
			if (nType == ActionResourceMgr)
			{
				m_pResourceManager->sendQueryMessage();
			}
			else
			{
				m_pResourceManager->sendStopMessage();
			}
		}
	}
}

QResourceManager* QWndManager::resourceManager()
{
	return m_pResourceManager;
}

QSystemManager* QWndManager::systemManager()
{
	return m_pSystemManager;
}

QProcessManager* QWndManager::processManager()
{
	return m_pProcessManager;
}
