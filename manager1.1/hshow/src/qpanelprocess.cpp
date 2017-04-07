#include "qpanelprocess.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QMessageBox>

#include "common.h"
#include "qpanelfactory.h"
#include "qframeprocesspanel.h"
#include "qfloatingdockwidget.h"
#include "qreceiverparser.h"
#include "qmessageparser.h"
#include "qipcsocket.h"
#include "qlogger.h"

QPanelProcess::QPanelProcess(QWidget *parent)
	: QAbstractPanel(parent)
	,m_pDockWidget(NULL)
	,m_pPanel(NULL)
	,m_bQuery(false)
{
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_pButton=new QToolButton;
	m_pButton->setIcon(QIcon(":/images/process.png"));
	m_pButton->setText(tr("Process"));
	
	addToolButton(m_pButton);

	m_pButton->installEventFilter(this);

//	connect(m_pButton, SIGNAL(clicked()), this, SLOT(onClickedBtn()));
}

QPanelProcess::~QPanelProcess()
{
	m_pButton->removeEventFilter(this);

	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
}

void QPanelProcess::showWidget(QObject* watched)
{
	Q_UNUSED(watched);

	if (m_pDockWidget == NULL)
	{
		m_pDockWidget=createDockWidget(tr("Process"), 560, 360);

		m_pPanel=new QFrameProcessPanel;
		m_pDockWidget->setWidget(m_pPanel);

		connect(m_pPanel, SIGNAL(terminate(const QString&, quint32)), this, SLOT(queryTerminateProcess(const QString&, quint32)));
		connect(m_pPanel, SIGNAL(execute(const QStringList&)), this, SLOT(queryExecuteProcess(const QStringList&)));
	}

	m_pDockWidget->show();
	m_pDockWidget->raise();
	m_pDockWidget->activateWindow();

	queryProcesses();
}

void QPanelProcess::hideWidget(QObject* watched)
{
	Q_UNUSED(watched);

	if (m_pDockWidget != NULL)
	{
		m_pDockWidget->hide();

		stopProcesses();
	}
}

void QPanelProcess::onClickedBtn()
{

}

void QPanelProcess::recvSystemProcess(int flag, const QVector<SystemProcess>& processes, quint64 memory)
{
	if (flag == QuerySucceed || m_pPanel != NULL)
	{
		m_pPanel->setSystemProcess(processes, memory);
	}
}

void QPanelProcess::recvTerminateProcess(quint32 pid, int result)
{
 	if (result != ExecMethodSucceed)
 	{
 		logplusError(HSHOW_LOGGER_STR, QString("Failed to terminate the process: pid=%1").arg(pid));
 
 		QString strText=tr("Failed to terminate the process pid=%1 !\nPlease check your privilege!").QString::number(pid);
 		QMessageBox::information(this, tr("Terminate Process"), strText, QMessageBox::Ok, QMessageBox::Ok);
 	}
 	else
 	{
 		logplusInfo(HSHOW_LOGGER_STR,  QString("Terminate the process successfully: pid=%1").arg(pid));
 	}
}

void QPanelProcess::recvCreateProcess(const QStringList& command, int result)
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

void QPanelProcess::queryProcesses()
{
	if (m_bQuery)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRefProcess, m_nUsrID);

		QIpcSocket::instance()->sendMessage(buffer, MSG_SYSTEM_PROCESS);

		m_bQuery=true;
	}
}

void QPanelProcess::stopProcesses()
{
	if (!m_bQuery)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryUnrefProcess, m_nUsrID);

		QIpcSocket::instance()->sendMessage(buffer, MSG_SYSTEM_PROCESS);

		m_bQuery=false;
	}
}

void QPanelProcess::queryTerminateProcess(const QString& name, quint32 pid)
{
	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
	if (pMsg != NULL)
	{
		logplusInfo(HSHOW_LOGGER_STR, QString("Try to terminate the process:%1 pid=%2").arg(name).arg(pid));

		Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

		stream<<pid;

		buffer=pMsg->query(QueryTerminateProcess, m_nUsrID, buffer);

		QIpcSocket::instance()->sendMessage(buffer, MSG_SYSTEM_PROCESS);
	}
}

void QPanelProcess::queryExecuteProcess(const QStringList& arguments)
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

REGISTER_PANEL_CLASS(QPanelProcess)
