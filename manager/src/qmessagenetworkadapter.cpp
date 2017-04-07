#include "qmessagenetworkadapter.h"

#include <QThreadPool>

#ifdef Q_OS_WIN
#include "qwmifactory.h"
#else
#include "qstatfactory.h"
#endif

QMessageNetworkAdapter::QMessageNetworkAdapter(QObject *parent)
	: QObject(parent)
{

}

QMessageNetworkAdapter::~QMessageNetworkAdapter()
{

}

QByteArray QMessageNetworkAdapter::query(int msgType, quint32 usrID, const QByteArray& contents)
{
	Q_UNUSED(contents);
	Q_UNUSED(usrID);

	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	in<<msgType;

	return buffer;
}

bool QMessageNetworkAdapter::parse(QConnection* sender, QByteArray buffer, QObject* view)
{
	if (sender == NULL || buffer.size() == 0)
	{
		return false;
	}

	QMessageNetworkAdapterRunnable* pRunnable=new QMessageNetworkAdapterRunnable(sender, buffer, view);
	pRunnable->setAutoDelete(true);

	connect(pRunnable, SIGNAL(finished(QConnection*, int, QByteArray)), this, SLOT(finished(QConnection*, int, QByteArray)));

	QThreadPool::globalInstance()->start(pRunnable);

	return true;
}

void QMessageNetworkAdapter::finished(QConnection* sender, int msgType, QByteArray buffer)
{
	Q_ASSERT_NO_NULL(sender);

	QByteArray buf;
	QDataStream in(&buf, QIODevice::WriteOnly);
	in.setVersion(DATA_STREAM_VERSION);

	switch (msgType)
	{
	case QueryNetworkAdapter:
		in<<AnswerNetworkAdapter;
		in.writeRawData(buffer.data(), buffer.size());
		break;
	default:
		break;
	}

	sender->sendMessage(buf, Type);
}

QMessageNetworkAdapterRunnable::QMessageNetworkAdapterRunnable(QConnection* sender, const QByteArray& buffer, QObject* view, QObject* parent)
:QObject(parent)
{
	setArg(sender, buffer, view);
}

QMessageNetworkAdapterRunnable::~QMessageNetworkAdapterRunnable()
{

}

void QMessageNetworkAdapterRunnable::setArg(QConnection* sender, const QByteArray& buffer, QObject* view)
{
	m_pSender=sender;
	m_buffer=buffer;
	m_pView=view;
}

void QMessageNetworkAdapterRunnable::run()
{
	int nMsgType;

	QDataStream out(m_buffer);
	out.setVersion(DATA_STREAM_VERSION);

	out>>nMsgType;

	switch (nMsgType)
	{
	case QMessageNetworkAdapter::QueryNetworkAdapter:
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);
			stream.setVersion(DATA_STREAM_VERSION);

			QVariant var;
#ifdef Q_OS_WIN
			var=QWmiFactory::instance()->exec(WMI_TYPE_NETWORK_ADAPTER);
#else
			var=QStatFactory::instance()->exec(WMI_TYPE_NETWORK_ADAPTER);
#endif
			QVector<NetworkAdapter> adapters=var.value< QVector<NetworkAdapter> >();
			if (adapters.isEmpty())
			{
				stream<<QMessageNetworkAdapter::QueryFailed;
			}
			else
			{
				stream<<QMessageNetworkAdapter::QuerySucceed;
				stream<<adapters.size();
				for (int i=0; i<adapters.size(); i++)
				{
					const NetworkAdapter& adapter=adapters[i];
					stream<<adapter.description;
					
					stream<<adapter.defaultIPGateway.size();
					for (int nIndex=0; nIndex<adapter.defaultIPGateway.size(); nIndex++)
					{
						stream<<adapter.defaultIPGateway[nIndex];
					}

					stream<<adapter.DNSServer.size();
					for (int nIndex=0; nIndex<adapter.DNSServer.size(); nIndex++)
					{
						stream<<adapter.DNSServer[nIndex];
					}

					stream<<adapter.gatewayCostMetric.size();
					for (int nIndex=0; nIndex<adapter.gatewayCostMetric.size(); nIndex++)
					{
						stream<<adapter.gatewayCostMetric[nIndex];
					}

					stream<<adapter.IPAddress.size();
					for (int nIndex=0; nIndex<adapter.IPAddress.size(); nIndex++)
					{
						stream<<adapter.IPAddress[nIndex];
					}

					stream<<adapter.IPSubnet.size();
					for (int nIndex=0; nIndex<adapter.IPSubnet.size(); nIndex++)
					{
						stream<<adapter.IPSubnet[nIndex];
					}

					stream<<adapter.MACAddress;
				}
			}

			emit finished(m_pSender, QMessageNetworkAdapter::QueryNetworkAdapter, buffer);
		}
		break;
	case QMessageNetworkAdapter::AnswerNetworkAdapter:
		{
			QVector<NetworkAdapter> vecAdapter;

			int nQueryFlag;
			out>>nQueryFlag;

			if (nQueryFlag == QMessageNetworkAdapter::QuerySucceed)
			{
				int nNum;
				out>>nNum;

				int nSize;
				quint16 nMetric;
				QString str;

				for (int i=0; i<nNum; i++)
				{
					NetworkAdapter adapter;

					out>>adapter.description;

					out>>nSize;
					for (int nIndex=0; nIndex<nSize; nIndex++)
					{
						out>>str;
						adapter.defaultIPGateway.append(str);
					}

					out>>nSize;
					for (int nIndex=0; nIndex<nSize; nIndex++)
					{
						out>>str;
						adapter.DNSServer.append(str);
					}

					out>>nSize;
					for (int nIndex=0; nIndex<nSize; nIndex++)
					{
						out>>nMetric;
						adapter.gatewayCostMetric.append(nMetric);
					}

					out>>nSize;
					for (int nIndex=0; nIndex<nSize; nIndex++)
					{
						out>>str;
						adapter.IPAddress.append(str);
					}

					out>>nSize;
					for (int nIndex=0; nIndex<nSize; nIndex++)
					{
						out>>str;
						adapter.IPSubnet.append(str);
					}

					out>>adapter.MACAddress;

					vecAdapter.append(adapter);
				}
			}

			QMetaObject::invokeMethod(m_pView, "showNetworkAdapter", Q_ARG(int, nQueryFlag),
				Q_ARG(QVector<NetworkAdapter>, vecAdapter));
		}
		break;
	default:
		break;
	}
}

