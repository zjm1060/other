#include "qmessageremoteserver.h"

#include <QHash>

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageRemoteServer::QMessageRemoteServer(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QList<ServerInfo> >("QList<ServerInfo>");
}

QMessageRemoteServer::~QMessageRemoteServer()
{

}

QByteArray QMessageRemoteServer::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	return buffer;
}

bool QMessageRemoteServer::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageRemoteServerRunnable* pRunnable=new QMessageRemoteServerRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageRemoteServer::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryRemoteServerList:
		in<<AnswerRemoteServerList;
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

QMessageRemoteServerRunnable::QMessageRemoteServerRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageRemoteServerRunnable::~QMessageRemoteServerRunnable()
{

}

void QMessageRemoteServerRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageRemoteServerRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryRemoteServerList:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_REMOTE_SERVER);
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
				QList<ServerInfo> servers=var.value<QList<ServerInfo> >();
				stream<<QuerySucceed<<servers;
			}

			emit finished(m_pSender, QueryRemoteServerList, nUsrID, buffer);
		}
		break;
	case AnswerRemoteServerList:
		{
			QList<ServerInfo> servers;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>servers;
			}

			QMetaObject::invokeMethod(m_pView, "recvRemoteServerList", Q_ARG(quint32, nUsrID),
				Q_ARG(int, nQueryFlag), Q_ARG(QList<ServerInfo>, servers));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageRemoteServer) 
