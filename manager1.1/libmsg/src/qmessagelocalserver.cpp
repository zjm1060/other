#include "qmessagelocalserver.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageLocalServer::QMessageLocalServer(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<ServerInfo>("ServerInfo");
}

QMessageLocalServer::~QMessageLocalServer()
{

}

QByteArray QMessageLocalServer::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	return buffer;
}

bool QMessageLocalServer::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageLocalServerRunnable* pRunnable=new QMessageLocalServerRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageLocalServer::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryLocalServer:
		in<<AnswerLocalServer;
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

QMessageLocalServerRunnable::QMessageLocalServerRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageLocalServerRunnable::~QMessageLocalServerRunnable()
{

}

void QMessageLocalServerRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageLocalServerRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryLocalServer:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer);

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_LOCAL_SERVER);
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
				ServerInfo info=var.value<ServerInfo>();
				stream<<QuerySucceed<<info;
			}

			emit finished(m_pSender, QueryLocalServer, nUsrID, buffer);
		}
		break;
	case AnswerLocalServer:
		{
			ServerInfo info;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>info;
			}

			QMetaObject::invokeMethod(m_pView, "recvLocalServer", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(ServerInfo, info));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageLocalServer) 
