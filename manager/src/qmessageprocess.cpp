#include "qmessageprocess.h"

#include <QThreadPool>

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#include "qwmiprocess.h"
#else
#include "qstatfactory.h"
#include "qstatprocess.h"
#endif

#include "qprocesssender.h"

QMessageProcess::QMessageProcess(QObject *parent)
	: QObject(parent)
{

}

QMessageProcess::~QMessageProcess()
{

}

QByteArray QMessageProcess::query(int msgType, quint32 usrID, const QByteArray& contents)
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

bool QMessageProcess::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageProcessRunnable* pRunnable=new QMessageProcessRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, QByteArray)), this, SLOT(finished(QConnection*, int, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageProcess::finished(QConnection* sender, int msgType, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	if (msgType == QueryProcess)
	{
		QProcessSender::instance()->addClient(sender);
		return;
	}
	else if (msgType == QueryUnrefProcess)
	{
		QProcessSender::instance()->removeClient(sender);
		return;
	}

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QueryCreateProcess:
		in<<AnswerCreateProcess;
		break;
	case QueryTerminateProcess:
		in<<AnswerTerminateProcess;
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

QMessageProcessRunnable::QMessageProcessRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageProcessRunnable::~QMessageProcessRunnable()
{

}

void QMessageProcessRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageProcessRunnable::run()
{
	int nMsgType;

	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	switch (nMsgType)
	{
	case QMessageProcess::QueryProcess:
		{
			emit finished(m_pSender, QMessageProcess::QueryProcess);
		}
		break;
	case QMessageProcess::QueryUnrefProcess:
		{
			emit finished(m_pSender, QMessageProcess::QueryUnrefProcess);
		}
		break;
	case QMessageProcess::AnswerProcess:
		{
			if (m_pView == NULL)
			{
				return;
			}

			QVector<SystemProcess> vecProcess;
			SystemProcess process;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageProcess::QuerySucceed)
			{
				int nNum;
				out>>nNum;
				for (int i=0; i<nNum; i++)
				{
					out>>process.usrName>>process.executablePath>>process.name>>process.processId
						>>process.sessionId>>process.workingSetSize>>process.percentProcessorTime;

					vecProcess.append(process);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showSystemProcess", Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<SystemProcess>, vecProcess));
		}
		break;
	case QMessageProcess::QueryCreateProcess:
		{
			QString strCommand;
			out>>strCommand;

			QStringList arguments;
			arguments<<strCommand;

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

#ifdef Q_OS_WIN
			int nRet=QWmiFactory::instance()->execMethod(WMI_TYPE_PROCESS, QWmiProcess::MethodCreate, arguments);
#else
			int nRet=QStatFactory::instance()->execMethod(WMI_TYPE_PROCESS, QStatProcess::MethodCreate, arguments);
#endif
			
			stream<<strCommand<<nRet;

			emit finished(m_pSender, QMessageProcess::QueryCreateProcess, buffer);
		}
		break;
	case QMessageProcess::AnswerCreateProcess:
		{
			QString strCommand;
			int nRet;
			out>>strCommand>>nRet;

			QMetaObject::invokeMethod(m_pView, "showCreateProcess", Q_ARG(QString, strCommand), Q_ARG(int, nRet));
		}
		break;
	case QMessageProcess::QueryTerminateProcess:
		{
			quint32 nProcessId;
			out>>nProcessId;

			QStringList arguments;
			arguments<<QString::number(nProcessId);

			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

#ifdef Q_OS_WIN
			int nRet=QWmiFactory::instance()->execMethod(WMI_TYPE_PROCESS, QWmiProcess::MethodTerminate, arguments);
#else
			int nRet=QStatFactory::instance()->execMethod(WMI_TYPE_PROCESS, QStatProcess::MethodTerminate, arguments);
#endif

			stream<<nProcessId<<nRet;

			emit finished(m_pSender, QMessageProcess::QueryTerminateProcess, buffer);
		}
		break;
	case QMessageProcess::AnswerTerminateProcess:
		{
			quint32 nProcessId;
			int nRet;
			out>>nProcessId>>nRet;

			QMetaObject::invokeMethod(m_pView, "showTerminateProcess", Q_ARG(quint32, nProcessId), Q_ARG(int, nRet));
		}
		break;
	default:
		break;
	}
}

