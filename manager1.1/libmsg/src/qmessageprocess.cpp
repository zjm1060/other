#include "qmessageprocess.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageProcess::QMessageProcess(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QVector<SystemProcess> >("QVector<SystemProcess>");
}

QMessageProcess::~QMessageProcess()
{

}

QByteArray QMessageProcess::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	if (contents.size() > 0)
	{
		in.writeRawData(contents.data(), contents.size());
	}

	return buffer;
}

bool QMessageProcess::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageProcessRunnable* pRunnable=new QMessageProcessRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageProcess::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	if (ackType == QueryRefProcess)
	{
		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			QMetaObject::invokeMethod(sender, "addReceiver", Q_ARG(quint32, usrID), Q_ARG(int, Type));
		}
		return;
	}
	else if (ackType == QueryUnrefProcess)
	{
		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			QMetaObject::invokeMethod(sender, "removeReceiver", Q_ARG(quint32, usrID), Q_ARG(int, Type));
		}
		return;
	}

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryProcess:
		in<<AnswerProcess;
		break;
	case QueryCreateProcess:
		in<<AnswerCreateProcess;
		break;
	case QueryTerminateProcess:
		in<<AnswerTerminateProcess;
		break;
	default:
		return;
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

QMessageProcessRunnable::QMessageProcessRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageProcessRunnable::~QMessageProcessRunnable()
{

}

void QMessageProcessRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageProcessRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryRefProcess:
	case QueryUnrefProcess:
		{
			emit finished(m_pSender, nAckType, nUsrID);
		}
		break;
	case QueryProcess:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QVariant variant;
			QAbstractMibPtr pProcessInstance=QMibFactory::instance()->product(UTIL_TYPE_PROCESS);
			QAbstractMibPtr pSystemInstance=QMibFactory::instance()->product(UTIL_TYPE_COMPUTER_SYSTEM);
			if (pProcessInstance != NULL && pSystemInstance != NULL)
			{
				var=pProcessInstance->result();
				variant=pSystemInstance->result();
			}

			if (!var.isValid() || !variant.isValid())
			{
				stream<<QueryFailed;
			}
			else
			{
				QVector<SystemProcess> processes=var.value<QVector<SystemProcess> >();
				ComputerSystem system=variant.value<ComputerSystem>();
				stream<<QuerySucceed<<processes<<system.totalPhysicalMemory;
			}

			emit finished(m_pSender, QueryProcess, nUsrID, buffer);
		}
	case AnswerProcess:
		{
			if (m_pView == NULL)
			{
				return;
			}

			QVector<SystemProcess> vecProcess;
			quint64 nPhysicalMemory;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>vecProcess>>nPhysicalMemory;
			}

			QMetaObject::invokeMethod(m_pView, "recvSystemProcess", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<SystemProcess>, vecProcess), Q_ARG(quint64, nPhysicalMemory));
		}
		break;
	case QueryCreateProcess:
		{
			QStringList strArguments;
			out>>strArguments;

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_PROCESS);
			int nRet=ExecMethodInvalid;
			if (pInstance != NULL)
			{
				nRet=pInstance->commond(CommandCreateProcess, strArguments);
			}
			
			stream<<strArguments<<nRet;

			emit finished(m_pSender, QueryCreateProcess, nUsrID, buffer);
		}
		break;
	case AnswerCreateProcess:
		{
			QStringList strArguments;
			int nRet;
			out>>strArguments>>nRet;

			QMetaObject::invokeMethod(m_pView, "recvCreateProcess", Q_ARG(quint32, nUsrID), Q_ARG(QStringList, strArguments), Q_ARG(int, nRet));
		}
		break;
	case QueryTerminateProcess:
		{
			quint32 nProcessId;
			out>>nProcessId;

			QStringList arguments;
			arguments<<QString::number(nProcessId);

			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_PROCESS);
			int nRet=ExecMethodInvalid;
			if (pInstance != NULL)
			{
				nRet=pInstance->commond(CommandTerminateProcess, arguments);
			}

			stream<<nProcessId<<nRet;

			emit finished(m_pSender, QueryTerminateProcess, nUsrID, buffer);
		}
		break;
	case AnswerTerminateProcess:
		{
			quint32 nProcessId;
			int nRet;
			out>>nProcessId>>nRet;

			QMetaObject::invokeMethod(m_pView, "recvTerminateProcess", Q_ARG(quint32, nUsrID), Q_ARG(quint32, nProcessId), Q_ARG(int, nRet));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageProcess) 
