#include "qpanelfilesystem.h"

#include <QHBoxLayout>
#include <QToolButton>

#include "common.h"
#include "qpanelfactory.h"
#include "qframefilesystempanel.h"
#include "qfloatingdockwidget.h"
#include "qreceiverparser.h"
#include "qmessageparser.h"
#include "qipcsocket.h"

QPanelFileSystem::QPanelFileSystem(QWidget *parent)
	: QAbstractPanel(parent)
	,m_pDockWidget(NULL)
	,m_pPanel(NULL)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_pButton=new QToolButton;
	m_pButton->setIcon(QIcon(":/images/disk.png"));
	m_pButton->setText(tr("Computer"));
	
	addToolButton(m_pButton);

	m_pButton->installEventFilter(this);

//	connect(m_pButton, SIGNAL(clicked()), this, SLOT(onClickedBtn()));
}

QPanelFileSystem::~QPanelFileSystem()
{
	m_pButton->removeEventFilter(this);

	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
}

void QPanelFileSystem::showWidget(QObject* watched)
{
	Q_UNUSED(watched);

	if (m_pDockWidget == NULL)
	{
		m_pDockWidget=createDockWidget(tr("File System"), 560, 360);

		m_pPanel=new QFrameFileSystemPanel;
		m_pDockWidget->setWidget(m_pPanel);
	}

	m_pDockWidget->show();
	m_pDockWidget->raise();
	m_pDockWidget->activateWindow();

	queryLogicalDisk();
}

void QPanelFileSystem::hideWidget(QObject* watched)
{
	Q_UNUSED(watched);

	if (m_pDockWidget != NULL)
	{
		m_pDockWidget->hide();
	}
}

void QPanelFileSystem::onClickedBtn()
{
	if (m_pDockWidget == NULL)
	{
		return;
	}
}

void QPanelFileSystem::queryLogicalDisk()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_DISK_DRIVE);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryLogicalDisk, m_nUsrID);
		QIpcSocket::instance()->sendMessage(buffer, MSG_DISK_DRIVE);
	}
}

void QPanelFileSystem::recvLogicalDisk(int flag, const QVector<LogicalDisk>& disc)
{
	if (flag == QuerySucceed && m_pPanel != NULL)
	{
		m_pPanel->setLogicalDisk(disc);
	}
}

REGISTER_PANEL_CLASS(QPanelFileSystem)
