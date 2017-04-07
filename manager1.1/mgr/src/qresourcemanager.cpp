#include "qresourcemanager.h"

#include "qmessageparser.h"

QResourceManager::QResourceManager(QClient* client, QWidget *parent)
	: QFrame(parent)
	,m_pFrameProcessor(NULL)
	,m_pFrameMemory(NULL)
	,m_pFrameDisk(NULL)
	,m_bQueryMemory(false)
	,m_bPerfProcessor(false)
{
	m_pClient=client;
	m_nUsrID=QReceiverParser::instance()->registerInterface(this);

	m_layout=new QHBoxLayout(this);
	setLayout(m_layout);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("Processor"), ButtonProcessor);
	m_pButtonList->addButton(tr("Memory"), ButtonMemory);
	m_pButtonList->addButton(tr("Disk"), ButtonDisk);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonProcessor);
}

QResourceManager::~QResourceManager()
{
	QReceiverParser::instance()->unregisterInterface(m_nUsrID);
}

void QResourceManager::buttonClicked(int id)
{
	switch (id)
	{
	case ButtonProcessor:
		{
			if (m_pFrameProcessor == NULL)
			{
				m_pFrameProcessor=new QFrameProcessor(this);
				m_pFrameProcessor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameProcessor->setFrameShape(QFrame::Box);
				m_pFrameProcessor->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameProcessor);

				queryProcessors();
				queryPerfProcessor();
			}
			m_pFrameProcessor->show();

			SAFE_HIDE_WIDGET(m_pFrameMemory);
			SAFE_HIDE_WIDGET(m_pFrameDisk);
		}
		break;
	case ButtonMemory:
		{
			SAFE_HIDE_WIDGET(m_pFrameProcessor);

			if (m_pFrameMemory == NULL)
			{
				m_pFrameMemory=new QFrameMemory(this);
				m_pFrameMemory->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameMemory->setFrameShape(QFrame::Box);
				m_pFrameMemory->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameMemory);

				queryPhysicalMemory();
				querySystemMemory();
			}
			m_pFrameMemory->show();

 			SAFE_HIDE_WIDGET(m_pFrameDisk);
		}
		break;
	case ButtonDisk:
		{
			SAFE_HIDE_WIDGET(m_pFrameProcessor);
			SAFE_HIDE_WIDGET(m_pFrameMemory);

			if (m_pFrameDisk == NULL)
			{
				m_pFrameDisk=new QFrameDisk(this);
				m_pFrameDisk->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
				m_pFrameDisk->setFrameShape(QFrame::Box);
				m_pFrameDisk->setFrameShadow(QFrame::Raised);

				m_layout->addWidget(m_pFrameDisk);
			}
			m_pFrameDisk->show();
			queryDrive();
		}
		break;
	default:
		break;
	}
}

void QResourceManager::reconnected(bool connected)
{
	if (connected)
	{
		int nClickButton=m_pButtonList->clickedButton();
		switch (nClickButton)
		{
		case ButtonProcessor:
			{
				if (m_pFrameProcessor != NULL)
				{
					m_bPerfProcessor=false;
					queryPerfProcessor();
				}
			}
			break;
		case ButtonMemory:
			{
				if (m_pFrameMemory != NULL)
				{
					m_bQueryMemory=false;
					querySystemMemory();
				}
			}
			break;
		case ButtonDisk:
			{
				if (m_pFrameDisk != NULL)
				{
					queryDrive();
				}
			}
			break;
		default:
			break;
		}
	}
}

void QResourceManager::recvProcessor(int flag, const QVector<ProcessorInfo>& processors)
{
	if (flag == QuerySucceed)
	{
		setProcessors(processors);
	}
}

void QResourceManager::recvPerfProcessor(int flag, const PerfProcessor& processor)
{
	if (flag == QuerySucceed)
	{
		setPerfProcessor(processor);
	}
}

void QResourceManager::recvPhysicalMemroy(int flag, const QVector<PhysicalMemory>& memories)
{
	if (flag == QuerySucceed)
	{
		setPhysicalMemory(memories);
	}
}

void QResourceManager::recvSystemMemory(int flag, const SystemMemory& memory)
{
	if (flag == QuerySucceed)
	{
		setSystemMemory(memory);
	}
}

void QResourceManager::recvDiskDrive(int flag, const QVector<DiskDrive>& drives)
{
	if (flag == QuerySucceed)
	{
		setDiskDrive(drives);
	}
}

void QResourceManager::recvCDROMDrive(int flag, const QVector<CDROMDrive>& drives)
{
	if (flag == QuerySucceed)
	{
		setCDROMDrive(drives);
	}
}

void QResourceManager::recvLogicalDisk(int flag, const QVector<LogicalDisk>& disc)
{
	if (flag == QuerySucceed)
	{
		setLogicalDisk(disc);
	}
}

void QResourceManager::sendQueryMessage()
{
	if (m_pFrameProcessor != NULL)
	{
		queryProcessors();
		queryPerfProcessor();
	}

	if (m_pFrameMemory != NULL)
	{
		queryPhysicalMemory();
		querySystemMemory();
	}
}

void QResourceManager::sendStopMessage()
{
	if (m_pFrameProcessor != NULL)
	{
		stopPerfProcessor();
	}

	if (m_pFrameMemory != NULL)
	{
		stopSystemMemory();
	}
}

void QResourceManager::queryProcessors()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_PROCESSOR);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryProcessor, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_PROCESSOR);
	}
}

void QResourceManager::queryPhysicalMemory()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_MEMORY);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryPhysicalMemory, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_SYSTEM_MEMORY);
	}
}

void QResourceManager::queryPerfProcessor()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (m_bPerfProcessor)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_PROCESSOR);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRefRuntimeProcessor, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_PROCESSOR);

		m_bPerfProcessor=true;
	}
}

void QResourceManager::stopPerfProcessor()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (!m_bPerfProcessor)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_PROCESSOR);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryUnrefRuntimeProcessor, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_PROCESSOR);

		m_bPerfProcessor=false;
	}

	if (m_pFrameProcessor != NULL)
	{
		m_pFrameProcessor->clear();
	}
}

void QResourceManager::querySystemMemory()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (m_bQueryMemory)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_MEMORY);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryRefSystemMemory, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_SYSTEM_MEMORY);

		m_bQueryMemory=true;
	}
}

void QResourceManager::stopSystemMemory()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (!m_bQueryMemory)
	{
		return;
	}

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_SYSTEM_MEMORY);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryUnrefSystemMemory, m_nUsrID);

		m_pClient->sendMessage(buffer, MSG_SYSTEM_MEMORY);

		m_bQueryMemory=false;
	}

	if (m_pFrameMemory != NULL)
	{
		m_pFrameMemory->clear();
	}
}

void QResourceManager::queryDrive()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_DISK_DRIVE);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryDiskDrive, m_nUsrID);
		m_pClient->sendMessage(buffer, MSG_DISK_DRIVE);

		buffer=pMsg->query(QueryLogicalDisk, m_nUsrID);
		m_pClient->sendMessage(buffer, MSG_DISK_DRIVE);
	}
}

void QResourceManager::setProcessors(const QVector<ProcessorInfo>& processors)
{
	if (m_pFrameProcessor != NULL)
	{
		m_pFrameProcessor->setProcessors(processors);
	}
}

void QResourceManager::setPerfProcessor(const PerfProcessor& processor)
{
	if (m_pFrameProcessor != NULL)
	{
		m_pFrameProcessor->setPerfProcessor(processor);
	}
}

void QResourceManager::setPhysicalMemory(const QVector<PhysicalMemory>& memory)
{
	if (m_pFrameMemory != NULL)
	{
		m_pFrameMemory->setMemoryDevice(memory);
	}
}

void QResourceManager::setSystemMemory(const SystemMemory& memory)
{
	if (m_pFrameMemory != NULL)
	{
		m_pFrameMemory->setSystemMemory(memory);
	}
}

void QResourceManager::setDiskDrive(const QVector<DiskDrive>& diskDrives)
{
	if (m_pFrameDisk != NULL)
	{
		m_pFrameDisk->setDiskDrive(diskDrives);
	}

	Q_ASSERT_NO_NULL(m_pClient);

	QIMessagePtr pMsg=QMessageParser::instance()->messageInstance(MSG_DISK_DRIVE);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QueryCDROMDrive, m_nUsrID);
		m_pClient->sendMessage(buffer, MSG_DISK_DRIVE);
	}
}

void QResourceManager::setCDROMDrive(const QVector<CDROMDrive>& drives)
{
	if (m_pFrameDisk != NULL)
	{
		m_pFrameDisk->setCDROMDrive(drives);
	}
}

void QResourceManager::setLogicalDisk(const QVector<LogicalDisk>& disk)
{
	if (m_pFrameDisk != NULL)
	{
		m_pFrameDisk->setLogicalDisk(disk);
	}
}
