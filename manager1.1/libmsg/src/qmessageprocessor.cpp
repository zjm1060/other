#include "qmessageprocessor.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageProcessor::QMessageProcessor(QObject* parent)
:QObject(parent)
{
	qRegisterMetaType<QVector<ProcessorInfo> >("QVector<ProcessorInfo>");
	qRegisterMetaType<PerfProcessor>("PerfProcessor");
}

QMessageProcessor::~QMessageProcessor()
{

}

QByteArray QMessageProcessor::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	return buffer;
}

bool QMessageProcessor::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageProcessorRunnable* pRunnable=new QMessageProcessorRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);
	
	return true;
}

void QMessageProcessor::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	if (ackType == QueryRefRuntimeProcessor)
	{
		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			QMetaObject::invokeMethod(sender, "addReceiver", Q_ARG(quint32, usrID), Q_ARG(int, Type));
		}
	}
	else if (ackType == QueryUnrefRuntimeProcessor)
	{
		QObjectPtr ptr=QConnectionManager::instance()->connection(sender);
		if (ptr != NULL)
		{
			QMetaObject::invokeMethod(sender, "removeReceiver", Q_ARG(quint32, usrID), Q_ARG(int, Type));
		}
	}
	else
	{
		Q_WRITE_ONLY_DATA_STREAM(in, buf)

		switch (ackType)
		{
		case QueryProcessor:
			in<<AnswerProcessor;
			break;
		case QueryRuntimeProcessor:
			in<<AnswerRuntimeProcessor;
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
}


QMessageProcessorRunnable::QMessageProcessorRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageProcessorRunnable::~QMessageProcessorRunnable()
{

}

void QMessageProcessorRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageProcessorRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryProcessor:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_PROCESSOR);
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
				QVector<ProcessorInfo> processors=var.value< QVector<ProcessorInfo> >();
				stream<<QuerySucceed<<processors;
			}

			emit finished(m_pSender, nAckType, nUsrID, buffer);
		}
		break;
	case AnswerProcessor:
		{
			if (m_pView == NULL)
			{
				return;
			}

			QVector<ProcessorInfo> processors;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>processors;
			}

			QMetaObject::invokeMethod(m_pView, "recvProcessor", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<ProcessorInfo>, processors));
		}
		break;
	case QueryRefRuntimeProcessor:
	case QueryUnrefRuntimeProcessor:
		{
			emit finished(m_pSender, nAckType, nUsrID);
		}
		break;
	case QueryRuntimeProcessor:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_PERF_PROCESSOR);
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
				PerfProcessor perf=var.value<PerfProcessor>();
				stream<<QuerySucceed<<perf;
			}

			emit finished(m_pSender, nAckType, nUsrID, buffer);
		}
		break;
	case AnswerRuntimeProcessor:
		{
			if (m_pView == NULL)
			{
				return;
			}

			PerfProcessor processor;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>processor;
			}

			QMetaObject::invokeMethod(m_pView, "recvPerfProcessor", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(PerfProcessor, processor));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageProcessor)
