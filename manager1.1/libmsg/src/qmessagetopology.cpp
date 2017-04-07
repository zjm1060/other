#include "qmessagetopology.h"

#include "qmibfactory.h"
#include "qutildef.h"
#include "qconnectionmanager.h"

QMessageTopology::QMessageTopology(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<GraphTopo>("GraphTopo");
}

QMessageTopology::~QMessageTopology()
{

}

QByteArray QMessageTopology::query(int ackType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);

	Q_WRITE_ONLY_DATA_STREAM(in, buffer)

	in<<ackType<<usrID;

	return buffer;
}

bool QMessageTopology::parse(QObject* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageTopologyRunnable* pRunnable=new QMessageTopologyRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QObject*, int, quint32, QByteArray)), this, SLOT(finished(QObject*, int, quint32, QByteArray)));

	globalThreadPool()->start(pRunnable);

	return true;
}

void QMessageTopology::finished(QObject* sender, int ackType, quint32 usrID, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	Q_WRITE_ONLY_DATA_STREAM(in, buf)

	switch (ackType)
	{
	case QueryTopology:
		in<<AnswerTopology;
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

QMessageTopologyRunnable::QMessageTopologyRunnable(QObject* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageTopologyRunnable::~QMessageTopologyRunnable()
{

}

void QMessageTopologyRunnable::setArg(QObject* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageTopologyRunnable::run()
{
	int nAckType;
	quint32 nUsrID;

	Q_READ_ONLY_DATA_STREAM(out, m_buffer)

	out>>nAckType>>nUsrID;

	switch (nAckType)
	{
	case QueryTopology:
		{
			Q_WRITE_ONLY_DATA_STREAM(stream, buffer)

			QVariant var;
			QAbstractMibPtr pInstance=QMibFactory::instance()->product(UTIL_TYPE_TOPOLOGY);
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
				GraphTopo graph=var.value<GraphTopo>();
				stream<<QuerySucceed<<graph;
			}

			emit finished(m_pSender, QueryLocalServer, nUsrID, buffer);
		}
		break;
	case AnswerTopology:
		{
			GraphTopo graph;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QuerySucceed)
			{
				out>>graph;
			}

			QMetaObject::invokeMethod(m_pView, "recvTopology", Q_ARG(quint32, nUsrID), Q_ARG(int, nQueryFlag), Q_ARG(GraphTopo, graph));
		}
		break;
	default:
		break;
	}
}

REGISTER_MESSAGE_INSTANCE(QMessageTopology) 
