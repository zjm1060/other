#include "qmessagenetworkadapter.h"

#include "qmibfactory.h"
#include "qconnectionmanager.h"

QMessageNetworkAdapter::QMessageNetworkAdapter(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<QVector<NetworkAdapter> >("QVector<NetworkAdapter>");
}

QMessageNetworkAdapter::~QMessageNetworkAdapter()
{

}

QByteArray QMessageNetworkAdapter::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	return buffer;
}

bool QMessageNetworkAdapter::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageNetworkAdapterRunnable* pRunnable=new QMessageNetworkAdapterRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageNetworkAdapter::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryNetworkAdapter:
		in<<AnswerNetworkAdapter;
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

QMessageNetworkAdapterRunnable::QMessageNetworkAdapterRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageNetworkAdapterRunnable::~QMessageNetworkAdapterRunnable()
{

}

void QMessageNetworkAdapterRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageNetworkAdapterRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryNetworkAdapter:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_NETWORK_ADAPTER);
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
				QVector<NetworkAdapter> adapters=var.value< QVector<NetworkAdapter> >();
				stream<<QuerySucceed<<adapters;
			}

			emit finished(m_pSender, QueryNetworkAdapter, nUsrID, buffer);
		}
		break;
	case AnswerNetworkAdapter:
		{
			QVector<NetworkAdapter> vecAdapter;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>vecAdapter;
			}

			QMetaObject::invokeMethod(m_pView, "recvNetworkAdapter", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<NetworkAdapter>, vecAdapter));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageNetworkAdapter) 
