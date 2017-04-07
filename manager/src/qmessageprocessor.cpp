#include "qmessageprocessor.h"

#include <QThreadPool>

#include "define.h"
#include "qviewcontroller.h"
#include "qprocessorsender.h"

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

QMessageProcessor::QMessageProcessor(QObject* parent)
:QObject(parent)
{
}

QMessageProcessor::~QMessageProcessor()
{

}

QByteArray QMessageProcessor::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);
	Q_UNUSED(usrID);

	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<msgType;

	return buffer;
}

bool QMessageProcessor::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageProcessorRunnable* pRunnable=new QMessageProcessorRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, QByteArray)), this, SLOT(finished(QConnection*, int, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);
	
	return true;
}

void QMessageProcessor::finished(QConnection* sender, int msgType, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	if (msgType == QueryRuntimeProcessor)
	{
		QProcessorSender::instance()->addClient(sender);
	}
	else if (msgType == QueryUnrefRuntimeProcessor)
	{
		QProcessorSender::instance()->removeClient(sender);
	}
	else
	{
		QByteArray buf;
		QDataStream in(&buf, QIODevice::WriteOnly);
		in.setVersion(DATA_STREAM_VERSION);

		switch (msgType)
		{
		case QueryProcessor:
			in<<AnswerProcessor;
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
}


QMessageProcessorRunnable::QMessageProcessorRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageProcessorRunnable::~QMessageProcessorRunnable()
{

}

void QMessageProcessorRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageProcessorRunnable::run()
{
	int nMsgType;

	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	switch (nMsgType)
	{
	case QMessageProcessor::QueryProcessor:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_PROCESSOR);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_PROCESSOR);
#endif
			
			QVector<ProcessorInfo> processorInfos=var.value< QVector<ProcessorInfo> >();
			if (processorInfos.isEmpty())
			{
				stream<<QMessageProcessor::QueryFailed;
			}
			else
			{
				stream<<QMessageProcessor::QuerySucceed;
				stream<<processorInfos.size();
				for (int i=0; i<processorInfos.size(); i++)
				{
					const ProcessorInfo& processor=processorInfos[i];
					stream<<processor;
				}
			}

			emit finished(m_pSender, nMsgType, buffer);
		}
		break;
	case QMessageProcessor::AnswerProcessor:
		{
			if (m_pView == NULL)
			{
				return;
			}

			QVector<ProcessorInfo> vecProcessor;
			ProcessorInfo processor;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageProcessor::QuerySucceed)
			{
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>processor;

					vecProcessor.append(processor);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showProcessor", Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<ProcessorInfo>, vecProcessor));
		}
		break;
	case QMessageProcessor::QueryRuntimeProcessor:
	case QMessageProcessor::QueryUnrefRuntimeProcessor:
		{
			emit finished(m_pSender, nMsgType);
		}
		break;
	case QMessageProcessor::AnswerRuntimeProcessor:
		{
			if (m_pView == NULL)
			{
				return;
			}

			PerfProcessor processor;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageProcessor::QuerySucceed)
			{
				out>>processor.currentClockSpeed>>processor.currentVoltage>>processor.status;

				int nNum;
				out>>nNum;
				PerfProcessorCore core;
				for (int i=0; i<nNum; i++)
				{
					out>>core.name>>core.percentProcessorTime;
					processor.core.append(core);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showPerfProcessor", Q_ARG(int, nQueryFlag),
				Q_ARG(PerfProcessor, processor));
		}
		break;
	default:
		break;
	}
}
