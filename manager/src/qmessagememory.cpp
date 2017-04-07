#include "qmessagememory.h"

#include <QThreadPool>

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#include "qwmiphysicalmemory.h"
#else
#include "qstatfactory.h"
#endif

#include "qmemorysender.h"

QMessageMemory::QMessageMemory(QObject *parent)
	: QObject(parent)
{

}

QMessageMemory::~QMessageMemory()
{

}

QByteArray QMessageMemory::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(usrID);

	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<msgType;

	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageMemory::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageMemoryRunnable* pRunnable=new QMessageMemoryRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, QByteArray)), this, SLOT(finished(QConnection*, int, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageMemory::finished(QConnection* sender, int msgType, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	if (msgType == QuerySystemMemory)
	{
		QMemorySender::instance()->addClient(sender);
		return;
	}
	else if (msgType == QueryUnrefSystemMemory)
	{
		QMemorySender::instance()->removeClient(sender);
		return;
	}

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QueryPhysicalMemory:
		in<<AnswerPhysicalMemory;
		break;
	default:
		return;
	}

	if (buffer.size() > 0)
	{
		in.writeRawData(buffer.data(), buffer.size());
	}

	sender->sendMessage(buf, Type);
}

QMessageMemoryRunnable::QMessageMemoryRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageMemoryRunnable::~QMessageMemoryRunnable()
{

}

void QMessageMemoryRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageMemoryRunnable::run()
{
	int nMsgType;

	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	switch (nMsgType)
	{
	case QMessageMemory::QueryPhysicalMemory:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_PHYSICAL_MEMORY);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_PHYSICAL_MEMORY);
#endif
			
			QVector<PhysicalMemory> vecMemory=var.value< QVector<PhysicalMemory> >();
			if (vecMemory.isEmpty())
			{
				stream<<QMessageMemory::QueryFailed;
			}
			else
			{
				stream<<QMessageMemory::QuerySucceed;
				stream<<vecMemory.size();
				for (int i=0; i<vecMemory.size(); i++)
				{
					const PhysicalMemory& memory=vecMemory[i];
					stream<<memory.capacity<<memory.dataWidth<<memory.description<<memory.manufacturer
						<<memory.memoryType<<memory.name<<memory.serialNumber<<memory.speed
						<<memory.tag<<memory.version;
				}
			}

			emit finished(m_pSender, QMessageMemory::QueryPhysicalMemory, buffer);
		}
		break;
	case QMessageMemory::AnswerPhysicalMemory:
		{
			QVector<PhysicalMemory> vecMemory;
			PhysicalMemory memory;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageMemory::QuerySucceed)
			{
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>memory.capacity>>memory.dataWidth>>memory.description>>memory.manufacturer
						>>memory.memoryType>>memory.name>>memory.serialNumber>>memory.speed
						>>memory.tag>>memory.version;

					vecMemory.append(memory);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showPhysicalMemroy", 
				Q_ARG(int, nQueryFlag), Q_ARG(QVector<PhysicalMemory>, vecMemory));
		}
		break;
	case QMessageMemory::QuerySystemMemory:
	case QMessageMemory::QueryUnrefSystemMemory:
		{
			emit finished(m_pSender, nMsgType);
		}
		break;
	case QMessageMemory::AnswerSystemMemory:
		{
			if (m_pView == NULL)
			{
				return;
			}

			SystemMemory memory;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageMemory::QuerySucceed)
			{
				out>>memory.datetime>>memory.freePhysicalMemory>>memory.freeVirtualMemory>>memory.totalVisibleMemorySize
					>>memory.totalVirtualMemorySize>>memory.freeSpaceInPagingFiles>>memory.sizeStoredInPagingFiles
					>>memory.sizeUsageInPagingFiles>>memory.sizeUsageInPagingFilesPeak
					>>memory.systemCache>>memory.poolNonpaged>>memory.poolPaged;
			}

			QMetaObject::invokeMethod(m_pView, "showSystemMemory", Q_ARG(int, nQueryFlag), Q_ARG(SystemMemory, memory));
		}
		break;
	default:
		break;
	}
}

