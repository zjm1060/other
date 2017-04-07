#include "qmessagedisk.h"

#include <QThreadPool>

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

QMessageDisk::QMessageDisk(QObject *parent)
	: QObject(parent)
{

}

QMessageDisk::~QMessageDisk()
{

}

QByteArray QMessageDisk::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);
	Q_UNUSED(usrID);

	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<msgType;

	return buffer;
}

bool QMessageDisk::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageDiskRunnable* pRunnable=new QMessageDiskRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, QByteArray)), this, SLOT(finished(QConnection*, int, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageDisk::finished(QConnection* sender, int msgType, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QueryDiskDrive:
		in<<AnswerDiskDrive;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryCDROMDrive:
		in<<AnswerCDROMDrive;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	case QueryLogicalDisk:
		in<<AnswerLogicalDisk;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	default:
		break;
	}

	sender->sendMessage(buf, Type);
}

QMessageDiskRunnable::QMessageDiskRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageDiskRunnable::~QMessageDiskRunnable()
{

}

void QMessageDiskRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageDiskRunnable::run()
{
	int nMsgType;

	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	switch (nMsgType)
	{
	case QMessageDisk::QueryDiskDrive:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_DISK_DRIVE);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_DISK_DRIVE);
#endif
			
			QVector<DiskDrive> drives=var.value< QVector<DiskDrive> >();
			if (drives.size() == 0)
			{
				stream<<QMessageDisk::QueryFailed;
			}
			else
			{
				stream<<QMessageDisk::QuerySucceed;
				stream<<drives.size();
				for (int i=0; i<drives.size(); i++)
				{
					stream<<drives[i];
				}
			}

			emit finished(m_pSender, QMessageDisk::QueryDiskDrive, buffer);
		}
		break;
	case QMessageDisk::AnswerDiskDrive:
		{
			QVector<DiskDrive> vecDiskDrive;
			DiskDrive diskDrive;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageDisk::QuerySucceed)
			{
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>diskDrive;
					
					vecDiskDrive.append(diskDrive);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showDiskDrive", Q_ARG(int, nQueryFlag), Q_ARG(QVector<DiskDrive>, vecDiskDrive));
		}
		break;
	case QMessageDisk::QueryCDROMDrive:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_CDROM_DRIVE);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_CDROM_DRIVE);
#endif
			
			QVector<CDROMDrive> drives=var.value< QVector<CDROMDrive> >();
			if (drives.isEmpty())
			{
				stream<<QMessageDisk::QueryFailed;
			}
			else
			{
				stream<<QMessageDisk::QuerySucceed;
				stream<<drives.size();
				for (int i=0; i<drives.size(); i++)
				{
					stream<<drives[i];
				}
			}

			emit finished(m_pSender, QMessageDisk::QueryCDROMDrive, buffer);
		}
		break;
	case QMessageDisk::AnswerCDROMDrive:
		{
			QVector<CDROMDrive> vecDrive;
			CDROMDrive drive;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageDisk::QuerySucceed)
			{
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>drive;

					vecDrive.append(drive);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showCDROMDrive", Q_ARG(int, nQueryFlag), Q_ARG(QVector<CDROMDrive>, vecDrive));
		}
		break;
	case QMessageDisk::QueryLogicalDisk:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_LOGICAL_DISK);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_LOGICAL_DISK);
#endif
			
			QVector<LogicalDisk> vecDisk=var.value< QVector<LogicalDisk> >();
			if (vecDisk.isEmpty())
			{
				stream<<QMessageDisk::QueryFailed;
			}
			else
			{
				stream<<QMessageDisk::QuerySucceed;
				stream<<vecDisk.size();
				for (int i=0; i<vecDisk.size(); i++)
				{
					const LogicalDisk& disk=vecDisk[i];
					stream<<disk.description<<disk.deviceID<<disk.driveType<<disk.driveTypeDesc
						<<disk.fileSystem<<disk.freeSpace<<disk.size;
				}
			}

			emit finished(m_pSender, QMessageDisk::QueryLogicalDisk, buffer);
		}
		break;
	case QMessageDisk::AnswerLogicalDisk:
		{
			QVector<LogicalDisk> vecDisk;
			LogicalDisk disk;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageDisk::QuerySucceed)
			{
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>disk.description>>disk.deviceID>>disk.driveType>>disk.driveTypeDesc
						>>disk.fileSystem>>disk.freeSpace>>disk.size;

					vecDisk.append(disk);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showLogicalDisk", Q_ARG(int, nQueryFlag), Q_ARG(QVector<LogicalDisk>, vecDisk));
		}
		break;
	default:
		break;
	}
}

