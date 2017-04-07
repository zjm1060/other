#include "qprocessmanager.h"

#include "common.h"
#include "qmessageparser.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#endif

#include "qmessageprocess.h"
#include "qmessagecomputersystem.h"
#include "qmessagenetworkadapter.h"

QProcessManager::QProcessManager(QClient* client, QWidget *parent)
	: QFrame(parent),
	m_pFrameSystemProcess(NULL),
	m_pFrameH9000Process(NULL),
	m_bQuery(false)
{
	m_pClient=client;

	m_layout=new QHBoxLayout(this);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("H9000 Processes"), ButtonH9000Process);
	m_pButtonList->addButton(tr("System Processes"), ButtonSystemProcess);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonH9000Process);
}

QProcessManager::~QProcessManager()
{

}

void QProcessManager::buttonClicked(int id)
{
	switch (id)
	{
	case ButtonH9000Process:
		{
			if (m_pFrameH9000Process == NULL)
			{
				m_pFrameH9000Process=new QFrameH9000Process(this);
				m_pFrameH9000Process->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameH9000Process->setFrameShape(QFrame::Box);
				m_pFrameH9000Process->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameH9000Process);
			}
			m_pFrameH9000Process->show();

			SAFE_HIDE_WIDGET(m_pFrameSystemProcess);
		}
		break;
	case ButtonSystemProcess:
		{
			SAFE_HIDE_WIDGET(m_pFrameH9000Process);

			if (m_pFrameSystemProcess == NULL)
			{
			 	m_pFrameSystemProcess=new QFrameSystemProcess(m_pClient, this);
			 	m_pFrameSystemProcess->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
			 	m_pFrameSystemProcess->setFrameShape(QFrame::Box);
			 	m_pFrameSystemProcess->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSystemProcess);
			}
			m_pFrameSystemProcess->show();
		}
		break;
	default:
		break;
	}
}

void QProcessManager::setSystemProcess(QVector<SystemProcess>& processes)
{
	if (m_pFrameSystemProcess != NULL)
	{
		m_pFrameSystemProcess->setSystemProcess(processes);
	}

	if (m_pFrameH9000Process != NULL)
	{
		QVector<SystemProcess> vecProcesses;
		foreach (SystemProcess process, processes)
		{
			if (process.executablePath.contains("H9000", Qt::CaseInsensitive))
			{
				vecProcesses.append(process);
			}
		}

		m_pFrameH9000Process->setProcess(vecProcesses);
	}
}
 
void QProcessManager::querySystemProcess()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (m_bQuery)
	{
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageProcess::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageProcess::QueryProcess);

		m_pClient->sendMessage(buffer, QMessageProcess::Type);

		m_bQuery=true;
	}
}

void QProcessManager::stopProcessInfo()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (!m_bQuery)
	{
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageProcess::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageProcess::QueryUnrefProcess);

		m_pClient->sendMessage(buffer, QMessageProcess::Type);

		m_bQuery=false;
	}
}
