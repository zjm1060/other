#include "qwndgroupmanager.h"

#include <QToolBar>
#include <QCloseEvent>

#include "common.h"
#include "qmessageparser.h"
#include "qservermanager.h"
#include "qclientmanager.h"
#include "qglobalinfo.h"
#include "qinfomanager.h"
#include "qdlgmodifypassword.h"

QWndGroupManager::QWndGroupManager(QWidget *parent)
: QMainWindow(parent)
,m_pFileTransferGroupManager(NULL)
,m_pSyncGroupCommitManager(NULL)
,m_pSyncGroupUpdateManager(NULL)
,m_pSyncGroupReleaseManager(NULL)
,m_pInspectManager(NULL)
,m_pConfigureManager(NULL)
,m_pPrivilegeManager(NULL)
{
	init();

	setWindowIcon(QIcon(":/images/workgroup_small.png"));
	setWindowTitle(tr("Workgroup")+QString(" - %1@%2").arg(QGlobalInfo::instance()->user().name).arg(getHostName()));

	int nMinWidth=parseConfig(getAppParentDir()+INI_FILE, "window/minWidth", 720).toInt();
	int nMinHeight=parseConfig(getAppParentDir()+INI_FILE, "window/minHeight", 540).toInt();
	setMinimumSize(nMinWidth, nMinHeight);

	QFrame* pFrame=new QFrame(this);
	m_pLayout=new QVBoxLayout(pFrame);
	pFrame->setLayout(m_pLayout);

	setCentralWidget(pFrame);

	m_pReceiver=new QReceiverPrivilege;
	connect(m_pReceiver, SIGNAL(modifyPasswordSuccess(const QString, const PrivilegeUser)), 
		this, SLOT(modifyPasswordSuccess(const QString, const PrivilegeUser)));
	connect(m_pReceiver, SIGNAL(modifyPasswordUserNonexistent(const QString)), this, SLOT(modifyPasswordUserNonexistent(const QString)));

	loadConfigure();

	showMaximized();
}

QWndGroupManager::~QWndGroupManager()
{
}

void QWndGroupManager::init()
{
	m_pBar=addToolBar(tr("Tool Bar"));
	m_pBar->setFloatable(false);
	m_pBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	int nSize=parseConfig(getAppParentDir()+INI_FILE, "window/iconSize", 64).toInt();
	m_pBar->setIconSize(QSize(nSize, nSize));

	m_pAction[ActionFileTransfer]=new QAction(QIcon(":/images/ftp.png"), tr("File Transfer"), m_pBar);
	m_pAction[ActionFileTransfer]->setCheckable(true);
	m_pAction[ActionFileTransfer]->setData(ActionFileTransfer);

	m_pAction[ActionCommitSync]=new QAction(QIcon(":/images/commit.png"), tr("Commit Update"), m_pBar);
	m_pAction[ActionCommitSync]->setCheckable(true);
	m_pAction[ActionCommitSync]->setData(ActionCommitSync);

	m_pAction[ActionUpdateSync]=new QAction(QIcon(":/images/update.png"), tr("Download Update"), m_pBar);
	m_pAction[ActionUpdateSync]->setCheckable(true);
	m_pAction[ActionUpdateSync]->setData(ActionUpdateSync);

	m_pAction[ActionFileRelease]=new QAction(QIcon(":/images/release.png"), tr("Synchronization"), m_pBar);
	m_pAction[ActionFileRelease]->setCheckable(true);
	m_pAction[ActionFileRelease]->setData(ActionFileRelease);

	m_pAction[ActionConfigure]=new QAction(QIcon(":/images/configure.png"), tr("Configure"), m_pBar);
	m_pAction[ActionConfigure]->setCheckable(true);
	m_pAction[ActionConfigure]->setData(ActionConfigure);

	m_pAction[ActionInspect]=new QAction(QIcon(":/images/inspect.png"), tr("Automatic Inspection"), m_pBar);
	m_pAction[ActionInspect]->setCheckable(true);
	m_pAction[ActionInspect]->setData(ActionInspect);

	m_pAction[ActionPrivileges]=new QAction(tr("Privilege Manager"), m_pBar);
	m_pAction[ActionPrivileges]->setData(ActionPrivileges);

	if (QGlobalInfo::instance()->isLogin())
	{
		QMap<int, quint32> privileges=QGlobalInfo::instance()->privileges();
		if (checkPrivileges(privileges, PrivilegeRole, AdministratorRole))
		{
			m_pAction[ActionPrivileges]->setIcon(QIcon(":/images/privileges.png"));
		}
		else
		{
			m_pAction[ActionPrivileges]->setIcon(QIcon(":/images/modifypwd.png"));
		}
	}

	m_pGroup=new QActionGroup(m_pBar);
	for (int i=0; i<ActionEnd; i++)
	{
		if (i == ActionPrivileges)
		{
			continue;
		}

		m_pGroup->addAction(m_pAction[i]);
	}
	m_pGroup->setExclusive(true);
	connect(m_pGroup, SIGNAL(triggered(QAction*)), this, SLOT(onActionToggled(QAction*)));

	connect(m_pAction[ActionPrivileges], SIGNAL(triggered()), this, SLOT(onToggledActionPrivalege()));

	for (int i=0; i<ActionEnd; i++)
	{
		m_pBar->addAction(m_pAction[i]);
	}
}

void QWndGroupManager::loadConfigure()
{
	QMap<QString, QString> configure;
	QReadConfigureFormXmlFile reader(appendPath(getAppParentDir(), CONFIGURE_SETTING_FILE));
	if (reader.read())
	{
		configure=reader.result();
	}

	labelSettingChanged(configure);

	for (int index=0; index<ActionEnd; index++)
	{
		if (m_pAction[index] != NULL && m_pAction[index]->isVisible())
		{
			m_pAction[index]->trigger();
			break;
		}
	}
}

void QWndGroupManager::closeEvent(QCloseEvent* event)
{
	if (m_pPrivilegeManager != NULL)
	{
		m_pPrivilegeManager->close();
	}

	emit onClosed(this);

	event->accept();
}

void QWndGroupManager::onToggledActionPrivalege()
{
	QMap<int, quint32> privileges=QGlobalInfo::instance()->privileges();
	if (checkPrivileges(privileges, PrivilegeRole, AdministratorRole))
	{
		//有权限管理帐号
		if (m_pPrivilegeManager == NULL)
		{
			m_pPrivilegeManager=new QMainWndPrivilege;

			connect(m_pPrivilegeManager, SIGNAL(onClosed(QWidget*)), this, SLOT(onClosedPrivelege(QWidget*)));
			m_pPrivilegeManager->show();
		}
		else
		{
			m_pPrivilegeManager->raise();
			m_pPrivilegeManager->activateWindow();
		}
	}
	else
	{
		QDlgModifyPassword dialog(findPixmap(":/images/user_icon.png"));
		dialog.setUser(QGlobalInfo::instance()->user());
		dialog.setModal(true);
		if (dialog.exec() != 0)
		{
			PrivilegeUser usr=dialog.user();

			m_pReceiver->queryModifyUserPassword(usr.name, usr.passwd);
		}
	}
}

void QWndGroupManager::onClosedPrivelege(QWidget* widget)
{
	if (widget == m_pPrivilegeManager)
	{
		SAFE_DELETE_CLASS(m_pPrivilegeManager);
	}
}

void QWndGroupManager::modifyPasswordSuccess(const QString usrName, const PrivilegeUser user)
{
	QInfoManager::instance()->append(QInfoManager::Important, 
		tr("User <%1> password updated successfully.").arg(usrName));

	QGlobalInfo::instance()->setUser(user);
}

void QWndGroupManager::modifyPasswordUserNonexistent(const QString usrName)
{
	QInfoManager::instance()->append(QInfoManager::Error, 
		tr("Failed to modify the user <%1> password! The user does not exist!").arg(usrName));
}

void QWndGroupManager::setServerList(const QList<QPair<ServerInfo, int> >& servers)
{
	if (m_pFileTransferGroupManager != NULL)
	{
		m_pFileTransferGroupManager->setServerList(servers);
	}

	if (m_pSyncGroupCommitManager != NULL)
	{
		m_pSyncGroupCommitManager->setServerList(servers);
	}

	if (m_pSyncGroupUpdateManager != NULL)
	{
		m_pSyncGroupUpdateManager->setServerList(servers);
	}

	if (m_pSyncGroupReleaseManager != NULL)
	{
		m_pSyncGroupReleaseManager->setServerList(servers);
	}

	if (m_pInspectManager != NULL)
	{
		m_pInspectManager->setServerList(servers);
	}

	if (m_pConfigureManager != NULL)
	{
		m_pConfigureManager->setServerList(servers);
	}
}

void QWndGroupManager::onActionToggled(QAction* action)
{
	int nType=action->data().toInt();

	switch (nType)
	{
	case ActionFileTransfer:
		{
			SAFE_HIDE_WIDGET(m_pSyncGroupCommitManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupUpdateManager);
			SAFE_HIDE_WIDGET(m_pInspectManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupReleaseManager);
			SAFE_HIDE_WIDGET(m_pConfigureManager);

			if (m_pFileTransferGroupManager == NULL)
			{
				m_pFileTransferGroupManager=new QFileTransferGroupManager(this);
				m_pLayout->addWidget(m_pFileTransferGroupManager);
			}

			m_pFileTransferGroupManager->setServerList(QGlobalInfo::instance()->serverList());
			m_pFileTransferGroupManager->show();
		}
		break;
	case ActionCommitSync:
		{
			SAFE_HIDE_WIDGET(m_pFileTransferGroupManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupUpdateManager);
			SAFE_HIDE_WIDGET(m_pInspectManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupReleaseManager);
			SAFE_HIDE_WIDGET(m_pConfigureManager);

			if (m_pSyncGroupCommitManager == NULL)
			{
				m_pSyncGroupCommitManager=new QSyncGroupCommitManager(this);
				m_pLayout->addWidget(m_pSyncGroupCommitManager);
			}

			m_pSyncGroupCommitManager->setServerList(QGlobalInfo::instance()->serverList());
			m_pSyncGroupCommitManager->show();
		}
		break;
	case ActionUpdateSync:
		{
			SAFE_HIDE_WIDGET(m_pFileTransferGroupManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupCommitManager);
			SAFE_HIDE_WIDGET(m_pInspectManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupReleaseManager);
			SAFE_HIDE_WIDGET(m_pConfigureManager);

			if (m_pSyncGroupUpdateManager == NULL)
			{
				m_pSyncGroupUpdateManager=new QSyncGroupUpdateManager(this);
				m_pLayout->addWidget(m_pSyncGroupUpdateManager);
			}

			m_pSyncGroupUpdateManager->setServerList(QGlobalInfo::instance()->serverList());
			m_pSyncGroupUpdateManager->show();
		}
		break;
	case ActionFileRelease:
		{
			SAFE_HIDE_WIDGET(m_pFileTransferGroupManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupCommitManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupUpdateManager);
			SAFE_HIDE_WIDGET(m_pInspectManager);
			SAFE_HIDE_WIDGET(m_pConfigureManager);

			if (m_pSyncGroupReleaseManager == NULL)
			{
				m_pSyncGroupReleaseManager=new QSyncGroupReleaseManager(this);
				m_pLayout->addWidget(m_pSyncGroupReleaseManager);
			}

			m_pSyncGroupReleaseManager->setServerList(QGlobalInfo::instance()->serverList());
			m_pSyncGroupReleaseManager->show();
		}
		break;
	case ActionConfigure:
		{
			SAFE_HIDE_WIDGET(m_pFileTransferGroupManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupCommitManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupUpdateManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupReleaseManager);
			SAFE_HIDE_WIDGET(m_pInspectManager);

			if (m_pConfigureManager == NULL)
			{
				m_pConfigureManager=new QConfigureGroupManager(this);
				m_pLayout->addWidget(m_pConfigureManager);

				connect(m_pConfigureManager, SIGNAL(labelSettingChanged(const QMap<QString, QString>&)),
					this, SLOT(labelSettingChanged(const QMap<QString, QString>&)));
			}

			m_pConfigureManager->setServerList(QGlobalInfo::instance()->serverList());
			m_pConfigureManager->show();
		}
		break;
	case ActionInspect:
		{
			SAFE_HIDE_WIDGET(m_pFileTransferGroupManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupCommitManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupUpdateManager);
			SAFE_HIDE_WIDGET(m_pSyncGroupReleaseManager);
			SAFE_HIDE_WIDGET(m_pConfigureManager);

			if (m_pInspectManager == NULL)
			{
				m_pInspectManager=new QInspectManager(this);
				m_pLayout->addWidget(m_pInspectManager);
			}

			m_pInspectManager->setServerList(QGlobalInfo::instance()->serverList());
			m_pInspectManager->show();
		}
		break;
	default:
		break;
	}
}

void QWndGroupManager::labelSettingChanged(const QMap<QString, QString>& configure)
{
	for (int index=0; index<ActionEnd; index++)
	{
		QString strConfigure;

		switch (index)
		{
		case ActionFileTransfer:
			strConfigure="FileTransfer";
			break;
		case ActionCommitSync:
			strConfigure="CommitSync";
			break;
		case ActionUpdateSync:
			strConfigure="UpdateSync";
			break;
		case ActionFileRelease:
			strConfigure="FileRelease";
			break;
		case ActionInspect:
			strConfigure="Inspect";
			break;
		default:
			break;
		}

		if (strConfigure.isEmpty())
		{
			continue;
		}

		if (!configure.contains(strConfigure) || configure[strConfigure] == "true")
		{
			m_pAction[index]->setVisible(true);
		}
		else
		{
			m_pAction[index]->setVisible(false);
		}
	}
}
