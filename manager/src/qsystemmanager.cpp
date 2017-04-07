#include "qsystemmanager.h"

#include "common.h"
#include "qmessageparser.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#endif

#include "qmessagesystem.h"
#include "qmessagecomputersystem.h"
#include "qmessagenetworkadapter.h"

QSystemManager::QSystemManager(QClient* client, QWidget *parent)
	: QFrame(parent),
	m_pFrameSystem(NULL),
	m_pFrameNetwork(NULL)
{
	m_pClient=client;

	m_layout=new QHBoxLayout(this);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("System"), ButtonSystem);
	m_pButtonList->addButton(tr("NetWork"), ButtonNetWork);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonSystem);
}

QSystemManager::~QSystemManager()
{

}

void QSystemManager::reconnected()
{
	if (m_pFrameSystem != NULL)
	{
		queryOperatingSystem();
		queryComputerSystem();
	}

	if (m_pFrameNetwork != NULL)
	{
		queryNetworkAdapter();
	}
}

void QSystemManager::buttonClicked(int id)
{
	switch (id)
	{
	case ButtonSystem:
		{
			if (m_pFrameSystem == NULL)
			{
			 	m_pFrameSystem=new QFrameSystem(this);
			 	m_pFrameSystem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
			 	m_pFrameSystem->setFrameShape(QFrame::Box);
			 	m_pFrameSystem->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameSystem);

				queryOperatingSystem();
				queryComputerSystem();
			}
			m_pFrameSystem->show();

			SAFE_HIDE_WIDGET(m_pFrameNetwork);
		}
		break;
	case ButtonNetWork:
		{
			SAFE_HIDE_WIDGET(m_pFrameSystem);

			if (m_pFrameNetwork == NULL)
			{
				m_pFrameNetwork=new QFrameNetwork(this);
				m_pFrameNetwork->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameNetwork->setFrameShape(QFrame::Box);
				m_pFrameNetwork->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameNetwork);

				queryNetworkAdapter();
			}
			m_pFrameNetwork->show();
		}
		break;
	default:
		break;
	}
}

void QSystemManager::setOperatingSystem(QVector<OperatingSystem>& system)
{
	if (m_pFrameSystem != NULL)
	{
		m_pFrameSystem->setOperatingSystem(system);
	}
}

void QSystemManager::setComputerSystem(QVector<ComputerSystem>& system)
{
	if (m_pFrameSystem != NULL)
	{
		m_pFrameSystem->setComputerSystem(system);
	}
}

void QSystemManager::setNetworkAdapter(QVector<NetworkAdapter>& adapter)
{
	if (m_pFrameNetwork != NULL)
	{
		m_pFrameNetwork->setNetworkAdapter(adapter);
	}
}

void QSystemManager::queryOperatingSystem()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageSystem::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageSystem::QueryOperatingSystem);

		m_pClient->sendMessage(buffer, QMessageSystem::Type);
	}
}

void QSystemManager::queryComputerSystem()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageComputerSystem::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageComputerSystem::QueryComputerSystem);

		m_pClient->sendMessage(buffer, QMessageComputerSystem::Type);
	}
}

void QSystemManager::queryNetworkAdapter()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageNetworkAdapter::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageNetworkAdapter::QueryNetworkAdapter);

		m_pClient->sendMessage(buffer, QMessageNetworkAdapter::Type);
	}
}
