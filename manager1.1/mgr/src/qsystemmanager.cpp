#include "qsystemmanager.h"

#include "common.h"
#include "qmessageparser.h"
#include "qreceiverparser.h"

QSystemManager::QSystemManager(QClient* client, QWidget *parent)
	: QFrame(parent),
	m_pFrameSystem(NULL),
	m_pFrameNetwork(NULL)
{
	m_pClient=client;
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

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
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
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

void QSystemManager::recvOperatingSystem(int flag, const OperatingSystem& system)
{
	if (flag == QuerySucceed)
	{
		setOperatingSystem(system);
	}
}

void QSystemManager::recvComputerSystem(int flag, const ComputerSystem& system)
{
	if (flag == QuerySucceed)
	{
		setComputerSystem(system);
	}
}

void QSystemManager::recvNetworkAdapter(int flag, const QVector<NetworkAdapter>& adapters)
{
	if (flag == QuerySucceed)
	{
		setNetworkAdapter(adapters);
	}
}

void QSystemManager::setOperatingSystem(const OperatingSystem& system)
{
	if (m_pFrameSystem != NULL)
	{
		m_pFrameSystem->setOperatingSystem(system);
	}
}

void QSystemManager::setComputerSystem(const ComputerSystem& system)
{
	if (m_pFrameSystem != NULL)
	{
		m_pFrameSystem->setComputerSystem(system);
	}
}

void QSystemManager::setNetworkAdapter(const QVector<NetworkAdapter>& adapter)
{
	if (m_pFrameNetwork != NULL)
	{
		m_pFrameNetwork->setNetworkAdapter(adapter);
	}
}

void QSystemManager::queryOperatingSystem()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryOperatingSystem, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_SYSTEM);
	}
}

void QSystemManager::queryComputerSystem()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_COMPUTER_SYSTEM);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryComputerSystem, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_COMPUTER_SYSTEM);
	}
}

void QSystemManager::queryNetworkAdapter()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_NETWORK_ADAPTER);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryNetworkAdapter, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_NETWORK_ADAPTER);
	}
}
