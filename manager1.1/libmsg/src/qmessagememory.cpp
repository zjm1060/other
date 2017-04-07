#include "qmessagememory.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageMemory::QMessageMemory(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QVector<PhysicalMemory> >("QVector<PhysicalMemory>");
	qRegisterMetaType<SystemMemory>("SystemMemory");
}

QMessageMemory::~QMessageMemory()
{

}

QByteArray QMessageMemory::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<msgType<<usrID;

	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageMemory::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageMemoryRunnable* pRunnable=new QMessageMemoryRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageMemory::finished(QObject* sender, int msgType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	if (msgType == QueryRefSystemMemory)
	{
		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			QMetaObject::invokeMethod(sender, "addReceiver", Q_ARG(quint32, usrID), Q_ARG(int, Type));
		}
		
		return;
	}
	else if (msgType == QueryUnrefSystemMemory)
	{
		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			QMetaObject::invokeMethod(sender, "removeReceiver", Q_ARG(quint32, usrID), Q_ARG(int, Type));
		}
		
		return;
	}

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (msgType)
	{
	case QueryPhysicalMemory:
		in<<AnswerPhysicalMemory;
		break;
	case QuerySystemMemory:
		in<<AnswerSystemMemory;
		break;
	default:
		return;
	}

	in<<usrID;

	if (buffer.size() > 0)
	{
		in.writeRawData(buffer.data(), buffer.size());
	}

	QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
	if (ptr != NULL)
	{
		QMetaObject::invokeMethod(sender, "sendMessage", Q_ARG(QByteArray, buf), Q_ARG(int, Type));
	}
}

QMessageMemoryRunnable::QMessageMemoryRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageMemoryRunnable::~QMessageMemoryRunnable()
{

}

void QMessageMemoryRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageMemoryRunnable::run()
{
	int nMsgType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nMsgType>>nUsrID;

	switch (nMsgType)
	{
	case QueryPhysicalMemory:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_PHYSICAL_MEMORY);
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
				QVector<PhysicalMemory> memories=var.value< QVector<PhysicalMemory> >();
				stream<<QuerySucceed<<memories;
			}

			emit finished(m_pSender, QueryPhysicalMemory, nUsrID, buffer);
		}
		break;
	case AnswerPhysicalMemory:
		{
			QVector<PhysicalMemory> memories;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>memories;
			}

			QMetaObject::invokeMethod(m_pView, "recvPhysicalMemroy", Q_ARG(quint32, nUsrID),
				Q_ARG(int, nQueryFlag), Q_ARG(QVector<PhysicalMemory>, memories));
		}
		break;
	case QueryRefSystemMemory:
	case QueryUnrefSystemMemory:
		{
			emit finished(m_pSender, nMsgType, nUsrID);
		}
		break;
	case QuerySystemMemory:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_SYSTEM_MEMORY);
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
				SystemMemory memory=var.value<SystemMemory>();
				stream<<QuerySucceed<<memory;
			}

			emit finished(m_pSender, QuerySystemMemory, nUsrID, buffer);
		}
		break;
	case AnswerSystemMemory:
		{
			if (m_pView == NULL)
			{
				return;
			}

			SystemMemory memory;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>memory;
			}

			QMetaObject::invokeMethod(m_pView, "recvSystemMemory", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(SystemMemory, memory));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageMemory)
