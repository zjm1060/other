#include "qprocessmanager.h"

#include "common.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"

QProcessManager::QProcessManager(QClient* client, QWidget *parent)
	: QFrame(parent),
	m_pFrameSystemProcess(NULL),
	m_pFrameH9000Process(NULL),
	m_bQuery(false)
{
	m_pClient=client;
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

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
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
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

void QProcessManager::setSystemProcess(const QVector<SystemProcess>& processes, quint64 memory)
{
	if (m_pFrameSystemProcess != NULL)
	{
		m_pFrameSystemProcess->setSystemProcess(processes, memory);
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

		m_pFrameH9000Process->setProcess(vecProcesses, memory);
	}
}

void QProcessManager::recvSystemProcess(int flag, const QVector<SystemProcess>& processes, quint64 memory)
{
	if (flag == QuerySucceed)
	{
		setSystemProcess(processes, memory);
	}
}
 
void QProcessManager::querySystemProcess()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (m_bQuery)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRefProcess, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_SYSTEM_PROCESS);

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

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_PROCESS);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryUnrefProcess, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_SYSTEM_PROCESS);

		m_bQuery=false;
	}
}
