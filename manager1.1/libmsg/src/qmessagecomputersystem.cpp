#include "qmessagecomputersystem.h"

#include "qmibfactory.h"
#include "qconnectionmanager.h"

QMessageComputerSystem::QMessageComputerSystem(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<ComputerSystem>("ComputerSystem");
}

QMessageComputerSystem::~QMessageComputerSystem()
{

}

QByteArray QMessageComputerSystem::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

	stream<<ackType<<usrID;

	return buffer;
}

bool QMessageComputerSystem::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageComputerSystemRunnable* pRunnable=new QMessageComputerSystemRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageComputerSystem::finished(QObject* sender, int ackType, quint32 nUsrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryComputerSystem:
		in<<AnswerComputerSystem;
		break;
	default:
		break;
	}

	in<<nUsrID;
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

QMessageComputerSystemRunnable::QMessageComputerSystemRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageComputerSystemRunnable::~QMessageComputerSystemRunnable()
{

}

void QMessageComputerSystemRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageComputerSystemRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryComputerSystem:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_COMPUTER_SYSTEM);
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
				ComputerSystem system=var.value<ComputerSystem>();
				stream<<QuerySucceed<<system;
			}

			emit finished(m_pSender, QueryComputerSystem, nUsrID, buffer);
		}
		break;
	case AnswerComputerSystem:
		{
			ComputerSystem system;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>system;
			}

			QMetaObject::invokeMethod(m_pView, "recvComputerSystem", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(ComputerSystem, system));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageComputerSystem)
