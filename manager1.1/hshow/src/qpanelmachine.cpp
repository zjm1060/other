#include "qpanelmachine.h"

#include <QHBoxLayout>
#include <QToolButton>

#include "common.h"
#include "qpanelfactory.h"
#include "qframesystem.h"
#include "qfloatingdockwidget.h"
#include "qreceiverparser.h"
#include "qmessageparser.h"
#include "qipcsocket.h"

QPanelMachine::QPanelMachine(QWidget *parent)
	: QAbstractPanel(parent)
	,m_pDockWidget(NULL)
	,m_pPanel(NULL)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_pButton=new QToolButton;
	m_pButton->setIcon(QIcon(":/images/system.png"));
	m_pButton->setText(tr("System"));

	addToolButton(m_pButton);

	m_pButton->installEventFilter(this);
}

QPanelMachine::~QPanelMachine()
{
	m_pButton->removeEventFilter(this);

	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
}

void QPanelMachine::recvOperatingSystem(int flag, const OperatingSystem& system)
{
	Q_ASSERT_NO_NULL(m_pPanel);

	if (flag == QuerySucceed)
	{
		m_pPanel->setOperatingSystem(system);
	}
}

void QPanelMachine::recvComputerSystem(int flag, const ComputerSystem& system)
{
	Q_ASSERT_NO_NULL(m_pPanel);

	if (flag == QuerySucceed)
	{
		m_pPanel->setComputerSystem(system);
	}
}

void QPanelMachine::showWidget(QObject* watched)
{
	Q_UNUSED(watched);

	if (m_pDockWidget == NULL)
	{
		m_pDockWidget=createDockWidget(tr("System"), 640, 480);

		m_pPanel=new QFrameSystem;
		m_pDockWidget->setWidget(m_pPanel);
	}

	m_pDockWidget->show();
	m_pDockWidget->raise();
	m_pDockWidget->activateWindow();

	queryOperatingSystem();
	queryComputerSystem();
}

void QPanelMachine::hideWidget(QObject* watched)
{
	Q_UNUSED(watched);

	if (m_pDockWidget != NULL)
	{
		m_pDockWidget->hide();
	}
}

void QPanelMachine::queryOperatingSystem()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryOperatingSystem, m_nUsrID);

		QIpcSocket::instance()->sendMessage(buffer, MSG_SYSTEM);
	}
}

void QPanelMachine::queryComputerSystem()
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_COMPUTER_SYSTEM);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryComputerSystem, m_nUsrID);

		QIpcSocket::instance()->sendMessage(buffer, MSG_COMPUTER_SYSTEM);
	}
}

REGISTER_PANEL_CLASS(QPanelMachine)
