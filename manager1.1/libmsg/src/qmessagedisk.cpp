#include "qmessagedisk.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageDisk::QMessageDisk(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QVector<DiskDrive> >("QVector<DiskDrive>");
	qRegisterMetaType<QVector<CDROMDrive> >("QVector<CDROMDrive>");
	qRegisterMetaType<QVector<LogicalDisk> >("QVector<LogicalDisk>");
}

QMessageDisk::~QMessageDisk()
{

}

QByteArray QMessageDisk::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<msgType<<usrID;

	return buffer;
}

bool QMessageDisk::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageDiskRunnable* pRunnable=new QMessageDiskRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageDisk::finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (msgType)
	{
	case QueryDiskDrive:
		in<<AnswerDiskDrive;
		break;
	case QueryCDROMDrive:
		in<<AnswerCDROMDrive;
		break;
	case QueryLogicalDisk:
		in<<AnswerLogicalDisk;
		break;
	default:
		break;
	}

	in<<usrID;

	if (!buffer.isEmpty())
	{
		in.writeRawData(buffer.data(), buffer.size());
	}

	QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
	if (ptr != NULL)
	{
		QMetaObject::invokeMethod(sender, "sendMessage", Q_ARG(QByteArray, buf), Q_ARG(int, Type));
	}
}

QMessageDiskRunnable::QMessageDiskRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageDiskRunnable::~QMessageDiskRunnable()
{

}

void QMessageDiskRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageDiskRunnable::run()
{
	int nMsgType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case QueryDiskDrive:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_DISK_DRIVE);
			if (pInstance != NULL)
			{
				var=pInstance->result();
			}
			
			if (!var.isValid())
			{
				stream<<QueryFailed;
			}
			else
			{
				QVector<DiskDrive> drives=var.value< QVector<DiskDrive> >();

				stream<<QuerySucceed<<drives;
			}

			emit finished(m_pSender, QueryDiskDrive, nUsrID, buffer);
		}
		break;
	case AnswerDiskDrive:
		{
			QVector<DiskDrive> drives;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>drives;
			}

			QMetaObject::invokeMethod(m_pView, "recvDiskDrive", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(QVector<DiskDrive>, drives));
		}
		break;
	case QueryCDROMDrive:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_CDROM_DRIVE);
			if (pInstance != NULL)
			{
				var=pInstance->result();
			}
			
			if (!var.isValid())
			{
				stream<<QueryFailed;
			}
			else
			{
				QVector<CDROMDrive> drives=var.value< QVector<CDROMDrive> >();

				stream<<QuerySucceed<<drives;
			}

			emit finished(m_pSender, QueryCDROMDrive, nUsrID, buffer);
		}
		break;
	case AnswerCDROMDrive:
		{
			QVector<CDROMDrive> drives;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>drives;
			}

			QMetaObject::invokeMethod(m_pView, "recvCDROMDrive", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(QVector<CDROMDrive>, drives));
		}
		break;
	case QueryLogicalDisk:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_LOGICAL_DISK);
			if (pInstance != NULL)
			{
				var=pInstance->result();
			}

			if (!var.isValid())
			{
				stream<<QueryFailed;
			}
			else
			{
				QVector<LogicalDisk> disks=var.value< QVector<LogicalDisk> >();

				stream<<QuerySucceed<<disks;
			}

			emit finished(m_pSender, QueryLogicalDisk, nUsrID, buffer);
		}
		break;
	case AnswerLogicalDisk:
		{
			QVector<LogicalDisk> disks;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>disks;
			}

			QMetaObject::invokeMethod(m_pView, "recvLogicalDisk", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(QVector<LogicalDisk>, disks));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageDisk) 
