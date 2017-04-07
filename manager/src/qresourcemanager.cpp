#include "qresourcemanager.h"

#include "qmessageparser.h"
#include "qmessageprocessor.h"
#include "qmessagedisk.h"
#include "qmessagememory.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#endif

QResourceManager::QResourceManager(QClient* client, QWidget *parent)
	: QFrame(parent),
	m_pFrameProcessor(NULL),
	m_pFrameMemory(NULL),
	m_pFrameDisk(NULL),
	m_bQueryMemory(false),
	m_bPerfProcessor(false)
{
	m_pClient=client;

	m_layout=new QHBoxLayout(this);

	m_pButtonList=new QButtonList(this);
	m_pButtonList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	m_pButtonList->setFixedWidth(150);

	m_pButtonList->addButton(tr("Processor"), ButtonProcessor);
	m_pButtonList->addButton(tr("Memory"), ButtonMemory);
	m_pButtonList->addButton(tr("Disk"), ButtonDisk);
	m_pButtonList->addButton(tr("Log"), ButtonLog);

	m_layout->addWidget(m_pButtonList);

	connect(m_pButtonList, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

	m_pButtonList->click(ButtonProcessor);
}

QResourceManager::~QResourceManager()
{

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
	case ButtonLog:
		{
			SAFE_HIDE_WIDGET(m_pFrameProcessor);
			SAFE_HIDE_WIDGET(m_pFrameMemory);
			SAFE_HIDE_WIDGET(m_pFrameDisk);
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
		case ButtonLog:
			break;
		default:
			break;
		}
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

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageProcessor::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageProcessor::QueryProcessor);

		m_pClient->sendMessage(buffer, QMessageProcessor::Type);
	}
}

void QResourceManager::queryPhysicalMemory()
{
	Q_ASSERT_NO_NULL(m_pClient);

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageMemory::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageMemory::QueryPhysicalMemory);

		m_pClient->sendMessage(buffer, QMessageMemory::Type);
	}
}

void QResourceManager::queryPerfProcessor()
{
	Q_ASSERT_NO_NULL(m_pClient);

	if (m_bPerfProcessor)
	{
		return;
	}

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageProcessor::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageProcessor::QueryRuntimeProcessor);

		m_pClient->sendMessage(buffer, QMessageProcessor::Type);

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

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageProcessor::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageProcessor::QueryUnrefRuntimeProcessor);

		m_pClient->sendMessage(buffer, QMessageProcessor::Type);

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

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageMemory::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageMemory::QuerySystemMemory);

		m_pClient->sendMessage(buffer, QMessageMemory::Type);

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

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageMemory::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageMemory::QueryUnrefSystemMemory);

		m_pClient->sendMessage(buffer, QMessageMemory::Type);

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

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageDisk::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageDisk::QueryDiskDrive);
		m_pClient->sendMessage(buffer, QMessageDisk::Type);

		buffer=pMsg->query(QMessageDisk::QueryLogicalDisk);
		m_pClient->sendMessage(buffer, QMessageDisk::Type);
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

	QIMessage* pMsg=QMessageParser::instance()->messageInstance(QMessageDisk::Type);
	if (pMsg != NULL)
	{
		QByteArray buffer=pMsg->query(QMessageDisk::QueryCDROMDrive);
		m_pClient->sendMessage(buffer, QMessageDisk::Type);
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
