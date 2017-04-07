#include "qwndgroupmanager.h"

#include <QToolBar>
#include <QCloseEvent>

#include "common.h"
#include "qmessageparser.h"
#include "qservermanager.h"
#include "qclientmanager.h"

QWndGroupManager::QWndGroupManager(QWidget *parent)
: QMainWindow(parent),
m_pSyncGroupManager(NULL)
{
	init();

	setWindowIcon(QIcon(":/images/workgroup_small.png"));
	setWindowTitle(tr("Workgroup"));

	QFrame* pFrame=new QFrame(this);
	m_pLayout=new QVBoxLayout(pFrame);
	pFrame->setLayout(m_pLayout);

	setCentralWidget(pFrame);

	m_pAction[ActionSynchronization]->trigger();
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

	m_pAction[ActionSynchronization]=new QAction(QIcon(":/images/sync.png"), tr("Synchronization"), m_pBar);
	m_pAction[ActionSynchronization]->setCheckable(true);
	m_pAction[ActionSynchronization]->setData(ActionSynchronization);

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

void QWndGroupManager::closeEvent(QCloseEvent* event)
{
	emit onClosed(this);

	event->accept();
}

void QWndGroupManager::onActionToggled(QAction* action)
{
	int nType=action->data().toInt();

	switch (nType)
	{
	case ActionSynchronization:
		{
			if (m_pSyncGroupManager == NULL)
			{
				m_pSyncGroupManager=new QSyncGroupManager(this);
				m_pLayout->addWidget(m_pSyncGroupManager);
			}
			m_pSyncGroupManager->show();
		}
		break;
	default:
		break;
	}
}
