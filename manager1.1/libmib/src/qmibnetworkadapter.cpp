#include "qmibnetworkadapter.h"

QMibNetworkAdapter::QMibNetworkAdapter()
:QAbstractMib(2000)
{

}

QMibNetworkAdapter::~QMibNetworkAdapter()
{

}

void QMibNetworkAdapter::setResult(const QVariant& var)
{
	if (var.isValid())
	{
		QVector<NetworkAdapter> adapters=var.value<QVector<NetworkAdapter> >();
		if (m_vecAdapter != adapters)
		{
			m_vecAdapter=adapters;

			emit changed();
		}
	}
}

void QMibNetworkAdapter::setResult(const QByteArray& buffer)
{
	Q_READ_ONLY_DATA_STREAM(out, buffer);

	QVector<NetworkAdapter> adapters;
	out>>adapters;

	if (m_vecAdapter != adapters)
	{
		m_vecAdapter=adapters;

		emit changed();
	}
}

REGISTER_MIB_INSTANCE(QMibNetworkAdapter)
