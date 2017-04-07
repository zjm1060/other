#include "qmessagesystem.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageSystem::QMessageSystem(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<OperatingSystem>("OperatingSystem");
}

QMessageSystem::~QMessageSystem()
{

}

QByteArray QMessageSystem::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	return buffer;
}

bool QMessageSystem::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageSystemRunnable* pRunnable=new QMessageSystemRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageSystem::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryOperatingSystem:
		in<<AnswerOperatingSystem;
		break;
	default:
		break;
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

QMessageSystemRunnable::QMessageSystemRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageSystemRunnable::~QMessageSystemRunnable()
{

}

void QMessageSystemRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageSystemRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryOperatingSystem:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_SYSTEM);
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
				OperatingSystem system=var.value<OperatingSystem>();

				stream<<QuerySucceed<<system;
			}

			emit finished(m_pSender, QueryOperatingSystem, nUsrID, buffer);
		}
		break;
	case AnswerOperatingSystem:
		{
			OperatingSystem system;

			int nQueryFlag;
			
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>system;
			}

			QMetaObject::invokeMethod(m_pView, "recvOperatingSystem", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(OperatingSystem, system));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageSystem) 
