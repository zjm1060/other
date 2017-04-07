#include "qpanelmenu.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QActionGroup>
#include <QMenu>
#include <QMessageBox>

#include "common.h"
#include "qpanelfactory.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"
#include "qipcsocket.h"
#include "qlogger.h"

QPanelMenu::QPanelMenu(QWidget *parent)
	: QAbstractPanel(parent)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_pGroup=new QActionGroup(this);
	connect(m_pGroup, SIGNAL(triggered(QAction*)), this, SLOT(onActionToggled(QAction*)));

	loadConfigure();
}

QPanelMenu::~QPanelMenu()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
}

void QPanelMenu::loadConfigure()
{
	QReadPanelMenuConfigureFromXmlFile reader(appendPath(getAppParentDir(), "/ini/hshow.xml"));
	reader.read();

	PanelMenuBarConfigure configure=reader.result();
	for (int i=0; i<configure.menus.size(); i++)
	{
		const PanelMenuConfigure& conf=configure.menus[i];

		QToolButton* pButton=new QToolButton;
		pButton->setText(conf.name);
		pButton->setPopupMode(QToolButton::InstantPopup);
		pButton->setAutoRaise(true);
		pButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

		m_pLayout->addWidget(pButton);

		QMenu* pMenu=new QMenu(conf.name);
		pButton->setMenu(pMenu);

		loadMenu(pMenu, conf);
	}
}

void QPanelMenu::loadMenu(QMenu* menu, const PanelMenuConfigure& configure)
{
	if (menu == NULL)
	{
		return;
	}

	int nIndex=0, nCount=configure.menus.size()+configure.actions.size();
	while (nCount > 0)
	{
		if (configure.menus.contains(nIndex))
		{
			nCount--;

			const PanelMenuConfigure& conf=configure.menus[nIndex];

			QMenu* pMenu=new QMenu(conf.name);
			menu->addMenu(pMenu);

			loadMenu(pMenu, conf);
		}
		else if (configure.actions.contains(nIndex))
		{
			nCount--;

			const PanelActionConfigure& conf=configure.actions[nIndex];

			QAction* pAction=new QAction(conf.name, menu);
			pAction->setData(conf.command);

			m_pGroup->addAction(pAction);

			menu->addAction(pAction);
		}

		nIndex++;
	}
}

void QPanelMenu::onActionToggled(QAction* action)
{
	if (action == NULL)
	{
		return;
	}

	QString strCommand=action->data().toString();
	if (strCommand.isEmpty())
	{
		logplusWarning(HSHOW_LOGGER_STR, QString("Command is empty!Please check your commands!"));
		return;
	}

	QStringList arguments(strCommand);

	queryExecuteProcess(arguments);
}

void QPanelMenu::recvCreateProcess(const QStringList& command, int result)
{
	QString strCommand;
	foreach (QString argument, command)
	{
		strCommand+=argument+" ";
	}

	if (result != ExecMethodSucceed)
	{
		logplusError(HSHOW_LOGGER_STR, QString("Failed to create the task:%1").arg(strCommand));

		QString strText=tr("Failed to create the process \" %1 \"!\nPlease check your command, or you have insufficient privilege!").arg(strCommand);
		QMessageBox::information(this, tr("Create Process"), strText, QMessageBox::Ok, QMessageBox::Ok);
	}
	else
	{
		logplusInfo(HSHOW_LOGGER_STR, QString("Create the task successfully:%1").arg(strCommand));
	}
}

void QPanelMenu::queryExecuteProcess(const QStringList& arguments)
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
	if (pMsg != NULL)
	{
		Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

		QString strText;
		QStringListIterator it(arguments);
		while (it.hasNext())
		{
			strText+=it.next()+" ";
		}
		logplusInfo(HSHOW_LOGGER_STR, QString("Try to create the new task: %1").arg(strText));

		stream<<arguments;

		buffer=pMsg->query(QueryCreateProcess, m_nUsrID, buffer);

		QIpcSocket::instance()->sendMessage(buffer, MSG_SYSTEM_PROCESS);
	}
}

REGISTER_PANEL_CLASS(QPanelMenu)
